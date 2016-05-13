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

#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Log.h>

#include <ti/sysbios/BIOS.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/WiFi.h>
#include <ti/drivers/wifi/WiFiCC3100.h>

#include <stdbool.h>

#if defined(MSP430WARE) || defined(MSP432WARE)
#if defined (MSP432WARE)
#include <rom.h>
#include <rom_map.h>
#endif
#include <gpio.h>
#else
#include <inc/hw_types.h>
#include <driverlib/gpio.h>
#endif

#include <simplelink.h>
#include <board.h>
#include <spi.h>
#include <osi.h>

void WiFiCC3100_close(WiFi_Handle handle);
int WiFiCC3100_control(WiFi_Handle handle, unsigned int cmd, void *arg);
void WiFiCC3100_init(WiFi_Handle handle);
WiFi_Handle WiFiCC3100_open(WiFi_Handle handle, unsigned int spiIndex,
	WiFi_evntCallback evntCallback, WiFi_Params *params);
void WiFiCC3100_hostIntHandler(UArg arg);
static void WiFiCC3100_spiCallbackFxn(SPI_Handle spiHandle,
	SPI_Transaction *transaction);
static void WiFiCC3100_assertCS();
static void WiFiCC3100_deAssertCS();

/* CC3100 WiFi driver function table implementation */
const WiFi_FxnTable WiFiCC3100_fxnTable = {
    WiFiCC3100_close,
    WiFiCC3100_control,
    WiFiCC3100_init,
    WiFiCC3100_open
};

extern const WiFi_Params WiFi_defaultParams;

volatile static WiFi_Handle wifiHandle = NULL;

#define DMA_WINDOW_SIZE (1024)

/* Defines to resolve differences between GPIO APIs */
#if defined(MWARE)
#define GPIOIntClear        GPIOPinIntClear
#define GPIOIntDisable      GPIOPinIntDisable
#define GPIOIntEnable       GPIOPinIntEnable
#endif

#if defined (MSP430WARE)
#define MAP_GPIO_clearInterruptFlag  GPIO_clearInterrupt
#define MAP_GPIO_disableInterrupt    GPIO_disableInterrupt
#define MAP_GPIO_enableInterrupt     GPIO_enableInterrupt
#define MAP_GPIO_setOutputHighOnPin  GPIO_setOutputHighOnPin
#define MAP_GPIO_setOutputLowOnPin   GPIO_setOutputLowOnPin
#endif

/*
 *  ======== WiFiCC3100_close ========
 */
void WiFiCC3100_close(WiFi_Handle handle)
{
    unsigned int              key;
    WiFiCC3100_Object        *object = handle->object;
    WiFiCC3100_HWAttrs const *hwAttrs = handle->hwAttrs;

#if defined(MSP430WARE) || defined(MSP432WARE)
    MAP_GPIO_disableInterrupt(hwAttrs->irqPort, hwAttrs->irqPin);
#else
    GPIOIntDisable(hwAttrs->irqPort, hwAttrs->irqPin);
#endif

#if !defined(MSP430WARE)
    Hwi_destruct(&(object->wifiHwi));
#endif

    Semaphore_destruct(&(object->readSemaphore));
    Semaphore_destruct(&(object->writeSemaphore));

    Log_print0(Diags_USER1, "WiFi Object closed.");

    key = Hwi_disable();
    object->spiState = WiFiCC3100_SPI_UNINITIALIZED;
    object->isOpen = false;
    Hwi_restore(key);
}

/*
 *  ======== WiFiCC3100_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int WiFiCC3100_control(WiFi_Handle handle, unsigned int cmd, void *arg)
{
    /* No implementation yet */
    return (WiFi_STATUS_UNDEFINEDCMD);
}

/*
 *  ======== WiFiCC3100_init ========
 */
void WiFiCC3100_init(WiFi_Handle handle)
{
	WiFiCC3100_Object *object = handle->object;

    /*
     *  We save the handle to a global variable to be used in the functions that
     *  are called from the CC3100 Host Driver.
     */
    if (wifiHandle == NULL) {
        wifiHandle = handle;

        /* Mark the object as available. */
        object->isOpen = false;
        object->spiState = WiFiCC3100_SPI_UNINITIALIZED;
    }
}

/*
 *  ======== WiFiCC3100_open ========
 */
WiFi_Handle WiFiCC3100_open(WiFi_Handle handle, unsigned int spiIndex,
	WiFi_evntCallback evntCallback, WiFi_Params *params)
{
    unsigned int              key;
    WiFiCC3100_Object        *object = handle->object;
    WiFiCC3100_HWAttrs const *hwAttrs = handle->hwAttrs;
    union {
#if !defined(MSP430WARE)
        Hwi_Params            hwiParams;
#endif
        Semaphore_Params      semParams;
    } paramsUnion;

    key = Hwi_disable();
    if (object->isOpen) {
        Hwi_restore(key);
        Log_warning0("WiFi Hwi already in use.");
        return (NULL);
    }
    object->isOpen = true;
    Hwi_restore(key);

    /* Construct semaphores to block read/write transactions. */
    Semaphore_Params_init(&(paramsUnion.semParams));
    paramsUnion.semParams.mode = Semaphore_Mode_BINARY;
    paramsUnion.semParams.instance->name = "WiFi.writeSemaphore";
    Semaphore_construct(&(object->writeSemaphore), 0, &(paramsUnion.semParams));

    paramsUnion.semParams.instance->name = "WiFi.readSemaphore";
    Semaphore_construct(&(object->readSemaphore), 0, &(paramsUnion.semParams));

#if !defined(MSP430WARE)
    Hwi_Params_init(&(paramsUnion.hwiParams));
    paramsUnion.hwiParams.arg = (UArg) handle;
    paramsUnion.hwiParams.enableInt = false;

    /* Hwi_construct cannot fail, use NULL instead of an Error Block */
    Hwi_construct(&(object->wifiHwi), hwAttrs->irqIntNum,
	    WiFiCC3100_hostIntHandler, &(paramsUnion.hwiParams), NULL);
#endif

#if defined(MSP430WARE) || defined(MSP432WARE)
    MAP_GPIO_clearInterruptFlag(hwAttrs->irqPort, hwAttrs->irqPin);
    MAP_GPIO_enableInterrupt(hwAttrs->irqPort, hwAttrs->irqPin);
#else
    GPIOIntClear(hwAttrs->irqPort, hwAttrs->irqPin);
    GPIOIntEnable(hwAttrs->irqPort, hwAttrs->irqPin);
#endif

    /* Store SPI interface parameters */
    object->spiIndex = spiIndex;
    object->bitRate = params->bitRate;

    return (handle);
}

/*
 *  ======== WiFiCC3100_spiCallbackFxn ========
 *  Callback function for when the SPI has completed a transfer
 *
 *  @pre    Function assumes that the handle is not NULL
 */
static void WiFiCC3100_spiCallbackFxn(SPI_Handle spiHandle,
	SPI_Transaction *transaction)
{
    WiFiCC3100_Object *object = wifiHandle->object;

    switch (object->spiState) {
        case WiFiCC3100_SPI_WRITE_MSG:
            Semaphore_post(Semaphore_handle(&(object->writeSemaphore)));
            break;

        case WiFiCC3100_SPI_WRITE_EOT:
            object->spiState = WiFiCC3100_SPI_IDLE;
            Semaphore_post(Semaphore_handle(&(object->writeSemaphore)));
            break;

        case WiFiCC3100_SPI_READ_MSG:
            Semaphore_post(Semaphore_handle(&(object->readSemaphore)));
            break;

        case WiFiCC3100_SPI_READ_EOT:
            object->spiState = WiFiCC3100_SPI_IDLE;
            Semaphore_post(Semaphore_handle(&(object->readSemaphore)));
            break;

        default:
            break;
    }
}

/*
 *  ======== spi_Open ========
 *  SimpleLink Host Driver API to open a SPI communication interface.
 */
Fd_t spi_Open(char *ifName, unsigned long flags)
{
    WiFiCC3100_Object *object = wifiHandle->object;
    SPI_Params         spiParams;

    SPI_Params_init(&spiParams);
    spiParams.bitRate = object->bitRate;
    spiParams.transferMode = SPI_MODE_CALLBACK;
    spiParams.transferCallbackFxn = WiFiCC3100_spiCallbackFxn;
    object->spiHandle = SPI_open(object->spiIndex, &spiParams);
    if (object->spiHandle == NULL) {
        Log_error1("WiFi SPI (%p) could not be initialized\n", object->spiIndex);
        return (-1);
    }

    object->spiState = WiFiCC3100_SPI_IDLE;

    Log_print1(Diags_USER1, "WiFi SPI (%p) open\n", (Fd_t) object->spiHandle);
    return ((Fd_t) object->spiHandle);
}

/*
 *  ======== spi_Close ========
 *  SimpleLink Host Driver API to close a SPI communication interface.
 */
int spi_Close(Fd_t fd)
{
    WiFiCC3100_Object *object = wifiHandle->object;

    SPI_close(object->spiHandle);
    object->spiState = WiFiCC3100_SPI_UNINITIALIZED;

    Log_print1(Diags_USER1, "WiFi SPI (%p) closed\n", (Fd_t) object->spiHandle);

    return (0);
}

/*
 *  ======== spi_Read ========
 *  SimpleLink Host Driver API to receive data from the SPI communication
 *  interface.
 */
int spi_Read(Fd_t fd, unsigned char *pBuff, int len)
{
	WiFiCC3100_Object *object = wifiHandle->object;
	SPI_Transaction    transaction;
	uint32_t           msgLength = 0;

	Assert_isTrue(object->spiState == WiFiCC3100_SPI_IDLE, NULL);

	while (msgLength < len) {
		/*
		 * The DMA can only perform a maximum of DMA_WINDOW_SIZE word transfers
		 * at a time.  Read transactions are split into DMA_WINDOW_SIZE
		 * transfers until complete.
		 */
		if (len - msgLength > DMA_WINDOW_SIZE) {
			object->spiState = WiFiCC3100_SPI_READ_MSG;
			transaction.count = DMA_WINDOW_SIZE;
		}
		else {
			object->spiState = WiFiCC3100_SPI_READ_EOT;
			transaction.count = len - msgLength;
		}
		transaction.rxBuf = (pBuff + msgLength);
		transaction.txBuf = NULL;

		WiFiCC3100_assertCS();

		if (!SPI_transfer(object->spiHandle, &transaction)) {
			Log_error1("SPI transfer failed: %d bytes read.", transaction.count);
			return (-1);
		}

		Semaphore_pend((Semaphore_Handle)(&(object->readSemaphore)),
			BIOS_WAIT_FOREVER);

		WiFiCC3100_deAssertCS();

		msgLength += transaction.count;
	}

    Log_print1(Diags_USER2, "Read transaction complete: %d bytes read.",
	    transaction.count);

    return (msgLength);
}

/*
 *  ======== spi_Write ========
 *  SimpleLink Host Driver API to transmit data through the SPI communication
 *  interface.
 */
int spi_Write(Fd_t fd, unsigned char *pBuff, int len)
{
    WiFiCC3100_Object *object = wifiHandle->object;
    SPI_Transaction    transaction;
    uint32_t           msgLength = 0;

    Assert_isTrue(object->spiState == WiFiCC3100_SPI_IDLE, NULL);

    while (msgLength < len) {
        /*
         * The DMA can only perform a maximum of DMA_WINDOW_SIZE word transfers
         * at a time.  The host driver TX buffer can send up to 1500 bytes.
         * Transaction is split into two transfers if necessary.
         */
        if (len - msgLength > DMA_WINDOW_SIZE) {
            object->spiState = WiFiCC3100_SPI_WRITE_MSG;
            transaction.count = DMA_WINDOW_SIZE;
        }
        else {
            object->spiState = WiFiCC3100_SPI_WRITE_EOT;
            transaction.count = len - msgLength;
        }
        transaction.rxBuf = NULL;
        transaction.txBuf = (pBuff + msgLength);

        WiFiCC3100_assertCS();

        if (!SPI_transfer(object->spiHandle, &transaction)) {
            Log_error0("SPI transfer failed.");
            return (-1);
        }

        Semaphore_pend((Semaphore_Handle)(&(object->writeSemaphore)),
            BIOS_WAIT_FOREVER);

        WiFiCC3100_deAssertCS();

        msgLength += transaction.count;
    }

    Log_print1(Diags_USER2, "Write transaction complete: %d bytes written.",
	    transaction.count);

    return (msgLength);
}

/*
 *  ======== osi_RegisterInterruptHandler ========
 *  SimpleLink Host Driver API to register a function to handle network
 *  processor interrupts.
 */
int osi_RegisterInterruptHandler(P_EVENT_HANDLER InterruptHdl , void *pValue)
{
    WiFiCC3100_Object        *object = wifiHandle->object;
#if !defined(MSP430WARE)
    WiFiCC3100_HWAttrs const *hwAttrs = wifiHandle->hwAttrs;
#endif

    if(InterruptHdl == NULL) {
#if !defined(MSP430WARE)
        Hwi_disableInterrupt(hwAttrs->irqIntNum);
        Hwi_clearInterrupt(hwAttrs->irqIntNum);
#endif
        object->wifiIntFxn = NULL;
    }
    else {
        object->wifiIntFxn = InterruptHdl;
#if !defined(MSP430WARE)
        Hwi_enableInterrupt(hwAttrs->irqIntNum);
#endif
    }

    return (0);
}

/*
 *  ======== WiFiCC3100_hostIntHandler ========
 *  SimpleLink Host Driver interrupt handler.  Calls previously registered
 *  functions to handle network processor.
 */
void WiFiCC3100_hostIntHandler(UArg arg)
{
    WiFiCC3100_Object        *object = wifiHandle->object;
    WiFiCC3100_HWAttrs const *hwAttrs = wifiHandle->hwAttrs;

    Assert_isTrue(object->wifiIntFxn != NULL, NULL);

    Log_print0(Diags_USER2, "Entering WiFi Hwi handler.");

#if defined(MSP430WARE) || defined(MSP432WARE)
    MAP_GPIO_clearInterruptFlag(hwAttrs->irqPort, hwAttrs->irqPin);
#else
    GPIOIntClear(hwAttrs->irqPort, hwAttrs->irqPin);
#endif

    (*object->wifiIntFxn)();

    Log_print0(Diags_USER2, "Exiting WiFi Hwi handler.");
}

/*
 *  ======== CC3100_enable ========
 *  SimpleLink Host Driver API to toggle the CC3100 device enable pin.
 */
void CC3100_enable() {
    WiFiCC3100_HWAttrs const *hwAttrs = wifiHandle->hwAttrs;

#if defined(MSP430WARE) || defined(MSP432WARE)
    MAP_GPIO_setOutputHighOnPin(hwAttrs->enPort, hwAttrs->enPin);
#else
    GPIOPinWrite(hwAttrs->enPort, hwAttrs->enPin, hwAttrs->enPin);
#endif

    Log_print0(Diags_USER2, "CC3100 has been enabled.");
}

/*
 *  ======== CC3100_disable ========
 *  SimpleLink Host Driver API to toggle the CC3100 device enable pin.
 */
void CC3100_disable() {
    WiFiCC3100_HWAttrs const *hwAttrs = wifiHandle->hwAttrs;

#if defined(MSP430WARE) || defined(MSP432WARE)
    MAP_GPIO_setOutputLowOnPin(hwAttrs->enPort, hwAttrs->enPin);
#else
    GPIOPinWrite(hwAttrs->enPort, hwAttrs->enPin, 0);
#endif

    Log_print0(Diags_USER2, "CC3100 has been disabled.");
}

/*
 *  ======== WiFiCC3100_assertCS ========
 *  Assert the CC3100 chip select pin.
 */
static void WiFiCC3100_assertCS() {
    WiFiCC3100_HWAttrs const *hwAttrs = wifiHandle->hwAttrs;

#if defined(MSP430WARE) || defined(MSP432WARE)
    MAP_GPIO_setOutputLowOnPin(hwAttrs->csPort, hwAttrs->csPin);
#else
    GPIOPinWrite(hwAttrs->csPort, hwAttrs->csPin, 0);
#endif

    Log_print0(Diags_USER2, "SPI CS asserted, ready to transer.");
}

/*
 *  ======== WiFiCC3100_deAssertCS ========
 *  Deassert the CC3100 chip select pin.
 */
static void WiFiCC3100_deAssertCS() {
    WiFiCC3100_HWAttrs const *hwAttrs = wifiHandle->hwAttrs;

#if defined(MSP430WARE) || defined(MSP432WARE)
    MAP_GPIO_setOutputHighOnPin(hwAttrs->csPort, hwAttrs->csPin);
#else
    GPIOPinWrite(hwAttrs->csPort, hwAttrs->csPin, hwAttrs->csPin);
#endif

    Log_print0(Diags_USER2, "SPI CS deasserted, end of transfer.");
}
