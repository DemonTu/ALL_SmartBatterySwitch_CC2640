//******************************************************************************
//! \file           npi_ss_ble_sap.c
//! \brief          NPI BLE Simple Peripheral Network Processor Subsystem
//
//   Revised        $Date: 2015-07-01 15:32:23 -0700 (Wed, 01 Jul 2015) $
//   Revision:      $Revision: 44291 $
//
//  Copyright 2015 Texas Instruments Incorporated. All rights reserved.
//
// IMPORTANT: Your use of this Software is limited to those specific rights
// granted under the terms of a software license agreement between the user
// who downloaded the software, his/her employer (which must be your employer)
// and Texas Instruments Incorporated (the "License").  You may not use this
// Software unless you agree to abide by the terms of the License. The License
// limits your use, and you acknowledge, that the Software may not be modified,
// copied or distributed unless used solely and exclusively in conjunction with
// a Texas Instruments radio frequency device, which is integrated into
// your product.  Other than for the foregoing purpose, you may not use,
// reproduce, copy, prepare derivative works of, modify, distribute, perform,
// display or sell this Software and/or its documentation for any purpose.
//
//  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
//  PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,l
//  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
//  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
//  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
//  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
//  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
//  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
//  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
//  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
//  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
//
//  Should you have any questions regarding your right to use this Software,
//  contact Texas Instruments Incorporated at www.TI.com.
//******************************************************************************

// ****************************************************************************
// includes
// ****************************************************************************

#include <xdc/std.h>
#include <string.h>

#include "SNP.h"
#include "snp_rpc.h"
#include "snp_rpc_synchro.h"

#include <inc/npi_data.h>
#include <inc/npi_task.h>
#include "npi_ss_ble_sap.h"

#include "hal_defs.h"

// ****************************************************************************
// defines
// ****************************************************************************

// ****************************************************************************
// typedefs
// ****************************************************************************

//*****************************************************************************
// externs
//*****************************************************************************

// Defined in snp_rpc.c
extern SNP_eventCBRouter_t SNP_eventCB;
extern SNP_asyncCB_t SNP_asyncCB;

extern snpSyncRspData_t npiRetMsg;

//*****************************************************************************
// function prototypes
//*****************************************************************************

void NPISS_BLE_SNP_msgFromSNP(_npiFrame_t *pNPIMsg);

// -----------------------------------------------------------------------------
//! \brief      NPI BLE Subsystem initialization function
//!
//! \return     void
// -----------------------------------------------------------------------------
void NPISS_BLE_SNP_init()
{
    // Register for messages from Host with RPC_SYS_BLE ssID
    NPITask_regSSFromHostCB(RPC_SYS_BLE_SNP,NPISS_BLE_SNP_msgFromSNP);
}

// -----------------------------------------------------------------------------
//! \brief      Call back function provided to NPI Task. All incoming NPI 
//!             received by NPI Task with the subsystem ID of this subsystem
//!             will be sent to this call back through the NPI routing system
//!
//!             *** This function MUST free pNPIMsg
//!
//! \param[in]  pNPIMsg    Pointer to a "framed" NPI message
//!
//! \return     void
// -----------------------------------------------------------------------------
void NPISS_BLE_SNP_msgFromSNP(_npiFrame_t *pNPIMsg)
{
  uint16_t msgLen = pNPIMsg->dataLen;

  switch(SNP_GET_OPCODE_HDR_CMD1(pNPIMsg->cmd1))
  {
    /* Device group */
    case SNP_DEVICE_GRP:
      {
        switch( pNPIMsg->cmd1 )
        {
          // NP is powered up indication.
          case SNP_POWER_UP_IND:
            // Device has restarted.
            if (SNP_asyncCB)
            {
              SNP_asyncCB(pNPIMsg->cmd1, NULL, msgLen);
            }
            break;
          
          // Mask Events response.
          case SNP_MASK_EVENT_RSP:
            if ( npiRetMsg.pMsg )
            {
              npiRetMsg.len = msgLen;
              
              // Explicitly copy response
              npiRetMsg.pMsg->maskEventCnf.maskedEvent = 
                BUILD_UINT16(pNPIMsg->pData[0],pNPIMsg->pData[1]);
            }
            break;
            
          case SNP_GET_REVISION_RSP:
            if ( npiRetMsg.pMsg )
            {
              npiRetMsg.len = msgLen;
              
              // Explicitly copy response
              npiRetMsg.pMsg->revisionRsp.status = pNPIMsg->pData[0];
              npiRetMsg.pMsg->revisionRsp.snpVer = 
                      BUILD_UINT16(pNPIMsg->pData[1],pNPIMsg->pData[2]);
              memcpy(npiRetMsg.pMsg->revisionRsp.stackBuildVer,
                     &pNPIMsg->pData[3],
                     sizeof(npiRetMsg.pMsg->revisionRsp.stackBuildVer));
            }
            break;
            
          // HCI command response
          case SNP_HCI_CMD_RSP:
            {
              snpHciCmdRsp_t hciRsp;
              
              // Initialize Response Struct
              hciRsp.status = pNPIMsg->pData[0];
              hciRsp.opcode = BUILD_UINT16(pNPIMsg->pData[1],pNPIMsg->pData[2]);
              hciRsp.pData = (uint8_t *)&pNPIMsg->pData[3];
              
              if (SNP_asyncCB)
              {
                SNP_asyncCB(pNPIMsg->cmd1, (snp_msg_t *)&hciRsp, msgLen);
              }
            }
            break;        
          
          case SNP_EVENT_IND:
            {
              snpEvt_t pEvt;
              
              // Copy non-Pointer members of Event Struct
              pEvt.event = BUILD_UINT16(pNPIMsg->pData[0],pNPIMsg->pData[1]);
              
              // Send event back up to NP.
              switch(pEvt.event)
              {
                case SNP_CONN_EST_EVT:
                  {    
                    snpConnEstEvt_t connEstEvt;
                    
                    // Initialize Event
                    connEstEvt.connHandle = 
                      BUILD_UINT16(pNPIMsg->pData[2],pNPIMsg->pData[3]);
                    connEstEvt.connInterval = 
                      BUILD_UINT16(pNPIMsg->pData[4],pNPIMsg->pData[5]);
                    connEstEvt.slaveLatency = 
                      BUILD_UINT16(pNPIMsg->pData[6],pNPIMsg->pData[7]);
                    connEstEvt.supervisionTimeout = 
                      BUILD_UINT16(pNPIMsg->pData[8],pNPIMsg->pData[9]);
                    connEstEvt.addressType = pNPIMsg->pData[10];
                    
                    memcpy(connEstEvt.pAddr, &pNPIMsg->pData[11], 
                           sizeof(connEstEvt.pAddr));
                    
                    pEvt.pEvtParams = (snpEventParam_t *)&connEstEvt;
                  }
                  break;
                  
                case SNP_CONN_TERM_EVT:
                  {
                    snpConnTermEvt_t connTermEvt;
                    
                    // Initialize Event
                    connTermEvt.connHandle = 
                      BUILD_UINT16(pNPIMsg->pData[2],pNPIMsg->pData[3]);
                    connTermEvt.reason = pNPIMsg->pData[4];
                    
                    pEvt.pEvtParams = (snpEventParam_t *)&connTermEvt;
                  }
                  break;

                case SNP_CONN_PARAM_UPDATED_EVT:
                  {
                    snpUpdateConnParamEvt_t event;
                    
                    // Initialize Event
                    event.connHandle = 
                      BUILD_UINT16(pNPIMsg->pData[2],pNPIMsg->pData[3]);
                    event.connInterval = 
                      BUILD_UINT16(pNPIMsg->pData[4],pNPIMsg->pData[5]);
                    event.slaveLatency = 
                      BUILD_UINT16(pNPIMsg->pData[6],pNPIMsg->pData[7]);
                    event.supervisionTimeout = 
                      BUILD_UINT16(pNPIMsg->pData[8],pNPIMsg->pData[9]);
                    
                    pEvt.pEvtParams = (snpEventParam_t *)&event;
                  }
                  break;

                case SNP_ADV_STARTED_EVT:
                case SNP_ADV_ENDED_EVT:
                  {
                    snpAdvStatusEvt_t event;
                    
                    // Initialize Event
                    event.status = pNPIMsg->pData[2];
                    
                    pEvt.pEvtParams = (snpEventParam_t *)&event;
                  }
                  break; 
                  
                case SNP_ATT_MTU_EVT:
                  {
                    snpATTMTUSizeEvt_t event;
                    
                    // Initialize Event
                    event.connHandle = 
                      BUILD_UINT16(pNPIMsg->pData[2],pNPIMsg->pData[3]);
                    event.attMtuSize = 
                      BUILD_UINT16(pNPIMsg->pData[4],pNPIMsg->pData[5]);
                    
                    pEvt.pEvtParams = (snpEventParam_t *)&event;
                  }
                  break;
              }
              // Send to NP layer.
              if ( SNP_eventCB )
              {
                SNP_eventCB(&pEvt);
              }
            }
            break;
            
        case SNP_GET_STATUS_RSP:
          if ( npiRetMsg.pMsg )
          {
            npiRetMsg.len = msgLen;

            // Explicitly copy response
            npiRetMsg.pMsg->getStatusRsp.gapRoleStatus = pNPIMsg->pData[0];
            npiRetMsg.pMsg->getStatusRsp.advStatus = pNPIMsg->pData[1];
            npiRetMsg.pMsg->getStatusRsp.ATTstatus = pNPIMsg->pData[2];
            npiRetMsg.pMsg->getStatusRsp.ATTmethod = pNPIMsg->pData[3];
          }
          break;
  
        case SNP_TEST_RSP:
          {
            snpTestCmdRsp_t rsp;
            
            // Initialize Response Struct
            rsp.memAlo = BUILD_UINT16(pNPIMsg->pData[0],pNPIMsg->pData[1]);
            rsp.memMax = BUILD_UINT16(pNPIMsg->pData[2],pNPIMsg->pData[3]);
            rsp.memSize = BUILD_UINT16(pNPIMsg->pData[4],pNPIMsg->pData[5]);
            
            if (SNP_asyncCB)
            {
              SNP_asyncCB(pNPIMsg->cmd1, (snp_msg_t *)&rsp, msgLen);
            }
          }
          break;
          
          default:
            // Unknown command
            break;
        }
      }
      break;
            
    /* GAP group */
    case SNP_GAP_GRP: 
      {
        switch(pNPIMsg->cmd1)
        {
          // GAP Role initialized.
          case SNP_INIT_DEVICE_CNF:
            // RFU
            break;
                  
          // Advertisement data confirmation
          case SNP_SET_ADV_DATA_CNF:
            {
              snpSetAdvDataCnf_t cnf;
              
              cnf.status = pNPIMsg->pData[0];
              
              if (SNP_asyncCB)
              {
                SNP_asyncCB(pNPIMsg->cmd1, (snp_msg_t *)&cnf, msgLen);
              }
            }
            
            
          case SNP_UPDATE_CONN_PARAM_CNF:
            {
              snpUpdateConnParamCnf_t cnf;
              
              cnf.status = pNPIMsg->pData[0];
              cnf.connHandle = 
                BUILD_UINT16(pNPIMsg->pData[1], pNPIMsg->pData[2]);
              
              if (SNP_asyncCB)
              {
                SNP_asyncCB(pNPIMsg->cmd1, (snp_msg_t *)&cnf, msgLen);
              }
            }
            break;
        
          // Set GAP parameter response.
          case SNP_SET_GAP_PARAM_RSP:
            if ( npiRetMsg.pMsg )
            {
              npiRetMsg.len = msgLen;
              
              // Explicitly copy response
              npiRetMsg.pMsg->setGapParamRsp.status = pNPIMsg->pData[0];
            }
            break;
            
          // Get GAP parameter response.
          case SNP_GET_GAP_PARAM_RSP:
            if ( npiRetMsg.pMsg )
            {
              npiRetMsg.len = msgLen;
              
              // Explicitly copy response
              npiRetMsg.pMsg->getGapParamRsp.status = pNPIMsg->pData[0];
              npiRetMsg.pMsg->getGapParamRsp.paramId = 
                BUILD_UINT16(pNPIMsg->pData[1],pNPIMsg->pData[2]);
              npiRetMsg.pMsg->getGapParamRsp.status = 
                BUILD_UINT16(pNPIMsg->pData[3],pNPIMsg->pData[4]);
            }
            break;
            
          default:
            // Unknown command
            break;
        }
      }
      break;
      
    /* GATT group */
    case SNP_GATT_GRP:  
      {
        switch(pNPIMsg->cmd1)
        {    
          case SNP_ADD_SERVICE_RSP:
            if ( npiRetMsg.pMsg )
            {
              npiRetMsg.len = msgLen;
              
              // Explicitly copy response
              npiRetMsg.pMsg->addServiceRsp.status = pNPIMsg->pData[0];
            }
            break;
            
          case SNP_ADD_CHAR_VAL_DECL_RSP:
            if ( npiRetMsg.pMsg )
            {
              npiRetMsg.len = msgLen;
              
              // Explicitly copy response
              npiRetMsg.pMsg->addCharValueDecRsp.status = pNPIMsg->pData[0];
              npiRetMsg.pMsg->addCharValueDecRsp.attrHandle = 
                BUILD_UINT16(pNPIMsg->pData[1],pNPIMsg->pData[2]);
            }
            break;
            
          case SNP_ADD_CHAR_DESC_DECL_RSP:
            if ( npiRetMsg.pMsg )
            {
              uint8_t i = 0;
              npiRetMsg.len = msgLen;
              
              // Explicitly copy response
              npiRetMsg.pMsg->addCharDescDeclRsp.status = pNPIMsg->pData[0];
              npiRetMsg.pMsg->addCharDescDeclRsp.header = pNPIMsg->pData[1];
              
              // Remaining Msg contents are uint16 handles
              while(i < (msgLen - 2))
              {
                npiRetMsg.pMsg->addCharDescDeclRsp.handles[(i/2)] = 
                  BUILD_UINT16(pNPIMsg->pData[2 + i],pNPIMsg->pData[3 + i]);
                i += 2;
              }

            }
            break;
            
          case SNP_REGISTER_SERVICE_RSP:
            if ( npiRetMsg.pMsg )
            {
              npiRetMsg.len = msgLen;

              // Explicitly copy response
              npiRetMsg.pMsg->addCharValueDecRsp.status = pNPIMsg->pData[0];
              npiRetMsg.pMsg->addCharValueDecRsp.attrHandle = 
                BUILD_UINT16(pNPIMsg->pData[1],pNPIMsg->pData[2]);
            }
            break;

          case SNP_SET_GATT_PARAM_RSP:  
            if ( npiRetMsg.pMsg )
            {
              npiRetMsg.len = msgLen;
              
              // Explicitly copy response
              npiRetMsg.pMsg->setGattParamRsp.status = pNPIMsg->pData[0];
            }
            break;
            
          // Get GATT parameter of predefined NP service response.
          case SNP_GET_GATT_PARAM_RSP:
            if ( npiRetMsg.pMsg )
            {
              npiRetMsg.len = msgLen;
              
              // Explicitly copy response
              npiRetMsg.pMsg->getGattParamRsp.serviceID = pNPIMsg->pData[0];
              npiRetMsg.pMsg->getGattParamRsp.paramID = pNPIMsg->pData[1];
              npiRetMsg.pMsg->getGattParamRsp.pData = 
                (uint8_t *)&pNPIMsg->pData[2];
            }
            break;
            
          // Get attribute value from NP response.
          case SNP_GET_ATTR_VALUE_RSP:
            //RFU
            break;
            
          // Set attribute value on NP response.
          case SNP_SET_ATTR_VALUE_RSP:
            //RFU
            break;
            
          case SNP_CHAR_READ_IND:
            if (SNP_asyncCB)
            {
              snpCharReadInd_t readInd;
              
              readInd.connHandle = 
                BUILD_UINT16(pNPIMsg->pData[0], pNPIMsg->pData[1]);              
              readInd.attrHandle = 
                BUILD_UINT16(pNPIMsg->pData[2], pNPIMsg->pData[3]);              
              readInd.offset = 
                BUILD_UINT16(pNPIMsg->pData[4], pNPIMsg->pData[5]);              
              readInd.maxSize = 
                BUILD_UINT16(pNPIMsg->pData[6], pNPIMsg->pData[7]);

              SNP_asyncCB(pNPIMsg->cmd1, (snp_msg_t *)&readInd, msgLen);
            }
            break;
            
          // Characteristic write indication.
          case SNP_CHAR_WRITE_IND:
            {
              snpCharWriteInd_t writeInd;

              writeInd.connHandle = 
                BUILD_UINT16(pNPIMsg->pData[0], pNPIMsg->pData[1]);               
              writeInd.attrHandle = 
                BUILD_UINT16(pNPIMsg->pData[2], pNPIMsg->pData[3]);              
              writeInd.rspNeeded = pNPIMsg->pData[4];              
              writeInd.offset = 
                BUILD_UINT16(pNPIMsg->pData[5], pNPIMsg->pData[6]);              
              writeInd.pData = (uint8_t *)&pNPIMsg->pData[7];
  
              if (SNP_asyncCB)
              {
                SNP_asyncCB(pNPIMsg->cmd1, (snp_msg_t *)&writeInd, msgLen);
              }
            }
            break;
            
          case SNP_SEND_NOTIF_IND_CNF:
            if (SNP_asyncCB)
            {
              snpNotifIndCnf_t notifCnf;
              
              notifCnf.status = pNPIMsg->pData[0];
              notifCnf.connHandle = 
                BUILD_UINT16(pNPIMsg->pData[1], pNPIMsg->pData[2]); 
              
              SNP_asyncCB(pNPIMsg->cmd1, (snp_msg_t *)&notifCnf, msgLen);
            }
            break;
            
          case SNP_CCCD_UPDATED_IND:
            if (SNP_asyncCB)
            {
              snpCharCfgUpdatedInd_t cccdInd;
              
              cccdInd.connHandle = 
                BUILD_UINT16(pNPIMsg->pData[0], pNPIMsg->pData[1]);                 
              cccdInd.cccdHandle = 
                BUILD_UINT16(pNPIMsg->pData[2], pNPIMsg->pData[3]);              
              cccdInd.rspNeeded = pNPIMsg->pData[4];  
              cccdInd.value = 
                BUILD_UINT16(pNPIMsg->pData[5], pNPIMsg->pData[6]); 
              
              SNP_asyncCB(pNPIMsg->cmd1, (snp_msg_t *)&cccdInd, msgLen);
            }
            break;    
            
          default:
            // Unknown command
            break;
        }
      }
      break;
      
    default:
      // Unknown.
      break;
  }
  
  if (pNPIMsg->cmd0 == SNP_NPI_SYNC_RSP_TYPE)
  {
    // This is a synchronous response, signal the application who requested this.
    SNP_responseReceived();
  }

  // Ok to deallocate
  SNP_free(pNPIMsg);
}
