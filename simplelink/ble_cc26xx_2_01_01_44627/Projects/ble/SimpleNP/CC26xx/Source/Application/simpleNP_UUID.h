/**************************************************************************************************
  Filename:       simpleNP_UUID.h
  Revised:        $Date: 2015-06-19 16:22:24 -0700 (Fri, 19 Jun 2015) $
  Revision:       $Revision: 44160 $

  Description:    This file contains the UUID management for 
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

#ifndef SIMPLENP_UUID_H
#define SIMPLENP_UUID_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
  
#include "gatt_profile_uuid.h"
#include "gatt_uuid.h"
#include "simpleGATTprofile.h"
  
/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */


/*********************************************************************
 * MACROS
 */
  
 // Bluetooth base 128-bit UUID: 00000000-0000-1000-8000-00805F9B34FB
#define BT_BASE_UUID_128(uuid) 0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, \
                               0x00, 0x80, 0x00, 0x10, 0x00, 0x00, \
                               LO_UINT16( uuid ), HI_UINT16( uuid ), 0x00, 0x00

// Vendor base 128-bit UUID: 00000000-0000-1000-8000-0026BB765291
#define VENDOR_BASE_1_UUID_128(uuid) 0x91, 0x52, 0x76, 0xBB, 0x26, 0x00, \
                                     0x00, 0x80, 0x00, 0x10, 0x00, 0x00, \
                                LO_UINT16( uuid ), HI_UINT16( uuid ), 0x00, 0x00

/*********************************************************************
 * TYPDEFS
 */

/*********************************************************************
 * FUNCTIONS
 */
                                  
extern const uint8_t *SNP_findUUIDRec(uint8_t *pUUID, uint8_t len,
                                      uint8_t *newLen);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLENP_UUID_H */
