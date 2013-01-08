#include "utils.h"

void *xfb[2] = {NULL, NULL};
GXRModeObj *rmode;
f32 yscale;
u32 xfbHeight;
u32	fb = 0;
GXColor background = {0, 0, 0, 0xff};

Mtx44 projection;
Mtx view, model, modelview;

float fps = 0.0f;
u32 pressed;
u32 expansion_type;
struct joystick_t *js;
WPADData *wmote_data;


void InitPad()
{
	WPAD_Init();
	WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);
	WPAD_SetVRes(WPAD_CHAN_ALL, rmode->fbWidth, rmode->xfbHeight);
	WPAD_SetIdleTimeout(60 * 1.5); // Remote will turn off after 1.5 minutes of inactivity.
	WPAD_Probe(0, &expansion_type);
}

void UpdatePad()
{
	WPAD_ScanPads();
	WPAD_Probe(0, &expansion_type);
	wmote_data = WPAD_Data(0);
	js = &wmote_data->exp.nunchuk.js;
	pressed = WPAD_ButtonsHeld(0);
}



void InitVideo()
{
// init the vi.
	VIDEO_Init();
	WPAD_Init();

	rmode = VIDEO_GetPreferredMode(NULL);

	// allocate 2 framebuffers for double buffering
	xfb[0] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	xfb[1] = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb[fb]);
	VIDEO_SetBlack(FALSE);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	// setup the fifo and then init the flipper
	void *gp_fifo = NULL;
	gp_fifo = memalign(32, DEFAULT_FIFO_SIZE);
	memset(gp_fifo, 0, DEFAULT_FIFO_SIZE);

	GX_Init(gp_fifo,DEFAULT_FIFO_SIZE);

	// clears the bg to color and clears the z buffer
	GX_SetCopyClear(background, 0x00ffffff);

	// other gx setup
	GX_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
	yscale = GX_GetYScaleFactor(rmode->efbHeight,rmode->xfbHeight);
	xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,rmode->fbWidth,rmode->efbHeight);
	GX_SetDispCopySrc(0,0,rmode->fbWidth,rmode->efbHeight);
	GX_SetDispCopyDst(rmode->fbWidth,xfbHeight);
	GX_SetCopyFilter(rmode->aa,rmode->sample_pattern,GX_TRUE,rmode->vfilter);
	GX_SetFieldMode(rmode->field_rendering,((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));

    if (rmode->aa)
		GX_SetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
	else
		GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);

	GX_SetDispCopyGamma(GX_GM_1_0);


	// setup the vertex descriptor
	// tells the flipper to expect direct data
	GX_InvVtxCache();
	GX_ClearVtxDesc();
    GX_SetVtxDesc(GX_VA_POS, GX_DIRECT);
	GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT);
	GX_SetVtxDesc(GX_VA_TEX0, GX_NONE);

	GX_SetNumChans(1);

	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GX_SetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);

	GX_SetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_DISABLE, GX_COLOR0A0);
	GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);

	GX_SetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
	GX_SetAlphaUpdate(GX_TRUE);

	GX_SetCullMode(GX_CULL_NONE);
	GX_SetColorUpdate(GX_TRUE);


    f32 w = rmode->viWidth;
    f32 h = rmode->viHeight;
	guPerspective(projection, 45, (f32)w/(f32)h, 0.1F, 300.0F);
	GX_LoadProjectionMtx(projection, GX_PERSPECTIVE);


	GX_SetLineWidth(25, GX_TO_ZERO);

	guMtxIdentity(model);

	 GX_InvalidateTexAll();

	//console
		console_init(xfb[0],20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

}


void Clean()
{
    //GX_SetViewport(0, 0, rmode->fbWidth, rmode->efbHeight, 0, 1);
	printf("\x1b[%d;%dH", 2, 0);
}

void SwapBuffer()
{
    GX_DrawDone();

    fb ^= 1;		// flip framebuffer
    GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GX_SetColorUpdate(GX_TRUE);
    GX_CopyDisp(xfb[fb],GX_TRUE);

    VIDEO_SetNextFramebuffer(xfb[fb]);
    VIDEO_Flush();
    VIDEO_WaitVSync();
}


void DrawCube(float x, float y, float z)  //THIS IS A PROVISIONAL SHITTY FUNCTION, JUST TESTING
{

	GX_Begin(GX_QUADS, GX_VTXFMT0, 24);			// Draw a Cube

	GX_Position3f32( x + 1.0f, y+1.0f,-1.0f + z);	// Top Left of the quad (top)
	GX_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green
	GX_Position3f32(x + -1.0f, y+1.0f,-1.0f + z);	// Top Right of the quad (top)
	GX_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green
	GX_Position3f32(x + -1.0f, y+1.0f, 1.0f + z);	// Bottom Right of the quad (top)
	GX_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green
	GX_Position3f32(x +  1.0f, y+1.0f, 1.0f + z);		// Bottom Left of the quad (top)
	GX_Color3f32(0.0f,1.0f,0.0f);			// Set The Color To Green

	GX_Position3f32(x +  1.0f,y+-1.0f, 1.0f + z);	// Top Left of the quad (bottom)
	GX_Color3f32(1.0f,0.5f,0.0f);			// Set The Color To Orange
	GX_Position3f32(x + -1.0f,y+-1.0f, 1.0f + z);	// Top Right of the quad (bottom)
	GX_Color3f32(1.0f,0.5f,0.0f);			// Set The Color To Orange
	GX_Position3f32(x + -1.0f,y+-1.0f,-1.0f + z);	// Bottom Right of the quad (bottom)
	GX_Color3f32(1.0f,0.5f,0.0f);			// Set The Color To Orange
	GX_Position3f32(x +  1.0f,y+-1.0f,-1.0f + z);	// Bottom Left of the quad (bottom)
	GX_Color3f32(1.0f,0.5f,0.0f);			// Set The Color To Orange

	GX_Position3f32( x + 1.0f, y+1.0f, 1.0f + z);		// Top Right Of The Quad (Front)
	GX_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red
	GX_Position3f32(x + -1.0f, y+1.0f, 1.0f + z);	// Top Left Of The Quad (Front)
	GX_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red
	GX_Position3f32(x + -1.0f,y+-1.0f, 1.0f + z);	// Bottom Left Of The Quad (Front)
	GX_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red
	GX_Position3f32(x +  1.0f,y+-1.0f, 1.0f + z);	// Bottom Right Of The Quad (Front)
	GX_Color3f32(1.0f,0.0f,0.0f);			// Set The Color To Red

	GX_Position3f32(x + 1.0f,y+-1.0f,-1.0 + z);	// Bottom Left Of The Quad (Back)
	GX_Color3f32(1.0f,1.0f,0.0f);			// Set The Color To Yellow
	GX_Position3f32(x +-1.0f,y+-1.0f,-1.0f + z);	// Bottom Right Of The Quad (Back)
	GX_Color3f32(1.0f,1.0f,0.0f);			// Set The Color To Yellow
	GX_Position3f32(x +-1.0f, y+1.0f,-1.0f + z);	// Top Right Of The Quad (Back)
	GX_Color3f32(1.0f,1.0f,0.0f);			// Set The Color To Yellow
	GX_Position3f32(x + 1.0f,y+ 1.0f,-1.0f + z);	// Top Left Of The Quad (Back)
	GX_Color3f32(1.0f,1.0f,0.0f);			// Set The Color To Yellow

	GX_Position3f32(x + -1.0f, y+1.0f, 1.0f + z);	// Top Right Of The Quad (Left)
	GX_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue
	GX_Position3f32(x + -1.0f,y+ 1.0f,-1.0f + z);	// Top Left Of The Quad (Left)
	GX_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue
	GX_Position3f32(x + -1.0f,y+-1.0f,-1.0f + z);	// Bottom Left Of The Quad (Left)
	GX_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue
	GX_Position3f32(x + -1.0f,y+-1.0f, 1.0f + z);	// Bottom Right Of The Quad (Left)
	GX_Color3f32(0.0f,0.0f,1.0f);			// Set The Color To Blue

	GX_Position3f32(x +  1.0f, y+1.0f,-1.0f + z);	// Top Right Of The Quad (Right)
	GX_Color3f32(1.0f,0.0f,1.0f);			// Set The Color To Violet
	GX_Position3f32(x +  1.0f,y+ 1.0f, 1.0f + z);		// Top Left Of The Quad (Right)
	GX_Color3f32(1.0f,0.0f,1.0f);			// Set The Color To Violet
	GX_Position3f32(x +  1.0f,y+-1.0f, 1.0f + z);	// Bottom Left Of The Quad (Right)
	GX_Color3f32(1.0f,0.0f,1.0f);			// Set The Color To Violet
	GX_Position3f32(x +  1.0f,y+-1.0f,-1.0f + z);	// Bottom Right Of The Quad (Right)
	GX_Color3f32(1.0f,0.0f,1.0f);			// Set The Color To Violet

	GX_End();									// Done Drawing The Quad
}

void DrawCubeWire(int chunk_x, int chunk_y, int chunk_z, int block_x, int block_y, int block_z)
{
	int x = chunk_x * CHUNK_TOTAL_SIZE + block_x * BLOCK_SIZE;
	int y = chunk_y * CHUNK_TOTAL_SIZE + block_y * BLOCK_SIZE;
	int z = chunk_z * CHUNK_TOTAL_SIZE + block_z * BLOCK_SIZE;
	#define wire_size BLOCK_SIZE + 0.025

	GX_Begin(GX_LINESTRIP, GX_VTXFMT0, 30);

		GX_Position3f32(x, y + wire_size, z); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x + wire_size, y + wire_size, z); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x + wire_size, y + wire_size, z + wire_size); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x, y + wire_size, z + wire_size); GX_Color4u8(255, 255, 255, 255);
				GX_Position3f32(x, y + wire_size, z); GX_Color4u8(255, 255, 255, 255);

		GX_Position3f32(x, y, z); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x + wire_size, y, z); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x + wire_size, y, z + wire_size); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x, y, z + wire_size); GX_Color4u8(255, 255, 255, 255);
				GX_Position3f32(x, y, z); GX_Color4u8(255, 255, 255, 255);

		GX_Position3f32(x + wire_size, y, z); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x + wire_size, y, z + wire_size); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x + wire_size, y + wire_size, z + wire_size); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x + wire_size, y + wire_size, z); GX_Color4u8(255, 255, 255, 255);
				GX_Position3f32(x + wire_size, y, z); GX_Color4u8(255, 255, 255, 255);

		GX_Position3f32(x, y, z); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x, y, z + wire_size); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x, y + wire_size, z + wire_size); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x, y + wire_size, z); GX_Color4u8(255, 255, 255, 255);
				GX_Position3f32(x, y, z); GX_Color4u8(255, 255, 255, 255);

		GX_Position3f32(x, y, z + wire_size); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x + wire_size, y, z + wire_size); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x + wire_size, y + wire_size, z + wire_size); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x, y + wire_size, z + wire_size); GX_Color4u8(255, 255, 255, 255);
				GX_Position3f32(x, y, z + wire_size); GX_Color4u8(255, 255, 255, 255);

		GX_Position3f32(x, y, z); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x + wire_size, y, z); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x + wire_size, y + wire_size, z); GX_Color4u8(255, 255, 255, 255);
		GX_Position3f32(x, y + wire_size, z); GX_Color4u8(255, 255, 255, 255);
				GX_Position3f32(x, y, z); GX_Color4u8(255, 255, 255, 255);

	GX_End();
}



//FPS
    unsigned long lastTime, currentTime, diffTime;
    unsigned int frame_count;

    void initFPS()
    {
        currentTime = getTime();
        lastTime = currentTime;
        frame_count = 0;
    }

    void FPS(float *fps_var)
    {
        frame_count++;
        currentTime = getTime();
        diffTime = currentTime - lastTime;
        if(diffTime >= 1000)
        {
            *fps_var = (float)frame_count/(diffTime/1000.0f);
            frame_count = 0;
            lastTime = getTime();
        }
    }

