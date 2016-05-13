/*******************************************************************************
  Filename:       thermometer.c
  Revised:        $Date: 2015-07-20 11:31:07 -0700 (Mon, 20 Jul 2015) $
  Revision:       $Revision: 44370 $

  Description:    This file contains the Thermometer BLE sample application 
                  for use with the CC2650 Bluetooth Low Energy Protocol Stack.

  Copyright 2011 - 2015 Texas Instruments Incorporated. All rights reserved.

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

#include "bcomdef.h"
#include "linkdb.h"
#include "gatt.h"
#include "gapgattserver.h"
#include "gattservapp.h"
#include "gatt_profile_uuid.h"
#include "peripheral.h"
#include "gapbondmgr.h"

#include "osal_snv.h"
#include "ICallBleAPIMSG.h"

#include "UTC_clock.h"
#include "util.h"
#include "board_key.h"

#include "thermometerservice.h"
#include "devinfoservice.h"
#include "thermometer.h"
#include "time_clock.h"
#include "bletime.h"

#include "Board.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

// Use limited discoverable mode to advertise for 30.72s, and then stop, or 
// use general discoverable mode to advertise indefinitely 
#define DEFAULT_DISCOVERABLE_MODE             GAP_ADTYPE_FLAGS_LIMITED
//#define DEFAULT_DISCOVERABLE_MODE           GAP_ADTYPE_FLAGS_GENERAL

// How often to perform periodic event
#define PERIODIC_EVT_PERIOD                   1000

// Whether to enable automatic parameter update request when a connection 
// is formed.
#define DEFAULT_ENABLE_UPDATE_REQUEST         FALSE

// Minimum connection interval (units of 1.25ms) if automatic parameter update 
// request is enabled.
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     200

// Maximum connection interval (units of 1.25ms) if automatic parameter update 
// request is enabled.
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     1600

// Slave latency to use if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SLAVE_LATENCY         1

// Supervision timeout value (units of 10ms) if automatic parameter update 
// request is enabled.
#define DEFAULT_DESIRED_CONN_TIMEOUT          1000

// Some values used to simulate measurements
#define FLAGS_IDX_MAX                         7 //3 flags c/f -- timestamp -- site

// Length of bd addr as a string
#define B_ADDR_STR_LEN                        15

// Delay to begin discovery from start of connection in ms
#define DEFAULT_DISCOVERY_DELAY               1000

// Delay to terminate after connection
#define DEFAULT_TERMINATE_DELAY               60000

// Default Thermometer measurement interval
#define DEFAULT_THERMOMETER_MEAS_DELAY        30

#define DEFAULT_THERMOMETER_IMEAS_DELAY       1000

// Max measurement storage count
#define TH_STORE_MAX                          3

#define THERMOMETER_IMEAS_LEN                 6
#define THERMOMETER_MEAS_LEN                  13

// Thermomometer Task Events
#define THERMOMETER_START_DEVICE_EVT          0x0001
#define THERMOMETER_PERIODIC_MEAS_EVT         0x0002
#define THERMOMETER_PERIODIC_IMEAS_EVT        0x0004
#define THERMOMETER_START_DISC_EVT            0x0008
#define THERMOMETER_DISCONNECT_EVT            0x0010
#define THERMOMETER_STATE_CHANGE_EVT          0x0020
#define THERMOMETER_PASSCODE_EVT              0x0040
#define THERMOMETER_PAIR_STATE_EVT            0x0080
#define THERMOMETER_SERVICE_EVT               0x0100
#define THERMOMETER_KEY_CHANGE_EVT            0x0200

// Task configuration
#define THERMOMETER_TASK_PRIORITY             1
#define THERMOMETER_TASK_STACK_SIZE           868

/*********************************************************************
 * TYPEDEFS
 */

// App event passed from profiles.
typedef struct 
{
  appEvtHdr_t hdr;            // Event header.
  uint8_t *pData;             // Event data.
} thermometerEvt_t;

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

// Entity ID globally used to check for source and/or destination of messages.
static ICall_EntityID selfEntity;

// Semaphore globally used to post events to the application thread.
static ICall_Semaphore sem;

// Clock instances for internal periodic events.
static Clock_Struct startDiscoveryClock;
static Clock_Struct disconnectClock;
static Clock_Struct perMeasClock;
static Clock_Struct perIMeasClock;

// Handle for the Message Queue.
static Queue_Struct appMsg;
static Queue_Handle appMsgQueue;

// GAP State.
static gaprole_States_t gapProfileState = GAPROLE_INIT;

// Task pending events.
static uint16_t events = 0;

// Task configuration.
Task_Struct Thermometer_task;
Char Thermometer_taskStack[THERMOMETER_TASK_STACK_SIZE];

// Service discovery state.
static uint8_t servDiscState = DISC_IDLE;

// Service discovery complete.
static uint8_t servDiscComplete = FALSE;

// Flag for delaying service discovery while devices are pairing.
// TRUE if discovery is postponed.
static uint8_t servDiscPostponed = FALSE;

// Characteristic configuration state.
static uint8_t thermometer_timeCfgState = TIME_CONFIG_START;

// GAP Profile - Name attribute for SCAN RSP data.
static uint8_t scanResponseData[] =
{
  0x12,   // length of this data
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,   
  'T',
  'h',
  'e',
  'r',
  'm',
  'o',
  'm',
  'e',
  't',
  'e',
  'r',
  'S',
  'e',
  'n',
  's',
  'o',
  'r',
    // connection interval range
  0x05,   // length of this data
  GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE,
  LO_UINT16(DEFAULT_DESIRED_MIN_CONN_INTERVAL),   // 100ms
  HI_UINT16(DEFAULT_DESIRED_MIN_CONN_INTERVAL),  
  LO_UINT16(DEFAULT_DESIRED_MAX_CONN_INTERVAL),   // 1s
  HI_UINT16(DEFAULT_DESIRED_MAX_CONN_INTERVAL),  

  // Tx power level
  GAP_ADTYPE_POWER_LEVEL,
  0       // 0dBm  
};

// Advertisement data.
static uint8_t advertData[] = 
{ 
  // Flags; this sets the device to use limited discoverable
  // mode (advertises for 30 seconds at a time) instead of general
  // discoverable mode (advertises indefinitely).
  0x02,   // length of this data
  GAP_ADTYPE_FLAGS,
  DEFAULT_DISCOVERABLE_MODE | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

  // service UUID
  0x03,   // length of this data
  GAP_ADTYPE_16BIT_MORE,
  LO_UINT16(THERMOMETER_SERV_UUID),
  HI_UINT16(THERMOMETER_SERV_UUID),
};

// Device name attribute value
static uint8_t attDeviceName[GAP_DEVICE_NAME_LEN] = "Thermometer Sensor";

// Bonded peer address.
static uint8_t timeAppBondedAddr[B_ADDR_LEN];

// Last connection address.
static uint8_t lastConnAddr[B_ADDR_LEN] = {0xf,0xf,0xf,0xf,0xf,0xe};;

static bool connectedToLastAddress = false;

// GAP connection handle.
static uint16_t thermometer_connHandle;

// GAP connected device address.
static uint8_t connDeviceAddr[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

static attHandleValueInd_t thStoreMeas[TH_STORE_MAX];
static uint8_t thStoreStartIndex =0;
static uint8_t thStoreIndex = 0;

static uint32_t thermometerCelcius = 0X000173;
static bool temperatureMeasCharConfig = false;
static bool temperatureIMeasCharConfig = false;
static bool temperatureIntervalConfig = false;
static bool thMeasTimerRunning = FALSE;

// flags for simulated measurements.
static const uint8_t thermometerFlags[FLAGS_IDX_MAX] =
{
  THERMOMETER_FLAGS_CELCIUS | THERMOMETER_FLAGS_TIMESTAMP | THERMOMETER_FLAGS_TYPE,
  THERMOMETER_FLAGS_CELCIUS | THERMOMETER_FLAGS_TIMESTAMP,
  THERMOMETER_FLAGS_CELCIUS,
  THERMOMETER_FLAGS_FARENHEIT,
  THERMOMETER_FLAGS_FARENHEIT | THERMOMETER_FLAGS_TIMESTAMP,
  THERMOMETER_FLAGS_FARENHEIT | THERMOMETER_FLAGS_TIMESTAMP | THERMOMETER_FLAGS_TYPE,
  0x00
};

// initial value of flags.
static uint8_t thermometerFlagsIdx = 0;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

// Application tasks and event processing.
static void Thermometer_init(void);
static void Thermometer_taskFxn(UArg a0, UArg a1);
static uint8_t Thermometer_enqueueMsg(uint16_t event, uint8_t status,
                                      uint8_t *pData);
static void Thermometer_processStackMsg(ICall_Hdr *pMsg);
static void Thermometer_processAppMsg(thermometerEvt_t *pMsg);
static void Thermometer_processGattMsg(gattMsgEvent_t *pMsg);
static void Thermometer_advertise(void);
static void Thermometer_disconnectEvt(void);
static void Thermometer_startDiscEvt(void);

// Thermometer Service.
static void Thermometer_performPeriodicTask(void);
static void Thermometer_performPeriodicImeasTask(void);
static void Thermometer_sendStoredMeas(void);
static void Thermometer_measIndicate(void);
static void Thermometer_serviceCB(uint8_t event);
static void Thermometer_processServiceEvt(uint8_t event);
static void Thermometer_storeIndications(attHandleValueInd_t* pInd);
static void Thermometer_updateCCC(void);
static void Thermometer_updateInterval(void);

// Peripheral Role.
static void Thermometer_stateChangeCB(gaprole_States_t newState);
static void Thermometer_processStateChangeEvt(gaprole_States_t newState);

// Passcode.
static void Thermometer_passcodeCB(uint8_t *deviceAddr, 
                                          uint16_t connHandle,
                                          uint8_t uiInputs, uint8_t uiOutputs);
static void Thermometer_processPasscodeEvt(uint16_t connHandle);

// Pairing state.
static void Thermometer_pairStateCB(uint16_t connHandle, uint8_t state, 
                                           uint8_t status);
static void Thermometer_processPairStateEvt(uint8_t state, uint8_t status);

// Keys.
static void Thermometer_keyPressHandler(uint8_t keys);
static void Thermometer_handleKeys(uint8_t shift, uint8_t keys);

// Clock handler.
static void Thermometer_clockHandler(UArg arg);

/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static gapRolesCBs_t thermometer_PeripheralCBs =
{
  Thermometer_stateChangeCB  // Profile State Change Callbacks
};

// GAP Bond Manager Callbacks
static const gapBondCBs_t thermometer_BondMgrCBs =
{
  Thermometer_passcodeCB,    // Passcode callback
  Thermometer_pairStateCB    // Pairing state callback
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      Thermometer_createTask
 *
 * @brief   Task creation function for the Time Application.
 *
 * @param   none
 *
 * @return  none
 */
void Thermometer_createTask(void)
{
  Task_Params taskParams;
    
  // Configure task.
  Task_Params_init(&taskParams);
  taskParams.stack = Thermometer_taskStack;
  taskParams.stackSize = THERMOMETER_TASK_STACK_SIZE;
  taskParams.priority = THERMOMETER_TASK_PRIORITY;
  
  Task_construct(&Thermometer_task, Thermometer_taskFxn, &taskParams, NULL);
}

/*********************************************************************
 * @fn      Thermometer_init
 *
 * @brief   Initialization function for the Thermometer App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notification ...).
 *
 * @param   None.
 *
 * @return  None.
 */
void Thermometer_init(void)
{
	// ******************************************************************
  // N0 STACK API CALLS CAN OCCUR BEFORE THIS CALL TO ICall_registerApp
  // ******************************************************************
  // Register the current thread as an ICall dispatcher application
  // so that the application can send and receive messages.
  ICall_registerApp(&selfEntity, &sem);

  // Hard code the DB Address till CC2650 board gets its own IEEE address.
  //uint8_t bdAddress[B_ADDR_LEN] = { 0x22, 0x22, 0x22, 0x22, 0x22, 0x22 };
  //HCI_EXT_SetBDADDRCmd(bdAddress);
  
  // Set device's Sleep Clock Accuracy
  //HCI_EXT_SetSCACmd(40);
  
  // Create an RTOS queue for message from profile to be sent to app.
  appMsgQueue = Util_constructQueue(&appMsg);
  
  // Setup clocks.
  Util_constructClock(&startDiscoveryClock, Thermometer_clockHandler, 
                      DEFAULT_DISCOVERY_DELAY, 0, false,
                      THERMOMETER_START_DISC_EVT);
  Util_constructClock(&disconnectClock, Thermometer_clockHandler, 
                      DEFAULT_TERMINATE_DELAY, 0, false,
                      THERMOMETER_DISCONNECT_EVT);
  Util_constructClock(&perMeasClock, Thermometer_clockHandler, 
                      DEFAULT_THERMOMETER_MEAS_DELAY, 0, false,
                      THERMOMETER_PERIODIC_MEAS_EVT);
  Util_constructClock(&perIMeasClock, Thermometer_clockHandler, 
                      DEFAULT_THERMOMETER_IMEAS_DELAY, 0, false,
                      THERMOMETER_PERIODIC_IMEAS_EVT);
  
  // Initialize keys on the SRF06.
  Board_initKeys(Thermometer_keyPressHandler);
  
  // Setup the GAP Peripheral Role Profile.
  {
    // Device doesn't start advertising until button is pressed.
    uint8_t initial_advertising_enable = FALSE;

    // By setting this to zero, the device will go into the waiting state after
    // being discoverable for 30.72 second, and will not being advertising again
    // until the enabler is set back to TRUE.
    uint16_t gapRole_AdvertOffTime = 0;
      
    uint8_t enable_update_request = DEFAULT_ENABLE_UPDATE_REQUEST;
    uint16_t desired_min_interval = DEFAULT_DESIRED_MIN_CONN_INTERVAL;
    uint16_t desired_max_interval = DEFAULT_DESIRED_MAX_CONN_INTERVAL;
    uint16_t desired_slave_latency = DEFAULT_DESIRED_SLAVE_LATENCY;
    uint16_t desired_conn_timeout = DEFAULT_DESIRED_CONN_TIMEOUT;

    // Set the GAP Role Parameters.
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), 
                         &initial_advertising_enable);
    GAPRole_SetParameter(GAPROLE_ADVERT_OFF_TIME, sizeof(uint16_t),
                         &gapRole_AdvertOffTime);
    
    GAPRole_SetParameter(GAPROLE_SCAN_RSP_DATA, sizeof (scanResponseData), 
                         scanResponseData);
    GAPRole_SetParameter(GAPROLE_ADVERT_DATA, sizeof(advertData), 
                         advertData);
    
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
  
  // Set the GAP Characteristics.
  GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, attDeviceName);

  // Setup the GAP Bond Manager.
  {
    uint32_t passkey = 0; // passkey "000000"
    uint8_t pairMode = GAPBOND_PAIRING_MODE_WAIT_FOR_REQ;
    uint8_t mitm = FALSE;
    uint8_t ioCap = GAPBOND_IO_CAP_DISPLAY_ONLY;
    uint8_t bonding = TRUE;
    
    GAPBondMgr_SetParameter(GAPBOND_DEFAULT_PASSCODE, sizeof (uint32_t), 
                            &passkey);
    GAPBondMgr_SetParameter(GAPBOND_PAIRING_MODE, sizeof(uint8_t), &pairMode);
    GAPBondMgr_SetParameter(GAPBOND_MITM_PROTECTION, sizeof(uint8_t), &mitm);
    GAPBondMgr_SetParameter(GAPBOND_IO_CAPABILITIES, sizeof(uint8_t), &ioCap);
    GAPBondMgr_SetParameter(GAPBOND_BONDING_ENABLED, sizeof(uint8_t), &bonding);
  }  
   
  // Initialize GATT Client.
  VOID GATT_InitClient();

  // Register to receive incoming ATT Indications/Notifications.
  GATT_RegisterForInd(selfEntity);
  
  // Initialize GATT attributes.
  GGS_AddService(GATT_ALL_SERVICES);         // GAP
  GATTServApp_AddService(GATT_ALL_SERVICES); // GATT attributes
  
  // Add Thermometer services.
  Thermometer_AddService(GATT_ALL_SERVICES);
  
  // Add device info services.
  DevInfo_AddService();

  // Register for Thermometer service callback.
  Thermometer_Register (Thermometer_serviceCB);
  
  // Setup the Thermometer Characteristic Values.
  {
    uint8_t thermometerSite = THERMOMETER_TYPE_MOUTH;
    thermometerIRange_t thermometerIRange= {4,60000};
    
    Thermometer_SetParameter(THERMOMETER_TYPE, sizeof(uint8_t), 
                             &thermometerSite);    
    
    Thermometer_SetParameter(THERMOMETER_IRANGE, sizeof(thermometerIRange_t), 
                             &thermometerIRange);
  }
  
  // Initialize measurement storage table
  memset(thStoreMeas, 0, (sizeof(attHandleValueInd_t) * TH_STORE_MAX));
  
  // Start the Device.
  VOID GAPRole_StartDevice(&thermometer_PeripheralCBs);
    
  // Register with bond manager after starting device.
  VOID GAPBondMgr_Register((gapBondCBs_t *)&thermometer_BondMgrCBs);
}

/*********************************************************************
 * @fn      Thermometer_taskFxn
 *
 * @brief   Thermometer Application Task entry point.  This function
 *          is called to initialize and then process all events for the task.
 *          Events include timers, messages and any other user defined events.
 *
 * @param   a0, a1 - not used.
 *
 * @return  None.
 */
void Thermometer_taskFxn(UArg a0, UArg a1)
{ 
  // Initialize the application.
  Thermometer_init();
    
  // Application main loop.
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
          // Process inter-task message.
          Thermometer_processStackMsg((ICall_Hdr *)pMsg);
        }
                
        if (pMsg)
        {
          ICall_freeMsg(pMsg);
        }
      }
    }
    
    // If RTOS queue is not empty, process app message.
    while (!Queue_empty(appMsgQueue))
    {
      thermometerEvt_t *pMsg = (thermometerEvt_t*)Util_dequeueMsg(appMsgQueue);
      if (pMsg)
      {
        // Process message.
        Thermometer_processAppMsg(pMsg);
        
        // Free the space from the message.
        ICall_free(pMsg);
      }
    }
    
    if (events)
    {
      // Start discovery event.
      if (events & THERMOMETER_START_DISC_EVT)
      {
        events &= ~THERMOMETER_START_DISC_EVT;
        
        Thermometer_startDiscEvt();
      }
      
      // Thermometer periodic measurement event.
      if (events & THERMOMETER_PERIODIC_MEAS_EVT)
      {
        events &= ~THERMOMETER_PERIODIC_MEAS_EVT;
        
        Thermometer_performPeriodicTask();
      }
      
      // Thermometer periodic I-measurement event.
      if (events & THERMOMETER_PERIODIC_IMEAS_EVT)
      {
        events &= ~THERMOMETER_PERIODIC_IMEAS_EVT;
        
        Thermometer_performPeriodicImeasTask();
      }
      
      // Disconnect event.
      if (events & THERMOMETER_DISCONNECT_EVT)
      {
        events &= ~THERMOMETER_DISCONNECT_EVT;
        
        Thermometer_disconnectEvt();
      }
    }
  }
}

/*********************************************************************
 * @fn      Thermometer_ProcessStackMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void Thermometer_processStackMsg(ICall_Hdr *pMsg)
{
  switch (pMsg->event)
  {
    case GATT_MSG_EVENT:
      Thermometer_processGattMsg((gattMsgEvent_t *)pMsg);
      break;

    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      Thermometer_processGattMsg
 *
 * @brief   Process GATT messages.
 *
 * @param   pMsg - pointer the the GATT message.
 *
 * @return  none
 */
static void Thermometer_processGattMsg(gattMsgEvent_t *pMsg)
{
  // Measurement Indication Confirmation.
  if (pMsg->method == ATT_HANDLE_VALUE_CFM)
  {
    Thermometer_sendStoredMeas();
  }
  
  if (pMsg->method == ATT_HANDLE_VALUE_NOTI ||
       pMsg->method == ATT_HANDLE_VALUE_IND)
  {
    Time_indGattMsg(pMsg);
  }
  else if (pMsg->method == ATT_READ_RSP ||
            pMsg->method == ATT_WRITE_RSP)
  {
    thermometer_timeCfgState = Time_configGattMsg(thermometer_timeCfgState, 
                                                  pMsg);
    
    if (thermometer_timeCfgState == TIME_CONFIG_CMPL)
    {
      servDiscComplete = TRUE;
    }
  }
  else
  {
    servDiscState = Time_discGattMsg(servDiscState, pMsg);
    
    if (servDiscState == DISC_IDLE)
    {      
      // Start characteristic configuration.
      thermometer_timeCfgState = Time_configNext(TIME_CONFIG_START);
    }
  }
  
  GATT_bm_free(&pMsg->msg, pMsg->method);
}

/*********************************************************************
 * @fn      Thermometer_processAppMsg
 *
 * @brief   Process an incoming stack message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void Thermometer_processAppMsg(thermometerEvt_t *pMsg)
{
  switch (pMsg->hdr.event)
  {
    case THERMOMETER_STATE_CHANGE_EVT:
      Thermometer_processStateChangeEvt((gaprole_States_t)pMsg->hdr.state);
      break;
      
    case THERMOMETER_KEY_CHANGE_EVT:
      Thermometer_handleKeys(0, pMsg->hdr.state);
      break;
      
    // Passcode event.
    case THERMOMETER_PASSCODE_EVT:
      Thermometer_processPasscodeEvt(thermometer_connHandle);
      break;  
      
    // Pair state event.
    case THERMOMETER_PAIR_STATE_EVT:
      Thermometer_processPairStateEvt(pMsg->hdr.state, *pMsg->pData);
    
      ICall_free(pMsg->pData);
      break;
      
    // Thermometer event.
    case THERMOMETER_SERVICE_EVT:      
      Thermometer_processServiceEvt(pMsg->hdr.state);
      break;
      
    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      Thermometer_keyPressHandler
 *
 * @brief   Key event handler function
 *
 * @param   keys - keys pressed
 *
 * @return  none
 */
void Thermometer_keyPressHandler(uint8_t keys)
{
  // Enqueue the event.
  Thermometer_enqueueMsg(THERMOMETER_KEY_CHANGE_EVT, keys, NULL);
}

/*********************************************************************
 * @fn      Thermometer_handleKeys
 *
 * @brief   Handles all key events for this device.
 *
 * @param   shift - true if in shift/alt.
 * @param   keys - bit field for key events.
 *
 * @return  none
 */
static void Thermometer_handleKeys(uint8_t shift, uint8_t keys)
{
  bStatus_t status; 
  uint8_t notify_interval;
  
  if (keys & KEY_UP)
  {
    // Set simulated measurement flag index.
    thermometerFlagsIdx+=1;
    
    if (thermometerFlagsIdx == FLAGS_IDX_MAX)
    {
      thermometerFlagsIdx = 0;
    }
  }
  
  // Read stored interval value.
  Thermometer_GetParameter(THERMOMETER_INTERVAL, &notify_interval); 

  if(notify_interval == 0)
  {
    thMeasTimerRunning = FALSE;
  }
  
  if (keys & KEY_RIGHT)
  {
    // If device is not in a connection, pressing the right key should toggle
    // advertising on and off. If timer is running, then will adv when meas is 
    // ready.
    if((gapProfileState != GAPROLE_CONNECTED) && (thMeasTimerRunning == FALSE))
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
    // Timer is running, so allow simulated changes.
    else
    {
      // Change temperature, remove single precision.
      if((thermometerCelcius) < 0x000175)
      {
        thermometerCelcius += 1;
      }
      else
      {
        uint16_t thInterval = 30;
        
        thermometerCelcius = 0x000173;

        // Simulate interval change.
        Thermometer_SetParameter(THERMOMETER_INTERVAL, THERMOMETER_INTERVAL_LEN,
                                 &thInterval);
        
        if(temperatureIntervalConfig == true) 
        {
          attHandleValueInd_t intervalInd;
          
          intervalInd.pValue = GATT_bm_alloc(thermometer_connHandle, 
                                             ATT_HANDLE_VALUE_IND, 
                                             THERMOMETER_INTERVAL_LEN, NULL);
          if (intervalInd.pValue != NULL)
          {
            intervalInd.len = THERMOMETER_INTERVAL_LEN;
            intervalInd.pValue[0] = LO_UINT16(thInterval);
            intervalInd.pValue[1] = HI_UINT16(thInterval);
            intervalInd.handle = THERMOMETER_INTERVAL_VALUE_POS;
        
            status = Thermometer_IntervalIndicate(thermometer_connHandle, 
                                                  &intervalInd,
                                                  ICall_getEntityId());
            // We can fail if there was pending meas or not connected.
            if (status != SUCCESS)
            {
              // Queue indication.
              Thermometer_storeIndications(&intervalInd);
            }
          }
        }
      }
    }
  }
}

/*********************************************************************
 * @fn      Thermometer_advertise
 *
 * @brief   Start advertisement when measurement is ready.
 *
 * @param   none
 *
 * @return  none
 */
static void Thermometer_advertise(void)
{
  // If not connected, toggle advertising.
  if (gapProfileState != GAPROLE_CONNECTED)
  {
    uint8_t current_adv_enabled_status;
    uint8_t new_adv_enabled_status;
    
    // Find the current GAP advertisement status.
    GAPRole_GetParameter(GAPROLE_ADVERT_ENABLED, &current_adv_enabled_status);
    
    if(current_adv_enabled_status == FALSE)
    {
      new_adv_enabled_status = TRUE;
    }
    
    // Change the GAP advertisement status .
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), 
                         &new_adv_enabled_status);   
  }
}

/*********************************************************************
 * @fn      Thermometer_startDiscEvt
 *
 * @brief   Start service discovery. 
 *
 * @param   none
 *
 * @return  none
 */
static void Thermometer_startDiscEvt(void)
{
  // Start service discovery.
  servDiscState = Time_discStart();
}

/*********************************************************************
 * @fn      Thermometer_disconnectEvt
 *
 * @brief   Disconnect. 
 * 
 * @param   none
 *
 * @return  none
 */
static void Thermometer_disconnectEvt(void)
{    
  // Terminate Connection.
  GAPRole_TerminateConnection();
}

/*********************************************************************
 * @fn      Thermometer_stateChangeCB
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state.
 *
 * @return  none
 */
static void Thermometer_stateChangeCB(gaprole_States_t newState)
{
  Thermometer_enqueueMsg(THERMOMETER_STATE_CHANGE_EVT, newState, NULL);
}

/*********************************************************************
 * @fn      Thermometer_processStateChangeEvt
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void Thermometer_processStateChangeEvt(gaprole_States_t newState)
{
  // If connected
  if (newState == GAPROLE_CONNECTED)
  {
    // Get connection handle.
    GAPRole_GetParameter(GAPROLE_CONNHANDLE, &thermometer_connHandle);

    // Get peer bd address.
    GAPRole_GetParameter(GAPROLE_CONN_BD_ADDR, &connDeviceAddr);
    
    // Set the Time module's copy of the connHandle.
    Time_connHandle = thermometer_connHandle;
    
    // If connected to device without bond do service discovery.
    if (memcmp(connDeviceAddr, timeAppBondedAddr, B_ADDR_LEN))
    {
      servDiscComplete = FALSE;
    }
    else
    {
      servDiscComplete = TRUE;
    }
    
    // If this was last connection address don't do discovery.
    if(!memcmp(connDeviceAddr, lastConnAddr, B_ADDR_LEN))
    {
      servDiscComplete = TRUE;
      connectedToLastAddress = true;
    }
    else
    {
      // Save the last connected address.
      memcpy(lastConnAddr, connDeviceAddr, B_ADDR_LEN);
    }

    // Initiate service discovery if necessary.
    if (servDiscComplete == FALSE)
    {
      // Start timer for service discovery.
      Util_startClock(&startDiscoveryClock);
    }
    
    // Start timer for disconnect.
    Util_startClock(&disconnectClock);
  }
  // If disconnected
  else if (gapProfileState == GAPROLE_CONNECTED && 
            newState != GAPROLE_CONNECTED)
  { 
    // Stop discovery clock in case it is running.
    Util_stopClock(&startDiscoveryClock);

    // Stop intermediate timers.
    Util_stopClock(&perIMeasClock);
    Util_stopClock(&perMeasClock);
  
    // Re-initialize state variables.
    servDiscState = DISC_IDLE;
    servDiscPostponed = FALSE;
    
    // Invalidate the connection handle.
    thermometer_connHandle = Time_connHandle = INVALID_CONNHANDLE;
  }
  // If started
  else if (newState == GAPROLE_STARTED)
  {
    // Time module configuration have not been set.
    Time_configDone = FALSE;
    
    // Initialize time clock.
    Time_clockInit();
  }
  
  // Update to new state.
  gapProfileState = newState;
}

/*********************************************************************
 * @fn      Thermometer_sendStoredMeas
 *
 * @brief   Prepare and send a stored Meas Indication.
 *
 * @param   none
 *
 * @return  none
 */
static void Thermometer_sendStoredMeas(void)
{ 
  // If previously connected to this peer send any stored measurements.
  if (thStoreStartIndex != thStoreIndex)
  {
    bStatus_t status = FAILURE;
    attHandleValueInd_t *pStoreInd = &thStoreMeas[thStoreStartIndex];
  
    if (pStoreInd->handle == THERMOMETER_TEMP_VALUE_POS)
    {
      // Send Measurement.
      status  = Thermometer_TempIndicate(thermometer_connHandle, 
                                         pStoreInd, ICall_getEntityId());
    }
    else if (pStoreInd->handle == THERMOMETER_INTERVAL_VALUE_POS)
    {
      // Send Interval.
      status = Thermometer_IntervalIndicate(thermometer_connHandle, 
                                            pStoreInd, ICall_getEntityId());
    }
    
    if (status == SUCCESS)
    {
      thStoreStartIndex = thStoreStartIndex + 1;
      
      // Wrap around buffer.
      if(thStoreStartIndex > TH_STORE_MAX)
      {
        thStoreStartIndex = 0;
      }
      
      // Clear out this Meas indication.
      memset(pStoreInd, 0, sizeof(attHandleValueInd_t));
    }
  }
}

/*********************************************************************
 * @fn      Thermometer_measIndicate
 *
 * @brief   Prepare and send a thermometer measurement notification.
 *
 * @param   none
 *
 * @return  none
 */
static void Thermometer_measIndicate(void)
{
  // Thermometer measurement value stored in this structure.
  attHandleValueInd_t thermometerMeas;

  thermometerMeas.pValue = GATT_bm_alloc(thermometer_connHandle,
                                         ATT_HANDLE_VALUE_IND,
                                         THERMOMETER_MEAS_LEN, NULL);
  if (thermometerMeas.pValue != NULL)
  {
    // ATT value notification structure.
    uint8_t *p = thermometerMeas.pValue;
    
    // Temperature.
    uint32_t temperature;
    
    // Flags.
    uint8_t flags = thermometerFlags[thermometerFlagsIdx];
    
    // Flags 1 byte long.
    *p++ = flags;
    
    if(flags & THERMOMETER_FLAGS_FARENHEIT)
    {
      temperature = (thermometerCelcius *9/5) +320;
    }
    else
    {
       temperature = thermometerCelcius;
    }
    
    temperature = 0xFF000000 | temperature;
    
    // Temperature is 4 bytes long.
    *p++ = BREAK_UINT32(temperature, 0);
    *p++ = BREAK_UINT32(temperature, 1);
    *p++ = BREAK_UINT32(temperature, 2);
    *p++ = BREAK_UINT32(temperature, 3);
     
    // Timestamp.
    if (flags & THERMOMETER_FLAGS_TIMESTAMP)
    {
      UTCTimeStruct time;
    
      // Get time structure from UTC.
      UTC_convertUTCTime(&time, UTC_getClock());
      
      *p++ = (time.year & 0x00FF);
      *p++ = (time.year & 0xFF00)>>8;
              
      *p++=time.month;    
      *p++=time.day;  
      *p++=time.hour;    
      *p++=time.minutes;    
      *p++=time.seconds;   
    }
    
    if(flags & THERMOMETER_FLAGS_TYPE)
    {
      uint8_t site;
      
      Thermometer_GetParameter(THERMOMETER_TYPE, &site); 
      *p++ =  site;    
    }
    
    thermometerMeas.len = (uint8_t) (p - thermometerMeas.pValue);
    thermometerMeas.handle = THERMOMETER_TEMP_VALUE_POS;
    
    // Send the temperature measurement indication
    if (Thermometer_TempIndicate(thermometer_connHandle, 
                                 &thermometerMeas,
                                 ICall_getEntityId()) != SUCCESS)
    {
      // Queue indication.
      Thermometer_storeIndications(&thermometerMeas);
      
      // Advertise measurement is ready.
      Thermometer_advertise();
    }
  }
}

/*********************************************************************
 * @fn      Thermometer_storeIndications
 *
 * @brief   Queue indications.
 *
 * @param   pInd - pointer to the indication struct.
 *
 * @return  none
 */
static void Thermometer_storeIndications(attHandleValueInd_t *pInd)
{
  attHandleValueInd_t *pStoreInd = &thStoreMeas[thStoreIndex];
  
  if (pStoreInd->pValue != NULL)
  {
    // Free old indication's payload.
    GATT_bm_free((gattMsg_t *)pStoreInd, ATT_HANDLE_VALUE_IND);
  }
  
  // Store measurement.
  memcpy(pStoreInd, pInd, sizeof(attHandleValueInd_t));
  
  // Store index.
  thStoreIndex = thStoreIndex + 1;
  
  if(thStoreIndex > TH_STORE_MAX)
  {
    thStoreIndex = 0;
  }
  
  if(thStoreIndex == thStoreStartIndex)
  {
    thStoreStartIndex = thStoreStartIndex + 1;
    
    if(thStoreStartIndex > TH_STORE_MAX)
    {
      thStoreStartIndex = 0;
    }
  }  
}

/*********************************************************************
 * @fn      Thermometer_ImeasNotify
 *
 * @brief   Prepare and send a thermometer measurement notification.
 *
 * @param   none
 *
 * @return  none
 */
static void Thermometer_ImeasNotify(void)
{
  if (temperatureIMeasCharConfig == true)
  {
    attHandleValueNoti_t thermometerIMeas;
    
    thermometerIMeas.pValue = GATT_bm_alloc(thermometer_connHandle,
                                            ATT_HANDLE_VALUE_NOTI, 
                                            THERMOMETER_IMEAS_LEN, NULL);
    if (thermometerIMeas.pValue != NULL)
    {
      // ATT value notification structure.
      uint8_t *p = thermometerIMeas.pValue;
      
      // Temperature.
      uint32_t temperature;
      
      // Flags.
      uint8_t flags = thermometerFlags[thermometerFlagsIdx];
      
      // Flags 1 byte long.
      *p++ = flags;
      
      if (flags & THERMOMETER_FLAGS_FARENHEIT)
      {
        temperature = (thermometerCelcius * 9/5) + 320;
      }
      else
      {
         temperature = thermometerCelcius;
      }
      
      temperature = 0xFF000000 | temperature;
        
      // Temperature is 4 bytes long.
      *p++ = BREAK_UINT32(temperature, 0);
      *p++ = BREAK_UINT32(temperature, 1);
      *p++ = BREAK_UINT32(temperature, 2);
      *p++ = BREAK_UINT32(temperature, 3);

      if (flags & THERMOMETER_FLAGS_TYPE)
      {
        uint8_t site;
        
        Thermometer_GetParameter(THERMOMETER_TYPE, &site); 
        
        *p++ = site;
      }
      
      thermometerIMeas.len = (uint8_t) (p - thermometerIMeas.pValue);

      if (Thermometer_IMeasNotify(thermometer_connHandle, 
                                  &thermometerIMeas) != SUCCESS)
      {
        GATT_bm_free((gattMsg_t *)&thermometerIMeas, ATT_HANDLE_VALUE_NOTI);
      }
    }
  }
}

/*********************************************************************
 * @fn      Thermometer_serviceCB
 *
 * @brief   Callback function for thermometer service.
 *
 * @param   event - service event
 *
 * @return  none
 */
static void Thermometer_serviceCB(uint8_t event)
{
  // Enqueue the event.
  Thermometer_enqueueMsg(THERMOMETER_SERVICE_EVT, event, NULL);
}

/*********************************************************************
 * @fn      Thermometer_processServiceEvt
 *
 * @brief   Callback function for thermometer service.
 *
 * @param   event - service event
 *
 * @return  none
 */
static void Thermometer_processServiceEvt(uint8_t event)
{
  switch (event)
  {
    case THERMOMETER_TEMP_IND_ENABLED:
      // CCC was updated.
      Thermometer_updateCCC();
      break;
        
    case THERMOMETER_TEMP_IND_DISABLED:
      temperatureMeasCharConfig = false;
      thMeasTimerRunning = FALSE;
      
      // Stop periodic measurement.
      Util_stopClock(&perMeasClock);
      break;

    case THERMOMETER_IMEAS_NOTI_ENABLED:
      temperatureIMeasCharConfig = true;
      
      if (gapProfileState == GAPROLE_CONNECTED)
      {
        // Start periodic IMeasurement.
        Util_startClock(&perIMeasClock);
      }
      break;

    case  THERMOMETER_IMEAS_NOTI_DISABLED:
      temperatureIMeasCharConfig = false;
    
      // Stop periodic IMeasurement.
      Util_stopClock(&perIMeasClock);
      break;
  
    case THERMOMETER_INTERVAL_IND_ENABLED:
      if (gapProfileState == GAPROLE_CONNECTED)
      {
        temperatureIntervalConfig = true;
      }
      break;

    case THERMOMETER_INTERVAL_IND_DISABLED:
      if (gapProfileState == GAPROLE_CONNECTED)
      {
        temperatureIntervalConfig = false;
      }
      break;   
 
    case THERMOMETER_INTERVAL_SET:
      Thermometer_updateInterval();
      break;
      
    default:  
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      Thermometer_updateInterval
 *
 * @brief   Measurement interval has been updated. Update periodic 
 *          measurement timer accordingly.
 *
 * @param   none
 *
 * @return  none
 */
static void Thermometer_updateInterval(void)
{
  if (thMeasTimerRunning == TRUE)
  {
    uint8_t notify_interval;
  
    // Read stored interval value.
    Thermometer_GetParameter(THERMOMETER_INTERVAL, &notify_interval); 
  
    if (notify_interval == 0)
    {
      // Stop periodic measurement.
      Util_stopClock(&perMeasClock);
      
      thMeasTimerRunning = FALSE;
    }
    else
    {
      int32_t n32 = ((uint32_t)(notify_interval)) * 1000;
      
      // Restart interval timer.
      Util_restartClock(&perMeasClock, n32);
    }
  }
}

/*********************************************************************
 * @fn      Thermometer_updateCCC
 *
 * @brief   CCC of the thermometer service has been updated.  Send
 *          stored measurements.
 *
 * @param   none
 *
 * @return  none
 */
static void Thermometer_updateCCC(void)
{ 
  // This event is triggered when CCC is enabled.
  if (gapProfileState == GAPROLE_CONNECTED)
  {
    temperatureMeasCharConfig = true;
    
    // If previously connected and measurements are active send stored.
    if (connectedToLastAddress == true)
    {
      // Send stored measurements.
      Thermometer_sendStoredMeas();
    }
    
    // Only start meas timer if it's not running.
    if (thMeasTimerRunning == FALSE)
    {
      uint8_t notify_interval;
      
      // Read stored interval value.
      Thermometer_GetParameter(THERMOMETER_INTERVAL, &notify_interval);
      
      // Zero interval means should not perform measurement.
      if (notify_interval != 0)
      {
        int32_t n32 = ((uint32_t)(notify_interval)) * 1000;
        
        // Start interval timer.
        Util_restartClock(&perMeasClock, n32);
        
        thMeasTimerRunning = TRUE;
      }
    }
  }
}

/*********************************************************************
 * @fn      Thermometer_performPeriodicTask
 *
 * @brief   Perform a periodic application task.
 *
 * @param   none
 *
 * @return  none
 */
static void Thermometer_performPeriodicTask(void)
{
  // Measurement Ready - send if Client Configuration is Configured.
  if (temperatureMeasCharConfig == true)
  {
    uint8_t notify_interval;
  
    // Read stored interval value.
    Thermometer_GetParameter(THERMOMETER_INTERVAL, &notify_interval); 
    
    // If interval is zero don't send indication.
    if (notify_interval != 0)
    {
      int32_t n32 = ((uint32_t)(notify_interval)) * 1000;
      
      // Send thermometer measurement notification.
      Thermometer_measIndicate();
      
      // Start interval timer.
      Util_restartClock(&perMeasClock, n32);
    }
  }
}

/*********************************************************************
 * @fn      Thermometer_performPeriodicImeasTask
 *
 * @brief   Perform a periodic application task.
 *
 * @param   none
 *
 * @return  none
 */
static void Thermometer_performPeriodicImeasTask(void)
{
  if (gapProfileState == GAPROLE_CONNECTED)
  {
    // Send thermometer measurement notification.
    Thermometer_ImeasNotify();
    
    // Start interval timer (simulated fast measurement for display).
    Util_startClock(&perIMeasClock);
  }
}

/*********************************************************************
 * @fn      Thermometer_passcodeCB
 *
 * @brief   Passcode callback.
 *
 * @param   deviceAddr - address of device requesting a passcode.
 * @param   connHandle - connection handle of device.
 * @param   uiInputs - this device's input capabilities for entering passcode.
 * @param   uiOutputs - this device's output capabilities for displaying 
 *                      passcode.
 *
 * @return  none
 */
static void Thermometer_passcodeCB(uint8_t *deviceAddr, uint16_t connHandle, 
                                   uint8_t uiInputs, uint8_t uiOutputs)
{
  // Queue the event.
  Thermometer_enqueueMsg(THERMOMETER_PASSCODE_EVT, 0, NULL);
}

/*********************************************************************
 * @fn      Thermometer_processPasscodeEvt
 *
 * @brief   Handles passcode callback event.
 *
 * @param   connHandle - connection handle of device requesting a
 *                       passcode.
 *
 * @return  none
 */
static void Thermometer_processPasscodeEvt(uint16_t connHandle)
{
  // Send passcode response.
  GAPBondMgr_PasscodeRsp(connHandle, SUCCESS, 0);  
}

/*********************************************************************
 * @fn      Thermometer_pairStateCB
 *
 * @brief   Pairing state callback.
 *
 * @param   connHandle - connection handle of device with which this device
 *                       is in a pairing process.
 * @param   state - state of pairing process.
 * @param   status - completion status of the pairing state.
 *
 * @return  none
 */
static void Thermometer_pairStateCB(uint16_t connHandle, uint8_t state, 
                                    uint8_t status)
{
  uint8_t *pData;
  
  // Allocate the data.
  if ((pData = ICall_malloc(sizeof(uint8_t))))
  {
    *pData = status;
    
    // Queue the event.
    Thermometer_enqueueMsg(THERMOMETER_PAIR_STATE_EVT, state, pData);
  }
}

/*********************************************************************
 * @fn      pairStateEvt
 *
 * @brief   Pairing state callback.
 *
 * @param   state - state of pairing process.
 * @param   status - completion status of the pairing state.
 *
 * @return  none
 */
static void Thermometer_processPairStateEvt(uint8_t state, uint8_t status)
{
  if (state == GAPBOND_PAIRING_STATE_STARTED)
  {
    // Stop the clock.  Service discovery will continue after pairing.
    Util_stopClock(&startDiscoveryClock);
    
    // Set flag.
    servDiscPostponed = TRUE;
  }
  else if (state == GAPBOND_PAIRING_STATE_COMPLETE)
  {
    if (status == SUCCESS)
    {
      // Store peer device's address in case we reconnect again.
      memcpy(timeAppBondedAddr, connDeviceAddr, B_ADDR_LEN);
      
      // If discovery was postponed earlier, start discovery now.
      if (servDiscPostponed && servDiscComplete == FALSE)
      {
        servDiscPostponed = FALSE;
        
        // Set an event for service discovery.
        events |= THERMOMETER_START_DISC_EVT;
        
        // Post the application's semaphore.
        Semaphore_post(sem);
      }
    }
  }
}

/*********************************************************************
 * @fn      Thermometer_clockHandler
 *
 * @brief   Clock handle for all clock events.  This function stores an event 
 *          flag and wakes up the application's event processor.
 *
 * @param   arg - event flag.
 *
 * @return  none
 */
static void Thermometer_clockHandler(UArg arg)
{
  // Store the event.
  events |= arg;
  
  // Wake up the application.
  Semaphore_post(sem);
}

/*********************************************************************
 * @fn      Thermometer_enqueueMsg
 *
 * @brief   Creates a message and puts the message in RTOS queue.
 *
 * @param   event - message event.
 * @param   state - message state.
 * @param   pData - message data pointer.
 *
 * @return  TRUE or FALSE
 */
static uint8_t Thermometer_enqueueMsg(uint16_t event, uint8_t state,
                                      uint8_t *pData)
{
  thermometerEvt_t *pMsg;
  
  // Create dynamic pointer to message.
  if (pMsg = ICall_malloc(sizeof(thermometerEvt_t)))
  {
    pMsg->hdr.event = event;
    pMsg->hdr.state = state;
    pMsg->pData = pData;
    
    // Enqueue the message.
    return Util_enqueueMsg(appMsgQueue, sem, (uint8_t *)pMsg);
  }
  
  return FALSE;
}


/*********************************************************************
*********************************************************************/
