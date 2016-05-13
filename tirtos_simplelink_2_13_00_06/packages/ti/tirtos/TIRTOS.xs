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
 *  ======== TIRTOS.xs ========
 */

var EMAC;
var GPIO;
var I2C;
var PWM;
var SDSPI;
var SPI;
var UART;
var USBMSCHFatFs;
var Watchdog;
var WiFi;
var TIRTOS;
var Program;

/*
 *  ======== module$use ========
 */
function module$use()
{
    TIRTOS       = xdc.module('ti.tirtos.TIRTOS');
    EMAC         = xdc.module('ti.drivers.EMAC');
    GPIO         = xdc.module('ti.drivers.GPIO');
    I2C          = xdc.module('ti.drivers.I2C');
    PWM          = xdc.module('ti.drivers.PWM');
    SDSPI        = xdc.module('ti.drivers.SDSPI');
    SPI          = xdc.module('ti.drivers.SPI');
    UART         = xdc.module('ti.drivers.UART');
    USBMSCHFatFs = xdc.module('ti.drivers.USBMSCHFatFs');
    Watchdog     = xdc.module('ti.drivers.Watchdog');
    WiFi         = xdc.module('ti.drivers.WiFi');
    Program      = xdc.module('xdc.cfg.Program');

    deprecate();
    determineSupport();
    addUse();
}

/*
 *  ======== deprecate ========
 *  Give the user a warning if they done a useModule on the driver.
 */
function deprecate()
{
    if (EMAC.$used == true) {
        TIRTOS.$logWarning("The EMAC module is deprecated. Please set " +
                "ti.tirtos.TIRTOS.useEMAC to true in the .cfg instead (via graphical tool or text editor).", TIRTOS);
    }

    if (GPIO.$used == true) {
        TIRTOS.$logWarning("The GPIO module is deprecated. Please set " +
                "ti.tirtos.TIRTOS.useGPIO to true in the .cfg instead (via graphical tool or text editor).", TIRTOS);
    }

    if (I2C.$used == true) {
        TIRTOS.$logWarning("The I2C module is deprecated. Please set " +
                "ti.tirtos.TIRTOS.useI2C to true in the .cfg instead (via graphical tool or text editor).", TIRTOS);
    }

    if (PWM.$used == true) {
        TIRTOS.$logWarning("The PWM module is deprecated. Please set " +
                "ti.tirtos.TIRTOS.usePWM to true in the .cfg instead (via graphical tool or text editor).", TIRTOS);
    }

    if (SDSPI.$used == true) {
        TIRTOS.$logWarning("The SDSPI module is deprecated. Please set " +
                "ti.tirtos.TIRTOS.useSDSPI to true in the .cfg instead (via graphical tool or text editor).", TIRTOS);
    }

    if (SPI.$used == true) {
        /* WiFi is still pulling SPI in. Do not warn for this */
        if (WiFi.$used != true) {
            TIRTOS.$logWarning("The SPI module is deprecated. Please set " +
                "ti.tirtos.TIRTOS.useSPI to true in the .cfg instead (via graphical tool or text editor).", TIRTOS);
        }
    }

    if (UART.$used == true) {
        TIRTOS.$logWarning("The UART module is deprecated. Please set " +
                "ti.tirtos.TIRTOS.useUART to true in the .cfg instead (via graphical tool or text editor).", TIRTOS);
    }

    if (USBMSCHFatFs.$used == true) {
        TIRTOS.$logWarning("The USBMSCHFatFs module is deprecated. Please set " +
                "ti.tirtos.TIRTOS.useUSBMSCHFatFs to true in the .cfg instead (via graphical tool or text editor).", TIRTOS);
    }

    if (Watchdog.$used == true) {
        TIRTOS.$logWarning("The Watchdog module is deprecated. Please set " +
                "ti.tirtos.TIRTOS.useWatchdog to true in the .cfg instead (via graphical tool or text editor).", TIRTOS);
    }
}

/*
 *  ======== determineSupport ========
 *  Determine, based on board, which drivers are supported
 */
function determineSupport()
{
    /* Camera driver is only support on CC3200 devices */
    if (Program.cpu.deviceName.match(/CC32/)) {
        TIRTOS.supportsCamera = true;
    }
    else {
        TIRTOS.supportsCamera = false;
    }

    /* EMAC driver is only support on Concerto and TM4C129 devices */
    if ((Program.cpu.deviceName.match(/F28M3/)) ||
        (Program.cpu.deviceName.match(/TM4C129/))) {
        TIRTOS.supportsEMAC = true;
    }
    else {
        TIRTOS.supportsEMAC = false;
    }

    /* GPIO driver is not supported on CC26xx/CC13xx */
    if ((Program.cpu.deviceName.match(/CC13/)) ||
        (Program.cpu.deviceName.match(/CC26/))) {
        TIRTOS.supportsGPIO = false;
    }
    else {
        TIRTOS.supportsGPIO = true;
    }

    /* Every board supported I2C */
    TIRTOS.supportsI2C = true;

    /* I2S driver is only support on CC3200 devices */
    if (Program.cpu.deviceName.match(/CC32/)) {
        TIRTOS.supportsI2S = true;
    }
    else {
        TIRTOS.supportsI2S = false;
    }

    /* Power driver (ti.drivers.power) is only supported for CC3200 & MSP432 */
    if ((Program.cpu.deviceName.match(/CC32/)) ||
        (Program.cpu.deviceName.match(/MSP432/))) {
        TIRTOS.supportsPower = true;
        TIRTOS.usePower = true;
    }
    else {
        TIRTOS.supportsPower = false;
        TIRTOS.usePower = false;
    }

    /* PWM driver is only support on CC3200, MSP430/2 and TivaC devices */
    if ((Program.cpu.deviceName.match(/CC32/)) ||
        (Program.cpu.deviceName.match(/TM4C12/)) ||
        (Program.cpu.deviceName.match(/MSP43/))) {
        TIRTOS.supportsPWM = true;
    }
    else {
        TIRTOS.supportsPWM = false;
    }

    /* SDSPI driver is not supported on CC26xx */
    if ((Program.cpu.deviceName.match(/CC13/)) ||
        (Program.cpu.deviceName.match(/CC26/))) {
        TIRTOS.supportsSDSPI = false;
    }
    else {
        TIRTOS.supportsSDSPI = true;
    }

    /* Every board supported SPI */
    TIRTOS.supportsSPI = true;

    /* Every board supported UART */
    TIRTOS.supportsUART = true;

    /* USBMSCHFatFs driver is only support on Concerto and TivaC devices */
    if ((Program.cpu.deviceName.match(/F28M3/)) ||
        (Program.cpu.deviceName.match(/TM4C12/))) {
        TIRTOS.supportsUSBMSCHFatFs = true;
    }
    else {
        TIRTOS.supportsUSBMSCHFatFs = false;
    }

    /* Watchdog driver is not supported on CC26xx */
    if ((Program.cpu.deviceName.match(/CC13/)) ||
        (Program.cpu.deviceName.match(/CC26/))) {
        TIRTOS.supportsWatchdog = false;
    }
    else {
        TIRTOS.supportsWatchdog = true;
    }
}

/*
 *  ======== addUse ========
 *  If they have requested the driver, call the useModule.
 */
function addUse()
{
    if (TIRTOS.useEMAC == true) {
        xdc.useModule('ti.drivers.EMAC');
    }

    if (TIRTOS.useGPIO == true) {
        xdc.useModule('ti.drivers.GPIO');
    }

    if (TIRTOS.useI2C == true) {
        xdc.useModule('ti.drivers.I2C');
    }

    if (TIRTOS.usePower == true) {
        xdc.useModule('ti.drivers.Power');
    }

    if (TIRTOS.usePWM == true) {
        xdc.useModule('ti.drivers.PWM');
    }

    if (TIRTOS.useSDSPI == true) {
        xdc.useModule('ti.drivers.SDSPI');
    }

    if (TIRTOS.useSPI == true) {
        xdc.useModule('ti.drivers.SPI');
    }

    if (TIRTOS.useUART == true) {
        xdc.useModule('ti.drivers.UART');
    }

    if (TIRTOS.useUSBMSCHFatFs == true) {
        xdc.useModule('ti.drivers.USBMSCHFatFs');
    }

    if (TIRTOS.useWatchdog == true) {
        xdc.useModule('ti.drivers.Watchdog');
    }
}

/*
 *  ======== module$validate ========
 */
function module$validate()
{
    Program.global.TI_DRIVERS_WIFI_INCLUDED = WiFi.$used;
}
