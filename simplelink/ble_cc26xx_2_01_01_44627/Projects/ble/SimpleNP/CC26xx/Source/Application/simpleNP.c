/**************************************************************************************************
  Filename:       simpleNP.c
  Revised:        $Date: 2015-07-22 15:05:16 -0700 (Wed, 22 Jul 2015) $
  Revision:       $Revision: 44399 $

  Description:    This file contains the Simple Network processor  application
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
#include "Board.h"

#include "SNP.h"
#include "simpleNP.h"
#include "simpleNP_DEV.h"
#include "simpleNP_GAP.h"
#include "simpleNP_GATT.h"
   
/*********************************************************************
 * CONSTANTS
 */
// Task configuration           
#define SNP_TASK_PRIORITY       1
                                
#ifndef SNP_TASK_STACK_SIZE     
#define SNP_TASK_STACK_SIZE     750
#endif                          

#ifndef SNP_NPI_TASK_STACK_SIZE     
#define SNP_NPI_TASK_STACK_SIZE     602
#endif                          

// MRDY/SRDY Pins used by NPI   
#ifdef POWER_SAVING             
#define SNP_MDRY_PIN            Board_KEY_UP
#define SNP_SDRY_PIN            Board_KEY_DOWN
#else                           
#define SNP_MDRY_PIN            IOID_UNUSED
#define SNP_SDRY_PIN            IOID_UNUSED
#endif
   
/*********************************************************************
 * TYPEDEFS
 */

typedef void (*SNP_pfnFunctionCommand) (uint8_t *pMsg);

// Struct of opcode and function pointer.
typedef struct
{
  uint16_t               opcode;   // opcode
  SNP_pfnFunctionCommand function; // function link to the opcode
} snp_CommandFunction_t;

typedef struct snp_HCIoperation
{
  uint8_t  validity;
  uint16_t opcode;        //!< method of the ongoing ATT operation.
}snp_HCIoperation_t;

/*********************************************************************
 * LOCAL VARIABLES
 */

// Entity ID globally used to check for source and/or destination of messages
ICall_EntityID snp_selfEntity;

// Semaphore globally used to post events to the application thread
static ICall_Semaphore sem;

// Queue object used for app messages
static Queue_Struct appMsg;
static Queue_Handle appMsgQueue;

// Task configuration
Task_Struct snp_task;
Char snp_TaskStack[SNP_TASK_STACK_SIZE];

snp_HCIoperation_t  snp_HCIstore;

// NP Parameters for opening serial port to SNP
static NPI_Params portParamsSNP;

/*********************************************************************
 * EXTERN VARIABLES
 */

extern uint16_t snp_gattEventMask;

/*********************************************************************
 * Extern FUNCTIONS
 */

extern void exceptionHandler(void);

//GAP
extern void SNP_initGAP(void);
extern void SNP_processGapEvent(ICall_HciExtEvt *pMsg);
extern void SNP_processStateChangeEvt(gaprole_States_t newState);

//GATT
extern void SNP_initGATT(void);
extern void SNP_resetGATT(uint16_t handle);
extern void SNP_processCharValueChangeEvt(uint8_t paramID);
extern void SNP_processGATTMsg(gattMsgEvent_t *pMsg);

//SMP
extern void SNP_processSmpEvent(ICall_HciExtEvt *pMsg);

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void SNP_init(void);
static void SNP_taskFxn(UArg a0, UArg a1);
static void SNP_processStackMsg(ICall_Hdr *pMsg);
static void SNP_processAppMsg(snp_Evt_t *pMsg);
static void SNP_processAPMsgEvt(uint8_t *pMsg);
static void SNP_processRxMsg(_npiFrame_t *pMsg);
static void SNP_sendPowerUpInd(void);

static void SNP_processMaskEvtCmd(uint8_t *pMsg);
static void SNP_processGetStatusCmd(uint8_t *pMsg);
static void SNP_processTestCmd(uint8_t *pMsg);
static void SNP_processGetRevCmd(uint8_t *pMsg);
static void SNP_processHciCmd(uint8_t *pMsg);
static void SNP_processInvalidCmd(uint8_t *pMsg);

static void SNP_processStartAdvCmd(uint8_t *pMsg);
static void SNP_processSetAdvDataCmd(uint8_t *pMsg);
static void SNP_processStopAdvCmd(uint8_t *pMsg);
static void SNP_processUpdateConnParamCmd(uint8_t *pMsg);
static void SNP_processTerminateConnCmd(uint8_t *pMsg);
static void SNP_processSetGapParamCmd(uint8_t *pMsg);
static void SNP_processGetGapParamCmd(uint8_t *pMsg);

static void SNP_processAddServiceCmd(uint8_t *pMsg);
static void SNP_processAddCharValDeclCmd(uint8_t *pMsg);
static void SNP_processAddCharDescDeclCmd(uint8_t *pMsg);
static void SNP_processRegisterServiceCmd(uint8_t *pMsg);
static void SNP_processGetAttrValueCmd(uint8_t *pMsg);
static void SNP_processSetAttrValueCmd(uint8_t *pMsg);
static void SNP_processCharReadCnfCmd(uint8_t *pMsg);
static void SNP_processCharWriteCnfCmd(uint8_t *pMsg);
static void SNP_processSendNotifIndCmd(uint8_t *pMsg);
static void SNP_processSetGattParamCmd(uint8_t *pMsg);
static void SNP_processGetGattParamCmd(uint8_t *pMsg);
static void SNP_processCharConfigUpdatedCnfCmd(uint8_t *pMsg);

static void SNP_ProcessHCICmdCompleteEvt(hciEvt_CmdComplete_t *pMsg);

snp_CommandFunction_t SNP_DeviceFctTable[]=
{
  { SNP_MASK_EVT_REQ                   , SNP_processMaskEvtCmd}, 
  { SNP_GET_REVISION_REQ               , SNP_processGetRevCmd},  
  { SNP_HCI_CMD_REQ                    , SNP_processHciCmd},
  { SNP_TEST_REQ                       , SNP_processTestCmd},
  { SNP_GET_STATUS_REQ                 , SNP_processGetStatusCmd},
};

snp_CommandFunction_t SNP_GapFctTable[]=
{
  { SNP_START_ADV_REQ                  , SNP_processStartAdvCmd},
  { SNP_SET_ADV_DATA_REQ               , SNP_processSetAdvDataCmd},
  { SNP_STOP_ADV_REQ                   , SNP_processStopAdvCmd},
  { SNP_UPDATE_CONN_PARAM_REQ          , SNP_processUpdateConnParamCmd},
  { SNP_TERMINATE_CONN_REQ             , SNP_processTerminateConnCmd},
  { SNP_SET_GAP_PARAM_REQ              , SNP_processSetGapParamCmd},
  { SNP_GET_GAP_PARAM_REQ              , SNP_processGetGapParamCmd},
};

snp_CommandFunction_t SNP_GattFctTable[]=
{
  { SNP_ADD_SERVICE_REQ                , SNP_processAddServiceCmd},
  { SNP_ADD_CHAR_VAL_DECL_REQ          , SNP_processAddCharValDeclCmd},
  { SNP_ADD_CHAR_DESC_DECL_REQ         , SNP_processAddCharDescDeclCmd}, 
  { SNP_REGISTER_SERVICE_REQ           , SNP_processRegisterServiceCmd},
  { SNP_GET_ATTR_VALUE_REQ             , SNP_processGetAttrValueCmd},
  { SNP_SET_ATTR_VALUE_REQ             , SNP_processSetAttrValueCmd},
  { SNP_CHAR_READ_CNF                  , SNP_processCharReadCnfCmd},
  { SNP_CHAR_WRITE_CNF                 , SNP_processCharWriteCnfCmd},
  { SNP_SEND_NOTIF_IND_REQ             , SNP_processSendNotifIndCmd},
  { SNP_CCCD_UPDATED_CNF               , SNP_processCharConfigUpdatedCnfCmd},
  { SNP_SET_GATT_PARAM_REQ             , SNP_processSetGattParamCmd},
  { SNP_GET_GATT_PARAM_REQ             , SNP_processGetGattParamCmd}, 
  { SNP_REG_PREDEF_SRV_REQ             , SNP_processInvalidCmd},      
};

snp_CommandFunction_t *groupOpcodeTable[4]= 
{
  SNP_DeviceFctTable,  //Subgroup 0
  SNP_GapFctTable,     //Subgroup 1 
  SNP_GattFctTable,    //Subgroup 2 
  NULL,                //Subgroup 3 
};

uint8_t groupOpcodeTableSize[4]= 
{
  sizeof(SNP_DeviceFctTable) / sizeof(snp_CommandFunction_t),  //Subgroup 0
  sizeof(SNP_GapFctTable) / sizeof(snp_CommandFunction_t),     //Subgroup 1 
  sizeof(SNP_GattFctTable) / sizeof(snp_CommandFunction_t),    //Subgroup 2 
  0,                           //Subgroup 3 
};


/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/**
 *  @fn          SNP_createTask
 *
 *  @brief       Task creation function for the Simple Peripheral network processor.
 *
 *  @param[in]   None
 *  @param[out]  None
 *
 *  @return      None.
 */
void SNP_createTask(void)
{
  Task_Params taskParams;

  // Configure task
  Task_Params_init(&taskParams);
  taskParams.stack = snp_TaskStack;
  taskParams.stackSize = SNP_TASK_STACK_SIZE;
  taskParams.priority = SNP_TASK_PRIORITY;

  Task_construct(&snp_task, SNP_taskFxn, &taskParams, NULL);
}

/**
 *  @fn          SNP_init
 *              
 *  @brief       Called during initialization and contains application
 *               specific initialization (ie. hardware initialization/setup,
 *               table initialization, power up notification, etc), and
 *               profile initialization/setup.
 *
 *  @param[in]   None
 *  @param[out]  None
 *
 *  @return      None.
 */
static void SNP_init(void)
{
  // ******************************************************************
  // N0 STACK API CALLS CAN OCCUR BEFORE THIS CALL TO ICall_registerApp
  // ******************************************************************
  // Register the current thread as an ICall dispatcher application
  // so that the application can send and receive messages.
  ICall_registerApp(&snp_selfEntity, &sem);

  // Hard code the BD Address till CC2650 board gets its own IEEE address
  //uint8_t bdAddress[B_ADDR_LEN] = { 0xAD, 0xDE, 0xBE, 0xBA, 0xFE, 0xCA };
  //HCI_EXT_SetBDADDRCmd(bdAddress);

  // Register the interception callback to the NPI server
  if(NPITask_regSSFromHostCB(RPC_SYS_BLE_SNP,SNP_processRxMsg))
  {
    exceptionHandler();
  }
      
  // Register for unwanted HCI messages, this will overwrite any registration 
  // done from higher priority RTOS task.
  GAP_RegisterForMsgs(snp_selfEntity);

  // Create an RTOS queue for message from profile to be sent to app.
  appMsgQueue = Util_constructQueue(&appMsg);

  SNP_initGAP();
  
  SNP_initGATT();
  
  snp_HCIstore.validity = FALSE;
}

/**
 *  @fn          SNP_taskFxn
 *
 *  @brief       Application task entry point for the Simple BLE Peripheral.
 *
 *  @param[in]   a0, a1 - not used.
 *  @param[out]  None
 *
 *  @return      None.
 */
static void SNP_taskFxn(UArg a0, UArg a1)
{
  // Initialize NPI Params
#ifdef NPI_USE_UART
  NPITask_Params_init(NPI_SERIAL_TYPE_UART, &portParamsSNP);
#elif defined(NPI_USE_SPI) 
  NPITask_Params_init(NPI_SERIAL_TYPE_SPI, &portParamsSNP);
#else
#error "Must define NPI_USE_UART or NPI_USE_SPI for any project including NPI"
#endif //NPI_USE_UART
  portParamsSNP.stackSize = SNP_NPI_TASK_STACK_SIZE;
  portParamsSNP.mrdyPinID = SNP_MDRY_PIN;
  portParamsSNP.srdyPinID = SNP_SDRY_PIN;
  portParamsSNP.bufSize   = 530;
                       
  // Kick off NPI
  NPITask_open(&portParamsSNP);
  
  // Initialize application 
  SNP_init();

  SNP_sendPowerUpInd();
 
  // Application main loop
  for(;;)
  {
    // Waits for a signal to the semaphore associated with the calling thread.
    // Note that the semaphore associated with a thread is signaled when a
    // message is queued to the message receive queue of the thread or when
    // ICall_signal() function is called onto the semaphore.
    ICall_Errno errno = ICall_wait(ICALL_TIMEOUT_FOREVER);

    if(errno == ICALL_ERRNO_SUCCESS)
    {
      ICall_EntityID dest;
      ICall_ServiceEnum src;
      ICall_HciExtEvt *pMsg = NULL;

      if(ICall_fetchServiceMsg(&src, &dest,
                                (void **)&pMsg) == ICALL_ERRNO_SUCCESS)
      {
        if((src == ICALL_SERVICE_CLASS_BLE) && (dest == snp_selfEntity))
        {
          // Process inter-task message
          SNP_processStackMsg((ICall_Hdr *)pMsg);
        }
        
        if(pMsg)
        {
          ICall_freeMsg(pMsg);
        }
      }

      // If RTOS queue is not empty, process app message.
      while(!Queue_empty(appMsgQueue))
      {
        snp_Evt_t *pMsg = (snp_Evt_t *)Util_dequeueMsg(appMsgQueue);
        if(pMsg)
        {
          // Process message.
          SNP_processAppMsg(pMsg);

          // Free the space from the message.
          ICall_free(pMsg);
        }
      }
    }
  }
}


/**
 *  @fn          SNP_processStackMsg
 *
 *  @brief       Application task entry point for the Simple BLE Peripheral.
 *
 *  @param[in]   pMsg :message to process
 *  @param[out]  None
 *
 *  @return      None.
 */
static void SNP_processStackMsg(ICall_Hdr *pMsg)
{
  switch(pMsg->event)
  {
    case GATT_MSG_EVENT:
      SNP_processGATTMsg((gattMsgEvent_t *)pMsg);
      break;

    case HCI_GAP_EVENT_EVENT:
      SNP_processGapEvent((ICall_HciExtEvt *)pMsg);
      break;

    case HCI_SMP_EVENT_EVENT:
      SNP_processSmpEvent((ICall_HciExtEvt *)pMsg);
      break;

  default:
      // do nothing
      break;
  }
}

/**
 *  @fn          SNP_processAppMsg
 *
 *  @brief       Process an incoming callback from a profile, or from the NPI.
 *
 *  @param[in]   pMsg :message to process
 *  @param[out]  None
 *
 *  @return      None.
 */
static void SNP_processAppMsg(snp_Evt_t *pMsg)
{
  switch (pMsg->event)
  {
    case SNP_STATE_CHANGE_EVT:
      SNP_processStateChangeEvt((gaprole_States_t)pMsg->status);
      break;

    case SNP_CHAR_CHANGE_EVT:
      SNP_processCharValueChangeEvt(pMsg->status);
      break;

    case SNP_NEW_AP_MSG_EVT:
        SNP_processAPMsgEvt((uint8_t*)pMsg->pNpiMsg);
        break;

    default:
        // Do nothing.
      break;
  }
}

/**
 *  @fn          SNP_stateChangeCB
 *
 *  @brief       Callback from GAP Role indicating a role state change.
 *
 *  @param[in]   newState :new state
 *  @param[out]  None
 *
 *  @return      None.
 */
void SNP_stateChangeCB(gaprole_States_t newState)
{
  SNP_enqueueMsg(SNP_STATE_CHANGE_EVT, newState);
}

/**
 *  @fn          SNP_processGapEvent
 *
 *  @brief       Process an incoming GAP Event.
 *
 *  @param[in]   pMsg :message to process
 *  @param[out]  None
 *
 *  @return      None.
 */
void SNP_processGapEvent(ICall_HciExtEvt *pMsg)
{
  switch(pMsg->hdr.status)
  {
    case HCI_COMMAND_COMPLETE_EVENT_CODE:
      SNP_ProcessHCICmdCompleteEvt((hciEvt_CmdComplete_t *)pMsg);
      break;

    case HCI_DISCONNECTION_COMPLETE_EVENT_CODE:
      break;

    case HCI_COMMAND_STATUS_EVENT_CODE:
      break;

    case HCI_LE_EVENT_CODE:
      break;
      
    case HCI_VE_EVENT_CODE:
      SNP_ProcessHCICmdCompleteEvt((hciEvt_CmdComplete_t *)pMsg);
      break;

    default:
      break;
  }
}

/**
 *  @fn          SNP_processSmpEvent
 *
 *  @brief       Process an incoming SMP Event.
 *
 *  @param[in]   pMsg :message to process
 *  @param[out]  None
 *
 *  @return      None.
 */
void SNP_processSmpEvent(ICall_HciExtEvt *pMsg)
{
  switch(pMsg->hdr.status)
  {
    case HCI_COMMAND_COMPLETE_EVENT_CODE:
      SNP_ProcessHCICmdCompleteEvt((hciEvt_CmdComplete_t *)pMsg);
      break;

    default:
      break;
  }
}
/**
 *  @fn          SNP_processAPMsgEvt
 *
 *  @brief       Process an incoming NPI Message.
 *
 *  @param[in]   apMsg :message to process
 *  @param[out]  None
 *
 *  @return      None.
 */
static void SNP_processAPMsgEvt(uint8_t *pMsg)
{ 
  uint8_t index;
  uint8_t cmdInvalid = false;
  
  _npiFrame_t *pNpiMsg = (_npiFrame_t*)pMsg; 
  uint8_t groupOpcode = SNP_GET_OPCODE_HDR_CMD1(pNpiMsg->cmd1);
  uint8_t opcode = pNpiMsg->cmd1;
    
  //go through the table associated to the API group, and run the corresponding 
  // function.  
  if((!((groupOpcode >  sizeof(groupOpcodeTable)) || (groupOpcode == 3))) &&        
     (groupOpcodeTable[groupOpcode]))
  {   
    //Walk through the table of opcode&function.
    for(index = 0; index < (groupOpcodeTableSize[groupOpcode]); index++)
    {
      if(opcode == groupOpcodeTable[groupOpcode][index].opcode)
      {
        cmdInvalid = true;
        groupOpcodeTable[groupOpcode][index].function((uint8_t*)pNpiMsg);
        break;
      }
    }
  }
  
  if(cmdInvalid == false)
  {
    // Invalid Command.
    SNP_processInvalidCmd(pMsg);
  }
  NPITask_freeFrame(pNpiMsg);  
}

/**
 *  @fn          SNP_enqueueMsg
 *
 *  @brief       Creates a message and puts the message in RTOS queue.
 *
 *  @param[in]   event  - message event.
 *  @param[in]   status - message status.
 *  @param[out]  None
 *
 *  @return      None.
 */
void SNP_enqueueMsg(uint8_t event, uint8_t status)
{
  snp_Evt_t *pMsg;

  // Create dynamic pointer to message.
  if(pMsg = ICall_malloc(sizeof(snp_Evt_t)))
  {
    pMsg->event = event;
    pMsg->status = status;

    // Enqueue the message.
    Util_enqueueMsg(appMsgQueue, sem, (uint8_t*)pMsg);
  }
}

/**
 *  @fn          SNP_processRxMsg
 *
 *  @brief       callback from NPI when a Rx frame is available.
 *
 *  @param[in]   pMsg  :NPI message 
 *  @param[out]  None
 *
 *  @return      None.
 */
static void SNP_processRxMsg(_npiFrame_t *pMsg)
{
  snp_Evt_t *qMsg;
#ifdef SWO_DEBUG   
  ITM_Port32(1) = 0x22;
  ITM_Port32(2) = pMsg->cmd1;
  ITM_Port32(3) = pMsg->dataLen;
#endif  
  if(qMsg = ICall_malloc(sizeof(snp_Evt_t)))
  {
    qMsg->event = SNP_NEW_AP_MSG_EVT;
    qMsg->status = 0;
    qMsg->pNpiMsg = pMsg;
    Util_enqueueMsg(appMsgQueue, sem, (uint8_t*)qMsg);
  }
  else
  {
    // Free NPI Frame after use
    NPITask_freeFrame(pMsg);
  }
}  

/**
 *  @fn          SNP_replyToHost_send
 *
 *  @brief       send a reply to NPI layer.
 *
 *  @param[in]   type           :type of reply : asynchronous or synchronous 
 *  @param[in]   opcode         : opcode the reply refer to 
 *  @param[in]   *pStatus       :status of the reply if needed (set to 
 *                              NULL if not needed)
 *  @param[in]   sizeParam      :size of the parameter array in Bytes. 
 *                              (set to 0 if none).
 *  @param[in]   *param         :array containing the parameter to return 
 *                              (set to NULL if none).
 *  @param[out]  None
 *
 *  @return      None.
 */
void SNP_replyToHost_send(uint8_t type, uint16_t opcode, uint8_t *pStatus,
                          uint16_t sizeParam, uint8_t *pParam)
{
  _npiFrame_t *pMsg;
  uint8_t dataOffset=0;

  // allocate memory for OSAL hdr + packet
  if(pStatus)
  {
    dataOffset= 1;
  }

  pMsg = (_npiFrame_t *) NPITask_mallocFrame(sizeParam + dataOffset);  

  if(pMsg)
  {
    pMsg->dataLen = sizeParam+dataOffset; //+1 for Status byte
    pMsg->cmd0 = type; 

    pMsg->cmd1 = opcode;
    if(dataOffset)
    {
      pMsg->pData[0] = *pStatus;
    }
    
    memcpy(&pMsg->pData[0+dataOffset], &pParam[0], sizeParam);
      
    NPITask_sendToHost(pMsg);
  }
}
/**
 *  @fn          SNP_replyToHostValue_send
 *
 *  @brief       send a reply to NPI layer using 2 parameter pointer.
 *
 *  @param[in]   type       :type of reply : asynchronous or synchronous 
 *  @param[in]   opcode     :opcode the reply refer to 
 *  @param[in]   *status    :status of the reply if needed (set to 
 *                              NULL if not needed)
 *  @param[in]   sizeParam  :size of the parameter array in Bytes. 
 *                          (set to 0 if none).
 *  @param[in]   *param     :array containing the parameter to return 
 *                          (set to NULL if none).
 *  @param[in]   sizeParam2 :size of the parameter array in Bytes. 
 *                          (set to 0 if none).
 *  @param[in]   *pParam2    :array containing the second parameter to return 
 *                          (set to NULL if none).
 *  @param[out]  None
 *
 *  @return      None.
 */
void SNP_replyToHostValue_send(uint8_t type, uint16_t opcode, uint8_t *pStatus,
                                uint16_t sizeParam, uint8_t *pParam,
                                uint16_t sizeParam2, uint8_t *pParam2)
{
  _npiFrame_t *pMsg;
  uint8_t dataOffset=0;

  // allocate memory for OSAL hdr + packet
  if(pStatus)
  {
    dataOffset= 1;
  }
  pMsg = (_npiFrame_t *) NPITask_mallocFrame(sizeParam + dataOffset +
                                             sizeParam2);  

  if(pMsg)
  {
    pMsg->dataLen = sizeParam+sizeParam2+dataOffset; //+1 for Status byte
    pMsg->cmd0 = type; 

    pMsg->cmd1 = opcode;
    if(dataOffset)
    {
      pMsg->pData[0] = *pStatus;
    }
    
    memcpy(&pMsg->pData[0+dataOffset], &pParam[0], sizeParam);
    memcpy(&pMsg->pData[0+dataOffset+sizeParam], &pParam2[0], sizeParam2);
      
    NPITask_sendToHost(pMsg);
  }
  
}
/**
 *  @fn          SNP_eventToHost_send
 *
 *  @brief       send an asynchronous event to NPI layer.
 *
 *  @param[in]   event           : event to send. 
 *  @param[in]   *pStatus        : status of the reply if needed (set to 
 *                                NULL if not needed)
 *  @param[in]   sizeParam       : size of the parameter array in Bytes. 
 *                                (set to 0 if none).
 *  @param[in]   *pParam         : array containing the parameter to return 
 *                                (set to NULL if none).
 *  @param[out]  None
 *
 *  @return      None.
 */
void SNP_eventToHost_send(uint16_t event, uint8_t *pStatus, uint16_t sizeParam,
                          uint8_t *pParam)
{
  uint8_t *pData;
  uint16_t offset = 0;

  //Only reply for unmasked event
  if((event & snp_gattEventMask))
  {
    return;
  }
  
  // the event data payload is the following
  // Event Type: 2 Bytes.
  // status, if present: 1 Byte.
  // pParam if present: 'sizeParam' bytes,
  if(pStatus)
  {
    pData = ICall_malloc(sizeof(uint16_t) + sizeof(uint8_t) +  sizeParam);
    if(pData)
    {
      pData[2] = *pStatus;
      offset = sizeof(uint8_t);
    }
  }
  else
  {
    pData = ICall_malloc(sizeof(uint16_t) + sizeParam);
  } 

  if(pData)
  {
    pData[0] = LO_UINT16(event);
    pData[1] = HI_UINT16(event);
    
    if(sizeParam)
    {
      memcpy(pData+offset+sizeof(uint16_t), pParam, sizeParam);
    }
    
    SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE, SNP_EVENT_IND, NULL,
                         sizeof(uint16_t)+offset+sizeParam, pData);   
  }
  else
  {
    //Not Enough memory to send the event, try to send a short one with only the 
    // error event type.
    pData = ICall_malloc(sizeof(uint16_t));
    if(pData)
    {
      pData[0] = LO_UINT16(SNP_ERROR_EVT);
      pData[1] = HI_UINT16(SNP_ERROR_EVT);      
      SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE, SNP_EVENT_IND, NULL,
                           sizeof(uint16_t), pData);   
    }
  }
    
  if(pData) 
  {
    ICall_free(pData);  
  }
}

/**
 *  @fn          SNP_sendPowerUpInd
 *
 *  @brief       Send an indication when the device is initialize.
 *
 *  @param[in]   None 
 *  @param[out]  None
 *
 *  @return      None.
 */
static void SNP_sendPowerUpInd(void)
{
  SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE, SNP_POWER_UP_IND, NULL, 0,
                       NULL);
}

/**
 *  @fn          SNP_processTestCmd
 *
 *  @brief       send back to the AP the HEAP usage.
 *
 *  @param[in]   Msg: Msg from NPI 
 *  @param[out]  None
 *
 *  @return      None.
 */
static void SNP_processTestCmd(uint8_t *pMsg)
{
  (void) pMsg;
  snpTestCmdRsp_t pRsp;
  
  SNP_executeTestCmd(&pRsp);
  
  //return the command status
  SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE, SNP_TEST_RSP, NULL, 
                       sizeof(snpTestCmdRsp_t), (uint8_t*)&pRsp);
}

/**
 *  @fn          SNP_processGetStatusCmd
 *
 *  @brief       send back to the status of the SNP.
 *
 *  @param[in]   Msg    :Msg from NPI 
 *  @param[out]  None
 *
 *  @return  None.
 */
static void SNP_processGetStatusCmd(uint8_t *pMsg)
{
  (void) pMsg;
  snpGetStatusCmdRsp_t rsp;
  
  SNP_getStatus(&rsp);
  
  //return the command status
  SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE, SNP_GET_STATUS_RSP, 
                       NULL, sizeof(snpGetStatusCmdRsp_t), (uint8_t*)&rsp);
}

/**
 *  @fn          SNP_processGetRevCmd
 *
 *  @brief       send back to the revision of the SNP.
 *
 *  @param[in]   Msg      : Msg from NPI 
 *  @param[out]  None
 *
 *  @return      None.
 */
static void SNP_processGetRevCmd(uint8_t *pMsg)
{
  (void) pMsg;
  snpGetRevisionRsp_t rsp;
  
  SNP_getRev(&rsp);

  SNP_replyToHost_send(SNP_NPI_SYNC_RSP_TYPE, SNP_GET_REVISION_RSP,
                       &rsp.status, sizeof(snpGetRevisionRsp_t)-1,
                       (uint8_t*)&rsp.snpVer);
}

/**
 *  @fn          SNP_processMaskEvtCmd
 *
 *  @brief       mask event, prevent them to be send to the AP when they 
 *               occurred.
 *
 *  @param[in]   Msg      : Msg from NPI 
 *  @param[out]  None
 *
 *  @return      None.
 */
static void SNP_processMaskEvtCmd(uint8_t *pMsg)
{
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpMaskEventReq_t req;
  snpMaskEventRsp_t pRsp;
  
  req.eventMask = BUILD_UINT16(apMsg->pData[0], apMsg->pData[1]);
  
  pRsp.maskedEvent = req.eventMask;
  
  SNP_maskEvt(&req, &pRsp);

  SNP_replyToHost_send(SNP_NPI_SYNC_RSP_TYPE, SNP_MASK_EVENT_RSP,
                       NULL, sizeof(snpMaskEventRsp_t), (uint8_t*)&pRsp);
}

/**
 *  @fn          SNP_processHciCmd
 *
 *  @brief       execute a HCI command send by the application processor
 *
 *  @param[in]   Msg    :message from the application processor
 *
 *  @return      None.
 */
static void SNP_processHciCmd(uint8_t *pMsg)
{
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpHciCmdReq_t *pReq = (snpHciCmdReq_t*)&apMsg->pData[0];
  uint8_t status;
  
  status = SNP_executeHCIcmd(pReq, (apMsg->dataLen) - SNP_HCI_OPCODE_SIZE);
    
  //return the command status
  if(status != blePending)
  {
    SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE, SNP_HCI_CMD_RSP, &status,
                         SNP_HCI_OPCODE_SIZE, (uint8_t*) (&(pReq->opcode)));
  }
}

/**
 *  @fn         SNP_processInvalidCmd
 *
 *  @brief      reply to the AP that the command is invalid.
 *
 *  @param[in]   Msg    :message from the application processor
 *
 *  @return      None.
 */
static void SNP_processInvalidCmd(uint8_t *pMsg)
{
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;

  uint8_t param[3];
  param[0] = apMsg->cmd0;
  param[1] = apMsg->cmd1;
  param[2] = SNP_CMD_REJECTED;

  // check if the invalid command use a sync req, if so reply with a 
  // dummy sync response to avoid dead lock
  if(NPI_GET_MSG_TYPE(apMsg) == NPI_MSG_TYPE_SYNCREQ)
  {
    SNP_replyToHost_send(SNP_NPI_SYNC_RSP_TYPE, SNP_SYNC_ERROR_CMD_IND, NULL,
                         0, NULL);
  }
    
  //Return Major Error Event
  SNP_eventToHost_send( SNP_ERROR_EVT, 
                         NULL, sizeof(param), param);
}

/**
 *  @fn          SNP_ProcessHCICmdCompleteEvt
 *
 *  @brief       process the reception of a HCI event from the stack. 
 *               send it back to the AP
 *
 *  @param[in]   pMsg      :HCI event
 *  @param[out]  None
 *
 *  @return  status of the command.
 */
static void SNP_ProcessHCICmdCompleteEvt(hciEvt_CmdComplete_t *pMsg)
{
  uint8_t status = SNP_FAILURE;
  switch (pMsg->cmdOpcode)
  {
    case HCI_READ_BDADDR:
      {
        // 0:    Status
        // 1..6: BDADDR
        uint8_t retParam[B_ADDR_LEN+SNP_HCI_OPCODE_SIZE];
        uint16_t len = SNP_HCI_OPCODE_SIZE;
        retParam[0] = LO_UINT16(pMsg->cmdOpcode);
        retParam[1] = HI_UINT16(pMsg->cmdOpcode); 
        if((snp_HCIstore.validity == TRUE) && (snp_HCIstore.opcode == pMsg->cmdOpcode))
        {
          snp_HCIstore.validity = FALSE;
          if((pMsg->pReturnParam[0] == SUCCESS))
          {
            status = SNP_SUCCESS;
            memcpy(&retParam[SNP_HCI_OPCODE_SIZE], &(pMsg->pReturnParam[1]), 
                    B_ADDR_LEN);
            len += B_ADDR_LEN;
          }
        }
        else
        {          
            status = SNP_HCI_RSP_COLLISION_RSP;
        }
        SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE,
                             SNP_HCI_CMD_RSP,
                             &status,
                             len,
                             retParam);
      }
      break;
      
    case HCI_READ_RSSI:
      {
        // 0: Status
        // 1: Connection Handle LSB
        // 2: Connection Handle MSB
        // 3: RSSI
        uint8_t retParam[SNP_HCI_OPCODE_SIZE+ sizeof(uint16_t) + 
                          sizeof(uint8_t)];
        uint16_t len = SNP_HCI_OPCODE_SIZE;
        retParam[0] = LO_UINT16(pMsg->cmdOpcode);
        retParam[1] = HI_UINT16(pMsg->cmdOpcode); 
        if((snp_HCIstore.validity == TRUE) && (snp_HCIstore.opcode == pMsg->cmdOpcode))
        {
          snp_HCIstore.validity = FALSE;
          if((pMsg->pReturnParam[0] == SUCCESS) && 
               (&(pMsg->pReturnParam[1]) != NULL))
          {
            status = SNP_SUCCESS;
            retParam[2] = LO_UINT16(pMsg->pReturnParam[1]);
            retParam[3] = HI_UINT16(pMsg->pReturnParam[2]);
            retParam[4] = pMsg->pReturnParam[3];
            len += sizeof(uint16_t) + sizeof(uint8_t);
          }
        }
        else
        {          
            status = SNP_HCI_RSP_COLLISION_RSP;
        }
        SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE,
                             SNP_HCI_CMD_RSP,
                             &status,
                             len,
                             retParam);
      }
      break;
      
    case HCI_LE_RECEIVER_TEST:
    case HCI_LE_TRANSMITTER_TEST:
      {
        // 0: Status
        uint8_t retParam[SNP_HCI_OPCODE_SIZE+ sizeof(uint16_t) + 
                         sizeof(uint8_t)];
        uint16_t len = SNP_HCI_OPCODE_SIZE;
        retParam[0] = LO_UINT16(pMsg->cmdOpcode);
        retParam[1] = HI_UINT16(pMsg->cmdOpcode); 
        if((snp_HCIstore.validity == TRUE) && (snp_HCIstore.opcode == pMsg->cmdOpcode))
        {
          snp_HCIstore.validity = FALSE;
          status = pMsg->pReturnParam[0];
        }
        else
        {          
            status = SNP_HCI_RSP_COLLISION_RSP;
        }
        SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE,
                             SNP_HCI_CMD_RSP,
                             &status,
                             len,
                             retParam);
      }
      break;
      
    case HCI_LE_TEST_END:
      {
        // 0:    Status
        // 1..2: Number of Packets (for Receive DTM only)
        uint8_t retParam[SNP_HCI_OPCODE_SIZE+ sizeof(uint16_t)];
        uint16_t len = SNP_HCI_OPCODE_SIZE;
        retParam[0] = LO_UINT16(pMsg->cmdOpcode);
        retParam[1] = HI_UINT16(pMsg->cmdOpcode); 
        if((snp_HCIstore.validity == TRUE) && (snp_HCIstore.opcode == pMsg->cmdOpcode))
        {
          snp_HCIstore.validity = FALSE;
          if((pMsg->pReturnParam[0] == SUCCESS) && 
               (&(pMsg->pReturnParam[1]) != NULL))
          {
            status = SNP_SUCCESS;
            retParam[2] = LO_UINT16(pMsg->pReturnParam[1]);
            retParam[3] = HI_UINT16(pMsg->pReturnParam[2]);
            len += sizeof(uint16_t);
          }
        }
        else
        {          
            status = SNP_HCI_RSP_COLLISION_RSP;
        }
        SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE,
                             SNP_HCI_CMD_RSP,
                             &status,
                             len,
                             retParam);
      }
      break;
      
    case HCI_EXT_PER:
      {
        // 2:  Status
        // 3:  Command
        // 4:  Number of Packets (LSB)
        // 5:  Number of Packets (MSB)
        // 6:  Number of CRC Errors (LSB)
        // 7:  Number of CRC Errors (MSB)
        // 8:  Number of Events (LSB)
        // 9:  Number of Events (MSB)
        // 10: Number of Missed Events(LSB)
        // 11: Number of Missed Events (MSB)
        uint8_t retParam[SNP_HCI_OPCODE_SIZE+ 4*sizeof(uint16_t) + 
                         sizeof(uint8_t)];
        uint16_t len = SNP_HCI_OPCODE_SIZE;
        retParam[0] = LO_UINT16(pMsg->cmdOpcode);
        retParam[1] = HI_UINT16(pMsg->cmdOpcode); 
        if((snp_HCIstore.validity == TRUE) && (snp_HCIstore.opcode == pMsg->cmdOpcode))
        {
          snp_HCIstore.validity = FALSE;
          if(pMsg->pReturnParam[2] == SUCCESS) 
          {
            status = SNP_SUCCESS;
            //Copy command
            retParam[2] = pMsg->pReturnParam[3];
            //Copy Number of Packets
            retParam[3] = LO_UINT16(pMsg->pReturnParam[4]);
            retParam[4] = HI_UINT16(pMsg->pReturnParam[5]);
            //Copy Number of CRC errors
            retParam[5] = LO_UINT16(pMsg->pReturnParam[6]);
            retParam[6] = HI_UINT16(pMsg->pReturnParam[7]);
            //Copy Number of Events
            retParam[7] = LO_UINT16(pMsg->pReturnParam[8]);
            retParam[8] = HI_UINT16(pMsg->pReturnParam[9]);
            //Copy Number of Missed Events
            retParam[9] = LO_UINT16(pMsg->pReturnParam[10]);
            retParam[10] = HI_UINT16(pMsg->pReturnParam[11]);
            len +=  4*sizeof(uint16_t) + sizeof(uint8_t);
          }
          else
          {
            status = pMsg->pReturnParam[2];
            retParam[2] = pMsg->pReturnParam[3];
            len += sizeof(uint8_t);
          }
          SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE, SNP_HCI_CMD_RSP, &status,
                               len, retParam);
        }
        else
        {          
            status = SNP_HCI_RSP_COLLISION_RSP;
        }
      }
      break;
      
    case HCI_EXT_SET_BDADDR:
    case HCI_EXT_SET_TX_POWER:
    case HCI_EXT_SET_SCA:
    case HCI_EXT_MODEM_TEST_TX:
    case HCI_EXT_MODEM_HOP_TEST_TX:
    case HCI_EXT_MODEM_TEST_RX:
    case HCI_EXT_END_MODEM_TEST:
    case HCI_EXT_ENABLE_PTM:
    case HCI_EXT_SET_MAX_DTM_TX_POWER:
    case HCI_EXT_OVERRIDE_SL:
    case HCI_EXT_SET_FAST_TX_RESP_TIME:
    case HCI_EXT_ONE_PKT_PER_EVT:
      {
        // 0: Event Opcode (LSB)
        // 1: Event Opcode (MSB)
        // 2: Status
        // For extended command, the event is not returned 
        uint8_t retParam[SNP_HCI_OPCODE_SIZE];
        uint16_t len = SNP_HCI_OPCODE_SIZE;
        retParam[0] = LO_UINT16(pMsg->cmdOpcode);
        retParam[1] = HI_UINT16(pMsg->cmdOpcode); 
        if((snp_HCIstore.validity == TRUE) && (snp_HCIstore.opcode == pMsg->cmdOpcode))
        {
          snp_HCIstore.validity = FALSE;
          status = pMsg->pReturnParam[2];
        }
        else
        {          
            status = SNP_HCI_RSP_COLLISION_RSP;
        }
        SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE, SNP_HCI_CMD_RSP, &status,
                             len, retParam);
      }
      break;
      
    case HCI_EXT_DECRYPT:
      {
        // 0: Event Opcode (LSB)
        // 1: Event Opcode (MSB)
        // 2: Status
        // 3..18: Plain Text Data
        // For extended command, the event opcode is not returned 
        uint8_t retParam[KEYLEN+SNP_HCI_OPCODE_SIZE + sizeof(uint8_t)];
        uint16_t len = SNP_HCI_OPCODE_SIZE;
        retParam[0] = LO_UINT16(pMsg->cmdOpcode);
        retParam[1] = HI_UINT16(pMsg->cmdOpcode); 
        if((snp_HCIstore.validity == TRUE) && (snp_HCIstore.opcode == pMsg->cmdOpcode))
        {
          snp_HCIstore.validity = FALSE;
          if(pMsg->pReturnParam[2] == SUCCESS)
          {
            status = SNP_SUCCESS;
            memcpy(&retParam[SNP_HCI_OPCODE_SIZE], &(pMsg->pReturnParam[2]), 
                    KEYLEN + sizeof(uint8_t));
            len = KEYLEN+SNP_HCI_OPCODE_SIZE + sizeof(uint8_t);
          }
        }
        else
        {          
            status = SNP_HCI_RSP_COLLISION_RSP;
        }
        SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE, SNP_HCI_CMD_RSP, &status,
                             len, retParam);
      }
      break;
      
    case HCI_LE_ENCRYPT:
      {
        // 0:     Status
        // 1..16: Encrypted Data
        uint8_t retParam[KEYLEN+SNP_HCI_OPCODE_SIZE + sizeof(uint8_t)];
        uint16_t len = SNP_HCI_OPCODE_SIZE;
        retParam[0] = LO_UINT16(pMsg->cmdOpcode);
        retParam[1] = HI_UINT16(pMsg->cmdOpcode); 
        if((snp_HCIstore.validity == TRUE) && (snp_HCIstore.opcode == pMsg->cmdOpcode))
        {
          snp_HCIstore.validity = FALSE;
          if(pMsg->pReturnParam[0] == SUCCESS)
          {
            status = SNP_SUCCESS;
            memcpy(&retParam[SNP_HCI_OPCODE_SIZE], &(pMsg->pReturnParam[1]), 
                    KEYLEN );
            len = KEYLEN + SNP_HCI_OPCODE_SIZE;
          }
        }
        else
        {          
            status = SNP_HCI_RSP_COLLISION_RSP;
        }
        SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE, SNP_HCI_CMD_RSP, &status,
                             len, retParam);
      }
      break;
      
    case HCI_EXT_GET_CONNECTION_INFO:
      {
        // 0:  Event Opcode (LSB)
        // 1:  Event Opcode (MSB)
        // 2:  Status
        // 3:  Number Allocated Connections
        // 4:  Number Active Connections
        // For each active connection:
        // 5:  Connection ID
        // 6:  Connection Role
        // 7:  Peer Device Address
        // 13: Peer Device Address Type
        // For extended command, the event opcode is not returned 
        uint8_t retParam[ SNP_HCI_OPCODE_SIZE + 3* sizeof(uint8_t) + 
                          sizeof(hciConnInfo_t)];  //one 1 connection managed.
        uint16_t len = SNP_HCI_OPCODE_SIZE;
        retParam[0] = LO_UINT16(pMsg->cmdOpcode);
        retParam[1] = HI_UINT16(pMsg->cmdOpcode); 
        if((snp_HCIstore.validity == TRUE) && (snp_HCIstore.opcode == pMsg->cmdOpcode))
        {
          snp_HCIstore.validity = FALSE;
          if(pMsg->pReturnParam[2] == SUCCESS)
          {
            status = SNP_SUCCESS;
            memcpy(&retParam[SNP_HCI_OPCODE_SIZE], &(pMsg->pReturnParam[3]), 
                    3* sizeof(uint8_t) + sizeof(hciConnInfo_t) );            
            len = 3* sizeof(uint8_t) + sizeof(hciConnInfo_t) + SNP_HCI_OPCODE_SIZE;
          }
        }
        else
        {          
            status = SNP_HCI_RSP_COLLISION_RSP;
        }
        SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE, SNP_HCI_CMD_RSP, &status,
                             len, retParam);
      }
      break;
     
    default:
      break;     
  }
}

/**
 *  @fn          SNP_processStartAdvCmd
 *
 *  @brief       start advertisement
 *
 *  @param[in]   Msg    :message from the application processor
 *
 *  @return  None.
 */
void SNP_processStartAdvCmd(uint8_t *pMsg)
{
  uint8_t status = SNP_SUCCESS;
  uint8_t value;
  _npiFrame_t * apMsg = (_npiFrame_t *) pMsg;
  snpStartAdvReq_t req;
 
  req.type = apMsg->pData[0];
  req.timeout = BUILD_UINT16(apMsg->pData[1], apMsg->pData[2]);
  req.interval = BUILD_UINT16(apMsg->pData[3], apMsg->pData[4]);
  req.behavior = apMsg->pData[13];
 
  if(req.type == GAP_ADTYPE_ADV_HDC_DIRECT_IND)
  {
    status = SNP_INVALID_PARAMS;
  }
  else
  {
    //If adv was going on, 
    GAPRole_GetParameter(GAPROLE_STATE, &value);
    if((value == GAPROLE_ADVERTISING) || 
       (value == GAPROLE_ADVERTISING_NONCONN)|| 
       (value == GAPROLE_CONNECTED_ADV))
    {
      //Adv need to be stopped first...
      status = SNP_ALREADY_ADVERTISING;
    }
  }

  if(!status)  
  {
    status = SNP_startAdv(&req);      
  }
  if(status)
  {
    //Only send the reply if fail.
    // if success , the indication will be send through gaprole state change,
    SNP_eventToHost_send(SNP_ADV_STARTED_EVT, 
                          &status, 0, NULL);
  }
}

/**
 *  @fn          SNP_processStartAdvCmd
 *
 *  @brief       start advertisement
 *
 *  @param[in]   Msg    :message from the application processor
 *
 *  @return  None.
 */
void SNP_processStopAdvCmd(uint8_t *pMsg)
{
  (void) pMsg;
  uint8_t status;
  uint8_t value = GAPROLE_INIT;

  //If adv was going on, wait for it to stop before sending the reply.
  GAPRole_GetParameter(GAPROLE_STATE, &value);
  if((value != GAPROLE_ADVERTISING) && 
     (value != GAPROLE_ADVERTISING_NONCONN) && 
     (value != GAPROLE_CONNECTED_ADV))
  {
    status = SNP_NOT_ADVERTISING;
    SNP_eventToHost_send(SNP_ADV_ENDED_EVT, &status, 0, NULL);
  }
  else
  {
    SNP_stopAdv();
  }
}

/**
 *  @fn          SNP_processSetAdvDataCmd
 *
 *  @brief       setup advertising data
 *
 *  @param[in]   Msg    :message from the application processor
 *
 *  @return  None.
 */
void SNP_processSetAdvDataCmd(uint8_t *pMsg)
{
  uint8_t status;
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpSetAdvDataReq_t cmdStruct;
  cmdStruct.type = apMsg->pData[0];
  cmdStruct.pData = &apMsg->pData[1];

  if((apMsg->dataLen - 1) <= (B_MAX_ADV_LEN))
  {
    //Move part of the simple peripheral init back here
    status = SNP_setAdvData(&cmdStruct, apMsg->dataLen - 1);    
  }
  else
  {
    status = SNP_INVALID_PARAMS;
  }

  //return the command status
  SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE, SNP_SET_ADV_DATA_CNF, &status,
                        0, NULL);
}

/**
 *  @fn          SNP_processUpdateConnParamCmd
 *
 *  @brief       Command to update Connection Parameter 
 *
 *  @param[in]   Msg    :message from the application processor
 *
 *  @return  None.
 */
void SNP_processUpdateConnParamCmd(uint8_t *pMsg)
{
  uint8_t status;
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpUpdateConnParamReq_t cmdStruct;
  uint8_t connH[2];
  
  cmdStruct.connHandle = BUILD_UINT16(apMsg->pData[0], apMsg->pData[1]);
  cmdStruct.intervalMin = BUILD_UINT16(apMsg->pData[2], apMsg->pData[3]);
  cmdStruct.intervalMax = BUILD_UINT16(apMsg->pData[4], apMsg->pData[5]);
  cmdStruct.slaveLatency = BUILD_UINT16(apMsg->pData[6], apMsg->pData[7]);
  cmdStruct.supervisionTimeout = BUILD_UINT16(apMsg->pData[8], apMsg->pData[9]);
  
  status = SNP_updateConnParam(&cmdStruct);    

  connH[0] = LO_UINT16(cmdStruct.connHandle);
  connH[1] = HI_UINT16(cmdStruct.connHandle);
  
  // return the command status
  SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE,
                        SNP_UPDATE_CONN_PARAM_CNF,
                        &status,
                        2, connH);    
}

/**
 *  @fn          SNP_processTerminateConnCmd
 *
 *  @brief       terminate a connection
 *
 *  @param[in]   Msg    :message from the application processor
 *
 *  @return  None.
 */
void SNP_processTerminateConnCmd(uint8_t *pMsg)
{
  uint8_t status;
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;      
  snpTermConnReq_t *cmdStruct = (snpTermConnReq_t *) &apMsg->pData[0];

  status = SNP_terminateConn(cmdStruct);
  if(status != SUCCESS)
  {
    uint8_t param[3];
    param[0] = 0xFF;   // connection handle
    param[1] = 0xFF;   // connection handle
    param[2] = SNP_INVALID_PARAMS;   // reason.
    //return the command status
    SNP_eventToHost_send(SNP_CONN_TERM_EVT, NULL, 3, param);
  }
}

/**
 *  @fn          SNP_processSetGapParamCmd
 *
 *  @brief       set a GAP parameter
 *
 *  @param[in]   Msg    :message from the application processor
 *
 *  @return  None.
 */
void SNP_processSetGapParamCmd(uint8_t *pMsg)
{
  uint8_t status;
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpSetGapParamReq_t cmdStruct;
 
  cmdStruct.paramId = BUILD_UINT16(apMsg->pData[0], apMsg->pData[1]);
  cmdStruct.value = BUILD_UINT16(apMsg->pData[2], apMsg->pData[3]);

  status = SNP_setGapParam(&cmdStruct);

  //return the command status
  SNP_replyToHost_send(SNP_NPI_SYNC_RSP_TYPE, SNP_SET_GAP_PARAM_RSP,
                       &status, 0, NULL);
}

/**
 *  @fn          SNP_processGetGapParamCmd
 *
 *  @brief       get a GAP parameter
 *
 *  @param[in]   Msg    :message from the application processor
 *
 *  @return  None.
 */
void SNP_processGetGapParamCmd(uint8_t *pMsg)
{
  uint8_t status;
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpGetGapParamReq_t cmdStruct;
  uint8_t param[4];
  
  cmdStruct.paramId = BUILD_UINT16(apMsg->pData[0], apMsg->pData[1]);
  status = SNP_getGapParam(&cmdStruct);
  
  param[0] = LO_UINT16(cmdStruct.paramId);
  param[1] = HI_UINT16(cmdStruct.paramId);
  param[2] = LO_UINT16(cmdStruct.value);
  param[3] = HI_UINT16(cmdStruct.value);
  //return the command status
  // Need to copy the data in a buffer
  SNP_replyToHost_send(SNP_NPI_SYNC_RSP_TYPE, SNP_GET_GAP_PARAM_RSP, &status,
                         sizeof(param),param);
}
/**
 *  @fn          SNP_processAddServiceCmd
 *
 *  @brief       process a 'add service' message.
 *
 *  @param[in]   Msg    : message to process
 *  @param[out]  None
 *
 *  @return  None.
 */
void SNP_processAddServiceCmd(uint8_t *pMsg)
{
  uint8_t status;
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpAddServiceReq_t *cmdStruct = (snpAddServiceReq_t *)&apMsg->pData[0];

  //Add Service
  status = SNP_addService(cmdStruct, apMsg->dataLen-1, NULL);
    
  //return the command status
  SNP_replyToHost_send(SNP_NPI_SYNC_RSP_TYPE, SNP_ADD_SERVICE_RSP, &status,
                       0, NULL);
}

/**
 *  @fn          SNP_processRegisterServiceCmd
 *
 *  @brief       process a 'register service' message.
 *
 *  @param[in]   Msg    : message to process
 *  @param[out]  None
 *
 *  @return  None.
 */
void SNP_processRegisterServiceCmd(uint8_t *pMsg)
{
  (void) pMsg;
  uint8_t status;
  snpRegisterServiceRsp_t rsp;

  //register Service
  status = SNP_registerService(&rsp);
  
  //return the command status
  SNP_replyToHost_send(SNP_NPI_SYNC_RSP_TYPE,
                        SNP_REGISTER_SERVICE_RSP,
                        &status,
                        sizeof(snpRegisterServiceRsp_t)-1,
                        ((uint8_t*) &rsp.status)+1);
}
/**
 *  @fn          SNP_processGetAttrValueCmd
 *
 *  @brief       process a 'get attribute request' message.
 *
 *  @param[in]   Msg    : message to process
 *  @param[out]  None
 *
 *  @return  None.
 */
void SNP_processGetAttrValueCmd(uint8_t *pMsg)
{
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpGetAttrValueReq_t cmdStruct;
  snpGetAttrValueRsp_t rsp;
  uint16_t len;

  cmdStruct.attrHandle = BUILD_UINT16(apMsg->pData[0], apMsg->pData[1]);

  rsp.status = SNP_getAttrValue(&cmdStruct, &rsp, &len);
  rsp.attrHandle = cmdStruct.attrHandle;
  //return the command status
  SNP_replyToHostValue_send(SNP_NPI_SYNC_RSP_TYPE, SNP_GET_ATTR_VALUE_RSP,
                        &rsp.status, 
                        sizeof(uint16_t),
                        (uint8_t*) &(rsp.attrHandle),
                        len ,
                        (uint8_t*) (rsp.pData));
}

/**
 *  @fn          SNP_processSetAttrValueCmd
 *
 *  @brief       process a 'set attribute request' message.
 *
 *  @param[in]   Msg    : message to process
 *  @param[out]  None
 *
 *  @return  None.
 */
void SNP_processSetAttrValueCmd(uint8_t *pMsg)
{
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpSetAttrValueReq_t cmdStruct;
  snpSetAttrValueRsp_t rsp;

  cmdStruct.attrHandle = BUILD_UINT16(apMsg->pData[0], apMsg->pData[1]);
  cmdStruct.pData = &(apMsg->pData[2]);
  
  rsp.status = SNP_setAttrValue(&cmdStruct, 
              apMsg->dataLen - sizeof(snpSetAttrValueReq_t) + sizeof(uint8_t *),
              &rsp);

  //return the command status
  SNP_replyToHost_send(SNP_NPI_SYNC_RSP_TYPE, SNP_SET_ATTR_VALUE_RSP, 
                        &rsp.status, sizeof(snpSetAttrValueRsp_t)-1,
                        (uint8_t*) &rsp.attrHandle);
}

/**
 *  @fn          SNP_processSetGattParamCmd
 *
 *  @brief       process a 'set GATT param request' message.
 *
 *  @param[in]   Msg    : message to process
 *  @param[out]  None
 *
 *  @return  None.
 */
void SNP_processSetGattParamCmd(uint8_t *pMsg)
{
  uint8_t status;
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpSetGattParamReq_t *pReq = (snpSetGattParamReq_t *)&apMsg->pData[0];
  snpSetGattParamRsp_t rsp;

  status = SNP_setGATTParam(pReq, 
       apMsg->dataLen + sizeof(uint8_t*) - sizeof(snpSetGattParamReq_t) , &rsp);

  //return the command status
  SNP_replyToHost_send(SNP_NPI_SYNC_RSP_TYPE, SNP_SET_GATT_PARAM_RSP,
                        &status, 0, NULL);
}

/**
 *  @fn          SNP_processGetGattParamCmd
 *
 *  @brief       process a 'get GATT param request' message.
 *
 *  @param[in]   Msg    : message to process
 *  @param[out]  None
 *
 *  @return  None.
 */
void SNP_processGetGattParamCmd(uint8_t *pMsg)
{
  uint8_t status;
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpGetGattParamReq_t *pReq = (snpGetGattParamReq_t *)&apMsg->pData[0];
  snpGetGattParamRsp_t rsp;
  uint16_t dataLen;
  uint8_t *param;

  status = SNP_getGATTParam(pReq, &rsp, &dataLen);
  
  param = ICall_malloc(dataLen+2);
  
  if((param) && (rsp.pData))
  {      
    param[0] = pReq->serviceID;
    param[1] = pReq->paramID;
    memcpy(&param[2], rsp.pData, dataLen);
    //return the command status
    SNP_replyToHost_send(SNP_NPI_SYNC_RSP_TYPE, SNP_GET_GATT_PARAM_RSP, &status,
                          dataLen+2, param);
    ICall_free(rsp.pData);
    ICall_free(param);
  }
  else
  {
    //return the command status
    status = SNP_OUT_OF_RESOURCES;
    uint8_t temp[2];
    temp[0] = pReq->serviceID;
    temp[1] = pReq->paramID;
    SNP_replyToHost_send(SNP_NPI_SYNC_RSP_TYPE, SNP_GET_GATT_PARAM_RSP, &status,
                          2, temp);
    if(rsp.pData)
    {
      ICall_free(rsp.pData);
    }
    if(param)
    {
      ICall_free(param);
    }
  }
    
}

/**
 *  @fn          SNP_processAddCharValDeclCmd
 *
 *  @brief       process a 'add characteristic value declaration request' message.
 *
 *  @param[in]   Msg    : message to process
 *  @param[out]  None
 *
 *  @return  None.
 */
void SNP_processAddCharValDeclCmd(uint8_t *pMsg)
{
  uint8_t status;
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpAddCharValueDeclReq_t cmdStruct;
  snpAddCharValueDeclRsp_t rsp;

  cmdStruct.charValPerms = apMsg->pData[0];
  cmdStruct.charValProps = BUILD_UINT16(apMsg->pData[1], apMsg->pData[2]);
  cmdStruct.mgmtOption = apMsg->pData[3];
  cmdStruct.charValMaxLen = BUILD_UINT16(apMsg->pData[4], apMsg->pData[5]);
  memcpy(cmdStruct.UUID, &(apMsg->pData[6]), apMsg->dataLen-6);
 
  status = SNP_addCharValueDecl(&cmdStruct, apMsg->dataLen-6, &rsp);
    
  SNP_replyToHost_send(SNP_NPI_SYNC_RSP_TYPE, SNP_ADD_CHAR_VAL_DECL_RSP,
                        &status, sizeof(snpAddCharValueDeclRsp_t)-1,
                        (uint8_t*) &rsp.attrHandle);
}

/**
 *  @fn          SNP_processAddCharDescDeclCmd
 *
 *  @brief       process a 'add characteristic description declaration request'.
 *
 *  @param[in]   Msg    : message to process
 *  @param[out]  None
 *
 *  @return  None.
 */
void SNP_processAddCharDescDeclCmd(uint8_t *pMsg)
{
  uint8_t status;
  uint8_t param[sizeof(snpAddCharDescDeclRsp_t)];
  uint16_t nbAttrib;
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;  

  status = SNP_addDescriptionValue(apMsg->pData[0], &apMsg->pData[1], 
                                    &nbAttrib, param);
    
  SNP_replyToHost_send(SNP_NPI_SYNC_RSP_TYPE, SNP_ADD_CHAR_DESC_DECL_RSP,
                        &status, sizeof(uint8_t)+nbAttrib*sizeof(uint16_t),
                        (uint8_t*) param);  
}

/**
 *  @fn          SNP_sendNotifCmd
 *
 *  @brief       process a 'send notification or indication request'.
 *
 *  @param[in]   Msg    : message to process
 *  @param[out]  None
 *
 *  @return  None.
 */
void SNP_processSendNotifIndCmd(uint8_t *pMsg)
{
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpNotifIndCnf_t rsp;
  snpNotifIndReq_t *pReq = (snpNotifIndReq_t *)&apMsg->pData[0];
  //register Service
  rsp.status = SNP_sendNotifInd(pReq , apMsg->dataLen-6);
  if(rsp.status || (pReq->type & GATT_CLIENT_CFG_NOTIFY))
  {
    rsp.connHandle = pReq->connHandle;
    
    //return the command status
    SNP_replyToHost_send(SNP_NPI_ASYNC_CMD_TYPE, SNP_SEND_NOTIF_IND_CNF,
                          &(rsp.status), sizeof(snpNotifIndCnf_t)-1,
                          (uint8_t*) &(rsp.connHandle));
  }
}
/**
 *  @fn          SNP_processCharWriteCnfCmd
 *
 *  @brief       process a 'Characteristic write confirmation' message.
 *
 *  @param[in]   Msg    : message to process
 *  @param[out]  None
 *
 *  @return  None.
 */
void SNP_processCharWriteCnfCmd(uint8_t *pMsg)
{
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpCharWriteCnf_t cmdStruct;
  cmdStruct.status = apMsg->pData[0];
  cmdStruct.connHandle = BUILD_UINT16(apMsg->pData[1], apMsg->pData[2]);

  SNP_writeCharCnf(&cmdStruct);
  
}
/**
 *  @fn          SNP_processCharConfigUpdatedCnfCmd
 *
 *  @brief       process a 'Characteristic Config update confirmation' message.
 *
 *  @param[in]   Msg    : message to process
 *  @param[out]  None
 *
 *  @return  None.
 */
void SNP_processCharConfigUpdatedCnfCmd(uint8_t *pMsg)
{
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpCharCfgUpdatedRsp_t cmdStruct;

  cmdStruct.status = apMsg->pData[0];
  cmdStruct.connHandle = BUILD_UINT16(apMsg->pData[1], apMsg->pData[2]);

  SNP_processCharConfigUpdatedCnf(&cmdStruct);
  
}

/**
 *  @fn          SNP_processCharReadCnfCmd
 *
 *  @brief       process a 'Characteristic read confirmation' message.
 *
 *  @param[in]   Msg    : message to process
 *  @param[out]  None
 *
 *  @return  None.
 */
void SNP_processCharReadCnfCmd(uint8_t *pMsg)
{
  _npiFrame_t *apMsg = (_npiFrame_t *)pMsg;
  snpCharReadCnf_t cmdStruct;
  
  cmdStruct.status = apMsg->pData[0];
  cmdStruct.connHandle = BUILD_UINT16(apMsg->pData[1], apMsg->pData[2]);
  cmdStruct.attrHandle = BUILD_UINT16(apMsg->pData[3], apMsg->pData[4]);
  cmdStruct.offset = BUILD_UINT16(apMsg->pData[5], apMsg->pData[6]);
  cmdStruct.pData = &(apMsg->pData[7]);
  
  //second parameter is the length of the data field. this field is calculated
  // based one frame length, minus the size of the other field
  SNP_readCharCnf(&cmdStruct,
                apMsg->dataLen + sizeof(uint8_t *) - sizeof(snpCharReadCnf_t));
}
