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

#include <stdint.h>
#include <string.h>
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
#include <ti/drivers/ports/ListP.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC3200.h>

#include <ti/drivers/i2s/I2SCC3200DMA.h>

/* driverlib header files */
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <inc/hw_types.h>
#include <driverlib/rom_map.h>
#include <driverlib/rom.h>
#include <driverlib/prcm.h>
#include <driverlib/interrupt.h>
#include <inc/hw_mcasp.h>
#include <driverlib/i2s.h>
#include <driverlib/udma.h>


/* I2SCC3200DMA functions */
void        I2SCC3200DMA_close(I2S_Handle handle);
int         I2SCC3200DMA_control(I2S_Handle handle, unsigned int cmd,
                                 void *arg);
void        I2SCC3200DMA_init(I2S_Handle handle);
I2S_Handle  I2SCC3200DMA_open(I2S_Handle handle, I2S_Params *params);
int         I2SCC3200DMA_readIssue(I2S_Handle handle, I2S_BufDesc *desc);
size_t      I2SCC3200DMA_readReclaim(I2S_Handle handle, I2S_BufDesc **desc);
int         I2SCC3200DMA_writeIssue(I2S_Handle handle, I2S_BufDesc *desc);
size_t      I2SCC3200DMA_writeReclaim(I2S_Handle handle, I2S_BufDesc **desc);

/* I2S function table for I2SCC3200DMA implementation */
const I2S_FxnTable I2SCC3200DMA_fxnTable = {
    I2SCC3200DMA_close,
    I2SCC3200DMA_control,
    I2SCC3200DMA_init,
    I2SCC3200DMA_open,
    I2SCC3200DMA_readIssue,
    I2SCC3200DMA_readReclaim,
    I2SCC3200DMA_writeIssue,
    I2SCC3200DMA_writeReclaim
};

/*
 * This lookup table is used to configure the DMA channels for the appropriate
 * (16bit or 32bit) transfer sizes.
 * Table for an I2S DMA RX channel.
 */
static const uint32_t dmaRxConfig[] = {
    UDMA_SIZE_16 | UDMA_SRC_INC_NONE | UDMA_CHCTL_DSTINC_16 | UDMA_ARB_8,
    UDMA_SIZE_32 | UDMA_SRC_INC_NONE | UDMA_CHCTL_DSTINC_32 | UDMA_ARB_8
};

/*
 * This lookup table is used to configure the DMA channels for the appropriate
 * (16bit or 32bit) transfer sizes.
 * Table for an I2 DMA TX channel
 */
static const uint32_t dmaTxConfig[] = {
    UDMA_SIZE_16 | UDMA_CHCTL_SRCINC_16 | UDMA_DST_INC_NONE | UDMA_ARB_8,
    UDMA_SIZE_32 | UDMA_CHCTL_SRCINC_32 | UDMA_DST_INC_NONE | UDMA_ARB_8
};

/* Default I2S params */
extern const I2S_Params I2S_defaultParams;

/*Zero buffer to write when there is no data from the application*/
unsigned short I2SCC3200DMA_zeroBuffer[32];
I2S_BufDesc  I2SCC3200DMA_zeroBufDesc;

/*Empty buffer to read into when there is no data requested
  from the application*/
unsigned char I2SCC3200DMA_emptyBuffer[32];
I2S_BufDesc  I2SCC3200DMA_emptyBufDesc;


/*
 *  ======== I2SCC3200DMA_configDMA ========
 */
static void I2SCC3200DMA_configDMA(I2S_Handle handle, I2S_BufDesc *desc,
                                                            bool isWrite)
{
    I2SCC3200DMA_Object           *object = handle->object;
    I2SCC3200DMA_HWAttrs const    *hwAttrs = handle->hwAttrs;
    unsigned long                  channelControlOptions;
    uint32_t                       divider = 1;

    if (object->dmaSize == I2SCC3200DMA_16bit) {
        divider = 2;
    }
    else if (object->dmaSize == I2SCC3200DMA_32bit) {
        divider = 4;
    }

    Power_setConstraint(PowerCC3200_DISALLOW_DEEPSLEEP);

    if (isWrite) {
        channelControlOptions = dmaTxConfig[object->dmaSize];

        /* Setup ping-pong transfer */
        MAP_uDMAChannelControlSet(hwAttrs->txChannelIndex,
                                  channelControlOptions);

        MAP_uDMAChannelAttributeEnable(hwAttrs->txChannelIndex,
                                       UDMA_ATTR_USEBURST);

        MAP_uDMAChannelTransferSet(hwAttrs->txChannelIndex,
            UDMA_MODE_PINGPONG, (void *)desc->bufPtr,
            (void *)I2S_TX_DMA_PORT, ((desc->bufSize / 2) / divider));
        MAP_uDMAChannelEnable(hwAttrs->txChannelIndex);

        /* Pong Buffer */
        MAP_uDMAChannelControlSet(hwAttrs->txChannelIndex | UDMA_ALT_SELECT,
            channelControlOptions);

        MAP_uDMAChannelAttributeEnable(
	    hwAttrs->txChannelIndex | UDMA_ALT_SELECT, UDMA_ATTR_USEBURST);

        MAP_uDMAChannelTransferSet(hwAttrs->txChannelIndex | UDMA_ALT_SELECT,
            UDMA_MODE_PINGPONG,
            (void *)(((char *)desc->bufPtr) + (desc->bufSize / 2)),
            (void *)I2S_TX_DMA_PORT,
            ((desc->bufSize / 2) / divider));
        MAP_uDMAChannelEnable(hwAttrs->txChannelIndex | UDMA_ALT_SELECT);

        object->i2sWritePingPongMode = 0;


        MAP_I2STxFIFOEnable(hwAttrs->baseAddr,8,1);
        MAP_I2SEnable(hwAttrs->baseAddr,object->operationMode);

    }
    else {
        if((HWREG(hwAttrs->baseAddr + MCASP_O_PDIR) & 0x4000000) != 0x4000000) {
            I2SCC3200DMA_writeIssue(handle, &I2SCC3200DMA_zeroBufDesc);
        }

        channelControlOptions = dmaRxConfig[object->dmaSize];

        /* Setup ping-pong transfer */
        MAP_uDMAChannelControlSet(hwAttrs->rxChannelIndex,
                                   channelControlOptions);

        MAP_uDMAChannelAttributeEnable(
                                  hwAttrs->rxChannelIndex,UDMA_ATTR_USEBURST);

        MAP_uDMAChannelTransferSet(hwAttrs->rxChannelIndex,
            UDMA_MODE_PINGPONG, (void *)I2S_RX_DMA_PORT,
            (void *)desc->bufPtr, ((desc->bufSize / 2) / divider));
        MAP_uDMAChannelEnable(hwAttrs->rxChannelIndex);

        /* Pong Buffer */
        MAP_uDMAChannelControlSet(hwAttrs->rxChannelIndex | UDMA_ALT_SELECT,
            channelControlOptions);

        MAP_uDMAChannelAttributeEnable(
            hwAttrs->rxChannelIndex | UDMA_ALT_SELECT, UDMA_ATTR_USEBURST);

        MAP_uDMAChannelTransferSet(hwAttrs->rxChannelIndex | UDMA_ALT_SELECT,
            UDMA_MODE_PINGPONG, (void *)I2S_RX_DMA_PORT,
            (void *)(((char *)desc->bufPtr) + (desc->bufSize / 2)),
            ((desc->bufSize / 2) / divider));
        MAP_uDMAChannelEnable(hwAttrs->rxChannelIndex | UDMA_ALT_SELECT);

        object->i2sReadPingPongMode = 0;

        MAP_I2SRxFIFOEnable(hwAttrs->baseAddr,8,1);
        MAP_I2SEnable(hwAttrs->baseAddr,object->operationMode);

    }

    DebugP_log1("I2S:(%p) DMA transfer enabled", hwAttrs->baseAddr);

    if (isWrite) {
        DebugP_log3("I2S:(%p) DMA transmit, txBuf: %p; Count: %d",
            hwAttrs->baseAddr, (uintptr_t)(desc->bufPtr), desc->bufSize);
    }
    else {
        DebugP_log3("I2S:(%p) DMA receive, rxBuf: %p; Count: %d",
            hwAttrs->baseAddr, (uintptr_t)(desc->bufPtr), desc->bufSize);
    }
}

/*
 *  ======== I2SCC3200DMA_hwiIntFxn ========
 *  Hwi function that processes I2S interrupts.
 *
 *  @param(arg)         The I2S_Handle for this Hwi.
 */
static void I2SCC3200DMA_hwiIntFxn(uintptr_t arg)
{
    I2SCC3200DMA_Object        *object = ((I2S_Handle)arg)->object;
    I2SCC3200DMA_HWAttrs const *hwAttrs = ((I2S_Handle)arg)->hwAttrs;
    I2S_BufDesc         *descX;
    I2S_BufDesc         *descX1;
    uint32_t            divider = 1;
    tDMAControlTable   *pControlTable = MAP_uDMAControlBaseGet();

    if (object->dmaSize == I2SCC3200DMA_16bit) {
        divider = 2;
    }
    else if (object->dmaSize == I2SCC3200DMA_32bit) {
        divider = 4;
    }

    if (MAP_uDMAIntStatus() & 0x00000020) {
        //Clear the MCASP write interrupt
        MAP_I2SIntClear(hwAttrs->baseAddr,(unsigned long)(1<<31));

        if (((pControlTable[(hwAttrs->txChannelIndex & 0xF)].ulControl
                                            & UDMA_CHCTL_XFERMODE_M) == 0)){
            descX = (I2S_BufDesc *)ListP_head(&object->writeActiveQueue);
            DebugP_assert(descX != NULL);

            descX1 =(I2S_BufDesc *)ListP_next(&(descX->qElem));
            if ((descX1 == NULL) && (descX != &I2SCC3200DMA_zeroBufDesc)){
                descX1 = &I2SCC3200DMA_zeroBufDesc;
                ListP_put(&object->writeActiveQueue,&(descX1->qElem));
            }
            if ((descX == &I2SCC3200DMA_zeroBufDesc) && (descX1 == NULL)){
                MAP_uDMAChannelTransferSet(hwAttrs->txChannelIndex,
                             UDMA_MODE_PINGPONG,
                            (void *)descX->bufPtr, (void *)I2S_TX_DMA_PORT,
                            ((descX->bufSize / 2) / divider));
                MAP_uDMAChannelEnable(hwAttrs->txChannelIndex);
            }
            else{
                MAP_uDMAChannelTransferSet(hwAttrs->txChannelIndex,
                             UDMA_MODE_PINGPONG,
                            (void *)descX1->bufPtr, (void *)I2S_TX_DMA_PORT,
                            ((descX1->bufSize / 2) / divider));
                MAP_uDMAChannelEnable(hwAttrs->txChannelIndex);
            }

            object->i2sWritePingPongMode = 1;
        }
        else if ((pControlTable[
                ((hwAttrs->txChannelIndex & 0xF) | UDMA_ALT_SELECT)].ulControl
                                       & UDMA_CHCTL_XFERMODE_M) == 0) {
            descX = (I2S_BufDesc *)ListP_get(&object->writeActiveQueue);
            DebugP_assert(descX != NULL);

            descX1 = (I2S_BufDesc *)ListP_head(&object->writeActiveQueue);
            if ((descX == &I2SCC3200DMA_zeroBufDesc) && (descX1 == NULL)){
                ListP_put(&object->writeActiveQueue,&(descX->qElem));
                MAP_uDMAChannelTransferSet(
                                hwAttrs->txChannelIndex | UDMA_ALT_SELECT,
                                UDMA_MODE_PINGPONG,
                                (void *)((char*)descX->bufPtr +
                                         (descX->bufSize / 2)),
                                (void *)I2S_TX_DMA_PORT,
                                ((descX->bufSize / 2) / divider));
                MAP_uDMAChannelEnable(hwAttrs->txChannelIndex | UDMA_ALT_SELECT);
            }
            else {
                DebugP_assert(descX1 != NULL);
                MAP_uDMAChannelTransferSet(
                                hwAttrs->txChannelIndex | UDMA_ALT_SELECT,
                                UDMA_MODE_PINGPONG,
                                (void *)((char*)descX1->bufPtr +
                                         (descX1->bufSize / 2)),
                                (void *)I2S_TX_DMA_PORT,
                                ((descX1->bufSize / 2) / divider));
                MAP_uDMAChannelEnable(hwAttrs->txChannelIndex | UDMA_ALT_SELECT);

                object->serialPinVars[object->writeIndex].readWriteCallback(
                                                     (I2S_Handle)arg, descX);
            }

            object->i2sWritePingPongMode = 0;
        }

        DebugP_log2("I2S:(%p) Write finished, %d bytes written",
               hwAttrs->baseAddr, descX->bufSize);
    }
    if (MAP_uDMAIntStatus() & 0x00000010) {
        /* Clear the MCASP read interrupt */
        //MAP_I2SIntClear(hwAttrs->baseAddr,(unsigned long)(1<<30)); todo
        HWREG(0x4402609c) = (1<<10);

        if ((pControlTable[(hwAttrs->rxChannelIndex & 0xF)].ulControl
                                            & UDMA_CHCTL_XFERMODE_M) == 0) {
            descX = (I2S_BufDesc *)ListP_head(&object->readActiveQueue);
            DebugP_assert(descX != NULL);

            descX1 = (I2S_BufDesc *)ListP_next(&(descX->qElem));
            if ((descX1 == NULL) && (descX != &I2SCC3200DMA_emptyBufDesc)){
                descX1 = &I2SCC3200DMA_emptyBufDesc;
                ListP_put(&object->readActiveQueue,&(descX1->qElem));
            }
            if ((descX == &I2SCC3200DMA_emptyBufDesc) && (descX1 == NULL)){
                MAP_uDMAChannelTransferSet(hwAttrs->rxChannelIndex,
                             UDMA_MODE_PINGPONG,
                            (void *)I2S_RX_DMA_PORT, (void *)descX->bufPtr,
                            ((descX->bufSize / 2) / divider));
                MAP_uDMAChannelEnable(hwAttrs->rxChannelIndex);
            }
            else{
                MAP_uDMAChannelTransferSet(hwAttrs->rxChannelIndex,
                             UDMA_MODE_PINGPONG,
                            (void *)I2S_RX_DMA_PORT, (void *)descX1->bufPtr,
                            ((descX1->bufSize / 2) / divider));
                MAP_uDMAChannelEnable(hwAttrs->rxChannelIndex);
            }
            object->i2sReadPingPongMode = 1;
        }
        else if ((pControlTable[
                  ((hwAttrs->rxChannelIndex & 0xF) | UDMA_ALT_SELECT)].ulControl
                                            & UDMA_CHCTL_XFERMODE_M) == 0) {
            descX = (I2S_BufDesc *)ListP_get(&object->readActiveQueue);
            DebugP_assert(descX != NULL);

            descX1 = (I2S_BufDesc *)ListP_head(&object->readActiveQueue);
            if ((descX == &I2SCC3200DMA_emptyBufDesc) && (descX1 == NULL)){
                ListP_put(&object->readActiveQueue,&(descX->qElem));
                MAP_uDMAChannelTransferSet(
                    hwAttrs->rxChannelIndex | UDMA_ALT_SELECT,
                    UDMA_MODE_PINGPONG, (void *)I2S_RX_DMA_PORT,
                    (void *)((char*)descX->bufPtr + (descX->bufSize / 2)),
                    ((descX->bufSize / 2) / divider));
                MAP_uDMAChannelEnable(hwAttrs->rxChannelIndex | UDMA_ALT_SELECT);
            }
            else{
                DebugP_assert(descX1 != NULL);
                object->serialPinVars[object->readIndex].readWriteCallback(
                                                (I2S_Handle)arg, descX);
                MAP_uDMAChannelTransferSet(
                    hwAttrs->rxChannelIndex | UDMA_ALT_SELECT,
                    UDMA_MODE_PINGPONG, (void *)I2S_RX_DMA_PORT,
                    (void *)((char*)descX1->bufPtr + (descX1->bufSize / 2)),
                    ((descX1->bufSize / 2) / divider));
                MAP_uDMAChannelEnable(hwAttrs->rxChannelIndex | UDMA_ALT_SELECT);

            }
            object->i2sReadPingPongMode = 0;
        }

        DebugP_log2("I2S:(%p) Read finished, %d bytes read",
            hwAttrs->baseAddr, descX->bufSize);
    }
}

/*
 *  ======== I2SCC3200_Params_init ========
 */
void I2SCC3200DMA_Params_init(I2SCC3200DMA_SerialPinParams *params)
{
     DebugP_assert(params != NULL);

     params->serialPinConfig[0].pinNumber         = 0;
     params->serialPinConfig[0].pinMode           = I2S_PINMODE_TX;
     params->serialPinConfig[0].inActiveConfig    =
                                     I2S_SERCONFIG_INACT_LOW_LEVEL;


     params->serialPinConfig[1].pinNumber         = 1;
     params->serialPinConfig[1].pinMode           = I2S_PINMODE_RX;
     params->serialPinConfig[1].inActiveConfig    =
                                     I2S_SERCONFIG_INACT_LOW_LEVEL;

}

/*
 *  ======== readIssueCallback ========
 *  Simple callback to post a semaphore for the issue-reclaim mode.
 */
static void readIssueCallback(I2S_Handle handle, I2S_BufDesc *desc)
{
    I2SCC3200DMA_Object *object = handle->object;

    ListP_put(&object->readDoneQueue, &(desc->qElem));
    SemaphoreP_post(object->readSem);
}

/*
 *  ======== writeIssueCallback ========
 *  Simple callback to post a semaphore for the issue-reclaim mode.
 */
static void writeIssueCallback(I2S_Handle handle, I2S_BufDesc *desc)
{
    I2SCC3200DMA_Object *object = handle->object;

    ListP_put(&object->writeDoneQueue, &(desc->qElem));
    SemaphoreP_post(object->writeSem);
}

/*
 *  ======== I2SCC3200DMA_close ========
 */
void I2SCC3200DMA_close(I2S_Handle handle)
{
    I2SCC3200DMA_Object           *object = handle->object;
    I2SCC3200DMA_HWAttrs const    *hwAttrs = handle->hwAttrs;

    /* Disable I2S and interrupts. */
    I2SDisable(hwAttrs->baseAddr);

    if (object->hwiHandle) {
        HwiP_delete(object->hwiHandle);
    }
    if (object->writeSem != NULL) {
        SemaphoreP_delete(&(object->writeSem));
    }
    if (object->readSem != NULL){
        SemaphoreP_delete(&(object->readSem));
    }

    Power_releaseConstraint(PowerCC3200_DISALLOW_DEEPSLEEP);

    Power_releaseDependency(PowerCC3200_PERIPH_I2S);
    Power_releaseDependency(PowerCC3200_PERIPH_UDMA);


    object->opened = false;

    DebugP_log1("I2S:(%p) closed", hwAttrs->baseAddr);
}

/*
 *  ======== I2SCC3200DMA_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int I2SCC3200DMA_control(I2S_Handle handle, unsigned int cmd, void *arg)
{
    I2SCC3200DMA_Object *object = handle->object;
    uint32_t newSize = *(uint32_t *)arg;

    switch(cmd){
        case I2SCC3200DMA_SET_ZEROBUF_LEN:
            if (newSize > 32){
                return -1;
            }
            I2SCC3200DMA_zeroBufDesc.bufSize = newSize;
            object->zeroWriteBufLength       = newSize;
            return(I2SCC3200DMA_SET_ZEROBUF_LEN);

        case I2SCC3200DMA_SET_EMPTYBUF_LEN:
            if (newSize > 32){
                return -1;
            }
            I2SCC3200DMA_emptyBufDesc.bufSize = newSize;
            object->emptyReadBufLength        = newSize;
            return(I2SCC3200DMA_SET_EMPTYBUF_LEN);

        default:
            return (I2S_STATUS_UNDEFINEDCMD);
    }
}

/*
 *  ======== I2SCC3200DMA_init ========
 */
void I2SCC3200DMA_init(I2S_Handle handle)
{
    I2SCC3200DMA_Object *object = handle->object;

    object->opened = false;
}

/*
 *  ======== I2SCC3200DMA_open ========
 */
I2S_Handle I2SCC3200DMA_open(I2S_Handle handle, I2S_Params *params)
{
    uintptr_t                     key;
    unsigned int                  bitClock;
    unsigned int                  slotConfig;
    I2SCC3200DMA_Object          *object = handle->object;
    I2SCC3200DMA_HWAttrs const   *hwAttrs = handle->hwAttrs;
    I2SCC3200DMA_SerialPinParams *cc3200CustomParams;
    SemaphoreP_Params             semParams;
    HwiP_Params                   hwiParams;
    int                           i;
    unsigned long                 i2s_data_line[] = {
        I2S_DATA_LINE_0, I2S_DATA_LINE_1
    };

    /* If params are NULL use defaults. */
    if (params == NULL) {
        I2SCC3200DMA_SerialPinParams I2SCC3200DMA_defaultParams;

        params = (I2S_Params *) &I2S_defaultParams;
        I2SCC3200DMA_Params_init(&I2SCC3200DMA_defaultParams);
        params->customParams =
          (I2SCC3200DMA_SerialPinParams *)&I2SCC3200DMA_defaultParams;
    }

    cc3200CustomParams = (I2SCC3200DMA_SerialPinParams *)params->customParams;

    /* Disable preemption while checking if the I2S is open. */
    key = HwiP_disable();

    /* Check if the I2S is open already with the base addr. */
    if (object->opened == true) {
        HwiP_restore(key);

        DebugP_log1("I2S:(%p) already in use.", hwAttrs->baseAddr);
        return (NULL);
    }
    object->opened = true;

    HwiP_restore(key);

    /* Set I2S variables to defaults. */
    object->readSem = NULL;
    object->writeSem = NULL;
    object->writeIndex = I2SCC3200DMA_INDEX_INVALID;
    object->readIndex  = I2SCC3200DMA_INDEX_INVALID;

    object->emptyReadBufLength = 32;
    object->zeroWriteBufLength = 32;

    memset(&I2SCC3200DMA_zeroBuffer,0,sizeof(I2SCC3200DMA_zeroBuffer));

    I2SCC3200DMA_zeroBufDesc.bufPtr  = &I2SCC3200DMA_zeroBuffer;
    I2SCC3200DMA_zeroBufDesc.bufSize = object->zeroWriteBufLength;

    I2SCC3200DMA_emptyBufDesc.bufPtr  = &I2SCC3200DMA_emptyBuffer;
    I2SCC3200DMA_emptyBufDesc.bufSize = object->emptyReadBufLength;

    object->operationMode  = params->operationMode;

    /*
     *  Register power dependency. Keeps the clock running in SLP
     *  and DSLP modes.
     */
    Power_setDependency(PowerCC3200_PERIPH_I2S);
    Power_setDependency(PowerCC3200_PERIPH_UDMA);
    MAP_PRCMPeripheralReset(PRCM_I2S);

    for (i = 0; i < 2; i++) {
        if (cc3200CustomParams->serialPinConfig[i].pinMode !=
	        I2S_PINMODE_INACTIVE) {
            if (cc3200CustomParams->serialPinConfig[i].pinMode ==
	            I2S_PINMODE_RX) {
                object->readIndex = i;
                object->serialPinVars[i].readWriteMode = params->readMode;
                object->serialPinVars[i].readWriteCallback =
                     params->readCallback;
                object->serialPinVars[i].readWriteTimeout = params->readTimeout;

                MAP_I2SSerializerConfig(hwAttrs->baseAddr,i2s_data_line[i],
                    I2S_SER_MODE_RX, I2S_INACT_LOW_LEVEL);
            }
            else if (cc3200CustomParams->serialPinConfig[i].pinMode ==
                   I2S_PINMODE_TX) {
                object->writeIndex = i;
                object->serialPinVars[i].readWriteMode =
                    params->writeMode;
                object->serialPinVars[i].readWriteCallback =
                    params->writeCallback;
                object->serialPinVars[i].readWriteTimeout =
                    params->writeTimeout;
                MAP_I2SSerializerConfig(hwAttrs->baseAddr,i2s_data_line[i],
                    I2S_SER_MODE_TX, I2S_INACT_LOW_LEVEL);
            }
        }
    }

    /* Disable the I2S interrupt. */
    MAP_I2SIntDisable(hwAttrs->baseAddr,I2S_INT_XDATA | I2S_INT_RDATA);

    HwiP_clearInterrupt(hwAttrs->intNum);

    HwiP_Params_init(&hwiParams);
    hwiParams.arg = (uintptr_t)handle;
    hwiParams.priority = hwAttrs->intPriority;
    object->hwiHandle = HwiP_create(hwAttrs->intNum,
                                    I2SCC3200DMA_hwiIntFxn,
                                    &hwiParams);
    if (object->hwiHandle == NULL) {
        I2SCC3200DMA_close(handle);
        return (NULL);
    }

    MAP_I2SIntEnable(hwAttrs->baseAddr,I2S_INT_XDATA | I2S_INT_RDATA);

    if (params->operationMode == I2S_OPMODE_TX_ONLY) {
        object->operationMode = I2S_MODE_TX_ONLY;
    }
    else if (params->operationMode == I2S_OPMODE_TX_RX_SYNC) {
        object->operationMode = I2S_MODE_TX_RX_SYNC;
    }
    else {
        I2SCC3200DMA_close(handle);
        return (NULL);
    }
    if (params->slotLength == 16) {
        slotConfig = I2S_SLOT_SIZE_16;
        object->dmaSize = I2SCC3200DMA_16bit;
    }
    else if (params->slotLength == 24) {
        slotConfig = I2S_SLOT_SIZE_24;
        object->dmaSize = I2SCC3200DMA_32bit;
    }
    else {
        I2SCC3200DMA_close(handle);
        return (NULL);
    }

    SemaphoreP_Params_init(&semParams);
    if (object->writeIndex != I2SCC3200DMA_INDEX_INVALID) {
        /* Initialize write Queue */
        ListP_clearList(&object->writeActiveQueue);

        if (object->serialPinVars[object->writeIndex].readWriteMode ==
                                               I2S_MODE_ISSUERECLAIM) {
            object->serialPinVars[object->writeIndex].readWriteCallback =
                                                   &writeIssueCallback;
            semParams.mode = SemaphoreP_Mode_COUNTING;

            object->writeSem = SemaphoreP_create(0, &semParams);
            if (object->writeSem == NULL) {
                DebugP_log1("I2S:(%p) Failed to create semaphore.",
                    hwAttrs->baseAddr);
                I2SCC3200DMA_close(handle);
                return (NULL);
            }
        }

        /* Initialize write Queue */
        ListP_clearList(&object->writeDoneQueue);
    }

    if (object->readIndex != I2SCC3200DMA_INDEX_INVALID) {
        ListP_clearList(&object->readActiveQueue);

        if (object->serialPinVars[object->readIndex].readWriteMode ==
                I2S_MODE_ISSUERECLAIM) {
            object->serialPinVars[object->readIndex].readWriteCallback =
                                                      &readIssueCallback;
            semParams.mode = SemaphoreP_Mode_COUNTING;
            object->readSem = SemaphoreP_create(0, &semParams);

            if (object->readSem == NULL) {
                if (object->writeSem) {
                    SemaphoreP_delete(object->writeSem);

                    DebugP_log1("I2S:(%p) Failed to create semaphore.",
                        hwAttrs->baseAddr);
                    I2SCC3200DMA_close(handle);
                    return (NULL);
                }

                /* Initialize read Queues */
                ListP_clearList(&object->readDoneQueue);
            }
        }
    }

    bitClock = (params->samplingFrequency *
        params->slotLength * params->numChannels);
    MAP_PRCMI2SClockFreqSet(bitClock);

    MAP_I2SConfigSetExpClk(hwAttrs->baseAddr,bitClock,bitClock,slotConfig |
                                      I2S_PORT_DMA);

    MAP_uDMAChannelAssign(hwAttrs->rxChannelIndex);
    MAP_uDMAChannelAttributeDisable(hwAttrs->rxChannelIndex,
                                     UDMA_ATTR_ALTSELECT);

    MAP_uDMAChannelAssign(hwAttrs->txChannelIndex);
    MAP_uDMAChannelAttributeDisable(hwAttrs->txChannelIndex,
                                     UDMA_ATTR_ALTSELECT);


    DebugP_log1("I2S:(%p) opened", hwAttrs->baseAddr);

    /* Return the handle */
    return (handle);
}

/*
 *  ======== I2SCC3200DMA_readIssue ========
 */
int I2SCC3200DMA_readIssue(I2S_Handle handle, I2S_BufDesc *desc)
{
    I2SCC3200DMA_Object *object = handle->object;
    bool                 startNeeded = false;
    uintptr_t            key;

    if (object->readIndex == I2SCC3200DMA_INDEX_INVALID) {
        return (I2S_STATUS_UNDEFINEDCMD);
    }

    /* Disable preemption while modifying queues. */
    key = HwiP_disable();

    if (ListP_empty(&object->readActiveQueue)) {
        startNeeded = true;
    }

    ListP_put(&object->readActiveQueue, &(desc->qElem));

    HwiP_restore(key);

    if (startNeeded) {
        I2SCC3200DMA_configDMA(handle, desc,false);
    }

    return (0);
}

/*
 *  ======== I2SCC3200DMA_readReclaim ========
 */
size_t I2SCC3200DMA_readReclaim(I2S_Handle handle, I2S_BufDesc **desc)
{
    uintptr_t            key;
    I2SCC3200DMA_Object *object = handle->object;
    size_t               size = 0;

    *desc = NULL;
    if (SemaphoreP_pend(object->readSem,
            object->serialPinVars[object->readIndex].readWriteTimeout) !=
	    SemaphoreP_OK){

        /* Cancel the DMA without posting the semaphore */
        // TODO: Cancel DMA and move buffers to readDoneQueue

        DebugP_log1("I2S:(%p) Read timed out",
                ((I2SCC3200DMA_HWAttrs const *)(handle->hwAttrs))->baseAddr);
    }
    else {
        key = HwiP_disable();

        *desc = (I2S_BufDesc *)ListP_get(&object->readDoneQueue);

        HwiP_restore(key);

        DebugP_assert(*desc != NULL);
        size = (*desc)->bufSize;
    }
    return (size);
}

/*
 *  ======== I2SCC3200DMA_writeIssue ========
 */
int I2SCC3200DMA_writeIssue(I2S_Handle handle, I2S_BufDesc *desc)
{
    I2SCC3200DMA_Object  *object = handle->object;
    uintptr_t             key;
    bool                  startNeeded = false;

    if (object->writeIndex == I2SCC3200DMA_INDEX_INVALID) {
        return (I2S_STATUS_UNDEFINEDCMD);
    }

    key = HwiP_disable();

    if (ListP_empty(&object->writeActiveQueue)) {
        startNeeded = true;
    }

    ListP_put(&object->writeActiveQueue, &(desc->qElem));

    HwiP_restore(key);

    if (startNeeded) {
        I2SCC3200DMA_configDMA(handle, desc,true);
    }

    return (0);
}

/*
 *  ======== I2SCC3200DMA_writeReclaim ========
 */
size_t I2SCC3200DMA_writeReclaim(I2S_Handle handle, I2S_BufDesc **desc)
{
    uintptr_t            key;
    I2SCC3200DMA_Object *object = handle->object;
    size_t               size = 0;

    *desc = NULL;

    if (SemaphoreP_pend(object->writeSem,
            object->serialPinVars[object->writeIndex].readWriteTimeout) !=
	    SemaphoreP_OK){

        /* Cancel the DMA without posting the semaphore */
        // TODO: Cancel DMA and move buffers to writeDoneQueue

        DebugP_log1("I2S:(%p) Write timed out",
                ((I2SCC3200DMA_HWAttrs const *)(handle->hwAttrs))->baseAddr);

    }
    else {
        key = HwiP_disable();

        *desc = (I2S_BufDesc *)ListP_get(&object->writeDoneQueue);

        HwiP_restore(key);

        DebugP_assert(*desc != NULL);
        size = (*desc)->bufSize;
    }
    return (size);
}
