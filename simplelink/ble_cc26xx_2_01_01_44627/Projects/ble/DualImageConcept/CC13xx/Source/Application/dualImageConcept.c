/**************************************************************************************************
  Filename:       dualImageConcept.c
  Revised:        $Date: 2015-05-12 08:44:57 -0700 (Tue, 12 May 2015) $
  Revision:       $Revision: 43753 $

  Description:    This file contains the dual image concept sample application
                  to demonstrate Bluetooth Low Energy Protocol Stack on a CC1350.

  Copyright 2014 - 2015 Texas Instruments Incorporated. All rights reserved.

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
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <string.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>

#include <driverlib/vims.h>
#include <driverlib/flash.h>

#include "hci_tl.h"
#include "gatt.h"

#include "gapgattserver.h"
#include "gattservapp.h"

#include "peripheral.h"
#include "gapbondmgr.h"

#include "osal_snv.h"
#include "ICallBleAPIMSG.h"

#include "util.h"
#include "board_lcd.h"
#include "board_key.h"
#include "Board.h"

#include <ti/drivers/lcd/LCDDogm1286.h>
/*********************************************************************
 * CONSTANTS
 */
// Advertising interval when device is discoverable (units of 625us, 160=100ms)
#define DEFAULT_ADVERTISING_INTERVAL          160

// Limited discoverable mode advertises for 30.72s, and then stops
// General discoverable mode advertises indefinitely
#define DEFAULT_DISCOVERABLE_MODE             GAP_ADTYPE_FLAGS_GENERAL

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
#define DEFAULT_CONN_PAUSE_PERIPHERAL         6

// How often to perform periodic event (in msec)
#define DIC_PERIODIC_EVT_PERIOD               5000

// Task configuration
#define DIC_TASK_PRIORITY                     1


#ifndef DIC_TASK_STACK_SIZE
#define DIC_TASK_STACK_SIZE                   644
#endif

// Internal Events for RTOS application
#define DIC_STATE_CHANGE_EVT                  0x0001
#define DIC_KEY_CHANGE_EVT                    0x0002

// Image select symbols
#define LOAD_IMG_A                            0x01
#define LOAD_IMG_B                            0x02

#define BIM_LEDGER_ADDR                       0x11000

/*********************************************************************
 * TYPEDEFS
 */

// App event passed from profiles.
typedef struct
{
  appEvtHdr_t hdr; // Event header
  uint8_t *pData;  // Event data
} dicEvt_t;

/*********************************************************************
 * LOCAL VARIABLES
 */

// Entity ID globally used to check for source and/or destination of messages
static ICall_EntityID selfEntity;

// Semaphore globally used to post events to the application thread
static ICall_Semaphore sem;

// Queue object used for app messages
static Queue_Struct appMsg;
static Queue_Handle appMsgQueue;

// Task configuration
Task_Struct dicTask;
Char dicTaskStack[DIC_TASK_STACK_SIZE];

// GAP - SCAN RSP data (max size = 31 bytes)
static uint8_t scanRspData[] =
{
  // complete name
  0x11,   // length of this data
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,
  'D',
  'u',
  'a',
  'l',
  'I',
  'm',
  'a',
  'g',
  'e',
  'C',
  'o',
  'n',
  'c',
  'e',
  'p',
  't',

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
// best kept short to conserve power while advertisting)
static uint8_t advertData[] =
{
  // Flags; this sets the device to use limited discoverable
  // mode (advertises for 30 seconds at a time) instead of general
  // discoverable mode (advertises indefinitely)
  0x02,   // length of this data
  GAP_ADTYPE_FLAGS,
  DEFAULT_DISCOVERABLE_MODE | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
};

// GAP GATT Attributes
static uint8_t attDeviceName[GAP_DEVICE_NAME_LEN] = "Dual Image Concept";

static uint8_t *pLedgerPg = (uint8_t *)BIM_LEDGER_ADDR;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void DualImageConcept_init( void );
static void DualImageConcept_taskFxn(UArg a0, UArg a1);

static void DualImageConcept_processStackMsg(ICall_Hdr *pMsg);
static void DualImageConcept_processGATTMsg(gattMsgEvent_t *pMsg);
static void DualImageConcept_processAppMsg(dicEvt_t *pMsg);
static void DualImageConcept_processStateChangeEvt(gaprole_States_t newState);
static void DualImageConcept_stateChangeCB(gaprole_States_t newState);
static void DualImageConcept_enqueueMsg(uint8_t event, uint8_t state,
                                        uint8_t *pData);

static void DualImageConcept_handleKeys(uint8_t shift, uint8_t keys);
void DualImageConcept_keyChangeHandler(uint8 keys);
static void switchImage(void);
/*********************************************************************
 * PROFILE CALLBACKS
 */

// GAP Role Callbacks
static gapRolesCBs_t DualImageConcept_gapRoleCBs =
{
  DualImageConcept_stateChangeCB     // Profile State Change Callbacks
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      DualImageConcept_createTask
 *
 * @brief   Task creation function for the Dual Image Concept.
 *
 * @param   None.
 *
 * @return  None.
 */
void DualImageConcept_createTask(void)
{
  Task_Params taskParams;
    
  // Configure task
  Task_Params_init(&taskParams);
  taskParams.stack = dicTaskStack;
  taskParams.stackSize = DIC_TASK_STACK_SIZE;
  taskParams.priority = DIC_TASK_PRIORITY;
  
  Task_construct(&dicTask, DualImageConcept_taskFxn, &taskParams, NULL);
}

/*********************************************************************
 * @fn      DualImageConcept_init
 *
 * @brief   Called during initialization and contains application
 *          specific initialization (ie. hardware initialization/setup,
 *          table initialization, power up notification, etc), and
 *          profile initialization/setup.
 *
 * @param   None.
 *
 * @return  None.
 */
static void DualImageConcept_init(void)
{
	// ******************************************************************
  // N0 STACK API CALLS CAN OCCUR BEFORE THIS CALL TO ICall_registerApp
  // ******************************************************************
  // Register the current thread as an ICall dispatcher application
  // so that the application can send and receive messages.
  ICall_registerApp(&selfEntity, &sem);
    
  // Hard code the BD Address till CC2650 board gets its own IEEE address
  //uint8 bdAddress[B_ADDR_LEN] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x77 };
  //HCI_EXT_SetBDADDRCmd(bdAddress);
  
  // Set device's Sleep Clock Accuracy
  //HCI_EXT_SetSCACmd(40);
  
  // Create an RTOS queue for message from profile to be sent to app.
  appMsgQueue = Util_constructQueue(&appMsg);
  
  Board_initKeys(DualImageConcept_keyChangeHandler);
  
  Board_openLCD();
  
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
  GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN, attDeviceName);

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

  // Start the Device
  VOID GAPRole_StartDevice(&DualImageConcept_gapRoleCBs);
  
#if defined(HAL_IMAGE_A)
  LCD_WRITE_STRING("CC1350 BLE Image A", LCD_PAGE0);
#elif defined(HAL_IMAGE_B)
  LCD_WRITE_STRING("CC1350 BLE Image B", LCD_PAGE0);
#endif // HAL_IMAGE
}

/*********************************************************************
 * @fn      DualImageConcept_taskFxn
 *
 * @brief   Application task entry point for the Dual Image Concept.
 *
 * @param   a0, a1 - not used.
 *
 * @return  None.
 */
static void DualImageConcept_taskFxn(UArg a0, UArg a1)
{
  // Initialize application
  DualImageConcept_init();
  
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
          DualImageConcept_processStackMsg((ICall_Hdr *)pMsg);
        }
                
        if (pMsg)
        {
          ICall_freeMsg(pMsg);
        }
      }

      // If RTOS queue is not empty, process app message.
      while (!Queue_empty(appMsgQueue))
      {
        dicEvt_t *pMsg = (dicEvt_t *)Util_dequeueMsg(appMsgQueue);
        if (pMsg)
        {
          // Process message.
          DualImageConcept_processAppMsg(pMsg);
          
          // Free the space from the message.
          ICall_free(pMsg);
        }
      }
    }
  }
}

/*********************************************************************
 * @fn      DualImageConcept_processStackMsg
 *
 * @brief   Process an incoming stack message.
 *
 * @param   pMsg - message to process
 *
 * @return  None.
 */
static void DualImageConcept_processStackMsg(ICall_Hdr *pMsg)
{
  switch (pMsg->event)
  {
    case GATT_MSG_EVENT:
      // Process GATT message
      DualImageConcept_processGATTMsg((gattMsgEvent_t *)pMsg);
      break;

    default:
      // do nothing
      break;
  }
}

/*********************************************************************
 * @fn      DualImageConcept_processGATTMsg
 *
 * @brief   Process GATT messages
 *
 * @return  None.
 */
static void DualImageConcept_processGATTMsg(gattMsgEvent_t *pMsg)
{  
  GATT_bm_free(&pMsg->msg, pMsg->method);
}

/*********************************************************************
 * @fn      DualImageConcept_processAppMsg
 *
 * @brief   Process an incoming callback from a profile.
 *
 * @param   pMsg - message to process
 *
 * @return  None.
 */                       
static void DualImageConcept_processAppMsg(dicEvt_t *pMsg)
{
  switch (pMsg->hdr.event)
  {
    case DIC_STATE_CHANGE_EVT:
      DualImageConcept_processStateChangeEvt((gaprole_States_t)pMsg->hdr.state);
      break;

    case DIC_KEY_CHANGE_EVT:
      DualImageConcept_handleKeys(0, pMsg->hdr.state);
      break;
      
    default:
      // Do nothing.
      break;
  }
}

/*********************************************************************
 * @fn      DualImageConcept_stateChangeCB
 *
 * @brief   Callback from GAP Role indicating a role state change.
 *
 * @param   newState - new state
 *
 * @return  None.
 */
static void DualImageConcept_stateChangeCB(gaprole_States_t newState)
{
  DualImageConcept_enqueueMsg(DIC_STATE_CHANGE_EVT, newState, NULL);
}

/*********************************************************************
 * @fn      DualImageConcept_processStateChangeEvt
 *
 * @brief   Process a pending GAP Role state change event.
 *
 * @param   newState - new state
 *
 * @return  None.
 */
static void DualImageConcept_processStateChangeEvt(gaprole_States_t newState)
{
  switch ( newState )
  {
    case GAPROLE_STARTED:
      {
        uint8_t ownAddress[B_ADDR_LEN];

        GAPRole_GetParameter(GAPROLE_BD_ADDR, ownAddress);

        // Display device address
        LCD_WRITE_STRING(Util_convertBdAddr2Str(ownAddress), LCD_PAGE1);
        LCD_WRITE_STRING("Initialized", LCD_PAGE2);
      }
      break;
      
    case GAPROLE_ADVERTISING:
        LCD_WRITE_STRING("Advertising", LCD_PAGE2);
      break;

    case GAPROLE_CONNECTED:
      {
        uint8_t peerAddress[B_ADDR_LEN];

        GAPRole_GetParameter(GAPROLE_CONN_BD_ADDR, peerAddress);

        LCD_WRITE_STRING("Connected", LCD_PAGE2);
        LCD_WRITE_STRING(Util_convertBdAddr2Str(peerAddress), LCD_PAGE3);
      }
      break;

    case GAPROLE_WAITING: 
      LCD_WRITE_STRING("Disconnected", LCD_PAGE2);

      // Clear remaining lines
      LCD_WRITE_STRING("", LCD_PAGE3);
      LCD_WRITE_STRING("", LCD_PAGE4);
      LCD_WRITE_STRING("", LCD_PAGE5);
      break;
      
    case GAPROLE_WAITING_AFTER_TIMEOUT:
      LCD_WRITE_STRING("Timed Out", LCD_PAGE2);
      
      // Clear remaining lines
      LCD_WRITE_STRING("", LCD_PAGE3);
      LCD_WRITE_STRING("", LCD_PAGE4);
      LCD_WRITE_STRING("", LCD_PAGE5);
      break;
      
    case GAPROLE_ERROR:
      break;
      
    default:
      break;
  }
}

/*********************************************************************
 * @fn      DualImageConcept_handleKeys
 *
 * @brief   Handles all key events for this device.
 *
 * @param   shift - true if in shift/alt.
 * @param   keys - bit field for key events. Valid entries:
 *                 HAL_KEY_SW_2
 *                 HAL_KEY_SW_1
 *
 * @return  none
 */
static void DualImageConcept_handleKeys(uint8_t shift, uint8_t keys)
{
  (void)shift;  // Intentionally unreferenced parameter

  if (keys & KEY_SELECT)
  { 
    // Signal a change in images to run.
    switchImage();
    
    // Reset.
    HAL_SYSTEM_RESET();
    
    // Should never get here.
  }
}

/*********************************************************************
 * @fn      DualImageConcept_keyChangeHandler
 *
 * @brief   Key event handler function
 *
 * @param   a0 - ignored
 *
 * @return  none
 */
void DualImageConcept_keyChangeHandler(uint8 keys)
{
  DualImageConcept_enqueueMsg(DIC_KEY_CHANGE_EVT, keys, NULL);
}

/*********************************************************************
 * @fn      DualImageConcept_enqueueMsg
 *
 * @brief   Creates a message and puts the message in RTOS queue.
 *
 * @param   event  - message event.
 * @param   state  - message state.
 * @param   pData  - message data pointer.
 *
 * @return  None.
 */
static void DualImageConcept_enqueueMsg(uint8_t event, uint8_t state, 
                                        uint8_t *pData)
{
  dicEvt_t *pMsg;
  
  // Create dynamic pointer to message.
  if ((pMsg = ICall_malloc(sizeof(dicEvt_t))))
  {
    pMsg->hdr.event = event;
    pMsg->hdr.state = state;
    pMsg->pData = pData;
    
    // Enqueue the message.
    Util_enqueueMsg(appMsgQueue, sem, (uint8*)pMsg);
  }
}

/*********************************************************************
 * @fn      enableCache
 *
 * @brief   enable cache.
 *
 * @param   state - the VIMS state returned from disableCache.
 *
 * @return  none.
 */
static void enableCache ( uint8_t state )
{
  if ( state != VIMS_MODE_DISABLED )
  {
    // Enable the Cache.
    VIMSModeSet( VIMS_BASE, VIMS_MODE_ENABLED );
  }
}

/*********************************************************************
 * @fn      disableCache
 *
 * @brief   invalidate and disable cache.
 *
 * @param   none
 *
 * @return  VIMS state
 */
static uint8_t disableCache ( void )
{
  uint8_t state = VIMSModeGet( VIMS_BASE );
  
  // Check VIMS state
  if ( state != VIMS_MODE_DISABLED )
  {
    // Invalidate cache
    VIMSModeSet( VIMS_BASE, VIMS_MODE_DISABLED );
  
    // Wait for disabling to be complete
    while ( VIMSModeGet( VIMS_BASE ) != VIMS_MODE_DISABLED ); 
    
  }
  
  return state;
}

/*********************************************************************
 * @fn      switchImage
 *
 * @brief   Write to BIM's ledger page to signal the other image should be
 *          executed after reset.
 *
 * @param   none
 *
 * @return  VIMS state
 */
static void switchImage(void)
{
  uint16_t i;
  uint8_t id;
  uint8_t state;
  
   // Find last offset unwritten offset in ledger page. 
   for (i=0; i < HAL_FLASH_PAGE_SIZE; i++)
   {
     id = pLedgerPg[i];
     
     if(id == 0xFF)
     {
       break;
     }
   }
   
   // Disable Cache.
   state = disableCache();
   
   // check if the ledger is full
   if (i == HAL_FLASH_PAGE_SIZE)
   {
     // Erase the ledger page.
     FlashSectorErase((uint32_t)pLedgerPg);
     
     // Set index to first byte
     i = 0;
   }
   
   // Write the image id to jump to.
#if defined(HAL_IMAGE_A)
   id = LOAD_IMG_B;
#elif defined (HAL_IMAGE_B)
   id = LOAD_IMG_A;
#endif // HAL_IMAGE
   
   FlashProgram(&id, (uint32_t)&pLedgerPg[i], 1);
   
   // Enable Cache.
   enableCache(state);
}

/*********************************************************************
*********************************************************************/
