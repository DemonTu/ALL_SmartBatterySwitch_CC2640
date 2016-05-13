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
 *  ======== SemaphoreP_tirtos.c ========
 */
#include <ti/drivers/ports/SemaphoreP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <ti/drivers/ports/tirtos/tirtos_config.h>

/* If static allocation is used, the following provides the structs */
#ifdef TIRTOS_CONFIG_DRIVERS_USE_STATIC_ALLOCATION
Semaphore_Struct semaphoreStructs[TIRTOS_CONFIG_NUM_SEMAPHORE];
bool semaphoreUsed[TIRTOS_CONFIG_NUM_SEMAPHORE] = {false};
#endif

/*
 *  ======== SemaphoreP_create ========
 */
SemaphoreP_Handle SemaphoreP_create(unsigned int count,
                                    SemaphoreP_Params *params)
{
    Semaphore_Struct *handle;
    union {
        Error_Block          eb;
        Semaphore_Params     semaphoreParams;
    } createUnion;

     /* Grab the memory from the above static array or from the system heap */
#ifdef TIRTOS_CONFIG_DRIVERS_USE_STATIC_ALLOCATION
     int i;
     UInt key;

     key = Hwi_disable();
     for (i = 0; i < TIRTOS_CONFIG_NUM_SEMAPHORE; i++) {
         if (semaphoreUsed[i] == false) {
             semaphoreUsed[i] = true;
             break;
         }
     }
     Hwi_restore(key);

     if (i == TIRTOS_CONFIG_NUM_SEMAPHORE) {
         return (NULL);
     }

     handle = &(semaphoreStructs[i]);
#else
    Error_init(&(createUnion.eb));
    handle = (Semaphore_Struct *)Memory_alloc(NULL, sizeof(Semaphore_Struct),
                                              0, &(createUnion.eb));
    if (handle == NULL) {
        return (NULL);
    }
#endif

    if (params == NULL) {
        Semaphore_construct(handle, count, NULL);
    }
    else {
        Semaphore_Params_init(&(createUnion.semaphoreParams));
        /*
         * The default mode for TI-RTOS is counting, so only change if a binary
         * semaphore is requested.
         */
        if (params->mode == SemaphoreP_Mode_BINARY) {
            (createUnion.semaphoreParams).mode = Semaphore_Mode_BINARY;
        }
        (createUnion.semaphoreParams).instance->name = params->name;
        Semaphore_construct(handle, count, &(createUnion.semaphoreParams));
    }

    return ((SemaphoreP_Handle)handle);
}

/*
 *  ======== SemaphoreP_delete ========
 */
SemaphoreP_Status SemaphoreP_delete(SemaphoreP_Handle handle)
{
    Semaphore_Struct *semaphore = (Semaphore_Struct *)handle;

    Semaphore_destruct(semaphore);

    /* Release or free the memory */
#ifdef TIRTOS_CONFIG_DRIVERS_USE_STATIC_ALLOCATION
    UInt key;
    int index = (semaphore - semaphoreStructs); /* struct subtraction */

    key = Hwi_disable();
    semaphoreUsed[index] = false;
    Hwi_restore(key);
#else
    Memory_free(NULL, (Semaphore_Struct *)handle, sizeof(Semaphore_Struct));
#endif

    return (SemaphoreP_OK);
}

/*
 *  ======== SemaphoreP_Params_init ========
 */
void SemaphoreP_Params_init(SemaphoreP_Params *params)
{
    params->mode = SemaphoreP_Mode_COUNTING;
    params->name = NULL;
}

/*
 *  ======== SemaphoreP_pend ========
 */
SemaphoreP_Status SemaphoreP_pend(SemaphoreP_Handle handle, uint32_t timeout)
{
    Bool flag;

    flag = Semaphore_pend((Semaphore_Handle)handle, timeout);
    if (FALSE == flag) {
        return (SemaphoreP_TIMEOUT);
    }

    return (SemaphoreP_OK);
}

/*
 *  ======== SemaphoreP_post ========
 */
SemaphoreP_Status SemaphoreP_post(SemaphoreP_Handle handle)
{
    Semaphore_post((Semaphore_Handle)handle);
    return (SemaphoreP_OK);
}

/*
 *  ======== SemaphoreP_postFromClock ========
 */
SemaphoreP_Status SemaphoreP_postFromClock(SemaphoreP_Handle handle)
{
    return (SemaphoreP_post(handle));
}

/*
 *  ======== SemaphoreP_postFromISR ========
 */
SemaphoreP_Status SemaphoreP_postFromISR(SemaphoreP_Handle handle)
{
    return (SemaphoreP_post(handle));
}
