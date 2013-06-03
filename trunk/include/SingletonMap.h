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
#include <sstream>
#include <sys/stat.h>
#include <vector>

#include "map.h"
#include "utils.h"

class SingletonMap{
public:
   static SingletonMap* Instance();
   MAP mainMAP;

private:
  SingletonMap(){};  // Private so that it can  not be called
  SingletonMap(SingletonMap const&){};             // copy constructor is private
  SingletonMap& operator=(SingletonMap const&){};  // assignment operator is private
  static SingletonMap* m_pInstance;
};

