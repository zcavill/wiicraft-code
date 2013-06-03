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
#include "SingletonMap.h"

// Global static pointer used to ensure a single instance of the class.
SingletonMap* SingletonMap::m_pInstance = NULL;  
  
SingletonMap* SingletonMap::Instance(){
   if (!m_pInstance)   // Only allow one instance of class to be generated.
      m_pInstance = new SingletonMap;

   return m_pInstance;
}
//SingletonMap::Instance()->