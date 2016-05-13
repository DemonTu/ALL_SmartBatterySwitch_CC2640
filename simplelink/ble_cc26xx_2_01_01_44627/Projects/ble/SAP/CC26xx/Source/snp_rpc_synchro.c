/**************************************************************************************************
  @headerfile:    snp_rpc_synchro.c
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

/*********************************************************************
 * INCLUDES
 */
#include <stdint.h>
#include <stdlib.h>

#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/BIOS.h>

#include "snp_rpc_synchro.h"

/*********************************************************************
 * LOCAL VARIABLES
 */         

// Used to block SNP calls during a synchronous transaction.
Semaphore_Handle syncReq_sem;

// Signal application which requested the synchronous request.
Semaphore_Handle waitRsp_sem;

// Mutual exclusion for the number of active asynchronous transaction in effect
// variable.
//Semaphore_Handle asyncCountIncrMutex_sem;
//Semaphore_Handle asyncCountDecrMutex_sem;
//uint8_t asyncCount;

/*********************************************************************
 * FUNCTIONS
 */

void SNP_open(void)
{
 // Create Synchronous message semaphore, ready to claim
 syncReq_sem = Semaphore_create(1, NULL, NULL);
 
 // Create waitResponse semaphore, must be signaled before progressing. 
 waitRsp_sem = Semaphore_create(0, NULL, NULL);
}

void SNP_close(void)
{
  // Delete SNP semaphores
  Semaphore_delete(&syncReq_sem);
  Semaphore_delete(&waitRsp_sem);
}

void SNP_enterCS(void)
{
  // Check if we are waiting on asynchronous requests to finish.
  // This also denies any more asynchronous requests from starting, thereby removing the starvation problem.
  //Semaphore_pend(asyncCountIncrMutex_sem, BIOS_WAIT_FOREVER);
    
  // Wait for any ongoing requests to finish.
  Semaphore_pend(syncReq_sem, BIOS_WAIT_FOREVER);
    
  // Release the asyncCountIncrMutex semaphore.
  //Semaphore_post(asyncCountIncrMutex_sem);
}

void SNP_exitCS(void)
{
  // Release the synchronous request semaphore.
  Semaphore_post(syncReq_sem);
}

void SNP_waitForResponse(void)
{
  // Wait for a response.
  Semaphore_pend(waitRsp_sem, BIOS_WAIT_FOREVER);
}

void SNP_responseReceived(void)
{
  // Response received, release the semaphore.
  Semaphore_post(waitRsp_sem);
}

// Pass in size get a pointer to dynamically allocated buffer.
void *SNP_malloc(uint16_t size)
{
  return malloc(size);
}

// Pass in the pointer to a buffer allocated by SNP_malloc to free the buffer.
void SNP_free(void* pointer)
{
  free(pointer);
}

/*********************************************************************
*********************************************************************/

