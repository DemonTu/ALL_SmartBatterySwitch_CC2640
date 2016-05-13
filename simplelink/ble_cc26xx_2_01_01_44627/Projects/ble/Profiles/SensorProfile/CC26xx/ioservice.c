/*******************************************************************************
  Filename:       ioservice.c
  Revised:        $Date: 2014-06-17 00:12:16 +0200 (ti, 17 jun 2014) $
  Revision:       $Revision: 39036 $

  Description:    IO Service.


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
*******************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "string.h"

#include "ioservice.h"
#include "st_util.h"


/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// GATT Profile Service UUID
CONST uint8_t ioServUUID[TI_UUID_SIZE] =
{
  TI_UUID(IO_SERV_UUID)
};

// Data Characteristic UUID
CONST uint8_t ioDataUUID[TI_UUID_SIZE] =
{
  TI_UUID(IO_DATA_UUID)
};

// Config Characteristic UUID
CONST uint8_t ioConfUUID[TI_UUID_SIZE] =
{
  TI_UUID(IO_CONF_UUID)
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

static sensorCBs_t *io_AppCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */

// IO Service attribute
static CONST gattAttrType_t ioService = { TI_UUID_SIZE, ioServUUID };

// IO Service Data Characteristic Properties
static uint8_t ioDataProps = GATT_PROP_READ | GATT_PROP_WRITE;

// IO Service Data Characteristic Value
static uint8_t ioData = 0;

// IO Characteristic Configuration
static gattCharCfg_t *ioDataConfig;

#ifdef USER_DESCRIPTION
// IO Service Data Characteristic User Description
static uint8_t ioDataUserDesp[] = "IO Data";
#endif

// IO Service Config Characteristic Properties
static uint8_t ioConfProps = GATT_PROP_READ | GATT_PROP_WRITE;

// IO Service Config Characteristic Value
static uint8_t ioConf = 0x00;

#ifdef USER_DESCRIPTION
// IO Service Config Characteristic User Description
static uint8_t ioConfUserDesp[] = "IO Config";
#endif

/*********************************************************************
 * Profile Attributes - Table
 */
static gattAttribute_t ioAttrTbl[] =
{
  // IO Service Service
  {
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8_t *)&ioService                     /* pValue */
  },

    // Data Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &ioDataProps
    },

      // Data Characteristic Value
      {
        { TI_UUID_SIZE, ioDataUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        &ioData
      },
#ifdef USER_DESCRIPTION
      // Data Characteristic User Description
      {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        ioDataUserDesp
      },
#endif
    // Config Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &ioConfProps
    },

      // Config Characteristic Value
      {
        { TI_UUID_SIZE, ioConfUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        &ioConf
      },
#ifdef USER_DESCRIPTION
      // Config Characteristic User Description
      {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        ioConfUserDesp
      },
#endif
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t io_ReadAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                               uint8_t *pValue, uint16_t *pLen, uint16_t offset,
                               uint16_t maxLen, uint8_t method);
static bStatus_t io_WriteAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                uint8_t *pValue, uint16_t len, uint16_t offset,
                                uint8_t method);

/*********************************************************************
 * PROFILE CALLBACKS
 */

// IO Service Service Callbacks
CONST gattServiceCBs_t ioCBs =
{
  io_ReadAttrCB,  // Read callback function pointer
  io_WriteAttrCB, // Write callback function pointer
  NULL            // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      Io_addService
 *
 * @brief   Initializes the IO Service service by registering
 *          GATT attributes with the GATT server.
 *
 * @return  Success or Failure
 */
bStatus_t Io_addService(void)
{
  // Allocate Client Characteristic Configuration table
  ioDataConfig = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) *
                                                linkDBNumConns);
  if (ioDataConfig == NULL)
  {
    return (bleMemAllocError);
  }
  
  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, ioDataConfig);

  // Register GATT attribute list and CBs with GATT Server App
  return GATTServApp_RegisterService( ioAttrTbl,
                                      GATT_NUM_ATTRS( ioAttrTbl ),
                                      GATT_MAX_ENCRYPT_KEY_SIZE,
                                      &ioCBs );
}

/*********************************************************************
 * @fn      Io_registerAppCBs
 *
 * @brief   Registers the application callback function. Only call
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t Io_registerAppCBs(sensorCBs_t *appCallbacks)
{
  if (appCallbacks != NULL)
  {
    io_AppCBs = appCallbacks;
    
    return (SUCCESS);
  }
  else
  {
    return (bleAlreadyInRequestedMode);
  }
}

/*********************************************************************
 * @fn      Io_setParameter
 *
 * @brief   Set a IO Service parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to write
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16_t will be cast to
 *          uint16_t pointer).
 *
 * @return  bStatus_t
 */
bStatus_t Io_setParameter(uint8_t param, uint8_t len, void *value)
{
  bStatus_t ret = SUCCESS;

  switch (param)
  {
    case SENSOR_DATA:
      if (len == sizeof(uint8_t))
      {
        ioData = *((uint8_t*)value);
        // See if Notification has been enabled
        ret = GATTServApp_ProcessCharCfg( ioDataConfig, &ioData, FALSE,
                                   ioAttrTbl, GATT_NUM_ATTRS( ioAttrTbl ),
                                   INVALID_TASK_ID, io_ReadAttrCB);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case SENSOR_CONF:
      if( len == sizeof(uint8_t))
      {
        ioConf = *((uint8_t*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }

  return (ret);
}

/*********************************************************************
 * @fn      Io_getParameter
 *
 * @brief   Get a IO Service parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16_t will be cast to
 *          uint16_t pointer).
 *
 * @return  bStatus_t
 */
bStatus_t Io_getParameter(uint8_t param, void *value)
{
  bStatus_t ret = SUCCESS;

  switch (param)
  {
    case SENSOR_DATA:
      *((uint8_t*)value) = ioData;
      break;

    case SENSOR_CONF:
      *((uint8_t*)value) = ioConf;
      break;

    default:
      ret = INVALIDPARAMETER;
      break;
  }

  return (ret);
}

/*********************************************************************
 * @fn          io_ReadAttrCB
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
static bStatus_t io_ReadAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                               uint8_t *pValue, uint16_t *pLen, uint16_t offset,
                               uint16_t maxLen, uint8_t method)
{
  uint16_t uuid;
  bStatus_t status = SUCCESS;

  // If attribute permissions require authorization to read, return error
  if (gattPermitAuthorRead(pAttr->permissions))
  {
    // Insufficient authorization
    return (ATT_ERR_INSUFFICIENT_AUTHOR);
  }

  // Make sure it's not a blob operation (no attributes in the profile are long)
  if (offset > 0)
  {
    return (ATT_ERR_ATTR_NOT_LONG);
  }

  if (utilExtractUuid16(pAttr,&uuid) == FAILURE)
  {
    // Invalid handle
    return ATT_ERR_INVALID_HANDLE;
  }

  if (uuid == IO_DATA_UUID || uuid == IO_CONF_UUID)
  {
    *pLen = sizeof(uint8_t);
    pValue[0] = pAttr->pValue[0];
  }
  else
  {
    // Should never get here!
    *pLen = 0;
    status = ATT_ERR_ATTR_NOT_FOUND;
  }

  return (status);
}

/*********************************************************************
 * @fn      io_WriteAttrCB
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
static bStatus_t io_WriteAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                uint8_t *pValue, uint16_t len, uint16_t offset,
                                uint8_t method)
{
  bStatus_t status = SUCCESS;
  uint8_t notifyApp = 0xFF;
  uint16_t uuid;

  // If attribute permissions require authorization to write, return error
  if (gattPermitAuthorWrite(pAttr->permissions))
  {
    // Insufficient authorization
    return (ATT_ERR_INSUFFICIENT_AUTHOR);
  }

  if (utilExtractUuid16(pAttr,&uuid) == FAILURE)
  {
    // Invalid handle
    return ATT_ERR_INVALID_HANDLE;
  }

  switch (uuid)
  {
    case IO_DATA_UUID:
    case IO_CONF_UUID:
      // Validate the value
      // Make sure it's not a blob oper
      if (offset == 0)
      {
        if (len == sizeof(uint8_t))
        {
          if (uuid == IO_CONF_UUID && (pValue[0] >= IO_MODE_NUM_MODES))
            status = ATT_ERR_INVALID_VALUE;
        } else
        {
          status = ATT_ERR_INVALID_VALUE_SIZE;
        }
      }
      else
      {
        status = ATT_ERR_ATTR_NOT_LONG;
      }

      // Write the value
      if (status == SUCCESS)
      {
        uint8_t *pCurValue = (uint8_t *)pAttr->pValue;
        pCurValue[0] = pValue[0];

        if (uuid == IO_CONF_UUID)
        {
          if (pAttr->pValue == &ioConf )
          {
            notifyApp = SENSOR_CONF;
          }
        }
        else  // uuid == IO_DATA_UUID
        {
          if (pAttr->pValue == &ioData )
          {
            notifyApp = SENSOR_DATA;
          }
        }
      }
      break;

    case GATT_CLIENT_CHAR_CFG_UUID:
      status = GATTServApp_ProcessCCCWriteReq(connHandle, pAttr, pValue, len,
                                              offset, GATT_CLIENT_CFG_NOTIFY);
      break;

    default:
      // Should never get here! 
      status = ATT_ERR_ATTR_NOT_FOUND;
      break;
  }

  // If a characteristic value changed then callback function to 
  // notify application of change
  if ((notifyApp != 0xFF ) && io_AppCBs && io_AppCBs->pfnSensorChange)
  {
    io_AppCBs->pfnSensorChange(notifyApp);
  }

  return (status);;
}

/*********************************************************************
*********************************************************************/
