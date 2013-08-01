#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <ogc/pad.h>
#include <ogc/lwp_watchdog.h>


#define getTime() (ticks_to_millisecs(gettime()))
#define DEFAULT_FIFO_SIZE	(256*1024)

//Variables
extern void *xfb[2];
extern GXRModeObj *rmode;
extern f32 yscale;
extern u32 xfbHeight;
extern u32 fb;
extern GXColor background;
extern void *gp_fifo;

extern Mtx44 projection;
extern Mtx view, model, modelview;
extern float fps;

extern u32 pressed; // GC, Wii Remote, Nuchuck, or Classic Controller input
extern u32 expansion_type;
extern struct joystick_t *js;
extern WPADData *wmote_data;

//Functions
void InitPad();
void UpdatePad();
u32 DetectInput(void);
void EndVideo();
void InitVideo();
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
