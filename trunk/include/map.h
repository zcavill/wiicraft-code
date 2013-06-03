/*									

	(c)Filiph Sandström
									
									*/
#include "ChunkHandler.hpp"
#ifndef MAP_H
#define MAP_H

#define Xmap 32
#define Ymap 32
#define Zmap 32
#define groundLevel 0
#define gorundID 1

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <cstdio>
#include <stdio.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <zlib.h>
#include <vector>

using namespace std;

class MAP {
	private:
		bool mapFromFile;
		
		
	public:
		MAP();
		~MAP();
		void loadMap();
		void createMapArray();
		void generateMap();
		void closeMap();
		void updateMap();
		void saveMap();
		
		//Public valuse:
		string mapPath;
		double mapArray[Zmap][Ymap][Xmap];
};
static int seeGeneratedMapBlock(int z, int y, int x){
	return 0;
}

#endif