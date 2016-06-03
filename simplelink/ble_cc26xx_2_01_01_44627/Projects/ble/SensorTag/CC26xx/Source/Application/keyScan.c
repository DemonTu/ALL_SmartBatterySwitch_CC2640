#include "includes.h"

extern ICall_Semaphore sem;

#ifdef INCLUDE_KEY

static PIN_Handle KEYGpioPin;
static PIN_State keyPinGpioState;

// Queue object used for app messages
Queue_Struct keyMsg;
Queue_Handle keyMsgQueue;

// Pins that are actively used by the application
static PIN_Config KEYPinTable[] =
{
    Board_KEY_3V3   | PIN_INPUT_EN       | PIN_PULLUP    | PIN_IRQ_BOTHEDGES,     
    Board_POWER     | PIN_INPUT_EN       | PIN_PULLUP    | PIN_IRQ_DIS, 
    Board_KEY_STATE | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,

	PIN_TERMINATE
};


static void KEY_Callback(PIN_Handle handle, PIN_Id pinId)
{
	KEY_stEvt_t *tempMsg;

  // Create dynamic pointer to message.
	if (tempMsg = ICall_malloc(sizeof(KEY_stEvt_t)))
	{
		switch (pinId) {

			case Board_KEY_3V3:
				//PIN_setInterrupt(KEYGpioPin, PIN_ID(Board_POWER)|PIN_IRQ_DIS);
				tempMsg->GPIOName = KEY_NAME_3V3;
				if (PIN_getInputValue(Board_KEY_3V3))
				{
					tempMsg->GPIOStatus = KEY_HIGH;
				}	
				else
				{	
					tempMsg->GPIOStatus = KEY_LOW;
				}
				break;
			case Board_POWER:
				//PIN_setInterrupt(KEYGpioPin, PIN_ID(Board_KEY_3V3)|PIN_IRQ_DIS);
				tempMsg->GPIOName = KEY_POWER;
				tempMsg->GPIOStatus = KEY_IQR;
				break;
			default:
				/* Do nothing */
				break;
		}
		Util_enqueueMsg(keyMsgQueue, sem, (uint8_t *)tempMsg);
	}	 	
}

/*****************wifi 模块电源控制 *************/

//开电源
void wifiPowerOn(void)
{
  PIN_setOutputValue(KEYGpioPin, Board_KEY_STATE,1);
}

// 关电源
void wifiPowerDown(void)
{
  PIN_setOutputValue(KEYGpioPin, Board_KEY_STATE,0);
}


void KEY_Init(void)
{
	KEYGpioPin = PIN_open(&keyPinGpioState, KEYPinTable);
	// 注册key 中断函数
	PIN_registerIntCb(KEYGpioPin, KEY_Callback);
	
	// 上电默认关wifi电源
    PIN_setOutputValue(KEYGpioPin, Board_KEY_STATE, 0);

	// Create an RTOS queue for message from profile to be sent to app.
	keyMsgQueue = Util_constructQueue(&keyMsg);
}
/************************************************************
* Funtion Name : KEY_Scan_10ms
* Description  : key scan
* OutPut       : None
* Input        : None
* Return       : None
*************************************************************/
void KEY_Scan_10ms(void)
{
	static uint8_t keyValue = 0;
	static uint16_t keyLongCnt = 0;
	
	keyValue <<= 1;	
	
	if (0 == PIN_getInputValue(Board_POWER))
	{
		keyValue |= 0x01;
	}

	switch(keyValue)
	{
		case 0x0f:
			{
				KEY_stEvt_t *tempMsg;
				if (tempMsg = ICall_malloc(sizeof(KEY_stEvt_t)))
				{
					tempMsg->GPIOName = KEY_POWER;
					tempMsg->GPIOStatus = KEY_HIGH;
					Util_enqueueMsg(keyMsgQueue, sem, (uint8_t *)tempMsg);
				}
			}
			uartWriteDebug("keydown", 7);
			// 短按
			break;
		case 0xff:
			// 长按 第一次大概80ms， 后面是每次10ms
			if (++keyLongCnt == 300)
			{
				KEY_stEvt_t *tempMsg;
				if (tempMsg = ICall_malloc(sizeof(KEY_stEvt_t)))
				{
					tempMsg->GPIOName = KEY_POWER;
					tempMsg->GPIOStatus = KEY_LONG;
					Util_enqueueMsg(keyMsgQueue, sem, (uint8_t *)tempMsg);
				}
				uartWriteDebug("keylong", 7);
			}
			else
			{
				// 长按事件已产生
			}
			break;
		case 0xf0:
			keyLongCnt = 0;
			{
				KEY_stEvt_t *tempMsg;
				if (tempMsg = ICall_malloc(sizeof(KEY_stEvt_t)))
				{
					tempMsg->GPIOName = KEY_POWER;
					tempMsg->GPIOStatus = KEY_LOW;
					Util_enqueueMsg(keyMsgQueue, sem, (uint8_t *)tempMsg);
				}
			}
			
			uartWriteDebug("keyup", 5);
			// 放开
			break;
		default:
			//tempMsg->GPIOName = KEY_NAME_NONE;
			break;
	}
	
}

// 系统电源脚 失能中断
void KEY_DisableIRQ(void)
{
	PIN_setInterrupt(KEYGpioPin, PIN_ID(Board_POWER)|PIN_IRQ_DIS);
}

// 系统电源脚 使能中断
void KEY_EnableIRQ(void)
{
	PIN_setInterrupt(KEYGpioPin, PIN_ID(Board_POWER)|PIN_IRQ_NEGEDGE);
}

#endif
