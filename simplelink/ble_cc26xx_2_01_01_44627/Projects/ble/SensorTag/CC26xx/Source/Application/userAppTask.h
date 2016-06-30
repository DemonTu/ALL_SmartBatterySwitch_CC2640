#ifndef __USERAPPTASK_H
	#define __USERAPPTASK_H

/*********************************************************************
 * FUNCTIONS
 */

/*
 * Task creation function for SensorTag
 */
extern void userApp_createTask(void);
extern void userAppInit(void);
extern void userAppPro(void);

extern void userStopClock10ms(void);
extern void userStartClock10ms(void);
//===========================================================
// system parameter manage
typedef struct
{
	uint8_t  ver[7];
	uint8_t  coordinateBac;	// ���걸��
	uint8_t  showChar;		// ��ʾ�ַ�A��B��C��D��E��F
	uint8_t  coordinate;	// ���� 1��2��3��4��5��6
	uint8_t  deviceNum[2];		// �豸���
}SYSTEMPARA_STR;

extern SYSTEMPARA_STR sysPara;

extern void userSystemParaSave(SYSTEMPARA_STR *para);

#endif	

