#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include "utils.h"
#include "Math_utils.h"
#include "Block.h"
#include "Mesh.h"
#include "Wiicraft.h"


class Chunk
{
public:
//Constructor
	Chunk(int initx, int inity, int initz, bool trans = false);
//Destructor
	~Chunk();
	
//Methods
	void update();
	void draw();
	Block* getBlockP(uint32_t x, uint32_t y, uint32_t z);
	//Cube faces
		void addTop(float x, float y, float z, Color4u8 color);
		void addBottom(float x, float y, float z, Color4u8 color);
		
		void addRight(float x, float y, float z, Color4u8 color);
		void addLeft(float x, float y, float z, Color4u8 color);
		
		void addFront(float x, float y, float z, Color4u8 color);
		void addBack(float x, float y, float z, Color4u8 color);
	//Cube faces index
		void addTopIndex(float x, float y, float z, Color4u8 color);
		void addBottomIndex(float x, float y, float z, Color4u8 color);
		
		void addRightIndex(float x, float y, float z, Color4u8 color);
		void addLeftIndex(float x, float y, float z, Color4u8 color);
		
		void addFrontIndex(float x, float y, float z, Color4u8 color);
		void addBackIndex(float x, float y, float z, Color4u8 color);
		
	
//private:
	guVector position;
	Mesh mesh;
	Block *blockList[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
};



#endif

