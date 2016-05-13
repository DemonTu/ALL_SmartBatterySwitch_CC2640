/*
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *    ======== USBCDCMOUSE.c ========
 */

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/gates/GateMutex.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <stdbool.h>
#include <stdint.h>

/* driverlib Header files */
#include <inc/hw_ints.h>
#include <inc/hw_types.h>

/* usblib Header files */
#include <usblib/usb-ids.h>
#include <usblib/usblib.h>
#include <usblib/usbcdc.h>
#include <usblib/usbhid.h>
#include <usblib/device/usbdevice.h>
#include <usblib/device/usbdcomp.h>
#include <usblib/device/usbdcdc.h>
#include <usblib/device/usbdhid.h>
#include <usblib/device/usbdhidmouse.h>

/* Example/Board Header files */
#include "USBCDCMOUSE.h"

#if defined(TIVAWARE)
typedef uint32_t            USBCDCMOUSEEventType;
#else
#define eUSBModeForceDevice USB_MODE_FORCE_DEVICE
typedef unsigned long       USBCDCMOUSEEventType;
#endif

/* Defines */
#define NUM_DEVICES         2   /* Number of Composite devices */
#define HCDMEMORYPOOLSIZE   128 /* Memory for the Host Class Driver */
#define MMEMORYPOOLSIZE     128 /* Memory for the mouse host driver */
#define USBBUFFERSIZE       256 /* Buffer size for the USB Buffers */
#define DESCRIPTORDATASIZE  (COMPOSITE_DCDC_SIZE + COMPOSITE_DHID_SIZE)

/* Typedefs */
typedef volatile enum {
    USBCDCMOUSE_CONNECTED = 0,
    USBCDCMOUSE_DISCONNECTED
} USBCDCMOUSE_USBState;

typedef volatile enum {
    USBCDCMOUSE_CDC_IDLE = 0,
    USBCDCMOUSE_CDC_INIT,
    USBCDCMOUSE_CDC_UNCONFIGURED,
} USBCDCMOUSE_USBCDCState;

typedef volatile enum {
    USBCDCMOUSE_M_IDLE = 0,
    USBCDCMOUSE_M_SENDING,
    USBCDCMOUSE_M_UNCONFIGURED
} USBCDCMOUSE_USBMState;

/* Static variables and handles */
static USBCDCMOUSE_USBCDCState  stateCDC;
static USBCDCMOUSE_USBMState    stateM;
static USBCDCMOUSE_USBState     stateUSB;
static unsigned char            receiveBuffer[USBBUFFERSIZE];
static unsigned char            receiveBufferWorkspace[USB_BUFFER_WORKSPACE_SIZE];
static unsigned char            transmitBuffer[USBBUFFERSIZE];
static unsigned char            transmitBufferWorkspace[USB_BUFFER_WORKSPACE_SIZE];
static unsigned char            descriptorData[DESCRIPTORDATASIZE];

const  tUSBBuffer               txBuffer;
const  tUSBBuffer               rxBuffer;
static GateMutex_Handle         gateTxSerial;
static GateMutex_Handle         gateRxSerial;
static GateMutex_Handle         gateMouse;
static GateMutex_Handle         gateUSBWait;
static Semaphore_Handle         semTxSerial;
static Semaphore_Handle         semRxSerial;
static Semaphore_Handle         semMouse;
static Semaphore_Handle         semUSBConnected;

/* Function prototypes */
static USBCDCMOUSEEventType cbCompositeHandler(void *cbData, USBCDCMOUSEEventType event,
                                USBCDCMOUSEEventType eventMsg, void *eventMsgPtr);
static USBCDCMOUSEEventType cbMouseHandler(void *cbData, USBCDCMOUSEEventType event,
                                USBCDCMOUSEEventType eventMsg, void *eventMsgPtr);
static USBCDCMOUSEEventType cbRxHandler(void *cbData, USBCDCMOUSEEventType event,
                                USBCDCMOUSEEventType eventMsg, void *eventMsgPtr);
static USBCDCMOUSEEventType cbSerialHandler(void *cbData, USBCDCMOUSEEventType event,
                                USBCDCMOUSEEventType eventMsg, void *eventMsgPtr);
static USBCDCMOUSEEventType cbTxHandler(void *cbData, USBCDCMOUSEEventType event,
                                USBCDCMOUSEEventType eventMsg, void *eventMsgPtr);
static Void  USBCDCMOUSE_hwiHandler(UArg arg0);
static unsigned int  rxData(unsigned char *pStr,
                            unsigned int length,
                            unsigned int timeout);
static int           sendState(USBCDCMOUSE_State *mouseState,
                               unsigned int timeout);
static unsigned int  txData(const unsigned char *pStr,
                            int length,
                            unsigned int timeout);
static bool          waitUntilSent(unsigned int timeout);
void                 USBCDCMOUSE_init(void);
unsigned int         USBCDCMOUSE_receiveData(unsigned char *pStr,
                                             unsigned int length,
                                             unsigned int timeout);
unsigned int         USBCDCMOUSE_sendData(const unsigned char *pStr,
                                          unsigned int length,
                                          unsigned int timeout);
unsigned int         USBCDCMOUSE_setState(USBCDCMOUSE_State *mouseState,
                                          unsigned int timeout);
bool                 USBCDCMOUSE_waitForConnect(unsigned int timeout);

/* The languages supported by this device. */
const unsigned char langDescriptor[] =
{
    4,
    USB_DTYPE_STRING,
    USBShort(USB_LANG_EN_US)
};

/* The manufacturer string. */
const unsigned char manufacturerString[] =
{
    (17 + 1) * 2,
    USB_DTYPE_STRING,
    'T', 0, 'e', 0, 'x', 0, 'a', 0, 's', 0, ' ', 0, 'I', 0, 'n', 0, 's', 0,
    't', 0, 'r', 0, 'u', 0, 'm', 0, 'e', 0, 'n', 0, 't', 0, 's', 0,
};

/* The product string. */
const unsigned char productString[] =
{
    (42 + 1) * 2,
    USB_DTYPE_STRING,
    'C', 0, 'o', 0, 'm', 0, 'p', 0, 'o', 0, 's', 0, 'i', 0, 't', 0,
    'e', 0, ' ', 0, 'H', 0, 'I', 0, 'D', 0, ' ', 0, 'M', 0, 'o', 0,
    'u', 0, 's', 0, 'e', 0, ' ', 0, 'a', 0, 'n', 0, 'd', 0, ' ', 0,
    'C', 0, 'D', 0, 'C', 0, ' ', 0, 'S', 0, 'e', 0, 'r', 0, 'i', 0,
    'a', 0, 'l', 0, ' ', 0, 'E', 0, 'x', 0, 'a', 0, 'm', 0, 'p', 0,
    'l', 0, 'e', 0,
};

/* The serial number string. */
const unsigned char serialNumberString[] =
{
    (8 + 1) * 2,
    USB_DTYPE_STRING,
    '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0, '8', 0
};

/* The descriptor string table. */
const unsigned char * const stringDescriptors[] =
{
    langDescriptor,
    manufacturerString,
    productString,
    serialNumberString,
};

#define STRINGDESCRIPTORSCOUNT (sizeof(stringDescriptors) / \
                                sizeof(unsigned char *))

#if defined(TIVAWARE)
static tUSBDHIDMouseDevice mouseDevice =
{
    USB_VID_TI_1CBE,
    USB_PID_MOUSE,
    250,
    USB_CONF_ATTR_BUS_PWR,

    cbMouseHandler,
    NULL,

    0,
    0,
};

static tUSBDCDCDevice serialDevice =
{
    USB_VID_TI_1CBE,
    USB_PID_SERIAL,
    250,
    USB_CONF_ATTR_BUS_PWR,

    cbSerialHandler,
    NULL,

    USBBufferEventCallback,
    (void *)&rxBuffer,

    USBBufferEventCallback,
    (void *)&txBuffer,

    0,
    0,
};
#else /* MWARE */
static tHIDMouseInstance    mouseInstance;
const tUSBDHIDMouseDevice mouseDevice =
{
    USB_VID_TI,
    USB_PID_MOUSE,
    250,
    USB_CONF_ATTR_BUS_PWR,

    cbMouseHandler,
    NULL,

    0,
    0,

    &mouseInstance /* Old usblib stores a pointer */
};

static tCDCSerInstance    serialInstance;
const tUSBDCDCDevice serialDevice =
{
    USB_VID_TI,
    USB_PID_SERIAL,
    250,
    USB_CONF_ATTR_BUS_PWR,

    cbSerialHandler,
    NULL,

    USBBufferEventCallback,
    (void *)&rxBuffer,

    USBBufferEventCallback,
    (void *)&txBuffer,

    0,
    0,

    &serialInstance /* Old usblib stores a pointer */
};
#endif

const tUSBBuffer rxBuffer =
{
    false,                      /* This is a receive buffer. */
    cbRxHandler,                /* pfnCallback */
    (void *)&serialDevice,      /* Callback data is our device pointer. */
    USBDCDCPacketRead,          /* pfnTransfer */
    USBDCDCRxPacketAvailable,   /* pfnAvailable */
    (void *)&serialDevice,      /* pvHandle */
    receiveBuffer,              /* pcBuffer */
    USBBUFFERSIZE,              /* ulBufferSize */
    receiveBufferWorkspace      /* pvWorkspace */
};

const tUSBBuffer txBuffer =
{
    true,                       /* This is a transmit buffer. */
    cbTxHandler,                /* pfnCallback */
    (void *)&serialDevice,      /* Callback data is our device pointer. */
    USBDCDCPacketWrite,         /* pfnTransfer */
    USBDCDCTxPacketAvailable,   /* pfnAvailable */
    (void *)&serialDevice,      /* pvHandle */
    transmitBuffer,             /* pcBuffer */
    USBBUFFERSIZE,              /* ulBufferSize */
    transmitBufferWorkspace     /* pvWorkspace */
};

static tLineCoding g_sLineCoding=
{
    115200,                     /* 115200 baud rate. */
    1,                          /* 1 Stop Bit. */
    0,                          /* No Parity. */
    8                           /* 8 Bits of data. */
};

#if defined(TIVAWARE)
static tCompositeEntry compositeDevices[NUM_DEVICES];

static tUSBDCompositeDevice compositeDevice =
{
    USB_VID_TI_1CBE,
    USB_PID_COMP_HID_SER,
    250,
    USB_CONF_ATTR_BUS_PWR,

    cbCompositeHandler,

    stringDescriptors,
    STRINGDESCRIPTORSCOUNT,

    NUM_DEVICES,
    compositeDevices
};

#else /* MWARE */
static tCompositeEntry compositeDevices[NUM_DEVICES] =
{
    /* HID Mouse Information */
    { &g_sHIDDeviceInfo, 0 },

    /* Serial Device Information */
    { &g_sCDCSerDeviceInfo, 0 }
};

static unsigned long            compositeWorkspace[NUM_DEVICES];
static tCompositeInstance       compositeInstance;
const tUSBDCompositeDevice compositeDevice =
{
    USB_VID_TI,
    USB_PID_COMP_HID_SER,
    250,
    USB_CONF_ATTR_BUS_PWR,

    cbCompositeHandler,

    stringDescriptors,
    STRINGDESCRIPTORSCOUNT,

    NUM_DEVICES,
    compositeDevices,

    compositeWorkspace,
    &compositeInstance
};
#endif

/*
 *  ======== cbCompositeHandler ========
 *  Callback handler for the USB stack.
 *
 *  Callback handler call by the USB stack to notify us on what has happened in
 *  regards to the keyboard.
 *
 *  @param(cbData)          A callback pointer provided by the client.
 *
 *  @param(event)           Identifies the event that occurred in regards to
 *                          this device.
 *
 *  @param(eventMsgData)    A data value associated with a particular event.
 *
 *  @param(eventMsgPtr)     A data pointer associated with a particular event.
 *
 */
static USBCDCMOUSEEventType cbCompositeHandler(void *cbData, USBCDCMOUSEEventType event,
                                USBCDCMOUSEEventType eventMsg, void *eventMsgPtr)
{
    /* Determine what event has happened */
    switch (event) {
        case USB_EVENT_CONNECTED:
            if (stateUSB != USBCDCMOUSE_CONNECTED) {
                stateUSB = USBCDCMOUSE_CONNECTED;
                Semaphore_post(semUSBConnected);
            }
            break;

        case USB_EVENT_DISCONNECTED:
            stateUSB = USBCDCMOUSE_DISCONNECTED;
            break;

        default:
            break;
    }

    return (0);
}

/*
 *  ======== cbMouseHandler ========
 *  Callback handler for the USB stack.
 *
 *  Callback handler call by the USB stack to notify us on what has happened in
 *  regards to the keyboard.
 *
 *  @param(cbData)          A callback pointer provided by the client.
 *
 *  @param(event)           Identifies the event that occurred in regards to
 *                          this device.
 *
 *  @param(eventMsgData)    A data value associated with a particular event.
 *
 *  @param(eventMsgPtr)     A data pointer associated with a particular event.
 *
 */
static USBCDCMOUSEEventType cbMouseHandler(void *cbData, USBCDCMOUSEEventType event,
                            USBCDCMOUSEEventType eventMsg, void *eventMsgPtr)
{
    /* Determine what event has happened */
    switch (event) {
        case USB_EVENT_CONNECTED:
            /* Handled by the cbCompositeHandler callback */
            stateM = USBCDCMOUSE_M_IDLE;
            break;

        case USB_EVENT_DISCONNECTED:
            if (stateM == USBCDCMOUSE_M_SENDING) {
                stateM = USBCDCMOUSE_M_UNCONFIGURED;
                Semaphore_post(semMouse);
            }
            else {
                stateM = USBCDCMOUSE_M_UNCONFIGURED;
            }
            break;

        case USB_EVENT_TX_COMPLETE:
            stateM = USBCDCMOUSE_M_IDLE;
            Semaphore_post(semMouse);
            break;

        default:
            break;
    }

    return (0);
}

/*
 *  ======== cbRxHandler ========
 *  Callback handler for the USB stack.
 *
 *  Callback handler call by the USB stack to notify us on what has happened in
 *  regards to the keyboard.
 *
 *  @param(cbData)          A callback pointer provided by the client.
 *
 *  @param(event)           Identifies the event that occurred in regards to
 *                          this device.
 *
 *  @param(eventMsgData)    A data value associated with a particular event.
 *
 *  @param(eventMsgPtr)     A data pointer associated with a particular event.
 *
 */
static USBCDCMOUSEEventType cbRxHandler(void *cbData, USBCDCMOUSEEventType event,
                         USBCDCMOUSEEventType eventMsg, void *eventMsgPtr)
{
    switch (event) {
        case USB_EVENT_RX_AVAILABLE:
            Semaphore_post(semRxSerial);
            break;

        case USB_EVENT_DATA_REMAINING:
            break;

        case USB_EVENT_REQUEST_BUFFER:
            break;

        default:
            break;
    }

    return (0);
}

/*
 *  ======== cbSerialHandler ========
 *  Callback handler for the USB stack.
 *
 *  Callback handler call by the USB stack to notify us on what has happened in
 *  regards to the keyboard.
 *
 *  @param(cbData)          A callback pointer provided by the client.
 *
 *  @param(event)           Identifies the event that occurred in regards to
 *                          this device.
 *
 *  @param(eventMsgData)    A data value associated with a particular event.
 *
 *  @param(eventMsgPtr)     A data pointer associated with a particular event.
 *
 */
static USBCDCMOUSEEventType cbSerialHandler(void *cbData, USBCDCMOUSEEventType event,
                             USBCDCMOUSEEventType eventMsg, void *eventMsgPtr)
{
    tLineCoding *psLineCoding;

    /* Determine what event has happened */
    switch (event) {
        case USB_EVENT_CONNECTED:
            stateCDC = USBCDCMOUSE_CDC_INIT;
            break;

        case USB_EVENT_DISCONNECTED:
            stateCDC = USBCDCMOUSE_CDC_UNCONFIGURED;
            break;

        case USBD_CDC_EVENT_GET_LINE_CODING:
            /* Create a pointer to the line coding information. */
            psLineCoding = (tLineCoding *)eventMsgPtr;

            /* Copy the current line coding information into the structure. */
            *(psLineCoding) = g_sLineCoding;
            break;

        case USBD_CDC_EVENT_SET_LINE_CODING:
            /* Create a pointer to the line coding information. */
            psLineCoding = (tLineCoding *)eventMsgPtr;

            /*
             * Copy the line coding information into the current line coding
             * structure.
             */
            g_sLineCoding = *(psLineCoding);
            break;

        case USBD_CDC_EVENT_SET_CONTROL_LINE_STATE:
            break;

        case USBD_CDC_EVENT_SEND_BREAK:
            break;

        case USBD_CDC_EVENT_CLEAR_BREAK:
            break;

        case USB_EVENT_SUSPEND:
            break;

        case USB_EVENT_RESUME:
            break;

        default:
            break;
    }

    return (0);
}

/*
 *  ======== cbTxHandler ========
 *  Callback handler for the USB stack.
 *
 *  Callback handler call by the USB stack to notify us on what has happened in
 *  regards to the keyboard.
 *
 *  @param(cbData)          A callback pointer provided by the client.
 *
 *  @param(event)           Identifies the event that occurred in regards to
 *                          this device.
 *
 *  @param(eventMsgData)    A data value associated with a particular event.
 *
 *  @param(eventMsgPtr)     A data pointer associated with a particular event.
 *
 */
static USBCDCMOUSEEventType cbTxHandler(void *cbData, USBCDCMOUSEEventType event,
                         USBCDCMOUSEEventType eventMsg, void *eventMsgPtr)
{
    switch (event) {
        case USB_EVENT_TX_COMPLETE:
            /*
             * Data was sent, so there should be some space available on the
             * buffer
             */
            Semaphore_post(semTxSerial);
            break;

        default:
            break;
    }

    return (0);
}

/*
 *  ======== USBCDCMOUSE_hwiHandler ========
 *  This function calls the USB library's device interrupt handler.
 */
static Void USBCDCMOUSE_hwiHandler(UArg arg0)
{
    USB0DeviceIntHandler();
}

/*
 *  ======== rxData ========
 */
static unsigned int rxData(unsigned char *pStr,
                           unsigned int length,
                           unsigned int timeout)
{
    unsigned int read = 0;

    if (USBBufferDataAvailable(&rxBuffer) || Semaphore_pend(semRxSerial, timeout)) {
        read = USBBufferRead(&rxBuffer, pStr, length);
    }

    return (read);
}

/*
 *  ======== sendState ========
 */
static int sendState(USBCDCMOUSE_State *mouseState, unsigned int timeout)
{
    unsigned int key;
    int retValue = 0;
    unsigned char buttons = 0;

    /* Set the bit packed button values */
    buttons |= (mouseState->button1) ? HID_MOUSE_BUTTON_1 : 0;
    buttons |= (mouseState->button2) ? HID_MOUSE_BUTTON_2 : 0;
    buttons |= (mouseState->button3) ? HID_MOUSE_BUTTON_3 : 0;

    key = GateMutex_enter(gateMouse);

    stateM = USBCDCMOUSE_M_SENDING;
    retValue = USBDHIDMouseStateChange((tUSBDHIDMouseDevice *)&mouseDevice,
                                        mouseState->deltaX,
                                        mouseState->deltaY,
                                        buttons);

    if (!retValue) {
        retValue = !waitUntilSent(timeout);
    }

    GateMutex_leave(gateMouse, key);

    return (retValue);
}

/*
 *  ======== txData ========
 */
static unsigned int txData(const unsigned char *pStr,
                           int length,
                           unsigned int timeout)
{
    unsigned int buffAvailSize;
    unsigned int bufferedCount = 0;
    unsigned int sendCount = 0;
    unsigned char *sendPtr;

    while (bufferedCount != length) {
        /* Determine the buffer size available */
        buffAvailSize = USBBufferSpaceAvailable(&txBuffer);

        /* Determine how much needs to be sent */
        if ((length - bufferedCount) > buffAvailSize) {
            sendCount = buffAvailSize;
        }
        else {
            sendCount = length - bufferedCount;
        }

        /* Adjust the pointer to the data */
        sendPtr = (unsigned char *)pStr + bufferedCount;

        /* Place the contents into the USB BUffer */
        bufferedCount += USBBufferWrite(&txBuffer, sendPtr, sendCount);

        /* Pend until some data was sent through the USB*/
        if (!Semaphore_pend(semTxSerial, timeout)) {
            break;
        }
    }

    return (bufferedCount);
}

/*
 *  ======== waitUntilSent ========
 *  Function will determine if the last key press/release was sent to the host
 *
 *  @return             0: Assume that there was an error (likely a disconnect)
 *                      1: Successful
 */
static bool waitUntilSent(unsigned int timeout)
{

    if (!Semaphore_pend(semMouse, timeout)) {
        stateM = USBCDCMOUSE_M_UNCONFIGURED;
        return (false);
    }

    return (true);
}

/*
 *  ======== USBCDCMOUSE_init ========
 */
void USBCDCMOUSE_init(void)
{
    Hwi_Handle hwi;
    Error_Block eb;
    Semaphore_Params semParams;

    Error_init(&eb);

    /* Install interrupt handler */
    hwi = Hwi_create(INT_USB0, USBCDCMOUSE_hwiHandler, NULL, &eb);
    if (hwi == NULL) {
        System_abort("Can't create USB Hwi");
    }

    /* RTOS primitives */
    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    semTxSerial = Semaphore_create(0, &semParams, &eb);
    if (semTxSerial == NULL) {
        System_abort("Can't create TX semaphore");
    }

    semRxSerial = Semaphore_create(0, &semParams, &eb);
    if (semRxSerial == NULL) {
        System_abort("Can't create RX semaphore");
    }

    semMouse = Semaphore_create(0, &semParams, &eb);
    if (semMouse == NULL) {
        System_abort("Can't create mouse semaphore");
    }

    semUSBConnected = Semaphore_create(0, &semParams, &eb);
    if (semUSBConnected == NULL) {
        System_abort("Can't create USB semaphore");
    }

    gateTxSerial = GateMutex_create(NULL, &eb);
    if (gateTxSerial == NULL) {
        System_abort("Can't create gate");
    }

    gateRxSerial = GateMutex_create(NULL, &eb);
    if (gateRxSerial == NULL) {
        System_abort("Can't create gate");
    }

    gateMouse = GateMutex_create(NULL, &eb);
    if (gateMouse == NULL) {
        System_abort("Can't create mouse gate");
    }

    gateUSBWait = GateMutex_create(NULL, &eb);
    if (gateUSBWait == NULL) {
        System_abort("Could not create USB Wait gate");
    }

    /* State specific variables */
    stateUSB = USBCDCMOUSE_DISCONNECTED;
    stateCDC = USBCDCMOUSE_CDC_UNCONFIGURED;
    stateM   = USBCDCMOUSE_M_UNCONFIGURED;

    /* Set the USB stack mode to Device mode with VBUS monitoring */
    USBStackModeSet(0, eUSBModeForceDevice, 0);

    /* Install the composite instances */
#if defined(TIVAWARE)
    if (!USBDHIDMouseCompositeInit(0, &mouseDevice, &compositeDevices[0])) {
        System_abort("Can't initialize HID Mouse composite component");
    }

    if (!USBDCDCCompositeInit(0, &serialDevice, &compositeDevices[1])) {
        System_abort("Can't initialize CDC composite component");
    }

#else /* MWARE */
    compositeDevices[0].pvInstance = USBDHIDMouseCompositeInit(0, &mouseDevice);
    compositeDevices[1].pvInstance = USBDCDCCompositeInit(0, &serialDevice);
#endif

    if (!compositeDevices[0].pvInstance || !compositeDevices[1].pvInstance){
        System_abort("Error initializing the composite device");
    }

    /* Initialize the USB stack with the composite device */
    USBDCompositeInit(0, (tUSBDCompositeDevice *) &compositeDevice,
                      DESCRIPTORDATASIZE, descriptorData);
}

/*
 *  ======== USBCDCMOUSE_receiveData ========
 */
unsigned int USBCDCMOUSE_receiveData(unsigned char *pStr,
                                     unsigned int length,
                                     unsigned int timeout)
{
    unsigned int retValue = 0;
    unsigned int key;

    switch (stateCDC) {
        case USBCDCMOUSE_CDC_UNCONFIGURED:
            break;

        case USBCDCMOUSE_CDC_INIT:
            key = GateMutex_enter(gateRxSerial);
            USBBufferInit(&txBuffer);
            USBBufferInit(&rxBuffer);
            stateCDC = USBCDCMOUSE_CDC_IDLE;
            retValue = rxData(pStr, length, timeout);
            GateMutex_leave(gateRxSerial, key);
            break;

        case USBCDCMOUSE_CDC_IDLE:
            key = GateMutex_enter(gateRxSerial);
            retValue = rxData(pStr, length, timeout);
            GateMutex_leave(gateRxSerial, key);
            break;

        default:
            break;
    }

    return (retValue);
}

/*
 *  ======== USBCDCMOUSE_sendData ========
 */
unsigned int USBCDCMOUSE_sendData(const unsigned char *pStr,
                                  unsigned int length,
                                  unsigned int timeout)
{
    unsigned int retValue = 0;
    unsigned int key;

    switch (stateCDC) {
        case USBCDCMOUSE_CDC_UNCONFIGURED:
            break;

        case USBCDCMOUSE_CDC_INIT:
            key = GateMutex_enter(gateTxSerial);
            USBBufferInit(&txBuffer);
            USBBufferInit(&rxBuffer);
            stateCDC = USBCDCMOUSE_CDC_IDLE;
            retValue = txData(pStr, length, timeout);
            GateMutex_leave(gateTxSerial, key);
            break;

        case USBCDCMOUSE_CDC_IDLE:
            key = GateMutex_enter(gateTxSerial);
            retValue = txData(pStr, length, timeout);
            GateMutex_leave(gateTxSerial, key);
            break;

        default:
            break;
    }

    return (retValue);
}

/*
 *  ======== USBCDCMOUSE_setState ========
 */
unsigned int USBCDCMOUSE_setState(USBCDCMOUSE_State *mouseState,
                                  unsigned int timeout)
{
    static unsigned int retValue = 0;

    if (stateM == USBCDCMOUSE_M_IDLE) {
        retValue = sendState(mouseState, timeout);
    }

    return (retValue);
}

/*
 *  ======== USBCDCMOUSE_waitForConnect ========
 */
bool USBCDCMOUSE_waitForConnect(unsigned int timeout)
{
    bool ret = true;
    unsigned int key;

    /* Need exclusive access to prevent a race condition */
    key = GateMutex_enter(gateUSBWait);

    if (stateUSB != USBCDCMOUSE_CONNECTED) {
        if (!Semaphore_pend(semUSBConnected, timeout)) {
            ret = false;
        }
    }

    GateMutex_leave(gateUSBWait, key);

    return (ret);
}
