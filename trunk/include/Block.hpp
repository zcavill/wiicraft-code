#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <gccore.h>
#include <Math_utils.h>
#include <BoundingBox.hpp>
#include <Wiicraft.hpp>
#include <Chunk.hpp>

class Chunk;

class Block
{
public:
//Constructor
	Block(Chunk *chunkP, int pos_x, int pos_y, int pos_z);
//Destructor
	~Block();
//Methods

//Variables
//private:
	Chunk *chunkPointer;
	uint8_t ID;
	bool transparent;
};



#endif
