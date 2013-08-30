// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 3.0 for more details.

// Copyright (C) 2012-2013	filfat, xerpi, JoostinOnline

#include <gccore.h>
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
//#include <GEMS_WifiWii.h>

#include "grass_png.h"
#include "block_png.h"

//Classes:
#include "debug.h"
#include "mainAPI.h"
#include "init.h"
#include "World.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "World.hpp"
#include "Image.hpp"
#include "map.h"
#include "debug.h"
#include "main.h"
#include "utils.h"

extern "C" {
	extern void __exception_setreload(int t);
}


//const unsigned int LOCAL_PLAYERS = 4; //i dont think the wii can handle 4 players but who knows ;)
//const unsigned int ONLINE_PLAYERS = 8; //due ram; maybe can increes that later.
//const unsigned int PORT = 8593; //PORT

int fatDevice = FAT_DEVICE_NONE;

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

World world;
void UpdateCamera();
void MoveCamera();
int Menu(int texture);

int main(int argc, char **argv)
{	
	// In the event of a code dump, the app will exit after 10 seconds (unless you press POWER)
	__exception_setreload(10);
	Initialize();
	fatMountSimple("sd", &__io_wiisd);
	#ifdef USBGECKO
	Debug("fatMountSimple() Done");
	#endif
	
	//Disk light turn on and init
	//WIILIGHT_Init();
	WIILIGHT_SetLevel(255);
	WIILIGHT_TurnOn();
	
	//MAP mainMAP;
	
	//Check if the user uses a SD or a USB
	char test;
	sscanf(argv[0], "%c", &test); //read first character from argv[0] into test
	if(test == 115){ fatDevice = FAT_DEVICE_SD; } //first character = s
	else if(test == 117){ fatDevice = FAT_DEVICE_USB; } //first character = u
	//#ifdef USBGECKO
	//Debug("The User Has a %s", fatDevice); for some reason it does not work.
	//#endif
	
	//API:
	//API mainAPI;
	//#ifdef USBGECKO
	//Debug("mainAPI() Done");
	//#endif
	//mainAPI.initAPI();
	//#ifdef USBGECKO
	//Debug("initAPI() Done");
	//#endif
	bool debugText = false;
	bool running = true;
	int texture = 0;
	bool mainGame = true;
	
	//END OF INIT'S
	
	Image grass((uint8_t *)grass_png);
	Image block((uint8_t *)block_png);
	#ifdef USBGECKO
	Debug("Image() Done");
	Debug("All Inits is Done");
	#endif
	
	/*while(true){
		if(false == !false){
			test:printf("TEST IF GOTO WORKS\n");
		}
		goto test;
	}*/
	
	
	bool menuRun = true;
	int choose = 0;
	
	printf("\x1b[2;0H");
	VIDEO_ClearFrameBuffer(rmode,xfb[fb],COLOR_BLACK);
	SwapBuffer();
	
	mainMenu:while(menuRun){
		printf("\x1b[2;0H");
		VIDEO_ClearFrameBuffer(rmode,xfb[fb],COLOR_BLACK);
		SwapBuffer();
		printf("WiiCraft %s\n", "6.3");
		printf("========[Menu]========\n");
		printf("[*]A: Start Game\n");
		if(texture == 0){
			printf("[*]B: Set Block Texture To: %s             \n", "Grass");
		}
		else if(texture == 1){
			printf("[*]B: Set Block Texture To: %s              \n", "Dirt");
		}
		printf("[*]Home: Quit Game\n");
		while(running == true) {
			UpdatePad();
			if(WPAD_ButtonsDown(0) & WPAD_BUTTON_A){ //make sure that the function only run one time every press
				choose = 1;
				printf("\x1b[2;0H");
				VIDEO_ClearFrameBuffer(rmode,xfb[fb],COLOR_BLACK);
				SwapBuffer();
				running = false;
			}
			else if(WPAD_ButtonsDown(0) & WPAD_BUTTON_B){ //make sure that the function only run one time every press
				choose = 0;
				if(texture == 0){
					texture = 1;
				}
				else if(texture == 1){
					texture = 0;
				}
				printf("\x1b[2;0H");
				VIDEO_ClearFrameBuffer(rmode,xfb[fb],COLOR_BLACK);
				SwapBuffer();
				goto mainMenu;
			}
			else if(pressed & WPAD_BUTTON_HOME){
				choose = 3;
				printf("\x1b[2;0H");
				VIDEO_ClearFrameBuffer(rmode,xfb[fb],COLOR_BLACK);
				SwapBuffer();
				printf("Stoping WiiCraft...");
				running = false;
			}
			
		}
		if(choose == 1 || choose == 3){
			break;
		}
	}

	
  /*----------------------------------------<Main Game Loop>-----------------------------------------*/
  if(choose == 1){
	while(mainGame == true && choose == 1){
		UpdatePad();
		printf("\x1b[%d;%dH", 2, 0);
		printf("Press + For Pause Menu");
		if(debugText == true){
			//GX_SetViewport(0, 0, rmode->fbWidth, rmode->efbHeight, 0, 1);
			VIDEO_ClearFrameBuffer(rmode,xfb[fb],COLOR_BLACK);
			printf("\x1b[%d;%dH", 2, 0);
			printf("FPS: %f\n", fps);
			printf("PosX: %f   PosY: %f  PosZ: %f\n", world.player->position.x, world.player->position.y, world.player->position.z);
			printf("Pitch: %f   Yaw: %f\n", world.player->pitch, world.player->yaw);
			printf("Size: %i\n", world.chunkHandler->chunkList.size());
			printf("ChunkX: %i  ChunkY: %i  ChunkZ: %i\n", world.player->chunk_x, world.player->chunk_y, world.player->chunk_z);
			printf("BlockX: %i  BlockY: %i  BlockZ: %i\n", world.player->block_x, world.player->block_y, world.player->block_z);	
			printf("Player Status: %s  Velocity.y: %f\n", world.player->status == ON_AIR ? "AIR" : "GROUND",world.player->velocity.y);
		}
		
		if(texture == 0){
		grass.setGX(GX_TEXMAP0);
		}
		else if(texture == 1){
		block.setGX(GX_TEXMAP0);
		}
		DrawCubeTex(0,0,-5);

		world.update();
		world.drawChunks();

		MoveCamera();
		UpdateCamera();
		SwapBuffer();
		FPS(&fps);
		if (pressed & WPAD_BUTTON_PLUS ){
			/*printf("========[Pause]========\n");
			printf("[*]A: Resume\n");
			printf("[*]Home: Quit to Menu\n");
			while(1){
				if(pressed & WPAD_BUTTON_A){
					break;
				}
				else if(pressed & WPAD_BUTTON_HOME){*/
					//--------
					printf("\x1b[2;0H");
					VIDEO_ClearFrameBuffer(rmode,xfb[fb],COLOR_BLACK);
					SwapBuffer();
					//--------
					printf("\x1b[2;0H");
					VIDEO_ClearFrameBuffer(rmode,xfb[fb],COLOR_BLACK);
					SwapBuffer();
					//--------
					#ifdef USBGECKO
					Debug("--<Main Loop Exited>--");
					#endif
					choose = 0;
					running = true;
					goto mainMenu;
				/*}
			}
			continue;*/
		}
		if ((pressed & WPAD_BUTTON_UP) && (pressed & WPAD_BUTTON_A) && (pressed & WPAD_BUTTON_B)) debugText = !debugText;
	}
}
else if(choose == 3){
	printf("\x1b[2;0H");
	VIDEO_ClearFrameBuffer(rmode,xfb[fb],COLOR_BLACK);
	SwapBuffer();
	Deinitialize();
	exit(0);
}
	#ifdef USBGECKO
	Debug("ERROR: PASSED LAST LINE OF CODE!");
	#endif
	exit(-1);
}
/*----------------------------------------<End Of Main Game Loop>-----------------------------------------*/





void MoveCamera()
{
	/*if(expansion_type == WPAD_EXP_NUNCHUK)
	{
		if(js->mag >= 0.2f)
		{
			float n_angle = DegToRad(NunchukAngle(js->ang));
			world.player->move( cos(n_angle)*js->mag / 5.0f, world.player->camera->getRightVector());
			world.player->move(-sin(n_angle)*js->mag / 5.0f, world.player->camera->getForwardVector());	
		}
	}*/
	Chunk *cp;
	Block *bp;
	
		/*cp = chunkHandler->chunkList[getWorldIndex(world.player->chunk_x, world.player->chunk_y, world.player->chunk_z)];
		cp = cp->rightNeighbour;
		bp = cp->blockList[3][0][0];
		bp->transparent = true;
		cp->needsUpdate = true;*/
	
	if(pressed & WPAD_BUTTON_A)
	{
		cp = world.chunkHandler->chunkList[getWorldIndex(world.player->chunk_x, world.player->chunk_y, world.player->chunk_z)];
		bp = cp->blockList[world.player->block_z][world.player->block_y][world.player->block_x];
		bp->transparent = true;
		cp->needsUpdate = true;
		if(world.player->block_x == 0)
			world.chunkHandler->setChunkToUpdate(cp->leftNeighbour);
		if(world.player->block_x == (CHUNK_SIZE-1))
			world.chunkHandler->setChunkToUpdate(cp->rightNeighbour);
		if(world.player->block_y == 0)
			world.chunkHandler->setChunkToUpdate(cp->downNeighbour);
		if(world.player->block_y == (CHUNK_SIZE-1))
			world.chunkHandler->setChunkToUpdate(cp->upNeighbour);
		if(world.player->block_z == 0)
			world.chunkHandler->setChunkToUpdate(cp->backNeighbour);
		if(world.player->block_z == (CHUNK_SIZE-1))
			world.chunkHandler->setChunkToUpdate(cp->frontNeighbour);
	}


	if(pressed & WPAD_BUTTON_B)
	{
		cp = world.chunkHandler->chunkList[getWorldIndex(world.player->chunk_x, world.player->chunk_y, world.player->chunk_z)];
		bp = cp->blockList[world.player->block_z][world.player->block_y][world.player->block_x];
		bp->transparent = false;
		cp->needsUpdate = true;
		if(world.player->block_x == 0)
			world.chunkHandler->setChunkToUpdate(cp->leftNeighbour);
		if(world.player->block_x == (CHUNK_SIZE-1))
			world.chunkHandler->setChunkToUpdate(cp->rightNeighbour);
		if(world.player->block_y == 0)
			world.chunkHandler->setChunkToUpdate(cp->downNeighbour);
		if(world.player->block_y == (CHUNK_SIZE-1))
			world.chunkHandler->setChunkToUpdate(cp->upNeighbour);
		if(world.player->block_z == 0)
			world.chunkHandler->setChunkToUpdate(cp->backNeighbour);
		if(world.player->block_z == (CHUNK_SIZE-1))
			world.chunkHandler->setChunkToUpdate(cp->frontNeighbour);
	}
	
	
	/*if(pressed & WPAD_NUNCHUK_BUTTON_C)
		world.player->position.y += 0.25f;
	if(pressed & WPAD_NUNCHUK_BUTTON_Z)
		world.player->position.y -= 0.25f;
		
	if(pressed & WPAD_BUTTON_1)
		world.player->position.y += 0.25f;
	if(pressed & WPAD_BUTTON_2)
		world.player->position.y -= 0.25f;
		
	if(pressed & WPAD_BUTTON_UP)
		world.player->pitch += 0.02f;
	if(pressed & WPAD_BUTTON_DOWN)
		world.player->pitch -= 0.02f;
		
	if(pressed & WPAD_BUTTON_RIGHT)
		world.player->yaw -= 0.02f;
	if(pressed & WPAD_BUTTON_LEFT)
		world.player->yaw += 0.02f;*/
		
}


void UpdateCamera()
{
	guMtxConcat(world.getCameraView(), model, modelview);
	GX_LoadPosMtxImm(modelview, GX_PNMTX0);
}


int Menu(int texture){return false;
}


