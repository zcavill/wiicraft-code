#include "Block.hpp"

//Constructor
	Block::Block(Chunk *chunkP, int pos_x, int pos_y, int pos_z)
	{
		ID = 1; //0 is air
		transparent = false;
		chunkPointer = chunkP;	
		
	}
	
//Destructor
	Block::~Block()
	{
		
	}
	
//Methods
