#include "includes.h"
#include "sensorTag.h"

#include "ext_flash.h"
#include "ext_flash_layout.h"

/**
��Ҫ�Ƕ� ������OLED�͵�ؼ�صĴ���
***/

#define SYSTEMVER								"V2.001"
#define VERSHOWTIME								60			// ��λ100ms

// Task configuration
#define USER_TASK_PRIORITY                      2
#define USER_TASK_STACK_SIZE                    700
// ��ʱ�¼�
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

// ����1-9ÿ1��ִ��һ��
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
					//�����ⲿ�ж�
					KEY_EnableIRQ();

					//�ص�ʱ��
					uartWriteDebug("tmout", 5);
					Util_stopClock(&periodicClock_10ms);
				}
			}
			else if (systemState.powerOffFlag!=1 || systemState.keyUpFlag==1)
			{
				/* 
				�ڳ����ػ�ʱ����û�ɿ�ǰĬ���ǹػ������Բ�����ʾͼ��
				
				�ڳ�������ʱ����Ϊ�̰������ж�ϵͳ����������ʱ���ǿ����¼���
				����Ҳ������ʾͼ��
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
					// û�� ͼ����˸����
				}
				else
				{
					//������ʾ
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
					/* ����ػ����ĵ��ͼ�� */
					OLED_Clear();
				}
				systemState.keyUpFlag = 0;
			}
            break;
        default:
            break;
    }
}

// ����1-9ÿ100����ִ��һ��
static void Pollint100mSec(void)
{
    switch(r_pollint._10msCount)
    {
		case 0:
			Pollint1Sec();
			break;
        case 1:
			//======
			if (1 == systemState.lowBatteryFlag)
			{
				static uint8_t bmpFlash = 0;
				if (bmpFlash<=4)
				{
					OLED_showBatteryBmp(0, 88, 8);
				}
				else if (bmpFlash<=8)
				{						
					OLED_showBatteryBmp(0, 88, 6);
				}
				else
				{
					bmpFlash = 0;
				}
				bmpFlash++;
			}
			//========
            break;
		case 2:
            if ((r_pollint._500msCount++) >= 5)
            {
                r_pollint._500msCount = 0;
               // OLED_Refresh_Gram();//������ʾ
            }
			break;
        case 3:
			OLED_Refresh_Gram();//������ʾ
            break;
        case 4:
			if (3 == systemState.keyUpFlag)
			{
				if (systemState.delayCnt-- == 0)
				{
					systemState.keyUpFlag = 0;
					{							
						// �رչ㲥
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
					userAppShowCoordinate(0);
				}
				else if (systemState.verShowTime == VERSHOWTIME/2)				
				{					
					uint8_t showTemp[16]={0};
					sprintf((char *)showTemp, "WiCore-%02x%02x", sysPara.deviceNum[0], sysPara.deviceNum[1]);
					OLED_ShowString(24,32, showTemp);					
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

// ÿ10������������ʱ��
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
// ���״̬����ʼ��I2C	
    SMB_Init(); 
// ���ڳ�ʼ��	
	bspUartInit();  
	
// ������ʼ��Ҫ��ϵͳ�ȶ��󣬷�ֹ�ⲿ�ж�����ϵͳ����
	KEY_Init();

//���״̬����ʼ��
	chargeDetection_Init();

	//OLED_ShowString(40,32, "WiCore"); 
	OLED_Clear();
	
	userSystemParaInit();	// ����ϵͳ������ȡ

	// Create one-shot clocks for internal periodic events.
	Util_constructClock(&periodicClock_10ms, userApp_clockHandler,
	                  10, 0, false, USER_10MS_EVT);

	systemState.powerOffFlag = 1;
	Util_startClock(&periodicClock_10ms);
	
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
						//OLED_ShowString(40,32, "WiCore"); 
						
						userAppShowCharge();
						OLED_ShowString(40,32, SYSTEMVER); 		
						systemState.verShowTime = VERSHOWTIME;
						// �����㲥
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
						// ��͵���˸
						systemState.lowBatteryFlag = 0;
						OLED_Clear(); // ���ִ��ʱ��ϳ� �����˶�ʱ���ڣ�����stopClock��û���õ�

						// �����ӣ��ر� 
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
							userAppShowCharge();
							//OLED_ShowString(40,32, "WiCore");
							OLED_ShowString(40,32, SYSTEMVER); 
							systemState.verShowTime = VERSHOWTIME;
							
							// �����㲥
							{
								uint8_t initialAdvertEnable = TRUE;
								GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &initialAdvertEnable);
							}

							uartWriteDebug("poweron", 7);
						}
						else
						{
							//ϵͳ�ϵ�
							systemState.keyUpFlag=0;
							systemState.powerOffFlag = 1;
							wifiPowerDown();
							uartWriteDebug("powerdown", 9);
							
							OLED_Clear();
													
							systemState.lowBatteryFlag = 0;  // ��͵���˸ 
							// �����ӣ��ر� 
							GAPRole_TerminateConnection();
							systemState.verShowTime = 0;
						}
						systemState.keyShortFlag = 0;	// ���Զ̰��¼� 
						
					}
					else if (KEY_LOW == pMsg->GPIOStatus)// �ɿ�
					{
						
						if (1 == systemState.keyShortFlag)// �̰��ɿ� ����һ�ζ̰������¼�
						{
							//�̰��¼�����
							uartWriteDebug("�̰�", 4);
							systemState.keyShortFlag = 0;
						}
					}
					else if (KEY_HIGH == pMsg->GPIOStatus) // �̰�
					{
						if (1 == systemState.powerOffFlag) // �ȴ������¼� ���Դ�ʱ�Ķ̰��¼�
						{
							// �ػ��� ������				
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
 * @brief   ���ͼ��Ͱٷֱȵ���ʾ
 *
 * @param   void
 *
 * @return  0 �е磬1 û��
 */
uint8_t userAppShowCharge(void)
{
	uint8_t smbBuf[4];
	uint8_t charge;
	uint8_t stat1[3] = {0};
	uint8_t chargeState = 6;
	uint8_t bmpMov = 0;
	
	SMB_Read(SPEC_INFO, stat1, 2);	
	//uartWriteDebug(stat1, 2);
	SMB_Read(RELATIVE_SOC, &charge, 1);
	//uartWriteDebug(&charge, 1);

	OLED_ShowString(40,0, "          ");	// ������ʾ����

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
			OLED_showBatteryBmp(0, 88-bmpMov, 7);	// ��ʾ���ͼ��
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
	6  , 50,
	6  , 14,
	114, 14,
	114, 50,
	61 , 50,
	61,  14,
		
};

static uint8_t cntFlag=0;

/*******************************************************************************
 * @fn      userAppShowCoordinate
 *
 * @brief   ��ʾ��������
 *
 * @param  showFlag(�Ƿ���ʾ��־λ ��0��ʾ��0����ʾ)
 *
 * @return  0 �е磬1 û��
 */
void userAppShowCoordinate(uint8_t showFlag)
{
	uint8_t tmpShow[2]={0};

	if (sysPara.deviceNum[0]&0x80)
	{
		/* �� */

		/* ������ */
		OLED_ShowString(8-2, 50, "e");
		OLED_ShowString(8-2, 14, "f");
		OLED_ShowString(8+8+96+2, 14, "c");
		OLED_ShowString(8+8+96+2, 50, "d");
		OLED_ShowString(8+8+44+1, 50, "a");
		OLED_ShowString(8+8+44+1, 14, "b");
	}
	else
	{
		/* С�� */
		/* ������ */
		OLED_ShowString(8-2, 50, "a");
		OLED_ShowString(8-2, 14, "b");
		OLED_ShowString(8+8+96+2, 14, "c");
		OLED_ShowString(8+8+96+2, 50, "d");
	}
	if (showFlag)
	{
		tmpShow[0] = sysPara.showChar;
		if (cntFlag >= 3)
		{
			tmpShow[0] = ' ';
		}
		OLED_ShowString(coordinateTable[sysPara.coordinate-1].x, coordinateTable[sysPara.coordinate-1].y, &tmpShow[0]); 		
		
		if (++cntFlag >=6)
		{
			cntFlag = 0;
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
//        ϵͳ��������
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
			if (sysPara.coordinate == 0xff)
			{
				sysPara.coordinate = 0;
			}
			if (sysPara.deviceNum[0]==0xFF && sysPara.deviceNum[1]==0xFF)
			{
				sysPara.deviceNum[0] = 0;	
				sysPara.deviceNum[1] = 0;	
			}
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
