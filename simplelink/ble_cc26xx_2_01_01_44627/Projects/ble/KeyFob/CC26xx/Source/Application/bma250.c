/*******************************************************************************
  Filename:       bma250.c
  Revised:        $Date: 2015-01-02 17:16:18 -0800 (Fri, 02 Jan 2015) $
  Revision:       $Revision: 41601 $

  Description:    This file contains the BMA250 accelerator API implementation
                  for CC26xx.

  Copyright 2011 - 2014 Texas Instruments Incorporated. All rights reserved.

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


/******************************************************************************
 * INCLUDES
 */
#include "bma250.h"

/******************************************************************************
 * DEFINES
 */

/******************************************************************************
 * FUNCTION PROTOTYPES
 */

/******************************************************************************
 * LOCAL VARIABLES
 */
static uint8_t acc_initialized = FALSE;

/******************************************************************************
 * FUNCTIONS
 */

/****************************************************************************
* @fn       Acc_init(void)
*
* @brief    Initialize SPI interface and BMA250 accelerometer.
*
* @param    None.
*
* @return   void
****************************************************************************/
void Acc_init(void)
{
  acc_initialized = TRUE;
}

/****************************************************************************
* @fn       Acc_stop(void)
*
* @brief    Sets the BMA250 accelerometer in low-power mode.
*
* @param    None.
*
* @return   void
******************************************************************************/
void Acc_stop(void)
{
  if (acc_initialized)
  {
    // We cheat and simply turn off power to the accelerometer
    acc_initialized = FALSE;
  }
}

/****************************************************************************
* @fn       Acc_writeReg
*
* @brief    Write one byte to a sensor register
*
* @param    reg     Register address
* @param    val     Value to write
*
* @return   void
****************************************************************************/
void Acc_writeReg(uint8_t reg, uint8_t val)
{
  // To be implemented.
}

/****************************************************************************
* @fn       Acc_readReg
*
* @brief    Read one byte from a sensor register
*
* @param    reg     Register address
* @param    val     Pointer to destination of read value
*
* @return   void
****************************************************************************/
void Acc_readReg(uint8_t reg, uint8_t *pVal)
{
  // To be implemented.
}

/****************************************************************************
* @fn       Acc_readAcc
*
* @brief    Read x, y and z acceleration data in one operation.
*
* @param    pXVal   Pointer to destination of read out X acceleration
* @param    pYVal   Pointer to destination of read out Y acceleration
* @param    pZVal   Pointer to destination of read out Z acceleration
*
* @return   void
****************************************************************************/
void Acc_readAcc(int8_t *pXVal, int8_t *pYVal, int8_t *pZVal)
{
  int8_t readout[6] = {0,0,0,0,0,0};
  uint8_t i;

  // Read all data from accelerometer
  for(i = 0; i<6; i++)
  {
    // To be implemented.
  }

  // Use only most significant byte of each channel.
  *pXVal = readout[1];
  *pYVal = readout[3];
  *pZVal = readout[5];
}

/****************************************************************************
* @fn       Acc_readAcc
*
* @brief    Read x, y and z acceleration data in one operation.
*
* @param    pXVal   Pointer to destination of read out X acceleration
* @param    pYVal   Pointer to destination of read out Y acceleration
* @param    pZVal   Pointer to destination of read out Z acceleration
*
* @return   void
****************************************************************************/
void Acc_readAcc16(int16_t *pXVal, int16_t *pYVal, int16_t *pZVal)
{
  int8_t readout[6] = {0,0,0,0,0,0};
  uint8_t i;
  // Read all data from accelerometer
  for(i = 0; i<6; i++)
  {
    // To be implemented.
  }

  // Merge high byte (8b) and low bits (2b) into 16b signed destination
  *pXVal = ((((uint8_t)readout[0]) >> 6) | ((int16_t)(readout[1]) << 2));
  *pYVal = ((((uint8_t)readout[2]) >> 6) | ((int16_t)(readout[3]) << 2));
  *pZVal = ((((uint8_t)readout[4]) >> 6) | ((int16_t)(readout[5]) << 2));
}


/*********************************************************************
*********************************************************************/
