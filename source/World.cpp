#include "World.h"


//Constructor
	World::World()
	{
		ss_init();
		ss_open(&sickSaxis);
		
		initPosition.x = WORLD_INIT_POS; initPosition.y = WORLD_INIT_POS; initPosition.z = WORLD_INIT_POS;
		
		//Player
			player = new Player(viewMatrix, (initPosition.x + WORLD_SIZE * CHUNK_SIZE * BLOCK_SIZE)/2,
											(initPosition.y + WORLD_SIZE * CHUNK_SIZE * BLOCK_SIZE)/2,
											(initPosition.z + WORLD_SIZE * CHUNK_SIZE * BLOCK_SIZE)/2);
		//Chunk handler
			chunkHandler = new ChunkHandler(initPosition.x, initPosition.y, initPosition.z);
											
		
	}
	
//Destructor
	World::~World()
	{
		delete player;
		delete chunkHandler;
		ss_close(&sickSaxis);
		ss_finish();
	}
	
//Methods

	void World::drawWorld()
	{
		chunkHandler->draw();
	}

//Update
	void World::update()
	{
		//Controls
			worldButtonsDown = WPAD_ButtonsDown(0);
		//Sicksaxis
			if(!sickSaxis.connected)
			{
				if((WPAD_ButtonsHeld(0) & WPAD_BUTTON_MINUS) && (WPAD_ButtonsHeld(0) & WPAD_BUTTON_PLUS))
				{
					ss_open(&sickSaxis);
				}
			}
			else
			{
				ss_read(&sickSaxis);
			}
		//Player
			playerAiming();
			playerUpdate();		
			player->update();				
	}
		
//Player
	void World::playerUpdate()
	{
		switch(player->status)
		{
		case ON_AIR:
			player->move(GRAVITY_VALUE, UP_VECTOR);
			if(chunkHandler->isSolid(player->chunk_x, player->chunk_y, player->chunk_z, player->block_x, player->block_y, player->block_z))
			{
				//Vertex32 blockPos = chunkHandler->getBlockPosition(player->chunk_x, player->chunk_y, player->chunk_z,
																	   //player->block_x, player->block_y + 1, player->block_z);					
				player->position.y += BLOCK_SIZE;
				player->status = ON_GROUND;
			}
			break;
			
		case ON_GROUND:
			if(!chunkHandler->isSolid(player->chunk_x, player->chunk_y, player->chunk_z, player->block_x, player->block_y - 1, player->block_z))
			{
				player->status = ON_AIR;
			}
			break;
		default:
			break;
		}		
	}


	void World::playerAiming()
	{
		float cos_pitch = cosf(player->pitch);
		float sin_pitch = sinf(player->pitch);
		
		float cos_yaw = cosf(-player->yaw - HALF_PI);
		float sin_yaw = sinf(-player->yaw - HALF_PI);
		
		float x_ray = player->position.x;
		float y_ray = player->position.y;
		float z_ray = player->position.z;
		
		int ray_chunk_x, ray_chunk_y, ray_chunk_z, ray_block_x, ray_block_y, ray_block_z; 
		
		Block *playerB = playerBlock();
		Chunk *rayChunk;
		Block *rayBlock;
		
		float inc_x = cos_yaw * cos_pitch * AIM_CAST_STEP;
		float inc_y = sin_pitch * AIM_CAST_STEP;
		float inc_z = sin_yaw * cos_pitch * AIM_CAST_STEP;
		
		for(float i = 0; i < AIM_MAX_DISTANCE; i += AIM_CAST_STEP)
		{
			x_ray += inc_x;
			y_ray += inc_y;
			z_ray += inc_z;
		
			
			ray_chunk_x = (int)((float)(x_ray - WORLD_INIT_POS) / (float)(CHUNK_TOTAL_SIZE));
			ray_chunk_y = (int)((float)(y_ray - WORLD_INIT_POS) / (float)(CHUNK_TOTAL_SIZE));
			ray_chunk_z = (int)((float)(z_ray - WORLD_INIT_POS) / (float)(CHUNK_TOTAL_SIZE));
			ray_block_x = (int)((x_ray - ray_chunk_x * CHUNK_TOTAL_SIZE) / (float)BLOCK_SIZE);
			ray_block_y = (int)((y_ray - ray_chunk_y * CHUNK_TOTAL_SIZE) / (float)BLOCK_SIZE);
			ray_block_z = (int)((z_ray - ray_chunk_z * CHUNK_TOTAL_SIZE) / (float)BLOCK_SIZE);	

			
			rayChunk = chunkHandler->getChunkP(ray_chunk_x, ray_chunk_y, ray_chunk_z);
			if(rayChunk == NULL) break;
			rayBlock = rayChunk->getBlockP(ray_block_x, ray_block_y, ray_block_z);
			
			if((rayBlock != NULL) && playerB != rayBlock)
			{
				if(chunkHandler->isSolid(ray_chunk_x, ray_chunk_y, ray_chunk_z,
								         ray_block_x, ray_block_y, ray_block_z))
				{
					
					if(worldButtonsDown & WPAD_BUTTON_B || sickSaxis.gamepad.buttons.R1)
					{
						chunkHandler->setTransparent(ray_chunk_x, ray_chunk_y, ray_chunk_z,
													 ray_block_x, ray_block_y, ray_block_z);
					}					
					if(worldButtonsDown & WPAD_BUTTON_A || sickSaxis.gamepad.buttons.L1)
					{
						chunkHandler->setNonTransparent(ray_chunk_x, ray_chunk_y, ray_chunk_z,
													 ray_block_x, ray_block_y + 1, ray_block_z);
					}						
					
					DrawCubeWire(ray_chunk_x, ray_chunk_y, ray_chunk_z,
								         ray_block_x, ray_block_y, ray_block_z);
					printf("X: %i   Y: %i   Z: %i\n", ray_block_x, ray_block_y, ray_block_z);
					break;			
				}
			}
		}
		
	}


	Chunk* World::playerChunk()
	{			
		return chunkHandler->getChunkP(player->chunk_x, player->chunk_y, player->chunk_z);
	}

	Block* World::playerBlock()
	{		
		return chunkHandler->getChunkP(player->chunk_x, player->chunk_y, player->chunk_z)->  \
							 getBlockP(player->block_x, player->block_y, player->block_z);
	}
					
		
		
		
