// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.0.
 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 3.0 for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// Copyright (C) 2012-2013    filfat, xerpi, JoostinOnline
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <wiikeyboard/keyboard.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fat.h>
#include <sdcard/wiisd_io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <network.h>
#include <gccore.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ogc/machine/processor.h>
#include <wiiuse/wpad.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "main.h"
#include "video.h"
#include "background_image.h"
#include "filelist.h"
#include "devicemounter.h"

#define EXECUTE_ADDR	((u8 *) 0x92000000)
#define BOOTER_ADDR		((u8 *) 0x93000000)
#define ARGS_ADDR		((u8 *) 0x93200000)

extern "C" {
	extern void __exception_setreload(int t);
}

u32 pressed = WPAD_ButtonsDown(0);
static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

int fatDevice = FAT_DEVICE_NONE;
bool wc_updating = false;

typedef void (*entrypoint) (void);
extern void __exception_setreload(int t);
extern void __exception_closeall();
extern const u8 app_booter_bin[];
extern const u32 app_booter_bin_size;

    //--------------------------------------------------
	//Wiilight
	//--------------------------------------------------
	lwp_t light_thread = 0;
	void *light_loop (void *arg);
	//vu32 *light_reg = (u32*) HW_GPIO;
	bool light_on = false;
	u8 light_level = 0;
	struct timespec light_timeon = { 0 };
	struct timespec light_timeoff = { 0 };
	//----------------------------------------------------------
	//Function's:
	//----------------------------------------------------------
void WIILIGHT_TurnOn(){
	*(u32*)0xCD0000C0 |= 0x20;
}

void WIILIGHT_TurnOff(){
	*(u32*)0xCD0000C0 &= ~0x20;
}

void WIILIGHT_SetLevel(int level){
	light_level = MIN(MAX(level, 0), 100);
	// Calculate the new on/off times for this light intensity
	u32 level_on;
	u32 level_off;
	level_on = (light_level * 2.55) * 40000;
	level_off = 10200000 - level_on;
	light_timeon.tv_nsec = level_on;
	light_timeoff.tv_nsec = level_off;
}
static FILE *open_file(const char *dev, char *filepath)
{
	sprintf(filepath, "%s:/apps/wiicraft/boot.dol", dev);
	FILE *exeFile = fopen(filepath ,"rb");

	if (exeFile == NULL)
	{
		sprintf(filepath, "%s:/apps/WiiCraft/boot.dol", dev);
		exeFile = fopen(filepath ,"rb");
	}
	if (exeFile == NULL)
	{
		sprintf(filepath, "%s:/apps/wiicraft/boot.elf", dev);
		exeFile = fopen(filepath ,"rb");
	}
	if (exeFile == NULL)
	{
		sprintf(filepath, "%s:/apps/WiiCraft/boot.elf", dev);
		exeFile = fopen(filepath ,"rb");
	}

	return exeFile;
}

void SystemMenu()
{
	*(vu32*)0x8132FFFB = 0x50756e65;
	DCFlushRange((void *)(0x8132FFFB), 4);
	ICInvalidateRange((void *)(0x8132FFFB), 4);
	SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);
}

void clearScreen();
void boot();
//void cleanup();
//void update_check();
//s32 create_and_request_file(char* path1, char* appname, char *filename);

int main(int argc, char **argv)
{	
	// In the event of a code dump, the app will exit after 10 seconds (unless you press POWER)
	__exception_setreload(10);
	
	
	WIILIGHT_SetLevel(255);
	WIILIGHT_TurnOn();
	
	u32 cookie;
	FILE *exeFile = NULL;
	void *exeBuffer = (void *)EXECUTE_ADDR;
	u32 exeSize = 0;
	entrypoint exeEntryPoint;
	__exception_setreload(0);
	
	fatMountSimple("sd", &__io_wiisd);
	
	/* get imagedata */
	u8 *imgdata = GetImageData();
	fadein(imgdata);

	char filepath[200];
	
	SDCard_Init();
	exeFile = open_file(DeviceName[SD], filepath);
	// if app not found on SD Card try USB
	if (exeFile == NULL)
	{
		USBDevice_Init();
		fatDevice = FAT_DEVICE_USB;
		int dev;
		for(dev = USB1; dev < MAXDEVICES; ++dev)
		{
			if(!exeFile)
				exeFile = open_file(DeviceName[dev], filepath);
		}
	}
	else{
		fatDevice = FAT_DEVICE_SD;
	}
	if (exeFile == NULL)
	{
		fadeout(imgdata);
		fclose(exeFile);
		SDCard_deInit();
		USBDevice_deInit();
		StopGX();
		free(imgdata);
		SystemMenu();
	}
	fseek(exeFile, 0, SEEK_END);
	exeSize = ftell(exeFile);
	rewind(exeFile);

	if(fread(exeBuffer, 1, exeSize, exeFile) != exeSize)
	{
		fadeout(imgdata);
		fclose(exeFile);
		SDCard_deInit();
		USBDevice_deInit();
		StopGX();
		free(imgdata);
		SystemMenu();
	}
	fclose(exeFile);

	memcpy(BOOTER_ADDR, app_booter_bin, app_booter_bin_size);
	DCFlushRange(BOOTER_ADDR, app_booter_bin_size);

	fadeout(imgdata);
	SDCard_deInit();
	USBDevice_deInit();
	StopGX();
	free(imgdata);

	exeEntryPoint = (entrypoint)BOOTER_ADDR;

	bool menuBool = true;

	VIDEO_Init();
	WPAD_Init();
	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
	
	
	
	WIILIGHT_TurnOff();
	printf("\x1b[2;0H");
	
	printf("Welcome To WiiCraft Launcher!\n");
	printf("Controlls:\n");
	printf(" [*] A: 		Check for Uppdate And Launch WiiCraft\n");
	printf(" [*] Minus: 	Launch WiiCraft Without Checking For Uppdate\n");
	printf(" [*] Home:		Exit\n");
	printf("\n");
	
	while(menuBool){
	
		WPAD_ScanPads();
		pressed = WPAD_ButtonsDown(0);
		
		if (pressed & WPAD_BUTTON_A){
			//wc_updating = true;
			//menuBool = false;
			SYS_ResetSystem(SYS_SHUTDOWN, 0, 0);
			_CPU_ISR_Disable(cookie);
			__exception_closeall();
			exeEntryPoint();
			_CPU_ISR_Restore(cookie);
		}
		else if (pressed & WPAD_BUTTON_MINUS){
			//wc_updating = false;
			//menuBool = false;
			SYS_ResetSystem(SYS_SHUTDOWN, 0, 0);
			_CPU_ISR_Disable(cookie);
			__exception_closeall();
			exeEntryPoint();
			_CPU_ISR_Restore(cookie);
		}
		else if (pressed & WPAD_BUTTON_HOME){
			printf("Exiting...\n");
			exit(0);
		}
		VIDEO_WaitVSync();
	}
	//if(wcupdating == true){ update_check();}
	clearScreen();
	printf("Exiting...\n");
	exit(0);
	//cleanup();
}

void clearScreen(){
	VIDEO_ClearFrameBuffer (rmode, xfb, COLOR_BLACK);
	printf("\x1b[2;0H");
}
void boot(){
}

//void cleanup(){
//	WPAD_Shutdown();
//	exit(0);
//}

/*void update_check() {
	
	printf("\n\nChecking for WiiCraft Updates...\n");
	
	// Open the file 
	FILE *f;
		f = fopen("sd:/wiicraft/boot.dol", "rb");
	
	// Problems opening the file?
	if (f == NULL) {
		printf("Could not find WiiCraft \n");
		
		printf("Would you like to install WiiCraft?");
        printf("[*]A: install\n");
		printf("[*]B: skip this installation\n");
		
		bool running_update = true;
		while (running_update == true) {
			WPAD_ScanPads();
			pressed = WPAD_ButtonsDown(0);
			
			// Grab the latest HBB
			if (pressed & WPAD_BUTTON_A) {
				wc_updating = true;
				s32 wc_update = 0;
				printf("Retrieving the latest version of the WiiCraft...\n");
				
				wc_update = create_and_request_file("sd:/", "wiicraft", "/wiicraft.dol"); 
				
				if (wc_update == -1) {
					printf("\nCould not install WiiCraft.\n\n");
					running_update = false;
				}
				else {
					printf("\nWiiCraft has been installed.\n\n");
					wc_updating = false;
					running_update = false;
				}
				
			}
			else if (pressed & WPAD_BUTTON_B) {
				printf("Skipping the installation of WiiCraft.\n\n");
				running_update = false;
			}
		}
	}
	else {
		// Open file
		fseek (f , 0, SEEK_END);
		long local_wc_size = ftell (f);
		rewind (f);
		fclose(f);
		
		// Different size?
		if (local_wc_size != remote_wc_size) {
			
			printf("\n");
			
			// Update text
			char *split_update_text = strtok (update_text, " ");
			
			while (split_update_text != NULL) {
				
				if (strcmp(split_update_text,"|") == 0) {
					printf("\n");
				}
				else {
					printf("%s ",split_update_text);
				}
				split_update_text = strtok (NULL, " ");
			}
			
			printf("\nNew update available! Press A to update or B to skip this update.\n");
			
			
			bool running_update = true;
			while (running_update == true) {
				WPAD_ScanPads();
				pressed = WPAD_ButtonsDown(0);
				
				// Grab the latest HBB
				if (pressed & WPAD_BUTTON_A) {
					wc_updating = true;
					
					printf("Retrieving the latest version of WiiCraft...\n");
					
					// Delete boot.bak if it exists
						remove_file("sd:/wiicraft/wiicraft.bak");
						// Rename the old HBB boot file to boot.bak
						if (rename("sd:/wiicraft/wiicraft.dol", "sd:/wiicraft/wiicraft.back") == 0) {
							
							// Download the new dol file
							create_and_request_file("sd:/", "wiicraft", "/wiicraft.dol");
							
							// Check to see if size matches, if so remove boot.bak, if not report error
							f = fopen("sd:/wiicraft/wiicraft.dol", "rb");
							
							if (f == NULL) {
								printf("\n\nCould not open the updated WiiCrafts wiicraft.dol file.\n");
								remove_file("sd:/wiicraft/wiicraft.dol");
								rename("sd:/wiicraft/wiicraft.bak", "sd:/wiicraft/wiicraft.dol");
								printf("The previous version of WiiCraft has been restored.\n\n");
								sleep(5);
								running_update = false;
							}
							else {
								// Open file
								fseek (f , 0, SEEK_END);
								long local_wc_size = ftell (f);
								rewind (f);
								fclose(f);
								
								if (local_wc_size == remote_wc_size) {
									
									create_and_request_file("sd:/apps/", "wiicraft", "/icon.png");
									
									printf("\nLatest version of the WiiCraft has been installed. Now returning you to the HBC.\n");
									die(0);
								}
								else {
									printf("\n\nThe updated WiiCraft boot.dol file is a different size(%li) than expected (%li).\n", local_wc_size, remote_wc_size);
									remove_file("sd:/wiicraft/wiicraft.dol");
									rename("sd:/wiicraft/wiicraft.bak", "sd:/wiicraft/wiicraft.dol");
									printf("The previous version of WiiCraft has been restored.\n\n");
									sleep(5);
									running_update = false;
								}
							}
						}
						else {
							printf("Couldn't rename wiicraft.dol to wiicraft.bak, can't update to latest version.\n\n"); 
							running_update = false;
						}
					
				}
				else if (pressed & WPAD_BUTTON_B) {
					printf("Skipping update of WiiCraft.\n\n");
					running_update = false;
				}
			}
		}
		else {
			printf("No updates available.\n");
		}
	}
}

s32 create_and_request_file(char* path1, char* appname, char *filename) {
	
	// Path
	char path[300];
	strcpy(path, path1);
	strcat(path, appname);
	
	// Create the folder if it's a request on a directory
	if (strcmp(filename, ".png") != 0) {
		if (!opendir(path)) {
			mkdir(path, 0777);
			if (!opendir(path)) {
				printf("Could not create %s directory.\n", path);
				return -1;
			}
		}
	}	
	
	strcat(path, filename);
	
	
	s32 result = 0;
	s32 main_server;
	int retry_times = 0;
	
	FILE *f;
	
	// Try to get the file, if failed for a 3rd time, then return -1
	while (result != 1) {
		
		// Sleep for a little bit so we don't do all 3 requests at once 
		if (retry_times > 1) {
			sleep(2);
		}
		else if (retry_times > 3) {
			sleep(3);
		}
		
		// Open file
		f = fopen(path, "wb");
		
		// If file can't be created
		if (f == NULL) {
			printf("There was a problem accessing the file %s.\n", path);
			return -1;
		}
		
		main_server = server_connect(0);
		
		//printf("Request: /homebrew/%s%s\n", filename, extension);
		
		char http_request[1000];
		if (setting_repo == 0) {
			strcpy(http_request, "GET /wiicraft/");
			if (strstr(appname,"ftpii")) {
				strcat(http_request, "ftpii");
			}
			else {
				strcat(http_request, appname);
			}
		}
		else {
			strcpy(http_request, "GET ");
			strcat(http_request, repo_list[setting_repo].apps_dir);
			strcat(http_request, appname);
		}
		
		strcat(http_request, filename);
		strcat(http_request, " HTTP/1.0\r\nHost: ");
		if (setting_repo == 0) {
				strcat(http_request, "www.filfat.com");
		}
		else {
			strcat(http_request, repo_list[setting_repo].domain);
		}
		strcat(http_request, "\r\nCache-Control: no-cache\r\n\r\n");
		//strcat(http_request, " HTTP/1.0\r\n\r\n");
		//strcat(http_request, " HTTP/1.0\r\nHost: www.codemii.com\r\nCache-Control: no-cache\r\n\r\n");
		
		//strcpy(testy, http_request);
		
		write_http_reply(main_server, http_request);
		result = request_file(main_server, f);
		
		retry_times++;
		
		fclose(f);
		net_close(main_server);
		
		// User cancelled download
		if (result == -2) {
			return -1;
		}
		
		if (retry_times >= 6) {
			return -1;
		}
	}
	
	//printf("Received %s%s.\n", appname, filename);
	
	return 1;
}*/










