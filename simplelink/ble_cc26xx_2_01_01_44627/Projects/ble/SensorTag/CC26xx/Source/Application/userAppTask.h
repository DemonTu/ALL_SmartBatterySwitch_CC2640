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

#endif	

