/*******************************************************************************
  Filename:       oad.h
  Revised:        $Date: 2014-09-19 17:55:47 +0200 (fr, 19 sep 2014) $
  Revision:       $Revision: 40206 $

  Description:    This file contains OAD Profile header file.


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
#ifndef OAD_H
#define OAD_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

#include "hal_types.h"
#include <ti/sysbios/knl/Queue.h>
  
/*********************************************************************
 * CONSTANTS
 */

#define OAD_SERVICE_UUID      0xFFC0
#define OAD_IMG_IDENTIFY_UUID 0xFFC1
#define OAD_IMG_BLOCK_UUID    0xFFC2

// Offsets in bytes.
#define OAD_IMG_CRC_OSET      0x0000

// Image header is placed 4 bytes above OAD_IMG_CRC_OSET.  the CRC and CRC
// shadow are NOT part of the image header.
#define OAD_IMG_HDR_OSET      0x0004

#define OAD_CHAR_CNT          2

// OAD Characteristic Indices
#define OAD_CHAR_IMG_IDENTIFY 0
#define OAD_CHAR_IMG_BLOCK    1

// Image Identification size
#define OAD_IMG_ID_SIZE       4

// Image header size (version + length + image id size)
#define OAD_IMG_HDR_SIZE      ( 2 + 2 + OAD_IMG_ID_SIZE )

// The Image is transported in 16-byte blocks in order to avoid using blob operations.
#define OAD_BLOCK_SIZE        16
#define OAD_BLOCKS_PER_PAGE  (HAL_FLASH_PAGE_SIZE / OAD_BLOCK_SIZE)
#define OAD_BLOCK_MAX        (OAD_BLOCKS_PER_PAGE * OAD_IMG_D_AREA)

//Callback Events
#define OAD_WRITE_IDENTIFY_REQ        0x01
#define OAD_WRITE_BLOCK_REQ           0x02

  
/*********************************************************************
 * MACROS
 */
    
/*
 * OAD_IMG_VER() sets the least significant bit of the build version to signify
 * which image region the image is built to run in.  If the LSB is set to 1, 
 * then the Image B region is intended.  Otherwise, it is built for the image A
 * region.  When that builds Image Header is sent OTA to the currently running 
 * Image, OAD_IMG_ID() is performed as a simple check to make sure that the 
 * current Image running and  the Image to OTD are not both and Image A or 
 * Image B builds.  OAD_VER_NUM() unmodifies the OAD image version number for
 * uses such as writing to the LCD.
 */
      
// Macros to get Image ID (LSB) and Version Number
#define OAD_IMG_ID( ver )    ( (ver) & 0x01 )
#define OAD_VER_NUM( ver )   ( (ver) >> 0x01 )

// Macro to set Image Version
#if defined (HAL_IMAGE_A)
  // Clear LSB for Image A
  #define OAD_IMG_VER( ver ) ( (uint16)( (ver) << 0x01 ) )
#else
  // Set LSB for Image B
  #define OAD_IMG_VER( ver ) ( (uint16)( ( (ver) << 0x01 ) | 0x01 ) )
#endif

  // Number of HAL Flash Words per Flash page
#define OAD_FLASH_PAGE_MULT  ((uint16)(HAL_FLASH_PAGE_SIZE / HAL_FLASH_WORD_SIZE))

/*********************************************************************
 * TYPEDEFS
 */
  
/* CRC is not included in the header; instead, IAR's 2 byte CRC is placed 4 
 * bytes before the header at the very start of the image. In the 2 bytes 
 * between IAR's CRC and the image header resides the shadow CRC.
 */ 
typedef struct {
  uint16 ver;
  uint16 len;    // Image length in 4-byte blocks (i.e. HAL_FLASH_WORD_SIZE blocks).
  uint8  uid[4]; // User-defined Image Identification bytes.
  uint8  res[4]; // Reserved space for future use.
} img_hdr_t;

typedef struct
{
  Queue_Elem _elem;
  uint8_t  event;
  uint16_t connHandle;
  uint8_t  *pData;
} oadTargetWrite_t;

/*********************************************************************
 * EXTERNAL VARIABLES
 */

  // OAD Image Header
extern const img_hdr_t _imgHdr;

/*********************************************************************
 * Profile Callbacks
 */

// Callback when a characteristic value has changed
typedef void (*oadReadCB_t)(void);
typedef void (*oadWriteCB_t)(uint8_t event, uint16_t connHandle, 
                             uint8_t *pData);

typedef struct
{
  oadReadCB_t        pfnOadRead;  // Called when characteristic value is read.
  oadWriteCB_t       pfnOadWrite; // Called when characteristic value changes.
} oadTargetCBs_t;

/*********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      OAD_addService
 *
 * @brief   Initializes the OAD Service by registering GATT attributes
 *          with the GATT server. Only call this function once.
 *
 * @param   None.
 *
 * @return  Success or Failure
 */
extern uint8_t OAD_addService(void);

/*********************************************************************
 * @fn      OAD_register
 *
 * @brief   Register a callback function with the OAD Target Profile.
 * 
 * @param   pfnOadCBs - struct holding function pointers to OAD application
 *                      callbacks.
 *
 * @return  None
 */
extern void OAD_register(oadTargetCBs_t *pfnOadCBs);

/*********************************************************************
 * @fn      OAD_imgIdentifyWrite
 *
 * @brief   Process the Image Identify Write.  Determine from the received OAD
 *          Image Header if the Downloaded Image should be acquired.
 *
 * @param   connHandle - connection message was received on
 * @param   pValue     - pointer to data to be written
 *
 * @return  status
 */
extern void OAD_imgIdentifyWrite(uint16 connHandle, uint8 *pValue);

/*********************************************************************
 * @fn      OAD_imgBlockWrite
 *
 * @brief   Process the Image Block Write.
 *
 * @param   connHandle - connection message was received on
 * @param   pValue     - pointer to data to be written
 *
 * @return  status
 */
extern void OAD_imgBlockWrite(uint16 connHandle, uint8 *pValue);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* OAD_H */
