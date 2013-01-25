#ifndef _CHUNK_H_
#define _CHUNK_H_

#include "Mesh.hpp"
#include "Block.hpp"
#include "Math_utils.h"
#include "Wiicraft.hpp"

class Block;

class Chunk
{
public:
//Constructor
	Chunk(int pos_x, int pos_y, int pos_z, int index_x, int index_y, int index_z, bool transp);
//Destructor
	~Chunk();
//Methods
	bool isSolid(Block *bp);
	bool isSolid(int x, int y, int z) {return isSolid(getBlockP(x, y, z));};
	bool isTransparent(Block *bp);
	bool isTransparent(int x, int y, int z) {return isTransparent(getBlockP(x, y, z));}
	void generateMesh();
	Block* getBlockP(int x, int y, int z) {return blockList[z][y][x];}
	//Neighbours
		void setLeftNeighbour(Chunk *p)  {leftNeighbour  = p;}
		void setRightNeighbour(Chunk *p) {rightNeighbour = p;}
		void setUpNeighbour(Chunk *p) 	 {upNeighbour    = p;}
		void setDownNeighbour(Chunk *p)	 {downNeighbour  = p;}
		void setFrontNeighbour(Chunk *p) {frontNeighbour = p;}
		void setBackNeighbour(Chunk *p)	 {backNeighbour  = p;}

//Methods
	void initBlocks(bool transp);
	void deleteBlocks();
	void draw();
//Variables
	bool needsUpdate;
	Mesh *mesh;
	Block *blockList[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
	Vertex32 position;
	Vertex32 index;
	//Neighbours
		Chunk *leftNeighbour;
		Chunk *rightNeighbour;
		Chunk *upNeighbour;
		Chunk *downNeighbour;
		Chunk *frontNeighbour;
		Chunk *backNeighbour;
	
};



#endif
