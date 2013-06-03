#include "Block.hpp"

//Constructor
	Block::Block(Chunk *chunkP, int pos_x, int pos_y, int pos_z, int ID2){
		
		ID = ID2;
		transparent = ID2;
		chunkPointer = chunkP;	
		
	}
	
//Destructor
	Block::~Block(){
		
	}
	
//Methods
