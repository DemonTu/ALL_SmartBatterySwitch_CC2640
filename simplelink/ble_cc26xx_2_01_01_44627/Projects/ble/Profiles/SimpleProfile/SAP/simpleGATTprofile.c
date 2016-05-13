/**************************************************************************************************
  Filename:       simpleGATTprofile.c
  Revised:        $Date: 2015-07-23 11:48:29 -0700 (Thu, 23 Jul 2015) $
  Revision:       $Revision: 44402 $

  Description:    This file contains the Simple GATT profile sample GATT service 
                  profile for use with the BLE sample application.

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
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <string.h>
#include <stdint.h>

#include "SNP.h"
#include "snp_rpc.h"
#include "sap.h"
#include "simpleGATTprofile.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define SP_DEFAULT_CCCD         0

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Simple GATT Profile Service UUID: 0xFFF0
uint8 simpleProfileServUUID[SNP_16BIT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_SERV_UUID), HI_UINT16(SIMPLEPROFILE_SERV_UUID)
};

// Characteristic 1 UUID: 0xFFF1
uint8 simpleProfilechar1UUID[SNP_16BIT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_CHAR1_UUID), HI_UINT16(SIMPLEPROFILE_CHAR1_UUID)
};

// Characteristic 2 UUID: 0xFFF2
uint8 simpleProfilechar2UUID[SNP_16BIT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_CHAR2_UUID), HI_UINT16(SIMPLEPROFILE_CHAR2_UUID)
};

// Characteristic 3 UUID: 0xFFF3
uint8 simpleProfilechar3UUID[SNP_16BIT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_CHAR3_UUID), HI_UINT16(SIMPLEPROFILE_CHAR3_UUID)
};

// Characteristic 4 UUID: 0xFFF4
uint8 simpleProfilechar4UUID[SNP_16BIT_UUID_SIZE] =
{ 
  LO_UINT16(SIMPLEPROFILE_CHAR4_UUID), HI_UINT16(SIMPLEPROFILE_CHAR4_UUID)
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

static simpleProfileWrite_t simpleProfile_AppWriteCB = NULL;
static simpleProfileCCCD_t simpleProfile_AppCccdCB = NULL;

static uint8_t cccdFlag = SP_DEFAULT_CCCD;

static uint16_t connHandle = 0; // Assumes only one connection at a time

/*********************************************************************
 * Profile Attributes - variables
 */

SAP_Service_t simpleService;
SAP_CharHandle_t simpleServiceCharHandles[4];

static UUIDType_t simpleServiceUUID = {SNP_16BIT_UUID_SIZE, simpleProfileServUUID};

// Characteristic 1 Value
static uint8 simpleProfileChar1 = 0;

// Simple Profile Characteristic 1 User Description
static uint8 simpleProfileChar1UserDesp[17] = "Characteristic 1";

// Characteristic 2 Value
static uint8 simpleProfileChar2 = 0;

// Simple Profile Characteristic 2 User Description
static uint8 simpleProfileChar2UserDesp[17] = "Characteristic 2";

// Characteristic 3 Value
static uint8 simpleProfileChar3 = 0;

// Simple Profile Characteristic 3 User Description
static uint8 simpleProfileChar3UserDesp[17] = "Characteristic 3";

// Characteristic 4 Value
static uint8 simpleProfileChar4 = 0;
                                        
// Simple Profile Characteristic 4 User Description
static uint8 simpleProfileChar4UserDesp[17] = "Characteristic 4";

/*********************************************************************
 * Profile Attributes - Table
 */
SAP_UserDescAttr_t char1UserDesc = {SNP_GATT_PERMIT_READ, SNP_GATT_CHAR_MAX_LENGTH, 17, simpleProfileChar1UserDesp};
SAP_UserDescAttr_t char2UserDesc = {SNP_GATT_PERMIT_READ, SNP_GATT_CHAR_MAX_LENGTH, 17, simpleProfileChar2UserDesp};
SAP_UserDescAttr_t char3UserDesc = {SNP_GATT_PERMIT_READ, SNP_GATT_CHAR_MAX_LENGTH, 17, simpleProfileChar3UserDesp};
SAP_UserDescAttr_t char4UserDesc = {SNP_GATT_PERMIT_READ, SNP_GATT_CHAR_MAX_LENGTH, 17, simpleProfileChar4UserDesp};

SAP_UserCCCDAttr_t char4CCCD = {SNP_GATT_PERMIT_READ | SNP_GATT_PERMIT_WRITE};
   
#define SERVAPP_NUM_ATTR_SUPPORTED 4
static SAP_Char_t simpleProfileAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] = 
{
    // Characteristic 1 Value Declaration
    { 
      { SNP_16BIT_UUID_SIZE, simpleProfilechar1UUID }, /* UUID             */
      SNP_GATT_PROP_READ | SNP_GATT_PROP_WRITE,        /* Properties       */
      SNP_GATT_PERMIT_READ | SNP_GATT_PERMIT_WRITE,    /* Permissions      */
      &char1UserDesc,                                  /* User Description */
      NULL,                                            /* CCCD             */
      NULL,                                            /* Format           */        
      NULL,                                            /* Short UUID       */
      NULL                                             /* Long UUID        */
    },
    
    // Characteristic 2 Value Declaration
    { 
      { SNP_16BIT_UUID_SIZE, simpleProfilechar2UUID }, /* UUID             */
      SNP_GATT_PROP_READ,                               /* Properties       */
      SNP_GATT_PERMIT_READ,                             /* Permissions      */
      &char2UserDesc                                /* User Description */
                                                    /* CCCD             */
                                                    /* Format           */
    },  
    
    // Characteristic 3 Value Declaration
    { 
      { SNP_16BIT_UUID_SIZE, simpleProfilechar3UUID }, /* UUID             */
      SNP_GATT_PROP_WRITE,                              /* Properties       */
      SNP_GATT_PERMIT_WRITE,                            /* Permissions      */
      &char3UserDesc                                /* User Description */
                                                    /* CCCD             */
                                                    /* Format           */
    },
    
    // Characteristic 4 Value Declaration
    { 
      { SNP_16BIT_UUID_SIZE, simpleProfilechar4UUID }, /* UUID             */
      SNP_GATT_PROP_NOTIFICATION,                       /* Properties       */
      0,                                            /* Permissions      */
      &char4UserDesc,                               /* User Description */
      &char4CCCD                                    /* CCCD             */
                                                    /* Format           */
    },
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static bStatus_t simpleProfile_ReadAttrCB(void *context, 
                                   uint16_t connectionHandle, 
                                   uint16_t charHdl, uint16_t offset, 
                                   uint16_t size, uint16_t * len, 
                                   uint8_t *pData);

bStatus_t simpleProfile_WriteAttrCB(void *context,
                                    uint16_t connectionHandle,
                                    uint16_t charHdl, uint16_t len,
                                    uint8_t *pData);

bStatus_t simpleProfile_CCCDIndCB(void *context,
                                  uint16_t connectionHandle,
                                  uint16_t cccdHdl, uint8_t type,
                                  uint16_t value);

/*********************************************************************
 * PROFILE CALLBACKS
 */

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

static void simpleProfile_processSNPEventCB(uint16_t event, 
                                            snpEventParam_t *param);

static uint16_t getCharIDFromHdl(uint16_t charHdl);
static uint16_t getHdlFromCharID(uint16_t charID);

/*********************************************************************
 * @fn      SimpleProfile_AddService
 *
 * @brief   Initializes the Simple Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   none
 *
 * @return  Success or Failure
 */
bStatus_t SimpleProfile_AddService(void)
{ 
  // Register to recieve Connection Established Events
  SAP_registerEventCB(simpleProfile_processSNPEventCB, SNP_CONN_EST_EVT);
  
   // reads through table, adding attributes to the NP.
  simpleService.serviceUUID       = simpleServiceUUID;
  simpleService.serviceType       = SNP_PRIMARY_SERVICE;
  simpleService.charTableLen      = SERVAPP_NUM_ATTR_SUPPORTED;
  simpleService.charTable         = simpleProfileAttrTbl;
  simpleService.context           = NULL;
  simpleService.charReadCallback  = simpleProfile_ReadAttrCB;
  simpleService.charWriteCallback = simpleProfile_WriteAttrCB;
  simpleService.cccdIndCallback   = simpleProfile_CCCDIndCB;
  simpleService.charAttrHandles   = simpleServiceCharHandles;
   
  // Service is setup, register with GATT server on the SNP.
  SAP_registerService(&simpleService);
  
  return SUCCESS;
}

/*********************************************************************
 * @fn      SimpleProfile_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call 
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t SimpleProfile_RegisterAppCB(simpleProfileWrite_t writeCB, 
                                      simpleProfileCCCD_t cccdCB)
{
  simpleProfile_AppWriteCB = writeCB;
  simpleProfile_AppCccdCB = cccdCB;
  
  return (SUCCESS);
}

/*********************************************************************
 * @fn      SimpleProfile_SetParameter
 *
 * @brief   Set a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to write
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t SimpleProfile_SetParameter(uint8 param, uint8 len, void *value)
{
  bStatus_t ret = SUCCESS;
  switch (SP_ID_CHAR(param))
  {
    case SP_CHAR1:
      if (len == sizeof (uint8)) 
      {
        simpleProfileChar1 = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case SP_CHAR2:
      if (len == sizeof (uint8)) 
      {
        simpleProfileChar2 = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case SP_CHAR3:
      if (len == sizeof (uint8)) 
      {
        simpleProfileChar3 = *((uint8*)value);
      }
      else
      {
        ret = bleInvalidRange;
      }
      break;

    case SP_CHAR4:
      if (len == sizeof (uint8)) 
      {
        snpNotifIndReq_t localReq;
        simpleProfileChar4 = *((uint8*)value);
        
        // Initialize Request
        localReq.connHandle = connHandle;
        localReq.attrHandle = getHdlFromCharID(SP_ID_CREATE(SP_CHAR4,SP_VALUE));
        localReq.pData = (uint8_t *)&simpleProfileChar4;
        localReq.authenticate = 0; // Default no authentication
        
        // Check for whether a notification or indication should be sent.
        // Both flags should never be allowed to be set by NWP
        if (cccdFlag & SNP_GATT_CLIENT_CFG_NOTIFY)
        {
          localReq.type = SNP_SEND_NOTIFICATION;
          SNP_sendNotifInd(&localReq, sizeof(simpleProfileChar4));
        }
        else if (cccdFlag & SNP_GATT_CLIENT_CFG_INDICATE)
        {
          localReq.type = SNP_SEND_INDICATION;
          SNP_sendNotifInd(&localReq, sizeof(simpleProfileChar4));
        }      
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
 * @fn      SimpleProfile_GetParameter
 *
 * @brief   Get a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   value - pointer to data to put.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t SimpleProfile_GetParameter(uint8 param, void *value)
{
  bStatus_t ret = SUCCESS;
  switch (SP_ID_CHAR(param))
  {
    case SP_CHAR1:
      *((uint8*)value) = simpleProfileChar1;
      break;

    case SP_CHAR2:
      *((uint8*)value) = simpleProfileChar2;
      break;      

    case SP_CHAR3:
      *((uint8*)value) = simpleProfileChar3;
      break;  

    case SP_CHAR4:
      *((uint8*)value) = simpleProfileChar4;
      break;
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return (ret);
}

/*********************************************************************
 * @fn          simpleProfile_ReadAttrCB
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

bStatus_t simpleProfile_ReadAttrCB(void *context, 
                                   uint16_t connectionHandle, 
                                   uint16_t charHdl, uint16_t offset, 
                                   uint16_t size, uint16_t * len, 
                                   uint8_t *pData)
{
  // Get characteristic from handle
  uint8_t charID = getCharIDFromHdl(charHdl);
  uint8_t isValid = 0;

  // Update Conn Handle (assumes one connection)
  connHandle = connectionHandle;

  switch(SP_ID_CHAR(charID))
  {
    case SP_CHAR1:
      switch (SP_ID_CHARHTYPE(charID))
      {
        case SP_VALUE:
          *len = sizeof(simpleProfileChar1);
          memcpy(pData,&simpleProfileChar1,sizeof(simpleProfileChar1));
          isValid = 1;
          break;

        default:
          // Should not receive SP_USERDESC || SP_FORMAT || SP_CCCD
          break;
      }
      break;
    case SP_CHAR2:
      switch (SP_ID_CHARHTYPE(charID))
      {
        case SP_VALUE:
          *len = sizeof(simpleProfileChar2);
          memcpy(pData,&simpleProfileChar2,sizeof(simpleProfileChar2));
          isValid = 1;
          break;

        default:
          // Should not receive SP_USERDESC || SP_FORMAT || SP_CCCD
          break;
      }
      break;
    default:
      // Should not receive SP_CHAR3 || SP_CHAR4 reads
      break;
  }

  if (isValid)
  {
	return (SNP_SUCCESS);
  }

  // Unable to find handle - set len to 0 and return error code
  *len = 0;
  return (SNP_UNKNOWN_ATTRIBUTE);
}

/*********************************************************************
 * @fn      simpleProfile_WriteAttrCB
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
bStatus_t simpleProfile_WriteAttrCB(void *context,
                                    uint16_t connectionHandle,
                                    uint16_t charHdl, uint16_t len,
                                    uint8_t *pData)
{
  bStatus_t status = SNP_UNKNOWN_ATTRIBUTE;
  uint8 notifyApp = SP_UNKNOWN_CHAR;
  
  // Update Conn Handle (assumes one connection)
  connHandle = connectionHandle;
  
  // Get characteristic from handle
  uint8_t charID = getCharIDFromHdl(charHdl);

  switch(SP_ID_CHAR(charID))
  {
    case SP_CHAR1:
      switch (SP_ID_CHARHTYPE(charID))
      {
        case SP_VALUE:
          if (len == sizeof(simpleProfileChar1))
          {
            simpleProfileChar1 = pData[0];
            status = SNP_SUCCESS;
            notifyApp = SP_CHAR1_ID;
          }
          break;
        default:
          // Should not receive SP_USERDESC || SP_FORMAT || SP_CCCD
          break;
      }
      break;
    case SP_CHAR3:
      switch (SP_ID_CHARHTYPE(charID))
      {
        case SP_VALUE:
          if (len == sizeof(simpleProfileChar3))
          {
            simpleProfileChar3 = pData[0];
            status = SNP_SUCCESS;
            notifyApp = SP_CHAR3_ID;
          }
          break;
        default:
          // Should not receive SP_USERDESC || SP_FORMAT || SP_CCCD
          break;
      }
      break;
    default:
      // Should not receive SP_CHAR2 || SP_CHAR4 writes
      break;
  }

  // If a characteristic value changed then callback function to notify application of change
  if ((notifyApp != SP_UNKNOWN_CHAR) && simpleProfile_AppWriteCB)
  {
    simpleProfile_AppWriteCB(notifyApp);  
  }
  
  return (status);
}

/*********************************************************************
 * @fn      simpleProfile_CCCDIndCB
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
bStatus_t simpleProfile_CCCDIndCB(void *context,
                                    uint16_t connectionHandle,
                                    uint16_t cccdHdl, uint8_t type,
                                    uint16_t value)
{
  bStatus_t status = SNP_UNKNOWN_ATTRIBUTE;
  uint8 notifyApp = SP_UNKNOWN_CHAR;
  
  // Update Conn Handle (assumes one connection)
  connHandle = connectionHandle;
  
  // Get characteristic from handle
  uint8_t charID = getCharIDFromHdl(cccdHdl);

  switch(SP_ID_CHAR(charID))
  {
    case SP_CHAR4:
      switch (SP_ID_CHARHTYPE(charID))
      {
        case SP_CCCD:
          // Set Global cccd Flag which will be used to to gate Indications
          // or Notifications when SetParameter() is called
          cccdFlag = value;
          notifyApp = charID;
          status = SNP_SUCCESS;
          break;
        default:
          // Should not receive SP_VALUE || SP_USERDESC || SP_FORMAT
          break;
      }
      break;
    default:
      // No other Characteristics have CCCB attributes
      break;
  }

  // If a characteristic value changed then callback function to notify application of change
  if ((notifyApp != SP_UNKNOWN_CHAR) && simpleProfile_AppCccdCB)
  {
    simpleProfile_AppCccdCB(notifyApp, value);  
  }
  
  return (status);
}


/*********************************************************************
 * @fn      simpleProfile_processSNPEventCB
 *
 * @brief   This is a callback operating in the NPI task. It will be envoked
 *          whenever an event is received from the SNP that this profile has
 *          registered for
 *
 * @param   event  - event mask
 * @param   pValue - pointer event struct
 *
 * @return  status
 */
void simpleProfile_processSNPEventCB(uint16_t event, snpEventParam_t *param)
{ 
  switch(event)
  {
    case SNP_CONN_EST_EVT:
      {
        cccdFlag = SP_DEFAULT_CCCD;
      }
      break;
    default:
      break;
  }
}

/*********************************************************************
/ Utility function for translating a characteristic handle to 
/ a handle ID (handle ID is a characteristic number plus type)
/
*/

uint16_t getCharIDFromHdl(uint16_t charHdl)
{
  uint16_t i;

  // Find Characteristic based on Handle
  for (i = 0; i < SERVAPP_NUM_ATTR_SUPPORTED; i++)
  {
    // Check if it is the characteristic value handle or cccd handle
    if (simpleServiceCharHandles[i].valueHandle == charHdl)
    {
      return SP_ID_CREATE(i,SP_VALUE);
    }
    else if (simpleServiceCharHandles[i].userDescHandle == charHdl)
    {
      return SP_ID_CREATE(i,SP_USERDESC);
    }
    else if (simpleServiceCharHandles[i].cccdHandle == charHdl)
    {
      return SP_ID_CREATE(i,SP_CCCD);
    }
    else if (simpleServiceCharHandles[i].formatHandle == charHdl)
    {
      return SP_ID_CREATE(i,SP_FORMAT);
    }
  }
  
  return SP_UNKNOWN_CHAR;
}

/*********************************************************************
/ Utility function for translating a characteristic ID to a handle
/
*/

uint16_t getHdlFromCharID(uint16_t charID)
{
  SAP_CharHandle_t charstick;
    
  if (SP_ID_CHAR(charID) < SERVAPP_NUM_ATTR_SUPPORTED)
  {
     charstick = simpleServiceCharHandles[SP_ID_CHAR(charID)];

    switch (SP_ID_CHARHTYPE(charID))
    {
    case SP_VALUE:
      return charstick.valueHandle;
    case SP_USERDESC:
      return charstick.userDescHandle;
    case SP_CCCD:
      return charstick.cccdHandle;
    case SP_FORMAT:
      return charstick.formatHandle;
    default:
      break;
    }
  }
  
  return SNP_INVALID_HANDLE;
}
                           
   
/*********************************************************************
*********************************************************************/
