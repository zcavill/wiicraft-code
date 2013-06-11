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
#include "main.h"

using namespace std;

int initFolders(int SD){

	if(SD == FAT_DEVICE_SD)
		return mkdir("sd:/WiiCraft", S_IRWXU|S_IRGRP|S_IXGRP);
	else if(SD == FAT_DEVICE_USB)
		return mkdir("usb:/WiiCraft", S_IRWXU|S_IRGRP|S_IXGRP);
	else
		return FAT_DEVICE_NONE;
}
