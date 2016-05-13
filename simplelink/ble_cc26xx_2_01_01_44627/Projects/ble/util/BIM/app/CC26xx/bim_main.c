/*******************************************************************************
  Filename:       bim_main.c
  Revised:        $Date: 2015-07-20 11:31:07 -0700 (Mon, 20 Jul 2015) $
  Revision:       $Revision: 44370 $

  Description:

  This module contains the definitions for the main functionality of a Boot 
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
#include "hal_flash.h"
#include <driverlib/pwr_ctrl.h>
#include "hal_types.h"
#include "oad_target.h"

#include <inc/hw_device.h>
#include <driverlib/sys_ctrl.h>
#include <driverlib/aon_event.h>
#include <driverlib/osc.h>
/* -----------------------------------------------------------------------------
 *                                          Constants
 * -----------------------------------------------------------------------------
 */

#define BIM_IMG_A_PAGE        OAD_IMG_A_PAGE
#define BIM_IMG_A_OSET        OAD_IMG_A_OSET
#define BIM_IMG_A_AREA        OAD_IMG_A_AREA

#define BIM_IMG_B_PAGE        OAD_IMG_B_PAGE
#define BIM_IMG_B_OSET        OAD_IMG_B_OSET
#define BIM_IMG_B_AREA        OAD_IMG_B_AREA

#define BIM_CRC_OSET          OAD_IMG_CRC_OSET
#define BIM_HDR_OSET          OAD_IMG_HDR_OSET

/* -----------------------------------------------------------------------------
 *                                          Typedefs
 * -----------------------------------------------------------------------------
 */
/*
typedef struct {
  // User-defined Image Version Number - default logic uses simple a '<' 
  // comparison to start an OAD.
  uint16 ver;
  uint16 len;        // Image length in 4-byte blocks (i.e. HAL_FLASH_WORD_SIZE blocks).
  uint8  uid[4];     // User-defined Image Identification bytes.
  uint8  res[4];     // Reserved space for future use.
} img_hdr_t;
*/
/* -----------------------------------------------------------------------------
 *                                       Global Variables
 * -----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------
 *                                       Local Variables
 * -----------------------------------------------------------------------------
 */

__no_init uint8 pgBuf[HAL_FLASH_PAGE_SIZE];

void halSleepExec(void);
static uint16 crc16(uint16 crc, uint8 val);

/*******************************************************************************
 * @fn          halSleepExec
 *
 * @brief       This function puts the CC254x.
 *
 * @param       None.
 *
 * @return      None.
 */
#pragma optimize=none
void halSleepExec(void)
{ 
  //AONEventMcuWakeUpSet(AON_EVENT_MCU_WU1, AON_EVENT_IO19); 
  
  // Go to sleep.
  PowerCtrlStateSet(PWRCTRL_STANDBY);
  
  // Reset
  OADTarget_systemReset();
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
 * @return      The CRC16 calculated.
 */
static uint16 crcCalc(uint8 page, uint16 offset)
{
  uint16 crc = 0;
  
  // Read first page of the image into the buffer.
  OADTarget_readFlash(page, 0, pgBuf, HAL_FLASH_PAGE_SIZE);

  const img_hdr_t *pImgHdr = (const img_hdr_t *)(pgBuf + offset + BIM_HDR_OSET);

  uint8 pageBeg = page;
  uint8 pageEnd = pImgHdr->len / (OAD_FLASH_PAGE_MULT);
 
  // Find ending offset on last page.
  uint16 osetEnd = ((pImgHdr->len + (offset / HAL_FLASH_WORD_SIZE)) - 
                    (pageEnd * (HAL_FLASH_PAGE_SIZE / HAL_FLASH_WORD_SIZE))) *
                   HAL_FLASH_WORD_SIZE;
  
  // Set pageEnd to the end page of the OAD range.
  pageEnd += pageBeg;
    
  // Read over image pages.
  for (uint8_t pageIdx = pageBeg; pageIdx <= pageEnd; pageIdx++)
  {
    // Read over all flash words in a page, excluding the CRC section of the
    // first page and all bytes after the remainder bytes on the last page.
    for (uint16_t oset = (pageIdx == pageBeg ? offset + 4 : 0); 
         oset < HAL_FLASH_PAGE_SIZE && (pageIdx < pageEnd || oset < osetEnd);
         oset++)
    {
      crc = crc16(crc, pgBuf[oset]);
    }

    // Read the next page into the buffer.
    if (pageIdx != pageEnd)
    {
      OADTarget_readFlash(pageIdx + 1, 0, pgBuf, HAL_FLASH_PAGE_SIZE);
    }
  }
  
  // IAR note explains that poly must be run with value zero for each byte of crc.
  crc = crc16(crc, 0);
  crc = crc16(crc, 0);
  
  return crc;
}

/*******************************************************************************
 * @fn          crcCheck
 *
 * @brief       Calculate the image CRC and set it ready-to-run if it is good.
 *
 * input parameters
 *
 * @param       page - Flash page on which to beginning the CRC calculation.
 *
 * @param       offset - offset into page at which the image starts.
 *
 * output parameters
 *
 * None.
 *
 * @return      None, but no return from this function if the CRC check is good.
 */
static uint8 crcCheck(uint8 page, uint16 offset, uint16 *crc)
{
  HAL_BOARD_INIT();

  // Calculate CRC and compare with original output.
  if (crc[0] == crcCalc(page, offset))
  {     
    // Set shadow equal to the original CRC output.
    crc[1] = crc[0];
    
    // Write CRC shadow to flash.
    OADTarget_writeFlash(page, offset + BIM_CRC_OSET, (uint8 *)crc, 4);
    
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
 * input parameters
 *
 * @param       crc - Running CRC calculated so far.
 * @param       val - Value on which to run the CRC16.
 *
 * output parameters
 *
 * None.
 *
 * @return      crc - Updated for the run.
 */
static uint16 crc16(uint16 crc, uint8 val)
{
  const uint16 poly = 0x1021;
  uint8 cnt;

  for (cnt = 0; cnt < 8; cnt++, val <<= 1)
  {
    uint8 msb = (crc & 0x8000) ? 1 : 0;

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
  uint16 crc[2];
  
  // Read Image B's CRC values from flash.  This is done first to prevent
  // Image A from ever invalidating itself.
  OADTarget_readFlash(BIM_IMG_B_PAGE, BIM_IMG_B_OSET + BIM_CRC_OSET, (uint8 *)crc, 4);

  if ((crc[0] != 0xFFFF) && (crc[0] != 0x0000))
  {
    if (crc[0] == crc[1] || crcCheck(BIM_IMG_B_PAGE, BIM_IMG_B_OSET, crc) )
    {
      // Load address of label __iar_program_start from the fixed location
      // of the image's reset vector and jump.        
      asm(" MOV R0, #0x6014 ");
      asm(" LDR R0, [R0] ");
      asm(" BX R0 ");
      
      OADTarget_systemReset();  // Should not get here.
    }
  }

  // Read Image A's CRC values from flash.
  OADTarget_readFlash(BIM_IMG_A_PAGE, BIM_IMG_A_OSET + BIM_CRC_OSET, (uint8 *)crc, 4);

  if ((crc[0] != 0xFFFF) && (crc[0] != 0x0000))
  {
#if !defined(FEATURE_FIXED_IMAGE)
    if (crc[0] == crc[1] || crcCheck(BIM_IMG_A_PAGE, BIM_IMG_A_OSET, crc))
#endif //FEATURE_FIXED_IMAGE
    {
      // Load address of label __iar_program_start from the fixed location
      // of the image's reset vector and jump.
      asm("MOV R0, #0x0614");
      asm("LDR R0, [R0]");
      asm("BX R0");
      
      OADTarget_systemReset();  // Should not get here.
    }
  }

  // Neither image is ready to run.  Go to sleep.
  halSleepExec();
  
  OADTarget_systemReset();  // Should not get here.
}


/**************************************************************************************************
*/
