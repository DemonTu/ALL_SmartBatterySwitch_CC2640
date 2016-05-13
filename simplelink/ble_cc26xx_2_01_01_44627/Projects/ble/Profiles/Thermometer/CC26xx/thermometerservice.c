/*******************************************************************************
  Filename:       thermometerservice.c
  Revised:        $Date: 2015-07-20 11:31:07 -0700 (Mon, 20 Jul 2015) $
  Revision:       $Revision: 44370 $

  Description:    This file contains the Thermometer Service for use with the
                  Thermometer sample application.

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
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <string.h>

#include "bcomdef.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gatt_profile_uuid.h"
#include "gattservapp.h"
#include "gapbondmgr.h"
#include "thermometerservice.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// Thermometer service
CONST uint8_t thermometerServUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(THERMOMETER_SERV_UUID), HI_UINT16(THERMOMETER_SERV_UUID)
};

// Thermometer temperature characteristic
CONST uint8_t thermometerTempUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(TEMP_MEAS_UUID), HI_UINT16(TEMP_MEAS_UUID)
};

// Thermometer Site
CONST uint8_t thermometerTypeUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(TEMP_TYPE_UUID), HI_UINT16(TEMP_TYPE_UUID)
};

// Thermometer Immediate Measurement
CONST uint8_t thermometerImeasUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(IMEDIATE_TEMP_UUID), HI_UINT16(IMEDIATE_TEMP_UUID)
};

// Thermometer Measurement Interval
CONST uint8_t thermometerIntervalUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(MEAS_INTERVAL_UUID), HI_UINT16(MEAS_INTERVAL_UUID)
};

// Thermometer Test Commands
CONST uint8_t thermometerIRangeUUID[ATT_BT_UUID_SIZE] =
{ 
  LO_UINT16(GATT_VALID_RANGE_UUID), HI_UINT16(GATT_VALID_RANGE_UUID)
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

static thermometerServiceCB_t thermometerServiceCB;

/*********************************************************************
 * Profile Attributes - variables
 */

// Thermometer Service attribute
static CONST gattAttrType_t thermometerService = { ATT_BT_UUID_SIZE, thermometerServUUID };

// Client Characteristic configuration. Each client has its own instantiation
// of the Client Characteristic Configuration. Reads of the Client Characteristic
// Configuration only shows the configuration for that client and writes only
// affect the configuration of that client.

// Thermometer Temperature Characteristic
static uint8_t thermometerTempProps = GATT_PROP_INDICATE;
static uint8_t thermometerTemp = 0;
static gattCharCfg_t *thermometerTempConfig;

// Site
static uint8_t thermometerTypeProps = GATT_PROP_READ;
static uint8_t thermometerType  = 0;

// Intermediate Measurement
static uint8_t  thermometerImeasProps = GATT_PROP_NOTIFY;
static uint8_t  thermometerImeas=0;
static gattCharCfg_t *thermometerIMeasConfig;

// Measurement Interval
static uint8_t  thermometerIntervalProps = GATT_PROP_INDICATE|GATT_PROP_READ|GATT_PROP_WRITE;
static uint16_t  thermometerInterval = 30;  //default
static gattCharCfg_t *thermometerIntervalConfig;

// Measurement Interval Range
static thermometerIRange_t  thermometerIRange = {1,60};

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t thermometerAttrTbl[] = 
{
  // Thermometer Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8_t *)&thermometerService            /* pValue */
  },

    // TEMPERATURE MEASUREMENT
    
    // 1. Characteristic Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &thermometerTempProps 
    },

    // 2. Characteristic Value
    { 
      { ATT_BT_UUID_SIZE, thermometerTempUUID },
      0, 
      0, 
      &thermometerTemp 
    },

    // 3. Characteristic Configuration 
    { 
      { ATT_BT_UUID_SIZE, clientCharCfgUUID },
      GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
      0, 
      (uint8_t *)&thermometerTempConfig
    },
 
    // MEASUREMENT TYPE
   
    // 4. Characteristic Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &thermometerTypeProps 
    },

    // 5. Characteristic Value
    { 
      { ATT_BT_UUID_SIZE, thermometerTypeUUID },
      GATT_PERMIT_READ, 
      0, 
      &thermometerType 
    },

    // IMMEDIATE MEASUREMENT
    
    // 6. Characteristic Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &thermometerImeasProps 
    },

    // 7. Characteristic Value 
    { 
      { ATT_BT_UUID_SIZE, thermometerImeasUUID },
      0, 
      0, 
      &thermometerImeas 
    },

    // 8. Characteristic Configuration
    { 
      { ATT_BT_UUID_SIZE, clientCharCfgUUID },
      GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
      0, 
      (uint8_t *)&thermometerIMeasConfig
    },

    // INTERVAL
    
    // 9. Characteristic Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &thermometerIntervalProps 
    },

    // 10. Characteristic Value
    { 
      { ATT_BT_UUID_SIZE, thermometerIntervalUUID },
      GATT_PERMIT_READ | GATT_PERMIT_AUTHEN_WRITE,
      0, 
     (uint8_t *)&thermometerInterval 
    },
    
    // 11. Characteristic Configuration
    { 
      { ATT_BT_UUID_SIZE, clientCharCfgUUID },
      GATT_PERMIT_READ | GATT_PERMIT_WRITE, 
      0, 
      (uint8_t *)&thermometerIntervalConfig
    }, 
    
    // 12. Interval Range
    { 
      { ATT_BT_UUID_SIZE, thermometerIRangeUUID },
      GATT_PERMIT_READ,
      0, 
      (uint8_t *)&thermometerIRange 
    },
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static bStatus_t Thermometer_ReadAttrCB(uint16_t connHandle, 
                                        gattAttribute_t *pAttr, uint8_t *pValue,
                                        uint16_t *pLen, uint16_t offset,
                                        uint16_t maxLen, uint8_t method);
static bStatus_t Thermometer_WriteAttrCB(uint16_t connHandle, 
                                         gattAttribute_t *pAttr,uint8_t *pValue,
                                         uint16_t len, uint16_t offset,
                                         uint8_t method);

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Thermometer Service Callbacks
CONST gattServiceCBs_t thermometerCBs =
{
  Thermometer_ReadAttrCB,  // Read callback function pointer
  Thermometer_WriteAttrCB, // Write callback function pointer
  NULL                     // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      Thermometer_AddService
 *
 * @brief   Initializes the Thermometer   service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t Thermometer_AddService(uint32 services)
{
  uint8_t status;

  // Allocate Client Characteristic Configuration table
  thermometerTempConfig = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) *
                                                        linkDBNumConns);
  if (thermometerTempConfig == NULL)
  {
    return (bleMemAllocError);
  }
  
  // Allocate Client Characteristic Configuration table
  thermometerIMeasConfig = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) *
                                                         linkDBNumConns);
  if (thermometerIMeasConfig == NULL)
  {
    // Free already allocated data
    ICall_free(thermometerTempConfig);
    
    return (bleMemAllocError);
  }
  
  // Allocate Client Characteristic Configuration table
  thermometerIntervalConfig = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) *
                                                            linkDBNumConns );
  if (thermometerIntervalConfig == NULL)
  {
    // Free already allocated data
    ICall_free(thermometerTempConfig);
    ICall_free(thermometerIMeasConfig);
    
    return (bleMemAllocError);
  }
  
  // Initialize Client Characteristic Configuration attributes.
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, thermometerTempConfig);
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, thermometerIMeasConfig);
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, thermometerIntervalConfig);
  
  if (services & THERMOMETER_SERVICE)
  {
    // Register GATT attribute list and CBs with GATT Server App.
    status = GATTServApp_RegisterService(thermometerAttrTbl, 
                                         GATT_NUM_ATTRS(thermometerAttrTbl),
                                         GATT_MAX_ENCRYPT_KEY_SIZE,
                                         &thermometerCBs);
  }
  else
  {
    status = SUCCESS;
  }

  return (status);
}

/*********************************************************************
 * @fn      Thermometer_Register
 *
 * @brief   Register a callback function with the Thermometer Service.
 *
 * @param   pfnServiceCB - Callback function.
 *
 * @return  None.
 */
void Thermometer_Register(thermometerServiceCB_t pfnServiceCB)
{
  thermometerServiceCB = pfnServiceCB;
}

/*********************************************************************
 * @fn      Thermometer_SetParameter
 *
 * @brief   Set a thermometer parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to right
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16_t will be cast to 
 *          uint16_t pointer).
 *
 * @return  bStatus_t
 */
bStatus_t Thermometer_SetParameter(uint8_t param, uint8_t len, void *value)
{
  bStatus_t ret = SUCCESS;
  
  switch (param)
  {
    case THERMOMETER_TYPE:
      thermometerType = *((uint8_t*)value);
      break;
      
    case THERMOMETER_INTERVAL:
      thermometerInterval = *((uint16_t*)value);
      break;      
 
    case THERMOMETER_TEMP_CHAR_CFG:      
      // Need connection handle
      //thermometerTempConfig.value = *((uint8_t*)value);
      break;
      
    case THERMOMETER_IMEAS_CHAR_CFG:      
      // Need connection handle
      //thermometerIMeasConfig.value = *((uint8_t*)value);
      break; 

    case THERMOMETER_INTERVAL_CHAR_CFG:  
      // Need connection handle    
      //thermometerIntervalConfig.value = *((uint8_t*)value);
      break;       
      
    case THERMOMETER_IRANGE:
      memcpy(&thermometerIRange, value, sizeof(thermometerIRange_t));
      break;       
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return (ret);
}

/*********************************************************************
 * @fn      Thermometer_GetParameter
 *
 * @brief   Get a Thermometer   parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to get.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16_t will be cast to 
 *          uint16_t pointer).
 *
 * @return  bStatus_t
 */
bStatus_t Thermometer_GetParameter(uint8_t param, void *value)
{
  bStatus_t ret = SUCCESS;

  switch (param)
  {
    case THERMOMETER_TYPE:
      *((uint8_t*)value) = thermometerType;
      break;

    case THERMOMETER_INTERVAL:
      *((uint16_t*)value) = thermometerInterval;
      break;
    
    case THERMOMETER_IRANGE:
      memcpy(&value, &thermometerIRange, sizeof(thermometerIRange_t));
      break;
      
    case THERMOMETER_TEMP_CHAR_CFG:
      // Need connection handle
      //*((uint16_t*)value) = thermometerTempConfig.value;
      break;
        
    case THERMOMETER_IMEAS_CHAR_CFG:
      // Need connection handle
      //*((uint16_t*)value) = thermometerIMeasConfig.value;
      break;        
        
    case THERMOMETER_INTERVAL_CHAR_CFG:
      // Need connection handle
      //*((uint16_t*)value) = thermometerIntervalConfig.value;
      break;        
    
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return (ret);
}

/*********************************************************************
 * @fn          Thermometer_TempIndicate
 *
 * @brief       Send a indication containing a thermometer
 *              measurement.
 *
 * @param       connHandle - connection handle
 * @param       pNoti - pointer to notification structure
 * @param       taskId - application's taskId
 *
 * @return      Success or Failure
 */
bStatus_t Thermometer_TempIndicate(uint16_t connHandle, 
                                   attHandleValueInd_t *pNoti,
                                   uint8_t taskId)
{
  uint16_t value = GATTServApp_ReadCharCfg(connHandle, thermometerTempConfig);

  // If indications enabled
  if (value & GATT_CLIENT_CFG_INDICATE)
  {
    // Set the handle (uses stored relative handle to lookup actual handle).
    pNoti->handle = thermometerAttrTbl[pNoti->handle].handle;
  
    // Send the Indication.
    return GATT_Indication(connHandle, pNoti, FALSE, taskId);
  }

  return bleIncorrectMode;
}

/*********************************************************************
 * @fn          Thermometer_IntervalIndicate
 *
 * @brief       Send a interval change indication
 *              
 * @param       connHandle - connection handle
 * @param       pNoti - pointer to notification structure
 * @param       taskId - application's taskId
 *
 * @return      Success or Failure
 */
bStatus_t Thermometer_IntervalIndicate(uint16_t connHandle, 
                                       attHandleValueInd_t *pNoti,
                                       uint8_t taskId)
{
  uint16_t value = GATTServApp_ReadCharCfg(connHandle, thermometerIntervalConfig);

  // If indications enabled
  if (value & GATT_CLIENT_CFG_INDICATE)
  {
    // Set the handle (uses stored relative handle to lookup actual handle).
    pNoti->handle = thermometerAttrTbl[pNoti->handle].handle;
  
    // Send the Indication.
    return GATT_Indication(connHandle, pNoti, FALSE, taskId);
  }

  return bleIncorrectMode;
}

/*********************************************************************
 * @fn          Thermometer_IMeasNotify
 *
 * @brief       Send a notification containing a thermometer
 *              measurement.
 *
 * @param       connHandle - connection handle
 * @param       pNoti - pointer to notification structure
 *
 * @return      Success or Failure
 */
bStatus_t Thermometer_IMeasNotify(uint16_t connHandle, 
                                  attHandleValueNoti_t *pNoti)
{
  uint16_t value = GATTServApp_ReadCharCfg(connHandle, thermometerIMeasConfig);

  // If notifications enabled
  if (value & GATT_CLIENT_CFG_NOTIFY)
  {
    // Set the handle.
    pNoti->handle = thermometerAttrTbl[THERMOMETER_IMEAS_VALUE_POS].handle;
  
    // Send the Notification.
    return GATT_Notification(connHandle, pNoti, FALSE);
  }

  return bleIncorrectMode;
}

/*********************************************************************
 * @fn          Thermometer_ReadAttrCB
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
static bStatus_t Thermometer_ReadAttrCB(uint16_t connHandle, 
                                        gattAttribute_t *pAttr, uint8_t *pValue,
                                        uint16_t *pLen, uint16_t offset,
                                        uint16_t maxLen, uint8_t method)
{
  bStatus_t status = SUCCESS;

  // If attribute permissions require authorization to read, return error.
  if (gattPermitAuthorRead(pAttr->permissions))
  {
    // Insufficient authorization.
    return (ATT_ERR_INSUFFICIENT_AUTHOR);
  }
  
  // Make sure it's not a blob operation (no attributes in the profile are long)
  if (offset > 0)
  {
    return (ATT_ERR_ATTR_NOT_LONG);
  }
 
  if (pAttr->type.len == ATT_BT_UUID_SIZE)
  {
    // 16-bit UUID
    uint16_t uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch (uuid)
    {
      case TEMP_TYPE_UUID:
        *pLen = THERMOMETER_TYPE_LEN;
        *pValue = thermometerType;
        break;
        
      case MEAS_INTERVAL_UUID:
        *pLen = THERMOMETER_INTERVAL_LEN;
        pValue[0] = LO_UINT16(thermometerInterval);
        pValue[1] = HI_UINT16(thermometerInterval);
        break;

      case GATT_VALID_RANGE_UUID:
        *pLen = THERMOMETER_IRANGE_LEN;
         pValue[0] = LO_UINT16(thermometerIRange.low);
         pValue[1] = HI_UINT16(thermometerIRange.low);
         pValue[2] = LO_UINT16(thermometerIRange.high);
         pValue[3] = HI_UINT16(thermometerIRange.high);
        break;        
        
      default:
        *pLen = 0;
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  
  return (status);
}

/*********************************************************************
 * @fn      Thermometer_WriteAttrCB
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
static bStatus_t Thermometer_WriteAttrCB(uint16_t connHandle, 
                                         gattAttribute_t *pAttr, 
                                         uint8_t *pValue, uint16_t len, 
                                         uint16_t offset, uint8_t method)
{
  bStatus_t status = SUCCESS;

  uint16_t uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);
  
  switch (uuid)
  {
    case  GATT_CLIENT_CHAR_CFG_UUID:
      // Validate/Write Temperature measurement setting.
      if (pAttr->handle == thermometerAttrTbl[THERMOMETER_TEMP_CHAR_CONFIG_POS].handle)
      {
        status = GATTServApp_ProcessCCCWriteReq(connHandle, pAttr, pValue, len,
                                                offset, GATT_CLIENT_CFG_INDICATE);
        if (status == SUCCESS)
        {
          uint16_t value = BUILD_UINT16(pValue[0], pValue[1]);      

          if (thermometerServiceCB != NULL)
          {
            (*thermometerServiceCB)((value == GATT_CFG_NO_OPERATION) ? 
                                     THERMOMETER_TEMP_IND_DISABLED :
                                     THERMOMETER_TEMP_IND_ENABLED);
          }
        }
      }
      // Validate/Write Intermediate measurement setting.
      else if (pAttr->handle == thermometerAttrTbl[THERMOMETER_IMEAS_CHAR_CONFIG_POS].handle)
      {
        status = GATTServApp_ProcessCCCWriteReq(connHandle, pAttr, pValue, len,
                                                offset, GATT_CLIENT_CFG_NOTIFY);
        if (status == SUCCESS)
        {
          uint16_t value = BUILD_UINT16(pValue[0], pValue[1]);
          
          // Notify the application.
          if (thermometerServiceCB != NULL)
          {
            (*thermometerServiceCB)((value == GATT_CFG_NO_OPERATION) ? 
                                     THERMOMETER_IMEAS_NOTI_DISABLED :
                                     THERMOMETER_IMEAS_NOTI_ENABLED);
          }
        }
      }
      // Validate/Write Interval Client Char Config.
      else if (pAttr->handle == thermometerAttrTbl[THERMOMETER_INTERVAL_CHAR_CONFIG_POS].handle)
      {
        status = GATTServApp_ProcessCCCWriteReq(connHandle, pAttr, pValue, len,
                                                offset, GATT_CLIENT_CFG_INDICATE);
        if (status == SUCCESS)
        {
          uint16_t value = BUILD_UINT16(pValue[0], pValue[1]);
           
          // Notify the application.
          if (thermometerServiceCB != NULL)
          {
            (*thermometerServiceCB)((value == GATT_CFG_NO_OPERATION) ? 
                                     THERMOMETER_INTERVAL_IND_DISABLED :
                                     THERMOMETER_INTERVAL_IND_ENABLED);
          }
        }
        else
        {
          status = ATT_ERR_INVALID_HANDLE;
        }
      }  
      else
      {
          status = ATT_ERR_INVALID_VALUE_SIZE;
      }
      break;
  
    case MEAS_INTERVAL_UUID:
      // Make sure it's not a blob operation.
      if (offset == 0)
      {
        if (len == THERMOMETER_INTERVAL_LEN)
        {
          uint16_t value = BUILD_UINT16(pValue[0], pValue[1]);
          
          // Validate range.
          if ((value > thermometerIRange.high) || 
              ((value < thermometerIRange.low) && (value != 0)))
          {
            status = ATT_ERR_INVALID_VALUE;
          }
        }
        else
        {
          status = ATT_ERR_INVALID_VALUE_SIZE;
        }
      }
      else
      {
        status = ATT_ERR_ATTR_NOT_LONG;
      }
      
      // Write the value.
      if (status == SUCCESS)
      {
        uint16_t *pCurValue = (uint16_t *)pAttr->pValue;        
        
        *pCurValue = BUILD_UINT16(pValue[0], pValue[1]);
        
        // Notify application of write.
        if (thermometerServiceCB != NULL)
        {
          (*thermometerServiceCB)(THERMOMETER_INTERVAL_SET);
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
