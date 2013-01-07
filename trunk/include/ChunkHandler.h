#ifndef _CHUNKHANDLER_H_
#define _CHUNKHANDLER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <vector>
#include <math.h>
#include <gccore.h>
#include "utils.h"
#include "Math_utils.h"
#include "Block.h"
#include "Chunk.h"
#include "Wiicraft.h"


class ChunkHandler
{
public:
//Constructor
	ChunkHandler(float init_x, float init_y, float init_z);
//Destructor
	~ChunkHandler();
	
//Methods
	void update();
	void draw();
	Chunk *getChunkP(int chunk_x, int chunk_y, int chunk_z);
	Block *getChunkBlockP(int chunk_x, int chunk_y, int chunk_z, int block_x, int block_y, int block_z);
	bool isSolid(int chunk_x, int chunk_y, int chunk_z, int block_x, int block_y, int block_z);
	void generateAllMeshes();
	void generateMesh(int chunk_x, int chunk_y, int chunk_z);
	Vertex32 getBlockPosition(int chunk_x, int chunk_y, int chunk_z, int block_x, int block_y, int block_z)
	{
		return (Vertex32){chunk_x + CHUNK_TOTAL_SIZE + block_x * BLOCK_SIZE,
						  chunk_y + CHUNK_TOTAL_SIZE + block_y * BLOCK_SIZE,
						  chunk_z + CHUNK_TOTAL_SIZE + block_z * BLOCK_SIZE};
	}
	//Blocks
		void setTransparent(int chunk_x, int chunk_y, int chunk_z, int block_x, int block_y, int block_z);
		void setNonTransparent(int chunk_x, int chunk_y, int chunk_z, int block_x, int block_y, int block_z);
		
//private:
	guVector position;
	Chunk *chunkList[WORLD_VOLUME];
};


#endif
