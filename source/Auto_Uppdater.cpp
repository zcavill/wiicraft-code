// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 3.0 for more details.

// Copyright (C) 2012-2013	filfat, xerpi, JoostinOnline

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

#include "Auto_Updater.h"
#include "mainAPI.h"
#include "Convert.h"
#include "ConfigFile.h"
#include "setUp.h"
#include "utils.h"
#include "debug.h"
#include "main.h"


using namespace std;

AutoUpdater::AutoUpdater(){
	bool DevMode = false;
	if(WIICRAFT_VERSION[0] == 'D' || WIICRAFT_VERSION[3] == 'D'){
		DevMode = true;
	}
	checkForUpdate(DevMode);
}

AutoUpdater::~AutoUpdater(){
	//Todo
}

bool AutoUpdater::checkForUpdate(bool dev){
	if(dev == true){
		//TODO
		
	}
	else{
		//TODO
	}
	
	return false;
}