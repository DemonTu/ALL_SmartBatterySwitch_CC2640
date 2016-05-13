/*******************************************************************************
  Filename:       cyclingservice.c
  Revised:        $Date: 2015-07-20 11:31:07 -0700 (Mon, 20 Jul 2015) $
  Revision:       $Revision: 44370 $

  Description:    This file contains the Cycling Speed and Cadence (CSC) service
                  for use with the Cycling Sensor sample application for CC2650.

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
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "gatt_profile_uuid.h"
#include "cyclingservice.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

#define CSC_MEAS_VALUE_POS     2
#define CSC_MEAS_CFG_POS       3
#define CSC_COMMAND_VALUE_POS  9
#define CSC_COMMAND_CFG_POS    10
#define COMMAND_IND_LENGTH     2

#define CSC_CMD_LEN            (3 + CSC_MAX_SENSOR_LOCS)

/*********************************************************************
 * GLOBAL VARIABLES
 */

// CSC service.
CONST uint8_t cyclingServUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(CSC_SERV_UUID), HI_UINT16(CSC_SERV_UUID)
};

// CSC measurement characteristic.
CONST uint8_t cyclingMeasUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(CSC_MEAS_UUID), HI_UINT16(CSC_MEAS_UUID)
};

// CSC feature characteristic.
CONST uint8_t cyclingFeatureUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(CSC_FEATURE_UUID), HI_UINT16(CSC_FEATURE_UUID)
};

// CSC sensor location characteristic.
CONST uint8_t cyclingSensLocUUID[ATT_BT_UUID_SIZE] =
{
  LO_UINT16(SENSOR_LOC_UUID), HI_UINT16(SENSOR_LOC_UUID)
};

// CSC command characteristic.
CONST uint8_t cyclingCommandUUID[ATT_BT_UUID_SIZE] =
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

static cyclingServiceCB_t cyclingServiceCB = NULL;
static cyclingServiceCtlPntCB_t cyclingServiceCtlPntCB = NULL;

static uint8_t supportedSensors = 0;
static bool cyclingServiceOpInProgress = FALSE;

// Variables used in CSC command processing.
static uint16 connectionHandle;
static attHandleValueInd_t cscCmdInd = { 0 };

/*********************************************************************
 * Profile Attributes - variables
 */

// CSC Service attribute.
static CONST gattAttrType_t cyclingService = { ATT_BT_UUID_SIZE, cyclingServUUID };

// Available sensor locations.
static uint8_t supportedSensorLocations[CSC_MAX_SENSOR_LOCS];

// Cycling Measurement Characteristic.
// Note: characteristic value is not stored here.
static uint8_t cyclingMeasProps = GATT_PROP_NOTIFY;
static uint8_t cyclingMeas = 0;
static gattCharCfg_t *cyclingMeasClientCharCfg;

// Feature Characteristic.
static uint8_t cyclingFeatureProps = GATT_PROP_READ;
static uint16 cyclingFeatures = CSC_NO_SUPPORT;


// Sensor Location Characteristic.
static uint8_t cyclingSensLocProps = GATT_PROP_READ;
static uint8_t cyclingSensLoc = CSC_SENSOR_LOC_TOP_OF_SHOE;

// Command Characteristic.
static uint8_t cyclingCommandProps = GATT_PROP_WRITE | GATT_PROP_INDICATE;
static uint8_t cyclingCommand = 0;
static gattCharCfg_t *cyclingCommandClientCharCfg;

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t cyclingAttrTbl[] =
{
  // CSC Service
  {
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8_t *)&cyclingService                /* pValue */
  },

    // CSC Measurement Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &cyclingMeasProps
    },

      // Measurement Value
      {
        { ATT_BT_UUID_SIZE, cyclingMeasUUID },
        0,
        0,
        &cyclingMeas
      },

      // Measurement Client Characteristic Configuration
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8_t *) &cyclingMeasClientCharCfg
      },

    // CSC Feature Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &cyclingFeatureProps
    },

      // Feature Value
      {
        { ATT_BT_UUID_SIZE, cyclingFeatureUUID },
        GATT_PERMIT_READ,
        0,
        (uint8_t *) &cyclingFeatures
      },

    // CSC Sensor Location Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &cyclingSensLocProps
    },

      // Sensor Location Value
      {
        { ATT_BT_UUID_SIZE, cyclingSensLocUUID },
        GATT_PERMIT_READ,
        0,
        &cyclingSensLoc
      },

    // CSC Command Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &cyclingCommandProps
    },

      // Command Value
      {
        { ATT_BT_UUID_SIZE, cyclingCommandUUID },
        GATT_PERMIT_WRITE,
        0,
        &cyclingCommand
      },

      // Command Client Characteristic Configuration
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8_t *) &cyclingCommandClientCharCfg
      }
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static bStatus_t CyclingService_readAttrCB(uint16_t connHandle, 
                                           gattAttribute_t *pAttr, 
                                           uint8_t *pValue, uint16_t *pLen,
                                           uint16_t offset, uint16_t maxLen,
                                           uint8_t method);
static bStatus_t CyclingService_writeAttrCB(uint16_t connHandle, 
                                            gattAttribute_t *pAttr,
                                            uint8_t *pValue, uint16_t len, 
                                            uint16_t offset, uint8_t method);

static bool CyclingService_sensorLocSupported(uint8_t sensorLoc);
static void CyclingService_processCSCCmd(uint16 attrHandle, uint8_t *pValue, 
                                         uint8_t len);

/*********************************************************************
 * PROFILE CALLBACKS
 */

// CSC Service Callbacks.
CONST gattServiceCBs_t cyclingCBs =
{
  CyclingService_readAttrCB,  // Read callback function pointer
  CyclingService_writeAttrCB, // Write callback function pointer
  NULL                        // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      CyclingService_sensorLocSupported
 *
 * @brief   check to see if sensor location is supported.
 *
 * @param   sensorLoc - location to check for
 *
 * @return  TRUE if supported, FALSE otherwise
 */
static bool CyclingService_sensorLocSupported(uint8_t sensorLoc)
{
  uint8_t i;
  
  for (i = 0; i <= supportedSensors; i++)
  {
    if (supportedSensorLocations[i] == sensorLoc)
    {
      return TRUE;
    }
  }
  
  return FALSE;
}

/*********************************************************************
 * @fn      CyclingService_addService
 *
 * @brief   Initializes the CSC service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t CyclingService_addService(uint32_t services)
{
  uint8_t status;

  // Allocate Client Characteristic Configuration table
  cyclingMeasClientCharCfg = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) *
                                                            linkDBNumConns );
  if ( cyclingMeasClientCharCfg == NULL )
  {
    return ( bleMemAllocError );
  }
  
  // Allocate Client Characteristic Configuration table
  cyclingCommandClientCharCfg = (gattCharCfg_t *)ICall_malloc( sizeof(gattCharCfg_t) *
                                                               linkDBNumConns );
  if ( cyclingCommandClientCharCfg == NULL )
  {
    // Free already allocated data
    ICall_free( cyclingMeasClientCharCfg );
    
    return ( bleMemAllocError );
  }
  
  // Initialize Client Characteristic Configuration attributes.
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, cyclingMeasClientCharCfg);
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, cyclingCommandClientCharCfg);

  if (services & CYCLING_SERVICE)
  {
    // Register GATT attribute list and CBs with GATT Server App.
    status = GATTServApp_RegisterService(cyclingAttrTbl,
                                         GATT_NUM_ATTRS(cyclingAttrTbl),
                                         GATT_MAX_ENCRYPT_KEY_SIZE,
                                         &cyclingCBs);
  }
  else
  {
    status = SUCCESS;
  }
  
  return (status);
}

/*********************************************************************
 * @fn      CyclingService_register
 *
 * @brief   Register a callback function with the CSC Service.
 *
 * @param   pfnServiceCB - Callback function for events.
 * @param   pfnCtlPntCB  - Callback for control point writes.
 *
 * @return  None.
 */
void CyclingService_register(cyclingServiceCB_t pfnServiceCB, 
                             cyclingServiceCtlPntCB_t pfnCtlPntCB)
{
  cyclingServiceCB = pfnServiceCB;
  
  cyclingServiceCtlPntCB = pfnCtlPntCB;
}


/*********************************************************************
 * @fn      CyclingService_setParameter
 *
 * @brief   Set a CSC parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len   - length of data to right
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t CyclingService_setParameter(uint8_t param, uint8_t len, void *pValue)
{
  bStatus_t ret = SUCCESS;

  switch (param)
  {
    case CSC_SENS_LOC:
      cyclingSensLoc = *((uint8_t*)pValue);
      break;

    case CSC_FEATURE:
      cyclingFeatures = *((uint8_t*)pValue);
      break;

    case CSC_AVAIL_SENS_LOCS:
      if (supportedSensors  < CSC_MAX_SENSOR_LOCS)
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
 * @fn      CyclingService_getParameter
 *
 * @brief   Get a CSC parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to get.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16 will be cast to
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t CyclingService_getParameter(uint8_t param, void *value)
{
  bStatus_t ret = SUCCESS;

  switch (param)
  {
    case CSC_FEATURE:
      *((uint8_t*)value) = cyclingFeatures;
      break;

    case CSC_SENS_LOC:
      *((uint8_t*)value) = cyclingSensLoc;
      break;

    case CSC_COMMAND:
      *((uint8_t*)value) = cyclingCommand;
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }

  return (ret);
}

/*********************************************************************
 * @fn          CyclingService_measNotify
 *
 * @brief       Send a notification containing a CSC
 *              measurement.
 *
 * @param       connHandle - connection handle
 * @param       pNoti      - pointer to notification structure
 *
 * @return      Success or Failure
 */
bStatus_t CyclingService_measNotify(uint16 connHandle, 
                                    attHandleValueNoti_t *pNoti)
{
  uint16 value = GATTServApp_ReadCharCfg(connHandle, cyclingMeasClientCharCfg);

  // If notifications enabled
  if (value & GATT_CLIENT_CFG_NOTIFY)
  {
    // Set the handle.
    pNoti->handle = cyclingAttrTbl[CSC_MEAS_VALUE_POS].handle;

    // Send the notification.
    return GATT_Notification(connHandle, pNoti, FALSE);
  }

  return bleIncorrectMode;
}

 /*********************************************************************
 * @fn      CyclingService_processCSCCmd
 *
 * @brief   process an incoming CSC command.
 *
 * @param   attrHandle - attribute handle
 * @param   pValue     - pointer to data to be written
 * @param   len        - length of data
 *
 * @return  none
 */
static void CyclingService_processCSCCmd(uint16 attrHandle, uint8_t *pValue, 
                                         uint8_t len)
{
  uint8_t cscStatus = CSC_SUCCESS;

  // See if need to alloc payload for new indication.
  if (cscCmdInd.pValue == NULL)
  {
    cscCmdInd.pValue = GATT_bm_alloc(connectionHandle, ATT_HANDLE_VALUE_IND, 
                                     CSC_CMD_LEN, NULL);
    if (cscCmdInd.pValue == NULL)
    {
      return; // failed to allocate space!
    }
  }
  
  // Set Control Point Cfg in progress.
  cyclingServiceOpInProgress = TRUE;

  // Set indication info to be sent out.
  cscCmdInd.handle = attrHandle;

  cscCmdInd.len = 3;
  cscCmdInd.pValue[0] = CSC_COMMAND_RSP;
  cscCmdInd.pValue[1] = pValue[0];

  switch (pValue[0])
  {
    case CSC_SET_CUMM_VAL:
      // If wheel revolutions is a feature.
      if ((len <= 5) && (cyclingFeatures & CSC_WHEEL_REV_SUPP))
      {
        uint32_t cummWheelRevolutions;

        // full 32 bits were specified.
        if ((len - 1) == 4)
        {
          cummWheelRevolutions = BUILD_UINT32(pValue[1], pValue[2], pValue[3], 
                                              pValue[4]);
        }
        else
        {
          uint8_t i;

          cummWheelRevolutions = 0;

          // In case only lower bits were specified and upper bits remain zero.
          for(i = 0; i < (len - 1); ++i)
          {
            cummWheelRevolutions += pValue[i + 1] << (i*8);
          }
        }

        // Notify app.
        if (cyclingServiceCB != NULL)
        {
          (*cyclingServiceCB)(CSC_CMD_SET_CUMM_VAL, &cummWheelRevolutions);
        }
      }
      else
      {
        // Characteristic not supported.
        cscStatus = CSC_INVALID_PARAMETER;
      }
      break;

    case CSC_UPDATE_SENS_LOC:
      // If multiple sensor locations is supported and that this is a 
      // valid location.
      if ((len == 2)                                              &&
          (cyclingFeatures & CSC_MULTI_SENS_SUPP)                 &&
          (CyclingService_sensorLocSupported(pValue[1]) == TRUE))
      {
        // Update sensor location.
        cyclingSensLoc = pValue[1];

        // Notify app.
        if (cyclingServiceCB != NULL)
        {
          (*cyclingServiceCB)(CSC_CMD_UPDATE_SENS_LOC, NULL);
        }
      }
      else
      {
        // Characteristic not supported.
        cscStatus = CSC_INVALID_PARAMETER;
      }
      break;

    case CSC_REQ_SUPP_SENS_LOC:
      // If multiple sensor locations are supported and list requested
      if ((len == 1) && (cyclingFeatures & CSC_MULTI_SENS_SUPP))
      {
        cscCmdInd.len += supportedSensors;
        memcpy(&(cscCmdInd.pValue[3]), supportedSensorLocations, supportedSensors);
      }
      else
      {
        // Characteristic not supported.
        cscStatus = CSC_INVALID_PARAMETER;
      }
      break;

     default:
      // Send an indication with opcode not supported response.
      cscStatus = CSC_OPCODE_NOT_SUPPORTED;
      break;
  }

  // Send indication of operation result.
  cscCmdInd.pValue[2] = cscStatus;
  
  // Call the control point callback.
  if (cyclingServiceCtlPntCB != NULL)
  {
    cyclingServiceCtlPntCB();
  }
}

 /*********************************************************************
 * @fn      CyclingService_sendCSCCmdIndication
 *
 * @brief   Send an indication in response to a CSC control point write.
 *
 * @param   taskId - the application's task ID.
 *
 * @return  none.
 */
void CyclingService_sendCSCCmdIndication(uint8_t taskId)
{
  // Send the indication.
  if (GATT_Indication(connectionHandle, &cscCmdInd, FALSE, taskId) != SUCCESS)
  {
    GATT_bm_free((gattMsg_t *)&cscCmdInd, ATT_HANDLE_VALUE_IND);
  }
  
  // Clear out this indication.
  memset(&cscCmdInd, 0, sizeof(attHandleValueInd_t));
  
  // Set the Control Point Cfg flag to done (false).
  cyclingServiceOpInProgress = FALSE;
}

/*********************************************************************
 * @fn          CyclingService_readAttrCB
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
static bStatus_t CyclingService_readAttrCB(uint16_t connHandle, 
                                           gattAttribute_t *pAttr, 
                                           uint8_t *pValue, uint16_t *pLen, 
                                           uint16_t offset, uint16_t maxLen, 
                                           uint8_t method)
{
  // Make sure it's not a blob operation
  // (no attributes in the profile are long).
  if (offset > 0)
  {
    return (ATT_ERR_ATTR_NOT_LONG);
  }

  bStatus_t status = SUCCESS;
  uint16 uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);

  switch (uuid)
  {
    case SENSOR_LOC_UUID:
    {
      // Read Sensor Location.
      *pLen = 1;
      pValue[0] = pAttr->pValue[0];
    }
    break;

    case CSC_FEATURE_UUID:
    {
      //Read Cycling Feature.
      *pLen = 2;
      pValue[0] = LO_UINT16(pAttr->pValue[0]);
      pValue[1] = HI_UINT16(pAttr->pValue[0]);
    }
    break;

    case GATT_CLIENT_CHAR_CFG_UUID:
    {
      // Read Measurement or Command Configuration.
      if (pAttr->pValue == (uint8_t*)cyclingMeasClientCharCfg)
      {
        *pLen = 1;
         pValue[0] = GATTServApp_ReadCharCfg(connHandle, 
                                             cyclingMeasClientCharCfg);
      }
      else if (pAttr->pValue == (uint8_t*)cyclingCommandClientCharCfg)
      {
        *pLen = 1;
         pValue[0] = GATTServApp_ReadCharCfg(connHandle, 
                                             cyclingCommandClientCharCfg);
      }
      else
      {
        status = ATT_ERR_ATTR_NOT_FOUND;
      }
    }
    break;

    default:
      status = ATT_ERR_ATTR_NOT_FOUND;
    break;
  }

  // Notify app.
  if (cyclingServiceCB != NULL)
  {
    (*cyclingServiceCB)(CSC_READ_ATTR, NULL);
  }

  return (status);
}


/*********************************************************************
 * @fn      CyclingService_writeAttrCB
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
static bStatus_t CyclingService_writeAttrCB(uint16_t connHandle, 
                                            gattAttribute_t *pAttr,
                                            uint8_t *pValue, uint16_t len, 
                                            uint16_t offset, uint8_t method)
{
  bStatus_t status = SUCCESS;
  uint16 uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);

  if (offset > 0)
  {
    return (ATT_ERR_ATTR_NOT_LONG);
  }

  switch (uuid)
  {
    case SC_CTRL_PT_UUID:
      // Make sure Control Point Cfg is not already in progress.
      if (cyclingServiceOpInProgress == TRUE)
      {
        status = CSC_ERR_PROC_IN_PROGRESS;
      }
      // Make sure Control Point Cfg is configured for Indications.
      else if ((cyclingCommandClientCharCfg[connHandle].value & GATT_CLIENT_CFG_INDICATE) == FALSE)
      {
        status = CSC_ERR_CCC_IMPROPER_CFG;
      }
      else
      {
        // First save connection handle to send new indication on.
        connectionHandle = connHandle;
        
        // Process CSC command.
        CyclingService_processCSCCmd(pAttr->handle, pValue, len);
      }
      break;

    // For Measure and Commands CCC.
    case GATT_CLIENT_CHAR_CFG_UUID:
      if (pAttr->handle == cyclingAttrTbl[CSC_COMMAND_CFG_POS].handle)
      {
        status = GATTServApp_ProcessCCCWriteReq(connHandle, pAttr, pValue, len,
                                                offset, 
                                                GATT_CLIENT_CFG_INDICATE);
        // Notify app.
        if (cyclingServiceCB != NULL)
        {
          (*cyclingServiceCB)(CSC_WRITE_ATTR, NULL);
        }
      }
      else if (pAttr->handle == cyclingAttrTbl[CSC_MEAS_CFG_POS].handle)
      {
        status = GATTServApp_ProcessCCCWriteReq(connHandle, pAttr, pValue, len,
                                                offset, GATT_CLIENT_CFG_NOTIFY);
        if (status == SUCCESS)
        {
          // Notify app.
          if (cyclingServiceCB != NULL)
          {
            uint16 charCfg = BUILD_UINT16(pValue[0], pValue[1]);

            (*cyclingServiceCB)(((charCfg == GATT_CFG_NO_OPERATION) ?
                                   CSC_MEAS_NOTI_DISABLED :
                                   CSC_MEAS_NOTI_ENABLED), NULL);
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
