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
 *  ======== llserial.h ========
 *  Serial driver interface
 */

#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>

#include <netmain.h>
#include <stkmain.h>

/*
 * Pre-Pad Packet Data Offset
 *
 * The TCP/IP stack library requires that every packet device
 * include enough Layer 2 header room for all supported headers. In
 * order to support PPPoE, this requires a 22 byte Layer 2 header.
 * Thus, since standard PPP is only 4 bytes, we must add
 * on an additional 18 byte offset, or PPPoE can not function
 * with our driver.
 */
#define PKT_PREPAD 18

#define RX_MAX 1506

/* Serial device information */

typedef struct _sdinfo {
    uint32_t        PhysIdx;  /* Physical index of this device (0 to n-1) */
    uint32_t        Open;     /* Open counter used by llSerial */
    void            *hHDLC;   /* Handle to logical driver (NULL when closed) */
    STKEVENT_Handle hEvent;   /* Handle to scheduler event object */
    uint32_t        PeerMap;  /* 32 bit character escape map (for HDLC) */
    uint32_t        Ticks;    /* Track timer ticks */
    uint32_t        Baud;     /* Baud rate */
    uint32_t        Mode;     /* Data bits, stop bits, parity */
    uint32_t        FlowCtrl; /* Flow Control Mode */
    uint32_t        TxFree;   /* Transmitter "free" flag */
    PBMQ            PBMQ_tx;  /* Tx queue (one for each SER device) */
    PBMQ            PBMQ_rx;  /* Rx queue (one for each SER device) */

    PBM_Handle      hRxPend;  /* Packet being rx'd */
    unsigned char   *pRxBuf;  /* Pointer to write next char */
    uint32_t        RxCount;  /* Number of bytes received */
    uint16_t        RxCRC;    /* Receive CRC */
    unsigned char   RxFlag;   /* Flag to "un-escape" character */

    PBM_Handle      hTxPend;  /* Packet being tx'd */
    unsigned char   *pTxBuf;  /* Pointer to next char to send */
    uint32_t        TxCount;  /* Number of bytes left to send */
    uint16_t        TxCRC;    /* Transmit CRC */
    unsigned char   TxFlag;   /* Flag to insert character */
    unsigned char   TxChar;   /* Insert character */

    void (*cbTimer)(void *h);         /* HDLC Timer callback (when open) */
    void (*cbInput)(PBM_Handle hPkt); /* HDLC Input callback (when open) */

    /* Per device statistics */
    uint32_t SerRxGood;
    uint32_t SerRxBadCRC;

    Semaphore_Handle spIsrStop; /* Semaphore used to signal spIsrTask to exit */
    Semaphore_Handle spIsrDone; /* Used to signal it's OK to delete resources */
    Task_Handle spIsrTaskHandle; /* Handle to the spIsr Task */
} SDINFO;

/* Functions Provided by Hw specific code */
extern uint32_t HwSerInit(void);
extern void HwSerShutdown(void);
extern uint32_t HwSerOpen(SDINFO *pi);
extern void HwSerClose(SDINFO *pi);
extern void HwSerTxNext(SDINFO *pi);
extern void HwSerSetConfig(SDINFO *pi);
extern void _HwSerPoll(SDINFO *pi, uint32_t fTimerTick);
