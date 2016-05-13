#ifndef __KEY_SCAN_H
	#define __KEY_SCAN_H
//#ifdef INCLUDE_KEY

extern Queue_Handle keyMsgQueue;

// key event passed from profiles.
typedef struct
{
  uint8_t GPIOName;   // Which key's event
  uint8_t GPIOStatus; // New status
} KEY_stEvt_t;

// °´¼ü±àºÅ
typedef enum{
    KEY_NAME_3V3,  
    KEY_NAME_STATE,
} KEY_NAME;

// °´¼ü±àºÅ
typedef enum{
    KEY_LOW,  
    KEY_HIGH,
} KEY_STATE;


extern void KEY_Init(void);
void wifiPowerOn(void);
void wifiPowerDown(void);

//#endif
#endif	
