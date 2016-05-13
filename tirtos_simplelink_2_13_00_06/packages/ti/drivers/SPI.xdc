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
 *  ======== SPI.xdc ========
 */
package ti.drivers;
import xdc.rov.ViewInfo;

/*!
 *  ======== SPI ========
 */
@NoRuntime
@HeaderName("")
module SPI {

    /*!
     *  ======== LibType ========
     *  SPI library selection options
     *
     *  This enumeration defines all the SPI library type
     *  provided by the product.  You can select the library type by setting
     *  the {@link #libType SPI.libType} configuration parameter.
     *
     *  @field(LibType_Instrumented) The library supplied is prebuilt with
     *  logging and assertions enabled.
     *  Diags_USER1 mask logs basic information
     *  Diags_USER2 mask logs more detailed information
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
     *  SPI Library type
     *
     *  Deprecated: Please use ti.tirtos.TIRTOS.libType instead.
     *
     *  The SPI runtime is provided in the form of a library that is
     *  linked with your application.  Several forms of this library are
     *  provided with the SPI product. This configuration parameter
     *  allows you to select the form of SPI to use.
     *
     *  The default value of libType is
     *  {@link #LibType_Instrumented SPI_LibType_Instrumented}.  For a
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
     *  ======== SPI_Config ========
     *  Structure must match the SPI_Config structure defined in SPI.h
     */
    struct SPI_Config {
        Void               *fxnTablePtr; //Used to determine the implementation
        Void               *object;      //Used to access the data
        Void               *hwAttrs;     //Used to get the attributes
    };

    /*!
     *  @_nodoc
     *  ======== SPI callback ========
     *  SPI Callback function
     */
    typedef Void (*CallbackFxn) (SPI_Config *, SPI_Transaction *);

    /*!
     *  @_nodoc
     *  ======== SPI_TransferMode ========
     *  Structure must match the SPI_TransferMode structure defined in SPI.h
     */
    enum TransferMode  {
        SPI_MODE_BLOCKING,
        SPI_MODE_CALLBACK
    };

    /*!
     *  @_nodoc
     *  ======== SPI_Transaction ========
     *  Structure must match the SPI_Transaction structure defined in SPI.h
     */
    struct SPI_Transaction {
        UInt                count;
        Ptr                 txBuf;
        Ptr                 rxBuf;
        UArg                arg;
    };

    /*!
     *  @_nodoc
     *  To avoid creating other modules (and adding a xdc.useModule) with driver
     *  specific implementations, the object structs are all listed in here.
     */
    /*! ======== SPITivaDMA ======== */
    enum SPITivaDMA_FrameSize {
        SPITivaDMA_8bit  = 0,
        SPITivaDMA_16bit = 1
    };

    /*!
     *  @_nodoc
     *  ======== SPITivaDMA_hwAttrs ========
     *  Structure must match the SPITivaDMA_hwAttrs structure defined in spi/SPITivaDMA.h
     */
    struct SPITivaDMA_HWAttrs {
        Bits32  baseAddr;
        Bits32  intNum;

        Bits32  rxChannelIndex;
        Bits32  txChannelIndex;

        Void  (*channelMappingFxn)(ULong);
        Bits32  rxChannelMappingFxnArg;
        Bits32  txChannelMappingFxnArg;
    };

    /*!
     *  @_nodoc
     *  ======== SPITivaDMA_Object ========
     *  Structure must match the SPITivaDMA_Object structure defined in
     *  spi/SPITivaDMA.h
     */
    struct SPITivaDMA_Object {
        Ptr                    transferComplete;
        Ptr                    hwi;
        TransferMode           transferMode;
        CallbackFxn            transferCallbackFxn;
        SPI_Transaction       *transaction;
        SPITivaDMA_FrameSize   frameSize;
        Bool                   isOpen;
    };

    /*!
     *  @_nodoc
     *  To avoid creating other modules (and adding a xdc.useModule) with driver
     *  specific implementations, the object structs are all listed in here.
     */
    enum SPICC3200DMA_FrameSize {
        SPICC3200DMA_8bit = 0,
        SPICC3200DMA_16bit = 1,
        SPICC3200DMA_32bit = 2
    };

    /*!
     *  @_nodoc
     *  ======== SPICC3200DMA_hwAttrs ========
     *  Structure must match the SPICC3200DMA_hwAttrs structure defined in spi/SPICC3200DMA.h
     */
    struct SPICC3200DMA_HWAttrs {
        Bits32   baseAddr;
        Bits32   intNum;
        Bits32   spiPRCM;
        Bits32   csControl;
        Bits32   csPolarity;
        Bits32   pinMode;
        Bits32   turboMode;
        Ptr      scratchBufPtr;
        Bits32   defaultTxBufValue;
        Bits32   rxChannelIndex;
        Bits32   txChannelIndex;
    };

    /*!
     *  @_nodoc
     *  ======== SPICC3200DMA_Object ========
     *  Structure must match the SPICC3200DMA_Object structure defined in
     *  spi/SPICC3200DMA.h
     */
    struct SPICC3200DMA_Object {
        Ptr                     transferComplete;
        Ptr                     hwi;
        TransferMode            transferMode;
        CallbackFxn             transferCallbackFxn;
        SPI_Transaction        *transaction;
        SPICC3200DMA_FrameSize  frameSize;
        ULong                   spiMode;
        ULong                   rxFifoTrigger;
        ULong                   txFifoTrigger;
        bool                    isOpen;
    };

    /*!
     *  @_nodoc
     *  ======== SPIUSCIADMA_HWAttrs ========
     *  Structure must match the SPIUSCIADMA_HWAttrs structure defined in
     *  spi/SPIUSCIADMA.h
     */
    struct SPIUSCIADMA_HWAttrs {
        Bits32  baseAddr;
        Bits8   clockSource;
        Bits8   bitOrder;

        Bits32  dmaBaseAddr;
        Bits8   rxDMAChannelIndex;
        Bits8   rxDMASourceTrigger;
        Bits8   txDMAChannelIndex;
        Bits8   txDMASourceTrigger;
    };

    /*!
     *  @_nodoc
     *  ======== SPIUSCIADMA_Object ========
     *  Structure must match the SPIUSCIADMA_Object structure defined in
     *  spi/SPIUSCIADMA.h
     */
    struct SPIUSCIADMA_Object {
        Bool                isOpen;

        Ptr                 transferComplete;

        TransferMode        transferMode;
        CallbackFxn         transferCallbackFxn;

        SPI_Transaction    *transaction;
    };

    /*!
     *  @_nodoc
     *  ======== SPIUSCIBDMA_HWAttrs ========
     *  Structure must match the SPIUSCIBDMA_HWAttrs structure defined in
     *  spi/SPIUSCIBDMA.h
     */
    struct SPIUSCIBDMA_HWAttrs {
        Bits32  baseAddr;
        Bits8   clockSource;
        Bits8   bitOrder;

        Bits32  dmaBaseAddr;
        Bits8   rxDMAChannelIndex;
        Bits8   rxDMASourceTrigger;
        Bits8   txDMAChannelIndex;
        Bits8   txDMASourceTrigger;
    };

    /*!
     *  @_nodoc
     *  ======== SPIUSCIBDMA_Object ========
     *  Structure must match the SPIUSCIBDMA_Object structure defined in
     *  spi/SPIUSCIBDMA.h
     */
    struct SPIUSCIBDMA_Object {
        Bool                isOpen;

        Ptr                 transferComplete;

        TransferMode        transferMode;
        CallbackFxn         transferCallbackFxn;

        SPI_Transaction    *transaction;
    };

     /*!
     *  @_nodoc
     *  ======== SPIEUSCIADMA_HWAttrs ========
     *  Structure must match the SPIEUSCIADMA_HWAttrs structure defined in
     *  spi/SPIEUSCIADMA.h
     */
    struct SPIEUSCIADMA_HWAttrs {
        Bits32  baseAddr;
        Bits8   clockSource;
        Bits16  BitOrder;

        Bits32  dmaBaseAddr;
        Bits8   rxDMAChannelIndex;
        Bits8   rxDMASourceTrigger;
        Bits8   txDMAChannelIndex;
        Bits8   txDMASourceTrigger;
    };

    /*!
     *  @_nodoc
     *  ======== SPIEUSCIADMA_Object ========
     *  Structure must match the SPIEUSCIADMA_Object structure defined in
     *  spi/SPIEUSCIADMA.h
     */
    struct SPIEUSCIADMA_Object {
        Bool                isOpen;

        Ptr                 transferComplete;

        TransferMode        transferMode;
        CallbackFxn         transferCallbackFxn;

        SPI_Transaction    *transaction;
    };

     /*!
     *  @_nodoc
     *  ======== SPIEUSCIBDMA_HWAttrs ========
     *  Structure must match the SPIEUSCIBDMA_HWAttrs structure defined in
     *  spi/SPIEUSCIBDMA.h
     */
    struct SPIEUSCIBDMA_HWAttrs {
        Bits32  baseAddr;
        Bits8   clockSource;
        Bits16  BitOrder;

        Bits32  dmaBaseAddr;
        Bits8   rxDMAChannelIndex;
        Bits8   rxDMASourceTrigger;
        Bits8   txDMAChannelIndex;
        Bits8   txDMASourceTrigger;
    };

    /*!
     *  @_nodoc
     *  ======== SPIEUSCIBDMA_Object ========
     *  Structure must match the SPIEUSCIBDMA_Object structure defined in
     *  spi/SPIEUSCIBDMA.h
     */
    struct SPIEUSCIBDMA_Object {
        Bool                isOpen;

        Ptr                 transferComplete;

        TransferMode        transferMode;
        CallbackFxn         transferCallbackFxn;

        SPI_Transaction    *transaction;
    };

    /*!
     *  @_nodoc
     *  ======== SPIMSP432DMA_HWAttrs ========
     *  Structure must match the SPIMSP432DMA_HWAttrs structure defined in spi/SPIMSP432DMA.h
     */
    struct SPIMSP432DMA_HWAttrs {
        Bits32   baseAddr;
        Bits16   bitOrder;
        Bits8    clockSource;
        Ptr      scratchBufPtr;
        Bits8    defaultTxBufValue;
        Bits8    dmaIntNum;
        Bits32   rxDMAChannelIndex;
        Bits32   txDMAChannelIndex;
    };

    /*!
     *  @_nodoc
     *  ======== SPIMSP432DMA_Object ========
     *  Structure must match the SPIMSP432DMA_Object structure defined in
     *  spi/SPIMSP432DMA.h
     */
    struct SPIMSP432DMA_Object {
        Ptr                     transferComplete;
        TransferMode            transferMode;
        CallbackFxn             transferCallbackFxn;
        SPI_Transaction        *transaction;
        bool                    isOpen;
    };

    /*!
     *  @_nodoc
     *  ======== BasicView ========
     */
    metaonly struct BasicView {
        String              spiHandle;
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
