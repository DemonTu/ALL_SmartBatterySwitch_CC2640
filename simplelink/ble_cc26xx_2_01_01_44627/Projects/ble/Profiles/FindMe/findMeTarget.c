/**************************************************************************************************
  Filename:       findMeTarget.c
  Revised:        $Date: 2015-05-08 08:20:47 -0700 (Fri, 08 May 2015) $
  Revision:       $Revision: 43731 $

  Description:    This file contains the Find Me profile target role sample 
                  GATT service profile.

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
#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "profile_uuid.h"
#include "gattcomdef.h"

#include "findMeTarget.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */

#define SERVAPP_NUM_ATTR_SUPPORTED        3

/*********************************************************************
 * TYPEDEFS
 */

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

static findMeTargetCBs_t *findMeTarget_AppCBs = NULL;

/*********************************************************************
 * Profile Attributes - variables
 */

// Simple Profile Service attribute
static CONST gattAttrType_t findMeImAlertService = { ATT_BT_UUID_SIZE, imAlertServUUID };

// Simple Profile Characteristic 3 Properties
static uint8 findMeAlertLevelProps = GATT_PROP_READ | GATT_PROP_WRITE;

// Characteristic 3 Value
static uint8 findMeAlertLevel = ALERT_LEVEL_NO;


/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t findMeTargetAttrTbl[SERVAPP_NUM_ATTR_SUPPORTED] = 
{
  // Simple Profile Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8 *)&findMeImAlertService            /* pValue */
  },
     
    // Immediate Alert Level Characteristic Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &findMeAlertLevelProps 
    },

      // Immediate Alert Level Characteristic Value
      { 
        { ATT_BT_UUID_SIZE, alertLevelUUID },
        GATT_PERMIT_AUTHEN_READ | GATT_PERMIT_AUTHEN_WRITE, 
        0, 
        &findMeAlertLevel 
      },
};


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t findMeTarget_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                                          uint8 *pValue, uint16 *pLen, uint16 offset,
                                          uint16 maxLen, uint8 method );
static bStatus_t findMeTarget_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                           uint8 *pValue, uint16 len, uint16 offset,
                                           uint8 method );

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Find Me Service Callbacks
CONST gattServiceCBs_t findMeCBs =
{
  findMeTarget_ReadAttrCB,  // Read callback function pointer
  findMeTarget_WriteAttrCB, // Write callback function pointer
  NULL                      // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      FindMeTarget_AddService
 *
 * @brief   Initializes the Simple Profile service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   services - services to add. This is a bit map and can
 *                     contain more than one service.
 *
 * @return  Success or Failure
 */
bStatus_t FindMeTarget_AddService( uint32 services )
{
  uint8 status = SUCCESS;

  if ( services & FINDME_IM_ALERT_SERVICE )
  {
    // Register GATT attribute list and CBs with GATT Server App
    status = GATTServApp_RegisterService( findMeTargetAttrTbl, 
                                          GATT_NUM_ATTRS( findMeTargetAttrTbl ),
                                          GATT_MAX_ENCRYPT_KEY_SIZE,
                                          &findMeCBs );
  }

  return ( status );
}

/*********************************************************************
 * @fn      FindMeTarget_RegisterAppCBs
 *
 * @brief   Registers the application callback function. Only call 
 *          this function once.
 *
 * @param   callbacks - pointer to application callbacks.
 *
 * @return  SUCCESS or bleAlreadyInRequestedMode
 */
bStatus_t FindMeTarget_RegisterAppCBs( findMeTargetCBs_t *appCallbacks )
{
  if ( appCallbacks )
  {
    findMeTarget_AppCBs = appCallbacks;
    
    return ( SUCCESS );
  }
  else
  {
    return ( bleAlreadyInRequestedMode );
  }
}
   

/*********************************************************************
 * @fn      FindMeTarget_SetParameter
 *
 * @brief   Set a Simple Profile parameter.
 *
 * @param   param - Profile parameter ID
 * @param   len - length of data to right
 * @param   value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 *
 * @return  bStatus_t
 */
bStatus_t FindMeTarget_SetParameter( uint8 param, uint8 len, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case FINDME_IM_ALERT_LEVEL:
      if ( (len == sizeof ( uint8 )) && ((*((uint8*)value) <= ALERT_LEVEL_HIGH)) ) 
      {
        findMeAlertLevel = *((uint8*)value);
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
  
  return ( ret );
}

/*********************************************************************
 * @fn      FindMeTarget_GetParameter
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
bStatus_t FindMeTarget_GetParameter( uint8 param, void *value )
{
  bStatus_t ret = SUCCESS;
  switch ( param )
  {
    case FINDME_IM_ALERT_LEVEL:
      *((uint8*)value) = findMeAlertLevel;
      break;
      
    default:
      ret = INVALIDPARAMETER;
      break;
  }
  
  return ( ret );
}

/*********************************************************************
 * @fn          findMeTarget_ReadAttrCB
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
static bStatus_t findMeTarget_ReadAttrCB( uint16 connHandle, gattAttribute_t *pAttr, 
                                          uint8 *pValue, uint16 *pLen, uint16 offset,
                                          uint16 maxLen, uint8 method )
{
  bStatus_t status = SUCCESS;

  // If attribute permissions require authorization to read, return error
  if ( gattPermitAuthorRead( pAttr->permissions ) )
  {
    // Insufficient authorization
    return ( ATT_ERR_INSUFFICIENT_AUTHOR );
  }
  
  // Make sure it's not a blob operation (no attributes in the profile are long)
  if ( offset > 0 )
  {
    return ( ATT_ERR_ATTR_NOT_LONG );
  }
 
  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      // No need for "GATT_PRIMARY_SERVICE_UUID" or "GATT_CHARACTER_UUID" cases;
      // gattserverapp handles those types for reads

      case ALERT_LEVEL_UUID:
        *pLen = 1;
        pValue[0] = *pAttr->pValue;
        break;
        
      default:
        // Should never get here! (characteristics 3 and 4 do not have read permissions)
        *pLen = 0;
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else
  {
    // 128-bit UUID
    *pLen = 0;
    status = ATT_ERR_INVALID_HANDLE;
  }

  return ( status );
}

/*********************************************************************
 * @fn      findMeTarget_WriteAttrCB
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
static bStatus_t findMeTarget_WriteAttrCB( uint16 connHandle, gattAttribute_t *pAttr,
                                           uint8 *pValue, uint16 len, uint16 offset,
                                           uint8 method )
{
  bStatus_t status = SUCCESS; 
  uint8 notify = 0xFF;
    
  if ( pAttr->type.len == ATT_BT_UUID_SIZE )
  {
    // 16-bit UUID
    uint16 uuid = BUILD_UINT16( pAttr->type.uuid[0], pAttr->type.uuid[1]);
    switch ( uuid )
    {
      case ALERT_LEVEL_UUID:
        
        // Validate the value
        // Make sure it's not a blob operation
        if ( offset == 0 )
        {
          if ( len > 1 )
            status = ATT_ERR_INVALID_VALUE_SIZE;
          else
          {
            if ( pValue[0] > ALERT_LEVEL_HIGH )
              status = ATT_ERR_INVALID_VALUE;
          }
        }
        else
        {
          status = ATT_ERR_ATTR_NOT_LONG;
        }
        
        //Write the value
        if ( status == SUCCESS )
        {
          uint8 *pCurValue = (uint8 *)pAttr->pValue;
          *pCurValue = pValue[0];
          notify = FINDME_IM_ALERT_LEVEL;
        }
        
        break;
        
      default:
        // Should never get here! (characteristics 2 and 4 do not have write permissions)
        status = ATT_ERR_ATTR_NOT_FOUND;
        break;
    }
  }
  else
  {
    // 128-bit UUID
    status = ATT_ERR_INVALID_HANDLE;
  }
  
  // If an attribute changed then callback function to notify application of change
  if ( (notify != 0xFF) && findMeTarget_AppCBs && findMeTarget_AppCBs->pfnAttrChange )
    findMeTarget_AppCBs->pfnAttrChange( notify );

  return ( status );
}

/*********************************************************************
*********************************************************************/
