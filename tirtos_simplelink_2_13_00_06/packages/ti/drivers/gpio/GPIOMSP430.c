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

#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Log.h>

#include <ti/sysbios/hal/Hwi.h>

#include <ti/drivers/GPIO.h>
#include <ti/drivers/gpio/GPIOMSP430.h>

#include <gpio.h>

/* Table of GPIO interrupt types */
static const uint8_t gpioInterruptTypes[] = {
    0,                  /* Undefined interrupt type */
    GPIO_HIGH_TO_LOW_TRANSITION,
    GPIO_LOW_TO_HIGH_TRANSITION,
    GPIO_HIGH_TO_LOW_TRANSITION | GPIO_LOW_TO_HIGH_TRANSITION,
    GPIO_INPUT_PIN_LOW,
    GPIO_INPUT_PIN_HIGH
};

#define NUM_INTERRUPT_PORTS  (4)
#define NUM_PINS_PER_PORT    (8)

/*
 * Extracts the GPIO interrupt type from the pinConfig.  Value to index into the
 * gpioInterruptTypes table.
 */
#define getGpioIntTypeIndex(pinConfig) \
    ((pinConfig & GPIO_CFG_INT_MASK) >> GPIO_CFG_INT_LSB)

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
static PortCallbackInfo gpioCallbackInfo[NUM_INTERRUPT_PORTS];

/* Boolean to confirm that GPIO_init() has been called */
static volatile bool initCalled = false;

extern const GPIOMSP430_Config GPIOMSP430_config;

/*
 *  ======== getPinNumber ========
 *
 *  Function to find the index of the right most set bit
 */
static uint8_t getPinNumber(uint8_t x) {
    uint8_t pinNum;

    for (pinNum = 0; x; pinNum++) {
        if (x & 0x01) {
            return pinNum;
        }

        x >>= 1;
    }

    return 0;
}

/*
 *  ======== GPIO_clearInt ========
 */
void GPIO_clearInt(unsigned int index)
{
    unsigned int key;
    PinConfig   *config = (PinConfig *) &GPIOMSP430_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOMSP430_config.numberOfPinConfigs,
        NULL);

    /* Make atomic update */
    key = Hwi_disable();

    GPIO_clearInterrupt(config->port, config->pin);

    Hwi_restore(key);

    Log_print2(Diags_USER1, "GPIO: port 0x%x, pin 0x%x interrupt flag cleared",
        config->port, config->pin);
}

/*
 *  ======== GPIO_disableInt ========
 */
void GPIO_disableInt(unsigned int index)
{
    unsigned int key;
    PinConfig   *config = (PinConfig *) &GPIOMSP430_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOMSP430_config.numberOfPinConfigs,
        NULL);

    /* Make atomic update */
    key = Hwi_disable();

    GPIO_disableInterrupt(config->port, config->pin);

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
    PinConfig   *config = (PinConfig *) &GPIOMSP430_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOMSP430_config.numberOfPinConfigs,
        NULL);

    /* Make atomic update */
    key = Hwi_disable();

    GPIO_enableInterrupt(config->port, config->pin);

    Hwi_restore(key);

    Log_print2(Diags_USER1, "GPIO: port 0x%x, pin 0x%x interrupts enabled",
        config->port, config->pin);
}

/*
 *  ======== GPIO_hwiIntFxn ========
 *  Hwi function that processes GPIO interrupts.
 */
void GPIO_hwiIntFxn(unsigned long portIndex)
{
    unsigned int      i;
    uint8_t           gpioPort;
    uint8_t           gpioIndex;
    uint16_t          gpioPins;
    PortCallbackInfo *portCallbackInfo;

    gpioPort = (uint8_t) ((uint32_t) portIndex);
    portCallbackInfo = &gpioCallbackInfo[gpioPort - 1];

    /* Find out which pins have their interrupt flags set */
    gpioPins = GPIO_getInterruptStatus(gpioPort, 0xFF);

    if ((gpioPort & 0x01) ^ 1) {
        /* If using a even port (high byte), shift bits down */
        gpioPins >>= 8;
    }

    /* Clear all the set bits at once */
    GPIO_clearInterrupt(gpioPort, gpioPins);

    /* Match each set bit to its corresponding callback function */
    for (i = 0; gpioPins; i++) {
        if (gpioPins & 0x1) {
            gpioIndex = portCallbackInfo->pinIndex[i];
            if (gpioIndex != CALLBACK_INDEX_NOT_CONFIGURED) {
                GPIOMSP430_config.callbacks[gpioIndex]();
            }
        }
        gpioPins >>= 1;
    }
}

/*
 *  ======== GPIO_init ========
 */
void GPIO_init()
{
    unsigned int i, j;

    initCalled = true;

    /* Initialize all entries with 'not configured' key */
    for (i = 0; i < NUM_INTERRUPT_PORTS; i++) {
        for (j = 0; j < NUM_PINS_PER_PORT; j++) {
            gpioCallbackInfo[i].pinIndex[j] = CALLBACK_INDEX_NOT_CONFIGURED;
        }
    }

    /* Configure pins */
    for (i = 0; i < GPIOMSP430_config.numberOfPinConfigs; i++) {
        if ((GPIOMSP430_config.pinConfigs[i] & GPIO_DO_NOT_CONFIG) !=
            GPIO_DO_NOT_CONFIG) {
            GPIO_setConfig(i, GPIOMSP430_config.pinConfigs[i]);
        }
        if (i < GPIOMSP430_config.numberOfCallbacks) {
            if (GPIOMSP430_config.callbacks[i] != NULL) {
                GPIO_setCallback(i, GPIOMSP430_config.callbacks[i]);
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
    PinConfig   *config = (PinConfig *) &GPIOMSP430_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOMSP430_config.numberOfPinConfigs,
        NULL);

    value = GPIO_getInputPinValue(config->port, config->pin);

    Log_print3(Diags_USER1, "GPIO: port 0x%x, pin 0x%x read 0x%x", config->port,
        config->pin, value);

    return (value);
}

/*
 *  ======== GPIO_setCallback ========
 */
void GPIO_setCallback(unsigned int index, GPIO_CallbackFxn callback)
{
    unsigned int key;
    uint8_t      gpioPinNum;
    uint8_t      gpioPortIdx;
    PinConfig   *config = (PinConfig *) &GPIOMSP430_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOMSP430_config.numberOfCallbacks,
        NULL);

    /* Make atomic update */
    key = Hwi_disable();

    gpioPortIdx = config->port - 1;
    gpioPinNum = getPinNumber(config->pin);

    /* Store index into corresponding port's callbackInfo pinIndex entry */
    gpioCallbackInfo[gpioPortIdx].pinIndex[gpioPinNum] =
        (callback) ? index : CALLBACK_INDEX_NOT_CONFIGURED;

    /*
     * Only update callBackFunctions entry if different. This allows the
     * callBackFunctions array to be in flash for static systems.
     */
    if (GPIOMSP430_config.callbacks[index] != callback) {
        GPIOMSP430_config.callbacks[index] = callback;
    }

    Hwi_restore(key);
}

/*
 *  ======== GPIO_setConfig ========
 */
void GPIO_setConfig(unsigned int index, GPIO_PinConfig pinConfig)
{
    unsigned int   key;
    uint8_t        gpioPort;
    uint8_t        gpioPin;
    GPIO_PinConfig gpioPinConfig;
    PinConfig     *config = (PinConfig *) &GPIOMSP430_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOMSP430_config.numberOfPinConfigs,
        NULL);

    gpioPort = config->port;
    gpioPin = config->pin;

    /* Make atomic update */
    key = Hwi_disable();

    if (!(pinConfig & GPIO_CFG_IN_INT_ONLY)) {
        if (pinConfig & GPIO_CFG_INPUT) {
            /* Configure as input */
            if ((pinConfig & GPIO_CFG_IN_PU) == GPIO_CFG_IN_PU) {
                GPIO_setAsInputPinWithPullUpResistor(gpioPort, gpioPin);
            }
            else if ((pinConfig & GPIO_CFG_IN_PD) == GPIO_CFG_IN_PD) {
                GPIO_setAsInputPinWithPullDownResistor(gpioPort, gpioPin);
            }
            else {
                GPIO_setAsInputPin(gpioPort, gpioPin);
            }
        }
        else {
            /* Configure as output */
            GPIO_setAsOutputPin(gpioPort, gpioPin);

            if (pinConfig & GPIO_CFG_OUT_HIGH) {
                GPIO_setOutputHighOnPin(gpioPort, gpioPin);
            }
            else {
                GPIO_setOutputLowOnPin(gpioPort, gpioPin);
            }
        }

        /* Update pinConfig with the latest GPIO configuration */
        gpioPinConfig = GPIOMSP430_config.pinConfigs[index];
        gpioPinConfig &= ~GPIO_CFG_IO_MASK;
        gpioPinConfig |= (pinConfig & GPIO_CFG_IO_MASK);
        GPIOMSP430_config.pinConfigs[index] = gpioPinConfig;
    }

    /* Set type of interrupt and clear it */
    if (pinConfig & GPIO_CFG_INT_MASK) {
        GPIO_selectInterruptEdge(gpioPort, gpioPin,
            gpioInterruptTypes[getGpioIntTypeIndex(pinConfig)]);
        GPIO_clearInterrupt(gpioPort, gpioPin);

        /* Update pinConfig with the latest interrupt configuration */
        gpioPinConfig = GPIOMSP430_config.pinConfigs[index];
        gpioPinConfig &= ~(GPIO_CFG_INT_MASK);
        gpioPinConfig |= (pinConfig & GPIO_CFG_INT_MASK);
        GPIOMSP430_config.pinConfigs[index] = gpioPinConfig;
    }

    Hwi_restore(key);
}

/*
 *  ======== GPIO_toggle ========
 */
void GPIO_toggle(unsigned int index)
{
    unsigned int key;
    PinConfig   *config = (PinConfig *) &GPIOMSP430_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOMSP430_config.numberOfPinConfigs,
        NULL);
    Assert_isTrue((GPIOMSP430_config.pinConfigs[index] & GPIO_CFG_INPUT) ==
        GPIO_CFG_OUTPUT, NULL);

    /* Make atomic update */
    key = Hwi_disable();

    GPIO_toggleOutputOnPin(config->port, config->pin);

    /* Update pinConfig with new output value */
    GPIOMSP430_config.pinConfigs[index] ^= GPIO_CFG_OUT_HIGH;

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
    PinConfig   *config = (PinConfig *) &GPIOMSP430_config.pinConfigs[index];

    Assert_isTrue(initCalled && index < GPIOMSP430_config.numberOfPinConfigs,
        NULL);
    Assert_isTrue((GPIOMSP430_config.pinConfigs[index] & GPIO_CFG_INPUT) ==
        GPIO_CFG_OUTPUT, NULL);

    /* Make atomic update */
    key = Hwi_disable();

    /* Clear output from pinConfig */
    GPIOMSP430_config.pinConfigs[index] &= ~GPIO_CFG_OUT_HIGH;

    if (value) {
        GPIO_setOutputHighOnPin(config->port, config->pin);

        /* Set the pinConfig output bit to high */
        GPIOMSP430_config.pinConfigs[index] |= GPIO_CFG_OUT_HIGH;
    }
    else {
        GPIO_setOutputLowOnPin(config->port, config->pin);
    }

    Hwi_restore(key);

    Log_print3(Diags_USER1, "GPIO: port 0x%x, pin 0x%x wrote 0x%x",
        config->port, config->pin, value);
}
