/*******************************************************************************
  Filename:       proximityTag.c
  Revised:        $Date: 2015-07-20 11:31:07 -0700 (Mon, 20 Jul 2015) $
  Revision:       $Revision: 44370 $

  Description:    This file contains the Proximity Tag sample application
                  for use with the CC2650 Bluetooth Low Energy Protocol Stack.

  Copyright 2010 - 2015 Texas Instruments Incorporated. All rights reserved.

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
#include "gapgattserver.h"
#include "gattservapp.h"
#include "gatt_profile_uuid.h"
#include "gatt_uuid.h"
#include "devinfoservice.h"
#include "proxreporter.h"

#include "peripheral.h"
#include "gapbondmgr.h"

#include "osal_snv.h"
#include "ICallBleAPIMSG.h"

#include "util.h"
#include "board_key.h"
#include "Board.h"
#include "buzzer.h"

#include "proximityTag.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

// Advertising interval when device is discoverable (units of 625us, 160=100ms).
#define DEFAULT_ADVERTISING_INTERVAL          160

// Whether to enable automatic parameter update request when a connection is 
// formed.
#define DEFAULT_ENABLE_UPDATE_REQUEST         TRUE

// Limited discoverable mode advertises for 30.72s, and then stops
#define DEFAULT_DISCOVERABLE_MODE             GAP_ADTYPE_FLAGS_LIMITED

// Minimum connection interval (units of 1.25ms, 40=50ms) if automatic 
// parameter update request is enabled.
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL     40

// Maximum connection interval (units of 1.25ms, 40=50ms) if automatic 
// parameter update request is enabled.
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     40

// Slave latency to use if automatic parameter update request is enabled.
#define DEFAULT_DESIRED_SLAVE_LATENCY         0

// Supervision timeout value (units of 10ms, 1000=10s) if automatic parameter 
// update request is enabled.
#define DEFAULT_DESIRED_CONN_TIMEOUT          1000

// Company Identifier: Texas Instruments Inc. (13)
#define TI_COMPANY_ID                         0x000D

// Number of beeps before buzzer stops by itself.
#define BUZZER_MAX_BEEPS                      10

// Buzzer beep tone frequency for "High Alert" (in Hz)
#define BUZZER_ALERT_HIGH_FREQ                4096

// Buzzer beep tone frequency for "Low Alert" (in Hz)
#define BUZZER_ALERT_LOW_FREQ                 250

// Delay for reset of device's bonds, connections, alerts.
#define PT_START_RESET_DELAY                  3000 // in ms, 3 seconds

// Delay between steps after reset event has begun.
#define PT_IN_RESET_DELAY                     500  // in ms

// proximityState values:
// Advertising after initialization or due to terminated link.
#define PROXSTATE_INITIALIZED                 0

// Connected and "within range" of the master, as defined by proximity profile.
#define PROXSTATE_CONNECTED_IN_RANGE          1   

// Connected and "out of range" of the master or Find Me Locator is calling.
#define PROXSTATE_IMM_ALERT                   2   

// Disconnected as a result of a supervision timeout
#define PROXSTATE_LINK_LOSS                   3   

// buzzer_state values.
#define BUZZER_OFF                            0
#define BUZZER_ON                             1

// max number of devices to be remembered.
#define MAX_KNOWN_ADDRS                       GAP_BONDINGS_MAX

// Gap Bond Manager States.
#define UNPAIRED_STATE                        0x00
#define NEW_DISCOVERY_STATE                   0x01
#define PAIRED_BONDED_STATE                   0x02

// Service Change flags.
#define NO_CHANGE                             0x00
#define CHANGE_OCCURED                        0x01

// Alert Levels.
#define NO_ALERT                              0x00
#define LO_ALERT                              0x01
#define HI_ALERT                              0x02

// Services to Discover.
#define GSC                                   0x01
#define IAS                                   0x02
#define ALL_SERVICES_DISCOVERED               0x03

// Task configuration.
#define PT_TASK_PRIORITY                      1
#define PT_TASK_STACK_SIZE                    644

// Proximity Tag Task Events.
#define PT_STATE_CHANGE_EVT                   0x0001
#define PT_KEY_CHANGE_EVT                     0x0002
#define PT_SERVICE_EVT                        0x0004
#define PT_ALERT_EVT                          0x0008
#define PT_ADV_IN_CONNECTION_EVT              0x0010
#define PT_SEND_PARAM_UPDATE_EVT              0x0020
#define PT_RESET_EVT                          0x0040
#define PT_PAIR_STATE_EVT                     0x0080
#define PT_SERVICE_DISC_EVT                   0x0100

/*********************************************************************
 * TYPEDEFS
 */

// App event passed from profiles.
typedef struct
{
  appEvtHdr_t hdr; // Event header.
  uint8_t *pData;  // Event data
  
} proximityEvt_t;

/*
 * Storage of known addresses with handle to IAS characteristic value
 */
typedef struct
{
  uint8_t addr[B_ADDR_LEN];   //!< Address of a known device
  uint16_t alertHandle;       //!< Handle of the IAS characteristic value
  uint16_t svcChangeHandle;   //!< Handle of the Service Change characteristic value
} AddrIASCharHandle_t;

typedef struct
{
  uint16_t handle;                                 //!< Handle of the sender
  uint8_t handleRange[ATT_BT_UUID_SIZE * 2];       //!< Handle range
}HandleChangeInd_t;


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
static Clock_Struct alertClock;
static Clock_Struct resetClock;

// Queue object used for app messages.
static Queue_Struct appMsg;
static Queue_Handle appMsgQueue;

// events flag for internal application events.
static uint16_t events;

// Task configuration.
Task_Struct ProximityTag_task;
Char ProximityTag_taskStack[PT_TASK_STACK_SIZE];

static bool proxTagUsingWhiteList = FALSE;

static gaprole_States_t gapProfileState = GAPROLE_INIT;
static uint8_t gapBondMgrState = UNPAIRED_STATE;

static uint16_t connHandle = GAP_CONNHANDLE_INIT;

// Used to find Characteristics of Services.
static uint16_t charUuidToFind = 0;
// Used to find Services.
static uint8_t serviceToFind = 0;

// Proximity and Find Me State Variables.
static uint8_t proxLLAlertLevel = PP_ALERT_LEVEL_NO;     // Link Loss Alert
static uint8_t proxIMAlertLevel = PP_ALERT_LEVEL_NO;     // Immediate Alert
static int8  proxTxPwrLevel = 0;  // Tx Power Level (0dBm default)

// proximityState is the current state of the device.
static uint8_t proximityState;

// Find Me Profile State Variables, for central device.
static uint8_t immAlertLevel = NO_ALERT;
static uint8_t serviceChange = NO_CHANGE;
static uint16_t immAlertCharHandle = GATT_INVALID_HANDLE;  // handle of IAS characteristic.
static uint16_t serviceChangeHandle = GATT_INVALID_HANDLE;  // handle of Service Change characteristic.

// Used for handle change indications
static HandleChangeInd_t handleChange;  

// Container of previously connected devices
static AddrIASCharHandle_t knownAddrs[MAX_KNOWN_ADDRS];

// Always points to the next available index.
static uint8_t addrIdx = 0;  

// Buzzer state.
static uint8_t buzzer_state = BUZZER_OFF;
static uint8_t buzzer_beep_count = 0;

// Used to determine if a reset delay is in progress.
static uint8_t resetInProgress = FALSE;

// GAP - SCAN RSP data (max size = 31 bytes).
static uint8_t scanRspData[] =
{
  // complete name
  0x0D,   // length of this data
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,
  0x50,   // 'P'
  0x72,   // 'r'
  0x6F,   // 'o'
  0x78,   // 'x'
  0x69,   // 'i'
  0x6D,   // 'm'
  0x69,   // 'i'
  0x74,   // 't'
  0x79,   // 'y'
  0x54,   // 'T'
  0x61,   // 'a'
  0x67,   // 'g'

  // connection interval range
  0x05,   // length of this data
  GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE,
  LO_UINT16(DEFAULT_DESIRED_MIN_CONN_INTERVAL),   // 100ms
  HI_UINT16(DEFAULT_DESIRED_MIN_CONN_INTERVAL),
  LO_UINT16(DEFAULT_DESIRED_MAX_CONN_INTERVAL),   // 1s
  HI_UINT16(DEFAULT_DESIRED_MAX_CONN_INTERVAL),

  // Tx power level
  0x02,   // length of this data
  GAP_ADTYPE_POWER_LEVEL,
  0       // 0dBm
};

// GAP - Advertisement data (max size = 31 bytes, though this is
// best kept short to conserve power while advertisting).
static uint8_t advertData[] =
{
  // Flags; this sets the device to use limited discoverable
  // mode (advertises for 30 seconds at a time) instead of general
  // discoverable mode (advertises indefinitely)
  0x02,   // length of this data
  GAP_ADTYPE_FLAGS,
  DEFAULT_DISCOVERABLE_MODE | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,

  // Service solicitation, the peripheral device wants IAS for the Find Me Profile
  0x03,
  GAP_ADTYPE_SERVICES_LIST_16BIT,
  LO_UINT16(IMMEDIATE_ALERT_SERV_UUID),
  HI_UINT16(IMMEDIATE_ALERT_SERV_UUID),

  // service UUID, to notify central devices what services are included
  // in this peripheral
  0x07,  // Length of this data
  GAP_ADTYPE_16BIT_MORE,    // Some of the UUID's, but not all.
  LO_UINT16(LINK_LOSS_SERV_UUID),
  HI_UINT16(LINK_LOSS_SERV_UUID),
  LO_UINT16(IMMEDIATE_ALERT_SERV_UUID),
  HI_UINT16(IMMEDIATE_ALERT_SERV_UUID),
  LO_UINT16(TX_PWR_LEVEL_SERV_UUID),
  HI_UINT16(TX_PWR_LEVEL_SERV_UUID),

};

// GAP GATT Attributes.
static uint8_t attDeviceName[GAP_DEVICE_NAME_LEN] = "Proximity Tag";

/*********************************************************************
 * LOCAL FUNCTIONS
 */
// Application task and event processing
static void ProximityTag_init(void);
static void ProximityTag_taskFxn(UArg a0, UArg a1);
static void ProximityTag_processStackMsg(ICall_Hdr *pMsg);
static void ProximityTag_processAppMsg(proximityEvt_t *pMsg);
static void ProximityTag_ProcessGATTMsg(gattMsgEvent_t *pMsg);
static void ProximityTag_handleAlertEvt(void);
static void ProximityTag_handleResetEvt(void);
static void ProximityTag_clockHandler(UArg arg);
static uint8_t ProximityTag_enqueueMsg(uint16_t event, uint8_t state, 
                                       uint8_t *pData);

// Peripheral state.
static void ProximityTag_stateChangeCB(gaprole_States_t newState);
static void ProximityTag_processStateChangeEvt(gaprole_States_t newState);

// Pairing state.
static void ProximityTag_pairStateCB(uint16_t connHandle, uint8_t state, 
                                     uint8_t status);
static void ProximityTag_processPairStateEvt(uint8_t state, uint8_t status);

// Proximity Profile.
static void ProximityTag_serviceCB(uint8_t attrParamID);
static void ProximityTag_processServiceEvt(uint8_t attrParamID);

// Alerts.
static void ProximityTag_performAlert(void);
static void ProximityTag_stopAlert(void);

// Keys.
static void ProximityTag_keyPressHandler(uint8_t keys);
static void ProximityTag_handleKeys(uint8_t shift, uint8_t keys);

// Support functions
static void ProximityTag_addNewDevice(uint8_t *pAddr);
static void ProximityTag_newConnectedDevice(uint8_t *pAddr, 
                                            AddrIASCharHandle_t *device);
static void ProximityTag_updateKnownAddrs (uint8_t *pAddr);

static uint8_t checkHandleChangeFromInd(HandleChangeInd_t *handleChange);

static void beginServiceDiscovery();

/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks.
static gapRolesCBs_t proximityTag_PeripheralCBs =
{
  ProximityTag_stateChangeCB   // Profile State Change Callbacks
};

// GAP Bond Manager Callbacks.
static const gapBondCBs_t proximityTag_BondMgrCBs =
{
  NULL,                        // Passcode callback (not used by application)
  ProximityTag_pairStateCB     // Pairing / Bonding state Callback
};

// Proximity Peripheral Profile Callbacks.
static proxReporterCBs_t proxTag_ProximityCBs =
{
  ProximityTag_serviceCB       // Whenever the Link Loss Alert
                               // or Immediate Alert attribute changes
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      ProximityTag_createTask
 *
 * @brief   Task creation function for the Proximity Tag application.
 *
 * @param   none
 *
 * @return  none
 */
void ProximityTag_createTask(void)
{
  Task_Params taskParams;
    
  // Configure task
  Task_Params_init(&taskParams);
  taskParams.stack = ProximityTag_taskStack;
  taskParams.stackSize = PT_TASK_STACK_SIZE;
  taskParams.priority = PT_TASK_PRIORITY;
  
  Task_construct(&ProximityTag_task, ProximityTag_taskFxn, &taskParams, NULL);
}

/*********************************************************************
 * @fn      ProximityTag_init
 *
 * @brief   Initialization function for the Proximity Tag App Task.
 *          This is called during initialization and should contain
 *          any application specific initialization (ie. hardware
 *          initialization/setup, table initialization, power up
 *          notification ...).
 *
 * @param   task_id - the ID assigned by OSAL.  This ID should be
 *                    used to send messages and set timers.
 *
 * @return  none
 */
void ProximityTag_init(void)
{
	// ******************************************************************
  // N0 STACK API CALLS CAN OCCUR BEFORE THIS CALL TO ICall_registerApp
  // ******************************************************************
  // Register the current thread as an ICall dispatcher application
  // so that the application can send and receive messages.
  ICall_registerApp(&selfEntity, &sem);
    
  // Hard code the DB Address till CC2650 board gets its own IEEE address
  //uint8_t bdAddress[B_ADDR_LEN] = { 0x22, 0x22, 0x22, 0x22, 0x22, 0x5A };
  //HCI_EXT_SetBDADDRCmd(bdAddress);
  
  // Set device's Sleep Clock Accuracy
  //HCI_EXT_SetSCACmd(40);
  
  // Create an RTOS queue for message from profile to be sent to app.
  appMsgQueue = Util_constructQueue(&appMsg);
  
  // Create periodic clocks for internal events.
  Util_constructClock(&alertClock, ProximityTag_clockHandler,
                      200, 800, false, PT_ALERT_EVT);
  Util_constructClock(&resetClock, ProximityTag_clockHandler,
                      PT_START_RESET_DELAY, PT_IN_RESET_DELAY, 
                      false, PT_RESET_EVT);

  // Initialize keys on the SRF06.
  Board_initKeys(ProximityTag_keyPressHandler);

  // Setup the GAP Peripheral Role Profile
  {
    // For the CC26xx, device doesn't start advertising until button is pressed
    uint8_t initial_advertising_enable = FALSE;

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

    GAPRole_SetParameter(GAPROLE_SCAN_RSP_DATA, sizeof (scanRspData), 
                         scanRspData);
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

  // Set the GAP Characteristics
  GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, attDeviceName);

  // Set advertising interval
  {
   /*
    *  Proximity Reporter Advertising:
    *  Recommended Minimum advertising: 20ms = 32 (units of 0.625us)
    *  Recommended Maximum advertising: 30ms = 48 (units of 0.625us)
    *
    *  Note: only use one of these values.
    */
    GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MIN, 32);  // used by Proximity
    GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MAX, 32);  // used by Proximity
  }

  // Setup the GAP Bond Manager
  {
    uint32_t passkey = 0; // passkey "000000"
    uint8_t pairMode = GAPBOND_PAIRING_MODE_INITIATE;  // request pairing.
    uint8_t mitm = TRUE;
    uint8_t ioCap = GAPBOND_IO_CAP_NO_INPUT_NO_OUTPUT;
    uint8_t bonding = TRUE;
    uint8_t autoSync = TRUE;
    uint8_t bondCount = 0;
    uint8_t wlSyncFlag = FALSE;
    
    GAPBondMgr_SetParameter(GAPBOND_DEFAULT_PASSCODE, sizeof (uint32_t), 
                            &passkey);
    GAPBondMgr_SetParameter(GAPBOND_PAIRING_MODE, sizeof(uint8_t), &pairMode);
    GAPBondMgr_SetParameter(GAPBOND_MITM_PROTECTION, sizeof(uint8_t), &mitm);
    GAPBondMgr_SetParameter(GAPBOND_IO_CAPABILITIES, sizeof(uint8_t), &ioCap);
    GAPBondMgr_SetParameter(GAPBOND_BONDING_ENABLED, sizeof(uint8_t), &bonding);
    wlSyncFlag = GAPBondMgr_SetParameter(GAPBOND_AUTO_SYNC_WL, sizeof(uint8_t),
                                         &autoSync);
    
    GAPBondMgr_GetParameter(GAPBOND_BOND_COUNT, &bondCount);

    //  White list syncing is enabled, so if there is at least one bond
    //  There must be an entry in the white list to filter for.
    if (wlSyncFlag == SUCCESS && bondCount > 0)
    {
      uint8_t value = GAP_FILTER_POLICY_WHITE;

      GAPRole_SetParameter(GAPROLE_ADV_FILTER_POLICY, sizeof(uint8_t), &value);

      proxTagUsingWhiteList = TRUE;
    }
  }

  // Initialize GATT attributes.
  GGS_AddService(GATT_ALL_SERVICES);           // GAP
  GATTServApp_AddService(GATT_ALL_SERVICES);   // GATT attributes
  DevInfo_AddService();                        // Device Information Service
  ProxReporter_AddService(GATT_ALL_SERVICES);  // Proximity Reporter Profile

  // Initialize Tx Power Level characteristic in Proximity Reporter.
  {
    int8 initialTxPowerLevel = 0;
    ProxReporter_SetParameter(PP_TX_POWER_LEVEL, sizeof(int8), 
                              &initialTxPowerLevel);
  }

  // Setup Client role.
  VOID GATT_InitClient();

  // Setup for indications from Service Change.
  GATT_RegisterForInd(selfEntity);

  // Initialize the buzzer.
  buzzerInit();

  // Make sure buzzer is off.
  buzzerStop();

  // Start the Device.
  VOID GAPRole_StartDevice(&proximityTag_PeripheralCBs);

  // Start Bond Manager.
  VOID GAPBondMgr_Register((gapBondCBs_t *)&proximityTag_BondMgrCBs);

  // Start the Proximity Profile.
  VOID ProxReporter_RegisterAppCBs(&proxTag_ProximityCBs);

  // Set the proximity attribute values to default.
  ProxReporter_SetParameter(PP_LINK_LOSS_ALERT_LEVEL, sizeof(uint8_t),
                            &proxLLAlertLevel);
  ProxReporter_SetParameter(PP_IM_ALERT_LEVEL, sizeof(uint8_t),
                            &proxIMAlertLevel);
  ProxReporter_SetParameter(PP_TX_POWER_LEVEL, sizeof(int8), &proxTxPwrLevel);
}

/*********************************************************************
 * @fn      ProximityTag_taskFxn
 *
 * @brief   Proximity Tag Application Task event processor.  This function
 *          is called to process all events for the task.  Events
 *          include timers, messages and any other user defined events.
 *
 * @param   a0, a1 - not used.
 *
 * @return  none
 */
void ProximityTag_taskFxn(UArg a0, UArg a1)
{
  // Initialize the application.
  ProximityTag_init();
  
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
          ProximityTag_processStackMsg((ICall_Hdr *)pMsg);
        }
                
        if (pMsg)
        {
          ICall_freeMsg(pMsg);
        }
      }

      // If RTOS queue is not empty, process app message.
      while (!Queue_empty(appMsgQueue))
      {
        proximityEvt_t *pMsg = (proximityEvt_t *)Util_dequeueMsg(appMsgQueue);
        if (pMsg)
        {
          // Process message.
          ProximityTag_processAppMsg(pMsg);
          
          // Free the space from the message.
          ICall_free(pMsg);
        }
      }
    }

    if (events)
    {   
      // Service discovery event.
      if (events & PT_SERVICE_DISC_EVT)
      {
        events &= ~PT_SERVICE_DISC_EVT;
        
        beginServiceDiscovery();
      }
      
      // Proximity buzzer event.
      if (events & PT_ALERT_EVT)
      {
        events &= ~PT_ALERT_EVT;
        
        ProximityTag_handleAlertEvt();
      }

      if (events & PT_SEND_PARAM_UPDATE_EVT)
      {
        events &= ~PT_SEND_PARAM_UPDATE_EVT;
        
        /*
         * Recommended values for Proximity after service discovery is complete.
         * Connection Interval: 500ms (400, units of 1.25ms)
         * Slave Latency:  0
         * Timeout:  10 seconds (1000, units of 10ms)
         */
        GAPRole_SendUpdateParam(400, 400, 0, 1000, GAPROLE_NO_ACTION);
      }

      // Soft reset event.
      if (events & PT_RESET_EVT)
      { 
        events &= ~PT_RESET_EVT;
        
        ProximityTag_handleResetEvt();
      }
    }
  }
}

/*********************************************************************
 * @fn      ProximityTag_processStackMsg
 *
 * @brief   Process an incoming stack message.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void ProximityTag_processStackMsg(ICall_Hdr *pMsg)
{
  switch (pMsg->event)
  {
    case GATT_MSG_EVENT:
      // Process GATT message.
      ProximityTag_ProcessGATTMsg((gattMsgEvent_t *)pMsg);
      break;

    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      ProximityTag_processAppMsg
 *
 * @brief   Process an incoming callback from a profile.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */                       
static void ProximityTag_processAppMsg(proximityEvt_t *pMsg)
{
  switch (pMsg->hdr.event)
  {
    // State change event.
    case PT_STATE_CHANGE_EVT:
      ProximityTag_processStateChangeEvt((gaprole_States_t)pMsg->hdr.state);
      break;
    
    // Key change event.  
    case PT_KEY_CHANGE_EVT:
      ProximityTag_handleKeys(0, pMsg->hdr.state);
      break;
      
    // Pairing state CB event.
    case PT_PAIR_STATE_EVT:
      ProximityTag_processPairStateEvt(pMsg->hdr.state, *pMsg->pData);
      
      ICall_free(pMsg->pData);
      break;
      
    // Proximity service event.
    case PT_SERVICE_EVT:
      ProximityTag_processServiceEvt(pMsg->hdr.state);
      break;
      
    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      ProximityTag_keyPressHandler
 *
 * @brief   Key event handler function
 *
 * @param   keys - keys pressed.
 *
 * @return  none
 */
void ProximityTag_keyPressHandler(uint8_t keys)
{
  // Enqueue the event.
  ProximityTag_enqueueMsg(PT_KEY_CHANGE_EVT, keys, NULL);
}

/*********************************************************************
 * @fn      ProximityTag_handleKeys
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
static void ProximityTag_handleKeys(uint8_t shift, uint8_t keys)
{
  // For writing to Immediate Alert.
  uint8 alertLevel = 0xFF;

  // Intentionally unreferenced parameter.
  VOID shift;  

  // If a reset was in progress, cancel it.
  if (resetInProgress == TRUE)
  {
    resetInProgress = FALSE;
    
    Util_stopClock(&resetClock);
    
    return;
  }
  
  if (keys & KEY_SELECT)
  {
    // Reset in progress has started.
    resetInProgress = TRUE;
  
    // Begin reset timer.
    Util_startClock(&resetClock);
  }
  else if (keys & KEY_UP)
  {
    // If alert level is active, pressing the left key stops the alert.
    if ((proximityState == PROXSTATE_LINK_LOSS) ||
        (proximityState == PROXSTATE_IMM_ALERT))
    {
      ProximityTag_stopAlert();

      if (proxIMAlertLevel != PP_ALERT_LEVEL_NO)
      {
        proxIMAlertLevel = PP_ALERT_LEVEL_NO;
      }
    }
    else if ((gapProfileState == GAPROLE_CONNECTED) && (immAlertCharHandle != 0))
    {
      // Check the immediate alert level.
      if (immAlertLevel != LO_ALERT)
      {
        alertLevel = LO_ALERT;
      }
      else
      {
        alertLevel = NO_ALERT;
      }

      // Set the immediate alert level.
      immAlertLevel = alertLevel;
    }
  }
  else if (keys & KEY_RIGHT)
  {
    // If device is not in a connection, pressing the right key toggles
    // advertising on and off.
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
      else
      {
        new_adv_enabled_status = FALSE;
      }

      // change the GAP advertisement status to opposite of current status.
      GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), 
                           &new_adv_enabled_status);
    }
    else if ((gapProfileState == GAPROLE_CONNECTED) && (immAlertCharHandle != 0))
    {
      if (immAlertLevel != HI_ALERT)
      {
        alertLevel = HI_ALERT;
      }
      else
      {
        alertLevel = NO_ALERT;
      }
      
      // Set the immediate alert level.
      immAlertLevel = alertLevel;
    }
  }
  
  // See if we need to send alert level
  if (alertLevel != 0xFF)
  {
    attWriteReq_t req; 
    
    req.pValue = GATT_bm_alloc(connHandle, ATT_WRITE_REQ, 1, NULL);
    if (req.pValue != NULL)
    {
      // Initialize all fields.
      req.handle = immAlertCharHandle;
      req.len = 0x01;
      req.pValue[0] = alertLevel;
      req.sig = 0x00;
      req.cmd = 0x01;

      // Send Request.
      if (GATT_WriteNoRsp(connHandle, &req) != SUCCESS)
      {
        GATT_bm_free((gattMsg_t *)&req, ATT_WRITE_REQ);
      }
    }
  }
}

/*********************************************************************
 * @fn      ProximityTag_ProcessGATTMsg
 *
 * @brief   Retrieve GATT message.
 *
 * @param   pMsg - GATT message to read
 *
 * @return  none
 */
static void ProximityTag_ProcessGATTMsg(gattMsgEvent_t *pMsg)
{
  // Find Me Profile Read Requests.
  static attReadByTypeReq_t readReq;
  static attAttrType_t readReqType;

  // Response from GATT_DiscPrimaryServiceByUUID.
  switch(pMsg->method)
  {
    case ATT_FIND_BY_TYPE_VALUE_RSP:
    {
      if (pMsg->hdr.status == SUCCESS)
      {
        // Begin the search for characteristic handle of the service.
        readReq.startHandle = ATT_ATTR_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
        readReq.endHandle = ATT_GRP_END_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
        readReqType.len = 2;
        readReqType.uuid[0] = LO_UINT16(charUuidToFind);
        readReqType.uuid[1] = HI_UINT16(charUuidToFind);
        readReq.type = readReqType;
      }
      else if (pMsg->hdr.status == bleProcedureComplete)
      {
        GATT_DiscCharsByUUID(connHandle, &readReq, selfEntity);
      }
    }
    break;

    // Response from GATT_DiscCharsByUUID.
    case ATT_READ_BY_TYPE_RSP:
    {
      // Discover handle for a characteristic value.
      if (pMsg->hdr.status == SUCCESS)
      {       
        uint16_t handle = BUILD_UINT16(pMsg->msg.readByTypeRsp.pDataList[3],
                                       pMsg->msg.readByTypeRsp.pDataList[4]);
        if (serviceToFind == GSC)
        {
          knownAddrs[0].svcChangeHandle = serviceChangeHandle = handle;

          // Enter a GAP Bond manager Paired state.
          gapBondMgrState = PAIRED_BONDED_STATE;
        }
        else if (serviceToFind == IAS)
        {
          knownAddrs[0].alertHandle = immAlertCharHandle = handle;
        }
        
        // Increment serviceToFind to allow search for next service.
        serviceToFind++;
      }
      else if (pMsg->hdr.status == bleProcedureComplete)
      {
        // Set event to continue service discovery.
        events |= PT_SERVICE_DISC_EVT;
        
        // Post application's semaphore.
        Semaphore_post(sem);
      }
    }
    break;

    // Service Change indication.
    case ATT_HANDLE_VALUE_IND:
    {
      if (pMsg->hdr.status == SUCCESS)
      {
        uint8_t idx = 0;

        serviceChange = CHANGE_OCCURED;

        // Acknowledge receipt of indication.
        ATT_HandleValueCfm(pMsg->connHandle);

        handleChange.handle = pMsg->msg.handleValueInd.handle;

        while(idx < ((ATT_BT_UUID_SIZE) * 2))
        {
          handleChange.handleRange[idx] = pMsg->msg.handleValueInd.pValue[idx];
          idx++;
        }

        // Handles in server have changed while devices are connected.
        if ((gapBondMgrState == PAIRED_BONDED_STATE) &&
            (serviceChangeHandle == handleChange.handle))
        {
          if (checkHandleChangeFromInd(&handleChange))
          {
            serviceToFind = IAS;

            // Set event to continue service discovery.
            events |= PT_SERVICE_DISC_EVT;
        
            // Post application's semaphore.
            Semaphore_post(sem);
          }
          
          serviceChange = NO_CHANGE;
        }
      }
    }
    break;
    
    default:
    {
      // Do nothing.
    }
    break;
  }
  
  GATT_bm_free(&pMsg->msg, pMsg->method);
}

/*********************************************************************
 * @fn      ProximityTag_handleResetEvt
 *
 * @brief   "soft" resets the device.  Disables alarms, puts the device
 *           into a waiting state, clears all white list, bonding and GATT
 *           service handle information about previously previously connected
 *           devices.
 *
 * @param   none
 *
 * @return  none
 */
static void ProximityTag_handleResetEvt(void)
{
  //  All alarms off.
  if (proxLLAlertLevel != PP_ALERT_LEVEL_NO || 
      proxIMAlertLevel != PP_ALERT_LEVEL_NO)
  {
    ProximityTag_stopAlert();
    proxLLAlertLevel = PP_ALERT_LEVEL_NO;
    proxIMAlertLevel = PP_ALERT_LEVEL_NO;
  }

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
  else if (proxTagUsingWhiteList)
  {
    // Temporary variable.
    uint8_t value = GAP_FILTER_POLICY_ALL;

    // Turn off white list filter policy.
    GAPRole_SetParameter(GAPROLE_ADV_FILTER_POLICY, sizeof(uint8_t), &value);

    proxTagUsingWhiteList = FALSE;

    // Clear the white list.
    HCI_LE_ClearWhiteListCmd();
  }
  else if (gapProfileState == GAPROLE_STARTED ||
           gapProfileState == GAPROLE_WAITING ||
           gapProfileState == GAPROLE_WAITING_AFTER_TIMEOUT)
  {
    uint8_t tempTrue = TRUE;
    
    // Stop the periodic expirations of the reset clock.
    Util_stopClock(&resetClock);
    
    // Erase all information known about previously connected devices.
    while (TRUE)
    {
      memset(knownAddrs[addrIdx].addr, 0x00, B_ADDR_LEN);
      
      knownAddrs[addrIdx].alertHandle = 0;
      knownAddrs[addrIdx].svcChangeHandle = 0;

      if (addrIdx == 0)
      {
        break;
      }
      else
      {
        addrIdx--;
      }
    }

    // Erase all bonds.
    GAPBondMgr_SetParameter(GAPBOND_ERASE_ALLBONDS, 0, &tempTrue);
    
    // Turn on GREEN LED for set time
    //HalLedSet(HAL_LED_1, HAL_LED_MODE_BLINK);
  }
}

/*********************************************************************
 * @fn      ProximityTag_handleAlertEvt
 *
 * @brief   handles enabling and disabling of the buzzer and flashing LEDS
 *          during an alert.
 *
 * @param   none
 *
 * @return  none
 */
static void ProximityTag_handleAlertEvt(void)
{
  // If this event was triggered while buzzer is on, turn it off, increment 
  // buzzer_beep_count, check whether max has been reached, and if not restart 
  // the clock for next event to turn the buzzer back on.
  if (buzzer_state == BUZZER_ON)
  {
    buzzerStop();
    buzzer_state = BUZZER_OFF;
    buzzer_beep_count++;

    // Check to see if buzzer has beeped maximum number of times or if the 
    // alert has been canceled. If it has, then disable the periodic clock.
    if ((buzzer_beep_count >= BUZZER_MAX_BEEPS) ||
        ((proxLLAlertLevel == PP_ALERT_LEVEL_NO) &&
         (proxIMAlertLevel == PP_ALERT_LEVEL_NO)))
    {
      Util_stopClock(&alertClock);
    }
  }
  else if ((proxLLAlertLevel != PP_ALERT_LEVEL_NO) ||
           (proxIMAlertLevel != PP_ALERT_LEVEL_NO))
  {
    // If this event was triggered while the buzzer is off then turn it on if 
    // appropriate.
    ProximityTag_performAlert();
  }
}

/*********************************************************************
 * @fn      ProximityTag_performAlert
 *
 * @brief   Performs an alert.
 *
 * @param   none
 *
 * @return  none
 */
static void ProximityTag_performAlert(void)
{
  if (proximityState == PROXSTATE_LINK_LOSS)
  {
    switch(proxLLAlertLevel)
    {
      case PP_ALERT_LEVEL_LOW:
        buzzerStart(BUZZER_ALERT_LOW_FREQ);
        
        buzzer_state = BUZZER_ON;

        // Only run buzzer for 200ms.
        Util_startClock(&alertClock);

        // Turn LEDs on.
        //HalLedSet((HAL_LED_1 | HAL_LED_2), HAL_LED_MODE_OFF);
        break;

      case PP_ALERT_LEVEL_HIGH:
        buzzerStart(BUZZER_ALERT_HIGH_FREQ);
        
        buzzer_state = BUZZER_ON;
        
        // Only run buzzer for 200ms.
        Util_startClock(&alertClock);

        // Turn LED 1 on.
        //HalLedSet(HAL_LED_1, HAL_LED_MODE_ON);
        
        // Flash LED 2.
        //HalLedSet(HAL_LED_2, HAL_LED_MODE_FLASH);
        break;

      case PP_ALERT_LEVEL_NO:
          // Fall through.
      default:
        ProximityTag_stopAlert();
        break;
    }
  }
  else if (proximityState == PROXSTATE_IMM_ALERT)
  {
    switch(proxIMAlertLevel)
    {
      case PP_ALERT_LEVEL_LOW:
        buzzerStart(BUZZER_ALERT_LOW_FREQ);
        
        buzzer_state = BUZZER_ON;
      
        // Only run buzzer for 200ms.
        Util_startClock(&alertClock);

        // Turn off LEDs.
        //HalLedSet((HAL_LED_1 | HAL_LED_2), HAL_LED_MODE_OFF);
        break;

      case PP_ALERT_LEVEL_HIGH:
        buzzerStart(BUZZER_ALERT_HIGH_FREQ);
        
        buzzer_state = BUZZER_ON;
        
        // Only run buzzer for 200ms.
        Util_startClock(&alertClock);

        // Turn LED 1 on.
        //HalLedSet(HAL_LED_1, HAL_LED_MODE_ON);
        
        // Flash LED 2.
        //HalLedSet(HAL_LED_2, HAL_LED_MODE_FLASH);
        break;

      case PP_ALERT_LEVEL_NO:
        // Fall through.
      default:
        ProximityTag_stopAlert();
        break;
    }
  }
}

/*********************************************************************
 * @fn      ProximityTag_stopAlert
 *
 * @brief   Stops an alert.
 *
 * @param   none
 *
 * @return  none
 */
void ProximityTag_stopAlert(void)
{
  // Disable alert clock.
  Util_stopClock(&alertClock);
  
  buzzerStop();
  buzzer_state = BUZZER_OFF;
  
  // Turn LEDs off.
  //HalLedSet((HAL_LED_1 | HAL_LED_2), HAL_LED_MODE_OFF);
  
  // Set the proximity state.
  if (gapProfileState == GAPROLE_CONNECTED)
  {
    // An immediate alert was triggered, as the device is still in a connection.
    proximityState = PROXSTATE_CONNECTED_IN_RANGE;
  }
  // The device is disconnected, therefore a link loss had occurred.
  else
  {
    proximityState = PROXSTATE_INITIALIZED;
  }
}

/*********************************************************************
 * @fn      ProximityTag_stateChangeCB
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void ProximityTag_stateChangeCB(gaprole_States_t newState)
{
  ProximityTag_enqueueMsg(PT_STATE_CHANGE_EVT, newState, NULL);
}

/*********************************************************************
 * @fn      ProximityTag_processStateChangeEvt
 *
 * @brief   Notification from the profile of a state change.
 *
 * @param   newState - new state
 *
 * @return  none
 */
static void ProximityTag_processStateChangeEvt(gaprole_States_t newState)
{
  switch (newState)
  {
    case GAPROLE_STARTED:
      {
        uint8_t ownAddress[B_ADDR_LEN];
        uint8_t systemId[DEVINFO_SYSTEM_ID_LEN];

        GAPRole_GetParameter(GAPROLE_BD_ADDR, ownAddress);

        // Use 6 bytes of device address for 8 bytes of system ID value.
        systemId[0] = ownAddress[0];
        systemId[1] = ownAddress[1];
        systemId[2] = ownAddress[2];

        // Set middle bytes to zero.
        systemId[4] = 0x00;
        systemId[3] = 0x00;

        // Shift three bytes up.
        systemId[7] = ownAddress[5];
        systemId[6] = ownAddress[4];
        systemId[5] = ownAddress[3];

        DevInfo_SetParameter(DEVINFO_SYSTEM_ID, DEVINFO_SYSTEM_ID_LEN, systemId);

        proximityState = PROXSTATE_INITIALIZED;
      }
      break;

    case GAPROLE_ADVERTISING:
      {
        //Turn on RED LED
        //HalLedSet(HAL_LED_2, HAL_LED_MODE_BLINK);
      }
      break;

    // If the state changed to connected, initially assume that the device is in 
    // range.
    case GAPROLE_CONNECTED:
      {
        GAPRole_GetParameter(GAPROLE_CONNHANDLE, &connHandle);

        // If not already in the white list, add address to white list and set 
        // scan and connection parameters from white list only.
        if (!proxTagUsingWhiteList)
        {
          uint8_t value = GAP_FILTER_POLICY_WHITE;

          GAPRole_SetParameter(GAPROLE_ADV_FILTER_POLICY, sizeof(uint8_t), &value);

          proxTagUsingWhiteList = TRUE;
        }
        
        // Set the proximity state to either path loss alert or in range 
        // depending on the value of ProxIMAlertLevel (which was set by 
        // the Proximity Monitor).
        if (proxIMAlertLevel != PP_ALERT_LEVEL_NO)
        {
          proximityState = PROXSTATE_IMM_ALERT;
          
          // Perform alert.
          ProximityTag_performAlert();
          buzzer_beep_count = 0;
        }
        // If no immediate alarm, turn off the alarms.
        else
        {
          proximityState = PROXSTATE_CONNECTED_IN_RANGE;
          ProximityTag_stopAlert();
        }
      }
      break;

    case GAPROLE_WAITING:
      {
        // Invalidate handles.
        immAlertCharHandle = GATT_INVALID_HANDLE;
        serviceChangeHandle = GATT_INVALID_HANDLE;
        connHandle = GAP_CONNHANDLE_INIT;

        gapBondMgrState = UNPAIRED_STATE;

        // Set proximity state.
        proximityState = PROXSTATE_INITIALIZED;

        // Stop alert if it was active.
        if (proxIMAlertLevel != PP_ALERT_LEVEL_NO)
        {
          ProximityTag_stopAlert();

          // Immediate Alert is turned off in case of device disconnect.
          proxIMAlertLevel = PP_ALERT_LEVEL_NO;
        }
      }
      break;

    case GAPROLE_WAITING_AFTER_TIMEOUT:
      {
        // Invalidate handles.
        immAlertCharHandle = GATT_INVALID_HANDLE;
        serviceChangeHandle = GATT_INVALID_HANDLE;
        connHandle = GAP_CONNHANDLE_INIT;

        gapBondMgrState = UNPAIRED_STATE;

        // The link was dropped due to supervision timeout.
        proximityState = PROXSTATE_LINK_LOSS;

        // Perform link loss alert if enabled.
        if(proxLLAlertLevel != PP_ALERT_LEVEL_NO)
        {
          ProximityTag_performAlert();
          buzzer_beep_count = 0;
        }

        // Immediate Alert is turned off in case of device disconnect.
        proxIMAlertLevel = PP_ALERT_LEVEL_NO;
      }
      break;

    case GAPROLE_ERROR:
      {
        // Do nothing.
      }
      break;

    default:
      {
        // Do nothing
      }
      break;
  }

  // Set new state.
  gapProfileState = newState;
}

/*********************************************************************
 * @fn      ProximityTag_serviceCB
 *
 * @brief   Notification from the profile of an attribute change by
 *          a connected device.
 *
 * @param   attrParamID - Profile's Attribute Parameter ID
 *            PP_LINK_LOSS_ALERT_LEVEL  - The link loss alert level value
 *            PP_IM_ALERT_LEVEL  - The immediate alert level value
 *
 * @return  none
 */
static void ProximityTag_serviceCB(uint8_t attrParamID)
{
    // Queue the event.
    ProximityTag_enqueueMsg(PT_SERVICE_EVT, attrParamID, NULL);
}

/*********************************************************************
 * @fn      ProximityTag_processServiceEvt
 *
 * @brief   Notification from the profile of an attribute change by
 *          a connected device.
 *
 * @param   attrParamID - Profile's Attribute Parameter ID
 *            PP_LINK_LOSS_ALERT_LEVEL  - The link loss alert level value
 *            PP_IM_ALERT_LEVEL  - The immediate alert level value
 *
 * @return  none
 */
static void ProximityTag_processServiceEvt(uint8_t attrParamID)
{
  switch(attrParamID)
  {
    case PP_LINK_LOSS_ALERT_LEVEL:
      ProxReporter_GetParameter(PP_LINK_LOSS_ALERT_LEVEL, &proxLLAlertLevel);
      break;

    // Immediate Alert, used in Find Me Profile and Proximity Monitor (Path Loss)
    case PP_IM_ALERT_LEVEL:
      {
        ProxReporter_GetParameter(PP_IM_ALERT_LEVEL, &proxIMAlertLevel);

        // Find Me alert was triggered by the Find Me Locator or
        // Path Loss has exceeded the threshold value found in the Proximity Monitor
        if(proxIMAlertLevel != PP_ALERT_LEVEL_NO)
        {
          proximityState = PROXSTATE_IMM_ALERT;
          ProximityTag_performAlert();
          buzzer_beep_count = 0;
        }
        else // proximity monitor turned off alert because the path loss is below threshold
        {
          proximityState = PROXSTATE_CONNECTED_IN_RANGE;
          ProximityTag_stopAlert();
        }
      }
      break;

    default:
      // should not reach here!
      break;
  }
}

/*********************************************************************
 * @fn      ProximityTag_addNewDevice
 *
 * @brief   Adds new device to the head, pushes everything else back.
 *          the tail element is overwritten, it is always the least recently 
 *          used connection.
 *
 * @param   pAddr - pointer to the device to move to front.
 *
 * @return  none
 */
static void ProximityTag_addNewDevice(uint8_t *pAddr)
{
  uint8_t index = addrIdx;
  AddrIASCharHandle_t newDev;

  ProximityTag_newConnectedDevice(pAddr, &newDev);

  if (addrIdx != 0)
  {
    // Move elements so that new device may be added on top.
    while((index - 1) >= 0)
    {
      if (index < MAX_KNOWN_ADDRS)
      {
        // If not null or not less than 0, move n-1 into n.
        knownAddrs[index] = knownAddrs[index - 1];
      }
      
      index--;
    }
  }

  knownAddrs[0] = newDev;

  // Increment addrIdx if it is not already at max.
  addrIdx = (addrIdx < MAX_KNOWN_ADDRS) ? (addrIdx + 1) : MAX_KNOWN_ADDRS;
}

/********************************************************************
 * @fn     ProximityTag_newConnectedDevice
 *
 * @brief  create new AddrIASCharHandle_t variable and
 *         copies in the address and sets the handle to 0.
 *
 * @param  pAddr - address of the new device.
 * @param  device - struct containing device's address and
 *                  IAS and service change handles.
 */
static void ProximityTag_newConnectedDevice(uint8_t *pAddr, 
                                            AddrIASCharHandle_t *device)
{
  uint8_t idx = 0;

  device->alertHandle = GATT_INVALID_HANDLE;
  device->svcChangeHandle = GATT_INVALID_HANDLE;

  while (idx < B_ADDR_LEN)
  {
    device->addr[idx] = *(pAddr++);
    idx++;
  }
}

/*******************************************************************
 * @fn     ProximityTag_updateKnownAddrs
 *
 * @brief  reorder the array as necessary to list in order most recent to oldest
 *
 * @param  pAddr     pointer to the address of the newly connected device.
 */
static void ProximityTag_updateKnownAddrs(uint8_t *pAddr)
{
  uint8_t idx = 0;
  AddrIASCharHandle_t temp;
  bool isFound = FALSE;

  // Search for device.
  while (idx < MAX_KNOWN_ADDRS)
  {
    //  When device is found, create a temp, shift down from top to the index
    //  and reinsert the temp at the top.
    if (!memcmp(pAddr, knownAddrs[idx].addr, B_ADDR_LEN))
    {
      // If found at the head, do nothing.
      if (idx != 0)
      {
        temp = knownAddrs[idx];
        // Insert the device at the top.
        while ((idx - 1) >= 0)
        {
          knownAddrs[idx] = knownAddrs[idx - 1];
          idx--;
        }
        
        knownAddrs[0] = temp;
      }
      
      isFound = TRUE;
      break;
    }
    
    idx++;
  }

  if (!isFound)
  {
    ProximityTag_addNewDevice(pAddr);
  }
}

/*********************************************************************
 * @fn      ProximityTag_pairStateCB
 *
 * @brief   Pairing state callback.
 *
 * @param   connHandle - connected device's connection handle
 * @param   state      - pairing state the devices have entered
 * @param   status     - status of upon entering this state.
 *
 * @return  none
 */
static void ProximityTag_pairStateCB(uint16_t connHandle, uint8_t state, 
                                     uint8_t status)
{
  uint8_t *pData;
  
  // Allocate message data.
  if ((pData = ICall_malloc(sizeof(uint8_t))))
  {
    *pData = status;
    
    // Queue the event.
    ProximityTag_enqueueMsg(PT_PAIR_STATE_EVT, state, pData);
  }
}

/*********************************************************************
 * @fn      ProximityTag_processPairStateEvt
 *
 * @brief   notifications from GAP Bond Manager
 *
 * @param   connHandle is the connection handle
 *          state is the current Gap Bond Manager state
 *          status is not used, but required
 *
 * @return  none
 */
static void ProximityTag_processPairStateEvt(uint8_t state, uint8_t status)
{
  uint8_t addr[B_ADDR_LEN];

  // Get address of the Central Device.
  GAPRole_GetParameter(GAPROLE_CONN_BD_ADDR, addr);

  switch (state)
  {
    case GAPBOND_PAIRING_STATE_COMPLETE:
    {
      if (status == SUCCESS)
      {
        // Start search for handles.
        gapBondMgrState = NEW_DISCOVERY_STATE;

        // Add new device to the list.
        ProximityTag_updateKnownAddrs(addr);

        serviceToFind = GSC;
        
        // Set event to start service discovery.
        events |= PT_SERVICE_DISC_EVT;
        
        // Post application's semaphore.
        Semaphore_post(sem);        
      }
    }
    break;

    case GAPBOND_PAIRING_STATE_BONDED:
    {
      // Enter a bonded state.
      gapBondMgrState = PAIRED_BONDED_STATE;

      // Figure out with which device we have re-established a bond.
      ProximityTag_updateKnownAddrs(addr);

      // Get the Service Change handle.
      serviceChangeHandle = knownAddrs[0].svcChangeHandle;
      
      // Get the Immediate Alert characteristic value handle.
      immAlertCharHandle = knownAddrs[0].alertHandle;

      // Check if there has been a service change or if this is a newly 
      // connected device.
      if ((serviceChangeHandle == GATT_INVALID_HANDLE)    ||
           ((serviceChange == CHANGE_OCCURED)             &&
             (serviceChangeHandle == handleChange.handle) &&
             (checkHandleChangeFromInd(&handleChange))))
      {

        if (serviceChangeHandle == GATT_INVALID_HANDLE)
        {
          gapBondMgrState = NEW_DISCOVERY_STATE;
          
          // Start search for handles.
          serviceToFind = GSC;
        }
        else
        {
          serviceToFind = IAS;
        }

        serviceChange = NO_CHANGE;
        
        // Set event to start service discovery.
        events |= PT_SERVICE_DISC_EVT;
        
        // Post application's semaphore.
        Semaphore_post(sem);
      }
      else
      {
        serviceToFind = ALL_SERVICES_DISCOVERED;
      }
    }
    break;

    default:
    {
      // Something else has occurred.
    }
    break;
  }
}

/*********************************************************************
 * @fn      beginServiceDiscovery
 *
 * @brief   perform service discovery based on the current state
 *          given by the gap bond manager.
 *
 * @param   none
 *
 * @return  none
 */
static void beginServiceDiscovery(void)
{
  uint16_t uuid;

  switch(serviceToFind)
  {
    case GSC:
      {
        uuid = GATT_SERVICE_UUID;
        charUuidToFind = SERVICE_CHANGED_UUID;
      }
      break;
      
    case IAS:
      {
        uuid = IMMEDIATE_ALERT_SERV_UUID;
        charUuidToFind = ALERT_LEVEL_UUID;
      }
      break;
      
    case ALL_SERVICES_DISCOVERED:
      {
        // Service discovery is finished.
        uuid = 0;
        charUuidToFind = 0;
        
        // Set a parameter update event.
        events |= PT_SEND_PARAM_UPDATE_EVT;
        
        // Post the application's semaphore.
        Semaphore_post(sem);
      }
      break;
      
    default:
      {
        uuid = 0;
        charUuidToFind = 0;
      }
      break;
  }

  if (serviceToFind != ALL_SERVICES_DISCOVERED)
  {
    uint8_t type[ATT_BT_UUID_SIZE] = { LO_UINT16(uuid), HI_UINT16(uuid) };

    GATT_DiscPrimaryServiceByUUID(connHandle, type, 0x02, selfEntity);
  }
}

/*********************************************************************
 * @fn     checkHandleChangeFromInd
 *
 * @brief  check if the range of handles given from a service change indication
 *         effects the immediate alert characteristic handle
 *
 * @param  handleChange contains the handle range from a Service Change 
 *         indication.
 *
 * @return TRUE if immediate alert characteristic handle fall within the given 
 *         range, FALSE if otherwise.
 */
static uint8_t checkHandleChangeFromInd(HandleChangeInd_t *handleChange)
{
  uint16_t startHandle = BUILD_UINT16(handleChange->handleRange[0], 
                                      handleChange->handleRange[1]);
  uint16_t endHandle = BUILD_UINT16(handleChange->handleRange[2], 
                                    handleChange->handleRange[3]);

  if ((startHandle <= immAlertCharHandle) && (endHandle >= immAlertCharHandle))
  {
    return TRUE;
  }

  return FALSE;
}

/*********************************************************************
 * @fn      ProximityTag_clockHandler
 *
 * @brief   Clock handle for all clock events.  This function stores an event 
 *          flag and wakes up the application's event processor.
 *
 * @param   arg - event flag.
 *
 * @return  none
 */
static void ProximityTag_clockHandler(UArg arg)
{
  // Store the event.
  events |= arg;
  
  // Wake up the application.
  Semaphore_post(sem);
}

/*********************************************************************
 * @fn      ProximityTag_enqueueMsg
 *
 * @brief   Creates a message and puts the message in RTOS queue.
 *
 * @param   event - message event.
 * @param   state - message state.
 * @param   pDdata - message data pointer.
 *
 * @return  TRUE or FALSE
 */
static uint8_t ProximityTag_enqueueMsg(uint16_t event, uint8_t state, 
                                       uint8_t *pData)
{
  proximityEvt_t *pMsg;
  
  // Create dynamic pointer to message.
  if (pMsg = ICall_malloc(sizeof(proximityEvt_t)))
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
