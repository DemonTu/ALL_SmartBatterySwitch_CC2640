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
 *  ======== ClockP_tirtos.c ========
 */

#include <ti/drivers/ports/ClockP.h>

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Types.h>
#include <xdc/runtime/Memory.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>

#include <ti/drivers/ports/tirtos/tirtos_config.h>

/* If static allocation is used, the following provides the structs */
#ifdef TIRTOS_CONFIG_DRIVERS_USE_STATIC_ALLOCATION
Clock_Struct clockStructs[TIRTOS_CONFIG_NUM_CLOCK];
bool clockUsed[TIRTOS_CONFIG_NUM_CLOCK] = {false};
#endif

/*
 *  ======== ClockP_create ========
 */
ClockP_Handle ClockP_create(ClockP_Fxn clockFxn, ClockP_Params *params)
{
    Clock_Struct *handle;
    union {
        Error_Block  eb;
        Clock_Params clockParams;
    } createUnion;

     /* Grab the memory from the above static array or from the system heap */
#ifdef TIRTOS_CONFIG_DRIVERS_USE_STATIC_ALLOCATION
     int i;
     UInt key;

     key = Hwi_disable();
     for (i = 0; i < TIRTOS_CONFIG_NUM_CLOCK; i++) {
         if (clockUsed[i] == false) {
             clockUsed[i] = true;
             break;
         }
     }
     Hwi_restore(key);

     if (i == TIRTOS_CONFIG_NUM_CLOCK) {
         return (NULL);
     }

     handle = &(clockStructs[i]);
#else
    Error_init(&(createUnion.eb));
    handle = (Clock_Struct *)Memory_alloc(NULL, sizeof(Clock_Struct),
                                          0, &(createUnion.eb));
    if (handle == NULL) {
        return (NULL);
    }
#endif

    /* Use 0 for timeout since we are only doing one-shot clocks */
    if (params == NULL) {
        Clock_construct(handle, (Clock_FuncPtr)clockFxn, 0, NULL);
    }
    else {
        Clock_Params_init(&(createUnion.clockParams));
        (createUnion.clockParams).arg            = params->arg;
        (createUnion.clockParams).instance->name = params->name;
        Clock_construct(handle, (Clock_FuncPtr)clockFxn, 0,
                        &(createUnion.clockParams));
    }

    return ((ClockP_Handle)handle);
}

/*
 *  ======== ClockP_delete ========
 */
ClockP_Status ClockP_delete(ClockP_Handle handle)
{
    Clock_Struct *clock = (Clock_Struct *)handle;

    Clock_destruct(clock);

    /* Release or free the memory */
#ifdef TIRTOS_CONFIG_DRIVERS_USE_STATIC_ALLOCATION
    UInt key;
    int index = (clock - clockStructs); /* struct subtraction */

    key = Hwi_disable();
    clockUsed[index] = false;
    Hwi_restore(key);
#else
    Memory_free(NULL, (Clock_Struct *)handle, sizeof(Clock_Struct));
#endif

    return (ClockP_OK);
}

/*
 *  ======== ClockP_getCpuFreq ========
 */
void ClockP_getCpuFreq(ClockP_FreqHz *freq)
{
    BIOS_getCpuFreq((Types_FreqHz *)freq);
}

/*
 *  ======== ClockP_getSystemTickPeriod ========
 */
uint32_t ClockP_getSystemTickPeriod()
{
    return (Clock_tickPeriod);
}

/*
 *  ======== ClockP_getSystemTicks ========
 */
uint32_t ClockP_getSystemTicks()
{
    return (Clock_getTicks());
}

/*
 *  ======== ClockP_Params_init ========
 */
void ClockP_Params_init(ClockP_Params *params)
{
    params->name = NULL;
    params->arg = 0;
}

/*
 *  ======== ClockP_start ========
 */
ClockP_Status ClockP_start(ClockP_Handle handle, uint32_t timeout)
{
    Clock_setTimeout((Clock_Handle)handle, timeout);
    Clock_start((Clock_Handle)handle);

    return (ClockP_OK);
}

/*
 *  ======== ClockP_startFromISR ========
 */
ClockP_Status ClockP_startFromISR(ClockP_Handle handle, uint32_t timeout)
{
    return(ClockP_start(handle, timeout));
}

/*
 *  ======== ClockP_stop ========
 */
ClockP_Status ClockP_stop(ClockP_Handle handle)
{
    Clock_stop((Clock_Handle)handle);

    return (ClockP_OK);
}

/*
 *  ======== ClockP_stopFromISR ========
 */
ClockP_Status ClockP_stopFromISR(ClockP_Handle handle)
{
    return(ClockP_stop(handle));
}
