/*******************************************************************************
  Filename:       buzzer.c
  Revised:        $Date: 2014-09-16 12:18:53 -0700 (Tue, 16 Sep 2014) $
  Revision:       $Revision: 40166 $

  Description:    Control of the buzzer of the keyfob board in the CC26xx.

  Copyright 2009 - 2014 Texas Instruments Incorporated. All rights reserved.

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

#include "hal_mcu.h"
#include "buzzer.h"

/*********************************************************************
 * @fn      Buzzer_init
 *
 * @brief   Initial the buzzer.
 *
 * @param   None.
 *
 * @return  None.
 */
void Buzzer_init(void)
{
  // To be implemented.
}

/*********************************************************************
 * @fn      Buzzer_start
 *
 * @brief   Start the buzzer at the given frequency
 *
 * @param   frequency - the frequency in Hertz to run the buzzer at.
 *
 * @return  1 - success. 0 - failure.
 */
uint8_t Buzzer_start(uint16_t frequency)
{
  Buzzer_init();

  // To be implemented.
  
  return 1;
}

/*********************************************************************
 * @fn      Buzzer_stop
 *
 * @brief   Stop the buzzer.
 *
 * @param   None.
 *
 * @return  None.
 */
void Buzzer_stop(void)
{
  // To be implemented.
}


/*********************************************************************
*********************************************************************/
