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
 *  ======== llserial.c ========
 *  Serial driver implementation for PPP HDLC mode
 */
#include <stkmain.h>
#include <tools/hdlcif.h>

#include "llserial.h"

/* Max number of serial instances */
#define MAX_INSTANCE 1

/* Number of devices */
static uint32_t SerDevCount = 0;

SDINFO sdi[MAX_INSTANCE];

/*
 *  ======== _llSerialInit ========
 *  Opens the serial driver environment and enumerates devices. This function
 *  is called automatically by the stack in the NetCtrl module and should not
 *  be called by the user's application.
 */
uint _llSerialInit(STKEVENT_Handle hEvent)
{
    int i;

    /* Initialize the serial driver(s) */
    SerDevCount = HwSerInit();

    if (SerDevCount > MAX_INSTANCE) {
        SerDevCount = MAX_INSTANCE;
    }

    /* Initialize SDINFO for each driver instance */
    for (i = 0; i < SerDevCount; i++) {
        mmZeroInit(&sdi[i], sizeof(SDINFO));

        /* Set physical index */
        sdi[i].PhysIdx = i;

        /* Set event object handle */
        sdi[i].hEvent = hEvent;

        /* Set a default operating mode */
        sdi[i].Baud     = 9600;
        sdi[i].Mode     = HAL_SERIAL_MODE_8N1;
        sdi[i].FlowCtrl = HAL_SERIAL_FLOWCTRL_NONE;
    }

    return (SerDevCount);
}

/*
 *  ======== _llSerialShutdown ========
 *  Called to shutdown serial driver environment. This function is called
 *  automatically by the stack in the NetCtrl module and should not be called
 *  by the user's application.
 */
void _llSerialShutdown(void)
{
    uint32_t dev;

    /* Reset and close all open devices */
    for (dev = 0; dev < SerDevCount; dev++) {
        if (sdi[dev].hHDLC) {
            /* Free the HDLC object. Results in call to llSerialCloseHDLC() */
            hdlcFree(sdi[dev].hHDLC);
            sdi[dev].hHDLC   = 0;
            sdi[dev].cbTimer = 0;
            sdi[dev].cbInput = 0;
        }
    }

    HwSerShutdown();
    SerDevCount = 0;
}

/*
 *  ======== _llSerialServiceCheck ========
 *  Check for HDLC activity (called from NetCtrl)
 */
void _llSerialServiceCheck(uint fTimerTick)
{
    uint32_t dev;

    /* Call the HDLC timer callback (if needbe) */
    for (dev = 0; dev < SerDevCount; dev++) {

        /*
         *  If the timer ticked, then call the timer callback function on one
         *  second intervals
         */
        if (sdi[dev].cbTimer && fTimerTick && (++sdi[dev].Ticks >= 10)) {
            sdi[dev].Ticks = 0;
            llEnter();
            (*sdi[dev].cbTimer)(sdi[dev].hHDLC);
            llExit();
        }
    }
}

/*
 *  ======== _llSerialSend ========
 *  Send data in character mode; not used in HDLC mode.
 */
uint _llSerialSend(uint dev, unsigned char *pBuf, uint len)
{
    /* Return failure */
    return (0);
}

/*
 *  ======== llSerialOpen ========
 *  Opens the device for charater mode; not used in HDLC mode.
 */
uint llSerialOpen(uint dev, void (*cbInput)(char c))
{
    /* Return failure */
    return (0);
}

/*
 *  ======== llSerialClose ========
 *  Closes the device for charater mode; not used in HDLC mode.
 */
void llSerialClose(uint dev)
{
}

/*
 *  ======== llSerialOpenHDLC ========
 *  Opens the device for HDLC mode
 */
uint llSerialOpenHDLC(uint dev, void *hHDLC,
        void (*cbTimer)(void *h), void (*cbInput)(PBM_Handle hPkt))
{
    /* Our device index is "1" based to the upper layers */
    dev--;
    if (dev >= SerDevCount) {
        return(0);
    }

    /* Init logical device */
    sdi[dev].hHDLC = hHDLC;

    /* Init callbacks */
    sdi[dev].cbTimer = cbTimer;
    sdi[dev].cbInput = cbInput;

    /* Set the peer map to a safe default */
    sdi[dev].PeerMap = 0xFFFFFFFF;

    /* If already open, just return */
    if (++sdi[dev].Open > 1) {
        return(1);
    }
    else {
        /* Else call low-level open function if not already open */
        return((sdi[dev].Open = HwSerOpen(&sdi[dev])));
    }
}

/*
 *  ======== llSerialCloseHDLC ========
 *  Close the device for HDLC mode.
 */
void llSerialCloseHDLC(uint dev)
{
    /* Our device index is "1" based to the upper layers */
    dev--;
    if (dev >= SerDevCount) {
        return;
    }

    /* Close the driver */
    if (sdi[dev].Open) {
        HwSerClose(&sdi[dev]);
        sdi[dev].Open = 0;
    }
}

/*
 *  ======== llSerialSendPkt ========
 *  Send data in packet form
 */
void llSerialSendPkt(uint dev, PBM_Handle hPkt)
{
    /* Our device index is "1" based to the upper layers */
    dev--;
    if (dev >= SerDevCount) {
        return;
    }

    if (hPkt) {
        PBMQ_enq(&(sdi[dev].PBMQ_tx), hPkt);

        if (sdi[dev].TxFree) {
            HwSerTxNext(&(sdi[dev]));
        }
    }
}

/*
 *  ======== llSerialHDLCPeerMap ========
 *  Update the sending peer map
 */
void llSerialHDLCPeerMap(uint dev, uint peerMap)
{
    /* Our device index is "1" based to the upper layers */
    dev--;

    if (dev < SerDevCount) {
        sdi[dev].PeerMap = peerMap;
    }
}

/*
 *  ======== llSerialService ========
 *  Pass up HDLC packets to their input callback functions. Called from NetCtrl
 *  upon STKEVENT_signal() call
 */
void llSerialService(void)
{
    uint32_t dev;
    PBM_Handle hPkt;

    /* Service all the drivers */
    for (dev = 0; dev < SerDevCount; dev++) {

        /* Give all queued packets to the callback function */
        while ((hPkt = PBMQ_deq(&sdi[dev].PBMQ_rx))) {
            if (sdi[dev].cbInput) {
                sdi[dev].cbInput(hPkt);
            }
            else {
                PBM_free(hPkt);
            }
        }
    }
}

/*
 *  ======== llSerialConfig ========
 *  Configure the serial port baud rate and mode
 */
void llSerialConfig(uint dev, uint baud, uint mode,
        uint flowctrl)
{
    /* Our device index is "1" based to the upper layers */
    dev--;
    if (dev >= SerDevCount) {
        return;
    }

    /* Set baud and mode */
    sdi[dev].Baud = baud;
    sdi[dev].Mode = mode;
    sdi[dev].FlowCtrl = flowctrl;

    /* If the driver is open, update the settings */
    if (sdi[dev].Open) {
        HwSerSetConfig(&sdi[dev]);
    }
}

/*
 *  ======== USBSerialPPP_NIMUInit ========
 *  The function is used to initialize and register the serial PPP
 *  device with the Network Interface Management Unit (NIMU)
 *
 *  Simply call hdlcNew().  This will result in the following:
 *
 *   - Create and set up a new HDLC instance
 *   - Call llSerialOpenHDLC()
 *   - Map the cbInput function pointer to hdlcInput()
 *   - Call pppNew() to initialize PPP
 *   - Map hdlcSI() as the serial interface control function
 *   - pppNew() takes care of assigning the NIMU register functions
 *     (start/stop/poll/send, etc.) and calls NIMURegister() for us
 */
int USBSerialPPP_NIMUInit(STKEVENT_Handle hEvent)
{
    void *hHDLC = NULL;

    /* Set up HDLC + PPP on serial device 1 */
    hHDLC = hdlcNew(1, PPPFLG_CLIENT | PPPFLG_OPT_USE_MSE |
            PPPFLG_SIOPT_SENDCMAP | PPPFLG_SIOPT_RECVCMAP | PPPFLG_OPT_ALLOW_HC,
            0, "username", "password");

    if (!hHDLC) {
        return (-1);
    }
    else {
        return (0);
    }
}
