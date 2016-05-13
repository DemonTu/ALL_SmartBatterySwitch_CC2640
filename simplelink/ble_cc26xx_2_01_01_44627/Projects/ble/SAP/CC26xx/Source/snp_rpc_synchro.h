/**************************************************************************************************
  @headerfile:    snp_rpc_synchro.h
  Revised:        $Date: 2015-06-22 20:01:12 -0700 (Mon, 22 Jun 2015) $
  Revision:       $Revision: 44190 $

  Description:    This file contains required synchronization and memory 
                  functions used by the SNP RPC functions

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
  PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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
**************************************************************************************************/

#ifndef SNP_SYNCHRO_H
#define SNP_SYNCHRO_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
             
/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * HELPER FUNCTIONS
 */

/*********************************************************************
 * FUNCTIONS
 */

// Call before using any synchronization.
extern void SNP_open(void);

// Call when closing connection with SNP
extern void SNP_close(void);

// enter critical section.
extern void SNP_enterCS(void);

// exist critical section.
extern void SNP_exitCS(void);

// Block an application until a response is received.
extern void SNP_waitForResponse(void);

// Unblock application from running.
extern void SNP_responseReceived(void);

// Pass in size get a pointer to dynamically allocated buffer.
extern void *SNP_malloc(uint16_t size);

// Pass in the pointer to a buffer allocated by SNP_malloc to free the buffer.
extern void SNP_free(void* pointer);

/*********************************************************************
*********************************************************************/
#ifdef __cplusplus
}
#endif

#endif /* SNP_SYNCHRO_H */
