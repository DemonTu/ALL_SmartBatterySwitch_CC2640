/*******************************************************************************
  Filename:       time_ind.c
  Revised:        $Date: 2015-06-16 15:14:24 -0700 (Tue, 16 Jun 2015) $
  Revision:       $Revision: 44104 $

  Description:    Time indication and notification handling routines.

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

/*********************************************************************
 * INCLUDES
 */
#include "string.h"
#include "bcomdef.h"
#include "board_lcd.h"
#include "Board.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gattservapp.h"
#include "time_clock.h"
#include "bletime.h"

#include <ti/drivers/lcd/LCDDogm1286.h>
/*********************************************************************
 * MACROS
 */

// Maximum category ID value
#define ALERT_CAT_ID_MAX            9

// Parse major category
#define ALERT_MAJOR_CAT(x)          ((x) >> 3)

// Parse subcategory
#define ALERT_SUBCAT(x)             ((x) & 0x07)

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

static const char *timeAppAlertCatStr[] =
{
  "New Alert:",      //  Simple Alert
  "New Email:",      //  Email
  "New News:",       //  News
  "New Call:",       //  Call
  "New Missed:",     //  Missed call
  "New SMS:",        //  SMS
  "New Vmail:",      //  Voice mail
  "New Sched:",      //  Schedule
  "New E!!!:",       //  High Prioritized Alert
  "New IM:"          //  Instant Message
};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void Time_displayAlert(uint8_t *pValue, uint8_t len);

/*********************************************************************
 * @fn      Time_indGattMsg
 *
 * @brief   Handle indications and notifications. 
 *
 * @param   pMsg - GATT message.
 *
 * @return  none
 */
void Time_indGattMsg(gattMsgEvent_t *pMsg)
{
  uint8_t i;
  
  // Look up the handle in the handle cache
  for (i = 0; i < HDL_CACHE_LEN; i++)
  {
    if (pMsg->msg.handleValueNoti.handle == Time_handleCache[i])
    {
      break;
    }
  }

  // Perform processing for this handle 
  switch (i)
  {
    case HDL_CURR_TIME_CT_TIME_START:
      // Set clock to time read from time server
      Time_clockSet(pMsg->msg.handleValueNoti.pValue);
      break;
   
    case HDL_NWA_NWA_START:             
      // Display network availability state
      if (pMsg->msg.handleValueInd.pValue[0] == 1)
      {
        LCD_WRITE_STRING("Network: Yes", LCD_PAGE0);
      }
      else
      {
        LCD_WRITE_STRING("Network: None", LCD_PAGE0);
      }
      break;
      
    case HDL_ALERT_NTF_UNREAD_START:
      // Display unread message alert
      {
        uint8_t *p = pMsg->msg.handleValueNoti.pValue;
        uint8_t len = pMsg->msg.handleValueNoti.len;
        
        if (p[0] <= ALERT_CAT_ID_MAX && len == 2)
        {
          LCD_WRITE_STRING_VALUE((char *) timeAppAlertCatStr[p[0]], p[1], 10, 
                                 LCD_PAGE0);
          LCD_WRITE_STRING("", LCD_PAGE1);
        }
      }
      break;
      
    case HDL_ALERT_NTF_NEW_START:
      // Display incoming message
      Time_displayAlert(pMsg->msg.handleValueNoti.pValue,
                          pMsg->msg.handleValueNoti.len);
      break;

    case HDL_BATT_LEVEL_START:
      // Display battery level
      LCD_WRITE_STRING_VALUE("Battery%", pMsg->msg.handleValueNoti.pValue[0], 
                             10, LCD_PAGE1);
      break;

    case HDL_PAS_ALERT_START:
      // Display phone alert status
      LCD_WRITE_STRING_VALUE("Phone Alert:",  
                             pMsg->msg.handleValueNoti.pValue[0], 16, 
                             LCD_PAGE0);
      break;

    case HDL_PAS_RINGER_START:
      // Display ringer state
      if (pMsg->msg.handleValueNoti.pValue[0] == 0)
      {
        LCD_WRITE_STRING("Ringer Off", LCD_PAGE1);
      }
      else
      {
        LCD_WRITE_STRING("Ringer On", LCD_PAGE1);
      }    
      break;
 
     default:
      break;
  }
  
  // Send confirm for indication
  if (pMsg->method == ATT_HANDLE_VALUE_IND)
  {
    ATT_HandleValueCfm(pMsg->connHandle);
  }
}

/*********************************************************************
 * @fn      Time_displayAlert
 *
 * @brief   Display the alert. 
 *
 * @param   pValue - Pointer to buffer containing a new incoming alert
 *                   characteristic.
 * @param   len - length of pValue.
 *
 * @return  none
 */
static void Time_displayAlert(uint8_t *pValue, uint8_t len)
{
  char buf[(LCD_COLS/LCD_CHAR_WIDTH) + 1];
  uint8_t buflen;
  
  // Verify alert category and length
  if ( pValue[0] <= ALERT_CAT_ID_MAX && len >= 2 )
  {
    // Write category and number of unread to first line
    LCD_WRITE_STRING_VALUE((char *) timeAppAlertCatStr[pValue[0]], pValue[1], 
                           10, LCD_PAGE0);
    
    pValue += 2;
    len -= 2;
    
    // Write alert text to second line
    buflen =  MIN((LCD_COLS/LCD_CHAR_WIDTH), len);
    memcpy(buf, pValue, buflen);
    buf[buflen] = '\0';
    LCD_WRITE_STRING(buf, LCD_PAGE1);
  }
}


/*********************************************************************
*********************************************************************/
