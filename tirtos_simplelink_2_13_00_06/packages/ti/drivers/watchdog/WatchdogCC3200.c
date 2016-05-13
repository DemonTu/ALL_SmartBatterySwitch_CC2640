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
#include <stdlib.h>

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

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC3200.h>

#include <ti/drivers/watchdog/WatchdogCC3200.h>

#include <driverlib/rom.h>
#include <driverlib/rom_map.h>

#include <ti/drivers/watchdog/WatchdogCC3200.h>

#include <inc/hw_types.h>
#include <driverlib/wdt.h>

/* Function prototypes */
void            WatchdogCC3200_clear(Watchdog_Handle handle);
void            WatchdogCC3200_close(Watchdog_Handle handle);
int             WatchdogCC3200_control(Watchdog_Handle handle, unsigned int cmd, void *arg);
void            WatchdogCC3200_init(Watchdog_Handle handle);
Watchdog_Handle WatchdogCC3200_open(Watchdog_Handle handle, Watchdog_Params *params);
void            WatchdogCC3200_setReload(Watchdog_Handle handle, uint32_t value);

/* Watchdog function table for CC3200 implementation */
const Watchdog_FxnTable WatchdogCC3200_fxnTable = {
    WatchdogCC3200_clear,
    WatchdogCC3200_close,
    WatchdogCC3200_control,
    WatchdogCC3200_init,
    WatchdogCC3200_open,
    WatchdogCC3200_setReload
};

/* Default Watchdog params */
extern const Watchdog_Params Watchdog_defaultParams;

/*
 *  ======== WatchdogCC3200_clear ========
 */
void WatchdogCC3200_clear(Watchdog_Handle handle)
{
    WatchdogCC3200_HWAttrs const *hwAttrs = handle->hwAttrs;

    MAP_WatchdogIntClear(hwAttrs->baseAddr);
}

/*
 *  ======== WatchdogCC3200_close ========
 */
void WatchdogCC3200_close(Watchdog_Handle handle)
{
    /*
     *  Not supported for CC3200 - Once the INTEN bit of the WDTCTL
     *  register has been set, it can only be cleared by a hardware
     *  reset.
     */
    DebugP_assert(false);
}

/*
 *  ======== WatchdogCC3200_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int WatchdogCC3200_control(Watchdog_Handle handle, unsigned int cmd, void *arg)
{
	/* No implementation yet */
	return (Watchdog_STATUS_UNDEFINEDCMD);
}

/*
 *  ======== Watchdog_init ========
 */
void WatchdogCC3200_init(Watchdog_Handle handle)
{
    WatchdogCC3200_Object *object = handle->object;

    object->isOpen = false;
}

/*
 *  ======== WatchdogCC3200_open ========
 */
Watchdog_Handle WatchdogCC3200_open(Watchdog_Handle handle, Watchdog_Params *params)
{
    uintptr_t                     key;
    HwiP_Handle                   hwiHandle;
    HwiP_Params                   hwiParams;
    WatchdogCC3200_HWAttrs const *hwAttrs = handle->hwAttrs;
    WatchdogCC3200_Object        *object  = handle->object;

    /* If params are NULL use defaults. */
    if (params == NULL) {
        params = (Watchdog_Params *) &Watchdog_defaultParams;
    }

    /* Don't allow preemption */
    key = HwiP_disable();

    /* Check if the Watchdog is open already with the HWAttrs */
    if (object->isOpen == true) {
        HwiP_restore(key);
        DebugP_log1("Watchdog: Handle %x already in use.", (uintptr_t)handle);
        return (NULL);
    }

    object->isOpen = true;
    HwiP_restore(key);

    /* Register the interrupt for this watchdog */
    if (params->callbackFxn) {
        HwiP_Params_init(&hwiParams);
        hwiParams.arg = (uintptr_t)handle;
        hwiParams.priority = hwAttrs->intPriority;
        hwiHandle = HwiP_create(hwAttrs->intNum, params->callbackFxn,
                                &hwiParams);
        if (hwiHandle == NULL) {
             return (NULL);
        }
    }

    /* Set power dependency on WDT */
    Power_setDependency(PowerCC3200_PERIPH_WDT);

    /* Don't allow the processor to go into LPDS (DeepSleep is ok) */
    Power_setConstraint(PowerCC3200_DISALLOW_LPDS);

    MAP_WatchdogUnlock(hwAttrs->baseAddr);
    MAP_WatchdogReloadSet(hwAttrs->baseAddr, hwAttrs->reloadValue);
    MAP_WatchdogIntClear(hwAttrs->baseAddr);

    /* Set debug stall mode */
    if (params->debugStallMode == Watchdog_DEBUG_STALL_ON) {
        MAP_WatchdogStallEnable(hwAttrs->baseAddr);
    }
    else {
        MAP_WatchdogStallDisable(hwAttrs->baseAddr);
    }

    MAP_WatchdogEnable(hwAttrs->baseAddr);

    MAP_WatchdogLock(hwAttrs->baseAddr);

    DebugP_log1("Watchdog: handle %x opened" ,(uintptr_t)handle);

    /* Return handle of the Watchdog object */
    return (handle);
}

/*
 *  ======== WatchdogCC3200_setReload ========
 */
void WatchdogCC3200_setReload(Watchdog_Handle handle, uint32_t value)
{
    WatchdogCC3200_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* Set value */
    MAP_WatchdogUnlock(hwAttrs->baseAddr);
    MAP_WatchdogReloadSet(hwAttrs->baseAddr, value);
    MAP_WatchdogLock(hwAttrs->baseAddr);

    DebugP_log2("Watchdog: WDT with handle 0x%x has been set to "
                "reload to 0x%x", (uintptr_t)handle, value);
}
