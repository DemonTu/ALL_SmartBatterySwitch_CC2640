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
	KEY_NAME_NONE,
    KEY_NAME_3V3,  
    KEY_NAME_STATE,
    KEY_POWER,
} KEY_NAME;

// °´¼ü±àºÅ
typedef enum{
	KEY_NONE,
    KEY_LOW,  
    KEY_HIGH,
    KEY_LONG,
    KEY_IQR,
} KEY_STATE;

extern void KEY_Scan_10ms(void);
extern void KEY_Init(void);

void wifiPowerOn(void);
void wifiPowerDown(void);

void KEY_DisableIRQ(void);
void KEY_EnableIRQ(void);

//#endif
#endif	
