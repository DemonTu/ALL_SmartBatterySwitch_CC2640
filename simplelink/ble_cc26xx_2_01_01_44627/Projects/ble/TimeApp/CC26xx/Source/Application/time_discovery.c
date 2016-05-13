/*******************************************************************************
  Filename:       time_discovery.c
  Revised:        $Date: 2015-06-16 15:14:24 -0700 (Tue, 16 Jun 2015) $
  Revision:       $Revision: 44104 $

  Description:    Time  service and characteristic discovery routines.

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
#include "bcomdef.h"

#include <ICall.h>

#include "gatt.h"
#include "gatt_uuid.h"
#include "gatt_profile_uuid.h"
#include "bletime.h"

/*********************************************************************
 * MACROS
 */

// Length of Characteristic declaration + handle with 16 bit UUID
#define CHAR_DESC_HDL_UUID16_LEN        7

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

// Attribute handle cache
uint16_t Time_handleCache[HDL_CACHE_LEN];

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

// Attribute handles used during discovery
static uint16_t Time_svcStartHdl;
static uint16_t Time_svcEndHdl;
static uint8_t Time_endHdlIdx;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static uint8_t Time_discCurrTime(uint8_t state, gattMsgEvent_t *pMsg);
static uint8_t Time_discDstChg(uint8_t state, gattMsgEvent_t *pMsg);
static uint8_t Time_discRefTime(uint8_t state, gattMsgEvent_t *pMsg);
static uint8_t Time_discNwa(uint8_t state, gattMsgEvent_t *pMsg);
static uint8_t Time_discAlertNtf(uint8_t state, gattMsgEvent_t *pMsg);
static uint8_t Time_discBatt(uint8_t state, gattMsgEvent_t *pMsg);
static uint8_t Time_discPhoneAlert(uint8_t state, gattMsgEvent_t *pMsg);

/*********************************************************************
 * @fn      Time_discStart()
 *
 * @brief   Start service discovery. 
 *
 * @return  New discovery state.
 */
uint8_t Time_discStart(void)
{
  // Clear handle cache
  memset(Time_handleCache, 0, sizeof(Time_handleCache));
  
  // Start discovery with first service
  return Time_discGattMsg(DISC_CURR_TIME_START, NULL);
}

/*********************************************************************
 * @fn      Time_discGattMsg()
 *
 * @brief   Handle GATT messages for characteristic discovery. 
 *
 * @param   state - Discovery state.
 * @param   pMsg  - GATT message.
 *
 * @return  New discovery state.
 */
uint8_t Time_discGattMsg(uint8_t state, gattMsgEvent_t *pMsg)
{
  // Execute discovery function for service
  do
  {
    switch (state & 0xF0)
    {
      // Current time service
      case DISC_CURR_TIME_START:
        state = Time_discCurrTime(state, pMsg);
        if (state == DISC_FAILED)
        {
          state = DISC_NWA_START;
        }
        else if (state == DISC_IDLE)
        {
          state = DISC_DST_CHG_START;
        }
        break;

      // DST change service
      case DISC_DST_CHG_START:
        state = Time_discDstChg(state, pMsg);
        if (state == DISC_FAILED)
        {
          state = DISC_NWA_START;
        }
        else if (state == DISC_IDLE)
        {
          state = DISC_REF_TIME_START;
        }
        break;

      // Reference time service
      case DISC_REF_TIME_START:
        state = Time_discRefTime(state, pMsg);
        if (state == DISC_FAILED || state == DISC_IDLE)
        {
          state = DISC_NWA_START;
        }
        break;

      // NwA service
      case DISC_NWA_START:
        state = Time_discNwa(state, pMsg);
        if (state == DISC_FAILED || state == DISC_IDLE)
        {
          state = DISC_ALERT_NTF_START;
        }
        break;

      // Alert notification service
      case DISC_ALERT_NTF_START:
        state = Time_discAlertNtf(state, pMsg);
        if (state == DISC_FAILED || state == DISC_IDLE)
        {
          state = DISC_BATT_START;
        }
        break;

      // Battery service
      case DISC_BATT_START:
        state = Time_discBatt(state, pMsg);
        if (state == DISC_FAILED || state == DISC_IDLE)
        {
          state = DISC_PAS_START;
        }
        break;

      // Phone alert status service
      case DISC_PAS_START:
        state = Time_discPhoneAlert(state, pMsg);
        if (state == DISC_FAILED)
        {
          state = DISC_IDLE;
        }
        break;

      default:
        break;
    }       
  } while ((state != 0) && ((state & 0x0F) == 0));
  
  return state;
}

/*********************************************************************
 * @fn      Time_discCurrTime()
 *
 * @brief   Current time service and characteristic discovery. 
 *
 * @param   state - Discovery state.
 * @param   pMsg  - GATT message.
 *
 * @return  New discovery state.
 */
static uint8_t Time_discCurrTime(uint8_t state, gattMsgEvent_t *pMsg)
{
  uint8_t newState = state;
  
  switch (state)
  {
    case DISC_CURR_TIME_START:  
      {
        uint8_t uuid[ATT_BT_UUID_SIZE] = { LO_UINT16(CURRENT_TIME_SERV_UUID),
                                           HI_UINT16(CURRENT_TIME_SERV_UUID) };

        // Initialize service discovery variables
        Time_svcStartHdl = Time_svcEndHdl = 0;
        Time_endHdlIdx = 0;
        
        // Discover service by UUID
        GATT_DiscPrimaryServiceByUUID(Time_connHandle, uuid,
                                      ATT_BT_UUID_SIZE, ICall_getEntityId());      

        newState = DISC_CURR_TIME_SVC;
      } 
      break;

    case DISC_CURR_TIME_SVC:
      // Service found, store handles
      if (pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP &&
          pMsg->msg.findByTypeValueRsp.numInfo > 0)
      {
        Time_svcStartHdl = 
          ATT_ATTR_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
        Time_svcEndHdl = 
          ATT_GRP_END_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
      }
      
      // If procedure complete
      if ((pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP  && 
           pMsg->hdr.status == bleProcedureComplete) ||
          (pMsg->method == ATT_ERROR_RSP))
      {
        // If service found
        if (Time_svcStartHdl != 0)
        {
          // Discover all characteristics
          GATT_DiscAllChars(Time_connHandle, Time_svcStartHdl,
                            Time_svcEndHdl, ICall_getEntityId());
          
          newState = DISC_CURR_TIME_CHAR;
        }
        else
        {
          // Service not found
          newState = DISC_FAILED;
        }
      }    
      break;

    case DISC_CURR_TIME_CHAR:
      {
         // Characteristics found
        if (pMsg->method == ATT_READ_BY_TYPE_RSP &&
            pMsg->msg.readByTypeRsp.numPairs > 0 && 
            pMsg->msg.readByTypeRsp.len == CHAR_DESC_HDL_UUID16_LEN)
        {
          uint8_t   i;
          uint8_t   *p;
          uint16_t  handle;
          uint16_t  uuid;
          
          p = pMsg->msg.readByTypeRsp.pDataList;
          
          // For each characteristic declaration
          for (i = pMsg->msg.readByTypeRsp.numPairs; i > 0; i--)
          {
            // Parse characteristic declaration
            handle = BUILD_UINT16(p[3], p[4]);
            uuid = BUILD_UINT16(p[5], p[6]);
                   
            // If looking for end handle
            if (Time_endHdlIdx != 0)
            {
              // End handle is one less than handle of characteristic 
              // declaration.
              Time_handleCache[Time_endHdlIdx] = BUILD_UINT16(p[0], p[1]) - 1;
              
              Time_endHdlIdx = 0;
            }

            // If UUID is of interest, store handle
            switch (uuid)
            {
              case CURRENT_TIME_UUID:
                Time_handleCache[HDL_CURR_TIME_CT_TIME_START] = handle;
                Time_endHdlIdx = HDL_CURR_TIME_CT_TIME_END;
                break;
                
              case LOCAL_TIME_INFO_UUID:
                Time_handleCache[HDL_CURR_TIME_LOC_INFO] = handle;
                break;
                
              case REF_TIME_INFO_UUID:
                Time_handleCache[HDL_CURR_TIME_REF_INFO] = handle;
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
          if (Time_endHdlIdx != 0)
          {
            Time_handleCache[Time_endHdlIdx] = Time_svcEndHdl;
            Time_endHdlIdx = 0;
          }

          // If didn't find time characteristic
          if (Time_handleCache[HDL_CURR_TIME_CT_TIME_START] == 0)
          {
            newState = DISC_FAILED;
          }

          else if (Time_handleCache[HDL_CURR_TIME_CT_TIME_START] < 
                   Time_handleCache[HDL_CURR_TIME_CT_TIME_END])
          {
            // Discover characteristic descriptors
            GATT_DiscAllCharDescs(Time_connHandle,
                                  Time_handleCache[HDL_CURR_TIME_CT_TIME_START] + 1,
                                  Time_handleCache[HDL_CURR_TIME_CT_TIME_END],
                                  ICall_getEntityId());
                                        
            newState = DISC_CURR_TIME_CT_TIME_CCCD;
          }
          else
          {
            newState = DISC_IDLE;
          }
        }
      }      
      break;

    case DISC_CURR_TIME_CT_TIME_CCCD:
      {
        // Characteristic descriptors found
        if (pMsg->method == ATT_FIND_INFO_RSP &&
            pMsg->msg.findInfoRsp.numInfo > 0 && 
            pMsg->msg.findInfoRsp.format == ATT_HANDLE_BT_UUID_TYPE)
        {
          uint8_t i;
          
          // For each handle/uuid pair
          for (i = 0; i < pMsg->msg.findInfoRsp.numInfo; i++)
          {
            // Look for CCCD
            if (ATT_BT_PAIR_UUID(pMsg->msg.findInfoRsp.pInfo, i) ==
                GATT_CLIENT_CHAR_CFG_UUID)
            {
              // CCCD found
              Time_handleCache[HDL_CURR_TIME_CT_TIME_CCCD] =
                ATT_BT_PAIR_HANDLE(pMsg->msg.findInfoRsp.pInfo, i);
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
 * @fn      Time_discDstChg()
 *
 * @brief   DST change service and characteristic discovery. 
 *
 * @param   state - Discovery state.
 * @param   pMsg  - GATT message.
 *
 * @return  New discovery state.
 */
static uint8_t Time_discDstChg(uint8_t state, gattMsgEvent_t *pMsg)
{
  uint8_t newState = state;
  
  switch (state)
  {
    case DISC_DST_CHG_START:  
      {
        uint8_t uuid[ATT_BT_UUID_SIZE] = 
          { LO_UINT16(NEXT_DST_CHANGE_SERV_UUID),
            HI_UINT16(NEXT_DST_CHANGE_SERV_UUID) };

        // Initialize service discovery variables
        Time_svcStartHdl = Time_svcEndHdl = 0;
        Time_endHdlIdx = 0;
        
        // Discover service by UUID
        GATT_DiscPrimaryServiceByUUID(Time_connHandle, uuid,
                                      ATT_BT_UUID_SIZE, ICall_getEntityId());      

        newState = DISC_DST_CHG_SVC;
      }
      break;

    case DISC_DST_CHG_SVC:
      // Service found, store handles
      if (pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP &&
          pMsg->msg.findByTypeValueRsp.numInfo > 0)
      {
        Time_svcStartHdl = 
          ATT_ATTR_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
        Time_svcEndHdl = 
          ATT_GRP_END_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
      }
      
      // If procedure complete
      if ((pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP  && 
           pMsg->hdr.status == bleProcedureComplete) ||
          (pMsg->method == ATT_ERROR_RSP))
      {
        // If service found
        if (Time_svcStartHdl != 0)
        {
          // Discover all characteristics
          GATT_DiscAllChars(Time_connHandle, Time_svcStartHdl,
                            Time_svcEndHdl, ICall_getEntityId());
          
          newState = DISC_DST_CHG_CHAR;
        }
        else
        {
          // Service not found
          newState = DISC_FAILED;
        }
      }    
      break;

    case DISC_DST_CHG_CHAR:
      {
        // Characteristics found
        if (pMsg->method == ATT_READ_BY_TYPE_RSP &&
            pMsg->msg.readByTypeRsp.numPairs > 0 && 
            pMsg->msg.readByTypeRsp.len == CHAR_DESC_HDL_UUID16_LEN)
        {
          uint8_t   i;
          uint8_t   *p;
          uint16_t  handle;
          uint16_t  uuid;
            
          p = pMsg->msg.readByTypeRsp.pDataList;
          
          // For each characteristic declaration
          for (i = pMsg->msg.readByTypeRsp.numPairs; i > 0; i--)
          {
            // Parse characteristic declaration
            handle = BUILD_UINT16(p[3], p[4]);
            uuid = BUILD_UINT16(p[5], p[6]);
                   
            // If UUID is of interest, store handle
            switch (uuid)
            {
              case TIME_WITH_DST_UUID:
                Time_handleCache[HDL_DST_CHG_TIME_DST] = handle;
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
          // If didn't find DST characteristic
          if (Time_handleCache[HDL_DST_CHG_TIME_DST] == 0)
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
 * @fn      Time_discRefTime()
 *
 * @brief   Reference time service and characteristic discovery. 
 *
 * @param   state - Discovery state.
 * @param   pMsg  - GATT message.
 *
 * @return  New discovery state.
 */
static uint8_t Time_discRefTime(uint8_t state, gattMsgEvent_t *pMsg)
{
  uint8_t newState = state;
  
  switch (state)
  {
    case DISC_REF_TIME_START:  
      {
        uint8_t uuid[ATT_BT_UUID_SIZE] = 
          { LO_UINT16(REF_TIME_UPDATE_SERV_UUID),
            HI_UINT16(REF_TIME_UPDATE_SERV_UUID) };

        // Initialize service discovery variables
        Time_svcStartHdl = Time_svcEndHdl = 0;
        Time_endHdlIdx = 0;
        
        // Discover service by UUID
        GATT_DiscPrimaryServiceByUUID(Time_connHandle, uuid,
                                      ATT_BT_UUID_SIZE, ICall_getEntityId());

        newState = DISC_REF_TIME_SVC;
      }
      break;

    case DISC_REF_TIME_SVC:
      // Service found, store handles
      if (pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP &&
          pMsg->msg.findByTypeValueRsp.numInfo > 0)
      {
        Time_svcStartHdl = 
          ATT_ATTR_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
        Time_svcEndHdl = 
          ATT_GRP_END_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
      }
      
      // If procedure complete
      if ((pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP  && 
           pMsg->hdr.status == bleProcedureComplete) ||
          (pMsg->method == ATT_ERROR_RSP))
      {
        // If service found
        if (Time_svcStartHdl != 0)
        {
          // Discover all characteristics
          GATT_DiscAllChars(Time_connHandle, Time_svcStartHdl,
                            Time_svcEndHdl, ICall_getEntityId());
          
          newState = DISC_REF_TIME_CHAR;
        }
        else
        {
          // Service not found
          newState = DISC_FAILED;
        }
      }    
      break;

    case DISC_REF_TIME_CHAR:
      {
        // Characteristics found
        if (pMsg->method == ATT_READ_BY_TYPE_RSP &&
             pMsg->msg.readByTypeRsp.numPairs > 0 && 
             pMsg->msg.readByTypeRsp.len == CHAR_DESC_HDL_UUID16_LEN)
        {
          uint8_t   i;
          uint8_t   *p;
          uint16_t  handle;
          uint16_t  uuid;
          
          p = pMsg->msg.readByTypeRsp.pDataList;
          
          // For each characteristic declaration
          for (i = pMsg->msg.readByTypeRsp.numPairs; i > 0; i--)
          {
            // Parse characteristic declaration
            handle = BUILD_UINT16(p[3], p[4]);
            uuid = BUILD_UINT16(p[5], p[6]);
                   
            // If UUID is of interest, store handle
            switch (uuid)
            {
              case TIME_UPDATE_CTRL_PT_UUID:
                Time_handleCache[HDL_REF_TIME_UPD_CTRL] = handle;
                break;
                
              case TIME_UPDATE_STATE_UUID:
                Time_handleCache[HDL_REF_TIME_UPD_STATE] = handle;
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
          // If didn't find mandatory characteristics
          if (Time_handleCache[HDL_REF_TIME_UPD_CTRL] == 0 ||
              Time_handleCache[HDL_REF_TIME_UPD_STATE] == 0)
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
 * @fn      Time_discNwa()
 *
 * @brief   NwA service and characteristic discovery. 
 *
 * @param   state - Discovery state.
 * @param   pMsg  - GATT message.
 *
 * @return  New discovery state.
 */
static uint8_t Time_discNwa(uint8_t state, gattMsgEvent_t *pMsg)
{
  uint8_t newState = state;
  
  switch (state)
  {
    case DISC_NWA_START:  
      {
        uint8_t uuid[ATT_BT_UUID_SIZE] = { LO_UINT16(NWA_SERV_UUID),
                                           HI_UINT16(NWA_SERV_UUID) };

        // Initialize service discovery variables
        Time_svcStartHdl = Time_svcEndHdl = 0;
        Time_endHdlIdx = 0;
        
        // Discover service by UUID
        GATT_DiscPrimaryServiceByUUID(Time_connHandle, uuid,
                                      ATT_BT_UUID_SIZE, ICall_getEntityId());      

        newState = DISC_NWA_SVC;
      }
      break;

    case DISC_NWA_SVC:
      // Service found, store handles
      if (pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP &&
          pMsg->msg.findByTypeValueRsp.numInfo > 0)
      {
        Time_svcStartHdl = 
          ATT_ATTR_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
        Time_svcEndHdl = 
          ATT_GRP_END_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
      }
      
      // If procedure complete
      if ((pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP  && 
           pMsg->hdr.status == bleProcedureComplete) ||
          (pMsg->method == ATT_ERROR_RSP))
      {
        // If service found
        if (Time_svcStartHdl != 0)
        {
          // Discover all characteristics
          GATT_DiscAllChars(Time_connHandle, Time_svcStartHdl,
                            Time_svcEndHdl, ICall_getEntityId());
          
          newState = DISC_NWA_CHAR;
        }
        else
        {
          // Service not found
          newState = DISC_FAILED;
        }
      }    
      break;

    case DISC_NWA_CHAR:
      {
        // Characteristics found
        if (pMsg->method == ATT_READ_BY_TYPE_RSP &&
            pMsg->msg.readByTypeRsp.numPairs > 0 && 
            pMsg->msg.readByTypeRsp.len == CHAR_DESC_HDL_UUID16_LEN)
        {
          uint8_t   i;
          uint8_t   *p;
          uint16_t  handle;
          uint16_t  uuid;
          
          p = pMsg->msg.readByTypeRsp.pDataList;
          
          // For each characteristic declaration
          for (i = pMsg->msg.readByTypeRsp.numPairs; i > 0; i--)
          {
            // Parse characteristic declaration
            handle = BUILD_UINT16(p[3], p[4]);
            uuid = BUILD_UINT16(p[5], p[6]);
                   
            // If looking for end handle
            if (Time_endHdlIdx != 0)
            {
              // End handle is one less than handle of characteristic declaration
              Time_handleCache[Time_endHdlIdx] = BUILD_UINT16(p[0], p[1]) - 1;
              
              Time_endHdlIdx = 0;
            }

            // If UUID is of interest, store handle
            switch (uuid)
            {
              case NETWORK_AVAIL_UUID:
                Time_handleCache[HDL_NWA_NWA_START] = handle;
                Time_endHdlIdx = HDL_NWA_NWA_END;
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
          if (Time_endHdlIdx != 0)
          {
            Time_handleCache[Time_endHdlIdx] = Time_svcEndHdl;
            Time_endHdlIdx = 0;
          }
          
          // If didn't find network availability characteristic
          if (Time_handleCache[HDL_NWA_NWA_START] == 0)
          {
            newState = DISC_FAILED;
          }
          else if (Time_handleCache[HDL_NWA_NWA_START] <
                   Time_handleCache[HDL_NWA_NWA_END])
          {
            // Discover characteristic descriptors
            GATT_DiscAllCharDescs(Time_connHandle,
                                  Time_handleCache[HDL_NWA_NWA_START] + 1,
                                  Time_handleCache[HDL_NWA_NWA_END],
                                  ICall_getEntityId());
                                        
            newState = DISC_NWA_NWA_CCCD;
          }
          else
          {
            newState = DISC_IDLE;
          }
        }
      }      
      break;

    case DISC_NWA_NWA_CCCD:
      {
        // Characteristic descriptors found
        if (pMsg->method == ATT_FIND_INFO_RSP &&
            pMsg->msg.findInfoRsp.numInfo > 0 && 
            pMsg->msg.findInfoRsp.format == ATT_HANDLE_BT_UUID_TYPE)
        {
          uint8_t i;
          
          // For each handle/uuid pair
          for (i = 0; i < pMsg->msg.findInfoRsp.numInfo; i++)
          {
            // Look for CCCD
            if (ATT_BT_PAIR_UUID(pMsg->msg.findInfoRsp.pInfo, i) ==
                GATT_CLIENT_CHAR_CFG_UUID)
            {
              // CCCD found
              Time_handleCache[HDL_NWA_NWA_CCCD] =
                ATT_BT_PAIR_HANDLE(pMsg->msg.findInfoRsp.pInfo, i);
              
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
 * @fn      Time_discAlertNtf()
 *
 * @brief   Alert notification service and characteristic discovery. 
 *
 * @param   state - Discovery state.
 * @param   pMsg  - GATT message.
 *
 * @return  New discovery state.
 */
static uint8_t Time_discAlertNtf(uint8_t state, gattMsgEvent_t *pMsg)
{
  uint8_t newState = state;
  
  switch (state)
  {
    case DISC_ALERT_NTF_START:  
      {
        uint8_t uuid[ATT_BT_UUID_SIZE] = { LO_UINT16(ALERT_NOTIF_SERV_UUID),
                                           HI_UINT16(ALERT_NOTIF_SERV_UUID) };

        // Initialize service discovery variables
        Time_svcStartHdl = Time_svcEndHdl = 0;
        Time_endHdlIdx = 0;
        
        // Discover service by UUID
        GATT_DiscPrimaryServiceByUUID(Time_connHandle, uuid,
                                      ATT_BT_UUID_SIZE, ICall_getEntityId());      

        newState = DISC_ALERT_NTF_SVC;
      }
      break;

    case DISC_ALERT_NTF_SVC:
      // Service found, store handles
      if (pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP &&
          pMsg->msg.findByTypeValueRsp.numInfo > 0)
      {
        Time_svcStartHdl = 
          ATT_ATTR_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
        Time_svcEndHdl = 
          ATT_GRP_END_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
      }
      
      // If procedure complete
      if ((pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP  && 
           pMsg->hdr.status == bleProcedureComplete) ||
          (pMsg->method == ATT_ERROR_RSP))
      {
        // If service found
        if (Time_svcStartHdl != 0)
        {
          // Discover all characteristics
          GATT_DiscAllChars(Time_connHandle, Time_svcStartHdl,
                            Time_svcEndHdl, ICall_getEntityId());
          
          newState = DISC_ALERT_NTF_CHAR;
        }
        else
        {
          // Service not found
          newState = DISC_FAILED;
        }
      }    
      break;

    case DISC_ALERT_NTF_CHAR:
      {
        // Characteristics found
        if (pMsg->method == ATT_READ_BY_TYPE_RSP &&
            pMsg->msg.readByTypeRsp.numPairs > 0 && 
            pMsg->msg.readByTypeRsp.len == CHAR_DESC_HDL_UUID16_LEN)
        {
          uint8_t   i;
          uint8_t   *p;
          uint16_t  handle;
          uint16_t  uuid;
          
          p = pMsg->msg.readByTypeRsp.pDataList;
          
          // For each characteristic declaration
          for (i = pMsg->msg.readByTypeRsp.numPairs; i > 0; i--)
          {
            // Parse characteristic declaration
            handle = BUILD_UINT16(p[3], p[4]);
            uuid = BUILD_UINT16(p[5], p[6]);
                   
            // If looking for end handle
            if (Time_endHdlIdx != 0)
            {
              // End handle is one less than handle of characteristic declaration
              Time_handleCache[Time_endHdlIdx] = BUILD_UINT16(p[0], p[1]) - 1;
              
              Time_endHdlIdx = 0;
            }

            // If UUID is of interest, store handle
            switch (uuid)
            {
              case ALERT_NOTIF_CTRL_PT_UUID:
                Time_handleCache[HDL_ALERT_NTF_CTRL] = handle;
                break;

              case UNREAD_ALERT_STATUS_UUID:
                Time_handleCache[HDL_ALERT_NTF_UNREAD_START] = handle;
                Time_endHdlIdx = HDL_ALERT_NTF_UNREAD_END;
                break;

              case NEW_ALERT_UUID:
                Time_handleCache[HDL_ALERT_NTF_NEW_START] = handle;
                Time_endHdlIdx = HDL_ALERT_NTF_NEW_END;
                break;

              case SUP_NEW_ALERT_CAT_UUID:
                Time_handleCache[HDL_ALERT_NTF_NEW_CAT] = handle;
                break;

              case SUP_UNREAD_ALERT_CAT_UUID:
                Time_handleCache[HDL_ALERT_NTF_UNREAD_CAT] = handle;
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
          if (Time_endHdlIdx != 0)
          {
            Time_handleCache[Time_endHdlIdx] = Time_svcEndHdl;
            Time_endHdlIdx = 0;
          }
          
          // If didn't find new alert characteristic
          if (Time_handleCache[HDL_ALERT_NTF_NEW_START] == 0)
          {
            newState = DISC_FAILED;
          }
          else if (Time_handleCache[HDL_ALERT_NTF_NEW_START] <
                   Time_handleCache[HDL_ALERT_NTF_NEW_END])
          {
            // Discover incoming alert characteristic descriptors
            GATT_DiscAllCharDescs(Time_connHandle,
                                  Time_handleCache[HDL_ALERT_NTF_NEW_START] + 1,
                                  Time_handleCache[HDL_ALERT_NTF_NEW_END],
                                  ICall_getEntityId());
                                        
            newState = DISC_ALERT_NTF_NEW_CCCD;
          }
          else
          {
            // Missing required characteristic descriptor
            Time_handleCache[HDL_ALERT_NTF_NEW_START] = 0;
            newState = DISC_FAILED;
          }
        }
      }      
      break;

    case DISC_ALERT_NTF_NEW_CCCD:
      {
        // Characteristic descriptors found
        if (pMsg->method == ATT_FIND_INFO_RSP &&
            pMsg->msg.findInfoRsp.numInfo > 0 && 
            pMsg->msg.findInfoRsp.format == ATT_HANDLE_BT_UUID_TYPE)
        {
          uint8_t i;
          
          // For each handle/uuid pair
          for (i = 0; i < pMsg->msg.findInfoRsp.numInfo; i++)
          {
            // Look for CCCD
            if (ATT_BT_PAIR_UUID(pMsg->msg.findInfoRsp.pInfo, i) ==
                GATT_CLIENT_CHAR_CFG_UUID)
            {
              // CCCD found
              Time_handleCache[HDL_ALERT_NTF_NEW_CCCD] =
                ATT_BT_PAIR_HANDLE(pMsg->msg.findInfoRsp.pInfo, i);
              
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
          if (Time_handleCache[HDL_ALERT_NTF_NEW_CCCD] != 0)
          {
            // Should we look for unread category status CCCD
            if (Time_handleCache[HDL_ALERT_NTF_UNREAD_START] <
                Time_handleCache[HDL_ALERT_NTF_UNREAD_END])
            {
              // Discover unread category status characteristic descriptors
              GATT_DiscAllCharDescs(Time_connHandle,
                                    Time_handleCache[HDL_ALERT_NTF_UNREAD_START] + 1,
                                    Time_handleCache[HDL_ALERT_NTF_UNREAD_END],
                                    ICall_getEntityId());
                                          
              newState = DISC_ALERT_NTF_UNREAD_CCCD;
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
            Time_handleCache[HDL_ALERT_NTF_NEW_START] = 0;
            newState = DISC_FAILED;
          }          
        }
      }
      break;

    case DISC_ALERT_NTF_UNREAD_CCCD:
      {
        // Characteristic descriptors found
        if (pMsg->method == ATT_FIND_INFO_RSP &&
            pMsg->msg.findInfoRsp.numInfo > 0 && 
            pMsg->msg.findInfoRsp.format == ATT_HANDLE_BT_UUID_TYPE)
        {
          uint8_t i;
          
          // For each handle/uuid pair
          for (i = 0; i < pMsg->msg.findInfoRsp.numInfo; i++)
          {
            // Look for CCCD
            if (ATT_BT_PAIR_UUID(pMsg->msg.findInfoRsp.pInfo, i) ==
                GATT_CLIENT_CHAR_CFG_UUID)
            {
              // CCCD found
              Time_handleCache[HDL_ALERT_NTF_UNREAD_CCCD] =
                ATT_BT_PAIR_HANDLE(pMsg->msg.findInfoRsp.pInfo, i);
              
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
 * @fn      Time_discBatt()
 *
 * @brief   Battery service and characteristic discovery. 
 *
 * @param   state - Discovery state.
 * @param   pMsg  - GATT message.
 *
 * @return  New discovery state.
 */
static uint8_t Time_discBatt(uint8_t state, gattMsgEvent_t *pMsg)
{
  uint8_t newState = state;
  
  switch (state)
  {
    case DISC_BATT_START:  
      {
        uint8_t uuid[ATT_BT_UUID_SIZE] = { LO_UINT16(BATT_SERV_UUID),
                                           HI_UINT16(BATT_SERV_UUID) };

        // Initialize service discovery variables
        Time_svcStartHdl = Time_svcEndHdl = 0;
        Time_endHdlIdx = 0;
        
        // Discover service by UUID
        GATT_DiscPrimaryServiceByUUID(Time_connHandle, uuid,
                                      ATT_BT_UUID_SIZE, ICall_getEntityId());      

        newState = DISC_BATT_SVC;
      }
      break;

    case DISC_BATT_SVC:
      // Service found, store handles
      if (pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP &&
          pMsg->msg.findByTypeValueRsp.numInfo > 0)
      {
        Time_svcStartHdl = 
          ATT_ATTR_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
        Time_svcEndHdl = 
          ATT_GRP_END_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
      }
      
      // If procedure complete
      if ((pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP  && 
           pMsg->hdr.status == bleProcedureComplete) ||
          (pMsg->method == ATT_ERROR_RSP))
      {
        // If service found
        if (Time_svcStartHdl != 0)
        {
          // Discover all characteristics
          GATT_DiscAllChars(Time_connHandle, Time_svcStartHdl,
                            Time_svcEndHdl, ICall_getEntityId());
          
          newState = DISC_BATT_CHAR;
        }
        else
        {
          // Service not found
          newState = DISC_FAILED;
        }
      }
      break;

    case DISC_BATT_CHAR:
      {
        // Characteristics found
        if (pMsg->method == ATT_READ_BY_TYPE_RSP &&
            pMsg->msg.readByTypeRsp.numPairs > 0 && 
            pMsg->msg.readByTypeRsp.len == CHAR_DESC_HDL_UUID16_LEN)
        {
          uint8_t   i;
          uint8_t   *p;
          uint16_t  handle;
          uint16_t  uuid;
          
          p = pMsg->msg.readByTypeRsp.pDataList;
          
          // For each characteristic declaration
          for (i = pMsg->msg.readByTypeRsp.numPairs; i > 0; i--)
          {
            // Parse characteristic declaration
            handle = BUILD_UINT16(p[3], p[4]);
            uuid = BUILD_UINT16(p[5], p[6]);
                   
            // If looking for end handle
            if (Time_endHdlIdx != 0)
            {
              // End handle is one less than handle of characteristic declaration
              Time_handleCache[Time_endHdlIdx] = BUILD_UINT16(p[0], p[1]) - 1;
              
              Time_endHdlIdx = 0;
            }

            // If UUID is of interest, store handle
            switch (uuid)
            {
              case BATT_LEVEL_UUID:
                Time_handleCache[HDL_BATT_LEVEL_START] = handle;
                Time_endHdlIdx = HDL_BATT_LEVEL_END;
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
          if (Time_endHdlIdx != 0)
          {
            Time_handleCache[Time_endHdlIdx] = Time_svcEndHdl;
            Time_endHdlIdx = 0;
          }
          
          // If didn't find mandatory characteristic
          if (Time_handleCache[HDL_BATT_LEVEL_START] == 0)
          {
            newState = DISC_FAILED;
          }
          else if (Time_handleCache[HDL_BATT_LEVEL_START] <
                   Time_handleCache[HDL_BATT_LEVEL_END])
          {
            // Discover characteristic descriptors
            GATT_DiscAllCharDescs(Time_connHandle,
                                  Time_handleCache[HDL_BATT_LEVEL_START] + 1,
                                  Time_handleCache[HDL_BATT_LEVEL_END],
                                  ICall_getEntityId());
                                        
            newState = DISC_BATT_LVL_CCCD;
          }
          else
          {
            newState = DISC_IDLE;
          }
        }
      }      
      break;

    case DISC_BATT_LVL_CCCD:
      {
        // Characteristic descriptors found
        if (pMsg->method == ATT_FIND_INFO_RSP &&
             pMsg->msg.findInfoRsp.numInfo > 0 && 
             pMsg->msg.findInfoRsp.format == ATT_HANDLE_BT_UUID_TYPE)
        {
          uint8_t i;
          
          // For each handle/uuid pair
          for (i = 0; i < pMsg->msg.findInfoRsp.numInfo; i++)
          {
            // Look for CCCD
            if (ATT_BT_PAIR_UUID(pMsg->msg.findInfoRsp.pInfo, i) ==
                GATT_CLIENT_CHAR_CFG_UUID)
            {
              // CCCD found
              Time_handleCache[HDL_BATT_LEVEL_CCCD] =
                ATT_BT_PAIR_HANDLE(pMsg->msg.findInfoRsp.pInfo, i);
              
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
 * @fn      Time_discPhoneAlert()
 *
 * @brief   Phone alert status service and characteristic discovery. 
 *
 * @param   state - Discovery state.
 * @param   pMsg  - GATT message.
 *
 * @return  New discovery state.
 */
static uint8_t Time_discPhoneAlert(uint8_t state, gattMsgEvent_t *pMsg)
{
  uint8_t newState = state;

  switch (state)
  {
    case DISC_PAS_START:  
      {
        uint8_t uuid[ATT_BT_UUID_SIZE] = 
          { LO_UINT16(PHONE_ALERT_STS_SERV_UUID),
            HI_UINT16(PHONE_ALERT_STS_SERV_UUID) };

        // Initialize service discovery variables
        Time_svcStartHdl = Time_svcEndHdl = 0;
        Time_endHdlIdx = 0;
        
        // Discover service by UUID
        GATT_DiscPrimaryServiceByUUID(Time_connHandle, uuid,
                                      ATT_BT_UUID_SIZE, ICall_getEntityId());      

        newState = DISC_PAS_SVC;
      } 
      break;

    case DISC_PAS_SVC:
      // Service found, store handles
      if (pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP &&
          pMsg->msg.findByTypeValueRsp.numInfo > 0)
      {
        Time_svcStartHdl = 
          ATT_ATTR_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
        Time_svcEndHdl = 
          ATT_GRP_END_HANDLE(pMsg->msg.findByTypeValueRsp.pHandlesInfo, 0);
      }
      
      // If procedure complete
      if ((pMsg->method == ATT_FIND_BY_TYPE_VALUE_RSP  && 
           pMsg->hdr.status == bleProcedureComplete) ||
          (pMsg->method == ATT_ERROR_RSP))
      {
        // If service found
        if (Time_svcStartHdl != 0)
        {
          // Discover all characteristics
          GATT_DiscAllChars(Time_connHandle, Time_svcStartHdl,
                            Time_svcEndHdl, ICall_getEntityId());
          
          newState = DISC_PAS_CHAR;
        }
        else
        {
          // Service not found
          newState = DISC_FAILED;
        }
      }    
      break;

    case DISC_PAS_CHAR:
      {
        // Characteristics found
        if (pMsg->method == ATT_READ_BY_TYPE_RSP &&
             pMsg->msg.readByTypeRsp.numPairs > 0 && 
             pMsg->msg.readByTypeRsp.len == CHAR_DESC_HDL_UUID16_LEN)
        {
          uint8_t   i;
          uint8_t   *p;
          uint16_t  handle;
          uint16_t  uuid;
          
          p = pMsg->msg.readByTypeRsp.pDataList;
          
          // For each characteristic declaration
          for (i = pMsg->msg.readByTypeRsp.numPairs; i > 0; i--)
          {
            // Parse characteristic declaration
            handle = BUILD_UINT16(p[3], p[4]);
            uuid = BUILD_UINT16(p[5], p[6]);
                   
            // If looking for end handle
            if (Time_endHdlIdx != 0)
            {
              // End handle is one less than handle of characteristic declaration
              Time_handleCache[Time_endHdlIdx] = BUILD_UINT16(p[0], p[1]) - 1;
              
              Time_endHdlIdx = 0;
            }

            // If UUID is of interest, store handle
            switch (uuid)
            {
              case RINGER_CTRL_PT_UUID:
                Time_handleCache[HDL_PAS_CTRL] = handle;
                break;

              case ALERT_STATUS_UUID:
                Time_handleCache[HDL_PAS_ALERT_START] = handle;
                Time_endHdlIdx = HDL_PAS_ALERT_END;
                break;

              case RINGER_SETTING_UUID:
                Time_handleCache[HDL_PAS_RINGER_START] = handle;
                Time_endHdlIdx = HDL_PAS_RINGER_END;
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
          if (Time_endHdlIdx != 0)
          {
            Time_handleCache[Time_endHdlIdx] = Time_svcEndHdl;
            Time_endHdlIdx = 0;
          }
          
          // If didn't find alert status characteristic
          if (Time_handleCache[HDL_PAS_ALERT_START] == 0)
          {
            newState = DISC_FAILED;
          }
          else if (Time_handleCache[HDL_PAS_ALERT_START] <
                   Time_handleCache[HDL_PAS_ALERT_END])
          {
            // Discover alert status characteristic descriptors
            GATT_DiscAllCharDescs(Time_connHandle,
                                  Time_handleCache[HDL_PAS_ALERT_START] + 1,
                                  Time_handleCache[HDL_PAS_ALERT_END],
                                  ICall_getEntityId());
                                        
            newState = DISC_PAS_ALERT_CCCD;
          }
          else
          {
            // Missing required characteristic descriptor
            Time_handleCache[HDL_PAS_ALERT_START] = 0;
            newState = DISC_FAILED;
          }
        }
      }      
      break;

    case DISC_PAS_ALERT_CCCD:
      {
        // Characteristic descriptors found
        if (pMsg->method == ATT_FIND_INFO_RSP &&
            pMsg->msg.findInfoRsp.numInfo > 0 && 
            pMsg->msg.findInfoRsp.format == ATT_HANDLE_BT_UUID_TYPE)
        {
          uint8_t i;
          
          // For each handle/uuid pair
          for (i = 0; i < pMsg->msg.findInfoRsp.numInfo; i++)
          {
            // Look for CCCD
            if (ATT_BT_PAIR_UUID(pMsg->msg.findInfoRsp.pInfo, i) ==
                GATT_CLIENT_CHAR_CFG_UUID)
            {
              // CCCD found
              Time_handleCache[HDL_PAS_ALERT_CCCD] =
                ATT_BT_PAIR_HANDLE(pMsg->msg.findInfoRsp.pInfo, i);
              
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
          if (Time_handleCache[HDL_PAS_ALERT_CCCD] != 0)
          {
            // Should we look for ringer status CCCD
            if (Time_handleCache[HDL_PAS_RINGER_START] <
                Time_handleCache[HDL_PAS_RINGER_END])
            {
              // Discover ringer status characteristic descriptors
              GATT_DiscAllCharDescs(Time_connHandle,
                                    Time_handleCache[HDL_PAS_RINGER_START] + 1,
                                    Time_handleCache[HDL_PAS_RINGER_END],
                                    ICall_getEntityId());
                                          
              newState = DISC_PAS_RINGER_CCCD;
            }
            else
            {
              // Missing required characteristic descriptor
              Time_handleCache[HDL_PAS_RINGER_START] = 0;
              newState = DISC_FAILED;
            }
          }
          else
          {
            // Missing required characteristic descriptor
            Time_handleCache[HDL_PAS_ALERT_START] = 0;
            newState = DISC_FAILED;
          }          
        }
      }
      break;

    case DISC_PAS_RINGER_CCCD:
      {
        // Characteristic descriptors found
        if (pMsg->method == ATT_FIND_INFO_RSP &&
            pMsg->msg.findInfoRsp.numInfo > 0 && 
            pMsg->msg.findInfoRsp.format == ATT_HANDLE_BT_UUID_TYPE)
        {
          uint8_t i;
          
          // For each handle/uuid pair
          for (i = 0; i < pMsg->msg.findInfoRsp.numInfo; i++)
          {
            // Look for CCCD
            if (ATT_BT_PAIR_UUID(pMsg->msg.findInfoRsp.pInfo, i) ==
                GATT_CLIENT_CHAR_CFG_UUID)
            {
              // CCCD found
              Time_handleCache[HDL_PAS_RINGER_CCCD] =
                             ATT_BT_PAIR_HANDLE(pMsg->msg.findInfoRsp.pInfo, i);
              
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
          if (Time_handleCache[HDL_PAS_RINGER_CCCD] != 0)
          {
            newState = DISC_IDLE;
          }
          else
          {
            // Missing required characteristic descriptor
            Time_handleCache[HDL_PAS_RINGER_START] = 0;
            newState = DISC_FAILED;
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
