/**************************************************************************************************
  Filename:       HostTestApp.c
  Revised:        $Date: 2015-05-21 14:50:07 -0700 (Thu, 21 May 2015) $
  Revision:       $Revision: 43900 $

  Description:    This file contains the HostTest sample application
                  for use with the CC2650 Bluetooth Low Energy Protocol Stack.

  Copyright 2013 - 2015 Texas Instruments Incorporated. All rights reserved.

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

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/drivers/LCD/LCDDogm1286.h>

#include <string.h>

#include "hal_types.h"
#include "hci_tl.h"
#include "hci_ext.h"
#include "gatt.h"
#include "hci.h"

#include "gapgattserver.h"
#include "gattservapp.h"

#include "gapbondmgr.h"
#include "osal_snv.h"
#include "ICallBleAPIMSG.h"

#include "util.h"
#include "board_lcd.h"

#include "inc/npi_ble.h"
#include "inc/npi_task.h"
#include <inc/hw_types.h>
#include "hal_defs.h"

#if defined ( GATT_TEST ) || defined ( GATT_QUAL )
  #include "gatttest.h"
#endif

#include "HostTestApp.h"

/*********************************************************************
 * CONSTANTS
 */

// LE Event Lengths
#define HCI_CMD_COMPLETE_EVENT_LEN              3
#define HCI_CMD_VS_COMPLETE_EVENT_LEN           2
#define HCI_CMD_STATUS_EVENT_LEN                4
#define HCI_PHY_UPDATE_COMPLETE_EVENT_LEN       6

// Task configuration
#define HTA_TASK_PRIORITY                       1
#define HTA_TASK_STACK_SIZE                     644

/*********************************************************************
 * LOCAL VARIABLES
 */
// Entity ID globally used to check for source and/or destination of messages
static ICall_EntityID selfEntity;

// Semaphore globally used to post events to the application thread
static ICall_Semaphore sem;

// Task configuration
Task_Struct htaTask;
Char htaTaskStack[HTA_TASK_STACK_SIZE];

#if !defined ( GATT_DB_OFF_CHIP )
  static uint8 deviceName[GAP_DEVICE_NAME_LEN] = { 0 };
  static uint16 appearance = 17;
#endif

// Stack build revision
ICall_BuildRevision buildRev;

/*********************************************************************
 * LOCAL FUNCTIONS
 */

static void HostTestApp_init(void);
static void HostTestApp_taskFxn(UArg a0, UArg a1);

static void HostTestApp_processGapEvent(ICall_HciExtEvt *pMsg);
static void HostTestApp_processBLEEvent(ICall_HciExtEvt *pMsg);

static void sendCommandCompleteEvent(uint8 eventCode, uint16 opcode,
                                     uint8 numParam, uint8 *param);
static void sendCommandStatusEvent(uint8_t eventCode, uint16_t status,
                                   uint16_t opcode);
static void sendBLECompleteEvent(uint8 eventLen, uint8 *pEvent);

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      HostTestApp_createTask
 *
 * @brief   Task creation function for the Host Test App.
 *
 * @param   none
 *
 * @return  none
 */
void HostTestApp_createTask(void)
{
  Task_Params taskParams;

  // Configure task
  Task_Params_init(&taskParams);
  taskParams.stack = htaTaskStack;
  taskParams.stackSize = HTA_TASK_STACK_SIZE;
  taskParams.priority = HTA_TASK_PRIORITY;

  Task_construct(&htaTask, HostTestApp_taskFxn, &taskParams, NULL);
}

/*********************************************************************
 * @fn      HostTestApp_init
 *
 * @brief   Called during initialization and contains application
 *          specific initialization (ie. hardware initialization/setup,
 *          table initialization, power up notification, etc), and
 *          profile initialization/setup.
 *
 * @param   none
 *
 * @return  none
 */
static void HostTestApp_init(void)
{
  // Register the current thread as an ICall dispatcher application
  // so that the application can send and receive messages.
  ICall_registerApp(&selfEntity, &sem);

  // Set device's Sleep Clock Accuracy
  //HCI_EXT_SetSCACmd(40);

  Board_openLCD();

  // Register for unprocessed HCI/Host event messages
  GAP_RegisterForMsgs(selfEntity);

  // Initialize GATT Client
  VOID GATT_InitClient();

  // Get build revision
  VOID Util_buildRevision(&buildRev);
  
#if !defined ( GATT_DB_OFF_CHIP )

  #if defined ( GATT_QUAL )
    VOID GATTQual_AddService( GATT_ALL_SERVICES ); // Includes GAP and GATT Services
  #else
    // Add our services to GATT Server
    VOID GGS_AddService( GATT_ALL_SERVICES );
    VOID GATTServApp_AddService( GATT_ALL_SERVICES );
    #if defined ( GATT_TEST )
      VOID GATTTest_AddService( GATT_ALL_SERVICES );
    #endif
  #endif

  // Set device name
  if ((buildRev.hostInfo & CENTRAL_CFG) && (buildRev.hostInfo & PERIPHERAL_CFG))
  {
    memcpy(deviceName, "TI BLE All", 10);
  }
  else if (buildRev.hostInfo & CENTRAL_CFG)
  {
    memcpy(deviceName, "TI BLE Central", 14);
  }
  else if (buildRev.hostInfo & PERIPHERAL_CFG)
  {
    memcpy(deviceName, "TI BLE Peripheral",  17);
  }
  else
  {
    memcpy(deviceName, "TI BLE Unknown",  14);
  }

  VOID GGS_SetParameter(GGS_DEVICE_NAME_ATT, strlen((char *)deviceName), deviceName);
  VOID GGS_SetParameter(GGS_APPEARANCE_ATT, sizeof(uint16), (void*)&appearance);

#endif // GATT_DB_OFF_CHIP

  LCD_WRITE_STRING("TI BLEv2.0", LCD_PAGE0);
  LCD_WRITE_STRING("HostTestApp", LCD_PAGE1);

  // Display Host build configuration
  if ((buildRev.hostInfo & CENTRAL_CFG) && (buildRev.hostInfo & PERIPHERAL_CFG))
  {
    LCD_WRITE_STRING("All", LCD_PAGE2);
  }
  else if ((buildRev.hostInfo & CENTRAL_CFG) && 
           (buildRev.hostInfo & BROADCASTER_CFG))
  {
    LCD_WRITE_STRING("Cent+Bcast", LCD_PAGE2);
  }
  else if ((buildRev.hostInfo & PERIPHERAL_CFG) && 
           (buildRev.hostInfo & OBSERVER_CFG))
  {
    LCD_WRITE_STRING("Peri+Observ", LCD_PAGE2);
  }
  else if (buildRev.hostInfo & CENTRAL_CFG)
  {
    LCD_WRITE_STRING("Central", LCD_PAGE2);
  }
  else if (buildRev.hostInfo & PERIPHERAL_CFG)
  {
    LCD_WRITE_STRING("Peripheral", LCD_PAGE2);
  }
  else
  {
    LCD_WRITE_STRING_VALUE("Unknown build cfg", buildRev.hostInfo, 10, LCD_PAGE2);
  }
}

/*********************************************************************
 * @fn      HostTestApp_taskFxn
 *
 * @brief   Application task entry point for the Host Test App.
 *
 * @param   none
 *
 * @return  none
 */
static void HostTestApp_taskFxn(UArg a0, UArg a1)
{
  // Initialize application
  HostTestApp_init();

  // Application main loop
  for (;;)
  {
    // Waits for a signal to the semaphore associated with the calling thread.
    // Note that the semaphore associated with a thread is signaled when a
    // message is queued to the message receive queue of the thread or when
    // ICall_signal() function is called onto the semaphore.
    ICall_Errno errno = ICall_wait(ICALL_TIMEOUT_FOREVER);

    if (errno == ICALL_ERRNO_SUCCESS)
    {
      ICall_EntityID dest;
      ICall_ServiceEnum src;
      ICall_HciExtEvt *pMsg = NULL;

      if (ICall_fetchServiceMsg(&src, &dest,
                                (void **)&pMsg) == ICALL_ERRNO_SUCCESS)
      {
        bool dealloc = true;

        if ((src == ICALL_SERVICE_CLASS_BLE) && (dest == selfEntity))
        {
          // Process incoming messages
          switch (pMsg->hdr.event)
          {
            case HCI_GAP_EVENT_EVENT:
              HostTestApp_processGapEvent(pMsg);
              break;
              
            default:
              break;
          }
        }

        if (dealloc == true)
        {
          ICall_freeMsg(pMsg);
        }
      }
    }
  }
}

/*********************************************************************
 * @fn      HostTestApp_processGapEvent
 *
 * @brief   Process an incoming GAP Event.
 *
 * @param   pMsg - message to process
 *
 * @return  none
 */
static void HostTestApp_processGapEvent(ICall_HciExtEvt *pMsg)
{
  switch(pMsg->hdr.status)
  {
    case HCI_COMMAND_COMPLETE_EVENT_CODE:
      {
        hciEvt_CmdComplete_t *pkt = (hciEvt_CmdComplete_t *)pMsg;

        if (lastAppOpcodeSent == pkt->cmdOpcode)
        {
          // app processes this as it was embedded msg to stack
          
          // Reset last opcode sent
          lastAppOpcodeSent = 0xFFFF;
        }
        else
        {
          osal_msg_hdr_t *msgHdr;
          uint8 len;

          msgHdr = (osal_msg_hdr_t *)pMsg;
          msgHdr--; // Backup to the msg header

          len = (uint8)(msgHdr->len - sizeof ( hciEvt_CmdComplete_t ));

          sendCommandCompleteEvent(HCI_COMMAND_COMPLETE_EVENT_CODE,
                                   pkt->cmdOpcode, len, pkt->pReturnParam);
        }
      }
      break;

    case HCI_DISCONNECTION_COMPLETE_EVENT_CODE:
      break;

    case HCI_COMMAND_STATUS_EVENT_CODE:
      {
        hciEvt_CommandStatus_t *pkt = (hciEvt_CommandStatus_t *)pMsg;
        
        if (lastAppOpcodeSent == pkt->cmdOpcode)
        {
          // app processes this as it was embedded msg to stack
          
          // Reset last opcode sent
          lastAppOpcodeSent = 0xFFFF;
        }
        else if (pkt->cmdOpcode == HCI_LE_SET_PHY)
        {
          sendCommandStatusEvent(HCI_COMMAND_STATUS_EVENT_CODE, pkt->cmdStatus,
                                 pkt->cmdOpcode);
        }
      }
      break;

    case HCI_LE_EVENT_CODE:
      HostTestApp_processBLEEvent(pMsg);
      break;
      
    case HCI_VE_EVENT_CODE:
      {
        hciEvt_VSCmdComplete_t *pkt = (hciEvt_VSCmdComplete_t *)pMsg;

        if (lastAppOpcodeSent == pkt->cmdOpcode)
        {
          // app processes this as it was embedded msg to stack
          
          // Reset last opcode sent
          lastAppOpcodeSent = 0xFFFF;
        }
        else
        {
          sendCommandCompleteEvent(HCI_VE_EVENT_CODE, pkt->cmdOpcode,
                                   pkt->length, pkt->pEventParam);
        }
      }
      break;

    default:
      break;
  }
}

/*********************************************************************
 * @fn      HostTestApp_processBLEEvent
 *
 * @brief   Process an incoming BLE Event.

 * @param   pMsg - message to process
 *
 * @return  None.
 */
static void HostTestApp_processBLEEvent(ICall_HciExtEvt *pMsg)
{
  hciEvt_BLEPhyUpdateComplete_t *pEvt = (hciEvt_BLEPhyUpdateComplete_t *)pMsg;
  uint8 event[HCI_PHY_UPDATE_COMPLETE_EVENT_LEN];
  uint8 eventLen;
     
  switch (pEvt->BLEEventCode)
  {
    case HCI_BLE_PHY_UPDATE_COMPLETE_EVENT:
      {
        event[0] = HCI_BLE_PHY_UPDATE_COMPLETE_EVENT; // event code
        event[1] = pEvt->status;                      // status
        event[2] = LO_UINT16(pEvt->connHandle);       // connection handle (LSB)
        event[3] = HI_UINT16(pEvt->connHandle);       // connection handle (MSB)
        event[4] = pEvt->txPhy;                       // TX PHY
        event[5] = pEvt->rxPhy;                       // RX PHY
        
        eventLen = HCI_PHY_UPDATE_COMPLETE_EVENT_LEN;
      }
      break;
      
    default:
      eventLen = 0;
      break;
  }
  
  if (eventLen > 0)
  {
    // Send BLE Complete Event
    sendBLECompleteEvent(eventLen, event);
  }
}

//*****************************************************************************
// the function prototypes

/*******************************************************************************
 * This generic function sends a Command Complete or a Vendor Specific Command
 * Complete Event to the Host.
 *
 */
static void sendCommandCompleteEvent(uint8_t eventCode, uint16_t opcode,
                                     uint8_t numParam, uint8_t *param)
{
  npiPkt_t *msg;
  uint8_t   totalLength;
  uint8_t   txLen = 0; // Length to transmit

  // The initial length will be:
  // OSAL message header(4) - not part of packet sent to HCI Host!
  // Minimum Event Data: Packet Type(1) + Event Code(1) + Length(1)
  // Return Parameters (0..N)
  totalLength = sizeof(npiPkt_t) + HCI_EVENT_MIN_LENGTH + numParam;

  // adjust the size of the event packet based on event code
  // Note: If not a vendor specific event, then the event includes:
  //       Command Complete Data: Number of HCI Commands Allowed(1) + Command Opcode(2)
  // Note: If a vendor specific event, then the event includes:
  //       Vendor Specific Command Complete Data: Vendor Specific Event Opcode(2)
  totalLength += ( (eventCode != HCI_VE_EVENT_CODE)  ?
                   HCI_CMD_COMPLETE_EVENT_LEN        :
                   HCI_CMD_VS_COMPLETE_EVENT_LEN );

  // allocate memory for OSAL hdr + packet
  msg = (npiPkt_t *)ICall_allocMsg(totalLength);
  if (msg)
  {
    // OSAL message event, status, and pointer to packet
    msg->hdr.event  = HCI_EVENT_PACKET;
    msg->hdr.status = 0xFF;
    msg->pData      = (uint8*)(msg+1);

    // fill in Command Complete Event data
    msg->pData[0] = HCI_EVENT_PACKET;
    msg->pData[1] = eventCode;

    txLen += 2;

    // check if this isn't a vendor specific event
    if ( eventCode != HCI_VE_EVENT_CODE )
    {
      msg->pData[2] = numParam + HCI_CMD_COMPLETE_EVENT_LEN;
      msg->pData[3] = 1;// hciCtrlCmdToken;     // event parameter 1
      msg->pData[4] = LO_UINT16( opcode ); // event parameter 2
      msg->pData[5] = HI_UINT16( opcode ); // event parameter 2

      txLen += 4;

      // remaining event parameters
      (void)memcpy(&msg->pData[6], param, numParam);

      txLen += numParam;
    }
    else // it is a vendor specific event
    {
      // less one byte as number of complete packets not used in vendor specific event
      msg->pData[2] = numParam + HCI_CMD_VS_COMPLETE_EVENT_LEN;
      msg->pData[3] = param[0];            // event parameter 0: event opcode LSB
      msg->pData[4] = param[1];            // event parameter 1: event opcode MSB
      msg->pData[5] = param[2];            // event parameter 2: status
      msg->pData[6] = LO_UINT16( opcode ); // event parameter 3: command opcode LSB
      msg->pData[7] = HI_UINT16( opcode ); // event parameter 3: command opcode MSB

      txLen += 6;

      // remaining event parameters
      // Note: The event opcode and status were already placed in the msg packet.
      (void)memcpy(&msg->pData[8], &param[3], numParam-HCI_EVENT_MIN_LENGTH);

      txLen += (numParam-HCI_EVENT_MIN_LENGTH);
    }

    msg->pktLen = txLen;

    NPITask_sendToHost((uint8_t *)msg);
  }
}

/*******************************************************************************
 * This generic function sends a Command Complete or a Vendor Specific Command
 * Complete Event to the Host.
 *
 */
static void sendCommandStatusEvent(uint8_t eventCode, uint16_t status,
                                   uint16_t opcode)
{
  npiPkt_t *msg;
  uint8_t   totalLength;

  // The initial length will be:
  // OSAL message header(4) - not part of packet sent to HCI Host!
  // Minimum Event Data: Packet Type(1) + Event Code(1) + Length(1)
  // Command Status Event Data: Status (1) + Num HCI Cmd Pkt (1) + Cmd Opcode (2)
  totalLength = sizeof(npiPkt_t)     + 
                HCI_EVENT_MIN_LENGTH + 
                HCI_CMD_STATUS_EVENT_LEN;

  // allocate memory for OSAL hdr + packet
  msg = (npiPkt_t *)ICall_allocMsg(totalLength);
  if (msg)
  {
    // OSAL message event, status, and pointer to packet
    msg->hdr.event  = HCI_EVENT_PACKET;
    msg->hdr.status = 0xFF;
    
    // fill in length and data pointer
    msg->pktLen = HCI_EVENT_MIN_LENGTH + HCI_CMD_STATUS_EVENT_LEN;
    msg->pData  = (uint8*)(msg+1);

    // fill in Command Complete Event data
    msg->pData[0] = HCI_EVENT_PACKET;
    msg->pData[1] = eventCode;
    msg->pData[2] = HCI_CMD_STATUS_EVENT_LEN;
    msg->pData[3] = status;
    msg->pData[4] = 1;                 // number of HCI command packets
    msg->pData[5] = LO_UINT16(opcode); // opcode (LSB)
    msg->pData[6] = HI_UINT16(opcode); // opcode (MSB)

    NPITask_sendToHost((uint8_t *)msg);
  }
}

/*******************************************************************************
 * This is a generic function used to send BLE Complete Event to the
 * Host processor.
 *
 */
static void sendBLECompleteEvent(uint8 eventLen, uint8 *pEvent)
{
  npiPkt_t *msg;
  uint8_t   totalLength;
  
  // The initial length will be:
  // OSAL message header(4) - not part of packet sent to HCI Host!
  // Minimum Event Data: Packet Type(1) + Event Code(1) + Length(1)
  // Event Data: eventLen
  totalLength = sizeof(npiPkt_t) + HCI_EVENT_MIN_LENGTH + eventLen;

  // allocate memory for OSAL hdr + packet
  msg = (npiPkt_t *)ICall_allocMsg(totalLength);
  if (msg)
  {
    // OSAL message event, status, and pointer to packet
    msg->hdr.event  = HCI_EVENT_PACKET;
    msg->hdr.status = 0xFF;
    
    // fill in length and data pointer
    msg->pktLen = HCI_EVENT_MIN_LENGTH + eventLen;
    msg->pData  = (uint8*)(msg+1);

    // fill in BLE Complete Event data
    msg->pData[0] = HCI_EVENT_PACKET;
    msg->pData[1] = HCI_LE_EVENT_CODE;
    msg->pData[2] = eventLen;
 
    // populate event data
    if (eventLen > 0)
    {
      memcpy(&msg->pData[3], pEvent, eventLen);
    }
      
    NPITask_sendToHost((uint8_t *)msg);
  }
}

/*********************************************************************
*********************************************************************/
