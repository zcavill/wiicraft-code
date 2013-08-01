#ifndef _CHUNKHANDLER_HPP_
#define _CHUNKHANDLER_HPP_

#include <stdio.h>
#include <vector>
#include "Math_utils.h"
#include "Chunk.hpp"
#include "Wiicraft.hpp"


class ChunkHandler
{
public:
//Constructor
	ChunkHandler(Vertex32 *pos);
//Destructor
	~ChunkHandler();
//Methods
	Chunk *getChunkP(int x, int y, int z) {return chunkList[getWorldIndex(x,y,z)];}
	Block *getBlockAtPosition(int x, int y, int z);
	bool chunkInBounds(Chunk *chunkPointer);
	void clearChunkList();
	void updateChunks();
	void generateNeighbours();
	void draw();
	void setChunkToUpdate(Chunk *chunkPointer) {if(chunkPointer != NULL) chunkPointer->needsUpdate = true;}
	
//Variables
	Vertex32 *position;
	std::vector<Chunk *> chunkList;
};

#endif
