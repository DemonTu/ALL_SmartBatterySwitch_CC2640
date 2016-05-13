/*
 * Copyright (c) 2015, Texas Instruments Incorporated
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
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Diags.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/gates/GateMutex.h>
#include <ti/sysbios/knl/Semaphore.h>

/* Example/Board Header files */
#include "USBCDCMOUSE.h"

/* driverlib header files */
#include "ucs.h"

/* usblib430 header files */
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/defMSP430USB.h"
#include "USB_API/USB_Common/usb.h"
#include "USB_API/USB_CDC_API/UsbCdc.h"
#include "USB_API/USB_HID_API/UsbHid.h"

/* Code taken from the MSP430 usblib430 mouse application */
typedef struct {
    unsigned char   buttons;
    unsigned char   dX;
    unsigned char   dY;
    unsigned char   dZ;
} MouseReport;

/* Static variables and handles */
static GateMutex_Handle         gateTxSerial;
static GateMutex_Handle         gateRxSerial;
static GateMutex_Handle         gateMouse;
static GateMutex_Handle         gateUSBWait;
static Semaphore_Handle         semTxSerial;
static Semaphore_Handle         semRxSerial;
static Semaphore_Handle         semMouse;
static Semaphore_Handle         semUSBConnected;

/* Function prototypes */
static unsigned int  rxData(unsigned char *pStr,
                            unsigned int length,
                            unsigned int timeout);
static int   sendState(USBCDCMOUSE_State *mouseState,
                       unsigned int timeout);
static unsigned int  txData(const unsigned char *pStr,
                            int length,
                            unsigned int timeout);
void         USBCDCMOUSE_init(void);
unsigned int         USBCDCMOUSE_receiveData(unsigned char *pStr,
                                             unsigned int length,
                                             unsigned int timeout);
unsigned int         USBCDCMOUSE_sendData(const unsigned char *pStr,
                                          unsigned int length,
                                          unsigned int timeout);
unsigned int         USBCDCMOUSE_setState(USBCDCMOUSE_State *mouseState,
                                          unsigned int timeout);
bool         USBCDCMOUSE_waitForConnect(unsigned int timeout);

/* Start of callback functions needed by USB_Common/Usb.[ch] */
uint8_t USB_handleClockEvent()   {return false;}
uint8_t USB_handleResetEvent()   {return false;}
uint8_t USB_handleResumeEvent()  {return false;}
uint8_t USB_handleSuspendEvent() {return false;}

uint8_t USB_handleVbusOffEvent()
{
    UCS_turnOffXT2();
    Log_print0(Diags_USER1, "USB: Disconnect complete");
    return (true);
}

uint8_t USB_handleVbusOnEvent()
{
    if (USB_enable() == USB_SUCCEED) {
        USB_reset();
        USB_connect();
        Log_print0(Diags_USER1, "USB: VBus detected");
    }
    return (true);
}

uint8_t USB_handleEnumerationCompleteEvent()
{
    Semaphore_post(semUSBConnected);
    Log_print0(Diags_USER1, "USB: Enumeration complete");
    return (true);
}

/* Start of callback functions needed by USB_CDC_API/UsbCdc.[ch] */
uint8_t USBCDC_handleDataReceived(uint8_t intfNum) {return false;}

uint8_t USBCDC_handleSetLineCoding(uint8_t intfNum, uint32_t lBaudrate)
{
    Log_print1(Diags_USER1, "SetLineCoding baudrate: %d\n", lBaudrate);
    return (true);
}

uint8_t USBCDC_handleSetControlLineState(uint8_t intfNum, uint8_t lineState)
{
    Log_print1(Diags_USER1, "SetControlLineState: %x\n", lineState);
    return (true);
}

uint8_t USBCDC_handleSendCompleted(uint8_t intfNum)
{
    if (intfNum == USBCDCD_INTFNUM) {
        Semaphore_post(semTxSerial);
        Log_print0(Diags_USER1, "USB: USBCDC_handleSendCompleted");
    }
    return (true);
}

uint8_t USBCDC_handleReceiveCompleted(uint8_t intfNum)
{
    if (intfNum == USBCDCD_INTFNUM) {
        Semaphore_post(semRxSerial);
        Log_print0(Diags_USER1, "USB: USBCDC_handleReceiveCompleted");
    }
    return (true);
}

/* Start of callback functions needed by USB_HID_API/UsbHid.[ch] */
/*
 * This event indicates that data has been received for port port, but no data receive operation is underway.
 * returns true to keep CPU awake
 */
uint8_t USBHID_handleDataReceived(uint8_t intfNum)
{
    Log_print0(Diags_USER1, "USB: USBHID_handleDataReceived");
    return (false);
}

/*
 * This event indicates that a send operation on port port has just been completed.
 * returns true to keep CPU awake
 */
uint8_t USBHID_handleSendCompleted(uint8_t intfNum)
{
    Semaphore_post(semMouse);
    Log_print0(Diags_USER1, "USB: USBHID_handleSendCompleted");
    return (true);
}

/*
 * This event indicates that a receive operation on port port has just been completed.
 * returns true to keep CPU awake
 */
uint8_t USBHID_handleReceiveCompleted(uint8_t intfNum)
{
    Log_print0(Diags_USER1, "USB: USBHID_handleReceiveCompleted");
    return (true);
}

/*
 * This event indicates that a Set_Protocol request was received from the host
 * The application may maintain separate reports for boot and report protocols.
 * The protocol field is either HID_BOOT_PROTOCOL or
 * HID_REPORT_PROTOCOL
 */
uint8_t USBHID_handleBootProtocol(uint8_t protocol, uint8_t intfnum)
{
    Log_print0(Diags_USER1, "USB: USBHID_handleBootProtocol");
    return (true);
}

/*
 * This event indicates that a Set_Report request was received from the host
 * The application needs to supply a buffer to retrieve the report data that will be sent
 * as part of this request. This handler is passed the reportType, reportId, the length of data
 * phase as well as the interface number.
 */
uint8_t *USBHID_handleEP0SetReport(uint8_t reportType, uint8_t reportId,
                                   uint16_t requestedLength,
                                   uint8_t intfnum)
{
    switch (reportType) {
        case USB_REQ_HID_INPUT:
        /* Return pointer to input Report Buffer */
        Log_print0(Diags_USER1, "USB: USBHID_handleEP0SetReport: USB_REQ_HID_INPUT");
        return 0;

    case USB_REQ_HID_OUTPUT:
        /* Return pointer to output Report Buffer */
        Log_print0(Diags_USER1, "USB: USBHID_handleEP0SetReport: USB_REQ_HID_OUTPUT");
        return 0;

    case USB_REQ_HID_FEATURE:
        /* Return pointer to feature Report Buffer */
        Log_print0(Diags_USER1, "USB: USBHID_handleEP0SetReport: USB_REQ_HID_FEATURE");
        return 0;

    default:
        Log_print0(Diags_USER1, "USB: USBHID_handleEP0SetReport: default");
        return 0;
    }
}

/*
 * This event indicates that data as part of Set_Report request was received from the host
 * The application can return true to wake up the CPU. If the application supplied a buffer
 * as part of USBHID_handleSetReport, then this buffer will contain the Set Report data.
 */
uint8_t USBHID_handleEP0SetReportDataAvailable(uint8_t intfnum)
{
    Log_print0(Diags_USER1, "USB: USBHID_handleEP0SetReportDataAvailable");
    return (true);
}

/*
 * This event indicates that a Get_Report request was received from the host
 * The application can supply a buffer of data that will be sent to the host.
 * This handler is passed the reportType, reportId, the requested length as
 * well as the interface number.
 */
uint8_t *USBHID_handleEP0GetReport(uint8_t reportType, uint8_t reportId,
                                   uint16_t requestedLength,
                                   uint8_t intfnum)
{
    switch (reportType) {
    case USB_REQ_HID_INPUT:
        /* Return pointer to input Report Buffer */
        Log_print0(Diags_USER1, "USB: USBHID_handleEP0GetReport: USB_REQ_HID_INPUT");
        return 0;
    case USB_REQ_HID_OUTPUT:
        /* Return pointer to OUTput Report Buffer */
        Log_print0(Diags_USER1, "USB: USBHID_handleEP0GetReport: USB_REQ_HID_OUTPUT");
        return 0;
    case USB_REQ_HID_FEATURE:
        /* Return pointer to FEATURE Report Buffer */
        Log_print0(Diags_USER1, "USB: USBHID_handleEP0GetReport: USB_REQ_HID_FEATURE");
        return 0;
    default:
        Log_print0(Diags_USER1, "USB: USBHID_handleEP0GetReport: default");
        return 0;
    }
}

/* Start of static helper functions */
/*
 *  ======== rxData ========
 */
static unsigned int rxData(unsigned char *pStr,
                           unsigned int length,
                           unsigned int timeout)
{
    unsigned char    recvResult;
    unsigned int    ret = 0;

    recvResult = USBCDC_receiveData(pStr, length, USBCDCD_INTFNUM);
    /*
     * We will pend on a Semaphore_pend() if we received a
     * USBCDC_RECEIVE_STARTED and block until we received all the data. If it
     * returned a USBCDC_RECEIVE_COMPLETED; a callback has already occurred
     * so we call Semaphore_post() to just consume it but it won't cause a
     * context switch.
     */
    if ((recvResult == USBCDC_RECEIVE_STARTED) || (recvResult == USBCDC_RECEIVE_COMPLETED)) {
        if (!Semaphore_pend(semRxSerial, timeout)) {
            USBCDC_abortReceive(&length, USBCDCD_INTFNUM);
        }
        ret = length;
    }

    return (ret);
}

/*
 *  ======== sendState ========
 */
static int sendState(USBCDCMOUSE_State *mouseState,
                     unsigned int timeout)
{
    unsigned int key;
    int retValue;
    MouseReport mouseReport = {0};

    /* Set the bit packed button values */
    mouseReport.buttons |= (mouseState->button1) ? 0x01 : 0;
    mouseReport.buttons |= (mouseState->button2) ? 0x02 : 0;
    mouseReport.buttons |= (mouseState->button3) ? 0x04 : 0;
    mouseReport.dX = mouseState->deltaX;
    mouseReport.dY = mouseState->deltaY;

    /* Acquire lock */
    key = GateMutex_enter(gateMouse);
    USBHID_sendReport((void*)&mouseReport, M_INTFNUM);
    retValue = Semaphore_pend(semMouse, timeout);

    /* Release lock */
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
    unsigned char    sendResult;
    unsigned int    ret = 0;

    sendResult = USBCDC_sendData(pStr, length, USBCDCD_INTFNUM);
    if (sendResult == USBCDC_SEND_STARTED) {
        if (!Semaphore_pend(semTxSerial, timeout)) {
            USBCDC_abortSend((unsigned int *)&length, USBCDCD_INTFNUM);
        }
        ret = length;
    }
    return (ret);
}

/*
 *  ======== USBCDCMOUSE_init ========
 */
void USBCDCMOUSE_init(void)
{
    Error_Block eb;
    Semaphore_Params semParams;
    GateMutex_Params gateParams;

    Error_init(&eb);

    /* RTOS primitives */
    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    semParams.__iprms.name = "semTxSerial";
    semTxSerial = Semaphore_create(0, &semParams, &eb);
    if (semTxSerial == NULL) {
        System_abort("Can't create TX semaphore");
    }

    semParams.__iprms.name = "semRxSerial";
    semRxSerial = Semaphore_create(0, &semParams, &eb);
    if (semRxSerial == NULL) {
        System_abort("Can't create RX semaphore");
    }

    semParams.__iprms.name = "semMouse";
    semMouse = Semaphore_create(0, &semParams, &eb);
    if (semMouse == NULL) {
        System_abort("Can't create USB semaphore");
    }

    semParams.__iprms.name = "semUSBConnected";
    semUSBConnected = Semaphore_create(0, &semParams, &eb);
    if (semUSBConnected == NULL) {
        System_abort("Can't create USB semaphore");
    }

    GateMutex_Params_init(&gateParams);
    gateParams.__iprms.name = "gateTxSerial";
    gateTxSerial = GateMutex_create(NULL, &eb);
    if (gateTxSerial == NULL) {
        System_abort("Can't create gate");
    }

    gateParams.__iprms.name = "gateRxSerial";
    gateRxSerial = GateMutex_create(NULL, &eb);
    if (gateRxSerial == NULL) {
        System_abort("Can't create gate");
    }

    gateParams.__iprms.name = "gateMouse";
    gateMouse = GateMutex_create(NULL, &eb);
    if (gateMouse == NULL) {
        System_abort("Can't create mouse gate");
    }

    gateParams.__iprms.name = "gateUSBWait";
    gateUSBWait = GateMutex_create(NULL, &eb);
    if (gateUSBWait == NULL) {
        System_abort("Could not create USB Wait gate");
    }

    /* Initialize the USB module, enable events and connect if UBUS is present */
    USB_setup(true, true);
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

    if (USB_getConnectionInformation() & USB_ENUMERATED) {
        key = GateMutex_enter(gateRxSerial);
        retValue = rxData(pStr, length, timeout);
        GateMutex_leave(gateRxSerial, key);
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

    if (USB_getConnectionInformation() & USB_ENUMERATED) {
        key = GateMutex_enter(gateTxSerial);
        retValue = txData(pStr, length, timeout);
        GateMutex_leave(gateTxSerial, key);
    }

    return (retValue);
}

/*
 *  ======== USBCDCMOUSE_setState ========
 */
unsigned int USBCDCMOUSE_setState(USBCDCMOUSE_State *mouseState,
                                  unsigned int timeout)
{
    unsigned int retValue = 0;

    if (USB_getConnectionInformation() & USB_ENUMERATED) {
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

    if (!(USB_getConnectionInformation() & USB_ENUMERATED)) {
        if (!Semaphore_pend(semUSBConnected, timeout)) {
            ret = false;
        }
    }

    GateMutex_leave(gateUSBWait, key);

    return (ret);
}
