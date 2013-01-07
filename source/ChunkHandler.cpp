#include "ChunkHandler.h"


//Constructor
	ChunkHandler::ChunkHandler(float init_x, float init_y, float init_z)
	{
		position.x = init_x;
		position.y = init_y;
		position.z = init_z;

		
		for(uint32_t chunk_z = 0; chunk_z < WORLD_SIZE; chunk_z++)
		{
			for(uint32_t chunk_y = 0; chunk_y < WORLD_SIZE; chunk_y++)
			{
				for(uint32_t chunk_x = 0; chunk_x < WORLD_SIZE; chunk_x++)
				{
					if(chunk_y < 1)
					{
						chunkList[toIndex(chunk_x, chunk_y, chunk_z)] = new Chunk(chunk_x * CHUNK_SIZE * BLOCK_SIZE,
																 chunk_y * CHUNK_SIZE * BLOCK_SIZE,
																 chunk_z * CHUNK_SIZE * BLOCK_SIZE);
					}
					else
					{
						chunkList[toIndex(chunk_x, chunk_y, chunk_z)] = new Chunk(chunk_x * CHUNK_SIZE * BLOCK_SIZE,
																 chunk_y * CHUNK_SIZE * BLOCK_SIZE,
																 chunk_z * CHUNK_SIZE * BLOCK_SIZE, true);						
					}
				}	
			}	
		}			
		
		generateAllMeshes();
		
	}
	
//Destructor
	ChunkHandler::~ChunkHandler()
	{
		for(uint32_t i = 0; i < WORLD_VOLUME; i++)
		{
			chunkList[i]->~Chunk();
			delete chunkList[i];
		}
	}
					
	
//Methods
	void ChunkHandler::update()
	{

	}
	
	Chunk *ChunkHandler::getChunkP(int chunk_x, int chunk_y, int chunk_z)
	{
		if(chunk_x < 0 || chunk_y < 0 || chunk_z < 0) return NULL;
		if(chunk_x > WORLD_SIZE_1 || chunk_y > WORLD_SIZE_1 || chunk_z > WORLD_SIZE_1) return NULL;
		return chunkList[toIndex(chunk_x, chunk_y, chunk_z)];
	}
	
	Block *ChunkHandler::getChunkBlockP(int chunk_x, int chunk_y, int chunk_z, int block_x, int block_y, int block_z)
	{
		if(block_x < 0){block_x = (CHUNK_SIZE) + block_x; chunk_x--;}
		if(block_y < 0){block_y = (CHUNK_SIZE) + block_y; chunk_y--;}
		if(block_z < 0){block_z = (CHUNK_SIZE) + block_z; chunk_z--;}
		
		if(block_x > CHUNK_SIZE_1){block_x = block_x - CHUNK_SIZE; chunk_x++;}
		if(block_y > CHUNK_SIZE_1){block_y = block_y - CHUNK_SIZE; chunk_y++;}
		if(block_z > CHUNK_SIZE_1){block_z = block_z - CHUNK_SIZE; chunk_z++;}
		
		Chunk *cp;
		if((cp = getChunkP(chunk_x, chunk_y, chunk_z)) == NULL) return NULL;
		
		return cp->getBlockP(block_x, block_y, block_z);
	}

	
	bool ChunkHandler::isSolid(int chunk_x, int chunk_y, int chunk_z, int block_x, int block_y, int block_z)
	{

		Block *bp = getChunkBlockP(chunk_x, chunk_y, chunk_z, block_x, block_y, block_z);
		if(bp == NULL) return false;
		
		if(bp->ID == 0 || bp->transparent)  //AIR
		{
			return false;
		}
		
		return true;
	}
	
	void ChunkHandler::generateAllMeshes()
	{
		for(uint32_t chunk_z = 0; chunk_z < WORLD_SIZE; chunk_z++)
		{
			for(uint32_t chunk_y = 0; chunk_y < WORLD_SIZE; chunk_y++)
			{
				for(uint32_t chunk_x = 0; chunk_x < WORLD_SIZE; chunk_x++)
				{
					generateMesh(chunk_x, chunk_y, chunk_z);
				}	
			}	
		}
	}
	
	void ChunkHandler::generateMesh(int chunk_x, int chunk_y, int chunk_z)
	{
		
		Chunk *cp = getChunkP(chunk_x, chunk_y, chunk_z);
		if(cp == NULL) return;
		Block *bp;
		
		cp->mesh.clear();
				
		for(uint32_t block_z = 0; block_z < CHUNK_SIZE; block_z++)
		{
			for(uint32_t block_y = 0; block_y < CHUNK_SIZE; block_y++)
			{
				for(uint32_t block_x = 0; block_x < CHUNK_SIZE; block_x++)
				{

					bp = cp->getBlockP(block_x,block_y,block_z);
					if(bp == NULL || bp->transparent == true) continue;		
								
					/* Check if the block is on the chunk limits or if the block is inside the chunk (not on the limits) */
					
						/* Left limit */
								if(!isSolid(chunk_x, chunk_y, chunk_z, block_x - 1, block_y, block_z))
									cp->addLeftIndex(block_x,block_y,block_z, (Color4u8){255, 255, 0, 255});								

						/* Right limit */
							if(!isSolid(chunk_x, chunk_y, chunk_z, block_x + 1, block_y, block_z))
									cp->addRightIndex(block_x,block_y,block_z, (Color4u8){255, 0, 0, 255});	
												
						/* Top limit */
							if(!isSolid(chunk_x, chunk_y, chunk_z, block_x, block_y + 1, block_z))
								cp->addTopIndex(block_x,block_y,block_z, (Color4u8){255, 126, 0, 255});						
		
						/* Bottom limit */
							if(!isSolid(chunk_x, chunk_y, chunk_z, block_x, block_y - 1, block_z))
								cp->addBottomIndex(block_x,block_y,block_z, (Color4u8){0, 255, 0, 255});

						/* Front limit */
							if(!isSolid(chunk_x, chunk_y, chunk_z, block_x, block_y, block_z + 1))
									cp->addFrontIndex(block_x,block_y,block_z,(Color4u8){0, 0, 255, 255});

					
						/* Back limit */
							if(!isSolid(chunk_x, chunk_y, chunk_z, block_x, block_y, block_z - 1))
								cp->addBackIndex(block_x,block_y,block_z, (Color4u8) {255, 0, 255, 255});							
							
					//Check Top
						/*bp = cp->getBlockP(block_x, block_y + 1, block_z);
						if(bp == NULL || bp->transparent || bp->ID == 0)
						{
							cp->addTopIndex(block_x,block_y,block_z, (Color4u8){255, 126, 0, 255});							
						}

					
					 //Check Bottom
						bp = cp->getBlockP(block_x, block_y - 1, block_z);
						if(bp == NULL || bp->transparent || bp->ID == 0)
						{
							cp->addBottomIndex(block_x,block_y,block_z, (Color4u8){0, 255, 0, 255});							
						}
						
					//Check Front
						bp = cp->getBlockP(block_x, block_y, block_z + 1);
						if(bp == NULL || bp->transparent || bp->ID == 0)
						{
							cp->addFrontIndex(block_x,block_y,block_z,(Color4u8){0, 0, 255, 255});						
						}
						
					//Check Back
						bp = cp->getBlockP(block_x, block_y, block_z - 1);
						if(bp == NULL || bp->transparent || bp->ID == 0)
						{
							cp->addBackIndex(block_x,block_y,block_z, (Color4u8) {255, 0, 255, 255});					
						}
						
					//Check Right
						bp = cp->getBlockP(block_x + 1, block_y, block_z);
						if(bp == NULL || bp->transparent || bp->ID == 0)
						{
							cp->addRightIndex(block_x,block_y,block_z, (Color4u8){255, 0, 0, 255});					
						}
						
					//Check Left
						bp = cp->getBlockP(block_x - 1, block_y, block_z);
						if(bp == NULL || bp->transparent || bp->ID == 0)
						{
							cp->addLeftIndex(block_x,block_y,block_z, (Color4u8){255, 255, 0, 255});			
						}*/							
					
					
				}
			}
		}		
	}
	
//Blocks
	void ChunkHandler::setTransparent(int chunk_x, int chunk_y, int chunk_z, int block_x, int block_y, int block_z)
	{
		Block *bp = getChunkBlockP(chunk_x, chunk_y, chunk_z, block_x, block_y, block_z);
		if(bp == NULL) return;
		bp->transparent = true;
		
		if(block_x == 0)
			generateMesh(chunk_x - 1, chunk_y, chunk_z);
		if(block_x > CHUNK_SIZE_1)
			generateMesh(chunk_x + 1, chunk_y, chunk_z);
		if(block_y == 0)
			generateMesh(chunk_x, chunk_y - 1, chunk_z);
		if(block_y > CHUNK_SIZE_1)
			generateMesh(chunk_x, chunk_y + 1, chunk_z);
		if(block_z == 0)
			generateMesh(chunk_x - 1, chunk_y, chunk_z - 1);
		if(block_z > CHUNK_SIZE_1)
			generateMesh(chunk_x + 1, chunk_y, chunk_z + 1);
								
		generateMesh(chunk_x, chunk_y, chunk_z);
		
	}
	
	void ChunkHandler::setNonTransparent(int chunk_x, int chunk_y, int chunk_z, int block_x, int block_y, int block_z)
	{
		Block *bp = getChunkBlockP(chunk_x, chunk_y, chunk_z, block_x, block_y, block_z);
		if(bp == NULL) return;
		bp->transparent = false;
		
		if(block_x == 0)
			generateMesh(chunk_x - 1, chunk_y, chunk_z);
		if(block_x > CHUNK_SIZE_1)
			generateMesh(chunk_x + 1, chunk_y, chunk_z);
		if(block_y == 0)
			generateMesh(chunk_x, chunk_y - 1, chunk_z);
		if(block_y > CHUNK_SIZE_1)
			generateMesh(chunk_x, chunk_y + 1, chunk_z);
		if(block_z == 0)
			generateMesh(chunk_x - 1, chunk_y, chunk_z - 1);
		if(block_z > CHUNK_SIZE_1)
			generateMesh(chunk_x + 1, chunk_y, chunk_z + 1);
								
		generateMesh(chunk_x, chunk_y, chunk_z);	
	}

	void ChunkHandler::draw()
	{
		for(uint32_t i = 0; i < WORLD_VOLUME; i++)
		{
			chunkList[i]->draw();
		}

	}

