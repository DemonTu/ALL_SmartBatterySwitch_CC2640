/*******************************************************************************
  Filename:       timeapp.c
  Revised:        $Date: 2015-07-20 11:31:07 -0700 (Mon, 20 Jul 2015) $
  Revision:       $Revision: 44370 $

  Description:    This file contains the time app sample application 
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
#include "hci_tl.h"
#include "linkdb.h"
#include "gatt.h"
#include "gapgattserver.h"
#include "gattservapp.h"
#include "gatt_profile_uuid.h"
#include "peripheral.h"
#include "gapbondmgr.h"

#include "osal_snv.h"
#include "ICallBleAPIMSG.h"

#include "util.h"
#include "board_key.h"
#include "board_lcd.h"
#include "Board.h"

#include "UTC_clock.h"
#include "time_clock.h"
#include "bletime.h"
#include "timeapp.h"
   
#include <ti/drivers/lcd/LCDDogm1286.h>

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

// Fast advertising interval in 625us units
#define DEFAULT_FAST_ADV_INTERVAL             32

// Duration of fast advertising duration in ms
#define DEFAULT_FAST_ADV_DURATION             30000

// Slow advertising interval in 625us units
#define DEFAULT_SLOW_ADV_INTERVAL             1600

// Duration of slow advertising duration in ms (set to 0 for continuous advertising)
#define DEFAULT_SLOW_ADV_DURATION             0

// Whether to enable automatic parameter update request when a connection is formed
#define DEFAULT_ENABLE_UPDATE_REQUEST         FALSE

// Minimum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     200

// Maximum connection interval (units of 1.25ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     1600

// Slave latency to use if automatic parameter update request is enabled
#define DEFAULT_DESIRED_SLAVE_LATENCY         1

// Supervision timeout value (units of 10ms) if automatic parameter update request is enabled
#define DEFAULT_DESIRED_CONN_TIMEOUT          1000

// Default passcode
#define DEFAULT_PASSCODE                      19655

// Default GAP pairing mode
#define DEFAULT_PAIRING_MODE                  GAPBOND_PAIRING_MODE_INITIATE

// Default MITM mode (TRUE to require passcode or OOB when pairing)
#define DEFAULT_MITM_MODE                     FALSE

// Default bonding mode, TRUE to bond
#define DEFAULT_BONDING_MODE                  TRUE

// Default GAP bonding I/O capabilities
#define DEFAULT_IO_CAPABILITIES               GAPBOND_IO_CAP_DISPLAY_ONLY

// Delay to begin discovery from start of connection in ms
#define DEFAULT_DISCOVERY_DELAY               1000

// Clock display update period in ms; set to 60 sec or less since time is displayed in minutes
#define DEFAULT_CLOCK_UPDATE_PERIOD           10000

// Reference Time Update Service values for time update control point
#define REF_TIME_UPDATE_GET                   0x01    // Get Reference Update
#define REF_TIME_UPDATE_CANCEL                0x02    // Cancel Reference Update

// Ringer control point
#define RINGER_SILENT_MODE                    1       // Silent Mode
#define RINGER_MUTE_ONCE                      2       // Mute Once
#define RINGER_CANCEL_SILENT                  3       // Cancel Silent Mode 

// Internal events for RTOS application.
#define TIMEAPP_STATE_CHANGE_EVT              0x0001
#define TIMEAPP_PASSCODE_EVT                  0x0002
#define TIMEAPP_PAIRING_STATE_EVT             0x0004
#define TIMEAPP_KEY_CHANGE_EVT                0x0008
#define TIMEAPP_CLOCK_UPDATE_EVT              0x0010
#define TIMEAPP_START_DISC_EVT                0x0020

// Task configuration
#define TIMEAPP_TASK_PRIORITY                 1
#define TIMEAPP_TASK_STACK_SIZE               612

// Application states
enum
{
  BLE_STATE_IDLE,
  BLE_STATE_CONNECTED,
  BLE_STATE_DISCONNECTING
};

/*********************************************************************
 * TYPEDEFS
 */

// App event passed from profiles.
typedef struct 
{
  appEvtHdr_t hdr;             // Event header.
  uint8_t *pData;              // Event data.
} timeAppEvt_t;

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
static Clock_Struct updateDisplayClock;

// Handle for the Message Queue.
static Queue_Struct appMsg;
static Queue_Handle appMsgQueue;

// GAP State.
static gaprole_States_t timeAppGapState = GAPROLE_INIT;

// Connecting device address.
static uint8_t TimeApp_connDevAddr[B_ADDR_LEN];

// Task pending events.
static uint16_t events = 0;

// Task configuration.
Task_Struct TimeApp_task;
Char TimeApp_taskStack[TIMEAPP_TASK_STACK_SIZE];

// Service discovery state.
static uint8_t TimeApp_discState = DISC_IDLE;

// Service discovery complete.
static uint8_t TimeApp_discoveryCmpl = FALSE;

// Characteristic configuration state.
static uint8_t TimeApp_configState = TIME_CONFIG_START;

// TRUE if discovery postponed due to pairing.
static uint8_t TimeApp_discPostponed = FALSE;

// GAP Profile - Name attribute for SCAN RSP data.
static uint8_t TimeApp_scanData[] =
{
  // local name
  0x09,
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,
  'T',
  'i',
  'm',
  'e',
  ' ',
  'A',
  'p',
  'p'
};

static uint8_t TimeApp_advData[] = 
{
  // flags
  0x02,
  GAP_ADTYPE_FLAGS,
  GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
  // service solicitation
  0x0D,
  GAP_ADTYPE_SERVICES_LIST_16BIT,
  LO_UINT16(CURRENT_TIME_SERV_UUID),
  HI_UINT16(CURRENT_TIME_SERV_UUID),
  LO_UINT16(REF_TIME_UPDATE_SERV_UUID),
  HI_UINT16(REF_TIME_UPDATE_SERV_UUID),
  LO_UINT16(NEXT_DST_CHANGE_SERV_UUID),
  HI_UINT16(NEXT_DST_CHANGE_SERV_UUID),
  LO_UINT16(NWA_SERV_UUID),
  HI_UINT16(NWA_SERV_UUID),
  LO_UINT16(ALERT_NOTIF_SERV_UUID),
  HI_UINT16(ALERT_NOTIF_SERV_UUID),
  LO_UINT16(BATT_SERV_UUID),
  HI_UINT16(BATT_SERV_UUID)
};

// Device name attribute value.
static uint8_t TimeApp_deviceName[GAP_DEVICE_NAME_LEN] = "Time App";

// Bonded peer address.
static uint8_t TimeApp_bondedAddr[B_ADDR_LEN];

// Reference time update value.
static uint8_t TimeApp_refUpdateVal = REF_TIME_UPDATE_GET;

// Alert notification control point command test values.
static const uint8_t TimeApp_alertCmd[] =
{
  ALERT_NOTIF_ENABLE_NEW,
  ALERT_NOTIF_ENABLE_UNREAD,
  ALERT_NOTIF_NOTIFY_NEW,
  ALERT_NOTIF_NOTIFY_UNREAD,
  ALERT_NOTIF_DISABLE_NEW,
  ALERT_NOTIF_DISABLE_UNREAD
};

// Pointer to above command test values.
static uint8_t const *pTimeAppAlertCmd = TimeApp_alertCmd;

// Advertising user-cancelled state.
static bool TimeApp_advCancelled = FALSE;

// Ringer control point test value.
static uint8_t TimeApp_ringerCmd = RINGER_SILENT_MODE;

// LCD parameters
LCD_Handle lcdHandle;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

// Application tasks and event processing.
static void TimeApp_init(void);
static void TimeApp_taskFxn(UArg a0, UArg a1);
static uint8_t TimeApp_enqueueMsg(uint8_t event, uint8_t state, 
                                  uint8_t *pData);
static void TimeApp_processStackMsg(ICall_Hdr *pMsg);
static void TimeApp_processAppMsg(timeAppEvt_t *pMsg);
static void TimeApp_processGattMsg(gattMsgEvent_t *pMsg);
static void TimeApp_disconnected(void);
static void TimeApp_writeCtrl(uint16_t handle, uint8_t value);

// Peripheral role.
static void TimeApp_stateChangeCB(gaprole_States_t newState);
static void TimeApp_stateChangeEvt(gaprole_States_t newState);

// Passcode.
static void TimeApp_passcodeCB(uint8_t *deviceAddr, uint16_t connectionHandle,
                                     uint8_t uiInputs, uint8_t uiOutputs);
static void TimeApp_passcodeEvt(uint16_t connectionHandle, uint8_t uiOutputs);
// Pair state.
static void TimeApp_pairStateCB(uint16_t connHandle, uint8_t state, 
                                      uint8_t status);
static void TimeApp_pairStateEvt(uint16_t connHandle, uint8_t state, 
                                 uint8_t status);

// Clock handler.
static void TimeApp_clockHandler(UArg arg);

// Service discovery
void TimeApp_startDiscEvt(void);

// Keys.
void TimeApp_keyPressHandler(uint8 keys);
static void TimeApp_handleKeys(uint8_t shift, uint8_t keys);

/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static gapRolesCBs_t TimeApp_peripheralCB =
{
  TimeApp_stateChangeCB           // Profile State Change Callbacks
};

// Bond Manager Callbacks
static const gapBondCBs_t TimeApp_bondCB =
{
  TimeApp_passcodeCB,             //  Passcode callback
  TimeApp_pairStateCB             //  Pairing state callback
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      TimeApp_createTask
 *
 * @brief   Task creation function for the Time Application.
 *
 * @param   none
 *
 * @return  none
 */
void TimeApp_createTask(void)
{
  Task_Params taskParams;
    
  // Configure task.
  Task_Params_init(&taskParams);
  taskParams.stack = TimeApp_taskStack;
  taskParams.stackSize = TIMEAPP_TASK_STACK_SIZE;
  taskParams.priority = TIMEAPP_TASK_PRIORITY;
  
  Task_construct(&TimeApp_task, TimeApp_taskFxn, &taskParams, NULL);
}

/*********************************************************************
 * @fn      TimeApp_init
 *
 * @brief   Initialization function for the Time Application Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notification ...).
 *
 * @param   None.
 *
 * @return  None.
 */
void TimeApp_init(void)
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
  Util_constructClock(&startDiscoveryClock, TimeApp_clockHandler, 
                      DEFAULT_DISCOVERY_DELAY, 0, false,
                      TIMEAPP_START_DISC_EVT);
  Util_constructClock(&updateDisplayClock, TimeApp_clockHandler, 
                      DEFAULT_CLOCK_UPDATE_PERIOD, 0, false,
                      TIMEAPP_CLOCK_UPDATE_EVT);
  
  Board_initKeys(TimeApp_keyPressHandler);

  Board_openLCD();
  
  // Setup the GAP Peripheral Role Profile.
  {
    uint8_t advEnable = FALSE;      
    uint16_t advertOffTime = 0;
    uint8_t updateRequest = DEFAULT_ENABLE_UPDATE_REQUEST;
    uint16_t minInterval = DEFAULT_DESIRED_MIN_CONN_INTERVAL;
    uint16_t maxInterval = DEFAULT_DESIRED_MAX_CONN_INTERVAL;
    uint16_t slaveLatency = DEFAULT_DESIRED_SLAVE_LATENCY;
    uint16_t connTimeout = DEFAULT_DESIRED_CONN_TIMEOUT;

    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &advEnable);
    GAPRole_SetParameter(GAPROLE_ADVERT_OFF_TIME, sizeof(uint16_t),
                         &advertOffTime);
    
    GAPRole_SetParameter(GAPROLE_PARAM_UPDATE_ENABLE, sizeof(uint8_t), 
                         &updateRequest);
    GAPRole_SetParameter(GAPROLE_MIN_CONN_INTERVAL, sizeof(uint16_t),
                         &minInterval);
    GAPRole_SetParameter(GAPROLE_MAX_CONN_INTERVAL, sizeof(uint16_t),
                         &maxInterval);
    GAPRole_SetParameter(GAPROLE_SLAVE_LATENCY, sizeof(uint16_t), 
                         &slaveLatency);
    GAPRole_SetParameter(GAPROLE_TIMEOUT_MULTIPLIER, sizeof(uint16_t), 
                         &connTimeout);
    
    GAPRole_SetParameter(GAPROLE_SCAN_RSP_DATA, sizeof (TimeApp_scanData), 
                         TimeApp_scanData);
    GAPRole_SetParameter(GAPROLE_ADVERT_DATA, sizeof(TimeApp_advData), 
                         TimeApp_advData);
  }
  
  // Setup GAP.
  GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, TimeApp_deviceName);

  // Setup the GAP Bond Manager.
  {
    uint32_t passkey = DEFAULT_PASSCODE;
    uint8_t pairMode = DEFAULT_PAIRING_MODE;
    uint8_t mitm = DEFAULT_MITM_MODE;
    uint8_t ioCap = DEFAULT_IO_CAPABILITIES;
    uint8_t bonding = DEFAULT_BONDING_MODE;
    
    GAPBondMgr_SetParameter(GAPBOND_DEFAULT_PASSCODE, sizeof(uint32_t), &passkey);
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
  
  // Start the Device.
  VOID GAPRole_StartDevice(&TimeApp_peripheralCB);

  // Register with bond manager after starting device.
  GAPBondMgr_Register((gapBondCBs_t *) &TimeApp_bondCB);
}

/*********************************************************************
 * @fn      TimeApp_taskFxn
 *
 * @brief   Time Application Task entry point.  This function
 *          is called to initialize and then process all events for the task.  
 *          Events include timers, messages and any other user defined events.
 *
 * @param   a0, a1 - not used.
 *
 * @return  none.
 */
void TimeApp_taskFxn(UArg a0, UArg a1)
{
  TimeApp_init();
  
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
          TimeApp_processStackMsg((ICall_Hdr *)pMsg);
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
      timeAppEvt_t *pMsg = 
        (timeAppEvt_t*)Util_dequeueMsg(appMsgQueue);
      if (pMsg)
      {
        // Process message.
        TimeApp_processAppMsg(pMsg);
        
        // Free the space from the message.
        ICall_free(pMsg);
      }
    }

    // Service discovery event.
    if (events & TIMEAPP_START_DISC_EVT)
    {
      events &= ~TIMEAPP_START_DISC_EVT;
      
      TimeApp_startDiscEvt();
    }
    
    // Update clock display event.
    if (events & TIMEAPP_CLOCK_UPDATE_EVT)
    {
      events &= ~TIMEAPP_CLOCK_UPDATE_EVT;
    
      // Update display.
      Time_clockDisplay();
    
      // Restart timer.
      Util_startClock(&updateDisplayClock);
    }
  }
}

/*********************************************************************
 * @fn      TimeApp_processStackMsg
 *
 * @brief   Process an incoming task message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void TimeApp_processStackMsg(ICall_Hdr *pMsg)
{
  switch (pMsg->event)
  {
    case GATT_MSG_EVENT:
      TimeApp_processGattMsg((gattMsgEvent_t *) pMsg);
      break;
      
    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      TimeApp_processAppMsg
 *
 * @brief   Process an incoming stack message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void TimeApp_processAppMsg(timeAppEvt_t *pMsg)
{
  switch (pMsg->hdr.event)
  {
    case TIMEAPP_STATE_CHANGE_EVT:
      TimeApp_stateChangeEvt((gaprole_States_t)pMsg->hdr.state);
      break;
      
    case TIMEAPP_KEY_CHANGE_EVT:
      TimeApp_handleKeys(0, pMsg->hdr.state);
      break;
     
    // Pairing state event.
    case TIMEAPP_PAIRING_STATE_EVT:
    {      
      TimeApp_pairStateEvt(Time_connHandle, pMsg->hdr.state, *pMsg->pData);
      
      ICall_free(pMsg->pData);
      break;
    }
    
    // Passcode event.
    case TIMEAPP_PASSCODE_EVT:
    {  
      TimeApp_passcodeEvt(Time_connHandle, *pMsg->pData);
      
      ICall_free(pMsg->pData);
      break;
    }  
      
    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      TimeApp_keyPressHandler
 *
 * @brief   Key event handler function
 *
 * @param   a0 - ignored
 *
 * @return  none
 */
void TimeApp_keyPressHandler(uint8 keys)
{
  // Enqueue the event.
  TimeApp_enqueueMsg(TIMEAPP_KEY_CHANGE_EVT, keys, NULL);
}

/*********************************************************************
 * @fn      TimeApp_handleKeys
 *
 * @brief   Handles all key events for this device.
 *
 * @param   shift - true if in shift/alt.
 * @param   keys - bit field for key events.
 *
 * @return  none
 */
static void TimeApp_handleKeys(uint8_t shift, uint8_t keys)
{
  if (keys & KEY_UP)
  {
    // Start or stop advertising
    if (timeAppGapState != GAPROLE_CONNECTED)
    {
      uint8_t advState;
      
      // Set fast advertising interval for user-initiated connections
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, DEFAULT_FAST_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, DEFAULT_FAST_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_MIN, DEFAULT_FAST_ADV_DURATION);

      // Toggle advertising state      
      GAPRole_GetParameter(GAPROLE_ADVERT_ENABLED, &advState);
      advState = !advState;
      GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &advState);
      
      // Set state variable
      if (advState == FALSE)
      {
        TimeApp_advCancelled = TRUE;
      }
    }
  }

  if (keys & KEY_LEFT)
  {
    // Send command to alert notification control point
    if ((timeAppGapState == GAPROLE_CONNECTED) &&
         (Time_handleCache[HDL_ALERT_NTF_CTRL] != 0))
    {     
      attWriteReq_t req;
      
      req.pValue = GATT_bm_alloc(Time_connHandle, ATT_WRITE_REQ, 2, NULL);
      if (req.pValue != NULL)
      {
        req.len = 2;
        req.pValue[0] = *pTimeAppAlertCmd;
        req.pValue[1] = ALERT_NOTIF_CAT_ALL;
        req.sig = 0;
        req.cmd = 0;
        
        req.handle = Time_handleCache[HDL_ALERT_NTF_CTRL];
        
        // Send write request
        if (GATT_WriteCharValue(Time_connHandle, &req, 
                                 ICall_getEntityId()) != SUCCESS)
        {
          GATT_bm_free((gattMsg_t *)&req, ATT_WRITE_REQ);
        }
        
        LCD_WRITE_STRING_VALUE("Alert cmd:", *pTimeAppAlertCmd, 10, LCD_PAGE0);
      }
    }
    
    // Cycle through command test values
    if (*pTimeAppAlertCmd == ALERT_NOTIF_DISABLE_UNREAD)
    {
      pTimeAppAlertCmd = TimeApp_alertCmd;
    }
    else
    {
      pTimeAppAlertCmd++;
    }
  }

  if (keys & KEY_RIGHT)
  {
    // Do a reference time update
    if ((timeAppGapState == GAPROLE_CONNECTED) &&
         (Time_handleCache[HDL_REF_TIME_UPD_CTRL] != 0))
    {
      TimeApp_writeCtrl(Time_handleCache[HDL_REF_TIME_UPD_CTRL], 
                        TimeApp_refUpdateVal);

      LCD_WRITE_STRING_VALUE("Time update:", TimeApp_refUpdateVal, 10, LCD_PAGE0);
      
      // Toggle between two reference time update values
      if (TimeApp_refUpdateVal == REF_TIME_UPDATE_GET)
      {
        TimeApp_refUpdateVal = REF_TIME_UPDATE_CANCEL;
      }
      else
      {
        TimeApp_refUpdateVal = REF_TIME_UPDATE_GET;
      }
    }
  }
  
  if (keys & KEY_SELECT)
  {
    // If connected, terminate connection
    if (timeAppGapState == GAPROLE_CONNECTED)
    {
      GAPRole_TerminateConnection();
    }
    else
    {
      // Clear all stored bonds.
      GAPBondMgr_SetParameter(GAPBOND_ERASE_ALLBONDS, 0, 0);
    }
  }
  
  if (keys & KEY_DOWN)
  {
    // Write ringer control point
    if ((timeAppGapState == GAPROLE_CONNECTED) &&
         (Time_handleCache[HDL_PAS_CTRL] != 0))
    {
      TimeApp_writeCtrl(Time_handleCache[HDL_PAS_CTRL], TimeApp_ringerCmd);

      LCD_WRITE_STRING_VALUE("Ringer ctrl:", TimeApp_ringerCmd, 10, LCD_PAGE0);
      
      // Toggle between values
      if (++TimeApp_ringerCmd > RINGER_CANCEL_SILENT)
      {
        TimeApp_ringerCmd = RINGER_SILENT_MODE;
      }
    }
  }
}

/*********************************************************************
 * @fn      TimeApp_writeCtrl
 *
 * @brief   Write a control point.
 *
 * @param   handle - attribute handle
 * @param   value - attribute value
 *
 * @return  none
 */
static void TimeApp_writeCtrl(uint16_t handle, uint8_t value)
{
  attWriteReq_t req;
   
  req.pValue = GATT_bm_alloc(Time_connHandle, ATT_WRITE_REQ, 1, NULL);
  if (req.pValue != NULL)
  { 
    req.len = 1;
    req.pValue[0] = value;
    req.sig = 0;
    req.cmd = 1;
    
    req.handle = handle;
    
    // Send write command
    if (GATT_WriteNoRsp(Time_connHandle, &req) != SUCCESS)
    {
      GATT_bm_free( (gattMsg_t *)&req, ATT_WRITE_REQ );
    }
  }
}

/*********************************************************************
 * @fn      TimeApp_stateChangeCB
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void TimeApp_stateChangeCB(gaprole_States_t newState)
{
  TimeApp_enqueueMsg(TIMEAPP_STATE_CHANGE_EVT, newState, NULL);
}

/*********************************************************************
 * @fn      TimeApp_stateChangeEvt
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void TimeApp_stateChangeEvt(gaprole_States_t newState)
{
  // if connected
  if (newState == GAPROLE_CONNECTED)
  {
    // Get connection handle.
    GAPRole_GetParameter(GAPROLE_CONNHANDLE, &Time_connHandle);

    // Get peer bd address.
    GAPRole_GetParameter(GAPROLE_CONN_BD_ADDR, &TimeApp_connDevAddr);
    
    if (TimeApp_connDevAddr != NULL)
    {
      // If connected to device without bond do service discovery
      if (memcmp(TimeApp_connDevAddr, TimeApp_bondedAddr, B_ADDR_LEN))
      {
        TimeApp_discoveryCmpl = FALSE;
      }
      
      // Initiate service discovery if necessary
      if (TimeApp_discoveryCmpl == FALSE)
      {
        // Start timer to commence service discovery.
        Util_startClock(&startDiscoveryClock);
      }
      // Perform configuration of characteristics on connection setup
      else
      {
        TimeApp_configState = Time_configNext(TIMEAPP_CONFIG_CONN_START);
      } 
    }
      
    LCD_WRITE_STRING("Connected", LCD_PAGE0);
    LCD_WRITE_STRING("", LCD_PAGE1);
  }
  // if disconnected
  else if (timeAppGapState == GAPROLE_CONNECTED && 
            newState != GAPROLE_CONNECTED)
  {
    uint8_t advState = TRUE;
      
    TimeApp_disconnected();
    
    if (newState == GAPROLE_WAITING_AFTER_TIMEOUT)
    {
      // link loss timeout-- use fast advertising
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, DEFAULT_FAST_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, DEFAULT_FAST_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_MIN, DEFAULT_FAST_ADV_DURATION);
    }
    else
    {
      // Else use slow advertising
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, DEFAULT_SLOW_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, DEFAULT_SLOW_ADV_INTERVAL);
      GAP_SetParamValue(TGAP_GEN_DISC_ADV_MIN, DEFAULT_SLOW_ADV_DURATION);
    }

    // Enable advertising
    GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &advState);   
    
    LCD_WRITE_STRING("Disconnected", LCD_PAGE0);
    LCD_WRITE_STRING("", LCD_PAGE1);
  }
  // if advertising stopped
  else if (timeAppGapState == GAPROLE_ADVERTISING && 
            newState == GAPROLE_WAITING)
  {
    // if advertising stopped by user
    if (TimeApp_advCancelled)
    {
      TimeApp_advCancelled = FALSE;
    }
    // if fast advertising switch to slow
    else if (GAP_GetParamValue(TGAP_GEN_DISC_ADV_INT_MIN) == DEFAULT_FAST_ADV_INTERVAL)
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
    uint8_t bdAddr[B_ADDR_LEN];
    
    GAPRole_GetParameter(GAPROLE_BD_ADDR, &bdAddr);
    LCD_WRITE_STRING("Time App", LCD_PAGE0);
    LCD_WRITE_STRING(Util_convertBdAddr2Str(bdAddr),  LCD_PAGE1);
    
    // Initialize time clock after writing first two lines of LCD.
    Time_clockInit();
    
    // Display time.
    Time_clockDisplay();
    
    // Start clock for UTC time display update.
    Util_startClock(&updateDisplayClock);
  }
  
  timeAppGapState = newState;
}

/*********************************************************************
 * @fn      TimeApp_processGattMsg
 *
 * @brief   Process GATT messages
 *
 * @return  none
 */
static void TimeApp_processGattMsg(gattMsgEvent_t *pMsg)
{
  if (pMsg->method == ATT_HANDLE_VALUE_NOTI ||
       pMsg->method == ATT_HANDLE_VALUE_IND)
  {
    Time_indGattMsg(pMsg);
  }
  else if ((pMsg->method == ATT_READ_RSP || pMsg->method == ATT_WRITE_RSP) ||
            (pMsg->method == ATT_ERROR_RSP &&
              (pMsg->msg.errorRsp.reqOpcode == ATT_READ_REQ ||
                pMsg->msg.errorRsp.reqOpcode == ATT_WRITE_REQ)))
  {
    TimeApp_configState = Time_configGattMsg(TimeApp_configState, pMsg);
    if (TimeApp_configState == TIME_CONFIG_CMPL)
    {
      TimeApp_discoveryCmpl = TRUE;
    }
  }
  else
  {
    TimeApp_discState = Time_discGattMsg(TimeApp_discState, pMsg);
    if (TimeApp_discState == DISC_IDLE)
    {      
      // Start characteristic configuration
      TimeApp_configState = Time_configNext(TIME_CONFIG_START);
    }
  }
  
  GATT_bm_free(&pMsg->msg, pMsg->method);
}

/*********************************************************************
 * @fn      TimeApp_disconnected
 *
 * @brief   Handle disconnect. 
 *
 * @return  none
 */
static void TimeApp_disconnected(void)
{
  // Stop discovery clock in case it is running.
  Util_stopClock(&startDiscoveryClock);
  
  // Initialize state variables
  TimeApp_discState = DISC_IDLE;
  TimeApp_discPostponed = FALSE;
  
  // Invalidate connection variables.
  Time_connHandle = INVALID_CONNHANDLE;
  memset(TimeApp_connDevAddr, 0xFF, B_ADDR_LEN);
}

/*********************************************************************
 * @fn      TimeApp_pairStateCB
 *
 * @brief   Pairing state callback.
 *
 * @return  none
 */
static void TimeApp_pairStateCB(uint16_t connHandle, uint8_t state, 
                                      uint8_t status)
{
  uint8_t *pData;
  
  // Allocate the data.
  if ((pData = ICall_malloc(sizeof(uint8_t))))
  {
    *pData = status;
    
    // Queue the event.
    TimeApp_enqueueMsg(TIMEAPP_PAIRING_STATE_EVT, state, pData);
  }
}

/*********************************************************************
 * @fn      TimeApp_pairStateEvt
 *
 * @brief   Pairing state event processor.
 *
 * @return  none
 */
static void TimeApp_pairStateEvt(uint16_t connHandle, uint8_t state, uint8_t status)
{
  if (state == GAPBOND_PAIRING_STATE_STARTED)
  {
    // Stop countdown to service discovery.  
    Util_stopClock(&startDiscoveryClock);
    
    // Set service discovery to occur after pairing or bonding completes.
    TimeApp_discPostponed = TRUE;
    
    LCD_WRITE_STRING("Pairing started", LCD_PAGE0);
  }
  else if (state == GAPBOND_PAIRING_STATE_COMPLETE)
  {
    if (status == SUCCESS)
    {
      // If really still connected      
      if (Time_connHandle != INVALID_CONNHANDLE)
      {
        memcpy(TimeApp_bondedAddr, TimeApp_connDevAddr, B_ADDR_LEN);
      }
      
      // If discovery was postponed start discovery
      if (TimeApp_discPostponed && TimeApp_discoveryCmpl == FALSE)
      {
        TimeApp_discPostponed = FALSE;
        
        // Set an event for service discovery to begin.
        events |= TIMEAPP_START_DISC_EVT;
        
        // Post the application's semaphore.
        Semaphore_post(sem);
      }
      
      LCD_WRITE_STRING("Pairing success", LCD_PAGE0);
    }
    else
    {
      LCD_WRITE_STRING_VALUE("Pairing fail", status, 10, LCD_PAGE0);
    }
  }
  else if (state == GAPBOND_PAIRING_STATE_BONDED)
  {
    if (status == SUCCESS)
    {
      LCD_WRITE_STRING("Bonding success", LCD_PAGE0);
    }
  }
}

/*********************************************************************
 * @fn      TimeApp_passcodeCB
 *
 * @brief   Passcode callback.
 *
 * @return  none
 */
static void TimeApp_passcodeCB(uint8_t *deviceAddr, uint16_t connectionHandle,
                                     uint8_t uiInputs, uint8_t uiOutputs)
{
  uint8_t *pData;
  
  // Allocate space for the passcode.
  if ((pData = ICall_malloc(sizeof(uint8_t))))
  {
    *pData = uiOutputs;
    
    // Queue the event.
    TimeApp_enqueueMsg(TIMEAPP_PASSCODE_EVT, 0, pData);
  }
}

/*********************************************************************
 * @fn      TimeApp_passcodeEvt
 *
 * @brief   Passcode callback.
 *
 * @return  none
 */
static void TimeApp_passcodeEvt(uint16_t connectionHandle, uint8_t uiOutputs)
{
#ifdef TI_DRIVERS_LCD_INCLUDED

  uint32_t  passcode;

  // Create random passcode
  passcode = Util_GetTRNG();
  passcode %= 1000000;
  
  // Display passcode to user
  if (uiOutputs != 0)
  {
    LCD_WRITE_STRING_VALUE("Passcode:", passcode, 10,  LCD_PAGE0);
  }
  
  // Send passcode response
  GAPBondMgr_PasscodeRsp(connectionHandle, SUCCESS, passcode);
#endif
}

/*********************************************************************
 * @fn      TimeApp_startDiscEvt
 *
 * @brief   Event to handle the start of service discovery.
 *
 * @return  none
 */
void TimeApp_startDiscEvt(void)
{
  TimeApp_discState = Time_discStart();
}

/*********************************************************************
 * @fn      TimeApp_clockHandler
 *
 * @brief   Clock handle for all clock events.  This function stores an event 
 *          flag and wakes up the application's event processor.
 *
 * @param   arg - event flag.
 *
 * @return  none
 */
static void TimeApp_clockHandler(UArg arg)
{
  // Store the event.
  events |= arg;
  
  // Wake up the application.
  Semaphore_post(sem);
}

/*********************************************************************
 * @fn      TimeApp_enqueueMsg
 *
 * @brief   Creates a message and puts the message in RTOS queue.
 *
 * @param   event - message event.
 * @param   state - message state.
 * @param   pData - message data pointer.
 *
 * @return  TRUE or FALSE
 */
static uint8_t TimeApp_enqueueMsg(uint8_t event, uint8_t state,
                                  uint8_t *pData)
{
  timeAppEvt_t *pMsg;
  
  // Create dynamic pointer to message.
  if (pMsg = ICall_malloc(sizeof(timeAppEvt_t)))
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
