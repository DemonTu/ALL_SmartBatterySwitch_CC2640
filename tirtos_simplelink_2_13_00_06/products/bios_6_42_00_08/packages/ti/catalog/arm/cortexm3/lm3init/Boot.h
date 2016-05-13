/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-A71
 */

/*
 * ======== GENERATED SECTIONS ========
 *     
 *     PROLOGUE
 *     INCLUDES
 *     
 *     FUNCTION DECLARATIONS
 *     
 *     EPILOGUE
 *     STATE STRUCTURES
 *     PREFIX ALIASES
 */


/*
 * ======== PROLOGUE ========
 */

#ifndef ti_catalog_arm_cortexm3_lm3init_Boot__include
#define ti_catalog_arm_cortexm3_lm3init_Boot__include

#ifndef __nested__
#define __nested__
#define ti_catalog_arm_cortexm3_lm3init_Boot__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_catalog_arm_cortexm3_lm3init_Boot___VERS 160


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <ti/catalog/arm/cortexm3/lm3init/package/package.defs.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* SysDiv */
enum ti_catalog_arm_cortexm3_lm3init_Boot_SysDiv {
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_1 = 0x07800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_2 = 0x00C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_3 = 0x01400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_4 = 0x01C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_5 = 0x02400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_6 = 0x02C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_7 = 0x03400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_8 = 0x03C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_9 = 0x04400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_10 = 0x04C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_11 = 0x05400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_12 = 0x05C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_13 = 0x06400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_14 = 0x06C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_15 = 0x07400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_16 = 0x07C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_17 = 0x88400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_18 = 0x88C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_19 = 0x89400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_20 = 0x89C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_21 = 0x8A400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_22 = 0x8AC00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_23 = 0x8B400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_24 = 0x8BC00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_25 = 0x8C400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_26 = 0x8CC00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_27 = 0x8D400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_28 = 0x8DC00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_29 = 0x8E400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_30 = 0x8EC00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_31 = 0x8F400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_32 = 0x8FC00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_33 = 0x90400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_34 = 0x90C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_35 = 0x91400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_36 = 0x91C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_37 = 0x92400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_38 = 0x92C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_39 = 0x93400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_40 = 0x93C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_41 = 0x94400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_42 = 0x94C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_43 = 0x95400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_44 = 0x95C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_45 = 0x96400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_46 = 0x96C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_47 = 0x97400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_48 = 0x97C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_49 = 0x98400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_50 = 0x98C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_51 = 0x99400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_52 = 0x99C00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_53 = 0x9A400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_54 = 0x9AC00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_55 = 0x9B400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_56 = 0x9BC00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_57 = 0x9C400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_58 = 0x9CC00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_59 = 0x9D400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_60 = 0x9DC00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_61 = 0x9E400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_62 = 0x9EC00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_63 = 0x9F400000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_64 = 0x9FC00000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_2_5 = 0xC1000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_3_5 = 0xC1800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_4_5 = 0xC2000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_5_5 = 0xC2800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_6_5 = 0xC3000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_7_5 = 0xC3800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_8_5 = 0xC4000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_9_5 = 0xC4800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_10_5 = 0xC5000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_11_5 = 0xC5800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_12_5 = 0xC6000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_13_5 = 0xC6800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_14_5 = 0xC7000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_15_5 = 0xC7800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_16_5 = 0xC8000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_17_5 = 0xC8800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_18_5 = 0xC9000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_19_5 = 0xC9800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_20_5 = 0xCA000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_21_5 = 0xCA800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_22_5 = 0xCB000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_23_5 = 0xCB800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_24_5 = 0xCC000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_25_5 = 0xCC800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_26_5 = 0xCD000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_27_5 = 0xCD800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_28_5 = 0xCE000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_29_5 = 0xCE800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_30_5 = 0xCF000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_31_5 = 0xCF800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_32_5 = 0xD0000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_33_5 = 0xD0800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_34_5 = 0xD1000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_35_5 = 0xD1800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_36_5 = 0xD2000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_37_5 = 0xD2800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_38_5 = 0xD3000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_39_5 = 0xD3800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_40_5 = 0xD4000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_41_5 = 0xD4800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_42_5 = 0xD5000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_43_5 = 0xD5800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_44_5 = 0xD6000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_45_5 = 0xD6800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_46_5 = 0xD7000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_47_5 = 0xD7800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_48_5 = 0xD8000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_49_5 = 0xD8800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_50_5 = 0xD9000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_51_5 = 0xD9800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_52_5 = 0xDA000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_53_5 = 0xDA800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_54_5 = 0xDB000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_55_5 = 0xDB800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_56_5 = 0xDC000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_57_5 = 0xDC800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_58_5 = 0xDD000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_59_5 = 0xDD800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_60_5 = 0xDE000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_61_5 = 0xDE800000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_62_5 = 0xDF000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_63_5 = 0xDF800000
};
typedef enum ti_catalog_arm_cortexm3_lm3init_Boot_SysDiv ti_catalog_arm_cortexm3_lm3init_Boot_SysDiv;

/* PwmDiv */
enum ti_catalog_arm_cortexm3_lm3init_Boot_PwmDiv {
    ti_catalog_arm_cortexm3_lm3init_Boot_PWMDIV_1 = 0x00000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_PWMDIV_2 = 0x00100000,
    ti_catalog_arm_cortexm3_lm3init_Boot_PWMDIV_4 = 0x00120000,
    ti_catalog_arm_cortexm3_lm3init_Boot_PWMDIV_8 = 0x00140000,
    ti_catalog_arm_cortexm3_lm3init_Boot_PWMDIV_16 = 0x00160000,
    ti_catalog_arm_cortexm3_lm3init_Boot_PWMDIV_32 = 0x00180000,
    ti_catalog_arm_cortexm3_lm3init_Boot_PWMDIV_64 = 0x001A0000
};
typedef enum ti_catalog_arm_cortexm3_lm3init_Boot_PwmDiv ti_catalog_arm_cortexm3_lm3init_Boot_PwmDiv;

/* XtalFreq */
enum ti_catalog_arm_cortexm3_lm3init_Boot_XtalFreq {
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_1MHZ = 0x00000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_1_84MHZ = 0x00000040,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_2MHZ = 0x00000080,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_2_45MHZ = 0x000000C0,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_3_57MHZ = 0x00000100,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_3_68MHZ = 0x00000140,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_4MHZ = 0x00000180,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_4_09MHZ = 0x000001C0,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_4_91MHZ = 0x00000200,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_5MHZ = 0x00000240,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_5_12MHZ = 0x00000280,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_6MHZ = 0x000002C0,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_6_14MHZ = 0x00000300,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_7_37MHZ = 0x00000340,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_8MHZ = 0x00000380,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_8_19MHZ = 0x000003C0,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_10MHZ = 0x00000400,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_12MHZ = 0x00000440,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_12_2MHZ = 0x00000480,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_13_5MHZ = 0x000004C0,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_14_3MHZ = 0x00000500,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_16MHZ = 0x00000540,
    ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_16_3MHZ = 0x00000580
};
typedef enum ti_catalog_arm_cortexm3_lm3init_Boot_XtalFreq ti_catalog_arm_cortexm3_lm3init_Boot_XtalFreq;

/* OscSrc */
enum ti_catalog_arm_cortexm3_lm3init_Boot_OscSrc {
    ti_catalog_arm_cortexm3_lm3init_Boot_OSCSRC_MAIN = 0x00000000,
    ti_catalog_arm_cortexm3_lm3init_Boot_OSCSRC_INT = 0x00000010,
    ti_catalog_arm_cortexm3_lm3init_Boot_OSCSRC_INT4 = 0x00000020,
    ti_catalog_arm_cortexm3_lm3init_Boot_OSCSRC_30 = 0x00000030
};
typedef enum ti_catalog_arm_cortexm3_lm3init_Boot_OscSrc ti_catalog_arm_cortexm3_lm3init_Boot_OscSrc;

/* LdoOut */
enum ti_catalog_arm_cortexm3_lm3init_Boot_LdoOut {
    ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_55V = 0x0000001F,
    ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_60V = 0x0000001E,
    ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_65V = 0x0000001D,
    ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_70V = 0x0000001C,
    ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_75V = 0x0000001B,
    ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_25V = 0x00000005,
    ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_30V = 0x00000004,
    ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_35V = 0x00000003,
    ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_40V = 0x00000002,
    ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_45V = 0x00000001,
    ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_50V = 0x00000000
};
typedef enum ti_catalog_arm_cortexm3_lm3init_Boot_LdoOut ti_catalog_arm_cortexm3_lm3init_Boot_LdoOut;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* getFrequency__E */
#define ti_catalog_arm_cortexm3_lm3init_Boot_getFrequency ti_catalog_arm_cortexm3_lm3init_Boot_getFrequency__E
xdc__CODESECT(ti_catalog_arm_cortexm3_lm3init_Boot_getFrequency__E, "ti_catalog_arm_cortexm3_lm3init_Boot_getFrequency")
__extern xdc_UInt32 ti_catalog_arm_cortexm3_lm3init_Boot_getFrequency__E( void );


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_catalog_arm_cortexm3_lm3init_Boot__top__
#undef __nested__
#endif

#endif /* ti_catalog_arm_cortexm3_lm3init_Boot__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_catalog_arm_cortexm3_lm3init_Boot__internalaccess))

#ifndef ti_catalog_arm_cortexm3_lm3init_Boot__include_state
#define ti_catalog_arm_cortexm3_lm3init_Boot__include_state


#endif /* ti_catalog_arm_cortexm3_lm3init_Boot__include_state */

#endif

/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_catalog_arm_cortexm3_lm3init_Boot__nolocalnames)

#ifndef ti_catalog_arm_cortexm3_lm3init_Boot__localnames__done
#define ti_catalog_arm_cortexm3_lm3init_Boot__localnames__done

/* module prefix */
#define Boot_SysDiv ti_catalog_arm_cortexm3_lm3init_Boot_SysDiv
#define Boot_PwmDiv ti_catalog_arm_cortexm3_lm3init_Boot_PwmDiv
#define Boot_XtalFreq ti_catalog_arm_cortexm3_lm3init_Boot_XtalFreq
#define Boot_OscSrc ti_catalog_arm_cortexm3_lm3init_Boot_OscSrc
#define Boot_LdoOut ti_catalog_arm_cortexm3_lm3init_Boot_LdoOut
#define Boot_SYSDIV_1 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_1
#define Boot_SYSDIV_2 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_2
#define Boot_SYSDIV_3 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_3
#define Boot_SYSDIV_4 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_4
#define Boot_SYSDIV_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_5
#define Boot_SYSDIV_6 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_6
#define Boot_SYSDIV_7 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_7
#define Boot_SYSDIV_8 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_8
#define Boot_SYSDIV_9 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_9
#define Boot_SYSDIV_10 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_10
#define Boot_SYSDIV_11 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_11
#define Boot_SYSDIV_12 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_12
#define Boot_SYSDIV_13 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_13
#define Boot_SYSDIV_14 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_14
#define Boot_SYSDIV_15 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_15
#define Boot_SYSDIV_16 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_16
#define Boot_SYSDIV_17 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_17
#define Boot_SYSDIV_18 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_18
#define Boot_SYSDIV_19 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_19
#define Boot_SYSDIV_20 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_20
#define Boot_SYSDIV_21 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_21
#define Boot_SYSDIV_22 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_22
#define Boot_SYSDIV_23 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_23
#define Boot_SYSDIV_24 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_24
#define Boot_SYSDIV_25 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_25
#define Boot_SYSDIV_26 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_26
#define Boot_SYSDIV_27 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_27
#define Boot_SYSDIV_28 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_28
#define Boot_SYSDIV_29 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_29
#define Boot_SYSDIV_30 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_30
#define Boot_SYSDIV_31 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_31
#define Boot_SYSDIV_32 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_32
#define Boot_SYSDIV_33 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_33
#define Boot_SYSDIV_34 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_34
#define Boot_SYSDIV_35 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_35
#define Boot_SYSDIV_36 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_36
#define Boot_SYSDIV_37 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_37
#define Boot_SYSDIV_38 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_38
#define Boot_SYSDIV_39 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_39
#define Boot_SYSDIV_40 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_40
#define Boot_SYSDIV_41 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_41
#define Boot_SYSDIV_42 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_42
#define Boot_SYSDIV_43 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_43
#define Boot_SYSDIV_44 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_44
#define Boot_SYSDIV_45 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_45
#define Boot_SYSDIV_46 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_46
#define Boot_SYSDIV_47 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_47
#define Boot_SYSDIV_48 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_48
#define Boot_SYSDIV_49 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_49
#define Boot_SYSDIV_50 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_50
#define Boot_SYSDIV_51 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_51
#define Boot_SYSDIV_52 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_52
#define Boot_SYSDIV_53 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_53
#define Boot_SYSDIV_54 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_54
#define Boot_SYSDIV_55 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_55
#define Boot_SYSDIV_56 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_56
#define Boot_SYSDIV_57 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_57
#define Boot_SYSDIV_58 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_58
#define Boot_SYSDIV_59 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_59
#define Boot_SYSDIV_60 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_60
#define Boot_SYSDIV_61 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_61
#define Boot_SYSDIV_62 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_62
#define Boot_SYSDIV_63 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_63
#define Boot_SYSDIV_64 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_64
#define Boot_SYSDIV_2_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_2_5
#define Boot_SYSDIV_3_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_3_5
#define Boot_SYSDIV_4_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_4_5
#define Boot_SYSDIV_5_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_5_5
#define Boot_SYSDIV_6_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_6_5
#define Boot_SYSDIV_7_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_7_5
#define Boot_SYSDIV_8_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_8_5
#define Boot_SYSDIV_9_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_9_5
#define Boot_SYSDIV_10_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_10_5
#define Boot_SYSDIV_11_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_11_5
#define Boot_SYSDIV_12_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_12_5
#define Boot_SYSDIV_13_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_13_5
#define Boot_SYSDIV_14_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_14_5
#define Boot_SYSDIV_15_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_15_5
#define Boot_SYSDIV_16_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_16_5
#define Boot_SYSDIV_17_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_17_5
#define Boot_SYSDIV_18_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_18_5
#define Boot_SYSDIV_19_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_19_5
#define Boot_SYSDIV_20_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_20_5
#define Boot_SYSDIV_21_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_21_5
#define Boot_SYSDIV_22_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_22_5
#define Boot_SYSDIV_23_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_23_5
#define Boot_SYSDIV_24_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_24_5
#define Boot_SYSDIV_25_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_25_5
#define Boot_SYSDIV_26_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_26_5
#define Boot_SYSDIV_27_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_27_5
#define Boot_SYSDIV_28_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_28_5
#define Boot_SYSDIV_29_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_29_5
#define Boot_SYSDIV_30_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_30_5
#define Boot_SYSDIV_31_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_31_5
#define Boot_SYSDIV_32_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_32_5
#define Boot_SYSDIV_33_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_33_5
#define Boot_SYSDIV_34_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_34_5
#define Boot_SYSDIV_35_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_35_5
#define Boot_SYSDIV_36_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_36_5
#define Boot_SYSDIV_37_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_37_5
#define Boot_SYSDIV_38_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_38_5
#define Boot_SYSDIV_39_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_39_5
#define Boot_SYSDIV_40_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_40_5
#define Boot_SYSDIV_41_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_41_5
#define Boot_SYSDIV_42_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_42_5
#define Boot_SYSDIV_43_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_43_5
#define Boot_SYSDIV_44_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_44_5
#define Boot_SYSDIV_45_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_45_5
#define Boot_SYSDIV_46_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_46_5
#define Boot_SYSDIV_47_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_47_5
#define Boot_SYSDIV_48_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_48_5
#define Boot_SYSDIV_49_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_49_5
#define Boot_SYSDIV_50_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_50_5
#define Boot_SYSDIV_51_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_51_5
#define Boot_SYSDIV_52_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_52_5
#define Boot_SYSDIV_53_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_53_5
#define Boot_SYSDIV_54_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_54_5
#define Boot_SYSDIV_55_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_55_5
#define Boot_SYSDIV_56_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_56_5
#define Boot_SYSDIV_57_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_57_5
#define Boot_SYSDIV_58_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_58_5
#define Boot_SYSDIV_59_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_59_5
#define Boot_SYSDIV_60_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_60_5
#define Boot_SYSDIV_61_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_61_5
#define Boot_SYSDIV_62_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_62_5
#define Boot_SYSDIV_63_5 ti_catalog_arm_cortexm3_lm3init_Boot_SYSDIV_63_5
#define Boot_PWMDIV_1 ti_catalog_arm_cortexm3_lm3init_Boot_PWMDIV_1
#define Boot_PWMDIV_2 ti_catalog_arm_cortexm3_lm3init_Boot_PWMDIV_2
#define Boot_PWMDIV_4 ti_catalog_arm_cortexm3_lm3init_Boot_PWMDIV_4
#define Boot_PWMDIV_8 ti_catalog_arm_cortexm3_lm3init_Boot_PWMDIV_8
#define Boot_PWMDIV_16 ti_catalog_arm_cortexm3_lm3init_Boot_PWMDIV_16
#define Boot_PWMDIV_32 ti_catalog_arm_cortexm3_lm3init_Boot_PWMDIV_32
#define Boot_PWMDIV_64 ti_catalog_arm_cortexm3_lm3init_Boot_PWMDIV_64
#define Boot_XTAL_1MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_1MHZ
#define Boot_XTAL_1_84MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_1_84MHZ
#define Boot_XTAL_2MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_2MHZ
#define Boot_XTAL_2_45MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_2_45MHZ
#define Boot_XTAL_3_57MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_3_57MHZ
#define Boot_XTAL_3_68MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_3_68MHZ
#define Boot_XTAL_4MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_4MHZ
#define Boot_XTAL_4_09MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_4_09MHZ
#define Boot_XTAL_4_91MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_4_91MHZ
#define Boot_XTAL_5MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_5MHZ
#define Boot_XTAL_5_12MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_5_12MHZ
#define Boot_XTAL_6MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_6MHZ
#define Boot_XTAL_6_14MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_6_14MHZ
#define Boot_XTAL_7_37MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_7_37MHZ
#define Boot_XTAL_8MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_8MHZ
#define Boot_XTAL_8_19MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_8_19MHZ
#define Boot_XTAL_10MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_10MHZ
#define Boot_XTAL_12MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_12MHZ
#define Boot_XTAL_12_2MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_12_2MHZ
#define Boot_XTAL_13_5MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_13_5MHZ
#define Boot_XTAL_14_3MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_14_3MHZ
#define Boot_XTAL_16MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_16MHZ
#define Boot_XTAL_16_3MHZ ti_catalog_arm_cortexm3_lm3init_Boot_XTAL_16_3MHZ
#define Boot_OSCSRC_MAIN ti_catalog_arm_cortexm3_lm3init_Boot_OSCSRC_MAIN
#define Boot_OSCSRC_INT ti_catalog_arm_cortexm3_lm3init_Boot_OSCSRC_INT
#define Boot_OSCSRC_INT4 ti_catalog_arm_cortexm3_lm3init_Boot_OSCSRC_INT4
#define Boot_OSCSRC_30 ti_catalog_arm_cortexm3_lm3init_Boot_OSCSRC_30
#define Boot_LDOPCTL_2_55V ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_55V
#define Boot_LDOPCTL_2_60V ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_60V
#define Boot_LDOPCTL_2_65V ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_65V
#define Boot_LDOPCTL_2_70V ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_70V
#define Boot_LDOPCTL_2_75V ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_75V
#define Boot_LDOPCTL_2_25V ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_25V
#define Boot_LDOPCTL_2_30V ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_30V
#define Boot_LDOPCTL_2_35V ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_35V
#define Boot_LDOPCTL_2_40V ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_40V
#define Boot_LDOPCTL_2_45V ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_45V
#define Boot_LDOPCTL_2_50V ti_catalog_arm_cortexm3_lm3init_Boot_LDOPCTL_2_50V
#define Boot_configureClock ti_catalog_arm_cortexm3_lm3init_Boot_configureClock
#define Boot_sysClockDiv ti_catalog_arm_cortexm3_lm3init_Boot_sysClockDiv
#define Boot_pwmClockDiv ti_catalog_arm_cortexm3_lm3init_Boot_pwmClockDiv
#define Boot_xtal ti_catalog_arm_cortexm3_lm3init_Boot_xtal
#define Boot_oscSrc ti_catalog_arm_cortexm3_lm3init_Boot_oscSrc
#define Boot_pllBypass ti_catalog_arm_cortexm3_lm3init_Boot_pllBypass
#define Boot_pllOutEnable ti_catalog_arm_cortexm3_lm3init_Boot_pllOutEnable
#define Boot_ioscDisable ti_catalog_arm_cortexm3_lm3init_Boot_ioscDisable
#define Boot_moscDisable ti_catalog_arm_cortexm3_lm3init_Boot_moscDisable
#define Boot_configureLdo ti_catalog_arm_cortexm3_lm3init_Boot_configureLdo
#define Boot_ldoOut ti_catalog_arm_cortexm3_lm3init_Boot_ldoOut
#define Boot_ulConfig ti_catalog_arm_cortexm3_lm3init_Boot_ulConfig
#define Boot_cpuFreq ti_catalog_arm_cortexm3_lm3init_Boot_cpuFreq
#define Boot_getFrequency ti_catalog_arm_cortexm3_lm3init_Boot_getFrequency

#endif /* ti_catalog_arm_cortexm3_lm3init_Boot__localnames__done */
#endif
