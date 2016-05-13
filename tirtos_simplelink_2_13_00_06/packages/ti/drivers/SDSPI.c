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

#include <ti/drivers/SDSPI.h>

#include <stdint.h>
#include <stdlib.h>

/* Externs */
extern const SDSPI_Config SDSPI_config[];

/* Used to check status and initialization */
static int SDSPI_count = -1;

/* Default SDSPI parameters structure */
const SDSPI_Params SDSPI_defaultParams = {
#if defined (MSP430WARE) || defined(MSP432WARE)
    2500000, /* bitRate */
#else
    12500000,/* bitRate */
#endif
    NULL    /* custom */
};

/*
 *  ======== SDSPI_close ========
 */
void SDSPI_close(SDSPI_Handle handle)
{
    handle->fxnTablePtr->closeFxn(handle);
}

/*
 *  ======== SDSPI_control ========
 */
int SDSPI_control(SDSPI_Handle handle, unsigned int cmd, void *arg)
{
    return (handle->fxnTablePtr->controlFxn(handle, cmd, arg));
}

/*
 *  ======== SDSPI_init ========
 */
void SDSPI_init(void)
{
    if (SDSPI_count == -1) {
        /* Call each driver's init function */
        for (SDSPI_count = 0; SDSPI_config[SDSPI_count].fxnTablePtr != NULL; SDSPI_count++) {
            SDSPI_config[SDSPI_count].fxnTablePtr->initFxn((SDSPI_Handle)&(SDSPI_config[SDSPI_count]));
        }
    }
}

/*
 *  ======== SDSPI_open ========
 */
SDSPI_Handle SDSPI_open(unsigned int index, unsigned char drv,
                        SDSPI_Params *params)
{
    SDSPI_Handle handle;

    /* Get handle for this driver instance */
    handle = (SDSPI_Handle)&(SDSPI_config[index]);

    return (handle->fxnTablePtr->openFxn(handle, drv, params));
}

/*
 *  ======== SDSPI_Params_init ========
 *
 *  Defaults values are:
 *  @code
 *  bitRate             = 12500000 (Hz) //Tiva
 *  bitRate             = 2500000  (Hz) //MSP430
 *  @endcode
 *
 *  @param  params  Parameter structure to initialize
 */
void SDSPI_Params_init(SDSPI_Params *params)
{
    *params = SDSPI_defaultParams;
}
