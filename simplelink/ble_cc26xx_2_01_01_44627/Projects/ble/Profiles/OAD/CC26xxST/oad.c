/*******************************************************************************
  Filename:       oad.c
  Revised:        $Date: 2014-09-19 17:55:47 +0200 (fr, 19 sep 2014) $
  Revision:       $Revision: 40206 $

  Description:    This file contains OAD Target implementation.


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
#include "linkdb.h"
#include "att.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "hal_flash.h"

#include "oad_target.h"
#include "oad.h"

#include "bsp_uart.h"
/*********************************************************************
 * CONSTANTS
 */
#define ERROR_BLOCK       0xFFFF

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// OAD Service UUID
static const uint8_t oadServUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(OAD_SERVICE_UUID)
};

static const uint8_t oadCharUUID[OAD_CHAR_CNT][ATT_UUID_SIZE] =
{
 // OAD Image Identify UUID
 TI_BASE_UUID_128(OAD_IMG_IDENTIFY_UUID),

 // OAD Image Block Request/Response UUID
 TI_BASE_UUID_128(OAD_IMG_BLOCK_UUID)
};

#if !defined (OAD_IMAGE_E_USER_ID)
  #define OAD_IMAGE_E_USER_ID  {'E', 'E', 'E', 'E'}
#endif

/*********************************************************************
 * Profile Attributes - variables
 */

// OAD Service attribute
static const gattAttrType_t oadService = { ATT_UUID_SIZE, oadServUUID };

// Place holders for the GATT Server App to be able to lookup handles.
static uint8_t oadCharVals[OAD_CHAR_CNT];

// OAD Characteristic Properties
static uint8_t oadCharProps = GATT_PROP_WRITE_NO_RSP | GATT_PROP_WRITE | GATT_PROP_NOTIFY;

// OAD Client Characteristic Configs
static gattCharCfg_t *oadImgIdentifyConfig;
static gattCharCfg_t *oadImgBlockConfig;

// OAD Characteristic user descriptions
static const uint8_t oadImgIdentifyDesc[] = "Img Identify";
static const uint8_t oadImgBlockDesc[] = "Img Block";

/*********************************************************************
 * Profile Attributes - Table
 */

static gattAttribute_t oadAttrTbl[] =
{
  // OAD Service
  {
    { ATT_BT_UUID_SIZE, primaryServiceUUID },
    GATT_PERMIT_READ,
    0,
    (uint8_t *)&oadService
  },

    // OAD Image Identify Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &oadCharProps
    },

      // OAD Image Identify Characteristic Value
      {
        { ATT_UUID_SIZE, oadCharUUID[0] },
        GATT_PERMIT_WRITE,
        0,
        oadCharVals
      },

      // Characteristic configuration
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8_t *)&oadImgIdentifyConfig
      },

      // OAD Image Identify User Description
      {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        (uint8_t *)oadImgIdentifyDesc
      },

    // OAD Image Block Request/Response Characteristic Declaration
    {
      { ATT_BT_UUID_SIZE, characterUUID },
      GATT_PERMIT_READ,
      0,
      &oadCharProps
    },

      // OAD Image Block Request/Response Characteristic Value
      {
        { ATT_UUID_SIZE, oadCharUUID[1] },
        GATT_PERMIT_WRITE,
        0,
        oadCharVals+1
      },

       // Characteristic configuration
      {
        { ATT_BT_UUID_SIZE, clientCharCfgUUID },
        GATT_PERMIT_READ | GATT_PERMIT_WRITE,
        0,
        (uint8_t *)&oadImgBlockConfig
      },

      // OAD Image Block Request/Response User Description
      {
        { ATT_BT_UUID_SIZE, charUserDescUUID },
        GATT_PERMIT_READ,
        0,
        (uint8_t *)oadImgBlockDesc
      }
};


/*********************************************************************
 * LOCAL VARIABLES
 */

// Intentionally commented out.  
// Used to store OAD application's function for processing a read callback.
static oadWriteCB_t oadTargetWriteCB = NULL;

static uint16_t oadBlkNum = 0, oadBlkTot = 0xFFFF;
static uint32_t imageAddress;
static uint16_t imagePage;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static bStatus_t oadReadAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                               uint8_t *pValue, uint16_t *pLen, uint16_t offset,
                               uint16_t maxLen, uint8_t method);

static bStatus_t oadWriteAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                uint8_t *pValue, uint16_t len, uint16_t offset,
                                uint8_t method);

static void OAD_getNextBlockReq(uint16_t connHandle, uint16_t blkNum);
static void OAD_rejectImage(uint16_t connHandle, img_hdr_t *pImgHdr);

static uint8_t checkDL(void);
static uint16_t crcCalcDL(void);
static uint16_t crc16(uint16_t crc, uint8_t val);

/*********************************************************************
 * PROFILE CALLBACKS
 */ 
CONST gattServiceCBs_t oadCBs =
{
  oadReadAttrCB,  // Read callback function pointer.
  oadWriteAttrCB, // Write callback function pointer.
  NULL            // Authorization callback function pointer.
};

/*********************************************************************
 * @fn      OAD_addService
 *
 * @brief   Initializes the OAD Service by registering GATT attributes
 *          with the GATT server. Only call this function once.
 *
 * @param   None.
 *
 * @return  The return value of GATTServApp_RegisterForMsg().
 */
bStatus_t OAD_addService(void)
{
  // Allocate Client Characteristic Configuration table.
  oadImgIdentifyConfig = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) *
                                                       linkDBNumConns);
  if (oadImgIdentifyConfig == NULL)
  {
    return (bleMemAllocError);
  }
  
  // Allocate Client Characteristic Configuration table.
  oadImgBlockConfig = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) *
                                                    linkDBNumConns);
  
  if (oadImgBlockConfig == NULL)
  {
    // Free already allocated data.
    ICall_free(oadImgIdentifyConfig);
    
    return (bleMemAllocError);
  }
  
  // Initialize Client Characteristic Configuration attributes.
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, oadImgIdentifyConfig);
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, oadImgBlockConfig);

  return GATTServApp_RegisterService(oadAttrTbl, GATT_NUM_ATTRS(oadAttrTbl), 
                                     GATT_MAX_ENCRYPT_KEY_SIZE, &oadCBs);
}

/*********************************************************************
 * @fn      OAD_register
 *
 * @brief   Register a callback function with the OAD Target Profile.
 *
 * @param   None.
 *
 * @return  None.
 */
void OAD_register(oadTargetCBs_t *pfnOadCBs)
{
  // Register a write callback function.
  oadTargetWriteCB = pfnOadCBs->pfnOadWrite;
}

/*********************************************************************
 * @fn      oadReadAttrCB
 *
 * @brief   Read an attribute.
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr      - pointer to attribute
 * @param   pValue     - pointer to data to be read
 * @param   pLen       - length of data to be read
 * @param   offset     - offset of the first octet to be read
 * @param   maxLen     - maximum length of data to be read
 * @param   method     - type of read message 
 *
 * @return  SUCCESS, blePending or Failure
 */
static bStatus_t oadReadAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                               uint8_t *pValue, uint16_t *pLen, uint16_t offset,
                               uint16_t maxLen, uint8_t method)
{
  bStatus_t status = SUCCESS;

  *pLen = 0;
  status = ATT_ERR_INVALID_HANDLE;

  return status;
}

/*********************************************************************
 * @fn      oadWriteAttrCB
 *
 * @brief   Validate and Write attribute data
 *
 * @param   connHandle - connection message was received on
 * @param   pAttr      - pointer to attribute
 * @param   pValue     - pointer to data to be written
 * @param   len        - length of data
 * @param   offset     - offset of the first octet to be written
 * @param   method     - type of write message 
 *
 * @return  SUCCESS, blePending or Failure
 */
static bStatus_t oadWriteAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                uint8_t *pValue, uint16_t len, uint16_t offset,
                                uint8_t method)
{
  bStatus_t status = SUCCESS;

  if (pAttr->type.len == ATT_BT_UUID_SIZE)
  {
    // 16-bit UUID
    uint16_t uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);
    if (uuid == GATT_CLIENT_CHAR_CFG_UUID)
    {
      // Process a CCC write request.
      status = GATTServApp_ProcessCCCWriteReq(connHandle, pAttr, pValue, len,
                                               offset, GATT_CLIENT_CFG_NOTIFY);
    }
    else
    {
      status = ATT_ERR_ATTR_NOT_FOUND; // Should never get here!
    }
  }
  else
  {
    // 128-bit UUID
    if (!memcmp(pAttr->type.uuid, oadCharUUID[OAD_CHAR_IMG_IDENTIFY], 
                ATT_UUID_SIZE))
    {
      /* OAD manager has sent new image information.
       * Read and decide whether to accept or reject an OAD 
       * of this new application image.
       */
      
      // Notify Application
      if (oadTargetWriteCB != NULL)
      {
        (*oadTargetWriteCB)(OAD_WRITE_IDENTIFY_REQ, connHandle, pValue);
      }
    }
    else if (!memcmp(pAttr->type.uuid, oadCharUUID[OAD_CHAR_IMG_BLOCK], 
                         ATT_UUID_SIZE))
    {
      /* OAD is ongoing.  
       * the OAD manager has sent a block from the new image. 
       */
      
      // Notify the application.
      if (oadTargetWriteCB != NULL)
      {
        (*oadTargetWriteCB)(OAD_WRITE_BLOCK_REQ, connHandle, pValue);
      }
    }
    else
    {
      status = ATT_ERR_ATTR_NOT_FOUND; // Should never get here!
    }
  }

  return status;
}

/*********************************************************************
 * @fn      OAD_imgIdentifyWrite
 *
 * @brief   Process the Image Identify Write.  Determined if the image
 *          header identified here should or should not be downloaded by
 *          this application.
 *
 * @param   connHandle - connection message was received on
 * @param   pValue     - pointer to image header data
 *
 * @return  status
 */
void OAD_imgIdentifyWrite(uint16_t connHandle, uint8_t *pValue)
{
  img_hdr_t ImgHdr;
  size_t hdr_offset;

  // Read out running image's header.
  if (OADTarget_hasExternalFlash())
  {
    OADTarget_getCurrentImageHeader(&ImgHdr);
    // CRC part of image identification
    hdr_offset = 4;
  }

  // Calculate block total of the new image.
  oadBlkTot = BUILD_UINT16(pValue[hdr_offset+2], pValue[hdr_offset+3]) / (OAD_BLOCK_SIZE / HAL_FLASH_WORD_SIZE);
  oadBlkNum = 0;

 	bspUartWrite(pValue, 16);
  /* Requirements to begin OAD:
   * 1) LSB of image version cannot be the same, this would imply a code overlap
   *    between currently running image and new image.
   * 2) Total blocks of new image must not exceed maximum blocks supported, else
   *    the new image cannot fit.
   * 3) Block total must be greater than 0.
   * 4) Optional: Add additional criteria for initiating OAD here.
   */
  if (OADTarget_validateNewImage(pValue+hdr_offset, &ImgHdr, oadBlkTot))
  { 
    imageAddress = OADTarget_imageAddress(pValue+hdr_offset);
    imagePage = imageAddress / HAL_FLASH_PAGE_SIZE;

    // Open the target interface
    OADTarget_open();

    // Store the image header
    OADTarget_storeImageHeader(pValue);

    // Image accepted, request block 0.
    OAD_getNextBlockReq(connHandle, 0);
  }
  else
  {
    // Image rejected, send header information of currently running image 
    // to OAD manager.
    OAD_rejectImage(connHandle, &ImgHdr);
  }
}

/*********************************************************************
 * @fn      OAD_imgBlockWrite
 *
 * @brief   Process the Image Block Write.
 *
 * @param   connHandle - connection message was received on
 * @param   pValue - pointer to data to be written
 *
 * @return  status
 */
void OAD_imgBlockWrite(uint16_t connHandle, uint8_t *pValue)
{
  // N.B. This must be left volatile.
  volatile uint16_t blkNum = BUILD_UINT16(pValue[0], pValue[1]);

  // Check that this is the expected block number.
  if (oadBlkNum == blkNum)
  {
    // Calculate address to write as (start of OAD range) + (offset into range)
    uint32_t addr = imageAddress + (oadBlkNum * OAD_BLOCK_SIZE);

    // If address starts a new page, erase that page first.
    if ((addr % HAL_FLASH_PAGE_SIZE) == 0)
    {
      OADTarget_eraseFlash(addr / HAL_FLASH_PAGE_SIZE);
    }

    // Write a 16 byte block to Flash.
    OADTarget_writeFlash(imagePage, (blkNum * OAD_BLOCK_SIZE), pValue+2, OAD_BLOCK_SIZE);
    
    // Increment received block count.
    oadBlkNum++;
  }
  else
  {
    // Overflow, abort OAD
      OADTarget_close();
      OAD_getNextBlockReq(connHandle, ERROR_BLOCK);
  }

  // Check if the OAD Image is complete.
  if (oadBlkNum == oadBlkTot)
  {
    // Run CRC check on new image.
    if (checkDL())
    {
      OADTarget_systemReset();
    }
    OADTarget_close();
  }
  else
  {
    // Request the next OAD Image block.
    OAD_getNextBlockReq(connHandle, oadBlkNum);
  }
}

/*********************************************************************
 * @fn      OAD_getNextBlockReq
 *
 * @brief   Process the Request for next image block.
 *
 * @param   connHandle - connection message was received on
 * @param   blkNum - block number to request from OAD Manager.
 *
 * @return  None
 */
static void OAD_getNextBlockReq(uint16_t connHandle, uint16_t blkNum)
{
  uint16_t value = GATTServApp_ReadCharCfg(connHandle, oadImgBlockConfig);

  // If notifications enabled
  if (value & GATT_CLIENT_CFG_NOTIFY)
  {
    attHandleValueNoti_t noti;
    
    noti.pValue = GATT_bm_alloc(connHandle, ATT_HANDLE_VALUE_NOTI, 2, NULL);
    
    if (noti.pValue != NULL)
    {
      gattAttribute_t *pAttr = GATTServApp_FindAttr(oadAttrTbl, 
                                                    GATT_NUM_ATTRS(oadAttrTbl),
                                                    oadCharVals+OAD_CHAR_IMG_BLOCK);
      
      noti.handle = pAttr->handle;
      noti.len = 2;
      
      noti.pValue[0] = LO_UINT16(blkNum);
      noti.pValue[1] = HI_UINT16(blkNum);
      
      if (GATT_Notification(connHandle, &noti, FALSE) != SUCCESS)
      {
        GATT_bm_free((gattMsg_t *)&noti, ATT_HANDLE_VALUE_NOTI);
      }
    }
  }
}

/*********************************************************************
 * @fn      OAD_rejectImage
 *
 * @brief   Reject the Image identified by the OAD manager, send back
 *          active image version, length and UID to manager.
 *
 * @param   connHandle - connection message was received on.
 * @param   pImgHdr    - pointer to the img_hdr_t data to send.
 *
 * @return  None.
 */
static void OAD_rejectImage(uint16_t connHandle, img_hdr_t *pImgHdr)
{
  uint16_t value = GATTServApp_ReadCharCfg(connHandle, oadImgIdentifyConfig);

  // If notifications enabled
  if (value & GATT_CLIENT_CFG_NOTIFY)
  {
    attHandleValueNoti_t noti;
    
    noti.pValue = GATT_bm_alloc(connHandle, ATT_HANDLE_VALUE_NOTI, 8, NULL);
    
    if (noti.pValue != NULL)
    {
      gattAttribute_t *pAttr = GATTServApp_FindAttr(oadAttrTbl,
                                  GATT_NUM_ATTRS(oadAttrTbl),
                                  oadCharVals+OAD_CHAR_IMG_IDENTIFY);
      
      noti.handle = pAttr->handle;
      noti.len = OAD_IMG_HDR_SIZE;
      
      noti.pValue[0] = LO_UINT16(pImgHdr->ver);
      noti.pValue[1] = HI_UINT16(pImgHdr->ver);

      noti.pValue[2] = LO_UINT16(pImgHdr->len);
      noti.pValue[3] = HI_UINT16(pImgHdr->len);

      (void)memcpy(noti.pValue+4, pImgHdr->uid, sizeof(pImgHdr->uid));

      if (GATT_Notification(connHandle, &noti, FALSE) != SUCCESS)
      {
        GATT_bm_free((gattMsg_t *)&noti, ATT_HANDLE_VALUE_NOTI);
      }
    }
  }
  
  // Close the OAD target if it is open
  OADTarget_close();
}


/*********************************************************************
 * @fn      crcCalcDL
 *
 * @brief   Run the CRC16 Polynomial calculation over the DL image.
 *
 * @param   None
 *
 * @return  The CRC16 calculated.
 */
static uint16_t crcCalcDL(void)
{
  uint16_t imageCRC = 0;
  uint8_t page;
  uint8_t lastPage = oadBlkTot / OAD_BLOCKS_PER_PAGE;
  
  // Remainder of bytes not divisible by the size of a flash page in bytes.
  uint16_t numRemBytes = (oadBlkTot - (lastPage * OAD_BLOCKS_PER_PAGE)) 
                            * OAD_BLOCK_SIZE;

  // Set last page to end of OAD image address range.
  lastPage += imagePage; // OAD_IMG_D_PAGE;

  // Read over downloaded pages
  for (page = imagePage; /*OAD_IMG_D_PAGE*/ page <= lastPage ; page++)
  {
	uint16_t offset;
    // Read over all flash words in a page, excluding the CRC section of the
    // first page and all bytes after remainder bytes on the last page.
    for (offset = (page == imagePage /*OAD_IMG_D_PAGE*/? HAL_FLASH_WORD_SIZE : 0);
                           offset < HAL_FLASH_PAGE_SIZE && 
                           (page < lastPage || offset < numRemBytes ); 
                           offset += HAL_FLASH_WORD_SIZE)
    {
      uint8_t buf[HAL_FLASH_WORD_SIZE];
      uint8_t idx;
      
      // Read a word from flash.
      OADTarget_readFlash(page, offset, buf, HAL_FLASH_WORD_SIZE);

      // Calculate CRC of word, byte by byte.
      for (idx = 0; idx < HAL_FLASH_WORD_SIZE; idx++)
      {
        imageCRC = crc16(imageCRC, buf[idx]);
      }
    }
  }
      
  // IAR note explains that poly must be run with value zero for each byte of crc.
  imageCRC = crc16(imageCRC, 0);
  imageCRC = crc16(imageCRC, 0);
  
  // Return the CRC calculated over the image.
  return imageCRC;      
}

/*********************************************************************
 * @fn      checkDL
 *
 * @brief   Check validity of the downloaded image.
 *
 * @param   None.
 *
 * @return  TRUE or FALSE for image valid.
 */
static uint8_t checkDL(void)
{
  uint16_t crc[2];
  
  OADTarget_getCrc(crc);
  
  if ((crc[0] == 0xFFFF) || (crc[0] == 0x0000))
  {
    return FALSE;
  }

  if (crc[1] == 0xFFFF || crc[1] == 0x0000)
  {
    // Calculate CRC of downloaded image.
    crc[1] = crcCalcDL();
  }

  return crc[0] == crc[1];
}

/*********************************************************************
 * @fn          crc16
 *
 * @brief       Run the CRC16 Polynomial calculation over the byte parameter.
 *
 * @param       crc - Running CRC calculated so far.
 * @param       val - Value on which to run the CRC16.
 *
 * @return      crc - Updated for the run.
 */
static uint16_t crc16(uint16_t crc, uint8_t val)
{
  const uint16_t poly = 0x1021;
  uint8_t cnt;

  for (cnt = 0; cnt < 8; cnt++, val <<= 1)
  {
    uint8_t msb = (crc & 0x8000) ? 1 : 0;

    crc <<= 1;
    
    if (val & 0x80)
    {
      crc |= 0x0001;
    }
    
    if (msb)
    {
      crc ^= poly;
    }
  }

  return crc;
}


/*********************************************************************
*********************************************************************/
