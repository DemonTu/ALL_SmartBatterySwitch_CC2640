/*******************************************************************************
  Filename:       SensorTag.c
  Revised:        $Date: 2013-11-20 20:28:12 +0100 (on, 20 nov 2013) $
  Revision:       $Revision: 36163 $

  Description:    This file is the SensorTag application's main body.

  Copyright 2015 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED ``AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
*/

/*******************************************************************************
 * INCLUDES
 */
#include <string.h>
#include <xdc/std.h>

#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>

#ifdef POWER_SAVING
#include <ti/sysbios/family/arm/cc26xx/Power.h>
#include <ti/sysbios/family/arm/cc26xx/PowerCC2650.h>
#endif

#include <ICall.h>

#include "gatt.h"
#include "hci.h"
#include "gapgattserver.h"
#include "gattservapp.h"
#include "gapbondmgr.h"
#include "osal_snv.h"
#include "ICallBleAPIMSG.h"
#include "util.h"

#include "bsp_i2c.h"
#include "bsp_spi.h"
#include "bsp_uart.h"

#include "SensorTag_Revision.h"
#include "sensor.h"
#include "Board.h"
#include "devinfoservice.h"
#include "movementservice.h"


#ifdef FEATURE_REGISTER_SERVICE
#include "registerservice.h"
#endif

// Sensor devices
#include "SensorTag_Mov.h"

// Other devices
#include "ext_flash.h"
#include "ext_flash_layout.h"

#include "oled.h"
#include "SmBus_Driver.h"
#include "keyScan.h"

#include "userAppTask.h"

// Optional services
#ifdef FEATURE_OAD
  #include "sensortag_connctrl.h"
  #include "oad_target.h"
  #include "oad.h"
#endif

/*******************************************************************************
 * CONSTANTS
 */

// How often to perform periodic event (in msec)
#define ST_PERIODIC_EVT_PERIOD               1000

// What is the advertising interval when device is discoverable
// (units of 625us, 160=100ms)
#define DEFAULT_ADVERTISING_INTERVAL          160

// General discoverable mode advertises indefinitely
#define DEFAULT_DISCOVERABLE_MODE             GAP_ADTYPE_FLAGS_LIMITED

// Minimum connection interval (units of 1.25ms, 80=100ms) if automatic
// parameter update request is enabled
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     8

// Maximum connection interval (units of 1.25ms, 800=1000ms) if automatic
// parameter update request is enabled
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     800

// Slave latency to use if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SLAVE_LATENCY         0

// Supervision timeout value (units of 10ms, 1000=10s) if automatic parameter
// update request is enabled
#define DEFAULT_DESIRED_CONN_TIMEOUT          100

// Whether to enable automatic parameter update request when a
// connection is formed
#define DEFAULT_ENABLE_UPDATE_REQUEST         FALSE

// Connection Pause Peripheral time value (in seconds)
#define DEFAULT_CONN_PAUSE_PERIPHERAL         1

// Company Identifier: Texas Instruments Inc. (13)
#define TI_COMPANY_ID                         0x000D
#define TI_ST_DEVICE_ID                       0x03
#define TI_ST_KEY_DATA_ID                     0x00

// Length of board address
#define B_ADDR_STR_LEN                        15

#if defined ( PLUS_BROADCASTER )
  #define ADV_IN_CONN_WAIT                    500 // delay 500 ms
#endif

// Task configuration
#define ST_TASK_PRIORITY                      1
#define ST_TASK_STACK_SIZE                    700

// Internal Events for RTOS application
#define ST_STATE_CHANGE_EVT                   0x0001
#define ST_CHAR_CHANGE_EVT                    0x0002
#define ST_PERIODIC_EVT                       0x0004
#ifdef FEATURE_OAD
#define SBP_OAD_WRITE_EVT                     0x0008
#endif //FEATURE_OAD

// Misc.
#define INVALID_CONNHANDLE                    0xFFFF
#define TEST_INDICATION_BLINKS                5  // Number of blinks
#define BLINK_DURATION                        20 // Milliseconds
#define OAD_PACKET_SIZE                       18
#define KEY_STATE_OFFSET                      13 // Offset in advertising data

/*******************************************************************************
 * TYPEDEFS
 */

// App event passed from profiles.
typedef struct
{
  uint8_t event;  // Which profile's event
  uint8_t serviceID; // New status
  uint8_t paramID;
} stEvt_t;

/*******************************************************************************
 * GLOBAL VARIABLES
 */
// Profile state and parameters
gaprole_States_t gapProfileState = GAPROLE_INIT;

// Semaphore globally used to post events to the application thread
ICall_Semaphore sem;

// Global pin resources
PIN_State pinGpioState;
PIN_Handle hGpioPin;

/*******************************************************************************
 * LOCAL VARIABLES
 */

// Task configuration
static Task_Struct sensorTagTask;
static Char sensorTagTaskStack[ST_TASK_STACK_SIZE];

// Entity ID globally used to check for source and/or destination of messages
static ICall_EntityID selfEntity;

// Clock instances for internal periodic events.
static Clock_Struct periodicClock;

// Queue object used for app messages
static Queue_Struct appMsg;
static Queue_Handle appMsgQueue;

// events flag for internal application events.
static uint16_t events;

// GAP - SCAN RSP data (max size = 31 bytes)
static uint8_t scanRspData[] =
{
  // complete name
  0x07,   // length of this data
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,
//  'C', 'C', '2', '6', '5', '0', ' ',
//  'S', 'e', 'n',  's',  'o',  'r', 'T', 'a', 'T',
  
//  'L', 'N', '-', 'W', 'i', 'C', ' ', 'I', 'n', 't', 
//  'u', 's', 'l', 'e', 'g', 'e', 'r', 'e', 'X',	 
	'W', 'i', 'C', 'o', 'r', 'e',
  // connection interval range
  0x05,   // length of this data
  GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE,
  LO_UINT16( DEFAULT_DESIRED_MIN_CONN_INTERVAL ),
  HI_UINT16( DEFAULT_DESIRED_MIN_CONN_INTERVAL ),
  LO_UINT16( DEFAULT_DESIRED_MAX_CONN_INTERVAL ),
  HI_UINT16( DEFAULT_DESIRED_MAX_CONN_INTERVAL ),

  // Tx power level
  0x02,   // length of this data
  GAP_ADTYPE_POWER_LEVEL,
  0       // 0dBm
};

// GAP - Advertisement data (max size = 31 bytes, though this is
// best kept short to conserve power while advertising)
static uint8_t advertData[] =
{
  // Flags; this sets the device to use limited discoverable
  // mode (advertises for 30 seconds at a time) instead of general
  // discoverable mode (advertises indefinitely)
  0x02,   // length of this data
  GAP_ADTYPE_FLAGS,
  DEFAULT_DISCOVERABLE_MODE | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

  // service UUID, to notify central devices what services are included
  // in this peripheral
  0x03,   // length of this data
  GAP_ADTYPE_16BIT_MORE,      // some of the UUID's, but not all
#ifdef FEATURE_LCD
  LO_UINT16(DISPLAY_SERV_UUID),
  HI_UINT16(DISPLAY_SERV_UUID),
#else
  LO_UINT16(MOVEMENT_SERV_UUID),
  HI_UINT16(MOVEMENT_SERV_UUID),
#endif

  // Manufacturer specific advertising data
  0x06,
  GAP_ADTYPE_MANUFACTURER_SPECIFIC,
  LO_UINT16(TI_COMPANY_ID),
  HI_UINT16(TI_COMPANY_ID),
  TI_ST_DEVICE_ID,
  TI_ST_KEY_DATA_ID,
  0x00                                    // Key state
};

// GAP GATT Attributes
static const uint8_t attDeviceName[GAP_DEVICE_NAME_LEN] = "WiCore 2.0";

#if FEATURE_OAD
// Event data from OAD profile.
static Queue_Struct oadQ;
static Queue_Handle hOadQ;
#endif //FEATURE_OAD

// Device information parameters
static const uint8_t devInfoModelNumber[] = "WiCore";
static const uint8_t devInfoNA[] =          "N.A.";
static const uint8_t devInfoFirmwareRev[] = FW_VERSION_STR;
static const uint8_t devInfoMfrName[] =     "Gengee";
static const uint8_t devInfoHardwareRev[] = "v1.4";

// Pins that are actively used by the application
static PIN_Config SensortagAppPinTable[] =
{
    Board_LED       | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,     /* LED initially off             */

    PIN_TERMINATE
};

/*******************************************************************************
 * LOCAL FUNCTIONS
 */

static void SensorTag_init( void);
static void SensorTag_taskFxn( UArg a0, UArg a1);
static void SensorTag_processStackMsg( ICall_Hdr *pMsg);
static void SensorTag_processGATTMsg(gattMsgEvent_t *pMsg);
static void SensorTag_processAppMsg( stEvt_t *pMsg);
static void SensorTag_processStateChangeEvt( gaprole_States_t newState ) ;
static void SensorTag_processCharValueChangeEvt( uint8_t serviceID, uint8_t paramID ) ;
static void SensorTag_performPeriodicTask( void);
static void SensorTag_stateChangeCB( gaprole_States_t newState);
static void SensorTag_clockHandler(UArg arg);
static void SensorTag_enqueueMsg(uint8_t event, uint8_t serviceID, uint8_t paramID);
static void SensorTag_callback(PIN_Handle handle, PIN_Id pinId);
static bool SensorTag_hasFactoryImage(void);
static void SensorTag_setDeviceInfo(void);

#ifdef FACTORY_IMAGE
static bool SensorTag_saveFactoryImage(void);
#endif

#ifdef FEATURE_OAD
static void SensorTag_processOadWriteCB(uint8_t event, uint16_t connHandle,
                                           uint8_t *pData);
#endif //FEATURE_OAD

/*******************************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static gapRolesCBs_t SensorTag_gapRoleCBs =
{
  SensorTag_stateChangeCB     // Profile State Change Callbacks
};

#ifdef FEATURE_OAD
static gapRolesParamUpdateCB_t paramUpdateCB =
{
  SensorTagConnControl_paramUpdateCB
};

static oadTargetCBs_t simpleBLEPeripheral_oadCBs =
{
  NULL,                       // Read Callback.  Optional.
  SensorTag_processOadWriteCB // Write Callback.  Mandatory.
};
#endif //FEATURE_OAD


/*******************************************************************************
 * PUBLIC FUNCTIONS
 */

/*******************************************************************************
 * @fn      SensorTag_createTask
 *
 * @brief   Task creation function for the SensorTag.
 *
 * @param   none
 *
 * @return  none
 */
void SensorTag_createTask(void)
{
  Task_Params taskParams;

  // Configure task
  Task_Params_init(&taskParams);
  taskParams.stack = sensorTagTaskStack;
  taskParams.stackSize = ST_TASK_STACK_SIZE;
  taskParams.priority = ST_TASK_PRIORITY;

  Task_construct(&sensorTagTask, SensorTag_taskFxn, &taskParams, NULL);
}


/*******************************************************************************
 * @fn      SensorTag_init
 *
 * @brief   Called during initialization and contains application
 *          specific initialization (ie. hardware initialization/setup,
 *          table initialization, power up notification, etc), and
 *          profile initialization/setup.
 *
 * @param   none
 *
 * @return  none
 */

uint8_t testBuf[4]={0};

static void SensorTag_init(void)
{

	// Handling of buttons, LED, relay
	hGpioPin = PIN_open(&pinGpioState, SensortagAppPinTable);
	PIN_registerIntCb(hGpioPin, SensorTag_callback );

	// Denote device restart by turning LED on for 1.5 seconds.
	PIN_setOutputValue(hGpioPin, Board_LED, Board_LED_ON);
	delay_ms(1500);
	PIN_setOutputValue(hGpioPin, Board_LED, Board_LED_OFF);

	userAppInit();
	
	
	// ***************************************************************************
	// N0 STACK API CALLS CAN OCCUR BEFORE THIS CALL TO ICall_registerApp
	// ***************************************************************************
	// Register the current thread as an ICall dispatcher application
	// so that the application can send and receive messages.
	ICall_registerApp(&selfEntity, &sem);

	// Create an RTOS queue for message from profile to be sent to app.
	appMsgQueue = Util_constructQueue(&appMsg);

	// Create one-shot clocks for internal periodic events.
	Util_constructClock(&periodicClock, SensorTag_clockHandler,
	                  ST_PERIODIC_EVT_PERIOD, 0, false, ST_PERIODIC_EVT);
	
	// Setup the GAP
	GAP_SetParamValue(TGAP_CONN_PAUSE_PERIPHERAL, DEFAULT_CONN_PAUSE_PERIPHERAL);

	// Setup the GAP Peripheral Role Profile
	{
		// For all hardware platforms, device starts advertising upon initialization
		uint8_t initialAdvertEnable = TRUE;

		// By setting this to zero, the device will go into the waiting state after
		// being discoverable for 30.72 second, and will not being advertising again
		// until the enabler is set back to TRUE
		uint16_t advertOffTime = 0;

		uint8_t enableUpdateRequest = DEFAULT_ENABLE_UPDATE_REQUEST;
		uint16_t desiredMinInterval = DEFAULT_DESIRED_MIN_CONN_INTERVAL;
		uint16_t desiredMaxInterval = DEFAULT_DESIRED_MAX_CONN_INTERVAL;
		uint16_t desiredSlaveLatency = DEFAULT_DESIRED_SLAVE_LATENCY;
		uint16_t desiredConnTimeout = DEFAULT_DESIRED_CONN_TIMEOUT;

		// Set the GAP Role Parameters
		GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t),
		                     &initialAdvertEnable);
		GAPRole_SetParameter(GAPROLE_ADVERT_OFF_TIME, sizeof(uint16_t),
		                     &advertOffTime);

		GAPRole_SetParameter(GAPROLE_SCAN_RSP_DATA, sizeof(scanRspData),
		                     scanRspData);
		GAPRole_SetParameter(GAPROLE_ADVERT_DATA, sizeof(advertData), advertData);

		GAPRole_SetParameter(GAPROLE_PARAM_UPDATE_ENABLE, sizeof(uint8_t),
		                     &enableUpdateRequest);
		GAPRole_SetParameter(GAPROLE_MIN_CONN_INTERVAL, sizeof(uint16_t),
		                     &desiredMinInterval);
		GAPRole_SetParameter(GAPROLE_MAX_CONN_INTERVAL, sizeof(uint16_t),
		                     &desiredMaxInterval);
		GAPRole_SetParameter(GAPROLE_SLAVE_LATENCY, sizeof(uint16_t),
		                     &desiredSlaveLatency);
		GAPRole_SetParameter(GAPROLE_TIMEOUT_MULTIPLIER, sizeof(uint16_t),
		                     &desiredConnTimeout);
	}

	// Set the GAP Characteristics
	GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN,
	               (void*)attDeviceName);

#ifdef FEATURE_OAD
	// Register connection parameter update
	GAPRole_RegisterAppCBs( &paramUpdateCB);
#endif

	// Set advertising interval
	{
		uint16_t advInt = DEFAULT_ADVERTISING_INTERVAL;

		GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MIN, advInt);
		GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MAX, advInt);
		GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, advInt);
		GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, advInt);
	}

	// Initialize GATT attributes
	GGS_AddService(GATT_ALL_SERVICES);           // GAP
	GATTServApp_AddService(GATT_ALL_SERVICES);   // GATT attributes
	DevInfo_AddService();                        // Device Information Service

	// Add application specific device information
	SensorTag_setDeviceInfo();

#ifdef FACTORY_IMAGE // �ж���
	// Check if a factory image exists and apply current image if necessary
	if (!SensorTag_hasFactoryImage())
	{
		SensorTag_saveFactoryImage();
	}  
#endif

	SensorTagMov_init();                            // Movement processor

#ifdef FEATURE_REGISTER_SERVICE
	Register_addService();                          // Generic register access
#endif


#ifdef FEATURE_OAD
	SensorTagConnectionControl_init();              // Connection control to
	                                              // support OAD for iOs/Android
	OAD_addService();                               // OAD Profile
	OAD_register((oadTargetCBs_t *)&simpleBLEPeripheral_oadCBs); // ֻ����128bit��UUID �����õ�
	hOadQ = Util_constructQueue(&oadQ);
#endif

	// Start the Device
	GAPRole_StartDevice(&SensorTag_gapRoleCBs);

	// Start Bond Manager
	VOID GAPBondMgr_Register(NULL);
}

/*******************************************************************************
 * @fn      SensorTag_taskFxn
 *
 * @brief   Application task entry point for the SensorTag
 *
 * @param   a0, a1 (not used)
 *
 * @return  none
 */
static void SensorTag_taskFxn(UArg a0, UArg a1)
{
	// Initialize application
	SensorTag_init();

	// Application main loop
	for (;;)
	{
		// Waits for a signal to the semaphore associated with the calling thread.
		// Note that the semaphore associated with a thread is signalled when a
		// message is queued to the message receive queue of the thread or when
		// ICall_signal() function is called onto the semaphore.
		ICall_Errno errno = ICall_wait(ICALL_TIMEOUT_FOREVER);

		if (errno == ICALL_ERRNO_SUCCESS)
		{
			ICall_EntityID dest;
			ICall_ServiceEnum src;
			ICall_HciExtEvt *pMsg = NULL;

			if (ICall_fetchServiceMsg(&src, &dest,
			                        (void **)&pMsg) == ICALL_ERRNO_SUCCESS)
			{
				if ((src == ICALL_SERVICE_CLASS_BLE) && (dest == selfEntity))
				{
					// Process inter-task message
					SensorTag_processStackMsg((ICall_Hdr *)pMsg);
				}
				if (pMsg)
				{
					ICall_freeMsg(pMsg);
				}
			}

			// If RTOS queue is not empty, process app message.
			while (!Queue_empty(appMsgQueue))
			{
				stEvt_t *pMsg = (stEvt_t *)Util_dequeueMsg(appMsgQueue);
				if (pMsg)
				{
					// Process message.
					SensorTag_processAppMsg(pMsg);

					// Free the space from the message.
					ICall_free(pMsg);
				}
			}
			
		}

		if (!!(events & ST_PERIODIC_EVT))
		{
			events &= ~ST_PERIODIC_EVT;

			if (gapProfileState == GAPROLE_CONNECTED
			  || gapProfileState == GAPROLE_ADVERTISING)
			{

				Util_startClock(&periodicClock);
			}

			// Perform periodic application task
			if (gapProfileState == GAPROLE_CONNECTED)
			{
				SensorTag_performPeriodicTask();
			}

			// Blink green LED when advertising
			if (gapProfileState == GAPROLE_ADVERTISING)
			{
			//	OLED_testShowGengeeRoll(3);
				uartWriteDebug("�㲥", 4);
				SensorTag_blinkLed(Board_LED,1);
			}
		}

#ifdef FEATURE_OAD  // �ж���
		while (!Queue_empty(hOadQ))
		{
			oadTargetWrite_t *oadWriteEvt = Queue_dequeue(hOadQ);

			// Identify new image.
			if (oadWriteEvt->event == OAD_WRITE_IDENTIFY_REQ)
			{
				OAD_imgIdentifyWrite(oadWriteEvt->connHandle,
							  oadWriteEvt->pData);
			}
			// Write a next block request.
			else if (oadWriteEvt->event == OAD_WRITE_BLOCK_REQ)
			{
				OAD_imgBlockWrite(oadWriteEvt->connHandle,
			                  oadWriteEvt->pData);
			}

			// Free buffer.
			ICall_free(oadWriteEvt);
		}
#endif //FEATURE_OAD
		userAppPro();
	} // task loop
}


/*******************************************************************************
 * @fn      SensorTag_setDeviceInfo
 *
 * @brief   Set application specific Device Information
 *
 * @param   none
 *
 * @return  none
 */
static void SensorTag_setDeviceInfo(void)
{
  DevInfo_SetParameter(DEVINFO_MODEL_NUMBER, sizeof(devInfoModelNumber),
                       (void*)devInfoModelNumber);
  DevInfo_SetParameter(DEVINFO_SERIAL_NUMBER, sizeof(devInfoNA),
                       (void*)devInfoNA);
  DevInfo_SetParameter(DEVINFO_SOFTWARE_REV, sizeof(devInfoNA),
                       (void*)devInfoNA);
  DevInfo_SetParameter(DEVINFO_FIRMWARE_REV, sizeof(devInfoFirmwareRev),
                       (void*)devInfoFirmwareRev);
  DevInfo_SetParameter(DEVINFO_HARDWARE_REV, sizeof(devInfoHardwareRev),
                       (void*)devInfoHardwareRev);
  DevInfo_SetParameter(DEVINFO_MANUFACTURER_NAME, sizeof(devInfoMfrName),
                       (void*)devInfoMfrName);
}

/*******************************************************************************
 * @fn      SensorTag_processAppMsg
 *
 * @brief   Process an incoming callback from a profile.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void SensorTag_processAppMsg(stEvt_t *pMsg)
{
  switch (pMsg->event)
  {
    case ST_STATE_CHANGE_EVT:
      SensorTag_processStateChangeEvt((gaprole_States_t)pMsg->serviceID);
      break;

    case ST_CHAR_CHANGE_EVT:
      SensorTag_processCharValueChangeEvt(pMsg->serviceID, pMsg->paramID);
      break;

    default:
      // Do nothing.
      break;
  }
}

/*******************************************************************************
 * @fn      SensorTag_stateChangeCB
 *
 * @brief   Callback from GAP Role indicating a role state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void SensorTag_stateChangeCB(gaprole_States_t newState)
{
  SensorTag_enqueueMsg(ST_STATE_CHANGE_EVT, newState, NULL);
}

/*******************************************************************************
 * @fn      SensorTag_processStateChangeEvt
 *
 * @brief   Process a pending GAP Role state change event.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void SensorTag_processStateChangeEvt(gaprole_States_t newState)
{
#ifdef PLUS_BROADCASTER
  static bool firstConnFlag = false;
#endif // PLUS_BROADCASTER

  switch (newState)
  {
  case GAPROLE_STARTED:
    {
      uint8_t ownAddress[B_ADDR_LEN];
      uint8_t systemId[DEVINFO_SYSTEM_ID_LEN];

//      SensorTag_blinkLed(Board_LED2, 5);

      GAPRole_GetParameter(GAPROLE_BD_ADDR, ownAddress);

      // use 6 bytes of device address for 8 bytes of system ID value
      systemId[0] = ownAddress[0];
      systemId[1] = ownAddress[1];
      systemId[2] = ownAddress[2];

      // set middle bytes to zero
      systemId[4] = 0x00;
      systemId[3] = 0x00;

      // shift three bytes up
      systemId[7] = ownAddress[5];
      systemId[6] = ownAddress[4];
      systemId[5] = ownAddress[3];

      DevInfo_SetParameter(DEVINFO_SYSTEM_ID, DEVINFO_SYSTEM_ID_LEN, systemId);
      uartWriteDebug("Initialized", 11);
    }
    break;

  case GAPROLE_ADVERTISING:
    // Start the clock
    if (!Util_isActive(&periodicClock))
    {
      Util_startClock(&periodicClock);
    }

    // Make sure key presses are not stuck
    sensorTag_updateAdvertisingData(0);

    uartWriteDebug("Advertising", 11);
    break;

  case GAPROLE_CONNECTED:
    {
      // Start the clock
      if (!Util_isActive(&periodicClock))
      {
        Util_startClock(&periodicClock);
      }

      // Turn of LEDs and buzzer
      PIN_setOutputValue(hGpioPin, Board_LED, Board_LED_OFF);
#ifdef FEATURE_OAD
      SensorTagConnectionControl_update();
#endif

    }
  
    uartWriteDebug("Connected", 9);
	OLED_showBluetoothBmp(0, 0);
	
    break;

  case GAPROLE_CONNECTED_ADV:
    break;
  case GAPROLE_WAITING:
  	uartWriteDebug("WAITING", 7);
	break;
  case GAPROLE_WAITING_AFTER_TIMEOUT:
  		uartWriteDebug("TIMEOUT", 7);
		//SensorTag_resetAllSensors();
		//LCD_WRITES_STATUS("Waiting...");
    break;

  case GAPROLE_ERROR:
    //SensorTag_resetAllSensors();
    //PIN_setOutputValue(hGpioPin,Board_LED1, Board_LED_ON);
    OLED_ShowString(0, 48, "Bluetooth Error");
    break;

  default:
    break;
  }

  gapProfileState = newState;
}

/*******************************************************************************
 * @fn      SensorTag_charValueChangeCB
 *
 * @brief   Callback from Sensor Profile indicating a characteristic
 *          value change.
 *
 * @param   paramID - parameter ID of the value that was changed.
 *
 * @return  none
 */
void SensorTag_charValueChangeCB(uint8_t serviceID, uint8_t paramID)
{
  SensorTag_enqueueMsg(ST_CHAR_CHANGE_EVT, serviceID, paramID);
}

/*******************************************************************************
 * @fn      SensorTag_blinkLed
 *
 * @brief   Blinks a led 'n' times, duty-cycle 50-50
 * @param   led - led identifier
 * @param   nBlinks - number of blinks
 *
 * @return  none
 */
void SensorTag_blinkLed(uint8_t led, uint8_t nBlinks)
{
  uint8_t i;

  for (i=0; i<nBlinks; i++)
  {
    PIN_setOutputValue(hGpioPin, led, Board_LED_ON);
    delay_ms(BLINK_DURATION);
    PIN_setOutputValue(hGpioPin, led, Board_LED_OFF);
    delay_ms(BLINK_DURATION);
  }
}

/*******************************************************************************
 * @fn      SensorTag_applyFactoryImage
 *
 * @brief   Load the factory image from external flash and reboot
 *
 * @return  none
 */
void SensorTag_applyFactoryImage(void)
{
  if (SensorTag_hasFactoryImage())
  {
    // Load and launch factory image; page 31 must be omitted
    ((void (*)(uint32_t, uint32_t, uint32_t))BL_OFFSET)(EFL_ADDR_RECOVERY,
                                                        EFL_SIZE_RECOVERY-0x1000,
                                                        0);
  }
}

/*******************************************************************************
 * @fn      SensorTag_processCharValueChangeEvt
 *
 * @brief   Process pending Profile characteristic value change
 *          events. The events are generated by the network task (BLE)
 *
 * @param   serviceID - ID of the affected service
 * @param   paramID - ID of the affected parameter
 *
 * @return  none
 */
static void SensorTag_processCharValueChangeEvt(uint8_t serviceID,
                                                uint8_t paramID)
{
  switch (serviceID)
  {
  case SERVICE_ID_MOV:
    SensorTagMov_processCharChangeEvt(paramID);
    break;
#ifdef FEATURE_OAD
  case SERVICE_ID_CC:
    SensorTagConnControl_processCharChangeEvt(paramID);
    break;
#endif

  default:
    break;
  }
}

/*******************************************************************************
 * @fn      SensorTag_processStackMsg
 *
 * @brief   Process an incoming stack message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void SensorTag_processStackMsg(ICall_Hdr *pMsg)
{
  switch (pMsg->event)
  {
    case GATT_MSG_EVENT:
      // Process GATT message
      SensorTag_processGATTMsg((gattMsgEvent_t *)pMsg);
      break;

    default:
      // do nothing
      break;
  }
}

/*******************************************************************************
 * @fn      SensorTag_processGATTMsg
 *
 * @brief   Process GATT messages
 *
 * @return  none
 */
static void SensorTag_processGATTMsg(gattMsgEvent_t *pMsg)
{
  GATT_bm_free(&pMsg->msg, pMsg->method);
}

/*******************************************************************************
 * @fn      SensorTag_performPeriodicTask
 *
 * @brief   Perform a periodic application task.
 *
 * @param   none
 *
 * @return  none
 */
static void SensorTag_performPeriodicTask(void)
{
#ifdef FEATURE_REGISTER_SERVICE
  // Force notification on Register Data (if enabled)
  Register_setParameter(SENSOR_DATA,0,NULL);
#endif
}

#if FEATURE_OAD
/*******************************************************************************
 * @fn      SensorTag_processOadWriteCB
 *
 * @brief   Process a write request to the OAD profile.
 *
 * @param   event      - event type:
 *                       OAD_WRITE_IDENTIFY_REQ
 *                       OAD_WRITE_BLOCK_REQ
 * @param   connHandle - the connection Handle this request is from.
 * @param   pData      - pointer to data for processing and/or storing.
 *
 * @return  None.
 */
static void SensorTag_processOadWriteCB(uint8_t event, uint16_t connHandle,
                                           uint8_t *pData)
{
  oadTargetWrite_t *oadWriteEvt = ICall_malloc( sizeof(oadTargetWrite_t) + \
                                             sizeof(uint8_t) * OAD_PACKET_SIZE);

  if ( oadWriteEvt != NULL )
  {
    oadWriteEvt->event = event;
    oadWriteEvt->connHandle = connHandle;

    oadWriteEvt->pData = (uint8_t *)(&oadWriteEvt->pData + 1);
    memcpy(oadWriteEvt->pData, pData, OAD_PACKET_SIZE);

    Queue_enqueue(hOadQ, (Queue_Elem *)oadWriteEvt);

    // Post the application's semaphore.
    Semaphore_post(sem);
  }
  else
  {
    // Fail silently.
  }

}
#endif //FEATURE_OAD

/*******************************************************************************
 * @fn      SensorTag_clockHandler
 *
 * @brief   Handler function for clock time-outs.
 *
 * @param   arg - event type
 *
 * @return  none
 */
static void SensorTag_clockHandler(UArg arg)
{
  // Store the event.
  events |= arg;

  // Wake up the application.
  Semaphore_post(sem);
}

/*******************************************************************************
 * @fn      SensorTag_enqueueMsg
 *
 * @brief   Creates a message and puts the message in RTOS queue.
 *
 * @param   event - message event.
 * @param   serviceID - service identifier
 * @param   paramID - parameter identifier
 *
 * @return  none
 */
static void SensorTag_enqueueMsg(uint8_t event, uint8_t serviceID, uint8_t paramID)
{
  stEvt_t *pMsg;

  // Create dynamic pointer to message.
  if (pMsg = ICall_malloc(sizeof(stEvt_t)))
  {
    pMsg->event = event;
    pMsg->serviceID = serviceID;
    pMsg->paramID = paramID;

    // Enqueue the message.
    Util_enqueueMsg(appMsgQueue, sem, (uint8_t*)pMsg);
  }
}


/*********************************************************************
 * @fn      SensorTag_resetAllSensors
 *
 * @brief   Reset all sensors, typically when a connection is intentionally
 *          terminated.
 *
 * @param   none
 *
 * @return  none
 */

/*!*****************************************************************************
 *  @fn         SensorTag_callback
 *
 *  Interrupt service routine for buttons, relay and MPU
 *
 *  @param      handle PIN_Handle connected to the callback
 *
 *  @param      pinId  PIN_Id of the DIO triggering the callback
 *
 *  @return     none
 ******************************************************************************/
static void SensorTag_callback(PIN_Handle handle, PIN_Id pinId)
{
  switch (pinId) {
    
  case Board_LED:
    break;
  default:
    /* Do nothing */
    break;
  }
}

/*******************************************************************************
 * @fn      SensorTag_checkBatteryInfo
 *
 * @brief   Handle periodical task to check battery information.
 *
 * @param   charge - battery charge.
 *
 * @return  none
 * user add tqy
 */
void SensorTag_checkBatteryInfo(uint8_t charge)
{
	if ((GAPROLE_ADVERTISING == gapProfileState) || (GAPROLE_CONNECTED == gapProfileState))
	{
		SensorTagMov_updateBatteryInfo(charge); 
	}

}

/*******************************************************************************
 * @fn      sensorTag_updateAdvertisingData
 *
 * @brief   Update the advertising data with the latest key press status
 *
 * @return  none
 */
void sensorTag_updateAdvertisingData(uint8_t keyStatus)
{
  // Record key state in advertising data
  advertData[KEY_STATE_OFFSET] = keyStatus;
  GAPRole_SetParameter(GAPROLE_ADVERT_DATA, sizeof(advertData), advertData);
}


#ifdef FACTORY_IMAGE
/*******************************************************************************
 * @fn      SensorTag_saveFactoryImage
 *
 * @brief   Save the current image to external flash as a factory image
 *
 * @return  none
 */
static bool SensorTag_saveFactoryImage(void)
{
  bool success;

  success = extFlashOpen();

  if (success)
  {
    uint32_t address;

    // Erase external flash
    for (address= 0; address<EFL_FLASH_SIZE; address+=EFL_PAGE_SIZE)
    {
      extFlashErase(address,EFL_PAGE_SIZE);
    }

    // Install factory image
    for (address=0; address<EFL_SIZE_RECOVERY && success; address+=EFL_PAGE_SIZE)
    {
      success = extFlashErase(EFL_ADDR_RECOVERY+address, EFL_PAGE_SIZE);
      if (success)
      {
        size_t offset;
        static uint8_t buf[256]; // RAM storage needed due to SPI/DMA limitation

        for (offset=0; offset<EFL_PAGE_SIZE; offset+=sizeof(buf))
        {
          const uint8_t *pIntFlash;

          // Copy from internal to external flash
          pIntFlash = (const uint8_t*)address + offset;
          memcpy(buf,pIntFlash,sizeof(buf));
          success = extFlashWrite(EFL_ADDR_RECOVERY+address+offset,
                                  sizeof(buf), buf);

          // Verify first few bytes
          if (success)
          {
            extFlashRead(EFL_ADDR_RECOVERY+address+offset, sizeof(buf), buf);
            success = buf[2] == pIntFlash[2] && buf[3] == pIntFlash[3];
          }
        }
      }
    }

    extFlashClose();
  }

  return success;
}
#endif

/*******************************************************************************
 * @fn      SensorTag_hasFactoryImage
 *
 * @brief   Determine if the SensorTag has a pre-programmed factory image
 *          in external flash. Criteria for deciding if a factory image is
 *          present is that the reset vector stored in external flash is valid.
 *
 * @return  none
 */
static bool SensorTag_hasFactoryImage(void)
{
  bool valid;

  valid = extFlashOpen();

  if (valid)
  {
    uint16_t buffer[2];

    // 1. Check reset vector
    valid = extFlashRead(EFL_ADDR_RECOVERY,sizeof(buffer),(uint8_t*)buffer);
    if (valid)
    {
      valid = (buffer[0] != 0xFFFF && buffer[1] != 0xFFFF) &&
        (buffer[0] != 0x0000 && buffer[1] != 0x0000);
    }

    extFlashClose();
  }

  return valid;
}

/*******************************************************************************
*******************************************************************************/
