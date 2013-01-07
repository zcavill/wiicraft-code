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


using namespace std;

API::API(){
	APIRunning = false;
	APIVERISONBool = false;
}

API::~API(){
	//Todo
}

void API::initAPI(int usingSD) {
	//initFolders(usingSD);
	
	if(usingSD == 1){
		ConfigFile cfg("sd:/APPS/Minecraft_Wii_Edition/config.cfg");
		
		existsAPI = cfg.keyExists("API");
		bool existsADDBLOCKS = cfg.keyExists("ADDBLOCKS");
		string ADDBLOCKSPATH = cfg.getValueOfKey<string>("ADDBLOCKSPATH");
		existsTEXTURES = cfg.keyExists("TEXTURES");
		string APIver = cfg.getValueOfKey<string>("API");
		
		//=================================
		if(existsADDBLOCKS){
		string path;
			if (usingSD) path = "sd:/";
			else path = "usb:/";
		path += ADDBLOCKSPATH;
		ConfigFile blocksTooAdd(path);
			
			float x = blocksTooAdd.getValueOfKey<float>("X");
			float y = blocksTooAdd.getValueOfKey<float>("Y");
			float z = blocksTooAdd.getValueOfKey<float>("Z");
			
			registerBlockToAddOnScreen(x,y,z);
			
		}
		//=================================
		
	} else if(usingSD == 2) {
		ConfigFile cfg("usb:/APPS/Minecraft_Wii_Edition/config.cfg");
		
		existsAPI = cfg.keyExists("API");
		existsTEXTURES = cfg.keyExists("TEXTURES");
		string APIver = cfg.getValueOfKey<string>("API");
	} else if(usingSD == 3) {
		ConfigFile cfg("sd:/APPS/Minecraft_Wii_Edition/config.cfg");
		
		existsAPI = cfg.keyExists("API");
		existsTEXTURES = cfg.keyExists("TEXTURES");
		string APIver = cfg.getValueOfKey<string>("API");
	}
	
	
	//existsAPI = cfg.keyExists("API");
	//existsTEXTURES = cfg.keyExists("TEXTURES");
	//string someValue = cfg.getValueOfKey<string>("mykey", "Unknown");
	//string APIver = cfg.getValueOfKey<string>("API");
	
}

void API::stopAPI(){
	APIRunning = false;
}