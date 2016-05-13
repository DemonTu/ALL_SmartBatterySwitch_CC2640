/*******************************************************************************
  Filename:       glucose_discovery.c
  Revised:        $Date: 2015-01-02 17:16:18 -0800 (Fri, 02 Jan 2015) $
  Revision:       $Revision: 41601 $

  Description:    Glucose Collector App service and characteristic discovery 
                  routines for use with the CC2540 Bluetooth Low Energy 
                  Protocol Stack.

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
#include <string.h>
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>

#include "bcomdef.h"
#include "gatt.h"
#include "gatt_uuid.h"
#include "gatt_profile_uuid.h"
#include "glucoseCollector.h"
#include "glucservice.h"
#include "devinfoservice.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS
 */
// Length of Characteristic declaration + handle with 16 bit UUID
#define CHAR_DESC_HDL_UUID16_LEN        7

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */
// Attribute handle cache
uint16_t glucoseHdlCache[HDL_CACHE_LEN];

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

// Attribute handles used during discovery
static uint16_t glucoseSvcStartHdl;
static uint16_t glucoseSvcEndHdl;
static uint8_t  glucoseEndHdlIdx;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static uint8_t GlucoseDisc(uint8_t state, gattMsgEvent_t *pMsg);
static uint8_t GlucoseDevInfoDisc(uint8_t state, gattMsgEvent_t *pMsg);

/*********************************************************************
 * @fn      glucoseDiscStart()
 *
 * @brief   Start service discovery. 
 *
 * @param   none
 *
 * @return  New discovery state.
 */
uint8_t glucoseDiscStart(void)
{
  // Clear handle cache
  memset(glucoseHdlCache, 0, sizeof(glucoseHdlCache));
  
  // Start discovery with first service
  return glucoseDiscGattMsg(DISC_GLUCOSE_START, NULL);
}

/*********************************************************************
 * @fn      glucoseDiscGattMsg()
 *
 * @brief   Handle GATT messages for characteristic discovery. 
 *
 * @param   state - Discovery state.
 * @param   pMsg - GATT message.
 *
 * @return  New discovery state.
 */
uint8_t glucoseDiscGattMsg(uint8_t state, gattMsgEvent_t *pMsg)
{
  // Execute discovery function for service
  do
  { 
    switch (state & 0xF0)
    {
      // Current glucose service
      case DISC_GLUCOSE_START:
        state = GlucoseDisc(state, pMsg);

        if(state == DISC_IDLE)
        {
          state = DISC_DEVINFO_START;
        }
        break;

      case DISC_DEVINFO_START:
        state = GlucoseDevInfoDisc(state, pMsg);
        break;

      default:
        break;
    }
  } while ((state != 0) && ((state & 0x0F) == 0));
  
  return state;    
}

/*********************************************************************
 * @fn      GlucoseDisc
 *
 * @brief   Current glucose service and characteristic discovery. 
 *
 * @param   state - Discovery state.
 * @param   pMsg - GATT message.
 *
 * @return  New discovery state.
 */
static uint8_t GlucoseDisc(uint8_t state, gattMsgEvent_t *pMsg)
{
  uint8_t newState = state;
  
  switch (state)
  {
    case DISC_GLUCOSE_START:  
      {
        uint8_t uuid[ATT_BT_UUID_SIZE] = { LO_UINT16(GLUCOSE_SERV_UUID),
                                           HI_UINT16(GLUCOSE_SERV_UUID) };

        // Initialize service discovery variables
        glucoseSvcStartHdl = glucoseSvcEndHdl = 0;
        glucoseEndHdlIdx = 0;
        
        // Discover service by UUID
        GATT_DiscPrimaryServiceByUUID(glucCollConnHandle, uuid,
                                      ATT_BT_UUID_SIZE, glucCollTaskId);      

        newState = DISC_GLUCOSE_SVC;
      } 
      break;

    case DISC_GLUCOSE_SVC:
      // Service found, store handles
      if (pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP &&
          pMsg->msg.findByTypeValueRsp.numInfo > 0)
      {
        glucoseSvcStartHdl = 
          ATT_ATTR_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
        glucoseSvcEndHdl = 
          ATT_GRP_END_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
      }
      
      // If procedure complete
      if ((pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP  && 
           pMsg->hdr.status == bleProcedureComplete) ||
          (pMsg->method == ATT_ERROR_RSP))
      {
        // If service found
        if (glucoseSvcStartHdl != 0)
        {
          // Discover all characteristics
          GATT_DiscAllChars(glucCollConnHandle, glucoseSvcStartHdl,
                            glucoseSvcEndHdl, glucCollTaskId);
          
          newState = DISC_GLUCOSE_CHAR;
        }
        else
        {
          // Service not found
          newState = DISC_FAILED;
        }
      }
      break;

    case DISC_GLUCOSE_CHAR:
      {
        uint8_t   i;
        uint8_t   *p;
        uint16_t  handle;
        uint16_t  uuid;

        // Characteristics found
        if (pMsg->method == ATT_READ_BY_TYPE_RSP &&
            pMsg->msg.readByTypeRsp.numPairs > 0 && 
            pMsg->msg.readByTypeRsp.len == CHAR_DESC_HDL_UUID16_LEN)
        {
          // For each characteristic declaration
          p = pMsg->msg.readByTypeRsp.pDataList;

          for (i = pMsg->msg.readByTypeRsp.numPairs; i > 0; i--)
          {
            // Parse characteristic declaration
            handle = BUILD_UINT16(p[3], p[4]);
            uuid = BUILD_UINT16(p[5], p[6]);

            // If looking for end handle
            if (glucoseEndHdlIdx != 0)
            {
              // End handle is one less than handle of characteristic declaration
              glucoseHdlCache[glucoseEndHdlIdx] = BUILD_UINT16(p[0], p[1]) - 1;
              glucoseEndHdlIdx = 0;
            }

            // If UUID is of interest, store handle
            switch (uuid)
            {
              case GLUCOSE_MEAS_UUID:
                glucoseHdlCache[HDL_GLUCOSE_START] = handle;
                glucoseEndHdlIdx = HDL_GLUCOSE_END;
                break;

              case GLUCOSE_CONTEXT_UUID:
                glucoseHdlCache[HDL_GLUCOSE_CONTEXT_START] = handle;
                glucoseEndHdlIdx = HDL_GLUCOSE_CONTEXT_END;
                break;

              case RECORD_CTRL_PT_UUID:
                glucoseHdlCache[HDL_GLUCOSE_CTL_PNT_START] = handle;
                glucoseEndHdlIdx = HDL_GLUCOSE_CTL_PNT_END;
                break;                
                
              case GLUCOSE_FEATURE_UUID:
                glucoseHdlCache[HDL_GLUCOSE_FEATURE] = handle;
                break;
                
              default:
                break;
            }
            
            p += CHAR_DESC_HDL_UUID16_LEN;
          }         
        }
          
        // If procedure complete
        if ((pMsg->method == ATT_READ_BY_TYPE_RSP  && 
             pMsg->hdr.status == bleProcedureComplete) ||
            (pMsg->method == ATT_ERROR_RSP))
        {
          // Special case of end handle at end of service
          if (glucoseEndHdlIdx != 0)
          {
            glucoseHdlCache[glucoseEndHdlIdx] = glucoseSvcEndHdl;
            glucoseEndHdlIdx = 0;
          }
          
          // If didn't find glucose characteristic
          if (glucoseHdlCache[HDL_GLUCOSE_START] == 0)
          {
            newState = DISC_FAILED;
          }
          else if (glucoseHdlCache[HDL_GLUCOSE_START] <
                   glucoseHdlCache[HDL_GLUCOSE_END])
          {
            // Discover characteristic descriptors
            GATT_DiscAllCharDescs(glucCollConnHandle,
                                  glucoseHdlCache[HDL_GLUCOSE_START] + 1,
                                  glucoseHdlCache[HDL_GLUCOSE_END],
                                  glucCollTaskId);
                                        
            newState = DISC_GLUCOSE_CCCD;
          }
          else
          {
            newState = DISC_IDLE;
          }
        }
      }      
      break;

    case DISC_GLUCOSE_CCCD:
      {
        uint8_t i;
        
        // Characteristic descriptors found
        if (pMsg->method == ATT_FIND_INFO_RSP &&
            pMsg->msg.findInfoRsp.numInfo > 0 && 
            pMsg->msg.findInfoRsp.format == ATT_HANDLE_BT_UUID_TYPE)
        {
          attFindInfoRsp_t *pRsp = &(pMsg->msg.findInfoRsp);
                      
          // For each handle/uuid pair
          for (i = 0; i < pRsp->numInfo; i++)
          {           
            // Look for CCCD
            if ( ATT_BT_PAIR_UUID( pRsp->pInfo, i ) == GATT_CLIENT_CHAR_CFG_UUID )
            {
              // CCCD found
              glucoseHdlCache[HDL_GLUCOSE_MEAS_CCCD] = 
                ATT_BT_PAIR_HANDLE( pRsp->pInfo, i );
              
              break;
            }
          }
        }
        
        // If procedure complete
        if ((pMsg->method == ATT_FIND_INFO_RSP  && 
             pMsg->hdr.status == bleProcedureComplete) ||
            (pMsg->method == ATT_ERROR_RSP))
        {
          // If CCCD found
          if (glucoseHdlCache[HDL_GLUCOSE_MEAS_CCCD] != 0)
          {
            // Should we look for unread category status CCCD
            if (glucoseHdlCache[HDL_GLUCOSE_CTL_PNT_START] <
                glucoseHdlCache[HDL_GLUCOSE_CTL_PNT_END])
            {
              // Discover unread category status characteristic descriptors
              GATT_DiscAllCharDescs(glucCollConnHandle,
                                    glucoseHdlCache[HDL_GLUCOSE_CTL_PNT_START] + 1,
                                    glucoseHdlCache[HDL_GLUCOSE_CTL_PNT_END],
                                    glucCollTaskId);
                                          
              newState = DISC_GLUCOSE_CTL_PNT_CCCD;
            }
            else
            {
              // Missing required characteristic
              newState = DISC_FAILED;
            }
          }
          else
          {
            // Missing required characteristic descriptor
            glucoseHdlCache[HDL_GLUCOSE_MEAS_CCCD] = 0;
            newState = DISC_FAILED;
          }          
        }
      }
      break;
         
   case DISC_GLUCOSE_CTL_PNT_CCCD:
      {
        uint8_t i;
        
        // Characteristic descriptors found
        if (pMsg->method == ATT_FIND_INFO_RSP &&
            pMsg->msg.findInfoRsp.numInfo > 0 && 
            pMsg->msg.findInfoRsp.format == ATT_HANDLE_BT_UUID_TYPE)
        {
          attFindInfoRsp_t *pRsp = &(pMsg->msg.findInfoRsp);
          
          // For each handle/uuid pair
          for (i = 0; i < pRsp->numInfo; i++)
          {
            // Look for CCCD
            if ( ATT_BT_PAIR_UUID( pRsp->pInfo, i ) == GATT_CLIENT_CHAR_CFG_UUID )
            {
              // CCCD found
              glucoseHdlCache[HDL_GLUCOSE_CTL_PNT_CCCD] =
                ATT_BT_PAIR_HANDLE( pRsp->pInfo, i );
              
              break;
            }
          }
        }
        
        // If procedure complete
        if ((pMsg->method == ATT_FIND_INFO_RSP  && 
             pMsg->hdr.status == bleProcedureComplete) ||
            (pMsg->method == ATT_ERROR_RSP))
        {
          // If CCCD found
          if (glucoseHdlCache[HDL_GLUCOSE_CTL_PNT_CCCD] != 0)
          {
            // Should we look for unread category status CCCD
            if (glucoseHdlCache[HDL_GLUCOSE_CONTEXT_START] <
                glucoseHdlCache[HDL_GLUCOSE_CONTEXT_END])
            {
              // Discover unread category status characteristic descriptors
              GATT_DiscAllCharDescs(glucCollConnHandle,
                                    glucoseHdlCache[HDL_GLUCOSE_CONTEXT_START] + 1,
                                    glucoseHdlCache[HDL_GLUCOSE_CONTEXT_END],
                                    glucCollTaskId);
                                          
              newState = DISC_GLUCOSE_CONTEXT_CCCD;
            }
            else
            {
              // Done
              newState = DISC_IDLE;
            }
          }
          else
          {
            // Missing required characteristic descriptor
            glucoseHdlCache[HDL_GLUCOSE_CTL_PNT_CCCD] = 0;
            newState = DISC_FAILED;
          }          
        }
      }
      break;

   case DISC_GLUCOSE_CONTEXT_CCCD:
      {
        uint8_t i;
        
        // Characteristic descriptors found
        if (pMsg->method == ATT_FIND_INFO_RSP &&
            pMsg->msg.findInfoRsp.numInfo > 0 && 
            pMsg->msg.findInfoRsp.format == ATT_HANDLE_BT_UUID_TYPE)
        {
          attFindInfoRsp_t *pRsp = &(pMsg->msg.findInfoRsp);
          
          // For each handle/uuid pair
          for (i = 0; i < pRsp->numInfo; i++)
          {
            // Look for CCCD
            if ( ATT_BT_PAIR_UUID( pRsp->pInfo, i ) == GATT_CLIENT_CHAR_CFG_UUID )
            {
              // CCCD found
              glucoseHdlCache[HDL_GLUCOSE_CONTEXT_CCCD] =
                ATT_BT_PAIR_HANDLE( pRsp->pInfo, i );
              
              break;
            }
          }
        }
        
        // If procedure complete
        if ((pMsg->method == ATT_FIND_INFO_RSP  && 
             pMsg->hdr.status == bleProcedureComplete) ||
            (pMsg->method == ATT_ERROR_RSP))
        {
          newState = DISC_IDLE;
        }
      }
      break;
      
    default:
      break;
  }
  
  return newState;
}

/*********************************************************************
 * @fn      GlucoseDevInfoDisc
 *
 * @brief   Current glucose service and characteristic discovery. 
 *
 * @param   state - Discovery state.
 * @param   pMsg - GATT message.
 *
 * @return  New discovery state.
 */
static uint8_t GlucoseDevInfoDisc(uint8_t state, gattMsgEvent_t *pMsg)
{
  uint8_t newState = state;
  
  switch (state)
  {
    case DISC_DEVINFO_START:  
      {
        uint8_t uuid[ATT_BT_UUID_SIZE] = { LO_UINT16(DEVINFO_SERV_UUID),
                                           HI_UINT16(DEVINFO_SERV_UUID) };

        // Initialize service discovery variables
        glucoseSvcStartHdl = glucoseSvcEndHdl = 0;
        glucoseEndHdlIdx = 0;
        
        // Discover service by UUID
        GATT_DiscPrimaryServiceByUUID(glucCollConnHandle, uuid,
                                      ATT_BT_UUID_SIZE, glucCollTaskId);      

        newState = DISC_DEVINFO_SVC;
      } 
      break;

    case DISC_DEVINFO_SVC:
      // Service found, store handles
      if (pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP &&
          pMsg->msg.findByTypeValueRsp.numInfo > 0)
      {
        glucoseSvcStartHdl = 
          ATT_ATTR_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
        glucoseSvcEndHdl = 
          ATT_GRP_END_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
      }
      
      // If procedure complete
      if ((pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP  && 
           pMsg->hdr.status == bleProcedureComplete) ||
          (pMsg->method == ATT_ERROR_RSP))
      {
        // If service found
        if (glucoseSvcStartHdl != 0)
        {
          // Discover all characteristics
          GATT_DiscAllChars(glucCollConnHandle, glucoseSvcStartHdl,
                            glucoseSvcEndHdl, glucCollTaskId);
          
          newState = DISC_DEVINFO_CHAR;
        }
        else
        {
          // Service not found
          newState = DISC_FAILED;
        }
      }    
      break;

    case DISC_DEVINFO_CHAR:
      {
        uint8_t   i;
        uint8_t   *p;
        uint16_t  handle;
        uint16_t  uuid;
        
        // Characteristics found
        if (pMsg->method == ATT_READ_BY_TYPE_RSP &&
            pMsg->msg.readByTypeRsp.numPairs > 0 && 
            pMsg->msg.readByTypeRsp.len == CHAR_DESC_HDL_UUID16_LEN)
        {
          // For each characteristic declaration
          p = pMsg->msg.readByTypeRsp.pDataList;
          
          for (i = pMsg->msg.readByTypeRsp.numPairs; i > 0; i--)
          {
            // Parse characteristic declaration
            handle = BUILD_UINT16(p[3], p[4]);
            uuid = BUILD_UINT16(p[5], p[6]);
                   
            // If UUID is of interest, store handle
            switch (uuid)
            {
              case MANUFACTURER_NAME_UUID:
                glucoseHdlCache[HDL_DEVINFO_MANUFACTURER_NAME] = handle;
                break;                
                
              case SYSTEM_ID_UUID:
                glucoseHdlCache[HDL_DEVINFO_SYSTEM_ID] = handle;
                break;
                
              case MODEL_NUMBER_UUID:
                glucoseHdlCache[HDL_DEVINFO_MODEL_NUM] = handle;
                break;
                
              default:
                break;
            }
            
            p += CHAR_DESC_HDL_UUID16_LEN;
          }
        }
          
        // If procedure complete
        if ((pMsg->method == ATT_READ_BY_TYPE_RSP  && 
             pMsg->hdr.status == bleProcedureComplete) ||
            (pMsg->method == ATT_ERROR_RSP))
        {
          // If didn't find required device info
          if (glucoseHdlCache[HDL_DEVINFO_MANUFACTURER_NAME] == 0 ||
              glucoseHdlCache[HDL_DEVINFO_SYSTEM_ID] == 0 ||
              glucoseHdlCache[HDL_DEVINFO_MODEL_NUM] == 0)
          {
            newState = DISC_FAILED;
          }
          else
          {
            newState = DISC_IDLE;
          }
        }
      }      
      break;
      
    default:
      break;
  }
  
  return newState;
}


/*********************************************************************
 *********************************************************************/
