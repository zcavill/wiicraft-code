#ifndef _SICKSAXIS_H_
#define _SICKSAXIS_H_

#include <gccore.h>
#include <wiiuse/wpad.h>
#include <gcutil.h>
#include <ogcsys.h>
#include <ogc/ipc.h>
#include <ogc/usb.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>

#ifdef SS_DEBUG
	#define ss_debug_printf printf
#else
	#define ss_debug_printf(...) ((void)0)
#endif

#define swap16(s) (s = (s >> 8) | (s << 8))

#define SS_DATA_LEN        53 //Should be 49
#define SS_LEDS_RUMBLE_LEN 32
#define SS_VENDOR_ID       0x054C //Sony Corp.
#define SS_PRODUCT_ID      0x0268 //Sixaxis and DS3
#define	SS_HEAP_SIZE       4096
#define SS_DEV_MAX 7

/*struct SS_BUTTONS    //Little endian
{
    uint8_t select   : 1;
    uint8_t L3       : 1;
    uint8_t R3       : 1;
    uint8_t start    : 1;
    uint8_t up       : 1;
    uint8_t right    : 1;
    uint8_t down     : 1;
    uint8_t left     : 1;
    uint8_t L2       : 1;
    uint8_t R2       : 1;
    uint8_t L1       : 1;
    uint8_t R1       : 1;
    uint8_t triangle : 1;
    uint8_t circle   : 1;
    uint8_t cross    : 1;
    uint8_t square   : 1;
    uint8_t PS       : 1;
    uint8_t not_used : 7;
};*/

struct SS_BUTTONS    //Big endian
{
	uint8_t left     : 1;
	uint8_t down     : 1;
	uint8_t right    : 1;
	uint8_t up       : 1;
	uint8_t start    : 1;
	uint8_t R3       : 1;
	uint8_t L3       : 1;
	uint8_t select   : 1;
	
	uint8_t square   : 1;
	uint8_t cross    : 1;
	uint8_t circle   : 1;
	uint8_t triangle : 1;
	uint8_t R1       : 1;
	uint8_t L1       : 1;
	uint8_t R2       : 1;
	uint8_t L2       : 1;
	 
    uint8_t not_used : 7;
    uint8_t PS       : 1;
};

struct SS_ANALOG
{
    uint8_t x;
    uint8_t y;
};

struct SS_DPAD_SENSITIVE
{
    uint8_t up;
    uint8_t right;
    uint8_t down;
    uint8_t left;
};

struct SS_SHOULDER_SENSITIVE
{
    uint8_t L2;
    uint8_t R2;
    uint8_t L1;
    uint8_t R1;
};

struct SS_BUTTON_SENSITIVE
{
    uint8_t triangle;
    uint8_t circle;
    uint8_t cross;
    uint8_t square;
};

struct SS_MOTION
{
    uint16_t accX;
    uint16_t accY;
    uint16_t accZ;
    uint16_t Zgyro;
};

struct SS_GAMEPAD
{
    uint8_t                        HIDdata;
    uint8_t                        unk0;
    struct SS_BUTTONS              buttons;
    uint8_t                        unk1;
    struct SS_ANALOG               leftAnalog;
    struct SS_ANALOG               rightAnalog;
    uint32_t                       unk2;
    struct SS_DPAD_SENSITIVE       dpad_sens;
    struct SS_SHOULDER_SENSITIVE   shoulder_sens;
    struct SS_BUTTON_SENSITIVE     button_sens;
    uint16_t                       unk3;
    uint8_t                        unk4;
    uint8_t                        status;
    uint8_t                        power_rating;
    uint8_t                        comm_status;
    uint32_t                       unk5;
    uint32_t                       unk6;
    uint8_t                        unk7;
    struct SS_MOTION               motion;
    uint8_t                        padding[3];
}__attribute__((packed));


struct SickSaxis
{
    struct SS_GAMEPAD __attribute__ ((aligned(32))) gamepad;
    uint8_t __attribute__ ((aligned(32))) leds_rumble[SS_LEDS_RUMBLE_LEN];  
	u8 connected;
	
    s32 fd, dev_id;
};


//Functions
	int  ss_init();
	int  ss_finish();
    int  ss_open(struct SickSaxis *sicksaxis);
    int  ss_close(struct SickSaxis *sicksaxis);
    int  ss_read(struct SickSaxis *sicksaxis);
    int  ss_get_mac(struct SickSaxis *sicksaxis, uint8_t *mac);
    int  ss_set_mac(struct SickSaxis *sicksaxis, uint8_t *mac);
    void ss_set_led(struct SickSaxis *sicksaxis, uint8_t led);
    void ss_set_rumble(struct SickSaxis *sicksaxis, uint8_t duration_right, uint8_t power_right, uint8_t duration_left, uint8_t power_left);
    int  ss_set_operational_mode(struct SickSaxis *sicksaxis);
	int  ss_is_connected(struct SickSaxis *sicksaxis);
	s32  ss_disconnect_cb(s32 result,void *usrdata);


#endif
