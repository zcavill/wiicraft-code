/*									

	(c)Filiph Sandström. ETC.
									
									*/
#ifndef _AUTO_UPDATER_H_
#define _AUTO_UPDATER_H_

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <cstdio>
#include <stdio.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <zlib.h>

#include "debug.h"
#include "main.h"

using namespace std;

class AutoUpdater {
	private:
		
		
	public:
		AutoUpdater();
		~AutoUpdater();
		bool checkForUpdate(bool dev);
		
};



#endif