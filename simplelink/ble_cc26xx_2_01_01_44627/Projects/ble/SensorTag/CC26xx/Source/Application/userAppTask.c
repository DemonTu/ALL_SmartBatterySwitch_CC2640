#include "includes.h"
#include "sensorTag.h"

#include "ext_flash.h"
#include "ext_flash_layout.h"

/**
主要是对 按键、OLED和电池监控的处理
***/

#define SYSTEMVER								"V2.000"
#define VERSHOWTIME								30			// 单位100ms

// Task configuration
#define USER_TASK_PRIORITY                      2
#define USER_TASK_STACK_SIZE                    700
// 定时事件
#define USER_10MS_EVT							0x0010
#define USER_100MS_EVT							0x0020

// Task configuration
static Task_Struct userAppTask;

// Clock instances for internal periodic events.
static Clock_Struct periodicClock_10ms;
static Clock_Struct periodicClock_100ms;

static uint16_t userEvents = 0;

extern ICall_Semaphore sem;

SYSTEMPARA_STR sysPara;

//===========================================================

typedef struct
{
    uint32_t             _10msCount;
    uint32_t             _50msCount;
    uint32_t             _100msCount;
    uint32_t             _500msCount;
    uint32_t             Pollintack;
} STR_POLLINT;

static STR_POLLINT r_pollint;

typedef struct
{
	uint16_t delayPowerOffTime;
	uint8_t  powerOffFlag;
	uint8_t  keyUpFlag;
	uint8_t  keyShortFlag;
	uint8_t  lowBatteryFlag;
	uint8_t  chargingFlag;
	uint8_t  chargeState;
	uint8_t  delayCnt;
	uint8_t  verShowTime;
} STR_USERAPP;

static STR_USERAPP systemState;

uint8_t userAppShowCharge(void);
static void userSystemParaInit(void);
void userAppShowCoordinate(uint8_t showFlag);

/*******************************************************************************
 * PUBLIC FUNCTIONS
 */
/*******************************************************************************
 * @fn		SensorTag_clockHandler
 *
 * @brief	Handler function for clock time-outs.
 *
 * @param	arg - event type
 *
 * @return	none
 */
static void userApp_clockHandler(UArg arg)
{
	// Store the event.
	userEvents |= arg;

	// Wake up the application.
	//Semaphore_post(userSem);
	Semaphore_post(sem);
}

// 以下1-9每1秒执行一次
static void Pollint1Sec(void)
{
    switch(r_pollint._100msCount)
    {
        case 1:
			#if 0
			{
				uint8_t smbBuf[3];
				uartWriteDebug("Test=", 5);
				SMB_Read(RELATIVE_SOC, smbBuf, 1);
				uartWriteDebug(smbBuf, 1);
				SMB_Read(ABSOLUTE_SOC, smbBuf, 1);
				uartWriteDebug(smbBuf, 1);
				SMB_Read(BATTERY_STATUS, smbBuf, 2);
				uartWriteDebug(smbBuf, 2);
				SMB_Read(BATTERY_MODE, smbBuf, 2);
				uartWriteDebug(smbBuf, 2);
			}
			#endif
            break;
        case 2:
			if (systemState.delayPowerOffTime)
			{
				if (--systemState.delayPowerOffTime == 0)
				{
					systemState.powerOffFlag = 0;
					//开启外部中断
					KEY_EnableIRQ();

					//关掉时钟
					uartWriteDebug("tmout", 5);
					Util_stopClock(&periodicClock_10ms);
				}
			}
			else if (systemState.powerOffFlag!=1 || systemState.keyUpFlag==1)
			{
				/* 
				在长按关机时，还没松开前默认是关机的所以不能显示图标
				
				在长按开机时，以为短按触发中断系统开启，但此时不是开机事件，
				所以也不能显示图标
				*/
				
				systemState.lowBatteryFlag = userAppShowCharge();

				if (2 == systemState.lowBatteryFlag)
				{
				//	OLED_ShowString(40, 0, "           ");
				//	OLED_ShowString(40, 0, "No battery ");  
					bspI2cReset();
				}
				else if (1 == systemState.lowBatteryFlag)
				{
					// 没电 图标闪烁警告
				}
				else
				{
					//正常显示
				}
				//OLED_testShowGengeeRoll(3);
			}
            break;
        case 3:
			if(CHARGING == chargeStateRead())
			{
				systemState.keyUpFlag = 1;
			}
			else
			{
				if (systemState.keyUpFlag && systemState.powerOffFlag)
				{
					/* 清除关机充电的电池图标 */
					OLED_Clear();
				}
				systemState.keyUpFlag = 0;
			}
            break;
        default:
            break;
    }
}

// 以下1-9每100毫秒执行一次
static void Pollint100mSec(void)
{
    switch(r_pollint._10msCount)
    {
		case 0:
			Pollint1Sec();
			break;
        case 1:
			
            break;
		case 2:
            if ((r_pollint._500msCount++) >= 5)
            {
                r_pollint._500msCount = 0;
				if (1 == systemState.lowBatteryFlag)
				{
					static uint8_t bmpFlash = 0;
					if (bmpFlash)
					{
						bmpFlash = 0;
						OLED_showBatteryBmp(0, 88, 8);
					}
					else
					{						
						bmpFlash = 1;
						OLED_showBatteryBmp(0, 88, 6);
					}
				}
                OLED_Refresh_Gram();//更新显示
            }
			break;
        case 3:
			//OLED_Refresh_Gram();//更新显示
            break;
        case 4:
			if (3 == systemState.keyUpFlag)
			{
				if (systemState.delayCnt-- == 0)
				{
					systemState.keyUpFlag = 0;
					{							
						// 关闭广播
						uint8_t initialAdvertEnable = FALSE;
						GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &initialAdvertEnable);
					}
					Util_stopClock(&periodicClock_10ms);
				}
					
			}
            break;
        case 5:
			if (systemState.verShowTime)			
			{			
				if (--systemState.verShowTime == 0)
				{
					OLED_showPitchBmp(1, 0);
				}
				else if (systemState.verShowTime == VERSHOWTIME/2)				
				{					
					OLED_ShowString(40,32, "WiCore");					
				}
			}
			else if ((sysPara.coordinate<7) && (systemState.powerOffFlag==0))
			{
				OLED_showPitchBmp(1, 0);
				userAppShowCoordinate(sysPara.coordinate);				
			}
            break;
        case 6:
			
            break;
        case 7:
            break;
        case 8:
            break;
        default:
            break;
    }
}

// 每10毫秒更改软件定时器
static void ChangeTime10mSec(void)
{
    r_pollint._10msCount = (r_pollint._10msCount + 1) % 10;
    if(r_pollint._10msCount == 0)
    {
        r_pollint._100msCount = (r_pollint._100msCount + 1) % 10;
    }
    
}

/*******************************************************************************
 * @fn      userAppInit
 *
 * @brief   user Application init
 *
 * @param   void
 *
 * @return  none
 */
void userAppInit(void)
{
	OLED_Init();
// 电池状态读初始化I2C	
    SMB_Init(); 
// 串口初始化	
	bspUartInit();  
	
// 按键初始化要等系统稳定后，防止外部中断引起系统死机
	KEY_Init();

//电池状态检测初始化
	chargeDetection_Init();

	//OLED_ShowString(40,32, "WiCore"); 
	OLED_Clear();
	
	userSystemParaInit();	// 增加系统参数读取

	// Create one-shot clocks for internal periodic events.
	Util_constructClock(&periodicClock_10ms, userApp_clockHandler,
	                  10, 0, false, USER_10MS_EVT);
#ifndef INCLUDE_CLKSTOP	
	systemState.powerOffFlag = 1;
	Util_startClock(&periodicClock_10ms);
#else
	Util_stopClock(&periodicClock_10ms);

#endif	
}

/*******************************************************************************
 * @fn      userAppPro
 *
 * @brief   user Application event process
 *
 * @param   evnet
 *
 * @return  none
 */
void userAppPro(void)
{
	if (userEvents & USER_10MS_EVT)
	{
		userEvents &= ~USER_10MS_EVT;
		
		Util_startClock(&periodicClock_10ms);
		
		KEY_Scan_10ms();
		
		ChangeTime10mSec();

		Pollint100mSec();
	}

#ifdef INCLUDE_CLKSTOP
	while (!Queue_empty(keyMsgQueue))
	{
		KEY_stEvt_t *pMsg = (KEY_stEvt_t *)Util_dequeueMsg(keyMsgQueue);
		if (pMsg)
		{
			// Process message.
			switch(pMsg->GPIOName)
			{
				case KEY_NAME_3V3:
					if (KEY_HIGH == pMsg->GPIOStatus)
					{
						wifiPowerOn();
						uartWriteDebug("poweron3v3", 10);
						OLED_ShowString(40,32, "WiCore"); 
						
						userAppShowCharge();
						// 启动广播
						{
							uint8_t initialAdvertEnable = TRUE;
							GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &initialAdvertEnable);
						}
						Util_startClock(&periodicClock_10ms);
					}
					else
					{
						wifiPowerDown();
						uartWriteDebug("powerdown3v3", 12);
						// 清低电闪烁
						systemState.lowBatteryFlag = 0;
						OLED_Clear(); // 这个执行时间较长 打乱了定时周期，所以stopClock是没有用的
						//Util_stopClock(&periodicClock_10ms);
					
						// 服务器的按键开关机 设置一个按键放开标志位，等待1s后没有放开
						// 就清标志位，关闭时钟	
						systemState.keyUpFlag = 3;	     // 2 为电源按键 等待按键放开标志，3为 服务器按键
						systemState.delayCnt = 10;
						// 有链接，关闭	
						GAPRole_TerminateConnection();
						
					}
					break;
				case KEY_POWER:
					if (KEY_IQR == pMsg->GPIOStatus)
					{
						KEY_DisableIRQ();
						uartWriteDebug("tttt", 4);
						systemState.powerOffFlag = 1;
						systemState.delayPowerOffTime = 5; // 延时5s 判断是否是按键长按
						Util_startClock(&periodicClock_10ms);
					}
					else if (KEY_LONG == pMsg->GPIOStatus)
					{
						if (1 == systemState.powerOffFlag)
						{
							systemState.powerOffFlag = 0;
							systemState.delayPowerOffTime = 0;
							wifiPowerOn();
							
							userAppShowCharge();
							OLED_ShowString(40,32, "WiCore");
							
							// 启动广播
							{
								uint8_t initialAdvertEnable = TRUE;
								GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &initialAdvertEnable);
							}

							uartWriteDebug("poweron", 7);
						}
						else
						{
							//系统断电
							
							wifiPowerDown();
							uartWriteDebug("powerdown", 9);
							
							OLED_Clear();
													
							systemState.lowBatteryFlag = 0;  // 清低电闪烁 
							systemState.keyUpFlag = 2;	     // 2 为电源按键 等待按键放开标志，3为 服务器按键
							// 有链接，关闭	
							GAPRole_TerminateConnection();
						}
						systemState.keyShortFlag = 0;   // 忽略短按事件 
						
					}
					else if (KEY_LOW == pMsg->GPIOStatus)// 松开
					{
						if (2 == systemState.keyUpFlag)	// 长按松开，关机 
						{
							systemState.keyUpFlag = 0;
							//开启外部中断
							KEY_EnableIRQ();
							{		
								// 关闭广播
								uint8_t initialAdvertEnable = FALSE;
								GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &initialAdvertEnable);
							}
							Util_stopClock(&periodicClock_10ms);
						}
						else if (1 == systemState.keyShortFlag)// 短按松开 产生一次短按完整事件
						{
							//短按事件处理
							uartWriteDebug("短按", 4);
						}
					}
					else if (KEY_HIGH == pMsg->GPIOStatus) // 短按
					{
						if (1 == systemState.powerOffFlag) // 等待长按事件 忽略此时的短按事件
						{
							systemState.delayPowerOffTime = 5;	// 防止timout 剩2s时又产生长按事件					
						}
						else
						{
							systemState.keyShortFlag = 1;
						}
						
					}
					break;
				default:
					break;
			}
	
			// Free the space from the message.
			ICall_free(pMsg);
		}
	}
#else
	while (!Queue_empty(keyMsgQueue))
	{
		KEY_stEvt_t *pMsg = (KEY_stEvt_t *)Util_dequeueMsg(keyMsgQueue);
		if (pMsg)
		{
			// Process message.
			switch(pMsg->GPIOName)
			{
				case KEY_NAME_3V3:
					if (KEY_HIGH == pMsg->GPIOStatus)
					{
						wifiPowerOn();
//						userSystemParaInit();	// 增加系统参数读取
						uartWriteDebug("poweron3v3", 10);
						//OLED_ShowString(40,32, "WiCore"); 
						
						userAppShowCharge();
						OLED_ShowString(40,32, SYSTEMVER); 		
						systemState.verShowTime = VERSHOWTIME;
						// 启动广播
						{
							uint8_t initialAdvertEnable = TRUE;
							GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &initialAdvertEnable);
						}

						systemState.powerOffFlag = 0;
					}
					else
					{
						wifiPowerDown();
						uartWriteDebug("powerdown3v3", 12);
						// 清低电闪烁
						systemState.lowBatteryFlag = 0;
						OLED_Clear(); // 这个执行时间较长 打乱了定时周期，所以stopClock是没有用的

						// 有链接，关闭 
						GAPRole_TerminateConnection();

						systemState.powerOffFlag = 1;
						systemState.verShowTime = 0;
						
					}
					break;
				case KEY_POWER:
					if (KEY_LONG == pMsg->GPIOStatus)
					{
						if (1 == systemState.powerOffFlag)
						{
							systemState.powerOffFlag = 0;
							systemState.delayPowerOffTime = 0;
							systemState.keyUpFlag=0;
							wifiPowerOn();
//							userSystemParaInit();	// 增加系统参数读取
							userAppShowCharge();
							//OLED_ShowString(40,32, "WiCore");
							OLED_ShowString(40,32, SYSTEMVER); 
							systemState.verShowTime = VERSHOWTIME;
							
							// 启动广播
							{
								uint8_t initialAdvertEnable = TRUE;
								GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &initialAdvertEnable);
							}

							uartWriteDebug("poweron", 7);
						}
						else
						{
							//系统断电
							systemState.keyUpFlag=0;
							systemState.powerOffFlag = 1;
							wifiPowerDown();
							uartWriteDebug("powerdown", 9);
							
							OLED_Clear();
													
							systemState.lowBatteryFlag = 0;  // 清低电闪烁 
							// 有链接，关闭 
							GAPRole_TerminateConnection();
							systemState.verShowTime = 0;
						}
						systemState.keyShortFlag = 0;	// 忽略短按事件 
						
					}
					else if (KEY_LOW == pMsg->GPIOStatus)// 松开
					{
						
						if (1 == systemState.keyShortFlag)// 短按松开 产生一次短按完整事件
						{
							//短按事件处理
							uartWriteDebug("短按", 4);
							systemState.keyShortFlag = 0;
						}
					}
					else if (KEY_HIGH == pMsg->GPIOStatus) // 短按
					{
						if (1 == systemState.powerOffFlag) // 等待长按事件 忽略此时的短按事件
						{
							// 关机中 不处理				
						}
						else
						{
							systemState.keyShortFlag = 1;
						}
						
					}
					break;
				default:
					break;
			}

			// Free the space from the message.
			ICall_free(pMsg);
		}
	}

#endif
}

/*******************************************************************************
 * @fn      userAppClockStop
 *
 * @brief   stop task clock
 *
 * @param   evnet
 *
 * @return  none
 */
void userAppClockStop(void)
{
	Util_stopClock(&periodicClock_10ms);
}

/*******************************************************************************
 * @fn      userAppShowCharge
 *
 * @brief   电池图标和百分比的显示
 *
 * @param   void
 *
 * @return  0 有电，1 没电
 */
uint8_t userAppShowCharge(void)
{
	uint8_t smbBuf[4];
	uint8_t charge;
	uint8_t stat1[3] = {0};
	uint8_t chargeState = 6;
	uint8_t bmpMov = 0;
	
	SMB_Read(SPEC_INFO, stat1, 2);	
	//bspUartWrite(stat1, 2);
	SMB_Read(RELATIVE_SOC, &charge, 1);
	//uartWriteDebug(&charge, 1);

	OLED_ShowString(40,0, "          ");	// 清电池显示区域

	if (charge >100 || stat1[0]!=0x21)
	{
		return 2;
	}
	
	SensorTag_checkBatteryInfo(charge);
	if (100 == charge)
	{
		uartWriteDebug("T", 1);
		bmpMov = 8;
		chargeState = 0;
	}
	else if (charge > 90)
	{
		chargeState = 0;
	}
	else if(charge > 76)
	{
		chargeState = 1;
	}
	else if(charge > 62)
	{
		chargeState = 2;
	}
	else if(charge > 48)
	{
		chargeState = 3;
	}
	else if(charge > 34)
	{
		chargeState = 4;
	}
	else if(charge > 10)
	{
		chargeState = 5;
	}
	else
	{
		sprintf((char *)smbBuf, "%02d%", charge);
		OLED_ShowString(104,0, smbBuf);	
		return 1;
	}
	
	sprintf((char *)smbBuf, "%02d%", charge);
	if (0 == isChargePowerUp())
	{
		
		OLED_showBatteryBmp(0, 88-bmpMov, chargeState);

	}
	else
	{
		if (charge == 100)
		{
			
			uartWriteDebug("I", 1);
			OLED_showBatteryBmp(0, 88-bmpMov, chargeState);
			//chargedLedState();
		}
		else
		{
			uartWriteDebug("O", 1);
			OLED_showBatteryBmp(0, 88-bmpMov, 7);	// 显示充电图标
		}
		
	}
	OLED_ShowString(104-bmpMov, 0, smbBuf);	
	return 0;
}

//================================================================================
typedef struct 
{
	uint8_t x;
	uint8_t y;
}COORDINATE_STR;
const COORDINATE_STR coordinateTable[]=
{
	0  , 48,
	0  , 16,
	120, 16,
	120, 48,
	56 , 48,
	56,  16,
		
};
/*******************************************************************************
 * @fn      userAppShowCoordinate
 *
 * @brief   显示球场坐标标号
 *
 * @param  showFlag(是否显示标志位 非0显示，0不显示)
 *
 * @return  0 有电，1 没电
 */
void userAppShowCoordinate(uint8_t showFlag)
{
	uint8_t tmpShow[2]={0};
	if (showFlag)
	{
		tmpShow[0] = sysPara.showChar;
		OLED_ShowString(coordinateTable[sysPara.coordinateBac-1].x, coordinateTable[sysPara.coordinateBac-1].y, " ");
		/* 补线 */
		if (sysPara.coordinateBac == 5)
		{
			OLED_PitchDrawLine(63, 56);
		}
		else if (6 == sysPara.coordinateBac)
		{			
			OLED_PitchDrawLine(16, 56);
		}
		OLED_ShowString(coordinateTable[sysPara.coordinate-1].x, coordinateTable[sysPara.coordinate-1].y, tmpShow);
		/* 补线 */
		if (sysPara.coordinate == 5)
		{
			OLED_PitchDrawLine(63, 56);
		}
		else if (6 == sysPara.coordinate)
		{			
			OLED_PitchDrawLine(16, 56);
		}
	}
	else
	{
		tmpShow[0] = ' ';
		OLED_ShowString(coordinateTable[sysPara.coordinateBac-1].x, coordinateTable[sysPara.coordinateBac-1].y, tmpShow);
		/* 补线 */
		if (sysPara.coordinateBac == 5)
		{
			OLED_PitchDrawLine(63, 56);
		}
		else if (6 == sysPara.coordinateBac)
		{			
			OLED_PitchDrawLine(16, 56);
		}
	}
}
//===============================================
void userStopClock10ms(void)
{
	Util_stopClock(&periodicClock_10ms);
}
void userStartClock10ms(void)
{
	Util_startClock(&periodicClock_10ms);
}
//========================================================
//        系统参数管理
static void userSystemParaInit(void)
{
	bool success;

	success = extFlashOpen();

	if (success)
	{
		extFlashRead(EFL_ADDR_USER, sizeof(SYSTEMPARA_STR), (uint8_t *)&sysPara);
		if (memcmp(sysPara.ver, SYSTEMVER, strlen(SYSTEMVER)))
		{
			memcpy(sysPara.ver, SYSTEMVER, strlen(SYSTEMVER));
			sysPara.coordinateBac = 1;
			extFlashErase(EFL_ADDR_USER, EFL_PAGE_SIZE);

			extFlashWrite(EFL_ADDR_USER, sizeof(SYSTEMPARA_STR), (uint8_t *)&sysPara);
		}
		extFlashClose();
	}	
	

}

void userSystemParaSave(SYSTEMPARA_STR *para)
{
	bool success;
	
	success = extFlashOpen();

	if (success)
	{
		extFlashErase(EFL_ADDR_USER, EFL_PAGE_SIZE);
		extFlashWrite(EFL_ADDR_USER, sizeof(SYSTEMPARA_STR), (uint8_t *)para);
			
		extFlashClose();
	}	
}
