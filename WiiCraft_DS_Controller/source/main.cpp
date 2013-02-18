/* Made By filfat */
#include <nds.h>
#include <stdio.h>

bool Connected = false;

void OnKeyPressed(int key) {
   if(key > 0)
      iprintf("%c", key);
}

int main(void)  {

   consoleDemoInit();

   Keyboard *kbd = 	keyboardDemoInit();

   kbd->OnKeyPressed = OnKeyPressed;

   while(Connected == false) {
      char wiiIP[256];

      iprintf("Type in your Wii's IP");

      scanf("%s", wiiIP);

      iprintf("\nConnecting To %s...", wiiIP);

	  //char IP1;
	  //char IP2;
	  //sscanf(wiiIP[3], "%c", &IP1);
	  //sscanf(wiiIP[7], "%c", &IP2); 
	  //if((IP1 == '.') && (IP2 == '.')){
	  //	Connected = true; //Will change later when i know more about network programming =)
	  //}
	  
      scanKeys();
      while(!keysDown())scanKeys();

      swiWaitForVBlank();
      consoleClear();
   }

   return 0;
}