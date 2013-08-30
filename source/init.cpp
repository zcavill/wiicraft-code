#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fat.h>
#include <asndlib.h>
#include <mp3player.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <grrlib.h>
#include "utils.h"
#include "debug.h"

const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://www.cplusplus.com/reference/clibrary/ctime/strftime/
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void Initialize(void){
	std::string welcome[255] = {currentDateTime()};
	fatInitDefault();	
	#ifdef USBGECKO
	DebugStart(true, "sd://wiicraft.log");
	Debug("-------------------[Wiicraft Debug %s]----------------------", welcome);
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
	Debug("\n");
	Debug("\n");
	Debug("\n");
	DebugStop();
	gprintf("DebugStop passed\n");
	#endif
	}