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

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC3200.h>

/*
 * By default disable both asserts and log for this module.
 * This must be done before DebugP.h is included.
 */
#ifndef DebugP_ASSERT_ENABLED
#define DebugP_ASSERT_ENABLED 0
#endif
#ifndef DebugP_LOG_ENABLED
#define DebugP_LOG_ENABLED 0
#endif
#include <ti/drivers/ports/DebugP.h>
#include <ti/drivers/ports/HwiP.h>
#include <ti/drivers/ports/SemaphoreP.h>

#include <ti/drivers/spi/SPICC3200DMA.h>

#include <inc/hw_mcspi.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>
#include <driverlib/prcm.h>
#include <driverlib/spi.h>
#include <driverlib/udma.h>

void        SPICC3200DMA_close(SPI_Handle handle);
int         SPICC3200DMA_control(SPI_Handle handle,
                                 unsigned int cmd,
                                 void *arg);
void        SPICC3200DMA_init(SPI_Handle handle);
SPI_Handle  SPICC3200DMA_open(SPI_Handle handle, SPI_Params *params);
void        SPICC3200DMA_serviceISR(SPI_Handle handle);
bool        SPICC3200DMA_transfer(SPI_Handle handle, SPI_Transaction *transaction);
void        SPICC3200DMA_transferCancel(SPI_Handle handle);
static void SPICC3200DMA_transferCallback(SPI_Handle handle,
                                          SPI_Transaction *transaction);
static void SPICC3200DMA_hwiFxn(uintptr_t arg);
static void SPICC3200DMA_initHw(SPI_Handle handle);
static int SPICC3200DMA_postNotify(unsigned int eventType, uintptr_t eventArg,
        uintptr_t clientArg);

static unsigned int getPowerMgrId(SPIBaseAddrType   baseAddr);

/* SPI function table for SPICC3200DMA implementation */
const SPI_FxnTable SPICC3200DMA_fxnTable = {
    SPICC3200DMA_close,
    SPICC3200DMA_control,
    SPICC3200DMA_init,
    SPICC3200DMA_open,
    SPICC3200DMA_transfer,
    SPICC3200DMA_transferCancel,
    SPICC3200DMA_serviceISR
};

static const uint32_t mode[] = {
    SPI_MODE_MASTER,    /* SPI_MASTER */
    SPI_MODE_SLAVE      /* SPI_SLAVE */
};

static const uint32_t frameFormat[] = {
    SPI_SUB_MODE_0,    /* SPI_POLO_PHA0 */
    SPI_SUB_MODE_1,    /* SPI_POLO_PHA1 */
    SPI_SUB_MODE_2,    /* SPI_POL1_PHA0 */
    SPI_SUB_MODE_3,    /* SPI_POL1_PHA1 */
    (0),               /* SPI_TI - not supported on CC3200 */
    (0)                /* SPI_MW - not supported on CC3200 */
};

static void *uDMAControlBase = NULL;

extern const SPI_Params SPI_defaultParams;

/*
 * This lookup table is used to configure the DMA channels for the appropriate
 * (8bit, 16bit or 32bit) transfer sizes.
 * Table for an SPI DMA RX channel.
 */
static const uint32_t dmaRxConfig[] = {
    UDMA_SIZE_8  | UDMA_SRC_INC_NONE | UDMA_DST_INC_8  | UDMA_ARB_1,
    UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_16 | UDMA_ARB_1,
    UDMA_SIZE_32 | UDMA_SRC_INC_NONE | UDMA_DST_INC_32 | UDMA_ARB_1
};

/*
 * This lookup table is used to configure the DMA channels for the appropriate
 * (8bit, 16bit or 32bit) transfer sizes.
 * Table for an SPI DMA TX channel
 */
static const uint32_t dmaTxConfig[] = {
    UDMA_SIZE_8  | UDMA_SRC_INC_8  | UDMA_DST_INC_NONE | UDMA_ARB_1,
    UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_1,
    UDMA_SIZE_32 | UDMA_SRC_INC_32 | UDMA_DST_INC_NONE | UDMA_ARB_1
};

/*
 * This lookup table is used to configure the DMA channels for the appropriate
 * (8bit, 16bit or 32bit) transfer sizes when either txBuf or rxBuf are NULL.
 */
static const uint32_t dmaNullConfig[] = {
    UDMA_SIZE_8  | UDMA_SRC_INC_NONE | UDMA_DST_INC_NONE | UDMA_ARB_1,
    UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_DST_INC_NONE | UDMA_ARB_1,
    UDMA_SIZE_32 | UDMA_SRC_INC_NONE | UDMA_DST_INC_NONE | UDMA_ARB_1
};

/*
 *  ======== SPICC3200DMA_close ========
 *  @pre    Function assumes that the handle is not NULL
 */
void SPICC3200DMA_close(SPI_Handle handle)
{
    uintptr_t                    key;
    SPICC3200DMA_Object         *object = handle->object;
    SPICC3200DMA_HWAttrs const  *hwAttrs = handle->hwAttrs;

    MAP_SPIDisable(hwAttrs->baseAddr);
    MAP_SPICSDisable(hwAttrs->baseAddr);

    MAP_SPIDmaDisable(hwAttrs->baseAddr, SPI_RX_DMA | SPI_TX_DMA);
    MAP_SPIFIFODisable(hwAttrs->baseAddr, SPI_RX_FIFO | SPI_TX_FIFO);

    /* Release power dependency on SPI. */
    Power_releaseDependency(getPowerMgrId(hwAttrs->baseAddr));
    Power_releaseDependency(PowerCC3200_PERIPH_UDMA);

    Power_unregisterNotify(&(object->notifyObj));

    if (object->hwiHandle) {
        HwiP_delete(object->hwiHandle);
    }
    if (object->transferComplete) {
        SemaphoreP_delete(object->transferComplete);
    }

    DebugP_log1("SPI:(%p) closed", hwAttrs->baseAddr);

    key = HwiP_disable();
    object->isOpen = false;
    HwiP_restore(key);
}

/*
 *  ======== SPICC3200DMA_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int SPICC3200DMA_control(SPI_Handle handle, unsigned int cmd, void *arg)
{
    /* No implementation yet */
    return (SPI_STATUS_UNDEFINEDCMD);
}

/*
 *  ======== SPICC3200DMA_configDMA ========
 *  This functions configures the transmit and receive DMA channels for a given
 *  SPI_Handle and SPI_Transaction
 *
 *  @pre    Function assumes that the handle and transaction is not NULL
 */
static void SPICC3200DMA_configDMA(SPI_Handle handle, SPI_Transaction *transaction)
{
    uintptr_t                   key;
    SPIDataType                 dummy;
    void                       *buf;
    uint32_t                    channelControlOptions;
    SPICC3200DMA_Object        *object = handle->object;
    SPICC3200DMA_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* Clear out the FIFO */
    while (MAP_SPIDataGetNonBlocking(hwAttrs->baseAddr, &dummy)) {}

    /* Configure DMA for RX */
    MAP_uDMAChannelAssign(hwAttrs->rxChannelIndex);
    MAP_uDMAChannelAttributeDisable(hwAttrs->rxChannelIndex, UDMA_ATTR_ALTSELECT);

    if (transaction->rxBuf) {
        channelControlOptions = dmaRxConfig[object->frameSize];
        buf = transaction->rxBuf;
    }
    else {
        channelControlOptions = dmaNullConfig[object->frameSize];
        buf = hwAttrs->scratchBufPtr;
    }
    MAP_uDMAChannelControlSet(hwAttrs->rxChannelIndex | UDMA_PRI_SELECT,
                              channelControlOptions);
    MAP_uDMAChannelTransferSet(hwAttrs->rxChannelIndex | UDMA_PRI_SELECT,
                               UDMA_MODE_BASIC,
                               (void *)(hwAttrs->baseAddr + MCSPI_O_RX0),
                               buf,
                               transaction->count);

    /* Configure DMA for TX */
    MAP_uDMAChannelAssign(hwAttrs->txChannelIndex);
    MAP_uDMAChannelAttributeDisable(hwAttrs->txChannelIndex, UDMA_ATTR_ALTSELECT);

    if (transaction->txBuf) {
        channelControlOptions = dmaTxConfig[object->frameSize];
        buf = transaction->txBuf;
    }
    else {
        channelControlOptions = dmaNullConfig[object->frameSize];
        *hwAttrs->scratchBufPtr = hwAttrs->defaultTxBufValue;
        buf = hwAttrs->scratchBufPtr;
    }
    MAP_uDMAChannelControlSet(hwAttrs->txChannelIndex | UDMA_PRI_SELECT,
                              channelControlOptions);
    MAP_uDMAChannelTransferSet(hwAttrs->txChannelIndex | UDMA_PRI_SELECT,
                               UDMA_MODE_BASIC,
                               buf,
                               (void *)(hwAttrs->baseAddr + MCSPI_O_TX0),
                               transaction->count);

    DebugP_log1("SPI:(%p) DMA transfer enabled", hwAttrs->baseAddr);
    DebugP_log4("SPI:   DMA transaction: %p, rxBuf: %p; txBuf: %p; Count: %d",
                (uintptr_t)transaction, (uintptr_t)transaction->rxBuf,
                (uintptr_t)transaction->txBuf, (uintptr_t)transaction->count);

    key = HwiP_disable();
    MAP_uDMAChannelEnable(hwAttrs->rxChannelIndex);
    MAP_uDMAChannelEnable(hwAttrs->txChannelIndex);
    HwiP_restore(key);

    MAP_SPIWordCountSet(hwAttrs->baseAddr, transaction->count);
}

/*
*  ======== SPICC3200DMA_hwiFxn ========
*  ISR for the SPI when we use the DMA is used.
*/
void SPICC3200DMA_hwiFxn(uintptr_t arg)
{
    SPI_Transaction            *msg;
    SPICC3200DMA_Object        *object = ((SPI_Handle)arg)->object;
    SPICC3200DMA_HWAttrs const *hwAttrs = ((SPI_Handle)arg)->hwAttrs;
    uint32_t                    intCode = 0;

    DebugP_log1("SPI:(%p) interrupt context start", hwAttrs->baseAddr);

    intCode = MAP_SPIIntStatus(hwAttrs->baseAddr, false);
    if (intCode & SPI_INT_DMATX) {
        /* DMA finished transfering; clear & disable interrupt */
        MAP_SPIIntDisable(hwAttrs->baseAddr, SPI_INT_DMATX);
        MAP_SPIIntClear(hwAttrs->baseAddr, SPI_INT_DMATX);
    }

    /* Determine if the TX & RX DMA channels have completed */
    if ((object->transaction) &&
            (MAP_uDMAChannelIsEnabled(hwAttrs->rxChannelIndex) == false) &&
            (MAP_uDMAIntStatus() & (1 << hwAttrs->rxChannelIndex))) {

        MAP_SPIDisable(hwAttrs->baseAddr);
        MAP_SPICSDisable(hwAttrs->baseAddr);

        MAP_SPIIntDisable(hwAttrs->baseAddr, SPI_INT_DMARX);
        MAP_SPIIntClear(hwAttrs->baseAddr, SPI_INT_DMARX);
        MAP_SPIIntClear(hwAttrs->baseAddr, SPI_INT_EOW);

        /*
         * Clear any pending interrupt
         * As the TX DMA channel interrupt gets service, it may be possible
         * that the RX DMA channel finished in the meantime, which means
         * the IRQ for RX DMA channel is still pending...
         */
        HwiP_clearInterrupt(hwAttrs->intNum);

        /*
         * Use a temporary transaction pointer in case the callback function
         * attempts to perform another SPI_transfer call
         */
        msg = object->transaction;

        /* Indicate we are done with this transfer */
        object->transaction = NULL;

        /* Release constraint since transaction is done */
        Power_releaseConstraint(PowerCC3200_DISALLOW_DEEPSLEEP);

        DebugP_log2("SPI:(%p) DMA transaction: %p complete",
                    hwAttrs->baseAddr, (uintptr_t)msg);

        object->transferCallbackFxn((SPI_Handle)arg, msg);
    }

    DebugP_log1("SPI:(%p) interrupt context end", hwAttrs->baseAddr);
}

/*
 *  ======== SPICC3200DMA_init ========
 *  @pre    Function assumes that the handle is not NULL
 */
void SPICC3200DMA_init(SPI_Handle handle)
{
    /* Mark the object as available */
    ((SPICC3200DMA_Object *)(handle->object))->isOpen = false;
}

/*
 *  ======== SPICC3200DMA_open ========
 *  @pre    Function assumes that the handle is not NULL
 */
SPI_Handle SPICC3200DMA_open(SPI_Handle handle, SPI_Params *params)
{
    uintptr_t                   key;
    SPICC3200DMA_Object        *object = handle->object;
    SPICC3200DMA_HWAttrs const *hwAttrs = handle->hwAttrs;
    SemaphoreP_Params           semParams;
    HwiP_Params                 hwiParams;

    key = HwiP_disable();
    if(object->isOpen == true) {
        HwiP_restore(key);
        return (NULL);
    }

    object->isOpen = true;
    HwiP_restore(key);

    if (params == NULL) {
        params = (SPI_Params *) &SPI_defaultParams;
    }

    DebugP_assert((params->dataSize >= 4) && (params->dataSize <= 32));

    /* Determine if we need to use an 8, 16 or 32-bit frameSize for the DMA */
    if (params->dataSize <= 8) {
        object->frameSize = SPICC3200DMA_8bit;
    }
    else if (params->dataSize <= 16) {
        object->frameSize = SPICC3200DMA_16bit;
    }
    else {
        object->frameSize = SPICC3200DMA_32bit;
    }

    /* Store SPI mode of operation */
    object->spiMode = params->mode;
    object->transferMode = params->transferMode;
    object->transaction = NULL;
    object->rxFifoTrigger = (1 << object->frameSize);
    object->txFifoTrigger = (1 << object->frameSize);
    object->bitRate = params->bitRate;
    object->frameFormat = params->frameFormat;
    object->dataSize = params->dataSize;

    /* Register power dependency - i.e. power up and enable clock for SPI. */
    Power_setDependency(getPowerMgrId(hwAttrs->baseAddr));
    Power_setDependency(PowerCC3200_PERIPH_UDMA);

    Power_registerNotify(&(object->notifyObj), PowerCC3200_AWAKE_LPDS,
            SPICC3200DMA_postNotify, (uintptr_t)handle);

    HwiP_Params_init(&hwiParams);
    hwiParams.arg = (uintptr_t)handle;
    hwiParams.priority = hwAttrs->intPriority;
    object->hwiHandle = HwiP_create(hwAttrs->intNum,
                                    SPICC3200DMA_hwiFxn,
                                    &hwiParams);
    if (object->hwiHandle == NULL) {
        SPICC3200DMA_close(handle);
        return (NULL);
    }

    if (object->transferMode == SPI_MODE_BLOCKING) {
        DebugP_log1("SPI:(%p) in SPI_MODE_BLOCKING mode", hwAttrs->baseAddr);

        SemaphoreP_Params_init(&semParams);
        semParams.mode = SemaphoreP_Mode_BINARY;
        object->transferComplete = SemaphoreP_create(0, &semParams);
        if (object->transferComplete == NULL) {
            SPICC3200DMA_close(handle);
            return (NULL);
        }

        object->transferCallbackFxn = SPICC3200DMA_transferCallback;
    }
    else {
        DebugP_log1("SPI:(%p) in SPI_MODE_CALLBACK mode", hwAttrs->baseAddr);

        DebugP_assert(params->transferCallbackFxn != NULL);
        object->transferCallbackFxn = params->transferCallbackFxn;
    }

    if (uDMAControlBase == NULL) {
        uDMAControlBase = MAP_uDMAControlBaseGet();
    }

    SPICC3200DMA_initHw(handle);

    DebugP_log3("SPI:(%p) CPU freq: %d; SPI freq to %d",
                hwAttrs->baseAddr, PRCMPeripheralClockGet(hwAttrs->spiPRCM),
                params->bitRate);
    DebugP_log1("SPI:(%p) opened", hwAttrs->baseAddr);

    return (handle);
}

/*
 *  ======== SPICC3200DMA_serviceISR ========
 */
void SPICC3200DMA_serviceISR(SPI_Handle handle)
{
    /* Function is not supported */
    DebugP_assert(false);
}

/*
 *  ======== SPICC3200DMA_transfer ========
 *  @pre    Function assumes that handle and transaction is not NULL
 */
bool SPICC3200DMA_transfer(SPI_Handle handle, SPI_Transaction *transaction)
{
    uintptr_t                   key;
    SPICC3200DMA_Object        *object = handle->object;
    SPICC3200DMA_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* This is a limitation by the micro DMA controller */
    if ((transaction->count == 0) || (transaction->count > 1024) ||
        !(transaction->rxBuf || transaction->txBuf) ||
        (!(transaction->rxBuf && transaction->txBuf) && !hwAttrs->scratchBufPtr)) {
        return (false);
    }

    /* Check if a transfer is in progress */
    key = HwiP_disable();
    if (object->transaction) {
        HwiP_restore(key);
        DebugP_log1("SPI:(%p) ERROR! Transaction still in progress",
                    ((SPICC3200DMA_HWAttrs const *)(handle->hwAttrs))->baseAddr);

        return (false);
    }
    else {
        object->transaction = transaction;
        HwiP_restore(key);
    }

    /* Set constraints to guarantee transaction */
    Power_setConstraint(PowerCC3200_DISALLOW_DEEPSLEEP);

    SPICC3200DMA_configDMA(handle, transaction);
    MAP_SPIIntClear(hwAttrs->baseAddr, SPI_INT_DMARX | SPI_INT_DMATX | SPI_INT_EOW);
    MAP_SPIIntEnable(hwAttrs->baseAddr, SPI_INT_DMARX | SPI_INT_DMATX | SPI_INT_EOW);

    MAP_SPIEnable(hwAttrs->baseAddr);
    MAP_SPICSEnable(hwAttrs->baseAddr);

    if (object->transferMode == SPI_MODE_BLOCKING) {
        DebugP_log1("SPI:(%p) transfer pending on transferComplete semaphore",
                    ((SPICC3200DMA_HWAttrs const *)(handle->hwAttrs))->baseAddr);

        SemaphoreP_pend(object->transferComplete, SemaphoreP_WAIT_FOREVER);
    }

    return (true);
}

/*
 *  ======== SPICC3200DMA_transferCancel ========
 *  A function to cancel a transaction (if one is in progress) when the driver
 *  is in SPI_MODE_CALLBACK.
 *
 *  @pre    Function assumes that the handle is not NULL
 */
void SPICC3200DMA_transferCancel(SPI_Handle handle)
{
    /* No implementation yet */
    DebugP_assert(false);
}

/*
 *  ======== SPICC3200DMA_transferCallback ========
 *  Callback function for when the SPI is in SPI_MODE_BLOCKING
 *
 *  @pre    Function assumes that the handle is not NULL
 */
static void SPICC3200DMA_transferCallback(SPI_Handle handle,
                                          SPI_Transaction *transaction)
{
    SPICC3200DMA_Object *object = handle->object;

    DebugP_log1("SPI:(%p) posting transferComplete semaphore",
                ((SPICC3200DMA_HWAttrs const *)(handle->hwAttrs))->baseAddr);

    SemaphoreP_post(object->transferComplete);
}

/*
 *  ======== SPICC3200DMA_postNotify ========
 *  This functions is called to notify the SPI driver of an ongoing transition
 *  out of LPDS mode.
 *  clientArg should be pointing to a hardware module which has already
 *  been opened.
 */
static int SPICC3200DMA_postNotify(unsigned int eventType, uintptr_t eventArg,
        uintptr_t clientArg)
{
    /* Reconfigure the hardware when returning from LPDS */
    MAP_uDMAEnable();
    MAP_uDMAControlBaseSet(uDMAControlBase);

    SPICC3200DMA_initHw((SPI_Handle)clientArg);

    return (Power_NOTIFYDONE);
}

/*
 *  ======== SPICC3200DMA_initHw ========
 */
static void SPICC3200DMA_initHw(SPI_Handle handle)
{
    SPICC3200DMA_Object        *object = handle->object;
    SPICC3200DMA_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* Configure the SPI peripheral */
    MAP_SPICSDisable(hwAttrs->baseAddr);
    MAP_SPIDisable(hwAttrs->baseAddr);
    MAP_SPIReset(hwAttrs->baseAddr);

    /*
     * To support 4-32 bit lengths, object->dataSize must be formatted to meet
     * hardware requirement.
     */
    MAP_SPIConfigSetExpClk(hwAttrs->baseAddr,
                           MAP_PRCMPeripheralClockGet(hwAttrs->spiPRCM),
                           object->bitRate,
                           mode[object->spiMode],
                           frameFormat[object->frameFormat],
                           (hwAttrs->csControl | hwAttrs->pinMode |
                            hwAttrs->turboMode | hwAttrs->csPolarity |
                           ((object->dataSize - 1) << 7)));

    /* Enable FIFOs, DMA, & interrupts  */
    MAP_SPIFIFOEnable(hwAttrs->baseAddr, SPI_RX_FIFO | SPI_TX_FIFO);
    MAP_SPIDmaEnable(hwAttrs->baseAddr, SPI_RX_DMA | SPI_TX_DMA);
    MAP_SPIFIFOLevelSet(hwAttrs->baseAddr, object->txFifoTrigger,
                        object->rxFifoTrigger);
}

/*
 *  ======== getPowerMgrId ========
 */
static unsigned int getPowerMgrId(SPIBaseAddrType baseAddr)
{
    switch (baseAddr) {
        case GSPI_BASE:
            return (PowerCC3200_PERIPH_GSPI);
        case LSPI_BASE:
            return (PowerCC3200_PERIPH_LSPI);
        case SSPI_BASE:
            return (PowerCC3200_PERIPH_SSPI);
        default:
            return ((unsigned int)-1);
    }
}
