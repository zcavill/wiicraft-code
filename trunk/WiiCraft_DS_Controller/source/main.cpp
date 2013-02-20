/* Made By filfat */
//Port: 8593
#include <nds.h>
#include <stdio.h>
#include <dswifi9.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>

#include <sys/socket.h>
#include <netdb.h>
#include <GEMS_WifiDS.h>

bool Connected = false;
bool debug = false;
bool CMDbool = false;
unsigned int clientForServer;
const unsigned int VERSION = 1;
char HELP[] = "/help or /?: shows this\n/version or ver: show the version of WiiCraft DS controller\n";

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
		
		uint16 keysPressed = ~(REG_KEYINPUT);
			if(keysPressed & KEY_A){
				debug = true;
			}
		
		ip = Wifi_GetIPInfo(&gateway, &mask, &dns1, &dns2);

		while(Connected == false && debug == false) {
			char wiiIP[256];

			iprintf("Your ip: %s\n",inet_ntoa(ip));
			iprintf("Type in your Wii's IP:\n");/* or /cmd for the console*/ // does not work at the moment =)

			scanf("%s", wiiIP);

			if((wiiIP[0] == 47) && (wiiIP[1] == 99) && (wiiIP[2] == 109) && (wiiIP[3] == 100)){
				CMDbool = true;
				break;
			}	

			iprintf("\nConnecting To %s\nfrom %s...", wiiIP,inet_ntoa(ip));
			
			int server = 0; server = TCP_ClientConnect(wiiIP, 8593);//TCP_ClientConnect(ip address, port)

			if(server == true){
				Connected = true;
				iprintf("Connected\n");
			}

			while(Connected == true){
				
			}
			
			iprintf("Press start to exit or click any other button to try again:\n");

			scanKeys();
			while(!keysDown()){
				scanKeys();
				if(keysPressed & KEY_START)
					exit(0);
			}

			swiWaitForVBlank();
			consoleClear();
	}
	
	if(CMDbool == true){
		char CMD[256];
		while(!CMD == "/stop"){
			scanf("%s", CMD);
			if(CMD == "/server"){
				debug = true;
				break;
			}
			else if((CMD == "/version") || (CMD == "/ver")){
				iprintf("Version: %s\n", VERSION);
			}
			else if((CMD == "/help") || (CMD == "/?")){
				iprintf("%s", HELP);
			}
		}
		
	}
	
	int host = 0;
	if(debug == true){
		iprintf("Your ip: %s\n",inet_ntoa(ip));
		host = TCP_Server(8593, 4);//TCP_Server(port, number of players)
	}
	
	while(debug == true){
		
		int client = 0; client = TCP_GetClient(host);//TCP_GetClient(host socket)
		iprintf("Client connected.\n");
		clientForServer++;
		
	}

   return 0;
}