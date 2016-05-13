/**************************************************************************************************
  Filename:       lightservice.h
  Revised:        $Date: 2010-08-06 08:56:11 -0700 (Fri, 06 Aug 2010) $
  Revision:       $Revision: 23333 $

  Description:    This file contains the Lights GATT profile definitions and
                  prototypes.

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
**************************************************************************************************/

#ifndef LIGHTSERVICE_H
#define LIGHTSERVICE_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
 * CONSTANTS
 */

// Profile Parameters
#define LIGHTSPROFILE_RED                   0  // RW uint8 - Profile Characteristic 1 value 
#define LIGHTSPROFILE_GREEN                 1  // RW uint8 - Profile Characteristic 2 value
#define LIGHTSPROFILE_BLUE                  2  // RW uint8 - Profile Characteristic 3 value
#define LIGHTSPROFILE_WHITE                 3  // RW uint8 - Profile Characteristic 4 value
#define LIGHTSPROFILE_RGBW                  4  // RW uint8 - Profile Characteristic 4 value
  
// Lights Profile Service UUID
#define LIGHTSPROFILE_SERV_UUID             0xFFB0
    
// Key Pressed UUID
#define LIGHTSPROFILE_RED_UUID              0xFFB1
#define LIGHTSPROFILE_GREEN_UUID            0xFFB2
#define LIGHTSPROFILE_BLUE_UUID             0xFFB3
#define LIGHTSPROFILE_WHITE_UUID            0xFFB4
#define LIGHTSPROFILE_RGBW_UUID             0xFFB5
  
// Lights Keys Profile Services bit fields
#define LIGHTSPROFILE_SERVICE               0x00000001

// Length of Characteristic 5 in bytes
#define LIGHTSPROFILE_RGBW_LEN              4  

/*********************************************************************
 * TYPEDEFS
 */

  
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * Profile Callbacks
 */

// Callback when a characteristic value has changed
typedef NULL_OK void (*lightsProfileChange_t)( uint8 paramID );

typedef struct
{
  lightsProfileChange_t        pfnLightsProfileChange;  // Called when characteristic value changes
} lightsProfileCBs_t;

    
/*********************************************************************
 * API FUNCTIONS 
 */


/*
 * Lights_addService- Initializes the Lights GATT Profile service by registering
 *          GATT attributes with the GATT server.
 *
 */

extern bStatus_t Lights_addService(void);

/*
 * Lights_registerAppCBs - Registers the application callback function.
 *                    Only call this function once.
 *
 *    appCallbacks - pointer to application callbacks.
 */
extern bStatus_t Lights_registerAppCBs(lightsProfileCBs_t *appCallbacks);

/*
 * Lights_setParameter - Set a Lights GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    len - length of data to right
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 */
extern bStatus_t Lights_setParameter(uint8_t param, uint8_t len, void *value);
  
/*
 * Lights_setParameter - Get a Lights GATT Profile parameter.
 *
 *    param - Profile parameter ID
 *    value - pointer to data to write.  This is dependent on
 *          the parameter ID and WILL be cast to the appropriate 
 *          data type (example: data type of uint16 will be cast to 
 *          uint16 pointer).
 */
extern bStatus_t Lights_getParameter(uint8_t param, void *value);


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* LIGHTSERVICE_H */
