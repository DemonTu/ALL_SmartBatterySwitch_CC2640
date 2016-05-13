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
 *  ======== SDSPI.xdc ========
 */
package ti.drivers;
import xdc.rov.ViewInfo;

/*!
 *  ======== SDSPI ========
 */
@NoRuntime
@HeaderName("")
module SDSPI {

    /*!
     *  ======== LibType ========
     *  SDSPI library selection options
     *
     *  This enumeration defines all the SDSPI library type
     *  provided by the product.  You can select the library type by setting
     *  the {@link #libType SDSPI.libType} configuration parameter.
     *
     *  @field(LibType_Instrumented) The library supplied is prebuilt with
     *  logging and assertions enabled.
     *  Diags_USER1 mask logs basic information
     *  Diags_USER2 mask logs detailed information
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
     *  SDSPI Library type
     *
     *  Deprecated: Please use ti.tirtos.TIRTOS.libType instead.
     *
     *  The SDSPI runtime is provided in the form of a library that is
     *  linked with your application.  Several forms of this library are
     *  provided with the SDSPI product. This configuration parameter
     *  allows you to select the form of SDSPI to use.
     *
     *  The default value of libType is
     *  {@link #LibType_Instrumented SDSPI_LibType_Instrumented}.  For a
     *  complete list of options and what they offer see {@link #LibType}.
     */
    metaonly config LibType libType = LibType_Instrumented;

    /*!
     *  @_nodoc
     *  ======== maxPeripheralCount ========
     */
    metaonly config UInt maxPeripheralCount = 32;

    /*!
     *  @_nodoc
     *  ======== SDSPI_Config ========
     *  Structure must match the SDSPI_Config structure defined in SDSPI.h
     */
    struct SDSPI_Config {
        Void               *fxnTablePtr; //Used to determine the implementation
        Void               *object;      //Used to access the data
        Void               *hwAttrs;     //Used to get the attributes
    };

    /*!
     *  @_nodoc
     *  ======== SDSPI_CardType ========
     *  Structure must match the SDSPI_CardType structure defined in SDSPI.h
     */
    enum SDSPITiva_CardType {
        NOCARD = 0x0,
        MMC = 1,
        SDSC = 2,
        SDHC = 3
    };

    /*!
     *  @_nodoc
     *  ======== SDSPITiva_hwAttrs ========
     *  Structure must match the SDSPITivaDMA_hwAttrs structure defined in sdspi/SDSPITivaDMA.h
     */
    struct SDSPITiva_HWAttrs {
        Bits32 baseAddr;

        Bits32 portSPI;
        Bits32 pinSCK;
        Bits32 pinMISO;
        Bits32 pinMOSI;

        Bits32 portCS;
        Bits32 pinCS;

        Bits32 portTX;
        Bits32 pinTX;
    };

    /*!
     *  @_nodoc
     *  ======== SDSPITiva_object ========
     *  Structure must match the SDSPITiva_object structure defined in sdspi/SDSPITivaDMA.h
     */
    struct SDSPITiva_Object {
        Bits32               driveNumber;
        UChar                diskState;
        SDSPITiva_CardType   cardType;
        Bits32               bitRate;
    };

    /*!
     *  @_nodoc
     *  ======== SDSPIUSCI_hwAttrs ========
     *  Structure must match the SDSPITivaDMA_hwAttrs structure defined in sdspi/SDSPITivaDMA.h
     */
    struct SDSPIUSCIA_HWAttrs {
        Bits32 baseAddr;

        Bits8  clockSource;

        Bits8  portSPI;
        Bits32 pinSCK;
        Bits32 pinMISO;
        Bits32 pinMOSI;

        Bits8  portCS;
        Bits32 pinCS;
    };

    struct SDSPIUSCIB_HWAttrs {
        Bits32 baseAddr;

        Bits8  clockSource;

        Bits8  portSPI;
        Bits32 pinSCK;
        Bits32 pinMISO;
        Bits32 pinMOSI;

        Bits8  portCS;
        Bits32 pinCS;
    };

    /*!
     *  @_nodoc
     *  ======== SDSPIUSCIA_object ========
     *  Structure must match the SDSPITiva_object structure defined in sdspi/SDSPITivaDMA.h
     */
    struct SDSPIUSCIA_Object {
        Bits16               driveNumber;
        UChar                diskState;
        SDSPITiva_CardType   cardType; // Same as SDSPITiva_CardType
        Bits32               bitRate;
    };

    /*!
     *  @_nodoc
     *  ======== SDSPIUSCIB_object ========
     *  Structure must match the SDSPITiva_object structure defined in sdspi/SDSPITivaDMA.h
     */
    struct SDSPIUSCIB_Object {
        Bits16               driveNumber;
        UChar                diskState;
        SDSPITiva_CardType   cardType; // Same as SDSPITiva_CardType
        Bits32               bitRate;
    };

    /*!
     *  @_nodoc
     *  ======== SDSPIEUSCI_hwAttrs ========
     *  Structure must match the SDSPITivaDMA_hwAttrs structure defined in sdspi/SDSPITivaDMA.h
     */
    struct SDSPIEUSCIA_HWAttrs {
        Bits32 baseAddr;

        Bits8  clockSource;

        Bits8  portSCK;
        Bits32 pinSCK;
        Bits8  portMISO;
        Bits32 pinMISO;
        Bits8  portMOSI;
        Bits32 pinMOSI;

        Bits8  portCS;
        Bits32 pinCS;
    };

    struct SDSPIEUSCIB_HWAttrs {
        Bits32 baseAddr;

        Bits8  clockSource;

        Bits8  portSCK;
        Bits32 pinSCK;
        Bits8  portMISO;
        Bits32 pinMISO;
        Bits8  portMOSI;
        Bits32 pinMOSI;

        Bits8  portCS;
        Bits32 pinCS;
    };

    /*!
     *  @_nodoc
     *  ======== SDSPIEUSCIA_object ========
     *  Structure must match the SDSPITiva_object structure defined in sdspi/SDSPITivaDMA.h
     */
    struct SDSPIEUSCIA_Object {
        Bits16               driveNumber;
        UChar                diskState;
        SDSPITiva_CardType   cardType; // Same as SDSPITiva_CardType
        Bits32               bitRate;
    };

    /*!
     *  @_nodoc
     *  ======== SDSPIEUSCIB_object ========
     *  Structure must match the SDSPITiva_object structure defined in sdspi/SDSPITivaDMA.h
     */
    struct SDSPIEUSCIB_Object {
        Bits16               driveNumber;
        UChar                diskState;
        SDSPITiva_CardType   cardType; // Same as SDSPITiva_CardType
        Bits32               bitRate;
    };

    /*!
     *  @_nodoc
     *  ======== SDSPICC3200_HWAttrs ========
     *  Structure must match the SDSPITivaDMA_hwAttrs structure defined in sdspi/SDSPITivaDMA.h
     */
    struct SDSPICC3200_HWAttrs {
        Bits32 baseAddr;
        Bits32 spiPRCM;
        Bits32 csGPIOBase;
        Bits32 csGPIOPin;
        Bits32 txGPIOBase;
        Bits32 txGPIOPin;
        Bits32 txGPIOMode;
        Bits32 txSPIMode;
        Bits32 txPackPin;
    };

    /*!
     *  @_nodoc
     *  ======== SDSPITiva_object ========
     *  Structure must match the SDSPITiva_object structure defined in sdspi/SDSPITivaDMA.h
     */
    struct SDSPICC3200_Object {
        Bits32               driveNumber;
        UChar                diskState;
        SDSPITiva_CardType   cardType;
        Bits32               bitRate;
    };

    /*!
     *  @_nodoc
     *  ======== SDSPIMSP432_HWAttrs ========
     *  Structure must match the SDSPIMSP432_HWAttrs structure defined in sdspi/SDSPIMSP432DMA.h
     */
    struct SDSPIMSP432_HWAttrs {
        Bits32 baseAddr;
        Bits8  clockSource;

        Bits8  portSCK;
        Bits16 pinSCK;
        Bits8  sckMode;

        Bits8  portMISO;
        Bits32 pinMISO;
        Bits8  misoMode;

        Bits8  portMOSI;
        Bits16 pinMOSI;
        Bits8  mosiMode;

        Bits8  portCS;
        Bits32 pinCS;
    };

    /*!
     *  @_nodoc
     *  ======== SDSPIMSP432_Object ========
     *  Structure must match the SDSPIMSP432_Object structure defined in sdspi/SDSPIMSP432DMA.h
     */
    struct SDSPIMSP432_Object {
        Bits16               driveNumber;
        UChar                diskState;
        SDSPITiva_CardType   cardType;
        Bits32               bitRate;
        Bits32               filesystem;
    };

    /*!
     *  @_nodoc
     *  ======== BasicView ========
     */
    metaonly struct BasicView {
        String              sdspiHandle;
        String              baseAddr;
        String              functionTable;
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
                ],
            ]
        });
}
