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
 *    ======== USBCDCD_LoggerIdle.c ========
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
#include "USBCDCD_LoggerIdle.h"

/* driverlib header files */
#include "ucs.h"

/* usblib430 header files */
#include "USB_API/USB_Common/device.h"
#include "USB_API/USB_Common/defMSP430USB.h"
#include "USB_API/USB_Common/usb.h"
#include "USB_API/USB_CDC_API/UsbCdc.h"

/* Function prototypes */
static unsigned int txData(const unsigned char *pStr, unsigned int length);
void USBCDCD_init(void);
int  USBCDCD_LoggerIdle_sendData(unsigned char *pStr, int length);

/* Start of callback functions needed by USB_Common/Usb.[ch] */
uint8_t USB_handleClockEvent()   {return false;}
uint8_t USB_handleResetEvent()   {return false;}
uint8_t USB_handleResumeEvent()  {return false;}
uint8_t USB_handleSuspendEvent() {return false;}

uint8_t USB_handleVbusOffEvent()
{
    UCS_turnOffXT2();
    return (true);
}

uint8_t USB_handleVbusOnEvent()
{
    if (USB_enable() == USB_SUCCEED) {
        USB_reset();
        USB_connect();
    }
    return (true);
}

uint8_t USB_handleEnumerationCompleteEvent(void)
{
    return (true);
}

/* Start of callback functions needed by USB_CDC_API/UsbCdc.[ch] */
uint8_t USBCDC_handleDataReceived(uint8_t intfNum) {return false;}

uint8_t USBCDC_handleSetLineCoding(uint8_t intfNum, uint32_t lBaudrate)
{
    return (true);
}

uint8_t USBCDC_handleSetControlLineState(uint8_t intfNum, uint8_t lineState)
{
    return (true);
}

uint8_t USBCDC_handleSendCompleted(uint8_t intfNum)
{
    return (true);
}

uint8_t USBCDC_handleReceiveCompleted(uint8_t intfNum)
{
    return (false);
}

/* Start of static helper functions */
/*
 *  ======== txData ========
 */
static unsigned int txData(const unsigned char *pStr, unsigned int length)
{
    unsigned char   sendResult;
    unsigned int    ret = 0;

    sendResult = USBCDC_sendData(pStr, length, USBCDCD_INTFNUM);
    if (sendResult == USBCDC_SEND_STARTED) {
        ret = length;
    }
    return (ret);
}

/*
 *  ======== USBCDCD_init ========
 */
void USBCDCD_init(void)
{
    /* Initialize the USB module, enable events and connect if UBUS is present */
    USB_setup(true, true);
}


/*
 *  ======== USBCDCD_LoggerIdle_sendData ========
 */
int USBCDCD_LoggerIdle_sendData(unsigned char *pStr, int length)
{
    unsigned int retValue = 0;

    if (USB_getConnectionInformation() & USB_ENUMERATED) {
        retValue = txData(pStr, length);
    }

    return (retValue);
}
