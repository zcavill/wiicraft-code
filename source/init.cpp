// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 3.0 for more details.

// Copyright (C) 2012-2013	filfat, xerpi, JoostinOnline

#include <stdio.h>
#include <stdlib.h>
#include <fat.h>
#include <asndlib.h>
#include <mp3player.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <grrlib.h>
#include "utils.h"
#include "debug.h"

void Initialize(void){
	fatInitDefault();	
	//Debug:
	#ifdef USBGECKO
	DebugStart(true, "sd://wiicraft.log");
	Debug("-------------------[Wiicraft Debug]----------------------");
	#endif
	InitVideo();
	#ifdef USBGECKO
	Debug("InitVideo() Done");
	#endif
	InitPad();
	#ifdef USBGECKO
	Debug("InitPad() Done");
	#endif
	initFPS();
	#ifdef USBGECKO
	Debug("InitFPS() Done");
	#endif
	ASND_Init();
	#ifdef USBGECKO
	Debug("ASND_Init() Done");
	#endif
	MP3Player_Init();
	#ifdef USBGECKO
	Debug("MP3Player_Init Done");
	Debug("Done With DebugStart\n");
	#endif

}

void Deinitialize(void){
	WPAD_Shutdown();
	#ifdef USBGECKO
	Debug("WPAD_Shutdown Passed");
	#endif
	
	EndVideo();
	#ifdef USBGECKO
	Debug("EndVideo Passed");
	#endif
	
	ASND_End();
	#ifdef USBGECKO
	Debug("ASND_End Passed");
	#endif

	MP3Player_Stop();
	#ifdef USBGECKO
	Debug("MP3Player_Stop Passed");
	#endif

	#ifdef USBGECKO
	DebugStop();
	gprintf("DebugStop passed\n");
	#endif
	}