/*
 * Copyright (c) 2014, Texas Instruments Incorporated
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
 * */
/*
 *  ======== Boot.c ========
 */

#include <xdc/std.h>
#include <xdc/runtime/Assert.h>

#include "package/internal/Boot.xdc.h"

#define REG16(x)  (*(volatile UInt16 *)(x))
#define REG32(x)  (*(volatile UInt32 *)(x))

/* CS defines */
#define CSKEY     0x695A
#define CSACC     0x40010400
#define CSCTL0    0x40010404
#define CSCTL1    0x40010408

#define DCO_BITS_48     0x00050000   /* 48 MHz */
#define DIV_SEL_BITS_H  0x20100033   /* DIVS=4, DIVA=1, DIVHS=2, DIVM=1 */
                                    /* SELA=LFXT/REFO, SELS=SELM=DCO */

#define DIV_SEL_BITS_M  0x20210033   /* DIVS=4, DIVA=1, DIVHS=4, DIVM=2 */
                                    /* SELA=LFXT/REFO, SELS=SELM=DCO */

#define DCO_BITS_12     0x00030000   /* 12 MHz */
#define DIV_SEL_BITS_L  0x20200033   /* DIVS=4, DIVA=1, DIVHS=4, DIVM=1 */
                                    /* SELA=LFXT/REFO, SELS=SELM=DCO */

/* WDT defines */
#define WDTCTL   0x4000480C
#define WDTPW    0x5a00
#define WDTHOLD  0x0080

/* FLCTL defines */
#define FLCTL_RDCTL_BNK0_REG  0x40011010
#define FLCTL_RDCTL_BNK1_REG  0x40011014
#define WAIT_MASK             0x0000F000
#define WAIT_0_BITS           0x00000000  /* 0 wait states up to 16MHz */
#define WAIT_1_BITS           0x00001000  /* 1 wait states up to 32MHz */
#define WAIT_2_BITS           0x00002000  /* 2 wait states for 48MHz */

/* PCM defines */
#define PCM_AM1_LDO  0x01
#define ROM_APITABLE ((unsigned long *)0x02000800)
#define ROM_PCMTABLE ((unsigned long *)(ROM_APITABLE[13]))
#define ROM_PCM_setPowerState ((UInt8 (*)(UInt8 state))ROM_PCMTABLE[6])

#define Boot_configureClocksLow ti_sysbios_family_arm_msp432_init_Boot_configureClocksLow
#define Boot_configureClocksMed ti_sysbios_family_arm_msp432_init_Boot_configureClocksMed
#define Boot_configureClocksHigh ti_sysbios_family_arm_msp432_init_Boot_configureClocksHigh
#define Boot_disableWatchdog ti_sysbios_family_arm_msp432_init_Boot_disableWatchdog

/*
 *  ======== Boot_configureClocksLow ========
 */
Void ti_sysbios_family_arm_msp432_init_Boot_configureClocksLow()
{
    UInt32 temp;

    /* setup Flash wait states (MCLK = 12MHz requires no wait states) */
    temp = REG32(FLCTL_RDCTL_BNK0_REG) & ~WAIT_MASK;
    REG32(FLCTL_RDCTL_BNK0_REG) = temp | WAIT_0_BITS;
    temp = REG32(FLCTL_RDCTL_BNK1_REG) & ~WAIT_MASK;
    REG32(FLCTL_RDCTL_BNK1_REG) = temp | WAIT_0_BITS;

    /* setup Clock System
     *  MCLK =  12MHz from DCO
     *  HSMCLK = 3MHz from DCO
     *  SMCLK =  3MHz from DCO
     *  ACLK =  32KHz from REFOCLK
     *  BCLK =  32KHz from REFOCLK
     */
    REG32(CSACC) = CSKEY;           /* unlock CS register access */
    REG32(CSCTL1) = DIV_SEL_BITS_L; /* set clock selector and divider bits */
    REG32(CSCTL0) = DCO_BITS_12;    /* set DCO to 12 MHz */
    REG32(CSACC) = 0;               /* lock CS register access */
}

/*
 *  ======== Boot_configureClocksMed ========
 */
Void ti_sysbios_family_arm_msp432_init_Boot_configureClocksMed()
{
    UInt32 temp;

    /* switch to AM1_LDO mode (MCLK = 24MHz requires VCORE = 1) */
    ROM_PCM_setPowerState(PCM_AM1_LDO);

    /* setup Flash wait states (MCLK = 24MHz requires 1 wait state) */
    temp = REG32(FLCTL_RDCTL_BNK0_REG) & ~WAIT_MASK;
    REG32(FLCTL_RDCTL_BNK0_REG) = temp | WAIT_1_BITS;
    temp = REG32(FLCTL_RDCTL_BNK1_REG) & ~WAIT_MASK;
    REG32(FLCTL_RDCTL_BNK1_REG) = temp | WAIT_1_BITS;

    /* setup Clock System
     *  MCLK =   24MHz from DCO
     *  HSMCLK =  6MHz from DCO
     *  SMCLK =   6MHz from DCO
     *  ACLK =   32KHz from REFOCLK
     *  BCLK =   32KHz from REFOCLK
     */
    REG32(CSACC) = CSKEY;           /* unlock CS register access */
    REG32(CSCTL1) = DIV_SEL_BITS_M; /* set clock selector and divider bits */
    REG32(CSCTL0) = DCO_BITS_48;    /* set DCO to 48MHz */
    REG32(CSACC) = 0;               /* lock CS register access */
}

/*
 *  ======== Boot_configureClocksHigh ========
 */
Void ti_sysbios_family_arm_msp432_init_Boot_configureClocksHigh()
{
    UInt32 temp;

    /* switch to AM1_LDO mode (MCLK = 48MHz requires VCORE = 1) */
    ROM_PCM_setPowerState(PCM_AM1_LDO);

    /* setup Flash wait states (MCLK = 48MHz requires 2 wait states */
    temp = REG32(FLCTL_RDCTL_BNK0_REG) & ~WAIT_MASK;
    REG32(FLCTL_RDCTL_BNK0_REG) = temp | WAIT_2_BITS;
    temp = REG32(FLCTL_RDCTL_BNK1_REG) & ~WAIT_MASK;
    REG32(FLCTL_RDCTL_BNK1_REG) = temp | WAIT_2_BITS;

    /* setup Clock System
     *  MCLK =   48MHz from DCO
     *  HSMCLK = 24MHz from DCO
     *  SMCLK =  12MHz from DCO
     *  ACLK =   32KHz from REFOCLK
     *  BCLK =   32KHz from REFOCLK
     */
    REG32(CSACC) = CSKEY;           /* unlock CS register access */
    REG32(CSCTL1) = DIV_SEL_BITS_H; /* set clock selector and divider bits */
    REG32(CSCTL0) = DCO_BITS_48;    /* set DCO to 48MHz */
    REG32(CSACC) = 0;               /* lock CS register access */
}

/*
 *  ======== Boot_disableWatchdog ========
 */
Void ti_sysbios_family_arm_msp432_init_Boot_disableWatchdog()
{
    REG16(WDTCTL) = WDTPW + WDTHOLD;
}
