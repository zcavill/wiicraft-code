/* by xerpi */

#include "pngx.h"

void pngx_read_fn(png_structp png_ptr, png_bytep buffer, uint32_t bytesToRead)
{
	FILE *fp = (FILE *)png_get_io_ptr(png_ptr);
	if(fp == NULL)
		return;
	uint32_t bytesReaded = fread(buffer, 1, bytesToRead, fp);
	if(bytesReaded != bytesToRead)
		return;
}

void pngx_initTexture(pngx_texture *texp)
{
	texp->data = NULL;
	texp->rowPointers = NULL;
	texp->hasAlpha = 0;
}

int pngx_loadTexture(char *filename, pngx_texture *texp)
{
	uint8_t      header[PNGX_SIG_LEN];
	uint8_t      *dataBuffer;
	png_structp  png_ptr  = NULL;
	png_infop    info_ptr = NULL;
	unsigned int i;
	FILE         *fp;
	if(!(fp = fopen(filename, "rb")))
		goto exit_error;
	
	fread(header, 1, PNGX_SIG_LEN, fp);
	if (png_sig_cmp((png_const_bytep)header, 0, PNGX_SIG_LEN))
		goto exit_close;
		
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr == NULL)
		goto exit_close;
		
    info_ptr = png_create_info_struct(png_ptr);
    if(info_ptr == NULL)
		goto exit_destroy;
		
	if(setjmp(png_jmpbuf(png_ptr)))
		goto exit_destroy;
	
	pngx_freeTexture(texp);	
	
	png_set_read_fn(png_ptr, (void *)fp, (png_rw_ptr)pngx_read_fn);
	png_set_sig_bytes(png_ptr, PNGX_SIG_LEN);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &texp->width, &texp->height, &texp->bitDepth, &texp->colorType, NULL, NULL, NULL);
	
	switch(texp->colorType)
	{
		case PNG_COLOR_TYPE_RGB_ALPHA:
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			texp->hasAlpha = 1;
			break;
		default:
			texp->hasAlpha = 0;
			break;	
	}
	
	texp->rowBytes = png_get_rowbytes(png_ptr, info_ptr);
	texp->data = (uint8_t *)memalign(32, texp->rowBytes * texp->height);
	dataBuffer = (uint8_t *)memalign(32, texp->rowBytes * texp->height);
	texp->rowPointers = (png_bytep *)malloc(sizeof(png_bytep) * texp->height);
	
	DCInvalidateRange(texp->data, texp->rowBytes * texp->height);
	memset(texp->data, 0x0, texp->rowBytes * texp->height);
	
	for(i = 0; i < texp->height; i++)
		texp->rowPointers[(texp->height - 1- i)] = (png_bytep)(texp->data + i * texp->rowBytes);
		
	png_read_image(png_ptr, texp->rowPointers);
	
	if(texp->hasAlpha)
		pngx_RGBAto4X4RGBA(texp->data, dataBuffer, texp->width, texp->height);
	else
		pngx_RGBto4X4RGBA(texp->data, dataBuffer, texp->width, texp->height);
		
	memcpy(texp->data, dataBuffer, texp->rowBytes * texp->height);
	free(dataBuffer);

	DCFlushRange(texp->data, texp->rowBytes * texp->height);

	GX_InitTexObj(&texp->texObj, // objeto textura a inicializar
				 (void *)texp->data, // direccion de memoria de la textura
				 texp->width,// ancho
				 texp->height,// alto
				 GX_TF_RGBA8, // formato de color
				 GX_CLAMP, // Wrap Mode para S
				 GX_CLAMP, // Wrap Mode para T
				 GX_FALSE // No MipMap
	);
	
	GX_InitTexObjLOD(&texp->texObj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);	

	png_destroy_read_struct(&png_ptr, NULL, NULL);
	fclose(fp);
	return 1;
		
exit_destroy:
	png_destroy_read_struct(&png_ptr, NULL, NULL);
exit_close:
	fclose(fp);
exit_error:
	return 0;
}

void pngx_freeTexture(pngx_texture *texp)
{
	if(texp->data != NULL)
	{
		free(texp->data);
		texp->data = NULL;
	}
	if(texp->rowPointers != NULL)
	{
		free(texp->rowPointers);
		texp->rowPointers = NULL;
	}
}

void pngx_RGBto4X4RGBA(uint8_t *srcBuff, uint8_t *dstBuff, uint32_t buffWidth, uint32_t buffHeight)
{
	//RGB format: 0xRRGGBB
	
	uint8_t *src = srcBuff;
	uint8_t *dst = dstBuff;
	uint32_t y, x, i, j;
	uint8_t *pixel;
	
	for(y = 0;  y < buffHeight; y+=4)
	{
		for(x = 0; x < buffWidth; x+=4)
		{
			for(i = 0; i < 4; i++)
			{	
				for(j = 0; j < 4; j++)
				{	
					pixel = src + ((x + j) + (y + i) * buffWidth) * 3;
					*(dst     ) = 0xFF;          //Alpha
					*(dst + 32) = *(pixel + 1);  //Green
					dst++;
					*(dst     ) = *(pixel);      //Red
					*(dst + 32) = *(pixel + 2);  //Blue
					dst++;
				}
			}
			dst+=32;
		}	
	}	
}

void pngx_RGBAto4X4RGBA(uint8_t *srcBuff, uint8_t *dstBuff, uint32_t buffWidth, uint32_t buffHeight)
{
	//RGBA format: 0xRRGGBBAA
	
	/*
	AARRAARR AARRAARR AARRAARR AARRAARR
	AARRAARR AARRAARR AARRAARR AARRAARR
	GGBBGGBB GGBBGGBB GGBBGGBB GGBBGGBB
	GGBBGGBB GGBBGGBB GGBBGGBB GGBBGGBB
	*/
	uint8_t *src = srcBuff;
	uint8_t *dst = dstBuff;
	uint32_t y, x, i, j;
	uint8_t *pixel;
	
	for(y = 0;  y < buffHeight; y+=4)
	{
		for(x = 0; x < buffWidth; x+=4)
		{
			for(i = 0; i < 4; i++)
			{	
				for(j = 0; j < 4; j++)
				{	
					pixel = src + ((x + j) + (y + i) * buffWidth) * 4;
					*(dst     ) = *(pixel + 3);  //Alpha
					*(dst + 32) = *(pixel + 1);  //Green
					dst++;
					*(dst     ) = *(pixel);      //Red
					*(dst + 32) = *(pixel + 2);  //Blue
					dst++;
				}
			}
			dst+=32;
		}	
	}
}


void pngx_setTexture(pngx_texture *texp, uint8_t mapid)
{
	GX_LoadTexObj(&texp->texObj, mapid);
}

void pngx_drawTexture(pngx_texture *texp, float x, float y)
{
	if(texp->data == NULL) return;
	GX_Begin(GX_TRIANGLES, GX_VTXFMT0, 6);
	{
		GX_Position3f32(x, y + texp->height, 0.0f);
			GX_TexCoord2f32(0.0f, 0.0f);	
		GX_Position3f32(x, y, 0.0f);
			GX_TexCoord2f32(0.0f, 1.0f);	
		GX_Position3f32(x + texp->width, y, 0.0f);
			GX_TexCoord2f32(1.0f, 1.0f);
			
		GX_Position3f32(x + texp->width, y, 0.0f);
			GX_TexCoord2f32(1.0f, 1.0f);
		GX_Position3f32(x + texp->width, y + texp->height, 0.0f);
			GX_TexCoord2f32(1.0f, 0.0f);
		GX_Position3f32(x, y + texp->height, 0.0f);
			GX_TexCoord2f32(0.0f, 0.0f);	
	}
	GX_End();
}

