/*******************************************************************************
  Filename:       registerservice.c
  Revised:        $Date: 2014-06-17 00:12:16 +0200 (ti, 17 jun 2014) $
  Revision:       $Revision: 39036 $

  Description:    Generic register access service for I2C/SPI devices


  Copyright 2015  Texas Instruments Incorporated. All rights reserved.

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

#ifdef FEATURE_REGISTER_SERVICE
/*********************************************************************
 * INCLUDES
 */
#include "bcomdef.h"
#include "linkdb.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "string.h"

#include "registerservice.h"
#include "st_util.h"
#include "sensor.h"
#include "bsp_i2c.h"
#include <ti/drivers/PIN.h>

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
// Attribute names
#ifdef USER_DESCRIPTION
#define REGISTER_DATA_DESCR       "Register Data"
#define REGISTER_ADDR_DESCR       "Register Address"
#define REGISTER_INTF_DESCR       "Register Device"
#endif

// Attribute Identifiers
#define REGISTER_DATA             0
#define REGISTER_ADDR             1
#define REGISTER_DEV              2

// Attribute sizes
#define REGISTER_DATA_LEN         16
#define REGISTER_ADDRESS_LEN      5 // length, on-chip address (1 to 4 bytes)
#define REGISTER_DEVICE_LEN       2 // interface, device address

// Interfaces
#define REGISTER_INTERFACE_I2C0   0 // TMP007,BMP280,OPT3001,SHT21
#define REGISTER_INTERFACE_I2C1   1 // MPU9250
#define REGISTER_INTERFACE_SPI0   2 // W25x20CL flash - TBD
#define REGISTER_INTERFACE_SPI2   3 // DevPack (LCD etc.) - TBD
#define REGISTER_INTERFACE_SPI3   4 // Spare
#define REGISTER_INTERFACE_MCU    5 // MCU address space
#define REGISTER_INTERFACE_NUM    6 // Number of defined interfaces

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// Service UUID
static CONST uint8_t sensorServiceUUID[TI_UUID_SIZE] =
{
  TI_UUID(REGISTER_SERV_UUID),
};

// Characteristic UUID: data
static CONST uint8_t registerDataUUID[TI_UUID_SIZE] =
{
  TI_UUID(REGISTER_DATA_UUID),
};

// Characteristic UUID: config
static CONST uint8_t registerAddressUUID[TI_UUID_SIZE] =
{
  TI_UUID(REGISTER_ADDR_UUID),
};

// Characteristic UUID: period
static CONST uint8_t registerDeviceIDUUID[TI_UUID_SIZE] =
{
  TI_UUID(REGISTER_DEV_UUID),
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

static sensorCBs_t *sensor_AppCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */

// Profile Service attribute
static CONST gattAttrType_t sensorService = { TI_UUID_SIZE, sensorServiceUUID };

// Characteristic Value: data
static uint8_t registerData[REGISTER_DATA_LEN] = { 0, 0, 0, 0, 0, 0, 0, 0,
                                                   0, 0, 0, 0, 0, 0, 0, 0
                                                 };

// Characteristic Properties: data
static uint8_t registerDataProps = GATT_PROP_READ | GATT_PROP_WRITE 
                                   | GATT_PROP_NOTIFY;

// Characteristic Configuration: data
static gattCharCfg_t *registerDataConfig;

#ifdef USER_DESCRIPTION
// Characteristic User Description: data
static uint8_t registerDataUserDescr[] = REGISTER_DATA_DESCR;
#endif

// Characteristic Properties: configuration
static uint8_t registerAddressProps = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic Value: configuration
static uint8_t registerAddress[REGISTER_ADDRESS_LEN];

#ifdef USER_DESCRIPTION
// Characteristic User Description: configuration
static uint8_t registerAddressUserDescr[] = REGISTER_ADDR_DESCR; 
#endif

// Characteristic Properties: interface/device address
static uint8_t registerDeviceIDProps = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic Value: interface/device address
static uint8_t registerDeviceID[REGISTER_DEVICE_LEN];

#ifdef USER_DESCRIPTION
// Characteristic User Description: period
static uint8_t registerDeviceIDUserDescr[] = REGISTER_INTF_DESCR;
#endif

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t sensorAttrTable[] =
{
  {
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8_t *)&sensorService                 /* pValue */
  },

    // Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &registerDataProps
    },

      // Characteristic Value "Data"
      {
        { TI_UUID_SIZE, registerDataUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        registerData
      },

      // Characteristic configuration
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8_t *)&registerDataConfig
      },

#ifdef USER_DESCRIPTION
      // Characteristic User Description
      {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        registerDataUserDescr
      },
#endif
    // Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &registerAddressProps
    },

      // Characteristic Value "Configuration"
      {
        { TI_UUID_SIZE, registerAddressUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8_t*)&registerAddress
      },

#ifdef USER_DESCRIPTION
      // Characteristic User Description
      {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        registerAddressUserDescr
      },
#endif
     // Characteristic Declaration "Period"
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &registerDeviceIDProps
    },

      // Characteristic Value "Period"
      {
        { TI_UUID_SIZE, registerDeviceIDUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8_t*)&registerDeviceID
      },

#ifdef USER_DESCRIPTION
      // Characteristic User Description "Period"
      {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        registerDeviceIDUserDescr
      },
#endif
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t sensor_ReadAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                    uint8_t *pValue, uint16_t *pLen, 
                                    uint16_t offset, uint16_t maxLen,
                                    uint8_t method);
static bStatus_t sensor_WriteAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                     uint8_t *pValue, uint16_t len, 
                                     uint16_t offset, uint8_t method);
static void sensor_readRegister(void);
static void sensor_writeRegister(void);
static void sensor_initRegister(uint8_t interfaceId, uint8_t address);

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Gyro Profile Service Callbacks
static CONST gattServiceCBs_t sensorCBs =
{
  sensor_ReadAttrCB,  // Read callback function pointer
  sensor_WriteAttrCB, // Write callback function pointer
  NULL                // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      Register_addService
 *
 * @brief   Initializes the Sensor Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @return  Success or Failure
 */
bStatus_t Register_addService(void)
{
  // Allocate Client Characteristic Configuration table
  registerDataConfig = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) *
                                                    linkDBNumConns);
  if (registerDataConfig == NULL)
  {
    return (bleMemAllocError);
  }
  
  // Register with Link DB to receive link status change callback
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, registerDataConfig);

  //sensor_initRegister(REGISTER_INTERFACE_I2C0, TMP007_I2C_ADDRESS);
  
  // Register GATT attribute list and CBs with GATT Server App
  return GATTServApp_RegisterService( sensorAttrTable,
                                      GATT_NUM_ATTRS (sensorAttrTable),
                                      GATT_MAX_ENCRYPT_KEY_SIZE,
                                      &sensorCBs );
}


/*********************************************************************
 * @fn      Register_registerAppCBs
 *
 * @brief   Registers the application callback function. Only call
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t Register_registerAppCBs(sensorCBs_t *appCallbacks)
{
  if (sensor_AppCBs == NULL)
  {
    if (appCallbacks != NULL)
    {
      sensor_AppCBs = appCallbacks;
    }

    return (SUCCESS);
  }

  return (bleAlreadyInRequestedMode);
}

/*********************************************************************
 * @fn      Register_setParameter
 *
 * @brief   Set a parameter.
 *
 * @param   param - Profile parameter ID (only data is applicable)
 * @param   len - length of data to write
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate
 *          data type (example: data type of uint16_t will be cast to
 *          uint16_t pointer).
 *
 * @return  bStatus_t
 */
bStatus_t Register_setParameter(uint8_t param, uint8_t len, void *value)
{
  bStatus_t ret = SUCCESS;

  switch (param)
  {
  case REGISTER_DATA:
    if (len <= REGISTER_DATA_LEN )
    {
      // See if Notification has been enabled
      GATTServApp_ProcessCharCfg( registerDataConfig, registerData, FALSE,
                                 sensorAttrTable, GATT_NUM_ATTRS (sensorAttrTable),
                                 INVALID_TASK_ID, sensor_ReadAttrCB);
    }
    
  default:
    ret = INVALIDPARAMETER;
    break;
  }

  return (ret);
}


/*********************************************************************
 * @fn          sensor_ReadAttrCB
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
static bStatus_t sensor_ReadAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                    uint8_t *pValue, uint16_t *pLen, 
                                    uint16_t offset, uint16_t maxLen,
                                    uint8_t method)
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

  if (utilExtractUuid16(pAttr,&uuid) == FAILURE) {
    // Invalid handle
    *pLen = 0;
    return ATT_ERR_INVALID_HANDLE;
  }

  switch (uuid)
  {
    // No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
    // gattserverapp handles those reads
    case REGISTER_DATA_UUID:
      sensor_readRegister();
      *pLen = registerAddress[0];
      memcpy(pValue, pAttr->pValue, *pLen);
      break;

    case REGISTER_ADDR_UUID:
      *pLen = REGISTER_ADDRESS_LEN;
      memcpy(pValue, pAttr->pValue, REGISTER_ADDRESS_LEN);
      break;
      
    case REGISTER_DEV_UUID:
      *pLen = REGISTER_DEVICE_LEN;
      memcpy(pValue, pAttr->pValue, REGISTER_DEVICE_LEN);
      break;

    default:
      *pLen = 0;
      status = ATT_ERR_ATTR_NOT_FOUND;
      break;
    }

  return (status);
}

/*********************************************************************
 * @fn      sensor_WriteAttrCB
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
static bStatus_t sensor_WriteAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                    uint8_t *pValue, uint16_t len, 
                                    uint16_t offset, uint8_t method)
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
  case REGISTER_DATA_UUID:
    // Validate the value
    // Make sure it's not a blob oper
    if (offset == 0)
    {
      if (len > REGISTER_DATA_LEN )
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
      memcpy(pAttr->pValue, pValue, len);
      sensor_writeRegister();
    }
    break;
    
  case REGISTER_ADDR_UUID:
    // Validate the value
    // Make sure it's not a blob oper
    if (offset == 0)
    {
      if (len > REGISTER_ADDRESS_LEN )
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
      if (pValue[0] <= REGISTER_DATA_LEN )
      {
        memcpy(pAttr->pValue, pValue, len);
        // Address changed; read data
        sensor_readRegister();
        
        if (pAttr->pValue == (uint8_t*)&registerAddress )
        {
          notifyApp = REGISTER_ADDR;
        }
      }
      else
      {
        status = ATT_ERR_INVALID_VALUE;
      }
    }
    break;
    
  case REGISTER_DEV_UUID:
    // Validate the value
    // Make sure it's not a blob oper
    if (offset == 0)
    {
      if (len != REGISTER_DEVICE_LEN )
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
      if (pValue[0] < REGISTER_INTERFACE_NUM )
      {
        memcpy(pAttr->pValue, pValue, REGISTER_DEVICE_LEN);
        
        if (pAttr->pValue == (uint8_t*)&registerDeviceID )
        {
          notifyApp = REGISTER_DEV;
          // bspI2cReset();
          
          sensor_initRegister(registerDeviceID[0], registerDeviceID[1]);
        }
      }
        else
        {
          status = ATT_ERR_INVALID_VALUE;
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
    
    // If a characteristic value changed then callback function 
    // to notify application of change
    if ((notifyApp != 0xFF ) && sensor_AppCBs && sensor_AppCBs->pfnSensorChange)
    {
      sensor_AppCBs->pfnSensorChange(notifyApp);
    }
    
    return (status);
  }
  

/*********************************************************************
 * @fn      sensor_initRegister
 *
 * @brief   Read registers and store the data in 'registerData'
 *
 * @param   interfaceId - interface ID
 *
 * @param   address - device address
 *
 * @return  none
 */
static void sensor_initRegister(uint8_t interfaceId, uint8_t address)
{
  switch (interfaceId)
  {
  case REGISTER_INTERFACE_I2C0:
    // Set TMP007 (status register) as default
    registerAddress[0] = 2;
    registerAddress[1] = 2;
    registerAddress[2] = 0;
    registerAddress[3] = 0;
    registerAddress[4] = 0;
    registerDeviceID[1] = (uint8_t)address;
    break;
    
  case REGISTER_INTERFACE_I2C1:
    // Set MPU9250 (PWR_MGMT_1) as default
    registerAddress[0] = 2;
    registerAddress[1] = 0x6B;
    registerAddress[2] = 0;
    registerAddress[3] = 0;
    registerAddress[4] = 0;
    registerDeviceID[1] = (uint8_t)address;
    break;
    
  case REGISTER_INTERFACE_SPI0:
    // Set external flash as default (register device ID)
    registerAddress[0] = 3;
    registerAddress[1] = 0x9F;
    registerAddress[2] = 0;
    registerAddress[3] = 0;
    registerAddress[4] = 0;
    registerDeviceID[1] = 0;
    break;

  case REGISTER_INTERFACE_MCU:
    // Set GPIO (DATI31 as default)
    registerDeviceID[1] = 0;
    registerAddress[0] = 4;
    registerAddress[1] = 0xC0;
    registerAddress[2] = 0x20;
    registerAddress[3] = 0x02;
    registerAddress[4] = 0x40;
    break;
    
  default:
    break;
  }
  
  registerDeviceID[0] = interfaceId;
}


/*********************************************************************
 * @fn      sensor_readRegister
 *
 * @brief   Read registers and store the data in 'registerData'
 *
 * @return  none
 */
static void sensor_readRegister(void)
{
  uint8_t deviceAddress, interface, len;
  
  interface = registerDeviceID[0];
  deviceAddress = registerDeviceID[1];
  len = registerAddress[0];
  
  switch (interface)
  {
  // I2C interfaces
  case REGISTER_INTERFACE_I2C0:
  case REGISTER_INTERFACE_I2C1:
    {
      uint8_t regaddr;
      
      regaddr = registerAddress[1];
      bspI2cSelect(interface, deviceAddress);
      if (!sensorReadReg(regaddr,registerData,len))
      {
    	uint8_t i;
        for (i=0; i<len; i++)
        {
          registerData[i]= 0xFF;
        }
      }
      
      bspI2cDeselect();
    }
    break;
        
  case REGISTER_INTERFACE_MCU:
    {
      uint32_t addr;
      
      addr= *(uint32_t*)(&registerAddress[1]);
      memcpy(registerData,(uint8_t*)addr,len);
    }
    break;
    
  default:
    break;
  }
}

/*********************************************************************
 * @fn      sensor_writeRegister
 *
 * @brief   Write registers from the data stored in 'registerData'
 *
 * @return  none
 */
static void sensor_writeRegister(void)
{
  uint8_t deviceAddress, interface, len;
  
  interface = registerDeviceID[0];
  deviceAddress = registerDeviceID[1];
  len = registerAddress[0];
  
  switch (interface)
  {
  // I2C interfaces
  case REGISTER_INTERFACE_I2C0:
  case REGISTER_INTERFACE_I2C1:
    {
      uint8_t regaddr;
      
      regaddr = registerAddress[1];
      bspI2cSelect(interface, deviceAddress);
      if (len>0)
      {
        sensorWriteReg(regaddr,registerData,len);
      }
      else
      {
        bspI2cWriteSingle(regaddr);
      }
      bspI2cDeselect();
    }
    break;
        
  case REGISTER_INTERFACE_MCU:
    {
      uint32_t addr;
      
      addr= *(uint32*)(&registerAddress[1]);
      memcpy((uint8_t*)addr,registerData,len);
    }

  default:
    break;
  }
}

/*********************************************************************
*********************************************************************/
#endif
