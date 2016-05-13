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

/*-----------------------------------------------------------------------*/
/* MMC/SDC (in SPI mode) control module  (C)ChaN, 2007                   */
/*-----------------------------------------------------------------------*/

#include <stdint.h>
#include <stdbool.h>

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
#include <ti/drivers/ports/ClockP.h>

#include <ti/drivers/sdspi/SDSPICC3200.h>

/* driverlib header files */
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>
#include <driverlib/gpio.h>
#include <driverlib/pin.h>
#include <driverlib/prcm.h>
#include <driverlib/spi.h>

/* Definitions for MMC/SDC command */
#define CMD0                        (0x40+0)    /* GO_IDLE_STATE */
#define CMD1                        (0x40+1)    /* SEND_OP_COND */
#define CMD8                        (0x40+8)    /* SEND_IF_COND */
#define CMD9                        (0x40+9)    /* SEND_CSD */
#define CMD10                       (0x40+10)   /* SEND_CID */
#define CMD12                       (0x40+12)   /* STOP_TRANSMISSION */
#define CMD16                       (0x40+16)   /* SET_BLOCKLEN */
#define CMD17                       (0x40+17)   /* READ_SINGLE_BLOCK */
#define CMD18                       (0x40+18)   /* READ_MULTIPLE_BLOCK */
#define CMD23                       (0x40+23)   /* SET_BLOCK_COUNT */
#define CMD24                       (0x40+24)   /* WRITE_BLOCK */
#define CMD25                       (0x40+25)   /* WRITE_MULTIPLE_BLOCK */
#define CMD41                       (0x40+41)   /* SEND_OP_COND (ACMD) */
#define CMD55                       (0x40+55)   /* APP_CMD */
#define CMD58                       (0x40+58)   /* READ_OCR */

#define SD_SECTOR_SIZE              512

#define START_BLOCK_TOKEN           0xFE
#define START_MULTIBLOCK_TOKEN      0xFC
#define STOP_MULTIBLOCK_TOKEN       0xFD

#define DRIVE_NOT_MOUNTED           ~0

/*
 * Array of SDSPI_Handles to determine the association of the FatFs drive number
 * with a SDSPI_Handle
 * _VOLUMES is defined in <ti/sysbios/fatfs/ffconf.h>
 */
static SDSPI_Handle sdspiHandles[_VOLUMES];

/* uS scaling to function timeouts */
static uint32_t       uSClockPeriod = 0;

/* Function prototypes */
static uint32_t       rcvr_datablock(SDSPICC3200_HWAttrs const *hwAttrs,
                                     uint8_t *buf, uint32_t btr);
static inline void    releaseSPIBus(SDSPICC3200_HWAttrs const *hwAttrs);
static inline uint8_t rxSPI(SDSPICC3200_HWAttrs const *hwAttrs);
static uint8_t        send_cmd(SDSPICC3200_HWAttrs const *hwAttrs, uint8_t cmd,
                               uint32_t arg);
static void           send_initial_clock_train(SDSPICC3200_HWAttrs const *hwAttrs);
static inline void    takeSPIBus(SDSPICC3200_HWAttrs const *hwAttrs);
static inline void    txSPI(SDSPICC3200_HWAttrs const *hwAttrs, uint8_t dat);
static uint8_t        wait_ready(SDSPICC3200_HWAttrs const *hwAttrs);
static bool           xmit_datablock(SDSPICC3200_HWAttrs const *hwAttrs,
                                     const uint8_t *buf, uint8_t token);

/* FatFs disk I/O functions */
DSTATUS SDSPICC3200_diskInitialize(BYTE drv);
DRESULT SDSPICC3200_diskIOctrl(BYTE drv, BYTE ctrl, void *buf);
DRESULT SDSPICC3200_diskRead(BYTE drv, BYTE *buf,
                             DWORD sector, BYTE count);
DSTATUS SDSPICC3200_diskStatus(BYTE drv);
DRESULT SDSPICC3200_diskWrite(BYTE drv, const BYTE *buf,
                              DWORD sector, BYTE count);

/* SDSPICC3200 functions */
void         SDSPICC3200_close(SDSPI_Handle handle);
int          SDSPICC3200_control(SDSPI_Handle handle, unsigned int cmd, void *arg);
void         SDSPICC3200_init(SDSPI_Handle handle);
SDSPI_Handle SDSPICC3200_open(SDSPI_Handle handle, unsigned char drv,
                              SDSPI_Params *params);

/* SDSPI function table for SDSPICC3200 implementation */
const SDSPI_FxnTable SDSPICC3200_fxnTable = {
    SDSPICC3200_init,
    SDSPICC3200_open,
    SDSPICC3200_close,
    SDSPICC3200_control
};

/* Default SDSPI params */
extern const SDSPI_Params SDSPI_defaultParams;

/*
 *  ======== rcvr_datablock ========
 *  Function to receive a block of data from the SDCard
 *
 *  btr count must be an even number
 */
static uint32_t rcvr_datablock(SDSPICC3200_HWAttrs const *hwAttrs,
                               uint8_t *buf, uint32_t btr)
{
    uint8_t   token;
    uint32_t  clockTimeout;
    uint32_t  clockStart;
    uint32_t  clockCurrent;

    /* Wait for data packet in timeout of 100 ms */
    clockStart = ClockP_getSystemTicks();
    clockTimeout = clockStart + (100 * 1000/uSClockPeriod);
    if (clockTimeout > clockStart) {
        clockStart = ~0;
    }
    do {
        token = rxSPI(hwAttrs);
        clockCurrent = ClockP_getSystemTicks();
    } while ((token == 0xFF) && ((clockCurrent <= clockTimeout) ||
                                 (clockCurrent >= clockStart)));

    if (token != START_BLOCK_TOKEN) {
        /* If not valid data token, return error */
        return (0);
    }

    /* Receive the data block into buffer */
    do {
        *(buf++) = rxSPI(hwAttrs);
    } while (--btr);

    /* Read the CRC, but discard it */
    rxSPI(hwAttrs);
    rxSPI(hwAttrs);

    /* Return with success */
    return (1);
}

/*
 *  ======== releaseSPIBus ========
 *  Function to release the SPI bus
 *
 *  @param  hwAttrs     Pointer to hardware attributes
 */
static inline void releaseSPIBus(SDSPICC3200_HWAttrs const *hwAttrs)
{
    /* Deselect the SD card. */
    MAP_GPIOPinWrite(hwAttrs->csGPIOBase, hwAttrs->csGPIOPin,
                     hwAttrs->csGPIOPin);
}

/*
 *  ======== rxSPI ========
 *  Function to receive one byte onto the SPI bus. Polling (Blocked)
 *
 *  @param  hwAttrs     Pointer to hardware attributes
 */
static inline uint8_t rxSPI(SDSPICC3200_HWAttrs const *hwAttrs)
{
    SDSPIDataType   rcvdat;

    /* write dummy data */
    MAP_SPIDataPut(hwAttrs->baseAddr, 0xFF);

    /* read data frm rx fifo */
    MAP_SPIDataGet(hwAttrs->baseAddr, &rcvdat);

    return ((uint8_t)rcvdat);
}

/*
 *  ======== send_cmd ========
 *  Function that will transmit an command to the SDCard
 *
 *  @param  hwAttrs     Pointer to hardware attributes
 *
 *  @param  cmd         SD command
 *
 *  @param  arg         SD command argument
 */
static uint8_t send_cmd(SDSPICC3200_HWAttrs const *hwAttrs, uint8_t cmd, uint32_t arg)
{
    uint8_t n;
    uint8_t res;

    if (wait_ready(hwAttrs) != 0xFF) {
        DebugP_log1("SDSPI:(%p) send_cmd: SD card wait time expired",
                    hwAttrs->baseAddr);
        return (0xFF);
    }

    /* Send command packet */
    txSPI(hwAttrs, cmd);                    /* Command */
    txSPI(hwAttrs, (uint8_t)(arg >> 24));   /* Argument[31..24] */
    txSPI(hwAttrs, (uint8_t)(arg >> 16));   /* Argument[23..16] */
    txSPI(hwAttrs, (uint8_t)(arg >> 8));    /* Argument[15..8] */
    txSPI(hwAttrs, (uint8_t)arg);           /* Argument[7..0] */

    if (cmd == CMD0) {
        /* CRC for CMD0(0) */
        n = 0x95;
    }
    else if (cmd == CMD8) {
        /* CRC for CMD8(0x1AA) */
        n = 0x87;
    }
    else {
        /* Default CRC should be at least 0x01 */
        n = 0x01;
    }

    /* Future enhancement to add CRC support */
    txSPI(hwAttrs, n);

    /* Receive command response */
    if (cmd == CMD12) {
        /* Skip a stuff byte when stop reading */
        rxSPI(hwAttrs);
    }

    /* Wait for a valid response in timeout; 10 attempts */
    n = 10;
    do {
        res = rxSPI(hwAttrs);
    } while ((res & 0x80) && --n);

    /* Return with the response value */
    return (res);
}

/*
 *  ======== send_initial_clock_train ========
 *  Function to get the SDCard into SPI mode
 *
 *  @param  hwAttrs     Pointer to hardware attributes
 */
static void send_initial_clock_train(SDSPICC3200_HWAttrs const *hwAttrs)
{
    unsigned char   i;
    SDSPIDataType   dat;

    /* Deselect the SD card. */
    MAP_GPIOPinWrite(hwAttrs->csGPIOBase, hwAttrs->csGPIOPin,
                     hwAttrs->csGPIOPin);

    /* Switch the SPI TX line to a GPIO and drive it high too. */
    MAP_PinTypeGPIO(hwAttrs->txPackPin, hwAttrs->txGPIOMode, false);
    MAP_GPIODirModeSet(hwAttrs->txGPIOBase, hwAttrs->txGPIOPin,
                       GPIO_DIR_MODE_OUT);
    MAP_GPIOPinWrite(hwAttrs->txGPIOBase, hwAttrs->txGPIOPin,
                     hwAttrs->txGPIOPin);

    /*
     * Send 10 bytes over the SPI bus. This causes the clock to toggle several
     * times to get the SD Card into SPI mode.
     */
    for (i = 0; i < 10; i++) {
        /*
         * Write DUMMY data. SPIDataPut() waits until there is room in the
         * FIFO.
         */
        MAP_SPIDataPut(hwAttrs->baseAddr, 0xFF);

        /* Flush data read during data write. */
        MAP_SPIDataGet(hwAttrs->baseAddr, &dat);
        //rxSPI(hwAttrs);
    }

    /* Revert to hardware control of the SPI TX line. */
    MAP_PinModeSet(hwAttrs->txPackPin, hwAttrs->txSPIMode);

    DebugP_log1("SDSPI:(%p) initialized SD card to SPI mode",
                hwAttrs->baseAddr);
}

/*
 *  ======== takeSPIBus ========
 *  Function to take the SPI bus
 *
 *  @param  hwAttrs     Pointer to hardware attributes
 */
static inline void takeSPIBus(SDSPICC3200_HWAttrs const *hwAttrs)
{
    /* Select the SD card. */
    MAP_GPIOPinWrite(hwAttrs->csGPIOBase, hwAttrs->csGPIOPin, 0);
}

/*
 *  ======== txSPI ========
 *  Function to transmit one byte onto the SPI bus. Polling (Blocked)
 *
 *  @param  hwAttrs     Pointer to hardware attributes
 *
 *  @param  dat         Data to be sent onto the SPI bus
 */
static inline void txSPI(SDSPICC3200_HWAttrs const *hwAttrs, uint8_t dat)
{
    SDSPIDataType   rcvdat;

    /* Write the data to the tx fifo */
    MAP_SPIDataPut(hwAttrs->baseAddr, dat);

    /* flush data read during the write */
    MAP_SPIDataGet(hwAttrs->baseAddr, &rcvdat);
}

/*
 *  ======== wait_ready ========
 *  Function to check if the SDCard is busy
 *
 *  This function queries the SDCard to see if it is in a busy state or ready
 *  state
 *
 *  @param  hwAttrs     Pointer to hardware attributes
 */
static uint8_t wait_ready(SDSPICC3200_HWAttrs const *hwAttrs)
{
    uint8_t   res;
    uint32_t  clockTimeout;
    uint32_t  clockStart;
    uint32_t  clockCurrent;

    /* Wait for data packet in timeout of 500 ms */
    clockStart = ClockP_getSystemTicks();
    clockTimeout = clockStart + (500 * 1000/uSClockPeriod);
    if (clockTimeout > clockStart) {
        clockStart = ~0;
    }
    rxSPI(hwAttrs);
    do {
        res = rxSPI(hwAttrs);
        clockCurrent = ClockP_getSystemTicks();
    } while ((res != 0xFF) && ((clockCurrent <= clockTimeout) ||
                               (clockCurrent >= clockStart)));

    return (res);
}

/* _READONLY is defined in <ti/sysbios/fatfs/diskio.h> */
#if _READONLY == 0
/*
 *  ======== xmit_datablock ========
 *  Function to transmit a block of data to the SDCard
 *
 *  @param  hwAttrs     Pointer to hardware attributes
 *
 *  @param  params      SDSPICC3200 hardware attributes
 *
 *  @param  buf         pointer to const data buffer
 *
 *  @param  token       command token to be sent to the SD card prior to
 *                      sending the data block. The available tokens are:
 *                      START_BLOCK_TOKEN
 *                      START_MULTIBLOCK_TOKEN
 *                      STOP_MULTIBLOCK_TOKEN
 */
static bool xmit_datablock(SDSPICC3200_HWAttrs const *hwAttrs,
                           const uint8_t *buf, uint8_t token)
{
    uint8_t resp;
    uint8_t wc;

    if (wait_ready(hwAttrs) != 0xFF) {
        /* Return with error */
        return (false);
    }

    /* Xmit data token */
    txSPI(hwAttrs, token);

    /* Send data only when token != STOP_MULTIBLOCK_TOKEN */
    if (token != STOP_MULTIBLOCK_TOKEN) {
        /* Is data token */
        wc = 0;
        /* Transferring 512 byte blocks using a 8 bit counter */
        do {
            /* Xmit the SD_SECTOR_SIZE byte data block */
            txSPI(hwAttrs, *buf++);
            txSPI(hwAttrs, *buf++);
        } while (--wc);

        /* Future enhancement to add CRC support */
        txSPI(hwAttrs, 0xFF);
        txSPI(hwAttrs, 0xFF);

        /* Reveive data response */
        resp = rxSPI(hwAttrs);

        /* If not accepted, return error */
        if ((resp & 0x1F) != 0x05) {
            return (false);
        }
    }

    /* Return with success */
    return (true);
}
#endif /* _READONLY */

/*
 *  ======== SDSPICC3200_close ========
 *  Function to unmount the FatFs filesystem and unregister the SDSPICC3200
 *  disk I/O functions from SYS/BIOS' FatFS module.
 *
 *  @param  handle      SDSPI_Handle returned by SDSPI_open()
 */
void SDSPICC3200_close(SDSPI_Handle handle)
{
    uintptr_t                    key;
    DRESULT                      dresult;
    FRESULT                      fresult;
    SDSPICC3200_Object          *object = handle->object;
    SDSPICC3200_HWAttrs const   *hwAttrs = handle->hwAttrs;

    /* Unmount the FatFs drive */
    fresult = f_mount(object->driveNumber, NULL);
    if (fresult != FR_OK) {
        DebugP_log2("SDSPI:(%p) Could not unmount FatFs volume @ drive number %d",
                    hwAttrs->baseAddr, object->driveNumber);
    }

    /* Unregister the disk_*() functions */
    dresult = disk_unregister(object->driveNumber);
    if (dresult != RES_OK) {
        DebugP_log2("SDSPI:(%p) Error unregistering disk functions @ drive number %d",
                    hwAttrs->baseAddr, object->driveNumber);
    }

    MAP_SPIDisable(hwAttrs->baseAddr);

    DebugP_log1("SDSPI:(%p) closed", hwAttrs->baseAddr);

    key = HwiP_disable();
    object->driveNumber = DRIVE_NOT_MOUNTED;
    HwiP_restore(key);
}

/*
 *  ======== SDSPICC3200_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int SDSPICC3200_control(SDSPI_Handle handle, unsigned int cmd, void *arg)
{
    /* No implementation yet */
    return (SDSPI_STATUS_UNDEFINEDCMD);
}

/*
 *  ======== SDSPICC3200_diskInitialize ========
 *  Function to initialize the SD Card.  This function is called by the FatFs
 *  module and must not be called by the application!
 *
 *  @param  drv         Drive Number
 */
DSTATUS SDSPICC3200_diskInitialize(BYTE drv)
{
    uint8_t                    n;
    uint8_t                    ocr[4];
    SDSPICC3200_CardType       cardType;
    ClockP_FreqHz              freq;
    uint32_t                   clockTimeout;
    uint32_t                   clockStart;
    uint32_t                   clockCurrent;
    SDSPICC3200_Object        *object = sdspiHandles[drv]->object;
    SDSPICC3200_HWAttrs const *hwAttrs = sdspiHandles[drv]->hwAttrs;

    /* No card in the socket */
    if (object->diskState & STA_NODISK) {
        DebugP_log1("SDSPI:(%p) disk initialization failed: No disk",
                    hwAttrs->baseAddr);

        return (object->diskState);
    }

    /* Initialize the SD Card for SPI mode */
    send_initial_clock_train(hwAttrs);

    /* Select the SD Card's chip select */
    takeSPIBus(hwAttrs);
    cardType = NOCARD;

    /* Send the CMD0 to put the SD Card in "Idle" state */
    if (send_cmd(hwAttrs, CMD0, 0) == 1) {
        /*
         * Determine what SD Card version we are dealing with
         * Depending on which SD Card version, we need to send different SD
         * commands to the SD Card, which will have different response fields.
         */
        if (send_cmd(hwAttrs, CMD8, 0x1AA) == 1) {
            /* SDC Ver2+ */
            for (n = 0; n < 4; n++) {
                ocr[n] = rxSPI(hwAttrs);
            }

            /*
             * Ensure that the card's voltage range is valid
             * The card can work at vdd range of 2.7-3.6V
             */
            if ((ocr[2] == 0x01) && (ocr[3] == 0xAA)) {
                /* Wait for data packet in timeout of 1s */
                clockStart = ClockP_getSystemTicks();
                clockTimeout = clockStart + (1000 * 1000/uSClockPeriod);
                if (clockTimeout > clockStart) {
                    clockStart = ~0;
                }
                do {
                    /* ACMD41 with HCS bit */
                    if (send_cmd(hwAttrs, CMD55, 0) <= 1 &&
                        send_cmd(hwAttrs, CMD41, 1UL << 30) == 0) {
                        clockTimeout = 0;
                        break;
                    }
                    clockCurrent = ClockP_getSystemTicks();
                } while ((clockCurrent <= clockTimeout) ||
                         (clockCurrent >= clockStart));

                /*
                 * Check CCS bit to determine which type of capacity we are
                 * dealing with
                 */
                if ((!clockTimeout) && send_cmd(hwAttrs, CMD58, 0) == 0) {
                    for (n = 0; n < 4; n++) {
                        ocr[n] = rxSPI(hwAttrs);
                    }
                    cardType = (ocr[0] & 0x40) ? SDHC : SDSC;
                }
            }
        }

        /* SDC Ver1 or MMC */
        else {
            /*
             * The card version is not SDC V2+ so check if we are dealing with a
             * SDC or MMC card
             */
            if ((send_cmd(hwAttrs, CMD55, 0) <= 1 &&
                 send_cmd(hwAttrs, CMD41, 0) <= 1)) {
                cardType = SDSC;
            }
            else {
                cardType = MMC;
            }

            /* Wait for data packet in timeout of 1s */
            clockStart = ClockP_getSystemTicks();
            clockTimeout = clockStart + (1000 * 1000/uSClockPeriod);
            if (clockTimeout > clockStart) {
                clockStart = ~0;
            }
            do {
                if (cardType == SDSC) {
                    /* ACMD41 */
                    if (send_cmd(hwAttrs, CMD55, 0) <= 1 &&
                        send_cmd(hwAttrs, CMD41, 0) == 0) {
                        clockTimeout = 0;
                        break;
                    }
                }
                else {
                    /* CMD1 */
                    if (send_cmd(hwAttrs, CMD1, 0) == 0) {
                        clockTimeout = 0;
                        break;
                    }
                }
                clockCurrent = ClockP_getSystemTicks();
            } while ((clockCurrent <= clockTimeout) ||
                     (clockCurrent >= clockStart));

            /* Select R/W block length */
            if ((clockTimeout) || send_cmd(hwAttrs, CMD16, SD_SECTOR_SIZE) != 0) {
                cardType = NOCARD;
            }
        }
    }

    object->cardType = cardType;

    /* Deselect the SD Card's chip select */
    releaseSPIBus(hwAttrs);

    /* Idle (Release DO) */
    rxSPI(hwAttrs);

    /* Check to see if a card type was determined */
    if (cardType != NOCARD) {
        /* Reconfigure the SPI bust at the new frequency rate */
        ClockP_getCpuFreq(&freq);

        MAP_SPIDisable(hwAttrs->baseAddr);
        MAP_SPIConfigSetExpClk(hwAttrs->baseAddr,
                               MAP_PRCMPeripheralClockGet(hwAttrs->spiPRCM),
                               object->bitRate,
                               SPI_MODE_MASTER,
                               SPI_SUB_MODE_0,
                               (SPI_HW_CTRL_CS | SPI_3PIN_MODE |
                                SPI_TURBO_OFF | SPI_CS_ACTIVELOW |
                                SPI_WL_8));
        MAP_SPIEnable(hwAttrs->baseAddr);

        DebugP_log3("SDSPI:(%p) CPU freq: %d; Reconfiguring SDSPI freq to %d",
                    hwAttrs->baseAddr, freq.lo, object->bitRate);

        /* Initialization succeeded */
        object->diskState &= ~STA_NOINIT;
    }
    else {
        DebugP_log1("SDSPI:(%p) disk initialization failed",
                    hwAttrs->baseAddr);
    }

    return (object->diskState);
}

/*
 *  ======== SDSPICC3200_diskIOctrl ========
 *  Function to perform specified disk operations. This function is called by the
 *  FatFs module and must not be called by the application!
 *
 *  @param  drv         Drive Number
 *
 *  @param  ctrl        Control code
 *
 *  @param  buf         Buffer to send/receive control data
 */
DRESULT SDSPICC3200_diskIOctrl(BYTE drv, BYTE ctrl, void *buf)
{
    DRESULT                    res = RES_ERROR;
    uint8_t                    n;
    uint8_t                    csd[16];
    WORD                       csize;
    SDSPICC3200_Object        *object = sdspiHandles[drv]->object;
    SDSPICC3200_HWAttrs const *hwAttrs = sdspiHandles[drv]->hwAttrs;

    if (object->diskState & STA_NOINIT) {
        DebugP_log1("SDSPI:(%p) disk IO control: disk not initialized",
                    hwAttrs->baseAddr);

        return (RES_NOTRDY);
    }

    /* Select the SD Card's chip select */
    takeSPIBus(hwAttrs);

    switch (ctrl) {
        case GET_SECTOR_COUNT:
            /* Get number of sectors on the disk (uint32_t) */
            if ((send_cmd(hwAttrs, CMD9, 0) == 0) &&
                 rcvr_datablock(hwAttrs, csd, 16)) {

                /* SDC ver 2.00 */
                if ((csd[0] >> 6) == 1) {
                    csize = csd[9] + ((WORD)csd[8] << 8) + 1;
                    *(uint32_t*)buf = (uint32_t)csize << 10;
                }
                /* MMC or SDC ver 1.XX */
                else {
                    n =  (csd[5] & 15) +
                        ((csd[10] & 128) >> 7) +
                        ((csd[9] & 3) << 1) + 2;

                    csize =        (csd[8] >> 6) +
                             ((WORD)csd[7] << 2) +
                            ((WORD)(csd[6] & 3) << 10) + 1;

                    *(uint32_t*)buf = (uint32_t)csize << (n - 9);
                }
                DebugP_log2("SDSPI:(%p) disk IO control: sector count: %d",
                            hwAttrs->baseAddr, *(uint32_t*)buf);
                res = RES_OK;
            }
            break;

        case GET_SECTOR_SIZE:
            /* Get sectors on the disk (WORD) */
            *(WORD*)buf = SD_SECTOR_SIZE;
            DebugP_log2("SDSPI:(%p) disk IO control: sector size: %d",
                        hwAttrs->baseAddr, *(WORD*)buf);
            res = RES_OK;
            break;

        case CTRL_SYNC:
            /* Make sure that data has been written */
            if (wait_ready(hwAttrs) == 0xFF) {
                DebugP_log1("SDSPI:(%p) disk IO control: control sync: ready",
                            hwAttrs->baseAddr);
                res = RES_OK;
            }
            else {
                DebugP_log1("SDSPI:(%p) disk IO control: control sync: not ready",
                            hwAttrs->baseAddr);
                res = RES_NOTRDY;
            }
            break;

        default:
            DebugP_log1("SDSPI:(%p) disk IO control: parameter error",
                        hwAttrs->baseAddr);
            res = RES_PARERR;
            break;
    }

    /* Deselect the SD Card's chip select */
    releaseSPIBus(hwAttrs);

    /* Idle (Release DO) */
    rxSPI(hwAttrs);

    return (res);
}

/*
 *  ======== SDSPICC3200_diskRead ========
 *  Function to perform a disk read from the SDCard. This function is called by
 *  the FatFs module and must not be called by the application!
 *
 *  @param  drv         Drive Number
 *
 *  @param  buf         Pointer to a buffer on which to store data
 *
 *  @param  sector      Starting sector number (LBA)
 *
 *  @param  count       Sector count (1...255)
 */
DRESULT SDSPICC3200_diskRead(BYTE drv, BYTE *buf,
                             DWORD sector, BYTE count)
{
    SDSPICC3200_Object        *object = sdspiHandles[drv]->object;
    SDSPICC3200_HWAttrs const *hwAttrs = sdspiHandles[drv]->hwAttrs;

    if (!count) {
        DebugP_log1("SDSPI:(%p) disk read: 0 sectors to read",
                    hwAttrs->baseAddr);

        return (RES_PARERR);
    }

    if (object->diskState & STA_NOINIT) {
        DebugP_log1("SDSPI:(%p) disk read: disk not initialized",
                    hwAttrs->baseAddr);

        return (RES_NOTRDY);
    }

    /*
     * On a SDSC card, the sector address is a byte address on the SD Card
     * On a SDHC card, the sector address is address by sector blocks
     */
    if (object->cardType != SDHC) {
        /* Convert to byte address */
        sector *= SD_SECTOR_SIZE;
    }

    /* Select the SD Card's chip select */
    takeSPIBus(hwAttrs);

    /* Single block read */
    if (count == 1) {
        if ((send_cmd(hwAttrs, CMD17, sector) == 0) &&
             rcvr_datablock(hwAttrs, buf, SD_SECTOR_SIZE)) {
            count = 0;
        }
    }
    /* Multiple block read */
    else {
        if (send_cmd(hwAttrs, CMD18, sector) == 0) {
            do {
                if (!rcvr_datablock(hwAttrs, buf, SD_SECTOR_SIZE)) {
                    break;
                }
                buf += SD_SECTOR_SIZE;
            } while (--count);

            /* STOP_TRANSMISSION */
            send_cmd(hwAttrs, CMD12, 0);
        }
    }

    /* Deselect the SD Card's chip select */
    releaseSPIBus(hwAttrs);

    /* Idle (Release DO) */
    rxSPI(hwAttrs);

    return (count ? RES_ERROR : RES_OK);
}

/*
 *  ======== SDSPICC3200_diskStatus ========
 *  Function to return the current disk status. This function is called by
 *  the FatFs module and must not be called by the application!
 *
 *  @param(drv)         Drive Number
 */
DSTATUS SDSPICC3200_diskStatus(BYTE drv)
{
    /* Get the pointer to the object */
    SDSPICC3200_Object  *object = sdspiHandles[drv]->object;

    DebugP_log2("SDSPI:(%p) disk status: diskState: %d",
                ((SDSPICC3200_HWAttrs const *)(sdspiHandles[drv]->hwAttrs))->baseAddr,
                object->diskState);

    return (object->diskState);
}

#if _READONLY == 0
/*
 *  ======== SDSPICC3200_diskWrite ========
 *  Function to perform a disk write from the SDCard. This function is called by
 *  the FatFs module and must not be called by the application!
 *
 *  @param  drv         Drive Number
 *
 *  @param  buf         Pointer to a buffer from which to read data
 *
 *  @param  sector      Starting sector number (LBA)
 *
 *  @param  count       Sector count (1...255)
 */
DRESULT SDSPICC3200_diskWrite(BYTE drv, const BYTE *buf,
                            DWORD sector, BYTE count)
{
    SDSPICC3200_Object        *object = sdspiHandles[drv]->object;
    SDSPICC3200_HWAttrs const *hwAttrs = sdspiHandles[drv]->hwAttrs;

    if (!count) {
        DebugP_log1("SDSPI:(%p) disk write: 0 sectors to write",
                    hwAttrs->baseAddr);

        return (RES_PARERR);
    }
    if (object->diskState & STA_NOINIT) {
        DebugP_log1("SDSPI:(%p) disk write: disk not initialized",
                    hwAttrs->baseAddr);

        return (RES_NOTRDY);
    }
    if (object->diskState & STA_PROTECT) {
        DebugP_log1("SDSPI:(%p) disk write: disk protected",
                    hwAttrs->baseAddr);

        return (RES_WRPRT);
    }

    /*
     * On a SDSC card, the sector address is a byte address on the SD Card
     * On a SDHC card, the sector address is address by sector blocks
     */
    if (object->cardType != SDHC) {
        /* Convert to byte address if needed */
        sector *= SD_SECTOR_SIZE;
    }

    /* Select the SD Card's chip select */
    takeSPIBus(hwAttrs);

    /* Single block write */
    if (count == 1) {
        if ((send_cmd(hwAttrs, CMD24, sector) == 0) &&
             xmit_datablock(hwAttrs, buf, START_BLOCK_TOKEN)) {
            count = 0;
        }
    }
    /* Multiple block write */
    else {
        if ((object->cardType == SDSC) || (object->cardType == SDHC)) {
            send_cmd(hwAttrs, CMD55, 0);
            send_cmd(hwAttrs, CMD23, count);    /* ACMD23 */
        }
        /* WRITE_MULTIPLE_BLOCK */
        if (send_cmd(hwAttrs, CMD25, sector) == 0) {
            do {
                if (!xmit_datablock(hwAttrs, buf, START_MULTIBLOCK_TOKEN)) {
                    break;
                }
                buf += SD_SECTOR_SIZE;
            } while (--count);

            /* STOP_TRAN token */
            if (!xmit_datablock(hwAttrs, 0, STOP_MULTIBLOCK_TOKEN)) {
                count = 1;
            }
        }
    }

    /* Deselect the SD Card's chip select */
    releaseSPIBus(hwAttrs);

    /* Idle (Release DO) */
    rxSPI(hwAttrs);

    return (count ? RES_ERROR : RES_OK);
}
#endif /* _READONLY */

/*
 *  ======== SDSPICC3200_init ========
 *  Function to initialize SDSPI module
 */
void SDSPICC3200_init(SDSPI_Handle handle)
{
    SDSPICC3200_Object       *object = handle->object;

    /* Mark the object as available */
    object->driveNumber = DRIVE_NOT_MOUNTED;
    object->diskState = STA_NOINIT;
    object->cardType = NOCARD;
}

/*
 *  ======== SDSPICC3200_open ========
 *  Function to mount the FatFs filesystem and register the SDSPICC3200 disk
 *  I/O functions with SYS/BIOS' FatFS module.
 *
 *  This function also configures some basic GPIO settings needed for the
 *  software chip select with the SDCard.
 *
 *  @param  handle      SDSPI handle
 *  @param  drv         Drive Number
 *  @param  params      SDSPI parameters
 */
SDSPI_Handle SDSPICC3200_open(SDSPI_Handle handle,
                              unsigned char drv,
                              SDSPI_Params *params)
{
    DRESULT                    dresult;
    FRESULT                    fresult;
    uintptr_t                  key;
    SDSPICC3200_Object        *object = handle->object;
    SDSPICC3200_HWAttrs const *hwAttrs = handle->hwAttrs;

    /* Determine if the device was already opened */
    key = HwiP_disable();
    if (object->driveNumber != DRIVE_NOT_MOUNTED) {
        HwiP_restore(key);
        return (NULL);
    }
    /* Mark as being used */
    object->driveNumber = drv;
    HwiP_restore(key);

    /* Store the SDSPI parameters */
    if (params == NULL) {
        /* No params passed in, so use the defaults */
        params = (SDSPI_Params *) &SDSPI_defaultParams;
    }

    /* Determine time scaling for timeouts */
    uSClockPeriod = ClockP_getSystemTickPeriod();
    DebugP_assert(uSClockPeriod != 0);

    /* Store desired bitRate */
    object->bitRate = params->bitRate;

    MAP_SPIDisable(hwAttrs->baseAddr);
    MAP_SPIReset(hwAttrs->baseAddr);

    /*
     * Configure the SPI bus to 400 kHz as required per SD specs. This frequency
     * will be adjusted later once the SD card has been successfully initialized
     */
    MAP_SPIConfigSetExpClk(hwAttrs->baseAddr,
                           MAP_PRCMPeripheralClockGet(hwAttrs->spiPRCM),
                           400000,
                           SPI_MODE_MASTER,
                           SPI_SUB_MODE_0,
                           (SPI_HW_CTRL_CS | SPI_3PIN_MODE |
                            SPI_TURBO_OFF | SPI_CS_ACTIVELOW |
                            SPI_WL_8));

    DebugP_log1("SDSPI:(%p) SDSPI freq to 400 kHz", hwAttrs->baseAddr);

    MAP_SPIEnable(hwAttrs->baseAddr);

    /* Register the new disk_*() functions */
    dresult = disk_register(object->driveNumber,
                            SDSPICC3200_diskInitialize,
                            SDSPICC3200_diskStatus,
                            SDSPICC3200_diskRead,
                            SDSPICC3200_diskWrite,
                            SDSPICC3200_diskIOctrl);

    /* Check for drive errors */
    if (dresult != RES_OK) {
        DebugP_log1("SDSPI:(%p) disk functions not registered",
                    hwAttrs->baseAddr);

        SDSPICC3200_close(handle);
        return (NULL);
    }

    /*
     * Register the filesystem with FatFs. This operation does not access the
     * SDCard yet.
     */
    fresult = f_mount(object->driveNumber, &(object->filesystem));
    if (fresult != FR_OK) {
        DebugP_log2("SDSPI:(%p) drive %d not mounted",
                    hwAttrs->baseAddr, object->driveNumber);

        SDSPICC3200_close(handle);
        return (NULL);
    }

    /* Store the new SDSPI handle for this FatFs drive number */
    sdspiHandles[drv] = handle;

    DebugP_log1("SDSPI:(%p) opened", hwAttrs->baseAddr);

    return (handle);
}
