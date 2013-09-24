#ifndef _WORLD_H_
#define _WORLD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include "utils.h"
#include "Math_utils.h"
#include "Block.h"
#include "Camera.h"
#include "Player.h"
#include "ChunkHandler.h"
#include "Wiicraft.h"
extern "C"{
#include "sicksaxis.h"
}


class World
{
public:
//Constructor
	World();
//Destructor
	~World();
	
//Methods
	//View Matrix
		MtxP getViewMarix(){return viewMatrix;}
		void updateViewMatrix(){player->updateViewMatrix();}
		
	//Draw
		void drawWorld();
	//Update
		void update();
	//Player
		Chunk* playerChunk();
		Block* playerBlock();
		void playerAiming();
		void playerUpdate();

//Variables
	guVector initPosition;
	Player *player;

	Mtx viewMatrix;
	ChunkHandler *chunkHandler;
//Controls
	u32 worldButtonsDown;
	SickSaxis sickSaxis;
	
};


#endif
