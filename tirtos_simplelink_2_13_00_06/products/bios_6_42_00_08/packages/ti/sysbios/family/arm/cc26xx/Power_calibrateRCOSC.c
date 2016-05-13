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
 * EXEMPLARY, OR CONSEQueueNTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 *  ======== Power_calibrateRCOSC.c ========
 */

#include <xdc/std.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/family/arm/cc26xx/Power.h>
#include <ti/sysbios/family/arm/cc26xx/PowerCC2650.h>

#include <driverlib/ddi.h>
#include <driverlib/ioc.h>
#include <driverlib/osc.h>
#include <driverlib/gpio.h>
#include <driverlib/sys_ctrl.h>
#include <driverlib/aux_wuc.h>
#include <inc/hw_aux_evctl.h>
#include <inc/hw_aux_smph.h>
#include <inc/hw_aux_wuc.h>
#include <inc/hw_aux_tdc.h>
#include <inc/hw_ddi_0_osc.h>
#include <inc/hw_ddi.h>
#include <inc/hw_ccfg.h>

#define AUX_TDC_SEMAPHORE_NUMBER        1  /* semaphore 1 protects TDC */
#define NUM_RCOSC_LF_PERIODS_TO_MEASURE 32 /* x RCOSC_LF periods vs XOSC_HF */
#define NUM_RCOSC_HF_PERIODS_TO_MEASURE 1  /* x RCOSC_HF periods vs XOSC_HF */
#define ACLK_REF_SRC_RCOSC_HF           0  /* Use RCOSC_HF for ACLK REF */
#define ACLK_REF_SRC_RCOSC_LF           2  /* Use RCOSC_LF for ACLK REF */

#define SCLK_LF_OPTION_RCOSC_LF         3  /* defined in cc26_ccfg.xls */

#define DDI_0_OSC_O_CTL1_LOCAL                   0x00000004     /* offset */
#define DDI_0_OSC_CTL1_RCOSCHFCTRIMFRACT_LOCAL_M 0x007C0000     /* mask */
#define DDI_0_OSC_CTL1_RCOSCHFCTRIMFRACT_LOCAL_S    18          /* shift */
#define DDI_0_OSC_CTL1_RCOSCHFCTRIMFRACT_EN_LOCAL_M 0x00020000  /* mask */
#define DDI_0_OSC_CTL1_RCOSCHFCTRIMFRACT_EN_LOCAL_S 17          /* shift */

/* AUX ISR states */
#define WAIT_SMPH       0   /* just took SMPH, start RCOSC_LF */
#define CAL_RCOSC_LF    1   /* just finished RCOSC_LF, start first RCOSC_HF */
#define CAL_RCOSC_HF1   2   /* just finished 1st RCOSC_HF, start 2nd */
#define CAL_RCOSC_HF2   3   /* just finished 2nd RCOSC_HF, decide best */

/* macros */
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/* forward declarations */
static Int abs(Int i);
static Void calibrateRcoscHf1(Int32 tdcResult);
static Void calibrateRcoscHf2(Int32 tdcResult);
static Void cleanupAfterMeasurements(Void);
static Bool getTdcSemaphore(Void);
static Int32 scaleRndInf(Int32 x);
static Void tdcInit();
static Void tdcPerformRcOscMeasurement(UInt32 refClkSrc,
    UInt32 numEdgesToCount);
static Void updateSubSecInc(UInt32 tdcResult);

/* static globals */
static UInt32 calibrateStep;
static Bool doLF = FALSE;
static Bool firstLF = TRUE;
static Int32 nDeltaFreqCurr;
static Int32 nCtrimCurr;
static Int32 nCtrimFractCurr;
static Int32 nCtrimNew;
static Int32 nCtrimFractNew;

#define INSTRUMENT 0

#if INSTRUMENT
volatile UInt gotSEM = 0;
volatile UInt calLF = 0;
volatile UInt calHF1 = 0;
volatile UInt calHF2 = 0;
UInt tdcResult_LF = 0;
UInt tdcResult_HF1 = 0;
UInt tdcResult_HF2 = 0;
UInt numISRs = 0;
#endif

/*
 *  ======== calibrateRCOSCs ========
 *  Initiate calibration of RCOSC_LF and RCOSCHF
 */
Bool Power_initiateCalibration()
{
    Bool status;
    Bool gotSem;
    UInt32 ccfgLfClkSrc;

    /* set contraints to prohibit powering down during calibration sequence */
    Power_setConstraint(Power_SB_DISALLOW);
    Power_setConstraint(Power_IDLE_PD_DISALLOW);

    /* set dependency to keep XOSC_HF active during calibration sequence */
    Power_setDependency(XOSC_HF);

    /* read the LF clock source from CCFG */
    ccfgLfClkSrc = (HWREG(CCFG_BASE + CCFG_O_MODE_CONF) &
        CCFG_MODE_CONF_SCLK_LF_OPTION_M) >> CCFG_MODE_CONF_SCLK_LF_OPTION_S;

    /* check to see if should do RCOSC_LF calibration,  */
    if (ccfgLfClkSrc == SCLK_LF_OPTION_RCOSC_LF) {
        doLF = TRUE;
    }
    else {
        doLF = FALSE;
    }

    /* initiate acquisition of semaphore protecting TDC */
    gotSem = getTdcSemaphore();

    /* if didn't acquire semaphore, must wait for autotake ISR */
    if (gotSem == FALSE) {
        calibrateStep = WAIT_SMPH;
        status = FALSE;  /* FALSE: don't do anything else until acquire SMPH */
    }

    /* else, semaphore acquired, OK to proceed with first measurement */
    else {
#if INSTRUMENT
        gotSEM = 1;
#endif
        status = TRUE;   /* TRUE: OK to start first measurement */
    }

    return (status);
}

/*
 *  ======== startFirstMeasurment ========
 *  Start the first RCOSC measurement
 */
Void Power_startFirstMeasurement()
{
    /* setup TDC */
    tdcInit();

    /* if LF clock is from RCOSC_LF then do compensation */
    if (doLF == TRUE) {
        /* clear UPD_REQ, new sub-second increment is NOT available */
        HWREG(AUX_WUC_BASE + AUX_WUC_O_RTCSUBSECINCCTL) = 0;

        /* initiate measurement of RCOSC_LF */
        tdcPerformRcOscMeasurement(ACLK_REF_SRC_RCOSC_LF,
            NUM_RCOSC_LF_PERIODS_TO_MEASURE);

        /* next state is RCOSC_LF measurement */
        calibrateStep = CAL_RCOSC_LF;

    }

    /* else, if LF clock is from XOSC_LF, start XOSC_HF calibration */
    else {
        /* initiate first measurement of RCOSC_HF */
        tdcPerformRcOscMeasurement(ACLK_REF_SRC_RCOSC_HF,
            NUM_RCOSC_HF_PERIODS_TO_MEASURE);

        /* next state is RCOSC_HF meas. #1 */
        calibrateStep = CAL_RCOSC_HF1;
    }

}

/*
 *  ======== Power_auxISR ========
 *  ISR for the AUX combo interrupt event.  Implements state machine to
 *  step through the RCOSC calibration steps.
 */
Void ti_sysbios_family_arm_cc26xx_Power_auxISR(UArg arg)
{
    UInt32 tdcResult;

#if INSTRUMENT
    numISRs++;
#endif

    /* read the interrupt source */
    UInt32 auxEvent = HWREG(AUX_EVCTL_BASE + AUX_EVCTL_O_EVTOMCUFLAGS);

    /* disable interrupts (clearing them does not clear the source (bug)) */
    HWREG(AUX_EVCTL_BASE + AUX_EVCTL_O_COMBEVTOMCUMASK) &= ~auxEvent;

    /* clear the interrupt source */
    HWREG(AUX_EVCTL_BASE + AUX_EVCTL_O_EVTOMCUFLAGSCLR) = auxEvent;

    /* ****** state = WAIT_SMPH: arrive here if just took the SMPH ****** */
    if (calibrateStep == WAIT_SMPH) {
#if INSTRUMENT
        gotSEM = 1;
#endif
        Power_startFirstMeasurement();
    }

    /* **** state = CAL_RCOSC_LF: here when just finished LF counting **** */
    else if (calibrateStep == CAL_RCOSC_LF) {
        calibrateStep = CAL_RCOSC_HF1;    /* next state is RCOSC_HF meas. #1 */
        tdcResult = HWREG(AUX_TDCIF_BASE + AUX_TDC_O_RESULT);

#if INSTRUMENT
        tdcResult_LF = tdcResult;
#endif
        /* update the RTC SUBSECINC register based on LF measurement result */
        updateSubSecInc(tdcResult);

#if INSTRUMENT
        calLF = 1;
#endif
        /* initiate first measurement of RCOSC_HF */
        tdcPerformRcOscMeasurement(ACLK_REF_SRC_RCOSC_HF,
            NUM_RCOSC_HF_PERIODS_TO_MEASURE);
    }

    /* ****** state = CAL_RCOSC_HF1: here when just finished 1st RCOSC_HF */
    else if (calibrateStep == CAL_RCOSC_HF1) {
        tdcResult = HWREG(AUX_TDCIF_BASE + AUX_TDC_O_RESULT);

#if INSTRUMENT
        tdcResult_HF1 = tdcResult;
#endif
        if (tdcResult == 1536)
        {
            /* settings perfect, nothing more to do, calibration is done.*/
            calibrateStep = WAIT_SMPH;
            cleanupAfterMeasurements();
        }
        else
        {
            /* next state is RCOSC_HF meas. #2 */
            calibrateStep = CAL_RCOSC_HF2;
            /* use first HF measurement to setup new trim values */
            calibrateRcoscHf1(tdcResult);
            /* initiate second measurement of RCOSC_HF */
            tdcPerformRcOscMeasurement(ACLK_REF_SRC_RCOSC_HF,
                                       NUM_RCOSC_HF_PERIODS_TO_MEASURE);
        }

#if INSTRUMENT
        calHF1 = 1;
#endif

    }

    /* ****** state = just finished second RCOSC_HF measurement ****** */
    else if (calibrateStep == CAL_RCOSC_HF2) {
        calibrateStep = WAIT_SMPH;  /* restart state will be for re-take SMPH */
        tdcResult = HWREG(AUX_TDCIF_BASE + AUX_TDC_O_RESULT);

#if INSTRUMENT
        tdcResult_HF2 = tdcResult;
#endif
        /* look for improvement on #2, else revert to previous trim values */
        calibrateRcoscHf2(tdcResult);

        /* done with this cal sequence, cleanup */
        cleanupAfterMeasurements();

    }
}

/*
 *  ======== getTdcSemaphore ========
 *  Get TDC semaphore (number 1)
 */
static Bool getTdcSemaphore()
{
    UInt own;

    /* try to acquire SMPH */
    own = HWREG(AUX_SMPH_BASE + AUX_SMPH_O_SMPH1);

    /* if acquired SMPH: done */
    if (own != 0) {
        return (TRUE);
    }

    /*
     * else, did not acquire the semaphore, enable SMPH_AUTOTAKE_DONE event
     * (don't OR, write entire register, no other interrupts can be enabled!)
     */
    HWREG(AUX_EVCTL_BASE + AUX_EVCTL_O_COMBEVTOMCUMASK) =
        AUX_EVCTL_COMBEVTOMCUMASK_SMPH_AUTOTAKE_DONE;

    /* start AUTOTAKE of semaphore for TDC access */
    HWREG(AUX_SMPH_BASE + AUX_SMPH_O_AUTOTAKE) = AUX_TDC_SEMAPHORE_NUMBER;

    return (FALSE);
}

/*
 *  ======== tdcInit ========
 *  Initialize the TDC
 */
static Void tdcInit()
{
    /* Turn on clock to TDC module */
    AUXWUCClockEnable(AUX_WUC_TDCIF_CLOCK);

    /* Set saturation config to 2^24 */
    HWREG(AUX_TDCIF_BASE + AUX_TDC_O_SATCFG) = AUX_TDC_SATCFG_LIMIT_ROVF;

    /* Set start and stop trigger sources and polarity */
    HWREG(AUX_TDCIF_BASE + AUX_TDC_O_TRIGSRC) =
        (AUX_TDC_TRIGSRC_STOP_SRC_ACLK_REF | AUX_TDC_TRIGSRC_STOP_POL_HIGH) |
        (AUX_TDC_TRIGSRC_START_SRC_ACLK_REF | AUX_TDC_TRIGSRC_START_POL_HIGH);

    /* Set TDC_SRC clock to be XOSC_HF/2 = 24 MHz */
    DDI16BitfieldWrite(AUX_DDI0_OSC_BASE, DDI_0_OSC_O_CTL0,
                           DDI_0_OSC_CTL0_ACLK_TDC_SRC_SEL_M,
                           DDI_0_OSC_CTL0_ACLK_TDC_SRC_SEL_S,
                           2);

    /* Set AUX_WUC:TDCCLKCTL.REQ... */
    HWREG(AUX_WUC_BASE + AUX_WUC_O_TDCCLKCTL) = AUX_WUC_TDCCLKCTL_REQ;

    /* Wait for AUX_WUC:TDCCLKCTL.ACK to be set... */
    while(!(HWREG(AUX_WUC_BASE + AUX_WUC_O_TDCCLKCTL) & AUX_WUC_TDCCLKCTL_ACK));

    /* Enable trig count */
    HWREG(AUX_TDCIF_BASE + AUX_TDC_O_TRIGCNTCFG) = AUX_TDC_TRIGCNTCFG_EN;
}

/*
 *  ======== tdcPerformRcOscMeasurement ========
 *  Initiate counting of clock edges versus XOSC_HF clock source
 *
 *  refClkSrc: The reference clock source:
 *      0 = RCOSC_HF (divided by 1536) = 31.25 kHz
 *      2 = RCOSC_LF
 *
 *  numEdgesToCount: Number of edges to count
 *
 *  Returns: The 32-bit TDC result count value
 *
 */
static Void tdcPerformRcOscMeasurement(UInt32 refClkSrc, UInt32 numEdgesToCount)
{
    /* Abort TDC */
    HWREG(AUX_TDCIF_BASE + AUX_TDC_O_CTL) = AUX_TDC_CTL_CMD_ABORT;

    /* Clear AUX_WUC:REFCLKCTL.REQ... */
    HWREG(AUX_WUC_BASE + AUX_WUC_O_REFCLKCTL) = 0;

    /* ...and wait for AUX_WUC:REFCLKCTL.ACK to be cleared */
    while(HWREG(AUX_WUC_BASE + AUX_WUC_O_REFCLKCTL) & AUX_WUC_REFCLKCTL_ACK);

    /* Set the ACLK reference clock */
    DDI16BitfieldWrite(AUX_DDI0_OSC_BASE, DDI_0_OSC_O_CTL0,
                           DDI_0_OSC_CTL0_ACLK_REF_SRC_SEL_M,
                           DDI_0_OSC_CTL0_ACLK_REF_SRC_SEL_S,
                           refClkSrc);

    /* ...and AUX_WUC:REFCLKCTL.REQ */
    HWREG(AUX_WUC_BASE + AUX_WUC_O_REFCLKCTL) = AUX_WUC_REFCLKCTL_REQ;

    /* Wait for AUX_WUC:REFCLKCTL.ACK to be set... */
    while(!(HWREG(AUX_WUC_BASE + AUX_WUC_O_REFCLKCTL) & AUX_WUC_REFCLKCTL_ACK));

    /* Set number of periods of ACLK to count */
    HWREG(AUX_TDCIF_BASE + AUX_TDC_O_TRIGCNTLOAD) = numEdgesToCount;

    /* Reset/clear result of TDC */
    HWREG(AUX_TDCIF_BASE + AUX_TDC_O_CTL) = AUX_TDC_CTL_CMD_CLR_RESULT;

    /* Clear possible pending interrupt source */
    HWREG(AUX_EVCTL_BASE + AUX_EVCTL_O_EVTOMCUFLAGSCLR) =
        AUX_EVCTL_EVTOMCUFLAGSCLR_TDC_DONE;

    /* Enable TDC done interrupt as part of AUX_COMBINED interrupt */
    HWREG(AUX_EVCTL_BASE + AUX_EVCTL_O_COMBEVTOMCUMASK) =
        AUX_EVCTL_COMBEVTOMCUMASK_TDC_DONE;

    /* Run TDC (start synchronously) */
    HWREG(AUX_TDCIF_BASE + AUX_TDC_O_CTL) = AUX_TDC_CTL_CMD_RUN_SYNC_START;
}

/*
 *  ======== updateSubSecInc ========
 *  Update the SUBSECINC register based on measured RCOSC_LF frequency
 */
static Void updateSubSecInc(UInt32 tdcResult)
{
    UInt32 newSubSecInc;
    UInt32 oldSubSecInc;
    UInt32 subSecInc;

    /*
     * Calculate the new SUBSECINC
     * Here's the formula: AON_RTC:SUBSECINC = (45813 * NR) / 256
     */
    newSubSecInc = (45813 * tdcResult) / 256;

    /* Apply filter, but not for first calibration */
    if (firstLF) {
        /* Don't apply filter first time, to converge faster */
        subSecInc = newSubSecInc;
        /* No longer first measurement */
        firstLF = FALSE;
    }
    else {
        /* Read old SUBSECINC value */
        oldSubSecInc = HWREG(AON_RTC_BASE + AON_RTC_O_SUBSECINC) & 0x00FFFFFF;
        /* Apply filter, 0.5 times old value, 0.5 times new value */
        subSecInc = (oldSubSecInc * 1 + newSubSecInc * 1) / 2;
    }

    /* Update SUBSECINC values */
    HWREG(AUX_WUC_BASE + AUX_WUC_O_RTCSUBSECINC0) = subSecInc;
    HWREG(AUX_WUC_BASE + AUX_WUC_O_RTCSUBSECINC1) = subSecInc >> 16;

    /* update to use new values */
    HWREG(AUX_WUC_BASE + AUX_WUC_O_RTCSUBSECINCCTL) =
        AUX_WUC_RTCSUBSECINCCTL_UPD_REQ;
}

/*
 *  ======== calibrateRcoscHf1 ========
 *  Calibrate RCOSC_HF agains XOSC_HF: compute and setup new trims
 */
static Void calibrateRcoscHf1(Int32 tdcResult)
{
    /* read current trims */
    nCtrimCurr = (DDI32RegRead(AUX_DDI0_OSC_BASE, DDI_0_OSC_O_RCOSCHFCTL) &
        DDI_0_OSC_RCOSCHFCTL_RCOSCHF_CTRIM_M) >>
        DDI_0_OSC_RCOSCHFCTL_RCOSCHF_CTRIM_S;

    nCtrimFractCurr = (DDI32RegRead(AUX_DDI0_OSC_BASE, DDI_0_OSC_O_CTL1_LOCAL)
        & DDI_0_OSC_CTL1_RCOSCHFCTRIMFRACT_LOCAL_M) >>
        DDI_0_OSC_CTL1_RCOSCHFCTRIMFRACT_LOCAL_S;

    /*
     * Find RCOSC_HF-XOSC_HF frequency offset with current trim settings
     *   Positive value => RCOSC_HF runs slow, CTRIM(FRACT) should be increased
     *   Negative value => RCOSC_HF runs fast, CTRIM(FRACT) should be decreased
     * Resolution: 31.25 kHz; CTRIMFRACT resolution ~30 kHz
     */
    nDeltaFreqCurr = (Int32) tdcResult - 1536;

    /*
     * Calculate change to CTRIMFRACT with safe assumptions of gain,
     * apply delta to current CTRIMFRACT and convert to valid CTRIM/CTRIMFRACT
     */
    nCtrimFractNew = nCtrimFractCurr + scaleRndInf(nDeltaFreqCurr);
    nCtrimNew = nCtrimCurr;

    /* One step of CTRIM is about 500 kHz, so limit to one CTRIM step */
    if (nCtrimFractNew < 2)
    {
        /* Below sweet spot of current CTRIM => decrease CTRIM */
        nCtrimNew = MAX(-0x40, nCtrimNew-1);
        nCtrimFractNew = MAX(0, nCtrimFractNew+16);
    }
    else if (nCtrimFractNew > 29)
    {
        /* Above sweet spot of current CTRIM => increase CTRIM */
        nCtrimNew = MIN(0x3F, nCtrimNew+1);
        nCtrimFractNew = MIN(31, nCtrimFractNew-16);
    }
    else
    {
        /* We're within sweet spot of current CTRIM => no change */
    }

    /* Find RCOSC_HF vs XOSC_HF frequency offset with new trim settings */
    DDI16BitfieldWrite(AUX_DDI0_OSC_BASE, DDI_0_OSC_O_RCOSCHFCTL,
                           DDI_0_OSC_RCOSCHFCTL_RCOSCHF_CTRIM_M,
                           DDI_0_OSC_RCOSCHFCTL_RCOSCHF_CTRIM_S,
                           nCtrimNew);

    /* Enable RCOSCHFCTRIMFRACT_EN */
    DDI16BitfieldWrite(AUX_DDI0_OSC_BASE, DDI_0_OSC_O_CTL1_LOCAL,
                           DDI_0_OSC_CTL1_RCOSCHFCTRIMFRACT_EN_LOCAL_M,
                           DDI_0_OSC_CTL1_RCOSCHFCTRIMFRACT_EN_LOCAL_S,
                           1);

    /* Modify CTRIM_FRACT */
    DDI16BitfieldWrite(AUX_DDI0_OSC_BASE, DDI_0_OSC_O_CTL1_LOCAL,
                           DDI_0_OSC_CTL1_RCOSCHFCTRIMFRACT_LOCAL_M,
                           DDI_0_OSC_CTL1_RCOSCHFCTRIMFRACT_LOCAL_S,
                           nCtrimFractNew);
}

/*
 *  ======== calibrateRcoscHf2 ========
 *  Calibrate RCOSC_HF agains XOSC_HF: determine better result, set new trims
 */
static Void calibrateRcoscHf2(Int32 tdcResult)
{
    Int32 nDeltaFreqNew;

    /* Calculate new delta freq */
    nDeltaFreqNew = (Int32) tdcResult - 1536;

    /* Determine whether the new settings are better or worse */
    if (abs(nDeltaFreqNew) <= abs(nDeltaFreqCurr))
    {
        /* new settings are better or same, keep them in registers */
    }
    else
    {
        /*
         * Current setting were better, check whether we are getting fooled
         * by non-monotonicity in CTRIM
         */
        nCtrimFractNew += scaleRndInf(nDeltaFreqNew);
        if (nCtrimCurr != nCtrimNew && abs(nCtrimFractCurr - 16) >
            abs(nCtrimFractNew - 16)) {

            /*
             * New settings updated based on frequency measurements are more
             * centered in its CTRIM than the current settings in its
             * CTRIM -> use updated new settings
             */
            DDI16BitfieldWrite(AUX_DDI0_OSC_BASE, DDI_0_OSC_O_CTL1_LOCAL,
                           DDI_0_OSC_CTL1_RCOSCHFCTRIMFRACT_LOCAL_M,
                           DDI_0_OSC_CTL1_RCOSCHFCTRIMFRACT_LOCAL_S,
                           nCtrimFractNew);
        }
        else {
            /* New settings updated based on frequency measurements are less
             * centered in its CTRIM than the current settings in its CTRIM,
             * or they are both in the same CTRIM -> revert to old settings
             */
            DDI16BitfieldWrite(AUX_DDI0_OSC_BASE, DDI_0_OSC_O_RCOSCHFCTL,
                           DDI_0_OSC_RCOSCHFCTL_RCOSCHF_CTRIM_M,
                           DDI_0_OSC_RCOSCHFCTL_RCOSCHF_CTRIM_S,
                           nCtrimCurr);
            DDI16BitfieldWrite(AUX_DDI0_OSC_BASE, DDI_0_OSC_O_CTL1_LOCAL,
                           DDI_0_OSC_CTL1_RCOSCHFCTRIMFRACT_LOCAL_M,
                           DDI_0_OSC_CTL1_RCOSCHFCTRIMFRACT_LOCAL_S,
                           nCtrimFractCurr);
        }
    }
}

/*
 *  ======== abs ========
 *  Absolute value
 */
static Int abs(Int i)
{
    /* compute absolute value of int argument */
    return (i < 0 ? -i : i);
}

/*
 *  ======== scaleRndInf ========
 */
static Int32 scaleRndInf(Int32 x)
{
    return (2*x + ((x<0) ? -2 : 2))/3;
}

/*
 *  ======== cleanupAfterMeasurements ========
 */
static Void cleanupAfterMeasurements(Void)
{
    /* disable TDC counter clock source */
    HWREG(AUX_WUC_BASE + AUX_WUC_O_TDCCLKCTL) = 0;

    /* wait for AUX_WUC:TDCCLKCTL.ACK to be set... */
    while ((HWREG(AUX_WUC_BASE + AUX_WUC_O_TDCCLKCTL) &
        AUX_WUC_TDCCLKCTL_ACK));

    /* release the TDC clock request */
    HWREG(AUX_WUC_BASE + AUX_WUC_O_TDCCLKCTL) = 0;

    /* relese the TDC reference clock request */
    HWREG(AUX_WUC_BASE + AUX_WUC_O_REFCLKCTL) = 0;

    /* release AUX semaphore */
    HWREG(AUX_SMPH_BASE + AUX_SMPH_O_SMPH1) = 1;

    /* release the power down constraints and XOSC_HF dependency */
    Power_releaseDependency(XOSC_HF);
    Power_releaseConstraint(Power_IDLE_PD_DISALLOW);
    Power_releaseConstraint(Power_SB_DISALLOW);

#if INSTRUMENT
        calHF2 = 1;
#endif
}
