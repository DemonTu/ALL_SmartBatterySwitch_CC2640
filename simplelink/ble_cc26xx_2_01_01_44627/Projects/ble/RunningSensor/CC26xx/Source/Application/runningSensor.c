/*******************************************************************************
  Filename:       runningSensor.c
  Revised:        $Date: 2015-07-20 11:31:07 -0700 (Mon, 20 Jul 2015) $
  Revision:       $Revision: 44370 $

  Description:    This file contains the Running Speed and Cadence (RSC) sample 
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
#include "runningservice.h"

#include "peripheral.h"
#include "gapbondmgr.h"

#include "osal_snv.h"
#include "ICallBleAPIMSG.h"

#include "util.h"
#include "board_key.h"
#include "Board.h"

#include "runningSensor.h"

/*********************************************************************
 * MACROS
 */

#define DISTANCE_TRAVELED(speed)  ((speed) * ((DEFAULT_RSC_PERIOD) / 1000))

/*********************************************************************
 * CONSTANTS
 */

// Fast advertising interval in 625us units.
#define DEFAULT_FAST_ADV_INTERVAL                48

// Duration of fast advertising duration in ms.
#define DEFAULT_FAST_ADV_DURATION                20000

// Duration of advertising to white list members only after link termination.
#define DEFAULT_WHITE_LIST_ADV_DURATION          10000

// Slow advertising interval in 625us units
#define DEFAULT_SLOW_ADV_INTERVAL                1600

// Duration of slow advertising duration in ms 
// (set to 0 for continuous advertising).
#define DEFAULT_SLOW_ADV_DURATION                20000

// How often to perform sensor's periodic event (ms).
#define DEFAULT_RSC_PERIOD                       2000

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

#define DEFAULT_PAIRING_PARAMETER                GAPBOND_PAIRING_MODE_INITIATE

#define USING_WHITE_LIST                         TRUE

#define REQ_BONDING                              TRUE

// Time alloted for service discovery before requesting more energy efficient 
// connection parameters.
#define SVC_DISC_DELAY                           5000

// After 15 seconds of no user input with notifications off, terminate 
// connection.
#define NEGLECT_TIMEOUT_DELAY                    15000

// Setting this to true lets this device disconnect after a period of no use.
#define USING_NEGLECT_TIMEOUT                    TRUE

// Delay for reset of device's bonds, connections, alerts
#define RSC_START_RESET_DELAY                    3000 // in ms, 3 seconds

// Delay between steps after reset event has begun.
#define RSC_IN_RESET_DELAY                       500  // in ms

// For simulated measurements.
#define FLAGS_IDX_MAX                            4

// Values for simulated measurements. units in revolutions in centimeters.
#define STRIDE_LENGTH_RUNNING                    500
#define STRIDE_LENGTH_WALKING                    350

// Cadence = footfalls/minute.
#define RUNNING_CADENCE                          180
#define WALKING_CADENCE                          85
// Speed = centimeters/second.
#define RUNNING_SPEED                            180
#define WALKING_SPEED                            50

// Motion that the runner is making.
#define STANDING_STILL                           0
#define WALKING_MOTION                           1
#define RUNNING_MOTION                           2
#define MOTION_IDX_MAX                           3

#define RSC_MEAS_LEN                             10

// Task configuration.
#define RSC_TASK_PRIORITY                        1
#define RSC_TASK_STACK_SIZE                      644

// Running Sensor Task Events.
#define RSC_PERIODIC_EVT                         0x0001
#define RSC_CONN_PARAM_UPDATE_EVT                0x0002
#define RSC_NEGLECT_TIMEOUT_EVT                  0x0004
#define RSC_RESET_EVT                            0x0008
#define RSC_SERVICE_EVT                          0x0010
#define RSC_STATE_CHANGE_EVT                     0x0020
#define RSC_KEY_CHANGE_EVT                       0x0040
#define RSC_CMD_SEND_IND_EVT                     0x0080

/*********************************************************************
 * TYPEDEFS
 */

// App event passed from profiles.
typedef struct
{
  appEvtHdr_t hdr; // Event header.
  uint8_t *pData;  // Event data.
} rscEvt_t;

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
static Task_Struct rscTask;
static Char rscTaskStack[RSC_TASK_STACK_SIZE];

static gaprole_States_t gapProfileState = GAPROLE_INIT;

static uint8_t sensorUsingWhiteList = FALSE;

// GAP Profile - Name attribute for SCAN RSP data.
static uint8_t scanData[] =
{
  0x0B,   // length of this data
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,
  'R',
  'S',
  'C',
  ' ',
  'S',
  'e',
  'n',
  's',
  'o',
  'r',
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
  LO_UINT16(RSC_SERV_UUID),
  HI_UINT16(RSC_SERV_UUID),
};

// Device name attribute value.
static uint8_t attDeviceName[GAP_DEVICE_NAME_LEN] = "RSC Sensor";

// GAP connection handle.
static uint16_t gapConnHandle;

// Flags for simulated measurements.
static const uint8_t sensorFlags[FLAGS_IDX_MAX] =
{
  RSC_FLAGS_ALL,
  RSC_FLAGS_AT_REST,
  RSC_FLAGS_STRIDE,
  RSC_FLAGS_DIST
};

static const uint8_t motionCycle[MOTION_IDX_MAX] =
{
  STANDING_STILL,
  WALKING_MOTION,
  RUNNING_MOTION,
};

static uint8_t motionIdx = 0;

// Flag index.
static uint8_t sensorFlagsIdx = 0;

// Advertising user-cancelled state.
static bool sensorAdvCancelled = FALSE;

// RSC parameters.
static uint32_t totalDistance = 0;
static uint16_t instSpeed = 0;
static uint8_t instCadence = 0;
static uint16_t instStrideLength = 0;
static uint8_t sensorLocation = RSC_NO_SENSOR_LOC;
static uint8_t motion = STANDING_STILL;

// Used to determine if a reset delay is in progress.
static uint8_t resetInProgress = FALSE;

/*********************************************************************
 * LOCAL FUNCTIONS
 */
// Application task and event processing.
static void RunningSensor_init(void);
static void RunningSensor_taskFxn(UArg a0, UArg a1);
static void RunningSensor_processStackMsg(ICall_Hdr *pMsg);
static void RunningSensor_processAppMsg(rscEvt_t *pMsg);
static void RunningSensor_processGATTMsg(gattMsgEvent_t *pMsg);
static void RunningSensor_periodicTask(void);
static void RunningSensor_measNotify(void);
static uint8_t RunningSensor_enqueueMsg(uint16_t event, uint8_t state,
                                        uint8_t *pData);
static void RunningSensor_clockHandler(UArg arg);
static void RunningSensor_handleResetEvt(void);

// Running service.
static bStatus_t RunningSensor_serviceCB(uint8_t event, uint32_t *pNewCummVal);
static bStatus_t RunningSensor_processServiceEvt(uint8_t event, 
                                                 uint32_t newCummVal);
static void RunningSensor_controlPointCB(void);

// GAP role.
static void RunningSensor_stateChangeCB(gaprole_States_t newState);
static void RunningSensor_processStateChangeEvt(gaprole_States_t newState);

// Keys.
static void RunningSensor_keyPressHandler(uint8_t keysPressed);
static void RunningSensor_handleKeys(uint8_t shift, uint8_t keys);

/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks.
static gapRolesCBs_t runningPeripheralCB =
{
  RunningSensor_stateChangeCB   // Profile State Change Callbacks.
};

// Bond Manager Callbacks.
static const gapBondCBs_t runningBondCB =
{
  NULL,                         // Passcode callback (not used).
  NULL                          // Pairing state callback.
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      RunningSensor_createTask
 *
 * @brief   Task creation function for the Running Sensor application.
 *
 * @param   none
 *
 * @return  none
 */
void RunningSensor_createTask(void)
{
  Task_Params taskParams;
    
  // Configure task.
  Task_Params_init(&taskParams);
  taskParams.stack = rscTaskStack;
  taskParams.stackSize = RSC_TASK_STACK_SIZE;
  taskParams.priority = RSC_TASK_PRIORITY;
  
  Task_construct(&rscTask, RunningSensor_taskFxn, &taskParams, NULL);
}

/*********************************************************************
 * @fn      RunningSensor_init
 *
 * @brief   Initialization function for the Running App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notification ...).
 *
 * @param   none
 *
 * @return  none
 */
void RunningSensor_init(void)
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
  Util_constructClock(&periodicClock, RunningSensor_clockHandler,
                      DEFAULT_RSC_PERIOD, 0, false, RSC_PERIODIC_EVT);
  Util_constructClock(&paramUpdateClock, RunningSensor_clockHandler,
                      SVC_DISC_DELAY, 0, false, RSC_CONN_PARAM_UPDATE_EVT);
  
#if USING_NEGLECT_TIMEOUT
  Util_constructClock(&neglectClock, RunningSensor_clockHandler,
                      NEGLECT_TIMEOUT_DELAY, 0, false, RSC_NEGLECT_TIMEOUT_EVT);
#endif //USING_NEGLECT_TIMEOUT
  
  // Periodic clock for reset events.
  Util_constructClock(&resetClock, RunningSensor_clockHandler,
                      RSC_START_RESET_DELAY, RSC_IN_RESET_DELAY, 
                      false, RSC_RESET_EVT);

  // Register for all key events - This app will handle all key events.
  Board_initKeys(RunningSensor_keyPressHandler);

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

    GAPRole_SetParameter(GAPROLE_SCAN_RSP_DATA, sizeof (scanData), scanData);
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
  RunningService_addService(GATT_ALL_SERVICES);
  DevInfo_AddService();
  
  // Register for running service callback.
  RunningService_register(RunningSensor_serviceCB,
                          RunningSensor_controlPointCB);
  
  // Setup RSC profile data.
  {
    uint16_t features = RSC_FULL_SUPPORT;
    uint8_t sensorLocationCurrent = RSC_SENSOR_LOC_0;
    uint8_t sensorLocation1 = RSC_SENSOR_LOC_0;
    uint8_t sensorLocation2 = RSC_SENSOR_LOC_1;
    uint8_t sensorLocation3 = RSC_SENSOR_LOC_2;
    uint8_t sensorLocation4 = RSC_SENSOR_LOC_3;

    // Add available sensor locations.
    RunningService_setParameter(RSC_AVAIL_SENS_LOCS, sizeof(uint8_t), 
                                &sensorLocation1);
    RunningService_setParameter(RSC_AVAIL_SENS_LOCS, sizeof(uint8_t), 
                                &sensorLocation2);
    RunningService_setParameter(RSC_AVAIL_SENS_LOCS, sizeof(uint8_t), 
                                &sensorLocation3);
    RunningService_setParameter(RSC_AVAIL_SENS_LOCS, sizeof(uint8_t), 
                                &sensorLocation4);

    // Set sensor location.
    RunningService_setParameter(RSC_SENS_LOC, sizeof(uint8_t), 
                                &sensorLocationCurrent);

    // Support all features.
    RunningService_setParameter(RSC_FEATURE, sizeof(uint16_t), &features);
  }
  
  // Start the Device.
  VOID GAPRole_StartDevice(&runningPeripheralCB);

  // Register with bond manager after starting device.
  GAPBondMgr_Register((gapBondCBs_t *)&runningBondCB);
}

/*********************************************************************
 * @fn      RunningSensor_taskFxn
 *
 * @brief   Running Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   a0, a1 - not used.
 *
 * @return  none
 */
void RunningSensor_taskFxn(UArg a0, UArg a1)
{
  // Initialize the application.
  RunningSensor_init();
  
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
          RunningSensor_processStackMsg((ICall_Hdr *)pMsg);
        }
                
        if (pMsg)
        {
          ICall_freeMsg(pMsg);
        }
      }

      // If RTOS queue is not empty, process app message.
      while (!Queue_empty(appMsgQueue))
      {
        rscEvt_t *pMsg = (rscEvt_t *)Util_dequeueMsg(appMsgQueue);
        if (pMsg)
        {
          // Process message.
          RunningSensor_processAppMsg(pMsg);
          
          // Free the space from the message.
          ICall_free(pMsg);
        }
      }
    }

    if (events)
    { 
      // Running sensor periodic event.
      if (events & RSC_PERIODIC_EVT)
      {
        events &= ~RSC_PERIODIC_EVT;
        
        // Perform periodic sensor's periodic task.
        RunningSensor_periodicTask();
      }

      // Parameter update event.
      if (events & RSC_CONN_PARAM_UPDATE_EVT)
      {
        events &= ~RSC_CONN_PARAM_UPDATE_EVT;
        
        // Send param update.  If it fails, retry until successful.
        GAPRole_SendUpdateParam(DEFAULT_DESIRED_MIN_CONN_INTERVAL, 
                                DEFAULT_DESIRED_MAX_CONN_INTERVAL,
                                DEFAULT_DESIRED_SLAVE_LATENCY, 
                                DEFAULT_DESIRED_CONN_TIMEOUT,
                                GAPROLE_RESEND_PARAM_UPDATE);

#if USING_NEGLECT_TIMEOUT
        // Assuming service discovery complete, start neglect timer.
        Util_startClock(&neglectClock);
#endif //USING_NEGLECT_TIMEOUT
      }

#if USING_NEGLECT_TIMEOUT
      // Neglect timer expired.
      if (events & RSC_NEGLECT_TIMEOUT_EVT)
      {
        events &= ~RSC_NEGLECT_TIMEOUT_EVT;
        
        // No user input, terminate connection.
        GAPRole_TerminateConnection();
      }
#endif //USING_NEGLECT_TIMEOUT
      
      // Soft reset event.
      if (events & RSC_RESET_EVT)
      {
        events &= ~RSC_RESET_EVT;
        
        RunningSensor_handleResetEvt();
      }
    }
  }
}

/*********************************************************************
 * @fn      RunningSensor_processStackMsg
 *
 * @brief   Process an incoming stack message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void RunningSensor_processStackMsg(ICall_Hdr *pMsg)
{
  switch (pMsg->event)
  {
    case GATT_MSG_EVENT:
      // Process GATT message.
      RunningSensor_processGATTMsg((gattMsgEvent_t *)pMsg);
      break;

    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      RunningSensor_processGATTMsg
 *
 * @brief   process incoming GATT msg.
 *
 * @param   pMsg - pointer to message to be read.
 *
 * @return  none
 */
void RunningSensor_processGATTMsg(gattMsgEvent_t *pMsg)
{
  if (pMsg->method == ATT_HANDLE_VALUE_CFM)
  {
    // Indication receipt was confirmed by the client.
    // This is a placeholder for future.
  }
  
  GATT_bm_free(&pMsg->msg, pMsg->method);
}

/*********************************************************************
 * @fn      RunningSensor_processAppMsg
 *
 * @brief   Process an incoming callback from a profile.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */                       
static void RunningSensor_processAppMsg(rscEvt_t *pMsg)
{
  switch (pMsg->hdr.event)
  {
    case RSC_STATE_CHANGE_EVT:
      RunningSensor_processStateChangeEvt((gaprole_States_t)pMsg->hdr.state);
      break;
      
    case RSC_KEY_CHANGE_EVT:
      RunningSensor_handleKeys(0, pMsg->hdr.state);
      break;
      
      
    // Running service callback event.
    case RSC_SERVICE_EVT:
      RunningSensor_processServiceEvt(pMsg->hdr.event, (uint32_t)*pMsg->pData);
      
      ICall_free(pMsg->pData);
      break;
  
    // Running service control point write event.
    case RSC_CMD_SEND_IND_EVT:
      RunningService_sendRSCCmdIndication(selfEntity);    
      break;
      
    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      RunningSensor_keyPressHandler
 *
 * @brief   Key event handler function
 *
 * @param   keys - keys pressed.
 *
 * @return  none
 */
void RunningSensor_keyPressHandler(uint8_t keys)
{
  // Enqueue the event.
  RunningSensor_enqueueMsg(RSC_KEY_CHANGE_EVT, keys, NULL);
}

/*********************************************************************
 * @fn      RunningSensor_HandleKeys
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
static void RunningSensor_handleKeys(uint8_t shift, uint8_t keys)
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
    else if (gapProfileState == GAPROLE_CONNECTED)
    {
      // If connected, change rate of motion.
      if(++motionIdx == MOTION_IDX_MAX)
      {
        motionIdx = 0;
      }

      motion = motionCycle[motionIdx];
    }
  }
}

/*********************************************************************
 * @fn      RunningSensor_measNotify
 *
 * @brief   Prepare and send a RSC measurement notification.
 *
 * @param   none
 *
 * @return  none
 */
static void RunningSensor_measNotify(void)
{
  static uint16_t centimeters = 0;
  attHandleValueNoti_t sensorMeas;

  sensorMeas.pValue = GATT_bm_alloc(gapConnHandle, ATT_HANDLE_VALUE_NOTI, 
                                    RSC_MEAS_LEN, NULL);
  if (sensorMeas.pValue != NULL)
  {
    uint8_t *p = sensorMeas.pValue;
    uint8_t flags = sensorFlags[sensorFlagsIdx];

    switch(motion)
    {
      case STANDING_STILL:
        instSpeed = instCadence = instStrideLength = STANDING_STILL;
        
        // 0 for walking bit.
        flags = flags & 0xFB; // 0b1111 1011
        break;

      case WALKING_MOTION:
        instStrideLength = STRIDE_LENGTH_WALKING;
        instCadence = WALKING_CADENCE;
        instSpeed = WALKING_SPEED;
        
        // 0 for walking bit.
        flags = flags & 0xFB;
        break;

      case RUNNING_MOTION:
        instStrideLength = STRIDE_LENGTH_RUNNING;
        instCadence = RUNNING_CADENCE;
        instSpeed = RUNNING_SPEED;
        
        // set in 1 for walking bit.
        flags = flags | 0x04;
        break;

      default: 
        // Do nothing.
        break;
    }

    // Add distance.
    centimeters += (uint16_t)DISTANCE_TRAVELED(instSpeed);

    // If travelled at least a meter
    if (centimeters >= 100)
    {
      // Add distance, truncated to meters.
      totalDistance += (centimeters / 100);
      
      // And continue to store the remaining centimeters.
      centimeters %= 100;
    }

    // Build RSC measurement structure from simulated values
    // Flags simulate the isPresent bits.
    *p++ = flags;

    // Included regardless of flags.
    *p++ = LO_UINT16(instSpeed);
    *p++ = HI_UINT16(instSpeed);
    *p++ = instCadence;

    if (flags & RSC_FLAGS_STRIDE)
    {
      *p++ = LO_UINT16(instStrideLength);
      *p++ = HI_UINT16(instStrideLength);
    }

    if (flags & RSC_FLAGS_DIST)
    {
      *p++ = BREAK_UINT32(totalDistance, 0);
      *p++ = BREAK_UINT32(totalDistance, 1);
      *p++ = BREAK_UINT32(totalDistance, 2);
      *p++ = BREAK_UINT32(totalDistance, 3);
    }

    // Get length.
    sensorMeas.len = (uint8_t)(p - sensorMeas.pValue);

    // Send to service to send the notification.
    if (RunningService_measNotify(gapConnHandle, &sensorMeas) != SUCCESS)
    {
      GATT_bm_free((gattMsg_t *)&sensorMeas, ATT_HANDLE_VALUE_NOTI);
    }
  }
}

/*********************************************************************
 * @fn      RunningSensor_stateChangeCB
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void RunningSensor_stateChangeCB(gaprole_States_t newState)
{
  RunningSensor_enqueueMsg(RSC_STATE_CHANGE_EVT, newState, NULL);
}

/*********************************************************************
 * @fn      RunningSensor_processStateChangeEvt
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void RunningSensor_processStateChangeEvt(gaprole_States_t newState)
{
  // If connected
  if (newState == GAPROLE_CONNECTED)
  {
    // Get the connection handle.
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

    // Get the bond count.
    GAPBondMgr_GetParameter(GAPBOND_BOND_COUNT, &bondCount);
    
    // If enabled and previously bonded, turn on white list if not already done.
    if(USING_WHITE_LIST && sensorUsingWhiteList == FALSE && bondCount > 0)
    {
      uint8_t value = GAP_FILTER_POLICY_WHITE;

      GAPRole_SetParameter(GAPROLE_ADV_FILTER_POLICY, sizeof(uint8_t), &value);

      sensorUsingWhiteList = TRUE;
    }
    
    // If waiting after a connection timeout
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

    // If white list is in use, turn off white list filtering to allow general 
    // access.
    if (sensorUsingWhiteList == TRUE)
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
    // If fast advertising was interrupted to cancel white list
    else if (((!USING_WHITE_LIST) || whiteListUsed) &&
              (GAP_GetParamValue(TGAP_GEN_DISC_ADV_INT_MIN) == DEFAULT_FAST_ADV_INTERVAL))
    {
      uint8_t advState = TRUE;

      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, DEFAULT_FAST_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, DEFAULT_FAST_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_MIN, DEFAULT_FAST_ADV_DURATION);
      GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &advState);
    }
    // If fast advertising or was using white list switch to slow advertising.
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

  // Update state.
  gapProfileState = newState;
}

/*********************************************************************
 * @fn      RunningSensor_serviceCB
 *
 * @brief   Callback function for RSC service.
 *
 * @param   event       - service event
 * @param   pNewCummVal - pointer to new total distance data
 *                        if specified by event.  NULL otherwise.
 *
 * @return  SUCCESS if operation successful. FAILURE, otherwise.
 */
static bStatus_t RunningSensor_serviceCB(uint8_t event, uint32_t *pNewCummVal)
{
  if ((event == RSC_CMD_START_SENS_CALIB) && 
      (sensorFlags[sensorFlagsIdx] != RSC_FLAGS_AT_REST))
  {
    // Induce a calibration error for conformance testing.
    return (FAILURE);
  }
  
  // Allocate message data.
  uint32_t *pData;
  
  if ((pData = ICall_malloc(sizeof(uint32_t)))) 
  {
    if (pNewCummVal != 0)
    {
      *pData = *pNewCummVal;
    }
    else
    {
      *pData = 0;
    }
  }
  
  RunningSensor_enqueueMsg(RSC_SERVICE_EVT, event, (uint8_t *)pData);
  
  return SUCCESS;
}

/*********************************************************************
 * @fn      RunningSensor_processServiceEvt
 *
 * @brief   Handler RSC service callback.
 *
 * @param   event       - service event
 * @param   newCummVal  - new total distance data if specified by event.  
 *                        0 otherwise.
 *
 * @return  SUCCESS if operation successful. FAILURE, otherwise.
 */
static bStatus_t RunningSensor_processServiceEvt(uint8_t event,
                                                 uint32_t newCummVal)
{
  bStatus_t status = SUCCESS;

  switch (event)
  {
    case RSC_CMD_SET_CUMM_VAL:
#if USING_NEGLECT_TIMEOUT
      // If notifications aren't enabled
      if (!Util_isActive(&periodicClock))
      {
        // Restart neglect timer.
        Util_startClock(&neglectClock);
      }
#endif //USING_NEGLECT_TIMEOUT

      // Update total distance.
      totalDistance = newCummVal;
      break;
      
    case RSC_CMD_START_SENS_CALIB:
      // Do nothing for now
      break;
      
    case RSC_CMD_UPDATE_SENS_LOC:
#if USING_NEGLECT_TIMEOUT
      // If notifications aren't enabled
      if (!Util_isActive(&periodicClock))
      {
        // Restart neglect timer.
        Util_startClock(&neglectClock);
      }
#endif //USING_NEGLECT_TIMEOUT

      // Get updated sensor location.
      RunningService_getParameter(RSC_SENS_LOC, &sensorLocation);
      break;

    case RSC_MEAS_NOTI_ENABLED:
#if (USING_NEGLECT_TIMEOUT)
      // Stop neglect timer.
      Util_stopClock(&neglectClock);
#endif //USING_NEGLECT_TIMEOUT

      // If connected start periodic measurement for notifications.
      if (gapProfileState == GAPROLE_CONNECTED)
      {
        Util_startClock(&periodicClock);
      }
      break;

    case RSC_MEAS_NOTI_DISABLED:
      // Stop periodic measurement.
      Util_stopClock(&periodicClock);

#if USING_NEGLECT_TIMEOUT
      // Start neglect timer.
      Util_startClock(&neglectClock);
#endif //USING_NEGLECT_TIMEOUT
      break;

    case RSC_READ_ATTR:
    case RSC_WRITE_ATTR:
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

  return (status);
}

/*********************************************************************
 * @fn      RunningSensor_periodicTask
 *
 * @brief   Perform a periodic running application task.
 *
 * @param   none
 *
 * @return  none
 */
static void RunningSensor_periodicTask(void)
{
  if (gapProfileState == GAPROLE_CONNECTED)
  {
    // Send speed and cadence measurement notification.
    RunningSensor_measNotify();

    // Restart periodic timer.
    Util_startClock(&periodicClock);
  }
}

/*********************************************************************
 * @fn      RunningSensor_handleResetEvt
 *
 * @brief   "soft" resets the device.  This puts the device into a waiting 
 *           state, clears all white list, bonding and GATT service handle 
 *           information about previously previously connected devices.
 *
 * @param   none
 *
 * @return  none
 */
static void RunningSensor_handleResetEvt(void)
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

    // Turn off advertising
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &value);
  }
  else if (USING_WHITE_LIST == TRUE && isWLClear == FALSE)
  {
    // Temporary variable.
    uint8_t value = GAP_FILTER_POLICY_ALL;

    // Set internal white list flag to true.
    isWLClear = TRUE;
    
    // Turn off white list filter policy.
    GAPRole_SetParameter(GAPROLE_ADV_FILTER_POLICY, sizeof(uint8_t), &value);

    sensorUsingWhiteList = FALSE;

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
 * @fn      RunningSensor_controlPointCB
 *
 * @brief   Running service control point was written.  Set an event
 *          to send out an indication to the client after the write
 *          response is sent.
 *
 * @param   none
 *
 * @ret     none
 */
static void RunningSensor_controlPointCB(void)
{
  RunningSensor_enqueueMsg(RSC_CMD_SEND_IND_EVT, 0, NULL);
}

/*********************************************************************
 * @fn      RunningSensor_clockHandler
 *
 * @brief   Clock handle for all clock events.  This function stores an event 
 *          flag and wakes up the application's event processor.
 *
 * @param   arg - event flag.
 *
 * @return  none
 */
static void RunningSensor_clockHandler(UArg arg)
{
  // Store the event.
  events |= arg;
  
  // Wake up the application.
  Semaphore_post(sem);
}

/*********************************************************************
 * @fn      RunningSensor_enqueueMsg
 *
 * @brief   Creates a message and puts the message in RTOS queue.
 *
 * @param   event - message event.
 * @param   status - message status.
 * @param   pData - message data
 *
 * @return  TRUE or FALSE
 */
static uint8_t RunningSensor_enqueueMsg(uint16_t event, uint8_t state, 
                                        uint8_t *pData)
{
  rscEvt_t *pMsg;
  
  // Create dynamic pointer to message.
  if (pMsg = ICall_malloc(sizeof(rscEvt_t)))
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
