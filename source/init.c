#include <stdio.h>
#include <stdlib.h>
#include <asndlib.h>
#include <mp3player.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <grrlib.h>

#include "debug.h"

void Initialize(void){
	//Debug:
	#ifdef USBGECKO
	GRRLIB_GeckoInit();
	DebugStart(true, "sd://wiicraft.log");
	Debug("-------------------[Wiicraft Debug]----------------------");
	gprintf("Done With init()\n");
	#endif

	ASND_Init();
	MP3Player_Init();

}

void Deinitialize(void)
	{
	//GRRLIB_Exit ();
	//#ifdef USBGECKO
	//Debug("GRRLIB_Exit Passed");
	//#endif

	WPAD_Shutdown();

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