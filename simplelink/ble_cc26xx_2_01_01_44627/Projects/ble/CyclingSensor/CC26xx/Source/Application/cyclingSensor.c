/*******************************************************************************
  Filename:       cyclingSensor.c
  Revised:        $Date: 2015-07-20 11:31:07 -0700 (Mon, 20 Jul 2015) $
  Revision:       $Revision: 44370 $

  Description:    This file contains the Cycling Speed and Cadence (CSC) sample
                  application for use with the CC2650 Bluetooth Low Energy 
                  Protocol Stack.

  Copyright 2012 - 2015 Texas Instruments Incorporated. All rights reserved.

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
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>

#include "gatt.h"
#include "hci.h"

#include "gapgattserver.h"
#include "gattservapp.h"
#include "gatt_profile_uuid.h"
#include "gatt_uuid.h"
#include "devinfoservice.h"
#include "cyclingservice.h"

#include "peripheral.h"
#include "gapbondmgr.h"

#include "osal_snv.h"
#include "ICallBleAPIMSG.h"

#include "util.h"
#include "board_key.h"
#include "Board.h"

#include "cyclingSensor.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

// Fast advertising interval in 625us units.  625*32 = 20ms (recommended)
#define DEFAULT_FAST_ADV_INTERVAL                32

// Duration of fast advertising duration in ms.
#define DEFAULT_FAST_ADV_DURATION                20000

// Duration of advertising to white list members only after link termination.
#define DEFAULT_WHITE_LIST_ADV_DURATION          10000

// Slow advertising interval in 625us units.  625*1704 = 1065ms (recommended)
#define DEFAULT_SLOW_ADV_INTERVAL                1704

// Duration of slow advertising duration in ms 
// (set to 0 for continuous advertising)
#define DEFAULT_SLOW_ADV_DURATION                20000

// How often to perform sensor's periodic event (ms).
#define DEFAULT_CSC_PERIOD                       1000

// Whether to enable automatic parameter update request when a connection is 
// formed.
#define DEFAULT_ENABLE_UPDATE_REQUEST            FALSE

// Minimum connection interval (units of 1.25ms) if automatic parameter update 
// request is enabled.
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL        200

// Maximum connection interval (units of 1.25ms) if automatic parameter update 
// request is enabled.
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL        1600

// Slave latency to use if automatic parameter update request is enabled.
#define DEFAULT_DESIRED_SLAVE_LATENCY            0

// Supervision timeout value (units of 10ms) if automatic parameter update 
// request is enabled.
#define DEFAULT_DESIRED_CONN_TIMEOUT             1000

// Sensor sends a slave security request.
#define DEFAULT_PAIRING_PARAMETER                GAPBOND_PAIRING_MODE_INITIATE

// Bonded devices' addresses are stored in white list.
#define USING_WHITE_LIST                         TRUE

// Request bonding.
#define REQ_BONDING                              TRUE

// Time alloted for service discovery before requesting more energy efficient 
// connection parameters.
#define SVC_DISC_DELAY                           5000

// After 15 seconds of no user input with notifications off, terminate 
// connection.
#define NEGLECT_TIMEOUT_DELAY                    15000

// Setting this to true lets this device disconnect after a period of no use.
#define USING_NEGLECT_TIMEOUT                    TRUE

// Delay for reset of device's bonds, connections, alerts.
#define CSC_START_RESET_DELAY                    3000 // in ms, 3 seconds

// Delay between steps after reset event has begun.
#define CSC_IN_RESET_DELAY                       500  // in ms

// For simulated measurements.
#define FLAGS_IDX_MAX                            4

// values for simulated measurements. units in revolutions.
#define CRANK_REV_INCREMENT                      2
#define WHEEL_REV_INCREMENT                      2

// values for simulated measurements.  units 1/1024 seconds.  
// 512 * (1/1024) = 500 milliseconds
#define WHEEL_EVT_INCREMENT                      512
#define CRANK_EVT_INCREMENT                      512

#define CSC_MEAS_LEN                             11

// Task configuration.
#define CSC_TASK_PRIORITY                        1
#define CSC_TASK_STACK_SIZE                      644
   
// Cycling Sensor Task Events.
#define CSC_SERVICE_EVT                          0x0001
#define CSC_PERIODIC_EVT                         0x0002
#define CSC_CONN_PARAM_UPDATE_EVT                0x0004
#define CSC_RESET_EVT                            0x0008   
#define CSC_KEY_CHANGE_EVT                       0x0010
#define CSC_STATE_CHANGE_EVT                     0x0020
#define CSC_CMD_SEND_IND_EVT                     0x0040
#define CSC_NEGLECT_TIMEOUT_EVT                  0x0080

/*********************************************************************
 * TYPEDEFS
 */

// App event passed from profiles.
typedef struct
{
  appEvtHdr_t hdr; // Event header
  uint8_t *pData;  // Event data
} cscEvt_t;

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
static Clock_Struct periodicClock;
static Clock_Struct resetClock;
static Clock_Struct paramUpdateClock;

#if USING_NEGLECT_TIMEOUT
static Clock_Struct neglectClock;
#endif //USING_NEGLECT_TIMEOUT

// Queue object used for app messages.
static Queue_Struct appMsg;
static Queue_Handle appMsgQueue;

// events flag for internal application events.
static uint16_t events;

// Task configuration.
Task_Struct cscTask;
Char cscTaskStack[CSC_TASK_STACK_SIZE];

static gaprole_States_t gapProfileState = GAPROLE_INIT;

static uint8_t sensorUsingWhiteList = FALSE;

// GAP Profile - Name attribute for SCAN RSP data.
static uint8_t scanData[] =
{
  0x0B,   // length of this data
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,
  'C',
  'S',
  'C',
  ' ',
  'S',
  'e',
  'n',
  's',
  'o',
  'r',

  // Tx power level
  0x02,   // length of this data
  GAP_ADTYPE_POWER_LEVEL,
  0       // 0dBm, default Tx power level
};

static uint8_t advertData[] =
{
  // flags
  0x02,
  GAP_ADTYPE_FLAGS,
  GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
  // service UUIDs
  0x03,
  GAP_ADTYPE_16BIT_MORE,
  LO_UINT16(CSC_SERV_UUID),
  HI_UINT16(CSC_SERV_UUID),
};

// Device name attribute value.
static uint8_t attDeviceName[GAP_DEVICE_NAME_LEN] = "CSC Sensor";

// GAP connection handle.
static uint16_t gapConnHandle;

// Flags for simulated measurements.
static const uint8_t sensorFlags[FLAGS_IDX_MAX] =
{
  CSC_FLAGS_SPEED_CADENCE,
  CSC_FLAGS_AT_REST,
  CSC_FLAGS_SPEED,
  CSC_FLAGS_CADENCE
};

// Flag index.
static uint8_t sensorFlagsIdx = 0;

// Advertising user-cancelled state.
static bool sensorAdvCancelled = FALSE;

// CSC parameters.
static uint32_t cummWheelRevs = 100;
static uint16_t cummCrankRevs = 50;
static uint16_t lastWheelEvtTime = 60;
static uint16_t lastCrankEvtTime = 70;
static uint8_t sensorLocation = CSC_SENSOR_LOC_OTHER;

// Used to determine if a reset delay is in progress.
static uint8_t resetInProgress = FALSE;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
// Application task and event processing.
static void CyclingSensor_init(void);
static void CyclingSensor_taskFxn(UArg a0, UArg a1);
static void CyclingSensor_processStackMsg(ICall_Hdr *pMsg);
static void CyclingSensor_processGATTMsg(gattMsgEvent_t *pMsg);
static void CyclingSensor_processAppMsg(cscEvt_t *pMsg);
static void CyclingSensor_periodicTask(void);
static void CyclingSensor_measNotify(void);
static uint8_t CyclingSensor_enqueueMsg(uint16_t event, uint8_t state,
                                        uint8_t *pdata);
static void CyclingSensor_clockHandler(UArg arg);
static void CyclingSensor_handleResetEvt(void);

// Cycling service.
static void CyclingSensor_serviceCB(uint8_t event, uint32_t *pNewCummVal);
static void CyclingSensor_processServiceEvt(uint8_t event, 
                                            uint32_t newCummVal);
static void CyclingSensor_controlPointCB(void);

// GAP role.
static void CyclingSensor_stateChangeCB(gaprole_States_t newState);
static void CyclingSensor_processStateChangeEvt(gaprole_States_t newState);

// Keys.
static void CyclingSensor_keyPressHandler(uint8_t keysPressed);
static void CyclingSensor_handleKeys(uint8_t shift, uint8_t keys);

/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static gapRolesCBs_t cyclingPeripheralCB =
{
  CyclingSensor_stateChangeCB  // Profile State Change Callbacks
};

// Bond Manager Callbacks
static const gapBondCBs_t cyclingBondCB =
{
  NULL,                        // Passcode callback
  NULL                         // Pairing state callback
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      CyclingSensor_createTask
 *
 * @brief   Task creation function for the Running Sensor application.
 *
 * @param   none
 *
 * @return  none
 */
void CyclingSensor_createTask(void)
{
  Task_Params taskParams;
    
  // Configure task.
  Task_Params_init(&taskParams);
  taskParams.stack = cscTaskStack;
  taskParams.stackSize = CSC_TASK_STACK_SIZE;
  taskParams.priority = CSC_TASK_PRIORITY;
  
  Task_construct(&cscTask, CyclingSensor_taskFxn, &taskParams, NULL);
}

/*********************************************************************
 * @fn      CyclingSensor_init
 *
 * @brief   Initialization function for the Cycling App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notificaiton ...).
 *
 * @param   none.
 *
 * @return  none
 */
void CyclingSensor_init(void)
{
	// ******************************************************************
  // N0 STACK API CALLS CAN OCCUR BEFORE THIS CALL TO ICall_registerApp
  // ******************************************************************
  // Register the current thread as an ICall dispatcher application
  // so that the application can send and receive messages.
  ICall_registerApp(&selfEntity, &sem);
    
  // Hard code the DB Address till CC2650 board gets its own IEEE address
  //uint8 bdAddress[B_ADDR_LEN] = { 0x22, 0x22, 0x22, 0x22, 0x22, 0x22 };
  //HCI_EXT_SetBDADDRCmd(bdAddress);
  
  // Set device's Sleep Clock Accuracy
  //HCI_EXT_SetSCACmd(40);
  
  // Create an RTOS queue for message from profile to be sent to app.
  appMsgQueue = Util_constructQueue(&appMsg);
  
  // Create one-shot clocks for internal periodic events.
  Util_constructClock(&periodicClock, CyclingSensor_clockHandler,
                      DEFAULT_CSC_PERIOD, 0, false, CSC_PERIODIC_EVT);
  Util_constructClock(&paramUpdateClock, CyclingSensor_clockHandler,
                      SVC_DISC_DELAY, 0, false, CSC_CONN_PARAM_UPDATE_EVT);
  
#if USING_NEGLECT_TIMEOUT
  Util_constructClock(&neglectClock, CyclingSensor_clockHandler,
                      NEGLECT_TIMEOUT_DELAY, 0, false, CSC_NEGLECT_TIMEOUT_EVT);
#endif //USING_NEGLECT_TIMEOUT
  
  // Periodic clock for reset events.
  Util_constructClock(&resetClock, CyclingSensor_clockHandler,
                      CSC_START_RESET_DELAY, CSC_IN_RESET_DELAY, 
                      false, CSC_RESET_EVT);

  // Register for all key events - This app will handle all key events.
  Board_initKeys(CyclingSensor_keyPressHandler);
  
  // Setup the GAP Peripheral Role Profile.
  {
    // For the CC2650, device doesn't start advertising until button is pressed.
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

    GAPRole_SetParameter(GAPROLE_SCAN_RSP_DATA, sizeof(scanData), scanData);
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

  // Set the GAP Characteristics.
  GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, attDeviceName);

  // Setup the GAP Bond Manager.
  {
    uint32_t passkey = 0; // passkey "000000"
    uint8_t pairMode = DEFAULT_PAIRING_PARAMETER;
    uint8_t mitm = FALSE;
    uint8_t ioCap = GAPBOND_IO_CAP_NO_INPUT_NO_OUTPUT;
    uint8_t bonding = REQ_BONDING;
    uint8_t autoSync = USING_WHITE_LIST;

    GAPBondMgr_SetParameter(GAPBOND_DEFAULT_PASSCODE, sizeof(uint32_t), 
                            &passkey);
    GAPBondMgr_SetParameter(GAPBOND_PAIRING_MODE, sizeof(uint8_t), &pairMode);
    GAPBondMgr_SetParameter(GAPBOND_MITM_PROTECTION, sizeof(uint8_t), &mitm);
    GAPBondMgr_SetParameter(GAPBOND_IO_CAPABILITIES, sizeof(uint8_t), &ioCap);
    GAPBondMgr_SetParameter(GAPBOND_BONDING_ENABLED, sizeof(uint8_t), &bonding);
    GAPBondMgr_SetParameter(GAPBOND_AUTO_SYNC_WL, sizeof(uint8_t), &autoSync);
  }

  // Initialize GATT attributes.
  GGS_AddService(GATT_ALL_SERVICES);         // GAP
  GATTServApp_AddService(GATT_ALL_SERVICES); // GATT attributes
  CyclingService_addService(GATT_ALL_SERVICES);
  DevInfo_AddService();

  // Register for cycling service callback.
  CyclingService_register(CyclingSensor_serviceCB, 
                          CyclingSensor_controlPointCB);

  // Setup CSC profile data.
  {
    uint8_t features = CSC_FULL_SUPPORT;
    uint8_t sensorLocationCurrent = CSC_SENSOR_LOC_REAR_DROPOUT;
    uint8_t sensorLocation1 = CSC_SENSOR_LOC_REAR_DROPOUT;
    uint8_t sensorLocation2 = CSC_SENSOR_LOC_LEFT_CRANK;
    uint8_t sensorLocation3 = CSC_SENSOR_LOC_REAR_WHEEL;
    uint8_t sensorLocation4 = CSC_SENSOR_LOC_HIP;

    // Add available sensor locations.
    CyclingService_setParameter(CSC_AVAIL_SENS_LOCS, 1, &sensorLocation1);
    CyclingService_setParameter(CSC_AVAIL_SENS_LOCS, 1, &sensorLocation2);
    CyclingService_setParameter(CSC_AVAIL_SENS_LOCS, 1, &sensorLocation3);
    CyclingService_setParameter(CSC_AVAIL_SENS_LOCS, 1, &sensorLocation4);

    // Set sensor location.
    CyclingService_setParameter(CSC_SENS_LOC, 1, &sensorLocationCurrent);

    // Support all features.
    CyclingService_setParameter(CSC_FEATURE, 1, &features);
  }
  
  // Start the Device.
  VOID GAPRole_StartDevice(&cyclingPeripheralCB);

  // Register with bond manager after starting device.
  GAPBondMgr_Register((gapBondCBs_t *)&cyclingBondCB);  
}

/*********************************************************************
 * @fn      CyclingSensor_taskFxn
 *
 * @brief   Cycling Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   a0, a1 - not used.
 *
 * @return  none
 */
void CyclingSensor_taskFxn(UArg a0, UArg a1)
{
  // Initialize the application.
  CyclingSensor_init();
  
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
          CyclingSensor_processStackMsg((ICall_Hdr *)pMsg);
        }
                
        if (pMsg)
        {
          ICall_freeMsg(pMsg);
        }
      }

      // If RTOS queue is not empty, process app message.
      while (!Queue_empty(appMsgQueue))
      {
        cscEvt_t *pMsg = (cscEvt_t *)Util_dequeueMsg(appMsgQueue);
        if (pMsg)
        {
          // Process message.
          CyclingSensor_processAppMsg(pMsg);
          
          // Free the space from the message.
          ICall_free(pMsg);
        }
      }
    }
    
    if (events)
    { 
      // Cycling service control point write event.
      if (events & CSC_CMD_SEND_IND_EVT)
      {
        events &= ~CSC_CMD_SEND_IND_EVT;
        
        CyclingService_sendCSCCmdIndication(selfEntity);
      }
      
      // Cycling sensor periodic event.
      if (events & CSC_PERIODIC_EVT)
      {
        events &= ~CSC_PERIODIC_EVT;
        
        // Perform periodic sensor's periodic task.
        CyclingSensor_periodicTask();
      }

      // Parameter update event.
      if (events & CSC_CONN_PARAM_UPDATE_EVT)
      {
        events &= ~CSC_CONN_PARAM_UPDATE_EVT;
        
        // Send param update.  If it fails, retry until successful.
        GAPRole_SendUpdateParam(DEFAULT_DESIRED_MIN_CONN_INTERVAL, DEFAULT_DESIRED_MAX_CONN_INTERVAL,
                                 DEFAULT_DESIRED_SLAVE_LATENCY, DEFAULT_DESIRED_CONN_TIMEOUT,
                                 GAPROLE_RESEND_PARAM_UPDATE);

  #if USING_NEGLECT_TIMEOUT
        // Assuming service discovery complete, start neglect timer.
        Util_startClock(&neglectClock);
  #endif //USING_NEGLECT_TIMEOUT        
      }

  #if USING_NEGLECT_TIMEOUT
      // Neglect timer expired.
      if (events & CSC_NEGLECT_TIMEOUT_EVT)
      {
        events &= ~CSC_NEGLECT_TIMEOUT_EVT;
        
        // No user input, terminate connection.
        GAPRole_TerminateConnection();
      }
  #endif //USING_NEGLECT_TIMEOUT
      
      // Soft reset event.
      if (events & CSC_RESET_EVT)
      {
        events &= ~CSC_RESET_EVT;
        
        CyclingSensor_handleResetEvt();
      }
    }
  }
}

/*********************************************************************
 * @fn      CyclingSensor_processStackMsg
 *
 * @brief   Process an incoming stack message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void CyclingSensor_processStackMsg(ICall_Hdr *pMsg)
{
  switch (pMsg->event)
  {
    case GATT_MSG_EVENT:
      // Process GATT message.
      CyclingSensor_processGATTMsg((gattMsgEvent_t *)pMsg);
      break;

    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      CyclingSensor_processGATTMsg
 *
 * @brief   process incoming GATT msg.
 *
 * @param   pMsg- pointer to message to be read.
 *
 * @return  none
 */
static void CyclingSensor_processGATTMsg(gattMsgEvent_t *pMsg)
{
  if (pMsg->method == ATT_HANDLE_VALUE_CFM)
  {
    // Indication receipt was confirmed by the client.
    // This is a placeholder for future.
  }
  
  GATT_bm_free(&pMsg->msg, pMsg->method);
}

/*********************************************************************
 * @fn      CyclingSensor_processAppMsg
 *
 * @brief   Process an incoming callback from a profile.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */                       
static void CyclingSensor_processAppMsg(cscEvt_t *pMsg)
{
  switch (pMsg->hdr.event)
  {
    case CSC_STATE_CHANGE_EVT:
      CyclingSensor_processStateChangeEvt((gaprole_States_t)pMsg->hdr.state);
      break;
      
    case CSC_KEY_CHANGE_EVT:
      CyclingSensor_handleKeys(0, pMsg->hdr.state);
      break;
      
    // Cycling service callback event.
    case CSC_SERVICE_EVT:
      {
        uint32_t cummVal = *((uint32_t*)(pMsg->pData));
        
        CyclingSensor_processServiceEvt(pMsg->hdr.state, cummVal);
        
        ICall_free(pMsg->pData);
      }
      break;
      
    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      CyclingSensor_keyPressHandler
 *
 * @brief   Key event handler function
 *
 * @param   keys - keys pressed.
 *
 * @return  none
 */
void CyclingSensor_keyPressHandler(uint8_t keys)
{
  // Enqueue the event.
  CyclingSensor_enqueueMsg(CSC_KEY_CHANGE_EVT, keys, NULL);
}

/*********************************************************************
 * @fn      CyclingSensor_handleKeys
 *
 * @brief   Handles all key events for this device.
 *
 * @param   shift - true if in shift/alt.
 * @param   keys - bit field for key events. Valid entries:
 *                 KEY_SELECT
 *                 KEY_UP
 *                 KEY_RIGHT
 *
 * @return  none
 */
static void CyclingSensor_handleKeys(uint8_t shift, uint8_t keys)
{
  // If a reset was in progress, cancel it.
  if (resetInProgress == TRUE)
  {
    resetInProgress = FALSE;
    
    Util_stopClock(&resetClock);
    
#if USING_NEGLECT_TIMEOUT
    // If using neglect time and in a connection.
    if (gapProfileState == GAPROLE_CONNECTED)
    {
      // Restart neglect timer.
      Util_startClock(&neglectClock);
    }
#endif //USING_NEGLECT_TIMEOUT
    
    return;
  }
  
  if (keys & KEY_SELECT)
  {
    // Reset in progress has started.
    resetInProgress = TRUE;

#if USING_NEGLECT_TIMEOUT
    // Stop the neglect timer.
    Util_stopClock(&neglectClock);
#endif //USING_NEGLECT_TIMEOUT
    
    // Start reset timer.
    Util_startClock(&resetClock);
  }
  else if (keys & KEY_UP)
  {
    // Set simulated measurement flag index.
    if (++sensorFlagsIdx == FLAGS_IDX_MAX)
    {
      sensorFlagsIdx = 0;
    }
  }
  else if (keys & KEY_RIGHT)
  {
    // If not in a connection, toggle advertising on and off.
    if (gapProfileState != GAPROLE_CONNECTED)
    {
      uint8_t advStatus;

      // Set fast advertising interval for user-initiated connections.
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, DEFAULT_FAST_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, DEFAULT_FAST_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_MIN, DEFAULT_WHITE_LIST_ADV_DURATION);

      // Toggle GAP advertisement status.
      GAPRole_GetParameter(GAPROLE_ADVERT_ENABLED, &advStatus);
      advStatus = !advStatus;

      // If not already using white list, begin to do so.
      // Only do so if about to begin advertising.
      if (USING_WHITE_LIST && advStatus == TRUE)
      {
        uint8_t bondCount = 0;

        GAPBondMgr_GetParameter(GAPBOND_BOND_COUNT, &bondCount);

        // If white lists are not in active use and we have at least one bond
        // store.
        if ((sensorUsingWhiteList == FALSE) && (bondCount > 0))
        {
          uint8_t value = GAP_FILTER_POLICY_WHITE;

          GAPRole_SetParameter(GAPROLE_ADV_FILTER_POLICY, sizeof(uint8_t), 
                               &value);

          sensorUsingWhiteList = TRUE;
        }
      }

      GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &advStatus);

      // Set state variable.
      if (advStatus == FALSE)
      {
        sensorAdvCancelled = TRUE;
      }
    }
  }
}

/*********************************************************************
 * @fn      CyclingSensor_measNotify
 *
 * @brief   Prepare and send a CSC measurement notification
 *
 * @param   none
 *
 * @return  none
 */
static void CyclingSensor_measNotify(void)
{
  attHandleValueNoti_t sensorMeas;
  
  sensorMeas.pValue = GATT_bm_alloc(gapConnHandle, ATT_HANDLE_VALUE_NOTI,
                                    CSC_MEAS_LEN, NULL);
  if (sensorMeas.pValue != NULL)
  {
    uint8_t *p = sensorMeas.pValue;
    uint8_t flags = sensorFlags[sensorFlagsIdx];

    // Build CSC measurement structure from simulated values
    // Flags simulate the isPresent bits.
    *p++ = flags;

    // If flag is present, add speed data into measurement.
    if (flags & CSC_FLAGS_SPEED)
    {
      *p++ = BREAK_UINT32(cummWheelRevs, 0);
      *p++ = BREAK_UINT32(cummWheelRevs, 1);
      *p++ = BREAK_UINT32(cummWheelRevs, 2);
      *p++ = BREAK_UINT32(cummWheelRevs, 3);

      *p++ = LO_UINT16(lastWheelEvtTime);
      *p++ = HI_UINT16(lastWheelEvtTime);

      // Update simulated values (simulate in the reverse direction).
      // Don't allow revolutions to roll over.
      if (cummWheelRevs < WHEEL_REV_INCREMENT) 
      {
        cummWheelRevs = 0;
      }
      else
      {
        cummWheelRevs -= WHEEL_REV_INCREMENT;
      }

      lastWheelEvtTime += WHEEL_EVT_INCREMENT;
    }

    // If present, add cadence data into measurement.
    if (flags & CSC_FLAGS_CADENCE)
    {
      *p++ = LO_UINT16(cummCrankRevs);
      *p++ = HI_UINT16(cummCrankRevs);

      *p++ = LO_UINT16(lastCrankEvtTime);
      *p++ = HI_UINT16(lastCrankEvtTime);

      // Update simulated values.
      cummCrankRevs += CRANK_REV_INCREMENT;
      lastCrankEvtTime += CRANK_EVT_INCREMENT;
    }

    // Get length.
    sensorMeas.len = (uint8_t) (p - sensorMeas.pValue);

    // Send to service to send the notification.
    if (CyclingService_measNotify(gapConnHandle, &sensorMeas) != SUCCESS)
    {
      GATT_bm_free((gattMsg_t *)&sensorMeas, ATT_HANDLE_VALUE_NOTI);
    }
  }
}

/*********************************************************************
 * @fn      CyclingSensor_stateChangeCB
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void CyclingSensor_stateChangeCB(gaprole_States_t newState)
{
  CyclingSensor_enqueueMsg(CSC_STATE_CHANGE_EVT, newState, NULL);
}

/*********************************************************************
 * @fn      CyclingSensor_stateChangeCB
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void CyclingSensor_processStateChangeEvt(gaprole_States_t newState)
{
  // If connected
  if (newState == GAPROLE_CONNECTED)
  {
    // Get connection handle.
    GAPRole_GetParameter(GAPROLE_CONNHANDLE, &gapConnHandle);

    // Begin delay for service discovery.
    Util_startClock(&paramUpdateClock);
  }
  // If disconnected
  else if (gapProfileState == GAPROLE_CONNECTED &&
           newState != GAPROLE_CONNECTED)
  {
    uint8_t advState = TRUE;
    uint8_t bondCount = 0;

    // Stop periodic measurement.
    Util_stopClock(&periodicClock);

    // If not already using white list, begin to do so.
    GAPBondMgr_GetParameter(GAPBOND_BOND_COUNT, &bondCount);

    if(USING_WHITE_LIST && sensorUsingWhiteList == FALSE && bondCount > 0)
    {
      uint8_t value = GAP_FILTER_POLICY_WHITE;

      GAPRole_SetParameter(GAPROLE_ADV_FILTER_POLICY, sizeof(uint8_t), &value);

      sensorUsingWhiteList = TRUE;
    }

    if (newState == GAPROLE_WAITING_AFTER_TIMEOUT)
    {
      // Link loss timeout-- use fast advertising.
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, DEFAULT_FAST_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, DEFAULT_FAST_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_MIN, DEFAULT_WHITE_LIST_ADV_DURATION);
    }
    else
    {
      // Use slow advertising.
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, DEFAULT_SLOW_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, DEFAULT_SLOW_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_MIN, DEFAULT_WHITE_LIST_ADV_DURATION);
    }

    // Enable advertising.
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &advState);
  }
  // If advertising stopped
  else if (gapProfileState == GAPROLE_ADVERTISING &&
            newState == GAPROLE_WAITING)
  {
    uint8_t whiteListUsed = FALSE;

    // If white list is in use, disable to allow general access.
    if(sensorUsingWhiteList == TRUE)
    {
      uint8_t value = GAP_FILTER_POLICY_ALL;

      GAPRole_SetParameter(GAPROLE_ADV_FILTER_POLICY, sizeof(uint8_t), &value);

      whiteListUsed = TRUE;

      sensorUsingWhiteList = FALSE;
    }

    // If advertising stopped by user
    if (sensorAdvCancelled)
    {
      sensorAdvCancelled = FALSE;
    }
    // If fast advertising interrupted to cancel white list
    else if (((!USING_WHITE_LIST) || whiteListUsed) &&
              (GAP_GetParamValue(TGAP_GEN_DISC_ADV_INT_MIN) == DEFAULT_FAST_ADV_INTERVAL))
    {
      uint8_t advState = TRUE;

      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, DEFAULT_FAST_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, DEFAULT_FAST_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_MIN, DEFAULT_FAST_ADV_DURATION);
      GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &advState);
    }
    // If fast advertising switch to slow or if was already slow but using white list.
    else if (((!USING_WHITE_LIST) || whiteListUsed) ||
             (GAP_GetParamValue(TGAP_GEN_DISC_ADV_INT_MIN) == DEFAULT_FAST_ADV_INTERVAL))
    {
      uint8_t advState = TRUE;

      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, DEFAULT_SLOW_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, DEFAULT_SLOW_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_MIN, DEFAULT_SLOW_ADV_DURATION);
      GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &advState);
    }
  }
  // If started
  else if (newState == GAPROLE_STARTED)
  {
    // Set the system ID from the bd addr.
    uint8_t systemId[DEVINFO_SYSTEM_ID_LEN];
    GAPRole_GetParameter(GAPROLE_BD_ADDR, systemId);

    // Shift three bytes up.
    systemId[7] = systemId[5];
    systemId[6] = systemId[4];
    systemId[5] = systemId[3];

    // Set middle bytes to zero.
    systemId[4] = 0;
    systemId[3] = 0;

    DevInfo_SetParameter(DEVINFO_SYSTEM_ID, DEVINFO_SYSTEM_ID_LEN, systemId);
  }

  gapProfileState = newState;
}

/*********************************************************************
 * @fn      CyclingSensor_serviceCB
 *
 * @brief   Callback function for CSC service.
 *
 * @param   event       - service event
 * @param   pNewCummVal - pointer to new total distance data
 *                        if specified by event.  NULL otherwise.
 *
 * @return  SUCCESS if operation successful. FAILURE, otherwise.
 */
static void CyclingSensor_serviceCB(uint8_t event, uint32_t *pNewCummVal)
{
  uint32_t *pData;
  
  // Allocate message data
  if ((pData = ICall_malloc(sizeof(uint32_t))))
  {
    *pData = (pNewCummVal != NULL) ? *pNewCummVal : 0;
  
    // Queue the event.
    CyclingSensor_enqueueMsg(CSC_SERVICE_EVT, event, (uint8_t *)pData);
  }
}

/*********************************************************************
 * @fn      CyclingSensor_processServiceEvt
 *
 * @brief   Handler function for CSC service callback.
 *
 * @param   event       - service event
 * @param   newCummVal  - new wheel revolution data if specified by event.
 *
 * @return  none
 */
static void CyclingSensor_processServiceEvt(uint8_t event, uint32_t newCummVal)
{
  switch (event)
  {
    case CSC_CMD_SET_CUMM_VAL:
#if USING_NEGLECT_TIMEOUT
      // If notifications aren't enabled
      if (!Util_isActive(&periodicClock))
      {
        // Restart neglect timer.
        Util_startClock(&neglectClock);
      }
#endif //USING_NEGLECT_TIMEOUT
      
      cummWheelRevs = newCummVal;
      break;

    case CSC_CMD_UPDATE_SENS_LOC:
#if USING_NEGLECT_TIMEOUT
      // If notifications aren't enabled
      if (!Util_isActive(&periodicClock))
      {
        // Restart neglect timer.
        Util_startClock(&neglectClock);
      }
#endif //USING_NEGLECT_TIMEOUT
      
      // Get updated sensor location.
      CyclingService_getParameter(CSC_SENS_LOC, &sensorLocation);
      break;

    case CSC_MEAS_NOTI_ENABLED:     
#if (USING_NEGLECT_TIMEOUT)
      // Stop neglect timer.
      Util_stopClock(&neglectClock);
#endif //USING_NEGLECT_TIMEOUT
      
      // If connected start periodic measurement.
      if (gapProfileState == GAPROLE_CONNECTED)
      {
        Util_startClock(&periodicClock);
      }
      break;

    case CSC_MEAS_NOTI_DISABLED:
      // Stop periodic measurement.
      Util_stopClock(&periodicClock);
      
#if USING_NEGLECT_TIMEOUT
      // Start neglect timer.
      Util_startClock(&neglectClock);
#endif //USING_NEGLECT_TIMEOUT
      break;

    case CSC_READ_ATTR:
    case CSC_WRITE_ATTR:
#if USING_NEGLECT_TIMEOUT
      // If notifications aren't enabled
      if (!Util_isActive(&periodicClock))
      {
        // Restart neglect timer.
        Util_startClock(&neglectClock);
      }
#endif //USING_NEGLECT_TIMEOUT
      break;

    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      CyclingSensor_periodicTask
 *
 * @brief   Perform a periodic cycling application task.
 *
 * @param   none
 *
 * @return  none
 */
static void CyclingSensor_periodicTask(void)
{
  if (gapProfileState == GAPROLE_CONNECTED)
  {
    // Send speed and cadence measurement notification.
    CyclingSensor_measNotify();

    // Restart periodic timer.
    Util_startClock(&periodicClock);
  }
}

/*********************************************************************
 * @fn      CyclingSensor_handleResetEvt
 *
 * @brief   "soft" resets the device.  This puts the device into a waiting 
 *           state, clears all white list, bonding and GATT service handle 
 *           information about previously previously connected devices.
 *
 * @param   none
 *
 * @return  none
 */
static void CyclingSensor_handleResetEvt(void)
{
  static uint8_t isWLClear = FALSE;
  
  if (gapProfileState == GAPROLE_CONNECTED)
  { 
    // Exit the connection.
    GAPRole_TerminateConnection();
  }
  else if (gapProfileState == GAPROLE_ADVERTISING)
  {
    uint8_t value = FALSE;

    // Turn off advertising.
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &value);
  }
  else if (USING_WHITE_LIST == TRUE && isWLClear == FALSE)
  {
    // Set internal white list flag to true.
    isWLClear = TRUE;
    
    // Disable white list use with advertising.
    sensorUsingWhiteList = FALSE;
    
    // Temporary variable.
    uint8_t value = GAP_FILTER_POLICY_ALL;

    // Turn off white list filter policy.
    GAPRole_SetParameter(GAPROLE_ADV_FILTER_POLICY, sizeof(uint8_t), &value);

    // Clear the white list.
    HCI_LE_ClearWhiteListCmd();
  }
  else if ((gapProfileState == GAPROLE_STARTED) ||
            (gapProfileState == GAPROLE_WAITING) ||
            (gapProfileState == GAPROLE_WAITING_AFTER_TIMEOUT))
  {
    uint8_t eraseBonds = TRUE;

    // Stop the periodic expirations of the reset clock.
    Util_stopClock(&resetClock);
    
    // Set internal white list flag to false for next reset event.
    isWLClear = FALSE;
    
    // Erase all bonds.
    GAPBondMgr_SetParameter(GAPBOND_ERASE_ALLBONDS, 0, &eraseBonds);
    
    // Turn on GREEN LED for set time.
    //HalLedSet(HAL_LED_1, HAL_LED_MODE_BLINK);
  }  
}

/*********************************************************************
 * @fn      CyclingSensor_controlPointCB
 *
 * @brief   Cycling service control point was written.  Set an event
 *          to send out an indication to the client after the write
 *          response is sent.
 *
 * @param   none
 *
 * @ret     none
 */
static void CyclingSensor_controlPointCB(void)
{
  // Set the event.
  events |= CSC_CMD_SEND_IND_EVT;
  
  // Post the application's semaphore.
  Semaphore_post(sem);
}

/*********************************************************************
 * @fn      CyclingSensor_clockHandler
 *
 * @brief   Clock handle for all clock events.  This function stores an event 
 *          flag and wakes up the application's event processor.
 *
 * @param   arg - event flag.
 *
 * @return  none
 */
static void CyclingSensor_clockHandler(UArg arg)
{
  // Store the event.
  events |= arg;
  
  // Wake up the application.
  Semaphore_post(sem);
}

/*********************************************************************
 * @fn      CyclingSensor_enqueueMsg
 *
 * @brief   Creates a message and puts the message in RTOS queue.
 *
 * @param   event  - message event.
 * @param   state  - message state.
 * @param   pData  - message data pointer.
 *
 * @return  TRUE or FALSE
 */
static uint8_t CyclingSensor_enqueueMsg(uint16_t event, uint8_t state, 
                                        uint8_t *pData)
{
  cscEvt_t *pMsg;
  
  // Create dynamic pointer to message.
  if (pMsg = ICall_malloc(sizeof(cscEvt_t)))
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
