/* by xerpi */

#ifndef _PNGX_H_
#define _PNGX_H_

#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <stdarg.h>
#include <malloc.h>
#include <png.h>


#define PNGX_SIG_LEN 8

typedef struct
{
	uint32_t width, height;
	uint8_t *data;
	int32_t rowBytes;
	png_bytep *rowPointers;
	int bitDepth, colorType;
	uint8_t hasAlpha;
	GXTexObj texObj;
}pngx_texture;

int  pngx_loadTexture(char *filename, pngx_texture *texp);
void pngx_freeTexture(pngx_texture *texp);
void pngx_initTexture(pngx_texture *texp);
void pngx_setTexture(pngx_texture *texp, uint8_t mapid);
void pngx_drawTexture(pngx_texture *texp, float x, float y);
void pngx_read_fn(png_structp png_ptr, png_bytep buffer, uint32_t bytesToRead);
void pngx_RGBAto4X4RGBA(uint8_t *srcBuff, uint8_t *dstBuff, uint32_t buffWidth, uint32_t buffHeight);
void pngx_RGBto4X4RGBA(uint8_t *srcBuff, uint8_t *dstBuff, uint32_t buffWidth, uint32_t buffHeight);


#endif
