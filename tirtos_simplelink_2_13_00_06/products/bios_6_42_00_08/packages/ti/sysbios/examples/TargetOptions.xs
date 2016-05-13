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
 *  ======== TargetOptions.xs ========
 */

/*
 * Target Options tree.
 *
 * Tree Structure:
 *
 * targetOptions
 * |
 * |---------------------------------------------|
 * |                                             |
 * TI                                           GCC           <- Toolchain
 * |                                             |
 * |------|------|-------|------|             |-----|
 * ARM  C2000  C6000   MSP430  EVE           ARM  MSP430      <- Family
 * |                                          |
 * |---|---|--- ...                      |----|----|--- ...
 * A8  A9  M3                           M3   M4   M4F         <- Variant
 *         |                                  |
 *      |--------|--- ...                |---------|-- ...    <- Devices
 *   Generic   CC26xx                 Generic   CC32xx
 * (Stellaris)                        (Tiva)
 *
 *
 * !!!MUST READ!!!
 *
 * The variant node defines default options that are inherited
 * by each device listed in the devices array. The device node
 * can override the default options by redefining them.
 *
 * The deviceId and deviceVariant options are special in that
 * one of them should be defined for a device. If the variant
 * node defines deviceId and the device node needs to override it
 * with a deviceVariant, the device node should set the deviceId
 * to "". Same is true if variant node defines deviceVariant by
 * default and the device node needs to use deviceId.
 *
 * Example:
 * ...
 *    "A8": {
 *        deviceId: ".*(AM335)*.",
 *        ...
 *        devices: {
 *            "AM35x": {
 *                deviceId: "",  <-- Change deviceId to "" so deviceVariant
 *                                   is used.
 *                deviceVariant: "CortexA8",
 *                ...
 *            },
 *        },
 *     },
 */
var targetOptions = {
    /* Toolchain */
    "TI": {
        /* Family */
        "ARM": {
            /* Variant */
            "GENERIC": {
                cfgPrefix: "default/",
                linkerCommandFile: "",
                compilerBuildOptions: "",
                linkerBuildOptions: "",
                productGroup: "",
                devices: {
                    "GENERIC": {
                        deviceId: "~.*(Cortex A|TM4C|TM4E|LM3|F28M3|LM4|" +
                            "RM4|TMS570LS|ARM7|ARM11|Generic|EVMDMRX45X" +
                            "|CC13|CC25|CM25|CC26|CC32|TMS470M|DM350|DM357|" +
                            "DM368|P401R).*",
                    }
                }
            },
            "A8": {
                deviceVariant: "CortexA8",
                cfgPrefix: "default/",
                linkerCommandFile: "",
                compilerBuildOptions: "",
                linkerBuildOptions: "",
                devices: {
                    "GENERIC": {
                        productGroup: "Sitara",
                    }
                }
            },
            "M3": {
                cfgPrefix: "cortexm/",
                compilerBuildOptions: "",
                linkerBuildOptions: "",
                devices: {
                    "STELLARIS": {
                        deviceId: ".*LM3.*",
                        platform: "ti.platforms.stellaris:$DeviceId$",
                        linkerCommandFile: "ti/platforms/stellaris/include/" +
                                           "$DeviceId$.cmd",
                        productGroup: "",
                    },
/* exclude CC26XX because it needs driverlib ...
                    "CC26XX": {
                        cfgPrefix: "cc26xx/",
                        deviceId: ".*(CC26|CC13).*",
                        platform: "ti.platforms.simplelink:$DeviceId$",
                        productGroup: "SimpleLink"
                    },
*/
                    "CONCERTO": {
                        deviceId: ".*F28M3.*",
                        platform: "ti.platforms.concertoM3:$DeviceId$",
                        linkerCommandFile: "ti/platforms/concertoM3/include/" +
                                           "$DeviceId$.cmd",
                        productGroup: "C2000"
                    }
                }
            },
            "M4": {
                cfgPrefix: "cortexm/",
                compilerBuildOptions: "",
                linkerBuildOptions: "",
                devices: {
                    "CC32XX": {
                        deviceId: ".*CC32.*",
                        platform: "ti.platforms.simplelink:$DeviceId$",
                        productGroup: "SimpleLink"
                    }
                }
            },
            "M4F": {
                cfgPrefix: "cortexm/",
                compilerBuildOptions: "",
                linkerBuildOptions: "",
                devices: {
                    "TIVA": {
                        deviceId: ".*(LM4|TM4C|TM4E|P401R).*",
                        platform: "ti.platforms.tiva:$DeviceId$",
                        linkerCommandFile: "ti/platforms/tiva/include/" +
                                           "$DeviceId$.cmd",
                        productGroup: "TivaC",
                    }
                }
            }
        },
        "MSP432": {
            "MSP432": {
                cfgPrefix: "cortexm/",
                compilerBuildOptions: "",
                linkerBuildOptions: "",
                productGroup: "MSP430",
                devices: {
                    "GENERIC": {
                        deviceId: ".*MSP432.*",
                        target: "ti.targets.arm.elf.M4F",
                        platform: "ti.platforms.msp432:$DeviceId$",
                        linkerCommandFile: "ti/platforms/msp432/include/" +
                                           "$DeviceId$.cmd",
                    }
                }
            }
        },
        "C2000": {
            /* Variant */
            "C28": {
                cfgPrefix: "c28/",
                compilerBuildOptions: "",
                linkerBuildOptions: "",
                productGroup: "C2000",
                devices: {
                    "GENERIC": {
                        deviceId: "~.*(F28M3|Generic|G00883).*",
                        platform: "ti.platforms.tms320x28:$DeviceId$",
                        linkerCommandFile: "ti/platforms/tms320x28/include/" +
                                           "$DeviceId$.cmd",
                    },
                    "CONCERTO": {
                        deviceId: ".*F28M3.*",
                        platform: "ti.platforms.concertoC28:$DeviceId$",
                        linkerCommandFile: "ti/platforms/concertoC28/include/" +
                                           "$DeviceId$.cmd"
                    }
                }
            }
        },
        "C6000": {
            /* Variant */
            "GENERIC": {
                cfgPrefix: "default/",
                linkerCommandFile: "",
                compilerBuildOptions: "",
                linkerBuildOptions: "",
                productGroup: "C6000",
                devices: {
                    "GENERIC": {
                        deviceId: "~.*(TMS320C670|TMS320C671|TMS320C672|" +
                                  "DSK6713|GenericC62xxDevice|" +
                                  "GenericC64xxDevice|GenericC67xxDevice|" +
                                  "GenericC67xPlusDevice|C641).*",
                    }
                }
            }
        },
        "EVE": {
            /* Variant */
            "ARP32": {
                deviceId: "",
                cfgPrefix: "default/",
                linkerCommandFile: "",
                compilerBuildOptions: "",
                linkerBuildOptions: "",
                productGroup: "",
                devices: {
                    "GENERIC": {
                    }
                }
            }
        },
        "MSP430": {
            /* Variant */
            "MSP430": {
                deviceId: "", /* Array populated dynamically by a function */
                cfgPrefix: "msp430/",
                compilerBuildOptions: "",
                linkerBuildOptions: "",
                productGroup: "MSP430",
                devices: {
                    "GENERIC": {
                        platform: "ti.platforms.msp430:$DeviceId$",
                    }
                }
            }
        }
    },
    /* Toolchain */
    "GNU": {
        /* Family */
        "ARM": {
            /* Variant */
            "A8": {
                deviceVariant: "CortexA8",
                cfgPrefix: "default/",
                target: "gnu.targets.arm.A8F",
                linkerCommandFile: "",
                compilerBuildOptions: "-mfloat-abi=hard",
                linkerBuildOptions: "-nostartfiles -static --gc-sections -lgcc"
                + " -lc -lm -lnosys -L${xdc_find:gnu/targets/"
                + "arm/libs/install-native/arm-none-eabi/lib/fpu:${ProjName}}",
                productGroup: "Sitara",
                devices: {
                    "GENERIC": {
                    },
                    "SEMIHOST": {
                        cfgPrefix: "cortexa_semihost/",
                        linkerBuildOptions: "-nostartfiles -static"
                        + " --gc-sections -lgcc -lc -lm -lrdimon"
                        + " -L${xdc_find:gnu/targets/arm/libs/"
                        + "install-native/arm-none-eabi/lib/fpu:${ProjName}}"
                    }
                }
            },
            "A9": {
                deviceVariant: "CortexA9",
                cfgPrefix: "default/",
                target: "gnu.targets.arm.A9F",
                linkerCommandFile: "",
                compilerBuildOptions: "-mfloat-abi=hard",
                linkerBuildOptions: "-nostartfiles -static --gc-sections -lgcc"
                + " -lc -lm -lnosys -L${xdc_find:gnu/targets/arm/"
                + "libs/install-native/arm-none-eabi/lib/fpu:${ProjName}}",
                productGroup: "Sitara",
                devices: {
                    "GENERIC": {
                    },
                    "SEMIHOST": {
                        cfgPrefix: "cortexa_semihost/",
                        linkerBuildOptions: "-nostartfiles -static"
                        + " --gc-sections -lgcc -lc -lm -lrdimon"
                        + " -L${xdc_find:gnu/targets/arm/libs/"
                        + "install-native/arm-none-eabi/lib/fpu:${ProjName}}"
                    }
                }
            },
            "A15": {
                deviceVariant: "CortexA15",
                cfgPrefix: "default/",
                target: "gnu.targets.arm.A15F",
                linkerCommandFile: "",
                compilerBuildOptions: "-mfloat-abi=hard",
                linkerBuildOptions: "-nostartfiles -static --gc-sections -lgcc"
                + " -lc -lm -lnosys -L${xdc_find:gnu/targets/arm/"
                + "libs/install-native/arm-none-eabi/lib/fpu:${ProjName}}",
                productGroup: "Sitara",
                devices: {
                    "GENERIC": {
                    },
                    "SEMIHOST": {
                        cfgPrefix: "cortexa_semihost/",
                        linkerBuildOptions: "-nostartfiles -static"
                        + " --gc-sections -lgcc -lc -lm -lrdimon"
                        + " -L${xdc_find:gnu/targets/arm/libs/"
                        + "install-native/arm-none-eabi/lib/fpu:${ProjName}}"
                    }
                }
            },
            "M3": {
                deviceId: ".*LM3.*",
                cfgPrefix: "cortexm/",
                target: "gnu.targets.arm.M3",
                platform: "ti.platforms.tiva:$DeviceId$",
                compilerBuildOptions: "-mthumb -march=armv7-m",
                linkerBuildOptions: "-nostartfiles -static --gc-sections -lgcc"
                + " -lc -lm -lnosys -L${xdc_find:gnu/targets/arm/"
                + "libs/install-native/arm-none-eabi/lib/armv7-m:${ProjName}}",
                productGroup: "",
                devices: {
                    "STELLARIS": {
                        linkerCommandFile: "ti/platforms/tiva/include_gnu/" +
                                           "$DeviceId$.lds",
                    },
                    "SEMIHOST": {
                        cfgPrefix: "cortexm_semihost/",
                        linkerCommandFile: "ti/platforms/tiva/include_gnu/" +
                                           "$DeviceId$.lds",
                        linkerBuildOptions: "-nostartfiles -static"
                        + " --gc-sections -lgcc -lc -lm -lrdimon "
                        + "-L${xdc_find:gnu/targets/arm/libs/"
                        + "install-native/arm-none-eabi/lib/armv7-m:${ProjName}}",
                    },
/* exclude CC26XX because it needs driverlib ...
                    "CC26XX": {
                        cfgPrefix: "cc26xx/",
                        deviceId: ".*(CC26|CC13).*",
                        linkerCommandFile: "ti/platforms/simplelink/include_gnu/" +
                                           "cc2650.lds",
                        platform: "ti.platforms.simplelink:$DeviceId$",
                        productGroup: "SimpleLink"
                    },
                    "CC26XX_SEMIHOST": {
                        cfgPrefix: "cc26xx_semihost/",
                        deviceId: ".*(CC26|CC13).*",
                        linkerCommandFile: "ti/platforms/simplelink/include_gnu/" +
                                           "cc2650.lds",
                        linkerBuildOptions: "-nostartfiles -static"
                        + " --gc-sections -lgcc -lc -lm -lrdimon "
                        + "-L${xdc_find:gnu/targets/arm/libs/"
                        + "install-native/arm-none-eabi/lib/armv7-m:${ProjName}}",
                    },
*/
                }
            },
            "M4": {
                cfgPrefix: "cortexm/",
                target: "gnu.targets.arm.M4",
                platform: "",
                compilerBuildOptions: "-mthumb -march=armv7e-m -mfloat-abi=soft",
                linkerBuildOptions: "-nostartfiles -static --gc-sections -lgcc"
                + " -lc -lm -lnosys -L${xdc_find:gnu/targets/arm/"
                + "libs/install-native/arm-none-eabi/lib/armv7e-m:${ProjName}}",
                productGroup: "",
                devices: {
                    "CC32XX": {
                        deviceId: ".*CC32.*",
                        linkerCommandFile: "ti/platforms/simplelink/include_gnu/" +
                                           "$DeviceId$.lds",
                        platform: "ti.platforms.simplelink:$DeviceId$",
                        productGroup: "SimpleLink"
                    }
                }
            },
            "M4F": {
                cfgPrefix: "cortexm/",
                target: "gnu.targets.arm.M4F",
                compilerBuildOptions: "-mthumb -march=armv7e-m -mfloat-abi=hard"
                + " -mfpu=fpv4-sp-d16",
                linkerBuildOptions: "-nostartfiles -static --gc-sections -lgcc"
                + " -lc -lm -lnosys -L${xdc_find:gnu/targets/arm/"
                + "libs/install-native/arm-none-eabi/lib/armv7e-m/fpu:${ProjName}}",
                productGroup: "TivaC",
                devices: {
                    "TIVA": {
                        deviceId: ".*(LM4|TM4C|TM4E|P401R).*",
                        platform: "ti.platforms.tiva:$DeviceId$",
                        linkerCommandFile: "ti/platforms/tiva/include_gnu/" +
                            "$DeviceId$.lds",
                    },
                    "SEMIHOST": {
                        deviceId: ".*(LM4|TM4C|TM4E|P401R).*",
                        platform: "ti.platforms.tiva:$DeviceId$",
                        linkerCommandFile: "ti/platforms/tiva/include_gnu/" +
                            "$DeviceId$.lds",
                        cfgPrefix: "cortexm_semihost/",
                        linkerBuildOptions: "-nostartfiles -static"
                        + " --gc-sections -lgcc -lc -lm -lrdimon"
                        + " -L${xdc_find:gnu/targets/arm/libs/install-native/"
                        + "arm-none-eabi/lib/armv7e-m/fpu:${ProjName}}",
                    }
                }
            },
        },
        "MSP432": {
	    "MSP432": {
                cfgPrefix: "cortexm/",
                target: "gnu.targets.arm.M4F",
                compilerBuildOptions: "-mthumb -march=armv7e-m -mfloat-abi=hard"
                + " -mfpu=fpv4-sp-d16",
                linkerBuildOptions: "-nostartfiles -static --gc-sections -lgcc"
                + " -lc -lm -lnosys -L${xdc_find:gnu/targets/arm/libs/"
                + "install-native/arm-none-eabi/lib/armv7e-m/fpu:${ProjName}}",
                productGroup: "MSP430",
                devices: {
                    "MSP432": {
                        deviceId: ".*MSP432.*",
                        target: "gnu.targets.arm.M4F",
                        platform: "ti.platforms.msp432:$DeviceId$",
                        linkerCommandFile: "ti/platforms/msp432/include_gnu/" +
                            "$DeviceId$.lds",
                    },
                    "MSP432_SEMIHOST": {
                        deviceId: ".*MSP432.*",
                        target: "gnu.targets.arm.M4F",
                        platform: "ti.platforms.msp432:$DeviceId$",
                        linkerCommandFile: "ti/platforms/msp432/include_gnu/" +
                            "$DeviceId$.lds",
                        cfgPrefix: "cortexm_semihost/",
                        linkerBuildOptions: "-nostartfiles -static"
                        + " --gc-sections -lgcc -lc -lm -lrdimon"
                        + " -L${xdc_find:gnu/targets/arm/libs/install-native/"
                        + "arm-none-eabi/lib/armv7e-m/fpu:${ProjName}}",
                    }
                }
            }
        }
    }
}

/*
 *  ======== populateDeviceIdMSP430 ========
 *  Generate a deviceId list for MSP430 based on the devices listed in
 *  msp430Settings.xs and populate the deviceId array in targetOptions.
 */
function populateDeviceIdMSP430()
{
    try {
        var msp430 =
	    xdc.loadCapsule("ti/sysbios/family/msp430/msp430Settings.xs");
    }
    catch (err) {
        return;
    }

    var devices = msp430.devices[0];

    for (var i = 1; i < msp430.devices.length; i++) {
        devices += "|" + msp430.devices[i];
    }

    targetOptions["TI"]["MSP430"]["MSP430"].deviceId = devices;
}
