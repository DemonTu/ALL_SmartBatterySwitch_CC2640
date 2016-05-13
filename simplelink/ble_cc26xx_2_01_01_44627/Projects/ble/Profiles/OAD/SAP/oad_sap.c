/*******************************************************************************
  Filename:       oad_sap.c
  Revised:        $Date: 2015-07-23 11:48:29 -0700 (Thu, 23 Jul 2015) $
  Revision:       $Revision: 44402 $

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

#include "SNP.h"
#include "snp_rpc.h"
#include "snp_rpc_synchro.h"
#include "sap.h"

#include "hal_flash.h"
#include "hal_defs.h"

#include <driverlib/rom.h>
#include <driverlib/vims.h>

#include "oad_sap.h"
#include "oad_target.h"

/*********************************************************************
 * CONSTANTS
 */

#define OAD_DEFAULT_CCCD        0x0000
#define OAD_DEFAULT_IMGCNT      0x01

#define ERROR_BLOCK             0xFFFF

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

// OAD Service UUID
static uint8_t oadServUUID[SNP_128BIT_UUID_SIZE] =
{
  TI_BASE_UUID_128(OAD_SERVICE_UUID)
};

static uint8_t oadCharUUID[OAD_CHAR_CNT][SNP_128BIT_UUID_SIZE] =
{
 // OAD Image Identify UUID
 TI_BASE_UUID_128(OAD_IMG_IDENTIFY_UUID),

 // OAD Image Block Request/Response UUID
 TI_BASE_UUID_128(OAD_IMG_BLOCK_UUID),
 
 // OAD Image Count UUID
 TI_BASE_UUID_128(OAD_IMG_COUNT_UUID)
};

/*********************************************************************
 * Profile Attributes - variables
 */

// OAD Service attribute
static UUIDType_t oadServiceUUID = { SNP_128BIT_UUID_SIZE, oadServUUID };

// OAD Client Characteristic Configs
SAP_UserCCCDAttr_t oadImgIdentifyConfig = {SNP_GATT_PERMIT_READ | SNP_GATT_PERMIT_WRITE};
SAP_UserCCCDAttr_t oadImgBlockConfig = {SNP_GATT_PERMIT_READ | SNP_GATT_PERMIT_WRITE};

uint16_t oadImgIdentifyCCCDFlag = OAD_DEFAULT_CCCD;
uint16_t oadImgBlockCCCDFlag = OAD_DEFAULT_CCCD;

// OAD Characteristic user descriptions
static uint8_t oadImgIdentifyDescStr[] = "Img Identify";
static uint8_t oadImgBlockDescStr[] = "Img Block";
static uint8_t oadImgCountDescStr[] = "Img Count";

SAP_UserDescAttr_t oadImgIdentifyDesc = {SNP_GATT_PERMIT_READ, SNP_GATT_CHAR_MAX_LENGTH, 
                                        sizeof(oadImgIdentifyDescStr), oadImgIdentifyDescStr};
SAP_UserDescAttr_t oadImgBlockDesc = {SNP_GATT_PERMIT_READ, SNP_GATT_CHAR_MAX_LENGTH, 
                                     sizeof(oadImgBlockDescStr), oadImgBlockDescStr};
SAP_UserDescAttr_t oadImgCountDesc = {SNP_GATT_PERMIT_READ, SNP_GATT_CHAR_MAX_LENGTH, 
                                     sizeof(oadImgCountDescStr), oadImgCountDescStr};

/*********************************************************************
 * Profile Attributes - Table
 */
   
// OAD Service Characteristics
static SAP_Char_t oadCharTbl[OAD_CHAR_CNT] =
{
  //  OAD Image Identity Characteristic Declaration
  { 
    { SNP_128BIT_UUID_SIZE, oadCharUUID[OAD_CHAR_IMG_IDENTIFY] }, /* UUID             */
    SNP_GATT_PROP_WRITE_NORSP | SNP_GATT_PROP_WRITE |             /* Properties       */     
    SNP_GATT_PROP_NOTIFICATION | SNP_GATT_PROP_READ,              
    SNP_GATT_PERMIT_WRITE,                                        /* Value Permissions*/
    &oadImgIdentifyDesc,                                          /* User Description */
    &oadImgIdentifyConfig,                                        /* CCCD             */
    NULL                                                         /* Format           */        
  },
  
  // OAD Image Block Request/Response Characteristic Declaration
  { 
    { SNP_128BIT_UUID_SIZE, oadCharUUID[OAD_CHAR_IMG_BLOCK] },    /* UUID             */
    SNP_GATT_PROP_WRITE_NORSP | SNP_GATT_PROP_WRITE |             /* Properties       */     
    SNP_GATT_PROP_NOTIFICATION | SNP_GATT_PROP_READ,              
    SNP_GATT_PERMIT_WRITE,                                        /* Value Permissions*/
    &oadImgBlockDesc,                                             /* User Description */
    &oadImgBlockConfig                                            /* CCCD             */
                                                                  /* Format           */
  },
  
  // OAD Image Count Characteristic Declaration
  { 
    { SNP_128BIT_UUID_SIZE, oadCharUUID[OAD_CHAR_IMG_COUNT] },    /* UUID             */
    SNP_GATT_PROP_WRITE_NORSP | SNP_GATT_PROP_WRITE |             /* Properties       */     
    SNP_GATT_PROP_NOTIFICATION | SNP_GATT_PROP_READ,              
    SNP_GATT_PERMIT_WRITE,                                        /* Value Permissions*/
    &oadImgCountDesc                                              /* User Description */
                                                                  /* CCCD             */
                                                                  /* Format           */
  },  
};

/*********************************************************************
 * LOCAL VARIABLES
 */

// Intentionally commented out.  
// Used to store OAD application's function for processing a read callback.
//static oadReadCB_t oadTargetReadCB = NULL;

static SAP_Service_t oadService;
static SAP_CharHandle_t oadServiceCharHandles[OAD_CHAR_CNT];

static oadWriteCB_t oadTargetWriteCB = NULL;

static uint16_t oadBlkNum = 0, oadBlkTot = 0xFFFF;
static uint32_t imageAddress;
static uint16_t imagePage;

#ifndef FEATURE_OAD_ONCHIP
// Used to keep a record of the last write to the OAD_CHAR_IMG_COUNT
// characteristic.
static uint8_t imgCount = OAD_DEFAULT_IMGCNT;
static uint8_t flagRecord = 0;
#endif //FEATURE_OAD_ONCHIP

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void OAD_processSNPEventCB(uint16_t event, snpEventParam_t *param);

static void OAD_getNextBlockReq(uint16_t connHandle, uint16_t blkNum);
static void OAD_rejectImage(uint16_t connHandle, img_hdr_t *pImgHdr);

#ifndef FEATURE_OAD_ONCHIP
static uint8_t CheckImageDownloadCount(void);
static uint8_t checkDL(void);
static uint16_t crcCalcDL(void);
static uint16_t crc16(uint16_t crc, uint8_t val);
#endif

/*********************************************************************
 * PROFILE CALLBACKS
 */ 

static bStatus_t oadReadAttrCB(void *context, uint16_t conn, 
                               uint16_t charHdl, uint16_t offset, 
                               uint16_t size, uint16_t * pLen, 
                               uint8_t *pData);

static bStatus_t oadWriteAttrCB(void *context, uint16_t conn,
                                uint16_t charHdl, uint16_t len,
                                uint8_t *pData);

static bStatus_t oadCCCDIndCB(void *context, uint16_t conn,
                              uint16_t cccdHdl, uint8_t type,
                              uint16_t value);

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
  // Register to recieve Connection Established Events
  SAP_registerEventCB(OAD_processSNPEventCB, SNP_CONN_EST_EVT);
  
  // Build Service to register with NP
  oadService.serviceUUID       = oadServiceUUID;
  oadService.serviceType       = SNP_PRIMARY_SERVICE;
  oadService.charTableLen      = OAD_CHAR_CNT;
  oadService.charTable         = oadCharTbl;
  oadService.context           = NULL;
  oadService.charReadCallback  = oadReadAttrCB;
  oadService.charWriteCallback = oadWriteAttrCB;
  oadService.cccdIndCallback   = oadCCCDIndCB;
  oadService.charAttrHandles   = oadServiceCharHandles;
   
  // Service is set up, register with GATT server on the SNP.
  return SAP_registerService(&oadService);
}

/*********************************************************************
 * @fn      OAD_register
 *
 * @brief   Register a callback function with the OAD Target Profile.
 *
 * @param   pfnOadCBs - call back functions
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
 * @brief   Read an attribute. This should never be invoked 
 *          since we have no readable value attributes
 *
 * @param   context - context used when registering service
 * @param   conn    - connection handle ID
 * @param   charHdl - characteristic value handle
 * @param   offset  - offset of data to be read
 * @param   size    - maximum size of data bytes to be read
 * @param   pLen    - amount of bytes copied into pData
 * @param   pData   - pointer to copy read data
 *
 * @return  SNP_UNKNOWN_ATTRIBUTE
 */
static bStatus_t oadReadAttrCB(void *context, uint16_t conn, 
                               uint16_t charHdl, uint16_t offset, 
                               uint16_t size, uint16_t * pLen, 
                               uint8_t *pData)
{
  return SNP_UNKNOWN_ATTRIBUTE;
}

/*********************************************************************
 * @fn      oadCCCDIndCB
 *
 * @brief   CCCD Attribute is written to
 *
 * @param   context - context used when registering service
 * @param   conn    - connection handle ID
 * @param   cccdHdl - characteristic cccd handle
 * @param   type    - Confirmation needed, handled by NP
 * @param   value   - cccd value written
 *
 * @return  SNP_SUCCESS, or SNP_UNKNOWN_ATTRIBUTE
 */
static bStatus_t oadCCCDIndCB(void *context, uint16_t conn,
                              uint16_t cccdHdl, uint8_t type,
                              uint16_t value)
{
  // Process a CCCD update indication
  bStatus_t status = SNP_SUCCESS;

  // Determine which characteristic is being written to
  if (oadServiceCharHandles[OAD_CHAR_IMG_IDENTIFY].cccdHandle ==
          cccdHdl)
  {
    oadImgIdentifyCCCDFlag = value;
  }
  else if (oadServiceCharHandles[OAD_CHAR_IMG_BLOCK].cccdHandle ==
              cccdHdl)
  {
    oadImgBlockCCCDFlag = value;
  }
  else
  {
    status = SNP_UNKNOWN_ATTRIBUTE; // Should never get here!
  }
  
  return status;
}
  
/*********************************************************************
 * @fn      oadWriteAttrCB
 *
 * @brief   Validate and Write attribute data
 *
 * @param   context - context used when registering service
 * @param   conn    - connection handle ID
 * @param   charHdl - characteristic value handle
 * @param   len     - length of data to be written
 * @param   pData   - pointer to data to be written
 *
 * @return  SNP_SUCCESS, or SNP_UNKNOWN_ATTRIBUTE
 */
static bStatus_t oadWriteAttrCB(void *context, uint16_t conn,
                                uint16_t charHdl, uint16_t len,
                                uint8_t *pData)
{
  bStatus_t status = SNP_SUCCESS;

  // Determine which characteristic is being written to
  if (oadServiceCharHandles[OAD_CHAR_IMG_IDENTIFY].valueHandle ==
          charHdl)
  {
    /* OAD manager has sent new image information.
     * Read and decide whether to accept or reject an OAD 
     * of this new application image.
     */
    
    // Notify Application
    if (oadTargetWriteCB != NULL)
    {
      (*oadTargetWriteCB)(OAD_WRITE_IDENTIFY_REQ, conn, pData, len);
    }
  }
  else if (oadServiceCharHandles[OAD_CHAR_IMG_BLOCK].valueHandle ==
            charHdl)
  {
    /* OAD is ongoing.  
     * the OAD manager has sent a block from the new image. 
     */
    
    // Notify the application.
    if (oadTargetWriteCB != NULL)
    {
      (*oadTargetWriteCB)(OAD_WRITE_BLOCK_REQ, conn, pData, len);
    }
  }
  else if (oadServiceCharHandles[OAD_CHAR_IMG_COUNT].valueHandle ==
              charHdl)
  {
#ifndef FEATURE_OAD_ONCHIP
    /*
     * This is a manipulation of the number of OAD Images to download before
     * issuing a reset.
     */
    if (len == sizeof(uint8_t))
    {
      imgCount = *pData;
    }
    else
    {
      status = SNP_INVALID_PARAMS;
    }
#endif // !FEATURE_OAD_ONCHIP
  }
  else
  {
    status = SNP_UNKNOWN_ATTRIBUTE; // Should never get here!
  }

  return status;
}

/*********************************************************************
 * @fn      OAD_processSNPEventCB
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
static void OAD_processSNPEventCB(uint16_t event, snpEventParam_t *param)
{ 
  switch(event)
  {
    case SNP_CONN_EST_EVT:
      {
        oadImgIdentifyCCCDFlag = OAD_DEFAULT_CCCD;
        oadImgBlockCCCDFlag = OAD_DEFAULT_CCCD;
        
#ifndef FEATURE_OAD_ONCHIP
        imgCount = OAD_DEFAULT_IMGCNT;
#endif //FEATURE_OAD_ONCHIP
      }
      break;
    default:
      break;
  }
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
  uint8_t hdrOffset = 0;
  
  if (OADTarget_hasExternalFlash())
  {
    hdrOffset = 4;
  }
  
  // Store the new image's header
  OADTarget_storeImageHeader(pValue);
  
  // Read out running image's header.
  OADTarget_getCurrentImageHeader(&ImgHdr);
  
  // Calculate block total of the new image.
  oadBlkTot = BUILD_UINT16(pValue[hdrOffset + 2], pValue[hdrOffset + 3]) / (OAD_BLOCK_SIZE / HAL_FLASH_WORD_SIZE);
  oadBlkNum = 0;

  /* Requirements to begin OAD:
   * 1) LSB of image version cannot be the same, this would imply a code overlap
   *    between currently running image and new image.
   * 2) Total blocks of new image must not exceed maximum blocks supported, else
   *    the new image cannot fit.
   * 3) Block total must be greater than 0.
   * 4) Optional: Add additional criteria for initiating OAD here.
   */
  if (OADTarget_validateNewImage(pValue + hdrOffset, &ImgHdr, oadBlkTot))
  {
    // Determine where image will be stored.
    imageAddress = OADTarget_imageAddress(pValue+hdrOffset);
    imagePage = imageAddress / HAL_FLASH_PAGE_SIZE;

    // Open the target interface
    OADTarget_open();

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
    //uint32_t addr = FLASH_ADDRESS(OAD_IMG_D_PAGE, (oadBlkNum * OAD_BLOCK_SIZE));

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
#if FEATURE_OAD_ONCHIP
    // Handle CRC verification in BIM.
    OADTarget_systemReset();
#else // !FEATURE_OAD_ONCHIP
    // Run CRC check on new image.
    if (checkDL())
    {
      // Store the flag of the downloaded image.
      flagRecord |= getImageFlag();
      
      // Store the image information.
      saveImageInfo();
      
      // Close external Flash - Must be closed prior to notifying application
      // via call back. Application may open/close external flash itself which
      // will crash device OAD profile closes an already closed external flash
      OADTarget_close();
      
      // Check if all expected images have been downloaded.
      if (CheckImageDownloadCount())
      {
        // if one image is a network processor image, inform the application now 
        // so that it can take action on that image.
        // Note: this callback is not being sent from the context of an interrupt.
        // It is ok to take any action here.
        if (flagRecord & OAD_IMG_NP_FLAG)
        {
          (*oadTargetWriteCB)(OAD_IMAGE_COMPLETE, connHandle, NULL, 0);
        }
        // if one image is an application or stack image, perform the reset here.
        if (flagRecord & (OAD_IMG_APP_FLAG|OAD_IMG_STACK_FLAG))
        {
          OADTarget_systemReset();
        }
        
        flagRecord = 0;
      }
    }
    else
    {
      // Close external Flash
      OADTarget_close();
    }
#endif //FEATURE_OAD_ONCHIP
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
  // If notifications enabled
  if (oadImgBlockCCCDFlag & SNP_GATT_CLIENT_CFG_NOTIFY)
  {
    snpNotifIndReq_t localReq;
        
    // Initialize Request
    localReq.connHandle = connHandle;
    localReq.attrHandle = oadServiceCharHandles[OAD_CHAR_IMG_BLOCK].valueHandle;
    localReq.pData = SNP_malloc(sizeof(blkNum));
    localReq.authenticate = 0; // Default no authentication
    localReq.type = SNP_SEND_NOTIFICATION;
    
    // Copy block number to notification payload
    memcpy(localReq.pData, &blkNum, sizeof(blkNum));
    
    SNP_sendNotifInd(&localReq, sizeof(blkNum));
    SNP_free(localReq.pData);  
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
  // If notifications enabled
  if (oadImgIdentifyCCCDFlag & SNP_GATT_CLIENT_CFG_NOTIFY)
  {
    snpNotifIndReq_t localReq;
        
    // Initialize Request
    localReq.connHandle = connHandle;
    localReq.attrHandle = oadServiceCharHandles[OAD_CHAR_IMG_IDENTIFY].valueHandle;
    localReq.pData = SNP_malloc(OAD_IMG_HDR_SIZE);
    localReq.authenticate = 0; // Default no authentication
    localReq.type = SNP_SEND_NOTIFICATION;
    
    // Copy ver,len,uid members of 
    localReq.pData[0] = LO_UINT16(pImgHdr->ver);
    localReq.pData[1] = HI_UINT16(pImgHdr->ver);
    
    localReq.pData[2] = LO_UINT16(pImgHdr->len);
    localReq.pData[3] = HI_UINT16(pImgHdr->len);
    
    (void)memcpy(&localReq.pData[4], pImgHdr->uid, sizeof(pImgHdr->uid));
    
    SNP_sendNotifInd(&localReq, OAD_IMG_HDR_SIZE);
    SNP_free(localReq.pData);
  }
  
  // Close the OAD target if it is open
  OADTarget_close();
}

#if !defined FEATURE_OAD_ONCHIP
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
         offset < HAL_FLASH_PAGE_SIZE && (page < lastPage || offset < numRemBytes); 
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

  //OADTarget_readFlash(OAD_IMG_D_PAGE, OAD_IMG_CRC_OSET, (uint8_t *)crc, sizeof(crc));
  OADTarget_getCrc(crc);

  if ((crc[0] == 0xFFFF) || (crc[0] == 0x0000))
  {
    return FALSE;
  }

  // Calculate CRC of downloaded image.
  crc[1] = crcCalcDL();
 
  if (crc[1] == crc[0])
  {
    // Set the CRC shadow as equivalent to the CRC.
    OADTarget_setCrc(crc);
  }
    
  return (crc[0] == crc[1]);
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
 * @fn          CheckImageDownloadCount
 *
 * @brief       Decrement the image download count.
 *
 * @param       Image Type of completed image.
 *
 * @return      returns TRUE if no more images are expected, FALSE otherwise.
 */
static uint8_t CheckImageDownloadCount(void)
{ 
  return !--imgCount;
}
#endif // !FEATURE_OAD_ONCHIP

/*********************************************************************
*********************************************************************/
