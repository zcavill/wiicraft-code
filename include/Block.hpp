#ifndef _BLOCK_HPP_
#define _BLOCK_HPP_

#include <gccore.h>
#include <Math_utils.h>
#include <BoundingBox.hpp>
#include <Wiicraft.hpp>
#include <Chunk.hpp>
//#include "map.h"

class Chunk;

class Block
{
public:
//Constructor
	Block(Chunk *chunkP, int pos_x, int pos_y, int pos_z, int ID2);
//Destructor
	~Block();
//Methods

//Variables
//private:
	Chunk *chunkPointer;
	uint8_t ID;
	bool transparent;
	Block & operator=(const Block &rhs);
};



#endif
