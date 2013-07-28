#ifndef _STFDEBUG_
	#define _STFDEBUG_

	#if defined(HW_RVL)
	
	//Comment next two lines to speed up the code:
	#define USBGECKO
	
	#ifdef __cplusplus
	extern "C" {
	#endif
	/* __cplusplus */

	s32 DebugStart (bool gecko, const char *fn);
	void DebugStop (void);
	void Debug (const char *text, ...);
	void Debug_hexdump(void *d, int len);
	void gprintf (const char *format, ...);

	#ifdef __cplusplus
	}
	#endif /* __cplusplus */

	#endif /* defined(HW_RVL) */

#endif