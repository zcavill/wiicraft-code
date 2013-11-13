#include "menu.h"
#include "libwiigui/gui.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <ogc/pad.h>
#include <ogc/lwp_watchdog.h>
#include <iostream>

using namespace std;

int Menu_Function(){
	GuiWindow menuwindow;
    GuiImage menuimage;
    menuwindow.Append(&menuimage);
	return 0;
}