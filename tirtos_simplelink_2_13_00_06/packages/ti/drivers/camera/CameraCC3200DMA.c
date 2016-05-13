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

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC3200.h>

#include <ti/drivers/camera/CameraCC3200DMA.h>

/* driverlib header files */
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <inc/hw_types.h>
#include <driverlib/rom_map.h>
#include <driverlib/rom.h>
#include <driverlib/prcm.h>
#include <driverlib/camera.h>
#include <driverlib/interrupt.h>
#include <driverlib/udma.h>
#include <driverlib/utils.h>

#define CAM_BT_CORRECT_EN   0x00001000

/* CameraCC3200DMA functions */
void           CameraCC3200DMA_close(Camera_Handle handle);
int            CameraCC3200DMA_control(Camera_Handle handle,
                                       unsigned int cmd, void *arg);
void           CameraCC3200DMA_init(Camera_Handle handle);
Camera_Handle  CameraCC3200DMA_open(Camera_Handle handle,
                                    Camera_Params *params);
int            CameraCC3200DMA_capture(Camera_Handle handle, void *buffer,
                                                     unsigned int bufferlen);

/* Camera function table for CameraCC3200DMA implementation */
const Camera_FxnTable CameraCC3200DMA_fxnTable = {
    CameraCC3200DMA_close,
    CameraCC3200DMA_control,
    CameraCC3200DMA_init,
    CameraCC3200DMA_open,
    CameraCC3200DMA_capture
};

/* Default Camera params */
extern const Camera_Params Camera_defaultParams;

/*
 *  ======== CameraCC3200DMA_configDMA ========
 */
static void CameraCC3200DMA_configDMA(Camera_Handle handle)
{
    CameraCC3200DMA_Object           *object = handle->object;
    CameraCC3200DMA_HWAttrs const    *hwAttrs = handle->hwAttrs;
    unsigned long **bufferPtr = (unsigned long**)&object->captureBuf;


    /* Setup ping-pong transfer */
    MAP_uDMAChannelControlSet(hwAttrs->channelIndex,
              UDMA_SIZE_32 | UDMA_SRC_INC_32 | UDMA_DST_INC_32 | UDMA_ARB_8);
    MAP_uDMAChannelAttributeEnable(hwAttrs->channelIndex,UDMA_ATTR_USEBURST);
    MAP_uDMAChannelTransferSet(hwAttrs->channelIndex, UDMA_MODE_PINGPONG,
                               (void *)CAM_BUFFER_ADDR, (void *)*bufferPtr,
                               CameraCC3200DMA_DMA_TRANSFER_SIZE);
    MAP_uDMAChannelEnable(hwAttrs->channelIndex);

    /* Pong Buffer */
    *bufferPtr += CameraCC3200DMA_DMA_TRANSFER_SIZE;
    MAP_uDMAChannelControlSet(hwAttrs->channelIndex | UDMA_ALT_SELECT,
              UDMA_SIZE_32 | UDMA_SRC_INC_32 | UDMA_DST_INC_32 | UDMA_ARB_8);
    MAP_uDMAChannelAttributeEnable(hwAttrs->channelIndex | UDMA_ALT_SELECT,
                                                       UDMA_ATTR_USEBURST);
    MAP_uDMAChannelTransferSet(hwAttrs->channelIndex | UDMA_ALT_SELECT,
                                UDMA_MODE_PINGPONG,
                               (void *)CAM_BUFFER_ADDR,
                               (void *)*bufferPtr,
                               CameraCC3200DMA_DMA_TRANSFER_SIZE);
    MAP_uDMAChannelEnable(hwAttrs->channelIndex | UDMA_ALT_SELECT);

    /*  Ping Buffer */
    *bufferPtr += CameraCC3200DMA_DMA_TRANSFER_SIZE;

    DebugP_log1("Camera:(%p) DMA transfer enabled", hwAttrs->baseAddr);

    /* Set mode to Ping buffer initially */
    object->cameraDMA_PingPongMode = 0;

    /* Clear any pending interrupt */
    MAP_CameraIntClear(hwAttrs->baseAddr, CAM_INT_DMA);

    /* DMA Interrupt unmask from apps config */
    MAP_CameraIntEnable(hwAttrs->baseAddr, CAM_INT_DMA);

    DebugP_log3("Camera:(%p) DMA receive, "
                   "CaptureBuf: %p; Count: %d",
                    hwAttrs->baseAddr,
                    (uintptr_t)*bufferPtr,
                    (uintptr_t)object->bufferlength);
}

/*
 *  ======== captureSemCallback ========
 *  Simple callback to post a semaphore for the blocking mode.
 */
static void captureSemCallback(Camera_Handle handle, void *buffer, size_t count)
{
    CameraCC3200DMA_Object *object = handle->object;

    SemaphoreP_post(object->captureSem);
}

/*
 *  ======== CameraCC3200DMA_hwiIntFxn ========
 *  Hwi function that processes Camera interrupts.
 *
 *  The Frame end,DMA interrupts is enabled for the camera.
 *  The DMA interrupt is triggered for every 64 elements.
 *  The ISR will check for Frame end interrupt to trigger the callback
 *  in the non-blocking mode/post a semaphore in the blocking mode to
 *  indicate capture complete.
 *
 *  @param(arg)         The Camera_Handle for this Hwi.
 */
static void CameraCC3200DMA_hwiIntFxn(uintptr_t arg)
{
    uint32_t                     status;
    CameraCC3200DMA_Object        *object = ((Camera_Handle)arg)->object;
    CameraCC3200DMA_HWAttrs const *hwAttrs = ((Camera_Handle)arg)->hwAttrs;
    unsigned long **bufferPtr = (unsigned long**)&object->captureBuf;

    status = MAP_CameraIntStatus(hwAttrs->baseAddr);
    if ((object->cameraDMAxIntrRcvd > 1) && (status & (CAM_INT_FE))) {
        DebugP_log2("Camera:(%p) Interrupt with mask 0x%x",
               hwAttrs->baseAddr,status);

        MAP_CameraIntClear(hwAttrs->baseAddr, CAM_INT_FE);
        object->captureCallback((Camera_Handle)arg, *bufferPtr,
                                 object->frameLength);
        DebugP_log2("Camera:(%p) capture finished, %d bytes written",
                hwAttrs->baseAddr, object->frameLength);
        object->inUse = 0;

        MAP_CameraCaptureStop(hwAttrs->baseAddr, true);

        Power_releaseConstraint(PowerCC3200_DISALLOW_DEEPSLEEP);
    }

    if (status & CAM_INT_DMA) {
        // Camera DMA Done clear
        MAP_CameraIntClear(hwAttrs->baseAddr, CAM_INT_DMA);

        object->cameraDMAxIntrRcvd++;

        object->frameLength +=
         (CameraCC3200DMA_DMA_TRANSFER_SIZE*sizeof(unsigned long));
        if (object->frameLength < object->bufferlength) {
             if (object->cameraDMA_PingPongMode == 0) {
                MAP_uDMAChannelControlSet(hwAttrs->channelIndex,
                UDMA_SIZE_32 | UDMA_SRC_INC_32 | UDMA_DST_INC_32 | UDMA_ARB_8);

                MAP_uDMAChannelAttributeEnable(hwAttrs->channelIndex,
                                                 UDMA_ATTR_USEBURST);
                MAP_uDMAChannelTransferSet(hwAttrs->channelIndex,
                                           UDMA_MODE_PINGPONG,
                                           (void *)CAM_BUFFER_ADDR,
                                           (void *)*bufferPtr,
                                           CameraCC3200DMA_DMA_TRANSFER_SIZE);
                MAP_uDMAChannelEnable(hwAttrs->channelIndex);
                *bufferPtr += CameraCC3200DMA_DMA_TRANSFER_SIZE;
                object->cameraDMA_PingPongMode = 1;
            }
            else {
                MAP_uDMAChannelControlSet(hwAttrs->channelIndex | UDMA_ALT_SELECT,
                  UDMA_SIZE_32 | UDMA_SRC_INC_32 | UDMA_DST_INC_32 | UDMA_ARB_8);

                MAP_uDMAChannelAttributeEnable(
                    hwAttrs->channelIndex | UDMA_ALT_SELECT,
                    UDMA_ATTR_USEBURST);
                MAP_uDMAChannelTransferSet(
                    hwAttrs->channelIndex | UDMA_ALT_SELECT,
                    UDMA_MODE_PINGPONG,
                    (void *)CAM_BUFFER_ADDR, (void *)*bufferPtr,
                    CameraCC3200DMA_DMA_TRANSFER_SIZE);
                MAP_uDMAChannelEnable(hwAttrs->channelIndex | UDMA_ALT_SELECT);
                *bufferPtr += CameraCC3200DMA_DMA_TRANSFER_SIZE;
                object->cameraDMA_PingPongMode = 0;
            }
        }
        else {
            // Disable DMA
            MAP_UtilsDelay(20000);
            MAP_uDMAChannelDisable(hwAttrs->channelIndex);
            MAP_CameraIntDisable(hwAttrs->baseAddr, CAM_INT_DMA);
            object->cameraDMA_PingPongMode = 0;
            object->captureCallback((Camera_Handle)arg, *bufferPtr,
                                     object->frameLength);
            DebugP_log2("Camera:(%p) capture finished, %d bytes written",
                    hwAttrs->baseAddr, object->frameLength);

            MAP_CameraCaptureStop(hwAttrs->baseAddr, true);

            Power_releaseConstraint(PowerCC3200_DISALLOW_DEEPSLEEP);
        }
    }
}

/*
 *  ======== CameraCC3200DMA_init ========
 */
void CameraCC3200DMA_init(Camera_Handle handle)
{
    CameraCC3200DMA_Object *object = handle->object;

    object->opened = false;
}

/*
 *  ======== CameraCC3200DMA_open ========
 */
Camera_Handle CameraCC3200DMA_open(Camera_Handle handle, Camera_Params *params)
{
    uintptr_t                      key;
    CameraCC3200DMA_Object        *object = handle->object;
    CameraCC3200DMA_HWAttrs const *hwAttrs = handle->hwAttrs;
    unsigned long                  hSyncPolarityConfig;
    unsigned long                  vSyncPolarityConfig;
    unsigned long                  byteOrderConfig;
    unsigned long                  interfaceSync;
    unsigned long                  pixelClkConfig;
    HwiP_Params                    hwiParams;
    SemaphoreP_Params              semParams;

    /* If params are NULL use defaults. */
    if (params == NULL) {
        params = (Camera_Params *) &Camera_defaultParams;
    }

    /* Timeouts cannot be 0 */
    DebugP_assert((params->captureTimeout != 0));

    /* Check that a callback is set */
    DebugP_assert((params->captureMode != Camera_MODE_CALLBACK) ||
                  (params->captureCallback != NULL));

    /* Disable preemption while checking if the Camera is open. */
    key = HwiP_disable();

    /* Check if the Camera is open already with the base addr. */
    if (object->opened == true) {
        HwiP_restore(key);

        DebugP_log1("Camera:(%p) already in use.", hwAttrs->baseAddr);
        return (NULL);
    }

    object->opened = true;
    HwiP_restore(key);

    object->operationMode    = params->captureMode;
    object->captureCallback  = params->captureCallback;
    object->captureTimeout   = params->captureTimeout;

    /* Set Camera variables to defaults. */
    object->captureBuf = NULL;
    object->bufferlength = 0;
    object->frameLength  = 0;
    object->inUse        = 0;

    /*
     *  Register power dependency. Keeps the clock running in SLP
     *  and DSLP modes.
     */
    Power_setDependency(PowerCC3200_PERIPH_CAMERA);
    Power_setDependency(PowerCC3200_PERIPH_UDMA);

    /* Disable the Camera interrupt. */
    MAP_CameraIntDisable(hwAttrs->baseAddr, (CAM_INT_FE | CAM_INT_DMA));

    HwiP_clearInterrupt(hwAttrs->intNum);

    /* Create Hwi object for the Camera peripheral. */
    /* Register the interrupt for this Camera peripheral. */
    HwiP_Params_init(&hwiParams);
    hwiParams.arg = (uintptr_t)handle;
    hwiParams.priority = hwAttrs->intPriority;
    object->hwiHandle = HwiP_create(hwAttrs->intNum, CameraCC3200DMA_hwiIntFxn,
                                    &hwiParams);
    if (object->hwiHandle == NULL) {
        CameraCC3200DMA_close(handle);
        return (NULL);
    }

    MAP_IntEnable(INT_CAMERA);

    SemaphoreP_Params_init(&semParams);
    semParams.mode = SemaphoreP_Mode_BINARY;

    /* If capture is blocking create a semaphore and set callback. */
    if (object->operationMode == Camera_MODE_BLOCKING) {
        object->captureSem = SemaphoreP_create(0, &semParams);
        if (object->captureSem) {
            CameraCC3200DMA_close(handle);
            return (NULL);
        }
        object->captureCallback = &captureSemCallback;
    }

    MAP_CameraReset(hwAttrs->baseAddr);

    if (params->hsyncPolarity == Camera_HSYNC_POLARITY_HIGH) {
        hSyncPolarityConfig = CAM_HS_POL_HI;
    }
    else {
        hSyncPolarityConfig = CAM_HS_POL_LO;
    }

    if (params->vsyncPolarity == Camera_VSYNC_POLARITY_HIGH) {
        vSyncPolarityConfig = CAM_VS_POL_HI;
    }
    else {
        vSyncPolarityConfig = CAM_VS_POL_LO;
    }

    if (params->byteOrder == Camera_BYTE_ORDER_SWAP) {
        byteOrderConfig = CAM_ORDERCAM_SWAP;
    }
    else {
        byteOrderConfig = 0;
    }

    if (params->interfaceSync == Camera_INTERFACE_SYNC_OFF) {
        interfaceSync = CAM_NOBT_SYNCHRO;
    }
    else {
        interfaceSync = CAM_NOBT_SYNCHRO | CAM_IF_SYNCHRO | CAM_BT_CORRECT_EN;
    }

    if (params->pixelClkConfig == Camera_PCLK_CONFIG_RISING_EDGE) {
        pixelClkConfig = CAM_PCLK_RISE_EDGE;
    }
    else {
        pixelClkConfig = CAM_PCLK_FALL_EDGE;
    }

    MAP_CameraParamsConfig(hwAttrs->baseAddr, hSyncPolarityConfig,
                           vSyncPolarityConfig,
                           byteOrderConfig | interfaceSync | pixelClkConfig);

    /*Set the clock divider based on the output clock */
    MAP_CameraXClkConfig(hwAttrs->baseAddr,
                         PRCMPeripheralClockGet(PRCM_CAMERA),
                         params->outputClock);

    /*Setting the FIFO threshold for a DMA request */
    MAP_CameraThresholdSet(hwAttrs->baseAddr, 8);

    MAP_CameraIntEnable(hwAttrs->baseAddr, (CAM_INT_FE | CAM_INT_DMA));
    MAP_CameraDMAEnable(hwAttrs->baseAddr);

    DebugP_log1("Camera:(%p) opened", hwAttrs->baseAddr);

    /* Return the handle */
    return (handle);
}

/*
 *  ======== CameraCC3200DMA_close ========
 */
void CameraCC3200DMA_close(Camera_Handle handle)
{
    CameraCC3200DMA_Object           *object = handle->object;
    CameraCC3200DMA_HWAttrs const    *hwAttrs = handle->hwAttrs;

    /* Disable Camera and interrupts. */
    MAP_CameraIntDisable(hwAttrs->baseAddr,CAM_INT_FE);
    MAP_CameraDMADisable(hwAttrs->baseAddr);

    if (object->hwiHandle) {
        HwiP_delete(object->hwiHandle);
    }
    if (object->captureSem) {
        SemaphoreP_delete(object->captureSem);
    }

    Power_releaseDependency(PowerCC3200_PERIPH_CAMERA);
    Power_releaseDependency(PowerCC3200_PERIPH_UDMA);

    object->opened = false;

    DebugP_log1("Camera:(%p) closed", hwAttrs->baseAddr);
}

/*
 *  ======== CameraCC3200DMA_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int CameraCC3200DMA_control(Camera_Handle handle, unsigned int cmd, void *arg)
{
    /* No implementation yet */
    return (CAMERA_STATUS_UNDEFINEDCMD);
}

/*
 *  ======== CameraCC3200DMA_capture ========
 */

int CameraCC3200DMA_capture(Camera_Handle handle, void *buffer,
                                        unsigned int bufferlen)
{
   CameraCC3200DMA_Object        *object = handle->object;
   CameraCC3200DMA_HWAttrs const *hwAttrs = handle->hwAttrs;
   uintptr_t                      key;

   key = HwiP_disable();
   if (object->inUse) {
       HwiP_restore(key);
       DebugP_log1("Camera:(%p) Could not capture data, camera in use.",
                      ((CameraCC3200DMA_HWAttrs const *)
                          (handle->hwAttrs))->baseAddr);

       return (CAMERA_STATUS_UNDEFINEDCMD);
   }

   object->captureBuf             = buffer;
   object->bufferlength           = bufferlen;
   object->frameLength            = 0;
   object->cameraDMAxIntrRcvd     = 0;
   object->inUse                  = 1;
   object->cameraDMA_PingPongMode = 0;

   HwiP_restore(key);

   /* Set constraints to guarantee transaction */
   Power_setConstraint(PowerCC3200_DISALLOW_DEEPSLEEP);

   /* Start the DMA transfer */
   CameraCC3200DMA_configDMA(handle);
   MAP_CameraCaptureStart(hwAttrs->baseAddr);

   /* If operationMode is blocking, block and get the status. */
   if (object->operationMode == Camera_MODE_BLOCKING) {
       /* Pend on semaphore and wait for Hwi to finish. */
       if (SemaphoreP_pend(object->captureSem, object->captureTimeout)
                                                        != SemaphoreP_OK) {

           DebugP_log2("Camera:(%p) Capture timed out, %d bytes captured",
                          ((CameraCC3200DMA_HWAttrs const *)
                          (handle->hwAttrs))->baseAddr,
                          object->frameLength);
       }
       else {
           MAP_CameraCaptureStop(hwAttrs->baseAddr, true);
           return (object->frameLength);
       }
   }

    return (0);
}
