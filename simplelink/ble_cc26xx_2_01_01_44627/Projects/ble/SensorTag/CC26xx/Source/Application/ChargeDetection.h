#ifndef __CHARGEDETECTION_H
	#define __CHARGEDETECTION_H

typedef enum
{
	CHARGENONE = 0,
	CHARGING,
	CHARGED
	
}STR_CHARGESTATE;	

//×´Ì¬¼ì²âI/O³õÊ¼»¯
extern void chargeDetection_Init(void);
extern STR_CHARGESTATE chargeStateRead(void);
extern uint8_t isChargePowerUp(void);

#endif	
