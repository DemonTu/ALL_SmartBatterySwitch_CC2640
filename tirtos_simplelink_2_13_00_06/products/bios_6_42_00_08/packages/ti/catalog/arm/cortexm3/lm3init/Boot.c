/*
 * Copyright (c) 2012, Texas Instruments Incorporated
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

#include "_sysctl.h"

#define Boot_sysCtlDelay      ti_catalog_arm_cortexm3_lm3init_Boot_sysCtlDelay
#define Boot_sysCtlClockSet   ti_catalog_arm_cortexm3_lm3init_Boot_sysCtlClockSet
#define Boot_configureLdo     ti_catalog_arm_cortexm3_lm3init_Boot_configureLdo

#define HWREG(x) (*((volatile unsigned long *)(x)))

#if defined(__ti__)
#pragma CODE_SECTION(Boot_sysCtlClockSet, ".text:.bootCodeSection")
#pragma CODE_SECTION(Boot_sysCtlDelay, ".text:.bootCodeSection")
#pragma CODE_SECTION(Boot_configureLdo, ".text:.bootCodeSection")
#endif

extern Void Boot_sysCtlDelay(UInt ulCount);

/*
 *  ======== sysCtlDelay ========
 *
 *  Provides a small delay.
 *
 *  ulCount is the number of delay loop iterations to perform.
 *
 *  This function provides a means of generating a constant length delay.  It
 *  is written in assembly to keep the delay consistent across tool chains,
 *  avoiding the need to tune the delay based on the tool chain in use.
 *
 *  The loop takes 3 cycles/loop.
 *
 *  \return None.
 */
#ifdef __IAR_SYSTEMS_ICC__
Void Boot_sysCtlDelay(UInt ulCount)
{
    asm("                                           \n\
        subs    r0, #1                         \n\
        bne.n   ti_catalog_arm_cortexm3_lm3init_Boot_sysCtlDelay \n\
        bx      lr                             \n\
    ");
}
#else
asm("						\n\
_ti_catalog_arm_cortexm3_lm3init_Boot_sysCtlDelay:	\n\
ti_catalog_arm_cortexm3_lm3init_Boot_sysCtlDelay:	\n\
	 subs    r0, #1				\n\
         bne.n   _ti_catalog_arm_cortexm3_lm3init_Boot_sysCtlDelay \n\
         bx	 lr 				\n\
");
#endif

/*
 *  ======== Boot_sysCtlClockSet ========
 *
 *  Sets the clocking of the device.
 *
 *  \param ulConfig is the required configuration of the device clocking.
 *
 *  This function configures the clocking of the device.  The input crystal
 *  frequency, oscillator to be used, use of the PLL, and the system clock
 *  divider are all configured with this function.
 *
 *  The \e ulConfig parameter is the logical OR of several different values,
 *  many of which are grouped into sets where only one can be chosen.
 *
 *  The system clock divider is chosen with one of the following values:
 *  \b SYSCTL_SYSDIV_1, \b SYSCTL_SYSDIV_2, \b SYSCTL_SYSDIV_3, ...
 *  \b SYSCTL_SYSDIV_64.  Only \b SYSCTL_SYSDIV_1 through \b SYSCTL_SYSDIV_16
 *  are valid on Sandstorm-class devices.
 *
 *  The use of the PLL is chosen with either \b SYSCTL_USE_PLL or
 *  \b SYSCTL_USE_OSC.
 *
 *  The external crystal frequency is chosen with one of the following values:
 *  \b SYSCTL_XTAL_1MHZ, \b SYSCTL_XTAL_1_84MHZ, \b SYSCTL_XTAL_2MHZ,
 *  \b SYSCTL_XTAL_2_45MHZ, \b SYSCTL_XTAL_3_57MHZ, \b SYSCTL_XTAL_3_68MHZ,
 *  \b SYSCTL_XTAL_4MHZ, \b SYSCTL_XTAL_4_09MHZ, \b SYSCTL_XTAL_4_91MHZ,
 *  \b SYSCTL_XTAL_5MHZ, \b SYSCTL_XTAL_5_12MHZ, \b SYSCTL_XTAL_6MHZ,
 *  \b SYSCTL_XTAL_6_14MHZ, \b SYSCTL_XTAL_7_37MHZ, \b SYSCTL_XTAL_8MHZ,
 *  \b SYSCTL_XTAL_8_19MHZ, \b SYSCTL_XTAL_10MHZ, \b SYSCTL_XTAL_12MHZ,
 *  \b SYSCTL_XTAL_12_2MHZ, \b SYSCTL_XTAL_13_5MHZ, \b SYSCTL_XTAL_14_3MHZ,
 *  \b SYSCTL_XTAL_16MHZ, or \b SYSCTL_XTAL_16_3MHZ.  Values below
 *  \b SYSCTL_XTAL_3_57MHZ are not valid when the PLL is in operation.  On
 *  Sandstorm- and Fury-class devices, values above \b SYSCTL_XTAL_8_19MHZ are
 *  not valid.
 *
 *  The oscillator source is chosen with one of the following values:
 *  \b SYSCTL_OSC_MAIN, \b SYSCTL_OSC_INT, \b SYSCTL_OSC_INT4,
 *  \b SYSCTL_OSC_INT30, or \b SYSCTL_OSC_EXT32.  On Sandstorm-class devices,
 *  \b SYSCTL_OSC_INT30 and \b SYSCTL_OSC_EXT32 are not valid.
 *  \b SYSCTL_OSC_EXT32 is only available on devices with the hibernate module,
 *  and then only when the hibernate module has been enabled.
 *
 *  The internal and main oscillators are disabled with the
 *  \b SYSCTL_INT_OSC_DIS and \b SYSCTL_MAIN_OSC_DIS flags, respectively.
 *  The external oscillator must be enabled in order to use an external clock
 *  source.  Note that attempts to disable the oscillator used to clock the
 *  device will be prevented by the hardware.
 *
 *  To clock the system from an external source (such as an external crystal
 *  oscillator), use \b SYSCTL_USE_OSC \b | \b SYSCTL_OSC_MAIN.  To clock the
 *  system from the main oscillator, use \b SYSCTL_USE_OSC \b |
 *  \b SYSCTL_OSC_MAIN.  To clock the system from the PLL, use
 *  \b SYSCTL_USE_PLL \b | \b SYSCTL_OSC_MAIN, and select the appropriate
 *  crystal with one of the \b SYSCTL_XTAL_xxx values.
 *
 *  \note If selecting the PLL as the system clock source (that is, via
 *  \b SYSCTL_USE_PLL), this function will poll the PLL lock interrupt to
 *  determine when the PLL has locked.  If an interrupt handler for the
 *  system control interrupt is in place, and it responds to and clears the
 *  PLL lock interrupt, this function will delay until its timeout has occurred
 *  instead of completing as soon as PLL lock is achieved.
 *
 *  \return None.
 */

Void Boot_sysCtlClockSet(UInt ulConfig)
{
    UInt ulDelay, ulRCC, ulRCC2;

    /*
     * Get the current value of the RCC and RCC2 registers.  If using a
     * Sandstorm-class device, the RCC2 register reads back as zero and the
     * writes to it from within this function are ignored.
     */
    ulRCC = HWREG(SYSCTL_RCC);
    ulRCC2 = HWREG(SYSCTL_RCC2);

    /*
     * Bypass the PLL and system clock dividers for now.
     */
    ulRCC |= SYSCTL_RCC_BYPASS;
    ulRCC &= ~(SYSCTL_RCC_USESYSDIV);
    ulRCC2 |= SYSCTL_RCC2_BYPASS2;

    /*
     * Write the new RCC value.
     */
    HWREG(SYSCTL_RCC) = ulRCC;
    HWREG(SYSCTL_RCC2) = ulRCC2;

    /*
     * See if either oscillator needs to be enabled.
     */
    if (((ulRCC & SYSCTL_RCC_IOSCDIS) && !(ulConfig & SYSCTL_RCC_IOSCDIS)) ||
       ((ulRCC & SYSCTL_RCC_MOSCDIS) && !(ulConfig & SYSCTL_RCC_MOSCDIS))) {
        /*
         * Make sure that the required oscillators are enabled.  For now, the
         * previously enabled oscillators must be enabled along with the newly
         * requested oscillators.
         */
        ulRCC &= (~(SYSCTL_RCC_IOSCDIS | SYSCTL_RCC_MOSCDIS) |
                  (ulConfig & (SYSCTL_RCC_IOSCDIS | SYSCTL_RCC_MOSCDIS)));

        /*
         * Write the new RCC value.
         */
        HWREG(SYSCTL_RCC) = ulRCC;

        /*
         * Wait for a bit, giving the oscillator time to stabilize.  The number
         * of iterations is adjusted based on the current clock source; a
         * smaller number of iterations is required for slower clock rates.
         */
        if (((ulRCC2 & SYSCTL_RCC2_USERCC2) &&
            (((ulRCC2 & SYSCTL_RCC2_OSCSRC2_M) == SYSCTL_RCC2_OSCSRC2_30) ||
             ((ulRCC2 & SYSCTL_RCC2_OSCSRC2_M) == SYSCTL_RCC2_OSCSRC2_32))) ||
           (!(ulRCC2 & SYSCTL_RCC2_USERCC2) &&
            ((ulRCC & SYSCTL_RCC_OSCSRC_M) == SYSCTL_RCC_OSCSRC_30))) {
            /*
             * Delay for 4096 iterations.
             */
            Boot_sysCtlDelay(4096);
        }
        else {
            /*
             * Delay for 524,288 iterations.
             */
            Boot_sysCtlDelay(524288);
        }
    }

    /*
     * Set the new crystal value, oscillator source, and PLL configuration.
     * Since the OSCSRC2 field in RCC2 overlaps the XTAL field in RCC, the
     * OSCSRC field has a special encoding within ulConfig to avoid the
     * overlap.
     */
    ulRCC &= ~(SYSCTL_RCC_XTAL_M | SYSCTL_RCC_OSCSRC_M);
    ulRCC |= ulConfig & (SYSCTL_RCC_XTAL_M | SYSCTL_RCC_OSCSRC_M);
    ulRCC2 &= ~(SYSCTL_RCC2_USERCC2 | SYSCTL_RCC2_OSCSRC2_M);
    ulRCC2 |= ulConfig & (SYSCTL_RCC2_USERCC2 | SYSCTL_RCC_OSCSRC_M);
    ulRCC2 |= (ulConfig & 0x00000008) << 3;

    /*
     * Write the new RCC value.
     */
    HWREG(SYSCTL_RCC) = ulRCC;
    HWREG(SYSCTL_RCC2) = ulRCC2;

    /*
     * Wait for a bit so that new crystal value and oscillator source can take
     * effect.
     */
    Boot_sysCtlDelay(16);

    /*
     * Set the PLL configuration.
     */
    ulRCC &= ~(SYSCTL_RCC_PWRDN | SYSCTL_RCC_OEN);
    ulRCC |= ulConfig & (SYSCTL_RCC_PWRDN | SYSCTL_RCC_OEN);
    ulRCC2 &= ~(SYSCTL_RCC2_PWRDN2);
    ulRCC2 |= ulConfig & SYSCTL_RCC2_PWRDN2;

    /*
     * Clear the PLL lock interrupt.
     */
    HWREG(SYSCTL_MISC) = SYSCTL_INT_PLL_LOCK;

    /*
     * Write the new RCC value.
     */
    if (ulRCC2 & SYSCTL_RCC2_USERCC2) {
        HWREG(SYSCTL_RCC2) = ulRCC2;
        HWREG(SYSCTL_RCC) = ulRCC;
    }
    else {
        HWREG(SYSCTL_RCC) = ulRCC;
        HWREG(SYSCTL_RCC2) = ulRCC2;
    }

    /*
     * Set the requested system divider and disable the appropriate
     * oscillators.  This value is not written immediately.
     */
    ulRCC &= ~(SYSCTL_RCC_SYSDIV_M | SYSCTL_RCC_USESYSDIV |
               SYSCTL_RCC_IOSCDIS | SYSCTL_RCC_MOSCDIS);
    ulRCC |= ulConfig & (SYSCTL_RCC_SYSDIV_M | SYSCTL_RCC_USESYSDIV |
                         SYSCTL_RCC_IOSCDIS | SYSCTL_RCC_MOSCDIS);
    ulRCC2 &= ~(SYSCTL_RCC2_SYSDIV2_M);
    ulRCC2 |= ulConfig & SYSCTL_RCC2_SYSDIV2_M;
    if (ulConfig & SYSCTL_RCC2_DIV400) {
        ulRCC |= SYSCTL_RCC_USESYSDIV;
        ulRCC2 &= ~(SYSCTL_RCC_USESYSDIV);
        ulRCC2 |= ulConfig & (SYSCTL_RCC2_DIV400 | SYSCTL_RCC2_SYSDIV2LSB);
    }
    else {
        ulRCC2 &= ~(SYSCTL_RCC2_DIV400);
    }

    /*
     * See if the PLL output is being used to clock the system.
     */
    if (!(ulConfig & SYSCTL_RCC_BYPASS)) {
        /*
         * Wait until the PLL has locked.
         */
        for(ulDelay = 32768; ulDelay > 0; ulDelay--) {
            if(HWREG(SYSCTL_RIS) & SYSCTL_INT_PLL_LOCK) {
                break;
            }
        }

        /*
         * Enable use of the PLL.
         */
        ulRCC &= ~(SYSCTL_RCC_BYPASS);
        ulRCC2 &= ~(SYSCTL_RCC2_BYPASS2);
    }

    /*
     * Write the final RCC value.
     */
    HWREG(SYSCTL_RCC) = ulRCC;
    HWREG(SYSCTL_RCC2) = ulRCC2;

    /*
     * Delay for a little bit so that the system divider takes effect.
     */
    Boot_sysCtlDelay(16);
}

/*
 *  ======== Boot_configureLdo ========
 */
Void Boot_configureLdo(UInt ulVoltage)
{
    /*
     * Set the LDO voltage to the requested value.
     */
    HWREG(SYSCTL_LDOPCTL) = ulVoltage;
}

