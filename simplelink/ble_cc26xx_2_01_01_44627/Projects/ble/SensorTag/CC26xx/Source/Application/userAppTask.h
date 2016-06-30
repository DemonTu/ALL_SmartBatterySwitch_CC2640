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
	uint8_t  coordinateBac;	// 坐标备份
	uint8_t  showChar;		// 显示字符A、B、C、D、E、F
	uint8_t  coordinate;	// 坐标 1、2、3、4、5、6
	uint8_t  deviceNum[2];		// 设备编号
}SYSTEMPARA_STR;

extern SYSTEMPARA_STR sysPara;

extern void userSystemParaSave(SYSTEMPARA_STR *para);

#endif	

