/*******************************************************************************
  Filename:       bim_main.c
  Revised:        $Date: 2015-02-13 09:59:52 -0800 (Fri, 13 Feb 2015) $
  Revision:       $Revision: 42569 $

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
#include "hal_types.h"

/* -----------------------------------------------------------------------------
 *                                          Constants
 * -----------------------------------------------------------------------------
 */

// As defined in applications.
#define LOAD_IMG_A                            0x01
#define LOAD_IMG_B                            0x02

#define BIM_LEDGER_ADDR  0x00011000

/* -----------------------------------------------------------------------------
 *                                          Typedefs
 * -----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------
 *                                       Global Variables
 * -----------------------------------------------------------------------------
 */

// Allocate BIM Ledger page in flash.
#if defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=4096
#pragma location = ".bimLedger"
const uint8 BIM_LEDGER_FLASH[HAL_FLASH_PAGE_SIZE];
#elif defined(__TI_COMPILER_VERSION__)
#pragma location = (0x11000);
const uint8 BIM_LEDGER_FLASH[HAL_FLASH_PAGE_SIZE] = {0x00};
#else
#error "Unknown Compiler! Support for SNV not provided!"
#endif

/* -----------------------------------------------------------------------------
 *                                       Local Variables
 * -----------------------------------------------------------------------------
 */

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
  uint8_t *pLedgerPg = (uint8_t *)BIM_LEDGER_ADDR;
  uint16_t i = 0;
  uint8_t id = LOAD_IMG_A;
  
  /*
   * VIMS disabling should not be necessary as this is disabled by default and
   * no application has had a chance to modify its state at this point.
   */
  
  // Check first byte of ledger page.  if 0x00, erase.
  if (pLedgerPg[i] == 0)
  {
    // Erase the ledger page.
    FlashSectorErase((uint32_t)pLedgerPg);
  }
  else
  {
    // Find last offset unwritten offset in ledger page. 
    for (i=0; i < HAL_FLASH_PAGE_SIZE; i++)
    {
      id = pLedgerPg[i];
    
      if(id == 0xFF)
      {
        if (i != 0)
        {
          id = pLedgerPg[i-1];
        }
        else
        {
          id = LOAD_IMG_A;
        }
        break;
      }
    }
  }
  
  // Check which image to load.
  if (id == LOAD_IMG_A)
  {
    // Load address of label __iar_program_start from the fixed location
    // of the image's reset vector and jump.        
    asm(" MOV R0, #0x1504 ");
    asm(" LDR R0, [R0] ");
    asm(" BX R0 ");
  }
  else
  {
    // Load address of the entry point from the fixed location
    // of the image's reset vector and jump.
#if defined(__IAR_SYSTEMS_ICC__)
    asm(" MOV R0, #0x9004 ");
#else
    asm(" MOV R0, #0x8004 ");
#endif
    asm(" LDR R0, [R0] ");
    asm(" BX R0 ");    
  }
}


/**************************************************************************************************
*/
