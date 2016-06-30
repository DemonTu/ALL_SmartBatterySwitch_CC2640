#include "includes.h"
//===============================================================
//==============电池充电状态检测模块=============================
#define INCLUDE_CHARGEIC

static PIN_Handle chargeGpioPin;
static PIN_State chargeGpioState;
static uint8_t chargingFlag = 0;
// Pins that are actively used by the application
static PIN_Config chargePinTable[] =
{
#ifdef INCLUDE_CHARGEIC
	Board_Charge_PG       | PIN_INPUT_EN       | PIN_NOPULL    | PIN_IRQ_DIS, 
	Board_Charge_Stat1	  | PIN_INPUT_EN	   | PIN_PULLUP    | PIN_IRQ_DIS,  
	Board_Charge_Stat2	  | PIN_INPUT_EN	   | PIN_PULLUP    | PIN_IRQ_DIS,	
#endif	

    Board_Charge_RedLED   | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,     
    Board_Charge_GreenLED | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,

    PIN_TERMINATE
};
#ifdef INCLUDE_CHARGEIC
static void chargeState_Callback(PIN_Handle handle, PIN_Id pinId)
{
	switch (pinId) 
	{

		case Board_Charge_PG:	// 充放电状态指示 1 为插上电源， 0 为拔掉电源
			if (PIN_getInputValue(pinId))
			{

				// 插上电源充电
				//uartWriteDebug("插电", 4);
				//userStartClock10ms();
				//OLED_ShowString(0, 48, "ch");
				chargingFlag = 1;
				
			}
			else
			{
				// 拔掉电源		
				//uartWriteDebug("断电", 4);
				//OLED_ShowString(0, 48, "rh");
				chargingFlag = 0;
			}
			break;
		default:
			/* Do nothing */
			break;
	}
}
#endif
void chargeDetection_Init(void)
{
	chargeGpioPin = PIN_open(&chargeGpioState, chargePinTable);
	// 注册charge 中断函数
#ifdef INCLUDE_CHARGEIC	
	PIN_registerIntCb(chargeGpioPin, chargeState_Callback );
#endif
	PIN_setOutputValue(chargeGpioPin, Board_Charge_RedLED, 0);
	PIN_setOutputValue(chargeGpioPin, Board_Charge_GreenLED, 0);
	
}


//=============================CHARGE STATE ==========================STAT1==STAT2
//Charge in progress                                                    ON    OFF
//Charge complete                                                       OFF   ON
//Charge suspend, timer fault, overvoltage, sleep mode, battery absent  OFF   OFF
// 读取充电状态
STR_CHARGESTATE chargeStateRead(void)
{
#ifdef INCLUDE_CHARGEIC
#if 1
    STR_CHARGESTATE chargeState = CHARGENONE;
	if (1 == PIN_getInputValue(Board_Charge_PG))
	{
		//uartWriteDebug("PG=1", 4);	
		chargeState = CHARGING;
		chargingFlag = 1;
	}
	else if (0 == PIN_getInputValue(Board_Charge_PG))
	{
		//uartWriteDebug("PG=0", 4);	
		chargingFlag = 0;
		chargeState = CHARGENONE;
	}
	else
	{
		chargeState = CHARGENONE;
	}
#else
	uint8_t stat1;
	uint8_t stat2;
	uint8_t charge = 0;
	
	STR_CHARGESTATE chargeState = CHARGENONE;
	
	stat1 = PIN_getInputValue(Board_Charge_Stat1);
	stat2 = PIN_getInputValue(Board_Charge_Stat2);

	SMB_Read(RELATIVE_SOC, &charge, 1);
	//bspUartWrite(&charge, 1);

	if (1==stat1 && 0==stat2 && charge<100)
	{
		chargeState = CHARGING;
		PIN_setOutputValue(chargeGpioPin, Board_Charge_RedLED, 1);
		PIN_setOutputValue(chargeGpioPin, Board_Charge_GreenLED, 0);
	}
	else if (0==stat1 && 1==stat2 || 100==charge)
	{
		chargeState = CHARGED;
		PIN_setOutputValue(chargeGpioPin, Board_Charge_RedLED, 0);
		PIN_setOutputValue(chargeGpioPin, Board_Charge_GreenLED, 1);
	}
	else if (0==stat1 && 0==stat2)
	{
		chargeState = CHARGENONE;
		PIN_setOutputValue(chargeGpioPin, Board_Charge_RedLED, 0);
		PIN_setOutputValue(chargeGpioPin, Board_Charge_GreenLED, 0);
	}
	else
	{
		PIN_setOutputValue(chargeGpioPin, Board_Charge_RedLED, 0);
		PIN_setOutputValue(chargeGpioPin, Board_Charge_GreenLED, 0);
		//错误状态
	}
#endif	
#else
	uint8_t stat1[3] = {0};
	static uint8_t stat2=0;
	uint8_t charge = 0;
	STR_CHARGESTATE chargeState = CHARGENONE;

	SMB_Read(BATTERY_STATUS, stat1, 2);
    
	SMB_Read(RELATIVE_SOC, &charge, 1);
	if (0x80 == stat1[0])
	{
		stat2 = 1;
		chargeState = CHARGING;
		chargingFlag = 1;
		PIN_setOutputValue(chargeGpioPin, Board_Charge_RedLED, 1);
		PIN_setOutputValue(chargeGpioPin, Board_Charge_GreenLED, 0);
	}
	else if (0xc0 == stat1[0])
	{
		if (stat2==1 && charge>=99)
		{
			stat2 = 0;
			chargeState = CHARGED;
			PIN_setOutputValue(chargeGpioPin, Board_Charge_RedLED, 0);
			PIN_setOutputValue(chargeGpioPin, Board_Charge_GreenLED, 1);
		}
		chargingFlag = 0;
	}	
	else
    {
        PIN_setOutputValue(chargeGpioPin, Board_Charge_RedLED, 0);
		PIN_setOutputValue(chargeGpioPin, Board_Charge_GreenLED, 0);
        chargingFlag = 0;
    } 
#endif
	return chargeState;
}

uint8_t isChargePowerUp(void)
{
	return chargingFlag;
}

void chargedLedState(void)
{
	PIN_setOutputValue(chargeGpioPin, Board_Charge_RedLED, 0);
	PIN_setOutputValue(chargeGpioPin, Board_Charge_GreenLED, 1);
}
