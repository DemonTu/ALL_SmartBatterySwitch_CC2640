/*
 * Copyright (c) 2014-2015, Texas Instruments Incorporated
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
 *  ======== TimerGPT.c ========
 */

#include <xdc/std.h>

#include <ti/sysbios/family/arm/m3/Hwi.h>

#include <ti/sysbios/family/arm/cc26xx/Power.h>
#include <ti/sysbios/family/arm/cc26xx/PowerCC2650.h>

/*
 *  ======== TimerGPT_enable ========
 */
Void ti_sysbios_family_arm_cc26xx_TimerGPT_enable(Int id)
{
    UInt key;

    key = Hwi_disable();

    switch (id) {
        case 0: Power_setDependency(PERIPH_GPT0);
                break;

        case 1: Power_setDependency(PERIPH_GPT1);
                break;

        case 2: Power_setDependency(PERIPH_GPT2);
                break;

        case 3: Power_setDependency(PERIPH_GPT3);
                break;

        default:
                break;
    }

    /* declare the disallow standby constraint while GP timer is in use */
    Power_setConstraint(Power_SB_DISALLOW);

    Hwi_restore(key);
}

/*
 *  ======== TimerGPT_disable ========
 */
Void ti_sysbios_family_arm_cc26xx_TimerGPT_disable(Int id)
{
    UInt key;

    key = Hwi_disable();

    switch (id) {
       case 0: Power_releaseDependency(PERIPH_GPT0);
                break;

        case 1: Power_releaseDependency(PERIPH_GPT1);
                break;

        case 2: Power_releaseDependency(PERIPH_GPT2);
                break;

        case 3: Power_releaseDependency(PERIPH_GPT3);
                break;

        default:
                break;
    }

    /* release the disallow standby constraint when the GP timer is disabled */
    Power_releaseConstraint(Power_SB_DISALLOW);

    Hwi_restore(key);
}
