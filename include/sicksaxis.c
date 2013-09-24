#include "sicksaxis.h"
#include <gccore.h>
#include <ogc/usb.h>


/*
 * the total time the led is active (0xff means forever)
 * |     duty_length: how long a cycle is in deciseconds:
 * |     |                              (0 means "blink very fast")
 * |     |     ??? (Maybe a phase shift or duty_length multiplier?)
 * |     |     |     % of duty_length led is off (0xff means 100%)
 * |     |     |     |     % of duty_length led is on (0xff is 100%)
 * |     |     |     |     |
 * 0xff, 0x27, 0x10, 0x00, 0x32,
 */
static uint8_t leds_rumble_payload[SS_LEDS_RUMBLE_LEN] =
{
	0x52, //0x52
	0x01,
	0x00, 0x00, 0x00, //Rumble
	0xff, 0x80, //Gyro
	0x00, 0x00,
	0x00, //* LED_1 = 0x02, LED_2 = 0x04, ... */
    0xff, 0x27, 0x10, 0x00, 0x32, /* LED_4 */
    0xff, 0x27, 0x10, 0x00, 0x32, /* LED_3 */
    0xff, 0x27, 0x10, 0x00, 0x32, /* LED_2 */
    0xff, 0x27, 0x10, 0x00, 0x32, /* LED_1 */
};

static uint8_t ss_led_pattern[8] = {0x0, 0x02, 0x04, 0x08, 0x10, 0x12, 0x14, 0x18};

static s32  ss_hId = -1;
static s32 ss_dev_id_list[SS_DEV_MAX];

//Functions

static int ss_dev_id_list_add(s32 id)
{
	int i;
	for(i = 0; i < SS_DEV_MAX; ++i)
	{
		if(ss_dev_id_list[i] == 0)
		{
			ss_dev_id_list[i] = id;
			return 0;
		}
	}
	return -1;
}

static int ss_dev_id_list_del(s32 id)
{
	int i;
	for(i = 0; i < SS_DEV_MAX; ++i)
	{
		if(ss_dev_id_list[i] == id)
		{
			ss_dev_id_list[i] = 0;
			return 0;
		}
	}
	return -1;
}

static int ss_dev_id_list_search(s32 id)
{
	int i;
	for(i = 0; i < SS_DEV_MAX; ++i)
	{
		if(ss_dev_id_list[i] == id)
		{
			return 1;
		}
	}
	return 0;
}


void ss_set_led(struct SickSaxis *sicksaxis, uint8_t led)
{
    sicksaxis->leds_rumble[9] = ss_led_pattern[led];
  	USB_WriteCtrlMsg(sicksaxis->fd,
					USB_REQTYPE_INTERFACE_SET,
					USB_REQ_SETREPORT,
					(USB_REPTYPE_OUTPUT<<8) | 0x01,
					0x0,
					SS_LEDS_RUMBLE_LEN,
					sicksaxis->leds_rumble); 
					                        
}

void ss_set_rumble(struct SickSaxis *sicksaxis, uint8_t duration_right, uint8_t power_right, uint8_t duration_left, uint8_t power_left)
{
    sicksaxis->leds_rumble[1] = duration_right;
    sicksaxis->leds_rumble[2] = power_right;
    sicksaxis->leds_rumble[3] = duration_left;
    sicksaxis->leds_rumble[4] = power_left;
  	USB_WriteCtrlMsg(sicksaxis->fd,
					USB_REQTYPE_INTERFACE_SET,
					USB_REQ_SETREPORT,
					(USB_REPTYPE_OUTPUT<<8) | 0x01,
					0x0,
					SS_LEDS_RUMBLE_LEN,
					sicksaxis->leds_rumble); 

}


int ss_init()
{
	if(ss_hId > 0) //Already initialized
		return 0;
	
	ss_hId = iosCreateHeap(SS_HEAP_SIZE);
	int i;
	for(i = 0; i < SS_DEV_MAX; ++i)
		ss_dev_id_list[i] = 0;
	
	ss_debug_printf("SickSaxis lib initialized. Heap created. id: %i length: %i\n", ss_hId, SS_HEAP_SIZE);
	
	if(ss_hId < 0)
		return -1;
	
	return 0;
}

int ss_finish()
{
	if(ss_hId < 0)
		return -1;
		
	//Destroy heap ¿?
	return 0;
}
	
int ss_open(struct SickSaxis *sicksaxis)
{			
	ss_debug_printf("\x1b[%d;%dH\n", 2, 1);
	if(ss_hId < 0)
	{
		ss_debug_printf("Heap not inited.\n");
		return -2;
	}
	if(sicksaxis->connected) //Already connected
	{
		return 0;
	}
	
//Allocate dev_entry list memory
	u8 dev_count;
	usb_device_entry *dev_entry;
	dev_entry = (usb_device_entry *)iosAlloc(ss_hId, SS_DEV_MAX * sizeof(usb_device_entry));
	if(dev_entry == NULL)
	{
		ss_debug_printf("Error allocating device entry memory.\n");
		goto exit_only;
	}
	memset(dev_entry, 0x0, SS_DEV_MAX * sizeof(usb_device_entry));

//USB device list
	ss_debug_printf("Getting device list...");
	if (USB_GetDeviceList(dev_entry, SS_DEV_MAX, USB_CLASS_HID, &dev_count) < 0)
	{
		ss_debug_printf("Error.\n");
		goto exit_free;
	}
	ss_debug_printf("Found %i usb device(s).\n", dev_count);
	
//Search SIXAXIS/DS3
    ss_debug_printf("Looking for SIXAXIS/DS3: ");
	int i, found = 0;
	for(i = 0; i < dev_count; i++)
	{
		if(dev_entry[i].vid  == SS_VENDOR_ID && dev_entry[i].pid == SS_PRODUCT_ID && !(ss_dev_id_list_search(dev_entry[i].device_id)))
		{
			ss_debug_printf("Found!\n");			
			found = 1;
			break;
		}
    }
    if(!found)
    {
		ss_debug_printf("Not found.\n");
		goto exit_free;
	}
	
//Open device
	s32  fd = -1;
	ss_debug_printf("Opening device...");
	if(USB_OpenDevice(dev_entry[i].device_id, SS_VENDOR_ID, SS_PRODUCT_ID, &fd)< 0)
	{
		ss_debug_printf("Could not open it. fd: %i\n", fd);
		goto exit_close;
	}
	sicksaxis->fd = fd;
	sicksaxis->dev_id = dev_entry[i].device_id;
	ss_dev_id_list_add(dev_entry[i].device_id);
	ss_debug_printf("Opened!\n");
	
//Setting disconnect callback
	ss_debug_printf("Setting disconnect callback...");
	if(USB_DeviceRemovalNotifyAsync(fd, &ss_disconnect_cb, (void *)sicksaxis) < 0)
	{
		ss_debug_printf("Error.\n");
		goto exit_close;
	}
	ss_debug_printf("Done!\n");
	
//Setting operational mode
	ss_debug_printf("Setting operational mode...");
	if(ss_set_operational_mode(sicksaxis) < 0)
	{
		ss_debug_printf("Error.\n");
		goto exit_close;
	}
	ss_debug_printf("Done!\n");

//Everything went OK
	memcpy ((void *)(sicksaxis->leds_rumble), (void *)leds_rumble_payload, SS_LEDS_RUMBLE_LEN);
	sicksaxis->connected = 1;
	ss_debug_printf("Sicksaxis initialized!\n");
	iosFree(ss_hId, dev_entry);
	ss_set_led(sicksaxis, 1);
	return 0;
	
exit_close:
	USB_CloseDevice(&fd);
	sicksaxis->fd = -1;
exit_free:
	iosFree(ss_hId, dev_entry);
exit_only:
	sicksaxis->connected = 0;
	return -1;
}


int ss_close(struct SickSaxis *sicksaxis)
{
	if(sicksaxis->connected)
	{
		USB_CloseDevice(&sicksaxis->fd);
		ss_debug_printf("Device closed\n");
		sicksaxis->connected = 0;
	}
    return 0;
}


int ss_read(struct SickSaxis *sicksaxis)
{                                         
	return USB_WriteCtrlMsg(sicksaxis->fd,
						USB_REQTYPE_INTERFACE_GET,
						USB_REQ_GETREPORT,
						(USB_REPTYPE_INPUT<<8) | 0x1,
						0x0,
						SS_DATA_LEN,
						(uint8_t *)&(sicksaxis->gamepad));

    /*swap16(sicksaxis->gamepad.motion.accX);
    swap16(sicksaxis->gamepad.motion.accY);
    swap16(sicksaxis->gamepad.motion.accZ);
    swap16(sicksaxis->gamepad.motion.Zgyro);*/ //Not needed, it's already big endian
}

int ss_get_mac(struct SickSaxis *sicksaxis, uint8_t *mac)
{
    uint8_t ATTRIBUTE_ALIGN(32) msg[8];
	int ret = USB_WriteCtrlMsg(sicksaxis->fd,
                   USB_REQTYPE_INTERFACE_GET,
                   USB_REQ_GETREPORT,
                   (USB_REPTYPE_FEATURE<<8) | 0xf5,
                   0,
                   8,
                   msg);

    mac[0] = msg[2];
    mac[1] = msg[3];
    mac[2] = msg[4];
    mac[3] = msg[5];
    mac[4] = msg[6];
    mac[5] = msg[7];
    return ret;
}

int ss_set_mac(struct SickSaxis *sicksaxis, uint8_t *mac)
{
    uint8_t ATTRIBUTE_ALIGN(32) msg[8] = {0x01, 0x00, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]};

	return USB_WriteCtrlMsg(sicksaxis->fd,
				USB_REQTYPE_INTERFACE_SET,
				USB_REQ_SETREPORT,
				(USB_REPTYPE_FEATURE<<8) | 0xf2,
				0,
				8,
				msg);
}

int ss_set_operational_mode(struct SickSaxis *sicksaxis)
{
	uint8_t ATTRIBUTE_ALIGN(32) buf[17];
	int ret = USB_WriteCtrlMsg(sicksaxis->fd,
						 USB_REQTYPE_INTERFACE_GET,
						 USB_REQ_GETREPORT,
						 (USB_REPTYPE_FEATURE<<8) | 0xf2,
						 0x0,
						 17,
						 buf);

	return ret;
}

inline int ss_is_connected(struct SickSaxis *sicksaxis)
{
	return sicksaxis->connected;
}

s32 ss_disconnect_cb(s32 result, void *usrdata)
{
	struct SickSaxis *ss = (struct SickSaxis *)usrdata;
	ss->connected = 0;
	ss_dev_id_list_del(ss->dev_id);
	return 1;
}

