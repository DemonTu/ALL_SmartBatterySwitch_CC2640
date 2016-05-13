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
 *  ======== Camera.c ========
 */

#include <stdint.h>

#include <ti/drivers/Camera.h>

/* Externs */
extern const Camera_Config Camera_config[];

/* Used to check status and initialization */
static int Camera_count = -1;

/* Default Camera parameters structure */
const Camera_Params Camera_defaultParams = {
     Camera_MODE_BLOCKING,               /* captureMode */
     24000000,                           /* outputClock */
     Camera_HSYNC_POLARITY_HIGH,         /* hsyncPolarity */
     Camera_VSYNC_POLARITY_HIGH,         /* vsyncPolarity */
     Camera_PCLK_CONFIG_RISING_EDGE,     /* pixelClkConfig */
     Camera_BYTE_ORDER_NORMAL,           /* byteOrder */
     Camera_INTERFACE_SYNC_ON,           /* interfaceSync */
     Camera_STOP_CAPTURE_FRAME_END,      /* stopConfig */
     Camera_START_CAPTURE_FRAME_START,   /* startConfig */
     Camera_WAIT_FOREVER,                /* captureTimeout */
     NULL,                               /* captureCallback */
     NULL
};

/*
 *  ======== Camera_close ========
 */
void Camera_close(Camera_Handle handle)
{
    handle->fxnTablePtr->closeFxn(handle);
}

/*
 *  ======== Camera_control ========
 */
int Camera_control(Camera_Handle handle, uint32_t cmd, void *arg)
{
    return (handle->fxnTablePtr->controlFxn(handle, cmd, arg));
}

/*
 *  ======== Camera_init ========
 */
void Camera_init(void)
{
    if (Camera_count == -1) {
        /* Call each driver's init function */
        for (Camera_count = 0; Camera_config[Camera_count].fxnTablePtr \
                                                != NULL; Camera_count++) {
            Camera_config[Camera_count].fxnTablePtr->initFxn(\
                          (Camera_Handle)&(Camera_config[Camera_count]));
        }
    }
}

/*
 *  ======== Camera_open ========
 */
Camera_Handle Camera_open(uint32_t index, Camera_Params *params)
{
    Camera_Handle         handle;

    /* Get handle for this driver instance */
    handle = (Camera_Handle)&(Camera_config[index]);

    return (handle->fxnTablePtr->openFxn(handle, params));
}

/*
 *  ======== Camera_Params_init ========
 */
void Camera_Params_init(Camera_Params *params)
{
    *params = Camera_defaultParams;
}

/*
 *  ======== Camera_capture ========
 */
int Camera_capture(Camera_Handle handle, void *buffer,size_t bufferlen)
{
    return (handle->fxnTablePtr->captureFxn(handle, buffer, bufferlen));
}
