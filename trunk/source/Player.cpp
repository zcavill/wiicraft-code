#include "Player.h"


//Constructor
	Player::Player(MtxP view, float init_x, float init_y, float init_z)
	{
		camera = new Camera(view, &position, &pitch, &yaw);
		position.x = init_x;
		position.y = init_y;
		position.z = init_z;
		yaw = 0.0f;
		pitch = 0.0f;
		status = ON_AIR;
	}
	
	Player::Player(float init_x, float init_y, float init_z)
	{
		position.x = init_x;
		position.y = init_y;
		position.z = init_z;
		yaw = 0.0f;
		pitch = 0.0f;
		status = ON_AIR;
	}	
//Destructor
	Player::~Player()
	{
		delete camera;
	}
	
//Methods	
	//Move and translate
		void Player::move(float distance, guVector direction)
		{
			position.x += direction.x * distance;
			position.y += direction.y * distance;
			position.z += direction.z * distance;
		}
		
		void Player::translate(float x, float y, float z)
		{
			position.x = x;
			position.y = y;
			position.z = z;
		}	
		
	//Update
		void Player::update()
		{
			//Camera
				if(yaw > PI_2) yaw -= PI_2;
				if(yaw < -PI_2) yaw += PI_2;
				if(pitch > HALF_PI) pitch = HALF_PI;
				if(pitch < -HALF_PI) pitch = -HALF_PI;
			//World limits
				if(position.x > WORLD_TOTAL_SIZE) position.x = WORLD_TOTAL_SIZE;
				if(position.y > WORLD_TOTAL_SIZE) position.y = WORLD_TOTAL_SIZE;
				if(position.z > WORLD_TOTAL_SIZE) position.z = WORLD_TOTAL_SIZE;

				if(position.x < 0) position.x = 0;
				if(position.y < 0) position.y = 0;
				if(position.z < 0) position.z = 0;
			//Chunk and block
				chunk_x = (int)((float)(position.x - WORLD_INIT_POS) / (float)(CHUNK_TOTAL_SIZE));
				chunk_y = (int)((float)(position.y - WORLD_INIT_POS) / (float)(CHUNK_TOTAL_SIZE));
				chunk_z = (int)((float)(position.z - WORLD_INIT_POS) / (float)(CHUNK_TOTAL_SIZE));
				block_x = (int)((position.x - chunk_x * CHUNK_TOTAL_SIZE) / (float)BLOCK_SIZE);
				block_y = (int)((position.y - chunk_y * CHUNK_TOTAL_SIZE) / (float)BLOCK_SIZE);
				block_z = (int)((position.z - chunk_z * CHUNK_TOTAL_SIZE) / (float)BLOCK_SIZE);		
				
				
		}
		
		

