/*

	(c)Mojang AB And Filiph Sandström

									*/

/*--------------------------------------------------------



Remove Thumbs.db!



---------------------------------------------------------*/

//Standard libray:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <iostream>
#include <fstream>
#include <asndlib.h>
#include <mp3player.h>

//#include <sys/stat.h>
//#include <time.h>
//#include <ogc/lwp.h>
//#include <ogc/lwp_watchdog.h>
//#include <sdcard/wiisd_io.h>
//#include <ogc/usbstorage.h>
//#include <ogcsys.h>
//#include <errno.h>

//Network:
#include <network.h>
//#include <GEMS_WifiWii.h>
//Home menu:
//Coming Soon
//GRRLIB:
//#include <grrlib.h>

//Classes:
#include "debug.h"
#include "init.h"
#include "mainAPI.h"
#include "utils.h"
#include "World.h"
#include "sicksaxis.h"
//#include "NETWORK_SERVER.H"
//#include "minecraft.h"

//Fonts:
//#include "minecraftFont_ttf.h"
//#include "gfx/BMfont5.h"

//Images:
//#include "terrain_png.h"
//#include "gfx/mojang.h"
//#include "gfx/pointer1.h"
//#include "gfx/pointer2.h"
//#include "gfx/pointer3.h"
//#include "gfx/pointer4.h"
//#include "gfx/grrlib_logo.h"
//#include "gfx/panorama4.h"

//Music:
//#include "calm1_mp3.h"
//#include "calm2_mp3.h"
#include "calm3_mp3.h"

//HomeMenu:

//RGBA Colors
#define BLACK   0x000000FF
#define MAROON  0x800000FF
#define GREEN   0x008000FF
#define OLIVE   0x808000FF
#define NAVY    0x000080FF
#define PURPLE  0x800080FF
#define TEAL    0x008080FF
#define GRAY    0x808080FF
#define SILVER  0xC0C0C0FF
#define RED     0xFF0000FF
#define LIME    0x00FF00FF
#define YELLOW  0xFFFF00FF
#define BLUE    0x0000FFFF
#define FUCHSIA 0xFF00FFFF
#define AQUA    0x00FFFFFF
#define WHITE   0xFFFFFFFF
//Wiilight
#define HW_GPIO             0xCD0000C0;
#define DISC_SLOT_LED       0x20

//Extern variables
	extern Mtx44 projection;
	extern Mtx view, model, modelview;
	extern float fps;
	extern u32 pressed;
	extern u32 expansion_type;
	extern struct joystick_t *js;
	extern WPADData *wmote_data;
//


World world;

void MoveCamera();
void UpdateCamera();
void UpdatePlayer();

//Server:
//#define CLIENT_PORT 19135
//#define SERVER_PORT 19132

extern "C" {
	extern void __exception_setreload(int t);
}

using namespace std;
//using namespace minecraft;

	const int sizex = 32;
	const int sizey = 32;
	const int sizez = 64;
	const int SIZEZ = 64;

	//Initialize the IR
	ir_t ir1;
	ir_t ir2;
	//ir_t ir3;
	//ir_t ir4;

	//--------------------------------------------------
	//Wiilight
	//--------------------------------------------------
	lwp_t light_thread = 0;
	void *light_loop (void *arg);
	vu32 *light_reg = (u32*) HW_GPIO;
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

/*int sizeX, sizeY;
int offsetX, offsetY;

typedef struct
{
	unsigned short sizeX, sizeY;
}header_struct;

header_struct *h;

unsigned char* readMap(char* filename)
{
	printf("opening %s\n",filename);
	FILE* f=fopen(filename,"rb");
	if(!f)return NULL;
	h=malloc(2048);
	fread(h,2048,1,f);
	sizeX=h->sizeX*4;
	sizeY=h->sizeY*4;
	printf("reading... %d %d\n", sizeX, sizeY);
	unsigned char* map=malloc(sizeX*sizeY*SIZEZ);
	printf("malloced\n");

	unsigned char data[1024];

	int x, y;
	for(x=0;x<sizeX/CLUSTERSIZE;x++)
	{
		for(y=0;y<sizeY/CLUSTERSIZE;y++)
		{
			fseek(f,2048+2048*(x+y*sizeX/CLUSTERSIZE),SEEK_SET);
			fread(data,1024,1,f);
			int i, j, k;
			for(i=0;i<CLUSTERSIZE;i++)
			{
				for(j=0;j<CLUSTERSIZE;j++)
				{
					for(k=0;k<SIZEZ;k++)
					{
						unsigned char d1=data[i+(j+k*CLUSTERSIZE)*CLUSTERSIZE];
						if(d1==13)d1=0;
						(map)[(x*CLUSTERSIZE+i)+((y*CLUSTERSIZE+j)+k*sizeY)*sizeX]=d1;
					}
				}
			}
		}
	}

	fclose(f);
	return map;
}*/

void checkupdate() {

	//TODO

	//Disk light turn off
	WIILIGHT_TurnOff();

}

int main(int argc, char *argv[])  {
	// In the event of a code dump, the app will exit after 10 seconds (unless you press POWER)
	__exception_setreload(10);
	
	fatInitDefault();
	
	int useSD;
	
	
	//Disk light turn on and init
	//WIILIGHT_Init();
	WIILIGHT_SetLevel(255);
	WIILIGHT_TurnOn();
	
	//if(*argv[1] == NULL){
	//	useSD = 3;
	//}
	
	//Check if it uses SD or USB:
	char test;
	//if(!(useSD ==3)){
		sscanf(argv[0], "%c", &test); //read first character from argv[0] into test
		if(test == 115){ useSD = 1; } //first character = s
		else if(test == 117){ useSD = 2; } //first character = u
		else{ useSD = 3; } //first character = NULL
	//}
	
	//Init's that's can't be made in c:
	
	//API:
	API mainAPI;
	mainAPI.initAPI(useSD);
	
	//END OF INIT'S
	
	Initialize();
	InitVideo();
	InitPad();
	initFPS();
	
	checkupdate();

	string ver = "A01";
	string line;
	bool updated = false;

	ofstream update1("sd:/version.txt", ios::out);
	if (update1.is_open())
	{
		update1 << ver;
		update1.close();
	}

	//checks for updates:
	fstream update("sd:/version.txt");
	if (update.is_open()){
		while(update.good()){
			
			getline (update,line);
			
			if((line == ver)){
				updated = true;
				break;
			}
		}
		update.close();
	}
	
	if(!updated){
		//ToDO
	}

	//-------------------------------------------------
	//Server
	//-------------------------------------------------



	//-------------------------------------------------
	//Server
	//-------------------------------------------------

	//Gets the IR positions
	WPAD_IR(WPAD_CHAN_1, &ir1);
	//WPAD_IR(WPAD_CHAN_2, &ir2);
	//WPAD_IR(WPAD_CHAN_3, &ir3);
	//WPAD_IR(WPAD_CHAN_4, &ir4);

		MP3Player_PlayBuffer(calm3_mp3, calm3_mp3_size, NULL);
	//Menu
	while(1){
	
		break;
	}
	
	while(1){
		Clean();
		UpdatePad();	
		
		//printf("USB: %s\n", argv[0]);
		
		printf("FPS: %f\n", fps);
		printf("posx: %f   posy: %f  posz: %f\n", world.player->position.x, world.player->position.y, world.player->position.z);
		printf("pitch: %f   yaw: %f\n", world.player->pitch, world.player->yaw);
		//if(expansion_type == WPAD_EXP_NUNCHUK)
			//printf("sin: %f   cos: %f", sin(DegToRad(NunchukAngle(js->ang))), cos(DegToRad(NunchukAngle(js->ang))));		
		printf("\n");
		
		
		printf("chunkX: %i  chunkY: %i  chunkZ: %i\n", world.player->chunk_x, world.player->chunk_y, world.player->chunk_z);
		printf("blockX: %i  blockY: %i  blockZ: %i\n", world.player->block_x, world.player->block_y, world.player->block_z);

		MoveCamera();
		UpdatePlayer();		
		world.update();
		
		world.updateViewMatrix();
		guMtxConcat(world.getViewMarix(), model, modelview);
		GX_LoadPosMtxImm(modelview, GX_PNMTX0);	
		
		
		world.drawWorld();
		

		SwapBuffer();
		FPS(&fps);
		if (pressed & WPAD_BUTTON_HOME ) break;
		//if (pressed & WPAD_BUTTON_1 ) {world.player->camera->translate(0.0f, 0.0f, 0.0f);}
		//if (pressed & WPAD_BUTTON_2 ) {guMtxIdentity(world.player->camera->matrix); world.player->camera->matrix[3][3] = 1.0f;}
	}
	
	WIILIGHT_TurnOn();
	
	WPAD_Shutdown();
	
	mainAPI.stopAPI();
	#ifdef USBGECKO
	Debug("stopAPI Passed");
	#endif
	
	Deinitialize();
	
	WIILIGHT_TurnOff();
	
	exit(0);
}

void UpdatePlayer()
{
	
	//DrawCubeWire(world.player->chunk_x, world.player->chunk_y, world.player->chunk_z,
				// world.player->block_x, world.player->block_y, world.player->block_z);
	
	//Block *bp = world.playerBlock();
	//Chunk *cp = world.playerChunk();
	
	/*if(pressed & WPAD_BUTTON_A)
	{
		if(cp != NULL)
		{
			if(bp != NULL)
			{
				bp->transparent = true;
				world.chunkHandler->generateMesh(world.player->chunk_x, world.player->chunk_y, world.player->chunk_z);
			}
		}	
	}

	if(pressed & WPAD_BUTTON_B)
	{
		if(cp != NULL)
		{
			if(bp != NULL)
			{
				bp->transparent = false;
				world.chunkHandler->generateMesh(world.player->chunk_x, world.player->chunk_y, world.player->chunk_z);
			}
		}
	}*/
	
}

void MoveCamera()
{
	if(expansion_type == WPAD_EXP_NUNCHUK)
	{
		if(js->mag >= 0.2f)
		{
			float n_angle = DegToRad(NunchukAngle(js->ang));
			
			world.player->move( cos(n_angle)*js->mag / 2.0f, world.player->camera->getRightVector());
			world.player->move(-sin(n_angle)*js->mag / 2.0f,  world.player->camera->getForwardVector());
			
		}
	}
	
	if(pressed & WPAD_NUNCHUK_BUTTON_C)
		world.player->position.y += 0.5f;
	if(pressed & WPAD_NUNCHUK_BUTTON_Z)
		world.player->position.y -= 0.5f;
		
	if(pressed & WPAD_BUTTON_1)
		world.player->position.y += 0.5f;
	if(pressed & WPAD_BUTTON_2)
		world.player->position.y -= 0.5f;
		
	if(pressed & WPAD_BUTTON_UP)
		world.player->pitch += 0.025f;
	if(pressed & WPAD_BUTTON_DOWN)
		world.player->pitch -= 0.025f;
		
	if(pressed & WPAD_BUTTON_RIGHT)
		world.player->yaw -= 0.025f;
	if(pressed & WPAD_BUTTON_LEFT)
		world.player->yaw += 0.025f;
		
		
	if(world.sickSaxis.connected)
	{
		int rx = world.sickSaxis.gamepad.rightAnalog.x - 128;
		int ry = world.sickSaxis.gamepad.rightAnalog.y - 128;
		int lx = world.sickSaxis.gamepad.leftAnalog.x - 128;
		int ly = world.sickSaxis.gamepad.leftAnalog.y - 128;
		
		if(fabs(ry) > 20)
		{
			world.player->pitch -= ry /2000.0f;
		}
		if(fabs(rx) > 20)
		{
			world.player->yaw -= rx /2000.0f;
		}	
			
		if(fabs(ly) > 20)
		{
			world.player->move( ly / 400.0f, world.player->camera->getForwardVector());
		}
		
		if(fabs(lx) > 20)
		{
			world.player->move( lx / 400.0f, world.player->camera->getRightVector());
		}	
		
		
		
		if(world.sickSaxis.gamepad.buttons.L2)
			world.player->position.y += 0.25f;
		if(world.sickSaxis.gamepad.buttons.R2)
			world.player->position.y -= 0.25f;
	}
}

void UpdateCamera()
{
	world.updateViewMatrix();
	guMtxConcat(world.getViewMarix(), model, modelview);

	GX_LoadPosMtxImm(modelview, GX_PNMTX0);
}
