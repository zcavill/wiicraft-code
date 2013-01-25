#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <ogc/lwp_watchdog.h>


#define getTime() (ticks_to_millisecs(gettime()))
#define DEFAULT_FIFO_SIZE	(256*1024)


//STUFF
	void InitPad();
	void UpdatePad();
	void EndVideo();
	void InitVideo();
	void Clean();
	void SwapBuffer();
	
	void DrawFront(float x, float y, float z);
	void DrawBottom(float x, float y, float z);
	void DrawTop(float x, float y, float z);
	void DrawRight(float x, float y, float z);
	
	void DrawCube(float x, float y, float z);
	void DrawCubeTex(float x, float y, float z);
//FPS
    void initFPS();
    void FPS(float *fps_var);


#endif
