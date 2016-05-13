/*******************************************************************************
  Filename:       runningservice.c
  Revised:        $Date: 2015-07-20 11:31:07 -0700 (Mon, 20 Jul 2015) $
  Revision:       $Revision: 44370 $

  Description:    This file contains the Running Speed and Cadence (RSC) service
                  for use with the RunningSensor sample application.

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
#include <string.h>
#include "bcomdef.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gatt_profile_uuid.h"
#include "runningservice.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

// Running Service Task Events
#define RSC_CMD_IND_SEND_EVT   0x0001

#define RSC_MEAS_VALUE_POS     2
#define RSC_MEAS_CFG_POS       3
#define RSC_COMMAND_VALUE_POS  9
#define RSC_COMMAND_CFG_POS    10
#define COMMAND_IND_LENGTH     2

#define RSC_CMD_LEN            (3 + RSC_MAX_SENSOR_LOCS)

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// RSC service
CONST uint8_t runningServUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(RSC_SERV_UUID), HI_UINT16(RSC_SERV_UUID)
};

// RSC measurement characteristic
CONST uint8_t runningMeasUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(RSC_MEAS_UUID), HI_UINT16(RSC_MEAS_UUID)
};

// RSC feature characteristic
CONST uint8_t runningFeatureUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(RSC_FEATURE_UUID), HI_UINT16(RSC_FEATURE_UUID)
};

// RSC sensor location characteristic
CONST uint8_t runningSensLocUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(SENSOR_LOC_UUID), HI_UINT16(SENSOR_LOC_UUID)
};

// RSC command characteristic
CONST uint8_t runningCommandUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(SC_CTRL_PT_UUID), HI_UINT16(SC_CTRL_PT_UUID)
};

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

static runningServiceCB_t runningServiceCB = NULL;
static runningServiceCtlPntCB_t runningServiceCtlPntCB = NULL;

static uint8_t supportedSensors = 0;
static bool runningServiceOpInProgress = FALSE;

// Variables used in RSC command processing
static uint16_t connectionHandle;
static attHandleValueInd_t rscCmdInd = { 0 };

/*********************************************************************
 * Profile Attributes - variables
 */

// RSC Service attribute.
static CONST gattAttrType_t runningService = { ATT_BT_UUID_SIZE, runningServUUID };

// Available sensor locations.
static uint8_t supportedSensorLocations[RSC_MAX_SENSOR_LOCS] = { RSC_NO_SENSOR_LOC };

// Running Measurement Characteristic.
// Note characteristic value is not stored here.
static uint8_t runningMeasProps = GATT_PROP_NOTIFY;
static uint8_t runningMeas = 0;
static gattCharCfg_t *runningMeasClientCharCfg;

// Feature Characteristic.
static uint8_t runningFeatureProps = GATT_PROP_READ;
static uint16_t runningFeatures = RSC_NO_SUPPORT;

// Sensor Location Characteristic.
static uint8_t runningSensLocProps = GATT_PROP_READ;
static uint8_t runningSensLoc = RSC_NO_SENSOR_LOC;

// Command Characteristic.
static uint8_t runningCommandProps = GATT_PROP_WRITE | GATT_PROP_INDICATE;
static uint8_t runningCommand = 0;
static gattCharCfg_t *runningCommandClientCharCfg;

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t runningAttrTbl[] =
{
  // RSC Service
  {
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8_t *)&runningService                /* pValue */
  },

    // RSC Measurement Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &runningMeasProps
    },

      // Measurement Value
      {
        { ATT_BT_UUID_SIZE, runningMeasUUID },
        0,
        0,
        &runningMeas
      },

      // Measurement Client Characteristic Configuration
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8_t *) &runningMeasClientCharCfg
      },

    // RSC Feature Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &runningFeatureProps
    },

      // Feature Value
      {
        { ATT_BT_UUID_SIZE, runningFeatureUUID },
        GATT_PERMIT_READ,
        0,
        (uint8_t *)&runningFeatures
      },

    // RSC Sensor Location Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &runningSensLocProps
    },

      // Sensor Location Value
      {
        { ATT_BT_UUID_SIZE, runningSensLocUUID },
        GATT_PERMIT_READ,
        0,
        &runningSensLoc
      },

    // RSC Command Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &runningCommandProps
    },

      // Command Value
      {
        { ATT_BT_UUID_SIZE, runningCommandUUID },
        GATT_PERMIT_WRITE,
        0,
        &runningCommand
      },

      // Command Client Characteristic Configuration
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8_t *)&runningCommandClientCharCfg
      }
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static bStatus_t RunningService_readAttrCB(uint16_t connHandle, 
                                           gattAttribute_t *pAttr, 
                                           uint8_t *pValue, uint16_t *pLen, 
                                           uint16_t offset, uint16_t maxLen,
                                           uint8_t method);
static bStatus_t RunningService_writeAttrCB(uint16_t connHandle, 
                                            gattAttribute_t *pAttr,
                                            uint8_t *pValue, uint16_t len, 
                                            uint16_t offset, uint8_t method);

static bool RunningService_sensorLocSupported(uint8_t sensorLoc);
static void RunningService_processRSCCmd(uint16_t attrHandle, uint8_t *pValue,
                                         uint8_t len);

/*********************************************************************
 * PROFILE CALLBACKS
 */

// RSC Service Callbacks.
CONST gattServiceCBs_t runningCBs =
{
  RunningService_readAttrCB,  // Read callback function pointer
  RunningService_writeAttrCB, // Write callback function pointer
  NULL                        // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      RunningService_sensorLocSupported
 *
 * @brief   check to see if sensor location is supported
 *
 * @param   sensorLoc - location to check for
 *
 * @return  TRUE if supported, FALSE otherwise
 */
static bool RunningService_sensorLocSupported(uint8_t sensorLoc)
{
  uint8_t i;

  for (i = 0; i < supportedSensors; i++)
  {
    if (supportedSensorLocations[i] == sensorLoc)
    {
      return TRUE;
    }
  }

  return FALSE;
}

/*********************************************************************
 * @fn      RunningService_addService
 *
 * @brief   Initializes the RSC service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t RunningService_addService(uint32_t services)
{
  uint8_t status;

  // Allocate Client Characteristic Configuration table
  runningMeasClientCharCfg = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) *
                                                            linkDBNumConns );
  if ( runningMeasClientCharCfg == NULL )
  {
    return ( bleMemAllocError );
  }
  
  // Allocate Client Characteristic Configuration table
  runningCommandClientCharCfg = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) *
                                                               linkDBNumConns );
  if ( runningCommandClientCharCfg == NULL )
  {
    // Free already allocated data
    ICall_free( runningMeasClientCharCfg );
    
    return ( bleMemAllocError );
  }
  
  // Initialize Client Characteristic Configuration attributes.
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, runningMeasClientCharCfg);
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, runningCommandClientCharCfg);

  if (services & RUNNING_SERVICE)
  {
    // Register GATT attribute list and CBs with GATT Server App.
    status = GATTServApp_RegisterService(runningAttrTbl,
                                         GATT_NUM_ATTRS(runningAttrTbl),
                                         GATT_MAX_ENCRYPT_KEY_SIZE,
                                         &runningCBs);
  }
  else
  {
    status = SUCCESS;
  }

  return (status);
}

/*********************************************************************
 * @fn      RunningService_register
 *
 * @brief   Register a callback function with the RSC Service.
 *
 * @param   pfnServiceCB - Callback function for events.
 * @param   pfnctlPntCB  - Callback for control point writes.
 *
 * @return  None.
 */
void RunningService_register(runningServiceCB_t pfnServiceCB,
                             runningServiceCtlPntCB_t pfnCtlPntCB)
{
  runningServiceCB = pfnServiceCB;
  
  runningServiceCtlPntCB = pfnCtlPntCB;
}

/*********************************************************************
 * @fn      RunningService_setParameter
 *
 * @brief   Set a RSC parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len   - length of data to right
 * @param   pValue - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16_t will be cast to
 *          uint16_t pointer).
 *
 * @return  bStatus_t
 */
bStatus_t RunningService_setParameter(uint8_t param, uint8_t len, void *pValue)
{
  bStatus_t ret = SUCCESS;

  switch (param)
  {
    case RSC_SENS_LOC:
      {
        runningSensLoc = *((uint8_t*)pValue);
      }
      break;

    case RSC_FEATURE:
      {
        runningFeatures = *((uint16_t*)pValue);
      }
      break;

    case RSC_AVAIL_SENS_LOCS:
      if (supportedSensors  < RSC_MAX_SENSOR_LOCS)
      {
        supportedSensorLocations[supportedSensors++] = *((uint8_t*)pValue);
      }
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }

  return (ret);
}

/*********************************************************************
 * @fn      RunningService_getParameter
 *
 * @brief   Get a RSC parameter.
 *
 * @param   param - Profile parameter ID
 * @param   pValue - pointer to data to get.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16_t will be cast to
 *          uint16_t pointer).
 *
 * @return  bStatus_t
 */
bStatus_t RunningService_getParameter(uint8_t param, void *pValue)
{
  bStatus_t ret = SUCCESS;

  switch (param)
  {
    case RSC_FEATURE:
      *((uint16_t*)pValue) = runningFeatures;
      break;

    case RSC_SENS_LOC:
      *((uint8_t*)pValue) = runningSensLoc;
      break;

    case RSC_COMMAND:
      *((uint8_t*)pValue) = runningCommand;
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }

  return (ret);
}

/*********************************************************************
 * @fn          RunningService_measNotify
 *
 * @brief       Send a notification containing a RSC measurement.
 *
 * @param       connHandle - connection handle
 * @param       pNoti      - pointer to notification structure
 *
 * @return      Success or Failure
 */
bStatus_t RunningService_measNotify(uint16_t connHandle, attHandleValueNoti_t *pNoti)
{
  uint16_t value = GATTServApp_ReadCharCfg(connHandle, runningMeasClientCharCfg);

  // If notifications enabled
  if (value & GATT_CLIENT_CFG_NOTIFY)
  {
    // Set the handle.
    pNoti->handle = runningAttrTbl[RSC_MEAS_VALUE_POS].handle;

    // Send the notification.
    return GATT_Notification(connHandle, pNoti, FALSE);
  }

  return bleIncorrectMode;
}

/*********************************************************************
 * @fn      RunningService_processRSCCmd
 *
 * @brief   process an incoming RSC command.
 *
 * @param   connHandle - connection handle
 * @param   attrHandle - attribute handle
 * @param   pValue     - pointer to data to be written
 * @param   len        - length of data
 *
 * @return  none
 */
static void RunningService_processRSCCmd(uint16_t attrHandle, uint8_t *pValue,
                                         uint8_t len)
{
  uint8_t rscStatus = RSC_SUCCESS;
  
  // See if need to alloc payload for new indication.
  if (rscCmdInd.pValue == NULL)
  {
    rscCmdInd.pValue = GATT_bm_alloc(connectionHandle, ATT_HANDLE_VALUE_IND, 
                                     RSC_CMD_LEN, NULL);
    if (rscCmdInd.pValue == NULL)
    {
      return; // failed to alloc space!
    }
  }
  
  // Set Control Point Cfg in progress.
  runningServiceOpInProgress = TRUE;

  // Set indication info to be sent out.
  rscCmdInd.handle = attrHandle;

  rscCmdInd.len = 3;
  rscCmdInd.pValue[0] = RSC_COMMAND_RSP;
  rscCmdInd.pValue[1] = pValue[0];

  switch (pValue[0])
  {
    case RSC_SET_CUMM_VAL:
      // If total distance is a feature
      if ((len <= 5) && (runningFeatures & RSC_TOTAL_DIST_SUPP))
      {
        uint32_t totalDistance;

        // Full 32 bits were specified.
        if ((len - 1) == 4)
        {
          totalDistance = BUILD_UINT32(pValue[1], pValue[2], pValue[3], pValue[4]);
        }
        else
        {
          uint8_t i;

          totalDistance = 0;

          // In case only lower bits were specified and upper bits remain zero.
          for(i = 0; i < (len - 1); ++i)
          {
            totalDistance += pValue[i + 1] << (i * 8);
          }
        }

        // Notify app.
        if (runningServiceCB != NULL)
        {
          VOID (*runningServiceCB)(RSC_CMD_SET_CUMM_VAL, &totalDistance);
        }
      }
      else
      {
        // Characteristic not supported.
        rscStatus = RSC_INVALID_PARAMETER;
      }
      break;

    case RSC_START_SENS_CALIB:
      // If sensor calibration is supported
      if ((len == 1) && (runningFeatures & RSC_SENSOR_CALIB_SUPP))
      {
        // Notify app.
        if (runningServiceCB != NULL)
        {
          if ((*runningServiceCB)(RSC_CMD_START_SENS_CALIB, NULL) != SUCCESS)
          {
            // Calibration wasn't started.
            rscStatus = RSC_OPERATION_FAILED;
          }
        }
      }
      else
      {
        // Characteristic not supported.
        rscStatus = RSC_INVALID_PARAMETER;
      }
      break;

    case RSC_UPDATE_SENS_LOC:
      // If multiple sensor locations is supported and that this is a 
      // valid location.
      if ((len == 2)                              &&
          (runningFeatures & RSC_MULTI_SENS_SUPP) &&
          (RunningService_sensorLocSupported(pValue[1]) == TRUE))
      {
        // Update sensor location.
        runningSensLoc = pValue[1];

        // Notify app.
        if (runningServiceCB != NULL)
        {
          VOID (*runningServiceCB)(RSC_CMD_UPDATE_SENS_LOC, NULL);
        }
      }
      else
      {
        // Characteristic not supported.
        rscStatus = RSC_INVALID_PARAMETER;
      }
      break;

    case RSC_REQ_SUPP_SENS_LOC:
      // If multiple sensor locations are supported and list requested
      if ((len == 1) && (runningFeatures & RSC_MULTI_SENS_SUPP))
      {
        rscCmdInd.len += supportedSensors;
        memcpy(&(rscCmdInd.pValue[3]), supportedSensorLocations, supportedSensors);
      }
      else // characteristic not supported.
      {
        // Send an indication with the list.
        rscStatus = RSC_INVALID_PARAMETER;
      }
      break;

     default:
      // Send an indication with opcode not supported response.
      rscStatus = RSC_OPCODE_NOT_SUPPORTED;
      break;
  }

  // Send indication of operation result.
  rscCmdInd.pValue[2] = rscStatus;

  // Call the control point callback.
  if (runningServiceCtlPntCB != NULL)
  {
    runningServiceCtlPntCB();
  }
}

 /*********************************************************************
 * @fn      RunningService_sendRSCCmdIndication
 *
 * @brief   Send an indication in response to a RSC control point write.
 *
 * @param   taskId - the application's task ID.
 *
 * @return  none.
 */
void RunningService_sendRSCCmdIndication(uint8_t taskId)
{
  // Send the indication.
  if (GATT_Indication(connectionHandle, &rscCmdInd, FALSE, taskId) != SUCCESS)
  {
    GATT_bm_free((gattMsg_t *)&rscCmdInd, ATT_HANDLE_VALUE_IND);
  }
  
  // Clear out this indication.
  memset(&rscCmdInd, 0, sizeof(attHandleValueInd_t));
  
  // Set the Control Point Cfg flag to done (false).
  runningServiceOpInProgress = FALSE;
}

/*********************************************************************
 * @fn          RunningService_readAttrCB
 *
 * @brief       Read an attribute.
 *
 * @param       connHandle - connection message was received on
 * @param       pAttr - pointer to attribute
 * @param       pValue - pointer to data to be read
 * @param       pLen - length of data to be read
 * @param       offset - offset of the first octet to be read
 * @param       maxLen - maximum length of data to be read
 * @param       method - type of read message 
 *
 * @return      SUCCESS, blePending or Failure
 */
static bStatus_t RunningService_readAttrCB(uint16_t connHandle, 
                                           gattAttribute_t *pAttr,
                                           uint8_t *pValue, uint16_t *pLen, 
                                           uint16_t offset, uint16_t maxLen, 
                                           uint8_t method)
{
  bStatus_t status = SUCCESS;

  // Make sure it's not a blob operation 
  // (no attributes in the profile are long).
  if (offset > 0)
  {
    return (ATT_ERR_ATTR_NOT_LONG);
  }

  uint16_t uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);

  switch (uuid)
  {
    // Read Sensor Location.
    case SENSOR_LOC_UUID:
    {
      *pLen = 1;
      pValue[0] = *pAttr->pValue;
    }
    break;

    // Read Running Feature List.
    case RSC_FEATURE_UUID:
    {
      *pLen = 2;
      pValue[0] = LO_UINT16(runningFeatures);
      pValue[1] = HI_UINT16(runningFeatures);
    }
    break;

    default:
      status = ATT_ERR_ATTR_NOT_FOUND;
    break;
  }

  // Notify app.
  if (runningServiceCB != NULL)
  {
    VOID (*runningServiceCB)(RSC_READ_ATTR, NULL);
  }

  return (status);
}

/*********************************************************************
 * @fn      RunningService_writeAttrCB
 *
 * @brief   Validate attribute data prior to a write operation
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr - pointer to attribute
 * @param   pValue - pointer to data to be written
 * @param   len - length of data
 * @param   offset - offset of the first octet to be written
 * @param   method - type of write message 
 *
 * @return  SUCCESS, blePending or Failure
 */
static bStatus_t RunningService_writeAttrCB(uint16_t connHandle, 
                                            gattAttribute_t *pAttr, 
                                            uint8_t *pValue, uint16_t len, 
                                            uint16_t offset, uint8_t method)
{
  bStatus_t status = SUCCESS;
  uint16_t uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);

  if (offset > 0)
  {
    return (ATT_ERR_ATTR_NOT_LONG);
  }

  switch (uuid)
  {
    case SC_CTRL_PT_UUID:
      // Make sure Control Point Cfg is not already in progress.
      if (runningServiceOpInProgress == TRUE)
      {
        status = RSC_ERR_PROC_IN_PROGRESS;
      }
      // Make sure Control Point Cfg is configured for Indications.
      else if ((runningCommandClientCharCfg[connHandle].value & GATT_CLIENT_CFG_INDICATE) == FALSE)
      {
        status = RSC_ERR_CCC_IMPROPER_CFG;
      }
      else
      {
        // First save connection handle to send new indication on
        connectionHandle = connHandle;
        
        // Process RSC command.
        RunningService_processRSCCmd(pAttr->handle, pValue, len);
      }
      break;

    // For Measure and Commands CCC.
    case GATT_CLIENT_CHAR_CFG_UUID:
      if (pAttr->handle == runningAttrTbl[RSC_COMMAND_CFG_POS].handle)
      {
        status = GATTServApp_ProcessCCCWriteReq(connHandle, pAttr, pValue, len,
                                                offset, 
                                                GATT_CLIENT_CFG_INDICATE);
        // Notify app.
        if (runningServiceCB != NULL)
        {
          VOID (*runningServiceCB)(RSC_WRITE_ATTR, NULL);
        }
      }
      else if (pAttr->handle == runningAttrTbl[RSC_MEAS_CFG_POS].handle)
      {
        status = GATTServApp_ProcessCCCWriteReq(connHandle, pAttr, pValue, len,
                                                offset, GATT_CLIENT_CFG_NOTIFY);
        if (status == SUCCESS)
        {
          // Notify app.
          if (runningServiceCB != NULL)
          {
            uint16_t charCfg = BUILD_UINT16(pValue[0], pValue[1]);

            VOID (*runningServiceCB)(((charCfg == GATT_CFG_NO_OPERATION) ?
                                        RSC_MEAS_NOTI_DISABLED :
                                        RSC_MEAS_NOTI_ENABLED), NULL);
          }
        }
      }
      break;

    default:
      status = ATT_ERR_ATTR_NOT_FOUND;
      break;
  }

  return (status);
}


/*********************************************************************
*********************************************************************/
