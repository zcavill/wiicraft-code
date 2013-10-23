#include <iostream>
#include <gccore.h>
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <wiikeyboard/keyboard.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fat.h>
#include <sdcard/wiisd_io.h>
#include <png.h>
#include <pngu.h>
#include <network.h>
#include "common.h"
#include "update.h"
#include "main.h"
#include "utils.h"

bool netoworkRAN = false;
s8 selected = 1;

//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
void Update(bool force){
	clear();
	if(netoworkRAN == false){
		printf("Initializing Network...");
		initialise_network();
		netoworkRAN = true;
		fatInitDefault();
	}
	printf("Attempting to connect to server...\n");
	s32 main_server = server_connect();
	printf("Connection established.\n\n");
	clear();
	while(true) {
		printf("\x1b[2;0H");
		CHANGE_COLOR(MAGENTA);
		printf("WiiCraft is Stored on?\n");
		CHANGE_COLOR(WHITE);
		MENU("SD Card", 1); // MENU(description, option number)
		MENU("USB/HDD", 2);
		do {pressed = DetectInput(DI_BUTTONS_DOWN);} while(!pressed);
		if (pressed & WPAD_BUTTON_DOWN) {
			selected++;
			if (selected>2) selected = 1;
			continue;
		}
		if (pressed & WPAD_BUTTON_UP) {
			selected--;
			if (selected<1) selected = 2;
			continue;
		}
		if (pressed & WPAD_BUTTON_A) {
			if(selected == 1) {
				fatDevice = FAT_DEVICE_SD;
				goto NEXT;
			}
			if(selected == 2) {
				fatDevice = FAT_DEVICE_USB;
				goto NEXT;
			}
		}
	}
	
	
	// Open file
	NEXT:FILE *f;
	if(fatDevice == FAT_DEVICE_SD){
		f=fopen("sd:/temp.txt", "wb");
	}
	else{
		f=fopen("USB:/temp.txt", "wb");
	}
	
	// If file can't be created
	if (f == NULL) {
		fclose(f);
		die("There was a problem creating/accessing the temp file.\n");
	}
	
	printf("Checking version with remote server...\n");
	
	char http_request[1000];
	strcpy(http_request,"GET /wiicraft/WiiCraft.txt");
	strcat(http_request, " HTTP/1.0\r\nHost: filfat.com\r\n\r\n");
	
	write_http_reply(main_server, http_request); // Send the HTTP message
	int result = request_file(main_server, f); // Store the servers reply in our file pointer

	fclose(f);
	net_close(main_server);
	
	if (result == true) {
		printf("\n\nSuccessfully downloaded the version file.\n");
	}
	else {
		die("\n\nDownload of remote version file failed.\n");
	}
	
	printf("Comparing remote version with current version...\n");
	
	// Reading and Compare the file
	
	ifstream WC_version;
	
	string line;
	if(fatDevice == FAT_DEVICE_SD){
		WC_version.open("sd:/temp.txt");
	}
	else if(fatDevice == FAT_DEVICE_USB){
		WC_version.open("usb:/temp.txt");
	}
	
	if (WC_version.is_open())
	{
		while ( getline (WC_version,line) ){
		
		}
		WC_version.close();
	}
	else{
		cout << "Unable to read file\n well this is embarrassing...\n";
		usleep(5000000);;
		if(fatDevice == FAT_DEVICE_SD){
			remove( "sd:/temp.txt" );
		}
		else if(fatDevice == FAT_DEVICE_USB){
			remove( "USB:/temp.txt" );
		}
		clear();
		return;
	} 
	
	printf("Current version installed: ");
	if(WIICRAFT_VERSION == line){
		CHANGE_COLOR(GREEN);
	}
	else{
		CHANGE_COLOR(RED);
	}
	printf("%s\n", WIICRAFT_VERSION);
	CHANGE_COLOR(WHITE);
	printf("Remote version: ");
	CHANGE_COLOR(GREEN);	
	printf("%s\n", line.c_str());
	CHANGE_COLOR(WHITE);
	
	if(WIICRAFT_VERSION == line && force == false){
		printf("Theres no need to update, you already have the current version.");
		usleep(5000000);
	}
	else{
		printf("Updating..");
		printf(".\n");
		downloadUpdate();
	}
	
	if(fatDevice == FAT_DEVICE_SD){
		remove( "sd:/temp.txt" );
	}
	else if(fatDevice == FAT_DEVICE_USB){
		remove( "USB:/temp.txt" );
	}
	clear();
	return;
}
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
void downloadUpdate(){
	printf("Attempting to connect to server...\n");
	s32 main_server = server_connect();
	printf("Connection established.\n\n");
	
	if(fatDevice == FAT_DEVICE_SD){
		remove( "sd:/wiicraft/wiicraft.dol" );
	}
	else if(fatDevice == FAT_DEVICE_USB){
		remove( "USB:/wiicraft/wiicraft.dol" );
	}
	
	// Open file
	FILE *f;
	if(fatDevice == FAT_DEVICE_SD){
		f=fopen("sd:/wiicraft/wiicraft.dol", "wb");
	}
	else{
		f=fopen("USB:/wiicraft/wiicraft.dol", "wb");
	}
	
	// If file can't be created
	if (f == NULL) {
		fclose(f);
		die("There was a problem creating/accessing the temp file.\n");
	}
	
	printf("Downloading Update");
	
	char http_request[1000];
	strcpy(http_request,"GET /wiicraft/wiicraft.dol");
	strcat(http_request, " HTTP/1.0\r\nHost: filfat.com\r\n\r\n");
	
	write_http_reply(main_server, http_request); // Send the HTTP message
	int result = request_file(main_server, f); // Store the servers reply in our file pointer

	fclose(f);
	net_close(main_server);
	
	if (result == true) {
		printf("\n\nSuccessfully downloaded the update.\n");
	}
	else {
		die("\n\nDownload of updated failed.\n");
	}
	printf("Updated successful (you need to reboot WiiCraft)...");
	usleep(5000000);
	clear();
	return;
}
//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
inline void clear(){
	printf("\x1b[2J");   // Clear
	printf("\x1b[2;0H"); // Reset cursor position
}