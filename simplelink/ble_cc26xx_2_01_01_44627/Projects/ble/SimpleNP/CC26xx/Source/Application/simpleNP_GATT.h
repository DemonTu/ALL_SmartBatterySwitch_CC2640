/******************************************************************************* 
  Filename:       simpleNP_GATT.h
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
**************************************************************************************************/
/**
@page GATT  API: GATT related 

  Those APIs are used to create and manage services and their characteristics.
  Custom services can be created using the following APIs:
    - SNP_addService
    - SNP_addCharValueDecl
    - SNP_addCharDescDecl
    - SNP_service_register

  Access to characteristics value can be done through a set of API.  

 @section gatt_api GATT API summary

 commands Acronyms                                         | Origin  | Type      | TL Cmd0                 | TL Cmd1                     | Parameter structure 
---------------------------------------------------------- |---------|-----------|-------------------------|-----------------------------| --------------------
 [SNP_addService_req] (\ref SNP_addService )               | AP      | Sync Req  | #SNP_NPI_SYNC_REQ_TYPE  | #SNP_ADD_SERVICE_REQ        | #snpAddServiceReq_t
 [SNP_addService_rsp] (\ref SNP_addService )               | NP      | Sync Rsp  | #SNP_NPI_SYNC_RSP_TYPE  | #SNP_ADD_SERVICE_RSP        | #snpAddServiceRsp_t
 [SNP_addCharValueDecl_req] (\ref SNP_addCharValueDecl )   | AP      | Sync Req  | #SNP_NPI_SYNC_REQ_TYPE  | #SNP_ADD_CHAR_VAL_DECL_REQ  | #snpAddCharValueDeclReq_t
 [SNP_addCharValueDecl_rsp] (\ref SNP_addCharValueDecl )   | NP      | Sync Rsp  | #SNP_NPI_SYNC_RSP_TYPE  | #SNP_ADD_CHAR_VAL_DECL_RSP  | #snpAddCharValueDeclRsp_t
 [SNP_addCharDescDecl_req] (\ref SNP_addCharDescDecl )     | AP      | Sync Req  | #SNP_NPI_SYNC_REQ_TYPE  | #SNP_ADD_CHAR_DESC_DECL_REQ |  |
 [SNP_addCharDescDecl_rsp] (\ref SNP_addCharDescDecl )     | NP      | Sync Rsp  | #SNP_NPI_SYNC_RSP_TYPE  | #SNP_ADD_CHAR_DESC_DECL_RSP |  |
 [SNP_registerService_req] (\ref SNP_registerService )     | AP      | Sync Req  | #SNP_NPI_SYNC_REQ_TYPE  | #SNP_REGISTER_SERVICE_REQ   |  |
 [SNP_registerService_rsp] (\ref SNP_registerService )     | NP      | Sync Rsp  | #SNP_NPI_SYNC_RSP_TYPE  | #SNP_REGISTER_SERVICE_RSP   | #snpRegisterServiceRsp_t
 [SNP_getAttrValue_req] (\ref SNP_getAttrValue )           | AP      | Sync Req  | #SNP_NPI_SYNC_REQ_TYPE  | #SNP_GET_ATTR_VALUE_REQ     | #snpGetAttrValueReq_t
 [SNP_getAttrValue_rsp] (\ref SNP_getAttrValue )           | NP      | Sync Rsp  | #SNP_NPI_SYNC_RSP_TYPE  | #SNP_GET_ATTR_VALUE_RSP     | #snpGetAttrValueRsp_t
 [SNP_setAttrValue_req] (\ref SNP_setAttrValue )           | AP      | Sync Req  | #SNP_NPI_SYNC_REQ_TYPE  | #SNP_SET_ATTR_VALUE_REQ     | #snpSetAttrValueReq_t
 [SNP_setAttrValue_rsp] (\ref SNP_setAttrValue )           | NP      | Sync Rsp  | #SNP_NPI_SYNC_RSP_TYPE  | #SNP_SET_ATTR_VALUE_RSP     | #snpSetAttrValueRsp_t
 [SNP_charRead_ind] (\ref SNP_charRead )                   | NP      | Async     | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_CHAR_READ_IND          | #snpCharReadInd_t
 [SNP_charRead_cnf] (\ref SNP_charRead )                   | AP      | Async     | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_CHAR_READ_CNF          | #snpCharReadCnf_t
 [SNP_charWrite_ind] (\ref SNP_charWrite )                 | NP      | Async     | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_CHAR_WRITE_IND         | #snpCharWriteInd_t
 [SNP_charWrite_cnf] (\ref SNP_charWrite )                 | AP      | Async     | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_CHAR_WRITE_CNF         | #snpCharWriteCnf_t
 [SNP_sendNotifInd_req] (\ref SNP_sendNotifInd )           | AP      | Async     | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_SEND_NOTIF_IND_REQ     | #snpNotifIndReq_t
 [SNP_sendNotifInd_rsp] (\ref SNP_sendNotifInd )           | NP      | Async     | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_SEND_NOTIF_IND_CNF     | #snpNotifIndCnf_t
 [SNP_charConfigUpdated_req] (\ref SNP_charConfigUpdated ) | NP      | Async     | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_CCCD_UPDATED_IND       | #snpCharCfgUpdatedInd_t
 [SNP_charConfigUpdated_rsp] (\ref SNP_charConfigUpdated ) | AP      | Async     | #SNP_NPI_ASYNC_CMD_TYPE | #SNP_CCCD_UPDATED_CNF       | #snpCharCfgUpdatedRsp_t
 [SNP_setGATTparam_req] (\ref SNP_setGATTparam )           | AP      | Sync Req  | #SNP_NPI_SYNC_REQ_TYPE  | #SNP_SET_GATT_PARAM_REQ     | #snpSetGattParamReq_t
 [SNP_setGATTparam_rsp] (\ref SNP_setGATTparam )           | NP      | Sync Rsp  | #SNP_NPI_SYNC_RSP_TYPE  | #SNP_SET_GATT_PARAM_RSP     | #snpSetGattParamRsp_t
 [SNP_getGATTparam_req] (\ref SNP_getGATTparam )           | AP      | Sync Req  | #SNP_NPI_SYNC_REQ_TYPE  | #SNP_GET_GATT_PARAM_REQ     | #snpGetGattParamReq_t
 [SNP_getGATTparam_rsp] (\ref SNP_getGATTparam )           | NP      | Sync Rsp  | #SNP_NPI_SYNC_RSP_TYPE  | #SNP_GET_GATT_PARAM_RSP     | #snpGetGattParamRsp_t

  All those command have some parameters. those parameters can be set using the indicated packed structure.
  Note that all parameters/structure field should be in little-endian format (LSB first).
  for UUID parameters, take a look at @ref UUID_desc

  Services and characteristic cannot be added if a device has started.
  Device start when the start advertisement command or the set advertisement command is send to the SNP.
  Once started, the only way to stop the device is to reset it.

  @section SNP_addService Add a Service
    This API starts the addition of a new service to the GATT server running on the NP.
    Another SNP command must be used to add additional characteristic to the service. 
    See @ref SNP_addCharValueDecl and @ref SNP_addCharDescDecl 

    Only one service can be added at a time. Once this command has been set, the service need to be register before being able to add another service.
    Registering a service is done by sending the register service command, see #SNP_registerService

    This SNP_addService_req request takes as parameter the structure #snpAddServiceReq_t . 

    The SNP_addService_rsp response uses as parameter the structure #snpAddServiceRsp_t .

    The snpAddServiceReq_t  has a UUID field. please check @ref UUID_desc to know more about how to use UUID field.

    Limitation:
     - services and characteristic definitions are not store in flash.
     - If a reset of the NP occurs, all services and characteristics created by the NP are lost and need to be setup again.

  @section SNP_addCharValueDecl Add a Characteristic to a service
    User can create their own custom services. User needs to know what he wants and how services and characteristics are working.
    Consider reading BLE core spec, vol 3, Part G, chapter 3.
    To give enough flexibility to create a custom service, the following commands are available:
      - SNP_addCharValueDecl
      - SNP_addCharDescDecl    

    The SNP_addCharValueDecl command will add the following attribute to the service:
      - Characteristic declaration.
      - Characteristic value declaration.

    Note that those attributes are the only ones mandatory per BLE specification. All others attributes are optional. 
    This API always needs to be call first when adding a new characteristic to a service. Other attributes can be added with a call to @ref SNP_addCharDescDecl. 

    If the NOTIFY and INDICATE properties are enable, the CCCD attribute must be added (using @ref SNP_addCharDescDecl), or those properties will never been usable.

    The option bit field is reserve for future used.
    characteristics value is managed by the AP:
      - If a GATT client request a read, the NP GATT server forward the request to the AP. 
      The AP has 30s to send the response. Characteristic value is not stored on the NP. AP needs to store them if needed.
      - If a GATT client request a write, the NP GATT server forward the request to the AP. 
      The AP has 30s to send the response. Characteristic value is not stored on the NP. AP needs to store them if needed.

    This SNP_addCharValueDecl_req request takes as parameter the structure #snpAddCharValueDeclReq_t . 

    The SNP_addCharValueDecl_rap response uses as parameter the structure #snpAddCharValueDeclRsp_t .

    The #snpAddCharValueDeclReq_t structure has a permission field. Attribute permissions are define by higher layer profile or are implementation specific.
    TI BLE STACK has define its own value for this field. it is also performing basic filtering using those value (e.g. a read request on a characteristic value those not permit read will be rejected by the stack directly).
    Permission value are define here: @ref SNP_PERMIT_BITMAPS_DEFINES

    The #snpAddCharValueDeclReq_t structure has a property field. Attribute properties are defined by the BLE specification.
    TI BLE STACK expect the properties to be align logically with the permission, meaning a read permission should lead to a the read properties enable.  
    Properties value are define here: @ref SNP_PROP_BITMAPS_DEFINES

  @section SNP_addCharDescDecl Add a optional attribute to a characteristic
    This API is used to add the following attributes to the service:
     - User description.
     - Characteristic Client Characteristic Configuration (CCCD).
     - Presentation format.
     - Server Characteristic configuration (RFU).
     - Aggregate format. (RFU)
     - generic attribute using short UUID (2 bytes)
     - generic attribute using long UUID (16 bytes)

    The attribute permission for the user description is force to READ ONLY.
  
    All those attributes are optional. But at least one needs to be added when this API is call.
      CCCD, presentation format and user description attribute can only be present each once for each characteristic. Trying to create a second one will lead to a #SNP_CMD_REJECTED error.
      

    The first parameter of the API is a header indicating which attribute need to be added.
    When a bit is set, the corresponding attribute is present on the following parameters and will be added to the characteristic.
    The parameter for an attribute needs to appear in the same order as the bit appear in the header.

    For example, if both CCCD and user description attribute needs to be added.
    Then the header will have the value 0x5 (b101).
    The CCCD parameter (1 byte) will then follow the header, and the user description parameter (N Bytes) will follow the CCCD parameter.
    Other parameters must be omitted.

    the SNP_addCharDescDecl_req command TL frame is the following:

    Header (1 Bytes)                                                                                                                    | Generic short UUID (5 Byte) \n present only if bit set in header  | Generic long UUID (19 Byte) \n present only if bit set in header | CCCD (1 Byte) \n present only if bit set in header  | Format (7 Bytes)  \n present only if bit set in header  | User Description  (N Bytes)  \n present only if bit set in header               
----------------------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------|------------------------------------------------------------------|-----------------------------------------------------|---------------------------------------------------------|------------------------ 
    bit 7: user Descr \n bit 6:4 RFU \n bit 3: Format Descr. \n bit 2 : CCCD \n bit 1: Generic-Long UUID \n bit 0 : Generic-Short UUID  | #snpAddAttrGenShortUUID_t                                             | #snpAddAttrGenLongUUID_t                                             | #snpAddAttrCccd_t                                   | #snpAddAttrFormat_t                                     | #snpAddAttrUserDesc_t

    The Generic Short UUID descriptor parameters are mapped on structure #snpAddAttrGenShortUUID_t

    The Generic Long UUID descriptor parameters are mapped on structure #snpAddAttrGenLongUUID_t

    The CCCD descriptor parameters are mapped on structure #snpAddAttrCccd_t

    The format descriptor parameters are mapped on structure #snpAddAttrFormat_t

    The user descriptor parameters are mapped on structure #snpAddAttrUserDesc_t

    The #snpAddAttrCccd_t  structure has a permission field. Attribute permissions are define by higher layer profile or are implementation specific.
    TI BLE STACK has define its own value for this field. It is also performing basic filtering using those values (e.g. a read request on a characteristic value those not permit read will be rejected by the stack directly).
    Permission value are define here: @ref SNP_PERMIT_BITMAPS_DEFINES.

    The SNP_addCharDescDecl_rsp will use the same concept has the SNP_addCharDescDecl_req.
    A header will describe which attribute has been added. If a bit is set, the corresponding attribute has been added and the handle value is indicated in the parameters following the header.

    The SNP_addCharDescDecl_rsp command TL frame is the following:

    Status                    | Header (1 Bytes)                                                                                                                    | Generic short UUID (2 Bytes) \n present only if bit set in header | Generic long UUID (2 Bytes) \n present only if bit set in header | CCCD handle (2 bytes) \n present only if bit set in header  | Format handle (2 Bytes)   \n present only if bit set in header | User Descr handle (2 Bytes) \n present only if bit set in header              
------------------------------|-------------------------------------------------------------------------------------------------------------------------------------|-------------------------------------------------------------------|------------------------------------------------------------------|-------------------------------------------------------------|----------------------------------------------------------------|------------------------------------------------------------------- 
     status of the operation  | bit 7: user Descr \n bit 6:4 RFU \n bit 3: Format Descr. \n bit 2 : CCCD \n bit 1: Generic-Long UUID \n bit 0 : Generic-Short UUID  | 16-bit assigned attribute handle                                  | 16-bit assigned attribute handle                                 | 16-bit assigned attribute handle                            | 16-bit assigned attribute handle                               | 16-bit assigned attribute handle


    @subsection about_CCCD About CCCD
      If the NOTIFY and INDICATE properties are enabled, the CCCD attribute must be added, or those properties will never been usable.

      This attribute has some default permission set at BLE core spec level : it is always readable without authentication or authorization. 
      However, right of this characteristic by a remote GATT client is subject to authentication or authorization by a higher layer.
      Since security in not enable yet, the automatic management of this authentication/authorization is not done by the BLE stack. 
      Therefore this is implementation specific. When this write request comes for this CCCD from the GATT client, it is forwarded to the AP for validation through the @ref SNP_charConfigUpdated command.
      This is an ATT transaction, the application has 30s to confirm the update or return an error (if the application does not allow the attribute value to be update, it must reply an error).

      The AP needs to reply with the SNP_charConfigUpdated_rsp to confirm that it allows the write to the attribute value (or not).

  @section SNP_registerService Register a Service
    This command will register the service and characteristic previously added with the GATT server.

    This SNP_registerService_req request takes no parameters

    The SNP_registerService_rsp response uses as parameter the structure #snpRegisterServiceRsp_t .
    The SNP_registerService_rsp return the first attribute handle and the last attribute handle of the service.

    Limitation:
     - Services and characteristic definitions are not store in flash.
     - If a reset of the NP occurs, all services and characteristics created by the NP are lost and need to be setup again.

  @section SNP_getAttrValue Get Attribute Value
    This action read the value of a attribute managed by the GATT server in the NP.

    This API can be used to get the user description attribute value, and format attribute value. Those values are managed by the GATT server and are therefore always store in the NP.

    For other attribute, this is RFU, since no characteristic value will be store in the NP for custom services. 
    For embedded service services, the API SNP_getGATTparam should be used.

    If the char value is managed by the AP, this request will be rejected.
      
    This SNP_getAttrValue_req request takes as parameter the structure #snpGetAttrValueReq_t . 

    The SNP_getAttrValue_rsp response uses as parameter the structure #snpGetAttrValueRsp_t .

    The list of parameters for the device information service can be found here: @ref SNP_DEV_INFO_SERVICE
    The list of parameters for the GAP/GATT service can be found here: @ref SNP_GAP_SERVICE

      
  @section SNP_setAttrValue Set GATT parameters
    If the characteristic value is managed by the NP (like the user descriptor attribute), this command is used to update the value. 
    This value must not be bigger than 512 Bytes.

    This API can be used to update the user description attribute value and format attribute value. Those values are managed by the GATT server and are therefore always store in the NP.

    For other attribute, this is RFU, since no characteristic value will be store in the NP for custom services. 
    For embedded services, the API SNP_setGATTparam should be used.

    The length should never exceed the maximum length of the attribute value.

    If the char value is managed by the AP, this request will be rejected.

    The SNP_setAttrValue_req request takes as parameter the structure #snpSetAttrValueReq_t . 

    The SNP_setAttrValue_rsp response uses as parameter the structure #snpSetAttrValueRsp_t .
       
  @section SNP_charRead Characteristic read indication
    The SNP_charRead_ind Event is sent by the SNP when a remote GATT client wants to read a characteristic value manage by the AP.
    The AP needs to answer with a SNP_charRead_rsp within 30s.
    Note that a characteristic value can be up to 512 Bytes long. However, not more than ATT_MTU_SIZE can be read at once by a ATT operation. 
    If the characteristic size is bigger than ATT_MTU_SIZE, the remote GATT client will send several read, until it reads all the value it needs. 
    There is several different way for the GATT client to read fully a characteristic, in order not to over-complicate the SNP design, each read request send by the client is translated in a SNP_charRead_req with two parameters: an offset and the maximum data size.
    The offset represent the start of the data to read, the maximum data size represent maximum size to answer.
    The AP needs to reply with the SNP_charRead_rsp command. It will indicate in this command the offset the data were read from, the size of the data and the data.
    The default ATT_MTU_SIZE is set to 27. If the size change (negotiate between GATT server and GATT client), then a MTU SIZE event will be send by the NP to the AP ( @ref SNP_Event_ind). 
    The AP cannot initiate such a change.

    This SNP_charRead_ind indication is mapped on the  #snpCharReadInd_t structure .

    This SNP_charRead_cnf confirmation is mapped on the  #snpCharReadCnf_t structure .

 @section SNP_charWrite Characteristic write indication
    The SNP_charWrite_ind Event is sent by the SNP when a remote GATT client wants to write a characteristic value manage by the AP.
    The AP needs to answer with a SNP_charWrite_rsp within 30s.
    Note that the GATT client may request a partial write, hence the usage of the offset where the write should begin relatively to the beginning of the characteristic value (the offset is a number of Bytes).
    Note: the characteristic value length is application specific. Neither the SNP or the AP NP library know the length of the characteristic value.
    Some applications may in some cases use unknown variable length. To enable those cases, the SNP is configured to be able to manage 512 Bytes char value length whatever the ATT_MTU_SIZE is negotiated to.

    Once a remote GATT client start to write a char value with offset 0, this means it will write the entire value (4.9.4, Part G, Vol3).
    If the remote side start to write with an offset different than 0, this means it is a partial write. 

    In any case, it is the responsibility of the application to know the length of each characteristic value. 

    The snpCharWriteInd_t::rspNeeded field set to 1 indicates that the AP must send the SNP_charWrite_rsp  back within 30s.
    If this field is set to 0, the SNP_charWrite_rsp is not needed. This last case can happen if a ATT_WRITE_CMD operation has been used by the remote GATT client. 
    If a confirmation is send anyway, it will be ignore by the NP.
    
    The application can also have their own permission mechanism and rules. In this case it is the responsibility of the application to deny the writing of the value by return a error status with  SNP_charRead_cnf.
    This SNP_charWrite_ind indication is mapped on the  #snpCharWriteInd_t structure.

    This SNP_charWrite_cnf confirmation is mapped on the  #snpCharWriteCnf_t structure.
    
  @section SNP_sendNotifInd  Sending Notification or Indication
    The SNP_charRead_ind Event send a notification or an indication for a characteristic value. 
    Note that notifications and indications are managed by the GATT server, and will only be sent to GATT client that are actually requesting for them.
    
    If a notification/indication is send to a GATT client that did not enabled it, a error #SNP_NOTIF_IND_NOT_ENABLE_BY_CLIENT will be return in the status of SNP_sendNotiInd_rsp.
    
    If a notification/indication is send for a characteristic which does not have this properties enabled, a error #SNP_NOTIF_IND_NOT_ALLOWED  will be return.
  
    If a notification/indication is send for a characteristic value, which has the properties enable, but has no CCCD attribute descriptor, a error #SNP_NOTIF_IND_NO_CCCD_ATTRIBUTE  will be return.

    Indication requires that the remote GATT client sends a confirmation. 
    Once the confirmation is received form the GATT client, the SNP_ sendNotiInd_rsp command will be send back by the SNP, with the status SUCCESS. 
    If the confirmation is not received from the GATT client within 30s, the SNP_ sendNotiInd_rsp will be send back by the SNP with the status FAILURE.
    If a timeout occurs while sending an indication, no more indication should be send while the connection is established.

    Notification does not require that the remote GATT clients send a confirmation. 
    The SNP_ sendNotiInd_rsp will be reply once the notification has been forwarded successfully to the BLE host.

    This SNP_sendNotifInd_req confirmation is mapped on the  #snpNotifIndReq_t structure.
    
    This SNP_sendNotifcnf_rsp  is mapped on the  #snpNotifIndCnf_t structure.
   
    The maximum size possible for a notification or indication value is ATT_MTU_SIZE. if the value send within the command is bigger, it will be truncated.
    
    Limitation:
     - Authentication is not supported .

  @section SNP_setGATTparam Set GATT parameters
    This API enables the update of parameters of predefined service.
    The device information service and the GAP/GATT service are predefined service always present in the NP.

    This SNP_setGATTparam_req request takes as parameter the structure #snpSetGattParamReq_t . 

    The SNP_setGATTparam_rsp response uses as parameter the structure #snpSetGattParamRsp_t .

    The list of parameters for the device information service can be found here: @ref SNP_DEV_INFO_SERVICE
    The list of parameters for the GAP/GATT service can be found here: @ref SNP_GAP_SERVICE

      
  @section SNP_getGATTparam Get GATT parameters
    This API enables the reading of parameters of predefined service.

    The SNP_getGATTparam_req request takes as parameter the structure #snpGetGattParamReq_t . 

    The SNP_getGATTparam_rsp response uses as parameter the structure #snpGetGattParamRsp_t .

      
  @section SNP_charConfigUpdated CCCD update indication
    The SNP_charConfigUpdated_req Event is sent by the SNP when a remote GATT client request to update the CCCD value(to enable notification or indication).
    The AP is responsible to remember the CCCD attribute for the GATT client if needed.
    The AP is responsible to accept (authorize) the modification of the CCCD.
    In the case notification has been enabled, the AP is able to notify or indicate the remote GATT client that the characteristic value has changed by calling API SNP_send_notiInd_req.
    Only one indication can be send at a time to a particular client. To send a new indication to the same client, the AP must wait the SNP_charConfigUpdated_rsp, see @ref SNP_charConfigUpdated.

    The snpCharWriteInd_t::rspNeeded field set to 1 indicates that the AP must send the SNP_charConfigUpdated_rap  back within 30s.                
    if the field is set to 0, the SNP_charConfigUpdated_rsp  is not needed. 
    This last case can happen if a ATT_WRITE_CMD operation has been used by the remote GATT client. If a confirmation is send anyway, it will be ignore by the NP.

    This SNP_charConfigUpdated_req request is mapped on the  #snpCharCfgUpdatedInd_t structure.
    
    This SNP_charConfigUpdated_rsp confirmation is mapped on the  #snpCharCfgUpdatedRsp_t structure.

*/
#ifndef SIMPLENP_GATT_H
#define SIMPLENP_GATT_H

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
 * TYPDEFS
 */
typedef struct snp_ATToperation
{
  uint8_t          validity;      //!< tell if an ATT operation is ongoing.
  uint8_t          method;        //!< method of the ongoing ATT operation.
  uint16_t         maxLen;        //!< max length of data for this ATT operation.  
  uint16_t         connHandle;    //!< handle of the connection the ATT operation is done for      
  gattAttribute_t  *pAttr;        //!< pointer to the attribute the ATT operation is perform on
  uint16_t         offset;        //!< offset of the value to perform the read/write operation
  uint8_t          *pValue;       //!< pointer to the data for the ATT operation
  uint8_t          cccdValue;     //!< the cccd value if the operation is a cccd update.
}snp_ATToperation_t;

/*********************************************************************
 * FUNCTIONS
 */


/**
 *  @fn          SNP_addService
 *
 * @brief        start the addition of a service, add the service attribute in the
 *               service creation queue.
 *
 *  @param[in]   addServiceCmd    : stucture of the service to add
 *  @param[out]  pRsp             : structure for the response
 *
 *  @return      status of the operation: 
 *               SPNP_SUCCESS, 
 *               SNP_OUT_OF_RESOURCES,
 *               SNP_INVALID_PARAMS
 */
uint8_t SNP_addService(snpAddServiceReq_t *pReq, uint8_t uuidLen, 
                       snpAddServiceRsp_t *pRsp); 

/**
 *  @fn          SNP_addCharValueDecl
 *
 * @brief        add a characteristic value and declaration. 
 *
 *  @param[in]   pReq  : parameter for the characteristic to add.
 *  @param[in]   uuidLen  : length of UUID field in the input structure.
 *  @param[out]  pRsp     : parameter to return to the AP
 *
 *  @return      status of the operation: 
 *               SPNP_SUCCESS, 
 *               SNP_OUT_OF_RESOURCES,
 *               SNP_INVALID_PARAMS
 */
uint8_t SNP_addCharValueDecl(snpAddCharValueDeclReq_t *pReq, uint8_t uuidLen,
                             snpAddCharValueDeclRsp_t *pRsp);

/**
 *  @fn          SNP_addDescriptionValue
 *
 * @brief        add a description attribute declaration to the attribute list. 
 *
 *  @param[in]   header    : header of the command.
 *  @param[in]   param     : pointer to the list of parameter in the command.
 *  @param[out]  nb_attrib : number of attribute added by this command.
 *  @param[out]  paramsRet : parameter to return to the AP.
 *
 *  @return      status of the operation: 
 *               SPNP_SUCCESS, 
 *               SNP_OUT_OF_RESOURCES,
 *               SNP_INVALID_PARAMS
 */
uint8_t SNP_addDescriptionValue(uint8_t header, uint8_t *pParam,
                                uint16_t *pNbAttrib, uint8_t *pParamsRet);

/**
 *  @fn           SNP_registerService
 *
 * @brief         end the service addition process and register the service with GATT 
 *
 *  @param[in]    None
 *  @param[out]   pRsp : parameter to return: start handle and end handle of 
 *                      the service.
 *
 *  @return       status of the operation: 
 *                SPNP_SUCCESS, 
 *                SNP_OUT_OF_RESOURCES,
 *                SNP_INVALID_PARAMS
 */
uint8_t SNP_registerService(snpRegisterServiceRsp_t *pRsp);

/**
 *  @fn           SNP_getAttrValue
 *
 * @brief         return an attribute value 
 *
 *  @param[in]    pReq : parameter structure of the request,see #snpGetAttrValueReq_t
 *  @param[out]   pRsp : structure for the response
 *  @param[out]   length : length of the data portion in the response
 *
 *  @return       status of the operation: 
 *                SPNP_SUCCESS, 
 *                SNP_UNKNOW_ATTRIBUTE
 */
uint8_t SNP_getAttrValue(snpGetAttrValueReq_t *pReq,
                         snpGetAttrValueRsp_t *pRsp, uint16_t *pDataLenght);

/**
 *  @fn           SNP_setAttrValue
 *
 * @brief         update an attribute value 
 *
 *  @param[in]    pReq : parameter structure of the request,see #snpSetAttrValueReq_t
 *  @param[in]    length : length of the data portion in the request
 *  @param[out]   pRsp : structure for the response
 *
 *  @return       status of the operation: 
 *                SPNP_SUCCESS, 
 *                SNP_CMD_REJECTED,
 *                SNP_INVALID_PARAMS
 */
uint8_t SNP_setAttrValue(snpSetAttrValueReq_t *pReq, uint16_t dataLenght, 
                         snpSetAttrValueRsp_t *pRsp);

/**
 *  @fn      SNP_setGATTParam
 *
 *  @brief   Set the value of some predefined service characteristic.
 *
 *  @param[in]   pReq  : structure of the request, see # snpSetGattParamReq_t
 *  @param[out]  pRsp  : structure of the response, see # snpSetGattParamRsp_t
 *
 *  @return  status of the command.
 *  @remark     Service ID are so far:
 *              GAP/GATT service        0x1.
 *              Device Info Service     0x2.
 *  
 */
uint8_t SNP_setGATTParam(snpSetGattParamReq_t *pReq, uint16_t reqDataLen, 
                         snpSetGattParamRsp_t *pRsp);

/**
 *  @fn          SNP_getGATTParam
 *
 *  @brief       Get the value of some prefined service characteristic.
 *
 *  @param[in]   pReq  : structure of the request, see # snpGetGattParamReq_t
 *  @param[out]  pRsp  : structure of the response, see # snpGetGattParamRsp_t
 *
 *  @return      status of the command.
 *
 *  @remark      Service ID are so far:
 *               GAP/GATT service        0x1.
 *               Device Info Service     0x2.
 *  
 */
uint8_t SNP_getGATTParam(snpGetGattParamReq_t *pReq,
                         snpGetGattParamRsp_t *pRsp, uint16_t *pDataLen);

/**
 *  @fn          SNP_sendNotifInd
 *
 *  @brief       Send notification
 *
 *  @param[in]   cmdStruct      :parameters for the command, # snpNotifIndReq_t
 *  @param[in]   length         :length of the data portion of the command
 *  @param[out]  None
 *
 *  @return      status of the command:
 *               SNP_SUCCESS
 *               ble_pending
 *               ble_timeout
 *               SNP_INVALID_PARAMS
 *               SNP_NOTIF_IND_NOT_ALLOWED
 *               SNP_NOTIF_IND_NOT_ENABLE_BY_CLIENT
 *               SNP_NOTIF_IND_NOT_ENABLE_BY_CLIENT
 *               SNP_NOTIF_IND_NO_CCCD_ATTRIBUTE
 *  
 */
uint8_t SNP_sendNotifInd(snpNotifIndReq_t *pCmdStruct, int16_t length);

/**
 *  @fn          SNP_writeCharCnf
 *
 *  @brief       process a 'Characteristic write confirmation' message.
 *
 *  @param[in]   pReq    : structure of the confirmation, see snpCharWriteCnf_t
 *  @param[out]  None
 *
 *  @return      SNP_SUCCESS.
 */
uint8_t SNP_writeCharCnf(snpCharWriteCnf_t *pReq);

/**
 *  @fn          SNP_readCharCnf
 *
 *  @brief       process a 'Characteristic read confirmation' message.
 *
 *  @param[in]   pReq     : structure of the read confirmation.
 *  @param[in]   dataSize : length of the data portion of the 
 *                             #snpCharReadCnf_t structure.
 *  @param[out]  None
 *
 *  @return      SNP_SUCCESS.
 */
uint8_t SNP_readCharCnf(snpCharReadCnf_t *pReq, uint16_t dataSize);


/**
 *  @fn          SNP_processCharConfigUpdatedCnf
 *
 *  @brief       process a 'Characteristic config update confirmation' message.
 *
 *  @param[in]   pReq    : structure of the confirmation, 
 *                           see snpCharCfgUpdatedRsp_t
 *  @param[out]  None
 *
 *  @return      SNP_SUCCESS.
 */
uint8_t SNP_processCharConfigUpdatedCnf(snpCharCfgUpdatedRsp_t *pReq);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SIMPLENP_GATT_H */
