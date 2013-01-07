#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include "Wiicraft.h"
#include "Math_utils.h"




class Block
{
public:
//Constructor
	Block();
	uint8_t ID;
	bool transparent;
};



#endif

