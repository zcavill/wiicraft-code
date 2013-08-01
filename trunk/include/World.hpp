#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include <gccore.h>
#include "Player.hpp"
#include "Camera.hpp"
#include "ChunkHandler.hpp"
#include "Block.hpp"
#include "Wiicraft.hpp"

class World
{
public:
//Constructor
	World();
//Destructor
	~World();
//Methods
	void update();
	
	void getBlockBoundingBox(int chunk_x, int chunk_y, int chunk_z, int block_x, int block_y, int block_z, BoundingBox *bBox);
	//Player
		void updatePlayerPhysics();
		void movePlayer();
		Block *getBlockUnderPlayer(Vertex32 *block_pos);
	void updateControls();
//Inline Methods
	MtxP getCameraView() {return (MtxP)cameraView;}
	void drawChunks()    {chunkHandler->draw();}
//Variables
	//View
		Mtx cameraView;
	//worldPosition
		Vertex32 worldPosition;
	//ChunkHandler
		ChunkHandler *chunkHandler;
	//Player
		Player *player;
	//Bounding box
		//BoundingBox worldBB;
	//Gravity
		static const float worldGravity = 9.8f;
	//Controls
		//Wiimote
			WPADData *wiimoteData;
		//Nunhcuk
			joystick_t *nunchukJoystick;
		//Expansion
			u32 wiimoteExpansion;

};



#endif
