/*

	(c)Filiph Sandström

									*/
//mapArray[Z][Y][X] = blockID <-- Good to know =) -filfat

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
#include <sstream>
#include <sys/stat.h>
#include <vector>

#include "map.h"
#include "utils.h"
#include "debug.h"

using namespace std;

MAP::MAP(){
	#ifdef USBGECKO
	Debug("Map() Started");
	#endif
	createMapArray();
	#ifdef USBGECKO
	Debug("Done With createMapArray();")
	#endif
	generateMap();
	#ifdef USBGECKO
	Debug("Done With generateMap();")
	Debug("Map() Ended");
	#endif
}

MAP::~MAP(){
	closeMap();
}

void MAP::loadMap() {
	
}

void MAP::saveMap() {
	
}

void MAP::updateMap() {
	
}

void MAP::createMapArray() {

  // Allocate memory
  /*mapArray = new double**[Xmap];
  for (int i = 0; i < Xmap; ++i) {
    mapArray[i] = new double*[Ymap];

    for (int j = 0; j < Ymap; ++j)
      mapArray[i][j] = new double[Zmap];
  }*/
}

void MAP::generateMap(){
	// Makes a Flat Map
	for(int tempX = 0; tempX <= Xmap; ++tempX){
		for(int tempY = 0; tempY <= Xmap; ++tempY){
			mapArray[groundLevel][tempY][tempX] = gorundID;
		}
	}
	
}

void MAP::closeMap(){
  /*// De-Allocate memory to prevent memory leak
  for (int i = 0; i < Xmap; ++i) {
    for (int j = 0; j < Ymap; ++j)
      delete [] mapArray[i][j];

    delete [] mapArray[i];
  }
  delete [] mapArray;*/
}