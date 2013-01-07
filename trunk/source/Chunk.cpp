#include "Chunk.h"


//Constructor
	Chunk::Chunk(int initx, int inity, int initz, bool trans)
	{
		position.x = initx;
		position.y = inity;
		position.z = initz;
		
		for(int z = 0; z < CHUNK_SIZE; z++)
		{
			for(int y = 0; y < CHUNK_SIZE; y++)
			{
				for(int x = 0; x < CHUNK_SIZE; x++)
				{
					blockList[z][y][x] = new Block();
					blockList[z][y][x]->transparent = trans;
				}
			}
		}
		//
		
		for(int z = 0; z < CHUNK_SIZE; z++)
		{
			for(int y = 0; y < CHUNK_SIZE; y++)
			{
				for(int x = 0; x < CHUNK_SIZE; x++)
				{
					
				//	if(z == 2) blockList[z][y][x]->transparent = true;
				}
			}
		}
		
		//
		

	}
	
	
//Destructor
	Chunk::~Chunk()
	{
		for(int z = 0; z < CHUNK_SIZE; z++)
		{
			for(int y = 0; y < CHUNK_SIZE; y++)
			{
				for(int x = 0; x < CHUNK_SIZE; x++)
				{
					delete blockList[z][y][x];
				}
				delete[] blockList[z][y];
			}
			delete[] blockList[z];
		}
		delete[] blockList;
		mesh.~Mesh();
	}
	
//Methods
	Block* Chunk::getBlockP(uint32_t x, uint32_t y, uint32_t z)
	{
		if(x < 0 || y < 0 || z < 0) return NULL;
		if(x >= CHUNK_SIZE ||y >= CHUNK_SIZE || z >= CHUNK_SIZE) return NULL;
		return blockList[z][y][x];
	}

	void Chunk::draw()
	{
		mesh.render();
	}
	
	//Cube faces
		void Chunk::addTop(float x, float y, float z, Color4u8 color)
		{
			mesh.addMeshData((MeshData){x, y + BLOCK_SIZE, z, color});
			mesh.addMeshData((MeshData){x + BLOCK_SIZE, y + BLOCK_SIZE, z, color});
			mesh.addMeshData((MeshData){x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE, color});
			mesh.addMeshData((MeshData){x, y + BLOCK_SIZE, z + BLOCK_SIZE, color});
		}
		
	
		void Chunk::addBottom(float x, float y, float z, Color4u8 color)
		{
			mesh.addMeshData((MeshData){x, y, z, color});
			mesh.addMeshData((MeshData){x + BLOCK_SIZE, y, z, color});
			mesh.addMeshData((MeshData){x + BLOCK_SIZE, y, z + BLOCK_SIZE, color});
			mesh.addMeshData((MeshData){x, y, z + BLOCK_SIZE, color});		
		}


		void Chunk::addRight(float x, float y, float z, Color4u8 color)
		{
			mesh.addMeshData((MeshData){x + BLOCK_SIZE, y, z, color});
			mesh.addMeshData((MeshData){x + BLOCK_SIZE, y, z + BLOCK_SIZE, color});
			mesh.addMeshData((MeshData){x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE, color});				
			mesh.addMeshData((MeshData){x + BLOCK_SIZE, y + BLOCK_SIZE, z, color});	
		}
		
		void Chunk::addLeft(float x, float y, float z, Color4u8 color)
		{
			mesh.addMeshData((MeshData){x, y, z, color});
			mesh.addMeshData((MeshData){x, y, z + BLOCK_SIZE, color});
			mesh.addMeshData((MeshData){x, y + BLOCK_SIZE, z + BLOCK_SIZE, color});			
			mesh.addMeshData((MeshData){x, y + BLOCK_SIZE, z, color});			
		}

		
		void Chunk::addFront(float x, float y, float z, Color4u8 color)
		{
			mesh.addMeshData((MeshData){x, y, z + BLOCK_SIZE, color});
			mesh.addMeshData((MeshData){x + BLOCK_SIZE, y, z + BLOCK_SIZE, color});
			mesh.addMeshData((MeshData){x + BLOCK_SIZE, y + BLOCK_SIZE, z + BLOCK_SIZE, color});
			mesh.addMeshData((MeshData){x, y + BLOCK_SIZE, z + BLOCK_SIZE, color});			
		}
		
		void Chunk::addBack(float x, float y, float z, Color4u8 color)
		{
			mesh.addMeshData((MeshData){x, y, z, color});
			mesh.addMeshData((MeshData){x + BLOCK_SIZE, y, z, color});
			mesh.addMeshData((MeshData){x + BLOCK_SIZE, y + BLOCK_SIZE, z, color});
			mesh.addMeshData((MeshData){x, y + BLOCK_SIZE, z, color});	
		}
		
	//Cube faces index
		void Chunk::addTopIndex(float x, float y, float z, Color4u8 color)
		{
			addTop(position.x + x * BLOCK_SIZE, position.y + y * BLOCK_SIZE, position.z + z * BLOCK_SIZE, color);
		}
		
		void Chunk::addBottomIndex(float x, float y, float z, Color4u8 color)
		{
			addBottom(position.x + x * BLOCK_SIZE, position.y + y * BLOCK_SIZE, position.z + z * BLOCK_SIZE, color);
		}

		void Chunk::addRightIndex(float x, float y, float z, Color4u8 color)
		{
			addRight(position.x + x * BLOCK_SIZE, position.y + y * BLOCK_SIZE, position.z + z * BLOCK_SIZE, color);
		}
		
		void Chunk::addLeftIndex(float x, float y, float z, Color4u8 color)
		{
			addLeft(position.x + x * BLOCK_SIZE, position.y + y * BLOCK_SIZE, position.z + z * BLOCK_SIZE, color);
		}
		
		void Chunk::addFrontIndex(float x, float y, float z, Color4u8 color)
		{
			addFront(position.x + x * BLOCK_SIZE, position.y + y * BLOCK_SIZE, position.z + z * BLOCK_SIZE, color);
		}
		
		void Chunk::addBackIndex(float x, float y, float z, Color4u8 color)
		{
			addBack(position.x + x * BLOCK_SIZE, position.y + y * BLOCK_SIZE, position.z + z * BLOCK_SIZE, color);
		}
		
		
		
