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
 *  ======== MutexP_tirtos.c ========
 */

#include <ti/drivers/ports/MutexP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/gates/GateMutex.h>

#include <ti/drivers/ports/tirtos/tirtos_config.h>

/* If static allocation is used, the following provides the structs */
#ifdef TIRTOS_CONFIG_DRIVERS_USE_STATIC_ALLOCATION
GateMutex_Struct gateMutexStructs[TIRTOS_CONFIG_NUM_MUTEX];
bool gateMutedUsed[TIRTOS_CONFIG_NUM_MUTEX] = {false};
#endif

/*
 *  ======== MutexP_create ========
 */
MutexP_Handle MutexP_create(MutexP_Params *params)
{
    GateMutex_Struct *handle;
    union {
        Error_Block      eb;
        GateMutex_Params gateMutexParams;
    } createUnion;

     /* Grab the memory from the above static array or from the system heap */
#ifdef TIRTOS_CONFIG_DRIVERS_USE_STATIC_ALLOCATION
     int i;
     UInt key;

     key = Hwi_disable();
     for (i = 0; i < TIRTOS_CONFIG_NUM_MUTEX; i++) {
         if (gateMutedUsed[i] == false) {
             gateMutedUsed[i] = true;
             break;
         }
     }
     Hwi_restore(key);

     if (i == TIRTOS_CONFIG_NUM_MUTEX) {
         return (NULL);
     }

     handle = &(gateMutexStructs[i]);
#else
    Error_init(&(createUnion.eb));
    handle = (GateMutex_Struct *)Memory_alloc(NULL, sizeof(GateMutex_Struct),
                                              0, &(createUnion.eb));
    if (handle == NULL) {
        return (NULL);
    }
#endif

    if (params == NULL) {
        GateMutex_construct(handle, NULL);
    }
    else {
        GateMutex_Params_init(&(createUnion.gateMutexParams));
        (createUnion.gateMutexParams).instance->name = params->name;
        GateMutex_construct(handle, &(createUnion.gateMutexParams));
    }

    return ((MutexP_Handle)handle);
}

/*
 *  ======== MutexP_delete ========
 */
MutexP_Status MutexP_delete(MutexP_Handle handle)
{
    GateMutex_Struct *gateMutex = (GateMutex_Struct *)handle;

    GateMutex_destruct(gateMutex);

    /* Release or free the memory */
#ifdef TIRTOS_CONFIG_DRIVERS_USE_STATIC_ALLOCATION
    UInt key;
    int index = (gateMutex - gateMutexStructs); /* struct subtraction */

    key = Hwi_disable();
    gateMutedUsed[index] = false;
    Hwi_restore(key);
#else
    Memory_free(NULL, (GateMutex_Struct *)handle, sizeof(GateMutex_Struct));
#endif

    return (MutexP_OK);
}


/*
 *  ======== MutexP_lock ========
 */
uintptr_t MutexP_lock(MutexP_Handle handle)
{
    unsigned int key;

    key = GateMutex_enter((GateMutex_Handle)handle);

    return ((uintptr_t)key);
}

/*
 *  ======== MutexP_Params_init ========
 */
void MutexP_Params_init(MutexP_Params *params)
{
    params->name = NULL;
}

/*
 *  ======== MutexP_unlock ========
 */
void MutexP_unlock(MutexP_Handle handle, uintptr_t key)
{
    GateMutex_leave((GateMutex_Handle)handle, (unsigned int)key);
}
