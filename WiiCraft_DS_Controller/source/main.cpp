/* Made By filfat */
//Port: 8593
#include <nds.h>
#include <stdio.h>
#include <dswifi9.h>

#include <sys/types.h>
#include <netinet/in.h>

#include <sys/socket.h>
#include <netdb.h>
#include <GEMS_WifiDS.h>

bool Connected = false;

void OnKeyPressed(int key) {
   if(key > 0)
      iprintf("%c", key);
}

int main(void)  {

	struct in_addr ip, gateway, mask, dns1, dns2;
	
	// set the mode for 2 text layers and two extended background layers
	videoSetMode(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG_0x06000000);

	consoleDemoInit();
   
	PrintConsole topScreen;
	PrintConsole bottomScreen;
	
	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);

	consoleInit(&topScreen, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
	consoleInit(&bottomScreen, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
   
   consoleSelect(&bottomScreen);
   
   Keyboard *kbd = keyboardDemoInit();
   
   kbd->OnKeyPressed = OnKeyPressed;
   
   consoleSelect(&topScreen);
   
   iprintf("Connecting to WiFi...");
	
	if(!Wifi_InitDefault(WFC_CONNECT)){
		iprintf("Failed to connect!");
		exit(1);
	}
		
		iprintf("Connected\n\n");
		
		ip = Wifi_GetIPInfo(&gateway, &mask, &dns1, &dns2);

		while(Connected == false) {
			char wiiIP[256];

			iprintf("Type in your Wii's IP:\n");

			scanf("%s", wiiIP);

			iprintf("\nConnecting To %s\nfrom %s...", wiiIP,inet_ntoa(ip));
			
			int host = 0; host = TCP_ClientConnect(wiiIP, 8593);//TCP_ClientConnect(ip address, port)

		//char IP1;
		//char IP2;
		//sscanf(wiiIP[3], "%c", &IP1);
		//sscanf(wiiIP[7], "%c", &IP2); 
		//if((IP1 == '.') && (IP2 == '.')){
		//	Connected = true; //Will change later when i know more about network programming =)
		//}
			iprintf("Press start to exit or click any other button to try again:\n");
			
			scanKeys();
			while(!keysDown()){
				scanKeys();
				uint16 keysPressed = ~(REG_KEYINPUT);
				if(keysPressed & KEY_START)
					exit(0);
			}

			swiWaitForVBlank();
			consoleClear();
	}

   return 0;
}