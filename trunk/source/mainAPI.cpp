/*

	(c)Mojang AB And Filiph Sandström

									*/

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

#include "mainAPI.h"
#include "Convert.h"
#include "ConfigFile.h"
#include "setUp.h"
#include "utils.h"
#include "main.h"


using namespace std;

API::API(){
	APIRunning = false;
	APIVERISONBool = false;
}

API::~API(){
	//Todo
}

void API::initAPI(void) {
	
	//int folders = initFolders(usingSD);
	
	if(fatDevice == FAT_DEVICE_SD){
		ConfigFile cfg("sd:/APPS/WiiCraft/config.cfg");
		
		existsAPI = cfg.keyExists("API");
		existsLUA = cfg.keyExists("LUA");
		LUAPATH = cfg.getValueOfKey<string>("LUAPATH");
		existsTEXTURES = cfg.keyExists("TEXTURES");
		
	} else if(fatDevice == FAT_DEVICE_USB) {
		ConfigFile cfg("usb:/APPS/WiiCraft/config.cfg");
		
		existsAPI = cfg.keyExists("API");
		existsLUA = cfg.keyExists("LUA");
		LUAPATH = cfg.getValueOfKey<string>("LUAPATH");
		existsTEXTURES = cfg.keyExists("TEXTURES");
		
	} else {
		//ConfigFile cfg("sd:/APPS/WiiCraft/config.cfg");
		
		existsAPI = true;			//cfg.keyExists("API");
		existsLUA = true;  		 	//cfg.keyExists("LUA");
		LUAPATH = "APPS/Wiicraft";  //cfg.getValueOfKey<string>("LUAPATH");
		existsTEXTURES = false;		//cfg.keyExists("TEXTURES");
	}
	
		//=================================
		if(existsLUA){
			string path;
			if (fatDevice == FAT_DEVICE_SD)
				path = "sd:/";
			else
				path = "usb:/";
			path += LUAPATH;
			
		}
		//=================================
	
	//existsAPI = cfg.keyExists("API");
	//existsTEXTURES = cfg.keyExists("TEXTURES");
	//string someValue = cfg.getValueOfKey<string>("mykey", "Unknown");
	//string APIver = cfg.getValueOfKey<string>("API");
	
}

void API::stopAPI(){
	APIRunning = false;
}