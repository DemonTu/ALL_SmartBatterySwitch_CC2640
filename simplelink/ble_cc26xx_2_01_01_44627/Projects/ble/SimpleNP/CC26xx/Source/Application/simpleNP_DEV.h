/**************************************************************************************************
  Filename:       simpleNP_DEV.h
  Revised:        $Date: 2015-07-23 15:49:28 -0700 (Thu, 23 Jul 2015) $
  Revision:       $Revision: 44404 $

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
/**
@page DEV  API: device related

  @section dev_api Device API summary
   commands Acronyms                              | Origin  | Type        | TL Cmd0                 | TL Cmd1                 | Parameter structure 
--------------------------------------------------|---------|-------------|-------------------------|-------------------------|---------------------
   \ref SNP_powerUp_ind                           | NP      | Async       | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_START_ADV_REQ      |  |
   [SNP_maskEvent_req  ] (\ref SNP_maskEvent)     | AP      | Sync Req    | #SNP_NPI_SYNC_REQ_TYPE  | #SNP_MASK_EVT_REQ       | #snpMaskEventReq_t
   [SNP_maskEvent_cnf  ] (\ref SNP_maskEvent)     | NP      | Sync Rsp    | #SNP_NPI_SYNC_RSP_TYPE  | #SNP_MASK_EVENT_RSP     | #snpMaskEventRsp_t
   [SNP_getRevision_req] (\ref SNP_getRevision)   | AP      | Sync Req    | #SNP_NPI_SYNC_REQ_TYPE  | #SNP_GET_REVISION_REQ   |  |
   [SNP_getRevision_rsp] (\ref SNP_getRevision)   | NP      | Sync Rsp    | #SNP_NPI_SYNC_RSP_TYPE  | #SNP_GET_REVISION_RSP   | #snpGetRevisionRsp_t
   [SNP_HCI_req] (\ref SNP_HCI)                   | AP      | Async       | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_HCI_CMD_REQ        | #snpHciCmdReq_t
   [SNP_HCI_rsp] (\ref SNP_HCI)                   | NP      | Async       | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_HCI_CMD_RSP        | #snpHciCmdRsp_t
   [SNP_event_ind] (\ref SNP_Event_ind)           | NP      | Async       | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_EVENT_IND          | #snpEvt_t
   [SNP_getStatus_req] (\ref SNP_getStatus)       | AP      | Sync Req    | #SNP_NPI_SYNC_REQ_TYPE  | #SNP_GET_STATUS_RSP     | #snpGetStatusCmdRsp_t
   [SNP_getStatus_rsp] (\ref SNP_getStatus)       | NP      | Sync Rsp    | #SNP_NPI_SYNC_RSP_TYPE  | #SNP_TEST_RSP           | #snpTestCmdRsp_t
   [SNP_test_req  (debug only)] (\ref SNP_test)   | AP      | Sync Req    | #SNP_NPI_SYNC_REQ_TYPE  | #SNP_TEST_REQ           |  |
   [SNP_test_rsp  (debug only)] (\ref SNP_test)   | NP      | Sync Rsp    | #SNP_NPI_SYNC_RSP_TYPE  | #SNP_TEST_RSP           |  |
   [invalid cmd] (\ref Sync_error)                | NP      | Sync Rsp    | #SNP_NPI_SYNC_RSP_TYPE  | #SNP_SYNC_ERROR_CMD_IND |  |
 
  all those command have some parameters. those parameters are can be set using teh indicated packed structure.
  note that all parameter/structure field should be in little-endian format (LSB first).
  for UUID parameters, take a look at @ref UUID_desc

  @section SNP_powerUp_ind SNP power up Indication
  This event is send by the AP once the device has powered up.
  This event will be received if the device reset unexpectedly.
  One reception of this event the AP should consider that the NP lost any previous configuration.

  When this event is received, the following services are initialized and ready to be used:
  - GAP service
  - GATT service
  - Device Info Service.

  This command doesn't have any parameters

  @section SNP_maskEvent SNP mask Event 
  This command enable the AP to mask some events. By default, all events are enabled.
  Purpose is to limit the number of possible wake up condition of the AP and reduce power consumption as much as possible.

  All those events can be triggered asynchronously, either due to an action from the AP, or due to an action of the remote peer, or timer expiration.

  This request takes as parameter the structure #snpMaskEventReq_t. 

  Possible Events are listed here: @ref SNP_EVENT , @see SNP_Event_ind

  @section SNP_Event_ind SNP Event indication
  This indication is send by the SNP to signal an event.
  Events can be masked by using the SNP_maskEvent API ([masking event] (\ref SNP_maskEvent)) (RFU).
  Only one event will be indicated at a time per packet. if several events occurs, they will each be encapsulated in their own TL packet.

  Each event has a 2-bytes Event type.
  along with this type, some events might have some parameters associated with them.

  events parameters are mapped on the @ref snpEvt_t structure, which is a union of #snpConnEstEvt_t, #snpConnTermEvt_t , #snpUpdateConnParamEvt_t , #snpAdvStatusEvt_t and #snpATTMTUSizeEvt_t

  Possible Events are listed here: @ref SNP_EVENT

  The table bellow list the events and the structure used to MAP the parameter of those events:
  
   Event                     |  Parameter structure 
---------------------------- | --------------------
  SNP_CONN_EST_EVT           | #snpEvt_t + #snpConnEstEvt_t
  SNP_CONN_TERM_EVT          | #snpEvt_t + #snpConnTermEvt_t
  SNP_CONN_PARAM_UPDATED_EVT | #snpEvt_t + #snpUpdateConnParamEvt_t
  SNP_ADV_STARTED_EVT        | #snpEvt_t + #snpAdvStatusEvt_t
  SNP_ADV_ENDED_EVT          | #snpEvt_t + #snpAdvStatusEvt_t
  SNP_ATT_MTU_EVT            | #snpEvt_t + #snpATTMTUSizeEvt_t
  SNP_ERROR_EVT              | #snpEvt_t + #snpConnEstEvt_t

  @subsection SNP_event_MTU about ATT MTU size
  The default ATT_MTU size is set to 23 bytes. this implies that no fragmentation is done at HCI level.
  The GATT client can request an ATT_MTU_EXCHANGE method to change the maximum possible ATT MTU size.
  The SNP is configure to manage ATT MTU size up to 251 Bytes.
  If this update occurs, the corresponding event will be send by the SNP. if this event is not received, the AP must assume that ATT MTU size is 23.

  @subsection SNP_event_Error about the error event indication
  This error event occurs if something unexpected occurs while SNP is running some operation.
  The list of all possible SNP errors can be found here \ref SNP_ERRORS 

  @section SNP_getRevision Get SNP Revision Command
  This is use to get the current revision of the SNP API. Along as the full stack revision number as define in the HCI vendor guide.  

  SNP_getRevision_req does not take any parameters.

  SNP_getRevision_rsp parameters are mapped on structure #snpGetRevisionRsp_t.

  @section SNP_getStatus  Get SNP Status Command
  This is use to get the current status of the SNP.  

  SNP_getStatus_req does not take any parameters.

  SNP_getStatus_rsp parameters are mapped on structure #snpGetStatusCmdRsp_t.

  @section SNP_test   SNP test Command
  Return the heap usage of the SNP.
  DEBUG use only, this command may not be present in the final release.

  SNP_test_req does not take any parameters.

  SNP_test_rsp parameters are mapped on structure #snpTestCmdRsp_t.

  @section SNP_HCI   SNP encapsulated HCI Command
  This is sending a HCI command. Only the HCI command listed [here] (\ref SNP_ALLOWED_HCI) are supported:
   
  This SNP_HCI_req command takes the 2 bytes HCI opcode, as define in the TI HCI vendor guide.
  Then the parameters as defined for this command in the TI HCI vendor guide.
  Those parameters can be mapped on the structure #snpHciCmdReq_t

  SNP_test_rsp parameters are mapped on structure #snpHciCmdRsp_t.

  @section Sync_error   SNP synchronous invalid command indication
  This indication packet will be send over TL if a unknown synchronous packet is send to the TL.
  An unknown command send as a synchronous packet will prevent any other command to be send.
  Therefore this indication is send back.

  This indication has no parameter.
  
  another asynchronous error event will also be send to signal this error, it will have the opcode of the faulting command as a parameter. see \ref SNP_Event_ind.
   
*/



#ifndef SIMPLENP_DEV_H
#define SIMPLENP_DEV_H

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
#define SNP_VERSION_MAJOR      0
#define SNP_VERSION_MINOR      7
         
/**********************************************************************
* TYPEDEFS - Initialization and Configuration
*/

/*********************************************************************
 * MACROS
 */
   
/*********************************************************************
 * FUNCTIONS
 */

/**
 *  @fn          SNP_getRev
 *
 *  @brief       send back to the revision of the SNP.
 *
 *  @param[in]   Msg      : Msg from NPI 
 *  @param[out]  None
 *
 *  @return  None.
 */
void SNP_getRev(snpGetRevisionRsp_t *Msg);

/**
 *  @fn          SNP_maskEvt
 *
 * @brief        mask event, prevent them to be send to the AP when they occurred..
 *
 *  @param[in]   Msg      : Msg from NPI 
 *  @param[out]  None
 *
 *  @return  None.
 */
void SNP_maskEvt(snpMaskEventReq_t* pReq, snpMaskEventRsp_t *pRsp);

/**
 *  @fn          SNP_getStatus
 *
 * @brief        send back to the status of the SNP.
 *
 *  @param[in]   None 
 *  @param[out]  pRsp : structure containing the response. 
 *
 *  @return  None.
 */
void SNP_getStatus(snpGetStatusCmdRsp_t *pRsp);

/**
 *  @fn          SNP_executeTestCmd
 *
 * @brief        send back to the AP the HEAP usage.
 *
 *  @param[in]   None 
 *  @param[out]  pRsp : structure containing the response. 
 *
 *  @return  None.
 */
void SNP_executeTestCmd(snpTestCmdRsp_t *pRsp );

/**
 *  @fn          SNP_executeHCIcmd
 *
 *  @brief       call a HCI command
 *
 *  @param[in]   pReq     :HCI request structure, see #snpHciCmdReq_t
 *  @param[in]   dataLen   :Len of variable length parameter field in Bytes
 *  @param[out]  None
 *
 *  @return  status of the command.
 */
uint8_t SNP_executeHCIcmd(snpHciCmdReq_t *pReq, uint16_t dataLen);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLENP_DEV_H */
