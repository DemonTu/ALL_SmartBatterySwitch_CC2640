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
 *  ======== PowerCC3200.c ========
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
#include <ti/drivers/ports/ListP.h>

#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC3200.h>

#if defined(__IAR_SYSTEMS_ICC__)
#include <intrinsics.h>
#endif

/* driverlib header files */
#include <inc/hw_types.h>
#include <inc/hw_gprcm.h>
#include <inc/hw_nvic.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/prcm.h>
#include <driverlib/cpu.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>

#define TRUE    1
#define FALSE   0

#define SYNCBARRIER() {          \
    __asm(" dsb \n"               \
          " isb \n");             \
}

/* Externs */
extern const PowerCC3200_Config PowerCC3200_config;

/* Module_State */
PowerCC3200_ModuleState PowerCC3200_module = {
    { NULL, NULL},  /* list */
    0,              /* constraintsMask */
    Power_ACTIVE,   /* state */
    /* dbRecords */
    {
        PRCM_CAMERA,  /* PERIPH_CAMERA */
        PRCM_I2S,     /* PERIPH_MCASP */
        PRCM_SDHOST,  /* PERIPH_MMCHS */
        PRCM_GSPI,    /* PERIPH_MCSPI_A1 */
        PRCM_LSPI,    /* PERIPH_MCSPI_A2 */
        PRCM_UDMA,    /* PERIPH_UDMA_A */
        PRCM_GPIOA0,  /* PERIPH_GPIO_A */
        PRCM_GPIOA1,  /* PERIPH_GPIO_B */
        PRCM_GPIOA2,  /* PERIPH_GPIO_C */
        PRCM_GPIOA3,  /* PERIPH_GPIO_D */
        PRCM_GPIOA4,  /* PERIPH_GPIO_E */
        PRCM_WDT,     /* PERIPH_WDOG_A */
        PRCM_UARTA0,  /* PERIPH_UART_A0 */
        PRCM_UARTA1,  /* PERIPH_UART_A1 */
        PRCM_TIMERA0, /* PERIPH_GPT_A0 */
        PRCM_TIMERA1, /* PERIPH_GPT_A1 */
        PRCM_TIMERA2, /* PERIPH_GPT_A2 */
        PRCM_TIMERA3, /* PERIPH_GPT_A3 */
        PRCM_DTHE,    /* PERIPH_CRYPTO */
        PRCM_SSPI,    /* PERIPH_MCSPI_S0 */
        PRCM_I2CA0    /* PERIPH_I2C */
    },
    /* enablePolicy */
    FALSE,
    /* refCount */
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* constraintCounts */
    { 0, 0, 0 }
};

/* context save variable */
PowerCC3200_SaveRegisters PowerCC3200_contextSave;

/* enter LPDS is an assembly function */
extern void PowerCC3200_enterLPDS(void);

/* internal functions */
static uint32_t notify(uint32_t eventType);
static void restoreNVICRegs(void);
static void restorePeriphClocks(void);
static void saveNVICRegs(void);

/*
 *  ======== Power_enablePolicy ========
 *  Run the configured policy
 */
void Power_enablePolicy(void)
{
    PowerCC3200_module.enablePolicy = TRUE;
}

/*
 *  ======== Power_getConstraintMask ========
 *  Get a bitmask indicating the constraints that have been registered with
 *  Power.
 */
unsigned int Power_getConstraintMask(void)
{
    return (PowerCC3200_module.constraintMask);
}

/*
 *  ======== Power_getDependencyCount ========
 *  Get the count of dependencies that are currently declared upon a resource.
 */
unsigned int Power_getDependencyCount(unsigned int resourceId)
{
    DebugP_assert(resourceId < PowerCC3200_NUMRESOURCES);
    return (PowerCC3200_module.refCount[resourceId]);
}

/*
 *  ======== Power_getTransitionLatency ========
 *  Get the transition latency for a sleep state.  The latency is reported
 *  in units of microseconds.
 */
uint32_t Power_getTransitionLatency(unsigned int sleepState,
                                    unsigned int type)
{
    uint32_t latency = 0;

    if (type == Power_RESUME) {
        if (sleepState == PowerCC3200_DEEPSLEEP) {
            latency = PowerCC3200_RESUMETIMEDEEPSLEEP;
        }
        else if (sleepState == PowerCC3200_LPDS) {
            latency = PowerCC3200_RESUMETIMELPDS;
        }
    }
    else {
        if (sleepState == PowerCC3200_DEEPSLEEP) {
            latency = PowerCC3200_TOTALTIMEDEEPSLEEP;
        }
        else if (sleepState == PowerCC3200_LPDS) {
            latency = PowerCC3200_TOTALTIMELPDS;
        }
    }

    return (latency);
}

/*
 *  ======== Power_getTransitionState ========
 *  Get the current sleep transition state.
 */
unsigned int Power_getTransitionState(void)
{
    return (PowerCC3200_module.state);
}

/*
 *  ======== Power_idleFunc ========
 *  Function needs to be plugged into the idle loop.
 *  It calls the configured policy function if the
 *  'enablePolicy' flag is set.
 */
void Power_idleFunc()
{
    if (PowerCC3200_module.enablePolicy) {
        if (PowerCC3200_config.policyFxn != NULL) {
            (*(PowerCC3200_config.policyFxn))();
        }
    }
}

/*
 *  ======== Power_init ========
 */
void Power_init()
{
    /* set the module state enablePolicy field */
    PowerCC3200_module.enablePolicy = PowerCC3200_config.enablePolicy;

    /* call the config policy init function if its not null */
    if (PowerCC3200_config.policyInitFxn != NULL) {
        (*(PowerCC3200_config.policyInitFxn))();
    }

    /* configure network (Host IRQ) as wakeup source for LPDS */
    if (PowerCC3200_config.enableNetworkWakeupLPDS) {
        MAP_PRCMLPDSWakeupSourceEnable(PRCM_LPDS_HOST_IRQ);
    }

    /* configure GPIO as wakeup source for LPDS */
    if (PowerCC3200_config.enableGPIOWakeupLPDS) {
        MAP_PRCMLPDSWakeUpGPIOSelect(
            PowerCC3200_config.wakeupGPIOSourceLPDS,
            PowerCC3200_config.wakeupGPIOTypeLPDS);
        MAP_PRCMLPDSWakeupSourceEnable(PRCM_LPDS_GPIO);
    }

    /* configure GPIO as wakeup source for Shutdown */
    if (PowerCC3200_config.enableGPIOWakeupShutdown) {
        MAP_PRCMHibernateWakeUpGPIOSelect(
            PowerCC3200_config.wakeupGPIOSourceShutdown,
            PowerCC3200_config.wakeupGPIOTypeShutdown);
        MAP_PRCMHibernateWakeupSourceEnable(
            PowerCC3200_config.wakeupGPIOSourceShutdown);
    }
}

/*
 *  ======== Power_registerNotify ========
 *  Register a function to be called on a specific power event.
 *
 */
unsigned int Power_registerNotify(Power_NotifyObj * pNotifyObj,
    unsigned int eventTypes, Power_NotifyFxn notifyFxn, uintptr_t clientArg)
{
    /* check for NULL pointers  */
    if ((pNotifyObj == NULL) || (notifyFxn == NULL)) {
        return (Power_EINVALIDPOINTER);
    }

    /* fill in notify object elements */
    pNotifyObj->eventTypes = eventTypes;
    pNotifyObj->notifyFxn = notifyFxn;
    pNotifyObj->clientArg = clientArg;

    /* place notify object on event notification queue */
    ListP_put(&PowerCC3200_module.notifyList, (ListP_Elem*)pNotifyObj);

    return (Power_SOK);
}

/*
 *  ======== Power_releaseConstraint ========
 *  Release a previously declared constraint.
 */
void Power_releaseConstraint(unsigned int constraintId)
{
    uintptr_t key;
    uint8_t count;

    /* assert constraintId is valid */
    DebugP_assert(constraintId < PowerCC3200_NUMCONSTRAINTS);

    /* disable interrupts */
    key = HwiP_disable();

    /* get the count of the constraint */
    count = PowerCC3200_module.constraintCounts[constraintId];

    /* assert that the count is not 0 */
    DebugP_assert(count != 0);

    /* decrement the count */
    count--;

    /* save the updated count */
    PowerCC3200_module.constraintCounts[constraintId] = count;

    /* if constraint count reaches 0, remove constraint from mask */
    if (count == 0) {
        PowerCC3200_module.constraintMask &= ~(1 << constraintId);
    }

    /* re-enable interrupts */
    HwiP_restore(key);
}

/*
 *  ======== Power_releaseDependency ========
 *  Release a previously declared dependency.
 */
void Power_releaseDependency(unsigned int resourceId)
{
    uint8_t count;
    uint32_t id;
    uintptr_t key;

    /* assert resourceId is valid */
    DebugP_assert(resourceId < PowerCC3200_NUMRESOURCES);

    /* disable interrupts */
    key = HwiP_disable();

    /* read the reference count */
    count = PowerCC3200_module.refCount[resourceId];

    /* assert count is not 0 */
    DebugP_assert(count != 0);

    /* decrement the reference count */
    count--;

    /* if this was the last dependency being released.., */
    if (count == 0) {
        /* deactivate this resource ... */
        id = PowerCC3200_module.dbRecords[resourceId];

        /* disable clk to peripheral */
        MAP_PRCMPeripheralClkDisable(id,
            PRCM_RUN_MODE_CLK | PRCM_SLP_MODE_CLK | PRCM_DSLP_MODE_CLK);
    }

    PowerCC3200_module.refCount[resourceId] = count;

    /* re-enable interrupts */
    HwiP_restore(key);
}

/*
 *  ======== Power_setConstraint ========
 *  Declare an operational constraint.
 */
void Power_setConstraint(unsigned int constraintId)
{
    uintptr_t key;

    /* assert constraint id is valid */
    DebugP_assert(constraintId < PowerCC3200_NUMCONSTRAINTS);

    /* disable interrupts */
    key = HwiP_disable();

    /* set the specified constraint in the constraintMask */
    PowerCC3200_module.constraintMask |= 1 << constraintId;

    /* increment the specified constraint count */
    PowerCC3200_module.constraintCounts[constraintId]++;

    /* re-enable interrupts */
    HwiP_restore(key);
}

/*
 *  ======== Power_setDependency ========
 *  Declare a dependency upon a resource.
 */
void Power_setDependency(unsigned int resourceId)
{
    uint8_t count;
    uint32_t id;
    uintptr_t key;

    /* assert resourceId is valid */
    DebugP_assert(resourceId < PowerCC3200_NUMRESOURCES);

    /* disable interrupts */
    key = HwiP_disable();

    /* read and increment reference count */
    count = PowerCC3200_module.refCount[resourceId]++;

    /* if resource was NOT activated previously ... */
    if (count == 0) {
        /* now activate this resource ... */
        id = PowerCC3200_module.dbRecords[resourceId];

        /* enable the peripheral clock to the resource */
        MAP_PRCMPeripheralClkEnable(id,
            PRCM_RUN_MODE_CLK | PRCM_SLP_MODE_CLK | PRCM_DSLP_MODE_CLK);

        /* spin here until status returns TRUE */
        while(!MAP_PRCMPeripheralStatusGet(id)) {
        }
    }

    /* re-enable interrupts */
    HwiP_restore(key);
}

/*
 *  ======== Power_shutdown ========
 */
unsigned int Power_shutdown(unsigned int shutdownState,
                            uint32_t shutdownTime)
{
    uint32_t status = Power_EFAIL;
    uint32_t constraints;
    uintptr_t hwiKey;
    uint64_t counts;

    /* disable interrupts */
    hwiKey = HwiP_disable();

    /* make sure shutdown request doesn't violate a constraint */
    constraints = Power_getConstraintMask();
    if (constraints & (1 << PowerCC3200_DISALLOW_SHUTDOWN)) {
        status = Power_ECHANGE_NOT_ALLOWED;
    }
    else {
        if (PowerCC3200_module.state == Power_ACTIVE) {
            /* set new transition state to entering shutdown */
            PowerCC3200_module.state = Power_ENTERING_SHUTDOWN;

            /* signal all clients registered for pre-shutdown notification */
            status = notify(PowerCC3200_ENTERING_SHUTDOWN);

            /* check for timeout or any other error */
            if (status != Power_SOK) {
                PowerCC3200_module.state = Power_ACTIVE;
                HwiP_restore(hwiKey);
                return (status);
            }

            /* if shutdown wakeup time was configured to be large enough */
            if (shutdownTime > (PowerCC3200_TOTALTIMESHUTDOWN / 1000)) {
                /* calculate the wakeup time for hibernate in RTC counts */
                counts =
                    (((uint64_t)(shutdownTime -
                                (PowerCC3200_TOTALTIMESHUTDOWN / 1000))
                                * 32768) / 1000);

                /* set the hibernate wakeup time */
                MAP_PRCMHibernateIntervalSet(counts);

                /* enable the wake source to be RTC */
                MAP_PRCMHibernateWakeupSourceEnable(PRCM_HIB_SLOW_CLK_CTR);
            }

            /* enter hibernate - we should never return from here */
            MAP_PRCMHibernateEnter();
        }
        else {
            status = Power_EBUSY;
        }
    }

    /* set state to Power_ACTIVE */
    PowerCC3200_module.state = Power_ACTIVE;

    /* re-enable interrupts */
    HwiP_restore(hwiKey);

    /* if get here, failed to shutdown, return error code */
    return (status);
}

/*
 *  ======== Power_sleep ========
 */
unsigned int Power_sleep(unsigned int sleepState)
{
    uint32_t status = Power_SOK;
    uint32_t preEvent;
    uint32_t postEvent;

    /* first validate the sleep code */
    DebugP_assert((sleepState == PowerCC3200_DEEPSLEEP) ||
                  (sleepState == PowerCC3200_LPDS));

    if (PowerCC3200_module.state == Power_ACTIVE) {
        /* set transition state to entering sleep */
        PowerCC3200_module.state = Power_ENTERING_SLEEP;

        /* setup sleep vars */
        if (sleepState == PowerCC3200_DEEPSLEEP) {
            preEvent = PowerCC3200_ENTERING_DEEPSLEEP;
            postEvent = PowerCC3200_AWAKE_DEEPSLEEP;
        }
        else if (sleepState == PowerCC3200_LPDS) {
            preEvent = PowerCC3200_ENTERING_LPDS;
            postEvent = PowerCC3200_AWAKE_LPDS;
        }

        /* signal all clients registered for pre-sleep notification */
        status = notify(preEvent);

        /* check for timeout or any other error */
        if (status != Power_SOK) {
            PowerCC3200_module.state = Power_ACTIVE;
            return (status);
        }

        /* invoke specific sequences to activate sleep states... */

        if (sleepState == PowerCC3200_DEEPSLEEP) {
            /* invoke deep sleep enter to go to deep sleep */
            MAP_PRCMDeepSleepEnter();
        }
        else if (sleepState == PowerCC3200_LPDS) {
            /* enable RAM retention */
            MAP_PRCMSRAMRetentionEnable(
                PowerCC3200_config.ramRetentionMaskLPDS,
                PRCM_SRAM_LPDS_RET);

            /* call the enter LPDS hook function if configured */
            if (PowerCC3200_config.enterLPDSHookFxn != NULL) {
                (*(PowerCC3200_config.enterLPDSHookFxn))();
            }

            /* save the NVIC registers */
            saveNVICRegs();

            /* call sync barrier */
            SYNCBARRIER();

            /* enter low power deep sleep - function does not return */
            PowerCC3200_enterLPDS();

            /* return here from Power_resumeLPDS */
            /* Restore NVIC registers */
            restoreNVICRegs();

            /* Restore clock to those peripherals with dependecy set */
            restorePeriphClocks();

            /* call the resume LPDS hook function if configured */
            if (PowerCC3200_config.resumeLPDSHookFxn != NULL) {
                (*(PowerCC3200_config.resumeLPDSHookFxn))();
            }

            /* re-enable Slow Clock Counter Interrupt */
            MAP_PRCMIntEnable(PRCM_INT_SLOW_CLK_CTR);
        }

        /* set transition state to EXITING_SLEEP */
        PowerCC3200_module.state = Power_EXITING_SLEEP;

        /*
         * signal clients registered for post-sleep notification
         */
        status = notify(postEvent);

        /* now clear the transition state before re-enabling scheduler */
        PowerCC3200_module.state = Power_ACTIVE;
    }
    else {
        status = Power_EBUSY;
    }

    return (status);
}

/*
 *  ======== Power_unregisterNotify ========
 *  Unregister for a power notification.
 *
 */
void Power_unregisterNotify(Power_NotifyObj * pNotifyObj)
{
    uintptr_t key;

    /* disable interrupts */
    key = HwiP_disable();

    /* remove notify object from its event queue */
    ListP_remove(&PowerCC3200_module.notifyList, (ListP_Elem *)pNotifyObj);

    /* re-enable interrupts */
    HwiP_restore(key);
}

/*********************** CC3200-specific functions **************************/
/*
 *  ======== PowerCC3200_configureWakeup ========
 *  Configure LPDS and shutdown wakeup
 */
void PowerCC3200_configureWakeup(PowerCC3200_Wakeup *wakeup)
{
    /* configure network (Host IRQ) as wakeup source for LPDS */
    if (wakeup->enableNetworkWakeupLPDS) {
        MAP_PRCMLPDSWakeupSourceEnable(PRCM_LPDS_HOST_IRQ);
    }
    else {
        MAP_PRCMLPDSWakeupSourceDisable(PRCM_LPDS_HOST_IRQ);
    }

    /* configure GPIO as wakeup source for LPDS */
    if (wakeup->enableGPIOWakeupLPDS) {
        MAP_PRCMLPDSWakeUpGPIOSelect(
            wakeup->wakeupGPIOSourceLPDS,
            wakeup->wakeupGPIOTypeLPDS);
        MAP_PRCMLPDSWakeupSourceEnable(PRCM_LPDS_GPIO);
    }
    else {
        MAP_PRCMLPDSWakeupSourceDisable(PRCM_LPDS_GPIO);
    }

    /* configure GPIO as wakeup source for Shutdown */
    if (wakeup->enableGPIOWakeupShutdown) {
        MAP_PRCMHibernateWakeUpGPIOSelect(
            wakeup->wakeupGPIOSourceShutdown,
            wakeup->wakeupGPIOTypeShutdown);
        MAP_PRCMHibernateWakeupSourceEnable(
            wakeup->wakeupGPIOSourceShutdown);
    }
    else {
        MAP_PRCMHibernateWakeupSourceDisable(
            wakeup->wakeupGPIOSourceShutdown);
    }
}

/*
 *  ======== PowerCC3200_parkPin ========
 *  Park a device pin in preparation for LPDS or shutdown
 *
 */
void PowerCC3200_parkPin(PowerCC3200_Pin pin, PowerCC3200_ParkState parkState)
{
    unsigned long strength;
    unsigned long type;

    /* get the current pin configuration */
    MAP_PinConfigGet(pin, &strength, &type);

    /* set the parking state */
    MAP_PinConfigSet(pin, strength, parkState);

    /* set pin direction to input to HiZ the pin */
    MAP_PinDirModeSet(pin, PIN_DIR_MODE_IN);
}

/*************************internal functions ****************************/
/*
 *  ======== notify ========
 *  Note: When this function is called hardware interrupts are disabled
 */
static uint32_t notify(uint32_t eventType)
{
    uint32_t status = Power_SOK;
    ListP_Elem *elem;
    Power_NotifyFxn notifyFxn;
    uintptr_t clientArg;

    /* if queue is empty, return immediately */
    if (!ListP_empty(&PowerCC3200_module.notifyList)) {
        /* point to first client notify object */
        elem = ListP_head(&PowerCC3200_module.notifyList);

        /* walk the queue and notify each registered client of the event */
        do {
            if (((Power_NotifyObj *)elem)->eventTypes & eventType) {
                /* pull params from notify object */
                notifyFxn = ((Power_NotifyObj *)elem)->notifyFxn;
                clientArg = ((Power_NotifyObj *)elem)->clientArg;

                /* call the client's notification function */
                status = (uint32_t) (*(Power_NotifyFxn)notifyFxn)(
                    eventType, 0, clientArg);

                /* if client declared error stop all further notifications */
                if (status == Power_NOTIFYERROR) {
                    return (Power_EFAIL);
                }
            }

            /* get next element in the notification queue */
            elem = ListP_next(elem);

        } while (elem != NULL);
    }

    return (status);
}

/*
 *  ======== restoreNVICRegs ========
 *  Restore the NVIC registers
 */
static void restoreNVICRegs(void)
{
    uint32_t i;
    uint32_t *base_reg_addr;

    /* Restore the NVIC control registers */
    HWREG(NVIC_VTABLE) = PowerCC3200_contextSave.nvicRegs.vectorTable;
    HWREG(NVIC_ACTLR) = PowerCC3200_contextSave.nvicRegs.auxCtrl;
    HWREG(NVIC_APINT) = PowerCC3200_contextSave.nvicRegs.appInt;
    HWREG(NVIC_INT_CTRL) = PowerCC3200_contextSave.nvicRegs.intCtrlState;
    HWREG(NVIC_SYS_CTRL) = PowerCC3200_contextSave.nvicRegs.sysCtrl;
    HWREG(NVIC_CFG_CTRL) = PowerCC3200_contextSave.nvicRegs.configCtrl;
    HWREG(NVIC_SYS_PRI1) = PowerCC3200_contextSave.nvicRegs.sysPri1;
    HWREG(NVIC_SYS_PRI2) = PowerCC3200_contextSave.nvicRegs.sysPri2;
    HWREG(NVIC_SYS_PRI3) = PowerCC3200_contextSave.nvicRegs.sysPri3;
    HWREG(NVIC_SYS_HND_CTRL) = PowerCC3200_contextSave.nvicRegs.sysHcrs;

    /* Systick registers */
    HWREG(NVIC_ST_CTRL) = PowerCC3200_contextSave.nvicRegs.systickCtrl;
    HWREG(NVIC_ST_RELOAD) = PowerCC3200_contextSave.nvicRegs.systickReload;
    HWREG(NVIC_ST_CAL) = PowerCC3200_contextSave.nvicRegs.systickCalib;

    /* Restore the interrupt priority registers */
    base_reg_addr = (uint32_t *)NVIC_PRI0;
    for(i = 0; i < PowerCC3200_numNVICIntPriority; i++) {
        base_reg_addr[i] = PowerCC3200_contextSave.nvicRegs.intPriority[i];
    }

    /* Restore the interrupt enable registers */
    base_reg_addr = (uint32_t *)NVIC_EN0;
    for(i = 0; i < PowerCC3200_numNVICSetEnableRegs; i++) {
        base_reg_addr[i] = PowerCC3200_contextSave.nvicRegs.intSetEn[i];
    }

    /* Data and instruction sync barriers */
    SYNCBARRIER();
}

/*
 *  ======== restorePeriphClocks ========
 *  Restores the peripheral clocks that had dependency set
 */
static void restorePeriphClocks(void)
{
    uint32_t dependCount;
    uint32_t i;

    /* need to re-enable peripheral clocks to those with set dependency */
    for (i = 0; i < PowerCC3200_NUMRESOURCES; i++) {
        dependCount = Power_getDependencyCount(i);
        if (dependCount > 0) {
            MAP_PRCMPeripheralClkEnable(PowerCC3200_module.dbRecords[i],
                PRCM_RUN_MODE_CLK | PRCM_SLP_MODE_CLK | PRCM_DSLP_MODE_CLK);

            while(!MAP_PRCMPeripheralStatusGet(PowerCC3200_module.dbRecords[i])) {
            }
        }
    }
}

/*
 *  ======== saveNVICRegs ========
 *  Save away the NVIC registers for LPDS mode.
 */
static void saveNVICRegs(void)
{
    uint32_t i;
    uint32_t *base_reg_addr;

    /* Save the NVIC control registers */
    PowerCC3200_contextSave.nvicRegs.vectorTable = HWREG(NVIC_VTABLE);
    PowerCC3200_contextSave.nvicRegs.auxCtrl = HWREG(NVIC_ACTLR);
    PowerCC3200_contextSave.nvicRegs.intCtrlState = HWREG(NVIC_INT_CTRL);
    PowerCC3200_contextSave.nvicRegs.appInt = HWREG(NVIC_APINT);
    PowerCC3200_contextSave.nvicRegs.sysCtrl = HWREG(NVIC_SYS_CTRL);
    PowerCC3200_contextSave.nvicRegs.configCtrl = HWREG(NVIC_CFG_CTRL);
    PowerCC3200_contextSave.nvicRegs.sysPri1 = HWREG(NVIC_SYS_PRI1);
    PowerCC3200_contextSave.nvicRegs.sysPri2 = HWREG(NVIC_SYS_PRI2);
    PowerCC3200_contextSave.nvicRegs.sysPri3 = HWREG(NVIC_SYS_PRI3);
    PowerCC3200_contextSave.nvicRegs.sysHcrs = HWREG(NVIC_SYS_HND_CTRL);

    /* Systick registers */
    PowerCC3200_contextSave.nvicRegs.systickCtrl = HWREG(NVIC_ST_CTRL);
    PowerCC3200_contextSave.nvicRegs.systickReload = HWREG(NVIC_ST_RELOAD);
    PowerCC3200_contextSave.nvicRegs.systickCalib = HWREG(NVIC_ST_CAL);

    /* Save the interrupt enable registers */
    base_reg_addr = (uint32_t *)NVIC_EN0;
    for (i = 0; i < PowerCC3200_numNVICSetEnableRegs; i++) {
        PowerCC3200_contextSave.nvicRegs.intSetEn[i] = base_reg_addr[i];
    }

    /* Save the interrupt priority registers */
    base_reg_addr = (uint32_t *)NVIC_PRI0;
    for (i = 0; i < PowerCC3200_numNVICIntPriority; i++) {
        PowerCC3200_contextSave.nvicRegs.intPriority[i] = base_reg_addr[i];
    }
}
