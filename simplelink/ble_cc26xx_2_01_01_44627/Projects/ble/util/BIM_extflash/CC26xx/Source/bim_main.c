/*******************************************************************************
  Filename:       bim_main.c
  Revised:        $Date: 2015-02-13 09:59:52 -0800 (Fri, 13 Feb 2015) $
  Revision:       $Revision: 42569 $

  Description:  This module contains the definitions for the main functionality of a Boot 
                Image Manager.

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

/* -----------------------------------------------------------------------------
 *                                          Includes
 * -----------------------------------------------------------------------------
 */
#include <stdint.h>
#include <string.h>
#include <driverlib/flash.h>
#include <driverlib/watchdog.h>
#include <inc/hw_prcm.h>

#include "hal_flash.h"
#include "bls.h"
#include "ext_flash_layout.h"
#include "oad_target.h"

/* -----------------------------------------------------------------------------
 *                                          Constants
 * -----------------------------------------------------------------------------
 */

/* Image offset into a page */
#define BIM_IMG_E_OSET       OAD_IMG_E_OSET

/**
 * @internal
 * Internal flash page size
 */
#define BASELOADER_IPAGESIZE 4096

/* -----------------------------------------------------------------------------
 *                                   Typedefs
 * -----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------
 *                                   Global Variables
 * -----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------
 *                                   Local Variables
 * -----------------------------------------------------------------------------
 */

#if defined(__IAR_SYSTEMS_ICC__)
__no_init uint8_t pgBuf[HAL_FLASH_PAGE_SIZE];
__no_init ExtImageInfo_t imgInfo;
#elif defined(__TI_COMPILER_VERSION__)
uint8_t pgBuf[HAL_FLASH_PAGE_SIZE];
ExtImageInfo_t imgInfo;
#endif

/* -----------------------------------------------------------------------------
 *                                   Functions
 * -----------------------------------------------------------------------------
 */

static uint16_t crc16(uint16_t crc, uint8_t val);
 
/*******************************************************************************
 * @fn     Bim_intErasePage
 *
 * @brief  Erase an internal flash page.
 *
 * @param  page - page number
 *
 * @return Zero when successful. Non-zero, otherwise.
 */
static int Bim_intErasePage(uint_least16_t page)
{
  /* Note that normally flash */
  if (FlashSectorErase(page * BASELOADER_IPAGESIZE) == FAPI_STATUS_SUCCESS)
  {
    return 0;
  }
  
  return -1;
}

/*******************************************************************************
 * @fn     Bim_intWriteWord
 *
 * @brief  Write a word to internal flash.
 *
 * @param  addr - address
 * @param  word - value to write
 *
 * @return Zero when successful. Non-zero, otherwise.
 */
static int Bim_intWriteWord(uint_least32_t addr, uint32_t word)
{
  if (FlashProgram((uint8_t *) &word, addr, 4) == FAPI_STATUS_SUCCESS)
  {
    return 0;
  }
  
  return -1;
}

/*******************************************************************************
 * @fn     Bim_extReadBuf
 *
 * @brief  Read a word from the external flash.
 *
 * @param  addr - address
 * @param  buf  - buffer to store the word
 * @param  len  - number of bytes to read.
 *
 * @return Zero when successful. Non-zero, otherwise.
 */
static int Bim_extReadBuf(uint_least32_t addr, uint32_t *buf, size_t len)
{
  return BLS_read(addr, len, (uint8_t *) buf);
}

/*******************************************************************************
 * @fn     Bim_extWriteBuf
 *
 * @brief  Write a buffer to the external flash.
 *
 * @param  addr - address
 * @param  pBuf - buffer to write
 * @param  len  - number of bytes to write
 *
 * @return Zero when successful. Non-zero, otherwise.
 */
static int Bim_extWriteBuf(uint_least32_t addr, uint32_t *pBuf, size_t len)
{
  return BLS_write(addr, len, (uint8_t *)pBuf);
}

/*******************************************************************************
 * @fn     Bim_copyImage
 *
 * @brief  Copies firmware image into the executable flash area.
 *
 * @param  imgStart - starting address of image in external flash.
 * @param  imgLen   - size of image in 4 byte blocks.
 * @param  dstAddr  - destination address within internal flash.
 *
 * @return Zero when successful. Non-zero, otherwise.
 */
static int Bim_copyImage(uint32_t imgStart, uint32_t imgLen,
                                uint32_t dstAddr)
{
  uint_fast16_t page = dstAddr/BASELOADER_IPAGESIZE;
  uint_fast16_t lastPage;
  
  lastPage = (uint_fast16_t) ((dstAddr + imgLen - 1) / BASELOADER_IPAGESIZE);

  /* Change image size into word unit */
  imgLen += 3;
  imgLen /= 4;

  if (dstAddr & 3)
  {
    /* Not an aligned address */
    return -1;
  }

  if (page > lastPage || lastPage > 30)
  {
    return -1;
  }

  for (; page <= lastPage; page++)
  {
    uint32_t buf;
    uint_least16_t count = (BASELOADER_IPAGESIZE / 4);

    if (page == lastPage)
    {
      /* count could be shorter */
      count = imgLen;
    }

    /* Erase the page */
    if (Bim_intErasePage(page))
    {
      /* Page erase failed */
      return -1;
    }
    while (count-- > 0)
    {
      /* Read word from external flash */
      if (Bim_extReadBuf(imgStart, &buf, 4))
      {
        /* read failed */
        return -1;
      }
      /* Write word to internal flash */
      if (Bim_intWriteWord(dstAddr, buf))
      {
        /* Program failed */
        return -1;
      }
	  
      imgStart += 4;
      dstAddr += 4;
      imgLen--;
    }
  }
  
  return 0;
}

/*******************************************************************************
 * @fn          crcCalc
 *
 * @brief       Run the CRC16 Polynomial calculation over the image specified.
 *
 * @param       page   - Flash page on which to beginning the CRC calculation.
 *
 * @param       offset - offset of first byte of image within first flash page
 *                       of the image.
 *
 * @return      The CRC16 calculated.
 */
static uint16_t crcCalc(uint8_t page, uint16_t offset)
{
  uint16_t crc = 0;
  uint8_t pageIdx;
  
  // Read first page of the image into the buffer.
  memcpy(pgBuf, (uint8_t *)(page * HAL_FLASH_PAGE_SIZE), HAL_FLASH_PAGE_SIZE);

  uint8_t pageBeg = page;
  uint8_t pageEnd = imgInfo.len / (OAD_FLASH_PAGE_MULT);
  
  // Find ending offset on last page.
  uint16_t osetEnd = ((imgInfo.len + (offset / HAL_FLASH_WORD_SIZE)) - 
                     (pageEnd * (HAL_FLASH_PAGE_SIZE / HAL_FLASH_WORD_SIZE))) *
                     HAL_FLASH_WORD_SIZE;
  
  // Set pageEnd to the end page of the OAD range.
  pageEnd += pageBeg;
  
  // Read over image pages.
  for (pageIdx = pageBeg; pageIdx <= pageEnd; pageIdx++)
  {
    uint16_t oset;

    // Read over all flash words in a page, excluding the CRC section of the
    // first page and all bytes after the remainder bytes on the last page.
    for (oset = (pageIdx == pageBeg ? offset + 4 : 0);
         oset < HAL_FLASH_PAGE_SIZE && (pageIdx < pageEnd || oset < osetEnd);
         oset++)
    {
      crc = crc16(crc, pgBuf[oset]);
    }

    // Read the next page into the buffer.
    if (pageIdx != pageEnd)
    {
      memcpy(pgBuf, (uint8_t *)((pageIdx + 1)*HAL_FLASH_PAGE_SIZE), 
      HAL_FLASH_PAGE_SIZE);
    }
  }
  
  // IAR note explains that poly must be run with value zero for each byte of 
  // the crc.
  crc = crc16(crc, 0);
  crc = crc16(crc, 0);
  
  return crc;
}

/*******************************************************************************
 * @fn          crcCheck
 *
 * @brief       Calculate the image CRC and set it ready-to-run if it is good.
 *
 * @param       page   - Flash page on which to begin the CRC calculation.
 * @param       offset - offset into page at which the image starts.
 *
 * @return      None, but no return from this function if the CRC check is good.
 */
static uint8_t crcCheck(uint8_t page, uint16_t offset, uint16_t *crc)
{
  HAL_BOARD_INIT();

  // Calculate CRC and compare with original output.
  if (crc[0] == crcCalc(page, offset))
  {     
    // Set shadow equal to the original CRC output.
    crc[1] = crc[0];
    
    // Allow branch to application.
    return 1;
  }
  
  // Image denied!
  return 0;
}

/*******************************************************************************
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

/*******************************************************************************
 * Actual  function to load image.
 *
 * @brief  Before entering this function, interrupts must be disabled and
 *         stack pointer must be set correctly.
 *
 * @param  start   - image start address in external flash.<br>
 *                   Must be 4 byte aligned address.
 * @param  len     - length of the image in bytes.
 * @param  dstaddr - address in internal flash to where to start writing
 *                   the image.<br>
 *                   Must be 4 byte aligned address.<br>
 *                   Note that when dstaddr is not internal flash page boundary
 *                   aligned, all preceding words in the same page will be 
 *                   erased.
 *
 * @return none
 */
void Bim_safeStart(uint32_t start, uint32_t len, uint32_t dstAddr)
{
  // Initialize external flash driver.
  BLS_init();

  // Copy over the image. Note that failure cannot be handled.
  Bim_copyImage(start, len, dstAddr);

  // Close external flash driver.
  BLS_close();

  // Trigger reset.
  HWREG(PRCM_BASE + PRCM_O_WARMRESET) = 4;
  WatchdogResetEnable();
  WatchdogEnable();
  WatchdogReloadSet(0);

  // Enable reset interrupt.
  CPUcpsie();

  // Stay here till reset.
  for (;;);
}

/*******************************************************************************
 * @fn     Bim_checkImages
 *
 * @brief  Check for new, valid images in external flash to copy into internal 
 *         flash.
 *
 * @param  none
 *
 * @return none
 */
void Bim_checkImages(void)
{ 
  uint32_t metaDataAddr;
  
  // Initialize external flash driver.
  BLS_init();

  for (metaDataAddr = EFL_IMAGE_INFO_ADDR_APP; 
       metaDataAddr <= EFL_IMAGE_INFO_ADDR_BLE; metaDataAddr += EFL_PAGE_SIZE)
  {
    // Check for the meta information on the image to see if a download is 
    // required.
    Bim_extReadBuf(metaDataAddr, (uint32_t *)&imgInfo, sizeof(ExtImageInfo_t));
    
    // Check if we have a valid image in external flash.
    if (imgInfo.crc[0] != 0xFFFF && imgInfo.crc[0] != 0x0000 && 
        imgInfo.crc[0] == imgInfo.crc[1] && imgInfo.status == 0xFF)
    {
      while(1)
      {
        uint32_t startAddr;
        uint8_t startPage;
        
        // Find the image address in external flash.
        if (imgInfo.imgType == EFL_OAD_IMG_TYPE_APP)
        {
          startAddr = EFL_ADDR_IMAGE_APP;
        }
        else if (imgInfo.imgType == EFL_OAD_IMG_TYPE_STACK)
        {
          startAddr = EFL_ADDR_IMAGE_BLE;
        }
        else
        {
          // This is neither Application nor Stack image. Check the next one.
          break;
        }
        
        // Copy over the image. Note that failure cannot be handled.
        Bim_copyImage(startAddr, (uint32_t)imgInfo.len * EFL_OAD_ADDR_RESOLUTION,
                             (uint32_t)imgInfo.addr * EFL_OAD_ADDR_RESOLUTION);
      
        startPage = ((uint32_t)imgInfo.addr * EFL_OAD_ADDR_RESOLUTION) / 
		            HAL_FLASH_PAGE_SIZE;
        
        // Check if the CRC of the internal image is valid.
        if (crcCheck(startPage, BIM_IMG_E_OSET, imgInfo.crc))
        {
          // Overwrite the status field of the meta data to reflect the contents of
          // internal flash.
          imgInfo.status = 0x80;
      
          Bim_extWriteBuf(metaDataAddr, (uint32_t *)&imgInfo, 
                          sizeof(ExtImageInfo_t));
          
          break;
        }
      }
    }
  }  
  
  // Close external flash driver.
  BLS_close();
}

/*******************************************************************************
 * @fn          main
 *
 * @brief       C-code main function.
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 */
void main(void)
{
// In case an imaged is flashed via debugger and no external image check should
// be made, define NO_COPY.
#ifndef NO_COPY
  Bim_checkImages();
#endif //NO_COPY
  
  // Load address of reset function from the fixed location of the image's 
  // reset vector and jump.        
  asm(" MOV R0, #0x1014 ");
  asm(" LDR R0, [R0] ");
  asm(" BX R0 ");
}


/**************************************************************************************************
*/
