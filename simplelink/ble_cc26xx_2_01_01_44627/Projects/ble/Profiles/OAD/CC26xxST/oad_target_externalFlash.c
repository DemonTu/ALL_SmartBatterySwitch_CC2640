/*******************************************************************************
  Filename:       oad_target_externalFlash.c
  Revised:        $Date: 2014-10-01 10:02:52 +0200 (on, 01 okt 2014) $
  Revision:       $Revision: 40366 $

  Description:    This file contains the externalFlash target implementation
                  of the OAD profile.

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
*******************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#ifdef FEATURE_OAD
#include <string.h>
#include "hal_board.h"
#include "oad_target.h"
#include "ext_flash.h"
#include "ext_flash_layout.h"
#include "driverlib/flash.h"
#include "driverlib/vims.h"

#ifdef POWER_SAVING
#include <ti/sysbios/family/arm/cc26xx/Power.h>
#include <ti/sysbios/family/arm/cc26xx/PowerCC2650.h>
#endif

/*********************************************************************
 * Constants and macros
 */
#define PROG_BUF_SIZE             16
#define PAGE_0                    0
#define PAGE_1                    1
#define PAGE_31                   31

#define MAX_BLOCKS                (EFL_SIZE_IMAGE_MIRROR / OAD_BLOCK_SIZE)

#ifndef APP_IMAGE_START
#define APP_IMAGE_START           0
#endif
/*********************************************************************
 * PRIVATE VARIABLES
 */
static bool isOpen = false;
static ExtImageInfo_t imgInfo;

/*********************************************************************
 * PRIVATE FUNCTIONS
 */
static void saveImageInfo(void);
static bool checkCurrentBaseLoader(void);
static bool checkNewBaseLoader(void);
static bool installBaseLoader(void);
static void execBaseLoader(void);

/*********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      OADTarget_open
 *
 * @brief   Open an OAD target for download.
 *
 * @param   none
 *
 * @return  TRUE if OAD target successfully opened
 */
uint8_t OADTarget_open(void)
{
  isOpen = extFlashOpen();

  return isOpen;
}


/*********************************************************************
 * @fn      OADTarget_close
 *
 * @brief   Close an OAD target after a download has finished
 *
 * @param   none
 *
 * @return  none
 */
void OADTarget_close(void)
{
  if (isOpen)
  {
    isOpen = false;
    extFlashClose();
  }
}

/*********************************************************************
 * @fn      OADTarget_hasExternalFlash
 *
 * @brief   Check if the target has external flash
 *
 * @param   none
 *
 * @return  none
 */
bool OADTarget_hasExternalFlash(void)
{
  return true;
}

/*********************************************************************
 * @fn      OADTarget_getCurrentImageHeader
 *
 * @brief   Store the image header of the new image
 *
 * @param   pValue - pointer to the new image header
 *
 * @return  none
 */
void OADTarget_getCurrentImageHeader(img_hdr_t *pHdr)
{
  if (extFlashOpen())
  {
    extFlashRead(EFL_IMAGE_INFO_ADDR_APP, sizeof(ExtImageInfo_t),
                 (uint8_t*)&imgInfo);
    extFlashClose();

    pHdr->len = imgInfo.len;
    pHdr->ver = imgInfo.ver;
    memcpy(pHdr->uid,imgInfo.uid,sizeof(imgInfo.uid));
    pHdr->res[0] = HI_UINT16(imgInfo.addr);
    pHdr->res[1] = LO_UINT16(imgInfo.addr);
    pHdr->res[2] = imgInfo.imgType;
    pHdr->res[3] = 0xAA;
  }
}


/*********************************************************************
 * @fn      OADTarget_validateNewImage
 *
 * @brief   Determine if a new image should be downloaded or not based on
 *          target specific criteria.
 *
 * @param   pValue - pointer to new Image header information
 * @param   pCur - pointer to contents of current image header
 * @param   blkTot - total number of blocks comprising new image.
 *
 * @return  TRUE to begin OAD otherwise FALSE to reject the image.
 */
uint8_t OADTarget_validateNewImage(uint8_t *pValue, img_hdr_t *pCur,
                                          uint16_t blkTot)
{
  img_hdr_t *pNew;
  bool valid;

  pNew = (img_hdr_t *)pValue;
  valid = false;

  // Check if number of blocks make sense
  if ((blkTot > MAX_BLOCKS) || (blkTot == 0))
  {
    return false;
  }


  if ((pNew->ver == 0) )
  {
    valid = true;
  }

  if (!valid)
  {
    valid =  pNew->ver > pCur->ver;
  }

  return valid;
}

/*********************************************************************
 * @fn      OADTarget_storeImageHeader
 *
 * @brief   Store the image header of the new image
 *
 * @param   pValue - pointer to the new image header
 *
 * @return  none
 */
void OADTarget_storeImageHeader(uint8_t *pValue)
{
  img_hdr_t* pNew;

  pNew = (img_hdr_t*)(pValue +4);

  // Storage image header (written to external flash before reboot)
  imgInfo.crc[0] = BUILD_UINT16(pValue[0],pValue[1]);
  imgInfo.crc[1] = BUILD_UINT16(pValue[2],pValue[3]);
  imgInfo.addr = BUILD_UINT16(pNew->res[0],pNew->res[1]);
  imgInfo.ver = pNew->ver;
  imgInfo.len = pNew->len;
  memcpy(imgInfo.uid,pNew->uid,sizeof(imgInfo.uid));
  imgInfo.imgType = pNew->res[2];
}

/*********************************************************************
 * @fn      OADTarget_imageAddress
 *
 * @brief   Get the address of the current image
 *
 * @param   pValue - pointer to the new image header
 *
 * @param   pHdr - pointer to the current image header
 *
 * @return  address
 */
uint32_t OADTarget_imageAddress(uint8_t *pValue)
{
  img_hdr_t *pNew;
  uint16_t blockAddr;

  pNew = (img_hdr_t *)pValue;
  blockAddr = BUILD_UINT16(pNew->res[0],pNew->res[1]);

  return  blockAddr == 0xFFFF ? APP_IMAGE_START :
    blockAddr * (OAD_BLOCK_SIZE / HAL_FLASH_WORD_SIZE);
}

/*********************************************************************
 * @fn      OADTarget_getCrc
 *
 * @brief   Get the CRC array from the image that is being downloaded
 *
 * @param   pCrc - pointer to the new image header
 *
 * @return  Non
 */
void OADTarget_getCrc(uint16_t *pCrc)
{
  pCrc[0] = imgInfo.crc[0];
  pCrc[1] = imgInfo.crc[1];
}

/*********************************************************************
 * @fn      OADTarget_enableCache
 *
 * @brief   Prepares system for a write to flash, if necessary.
 *
 * @param   None.
 *
 * @return  None.
 */
void OADTarget_enableCache(void)
{

}

/*********************************************************************
 * @fn      OADTarget_disableCache
 *
 * @brief   Resumes system after a write to flash, if necessary.
 *
 * @param   None.
 *
 * @return  None.
 */
void OADTarget_disableCache(void)
{

}

/*********************************************************************
 * @fn      OADTarget_readFlash
 *
 * @brief   Read data from flash.
 *
 * @param   page   - page to read from in flash
 * @param   offset - offset into flash page to begin reading
 * @param   pBuf   - pointer to buffer into which data is read.
 * @param   len    - length of data to read in bytes.
 *
 * @return  None.
 */
void OADTarget_readFlash(uint8_t page, uint32_t offset, uint8_t *pBuf,
                         uint16_t len)
{
  extFlashRead(FLASH_ADDRESS(page,offset), len, pBuf);
}

/*********************************************************************
 * @fn      OADTarget_writeFlash
 *
 * @brief   Write data to flash.
 *
 * @param   page   - page to write to in flash
 * @param   offset - offset into flash page to begin writing
 * @param   pBuf   - pointer to buffer of data to write
 * @param   len    - length of data to write in bytes
 *
 * @return  None.
 */
void OADTarget_writeFlash(uint8_t page, uint32_t offset, uint8_t *pBuf,
                          uint16_t len)
{
  extFlashWrite(FLASH_ADDRESS(page,offset), len, pBuf);
}

/*********************************************************************
 * @fn      OADTarget_eraseFlash
 *
 * @brief   Erase selected flash page.
 *
 * @param   page - the page to erase.
 *
 * @return  None.
 */
void OADTarget_eraseFlash(uint8_t page)
{
  extFlashErase(FLASH_ADDRESS(page,0), HAL_FLASH_PAGE_SIZE);
}

/*********************************************************************
 * @fn      OADTarget_systemReset
 *
 * @brief   Prepare system for a reset and trigger a reset to the boot
 *          image manager. If the image contains a new BIM, copy it to
 *          page '31' before resetting. This is a critical operation.
 *
 * @param   None.
 *
 * @return  None.
 */
void OADTarget_systemReset(void)
{
  bool newBaseLoaderValid;

  // Store the image information
  saveImageInfo();

  newBaseLoaderValid = checkNewBaseLoader();

  // Copy base loader from external to internal flash if necessary
  if (newBaseLoaderValid)
  {
    installBaseLoader();
  }

  // Launch base loader
  if (checkCurrentBaseLoader())
  {
    execBaseLoader();
  } // else: not able to upgrade
}

/*******************************************************************************
 * @fn      checkCurrentBaseLoader
 *
 * @brief   Check if a base loader is present in page 31
 *
 * @return  none
 */
static bool checkCurrentBaseLoader(void)
{
  bool present;
  uint32_t *pBL;

  pBL = (uint32_t*)(BL_OFFSET -1); // Start of page 31
  present = (*pBL) != 0xFFFFFFFF;

  return present;
}


/*******************************************************************************
 * @fn      checkNewBaseLoader
 *
 * @brief   Check if a base loader is present in page 31 of the newly
 *          downloaded image.
 *
 * @return  none
 */
static bool checkNewBaseLoader(void)
{
  bool present;
  uint32_t tmp;

  extFlashRead(FLASH_ADDRESS(PAGE_31,0), sizeof(tmp), (uint8_t*)&tmp);
  present = tmp != 0xFFFFFFFF;

  return present;
}


/*******************************************************************************
 * @fn      execBaseLoader
 *
 * @brief   Load new image from external flash and reboot
 *
 * @return  none
 */
static void execBaseLoader(void)
{
  /*
  extern void __baseloader_addr(uint32_t start, uint32_t len, uint32_t dst);
  __baseloader_addr(BASEOAD_STORAGE_IMAGE_OFFSET, imglen, 0);

  In linker command file (.icf) add the following:

  define exported symbol __baseloader_addr = 0x1f001;
  */
  int excessSpace;
  uint32_t extFlashAddr;
  uint32_t extImageSize;
  uint32_t intFlashAddr;

  // Get Image information
  extFlashAddr = imgInfo.addr * EFL_OAD_ADDR_RESOLUTION;
  extImageSize = imgInfo.len * EFL_OAD_ADDR_RESOLUTION;
  intFlashAddr = extFlashAddr;
  excessSpace = intFlashAddr + extImageSize - 0x1F000;
  if (excessSpace > 0)
  {
    // Base loader does not accept programming of page 31,
    // program all but this page
    extImageSize -= excessSpace;
  }

  // Launch new image
  ((void (*)(uint32_t, uint32_t, uint32_t))BL_OFFSET)(extFlashAddr,
                                                      extImageSize,
                                                      intFlashAddr);
}


/*******************************************************************************
 * @fn      installBaseLoader
 *
 * @brief   Program the 'base loader' to page 31 of the internal flash. Assumes
 *          that external flash already contains a downloaded 'base loader'.
 *
 * @return  none
 */
static bool installBaseLoader(void)
{
  bool success;
  uint8_t buffer[PROG_BUF_SIZE];
  uint32_t addr;

  // Enable iCache pre-fetching.
  ROM_VIMSConfigure(VIMS_BASE, TRUE, TRUE);

  // Re-enable cache.
  ROM_VIMSModeSet(VIMS_BASE, VIMS_MODE_ENABLED);

  // Erase page 31
  FlashSectorErase(FLASH_ADDRESS(PAGE_31,0));

  success = true;

  // Copy Page 31 from external to internal flash
  for (addr=FLASH_ADDRESS(PAGE_31,0); addr<0x20000; addr+=PROG_BUF_SIZE)
  {
    if (extFlashRead(addr, PROG_BUF_SIZE, buffer))
    {
      FlashProgram(buffer, addr, PROG_BUF_SIZE);
    }
    else
    {
      success = false;
      break;
    }
  }

  // Disable cache.
  ROM_VIMSModeSet(VIMS_BASE, VIMS_MODE_DISABLED);

  return success;
}


/*******************************************************************************
 * @fn      saveImageInfo
 *
 * @brief   Save image information in the meta-data area
 *
 * @return  none
 */
static void saveImageInfo(void)
{
  extFlashErase(EFL_IMAGE_INFO_ADDR_APP, HAL_FLASH_PAGE_SIZE);
  extFlashWrite(EFL_IMAGE_INFO_ADDR_APP, sizeof(ExtImageInfo_t),
                (uint8_t*)&imgInfo);
}

#endif
