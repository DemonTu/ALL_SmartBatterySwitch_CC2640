/**
  @headerfile:    sap.h
  $Date: 2015-07-20 11:31:07 -0700 (Mon, 20 Jul 2015) $
  $Revision: 44370 $

  Description:    This file contains the Simple Application Processor API. 
                  SAP utilizes the SNP Remote Procedure call APIs to 
                  control an external BLE Network Processor

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

#ifndef SAP_H
#define SAP_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */

#include "SNP.h"
  
/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */

#define GAP_ADTYPE_FLAGS                        0x01 //!< Discovery Mode: @ref GAP_ADTYPE_FLAGS_MODES
#define GAP_ADTYPE_FLAGS_GENERAL                0x02 //!< Discovery Mode: LE General Discoverable Mode
#define GAP_ADTYPE_16BIT_MORE                   0x02 //!< Service: More 16-bit UUIDs available
#define GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED    0x04 //!< Discovery Mode: BR/EDR Not Supported
#define GAP_ADTYPE_LOCAL_NAME_COMPLETE          0x09 //!< Complete local name   
#define GAP_ADTYPE_SERVICE_DATA                 0x16 //!< Service Data - 16-bit UUID     
  
/* @defgroup SAP_DEFINES Constants and Structures
 * @{ 
 */
  
/** @defgroup SAP_PARAM_SUBSYSTEMS SAP Parameter Subsystems
 * @{
 */
#define SAP_PARAM_HCI             0x0001 //!< GAP HCI parameters: @ref SAP_GAP_HCI_PARAMS
#define SAP_PARAM_ADV             0x0002 //!< Advertising parameters: @ref SAP_ADV_PARAMS
#define SAP_PARAM_CONN            0x0003 //!< Connection parameters: @ref SAP_CONN_PARAMS
/** @} End SAP_PARAM_SUBSYSTEMS */


/** @defgroup SAP_ADV_PARAMS SAP Advertising Parameters
 * @{
 */
#define SAP_ADV_DATA_NOTCONN      0x0001 //!< Advertising Data when not in Connection
#define SAP_ADV_DATA_CONN         0x0002 //!< Advertising Data when in a Connection
#define SAP_ADV_DATA_SCANRSP      0x0003 //!< Scan Response data
#define SAP_ADV_STATE             0x0004 //!< Advertising state
/** @} End SAP_ADV_PARAMS */
  
/** @defgroup SAP_CONN_PARAMS SAP Connection Parameters
 * @{
 */
#define SAP_CONN_PARAM            0x0001 //!< Connection parameters
#define SAP_CONN_STATE            0x0002 //!< Connection state
/** @} End SAP_CONN_PARAMS */

/** @defgroup SAP_PORT_TYPES SAP port types
 * @{
 */
#define SAP_PORT_LOCAL                   0x00 //!< Locally running SAP (not supported)
#define SAP_PORT_REMOTE_UART             0x01 //!< Remote connection w/ SAP over UART
#define SAP_PORT_REMOTE_SPI              0x02 //!< Remote connection w/ SAP over SPI (not supported)
/** @} End SAP_PORT_TYPES */  
  
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */
   
/** @defgroup SAP_GAP_EVENT_CB SAP GAP Event Callback.
 * @{
 */
typedef void (*pfnEventCB_t)(uint16_t event, snpEventParam_t *param);

typedef void (*pfnAsyncCB_t)(uint8_t cmd1, void *pParams);
 
/** @} End SAP_GAP_EVENT_CB */

/** @defgroup SAP_GATT_READ_ATTR_CB SAP Write Attribue Callback.
 * @{
 */
typedef uint8_t (*pfnGATTReadAttrCB_t)(void *context, 
                                       uint16_t connectionHandle, 
                                       uint16_t charHdl, uint16_t offset, 
                                       uint16_t size, uint16_t * len, 
                                       uint8_t *pData);
/** @} End SAP_GATT_READ_ATTR_CB */

/** @defgroup SAP_GATT_WRITE_ATTR_CB SAP Write Attribute Callback.
 * @{
 */
typedef uint8_t (*pfnGATTWriteAttrCB_t)(void *context,
                                        uint16_t connectionHandle,
                                        uint16_t charHdl, uint16_t len,
                                        uint8_t *pData);
/** @} End SAP_GATT_WRITE_ATTR_CB */
 
/** @defgroup SAP_CCCD_Req_CB SAP CCCB Request Call back.
 * @{
 */
typedef uint8_t (*pfnCCCDIndCB_t)(void *context,
                                  uint16_t connectionHandle,
                                  uint16_t cccdHdl, uint8_t rspNeeded,
                                  uint16_t value);
/** @} End SAP_CCCD_REQ_CB */

//!< UUID type
typedef struct
{
  uint8_t len;    //!< Length of UUID buffer
  uint8_t *pUUID; //!< UUID buffer
} UUIDType_t;

//!< User Description Attribute type
typedef snpAddAttrCccd_t                SAP_UserCCCDAttr_t;
typedef snpAddAttrFormat_t              SAP_FormatAttr_t;
typedef snpAddAttrUserDesc_t            SAP_UserDescAttr_t;
typedef snpAddAttrGenShortUUID_t        SAP_ShortUUID_t;
typedef snpAddAttrGenLongUUID_t         SAP_LongUUID_t;        

// Characteristic handle type
typedef struct 
{
  uint16_t valueHandle;    //!< Handle of characteristic value
  uint16_t userDescHandle; //!< Handle of characteristic user description
  uint16_t cccdHandle;     //!< Handle of characteristic CCCD
  uint16_t formatHandle;   //!< Handle of characteristic format
  uint16_t sUUIDHandle;    //!< Handle of Generic Characteristic Short UUID
  uint16_t lUUIDHandle;    //!< Handle of Generic Characteristic Long UUID
} SAP_CharHandle_t;

//!< Characteristic Value Declration type.
typedef struct
{
  UUIDType_t         UUID;        //!< Identity of the characteristic
  uint8_t            properties;  //!< Characteristic value properties
  uint8_t            permissions; //!< Characteristic value permissions
  SAP_UserDescAttr_t *pUserDesc;  //!< User descriptor characteristic
  SAP_UserCCCDAttr_t *pCccd;      //!< User CCCD
  SAP_FormatAttr_t   *pFormat;    //!< User format.
  SAP_ShortUUID_t    *pShortUUID; //!< Generic Attribute Descriptor (Short UUID)
  SAP_LongUUID_t     *pLongUUID;  //!< Generic Attribute Descriport (Long UUID)
} SAP_Char_t;

/** @defgroup SAP_GATT_SERV_REG SAP GATT service registration parameter
 * @{
 */
typedef struct
{
  UUIDType_t           serviceUUID;       //!< Identity of the service
  uint8_t              serviceType;       //!< Primary, Secondary, Included.
  uint16_t             charTableLen;      //!< Length of custom characteristic array
  SAP_Char_t           *charTable;        //!< Array of custom characters to add to the service
  void*                context;           //!< The context is passed by the callbacks
  pfnGATTReadAttrCB_t  charReadCallback;  //!< Read callback function pointer: @ref SAP_GATT_WRITE_ATTR_CB
  pfnGATTWriteAttrCB_t charWriteCallback; //!< Write callback function pointer: @ref SAP_GATT_WRITE_ATTR_CB
  pfnCCCDIndCB_t       cccdIndCallback;   //!< Write callback function pointer: @ref SAP_CCCB_REQ_CB
  uint16_t             serviceHandle;     //!< Handle of the service characteristic
  SAP_CharHandle_t     *charAttrHandles;  //!< Array of handles for the characteristics
} SAP_Service_t;
/** @} End SAP_GATT_SERV_REG */


//!< Port structure when running a local version of SAP (not currently supported)
typedef struct
{
  uint8_t              reserved;
} SAP_LocalPort_t;

//!< Port structure when using a remote SNP
typedef struct
{
  uint16_t             stackSize;      //!< Configurable size of stack for SAP Port
  uint16_t             bufSize;        //!< Size of Tx/Rx Transport layer buffers
  uint32_t             mrdyPinID;      //!< Pin ID Mrdy (only with Power Saving enabled)
  uint32_t             srdyPinID;      //!< Pin ID Srdy (only with Power Saving enabled)
  uint8_t              boardID;        //!< Board ID for physical port, i.e. CC2650_UART0
  uint32_t             bitRate;        //!< Baud/Bit Rate for physical port
} SAP_RemotePort_t;

//!< SAP Parameters
typedef struct
{
  uint8_t               portType;
  union 
  {
    SAP_LocalPort_t      local;
    SAP_RemotePort_t     remote;     
  } port;
} SAP_Params;

/** @} End SAP_DEFINES */

/*********************************************************************
 * FUNCTIONS
 */

/**
 * @defgroup SAP_API High Level SAP API Functions
 *
 * @{
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
extern void SAP_initParams(uint8_t portType, SAP_Params *params);

/**
 * @fn      SAP_open
 *
 * @brief   Opens serial port with the network processor
 *
 * @param   params - list of parameters needed to initialize serial port used
 *                   with network processor
 *
 * @return  uint8_t - SNP_SUCCESS if NPI is open
 */
extern uint8_t SAP_open(SAP_Params *params);

/**
 * @fn      SAP_close
 *
 * @brief   Tears down serial port with the network processor
 *
 * @param   None.
 *
 * @return  uint8_t - SNP_SUCCESS if able to close
 */
extern uint8_t SAP_close(void);

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
extern uint8_t SAP_setAsyncCB(pfnAsyncCB_t asyncCB);

/**
 * @brief       Initialize the SAP module
 *
 * @param       None.
 *
 * @return      SNP_SUCCESS: the device correctly initialized.<BR>
 *              SAP_FAILRUE: the device failed to initialize.<BR>
 */
extern uint8_t SAP_initDevice(void);

/**
 * @brief       Reset the SAP and return only when it has restarted.
 *
 * @param       none
 *
 * @return      SNP_SUCCESS: the device correctly reset.<BR>
 */
extern uint8_t SAP_reset(void);

/**
 * @brief       Register a GATT service with the GATT Server.
 *
 * @param       serviceParams - a pointer to service data: @ref SAP_GATT_SERV_REG
 *                              It is the callers responsibility to allocate 
 *                              this structure.
 *
 * @return      SNP_SUCCESS: the service is registered.<BR>
 *              SAP_FAILRUE: service registration failed.<BR>
 */
extern uint8_t SAP_registerService(SAP_Service_t *serviceParams);

/**
 * @brief       Register a callback to receive a GAP event.  This shall be
 *              called once for each callback to be registered.
 *              
 * @param       eventCB   - a Callback function to register: @ref SAP_GAP_EVENT_CB
 * @param       eventMask - the mask of events which trigger this 
 *                          callback. Events types: @ref SAP_GAP_EVENTS
 *
 * @return      SNP_SUCCESS: the device correctly reset.<BR>
 *              SAP_FAILRUE: callback registration failed.<BR>
 */
extern uint8_t SAP_registerEventCB(pfnEventCB_t eventCB, uint16_t eventMask);

/**
 * @brief       Read a characteristic value of a service.
 *
 * @param       serviceID - the UUID of the service
 * @param       charID    - the unique handle of the characteristic
 * @param       len       - length of the data read
 * @param       pData     - pointer to buffer to write to
 *
 * @return      SNP_SUCCESS: the read completed successfully.<BR>
 *              SAP_FAILRUE: service param request failed.<BR>
 */
extern uint8_t SAP_getServiceParam(uint8_t serviceID, uint8_t charID, 
                                   uint16_t *len, uint8_t *pData);

/**
 * @brief       Write a characteristic value of a service.
 *
 * @param       serviceID - the UUID of the service
 * @param       charID    - the unique handle of the characteristic
 * @param       len       - length of the data to write
 * @param       pData     - pointer to buffer of data to write
 *
 * @return      SNP_SUCCESS: the write completed successfully.<BR>
 *              SAP_FAILRUE: service param write request failed.<BR>
 */
extern uint8_t SAP_setServiceParam(uint8_t serviceID, uint8_t charID, 
                                   uint16_t len, uint8_t *pData);

/**
 * @brief       Write to a stack parameter on the SAP. Some responses will
 *              Return immediately, others will generate an event for which
 *              a callback must be registered with the correct event mask.
 *
 * @param       subsystemID  - the subsystem ID: @ref SAP_PARAM_SUBSYSTEMS
 * @param       paramID     - the parameter within the subsystem to write
 * @param       len         - length of the data to write
 * @param       pData       - pointer to buffer of data to write
 *
 * @return      SNP_SUCCESS: the write completed successfully.<BR>
 *              SAP_FAILRUE: stack parameter write failed.<BR>
 */
extern uint8_t SAP_setParam(uint8_t subsystemID, uint16_t paramID, 
                            uint16_t len, uint8_t *pData);

/**
 * @brief       Read a stack parameter on the SAP. Some responses will
 *              Return immediately, others will generate an event for which
 *              a callback must be registered with the correct event mask.
 *
 * @param       subsystemID - the subsystem ID: @ref SAP_PARAM_SUBSYSTEMS
 * @param       paramID    - the parameter within the subsystem to read
 * @param       len        - length of the data to read
 * @param       pData      - pointer to buffer to write to
 *
 * @return      SNP_SUCCESS: the read completed successfully.<BR>
 *              SAP_FAILRUE: stack param read failed.<BR>
 */
extern uint8_t SAP_getParam(uint8_t subsystemID, uint8_t paramID,
                            uint16_t len, uint8_t *pData);


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* SAP_H */
