/**************************************************************************************************
  Filename:       simpleNP_DEV.c
  Revised:        $Date: 2015-07-21 21:04:33 -0700 (Tue, 21 Jul 2015) $
  Revision:       $Revision: 44390 $

  Description:    This file contains the parsing of Device related command for 
                  the Simple BLE Peripheral sample application,
                  for use with the CC2650 Bluetooth Low Energy Protocol Stack.

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
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <string.h>
#include <xdc/std.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>

#include "hal_types.h"
#include "comdef.h"

#include <ICall.h>

#include "gatt.h"
#include "gap.h"

#include "gattservapp.h"
#include "npi_task.h"

#include "peripheral.h"
#include "gapbondmgr.h"
#include "hci_tl.h"
#include "npi_data.h"

#include "osal_snv.h"
#include "ICallBleAPIMSG.h"

#include "util.h"

#include "SNP.h"
#include "simpleNP.h"
#include "simpleNP_GAP.h"
#include "simpleNP_GATT.h"
   
/*********************************************************************
 * CONSTANTS
 */
   
/*********************************************************************
 * TYPEDEFS
 */

typedef struct snp_HCIoperation
{
  uint8_t  validity;
  uint16_t opcode;      //!< method of the ongoing ATT operation.
}snp_HCIoperation_t;

/*********************************************************************
 * LOCAL VARIABLES
 */

uint16_t snp_gattEventMask = 0;

/*********************************************************************
 * EXTERN VARIABLES
 */

extern snp_HCIoperation_t  snp_HCIstore;
 
/*********************************************************************
 * SPNP TEST SERVICE
 */

/*********************************************************************
 * Extern FUNCTIONS
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void SNP_ReverseBytes(uint8_t *buf, uint8_t len);

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/**
 *  SNP_executeTestCmd
 */
void SNP_executeTestCmd(snpTestCmdRsp_t *pRsp)
{    
#ifdef HEAPMGR_METRICS
  {
    extern uint16_t heapmgrMemAlo;
    extern uint16_t heapmgrMemMax;
    pRsp->memAlo = heapmgrMemAlo;
    pRsp->memMax = heapmgrMemMax;
    pRsp->memSize = HEAPMGR_SIZE;
  }
#else
  {
    pRsp->memAlo = 0xFFFF;
    pRsp->memMax = 0xFFFF;
    pRsp->memSize = 0xFFFF;
  }
#endif
}

/**
 *  SNP_getStatus
 */
void SNP_getStatus(snpGetStatusCmdRsp_t *pMsg)
{    
  extern snp_ATToperation_t  snp_ATTstore;
  GAPRole_GetParameter(GAPROLE_STATE, &pMsg->gapRoleStatus);
  GAPRole_GetParameter(GAPROLE_ADVERT_ENABLED, &pMsg->advStatus);
  if(pMsg->advStatus == FALSE) 
  {
    // Connectable Advertisement not enabled, Check of non-connectable adv 
    // enabled
    GAPRole_GetParameter(GAPROLE_ADV_NONCONN_ENABLED, &pMsg->advStatus);
  }
  
  pMsg->ATTstatus = snp_ATTstore.validity;
  if(snp_ATTstore.validity)
  {
    pMsg->ATTmethod = snp_ATTstore.method;
  }
  else
  {
    pMsg->ATTmethod = 0;
  }
}

/**
 *  SNP_getRev
 *
 */
void SNP_getRev(snpGetRevisionRsp_t *pRsp)
{
  ICall_BuildRevision buildRev = {0};
  pRsp->snpVer = BUILD_UINT16(HI_UINT16(SNP_VERSION), LO_UINT16(SNP_VERSION));
  pRsp->status = Util_buildRevision(&buildRev);
  
  // Stack revision
  // Byte 0: Major
  // Byte 1: Minor
  // Byte 2: Patch
  pRsp->stackBuildVer[0] = BREAK_UINT32(buildRev.stackVersion, 0);
  pRsp->stackBuildVer[1] = BREAK_UINT32(buildRev.stackVersion, 1);
  pRsp->stackBuildVer[2] = BREAK_UINT32(buildRev.stackVersion, 2);

  // Build revision
  pRsp->stackBuildVer[3] = LO_UINT16(buildRev.buildVersion);
  pRsp->stackBuildVer[4] = HI_UINT16(buildRev.buildVersion);

  // Stack info (Byte 5)
  pRsp->stackBuildVer[5] = buildRev.stackInfo;

  // Controller info - part 1 (Byte 6)
  pRsp->stackBuildVer[6] = LO_UINT16(buildRev.ctrlInfo);

  // Controller info - part 2 (Byte 7)
  pRsp->stackBuildVer[7] = 0; // reserved

  // Host info - part 1 (Byte 8)
  pRsp->stackBuildVer[8] = LO_UINT16(buildRev.hostInfo);

  // Host info - part 2 (Byte 9)
  pRsp->stackBuildVer[9] = 0; // reserved

}

/**
 *   SNP_maskEvt
 */
void SNP_maskEvt(snpMaskEventReq_t *pReq, snpMaskEventRsp_t *pRsp)
{
  snp_gattEventMask = pReq->eventMask;
}


/**
 *  SNP_executeHCIcmd
 */
uint8_t SNP_executeHCIcmd(snpHciCmdReq_t *pReq, uint16_t dataLen)
{  
  bStatus_t stat;   
  uint16_t opcode =  pReq->opcode;
  uint8_t *pData = (uint8_t*)&pReq->pData;
  uint8_t status = blePending; 
  
  if(snp_HCIstore.validity)
  {
    return SNP_CMD_ALREADY_IN_PROGRESS;
  }     
      
  switch (opcode)
  {     
    case SNP_HCI_OPCODE_EXT_RESET_SYSTEM:
    {
      stat =  HCI_EXT_ResetSystemCmd(LL_EXT_RESET_SYSTEM_HARD);
      if(stat == SUCCESS)
      {
        status = SNP_SUCCESS;
      }
      else
      {
        status = bleIncorrectMode;
      }
    }
    break;
    
    case SNP_HCI_OPCODE_READ_BDADDR:
    {
      stat = HCI_ReadBDADDRCmd();
      if(stat == SUCCESS)
      {
        // Set state to wait for the HCI event with the address
        snp_HCIstore.validity = TRUE;
        snp_HCIstore.opcode = opcode;
        status = blePending;
      }
      else
      {
        status = stat;
      }
    }
    break;
    
    case SNP_HCI_OPCODE_EXT_SET_BDADDR:
      {
        if(dataLen != B_ADDR_LEN) 
        {
          status = SNP_INVALID_PARAMS;
        }
        else
        {
          stat = HCI_EXT_SetBDADDRCmd(pData);
          if(stat == SUCCESS)
          {
            // Set state to wait for the HCI event with the address
            snp_HCIstore.validity = TRUE;
            snp_HCIstore.opcode = opcode;
            status = blePending;
          }
          else
          {
            status = stat;
          }
        }
      }
      break;
      
    case SNP_HCI_OPCODE_EXT_SET_TX_POWER:
      {
        if(dataLen != sizeof(uint8_t)) 
        {
          status = SNP_INVALID_PARAMS;
        }
        else
        {
          stat = HCI_EXT_SetTxPowerCmd(pData[0]);
          if(stat == SUCCESS)
          {
            // Set state to wait for the HCI event with the address
            snp_HCIstore.validity = TRUE;
            snp_HCIstore.opcode = opcode;
            status = blePending;
          }
          else
          {
            status = stat;
          }
        }
      }
      break;
      
    case SNP_HCI_OPCODE_EXT_SET_SCA:
      if(dataLen != sizeof(uint16_t)) 
      {
        status = SNP_INVALID_PARAMS;
      }
      else
      {
        stat = HCI_EXT_SetSCACmd((pData[0] &0xFF) +  (pData[1] << 8));
        if(stat == SUCCESS)
        {
          // Set state to wait for the HCI event with the address
          snp_HCIstore.validity = TRUE;
          snp_HCIstore.opcode = opcode;
          status = blePending;
        }
        else
        {
          status = stat;
        }
      }
      break;
      
    case SNP_HCI_OPCODE_EXT_MODEM_TEST_TX:
      if(dataLen != 2*sizeof(uint8_t)) 
      {
        status = SNP_INVALID_PARAMS;
      }
      else
      {
        stat = HCI_EXT_ModemTestTxCmd(pData[0], pData[1]);
        if(stat == SUCCESS)
        {
          // Set state to wait for the HCI event with the address
          snp_HCIstore.validity = TRUE;
          snp_HCIstore.opcode = opcode;
          status = blePending;
        }
        else
        {
          status = stat;
        }
      }
      break;
      
    case SNP_HCI_OPCODE_EXT_MODEM_HOP_TEST_TX:
      stat = HCI_EXT_ModemHopTestTxCmd();
      if(stat == SUCCESS)
      {
        // Set state to wait for the HCI event with the address
        snp_HCIstore.validity = TRUE;
        snp_HCIstore.opcode = opcode;
        status = blePending;
      }
      else
      {
        status = stat;
      }
      break;
      
    case SNP_HCI_OPCODE_EXT_MODEM_TEST_RX:
      if(dataLen != sizeof(uint8_t)) 
      {
        status = SNP_INVALID_PARAMS;
      }
      else
      {
        stat = HCI_EXT_ModemTestRxCmd(pData[0]);
        if(stat == SUCCESS)
        {
          // Set state to wait for the HCI event with the address
          snp_HCIstore.validity = TRUE;
          snp_HCIstore.opcode = opcode;
          status = blePending;
        }
        else
        {
          status = stat;
        }
      }
      break;
      
    case SNP_HCI_OPCODE_EXT_END_MODEM_TEST:
      stat = HCI_EXT_EndModemTestCmd();
      if(stat == SUCCESS)
      {
        // Set state to wait for the HCI event with the address
        snp_HCIstore.validity = TRUE;
        snp_HCIstore.opcode = opcode;
        status = blePending;
      }
      else
      {
        status = stat;
      }
      break;
      
    case SNP_HCI_OPCODE_EXT_ENABLE_PTM:
      stat = HCI_EXT_EnablePTMCmd();
      if(stat == SUCCESS)
      {
        status = SNP_SUCCESS;
      }
      else
      {
        status = stat;
      }
      break;
      
    case SNP_HCI_OPCODE_EXT_SET_MAX_DTM_TX_POWER:
      if(dataLen != sizeof(uint8_t)) 
      {
        status = SNP_INVALID_PARAMS;
      }
      else
      {
        stat = HCI_EXT_SetMaxDtmTxPowerCmd(pData[0]);
        if(stat == SUCCESS)
        {
          // Set state to wait for the HCI event with the address
          snp_HCIstore.validity = TRUE;
          snp_HCIstore.opcode = opcode;
          status = blePending;
        }
        else
        {
          status = stat;
        }
      }
      break;
      
    case SNP_HCI_OPCODE_READ_RSSI:
      if(dataLen != sizeof(uint16_t)) 
      {
        status = SNP_INVALID_PARAMS;
      }
      else
      {
        stat = HCI_ReadRssiCmd((pData[0] &0xFF) +  (pData[1] << 8));
        if(stat == SUCCESS)
        {
          // Set state to wait for the HCI event with the address
          snp_HCIstore.validity = TRUE;
          snp_HCIstore.opcode = opcode;
          status = blePending;
        }
        else
        {
          status = stat;
        }
      }
      break;
      
    case SNP_HCI_OPCODE_LE_RECEIVER_TEST:
      if(dataLen != sizeof(uint8_t)) 
      {
        status = SNP_INVALID_PARAMS;
      }
      else
      {
        stat = HCI_LE_ReceiverTestCmd(pData[0]);
        if(stat == SUCCESS)
        {
          // Set state to wait for the HCI event with the address
          snp_HCIstore.validity = TRUE;
          snp_HCIstore.opcode = opcode;
          status = blePending;
        }
        else
        {
          status = stat;
        }
      }
      break;
      
    case SNP_HCI_OPCODE_LE_TRANSMITTER_TEST:
      if(dataLen != 3 * sizeof(uint8_t)) 
      {
        status = SNP_INVALID_PARAMS;
      }
      else
      {
        stat = HCI_LE_TransmitterTestCmd(pData[0], pData[1], pData[2]);
        if(stat == SUCCESS)
        {
          // Set state to wait for the HCI event with the address
          snp_HCIstore.validity = TRUE;
          snp_HCIstore.opcode = opcode;
          status = blePending;
        }
        else
        {
          status = stat;
        }
      }
      break;
      
    case SNP_HCI_OPCODE_LE_TEST_END:
      stat = HCI_LE_TestEndCmd();
      if(stat == SUCCESS)
      {
        // Set state to wait for the HCI event with the address
        snp_HCIstore.validity = TRUE;
        snp_HCIstore.opcode = opcode;
        status = blePending;
      }
      else
      {
        status = stat;
      }
      break;
      
    case SNP_HCI_OPCODE_EXT_PER:
      if(dataLen != (sizeof(uint8_t) + sizeof(uint16_t))) 
      {
        status = SNP_INVALID_PARAMS;
      }
      else
      {
        stat = HCI_EXT_PacketErrorRateCmd(pData[0] +  (pData[1] << 8),
                                          pData[2]);
        if(stat == SUCCESS)
        {
          // Set state to wait for the HCI event with the address
          snp_HCIstore.validity = TRUE;
          snp_HCIstore.opcode = opcode;
          status = blePending;
        }
        else
        {
          status = stat;
        }
      }
      break;
      
    case SNP_HCI_OPCODE_EXT_DECRYPT:
      if(dataLen != (KEYLEN + KEYLEN)) 
      {
        status = SNP_INVALID_PARAMS;
      }
      else
      {
        // reverse byte order of key (MSB..LSB required)
        SNP_ReverseBytes(&pData[0], KEYLEN);

        // reverse byte order of encText (MSB..LSB required)
        SNP_ReverseBytes(&pData[KEYLEN], KEYLEN);
        
        stat = HCI_EXT_DecryptCmd(&pData[0], &pData[KEYLEN]);
        if(stat == SUCCESS)
        {
          // Set state to wait for the HCI event with the address
          snp_HCIstore.validity = TRUE;
          snp_HCIstore.opcode = opcode;
          status = blePending;
        }
        else
        {
          status = stat;
        }
      }
      break;
      
    case SNP_HCI_OPCODE_LE_ENCRYPT:
      if(dataLen != (KEYLEN + KEYLEN)) 
      {
        status = SNP_INVALID_PARAMS;
      }
      else
      {
        // reverse byte order of key (MSB..LSB required)
        SNP_ReverseBytes(&pData[0], KEYLEN);

        // reverse byte order of encText (MSB..LSB required)
        SNP_ReverseBytes(&pData[KEYLEN], KEYLEN);
        
        stat = HCI_LE_EncryptCmd(&pData[0], &pData[KEYLEN]);
        if(stat == SUCCESS)
        {
          // Set state to wait for the HCI event with the address
          snp_HCIstore.validity = TRUE;
          snp_HCIstore.opcode = opcode;
          status = blePending;
        }
        else
        {
          status = stat;
        }
      }
      break;
      
    case SNP_HCI_OPCODE_EXT_OVERRIDE_SL:
      if(dataLen != sizeof(uint8_t)) 
      {
        status = SNP_INVALID_PARAMS;
      }
      else
      {
      
        stat = HCI_EXT_SetSlaveLatencyOverrideCmd(pData[0]);
        if(stat == SUCCESS)
        {
          // Set state to wait for the HCI event with the address
          snp_HCIstore.validity = TRUE;
          snp_HCIstore.opcode = opcode;
          status = blePending;
        }
        else
        {
          status = stat;
        }
      }
      break;
      
    case SNP_HCI_OPCODE_EXT_SET_FAST_TX_RESP_TIME:
      if(dataLen != sizeof(uint8_t)) 
      {
        status = SNP_INVALID_PARAMS;
      }
      else
      {        
        stat = HCI_EXT_SetFastTxResponseTimeCmd(pData[0]);
        if(stat == SUCCESS)
        {
          // Set state to wait for the HCI event with the address
          snp_HCIstore.validity = TRUE;
          snp_HCIstore.opcode = opcode;
          status = blePending;
        }
        else
        {
          status = stat;
        }
      }
      break;
      
    case SNP_HCI_OPCODE_EXT_ONE_PKT_PER_EVT:
      if(dataLen != sizeof(uint8_t)) 
      {
        status = SNP_INVALID_PARAMS;
      }
      else
      {        
        stat = HCI_EXT_OnePktPerEvtCmd(pData[0]);
        if(stat == SUCCESS)
        {
          // Set state to wait for the HCI event with the address
          snp_HCIstore.validity = TRUE;
          snp_HCIstore.opcode = opcode;
          status = blePending;
        }
        else
        {
          status = stat;
        }
      }
      break;
      
    case SNP_HCI_OPCODE_EXT_GET_CONNECTION_INFO:
      stat = HCI_EXT_GetConnInfoCmd(NULL, NULL, NULL);
      if(stat == SUCCESS)
      {
        // Set state to wait for the HCI event with the address
        snp_HCIstore.validity = TRUE;
        snp_HCIstore.opcode = opcode;
        status = blePending;
      }
      else
      {
        status = stat;
      }
      break;
  default:
      status = SNP_HCI_CMD_UNKNOWN;
      break;
  }
   return status;
}
            
/*******************************************************************************
 * This function is used to reverse the order of the bytes in an array in place.
 */
static void SNP_ReverseBytes(uint8_t *buf, uint8_t len)
{
  uint8_t temp;
  uint8_t index = (uint8_t)(len - 1);
  uint8_t i;

  // adjust length as only half the operations are needed
  len >>= 1;

  // reverse the order of the bytes
  for (i=0; i<len; i++)
  {
    temp           = buf[i];
    buf[i]         = buf[index - i];
    buf[index - i] = temp;
  }

  return;
}

