/**************************************************************************************************
  Filename:       simpleNP_GAP.h
  Revised:        $Date: 2015-07-20 11:31:07 -0700 (Mon, 20 Jul 2015) $
  Revision:       $Revision: 44370 $

  Description:    This file contains the parsing of GAP related command for 
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
@page GAP  API: GAP related 

 @section gap_api GAP API summary

 commands Acronyms                                 | Origin  | Type     | TL Cmd0                 | TL Cmd1                    | Parameter structure 
-------------------------------------------------- | --------|--------- |------------------------ |----------------------------| --------------------
 [SNP_startAdv_req]   (\ref SNP_startAdv)          | AP      | Async    | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_POWER_UP_IND          | #snpStartAdvReq_t
 [SNP_setAdvData_req] (\ref SNP_setAdvData )       | AP      | Async    | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_SET_ADV_DATA_REQ      | #snpSetAdvDataReq_t
 [SNP_setAdvData_rsp] (\ref SNP_setAdvData )       | NP      | Async    | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_SET_ADV_DATA_CNF      | #snpSetAdvDataCnf_t
 [SNP_stopAdv_req]    (\ref SNP_stopAdv)           | AP      | Async    | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_STOP_ADV_REQ          |  |
 [SNP_updateConnParam_req] (\ref SNP_updateConnParam ) | AP  | Async    | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_UPDATE_CONN_PARAM_REQ | #snpUpdateConnParamReq_t
 [SNP_updateConnParam_rsp] (\ref SNP_updateConnParam ) | NP  | Async    | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_UPDATE_CONN_PARAM_CNF | #snpUpdateConnParamCnf_t
 [SNP_terminateConn_req]   (\ref SNP_terminateConn)    | AP  | Async    | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_TERMINATE_CONN_REQ    | #snpTermConnReq_t
 [SNP_setGAPparam_req] (\ref SNP_setGAPparam )         | AP  | Sync Req | #SNP_NPI_SYNC_REQ_TYPE  | #SNP_SET_GAP_PARAM_REQ     | #snpSetGapParamReq_t
 [SNP_setGAPparam_rsp] (\ref SNP_setGAPparam )         | NP  | Sync Rsp | #SNP_NPI_SYNC_RSP_TYPE  | #SNP_SET_GAP_PARAM_REQ     | #snpSetGapParamRsp_t
 [SNP_getGAPparam_req] (\ref SNP_getGAPparam )         | AP  | Sync Req | #SNP_NPI_SYNC_REQ_TYPE  | #SNP_GET_GAP_PARAM_REQ     | #snpGetGapParamReq_t
 [SNP_getGAPparam_rsp] (\ref SNP_getGAPparam )         | NP  | Sync Rsp | #SNP_NPI_SYNC_RSP_TYPE  | #SNP_GET_GAP_PARAM_REQ     | #snpGetGapParamRsp_t

all those command have some parameters. those parameters are can be set using the indicated packed structure.
note that all parameter/structure field should be in little-endian format (LSB first).
for UUID parameters, take a look at @ref UUID_desc


  @section SNP_startAdv Start Advertisement
    Start advertising on all 3 channels, every 'interval' , for 'timeout' . 

   This request takes as parameter the structure #snpStartAdvReq_t. 

   if a timeout value different than 0 is used, then the following GAP timer will be update :
       - TGAP_GEN_DISC_ADV_INT_MIN : general mode advertisement duration. 
       - TGAP_LIM_ADV_TIMEOUT: limited mode advertisement duration.

   if a timeout value equal 0 is used, the value of the GAP timer will be used:
       - TGAP_GEN_DISC_ADV_INT_MIN : default 0ms, means infinite general mode advertisement duration. 
       - TGAP_LIM_ADV_TIMEOUT: default 180s, limited mode advertisement duration.
   if those value has been change using the  @ref SNP_setGAPparam  request, then set value will be used.

   if a interval value different than 0 is used, then the following GAP parameter will be update :
       - TGAP_GEN_DISC_ADV_INT_MIN and TGAP_GEN_DISC_ADV_INT_MAX : general mode advertisement duration. 
       - TGAP_LIM_DISC_ADV_INT_MIN and TGAP_LIM_DISC_ADV_INT_MAX: limited mode advertisement interval.

   if a interval value equal 0 is used, the value of the GAP parameter will be used:
       - TGAP_GEN_DISC_ADV_INT_MIN and TGAP_GEN_DISC_ADV_INT_MAX : default 100ms, means infinite general mode advertisement duration. 
       - TGAP_LIM_DISC_ADV_INT_MIN and TGAP_LIM_DISC_ADV_INT_MAX: default 100ms, limited mode advertisement duration.
   if those value has been change using the  @ref SNP_setGAPparam  request, then set value will be used.

   The timeout is in ms. 
   The interval in in multiple of 625us, meaning an interval value of 160 will lead to a advertisement every 100ms. 
   Since for now the SNP support only one connection, advertisement done while in a connection will only be non-connectable advertisement.

   Also, when the connection is terminated, if bit 2 of snpStartAdvReq_t::behavior field is set, advertisement will start automatically, even if it was not adv during the connection, or the advertisement was explicitly stopped before.

   When a connection is established, if bit 1 of snpStartAdvReq_t::behavior field is set, advertisement will continue automatically.
   Interval of advertising during a connection is ruled by another set of parameter: TGAP_CONN_ADV_INT_MIN and TGAP_CONN_ADV_INT_MAX.
   By default, those parameters are set to 1280ms. they can be change by using the @ref SNP_setGAPparam  request.

   When advertisement is starting, an event is send by the SNP (@ref SNP_Event_ind).

   Limitation:
    - Advertisement is always performed on all 3 advertisement channels.
    - The address type is always static (no private or public address).
    - The advertisement cannot be directed to a particular address.
    - In order to change the type of an ongoing advertisement, it must be stopped first.

    - This event is send by the AP once the device has powered up.
    - This event will be received if the device reset unexpectedly.
    - One reception of this event the AP should consider that the NP lost any previous configuration.

 @section SNP_setAdvData Set Advertisement Data
    update the raw information data of either the scan response or the advertisement information.
    
    There are 2 buffers for the advertisement data:
      - one for the non-connected state (device is NOT in a connection)
      - one the the connected state (device is in a connection)
    
    When NOT in a connection, if advertisement is requested, the advertisement data store in the non-connected state buffer will be advertise.
    
    When in a connection, if advertisement is requested, the advertisement data store in the connected state buffer will be advertise. 
    If the connected state buffer has not been set (empty), then the adv data of the non-connected mode will be used automatically. 
    This way, if the user does not care about differentiating adv data in a connected mode or in a non connected mode, 
    he does not have to set data for the connected mode.
 
    The SNP_setAdvData_req request takes as parameter the structure #snpSetAdvDataReq_t . 

    The SNP_setAdvData_rsp response uses as parameter the structure #snpSetAdvDataCnf_t .
  
    The maximum advertisement size is for now 31 Bytes.
      
    The default advertising data is the following value  :
    \snippet simpleNP_GAP.c Default adv data      
        
    The default scanning response data is the following value  :
    \snippet simpleNP_GAP.c Default Scan rsp data      
               
    Limitation:
    No advertisement filters policy available for now.


 @section SNP_stopAdv Stop Advertisement
   Stop advertising.
    When advertisement stop, an event is send by the SNP (@ref SNP_Event_ind).

    This request doesn't take any parameters.

 @section SNP_updateConnParam Update connection parameters
   Update the connection parameters.
   A confirmation packet will be reply to this request: SNP_updateConnParam_Rsp. 
   This rsp will return the status of the request: ble_pending, SNP_FAILURE, invalid range. 
   Changing the connection parameters is performed (or not) by the master. 
   Once the parameters has been updated and are active, an event will be send by the SNP (SNP_event_ind)

   This SNP_updateConnParam_req request takes as parameter the structure #snpUpdateConnParamReq_t . 

   The SNP_updateConnParam_req response uses as parameter the structure #snpUpdateConnParamCnf_t .

 @section SNP_terminateConn Terminate Connection
   Terminate any ongoing connection.
   When advertisement stop, an event is send by the SNP (@ref SNP_Event_ind).

   This request takes as parameter the structure #snpTermConnReq_t. 

   Limitation:
   Only one connection is enabled for first release. Therefore the Handle parameter will not be used by the SNP. For API consistency this parameter still needs to be present in the request.

 @section SNP_setGAPparam Set GAP parameters
    This API enables the update of GAP parameters.
    Available GAP parameters are listed in TI HCI vendor Guide, chapter 12.17.


    The SNP_setGAPparam_req request takes as parameter the structure #snpSetGapParamReq_t . 

    The SNP_setGAPparam_rsp response uses as parameter the structure #snpSetGapParamRsp_t .

 @section SNP_getGAPparam Get GAP parameters
    This API enables the read GAP parameters.
    Available GAP parameters are listed in TI HCI vendor Guide, chapter 12.17.

    The SNP_getGAPparam_req request takes as parameter the structure #snpGetGapParamReq_t . 

    The SNP_getGAPparam_rsp response uses as parameter the structure #snpGetGapParamRsp_t .

 */
#ifndef SIMPLEBLEPROCESSOR_GAP_H
#define SIMPLEBLEPROCESSOR_GAP_H

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

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */

/**
 * @defgroup SNP_GAP_API SPNP GAP Command Functions
 *
 * @{
 */

/*-------------------------------------------------------------------
 * FUNCTIONS - Initialization and Configuration
 */

  
/**
 *  @brief Initialize SPNP GAP internal state
 *
 *
 */

/**
 *  @fn      SNP_startAdv
 *
 *  @brief      start advertising
 *
 *  @param[in]   *cmdStartAdv    :advertising control parameter structure
 *
 *  @return  status of the command.
 */
uint8_t SNP_startAdv(snpStartAdvReq_t* cmdStartAdv);

/**
 *  @fn      SNP_stopAdv
 *
 *  @brief      stop advertising
 *
 *  @return  status of the command.
 */
uint8_t SNP_stopAdv( void );

/**
 *  @fn      SNP_setAdvData
 *
 *  @brief      set advertising data
 *
 *  @param[in]   *cmdData    :advertising control parameter structure
 *
 *  @return  status of the command.
 */
uint8_t SNP_setAdvData(snpSetAdvDataReq_t *cmdData, uint8_t len);
/**
 *  @fn      SNP_updateConnParam
 *
 *  @brief      set advertising data
 *
 *  @param[in]   *cmdData    :connection parameter structure
 *
 *  @return  status of the command.
 */
uint8_t SNP_updateConnParam(snpUpdateConnParamReq_t *cmdData);

/**
 *  @fn      SNP_terminateConn
 *
 *  @brief      Terminate an ongoing connection
 *
 *  @param[in]   connHandle  :handle of the connection to terminate
 *  @param[in]   option      :option of the termination
 *
 *  @return  status of the command.
 */
uint8_t SNP_terminateConn( snpTermConnReq_t* cmdStruct);

/**
 *  @fn      SNP_setGapParam
 *
 *  @brief      set a GAP parameter
 *
 *  @param[in]   cmdStruct      :parameters for the command, 
 *                                              #snpSetGapParamReq_t
 *
 *  @return  status of the command.
 */
uint8_t SNP_setGapParam( snpSetGapParamReq_t* cmdStruct);

/**
 *  @fn      SNP_getGapParam
 *
 *  @brief      get a GAP parameter
 *
 *  @param[in]   cmdStruct      :parameters for the command, 
 *                                                      #snpGetGapParamReq_t
 *
 *  @return  status of the command.
 */
uint8_t SNP_getGapParam( snpGetGapParamReq_t* cmdStruct);
/**
 * @}
 */

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLEBLEPROCESSOR_H */
