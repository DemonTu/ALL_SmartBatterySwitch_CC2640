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
#include <stdbool.h>
#if defined(__IAR_SYSTEMS_ICC__)
#include <intrinsics.h>
#endif

#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>

#include <ti/sysbios/family/arm/m3/Hwi.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOTiva.h>

#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#if defined(TIVAWARE)
#include <inc/hw_sysctl.h>
#endif
#include <driverlib/gpio.h>

/* Table of GPIO interrupt types */
static const uint8_t gpioInterruptTypes[] = {
    0,                  /* Undefined interrupt type */
    GPIO_FALLING_EDGE,
    GPIO_RISING_EDGE,
    GPIO_BOTH_EDGES,
    GPIO_LOW_LEVEL,
    GPIO_HIGH_LEVEL
};

#if defined(TIVAWARE)
/* Table of port base addresses. */
static const uint32_t gpioBaseAddresses[] = {
    GPIO_PORTA_BASE, GPIO_PORTB_BASE, GPIO_PORTC_BASE,
    GPIO_PORTD_BASE, GPIO_PORTE_BASE, GPIO_PORTF_BASE,
    GPIO_PORTG_BASE, GPIO_PORTH_BASE, GPIO_PORTJ_BASE,
    GPIO_PORTK_BASE, GPIO_PORTL_BASE, GPIO_PORTM_BASE,
    GPIO_PORTN_BASE, GPIO_PORTR_BASE, GPIO_PORTS_BASE,
    GPIO_PORTT_BASE, GPIO_PORTP_BASE, GPIO_PORTQ_BASE
};

/* Table of GPIO input types */
static const uint8_t gpioCfgInputTypes [] = {
    GPIO_PIN_TYPE_STD,        /* GPIO_CFG_IN_NOPULL */
    GPIO_PIN_TYPE_STD_WPU,    /* GPIO_CFG_IN_PU */
    GPIO_PIN_TYPE_STD_WPD     /* GPIO_CFG_IN_PD */
};

/* Table of GPIO output types */
static const uint8_t gpioCfgOutputTypes [] = {
    GPIO_PIN_TYPE_STD,    /* GPIO_CFG_OUT_STD */
    GPIO_PIN_TYPE_OD,     /* GPIO_CFG_OUT_OD_NOPULL */
    GPIO_PIN_TYPE_OD,     /* GPIO_CFG_OUT_OD_PU not supported by TivaWare */
    GPIO_PIN_TYPE_OD      /* GPIO_CFG_OUT_OD_PD not supported by TivaWare */
};

/* Table of port interrupt vectors. Used by setCallback() to create Hwis. */
static const uint8_t gpioTM4C123InterruptVectors[] = {
    /* TM4C123 devices do not have R,S, and T GPIO ports, interrupts as NULLs */
    INT_GPIOA_TM4C123,  INT_GPIOB_TM4C123,  INT_GPIOC_TM4C123,
    INT_GPIOD_TM4C123,  INT_GPIOE_TM4C123,  INT_GPIOF_TM4C123,
    INT_GPIOG_TM4C123,  INT_GPIOH_TM4C123,  INT_GPIOJ_TM4C123,
    INT_GPIOK_TM4C123,  INT_GPIOL_TM4C123,  INT_GPIOM_TM4C123,
    INT_GPION_TM4C123,  NULL,               NULL,
    NULL,               INT_GPIOP0_TM4C123, INT_GPIOP1_TM4C123,
    INT_GPIOP2_TM4C123, INT_GPIOP3_TM4C123, INT_GPIOP4_TM4C123,
    INT_GPIOP5_TM4C123, INT_GPIOP6_TM4C123, INT_GPIOP7_TM4C123,
    INT_GPIOQ0_TM4C123, INT_GPIOQ1_TM4C123, INT_GPIOQ2_TM4C123,
    INT_GPIOQ3_TM4C123, INT_GPIOQ4_TM4C123, INT_GPIOQ5_TM4C123,
    INT_GPIOQ6_TM4C123, INT_GPIOQ7_TM4C123
};

static const uint8_t gpioTM4C129InterruptVectors[] = {
    INT_GPIOA_TM4C129,  INT_GPIOB_TM4C129,  INT_GPIOC_TM4C129,
    INT_GPIOD_TM4C129,  INT_GPIOE_TM4C129,  INT_GPIOF_TM4C129,
    INT_GPIOG_TM4C129,  INT_GPIOH_TM4C129,  INT_GPIOJ_TM4C129,
    INT_GPIOK_TM4C129,  INT_GPIOL_TM4C129,  INT_GPIOM_TM4C129,
    INT_GPION_TM4C129,  INT_GPIOR_TM4C129,  INT_GPIOS_TM4C129,
    INT_GPIOT_TM4C129,  INT_GPIOP0_TM4C129, INT_GPIOP1_TM4C129,
    INT_GPIOP2_TM4C129, INT_GPIOP3_TM4C129, INT_GPIOP4_TM4C129,
    INT_GPIOP5_TM4C129, INT_GPIOP6_TM4C129, INT_GPIOP7_TM4C129,
    INT_GPIOQ0_TM4C129, INT_GPIOQ1_TM4C129, INT_GPIOQ2_TM4C129,
    INT_GPIOQ3_TM4C129, INT_GPIOQ4_TM4C129, INT_GPIOQ5_TM4C129,
    INT_GPIOQ6_TM4C129, INT_GPIOQ7_TM4C129
};

/* Pointer to array of device GPIO interrupts */
static const uint8_t *gpioInterruptVectors;

/* Table of GPIO drive strengths */
static const uint8_t gpioStrengths [] = {
    GPIO_STRENGTH_2MA,       /* GPIO_CFG_OUT_STR_LOW */
    GPIO_STRENGTH_6MA,       /* GPIO_CFG_OUT_STR_MED */
    GPIO_STRENGTH_12MA,      /* GPIO_CFG_OUT_STR_HIGH */
    GPIO_STRENGTH_4MA,
    GPIO_STRENGTH_8MA,
    GPIO_STRENGTH_8MA_SC,
    GPIO_STRENGTH_10MA
};

#define NUM_PORTS           (18)
#define NUM_PINS_PER_PORT    (8)

#elif defined(MWARE)
/* Table of port base addresses. */
static const uint32_t gpioBaseAddresses[] = {
    GPIO_PORTA_BASE, GPIO_PORTB_BASE, GPIO_PORTC_BASE,
    GPIO_PORTD_BASE, GPIO_PORTE_BASE, GPIO_PORTF_BASE,
    GPIO_PORTG_BASE, GPIO_PORTH_BASE, GPIO_PORTJ_BASE,
    GPIO_PORTK_BASE, GPIO_PORTL_BASE, GPIO_PORTM_BASE,
    GPIO_PORTN_BASE, GPIO_PORTP_BASE, GPIO_PORTQ_BASE,
    GPIO_PORTR_BASE, GPIO_PORTS_BASE
};

/* Table of GPIO input types */
static const uint8_t gpioCfgInputTypes [] = {
    GPIO_PIN_TYPE_STD,
    GPIO_PIN_TYPE_STD_WPU,
    GPIO_PIN_TYPE_STD
};

/* Table of GPIO output types */
static const uint8_t gpioCfgOutputTypes [] = {
    GPIO_PIN_TYPE_STD,
    GPIO_PIN_TYPE_OD,
    GPIO_PIN_TYPE_OD_WPU,
    GPIO_PIN_TYPE_OD
};

/* Table of port interrupt vectors. Used by setCallback() to create Hwis. */
static const uint8_t gpioInterruptVectors[] = {
    INT_GPIOA, INT_GPIOB, INT_GPIOC, INT_GPIOD, INT_GPIOE, INT_GPIOF,
    INT_GPIOG, INT_GPIOH, INT_GPIOJ, INT_GPIOK, INT_GPIOL, INT_GPIOM,
    INT_GPION, INT_GPIOP, INT_GPIOQ, INT_GPIOR, INT_GPIOS
};

/* Map Mware APIs */
#define GPIOIntClear        GPIOPinIntClear
#define GPIOIntStatus       GPIOPinIntStatus
#define GPIOIntDisable      GPIOPinIntDisable
#define GPIOIntEnable       GPIOPinIntEnable

#define NUM_PORTS           (17)
#define NUM_PINS_PER_PORT    (8)
#endif

/* Returns the GPIO port base address */
#define getGpioBaseAddr(port) (gpioBaseAddresses[port])

/* Extracts the GPIO interrupt type from the pinConfig */
#define getGpioIntType(pinConfig) \
    gpioInterruptTypes[(pinConfig & GPIO_CFG_INT_MASK) >> GPIO_CFG_INT_LSB]

/* Extracts the GPIO strength from the pinConfig. */
#define getGpioStrength(pinConfig) \
    gpioStrengths[(pinConfig & GPIO_CFG_OUT_STRENGTH_MASK) >> \
    GPIO_CFG_OUT_STRENGTH_LSB]

/*
 * Extracts the GPIO configuration from the pinConfig.  Value to index into the
 * gpioInputTypes or gpioOutputTypes tables.
 */
#define getGpioTypeIndex(pinConfig) \
    ((pinConfig & GPIO_CFG_IN_TYPE_MASK) >> GPIO_CFG_IN_TYPE_LSB)

/* Uninitialized callbackInfo pinIndex */
#define CALLBACK_INDEX_NOT_CONFIGURED 0xFF

/* Device specific interpretation of the GPIO_PinConfig content */
typedef struct PinConfig {
    uint8_t pin;
    uint8_t port;
    uint16_t config;
} PinConfig;

/*
 * User defined pin indexes assigned to a port's 8 pins. Used by port interrupt
 * function to located callback assigned to a pin.
 */
typedef struct PortCallbackInfo {
    /* The port's 8 corresponding user defined pinId indices */
    uint8_t pinIndex[NUM_PINS_PER_PORT];
} PortCallbackInfo;


/* Table of callbacks per port. */
static PortCallbackInfo gpioCallbackInfo[NUM_PORTS];

/*
 * Bit mask used to determine if a Hwi has been created/constructed for a port
 * already.
 */
static uint32_t portHwiCreatedBitMask = 0;

/* Boolean to confirm that GPIO_init() has been called */
static volatile bool initCalled = false;

extern const GPIOTiva_Config GPIOTiva_config;

/*
 *  ======== getPinNumber ========
 *
 *  Internal function to efficiently find the index of the right most set bit.
 */
static inline uint32_t getPinNumber(uint32_t x) {
#if defined(__TI_COMPILER_VERSION__)
    return __clz(__rbit(x));
#elif defined(codered) || defined(__GNUC__) || defined(sourcerygxx)
    return __builtin_ctz(x);
#elif defined(__IAR_SYSTEMS_ICC__)
    return __CLZ(__RBIT(x));
#elif defined(rvmdk) || defined(__ARMCC_VERSION)
    return __clz(__rbit(x));
#else
    #error "Unsupported compiler used"
#endif
}

/*
 *  ======== getGpioIntIndex ========
 *
 *  Calculates the index into gpioInterruptVectors for a given pinConfig.
 */
static uint8_t getGpioIntIndex(PinConfig *config) {
#if defined(TIVAWARE)
    /*
     * Ports which have a single interrupt for all pins are stored within the
     * first 15 entries.  Need only return the index.
     */
    if (config->port <= 0x0F) {
        return config->port;
    }

    /*
     * Ports P & Q have interrupts per each pin on the port.  These have been
     * placed at the end of the gpioInterruptVectors array.  The gpioInterruptVectors
     * index for each interrupt is determined below.
     */
    return (((config->port - 14) * 8) + getPinNumber(config->pin));
#else
    return (config->port);
#endif
}

/*
 *  ======== GPIO_clearInt ========
 */
void GPIO_clearInt(unsigned int index)
{
    PinConfig *config = (PinConfig *) &GPIOTiva_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOTiva_config.numberOfPinConfigs,
        NULL);

    GPIOIntClear(getGpioBaseAddr(config->port), config->pin);

    Log_print2(Diags_USER1, "GPIO: port 0x%x, pin 0x%x interrupt flag cleared",
        config->port, config->pin);
}

/*
 *  ======== GPIO_disableInt ========
 */
void GPIO_disableInt(unsigned int index)
{
    unsigned int key;
    PinConfig   *config = (PinConfig *) &GPIOTiva_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOTiva_config.numberOfPinConfigs,
        NULL);

    /* Make atomic update */
    key = Hwi_disable();

    GPIOIntDisable(getGpioBaseAddr(config->port), config->pin);

    Hwi_restore(key);

    Log_print2(Diags_USER1, "GPIO: port 0x%x, pin 0x%x interrupts disabled",
        config->port, config->pin);
}

/*
 *  ======== GPIO_enableInt ========
 */
void GPIO_enableInt(unsigned int index)
{
    unsigned int key;
    PinConfig   *config = (PinConfig *) &GPIOTiva_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOTiva_config.numberOfPinConfigs,
        NULL);

    /* Make atomic update */
    key = Hwi_disable();

    GPIOIntEnable(getGpioBaseAddr(config->port), config->pin);

    Hwi_restore(key);

    Log_print2(Diags_USER1, "GPIO: port 0x%x, pin 0x%x interrupts enabled",
        config->port, config->pin);
}

/*
 *  ======== GPIO_hwiIntFxn ========
 *  Hwi function that processes GPIO interrupts.
 */
void GPIO_hwiIntFxn(uintptr_t portIndex)
{
    uint32_t          gpioPins;
    uint32_t          gpioBase;
    uint32_t          gpioIndex;
    uint32_t          bitNum;
    PortCallbackInfo *portCallbackInfo;

    portCallbackInfo = &gpioCallbackInfo[portIndex];
    gpioBase = getGpioBaseAddr(portIndex);

    /* Find out which pins have their interrupt flags set */
    gpioPins = GPIOIntStatus(gpioBase, 0xFF) & 0xFF;

    /* Clear all the set bits at once */
    GPIOIntClear(gpioBase, gpioPins);

    /* Match each set bit to its corresponding callback function */
    while (gpioPins) {
        /* Gets the lowest order set bit number */
        bitNum = getPinNumber(gpioPins);
        gpioIndex = portCallbackInfo->pinIndex[bitNum];
        if (gpioIndex != CALLBACK_INDEX_NOT_CONFIGURED) {
            GPIOTiva_config.callbacks[gpioIndex]();
        }
        gpioPins &= ~(1 << bitNum);
    }
}

/*
 *  ======== GPIO_init ========
 */
void GPIO_init()
{
    unsigned int i, j;

    initCalled = true;

#if defined(TIVAWARE)
    if ((HWREG(SYSCTL_DID0) & SYSCTL_DID0_CLASS_M) ==
        SYSCTL_DID0_CLASS_TM4C129) {
        gpioInterruptVectors = gpioTM4C129InterruptVectors;
    }
    else {
        gpioInterruptVectors = gpioTM4C123InterruptVectors;
    }
#endif

    /* Initialize all entries with 'not configured' key */
    for (i = 0; i < NUM_PORTS; i++) {
        for (j = 0; j < NUM_PINS_PER_PORT; j++) {
            gpioCallbackInfo[i].pinIndex[j] = CALLBACK_INDEX_NOT_CONFIGURED;
        }
    }

    /* Configure pins and create Hwis per static array content */
    for (i = 0; i < GPIOTiva_config.numberOfPinConfigs; i++) {
        if ((GPIOTiva_config.pinConfigs[i] & GPIO_DO_NOT_CONFIG) !=
            GPIO_DO_NOT_CONFIG) {
            GPIO_setConfig(i, GPIOTiva_config.pinConfigs[i]);
        }
        if (i < GPIOTiva_config.numberOfCallbacks) {
            if (GPIOTiva_config.callbacks[i] != NULL) {
                GPIO_setCallback(i, GPIOTiva_config.callbacks[i]);
            }
        }
    }
}

/*
 *  ======== GPIO_read ========
 */
unsigned int GPIO_read(unsigned int index)
{
    unsigned int value;
    PinConfig   *config = (PinConfig *) &GPIOTiva_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOTiva_config.numberOfPinConfigs,
        NULL);

    value = GPIOPinRead(getGpioBaseAddr(config->port), config->pin);

    Log_print3(Diags_USER1, "GPIO: port 0x%x, pin 0x%x read 0x%x", config->port,
        config->pin, value);

    return ((value & config->pin) ? 1 : 0);
}

/*
 *  ======== GPIO_setCallback ========
 */
void GPIO_setCallback(unsigned int index, GPIO_CallbackFxn callback)
{
    unsigned int key;
    PinConfig   *config = (PinConfig *) &GPIOTiva_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOTiva_config.numberOfCallbacks,
        NULL);

    /* Make atomic update */
    key = Hwi_disable();

    /* Store index into corresponding port's callbackInfo pinIndex entry */
    gpioCallbackInfo[config->port].pinIndex[getPinNumber(config->pin)] =
        (callback) ? index : CALLBACK_INDEX_NOT_CONFIGURED;

    /*
     * Only update callBackFunctions entry if different. This allows the
     * callBackFunctions array to be in flash for static systems.
     */
    if (GPIOTiva_config.callbacks[index] != callback) {
        GPIOTiva_config.callbacks[index] = callback;
    }

    Hwi_restore(key);
}

/*
 *  ======== GPIO_setConfig ========
 */
void GPIO_setConfig(unsigned int index, GPIO_PinConfig pinConfig)
{
    unsigned int   key;
    Error_Block    eb;
    Hwi_Params     hwiParams;
    Hwi_Handle     handle;
    uint32_t       gpioBase;
    uint8_t        gpioDirection;
    uint8_t        gpioPin;
    uint8_t        gpioPortIntIdx;
    uint32_t       gpioPortIntBitMask;
#if defined(TIVAWARE)
    uint8_t        gpioStrength;
#endif
    uint8_t        gpioType;
    uint8_t        gpioTypeIdx;
    GPIO_PinConfig gpioPinConfig;
    PinConfig     *config = (PinConfig *) &GPIOTiva_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOTiva_config.numberOfPinConfigs,
        NULL);

    gpioBase = getGpioBaseAddr(config->port);
    gpioPin = config->pin;

    if (!(pinConfig & GPIO_CFG_IN_INT_ONLY)) {
        /* Get GPIO configuration settings */

#if defined(TIVAWARE)
        gpioStrength = getGpioStrength(pinConfig);
#endif
        gpioTypeIdx = getGpioTypeIndex(pinConfig);

        /* Determine settings for GPIO as input or output */
        if (pinConfig & GPIO_CFG_INPUT) {
            gpioDirection = GPIO_DIR_MODE_IN;
            gpioType = gpioCfgInputTypes[gpioTypeIdx];
        }
        else {
            gpioDirection = GPIO_DIR_MODE_OUT;
            gpioType = gpioCfgOutputTypes[gpioTypeIdx];
        }

        /* Make atomic update */
        key = Hwi_disable();

        /* Configure the GPIO pin */
        GPIODirModeSet(gpioBase, gpioPin, gpioDirection);
#if defined(TIVAWARE)
        GPIOPadConfigSet(gpioBase, gpioPin, gpioStrength, gpioType);
#else
        GPIOPadConfigSet(gpioBase, gpioPin, gpioType);
#endif

        /* Set output value */
        if (gpioDirection == GPIO_DIR_MODE_OUT) {
            GPIOPinWrite(gpioBase, gpioPin,
                ((pinConfig & GPIO_CFG_OUT_HIGH) ? gpioPin : 0));
        }

        /* Update pinConfig with the latest GPIO configuration */
        gpioPinConfig = GPIOTiva_config.pinConfigs[index];
        gpioPinConfig &= ~GPIO_CFG_IO_MASK;
        gpioPinConfig |= (pinConfig & GPIO_CFG_IO_MASK);
        GPIOTiva_config.pinConfigs[index] = gpioPinConfig;

        Hwi_restore(key);
    }

    /* Set type of interrupt and then clear it */
    if (pinConfig & GPIO_CFG_INT_MASK) {
        /* Calculate the gpioInterruptVectors index for the GPIO pin */
        gpioPortIntIdx = getGpioIntIndex(config);
        gpioPortIntBitMask = 1 << gpioPortIntIdx;

        /* If Hwi has not already been created, do so */
        if (!(portHwiCreatedBitMask & gpioPortIntBitMask)) {
            Error_init(&eb);
            Hwi_Params_init(&hwiParams);
            hwiParams.arg = (uintptr_t) config->port;
            hwiParams.priority = GPIOTiva_config.intPriority;
            handle = Hwi_create(gpioInterruptVectors[gpioPortIntIdx],
                GPIO_hwiIntFxn, &hwiParams, &eb);
            if (!handle) {
                Log_print1(Diags_USER1,
                    "GPIO: Error constructing Hwi for GPIO Port %d",
                    config->port);
                return;
            }
        }

        key = Hwi_disable();

        /* Mark the Hwi as created */
        portHwiCreatedBitMask |= gpioPortIntBitMask;

        GPIOIntTypeSet(gpioBase, gpioPin, getGpioIntType(pinConfig));
        GPIOIntClear(gpioBase, gpioPin);

        /* Update pinConfig with the latest interrupt configuration */
        gpioPinConfig = GPIOTiva_config.pinConfigs[index];
        gpioPinConfig &= ~(GPIO_CFG_INT_MASK);
        gpioPinConfig |= (pinConfig & GPIO_CFG_INT_MASK);
        GPIOTiva_config.pinConfigs[index] = gpioPinConfig;

        Hwi_restore(key);
    }
}

/*
 *  ======== GPIO_toggle ========
 */
void GPIO_toggle(unsigned int index)
{
    unsigned int key;
    uint32_t     value;
    PinConfig   *config = (PinConfig *) &GPIOTiva_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOTiva_config.numberOfPinConfigs,
        NULL);
    Assert_isTrue((GPIOTiva_config.pinConfigs[index] & GPIO_CFG_INPUT) ==
        GPIO_CFG_OUTPUT, NULL);

    /* Make atomic update */
    key = Hwi_disable();

    value = GPIOPinRead(getGpioBaseAddr(config->port), config->pin);
    value ^= config->pin;
    GPIOPinWrite(getGpioBaseAddr(config->port), config->pin, value);

    /* Update pinConfig with new output value */
    GPIOTiva_config.pinConfigs[index] ^= GPIO_CFG_OUT_HIGH;

    Hwi_restore(key);

    Log_print2(Diags_USER1, "GPIO: port 0x%x, pin 0x%x toggled", config->port,
        config->pin);
}

/*
 *  ======== GPIO_write ========
 */
void GPIO_write(unsigned int index, unsigned int value)
{
    unsigned int key;
    uint32_t     output;
    PinConfig   *config = (PinConfig *) &GPIOTiva_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOTiva_config.numberOfPinConfigs,
        NULL);
    Assert_isTrue((GPIOTiva_config.pinConfigs[index] & GPIO_CFG_INPUT) ==
        GPIO_CFG_OUTPUT, NULL);

    key = Hwi_disable();

    /* Clear output from pinConfig */
    GPIOTiva_config.pinConfigs[index] &= ~GPIO_CFG_OUT_HIGH;

    if (value) {
        output = config->pin;

        /* Set the pinConfig output bit to high */
        GPIOTiva_config.pinConfigs[index] |= GPIO_CFG_OUT_HIGH;
    }
    else {
        output = value;
    }

    GPIOPinWrite(getGpioBaseAddr(config->port), config->pin, output);

    Hwi_restore(key);

    Log_print3(Diags_USER1, "GPIO: port 0x%x, pin 0x%x wrote 0x%x",
        config->port, config->pin, value);
}
