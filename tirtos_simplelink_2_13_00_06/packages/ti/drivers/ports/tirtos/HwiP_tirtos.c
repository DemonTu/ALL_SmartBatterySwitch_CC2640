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
 *  ======== HwiP_tirtos.c ========
 */

#include <ti/drivers/ports/HwiP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>

#ifdef xdc_target__isaCompatible_28
#include <ti/sysbios/hal/Hwi.h>
#else
#define ti_sysbios_family_arm_m3_Hwi__internalaccess
#include <ti/sysbios/family/arm/m3/Hwi.h>
#endif
//todo msp430
#include <ti/drivers/ports/tirtos/tirtos_config.h>

/* If static allocation is used, the following provides the structs */
#ifdef TIRTOS_CONFIG_DRIVERS_USE_STATIC_ALLOCATION
Hwi_Struct hwiStructs[TIRTOS_CONFIG_NUM_HWI];
bool hwiUsed[TIRTOS_CONFIG_NUM_HWI] = {false};
#endif

/*
 *  ======== HwiP_clearInterrupt ========
 */
void HwiP_clearInterrupt(int interruptNum)
{
    Hwi_clearInterrupt(interruptNum);
}

/*
 *  ======== HwiP_create ========
 */
HwiP_Handle HwiP_create(int interruptNum, HwiP_Fxn hwiFxn,
                        HwiP_Params *params)
{
    Hwi_Struct *handle;
    Error_Block eb;
    Hwi_Params  hwiParams;

    Error_init(&eb);

     /* Grab the memory from the above static array or from the system heap */
#ifdef TIRTOS_CONFIG_DRIVERS_USE_STATIC_ALLOCATION
     int i;
     UInt key;

     key = Hwi_disable();
     for (i = 0; i < TIRTOS_CONFIG_NUM_HWI; i++) {
         if (hwiUsed[i] == false) {
             hwiUsed[i] = true;
             break;
         }
     }
     Hwi_restore(key);
     handle = &(hwiStructs[i]);
#else
    handle = (Hwi_Struct *)Memory_alloc(NULL, sizeof(Hwi_Struct),
                                        0, &eb);
    if (handle == NULL) {
        return (NULL);
    }
#endif

    if (params == NULL) {
        Hwi_construct(handle, interruptNum, (Hwi_FuncPtr)hwiFxn, NULL, &eb);
    }
    else {
        Hwi_Params_init(&hwiParams);
        hwiParams.instance->name = params->name;
        hwiParams.arg            = (xdc_UArg)(params->arg);
        hwiParams.priority       = (int)params->priority;
        Hwi_construct(handle, interruptNum, (Hwi_FuncPtr)hwiFxn,
                      &hwiParams, &eb);
    }

    return ((HwiP_Handle)handle);
}

/*
 *  ======== HwiP_delete ========
 */
HwiP_Status HwiP_delete(HwiP_Handle handle)
{
    Hwi_Struct *hwi = (Hwi_Struct *)handle;

    Hwi_destruct(hwi);

    /* Release or free the memory */
#ifdef TIRTOS_CONFIG_DRIVERS_USE_STATIC_ALLOCATION
    UInt key;
    int index = (hwi - hwiStructs); /* struct subtraction */

    key = Hwi_disable();
    hwiUsed[index] = false;
    Hwi_restore(key);
#else
    Memory_free(NULL, (Hwi_Struct *)handle, sizeof(Hwi_Struct));
#endif

    return (HwiP_OK);
}

/*
 *  ======== HwiP_disable ========
 */
uintptr_t HwiP_disable(void)
{
    uintptr_t key;

    key = Hwi_disable();

    return (key);
}

/*
 *  ======== HwiP_disableInterrupt ========
 */
void HwiP_disableInterrupt(int interruptNum)
{
    Hwi_disableInterrupt(interruptNum);
}

/*
 *  ======== HwiP_enableInterrupt ========
 */
void HwiP_enableInterrupt(int interruptNum)
{
    Hwi_enableInterrupt(interruptNum);
}

/*
 *  ======== HwiP_Params_init ========
 */
void HwiP_Params_init(HwiP_Params *params)
{
    params->name = NULL;
    params->arg = 0;
    params->priority = ~0;
}

/*
 *  ======== HwiP_restore ========
 */
void HwiP_restore(uintptr_t key)
{
    Hwi_restore(key);
}
