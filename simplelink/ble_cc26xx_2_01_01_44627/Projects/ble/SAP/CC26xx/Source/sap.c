/**************************************************************************************************
  @headerfile:    sap.c
  Revised:        $Date: 2015-07-21 17:00:05 -0700 (Tue, 21 Jul 2015) $
  Revision:       $Revision: 44387 $

  Description:    This file contains Simple AP implementation

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
//#include <xdc/std.h>
#include <stdint.h>
#include <string.h>
#include "hal_defs.h"

#include "npi_ss_ble_sap.h"

#include "snp_rpc_synchro.h"
#include "snp_rpc.h"
#include "sap.h"

#include "Board.h"
#include "inc/npi_task.h"

/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */

            
/*********************************************************************
 * TYPEDEFS
 */

// GAP event callback node.
typedef struct
{
  struct eventCBNode_t *next;      // pointer to next callback node in the list.
  uint16_t             eventmask;  // eventMask of events this callback listens for.
  pfnEventCB_t         eventCB;    // Callback function
} eventCBNode_t;

// Asynchronous event callback node.
typedef struct
{
  struct asyncCBNode_t *next;  // pointer to next callback node in the list.
  uint8_t subystem;            // Specify the NP subsystem: GAP(HCI), Conn or ADV or GATT.
  pfnAsyncCB_t asyncCB;
} asyncCBNode_t;

// Service node
typedef struct
{
  struct serviceNode_t *next; // pointer to next service node in the list
  void *context; // stored context of service
  pfnGATTReadAttrCB_t charReadCB; // pointer to registered service read call back
  pfnGATTWriteAttrCB_t charWriteCB; // pointer to registered service write call back
  pfnCCCDIndCB_t cccdIndCB; // pointer to registered service cccb request call back
  uint16_t minHandle; // minimum handle in service
  uint16_t maxHandle; // maximum handle in service
} serviceNode_t;

// Root of the event callback list.
eventCBNode_t *eventCBListRoot = NULL;

// Root of the async callback list.
asyncCBNode_t *asyncCBListRoot = NULL;

// Root of the service list
serviceNode_t *serviceListRoot = NULL;

/* Default Remote Port UART */
const SAP_RemotePort_t default_remUARTPort = {
    .stackSize          = 1024,
    .bufSize            = 530,
    .mrdyPinID          = IOID_UNUSED,
    .srdyPinID          = IOID_UNUSED,
    .boardID            = CC2650_UART0,
    .bitRate            = 115200
};

/* Default Remote Port SPI */
const SAP_RemotePort_t default_remSPIPort = {
    .stackSize          = 1024,
    .bufSize            = 530,
    .mrdyPinID          = IOID_UNUSED,
    .srdyPinID          = IOID_UNUSED,
    .boardID            = CC2650_SPI1,
    .bitRate            = 800000
};

/*********************************************************************
 * LOCAL VARIABLES
 */
            
uint8_t SAP_isConnected = 0;

/*********************************************************************
 * HELPER FUNCTIONS
 */

static uint8_t SAP_addToEventCBList(pfnEventCB_t eventCB, uint16_t eventMask)
{
  eventCBNode_t *newNode;
  
  //Create node
  newNode = (eventCBNode_t *)SNP_malloc(sizeof(eventCBNode_t));
  
  if (newNode == NULL)
  {
    return SNP_FAILURE;
  }
  
  // Populate the node.
  newNode->eventCB = eventCB;
  newNode->eventmask = eventMask;
  newNode->next = NULL;
  
  // Check if this is the first event
  if(eventCBListRoot == NULL)
  {
    eventCBListRoot = newNode;
  }
  else
  {
    eventCBNode_t *iter = eventCBListRoot;
   
    // Iterate through the list.
    while(iter->next != NULL)
    {
      iter = (eventCBNode_t *)iter->next;
    }
    
    // add to the end of the list.
    iter->next = (struct eventCBNode_t *)newNode;
  }
  
  return SNP_SUCCESS;
}

/*
 * Add an application asynchronous callback to the ASYNC callback list.
 */
static uint8_t SAP_addToAsyncCBList(pfnAsyncCB_t eventCB)
{
  asyncCBNode_t *newNode;
  
  // Create node.
  newNode = (asyncCBNode_t *)SNP_malloc(sizeof(asyncCBNode_t));
  
  if (newNode == NULL)
  {
    return SNP_FAILURE;
  }
  
  // Populate the node.
  newNode->asyncCB = eventCB;
  newNode->next = NULL;
  
  // Check if this is the first event.
  if(asyncCBListRoot == NULL)
  {
    // Make this node the root.
    asyncCBListRoot = newNode;
  }
  else
  {
    asyncCBNode_t *iter = asyncCBListRoot;
    
    // Iterate through the list.
    while(iter->next != NULL)
    {
      iter = (asyncCBNode_t *)iter->next;
    }
    
    // Add to the end of the list.
    iter->next = (struct asyncCBNode_t *)newNode;
  }

  return SNP_SUCCESS;
}

/*
 * Iterate through event callbacks and send an event up to those which are 
 * listening for it.
 */
static void getEventCallbacks(snpEvt_t *pEvt)
{
  eventCBNode_t *iter = eventCBListRoot;
  
  // Send to all callbacks which requested this event.
  while(iter != NULL)
  {
    // If this callback listens for this event.
    if (iter->eventmask & pEvt->event != 0x0000)
    {  
      // Send to callback.
      if (iter->eventCB)
      {
        iter->eventCB(pEvt->event, (snpEventParam_t *) pEvt->pEvtParams);
      }
    }
    
    // Next callback.
    iter = (eventCBNode_t *)iter->next;
  }
}

/*
 * Iterate through list of asynchronous callbacks and 
 */
void handleAsyncCB(uint8_t cmd1, snp_msg_t *pMsg, uint16_t msgLen)
{
  asyncCBNode_t *iter = asyncCBListRoot;
  serviceNode_t *curr = serviceListRoot;
  
  switch(SNP_GET_OPCODE_HDR_CMD1(cmd1))
  {
    case SNP_GATT_GRP:
      // GATT Request, determine which service call back to invoke based upon
      // the handle of the request.
        switch (cmd1)
        {
          case SNP_CHAR_READ_IND:
            {
              snpCharReadCnf_t lCnf;
              uint16_t size = 0;
              snpCharReadInd_t *rI = (snpCharReadInd_t *)pMsg;
              
              // Initialize Confirmation Struct 
              lCnf.connHandle = rI->connHandle;
              lCnf.attrHandle = rI->attrHandle; 
              lCnf.offset = rI->offset;
              lCnf.pData = (uint8_t *)SNP_malloc(rI->maxSize);
              lCnf.status = SNP_FAILURE;
              
              // Searching for service that contains handle
              while (curr != NULL)
              {
                if (rI->attrHandle >= curr->minHandle &&
                     rI->attrHandle <= curr->maxHandle)
                {
                  // Found correct service. If call back is registered invoke, 
                  // else return error status
                  if (curr->charReadCB)
                  {
                    lCnf.status = curr->charReadCB(curr->context, rI->connHandle,
                                                        rI->attrHandle, rI->offset,
                                                        rI->maxSize, &size,
                                                        lCnf.pData);
                  }                           
                  break;
                }
                curr = (serviceNode_t *)curr->next;
              }
              SNP_readCharCnf(&lCnf,size);
              SNP_free(lCnf.pData);
            }
            break;
          
          case SNP_CHAR_WRITE_IND:
            {
              snpCharWriteCnf_t lCnf;              
              snpCharWriteInd_t *wI = (snpCharWriteInd_t *)pMsg;
              // msgLen is the amount of payload bytes. Adjust it to only be 
              // the amount of bytes to be written to the characteristic
              msgLen = msgLen - sizeof(pMsg->charWriteInd) + 
                       sizeof(pMsg->charWriteInd.pData);
              
              // Initialize Confirmation Struct
              lCnf.status = SNP_FAILURE;
              lCnf.connHandle = wI->connHandle;
              
              // Searching for service that contains handle
              while (curr != NULL)
              {
                if (wI->attrHandle >= curr->minHandle &&
                     wI->attrHandle <= curr->maxHandle)
                {
                  // Found correct service. If call back is registered invoke, 
                  // else return error status
                  if (curr->charWriteCB)
                  {
                    
                    lCnf.status = curr->charWriteCB(curr->context, wI->connHandle,
                                                         wI->attrHandle, msgLen,
                                                         wI->pData);
                  }
                  break;
                }
                curr = (serviceNode_t *)curr->next;
              }
              // Respond to write request
              SNP_writeCharCnf(&lCnf);
            }
            break;
          
          case SNP_CCCD_UPDATED_IND:
            {
              snpCharCfgUpdatedRsp_t lRsp;
              snpCharCfgUpdatedInd_t *cu  = (snpCharCfgUpdatedInd_t *)pMsg;
              
              // Initialize Response Struct
              lRsp.status = SNP_FAILURE;
              lRsp.connHandle = cu->connHandle;
                            
              while (curr != NULL)
              {
                if (cu->cccdHandle >= curr->minHandle &&
                     cu->cccdHandle <= curr->maxHandle)
                {
                  if (curr->cccdIndCB)
                  {
                    // Found correct service. If call back is registered invoke, 
                    // else return error status
                    lRsp.status = curr->cccdIndCB(curr->context, cu->connHandle,
                                                       cu->cccdHandle, cu->rspNeeded,
                                                       cu->value);
                  }
                  break;
                }
                curr = (serviceNode_t *)curr->next;
              }
              // Respond to CCCD Indication                  
              SNP_charConfigUpdatedRsp(&lRsp);
            }
            break;
          
          default:
            break;
        }
      break;

    default:
      // Not GATT request, pass on to all registered call backs
      while(iter != NULL)
      {
        // Send to callback.
        iter->asyncCB(cmd1, pMsg);
        
        // Next callback.
        iter = (asyncCBNode_t *)iter->next;
      }
      break;
  }
}

/*********************************************************************
 * FUNCTIONS
 */

/**
 * @fn      SAP_initParams
 *
 * @brief   Initializes network processor port parameters 
 *
 * @param   None.
 *
 * @return  None.
 */
void SAP_initParams(uint8_t portType, SAP_Params *params)
{
  if (params != NULL)
  {
    params->portType = portType;
    
    if (portType == SAP_PORT_LOCAL)
    {
      // Currently not supported
      params->port.local.reserved = 0xff;
    }
    else if (portType == SAP_PORT_REMOTE_UART)
    {
      params->port.remote = default_remUARTPort;
    }
    else if (portType == SAP_PORT_REMOTE_SPI)
    {
      params->port.remote = default_remSPIPort;
    }
  }
}

/**
 * @fn      SAP_open
 *
 * @brief   Opens the port with the network processor
 *
 * @param   params - list of parameters needed to initialize serial port used
 *                   with network processor
 *
 * @return  uint8_t - SNP_SUCCESS if NPI is open
 */
uint8_t SAP_open(SAP_Params *params)
{
  NPI_Params npiParams;
  SAP_Params sapParams;

  // Check to see if params were given
  if (params == NULL)
  {
    SAP_initParams(SAP_PORT_REMOTE_UART,&sapParams);
    params = &sapParams;
  }
  
  if (params->portType == SAP_PORT_LOCAL)
  {
    // Not currently supported
    return SNP_FAILURE;
  }
  else
  {
    SNP_open();
    
    // set SNP TL Callback to receive events from NP.
    SNP_registerSAPCBs(handleAsyncCB, getEventCallbacks);
    
    // Initialize Network Processor Interface
    if (params->portType == SAP_PORT_REMOTE_UART)
    {
      NPITask_Params_init(NPI_SERIAL_TYPE_UART,&npiParams);
    }
    else if (params->portType == SAP_PORT_REMOTE_SPI)
    {
      NPITask_Params_init(NPI_SERIAL_TYPE_SPI,&npiParams);
    }
    else
    {
      return SNP_FAILURE;
    }
    
    // Update npi params with sap port params
    npiParams.stackSize         = params->port.remote.stackSize;
    npiParams.bufSize           = params->port.remote.bufSize;
    npiParams.mrdyPinID         = params->port.remote.mrdyPinID;
    npiParams.srdyPinID         = params->port.remote.srdyPinID;
    npiParams.portBoardID       = params->port.remote.boardID; 

    if (params->portType == SAP_PORT_REMOTE_UART)
    {
      npiParams.portType = NPI_SERIAL_TYPE_UART;
      npiParams.portParams.uartParams.baudRate = params->port.remote.bitRate;
    }
    else if (params->portType == SAP_PORT_REMOTE_SPI)
    {
      npiParams.portType = NPI_SERIAL_TYPE_SPI;
      npiParams.portParams.spiParams.bitRate = params->port.remote.bitRate;
    }
    
    if (NPITask_open(&npiParams) == NPI_SUCCESS)
    {
      // Setup NPI SNP subsystem
      NPISS_BLE_SNP_init();    
      return SNP_SUCCESS;
    }
  }
  
  return SNP_FAILURE;
}

/**
 * @fn      SAP_close
 *
 * @brief   Tears down the port with the network processor
 *
 * @param   None.
 *
 * @return  uint8_t - SNP_SUCCESS if able to close
 */
uint8_t SAP_close(void)
{
  asyncCBNode_t *aNode = asyncCBListRoot;
  asyncCBNode_t *aTempNode = NULL;
  eventCBNode_t *eNode = eventCBListRoot;
  eventCBNode_t *eTempNode = NULL;
  serviceNode_t *sNode = serviceListRoot;
  serviceNode_t *sTempNode = NULL;

  // Clean up call back and service lists
  while(aNode != NULL)
  {
    aTempNode = (asyncCBNode_t *)aNode->next;
    SNP_free(aNode);
    aNode = aTempNode;
  }
  asyncCBListRoot = NULL;
  
  while(eNode != NULL)
  {
    eTempNode = (eventCBNode_t *)eNode->next;
    SNP_free(eNode);
    eNode = eTempNode;
  }
  eventCBListRoot = NULL;
  
  while(sNode != NULL)
  {
    sTempNode = (serviceNode_t *)sNode->next;
    SNP_free(sNode);
    sNode = sTempNode;
  }
  serviceListRoot = NULL;

  SNP_close();
  
  return NPITask_close();
}

/**
 * @fn      SAP_setAsyncCB
 *
 * @brief   setup Applications' asynchronous callbacks.  This must be called before
 *          using any other calls to SAP.  This function may be called multiple times
 *          to register multiple Callbacks.
 *
 * @param   asyncCB - the asynchronous callback.
 *
 * @return  None.
 */
uint8_t SAP_setAsyncCB(pfnAsyncCB_t asyncCB)
{
  // Register callback.
  return SAP_addToAsyncCBList(asyncCB);
}

/**
 * @fn      SAP_reset
 *
 * @brief   Reset the network processor.
 *
 * @param   None.
 *
 * @return  None.
 */
uint8_t SAP_reset(void)
{
  snpHciCmdReq_t lReq;
  uint8_t status;
  
  // Initialize Request Struct
  lReq.opcode = SNP_HCI_OPCODE_EXT_RESET_SYSTEM;
  lReq.pData = (uint8_t *)SNP_malloc(sizeof(uint8_t));
  lReq.pData[0] = 0x01; // Default to Soft Reset
  
  status = SNP_sendHCICommand(&lReq, sizeof(uint8_t));
  
  // Free Allocated Mem in Request Struct
  SNP_free(lReq.pData);
  
  return status;
}

/**
 * @fn          SAP_registerService
 *
 * @brief       Add a HAP service to the GATT server.
 *
 * @param       service - data to construct the service. 
 *
 * @return      SNP_SUCCESS: The service has been registered.
 *              SNP_FAILURE: service registration failed.
 */

uint8_t SAP_registerService(SAP_Service_t *service)
{
  snpAddServiceReq_t lAddServReq;
  snpRegisterServiceRsp_t regServRsp;
  serviceNode_t *newNode = NULL;
  serviceNode_t *currNode = serviceListRoot;

  // Initialize Add Service Request Struct
  lAddServReq.type = service->serviceType;
  memcpy(lAddServReq.UUID, service->serviceUUID.pUUID, 
         service->serviceUUID.len);
  
  // Add the service, Does not return until handle is received.
  // NULL Passed for response, not currently checking status
  SNP_addService(&lAddServReq, service->serviceUUID.len,
                  NULL);
  
  // Add the characteristics.
  uint8_t i;
  for (i = 0; i < service->charTableLen; i++)
  {
    snpAddCharValueDeclReq_t lValReq;
    snpAddCharValueDeclRsp_t lValRsp;
    SAP_Char_t ch = service->charTable[i];
    
    // Initialize Request
    lValReq.charValPerms = ch.permissions;
    lValReq.charValProps = ch.properties;
    lValReq.mgmtOption = SNP_CHAR_MANAGED_BY_AP;
    lValReq.charValMaxLen = SNP_GATT_CHAR_MAX_LENGTH;
    memcpy(lValReq.UUID, ch.UUID.pUUID, ch.UUID.len);
                               
    // Add the characteristic value and save returned handle
    SNP_addCharValueDec(&lValReq, ch.UUID.len, &lValRsp);
    service->charAttrHandles[i].valueHandle = lValRsp.attrHandle;
    
    // If there are any descriptors
    if (ch.pUserDesc || ch.pCccd || ch.pFormat || ch.pShortUUID || ch.pLongUUID)
    {
      uint8_t hIdx = 0;
      snpAddCharDescDeclReq_t lDescReq;
      snpAddCharDescDeclRsp_t lDescRsp;
      
      // Build Header
      lDescReq.header = (ch.pCccd)? SNP_DESC_HEADER_CCCD : 0;
      lDescReq.header |= (ch.pFormat)? SNP_DESC_HEADER_FORMAT : 0;
      lDescReq.header |= (ch.pUserDesc)? SNP_DESC_HEADER_USER_DESC : 0;
      lDescReq.header |= (ch.pShortUUID)? SNP_DESC_HEADER_GEN_SHORT_UUID: 0;
      lDescReq.header |= (ch.pLongUUID)? SNP_DESC_HEADER_GEN_LONG_UUID: 0;
      
      // Initialize pointers to sub-structs
      lDescReq.pCCCD = ch.pCccd;
      lDescReq.pFormat = ch.pFormat;
      lDescReq.pUserDesc = ch.pUserDesc;
      lDescReq.pShortUUID = ch.pShortUUID;
      lDescReq.pLongUUID = ch.pLongUUID;
      
      SNP_addCharDescDec(&lDescReq, &lDescRsp);
      
      // Copy handles from response into the service characteristic handle array
      service->charAttrHandles[i].sUUIDHandle = 
        (ch.pShortUUID) ? lDescRsp.handles[hIdx++] : SNP_INVALID_HANDLE;
      service->charAttrHandles[i].lUUIDHandle = 
        (ch.pLongUUID) ? lDescRsp.handles[hIdx++] : SNP_INVALID_HANDLE;
      service->charAttrHandles[i].cccdHandle = 
        (ch.pCccd) ? lDescRsp.handles[hIdx++] : SNP_INVALID_HANDLE;
      service->charAttrHandles[i].formatHandle = 
        (ch.pFormat) ? lDescRsp.handles[hIdx++] : SNP_INVALID_HANDLE;
      service->charAttrHandles[i].userDescHandle = 
        (ch.pUserDesc) ? lDescRsp.handles[hIdx++] : SNP_INVALID_HANDLE;
    }
  }
  
  // Register the service.
  SNP_registerService(&regServRsp);
  
  // Set the service handle
  service->serviceHandle = regServRsp.startHandle;

  //Create node
  newNode = (serviceNode_t *)SNP_malloc(sizeof(serviceNode_t));
  
  if (newNode != NULL)
  {
    // Tie service into service list
    if (serviceListRoot == NULL)
    {
      serviceListRoot = newNode;
    }
    else
    {
      while(currNode != NULL)
      {
        if(currNode->next == NULL)
        {
          // Last service in list
          currNode->next = (struct serviceNode_t *)newNode;
          break;
        }
        
        currNode = (serviceNode_t *)currNode->next;
      }
    }
    
    newNode->context = service->context;
    newNode->charReadCB = service->charReadCallback;
    newNode->charWriteCB = service->charWriteCallback;
    newNode->cccdIndCB = service->cccdIndCallback;
    newNode->next = NULL;
    
    // Determine min handle of this service and determine the max handle of the prev service
    newNode->minHandle = service->serviceHandle; //assumption min handle is service handle
    newNode->maxHandle = 0xFFFF;
    
    if (currNode != NULL)
    {
       currNode->maxHandle = service->serviceHandle - 1;
    }
    
  }
  
  return SNP_SUCCESS;
}

/**
 * @brief       Register a callback to receive a GAP event.  This shall be
 *              called once for each callback to be registered.
 *              
 * @param       bleEventCallback - a Callback function to register: @ref SNP_GAP_EVENT_CB
 * @param       bleEventMask     - the mask of events which trigger this 
 *                                 callback. Events types: @ref SNP_GAP_EVENTS
 *
 * @return      SNP_SUCCESS: The callback is registered.
 *              SNP_FAILURE: Callback registration failed.
 */
uint8_t SAP_registerEventCB(pfnEventCB_t eventCB, uint16_t eventMask)
{
  uint8_t status;
  
  // Register callback.
  status = SAP_addToEventCBList(eventCB, eventMask);
  
  return status;
}

/**
 * @fn          SAP_setServiceParam
 *
 * @brief       Write a characteristic value of a service.
 *
 * @param       serviceID - the UUID of the service
 * @param       charID    - the unique handle of the characteristic
 * @param       len       - length of the data to write
 * @param       pData     - pointer to buffer of data to write
 *
 * @return      SNP_SUCCESS: The write completed successfully.
 *              SNP_FAILURE: The write failed.
 */
uint8_t SAP_setServiceParam(uint8_t serviceID, uint8_t charID, 
                            uint16_t len, uint8_t *pData)
{
  snpSetGattParamReq_t lReq; 
  
  // Initialize Request
  lReq.serviceID = serviceID;
  lReq.paramID = charID;
  lReq.pData = pData;
    
  // Send a set parameter request to the SAP. Ignore response. It only
  // contains status which is also what the function returns
  return SNP_setGATTParam(&lReq, len, NULL);
}

/**
 * @fn          SAP_getServiceParam
 *
 * @brief       Read a characteristic value of a service.
 *
 * @param       serviceID - the UUID of the service
 * @param       charID    - the unique handle of the characteristic
 * @param       len       - length of the data read
 * @param       pData     - pointer to buffer to write to
 *
 * @return      SNP_SUCCESS: the read completed successfully.
 *              SNP_FAILURE: The read failed.
 */
uint8_t SAP_getServiceParam(uint8_t serviceID, uint8_t charID, 
                            uint16_t * len, uint8_t * pData)
{
  snpGetGattParamReq_t lReq;
  snpGetGattParamRsp_t lRsp;
  
  // Initialize Request
  lReq.serviceID = serviceID;
  lReq.paramID = charID;
  
  // Initialize Response data field. SNP call will copy into this buffer
  lRsp.pData = pData;

  return SNP_getGATTParam(&lReq, &lRsp, len);
}

/**
 * @brief       Write to a stack parameter on the SAP. Some responses will
 *              Return immediately, others will generate an event for which
 *              a callback must be registered with the correct event mask.
 *
 * @param       subsystemID - the subsystem ID: @ref SNP_PARAM_SUBSYSTEMS
 * @param       paramID     - the parameter within the subsystem to write
 * @param       len         - length of the data to write
 * @param       pData       - pointer to buffer of data to write
 *
 * @return      SNP_SUCCESS: the write completed successfully.<BR>
 *              SNP_FAILURE: stack parameter write failed.<BR>
 */
uint8_t SAP_setParam(uint8_t subsystemID, uint16_t paramID, uint16_t len, 
                     uint8_t *pData)
{
  uint8_t status = SNP_SUCCESS;
  
  // Determine the subsystem.
  switch(subsystemID)
  {
    // HCI command subsystem.
    case SAP_PARAM_HCI:
      {
        snpHciCmdReq_t lReq;
    
        // Initialize Request
        lReq.opcode = paramID;
        lReq.pData = pData;
    
        status = SNP_sendHCICommand(&lReq, len);
      }
      break;
      
    // Advertising subsystem
    case SAP_PARAM_ADV:
      {
        // Determine parameter to write
        switch(paramID)
        {
          // These all send in the same command.
          case SAP_ADV_DATA_NOTCONN:
          case SAP_ADV_DATA_CONN:
          case SAP_ADV_DATA_SCANRSP: 
            {
              snpSetAdvDataReq_t lReq;
              
              // Initialize Request
              lReq.type = (paramID == SAP_ADV_DATA_NOTCONN) ? SNP_NONCONN_ADV_DATA :
                          (paramID == SAP_ADV_DATA_CONN) ? SNP_CONN_ADV_DATA :
                            SNP_SCANRSP_DATA ;
              lReq.pData = pData;
              
              status = SNP_setAdvertisementData(&lReq , len);
            }
            break;
          case SAP_ADV_STATE:
            if (pData[0] == 0)
            {
              // Stop advertising.
              SNP_stopAdvertising();
            }
            else
            {
              snpStartAdvReq_t lReq;
              
              // Initialize Req
              lReq.type = SNP_ADV_TYPE_CONN; // connectable advertising
              lReq.timeout = 0;              // never stops        
              lReq.interval = 100;           // 100 ms interval between advertisement events
              lReq.behavior = SNP_ADV_RESTART_ON_CONN_TERM; // advertising stops upon connection and resumes after the connection terminates
              
              status = SNP_startAdvertising(&lReq);
            }
            break;
          default:
            // Unknown command.
            status = SNP_FAILURE;
            break;
        }
      }
      break;
    
    // Connection subsystem.
    case SAP_PARAM_CONN:
      {
        switch(paramID)
        {
          case SAP_CONN_PARAM:
            {
              snpUpdateConnParamReq_t *lReq;
              
              // Initialize Req
              lReq = (snpUpdateConnParamReq_t *)pData;
              
              status = SNP_updateConnectionParams(lReq);
            }
            break;
          case SAP_CONN_STATE:
            {
              snpTermConnReq_t lReq;
              
              // Initialize Request
              memcpy(&lReq.connHandle,pData,len);
              lReq.option = 0;
              
              // For now, we are peripheral only, so this can only terminate a connection.
              SNP_terminateConnection(&lReq);
            }
            break;
        }
      }
      break;
    
    default:
      // Unknown command
      status = SNP_FAILURE;
      break;
  }
  
  return status;
}

/**
 * @brief       Read a stack parameter on the SAP. Some responses will
 *              Return immediately, others will generate an event for which
 *              a callback must be registered with the correct event mask.
 *
 * @param       subsystemID - the subsystem ID: @ref SNP_PARAM_SUBSYSTEMS
 * @param       paramID     - the parameter within the subsystem to read
 * @param       len         - length of the data to read
 * @param       pData       - pointer to buffer to write to
 *
 * @return      SNP_SUCCESS: the read completed successfully.<BR>
 *              SNP_FAILRUE: stack param read failed.<BR>
 */
uint8_t SAP_getParam(uint8_t subsystemID, uint8_t paramID, uint16_t len,
                     uint8_t *pData)
{
  uint8_t status = SNP_SUCCESS;
  
  // Determine the subsystem.
  switch(subsystemID)
  {
    // HCI command subsystem.
    case SAP_PARAM_HCI:
      {
        // Add HCI commands with accessible fields.
        snpHciCmdReq_t lReq;
    
        // Initialize Request
        lReq.opcode = paramID;
        lReq.pData = pData;
    
        status = SNP_sendHCICommand(&lReq, len);
      }
      break;
    default:
      // Unknown parameter
      status = SNP_FAILURE;
      break;
  }
  
  return status;
}


/*********************************************************************
*********************************************************************/

