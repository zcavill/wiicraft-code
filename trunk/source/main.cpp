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
#include "Image.hpp"
#include "utils.h"
#include "World.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "World.hpp"

#include "grass_png.h"
#include "stone_png.h"

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
void UpdateCamera();
void MoveCamera();

int main(int argc, char **argv)
{	
	fatInitDefault();
	fatMountSimple("sd", &__io_wiisd);	
	WPAD_Init();
	InitVideo();
	InitPad();
	initFPS();
	
	Image grass((uint8_t *)grass_png);
	Image stone((uint8_t *)stone_png);

	while(1)
	{
		Clean();
		UpdatePad();
		printf("FPS: %f\n", fps);
		printf("posx: %f   posy: %f  posz: %f\n", world.player->position.x, world.player->position.y, world.player->position.z);
		printf("pitch: %f   yaw: %f\n", world.player->pitch, world.player->yaw);
		printf("size: %i\n", world.chunkHandler->chunkList.size());
		
		printf("chunkX: %i  chunkY: %i  chunkZ: %i\n", world.player->chunk_x, world.player->chunk_y, world.player->chunk_z);
		printf("blockX: %i  blockY: %i  blockZ: %i\n", world.player->block_x, world.player->block_y, world.player->block_z);	
		printf("player status: %s  velocity.y: %f\n", world.player->status == ON_AIR ? "AIR" : "GROUND",world.player->velocity.y);


		grass.setGX(GX_TEXMAP0);
		DrawCubeTex(0,0,-5);
		
		world.update();
		world.drawChunks();

			

		MoveCamera();
		UpdateCamera();
		SwapBuffer();
		FPS(&fps);
		if (pressed & WPAD_BUTTON_HOME ) break;
	}
	WPAD_Shutdown();
	EndVideo();
	exit(0);
	return 0;
}

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


