/*******************************************************************************
  Filename:       keyfobdemo.c
  Revised:        $Date: 2015-07-20 11:31:07 -0700 (Mon, 20 Jul 2015) $
  Revision:       $Revision: 44370 $

  Description:    Key Fob Demo Application for CC26xx.

  Copyright 2009 - 2015 Texas Instruments Incorporated. All rights reserved.

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
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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
*******************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <string.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>

#include "gatt.h"
#include "hci.h"

#include "bcomdef.h"
#include "gapgattserver.h"
#include "gattservapp.h"
#include "gatt_profile_uuid.h"
#include "devinfoservice.h"
#include "proxreporter.h"
#include "battservice.h"
#include "accelerometer.h"
#include "simplekeys.h"

#include "buzzer.h"
#include "bma250.h"

#include "peripheral.h"
#include "gapbondmgr.h"

#include "osal_snv.h"
#include "ICallBleAPIMSG.h"

#include "util.h"
#include "board_key.h"
#include "Board.h"

#include "keyfobdemo.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
// Number of beeps before buzzer stops by itself
#define BUZZER_MAX_BEEPS                      10

// Buzzer beep tone frequency for "High Alert" (in Hz)
#define BUZZER_ALERT_HIGH_FREQ                4096

// Buzzer beep tone frequency for "Low Alert" (in Hz)
#define BUZZER_ALERT_LOW_FREQ                 250

// How often to check battery voltage (in ms)
#define BATTERY_CHECK_PERIOD                  10000

// How often (in ms) to read the accelerometer
#define ACCEL_READ_PERIOD                     50

// Minimum change in accelerometer before sending a notification
#define ACCEL_CHANGE_THRESHOLD                5

//GAP Peripheral Role desired connection parameters

// Use limited discoverable mode to advertise for 30.72s, and then stop, or
// use general discoverable mode to advertise indefinitely
#define DEFAULT_DISCOVERABLE_MODE             GAP_ADTYPE_FLAGS_LIMITED
//#define DEFAULT_DISCOVERABLE_MODE             GAP_ADTYPE_FLAGS_GENERAL

// Minimum connection interval (units of 1.25ms, 80=100ms) if automatic 
// parameter update request is enabled
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     80

// Maximum connection interval (units of 1.25ms, 800=1000ms) if automatic 
// parameter update request is enabled
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     800

// Slave latency to use if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SLAVE_LATENCY         0

// Supervision timeout value (units of 10ms, 1000=10s) if automatic parameter 
// update request is enabled
#define DEFAULT_DESIRED_CONN_TIMEOUT          1000

// Whether to enable automatic parameter update request when a connection is 
// formed
#define DEFAULT_ENABLE_UPDATE_REQUEST         TRUE

// Connection Pause Peripheral time value (in seconds)
#define DEFAULT_CONN_PAUSE_PERIPHERAL         5

/* keyfobProximityState values */
// Advertising after initialization or due to terminated link
#define KEYFOB_PROXSTATE_INITIALIZED          0   

// Connected and "within range" of the master, as defined by proximity profile
#define KEYFOB_PROXSTATE_CONNECTED_IN_RANGE   1  

// Connected and "out of range" of the master, as defined by proximity profile
#define KEYFOB_PROXSTATE_PATH_LOSS            2  

// Disconnected as a result of a supervision timeout
#define KEYFOB_PROXSTATE_LINK_LOSS            3   

// buzzer_state values
#define BUZZER_OFF                            0
#define BUZZER_ON                             1

// keyfobAlertState values
#define ALERT_STATE_OFF                       0
#define ALERT_STATE_LOW                       1
#define ALERT_STATE_HIGH                      2

#define INVALID_CONNHANDLE                    0xFFFF

#if defined (PLUS_BROADCASTER)
  #define ADV_IN_CONN_WAIT                    500 // delay 500 ms
#endif

// Task configuration
#define KFD_TASK_PRIORITY                     1

#ifndef KFD_TASK_STACK_SIZE
#define KFD_TASK_STACK_SIZE                   644
#endif

// Key Fob Demo task internal events.
#define KFD_STATE_CHANGE_EVT                  0x0001
#define KFD_BATTERY_CHECK_EVT                 0x0002
#define KFD_ACCEL_CHANGE_EVT                  0x0004
#define KFD_ACCEL_READ_EVT                    0x0008
#define KFD_TOGGLE_BUZZER_EVT                 0x0010
#define KFD_ADV_IN_CONNECTION_EVT             0x0020
#define KFD_POWERON_LED_TIMEOUT_EVT           0x0040
#define KFD_KEY_CHANGE_EVT                    0x0080
#define KFD_PROXIMITY_EVT                     0x0100

/*********************************************************************
 * TYPEDEFS
 */

// App event passed from profiles.
typedef struct
{
  appEvtHdr_t hdr; // Event header
} kfdEvt_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

// Entity ID globally used to check for source and/or destination of messages
static ICall_EntityID selfEntity;

// Semaphore globally used to post events to the application thread
static ICall_Semaphore sem;

// Clock instances for internal periodic events.
static Clock_Struct batteryCheckClock;
static Clock_Struct ledTimeoutClock;
static Clock_Struct accelReadClock;
static Clock_Struct toggleBuzzerClock;

#ifdef PLUS_BROADCASTER
static Clock_Struct PB_advInConnWaitClock;
#endif //PLUS_BROADCASTER

// Queue object used for app messages
static Queue_Struct appMsg;
static Queue_Handle appMsgQueue;

// events flag for internal application events.
static uint16_t events;

// Task configuration
Task_Struct kfdTask;
Char kfdTaskStack[KFD_TASK_STACK_SIZE];

static gaprole_States_t gapProfileState = GAPROLE_INIT;

// Proximity State Variables
static uint8_t keyfobProxLLAlertLevel = PP_ALERT_LEVEL_NO; // Link Loss Alert
static uint8_t keyfobProxIMAlertLevel = PP_ALERT_LEVEL_NO; // Link Loss Alert
static int8_t  keyfobProxTxPwrLevel = 0;  // Tx Power Level (0dBm default)

// keyfobProximityState is the current state of the device
static uint8_t keyfobProximityState;

static uint8_t keyfobAlertState;

// GAP - SCAN RSP data (max size = 31 bytes)
static uint8_t deviceName[] =
{
  // complete name
  0x0b,   // length of first data structure (11 bytes excluding length byte)
  0x09,   // AD Type = Complete local name
  0x4b,   // 'K'
  0x65,   // 'e'
  0x79,   // 'y'
  0x66,   // 'f'
  0x6f,   // 'o'
  0x62,   // 'b'
  0x64,   // 'd'
  0x65,   // 'e'
  0x6d,   // 'm'
  0x6f,   // 'o'
};

// GAP - Advertisement data (max size = 31 bytes, though this is
// best kept short to conserve power while advertisting)
static uint8_t advertData[] =
{
  0x02,   // length of first data structure (2 bytes excluding length byte)
  GAP_ADTYPE_FLAGS,   // AD Type = Flags
  DEFAULT_DISCOVERABLE_MODE | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

  // service UUID, to notify central devices what services are included
  // in this peripheral
  0x07,   // length of second data structure (7 bytes excluding length byte)
  GAP_ADTYPE_16BIT_MORE, // list of 16-bit UUID's available, but not complete list
  LO_UINT16(LINK_LOSS_SERV_UUID),       // Link Loss Service (Proximity Profile)
  HI_UINT16(LINK_LOSS_SERV_UUID),
  LO_UINT16(IMMEDIATE_ALERT_SERV_UUID), // Immediate Alert Service (Proximity/Find Me Profile)
  HI_UINT16(IMMEDIATE_ALERT_SERV_UUID),
  LO_UINT16(TX_PWR_LEVEL_SERV_UUID),    // Tx Power Level Service (Proximity Profile)
  HI_UINT16(TX_PWR_LEVEL_SERV_UUID)
};

// GAP GATT Attributes
static uint8_t attDeviceName[GAP_DEVICE_NAME_LEN] = "TI BLE Keyfob";
  
// Buzzer state
static uint8_t buzzer_state = BUZZER_OFF;
static uint8_t buzzer_beep_count = 0;

// Accelerometer Profile Parameters
static uint8_t accelEnabler = FALSE;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void KeyFobDemo_init(void);
static void KeyFobDemo_taskFxn(UArg a0, UArg a1);

static void KeyFobDemo_processStackMsg(ICall_Hdr *pMsg);
static void KeyFobDemo_processGATTMsg(gattMsgEvent_t *pMsg);
static void KeyFobDemo_processAppMsg(kfdEvt_t *pMsg);
static void KeyFobDemo_enqueueMsg(uint16_t event, uint8_t state);
static void KeyFobDemo_clockHandler(UArg arg);
static void KeyFobDemo_keyPressHandler(uint8_t keys);

static void KeyFobDemo_performAlert(void);
static void KeyFobDemo_stopAlert(void);
static void KeyFobDemo_handleKeys(uint8_t shift, uint8_t keys);
static void KeyFobDemo_stateChangeCB(gaprole_States_t newState);
static void KeyFobDemo_processStateChangeEvt(gaprole_States_t newState);
static void KeyFobDemo_proximityAttrCB(uint8_t attrParamID);
static void KeyFobDemo_processProximityAttrEvt(uint8_t attrParamID);
static void KeyFobDemo_processToggleBuzzerEvt(void);

static void KeyFobDemo_accelEnablerChangeCB(void);
static void KeyFobDemo_processAccelEnablerChangeEvt();
static void KeyFobDemo_processAccelReadEvt(void);
static void KeyFobDemo_accelRead(void);

/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static gapRolesCBs_t KeyFobDemo_peripheralCBs =
{
  KeyFobDemo_stateChangeCB  // Profile State Change Callbacks 
};

// GAP Bond Manager Callbacks
static gapBondCBs_t KeyFobDemo_bondMgrCBs =
{
  NULL,       // Passcode callback (not used by application)
  NULL        // Pairing / Bonding state Callback (not used by application)
};

// Proximity Peripheral Profile Callbacks
static proxReporterCBs_t KeyFobDemo_proximityCBs =
{
  KeyFobDemo_proximityAttrCB  // Whenever the Link Loss Alert attribute changes
};

// Accelerometer Profile Callbacks
static accelCBs_t KeyFobDemo_accelCBs =
{
  KeyFobDemo_accelEnablerChangeCB  // Called when Enabler attribute changes
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      KeyFobDemo_createTask
 *
 * @brief   Task creation function for the Key Fob Demo.
 *
 * @param   none
 *
 * @return  none
 */
void KeyFobDemo_createTask(void)
{
  Task_Params taskParams;
    
  // Configure task
  Task_Params_init(&taskParams);
  taskParams.stack = kfdTaskStack;
  taskParams.stackSize = KFD_TASK_STACK_SIZE;
  taskParams.priority = KFD_TASK_PRIORITY;
  
  Task_construct(&kfdTask, KeyFobDemo_taskFxn, &taskParams, NULL);
}

/*********************************************************************
 * @fn      KeyFobDemo_init
 *
 * @brief   Called during initialization and contains application
 *          specific initialization (ie. hardware initialization/setup,
 *          table initialization, power up notification, etc.), and 
 *          profile initialization/setup.
 *
 * @param   None.
 *
 * @return  None.
 */
static void KeyFobDemo_init(void)
{
	// ******************************************************************
  // N0 STACK API CALLS CAN OCCUR BEFORE THIS CALL TO ICall_registerApp
  // ******************************************************************
  // Register the current thread as an ICall dispatcher application
  // so that the application can send and receive messages.
  ICall_registerApp(&selfEntity, &sem);
    
  // Hard code the BD Address till CC2650 board gets its own IEEE address
  //uint8_t bdAddress[B_ADDR_LEN] = { 0xDA, 0xDA, 0x22, 0x22, 0x12, 0x6A };
  //HCI_EXT_SetBDADDRCmd(bdAddress);
  
  // Set device's Sleep Clock Accuracy
  //HCI_EXT_SetSCACmd(40);
  
  // Create an RTOS queue for message from profile to be sent to app.
  appMsgQueue = Util_constructQueue(&appMsg);
  
  // Create one-shot clocks for internal periodic events.
  Util_constructClock(&batteryCheckClock, KeyFobDemo_clockHandler,
                      BATTERY_CHECK_PERIOD, 0, false, KFD_BATTERY_CHECK_EVT);
  
  Util_constructClock(&ledTimeoutClock, KeyFobDemo_clockHandler,
                      1000, 0, false, KFD_POWERON_LED_TIMEOUT_EVT);
  
  Util_constructClock(&accelReadClock, KeyFobDemo_clockHandler,
                      ACCEL_READ_PERIOD, 0, false, KFD_ACCEL_READ_EVT);
  
  Util_constructClock(&toggleBuzzerClock, KeyFobDemo_clockHandler,
                      200, 800, false, KFD_TOGGLE_BUZZER_EVT);  

#if defined (PLUS_BROADCASTER)
  Util_constructClock(&PB_advInConnWaitClock, KeyFobDemo_clockHandler,
                      ADV_IN_CONN_WAIT, 0, false, KFD_ADV_IN_CONNECTION_EVT);  
#endif //PLUS_BROADCASTER

  // Initialize keys on the SRF06.
  Board_initKeys(KeyFobDemo_keyPressHandler);
  
  // Setup the GAP
  GAP_SetParamValue(TGAP_CONN_PAUSE_PERIPHERAL, DEFAULT_CONN_PAUSE_PERIPHERAL);
  
  // Setup the GAP Peripheral Role Profile
  {
    // For all hardware platforms, device starts advertising upon initialization
    uint8_t initial_advertising_enable = TRUE;
    
    // By setting this to zero, the device will go into the waiting state after
    // being discoverable for 30.72 second, and will not being advertising again
    // until the enabler is set back to TRUE
    uint16_t gapRole_AdvertOffTime = 0;
  
    uint8_t enable_update_request = DEFAULT_ENABLE_UPDATE_REQUEST;
    uint16_t desired_min_interval = DEFAULT_DESIRED_MIN_CONN_INTERVAL;
    uint16_t desired_max_interval = DEFAULT_DESIRED_MAX_CONN_INTERVAL;
    uint16_t desired_slave_latency = DEFAULT_DESIRED_SLAVE_LATENCY;
    uint16_t desired_conn_timeout = DEFAULT_DESIRED_CONN_TIMEOUT;
   
    // Set the GAP Role Parameters
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), 
                         &initial_advertising_enable);

    GAPRole_SetParameter(GAPROLE_ADVERT_OFF_TIME, sizeof(uint16_t),
                         &gapRole_AdvertOffTime);
  
    GAPRole_SetParameter(GAPROLE_SCAN_RSP_DATA, sizeof (deviceName),
                         deviceName);
    GAPRole_SetParameter(GAPROLE_ADVERT_DATA, sizeof(advertData), advertData);
  
    GAPRole_SetParameter(GAPROLE_PARAM_UPDATE_ENABLE, sizeof(uint8_t),
                         &enable_update_request);
    GAPRole_SetParameter(GAPROLE_MIN_CONN_INTERVAL, sizeof(uint16_t), 
                         &desired_min_interval);
    GAPRole_SetParameter(GAPROLE_MAX_CONN_INTERVAL, sizeof(uint16_t), 
                         &desired_max_interval);
    GAPRole_SetParameter(GAPROLE_SLAVE_LATENCY, sizeof(uint16_t), 
                         &desired_slave_latency);
    GAPRole_SetParameter(GAPROLE_TIMEOUT_MULTIPLIER, sizeof(uint16_t),
                         &desired_conn_timeout);
  }
  
  // Set the GAP Attributes
  GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, attDeviceName);

  // Setup the GAP Bond Manager
  {
    uint8_t pairMode = GAPBOND_PAIRING_MODE_WAIT_FOR_REQ;
    uint8_t mitm = TRUE;
    uint8_t ioCap = GAPBOND_IO_CAP_DISPLAY_ONLY;
    uint8_t bonding = TRUE;
    
    GAPBondMgr_SetParameter(GAPBOND_PAIRING_MODE, sizeof (uint8_t), &pairMode);
    GAPBondMgr_SetParameter(GAPBOND_MITM_PROTECTION, sizeof (uint8_t), &mitm);
    GAPBondMgr_SetParameter(GAPBOND_IO_CAPABILITIES, sizeof (uint8_t), &ioCap);
    GAPBondMgr_SetParameter(GAPBOND_BONDING_ENABLED, sizeof (uint8_t), &bonding);
  }

  // Initialize GATT attributes
  GGS_AddService(GATT_ALL_SERVICES);          // GAP
  GATTServApp_AddService(GATT_ALL_SERVICES);  // GATT attributes
  DevInfo_AddService();                       // Device Information Service
  ProxReporter_AddService(GATT_ALL_SERVICES); // Proximity Reporter Profile
  Batt_AddService();                          // Battery Service
  Accel_AddService(GATT_ALL_SERVICES);        // Accelerometer Profile
  SK_AddService(GATT_ALL_SERVICES);           // Simple Keys Profile

  keyfobProximityState = KEYFOB_PROXSTATE_INITIALIZED;

  // Initialize Tx Power Level characteristic in Proximity Reporter
  {
    int8_t initialTxPowerLevel = 0;
    
    ProxReporter_SetParameter(PP_TX_POWER_LEVEL, sizeof (int8_t), 
                              &initialTxPowerLevel);
  }

  keyfobAlertState = ALERT_STATE_OFF;

  // Make sure buzzer is off.
  Buzzer_stop();

  // Make sure LEDs are off.
  //HalLedSet((HAL_LED_1 | HAL_LED_2), HAL_LED_MODE_OFF);

  // Start the Proximity Profile
  VOID ProxReporter_RegisterAppCBs(&KeyFobDemo_proximityCBs);

  // Start the Accelerometer Profile
  VOID Accel_RegisterAppCBs(&KeyFobDemo_accelCBs);

  // Set the proximity attribute values to default
  ProxReporter_SetParameter(PP_LINK_LOSS_ALERT_LEVEL, sizeof(uint8_t), 
                            &keyfobProxLLAlertLevel);
  ProxReporter_SetParameter(PP_IM_ALERT_LEVEL, sizeof(uint8_t), 
                            &keyfobProxIMAlertLevel);
  ProxReporter_SetParameter(PP_TX_POWER_LEVEL, sizeof(int8_t), 
                            &keyfobProxTxPwrLevel);

  // Set LED1 on to give feedback that the power is on, and a timer to turn off
  //HalLedSet(HAL_LED_1, HAL_LED_MODE_ON);
  
  Util_startClock(&ledTimeoutClock);
  
    // Set timer for first battery read event
  Util_startClock(&batteryCheckClock);  
  
  // Start the Device
  VOID GAPRole_StartDevice(&KeyFobDemo_peripheralCBs);

  // Start Bond Manager
  VOID GAPBondMgr_Register(&KeyFobDemo_bondMgrCBs); 
}

/*********************************************************************
 * @fn      KeyFobDemo_taskFxn
 *
 * @brief   Application task entry point for the Key Fob Demo application.
 *
 * @param   a0, a1 - not used.
 *
 * @return  None.
 */
static void KeyFobDemo_taskFxn(UArg a0, UArg a1)
{
  // Initialize application
  KeyFobDemo_init();
  
  // Application main loop
  for (;;)
  {
    // Waits for a signal to the semaphore associated with the calling thread.
    // Note that the semaphore associated with a thread is signaled when a
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
          KeyFobDemo_processStackMsg((ICall_Hdr *)pMsg);
        }
                
        if (pMsg)
        {
          ICall_freeMsg(pMsg);
        }
      }

      // If RTOS queue is not empty, process app message.
      while (!Queue_empty(appMsgQueue))
      {
        kfdEvt_t *pMsg = (kfdEvt_t *)Util_dequeueMsg(appMsgQueue);
        if (pMsg)
        {
          // Process message.
          KeyFobDemo_processAppMsg(pMsg);
          
          // Free the space from the message.
          ICall_free(pMsg);
        }
      }
    }

    if (events & KFD_POWERON_LED_TIMEOUT_EVT)
    {
      events &= ~KFD_POWERON_LED_TIMEOUT_EVT;
      
      //HalLedSet(HAL_LED_1, HAL_LED_MODE_OFF); 
    }
    
    if (events & KFD_ACCEL_CHANGE_EVT)
    {
      events &= ~KFD_ACCEL_CHANGE_EVT;
      
      KeyFobDemo_processAccelEnablerChangeEvt();
    }
    
    if (events & KFD_ACCEL_READ_EVT)
    {
      events &= ~KFD_ACCEL_READ_EVT;
      
      KeyFobDemo_processAccelReadEvt();
    }

    if (events & KFD_BATTERY_CHECK_EVT)
    {
      events &= ~KFD_BATTERY_CHECK_EVT;
      
      // Restart timer
      if (BATTERY_CHECK_PERIOD)
      {
        Util_startClock(&batteryCheckClock);
      }

      // perform battery level check
      Batt_MeasLevel();
    }
   
    if (events & KFD_TOGGLE_BUZZER_EVT)
    {
      events &= ~KFD_TOGGLE_BUZZER_EVT;
      
      KeyFobDemo_processToggleBuzzerEvt();
    }

#if defined (PLUS_BROADCASTER)
    if (events & KFD_ADV_IN_CONNECTION_EVT)
    {
      uint8_t turnOnAdv = TRUE;
      
      events &= ~KFD_ADV_IN_CONNECTION_EVT;
      
      // Turn on advertising while in a connection
      GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &turnOnAdv);
    }
#endif
  }
}

/*********************************************************************
 * @fn      KeyFobDemo_processStackMsg
 *
 * @brief   Process an incoming stack message.
 *
 * @param   pMsg - message to process
 *
 * @return  None.
 */
static void KeyFobDemo_processStackMsg(ICall_Hdr *pMsg)
{
  switch (pMsg->event)
  {
    case GATT_MSG_EVENT:
      // Process GATT message
      KeyFobDemo_processGATTMsg((gattMsgEvent_t *)pMsg);
      break;
      
    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      KeyFobDemo_processGATTMsg
 *
 * @brief   Process GATT messages
 *
 * @return  None.
 */
static void KeyFobDemo_processGATTMsg(gattMsgEvent_t *pMsg)
{
  GATT_bm_free(&pMsg->msg, pMsg->method);
}

/*********************************************************************
 * @fn      KeyFobDemo_processAppMsg
 *
 * @brief   Process an incoming callback from a profile.
 *
 * @param   pMsg - message to process
 *
 * @return  None.
 */                       
static void KeyFobDemo_processAppMsg(kfdEvt_t *pMsg)
{
  switch (pMsg->hdr.event)
  {
    case KFD_STATE_CHANGE_EVT:
      KeyFobDemo_processStateChangeEvt((gaprole_States_t)pMsg->hdr.state);
      break;
    
    case KFD_KEY_CHANGE_EVT:
      KeyFobDemo_handleKeys(0, pMsg->hdr.state);
      break;
      
    case KFD_PROXIMITY_EVT:
      KeyFobDemo_processProximityAttrEvt(pMsg->hdr.state);
      break;
      
    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      KeyFobDemo_stateChangeCB
 *
 * @brief   Callback from GAP Role indicating a role state change.
 *
 * @param   newState - new state
 *
 * @return  None.
 */
static void KeyFobDemo_stateChangeCB(gaprole_States_t newState)
{
  KeyFobDemo_enqueueMsg(KFD_STATE_CHANGE_EVT, newState);
}

/*********************************************************************
 * @fn      KeyFobDemo_processStateChangeEvt
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void KeyFobDemo_processStateChangeEvt(gaprole_States_t newState)
{
  uint16_t connHandle = INVALID_CONNHANDLE;
  uint8_t valFalse = FALSE;

  if (gapProfileState != newState)
  {
    switch(newState)
    {
    case GAPROLE_STARTED:
      {
        // Set the system ID from the bd addr
        uint8_t systemId[DEVINFO_SYSTEM_ID_LEN];
        GAPRole_GetParameter(GAPROLE_BD_ADDR, systemId);

        // shift three bytes up
        systemId[7] = systemId[5];
        systemId[6] = systemId[4];
        systemId[5] = systemId[3];

        // set middle bytes to zero
        systemId[4] = 0;
        systemId[3] = 0;

        DevInfo_SetParameter(DEVINFO_SYSTEM_ID, DEVINFO_SYSTEM_ID_LEN, systemId);
      }
      break;

    // If the state changed to connected, initially assume that keyfob is in 
    // range.
    case GAPROLE_ADVERTISING:
      {
        // Visual feedback that we are advertising.
        //HalLedSet(HAL_LED_2, HAL_LED_MODE_ON);
      }
      break;
      
    // If the state changed to connected, initially assume that keyfob is in 
    // range.
    case GAPROLE_CONNECTED:
      {
        // Set the proximity state to either path loss alert or in range depending
        // on the value of keyfobProxIMAlertLevel (which was set by proximity 
        // monitor).    
        if(keyfobProxIMAlertLevel != PP_ALERT_LEVEL_NO)
        {
          keyfobProximityState = KEYFOB_PROXSTATE_PATH_LOSS;
          
          // Perform alert.
          KeyFobDemo_performAlert();
          buzzer_beep_count = 0;
        }
        // If keyfobProxIMAlertLevel is PP_ALERT_LEVEL_NO
        else
        {
          keyfobProximityState = KEYFOB_PROXSTATE_CONNECTED_IN_RANGE;
          KeyFobDemo_stopAlert();
        }

        GAPRole_GetParameter(GAPROLE_CONNHANDLE, &connHandle);

        #if defined (PLUS_BROADCASTER)
          Util_startClock(&PB_advInConnWaitClock);
        #endif
          
        // Turn off LED that shows we're advertising.
        //HalLedSet(HAL_LED_2, HAL_LED_MODE_OFF);
      }
      break;

    case GAPROLE_WAITING:
      {
        // Then the link was terminated intentionally by the slave or master,
        // or advertising timed out
        keyfobProximityState = KEYFOB_PROXSTATE_INITIALIZED;

        // Turn off immediate alert.
        ProxReporter_SetParameter(PP_IM_ALERT_LEVEL, sizeof(valFalse), &valFalse);
        keyfobProxIMAlertLevel = PP_ALERT_LEVEL_NO;
        
        // Change attribute value of Accelerometer Enable to FALSE.
        Accel_SetParameter(ACCEL_ENABLER, sizeof(valFalse), &valFalse);
        
        // Stop the acceleromter.
        // Note: Accel_SetParameter does not trigger the callback
        KeyFobDemo_accelEnablerChangeCB(); 
        
        // Turn off LED that shows we're advertising.
        //HalLedSet(HAL_LED_2, HAL_LED_MODE_OFF);
        
        // Stop alert if it was active.
        if(keyfobAlertState != ALERT_STATE_OFF)
        {
          KeyFobDemo_stopAlert();
        }
      }
      break;

    case GAPROLE_WAITING_AFTER_TIMEOUT:
      {
        // The link was dropped due to supervision timeout.
        keyfobProximityState = KEYFOB_PROXSTATE_LINK_LOSS;

        // Turn off immediate alert
        ProxReporter_SetParameter(PP_IM_ALERT_LEVEL, sizeof(valFalse), &valFalse);
        keyfobProxIMAlertLevel = PP_ALERT_LEVEL_NO;
        
        // Change attribute value of Accelerometer Enable to FALSE.
        Accel_SetParameter(ACCEL_ENABLER, sizeof(valFalse), &valFalse);
        
        // Stop the acceleromter.
        KeyFobDemo_accelEnablerChangeCB(); // SetParameter does not trigger the callback
        
        // Perform link loss alert if enabled.
        if(keyfobProxLLAlertLevel != PP_ALERT_LEVEL_NO)
        {
          KeyFobDemo_performAlert();
          buzzer_beep_count = 0;
        }
      }
      break;

    default:
      // do nothing
      break;
    }
  }

  gapProfileState = newState;
}

/*********************************************************************  
 * @fn      KeyFobDemo_keyPressHandler
 *
 * @brief   Key event handler function
 *
 * @param   keys - keys pressed.
 *
 * @return  none
 */
static void KeyFobDemo_keyPressHandler(uint8_t keys)
{
  // Enqueue the event.
  KeyFobDemo_enqueueMsg(KFD_KEY_CHANGE_EVT, keys);
}

/*********************************************************************
 * @fn      KeyFobDemo_handleKeys
 *
 * @brief   Handles all key events for this device.
 *
 * @param   shift - true if in shift/alt.
 * @param   keys - bit field for key events. Valid entries:
 *                 KEY_UP
 *                 KEY_RIGHT
 *
 * @return  none
 */
static void KeyFobDemo_handleKeys(uint8_t shift, uint8_t keys)
{
  uint8_t SK_Keys = 0;

  (void)shift;  // Intentionally unreferenced parameter

  if (keys & KEY_UP)
  {
    SK_Keys |= SK_KEY_LEFT;

    // If is active, pressing the left key should toggle
    // stop the alert.
    if(keyfobAlertState != ALERT_STATE_OFF)
    {
      KeyFobDemo_stopAlert();
    }

    // If device is in a connection, toggle the Tx power level between 0 and
    // -6 dBm.
    if(gapProfileState == GAPROLE_CONNECTED)
    {
      int8_t currentTxPowerLevel;
      int8_t newTxPowerLevel;

      ProxReporter_GetParameter(PP_TX_POWER_LEVEL, &currentTxPowerLevel);

      switch (currentTxPowerLevel)
      {
      case 0:
        newTxPowerLevel = -6;
        
        // Change power to -6 dBm.
        HCI_EXT_SetTxPowerCmd(HCI_EXT_TX_POWER_MINUS_6_DBM);
        
        // Update Tx power level in Proximity Reporter (and send notification)
        // if enabled).
        ProxReporter_SetParameter(PP_TX_POWER_LEVEL, sizeof(int8_t), 
                                  &newTxPowerLevel);
        break;

      case (-6):
        newTxPowerLevel = 0;
        
        // Change power to 0 dBm.
        HCI_EXT_SetTxPowerCmd(HCI_EXT_TX_POWER_0_DBM);
        
        // Update Tx power level in Proximity Reporter (and send notification)
        // if enabled).
        ProxReporter_SetParameter(PP_TX_POWER_LEVEL, sizeof(int8_t), 
                                  &newTxPowerLevel);
        break;

      default:
        // do nothing
        break;
      }
    }
  }

  if (keys & KEY_RIGHT)
  {
    SK_Keys |= SK_KEY_RIGHT;

    // If device is not in a connection, pressing the right key should toggle
    // advertising on and off.
    if(gapProfileState != GAPROLE_CONNECTED)
    {
      uint8_t current_adv_enabled_status;
      uint8_t new_adv_enabled_status;

      // Find the current GAP advertisement status.
      GAPRole_GetParameter(GAPROLE_ADVERT_ENABLED, &current_adv_enabled_status);

      if(current_adv_enabled_status == FALSE)
      {
        new_adv_enabled_status = TRUE;
      }
      else
      {
        new_adv_enabled_status = FALSE;
      }

      // Change the GAP advertisement status to opposite of current status.
      GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), 
                           &new_adv_enabled_status);
    }
  }

  SK_SetParameter(SK_KEY_ATTR, sizeof(uint8_t), &SK_Keys);
}

/*********************************************************************
 * @fn      KeyFobDemo_processToggleBuzzerEvt
 *
 * @brief   Handles enabling and disabling of the buzzer and flashing LEDS
 *          during an alert.
 *
 * @param   None.
 *
 * @return  None.
 */
static void KeyFobDemo_processToggleBuzzerEvt(void)
{
  // If this event was triggered while buzzer is on, turn it off, increment 
  // beep_count, check whether max has been reached, and if not set the OSAL 
  // timer for next event to turn buzzer back on.
  if (buzzer_state == BUZZER_ON)
  {
    Buzzer_stop();
    
    buzzer_state = BUZZER_OFF;
    buzzer_beep_count++;

    // Check to see if buzzer has beeped maximum number of times.
    // If it has, then disable clock.
    if ((buzzer_beep_count >= BUZZER_MAX_BEEPS)  ||
        ((keyfobProximityState != KEYFOB_PROXSTATE_LINK_LOSS) &&
         (keyfobProximityState != KEYFOB_PROXSTATE_PATH_LOSS)))
    {
      Util_stopClock(&toggleBuzzerClock);
    }
  }
  else if (keyfobAlertState != ALERT_STATE_OFF)
  {
    // If this event was triggered while the buzzer is off then turn it on if 
    // appropriate.
    KeyFobDemo_performAlert();
  }
}

/*********************************************************************
 * @fn      KeyFobDemo_performAlert
 *
 * @brief   Perform alert.
 *
 * @param   None.
 *
 * @return  None.
 */
static void KeyFobDemo_performAlert(void)
{
  if (keyfobProximityState == KEYFOB_PROXSTATE_LINK_LOSS)
  {
    switch(keyfobProxLLAlertLevel)
    {
      case PP_ALERT_LEVEL_LOW:
        keyfobAlertState = ALERT_STATE_LOW;

        Buzzer_start(BUZZER_ALERT_LOW_FREQ);
        buzzer_state = BUZZER_ON;
        
        // Only run buzzer for 200ms.
        Util_startClock(&toggleBuzzerClock);

        //HalLedSet((HAL_LED_1 | HAL_LED_2), HAL_LED_MODE_OFF);
        break;

      case PP_ALERT_LEVEL_HIGH:
        keyfobAlertState = ALERT_STATE_HIGH;

        Buzzer_start(BUZZER_ALERT_HIGH_FREQ);
        buzzer_state = BUZZER_ON;
        
        // Only run buzzer for 200ms.
        Util_startClock(&toggleBuzzerClock);

        //HalLedSet(HAL_LED_1, HAL_LED_MODE_ON);
        //HalLedSet(HAL_LED_2, HAL_LED_MODE_FLASH);
        break;

      case PP_ALERT_LEVEL_NO:
          // Fall through
      default:
        KeyFobDemo_stopAlert();
        break;
    }
  }
  else if (keyfobProximityState == KEYFOB_PROXSTATE_PATH_LOSS)
  {
    switch(keyfobProxIMAlertLevel)
    {
      case PP_ALERT_LEVEL_LOW:
        keyfobAlertState = ALERT_STATE_LOW;

        Buzzer_start(BUZZER_ALERT_LOW_FREQ);
        buzzer_state = BUZZER_ON;
        
        // Only run buzzer for 200ms.
        Util_startClock(&toggleBuzzerClock);

        //HalLedSet((HAL_LED_1 | HAL_LED_2), HAL_LED_MODE_OFF);
        break;


      case PP_ALERT_LEVEL_HIGH:
        keyfobAlertState = ALERT_STATE_HIGH;

        Buzzer_start(BUZZER_ALERT_HIGH_FREQ);
        buzzer_state = BUZZER_ON;
        
        // Only run buzzer for 200ms.
        Util_startClock(&toggleBuzzerClock);

        //HalLedSet(HAL_LED_1, HAL_LED_MODE_ON);
        //HalLedSet(HAL_LED_2, HAL_LED_MODE_FLASH);
        break;

        case PP_ALERT_LEVEL_NO:
          // Fall through
        default:
          KeyFobDemo_stopAlert();
          break;
    }
  }
}

/*********************************************************************
 * @fn      KeyFobDemo_stopAlert
 *
 * @brief   Stops an alert.
 *
 * @param   none
 *
 * @return  none
 */
void KeyFobDemo_stopAlert(void)
{

  keyfobAlertState = ALERT_STATE_OFF;

  Buzzer_stop();
  
  buzzer_state = BUZZER_OFF;
  
  //HalLedSet((HAL_LED_1 | HAL_LED_2), HAL_LED_MODE_OFF);
}

/*********************************************************************
 * @fn      KeyFobDemo_proximityAttrCB
 *
 * @brief   Notification from the profile of an atrribute change by
 *          a connected device.
 *
 * @param   attrParamID - Profile's Attribute Parameter ID
 *            PP_LINK_LOSS_ALERT_LEVEL  - The link loss alert level value
 *            PP_IM_ALERT_LEVEL         - The immediate alert level value
 *
 * @return  none
 */
static void KeyFobDemo_proximityAttrCB(uint8_t attrParamID)
{
  KeyFobDemo_enqueueMsg(KFD_PROXIMITY_EVT, attrParamID);
}

/*********************************************************************
 * @fn      KeyFobDemo_processProximityAttrEvt
 *
 * @brief   Process a notification from the profile of an atrribute
 *          change by a connected device.
 *
 * @param   attrParamID - Profile's Attribute Parameter ID
 *            PP_LINK_LOSS_ALERT_LEVEL  - The link loss alert level value
 *            PP_IM_ALERT_LEVEL         - The immediate alert level value
 *
 * @return  none
 */
static void KeyFobDemo_processProximityAttrEvt(uint8_t attrParamID)
{
  switch(attrParamID)
  {
    case PP_LINK_LOSS_ALERT_LEVEL:
      ProxReporter_GetParameter(PP_LINK_LOSS_ALERT_LEVEL, &keyfobProxLLAlertLevel);
      break;

    case PP_IM_ALERT_LEVEL:
      ProxReporter_GetParameter(PP_IM_ALERT_LEVEL, &keyfobProxIMAlertLevel);

      // If proximity monitor set the immediate alert level to low or high, then
      // the monitor calculated that the path loss to the keyfob
      // (proximity observer) has exceeded the threshold.
      if(keyfobProxIMAlertLevel != PP_ALERT_LEVEL_NO)
      {
        keyfobProximityState = KEYFOB_PROXSTATE_PATH_LOSS;
        KeyFobDemo_performAlert();
        buzzer_beep_count = 0;
      }
      // Proximity monitor turned off alert because the path loss is below 
      // threshold.
      else 
      {
        keyfobProximityState = KEYFOB_PROXSTATE_CONNECTED_IN_RANGE;
        KeyFobDemo_stopAlert();
      }
      break;

    default:
      // should not reach here!
      break;
  }
}

/*********************************************************************
 * @fn      KeyFobDemo_accelEnablerChangeCB
 *
 * @brief   Called by the Accelerometer Profile when the Enabler Attribute
 *          is changed.
 *
 * @param   none
 *
 * @return  none
 */
static void KeyFobDemo_accelEnablerChangeCB(void)
{
  // Set Event.
  events |= KFD_ACCEL_CHANGE_EVT;
  
  // Post application's Semaphore.
  Semaphore_post(sem);
}

/*********************************************************************
 * @fn      KeyFobDemo_processAccelEnablerChangeEvt
 *
 * @brief   Process a callback from the Accelerometer Profile when the
 *          Enabler Attribute is changed.
 *
 * @param   none
 *
 * @return  none
 */
static void KeyFobDemo_processAccelEnablerChangeEvt(void)
{
  if (Accel_GetParameter(ACCEL_ENABLER, &accelEnabler) == SUCCESS)
  {
    if (accelEnabler)
    {
      // Initialize accelerometer
      Acc_init();
      
      // Setup timer for accelerometer task
      Util_startClock(&accelReadClock);
    } 
    else
    {
      // Stop the accelerometer
      Acc_stop();
      
      Util_stopClock(&accelReadClock);
    }
  } 
}

/*********************************************************************
 * @fn      KeyFobDemo_processAccelReadEvt
 *
 * @brief   Process accelerometer task.
 *
 * @param   none
 *
 * @return  none
 */
static void KeyFobDemo_processAccelReadEvt(void)
{
  // Check if accelerator is enabled.
  if (Accel_GetParameter(ACCEL_ENABLER, &accelEnabler) == SUCCESS)
  {
    if (accelEnabler)
    {
      // Restart timer
      if (ACCEL_READ_PERIOD)
      {
        Util_startClock(&accelReadClock);
      }

      // Read accelerometer data.
      KeyFobDemo_accelRead();
    }
    else
    {
      // Stop the accelerometer.
      Util_stopClock(&accelReadClock);
    }
  }
}

/*********************************************************************
 * @fn      KeyFobDemo_accelRead
 *
 * @brief   Called by the application to read accelerometer data
 *          and put data in accelerometer profile
 *
 * @param   none
 *
 * @return  none
 */
static void KeyFobDemo_accelRead(void)
{
  static int8_t x = 0, y = 0, z = 0;
  int8_t new_x = 0, new_y = 0, new_z = 0;

  // Read data for each axis of the accelerometer
  Acc_readAcc(&new_x, &new_y, &new_z);

  // Check if x-axis value has changed by more than the threshold value and
  // set profile parameter if it has (this will send a notification if enabled)
  if((x < (new_x-ACCEL_CHANGE_THRESHOLD)) || (x > (new_x+ACCEL_CHANGE_THRESHOLD)))
  {
    x = new_x;
    Accel_SetParameter(ACCEL_X_ATTR, sizeof(int8_t), &x);
  }

  // Check if y-axis value has changed by more than the threshold value and
  // set profile parameter if it has (this will send a notification if enabled)
  if((y < (new_y-ACCEL_CHANGE_THRESHOLD)) || (y > (new_y+ACCEL_CHANGE_THRESHOLD)))
  {
    y = new_y;
    Accel_SetParameter(ACCEL_Y_ATTR, sizeof(int8_t), &y);
  }

  // Check if z-axis value has changed by more than the threshold value and
  // set profile parameter if it has (this will send a notification if enabled)
  if((z < (new_z-ACCEL_CHANGE_THRESHOLD)) || (z > (new_z+ACCEL_CHANGE_THRESHOLD)))
  {
    z = new_z;
    Accel_SetParameter(ACCEL_Z_ATTR, sizeof(int8_t), &z);
  }
}

/*********************************************************************
 * @fn      KeyFobDemo_clockHandler
 *
 * @brief   Handler function for clock timeouts.
 *
 * @param   arg - event type
 *
 * @return  None.
 */
static void KeyFobDemo_clockHandler(UArg arg)
{ 
  // Store the event.
  events |= arg;
  
  // Wake up the application.
  Semaphore_post(sem);
}

/*********************************************************************
 * @fn      KeyFobDemo_enqueueMsg
 *
 * @brief   Creates a message and puts the message in RTOS queue.
 *
 * @param   event  - message event.
 * @param   state - message state.
 *
 * @return  None.
 */
static void KeyFobDemo_enqueueMsg(uint16_t event, uint8_t state)
{
  kfdEvt_t *pMsg;
  
  // Create dynamic pointer to message.
  if (pMsg = ICall_malloc(sizeof(kfdEvt_t)))
  {
    pMsg->hdr.event = event;
    pMsg->hdr.state = state;
    
    // Enqueue the message.
    Util_enqueueMsg(appMsgQueue, sem, (uint8_t*)pMsg);
  }
}

/*********************************************************************
*********************************************************************/
