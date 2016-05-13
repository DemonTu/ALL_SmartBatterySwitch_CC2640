/**************************************************************************************************
  Filename:       oadResetService.c
  Revised:        $Date: 2015-05-08 08:20:47 -0700 (Fri, 08 May 2015) $
  Revision:       $Revision: 43731 $

  Description:    This is the TI proprietary OAD Reset Service.

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

#include "hal_board.h"
#include "bcomdef.h"
#include "OSAL.h"
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"

#include "oad.h"
#include "oad_target.h"

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
// OAD Reset GATT Profile Service UUID
static const uint8_t resetServUUID[ATT_UUID_SIZE] =
{ 
  TI_BASE_UUID_128(OAD_RESET_SERVICE_UUID)
};

static const uint8_t resetCharUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(OAD_RESET_CHAR_UUID)
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

/*********************************************************************
 * Profile Attributes - variables
 */

// Simple Profile Service attribute
static CONST gattAttrType_t resetProfileService = { ATT_UUID_SIZE, resetServUUID };

// Simple Profile Characteristic 1 Properties
static uint8_t resetChar1Props = GATT_PROP_WRITE_NO_RSP | GATT_PROP_WRITE;

// Characteristic 1 Value
static uint8_t resetCharVal = 0;

// Simple Profile Characteristic 1 User Description
static uint8_t resetChar1UserDesc[] = "Reset";

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t resetServiceAttrTbl[] = 
{
  // Simple Profile Service
  { 
    { ATT_BT_UUID_SIZE, primaryServiceUUID }, /* type */
    GATT_PERMIT_READ,                         /* permissions */
    0,                                        /* handle */
    (uint8_t *)&resetProfileService            /* pValue */
  },

    // OAD Reset Characteristic Declaration
    { 
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ, 
      0,
      &resetChar1Props 
    },

      // OAD Reset Characteristic Value
      { 
        { ATT_UUID_SIZE, resetCharUUID },
        GATT_PERMIT_WRITE, 
        0, 
        &resetCharVal
      },

      // OAD Reset User Description
      { 
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ, 
        0, 
        resetChar1UserDesc 
      }
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static bStatus_t ResetReadAttrCB(uint16_t connHandle,
                                 gattAttribute_t *pAttr, 
                                 uint8_t *pValue, uint16_t *pLen,
                                 uint16_t offset, uint16_t maxLen,
                                 uint8_t method);
								 
static bStatus_t ResetWriteAttrCB(uint16_t connHandle,
                                  gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint16_t len,
                                  uint16_t offset, uint8_t method);

/*********************************************************************
 * PROFILE CALLBACKS
 */
// Simple Profile Service Callbacks
CONST gattServiceCBs_t resetServiceCBs =
{
  ResetReadAttrCB,  // Read callback function pointer
  ResetWriteAttrCB, // Write callback function pointer
  NULL              // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      Reset_addService
 *
 * @brief   Initializes the OAD Reset service by registering
 *          GATT attributes with the GATT server.
 *
 * @param   none
 *
 * @return  Success or Failure
 */
uint8_t Reset_addService(void)
{
  // Register GATT attribute list and CBs with GATT Server App
  return GATTServApp_RegisterService(resetServiceAttrTbl, 
                                     GATT_NUM_ATTRS(resetServiceAttrTbl),
                                     GATT_MAX_ENCRYPT_KEY_SIZE,
                                     &resetServiceCBs);
}

/*********************************************************************
 * @fn          ResetReadAttrCB
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
 * @return      ATT_ERR_INVALID_HANDLE, reads are not allowed.
 */
static bStatus_t ResetReadAttrCB(uint16_t connHandle,
                                 gattAttribute_t *pAttr,
                                 uint8_t *pValue, uint16_t *pLen,
                                 uint16_t offset, uint16_t maxLen,
                                 uint8_t method)
{
  return ATT_ERR_INVALID_HANDLE;
}

/*********************************************************************
 * @fn      ResetWriteAttrCB
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
static bStatus_t ResetWriteAttrCB(uint16_t connHandle,
                                  gattAttribute_t *pAttr,
                                  uint8_t *pValue, uint16_t len,
                                  uint16_t offset, uint8_t method)
{
  bStatus_t status = SUCCESS;
  
  if ( pAttr->type.len == ATT_UUID_SIZE )
  {
    // 128-bit UUID
    if (!memcmp(pAttr->type.uuid, resetCharUUID, ATT_UUID_SIZE))
    {
      uint16_t crc[2] = {0x0000, 0x0000};
      
      // Invalidate the image.
      OADTarget_writeFlash(OAD_IMG_R_PAGE, OAD_IMG_R_OSET + OAD_IMG_CRC_OSET, 
                           (uint8_t *)crc, 4);
      
      // Reset.
      HAL_SYSTEM_RESET();
    }
    else
    {
      status = ATT_ERR_INVALID_HANDLE;
    }
  }
  
  return status;
}

/*********************************************************************
*********************************************************************/
