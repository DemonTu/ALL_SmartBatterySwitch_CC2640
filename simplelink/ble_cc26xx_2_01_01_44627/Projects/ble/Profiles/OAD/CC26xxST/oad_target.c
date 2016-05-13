/*******************************************************************************
  Filename:       oad_target.c
  Revised:        $Date: 2012-11-16 18:39:26 -0800 (Fri, 16 Nov 2012) $
  Revision:       $Revision: 32218 $

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
#include "bcomdef.h"
#include "linkdb.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"

#include "Board.h"
#include "GPIO.h"
#include "ext_flash.h"
#include "sensor.h"
#include <string.h>

#ifdef POWER_SAVING
#include <ti/sysbios/family/arm/cc26xx/Power.h>
#include <ti/sysbios/family/arm/cc26xx/PowerCC2650.h>
#endif

#include "oad.h"
#include "oad_target.h"

/*********************************************************************
 * CONSTANTS
 */

// To make buzzer beep during OAD
#if !defined (OAD_IMAGE_VERSION)
#define OAD_IMAGE_VERSION   0x0000
#endif

#if !defined (OAD_IMAGE_A_USER_ID)
  #define OAD_IMAGE_A_USER_ID  {'E', 'E', 'E', 'E'}
#endif

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// OAD Service UUID
static CONST uint8_t oadServUUID[ATT_UUID_SIZE] =
{
  TI_BASE_UUID_128(OAD_SERVICE_UUID)
};

static CONST uint8_t oadCharUUID[OAD_CHAR_CNT][ATT_UUID_SIZE] =
{
 // OAD Image Identify UUID
 TI_BASE_UUID_128(OAD_IMG_IDENTIFY_UUID),

 // OAD Image Block Request/Response UUID
 TI_BASE_UUID_128(OAD_IMG_BLOCK_UUID)
};

/*********************************************************************
 * Profile Attributes - variables
 */

// OAD Service attribute
static const gattAttrType_t oadService = { ATT_UUID_SIZE, oadServUUID };

// Place holders for the GATT Server App to be able to lookup handles.
static uint8_t oadCharVals[OAD_CHAR_CNT];

// OAD Characteristic Properties
static uint8_t oadCharProps = GATT_PROP_WRITE_NO_RSP | GATT_PROP_WRITE 
    | GATT_PROP_NOTIFY;

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

#pragma location="IMAGE_HEADER"
const img_hdr_t _imgHdr = 
{
  OAD_IMG_VER( OAD_IMAGE_VERSION ), // 15-bit Version #, left-shifted 1; 
                                    // OR with Image-B/Not-A bit.
  OAD_IMG_A_AREA * OAD_FLASH_PAGE_MULT,
  OAD_IMAGE_A_USER_ID,        // User-Id
  { 0xFF, 0xFF, 0xFF, 0xFF }  // Reserved
};
#pragma required=_imgHdr

/*********************************************************************
 * LOCAL VARIABLES
 */

static oadWriteCB_t oadTargetWriteCB = NULL;
static uint16_t oadBlkNum;
static uint16_t oadBlkTot;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static bStatus_t oadWriteAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                uint8_t *pValue, uint16_t len, uint16_t offset,
                                uint8_t method);
static const gattServiceCBs_t oadCBs =
{
  NULL,           // Read callback function pointer.
  oadWriteAttrCB, // Write callback function pointer.
  NULL            // Authorization callback function pointer.
};

// CRC16 functions
static uint16_t crc16(uint16_t crc, uint8_t val);
static uint8_t checkDL(void);

// Processing received image
static void OADTarget_getNextBlockReq(uint16_t connHandle, uint16_t blkNum);
static void OADTarget_rejectImage(uint16_t connHandle, img_hdr_t *pImgHdr);

/*********************************************************************
 * @fn      OADTarget_addService
 *
 * @brief   Initializes the OAD Service by registering GATT attributes
 *          with the GATT server. Only call this function once.
 *
 * @return  The return value of GATTServApp_RegisterForMsg().
 */
bStatus_t OADTarget_addService(void)
{
  // Allocate Client Characteristic Configuration table
  oadImgIdentifyConfig = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) *
                                                       linkDBNumConns);
  if (oadImgIdentifyConfig == NULL)
  {
    return (bleMemAllocError);
  }
  
  // Allocate Client Characteristic Configuration table
  oadImgBlockConfig = (gattCharCfg_t *)ICall_malloc(sizeof(gattCharCfg_t) *
                                                    linkDBNumConns);
  
  if (oadImgBlockConfig == NULL)
  {
    // Free already allocated data
    ICall_free(oadImgIdentifyConfig);
    
    return (bleMemAllocError);
  }
  
  // Initialize Client Characteristic Configuration attributes
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, oadImgIdentifyConfig);
  GATTServApp_InitCharCfg(INVALID_CONNHANDLE, oadImgBlockConfig);

  return GATTServApp_RegisterService(oadAttrTbl, GATT_NUM_ATTRS(oadAttrTbl), 
                                     GATT_MAX_ENCRYPT_KEY_SIZE, &oadCBs);
}

/*********************************************************************
 * @fn      OADTarget_register
 *
 * @brief   Register a callback function with the OAD Target Profile.
 *
 * @return  None
 */
void OADTarget_register(oadTargetCBs_t *pfnOadCBs)
{
  // Register a write callback function.
  oadTargetWriteCB = pfnOadCBs->pfnOadWrite;
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
      /* OAD manager is identifying the new image.
       * The manager presents header information by which this device can
       * accept or reject an OAD of a new application image.
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
      /* The OAD manager has sent a block from the new image. */
      
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

//
// SensorTag OAD application part
//

#undef OAD_IMG_D_PAGE

#define FIRST_PAGE          ( APP_IMAGE_START / HAL_FLASH_PAGE_SIZE )

static bool flashOk;

/*********************************************************************
 * @fn      OADTarget_imgIdentifyWrite
 *
 * @brief   Process the Image Identify Write.  Determined if the image
 *          header identified here should or should not be downloaded by
 *          this application.
 *
 * @param   connHandle - connection message was received on
 * @param   pValue - pointer to image header data
 *
 * @return  status
 */
bStatus_t OADTarget_imgIdentifyWrite(uint16_t connHandle, uint8_t *pValue)
{
  static img_hdr_t rxHdr;
  static img_hdr_t ImgHdr;
  
  oadBlkNum = 0;

  rxHdr.ver = BUILD_UINT16(pValue[0], pValue[1]);
  rxHdr.len = BUILD_UINT16(pValue[2], pValue[3]);
  
  (void)memcpy(rxHdr.uid, pValue+4, sizeof(rxHdr.uid));

  // Read out running image's header.
  uint8_t *flashAddr = (uint8_t *)(APP_IMAGE_START + OAD_IMG_HDR_OSET);
  memcpy(&ImgHdr,flashAddr,sizeof(img_hdr_t));
  
  // Calculate block total of the new image.
  // total Blocks = (length in HAL Flash words) / ((16 Byte Block)/(4 Byte Word))
  oadBlkTot = rxHdr.len / (OAD_BLOCK_SIZE / HAL_FLASH_WORD_SIZE);

  /* Requirements to begin OAD:
   * 1) LSB of image version cannot be the same, this would imply a code overlap
   *    between currently running image and new image, which is illegal.
   * 2) Total blocks of new image must not exceed maximum blocks supported, else
   *    the new image cannot fit.
   * 3) New image must not be empty header.
   * 4) Optional: Add customer criteria for initiating OAD here.
   */
  if ( (oadBlkTot <= OAD_BLOCK_MAX) && (oadBlkTot != 0))
  {
    oadBlkNum = 0;
    
    // Image accepted, request image.
    OADTarget_getNextBlockReq(connHandle, 0);
  }
  else
  {
    // Image rejected, identify current image to OAD manager.
    OADTarget_rejectImage(connHandle, &ImgHdr);
  }

  // Ready to go: open flash driver
  flashOk = extFlashOpen();
  
  return (flashOk ? SUCCESS : FAILURE);
}


/*********************************************************************
 * @fn      OADTarget_imgBlockWrite
 *
 * @brief   Process the Image Block Write.
 *
 * @param   connHandle - connection message was received on
 * @param   pValue - pointer to data to be written
 *
 * @return  status
 */
bStatus_t OADTarget_imgBlockWrite(uint16_t connHandle, uint8_t *pValue)
{
  volatile uint16_t blkNum;
 
  blkNum  = BUILD_UINT16(pValue[0], pValue[1]);

  // First block of OAD which included image header and CRC and CRC shadow
  // values. Do a sanity check on the received image header
  if (blkNum == 0)
  {
    img_hdr_t ImgHdr;
    uint16_t blkTot;
   
    blkTot = BUILD_UINT16(pValue[8], pValue[9]) / 
                                  (OAD_BLOCK_SIZE / HAL_FLASH_WORD_SIZE);

    // Read out running image's header.
    uint8_t *flashAddr = (uint8_t *)(APP_IMAGE_START + OAD_IMG_HDR_OSET);
    memcpy(&ImgHdr,flashAddr,sizeof(img_hdr_t));

    // Note:  if additional customer criteria was checked in the Image
    // Identification step, it may be important to check again here.
    if ((oadBlkNum != blkNum) || (oadBlkTot != blkTot) )
    {
      // Cancel download
      OADTarget_rejectImage(connHandle, &ImgHdr);

      // NB! This is meaningless for a WriteNoResp operation
      return (ATT_ERR_WRITE_NOT_PERMITTED);
    }
#ifdef POWER_SAVING
    Power_setConstraint(Power_SB_DISALLOW);
#endif
  }

  // Check that this is the expected block number.
  if (oadBlkNum == blkNum && flashOk)
  {
    uint32_t addr;
   
    // Calculate address to write as (start of OAD range) + (offset)
    addr = APP_IMAGE_START + oadBlkNum * OAD_BLOCK_SIZE;

    // If address starts a new page, erase that page first.
    if ((addr % HAL_FLASH_PAGE_SIZE) == 0)
    {
      flashOk = extFlashErase(addr, HAL_FLASH_PAGE_SIZE);
    }

    // Write a 16 byte block to Flash.
    if (flashOk)
    {
      flashOk = extFlashWrite(addr, OAD_BLOCK_SIZE, pValue+2);
    
      // Increment received block count.
      if (flashOk)
        oadBlkNum++;
    }

    // Toggle Green LED for every 8th block
    if ( (oadBlkNum % 8) == 0)
    {
      GPIO_toggle(Board_LED2);
    }
  }
  else
  {
    img_hdr_t ImgHdr;
    
     // Toggle RED LED and sound buzzer when overflow
     GPIO_toggle(Board_LED1);
     GPIO_toggle(Board_BUZZER);
#ifdef POWER_SAVING
    Power_releaseConstraint(Power_SB_DISALLOW);
#endif     
    
    // Cancel download
    ImgHdr.len = 0; // Don't care content
    OADTarget_rejectImage(connHandle, &ImgHdr);
  }
  
  // Check if the OAD Image is complete.
  if (oadBlkNum == oadBlkTot)
  {
    extFlashClose();
    
    // Run CRC check on new image.
    if (checkDL())
    {
      HAL_SYSTEM_RESET();
    }
    else
    {
      GPIO_toggle(Board_LED1);
    }
#ifdef POWER_SAVING
    Power_releaseConstraint(Power_SB_DISALLOW);
#endif     
  }
  else
  {
    // Request the next OAD Image block.
    OADTarget_getNextBlockReq(connHandle, oadBlkNum);
  }

  return (SUCCESS);
}

/*********************************************************************
 * @fn      OADTarget_getNextBlockReq
 *
 * @brief   Process the Request for next image block.
 *
 * @param   connHandle - connection message was received on
 * @param   blkNum - block number to request from OAD Manager.
 *
 * @return  None
 */
static void OADTarget_getNextBlockReq(uint16_t connHandle, uint16_t blkNum)
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
 * @fn      OADTarget_rejectImage
 *
 * @brief   Reject the Image identified by the OAD manager, send back
 *          active image version, length and UID to manager.
 *
 * @param   connHandle - connection message was received on.
 * @param   pImgHdr    - pointer to the img_hdr_t data to send.
 *
 * @return  None.
 */
static void OADTarget_rejectImage(uint16_t connHandle, img_hdr_t *pImgHdr)
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
}

/*********************************************************************
 * @fn          crcCalcDL
 *
 * @brief       Run the CRC16 Polynomial calculation over the DL image.
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      The CRC16 calculated.
 */
static uint16_t crcCalcDL(void)
{
  volatile uint16_t imageCRC;
  uint16_t remBytes;
  uint8_t lastPage;
  
  lastPage = ( oadBlkTot / OAD_BLOCKS_PER_PAGE );
  remBytes = (oadBlkTot - (lastPage * OAD_BLOCKS_PER_PAGE)) * OAD_BLOCK_SIZE;
  lastPage += FIRST_PAGE;
  imageCRC = 0;
  
  for (uint8_t page = APP_IMAGE_START / HAL_FLASH_PAGE_SIZE; ; page++)
  {
    for (uint16_t offset = 0; offset < HAL_FLASH_PAGE_SIZE; 
                              offset += HAL_FLASH_WORD_SIZE)
    {
      if ((page == FIRST_PAGE) && (offset == OAD_IMG_CRC_OSET))
      {
        continue;  // Skip the CRC and shadow.
      }
      else if ((page == lastPage) && (offset == remBytes))
      {
        // IAR note explains that poly must be run with value 
        // zero for each byte of crc.
        imageCRC = crc16(imageCRC, 0);
        imageCRC = crc16(imageCRC, 0);
        
        return imageCRC;
      }
      else
      {
        uint8_t buf[HAL_FLASH_WORD_SIZE];
        uint16_t addr;
        
        // Read from external flash and calculate CRC
        addr = (page * HAL_FLASH_PAGE_SIZE) + offset;
        extFlashRead(addr, HAL_FLASH_WORD_SIZE, buf);
        for (uint8_t idx = 0; idx < HAL_FLASH_WORD_SIZE; idx++)
        {
          imageCRC = crc16(imageCRC, buf[idx]);
        }
      }
    }
  }
}

/*********************************************************************
 * @fn          checkDL
 *
 * @brief       Check validity of the downloaded image.
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      TRUE or FALSE for image valid.
 */
static uint8_t checkDL(void)
{
  bool ret;
  uint16_t crc[2];

  ret = extFlashOpen();
  if (ret)
  {
    ret = extFlashRead((size_t)APP_IMAGE_START, sizeof(crc), (uint8_t*)crc);
    
    if (ret) 
    {
      if ((crc[0] == 0xFFFF) || (crc[0] == 0x0000))
      {
        ret = false;
      }
      
      if (crc[1] == 0xFFFF || crc[1] == 0x0000)
      {
        crc[1] = crcCalcDL();
      }
      
      ret = crc[0] == crc[1];
    }
    
    extFlashClose();
  }
  
  return ret ? TRUE : FALSE;
}

/**************************************************************************************************
 * @fn          crc16
 *
 * @brief       Run the CRC16 Polynomial calculation over the byte parameter.
 *
 * input parameters
 *
 * @param       crc - Running CRC calculated so far.
 * @param       val - Value on which to run the CRC16.
 *
 * output parameters
 *6
 * None.
 *
 * @return      crc - Updated for the run.
 **************************************************************************************************
 */
static uint16_t crc16(uint16_t crc, uint8_t val)
{
  const uint16_t poly = 0x1021;
  uint8_t cnt;

  for (cnt = 0; cnt < 8; cnt++, val <<= 1)
  {
    uint8_t msb = (crc & 0x8000) ? 1 : 0;

    crc <<= 1;
    if (val & 0x80)  crc |= 0x0001;
    if (msb)         crc ^= poly;
  }

  return crc;
}

/*********************************************************************
*********************************************************************/
