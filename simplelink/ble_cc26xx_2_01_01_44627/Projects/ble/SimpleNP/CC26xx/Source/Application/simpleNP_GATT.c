/******************************************************************************* 
  Filename:       simpleNP_GATT.c
  Revised:        $Date: 2015-07-22 15:05:16 -0700 (Wed, 22 Jul 2015) $
  Revision:       $Revision: 44399 $

  Description:    This file contains the parsing of GATT related command for 
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
*******************************************************************************/

/*******************************************************************************
 * INCLUDES
 */
#include <string.h>
#include <xdc/std.h>

#include "hal_types.h"
#include "comdef.h"

#include <ICall.h>

#include "gatt.h"

#include "gattservapp.h"
#include "inc/npi_task.h"

#include "peripheral.h"
#include "gapbondmgr.h"
#include "hci_tl.h"

#include "osal_snv.h"
#include "ICallBleAPIMSG.h"

#include "util.h"

#include "SNP.h"
#include "simpleNP.h"
#include "simpleNP_GAP.h"
#include "simpleNP_GATT.h"
#include "simpleNP_UUID.h"
#include "devinfoservice.h"
#include "gapgattserver.h"
#include "gatt_uuid.h"
#include "linkdb.h"

/*******************************************************************************
 * CONSTANTS
 */

//! \brief state of the service addition process.
#define SERVICE_ADDITION_DISABLE                0
#define SERVICE_ADDITION_ENABLE                 1

//! \brief EVENT type for a ATT operation send to the NP process thread.
#define GATT_WRITE_ATTR                         1

#define SNP_FLASH_TOP_ADDRESS                   0xFFFFF
   
/*******************************************************************************
 * TYPEDEFS
 */

//! \brief structure to store a attribute definition. this definition will be 
// used to create the attribute list for a service.
typedef struct snp_attributeEntry
{
  gattAttribute_t  attributeData; //!< attribute definition per GATT.
  uint16_t         value_len;     //!< Length of used pValue_hold
  uint8_t          *pValue_hold;   //!< storage for pValue data referenced from 
                                  //    attrData->pValue pointer.
} snp_attributeEntry_t;

//! \brief structure to store length of each attribute managed by the NP
typedef struct snp_attrLength
{
  uint16_t handle;               //!< handle of the attribute
  uint16_t max_value_len;        //!< length of the attribute value
} snp_attrLength_t;

//! \brief structure to store a registered dynamic service.
typedef struct snp_serviceEntry
{
  gattAttribute_t  *list;            //!<  Attribute table
  uint8_t          numAttr;          //!< number of attribute in the attribute 
                                     // table
  snp_attrLength_t *attrLengthTable; //!<  Table containing the length of every 
                                     // attribute of the service
  uint16_t         startHandle;      //!< start handle of the service
  uint16_t         endHandle;        //!< end handle of the service
} snp_serviceEntry_t;

//! \brief structure to store the state of a ATT operation.
// per specification, only one ATT req can be active at a time (except for 
// ATT WRITE CMD and NOTIFICATION
typedef struct snp_ATTprepare
{
  struct snp_ATTprepare   *pNext;
  uint8_t                 validity;   //!< state of ATT operation
  uint8_t                 method;     //!< method of the ongoing ATT operation.
  uint16_t                maxLen;     //!< max length of data for this ATT 
                                      // operation.  
  uint16_t                connHandle; //!< connHandle of the ATT operation
  gattAttribute_t*        pAttr;      //!< attr pointer to the ATT operation
  uint16_t                offset;     //!< offset of the ATT operation
  uint8_t*                pValue;     //!< pValue of the ATT operation
}snp_ATTprepare_t;

//! \brief structure to store the element of a service being created.
typedef struct _queueRec_ 
{
  Queue_Elem _elem;          // queue element
  uint8_t    *pData;            // pointer to app data
} snp_serviceQueueRec_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */

//! \brief instance of the ATT operation. see #snp_ATTprepare_t
snp_ATToperation_t  snp_ATTstore;

//! \brief instance of the ATT PREPARE WRITE operation. see #snp_ATTstorePrepareHead
static snp_ATTprepare_t  *snp_ATTstorePrepareHead = NULL;

/*********************************************************************
 * LOCAL VARIABLES
 */

//! \brief variables used for the service creation
static uint8_t  addingNewAttrCounter;
static uint8_t  addServiceState;
static uint16_t serviceStarthandle;

//! \brief Queue to store all created attribute of a added service
static Queue_Struct addServiceMsg;
static Queue_Handle addServiceQueue;

//! \brief Create a Queue to store all created services
static Queue_Struct addedServiceMsg;
static Queue_Handle addedServiceQueue;

//! \brief Store added extended attribute for the current characteristic 
//  being added.
static uint8_t currentCharExtAtrrHeader;

/*********************************************************************
 * SPNP TEST SERVICE
 */

// GAP GATT Attributes
static const uint8_t attDeviceName[GAP_DEVICE_NAME_LEN] = "SNP";

/*********************************************************************
 * Extern FUNCTIONS
 */

extern void exceptionHandler(void);

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static  bStatus_t SNP_readAttrCB(uint16_t connHandle, gattAttribute_t *pAttr, 
                                 uint8_t *pValue, uint16_t *pLen, 
                                 uint16_t offset, uint16_t maxLen, 
                                 uint8_t method);
static  bStatus_t SNP_writeAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                 uint8_t *pValue, uint16_t len, uint16_t offset,
                                 uint8_t method);
static  void SNP_sendWriteRequest(gattAttribute_t *pAttr, uint8_t *pValue, 
                                 uint16_t len, uint16_t offset, uint8_t method);
void    SNP_resetGATT(uint16_t handle);
uint8_t SNP_testCharSanity(uint8_t *status);

/*********************************************************************
 * PROFILE CALLBACKS
 */

CONST gattServiceCBs_t SNP_CBs =
{
  SNP_readAttrCB,   // Read callback function pointer
  SNP_writeAttrCB,  // Write callback function pointer
  NULL              // Authorization callback function pointer
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/**
 *  @fn          SNP_initGATT
 *
 *  @brief       Initial parameter needed for using SPNP GATT functionality
 *
 *  @param[in]   None
 *  @param[out]  None
 *
 *  @return      None.
 */
void SNP_initGATT(void)
{
  // Do some SPNP specific init
  addingNewAttrCounter = 0;
    
  // Construct Queue needed for service creation and storage.
  addServiceQueue = Util_constructQueue(&addServiceMsg);
  addedServiceQueue = Util_constructQueue(&addedServiceMsg);

  // Register to receive incoming ATT Indications/Notifications.
  GATT_RegisterForInd(snp_selfEntity);

  // Register for GATT local events and ATT Responses pending for transmission
  GATT_RegisterForMsgs(snp_selfEntity);

  // Set the GAP Characteristics
  GGS_SetParameter(GGS_DEVICE_NAME_ATT, GAP_DEVICE_NAME_LEN,
                   (void*) attDeviceName);

  // Initialize default services.
  GGS_AddService(GATT_ALL_SERVICES);           // GAP
  GATTServApp_AddService(GATT_ALL_SERVICES);   // GATT attributes
  DevInfo_AddService();                        // Device Information Service
 
  // Reset GATT session, hard-coded to 0 for first init.
  SNP_resetGATT(0);
}

/**
 *  @fn          SNP_resetGATT
 *
 *  @brief       reset a GATT session for a specific connection.
 *
 *  @param[in]   connhandle : handle of the connection the GATT session will be
 *                             reset (RFU)
 *  @param[out]  None
 *
 *  @return      None.
 */
void SNP_resetGATT(uint16_t connHandle)
{
  (void) connHandle;
  snp_ATTstore.validity = 0;
  
  if(snp_ATTstorePrepareHead)
  {
    snp_ATTprepare_t *current = snp_ATTstorePrepareHead;
    snp_ATTprepare_t *next;
    while(current)
    {
      next = current->pNext;
      ICall_free(current);
      current = next;
    }
    snp_ATTstorePrepareHead->pNext = NULL;
    snp_ATTstorePrepareHead = NULL;
  }
}

/**
 *  @fn          SNP_processGATTMsg
 *
 *  @brief       Process GATT messages
 *
 *  @param[in]   pMsg : GATT message to process.
 *  @param[out]  None
 *
 *  @return      None.
 */
void SNP_processGATTMsg(gattMsgEvent_t *pMsg)
{
  if(pMsg->method == ATT_HANDLE_VALUE_CFM)
  {
    snpNotifIndCnf_t rsp;
    rsp.connHandle = BUILD_UINT16(LO_UINT16(pMsg->connHandle), 
                                  HI_UINT16(pMsg->connHandle));
    rsp.status = pMsg->hdr.status;
    SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE, SNP_SEND_NOTIF_IND_CNF,
                         &(rsp.status), sizeof(snpNotifIndCnf_t)-1, 
                         (uint8_t*)&(rsp.connHandle));   
  }
  else if(pMsg->method == ATT_MTU_UPDATED_EVENT)
  {    
    attMtuUpdatedEvt_t *mtuUpdatedEvt = (attMtuUpdatedEvt_t*) &(pMsg->msg);
    uint8_t param[4];

    // connection handle, 2 bytes
    param[0] = LO_UINT16(pMsg->connHandle);
    param[1] = HI_UINT16(pMsg->connHandle);
	
    // MTU size, 2 bytes
    param[2] = LO_UINT16(mtuUpdatedEvt->MTU); 
    param[3] = HI_UINT16(mtuUpdatedEvt->MTU);

    SNP_eventToHost_send(SNP_ATT_MTU_EVT, NULL, 4, param);
  }
  GATT_bm_free(&pMsg->msg, pMsg->method);
}

/**
 *  @fn          SNP_processCharValueChangeEvt
 *
 *  @brief       Process a characteristic value change event. the information of 
 *               requested change is stored in #ATT_store
 *
 *  @param[in]   eventID : event ID of the requested change.
 *  @param[out]  None
 *
 *  @return      None.
 */
void SNP_processCharValueChangeEvt(uint8_t eventID)
{
  switch(eventID)
  {
    case GATT_WRITE_ATTR:
    {  
      switch(snp_ATTstore.method)
      {
        case ATT_WRITE_REQ:
          if(snp_ATTstorePrepareHead)
          {
            // This should not happens. impossible to have two ATT operation at 
            // the same time.
            uint8_t param[3];
            param[0] = LO_UINT16(SNP_CHAR_READ_CNF);
            param[1] = HI_UINT16(SNP_CHAR_READ_CNF);
            param[2] = SNP_GATT_COLLISION;
            //Return Major Error Event
            SNP_eventToHost_send(SNP_ERROR_EVT, NULL, sizeof(param), param);
            SNP_resetGATT(snp_ATTstore.connHandle);
          }
          //FALLTHROUGH  
        case ATT_EXECUTE_WRITE_REQ:
          if(snp_ATTstorePrepareHead)
          {            
            // a reliable write is ongoing. it's time to execute all accumulated 
            // 'prepare write'
            // send all prepare write to the AP.
            SNP_sendWriteRequest(snp_ATTstorePrepareHead->pAttr, 
                                 snp_ATTstorePrepareHead->pValue, 
                                 snp_ATTstorePrepareHead->maxLen, 
                                 snp_ATTstorePrepareHead->offset, 
                                 snp_ATTstorePrepareHead->method); 
            //signal that the first elem has been processed.
            snp_ATTstorePrepareHead->validity = 0;
            BM_free(snp_ATTstorePrepareHead->pValue);            
            break;
          }
          // a cmd should not impact the flow of Request. 
          // (ble spec chapter 3.3.2)
          SNP_sendWriteRequest(snp_ATTstore.pAttr,snp_ATTstore.pValue, snp_ATTstore.maxLen,  
                               snp_ATTstore.offset, snp_ATTstore.method);
          BM_free(snp_ATTstore.pValue);            
          break;    
          
        default:
          break;
          
      } //end switch
    }
    default:
      // should not reach here!
      break;
  }
}

/**
 *  @fn          SNP_purgeResources
 *
 *  @brief       reset service creation state and free memory already allocated.
 *
 *  @param[in]   None
 *  @param[out]  None
 *
 *  @return      None.
 */
static void SNP_purgeResources(void)
{
  snp_attributeEntry_t *pAttrEntry;
  //Purge all allocated Memory and empty existing queue.
  while(pAttrEntry = (snp_attributeEntry_t*)Util_dequeueMsg(addServiceQueue))
  {
    if(pAttrEntry->pValue_hold)
    {
      //if service attribute, need to free the hear reserve for the UUID if the
      // service
      if(!memcmp(pAttrEntry->attributeData.type.uuid, primaryServiceUUID, 2) ||
         !memcmp(pAttrEntry->attributeData.type.uuid, secondaryServiceUUID, 2))
      {         
        if(((int)((gattAttrType_t*)(pAttrEntry->pValue_hold))->uuid) > 
            SNP_FLASH_TOP_ADDRESS)
        {
          uint8_t *pTemp;
          pTemp = 
            (uint8_t *)((gattAttrType_t*)(pAttrEntry->pValue_hold))->uuid;
          ICall_free(pTemp);
        }
      }
      ICall_free(pAttrEntry->pValue_hold);
    }
    
    // then the UUID itself should be free if not allocated in flash.
    // this will be the case for the characteristic declaration attribute, 
    // or generic attribute    
    if(((int)(pAttrEntry->attributeData.type.uuid)) > SNP_FLASH_TOP_ADDRESS)
    {
      static uint8_t *pTemp;
      pTemp = (uint8_t *)(pAttrEntry->attributeData.type.uuid);
      ICall_free(pTemp);
    }
          
    ICall_free(pAttrEntry);
  }
  // This need to be call if a memory allocation problem occurs.
  addServiceState = SERVICE_ADDITION_ENABLE;  
}

/**
 *  SNP_readCharCnf
 */
uint8_t SNP_readCharCnf(snpCharReadCnf_t *pReq, uint16_t dataSize)
{
  uint16_t handle;
  GAPRole_GetParameter(GAPROLE_CONNHANDLE, &handle);
   
  if(snp_ATTstore.validity)
  {
    snp_ATTstore.validity = 0;
#ifdef SWO_DEBUG   
    ITM_Port32(1) = 0xBB;
    ITM_Port32(2) = dataSize;
#endif
    if(pReq->status != SNP_SUCCESS)
    {
      //0 == SUCCESS, other means failure
      attErrorRsp_t pRsp;
      pRsp.reqOpcode = snp_ATTstore.method;
      pRsp.handle = snp_ATTstore.pAttr->handle;
      pRsp.errCode = pReq->status;
      ATT_ErrorRsp(snp_ATTstore.connHandle, &pRsp);  
      snp_ATTstore.pValue = NULL;
    }    
    else if(dataSize > snp_ATTstore.maxLen)
    {
      uint8_t param[3];
      param[0] = LO_UINT16(SNP_CHAR_READ_CNF);
      param[1] = HI_UINT16(SNP_CHAR_READ_CNF);
      param[2] = SNP_INVALID_PARAMS;
      //Return Major Error Event
      SNP_eventToHost_send(SNP_ERROR_EVT, NULL, sizeof(param), param);
    }
    else
    {
      switch(snp_ATTstore.method)
      {
        case ATT_READ_REQ:
        {
          attReadRsp_t rsp;
          rsp.pValue = (uint8_t *)GATT_bm_alloc(snp_ATTstore.connHandle, 
                                                ATT_READ_RSP,
                                                dataSize, NULL);
          if(rsp.pValue == NULL)
          {
            uint8_t param[3];
            param[0] = LO_UINT16(SNP_CHAR_READ_CNF);
            param[1] = HI_UINT16(SNP_CHAR_READ_CNF);
            param[2] = SNP_OUT_OF_RESOURCES;
            //Return Major Error Event
            SNP_eventToHost_send(SNP_ERROR_EVT, NULL, sizeof(param), param);
          }
          else
          {
            uint8_t error;
            rsp.len = dataSize;
            memcpy(rsp.pValue, pReq->pData, dataSize);
            if((error = ATT_ReadRsp(snp_ATTstore.connHandle , &rsp) != SUCCESS))
            {
              uint8_t param[3];
              param[0] = LO_UINT16(SNP_CHAR_READ_CNF);
              param[1] = HI_UINT16(SNP_CHAR_READ_CNF);
              param[2] = error;
              //Return Major Error Event
              SNP_eventToHost_send(SNP_ERROR_EVT, NULL, sizeof(param), param);
              GATT_bm_free((gattMsg_t*)&rsp, ATT_READ_RSP);
            }
          }
        }
        break;
        
        case ATT_READ_BLOB_REQ:
        {
          attReadBlobRsp_t rsp;
          rsp.pValue = (uint8_t *)GATT_bm_alloc(snp_ATTstore.connHandle, 
                                                ATT_READ_BLOB_RSP,
                                                dataSize, NULL);
          if(rsp.pValue == NULL)
          {
            uint8_t param[3];
            param[0] = LO_UINT16(SNP_CHAR_READ_CNF);
            param[1] = HI_UINT16(SNP_CHAR_READ_CNF);
            param[2] = SNP_OUT_OF_RESOURCES;
            //Return Major Error Event
            SNP_eventToHost_send(SNP_ERROR_EVT, NULL, sizeof(param), param);
          }
          else
          {
            uint8_t error;
            rsp.len = dataSize;
            memcpy(rsp.pValue, pReq->pData, dataSize);
            if((error = ATT_ReadBlobRsp(snp_ATTstore.connHandle , &rsp)) 
                  != SUCCESS)
            {
              uint8_t param[3];
              param[0] = LO_UINT16(SNP_CHAR_READ_CNF);
              param[1] = HI_UINT16(SNP_CHAR_READ_CNF);
              param[2] = error;
              //Return Major Error Event
              SNP_eventToHost_send(SNP_ERROR_EVT, NULL, sizeof(param), param);
              GATT_bm_free((gattMsg_t*)&rsp, ATT_READ_BLOB_RSP);
            }
          }
        }
        break;
        
        default:
        {
          // Not manage for now
          uint8_t param[3];
          param[0] = LO_UINT16(SNP_CHAR_READ_CNF);
          param[1] = HI_UINT16(SNP_CHAR_READ_CNF);
          param[2] = SNP_INVALID_PARAMS;
          //Return Major Error Event
          SNP_eventToHost_send(SNP_ERROR_EVT, NULL, sizeof(param), param);
        }
        break;
      } //end switch
    } //end max length test
  } // end validity test                                    
  return SNP_SUCCESS;
}
/**
 *  SNP_processCharConfigUpdatedCnf
 *
 */
uint8_t SNP_processCharConfigUpdatedCnf(snpCharCfgUpdatedRsp_t *pReq)
{  
  return SNP_writeCharCnf((snpCharWriteCnf_t*) pReq);
}
/**
 *  SNP_charWriteCnf
 *
 */
uint8_t SNP_writeCharCnf(snpCharWriteCnf_t * pRsp)
{
  if(snp_ATTstore.validity)
  {
#ifdef SWO_DEBUG   
    ITM_Port32(1) = 0xDD;
    ITM_Port32(2) = pRsp->status;
    ITM_Port32(3) = snp_ATTstore.method;
#endif    
    if(pRsp->status)
    {
      attErrorRsp_t pAttRsp;
      pAttRsp.reqOpcode = snp_ATTstore.method;
      pAttRsp.handle = snp_ATTstore.pAttr->handle;
      pAttRsp.errCode = pRsp->status; //0 == SUCCESS, other means failure
      ATT_ErrorRsp(snp_ATTstore.connHandle, &pAttRsp);  
      snp_ATTstore.pValue = NULL;
      SNP_resetGATT(snp_ATTstore.connHandle);
    }
    else if(pRsp->connHandle != snp_ATTstore.connHandle)
    {
      //Only assume one connection for now, so one ATT operation at the same 
      // time.
      // Receive Wrong attribute handler.
      // keep the global variable as is but return an error event.
      uint8_t param[3];
      param[0] = LO_UINT16(SNP_CHAR_WRITE_CNF);
      param[1] = HI_UINT16(SNP_CHAR_WRITE_CNF);
      param[2] = SNP_INVALID_PARAMS;
      //Return Major Error Event
      SNP_eventToHost_send(SNP_ERROR_EVT, NULL, sizeof(param), param);      
    }
    else
    {      
      switch(snp_ATTstore.method)
      {
        case ATT_WRITE_REQ:
        {
          snp_ATTstore.validity = 0;
          ATT_WriteRsp(snp_ATTstore.connHandle);
        }
        break;
        
        case ATT_EXECUTE_WRITE_REQ:
          if(snp_ATTstorePrepareHead)
          {
            snp_ATTprepare_t *current = snp_ATTstorePrepareHead;
            // Specific case where we are in a reliable write request.
            // Go through the list of elements and send the one that has not 
            // been forwarded to the AP
            while((current->validity == 0) && (current->pNext))
            {
              current = current->pNext;
            }
            
            if((current->validity == 0) && (current->pNext == NULL))
            {
              //We did go thought the list entirely... 
              snp_ATTstore.validity = 0;
              ATT_ExecuteWriteRsp(snp_ATTstore.connHandle);
              BM_free(snp_ATTstore.pValue);
              snp_ATTstore.pValue = NULL;
              SNP_resetGATT(snp_ATTstore.connHandle);
            }
            else 
            {
              SNP_sendWriteRequest(current->pAttr, current->pValue, 
                                   current->maxLen, current->offset, 
                                   ATT_EXECUTE_WRITE_REQ); 
              //signal that the first elem has been processed.
              current->validity = 0;
              
              //Free pValue
              BM_free(current->pValue);
              
              //Force the current ATT operation to remain valid until all 
              // elements in the queue has been send to the application 
              // processor
              snp_ATTstore.validity = 1;
            }                      
          }
          else
          {
            snp_ATTstore.validity = 0;
            ATT_ExecuteWriteRsp(snp_ATTstore.connHandle);
          }
        break;
        
        case ATT_WRITE_CMD:
        default:
        break;
        
      } //end switch
      
      // if attribute is a CCCD, store the value
      uint16_t uuid = BUILD_UINT16(snp_ATTstore.pAttr->type.uuid[0], 
                                   snp_ATTstore.pAttr->type.uuid[1]);
      if(uuid == GATT_CLIENT_CHAR_CFG_UUID)
      {
        gattCharCfg_t *temp;
        temp = (*((gattCharCfg_t **)(snp_ATTstore.pAttr->pValue)));
        temp->value = snp_ATTstore.cccdValue;
        temp->connHandle = snp_ATTstore.connHandle;
#ifdef SWO_DEBUG   
        ITM_Port32(1) = 0x77;
        ITM_Port32(2) = snp_ATTstore.connHandle;
        ITM_Port32(3) = (uint32_t)temp;
#endif
      }
      snp_ATTstore.pValue = NULL;          
      
    } //end of state test
  } //end validity test
  return SNP_SUCCESS;
}

/**
 *        SNP_addService
 *
 */
uint8_t SNP_addService(snpAddServiceReq_t *pAddServiceCmd, uint8_t uuidLen, 
                       snpAddServiceRsp_t *pRsp) 
{
  uint8_t status = SNP_OUT_OF_RESOURCES;
  snp_attributeEntry_t* pAddServiceEntry;  
  uint8_t value;

  if(pRsp)
  {
    pRsp->status = status;
  }

  GAPRole_GetParameter(GAPROLE_STATE, &value);
  if(value != GAPROLE_INIT)
  {
    //service can only be added if the device is in the INIT state.
    status = SNP_CMD_REJECTED;
    if(pRsp)
    {
      pRsp->status = status;
    }
    return status;
  }
  // a service should be added only if a service is bot being added.
  if(addServiceState == SERVICE_ADDITION_ENABLE)
  {
    status = SNP_CMD_REJECTED;
    if(pRsp)
    {
      pRsp->status = status;
    }
    return status;
  }
  
  addServiceState = SERVICE_ADDITION_ENABLE;
  
  // Allocate the memory to add an attribute to the gatt attribute list.
  // All attribute will be store in a Queue until the service is registered.
  if(pAddServiceEntry = 
      (snp_attributeEntry_t *)ICall_malloc(sizeof(snp_attributeEntry_t)))
  {
    //Populate the Service Attribute with the data receive in the command.
    memset(pAddServiceEntry, 0, sizeof(snp_attributeEntry_t));
      
    // #1 type of service
    pAddServiceEntry->attributeData.type.len = ATT_BT_UUID_SIZE;
    switch(pAddServiceCmd->type)
    {
      case SNP_PRIMARY_SERVICE: // Primary
        pAddServiceEntry->attributeData.type.uuid = primaryServiceUUID;
        break;
        
      case SNP_SECONDARY_SERVICE: // Secondary
        pAddServiceEntry->attributeData.type.uuid = secondaryServiceUUID;
        break;
        
      default:
          status = SNP_INVALID_PARAMS;
          if(pRsp)
          {
            pRsp->status = status;
          }
          return status;
    }
    
    // #2, set the permission and handle default value for the service.
    pAddServiceEntry->attributeData.permissions = GATT_PERMIT_READ;
    pAddServiceEntry->attributeData.handle = 0;
    
    // #3, Copy the UUID value in the attribute value field.
    // the attribute value field is a 'const uint8_t *', this need special 
    // handling.
    {
      uint8_t **pVal;
      
      // for a service: the attribute value, is a gattAttrType_t structure 
      // instance.
      // memory will be allocated to hold this value.
      pAddServiceEntry->value_len = sizeof(gattAttrType_t);
      if(pAddServiceEntry->pValue_hold = ICall_malloc(sizeof(gattAttrType_t)))
      {
          const uint8_t * pRec;
          uint8_t newUUIDLen;
          //Check if the UUID is a known one.
          pRec = SNP_findUUIDRec(pAddServiceCmd->UUID, uuidLen, &newUUIDLen);
          if(pRec)
          {
            //then the value is copy in this field.
            ((gattAttrType_t*)(pAddServiceEntry->pValue_hold))->len = 
                                                        newUUIDLen;
            ((gattAttrType_t*)(pAddServiceEntry->pValue_hold))->uuid = pRec;
            pVal = (uint8_t **)&pAddServiceEntry->attributeData.pValue;
            *pVal = pAddServiceEntry->pValue_hold;
          }
          else
          {
            ICall_free(pAddServiceEntry);
            return status;
          }
      }
      else
      {
        ICall_free(pAddServiceEntry);
        return status;
      }
    }
    
    // #4 Manage Global Variable, return status 
    //Track Number of Attribute for the GATT Attribute list
    serviceStarthandle = GATT_GetNextHandle();
    addingNewAttrCounter++;
      
    // Enqueue the message.
    if(Util_enqueueMsg(addServiceQueue, NULL, (uint8_t*)pAddServiceEntry))
    {   
        status = SNP_SUCCESS;
        if(pRsp)
        {
          pRsp->status = status;
        }
    }
    else
    {
      //Test if UUID value is stored in flash or allocated in RAM.        
      if(((int)((gattAttrType_t*)(pAddServiceEntry->pValue_hold))->uuid) > 
          SNP_FLASH_TOP_ADDRESS)
      {
        uint8_t *pTemp;
        pTemp = 
          (uint8_t *)((gattAttrType_t*)(pAddServiceEntry->pValue_hold))->uuid;
        ICall_free(pTemp);
      }
      if(((int)pAddServiceEntry->pValue_hold))
        ICall_free(pAddServiceEntry->pValue_hold);
      
      ICall_free(pAddServiceEntry);
      return status;
    }         
  }
  return status;
}

/**
 *  @fn          SNP_addCharDeclaration
 *
 *  @brief       add a characteristic declaration attribute to the attribute entry
 *               list.
 *
 *  @param[in]   props    : properties of the characteristic
 *  @param[out]  None
 *
 *  @return      status of the operation: 
 *               TRUE: SUCCESS
 *               FALSE: error
 */
uint8_t SNP_addCharDeclaration(uint8_t props)
{
  snp_attributeEntry_t *pAddAttributeEntry;

  if(pAddAttributeEntry = 
      (snp_attributeEntry_t *)ICall_malloc(sizeof(snp_attributeEntry_t)))
  {
    memset(pAddAttributeEntry,0, sizeof(snp_attributeEntry_t));
    pAddAttributeEntry->attributeData.type.len = ATT_BT_UUID_SIZE;
    pAddAttributeEntry->attributeData.type.uuid = characterUUID;
    pAddAttributeEntry->attributeData.permissions = GATT_PERMIT_READ;
    pAddAttributeEntry->attributeData.handle = 0;
    
    pAddAttributeEntry->value_len = 1;
    if(pAddAttributeEntry->pValue_hold = ICall_malloc(1))
    {
      uint8_t **pVal;
      uint8_t *pTemp;
      //Assign the value
      pVal = (uint8_t **)&pAddAttributeEntry->attributeData.pValue;
      *pVal = pAddAttributeEntry->pValue_hold;
      
      //Then the value is copy in this field.
      // Since the pointer type is a const, it cannot be copied directly, 
      // a intermediate pointer type needs to be used.
      pTemp = (uint8_t *)((pAddAttributeEntry->pValue_hold));
      memcpy(pTemp, &props, 1);
    }
    else
    {
      ICall_free(pAddAttributeEntry);
      return FALSE;
    }
  }
  else
  {
    return FALSE;
  }
  
  // Enqueue the entry in the queue.
  if(!Util_enqueueMsg(addServiceQueue, NULL, (uint8_t*)pAddAttributeEntry))
  {
    ICall_free(pAddAttributeEntry->pValue_hold);
    ICall_free(pAddAttributeEntry);
    return FALSE;
  }
  return TRUE;
}

/**
 *  @fn          SNP_addCharFormat
 *
 *  @brief       add a format description attribute to the attribute entry
 *               list. see BLE spec for details on parameters.
 *
 *  @param[in]   format      : format
 *  @param[in]   exp         : exponential
 *  @param[in]   unit        : unit
 *  @param[in]   namespace   : namespace
 *  @param[in]   description : description
 *  @param[out]  None
 *
 *  @return  status of the operation: 
 *  TRUE: SUCCESS
 *  FALSE: error
 */
uint8_t SNP_addCharFormat(uint8_t format, uint8_t exp, uint16_t unit, 
                          uint8_t namespace, uint16_t description)
{
  snp_attributeEntry_t *pAddAttributeEntry;

  if(pAddAttributeEntry = 
      (snp_attributeEntry_t *)ICall_malloc(sizeof(snp_attributeEntry_t)))
  {
    memset(pAddAttributeEntry,0, sizeof(snp_attributeEntry_t));
    pAddAttributeEntry->attributeData.type.len = ATT_BT_UUID_SIZE;
    pAddAttributeEntry->attributeData.type.uuid = charFormatUUID;
    pAddAttributeEntry->attributeData.permissions = GATT_PERMIT_READ;
    pAddAttributeEntry->attributeData.handle = 0;
    
    pAddAttributeEntry->value_len = 6;
    if(pAddAttributeEntry->pValue_hold = ICall_malloc(6))
    {
      uint8_t **pVal;
      uint8_t *pTemp;
      //Assign the value
      pVal = (uint8_t **)&pAddAttributeEntry->attributeData.pValue;
      *pVal = pAddAttributeEntry->pValue_hold;
      
      //Then the value is copy in this field.
      // Since the pointer type is a const, it cannot be copied directly, 
      // a intermediate pointer type needs to be used.
      pTemp = (uint8_t *)((pAddAttributeEntry->pValue_hold));
      memcpy(pTemp, &format, sizeof(uint8_t));
      memcpy(pTemp+1, &exp, sizeof(uint8_t));
      memcpy(pTemp+2, &unit, sizeof(uint16_t));
      memcpy(pTemp+4, &namespace, sizeof(uint8_t));
      memcpy(pTemp+5, &description, sizeof(uint16_t));
    }
    else
    {
      ICall_free(pAddAttributeEntry);
      return FALSE;
    }  
  }
  else
  {
    return FALSE;
  }
  
  // Enqueue the entry in the queue.
  if(!Util_enqueueMsg(addServiceQueue, NULL, (uint8_t*)pAddAttributeEntry))
  {
    ICall_free(pAddAttributeEntry->pValue_hold);
    ICall_free(pAddAttributeEntry);
    return FALSE;
  }
  return TRUE;
}

/**
 *  @fn          SNP_addCharUserDesc
 *
 * @brief        add a user description attribute to the attribute entry
 *               list. 
 *
 *  @param[in]   perm      : permission of the attribute.
 *  @param[in]   maxLen    : maximum size possible for the string.
 *  @param[in]   lenString : initial string length.
 *  @param[in]   string    : initial user description string.
 *  @param[out]  None
 *
 *  @return      status of the operation: 
 *               TRUE: SUCCESS
 *               FALSE: error
 */
uint8_t SNP_addCharUserDesc(uint8_t perms, uint16_t maxLen, uint16_t lenString,
                            uint8_t* pString)
{
  snp_attributeEntry_t *pAddAttributeEntry;
  uint8_t perm = perms & SNP_GATT_PERM_RFU_MASK;
  
  if(pAddAttributeEntry = 
      (snp_attributeEntry_t *)ICall_malloc(sizeof(snp_attributeEntry_t)))
  {
    memset(pAddAttributeEntry,0, sizeof(snp_attributeEntry_t));
    pAddAttributeEntry->attributeData.type.len = ATT_BT_UUID_SIZE;
    pAddAttributeEntry->attributeData.type.uuid = charUserDescUUID;
    pAddAttributeEntry->attributeData.permissions = perm;
    pAddAttributeEntry->attributeData.handle = 0;
    
    pAddAttributeEntry->value_len = maxLen;
    if(pAddAttributeEntry->pValue_hold = 
        ICall_malloc(pAddAttributeEntry->value_len))
    {
      uint8_t **pVal;
      uint8_t *pTemp;
      //Assign the value
      pVal = (uint8_t **)&pAddAttributeEntry->attributeData.pValue;
      *pVal = pAddAttributeEntry->pValue_hold;
      
      //Then the value is copy in this field.
      // Since the pointer type is a const, it cannot be copied directly, 
      // a intermediate pointer type needs to be used.
      pTemp = (uint8_t *)((pAddAttributeEntry->pValue_hold));
      memcpy(pTemp, pString, lenString);
    }
    else
    {
      ICall_free(pAddAttributeEntry);
      return FALSE;
    }
  }
  else
  {
    return FALSE;
  }
  
  // Enqueue the entry in the queue.
  if(!Util_enqueueMsg(addServiceQueue, NULL, (uint8_t*)pAddAttributeEntry))
  {
    ICall_free(pAddAttributeEntry->pValue_hold);
    ICall_free(pAddAttributeEntry);
    return FALSE;
  }
  return TRUE;
}

/**
 *  @fn          SNP_addCharValue
 *
 * @brief        add a characteristic value attribute to the attribute entry
 *               list. 
 *
 *  @param[in]   len         : length of the value.
 *  @param[in]   permissions : permission of the char value.
 *  @param[in]   uuidLen     : length of the char UUID.
 *  @param[in]   uuid        : UUID of the char
 *  @param[out]  None
 *
 *  @return      status of the operation: 
 *               TRUE: SUCCESS
 *               FALSE: error
 */
uint8_t SNP_addCharValue(uint16_t length, uint8_t permissions, uint8_t uuidLen,
                          uint8_t *pUUID)
{
  snp_attributeEntry_t *pAddAttributeEntry;
  const uint8_t *pRec;
  uint8_t newUUIDLen;
  uint8_t perms = permissions & SNP_GATT_PERM_RFU_MASK;

  //Clear the RFU bits for the permission
  if(pAddAttributeEntry = 
      (snp_attributeEntry_t *)ICall_malloc(sizeof(snp_attributeEntry_t)))
  {
    memset(pAddAttributeEntry,0, sizeof(snp_attributeEntry_t));
    //create placeholder for UUID and copy it.
    pRec = SNP_findUUIDRec(pUUID, uuidLen, &newUUIDLen);
    if(pRec)
    {
      pAddAttributeEntry->attributeData.type.len = newUUIDLen;
      pAddAttributeEntry->attributeData.type.uuid = pRec;
    }
    else
    {
      ICall_free(pAddAttributeEntry);
      return FALSE;
    }
        
    pAddAttributeEntry->attributeData.permissions = perms;
  }
  else
  {
    return FALSE;
  }
  
  // Enqueue the entry in the queue.
  if(!Util_enqueueMsg(addServiceQueue, NULL, (uint8_t*)pAddAttributeEntry))
  {
    if(((int)pRec) > SNP_FLASH_TOP_ADDRESS) //Test if pRec in Heap or in Flash
    {
      ICall_free((void*)pRec);
    }
    ICall_free(pAddAttributeEntry);
    return FALSE;
  }  
  return TRUE;    
}

/**
 *  @fn          SNP_addGenericAttr
 *
 *  @brief       add a generic attribute (short or long UUID)
 *
 *  @param[in]   pReq     : pointer to the structure holding the parameter.
 *  @param[in]   pUUID    : pointer to UUID
 *  @param[in]   uuidLen  : Length of UUID of the attribute to add (2 or 16 bytes).
 *  @param[out]  None
 *
 *  @return      status of the operation: 
 *               TRUE: SUCCESS
 *               FALSE: error
 */
uint8_t SNP_addGenericAttr(uint8_t perms, uint8_t *pUUID , uint8_t uuidLen)
{
  snp_attributeEntry_t *pAddAttributeEntry;
  const uint8_t *pRec;
  uint8_t newUUIDLen;
  uint8_t perm = perms & SNP_GATT_PERM_RFU_MASK;

  if(pAddAttributeEntry = 
      (snp_attributeEntry_t *)ICall_malloc(sizeof(snp_attributeEntry_t)))
  {
    memset(pAddAttributeEntry, 0, sizeof(snp_attributeEntry_t));

    //get pointer to UUID (either flash if found , or allocated from Heap).
    pRec = SNP_findUUIDRec(pUUID, uuidLen, &newUUIDLen);
    if(pRec)
    {
      pAddAttributeEntry->attributeData.type.len = newUUIDLen;
      pAddAttributeEntry->attributeData.type.uuid = pRec;
    }
    else
    {
      ICall_free(pAddAttributeEntry);
      return FALSE;
    }
    pAddAttributeEntry->attributeData.permissions = perm;
  }
  else
  {
    return FALSE;
  }
  
  // Enqueue the entry in the queue.
  if(!Util_enqueueMsg(addServiceQueue, NULL, (uint8_t*)pAddAttributeEntry))
  {
    if(((int)pRec) > SNP_FLASH_TOP_ADDRESS) //Test if pRec in Heap or in Flash
    {
      ICall_free((void*)pRec);
    }
    ICall_free(pAddAttributeEntry);
    return FALSE;
  }
  
  return TRUE;    
}

/**
 *  @fn          SNP_addCCCD
 *
 *  @brief       add a client characteristic configuration attribute to the attribute 
 *               entry list. 
 *
 *  @param[in]   perm  : permission of the attribute.
 *  @param[out]  None
 *
 *  @return      status of the operation: 
 *               TRUE: SUCCESS
 *               FALSE: error
 */
uint8_t SNP_addCCCD(uint8_t perms)
{
  snp_attributeEntry_t *pAddAttributeEntry;
  uint8_t perm = perms & SNP_GATT_PERM_RFU_MASK;

  if(pAddAttributeEntry = 
      (snp_attributeEntry_t *)ICall_malloc(sizeof(snp_attributeEntry_t)))
  {
    memset(pAddAttributeEntry, 0, sizeof(snp_attributeEntry_t));
    pAddAttributeEntry->attributeData.type.len = ATT_BT_UUID_SIZE;
    pAddAttributeEntry->attributeData.type.uuid = clientCharCfgUUID;
    pAddAttributeEntry->attributeData.permissions = GATT_PERMIT_READ | perm;
    pAddAttributeEntry->attributeData.handle = 0;
    
    //Assign the value
    // for CCCD , the Pvalue point on a table (so a pointer) of size: 
    // sizeof(gattCharCfg_t) * linkDBNumConns
    // 
    pAddAttributeEntry->value_len = sizeof(gattCharCfg_t*) + 
                                    (sizeof(gattCharCfg_t) * linkDBNumConns);
    if(pAddAttributeEntry->pValue_hold = 
        ICall_malloc(pAddAttributeEntry->value_len))
    {
      int i;
      uint8_t        **pVal;
      gattCharCfg_t  *pTemp;

      //Assign the value
      *((uint8_t**)(pAddAttributeEntry->pValue_hold)) = 
        (uint8_t*)((pAddAttributeEntry->pValue_hold + sizeof(gattCharCfg_t*)));
      pVal = (uint8_t **)&pAddAttributeEntry->attributeData.pValue;
      *pVal = (pAddAttributeEntry->pValue_hold) ;
      
      //Then the value is copy in this field.
      // Since the pointer type is a const, it cannot be copied directly, 
      // a intermediate pointer type needs to be used.
      // the default cccd value is 0.
      pTemp = (gattCharCfg_t *)((pAddAttributeEntry->pValue_hold)+ 
                                                    sizeof(gattCharCfg_t*));
      //memset(pTemp, 0, (sizeof(gattCharCfg_t) * linkDBNumConns);
      for(i = 0; i < linkDBNumConns; i++)
      {
        pTemp[i].connHandle = 0xFF;
        pTemp[i].value = 0;
      }
    }
    else
    {
      ICall_free(pAddAttributeEntry);
      return FALSE;
    }
  }
  else
  {
    return FALSE;
  }
  
  // Enqueue the entry in the queue.
  if(!Util_enqueueMsg(addServiceQueue, NULL, (uint8_t*)pAddAttributeEntry))
  {
    ICall_free(pAddAttributeEntry->pValue_hold);
    ICall_free(pAddAttributeEntry);
    return FALSE;
  }
  
  return TRUE;    
}

/**
 *  @fn          SNP_testCharSanity
 *
 *  @brief       this routine perform a sanity check for the characteristic creation. 
 *
 *  @param[in]   perm  : permission of the attribute.
 *  @param[out]  status: status to return to the network processor.
 *
 *  @return      status of the operation: 
 *               TRUE: SUCCESS
 *               FALSE: error
 */
uint8_t SNP_testCharSanity(uint8_t *pStatus)
{
  uint8_t value;
  //Perform some sanity check:
  GAPRole_GetParameter(GAPROLE_STATE, &value);
  if(value != GAPROLE_INIT)
  {
    //characteristic can only be added if the device is in the INIT state.
    *pStatus = SNP_CMD_REJECTED;
    return FALSE;
  }
  
  // a characteristic should be added only if a service is being added..
  if(addServiceState != SERVICE_ADDITION_ENABLE)
  {
    *pStatus = SNP_CMD_REJECTED;
    return FALSE;
  }
  
  return TRUE;
}

/**
 *  SNP_addCharValueDecl
 *
 */
uint8_t SNP_addCharValueDecl(snpAddCharValueDeclReq_t *pReq, uint8_t uuidLen,
                             snpAddCharValueDeclRsp_t *pRsp)
{
  uint8_t status = SNP_OUT_OF_RESOURCES;
  if(pRsp)
  {
      pRsp->attrHandle = 0;
      pRsp->status = SNP_OUT_OF_RESOURCES;
  }

  if(!SNP_testCharSanity(&status))
  {
    SNP_purgeResources();
    if(pRsp)
    {
        pRsp->status = status;
    }
    return status;
  }
  
  // The characteristic is compose of 2 mandatory attribute:
  // Characteristic declaration
  // Characteristic Value
  // #1 , add Characteristic declaration attribute
  if(!SNP_addCharDeclaration(pReq->charValProps))
  {
    SNP_purgeResources();
    return SNP_OUT_OF_RESOURCES;
  }
    
  // #2, add Characteristic Value attribute
  if(!SNP_addCharValue(pReq->charValMaxLen, 
                        pReq->charValPerms, 
                        uuidLen,
                        pReq->UUID))
  {
    SNP_purgeResources();
    if(pRsp)
    {
        pRsp->status = SNP_OUT_OF_RESOURCES;
    }
    return SNP_OUT_OF_RESOURCES;
  }
  addingNewAttrCounter+=2;

  if(pRsp)
  {
    pRsp->attrHandle = BUILD_UINT16(LO_UINT16(serviceStarthandle+
                                              addingNewAttrCounter-1),
                                    HI_UINT16(serviceStarthandle+
                                              addingNewAttrCounter-1));
    pRsp->status = SNP_SUCCESS;
  }
  
  // Initialize the extra descriptor presence to 0
  // Only one extra descriptor (CCCD, user descriptor, format, can be present
  // for a characteristic.
  currentCharExtAtrrHeader = 0;
  return SNP_SUCCESS;
}

/**
 *  SNP_addDescriptionValue
 *
 *
 */
uint8_t SNP_addDescriptionValue(uint8_t header, uint8_t *pParam,
                                uint16_t *pNbAttrib, uint8_t *pParamsRet) 
{  
  uint8_t index = 0;
  uint8_t indexRet = 0;
  uint16_t *pParamHandleRet = (uint16_t*)(&pParamsRet[1]);
  uint8_t status = SNP_OUT_OF_RESOURCES;
  pParamsRet[0] = 0;

  if(header & SNP_DESC_HEADER_UNSUPPORTED_MASK)
  {
    SNP_purgeResources();
    return SNP_INVALID_PARAMS;    
  }
  
  if(!SNP_testCharSanity(&status))
  {
    SNP_purgeResources();
    return status;
  }
  
  if(currentCharExtAtrrHeader & header)
  {
    return SNP_CMD_REJECTED;   
  }
  
  if(header & SNP_DESC_HEADER_GEN_SHORT_UUID)
  {
    // Add generic descriptor with short UUID
    snpAddAttrGenShortUUID_t req;
	
    req.perms = pParam[index++];
    req.maxLen = BUILD_UINT16(pParam[index], pParam[index+1]);
    index += 2;
    req.UUID[0] = pParam[index++];
    req.UUID[1] = pParam[index++];
    if(!SNP_addGenericAttr(req.perms, req.UUID, ATT_BT_UUID_SIZE))
    {
      SNP_purgeResources();
      return status;
    }    
    pParamHandleRet[indexRet++] = BUILD_UINT16(LO_UINT16(serviceStarthandle +
                                                         addingNewAttrCounter),  
                                               HI_UINT16(serviceStarthandle + 
                                                        addingNewAttrCounter));
    addingNewAttrCounter++;
  }
  
  if(header & SNP_DESC_HEADER_GEN_LONG_UUID)
  {
    // Add generic descriptor with long UUID
    snpAddAttrGenLongUUID_t req;
	
    req.perms = pParam[index++];
    req.maxLen = BUILD_UINT16(pParam[index], pParam[index+1]);
    index += 2;
    memcpy(&req.UUID[0], &pParam[index], ATT_UUID_SIZE);
    index += ATT_UUID_SIZE;
    if(!SNP_addGenericAttr(req.perms, req.UUID, ATT_UUID_SIZE))
    {
      SNP_purgeResources();
      return status;
    }    
    pParamHandleRet[indexRet++] = BUILD_UINT16(LO_UINT16(serviceStarthandle +
                                                         addingNewAttrCounter),  
                                               HI_UINT16(serviceStarthandle + 
                                                        addingNewAttrCounter));
    addingNewAttrCounter++;
  }
  
  if(header & SNP_DESC_HEADER_CCCD)
  {
    //Add CCCD Descriptor 
    if(!SNP_addCCCD(pParam[index++]))
    {
      SNP_purgeResources();
      return status;
    }    
    addingNewAttrCounter++;
    pParamHandleRet[indexRet++] = BUILD_UINT16(LO_UINT16(serviceStarthandle +
                                                       addingNewAttrCounter-1),  
                                               HI_UINT16(serviceStarthandle + 
                                                      addingNewAttrCounter-1));
    currentCharExtAtrrHeader |= SNP_DESC_HEADER_CCCD;
  }
  
  if(header & SNP_DESC_HEADER_FORMAT)
  {
    //Add Format Descriptor 
    if(!SNP_addCharFormat(pParam[index],     //Format
                          pParam[index+1],   // Exp
                          (pParam[index+2] + (pParam[index+3]<<8)),  //Unit
                          pParam[index+4],   //Namespace
                          (pParam[index+5] + (pParam[index+6]<<8)))) //Desc
    {
      SNP_purgeResources();
      return status;
    }
    index +=7;
    addingNewAttrCounter++;
    pParamHandleRet[indexRet++] = BUILD_UINT16(LO_UINT16(serviceStarthandle +
                                                       addingNewAttrCounter-1),  
                                               HI_UINT16(serviceStarthandle +
                                                      addingNewAttrCounter-1));
    currentCharExtAtrrHeader |= SNP_DESC_HEADER_FORMAT;
  }
    
  if(header & SNP_DESC_HEADER_USER_DESC)
  {
    uint16_t maxLen = pParam[index+1] + (pParam[index+2]<<8);
    uint16_t stringLen = pParam[index+3] + (pParam[index+4]<<8);
    if(stringLen > maxLen)
    {
      SNP_purgeResources();
      return SNP_INVALID_PARAMS;
    }
    //Check that MaxLen is always higher or equal to len
    //Add User Description Descriptor 
    if(!SNP_addCharUserDesc(pParam[index], maxLen, stringLen, &pParam[index+5]))
    {
      SNP_purgeResources();
      return status;
    }
    index += (5+ stringLen);
    addingNewAttrCounter++;
    pParamHandleRet[indexRet++] = BUILD_UINT16(LO_UINT16(serviceStarthandle +
                                                       addingNewAttrCounter-1),
                                               HI_UINT16(serviceStarthandle +
                                                      addingNewAttrCounter-1));
    currentCharExtAtrrHeader |= SNP_DESC_HEADER_USER_DESC;
  }
  *pNbAttrib = indexRet;
  pParamsRet[0]= header;
  
  
  return SNP_SUCCESS;
}

/**
 *  SNP_registerService
 *
 */
uint8_t SNP_registerService(snpRegisterServiceRsp_t *pRsp)
{
  uint8_t value;
  uint16_t attrIndex = 0;
  snp_attributeEntry_t *pAttrEntry; 
  
  if(!pRsp)
  {
    return SNP_INVALID_PARAMS;
  }
  
  pRsp->startHandle = 0;
  pRsp->endHandle = 0;
  pRsp->status = SNP_OUT_OF_RESOURCES;
  
  snp_serviceEntry_t *pServiceEntry;
  // Create the attribute table big enough to handle all attribute.
  // The attribute table will be store in a Queue.
  gattAttribute_t  *pTempAttributeTable;
  snp_attrLength_t *pTempAttributeLengthTable;
  
  //Perform some sanity check:
  GAPRole_GetParameter(GAPROLE_STATE, &value);
  if(value != GAPROLE_INIT)
  {
    //characteristic can only be added if the device is in the INIT state.
    pRsp->status = SNP_CMD_REJECTED;
    return SNP_CMD_REJECTED;
  }
  // a service should be added if has been added previously.
  if(addServiceState != SERVICE_ADDITION_ENABLE)
  {
    pRsp->status = SNP_CMD_REJECTED;
    return SNP_CMD_REJECTED;
  }
  
  pServiceEntry = ICall_malloc(sizeof(snp_serviceEntry_t)); 
  if(pServiceEntry == NULL)
  {
      SNP_purgeResources();
      return pRsp->status;
  }
      
  pTempAttributeTable = 
                   ICall_malloc(sizeof(gattAttribute_t)*addingNewAttrCounter);
  if(pTempAttributeTable == NULL)
  {
    ICall_free(pServiceEntry);
    SNP_purgeResources();
    return pRsp->status;
  }
  pTempAttributeLengthTable = 
                 ICall_malloc(sizeof(snp_attrLength_t)*addingNewAttrCounter);
  if(pTempAttributeLengthTable == NULL)
  {
    ICall_free(pTempAttributeTable);
    ICall_free(pServiceEntry);
    SNP_purgeResources();
    return pRsp->status;
  }
  
  pServiceEntry->list = pTempAttributeTable;
  pServiceEntry->attrLengthTable = pTempAttributeLengthTable;
  pServiceEntry->startHandle = serviceStarthandle;
  pServiceEntry->endHandle = serviceStarthandle + addingNewAttrCounter-1;
  pServiceEntry->numAttr = addingNewAttrCounter;
  //Go through the attribute queue to create the full-contigus attribute table.
  while(pAttrEntry = (snp_attributeEntry_t*)Util_dequeueMsg(addServiceQueue))
  {
    if(pAttrEntry)
    {
      //We have an attribute, set it in the attribute table.
      // Do the usable trick to downcast const.
      static uint8_t    *pTemp;
      static uint8_t    **pVal;
      pVal = (uint8_t **)&pAttrEntry->attributeData;
      pTemp = (uint8_t *)(&pTempAttributeTable[attrIndex]);
      pTempAttributeLengthTable[attrIndex].handle =  
                                                serviceStarthandle+ attrIndex;  
      pTempAttributeLengthTable[attrIndex].max_value_len =  pAttrEntry->value_len;
      memcpy(pTemp, pVal, sizeof(gattAttribute_t));
      attrIndex++;
      ICall_free(pAttrEntry);
    }
  }
    
  //Save the attribute table for further references.
  //use another queue for that.  
  if(Util_enqueueMsg(addedServiceQueue, NULL, (uint8_t*)pServiceEntry))
  {   
    pRsp->startHandle = BUILD_UINT16(LO_UINT16(serviceStarthandle),
                                     HI_UINT16(serviceStarthandle)); ;
    pRsp->endHandle = BUILD_UINT16(LO_UINT16(serviceStarthandle + 
                                             addingNewAttrCounter-1),
                                   HI_UINT16(serviceStarthandle + 
                                             addingNewAttrCounter-1)); ;
                                      
    addServiceState = SERVICE_ADDITION_DISABLE;
  }
  else
  {
    ICall_free(pTempAttributeTable);
    ICall_free(pTempAttributeLengthTable);
    ICall_free(pServiceEntry);
    SNP_purgeResources();
    return pRsp->status;
  }

  //the attribute table has been filled in, register the service.
  pRsp->status = GATTServApp_RegisterService(pTempAttributeTable, 
                                             addingNewAttrCounter ,
                                             GATT_MAX_ENCRYPT_KEY_SIZE,
                                             &SNP_CBs);
  if(pRsp->status)
  {
    SNP_purgeResources();
  }
  addingNewAttrCounter = 0;
  return pRsp->status;
}

/**
 *  @fn      SNP_findAttrValuebyhandle
 *
 * @brief   Find attribute by handle 
 *
 *  @param[in]   handle : handle of the attribute to find
 *  @param[out]   data : data to return
 *  @param[out]   length : length of the data to return
 *
 *  @return  status of the operation: 
 *  SPNP_SUCCESS, 
 */
static uint8_t SNP_findAttrValuebyhandle(uint16_t handle, uint8_t **data, 
                                         uint16_t *length)
{  
  snp_serviceQueueRec_t *pRec;  
  gattAttribute_t *pAttrTable= NULL;
  snp_attrLength_t *pLengthTable= NULL;
  uint16_t nbAttr = 0;
  uint16_t i;
  
  for (pRec = Queue_head(addedServiceQueue); 
      (&pRec->_elem) != (Queue_Elem *)addedServiceQueue; 
      pRec = Queue_next(&pRec->_elem)) 
  {
    snp_serviceEntry_t *pServiceEntry = (snp_serviceEntry_t *)pRec->pData;
    
    if((handle >= (pServiceEntry->startHandle)) && 
         (handle <= (pServiceEntry->endHandle)))
    {
      //Find the service!
      pAttrTable = pServiceEntry->list;
      pLengthTable = pServiceEntry->attrLengthTable;
      nbAttr = pServiceEntry->numAttr;
      break;
    }
  }
  
  if(pAttrTable)
  {
    for (i = 0; i < nbAttr; i++)
    {
      if(pAttrTable[i].handle == handle)
      {
        // Attribute record found
        *data = (pAttrTable[i].pValue);
      }
      if(pLengthTable[i].handle == handle)
      {
        // Attribute record found
        *length = pLengthTable[i].max_value_len;
      }
    }
  }
  return SNP_SUCCESS;
}

/**
 *   SNP_getAttrValue
 *
 */
uint8_t SNP_getAttrValue(snpGetAttrValueReq_t *pReq,
                         snpGetAttrValueRsp_t *pRsp, uint16_t *pDataLenght)
{
  uint8_t *pData = NULL;
  uint16_t len;

  // find services that contain this attribute.
  // walkthrough the queue, find the service, then find the attribute value 
  // pointer.
  if(SNP_findAttrValuebyhandle(pReq->attrHandle , &pData, &len) == SNP_SUCCESS)
  {
    //found, return the value
    pRsp->pData = pData;
    *pDataLenght = len;
  }
  else
  {
    //Attribute not found
    return SNP_UNKNOWN_ATTRIBUTE;
  }
  return SNP_SUCCESS;
}

/**
 *  SNP_setAttrValue
 *
 */
uint8_t SNP_setAttrValue(snpSetAttrValueReq_t *pReq, uint16_t dataLenght, 
                         snpSetAttrValueRsp_t *pRsp)
{
  uint8_t *pData = NULL;
  uint16_t len;
  
  // find services that contain this attribute.
  // walkthrough the queue, find the service, then find the attribute value 
  // pointer and length.
  if(SNP_findAttrValuebyhandle(pReq->attrHandle, &pData, &len) == SNP_SUCCESS)
  {
    //found, check the len
    if(len == 0)
    {
      //attribute can not be set/get , this is an attribute managed by the AP
      return SNP_CMD_REJECTED;
    }
    else if(len < dataLenght) 
    {
      return SNP_INVALID_PARAMS;
    }
    else
    {
      //good attribute, good length.
      memcpy(pData, pReq->pData, dataLenght);
      pRsp->attrHandle = pReq->attrHandle;
    }
  }
  else
  {
    //Attribute not found
    return SNP_UNKNOWN_ATTRIBUTE;
  }
  return SNP_SUCCESS;
}

/**
 *  @fn      SNP_readAttrCB
 *
 *  @brief   remote GATT client wants to read an attribute.
 *
 *  @param[in]   connHandle: connection message was received on
 *  @param[in]   pAttr     : pointer to attribute
 *  @param[in]   pValue    : pointer to data to be read
 *  @param[in]   pLen      : length of data to be read
 *  @param[in]   offset    : offset of the first octet to be read
 *  @param[in]   maxLen    : maximum length of data to be read
 *  @param[in]   method    : method of the ongoing ATT operation.
 *  @param[out]  None
 *
 *  @return  status.
 *  success or ble_pending
 */
static bStatus_t SNP_readAttrCB(uint16_t connHandle, gattAttribute_t *pAttr, 
                                uint8_t *pValue, uint16_t *pLen,
                                uint16_t offset, uint16_t maxLen, 
                                uint8_t method)
{
  bStatus_t status = SUCCESS;
   
  //if handle in the pool of attribute manage by the AP, forward it
  // So far all attribute are managed by the AP
  // No need for "GATT_SERVICE_UUID" or "GATT_CLIENT_CHAR_CFG_UUID" cases;
  // gattserverapp handles those reads.
  // What about other attributes? user description, format, etc...
  if(((method == ATT_READ_REQ) || (method == ATT_READ_BLOB_REQ))
      && (!snp_ATTstore.validity))
  {
    //Only Support READ REQ method
    uint16_t param[4];
    GAPRole_GetParameter(GAPROLE_CONNHANDLE, &param[0]);
    param[1] = BUILD_UINT16(LO_UINT16(pAttr->handle), HI_UINT16(pAttr->handle));
    param[2] = BUILD_UINT16(LO_UINT16(offset), HI_UINT16(offset));
    param[3] = BUILD_UINT16(LO_UINT16(maxLen), HI_UINT16(maxLen));
    snp_ATTstore.validity = 1;
    snp_ATTstore.maxLen = maxLen;
    snp_ATTstore.method = method;
    snp_ATTstore.connHandle = connHandle;
#ifdef SWO_DEBUG   
    ITM_Port32(1) = 0xAA;
    ITM_Port32(2) = method;
    ITM_Port32(3) = maxLen;
#endif    
    status = blePending;
    SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE, SNP_CHAR_READ_IND, NULL, 
                         sizeof(param), (uint8_t*)param);
  }
  else
  {
    status = ATT_ERR_INSUFFICIENT_RESOURCES;
  }


  return (status);
}

/**
 *  @fn      SNP_sendWriteRequest
 *
 *  @brief   this routine sends a write request indication to the AP.
 *
 *  @param[in]   pAttr     : pointer to attribute
 *  @param[in]   pValue    : pointer to data to be send
 *  @param[in]   len       : length of data to be send
 *  @param[in]   offset    : offset of the first octet for the attribute value
 *  @param[in]   method    : method of the ongoing ATT operation.
 *  @param[out]  None
 *
 *  @return  None
 */
                               
static void SNP_sendWriteRequest(gattAttribute_t *pAttr, uint8_t *pValue, 
                             uint16_t len, uint16_t offset, uint8_t method)
{
  uint8_t cmdType;
  uint16_t uuid = BUILD_UINT16(pAttr->type.uuid[0], pAttr->type.uuid[1]);
  
  if(pValue)
  {
    uint8_t *pParam = ICall_malloc(7);
    if(pParam)
    {
  #ifdef SWO_DEBUG   
      ITM_Port32(1) = 0xCC;
      ITM_Port32(2) = method;
      ITM_Port32(3) = len;    
  #endif
      GAPRole_GetParameter(GAPROLE_CONNHANDLE, &pParam[0]);
      pParam[2] = LO_UINT16(pAttr->handle);
      pParam[3] = HI_UINT16(pAttr->handle);
      
      if(uuid == GATT_CLIENT_CHAR_CFG_UUID)
      {
        // for a CCCCD, the ATT_WRITE_CMD is process the same way as 
        // ATT_WRITE_REQ
        snp_ATTstore.validity = TRUE;
        snp_ATTstore.pValue = pValue;
        snp_ATTstore.maxLen = len;
        snp_ATTstore.method = method;
        snp_ATTstore.offset = offset;
        snp_ATTstore.pAttr = pAttr;
        pParam[4] = SNP_RESPONSE_NEEDED;
        memcpy(&pParam[5], pValue, 2);
        // Forbid update of CCCD by WRITE CMD.
        // since then the AP has no way to perform authentication...
        snp_ATTstore.cccdValue = pValue[0];
        SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE,
                              SNP_CCCD_UPDATED_IND,
                              NULL,
                              7, pParam);
      }
      else
      {
        cmdType = (method == ATT_WRITE_CMD) ? 
                                    SNP_RESPONSE_NOT_NEEDED : SNP_RESPONSE_NEEDED;
        pParam[4] = cmdType;
        pParam[5] = LO_UINT16(offset);
        pParam[6] = HI_UINT16(offset);
        SNP_replyToHostValue_send(SNP_NPI_ASYNC_CMD_TYPE, 
                                  SNP_CHAR_WRITE_IND, 
                                  NULL, 7, pParam, len, pValue);
      }
      ICall_free(pParam);
    }
  }
}
                                 
/**
 *  @fn      SNP_writeAttrCB
 *
 *  @brief   remote GATT client wants to write to an attribute.
 *
 *  @param[in]   connHandle: connection message was received on
 *  @param[in]   pAttr     : pointer to attribute
 *  @param[in]   pValue    : pointer to data to be read
 *  @param[in]   Len       : length of data to be read
 *  @param[in]   offset    : offset of the first octet to be read
 *  @param[in]   method    : maximum length of data to be read
 *  @param[out]  None
 *
 *  @return  status.
 *  success or ble_pending
 */
static bStatus_t SNP_writeAttrCB(uint16_t connHandle, gattAttribute_t *pAttr,
                                 uint8_t *pValue, uint16_t len, 
                                 uint16_t offset, uint8_t method)
{
  bStatus_t status;
  snp_ATTprepare_t *pCurrent;
#ifdef SWO_DEBUG   
  ITM_Port32(1) = 0xEE;
  ITM_Port32(2) = method;
  ITM_Port32(3) = len;
  ITM_Port32(4) = connHandle;
#endif  
  //the stack does not filter the maximum possible size for a Characteristic.
  // it should not exceed 512 Bytes, per BLE characteristic.
  if((offset + len) > 512)
  {
    return bleInvalidRange;    
  }
  
  if(snp_ATTstore.validity == 1)
  {
    // This case could happens if a ATT_WRITE_CMD happens for a CCCD attribute 
    // and a ATT_WRITE_REQ is done before the answer to the ATT_WRITE_CMD is 
    //received from the AP.
    return ATT_ERR_UNLIKELY;
  }
  switch(method)
  {
    case ATT_WRITE_REQ:
      if(snp_ATTstorePrepareHead)
      {
        // This should not happens. impossible to have two ATT WRITE REQ 
        // and ATT_PREPARE_WRITE_REQ operations at 
        // the same time.
        uint8_t param[3];
        param[0] = LO_UINT16(SNP_CHAR_READ_CNF);
        param[1] = HI_UINT16(SNP_CHAR_READ_CNF);
        param[2] = SNP_GATT_COLLISION;
        //Return Major Error Event
        SNP_eventToHost_send( SNP_ERROR_EVT, 
                               NULL, sizeof(param), param);
        SNP_resetGATT(snp_ATTstore.connHandle);
      }
      //FALLTHROUGH  
    case ATT_EXECUTE_WRITE_REQ:
        if(snp_ATTstorePrepareHead)
        {
          pCurrent = snp_ATTstorePrepareHead;
          //find next free handle
          while(pCurrent->pNext) pCurrent = pCurrent->pNext;
          pCurrent->pNext = ICall_malloc(sizeof(snp_ATTprepare_t));
          pCurrent = pCurrent->pNext;      
          pCurrent->pNext = NULL;
          pCurrent->validity = 1;
          pCurrent->pValue = pValue;
          pCurrent->maxLen = len;
          pCurrent->offset = offset;
          pCurrent->pAttr = pAttr;
          pCurrent->connHandle = connHandle;
#ifdef SWO_DEBUG   
          ITM_Port32(1) = 0x99;
          ITM_Port32(2) = pCurrent->pAttr->handle;
#endif
        }
        snp_ATTstore.validity = 1;
        snp_ATTstore.pValue = pValue;
        snp_ATTstore.maxLen = len;
        snp_ATTstore.method = method;
        snp_ATTstore.offset = offset;
        snp_ATTstore.pAttr = pAttr;
        snp_ATTstore.connHandle = connHandle;
        SNP_enqueueMsg(SNP_CHAR_CHANGE_EVT, GATT_WRITE_ATTR);
      break;
      
    case ATT_WRITE_CMD:
      // a cmd should not impact the flow of notification. 
      // (ble spec chapter 3.3.2)
      // Therefore the snp_ATTstore.validity should not be set to 0 for this method
      // for the same reason, the packet is directly forwarded to the AP. 
      SNP_sendWriteRequest(pAttr,pValue, len,  offset,  method);
      BM_free(pValue);
      break;
      
    case ATT_PREPARE_WRITE_REQ:
      // The prepare Write will be receive in case a reliable write is done.
      // Each prepare write might be for a different handle.
      // Each prepare write is stored until the execute write is received, then 
      // they are all send at once to the application
      if(snp_ATTstorePrepareHead)
      {
        pCurrent = snp_ATTstorePrepareHead;
        //find next free handle
        while(pCurrent->pNext) pCurrent = pCurrent->pNext;
        pCurrent->pNext = ICall_malloc(sizeof(snp_ATTprepare_t));
        pCurrent = pCurrent->pNext;      
      }
      else
      {
        snp_ATTstorePrepareHead = ICall_malloc(sizeof(snp_ATTprepare_t));
        pCurrent = snp_ATTstorePrepareHead;
      }
      if(pCurrent)
      {
        pCurrent->pNext = NULL;
        pCurrent->validity = 1;
        pCurrent->pValue = pValue;
        pCurrent->maxLen = len;
        pCurrent->offset = offset;
        pCurrent->pAttr = pAttr;
        pCurrent->connHandle = connHandle;
  #ifdef SWO_DEBUG   
        ITM_Port32(1) = 0x99;
        ITM_Port32(2) = pCurrent->pAttr->handle;
  #endif
      }
      else
      {
        {
          uint8_t param[3];
          param[0] = LO_UINT16(SNP_CHAR_READ_CNF);
          param[1] = HI_UINT16(SNP_CHAR_READ_CNF);
          param[2] = SNP_OUT_OF_RESOURCES;
          //Return Major Error Event
          SNP_eventToHost_send( SNP_ERROR_EVT, 
                                 NULL, sizeof(param), param);
          SNP_resetGATT(snp_ATTstore.connHandle);
        }
        return bleMemAllocError;
      }
      break;
      
    default:
      break;
      
  } //end switch
  
  status = (method == ATT_WRITE_CMD) ? SUCCESS : blePending;  
  return (status);
}

/**
 *  SNP_setGattParam
 *
 */
uint8_t SNP_setGATTParam(snpSetGattParamReq_t *pReq, uint16_t reqDataLen, 
                         snpSetGattParamRsp_t *pRsp)
{
    switch(pReq->serviceID)
    {
      case SNP_GGS_SERV_ID:
        pRsp->status = GGS_SetParameter(pReq->paramID, reqDataLen, 
                                        &pReq->pData);
        break;
        
      case SNP_DIS_SERV_ID:
        pRsp->status = DevInfo_SetParameter(pReq->paramID, reqDataLen, 
                                            &pReq->pData);
        break;
        
      default:
        pRsp->status = SNP_UNKNOWN_SERVICE;
        break;
        
    }
    return pRsp->status;
}

/**
 *  SNP_getGattParam
 *
 */
uint8_t SNP_getGATTParam(snpGetGattParamReq_t *pReq,
                         snpGetGattParamRsp_t *pRsp, uint16_t *pDataLen)
{
    uint8_t status = SNP_SUCCESS;
    *pDataLen = 0;
    pRsp->pData = 0;
    pRsp->paramID = pReq->paramID;
    pRsp->serviceID = pReq->serviceID;
    switch(pReq->serviceID)
    {
      case SNP_GGS_SERV_ID:
        switch(pReq->paramID)
        {
          case GGS_DEVICE_NAME_ATT:
            *pDataLen = GAP_DEVICE_NAME_LEN;
            break;
            
          case GGS_APPEARANCE_ATT:
            *pDataLen = sizeof(uint16_t);
            break;
            
          case GGS_RECONNCT_ADDR_ATT:
            *pDataLen = B_ADDR_LEN;
            break;
            
          case GGS_PERI_CONN_PARAM_ATT:
            *pDataLen = sizeof(gapPeriConnectParams_t);
            break;
            
          case GGS_PERI_PRIVACY_FLAG_ATT:
          case GGS_PERI_PRIVACY_FLAG_PROPS:
          case GGS_W_PERMIT_DEVICE_NAME_ATT:
          case GGS_W_PERMIT_APPEARANCE_ATT:
          case GGS_W_PERMIT_PRIVACY_FLAG_ATT:
            *pDataLen = sizeof(uint8_t);
            break;
            
          default:
            status = SNP_INVALID_PARAMS;
            break;
            
        }
        if(*pDataLen)
        {
          pRsp->pData = ICall_malloc(*pDataLen);
          if(pRsp->pData)
          {
            status = GGS_GetParameter(pRsp->paramID, pRsp->pData);
            if(status == INVALIDPARAMETER)
            {
              status = SNP_INVALID_PARAMS;  
            }
          }
          else
          {
              return SNP_OUT_OF_RESOURCES;
          }
        }
        break;
      case SNP_DIS_SERV_ID:
        switch(pReq->paramID)
        {
          case DEVINFO_SYSTEM_ID:
            *pDataLen = DEVINFO_SYSTEM_ID_LEN;
            break;
            
          case DEVINFO_PNP_ID:
            *pDataLen = DEVINFO_PNP_ID_LEN;
            break;
            
          case DEVINFO_MODEL_NUMBER:
          case DEVINFO_SERIAL_NUMBER:
          case DEVINFO_FIRMWARE_REV:
          case DEVINFO_HARDWARE_REV:
          case DEVINFO_SOFTWARE_REV:
          case DEVINFO_MANUFACTURER_NAME:
          case DEVINFO_11073_CERT_DATA:
            *pDataLen = SNP_DEVINFO_STRING_LEN;
            break;
            
          default:
            status = SNP_INVALID_PARAMS;
            break;
            
        }
        if(*pDataLen)
        {
          pRsp->pData = ICall_malloc(*pDataLen);
          if(pRsp->pData)
          {
            status = DevInfo_GetParameter(pRsp->paramID, pRsp->pData);
            if(status == INVALIDPARAMETER)
            {
              status = SNP_INVALID_PARAMS;  
            }
          }
          else
          {
            status = SNP_OUT_OF_RESOURCES;
          }
        }
        break;
        
      default:
        status = SNP_UNKNOWN_SERVICE;
        pRsp->pData = NULL;
        break;
        
    }
    return status;
}


/**
 *   SNP_sendNotifInd
 *
 */
uint8_t SNP_sendNotifInd(snpNotifIndReq_t *pReq, int16_t length)
{
  attHandleValueNoti_t noti; 
  uint16_t len;
  uint8_t status;
  
  uint8_t res = 0;
  snp_serviceQueueRec_t *pRec;  
  gattAttribute_t* pAttrTable= NULL;
  uint16_t nbAttr = 0;
  uint8_t i,j, k;
  
  //Double check the parameter.
  if(((pReq->type & 0x3) == 0) || 
      ((pReq->type & 0x3) == (GATT_CLIENT_CFG_NOTIFY | 
                              GATT_CLIENT_CFG_INDICATE))
    )
  {
    //Only one type can be set, and at least one must be set
    return SNP_INVALID_PARAMS;
  }
  
  //Before sending a notification or indication, check that the remote 
  // GATT client subscribe for it.
  for (pRec = Queue_head(addedServiceQueue); 
      (&pRec->_elem) != (Queue_Elem *)addedServiceQueue; 
      pRec = Queue_next(&pRec->_elem))
  {
    snp_serviceEntry_t *pServiceEntry = (snp_serviceEntry_t *)pRec->pData;
    
    if((pReq->attrHandle >= (pServiceEntry->startHandle)) && 
       (pReq->attrHandle <= (pServiceEntry->endHandle)))
    {
      //Find the service!
      pAttrTable = pServiceEntry->list;
      nbAttr = pServiceEntry->numAttr;
      
      for(i = 0; i<nbAttr ; i++)
      {
        if((pAttrTable[i].handle) == pReq->attrHandle)
        {
          uint8_t properties;
          // First Check that it is a Char UUID, and that the properties has been 
          // enable.
          // Per Construction, the Char declaration is the attribute before 
          // the char value.
          uint16_t uuid = BUILD_UINT16(pAttrTable[i-1].type.uuid[0], 
                                       pAttrTable[i-1].type.uuid[1]);
          if(uuid != GATT_CHARACTER_UUID)
          {
            //Unexpected Error
            return SNP_FAILURE;
          }
          else
          {
            properties = *(pAttrTable[i-1].pValue);
            if(!((properties>>4) & pReq->type))
            {
              //This operation not allowed per Characteristic properties...
              return SNP_NOTIF_IND_NOT_ALLOWED;
            }
                 
             //Now check that the client enabled CCCD type
            for(j = i+1;j < nbAttr;j++)
            {
              uint16_t uuid = BUILD_UINT16(pAttrTable[j].type.uuid[0], 
                                            pAttrTable[j].type.uuid[1]);
              if(uuid == GATT_CLIENT_CHAR_CFG_UUID)
              {
                //we find the attribute, check if the notification or indication
                // are enable for this connection handle.
                gattCharCfg_t *pTemp;
                
                pTemp = (*((gattCharCfg_t **)(pAttrTable[j].pValue)));
#ifdef SWO_DEBUG   
                ITM_Port32(1) = 0x66;
                ITM_Port32(2) = pReq->connHandle;
                ITM_Port32(3) = (uint32_t) pTemp;
#endif
                for (k =0; k< linkDBNumConns; k++)
                {
                  if(pTemp[k].connHandle == pReq->connHandle)
                  {
                    if(pTemp[k].value & pReq->type)
                    {
                      res=1;
                      break;
                    }
                    else
                    {
                      return SNP_NOTIF_IND_NOT_ENABLE_BY_CLIENT;
                    }
                  }
                }
                if(res)
                {
                  break;
                }
                else
                {
                  return SNP_NOTIF_IND_NOT_ENABLE_BY_CLIENT;
                }
              }
              else if(uuid == GATT_CHARACTER_UUID)
              {
                //that's another characteristic, so no CCCD found.
                return SNP_NOTIF_IND_NO_CCCD_ATTRIBUTE;
              }
              if(res)
              {
                break;
              }
            }          
          }
        }              
      }
    }
  }
  
  if(!res)
  {
    return  SNP_UNKNOWN_ATTRIBUTE;
  }
  
  if(NULL ==(noti.pValue = (uint8_t *)GATT_bm_alloc(pReq->connHandle, 
                                             ATT_HANDLE_VALUE_NOTI,
                                             GATT_MAX_MTU, &len)))
  {
    return SNP_OUT_OF_RESOURCES;
  }

  noti.handle = pReq->attrHandle;
  noti.len = MIN(len, length);
  memcpy(noti.pValue, &(pReq->pData) , MIN(len, length));
  
  if(pReq->type & GATT_CLIENT_CFG_NOTIFY)
  {
    status = GATT_Notification(pReq->connHandle, &noti, 
                                pReq->authenticate); 
    if(status != SUCCESS)
    {
      GATT_bm_free((gattMsg_t *)&noti, ATT_HANDLE_VALUE_NOTI);
    }
  }
  else if(pReq->type & GATT_CLIENT_CFG_INDICATE)
  {
    status = GATT_Indication(pReq->connHandle,
                              (attHandleValueInd_t *)&noti,
                              pReq->authenticate,
                              snp_selfEntity);
    if(status != SUCCESS)
    {
      GATT_bm_free((gattMsg_t *)&noti, ATT_HANDLE_VALUE_NOTI);
    }
  }
  else
  {
    return SNP_INVALID_PARAMS;
  }
  return status;       
}
/*********************************************************************
*********************************************************************/
