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
 *  ======== WiFi.xdc ========
 */
package ti.drivers;
import xdc.rov.ViewInfo;

/*!
 *  ======== WiFi ========
 *  WiFi module configuration
 */
@NoRuntime
@HeaderName("")
@Template("./WiFi.xdt")
module WiFi {

    /*!
     *  ======== DeviceType ========
     *  WiFi device selection options
     *
     *  This enumeration defines all supported WiFi device types
     *  provided by the product.  You can select the device type by setting
     *  the {@link #deviceType WiFi.deviceType} configuration parameter.
     *
     *  @field(DeviceType_SimpleLink_CC3100) The library supplied is prebuilt
     *  with the SimpleLink CC3100 Host Driver.
     */
    enum DeviceType {
        DeviceType_SimpleLink_CC3100         /*! SimpleLink CC3100 Wi-Fi Module */
    };

    /*!
     *  ======== deviceType ========
     *  WiFi Device type
     *
     *  WiFi support is provided in the form of a libraries that are linked
     *  with your application.  Several libraries are provided with the
     *  WiFi product to support the various devices. This configuration
     *  parameter allows you to select the proper library for the WiFi
     *  device to use.
     *
     *  The default value of deviceType is
     *  {@link #DeviceType_SimpleLink_CC3100 WiFi_DeviceType_SimpleLink_CC3100}.
     *  For a complete list of options and what they offer see
     *  {@link #DeviceType}.
     */
    metaonly config DeviceType deviceType = DeviceType_SimpleLink_CC3100;

    /*!
     *  ======== LibType ========
     *  WiFi library selection options
     *
     *  This enumeration defines all the WiFi library type
     *  provided by the product.  You can select the library type by setting
     *  the {@link #libType WiFi.libType} configuration parameter.
     *
     *  @field(LibType_Instrumented) The library supplied is prebuilt with
     *  logging and assertions enabled.
     *
     *  @field(LibType_NonInstrumented) The library supplied is prebuilt
     *  with logging and assertions disabled.
     */
    enum LibType {
        LibType_Instrumented,           /*! instrumented */
        LibType_NonInstrumented         /*! non-instrumented */
    };

    /*!
     *  ======== libType ========
     *  WiFi Library type
     *
     *  The WiFi runtime is provided in the form of a library that is
     *  linked with your application.  Several forms of this library are
     *  provided with the WiFi product. This configuration parameter
     *  allows you to select the form of WiFi to use.
     *
     *  The default value of libType is
     *  {@link #LibType_Instrumented WiFi_LibType_Instrumented}.  For a
     *  complete list of options and what they offer see {@link #LibType}.
     */
    metaonly config LibType libType = LibType_Instrumented;

    /*!
     *  SpawnTask priority
     *
     *  The SpawnTask is a task created for the host driver to allow the
     *  execution of functions in a temporary context.
     */
    metaonly config Int spawnTaskPriority = 3;

    /*!
     *  @_nodoc
     *  ======== WiFi_Config ========
     *  Structure must match the WiFi_Config structure defined in WiFi.h
     */
    struct WiFi_Config {
        Void               *fxnTablePtr;
        Void               *object;
        Void               *hwAttrs;
    };

    /*!
     *  @_nodoc
     *  ======== WiFiCC3100_SPI_State ========
     *  Must match the WiFiCC3100_SPI_State enum defined in
     *  WiFiCC3100.h
     */
    enum WiFiCC3100_SPI_State {
        WiFiCC3100_SPI_UNINITIALIZED,
        WiFiCC3100_SPI_IDLE,
        WiFiCC3100_SPI_READ_EOT,
        WiFiCC3100_SPI_READ_MSG,
        WiFiCC3100_SPI_WRITE_EOT,
        WiFiCC3100_SPI_WRITE_MSG
    };

    /*!
     *  @_nodoc
     *  ======== WiFiCC3100_HWAttrs ========
     *  Structure must match the WiFiCC3100_HWAttrs structure defined
     *  in wifi/WiFiCC3100.h
     */
    struct WiFiCC3100_HWAttrs {
        ULong    irqPort;
        ULong    irqPin;
        ULong    irqIntNum;

        ULong    csPort;
        ULong    csPin;

        ULong    enPort;
        ULong    enPin;
    };

    /*!
     *  @_nodoc
     *  ======== WiFiCC3100_Object ========
     *  Structure must match the WiFiCC3100_Object structure defined
     *  in wifi/WiFiCC3100.h
     */
    struct WiFiCC3100_Object {
        Ptr       wifiHwi;
        Ptr       spiHandle;

        Void      (*wifiIntFxn)();

        Ptr       readSemaphore;
        Ptr       writeSemaphore;

        WiFiCC3100_SPI_State spiState;

        bool      isOpen;
    };

    /*!
     *  @_nodoc
     *  ======== BasicView ========
     */
    metaonly struct BasicView {
        String              functionTable;
        String              wifiHandle;
        String              spiHandle;
        String              spiState;
    };

    /*!
     *  @_nodoc
     *  ======== rovViewInfo ========
     */
    @Facet
    metaonly config ViewInfo.Instance rovViewInfo =
        ViewInfo.create({
            viewMap: [
                ['Basic',
                    {
                        type: ViewInfo.MODULE_DATA,
                        viewInitFxn: 'viewInitBasic',
                        structName: 'BasicView'
                    }
                ]
            ]
        });
}
