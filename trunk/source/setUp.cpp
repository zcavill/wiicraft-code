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
#include "setUp.h"
#include "utils.h"

using namespace std;

void initFolders(bool SD){
	
	int status;
	
	if(SD){
		status = mkdir("sd:/Wiicraft", 0777);
	}
	//else
		status = mkdir("usb:/Wiicraft", 0777);
	
}

void registerBlockToAddOnScreen(float x,float y,float z){
	
	DrawCube(x,y,z);
	
}
