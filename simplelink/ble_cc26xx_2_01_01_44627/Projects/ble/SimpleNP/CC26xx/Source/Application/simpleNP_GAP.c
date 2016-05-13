/**************************************************************************************************
  Filename:       simpleNP_GAP.c
  Revised:        $Date: 2016-01-11 17:54:51 -0800 (Mon, 11 Jan 2016) $
  Revision:       $Revision: 44608 $

  Description:    This file contains the parsing of GAP related command for 
                  the Simple BLE Peripheral sample application,
                  for use with the CC2650 Bluetooth Low Energy Protocol Stack.

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
#include <xdc/std.h>

#include "hal_types.h"
#include "comdef.h"

#include <ICall.h>

#include "gatt.h"

#include "gattservapp.h"
#include "inc/npi_task.h"

#include "peripheral.h"
#include "gapbondmgr.h"
#include "hci_tl.h"

#include "osal_snv.h"
#include "ICallBleAPIMSG.h"

#include "util.h"

#include "SNP.h"
#include "simpleNP.h"
#include "simpleNP_GAP.h"
#include "simpleNP_GATT.h"
#include "devinfoservice.h"

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
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL     160

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

#define SNP_ADV_DATA_IN_CONNECTION             0x1
#define SNP_ADV_DATA_AFTER_CONNECTION          0x2

#define SNP_ADV_DATA_SCAN_RSP_IDX              0
#define SNP_ADV_DATA_NON_CONN_IDX              1
#define SNP_ADV_DATA_CONN_IDX                  2
#define SNP_ADV_DATA_MAX_IDX                   3

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

// Advertisement attribute
static uint8_t advBehavior = 0;
static uint8_t advReqAdvType = 0;
//! [Default Scan rsp data]
// GAP - SCAN RSP data (max size = 31 bytes)
static uint8_t scanRspData[] =
{
  // complete name
  0x08,   // length of this data
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,
  0x53,   // 'S'
  0x4E,   // 'N'
  0x50,   // 'P'
  
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

//! [Default Adv. data]
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
  
  // Local name
  0x04,
  GAP_ADTYPE_LOCAL_NAME_COMPLETE,
  0x53, //'S'
  0x4E, //'N'
  0x50, //'P'
};
//! [Default Adv. data]

//
//// GAP GATT Attributes
typedef struct snp_advData
{
  uint8_t length;
  uint8_t *pData;
} snp_advData_t;

// The following table contains pointer to adv. data buffer.
// adv data buffer must be created from Heap only, so they can be release 
// when a update of the pointer is needed.
snp_advData_t advPtrTable[3] = 
{
  {0, NULL},   // reserve for scan rsp 
  {0, NULL},   // reserve for adv data  (while not in a connection)
  {0, NULL},   // reserve for adv data  (while in a connection)
};
  
/*********************************************************************
 * EXTERN FUNCTIONS
 */

extern void SNP_resetGATT(uint16_t handle);
extern void exceptionHandler(void);

/*********************************************************************
 * LOCAL FUNCTIONS
 */
void SNP_paramUpdateCB(uint16_t connInterval, uint16_t connSlaveLatency,
                        uint16_t connTimeout);

/*********************************************************************
 * PROFILE CALLBACKS
 */
// GAP Role Callbacks
static  gapRolesCBs_t SNP_gapRoleCBs =
{
  SNP_stateChangeCB,                   // Profile State Change Callbacks
};

static gapRolesParamUpdateCB_t paramUpdateCB =
{
  SNP_paramUpdateCB
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */
/*********************************************************************

 * @fn      SNP_initGAP
 *
 * @brief   Initial parameter needed for using SPNP GATT functionality
 *
 * @return  None.
 */
void SNP_initGAP(void)
{

  // Setup the GAP
  GAP_SetParamValue(TGAP_CONN_PAUSE_PERIPHERAL, DEFAULT_CONN_PAUSE_PERIPHERAL);

  // Setup the GAP Peripheral Role Profile
  {
    // For all hardware platforms, device starts advertising upon initialization
    uint8_t initialAdvertEnable = FALSE;

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
    
    GAPRole_SetParameter(GAPROLE_ADVERT_DATA, sizeof(advertData),
                         advertData);
    
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
  // Initialize the Advertising data store table with a valid entry 
  // for the non-connectable mode.
  {
    uint8_t *pDataPtr;
    advPtrTable[SNP_ADV_DATA_NON_CONN_IDX].pData = pDataPtr = ICall_malloc(sizeof(advertData));
    if(pDataPtr)
    {
      advPtrTable[SNP_ADV_DATA_NON_CONN_IDX].length = sizeof(advertData);
      memcpy(pDataPtr, advertData, sizeof(advertData));
    }
    else
    {
      exceptionHandler();
    }
  }

  
  
  // Set advertising interval
  {
    uint16_t advInt = DEFAULT_ADVERTISING_INTERVAL;

    GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MIN, advInt);
    GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MAX, advInt);
    GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, advInt);
    GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, advInt);
  }
  
  GAPRole_RegisterAppCBs(&paramUpdateCB);
}

/*********************************************************************
 * @fn      SNP_AdvStateChange
 *
 * @brief   Process a pending GAP Role state change event. This function 
 *          will decide how and when to change Advertisement state and send
 *          event to AP accordingly.
 *
 * @param   newState      - new GAP ROLE state
 * @param   previousState - previous GAP ROLE state
 *
 * @return  None.
 */
void SNP_AdvStateChange(gaprole_States_t newState, 
                        gaprole_States_t previousState)
{
  static uint8_t advCurrentState = false; 
  static uint8_t advDbleStateChange = false;
  
  if(newState == GAPROLE_CONNECTED)
  {
    if(previousState == GAPROLE_CONNECTED_ADV)
    {
      //Adv. has been stop while in a connection.
      uint8_t status = SNP_SUCCESS;
      SNP_eventToHost_send(SNP_ADV_ENDED_EVT, &status, 0, NULL);
      advCurrentState = false;
    }
    else
    {
      uint8_t advertEnabled = false;
      // We just enter a connection
      if(advBehavior &  SNP_ADV_DATA_IN_CONNECTION) 
      {
        // need to disable connect. adv and enable non-conn. adv.
        GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t),
                             &advertEnabled);
        advertEnabled = true;
        GAPRole_SetParameter(GAPROLE_ADV_NONCONN_ENABLED, sizeof(uint8_t),
                             &advertEnabled);
        // Set it to true to be sure no adv event are send to the AP.
        advCurrentState = true;
      }
      else
      {
        // Need to Stop Advertisement and send the event to the application
        // processor
        GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t),
                             &advertEnabled);
        {
          //This is a normal non-conn advertisement start
          uint8_t status = SNP_SUCCESS;
          //Advertisement Ended due to the disconnection
          SNP_eventToHost_send(SNP_ADV_ENDED_EVT, &status, 0, NULL);
        }
        advCurrentState = false;
      }
    }
    // if connected adv data are available, use them
    if(advPtrTable[SNP_ADV_DATA_CONN_IDX].length)
    {
      // Use Adv. Data for connected mode
      GAPRole_SetParameter(GAPROLE_ADVERT_DATA, 
                           advPtrTable[SNP_ADV_DATA_CONN_IDX].length,
                           advPtrTable[SNP_ADV_DATA_CONN_IDX].pData);
    }
  }
  else if(newState == GAPROLE_CONNECTED_ADV)
  {
    // This state can be reach only from the GAPROLE_CONNECTED state
    // Adv has started while in a connection.
    // check if it was on going before.
    if(advCurrentState == true)
    {
      //nothing to do, keep it for clarity, compiler will optimize it.
    }
    else
    {
      //request to start adv. was done from the startAdv API
      uint8_t status = SNP_SUCCESS;
      SNP_eventToHost_send(SNP_ADV_STARTED_EVT, &status, 0, NULL);
    }
    advCurrentState = true;
  }
  else if((newState == GAPROLE_WAITING) || 
          (newState == GAPROLE_WAITING_AFTER_TIMEOUT) || 
          (newState == GAPROLE_ADVERTISING_NONCONN))
  {
    // Check if we came from a connection or not
    // Check current Adv state, if currently 
    if((previousState == GAPROLE_CONNECTED) || 
       (previousState == GAPROLE_CONNECTED_ADV))
    {
      // a connection just terminate.
      // check if adv. need to be started.
      if(advBehavior & SNP_ADV_DATA_AFTER_CONNECTION) 
      {
        uint8_t advertEnabled;
        // check if we were currently advertising
        if(advCurrentState == true)
        {
          // Still advertising, since we came from a connection, non-conn adv 
          // was used.
          if(advReqAdvType != GAP_ADTYPE_ADV_NONCONN_IND)
          {
            // the following double action will trigger two state changes: 
            // to GAPROLE_WAITING and then to GAPROLE_ADVERTISING
            // In order to avoid false interpretation and sending wrong event
            // a static variable is used to keep this action in mind.
            advDbleStateChange = true;
            advertEnabled = false;
            GAPRole_SetParameter(GAPROLE_ADV_NONCONN_ENABLED, sizeof(uint8_t),
                                 &advertEnabled);
            advertEnabled = true;
            GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t),
                                 &advertEnabled);
          }
          else
          {
            // Nothing to be done, continue advertising with non-connect.
            // keep it for clarity, compiler will optimize it.
          }
        }
        else
        {
          advertEnabled = true;
          // Need to Switch  Adv.  ONM.
          if(advReqAdvType == GAP_ADTYPE_ADV_NONCONN_IND)
          {
            GAPRole_SetParameter(GAPROLE_ADV_NONCONN_ENABLED, sizeof(uint8_t),
                                 &advertEnabled);
          }
          else
          {
            GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t),
                                 &advertEnabled);
          }
        }
      } 
      else
      {
        // No advertisement after a connection is terminated.
        // need to disable non-connect. adv and enable conn. adv.
        // Force both connectable and non-coonectable adv to stop.
        uint8_t advertEnabled = false;
        GAPRole_SetParameter(GAPROLE_ADV_NONCONN_ENABLED, sizeof(uint8_t),
                             &advertEnabled);
        GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t),
                             &advertEnabled);
      }
      if(advPtrTable[SNP_ADV_DATA_CONN_IDX].length)
      {
        // SNP was using adv data for connected mode, switch to use adv 
        // Data for non-connected mode
        GAPRole_SetParameter(GAPROLE_ADVERT_DATA, 
                             advPtrTable[SNP_ADV_DATA_NON_CONN_IDX].length,
                             advPtrTable[SNP_ADV_DATA_NON_CONN_IDX].pData);
      }
    }
    else
    {
      // we were not in a connection, meaning we came here from an advertisement
      // state, or from waiting state.
      if(newState == GAPROLE_ADVERTISING_NONCONN)
      {
        if(advDbleStateChange == false)
        {
          //Advertisement has started.
          uint8_t status = SNP_SUCCESS;
          SNP_eventToHost_send(SNP_ADV_STARTED_EVT, &status, 0, NULL);
          advCurrentState = true;
        }
        else
        {
          advDbleStateChange = false;
        }
      }
      else if(advCurrentState == true)
      {
        if(previousState == GAPROLE_ADVERTISING)
        {
          // Advertisement has stopped, need to send event
          uint8_t status = SNP_SUCCESS;
          SNP_eventToHost_send(SNP_ADV_ENDED_EVT, &status, 0, NULL);
          advCurrentState = false;
          advDbleStateChange = false;
        }
        else if(advDbleStateChange == false)
        {
          // Advertisement has stopped, need to send event
          uint8_t status = SNP_SUCCESS;
          SNP_eventToHost_send(SNP_ADV_ENDED_EVT, &status, 0, NULL);
          advCurrentState = false;
        }
        else
        {
          // We are in a double state change, do nothing.
        }
      }
    }
  }
  else if(newState == GAPROLE_ADVERTISING)
  {
    // Advertising as started
    if(advCurrentState == false)
    {
      uint8_t status = SNP_SUCCESS;
      SNP_eventToHost_send(SNP_ADV_STARTED_EVT, &status, 0, NULL);
    }
    advCurrentState = true;
  }  
}

/*********************************************************************
 * @fn      SNP_processStateChangeEvt
 *
 * @brief   Process a pending GAP Role state change event.
 *
 * @param   newState - new state
 *
 * @return  None.
 */
void SNP_processStateChangeEvt(gaprole_States_t newState)
{
  static bool firstConnFlag = false;
  static gaprole_States_t currentState = GAPROLE_STARTED;
  static uint16_t currentConnectHandle;
  
 
  switch (newState)
  {
    case GAPROLE_STARTED:
      GAPRole_GetParameter(GAPROLE_CONNHANDLE, &currentConnectHandle);
      break;

    case GAPROLE_ADVERTISING:
      break;

    case GAPROLE_ADVERTISING_NONCONN:
    /* After a connection is dropped a device will continue
     * sending non-connectable advertisements and shall sending this change of 
     * state to the application.  These are then disabled here so that sending 
     * connectable advertisements can resume.
     */
      {
        if(firstConnFlag)
        {
          // We were in a connection.
          uint8_t param[3];

          param[0] = LO_UINT16(currentConnectHandle);
          param[1] = HI_UINT16(currentConnectHandle);
          GAPRole_GetParameter(GAPROLE_CONN_TERM_REASON, &param[2]);

          //reset the GATT state for this connection handle
          SNP_resetGATT(*((uint16_t*) &param[0])); 
          //Connection Ended
          SNP_eventToHost_send(SNP_CONN_TERM_EVT, NULL, sizeof(param), param);
        }
        // Reset flag for next connection.
        firstConnFlag = false;
      }
      break;
    case GAPROLE_CONNECTED:
      if(firstConnFlag == false)
      {
        uint8_t peerAddress[B_ADDR_LEN];

        GAPRole_GetParameter(GAPROLE_CONN_BD_ADDR, peerAddress);
        GAPRole_GetParameter(GAPROLE_CONNHANDLE, &currentConnectHandle);
        {
          uint8_t param[15];
          GAPRole_GetParameter(GAPROLE_CONNHANDLE, &param[0]);
          GAPRole_GetParameter(GAPROLE_CONN_INTERVAL, &param[2]);
          GAPRole_GetParameter(GAPROLE_CONN_LATENCY, &param[4]);
          GAPRole_GetParameter(GAPROLE_CONN_TIMEOUT, &param[6]);
          GAPRole_GetParameter(GAPROLE_BD_ADDR_TYPE, &param[8]);
          memcpy(&param[9], peerAddress, 6);
          //Advertisement Ended due to the connection
          SNP_eventToHost_send(SNP_CONN_EST_EVT, 
                                NULL, sizeof(param), param);
        }

        // if 4.1 feature are enable on the controller, 
        // then the adv needs to be forced to 
        // be non-connectable, since peripheral.c does not support multiple 
        // connection.
        // Only turn advertising on for this state when we first connect
        // otherwise, when we go from connected_advertising back to this 
        // state we will be turning advertising back on.
        firstConnFlag = true;                   
      }
      break;

    case GAPROLE_CONNECTED_ADV:
      break;

    case GAPROLE_WAITING:
      {
        if(firstConnFlag)
        {
          // We were in a connection.
          uint8_t param[3];
          
          param[0] = LO_UINT16(currentConnectHandle);
          param[1] = HI_UINT16(currentConnectHandle);
          GAPRole_GetParameter(GAPROLE_CONN_TERM_REASON, &param[2]);

          //reset the GATT state for this connection handle
          SNP_resetGATT(*((uint16_t*)&param[0])); 
          //Connection Ended
          SNP_eventToHost_send(SNP_CONN_TERM_EVT, NULL, sizeof(param), param);
         
          // Reset flag for next connection.
          firstConnFlag = false;
        }        
      }
      break;

    case GAPROLE_WAITING_AFTER_TIMEOUT:
      if(firstConnFlag)
      {
          uint8_t param[3];
          param[0] = LO_UINT16(currentConnectHandle);
          param[1] = HI_UINT16(currentConnectHandle);
          GAPRole_GetParameter(GAPROLE_CONN_TERM_REASON, &param[2]);

          //reset the GATT state for this connection handle
          SNP_resetGATT(*((uint16_t*) &param[0])); 
          
          //Connection Ended
          SNP_eventToHost_send(SNP_CONN_TERM_EVT, 
                                NULL, sizeof(param), param);
      } 
      // Reset flag for next connection.
      firstConnFlag = false;
     break;

    case GAPROLE_ERROR:
      break;

    default:
      break;
  }
  SNP_AdvStateChange(newState, currentState);
  currentState = newState;
}

/**
 *  SNP_startAdv
 *
 */
uint8_t SNP_startAdv(snpStartAdvReq_t *pReq)
{
  uint8_t status = SNP_SUCCESS;
  uint8_t value;
  VOID GAPRole_StartDevice(&SNP_gapRoleCBs);
  
  // Duration of the advertisement
  // In order to simplify design, set the timeout and interval for both
  // limited and general mode. The mode is define by the Adflags advertisement 
  // token. 
  // User can still set them individually by using the 'setGapParam' command.
  if(pReq->timeout)
  {
    // Save the timeout in second for the limited mode.
    uint16_t temp = pReq->timeout/1000;  
    status |= GAPRole_SetParameter(TGAP_LIM_ADV_TIMEOUT, sizeof(uint16_t),
                                    &temp);
    status |= GAPRole_SetParameter(TGAP_GEN_DISC_ADV_MIN, sizeof(uint16_t),
                                    &pReq->timeout);
  }
  // Set advertising interval
  if(pReq->interval)
  {
    status |= GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MIN, pReq->interval);
    status |= GAP_SetParamValue(TGAP_LIM_DISC_ADV_INT_MAX, pReq->interval);
    status |= GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MIN, pReq->interval);
    status |= GAP_SetParamValue(TGAP_GEN_DISC_ADV_INT_MAX, pReq->interval);
  }

  //Set Advertise Type
  status |= GAPRole_SetParameter(GAPROLE_ADV_EVENT_TYPE, sizeof(uint8_t),
                                 &pReq->type);
  
  if(status)
  {
    //Error management
    return SNP_OUT_OF_RESOURCES ;
  }
    
  //Store behavior of the advertisement.
  advBehavior = (pReq->behavior) & 0x3;

  advReqAdvType = pReq->type; 
  //Enable Advertising.
  if(GAP_ADTYPE_ADV_NONCONN_IND == pReq->type)
  {
    uint8_t start = TRUE;
    GAPRole_GetParameter(GAPROLE_STATE, &value);
    if(value == GAPROLE_CONNECTED)
    {
      if(advPtrTable[SNP_ADV_DATA_CONN_IDX].length)
      {
        // SNP was using adv data for connected mode, switch to use adv 
        // Data for non-connected mode
        GAPRole_SetParameter(GAPROLE_ADVERT_DATA, 
                             advPtrTable[SNP_ADV_DATA_NON_CONN_IDX].length,
                             advPtrTable[SNP_ADV_DATA_NON_CONN_IDX].pData);
      }
    }
    status = GAPRole_SetParameter(GAPROLE_ADV_NONCONN_ENABLED, sizeof(uint8_t),
                         &start);
  }
  else
  {
    uint8_t start = TRUE;
    GAPRole_GetParameter(GAPROLE_STATE, &value);
    if(value == GAPROLE_CONNECTED)
    {
      return SNP_CMD_REJECTED;
    }
    status = GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t),
                         &start);
  }
  return status;
}


/**
 *  @fn      SNP_stopAdv
 *
 */
uint8_t SNP_stopAdv(void)
{
  //Disable Advertising.
  uint8_t start = FALSE;
  GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t),
                       &start);
  GAPRole_SetParameter(GAPROLE_ADV_NONCONN_ENABLED, sizeof(uint8_t),
                       &start);
  return SNP_SUCCESS;
}

/**
 *  SNP_setAdvData
 *
 */
uint8_t SNP_setAdvData(snpSetAdvDataReq_t *pReq, uint8_t len)
{
  uint8_t status = 0;
  uint8_t *pDataPtr;
  
  //Device must be started, or the set adv command will failed.
  VOID GAPRole_StartDevice(&SNP_gapRoleCBs);  
  
  if(pReq->type < SNP_ADV_DATA_MAX_IDX)
  {
    pDataPtr = advPtrTable[pReq->type].pData;
    if(pDataPtr)
    {
      ICall_free(advPtrTable[pReq->type].pData);
    }
    advPtrTable[pReq->type].pData = pDataPtr = ICall_malloc(len);
    if(pDataPtr)
    {
      advPtrTable[pReq->type].length = len;
      memcpy(pDataPtr, pReq->pData, len);
      if(pReq->type == SNP_ADV_DATA_SCAN_RSP_IDX)
      {
        status = GAPRole_SetParameter(GAPROLE_SCAN_RSP_DATA, len, pDataPtr);
      }
      else if(pReq->type == SNP_ADV_DATA_NON_CONN_IDX)
      {
        status = GAPRole_SetParameter(GAPROLE_ADVERT_DATA, len, pDataPtr);
      }
      else if(pReq->type == SNP_ADV_DATA_CONN_IDX)
      {
        uint8_t value;
        GAPRole_GetParameter(GAPROLE_STATE, &value);
        if(value == GAPROLE_CONNECTED_ADV)
        {
          status = GAPRole_SetParameter(GAPROLE_ADVERT_DATA, len, pDataPtr);        
        }      
      }
    }
    else
    {
      status = SNP_OUT_OF_RESOURCES;
    }      
  }
  else
  {
    //Error, bad type
    status = SNP_INVALID_PARAMS;
  }  
  return status;
}

/**
 *  SNP_updateConnParam
 *
 */
uint8_t SNP_updateConnParam(snpUpdateConnParamReq_t *pReq)
{
    uint8_t enableUpdateRequest = TRUE;
    uint8_t status;
    uint8_t value;
  
    status = GAPRole_SetParameter(GAPROLE_PARAM_UPDATE_ENABLE, sizeof(uint8_t),
                         &enableUpdateRequest);
    status |= GAPRole_SetParameter(GAPROLE_MIN_CONN_INTERVAL, sizeof(uint16_t),
                         &pReq->intervalMin);
    status |= GAPRole_SetParameter(GAPROLE_MAX_CONN_INTERVAL, sizeof(uint16_t),
                         &pReq->intervalMax);
    status |= GAPRole_SetParameter(GAPROLE_SLAVE_LATENCY, sizeof(uint16_t),
                         &pReq->slaveLatency);
    status |= GAPRole_SetParameter(GAPROLE_TIMEOUT_MULTIPLIER, sizeof(uint16_t),
                         &pReq->supervisionTimeout);
  
    if(status == bleInvalidRange)
    {
      //Error management
      return SNP_INVALID_PARAMS;
    }
    else if(status) 
    {
      // Mix of errors
      return SNP_FAILURE;
    }
    
    //check Status, only force update if in a connection.
    GAPRole_GetParameter(GAPROLE_STATE, &value);
    if((value == GAPROLE_CONNECTED) || (value == GAPROLE_CONNECTED_ADV))
    {
      if (GAPRole_SetParameter( GAPROLE_PARAM_UPDATE_REQ,
                                      sizeof(uint8_t),
                                      &enableUpdateRequest))
      {
        status = SNP_INVALID_PARAMS;
      }
    }
    else
    {
      status = SNP_NOT_CONNECTED;
    }
    return status;
}

/**
 *  SNP_terminateConn
 *
 */
uint8_t SNP_terminateConn(snpTermConnReq_t *pReq)
{
    uint8_t status = SNP_SUCCESS;
  
    if(pReq->option == SNP_GAP_TERM_CONN_IMMEDIATLY)
    {
      uint16_t value;
      GAPRole_GetParameter(GAPROLE_CONNHANDLE, &value);
      if(pReq->connHandle != value)
      {
        return SNP_INVALID_PARAMS;
      }
      else
      {
        HCI_EXT_DisconnectImmedCmd(value);
      }
    }
    else if(pReq->option == SNP_GAP_TERM_CONN_DEFAULT)
    {
      status = GAPRole_TerminateConnection();
    }
    else
    {
      return SNP_INVALID_PARAMS;
    }
    return status;
}

/**
 *  SNP_setGapParam
 *
 */
uint8_t SNP_setGapParam(snpSetGapParamReq_t *pReq)
{
    uint8_t status;
 
    if((pReq->paramId != TGAP_AUTH_TASK_ID) && 
       (pReq->paramId < TGAP_PARAMID_MAX))
    {
      status = GAP_SetParamValue(pReq->paramId, pReq->value);
    }
    else
    {
      status = SNP_INVALID_PARAMS;
    }
    return status;
}

/**
 *  SNP_getGapParam
 *
 */
uint8_t SNP_getGapParam(snpGetGapParamReq_t* pReq)
{  
    uint8_t status;
    //Move part of the simple peripheral init back here
    if((pReq->paramId != TGAP_AUTH_TASK_ID) && 
        (pReq->value < TGAP_PARAMID_MAX))
    {
      uint16_t value;
      value = GAP_GetParamValue(pReq->paramId);
      pReq->value = BUILD_UINT16( LO_UINT16(value),  HI_UINT16(value));
    }

    if(pReq->value != 0xFFFF)
    {
      status = SNP_SUCCESS;
    }
    else
    {
      status = SNP_INVALID_PARAMS;
    }
    return status;
}

/**
 *  @fn      SNP_paramUpdateCB
 *
 *  @brief   callback to be notify a update of the connection parameters 
 *           has happened.
 *
 *  @param[in]   connInterval     : interval of the connection
 *  @param[in]  connSlaveLatency  : slave latency of the connection
 *  @param[in]  connTimeout       : supervision timeout of the connection
 *
 *  @return  status of the command.
 */
void SNP_paramUpdateCB(uint16_t connInterval,
                       uint16_t connSlaveLatency,
                       uint16_t connTimeout)
{
  uint16_t param[4];
  uint16_t temp;
  GAPRole_GetParameter(GAPROLE_CONNHANDLE, &temp);
  
  param[0] = BUILD_UINT16(LO_UINT16(temp), HI_UINT16(temp)); 
  param[1] = BUILD_UINT16(LO_UINT16(connInterval), HI_UINT16(connInterval)); 
  param[2] = BUILD_UINT16(LO_UINT16(connSlaveLatency), 
                          HI_UINT16(connSlaveLatency));
  param[3] = BUILD_UINT16(LO_UINT16(connTimeout), HI_UINT16(connTimeout));
  SNP_eventToHost_send(SNP_CONN_PARAM_UPDATED_EVT, NULL, sizeof(param),
                       (uint8_t*)param);  
}

/*********************************************************************
*********************************************************************/
