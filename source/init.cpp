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