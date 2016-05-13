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
 *  ======== Boot.xs ========
 */

var Boot = null;
var Program = null;

/*
 *  ======== module$meta$init ========
 */
function module$meta$init()
{
    /* Only process during "cfg" phase */
    if (xdc.om.$name != "cfg") {
        return;
    }
    Boot = this;

    /* Assign setters to the Clock configs. */
    var GetSet = xdc.module("xdc.services.getset.GetSet");

    GetSet.init(Boot);

    GetSet.onSet(this, "configureClock", updateFrequency);
    GetSet.onSet(this, "sysClockDiv", updateFrequency);
    GetSet.onSet(this, "xtal", updateFrequency);
    GetSet.onSet(this, "oscSrc", updateFrequency);
    GetSet.onSet(this, "pllBypass", updateFrequency);
    GetSet.onSet(this, "ioscDisable", updateFrequency);
    GetSet.onSet(this, "moscDisable", updateFrequency);
}

/*
 *  ======== module$use ========
 */
function module$use()
{
    Program = xdc.useModule('xdc.cfg.Program');

    /* only install Boot_init if using XDC runtime */
    if (Program.build.rtsName === null) {
    	return;
    }

    /* Install Boot_init as one of Reset functions */
    var Reset = xdc.useModule('xdc.runtime.Reset');
    Reset.fxns[Reset.fxns.length++]
        = '&ti_catalog_arm_cortexm3_lm3init_Boot_init';

    if (Boot.configureClock == false) {
    	return;
    }

    /* Compute ulConfig based on config settings */

    /* enable PLL Output */
    if (Boot.pllOutEnable) {
	Boot.ulConfig = 0x00001000;
    }

    Boot.ulConfig |= Boot.sysClockDiv;

    Boot.ulConfig |= Boot.pwmClockDiv;

    Boot.ulConfig |= Boot.xtal;

    Boot.ulConfig |= Boot.oscSrc;

    if (Boot.pllBypass) {
    	Boot.ulConfig |= 0x00000800;
    }

    if (Boot.ioscDisable) {
    	Boot.ulConfig |= 0x00000002;
    }

    if (Boot.moscDisable) {
    	Boot.ulConfig |= 0x00000001;
    }
}

/*
 *  ======== getEnumString ========
 *  Get the enum value string name, not 0, 1, 2 or 3, etc.  For an enumeration
 *  type property.
 *
 *  Example usage:
 *  if obj contains an enumeration type property "Enum enumProp"
 *
 *  view.enumString = getEnumString(obj.enumProp);
 *
 */
function getEnumString(enumProperty)
{
    /*
     *  Split the string into tokens in order to get rid of the huge package
     *  path that precedes the enum string name. Return the last 2 tokens
     *  concatenated with "_"
     */
    var enumStrArray = String(enumProperty).split(".");
    var len = enumStrArray.length;
    return (enumStrArray[len - 1]);
}

/*
 *  ======== viewInitModule ========
 *  Display the module properties in ROV
 */
function viewInitModule(view, obj)
{
    var Program = xdc.useModule('xdc.rov.Program');
    var Boot = xdc.useModule('ti.catalog.arm.cortexm3.lm3init.Boot');
    var modCfg = Program.getModuleConfig(Boot.$name);

    view.configureClock		= modCfg.configureClock;
    view.sysClockDivEnable	= (modCfg.sysClockDiv != Boot.SYSDIV_1);
    view.sysClockDiv		= getEnumString(modCfg.sysClockDiv);
    view.pwmClockDivEnable	= (modCfg.pwmClockDiv != Boot.PWMDIV_1);
    view.pwmClockDiv		= getEnumString(modCfg.pwmClockDiv);
    view.xtal			= getEnumString(modCfg.xtal);
    view.oscSrc			= getEnumString(modCfg.oscSrc);
    view.pllBypass		= modCfg.pllBypass;
    view.pllOutEnable		= modCfg.pllOutEnable;
    view.ioscDisable		= modCfg.ioscDisable;
    view.moscDisable		= modCfg.moscDisable;
    view.configureLdo		= modCfg.configureLdo;
    view.ldoOut			= getEnumString(modCfg.ldoOut);
}

function doSysDiv(freq)
{
    switch(Boot.sysClockDiv) {
        case Boot.SYSDIV_1:
	    freq /= 1;
	    break;

        case Boot.SYSDIV_2:
	    freq /= 2;
	    break;

        case Boot.SYSDIV_3:
	    freq /= 3;
	    break;

        case Boot.SYSDIV_4:
	    freq /= 4;
	    break;

        case Boot.SYSDIV_5:
	    freq /= 5;
	    break;

        case Boot.SYSDIV_6:
	    freq /= 6;
	    break;

        case Boot.SYSDIV_7:
	    freq /= 7;
	    break;

        case Boot.SYSDIV_8:
	    freq /= 8;
	    break;

        case Boot.SYSDIV_9:
	    freq /= 9;
	    break;

        case Boot.SYSDIV_10:
	    freq /= 10;
	    break;

        case Boot.SYSDIV_11:
	    freq /= 11;
	    break;

        case Boot.SYSDIV_12:
	    freq /= 12;
	    break;

        case Boot.SYSDIV_13:
	    freq /= 13;
	    break;

        case Boot.SYSDIV_14:
	    freq /= 14;
	    break;

        case Boot.SYSDIV_15:
	    freq /= 15;
	    break;

        case Boot.SYSDIV_16:
	    freq /= 16;
	    break;

        case Boot.SYSDIV_17:
	    freq /= 17;
	    break;

        case Boot.SYSDIV_18:
	    freq /= 18;
	    break;

        case Boot.SYSDIV_19:
	    freq /= 19;
	    break;

        case Boot.SYSDIV_20:
	    freq /= 20;
	    break;

        case Boot.SYSDIV_21:
	    freq /= 21;
	    break;

        case Boot.SYSDIV_22:
	    freq /= 22;
	    break;

        case Boot.SYSDIV_23:
	    freq /= 23;
	    break;

        case Boot.SYSDIV_24:
	    freq /= 24;
	    break;

        case Boot.SYSDIV_25:
	    freq /= 25;
	    break;

        case Boot.SYSDIV_26:
	    freq /= 26;
	    break;

        case Boot.SYSDIV_27:
	    freq /= 27;
	    break;

        case Boot.SYSDIV_28:
	    freq /= 28;
	    break;

        case Boot.SYSDIV_29:
	    freq /= 29;
	    break;

        case Boot.SYSDIV_30:
	    freq /= 30;
	    break;

        case Boot.SYSDIV_31:
	    freq /= 31;
	    break;

        case Boot.SYSDIV_32:
	    freq /= 32;
	    break;

        case Boot.SYSDIV_33:
	    freq /= 33;
	    break;

        case Boot.SYSDIV_34:
	    freq /= 34;
	    break;

        case Boot.SYSDIV_35:
	    freq /= 35;
	    break;

        case Boot.SYSDIV_36:
	    freq /= 36;
	    break;

        case Boot.SYSDIV_37:
	    freq /= 37;
	    break;

        case Boot.SYSDIV_38:
	    freq /= 38;
	    break;

        case Boot.SYSDIV_39:
	    freq /= 39;
	    break;

        case Boot.SYSDIV_40:
	    freq /= 40;
	    break;

        case Boot.SYSDIV_41:
	    freq /= 41;
	    break;

        case Boot.SYSDIV_42:
	    freq /= 42;
	    break;

        case Boot.SYSDIV_43:
	    freq /= 43;
	    break;

        case Boot.SYSDIV_44:
	    freq /= 44;
	    break;

        case Boot.SYSDIV_45:
	    freq /= 45;
	    break;

        case Boot.SYSDIV_46:
	    freq /= 46;
	    break;

        case Boot.SYSDIV_47:
	    freq /= 47;
	    break;

        case Boot.SYSDIV_48:
	    freq /= 48;
	    break;

        case Boot.SYSDIV_49:
	    freq /= 49;
	    break;

        case Boot.SYSDIV_50:
	    freq /= 50;
	    break;

        case Boot.SYSDIV_51:
	    freq /= 51;
	    break;

        case Boot.SYSDIV_52:
	    freq /= 52;
	    break;

        case Boot.SYSDIV_53:
	    freq /= 53;
	    break;

        case Boot.SYSDIV_54:
	    freq /= 54;
	    break;

        case Boot.SYSDIV_55:
	    freq /= 55;
	    break;

        case Boot.SYSDIV_56:
	    freq /= 56;
	    break;

        case Boot.SYSDIV_57:
	    freq /= 57;
	    break;

        case Boot.SYSDIV_58:
	    freq /= 58;
	    break;

        case Boot.SYSDIV_59:
	    freq /= 59;
	    break;

        case Boot.SYSDIV_60:
	    freq /= 60;
	    break;

        case Boot.SYSDIV_61:
	    freq /= 61;
	    break;

        case Boot.SYSDIV_62:
	    freq /= 62;
	    break;

        case Boot.SYSDIV_63:
	    freq /= 63;
	    break;

        case Boot.SYSDIV_64:
	    freq /= 64;
	    break;

        case Boot.SYSDIV_2_5:
	    freq /= 2.5;
	    break;

        case Boot.SYSDIV_3_5:
	    freq /= 3.5;
	    break;

        case Boot.SYSDIV_4_5:
	    freq /= 4.5;
	    break;

        case Boot.SYSDIV_5_5:
	    freq /= 5.5;
	    break;

        case Boot.SYSDIV_6_5:
	    freq /= 6.5;
	    break;

        case Boot.SYSDIV_7_5:
	    freq /= 7.5;
	    break;

        case Boot.SYSDIV_8_5:
	    freq /= 8.5;
	    break;

        case Boot.SYSDIV_9_5:
	    freq /= 9.5;
	    break;

        case Boot.SYSDIV_10_5:
	    freq /= 10.5;
	    break;

        case Boot.SYSDIV_11_5:
	    freq /= 11.5;
	    break;

        case Boot.SYSDIV_12_5:
	    freq /= 12.5;
	    break;

        case Boot.SYSDIV_13_5:
	    freq /= 13.5;
	    break;

        case Boot.SYSDIV_14_5:
	    freq /= 14.5;
	    break;

        case Boot.SYSDIV_15_5:
	    freq /= 15.5;
	    break;

        case Boot.SYSDIV_16_5:
	    freq /= 16.5;
	    break;

        case Boot.SYSDIV_17_5:
	    freq /= 17.5;
	    break;

        case Boot.SYSDIV_18_5:
	    freq /= 18.5;
	    break;

        case Boot.SYSDIV_19_5:
	    freq /= 19.5;
	    break;

        case Boot.SYSDIV_20_5:
	    freq /= 20.5;
	    break;

        case Boot.SYSDIV_21_5:
	    freq /= 21.5;
	    break;

        case Boot.SYSDIV_22_5:
	    freq /= 22.5;
	    break;

        case Boot.SYSDIV_23_5:
	    freq /= 23.5;
	    break;

        case Boot.SYSDIV_24_5:
	    freq /= 24.5;
	    break;

        case Boot.SYSDIV_25_5:
	    freq /= 25.5;
	    break;

        case Boot.SYSDIV_26_5:
	    freq /= 26.5;
	    break;

        case Boot.SYSDIV_27_5:
	    freq /= 27.5;
	    break;

        case Boot.SYSDIV_28_5:
	    freq /= 28.5;
	    break;

        case Boot.SYSDIV_29_5:
	    freq /= 29.5;
	    break;

        case Boot.SYSDIV_30_5:
	    freq /= 30.5;
	    break;

        case Boot.SYSDIV_31_5:
	    freq /= 31.5;
	    break;

        case Boot.SYSDIV_32_5:
	    freq /= 32.5;
	    break;

        case Boot.SYSDIV_33_5:
	    freq /= 33.5;
	    break;

        case Boot.SYSDIV_34_5:
	    freq /= 34.5;
	    break;

        case Boot.SYSDIV_35_5:
	    freq /= 35.5;
	    break;

        case Boot.SYSDIV_36_5:
	    freq /= 36.5;
	    break;

        case Boot.SYSDIV_37_5:
	    freq /= 37.5;
	    break;

        case Boot.SYSDIV_38_5:
	    freq /= 38.5;
	    break;

        case Boot.SYSDIV_39_5:
	    freq /= 39.5;
	    break;

        case Boot.SYSDIV_40_5:
	    freq /= 40.5;
	    break;

        case Boot.SYSDIV_41_5:
	    freq /= 41.5;
	    break;

        case Boot.SYSDIV_42_5:
	    freq /= 42.5;
	    break;

        case Boot.SYSDIV_43_5:
	    freq /= 43.5;
	    break;

        case Boot.SYSDIV_44_5:
	    freq /= 44.5;
	    break;

        case Boot.SYSDIV_45_5:
	    freq /= 45.5;
	    break;

        case Boot.SYSDIV_46_5:
	    freq /= 46.5;
	    break;

        case Boot.SYSDIV_47_5:
	    freq /= 47.5;
	    break;

        case Boot.SYSDIV_48_5:
	    freq /= 48.5;
	    break;

        case Boot.SYSDIV_49_5:
	    freq /= 49.5;
	    break;

        case Boot.SYSDIV_50_5:
	    freq /= 50.5;
	    break;

        case Boot.SYSDIV_51_5:
	    freq /= 51.5;
	    break;

        case Boot.SYSDIV_52_5:
	    freq /= 52.5;
	    break;

        case Boot.SYSDIV_53_5:
	    freq /= 53.5;
	    break;

        case Boot.SYSDIV_54_5:
	    freq /= 54.5;
	    break;

        case Boot.SYSDIV_55_5:
	    freq /= 55.5;
	    break;

        case Boot.SYSDIV_56_5:
	    freq /= 56.5;
	    break;

        case Boot.SYSDIV_57_5:
	    freq /= 57.5;
	    break;

        case Boot.SYSDIV_58_5:
	    freq /= 58.5;
	    break;

        case Boot.SYSDIV_59_5:
	    freq /= 59.5;
	    break;

        case Boot.SYSDIV_60_5:
	    freq /= 60.5;
	    break;

        case Boot.SYSDIV_61_5:
	    freq /= 61.5;
	    break;

        case Boot.SYSDIV_62_5:
	    freq /= 62.5;
	    break;

        case Boot.SYSDIV_63_5:
	    freq /= 63.5;
	    break;

    }

    return (freq);
}

/*
 *  ======== getXtalFrequency ========
 */
function getXtalFrequency()
{
   switch (Boot.xtal) {
	case Boot.XTAL_1MHZ:
	    return (1000000);

	case Boot.XTAL_1_84MHZ:
	    return (1843200);

	case Boot.XTAL_2MHZ:
	    return (2000000);

	case Boot.XTAL_2_45MHZ:
	    return (2457600);

	case Boot.XTAL_3_57MHZ:
	    return (3579450);

	case Boot.XTAL_3_68MHZ:
	    return (3686400);

	case Boot.XTAL_4MHZ:
	    return (4000000);

	case Boot.XTAL_4_09MHZ:
	    return (4096000);

	case Boot.XTAL_4_91MHZ:
	    return (4915200);

	case Boot.XTAL_5MHZ:
	    return (5000000);

	case Boot.XTAL_5_12MHZ:
	    return (5120000);

	case Boot.XTAL_6MHZ:
	    return (6000000);

	case Boot.XTAL_6_14MHZ:
	    return (6144000);

	case Boot.XTAL_7_37MHZ:
	    return (7372800);

	case Boot.XTAL_8MHZ:
	    return (8000000);

	case Boot.XTAL_8_19MHZ:
	    return (8192000);

	case Boot.XTAL_10MHZ:
	    return (10000000);

	case Boot.XTAL_12MHZ:
	    return (12000000);

	case Boot.XTAL_12_2MHZ:
	    return (12288000);

	case Boot.XTAL_13_5MHZ:
	    return (13560000);

	case Boot.XTAL_14_3MHZ:
	    return (14318180);

	case Boot.XTAL_16MHZ:
	    return (16000000);

	case Boot.XTAL_16_3MHZ:
	    return (16384000);
    }
}

/*
 *  ======== getFrequency ========
 */
function getFrequency()
{
    if (Boot.configureClock == false) {
    	/* no way to know */
	return (0);
    }

    /*
     * If the pll is being used (which is normal)
     * then it will always be configured to output 200MHz.
     * Then the System Clock is the 200MHz PLL output or
     * 200MHz divided down by some fixed amount (Boot.SYSDIV).
     */
    if (Boot.pllBypass == false) {
    	var freq = doSysDiv(200000000);
	return (freq);
    }

    switch (Boot.oscSrc) {
    	case Boot.OSCSRC_MAIN:
	    return (doSysDiv(getXtalFrequency()));

    	case Boot.OSCSRC_INT:
	    return (doSysDiv(15000000)); /* 15MHz */

    	case Boot.OSCSRC_INT4:
	    return (doSysDiv(3750000));  /* 3.75MHz */

    	case Boot.OSCSRC_30:
	    return (doSysDiv(30000));    /* 30KHz */
    }
}


/* Array of listeners, used by 'registerFreqListener' and 'updateFrequency'. */
var listeners = new Array();

/*
 *  ======== registerFreqListener ========
 *  Called by other modules (e.g., BIOS), to register themselves to listen
 *  for changes to the device frequency configured by the Boot module.
 */
function registerFreqListener(listener)
{
    listeners[listeners.length] = listener;
}

/*
 *  ======== updateFrequency ========
 *  Update all of the listeners whenever the PLL configuration changes
 *  (assuming configurePll is true).
 */
function updateFrequency(field, val)
{
    /* Don't fire the update if configurePll is false. */
    if (!Boot.configureClock) {
        return;
    }

    /* calculate the new frequency */
    var newFreq = getFrequency();

    /* for the Grace view to display */
    Boot.cpuFreq = newFreq;

    /* Notify each of the listeners of the new frequency value. */
    for each (var listener in listeners) {
        listener.fireFrequencyUpdate(newFreq);
    }
}
