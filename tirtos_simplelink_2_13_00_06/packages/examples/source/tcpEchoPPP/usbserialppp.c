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
 *  ======== usbserialppp.c ========
 *  Connection to hardware layer for serial driver
 */

#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

#include <netmain.h>

#include "llserial.h"
#include "USBCDCD.h"

/* USBCDCD APIs currently only support a single USB device */
#define NUM_USB_DEVICES 1

#define PPP_BUFSIZE 128
#define PPP_RECVTIMEOUT 1000

/* HDLC constants */
#define HDLC_FLAGCHAR 0x7E /* Character to indicate frame boundary */
#define HDLC_MINCHAR  0x20 /* Minimum non-escaped char */
#define HDLC_ESCCHAR  0x7D /* Character to indicate escape */
#define HDLC_XORCHAR  0x20 /* Escape XOR modifier */

#define PPPINITFCS16 0xffff /* Initial FCS value */
#define PPPGOODFCS16 0xf0b8 /* Good final FCS value */

extern SDINFO sdi[];

/* FCS lookup table as calculated by the table generator */
static const uint16_t fcstab[16] = {
   0x0000, 0x1081, 0x2102, 0x3183, 0x4204, 0x5285, 0x6306, 0x7387,
   0x8408, 0x9489, 0xA50A, 0xB58B, 0xC60C, 0xD68D, 0xE70E, 0xF78F
};

/* TX and RX functions */
static uint32_t spWriteFifo(SDINFO *pi);
Void spIsrTask(UArg arg0, UArg arg1);

/*
 *  ======== HwSerInit ========
 */
uint32_t HwSerInit(void)
{
    /* Return the number of device instances supported */
    return(NUM_USB_DEVICES);
}

/*
 *  ======== HwSerShutdown ========
 */
void HwSerShutdown(void)
{
}

/*
 *  ======== HwSerOpen ========
 *  Open a serial device instance
 */
uint32_t HwSerOpen(SDINFO *pi)
{
    Semaphore_Params semParams;

    /* Semaphore used to signal spIsrTask to exit */
    Semaphore_Params_init(&semParams);
    semParams.mode = Semaphore_Mode_BINARY;
    pi->spIsrStop = Semaphore_create(0, &semParams, NULL);
    if (!pi->spIsrStop) {
        System_printf(
                "HwSerOpen: Error: Failed to create spIsrStop Semaphore\n");
        goto EXIT_FAIL;
    }

    /* Semaphore used to signal HwSerClose that it's OK to delete resources */
    pi->spIsrDone = Semaphore_create(0, &semParams, NULL);
    if (!pi->spIsrDone) {
        System_printf(
                "HwSerOpen: Error: Failed to create spIsrDone Semaphore\n");
        goto EXIT_FAIL;
    }

    pi->SerRxGood = 0;
    pi->SerRxBadCRC = 0;

    /* Mark transmitter as available */
    pi->TxFree = 1;

    /* Initialized successfully */
    return (1);

EXIT_FAIL:
    /* If any *_create() fails, clean up any successful previous allocations */
    if (pi->spIsrStop) {
        Semaphore_delete(&pi->spIsrStop);
    }
    if (pi->spIsrDone) {
        Semaphore_delete(&pi->spIsrDone);
    }

    return(0);
}

/*
 *  ======== HwSerClose ========
 *  Close a serial device instance
 */
void HwSerClose(SDINFO *pi)
{
    /* Signal spIsrTask to exit */
    Semaphore_post(pi->spIsrStop);

    /* Wait for spIsrTask to complete/exit, in order to safely clean up */
    Semaphore_pend(pi->spIsrDone, BIOS_WAIT_FOREVER);

    /* Free any outstanding RX packet buffers */
    if (pi->hRxPend) {
        PBM_free(pi->hRxPend);
        pi->hRxPend = 0;
    }

    /* Free any outstanding TX packet buffers */
    if (pi->hTxPend) {
        PBM_free(pi->hTxPend);
        pi->hTxPend = 0;
    }

    /* Free all queued RX packet buffers */
    while (PBMQ_count(&pi->PBMQ_rx)) {
        PBM_free(PBMQ_deq(&pi->PBMQ_rx));
    }

    /* Free all queued TX packet buffers */
    while (PBMQ_count(&pi->PBMQ_tx)) {
        PBM_free(PBMQ_deq(&pi->PBMQ_tx));
    }

    /* Delete dynamically allocated objects */
    Semaphore_delete(&pi->spIsrDone);
    Semaphore_delete(&pi->spIsrStop);

    /* Reset device instance variables */
    pi->spIsrDone = NULL;
    pi->spIsrStop = NULL;
    pi->spIsrTaskHandle = NULL;
    pi->SerRxGood = 0;
    pi->SerRxBadCRC = 0;

    /* Mark transmitter status as unavailable */
    pi->TxFree = 0;
}

/*
 *  ======== HwSerSetConfig ========
 *  Update hardware configuration
 */
void HwSerSetConfig(SDINFO *pi)
{
    /* Do nothing. Baud rate, mode, flow control settings handled in USBCDCD */
}

/*
 *  ======== HwSerTxNext ========
 *  Transmit Next Buffer on Tx Queue
 */
void HwSerTxNext(SDINFO *pi)
{
    pi->TxFree = 0;
    spWriteFifo(pi);
}

/*
 *  ======== HwSerPoll ========
 */
void _HwSerPoll(SDINFO *pi, uint32_t fTimerTick)
{
}

/*
 *  ======== spWriteFifo ========
 *  Write out TX data to the TX FIFO.
 *
 *  Additionally, this code frames, escapes and validates the CRC for HDLC.
 *
 *  Returns 1 if the system needs the TX FIFO Empty interrupt
 */
static uint32_t spWriteFifo(SDINFO *pi)
{
    char c;
    int currCharPos = 0;

    /* Transmit buffer */
    unsigned char pppTxBuf[PPP_BUFSIZE];

    do {
        /* Start a new packet if we need to */
        if (!pi->hTxPend) {
            pi->hTxPend = PBMQ_deq(&pi->PBMQ_tx);

            /* If we don't have anything to send here, quit */
            if (!pi->hTxPend) {
                pi->TxFree = 1;
                return(0);
            }

            pi->pTxBuf = PBM_getDataBuffer(pi->hTxPend) +
                    PBM_getDataOffset(pi->hTxPend);

            pi->TxCount = PBM_getValidLen(pi->hTxPend);

            /* If in HDLC mode, setup for HDLC */
            if (pi->hHDLC) {
                pi->TxCRC   = PPPINITFCS16;
                pi->TxFlag  = 1;
                pi->TxChar  = HDLC_FLAGCHAR;
            }
            else {
                pi->TxFlag  = 0;
            }
        }

        /* Continue with pending packet */

        /* If inserting a character, do it now, else read from packet */
        if (pi->TxFlag) {
            c = pi->TxChar;
            pi->TxFlag = 0;
        }
        else {
            c = *pi->pTxBuf++;
            pi->TxCount--;

            /* If in HDLC mode, handle CRC and any "escape" */
            if (pi->hHDLC) {
                /*
                 *  If we get here, we have an HDLC character, track CRC
                 *  We use 4 bit CRC to save table space
                 */
                pi->TxCRC ^= c;
                pi->TxCRC = (pi->TxCRC >> 4) ^ fcstab[0xF & pi->TxCRC];
                pi->TxCRC = (pi->TxCRC >> 4) ^ fcstab[0xF & pi->TxCRC];

                /* See if we must "escape" this char */
                if ((c < HDLC_MINCHAR && (pi->PeerMap & (1 << c))) ||
                        c == HDLC_ESCCHAR || c == HDLC_FLAGCHAR) {
                    pi->TxFlag = 1;
                    pi->TxChar = c ^ HDLC_XORCHAR;
                    c = HDLC_ESCCHAR;
                }

                /*
                 *  If count is 2, then we can insert the checksum into
                 *  the packet
                 */
                if (pi->TxCount == 2) {
                    pi->TxCRC = ~pi->TxCRC;
                    *pi->pTxBuf = (char)(pi->TxCRC % 256);
                    *(pi->pTxBuf + 1) = (char)(pi->TxCRC / 256);
                }
            }
        }

        /* Buffer up data until we have PPP_BUFSIZE bytes to hand to USB */
        pppTxBuf[currCharPos++] = c;
        if (currCharPos == PPP_BUFSIZE) {
            /*
             *  We have a full buffer, pass data to USB for transfer
             *  Block while the device is NOT connected to the USB
             */
            USBCDCD_waitForConnect(BIOS_WAIT_FOREVER);

            /* Flush the buffer out to USB */
            USBCDCD_sendData(pppTxBuf, PPP_BUFSIZE, BIOS_WAIT_FOREVER);

            /* Reset our buffer */
            currCharPos = 0;
        }

        /* See if we're done */
        if (!pi->TxFlag && !pi->TxCount) {
            /* In HDLC mode we may need to add one more flag byte */
            if (pi->hHDLC && c != HDLC_FLAGCHAR) {
                pi->TxFlag = 1;
                pi->TxChar = HDLC_FLAGCHAR;
            }
            else {
                /* Here we're done with the packet */
                PBM_free(pi->hTxPend);
                pi->hTxPend = 0;
            }
        }
    } while (pi->TxCount > 0 || pi->TxFlag);

    if (currCharPos > 0) {
        /* Flush out any remaining data */
        USBCDCD_waitForConnect(BIOS_WAIT_FOREVER);
        USBCDCD_sendData(pppTxBuf, currCharPos, BIOS_WAIT_FOREVER);
    }

    /* Done with our transfer; set transmitter status as available */
    pi->TxFree = 1;
    return(1);
}

/*
 * ======== spIsrTask ========
 *  Read data from the USB device. Signal the NDK scheduler as soon as a full
 *  HDLC buffer is ready. This task is created/run on a per device basis.
 */
Void spIsrTask(UArg arg0, UArg arg1)
{
    char c;
    int active = 1;
    uint32_t fSignal = 0;
    uint32_t numBytes = 0;
    uint32_t currCharPos = 0;
    /* Receive buffer */
    unsigned char pppRxBuf[PPP_BUFSIZE];
    /*
     *  Get handle to the PPP struct for the device instance corresponding to
     *  this Task:
     */
    int dev = (int)arg0;
    SDINFO *pi = &sdi[dev];

    Task_sleep(1000);

    /* Save the association between this Task and device instance */
    pi->spIsrTaskHandle = Task_self();

    do {
        if (Semaphore_pend(pi->spIsrStop, BIOS_NO_WAIT)) {
            /* HwSerClose was called. Stop receiving data, exit Task */
            active = 0;
            continue;
        }

        if (currCharPos == numBytes) {
            /*
             *  Initial loop iteration or we've processed an entire buffer
             *
             *  Buffer up a new chunk of data from USB. Limit wait time to
             *  prevent dead lock during dev instance clean up (HwSerClose).
             */
            numBytes = USBCDCD_receiveData(pppRxBuf, PPP_BUFSIZE,
                    PPP_RECVTIMEOUT);

            /* Reset read position */
            currCharPos = 0;

            if (numBytes == 0) {
                /* If USB didn't give us anything, come back and try again */
                continue;
            }
        }

        /* Read a character and process it */
        c = pppRxBuf[currCharPos++];

        /* If we don't have a packet ready, get one */
        if (!pi->hRxPend) {
            pi->hRxPend = PBM_alloc(RX_MAX + PKT_PREPAD);

            /* If we don't get a buffer, toss the character */
            if (!pi->hRxPend) {
                goto CHECK_SIGNAL_STACK;
            }
InitRxPacket:
            PBM_setDataOffset(pi->hRxPend,PKT_PREPAD);
            pi->pRxBuf  = PBM_getDataBuffer(pi->hRxPend)+PKT_PREPAD;
            pi->RxCount = 0;
            pi->RxCRC   = PPPINITFCS16;
            pi->RxFlag  = 0;
        }

        /* If its a marker char, act on it */
        if (c == HDLC_FLAGCHAR) {
            /*
             *  If we have some data in our HDLC packet, try and validate it.
             *  Otherwise, ignore this packet
             */
            if (!pi->RxCount) {
                goto CHECK_SIGNAL_STACK;
            }

            /* Verify the packet CRC */
            if (pi->RxCRC != PPPGOODFCS16) {
                pi->SerRxBadCRC++;
                goto InitRxPacket;
            }
            else {
                /* Correct the packet size and offset to give to PPP */
                pi->SerRxGood++;
                PBM_setValidLen(pi->hRxPend, pi->RxCount);
                PBM_setIFRx(pi->hRxPend, pi->hHDLC);
                PBMQ_enq(&pi->PBMQ_rx, pi->hRxPend);
                pi->hRxPend = 0;
                fSignal = 1;
            }
            goto CHECK_SIGNAL_STACK;
        }

        /* If this is an escape char, enter escape mode */
        if (c == HDLC_ESCCHAR) {
            pi->RxFlag = 1;
            goto CHECK_SIGNAL_STACK;
        }

        /* Don't receive too much */
        if (pi->RxCount == RX_MAX) {
            goto CHECK_SIGNAL_STACK;
        }

        /*
         *  If we are in the middle of an "escape", then we convert this
         *  character
         */
        if (pi->RxFlag) {
            pi->RxFlag = 0;
            c ^= HDLC_XORCHAR;
        }

        /* Track the CRC so we can verify it later */
        pi->RxCRC ^= c;
        pi->RxCRC = (pi->RxCRC >> 4) ^ fcstab[0xF & pi->RxCRC];
        pi->RxCRC = (pi->RxCRC >> 4) ^ fcstab[0xF & pi->RxCRC];

        *pi->pRxBuf++ = c;
        pi->RxCount++;

CHECK_SIGNAL_STACK:
        if (fSignal) {
            /* signal stack that a packet is ready */
            STKEVENT_signal(pi->hEvent, STKEVENT_SERIAL, 1);
        }
    } while (active);

    /* Signal HwSerClose that spIsrTask has completed */
    Semaphore_post(pi->spIsrDone);
}
