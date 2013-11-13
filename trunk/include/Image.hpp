// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 3.0 for more details.

// Copyright (C) 2012-2013      filfat, xerpi, JoostinOnline

#ifndef _IMAGE_HPP_
#define _IMAGE_HPP_

#include <gccore.h>
#include <png.h>
#include "../PNGU/pngu.h"
#include <malloc.h>

class Image
{
public:
//Constructor
        //From file
                Image(char *filename) {loadFromFile(filename);}
        //From buffer
                Image(uint8_t *buf_data)  {loadFromBuffer(buf_data);}
//Destructor
        ~Image();
//Methods
        //Clears the image
                void clear();
        //Loads from file
                int loadFromFile(char *filename);
        //Loads from buffer
                int loadFromBuffer(uint8_t *buf_data);
        //Sets to the GX
                void setGX(int texmap) {GX_LoadTexObj(&texObj, texmap);}
        //Gets image width
                int getWidth() {return width;}
        //Gets image height
                int getHeight() {return height;}
//Variables
        uint8_t  *data;
        uint32_t width, height;
        GXTexObj texObj;
};


#endif
