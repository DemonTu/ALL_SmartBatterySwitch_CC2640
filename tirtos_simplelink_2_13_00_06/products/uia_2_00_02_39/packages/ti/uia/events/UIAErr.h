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
 *     INTERNAL DEFINITIONS
 *     MODULE-WIDE CONFIGS
 *     VIRTUAL FUNCTIONS
 *     FUNCTION DECLARATIONS
 *     CONVERTORS
 *     SYSTEM FUNCTIONS
 *     
 *     EPILOGUE
 *     STATE STRUCTURES
 *     PREFIX ALIASES
 */


/*
 * ======== PROLOGUE ========
 */

#ifndef ti_uia_events_UIAErr__include
#define ti_uia_events_UIAErr__include

#ifndef __nested__
#define __nested__
#define ti_uia_events_UIAErr__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_uia_events_UIAErr___VERS 160


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/uia/events/package/package.defs.h>

#include <xdc/runtime/Diags.h>
#include <ti/uia/events/IUIAEvent.h>
#include <xdc/runtime/Log.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */


/*
 * ======== INTERNAL DEFINITIONS ========
 */


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_uia_events_UIAErr_Module__diagsEnabled;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Module__diagsEnabled ti_uia_events_UIAErr_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_uia_events_UIAErr_Module__diagsIncluded;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Module__diagsIncluded ti_uia_events_UIAErr_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16 *CT__ti_uia_events_UIAErr_Module__diagsMask;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Module__diagsMask ti_uia_events_UIAErr_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_uia_events_UIAErr_Module__gateObj;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Module__gateObj ti_uia_events_UIAErr_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_uia_events_UIAErr_Module__gatePrms;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Module__gatePrms ti_uia_events_UIAErr_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_uia_events_UIAErr_Module__id;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Module__id ti_uia_events_UIAErr_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_uia_events_UIAErr_Module__loggerDefined;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Module__loggerDefined ti_uia_events_UIAErr_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_uia_events_UIAErr_Module__loggerObj;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Module__loggerObj ti_uia_events_UIAErr_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_uia_events_UIAErr_Module__loggerFxn0;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Module__loggerFxn0 ti_uia_events_UIAErr_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_uia_events_UIAErr_Module__loggerFxn1;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Module__loggerFxn1 ti_uia_events_UIAErr_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_uia_events_UIAErr_Module__loggerFxn2;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Module__loggerFxn2 ti_uia_events_UIAErr_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_uia_events_UIAErr_Module__loggerFxn4;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Module__loggerFxn4 ti_uia_events_UIAErr_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_uia_events_UIAErr_Module__loggerFxn8;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Module__loggerFxn8 ti_uia_events_UIAErr_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_uia_events_UIAErr_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_uia_events_UIAErr_Module__startupDoneFxn ti_uia_events_UIAErr_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_uia_events_UIAErr_Object__count;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Object__count ti_uia_events_UIAErr_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_uia_events_UIAErr_Object__heap;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Object__heap ti_uia_events_UIAErr_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_uia_events_UIAErr_Object__sizeof;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Object__sizeof ti_uia_events_UIAErr_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_uia_events_UIAErr_Object__table;
__extern __FAR__ const CT__ti_uia_events_UIAErr_Object__table ti_uia_events_UIAErr_Object__table__C;

/* error */
#define ti_uia_events_UIAErr_error (ti_uia_events_UIAErr_error__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_error;
__extern __FAR__ const CT__ti_uia_events_UIAErr_error ti_uia_events_UIAErr_error__C;

/* errorWithStr */
#define ti_uia_events_UIAErr_errorWithStr (ti_uia_events_UIAErr_errorWithStr__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_errorWithStr;
__extern __FAR__ const CT__ti_uia_events_UIAErr_errorWithStr ti_uia_events_UIAErr_errorWithStr__C;

/* hwError */
#define ti_uia_events_UIAErr_hwError (ti_uia_events_UIAErr_hwError__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_hwError;
__extern __FAR__ const CT__ti_uia_events_UIAErr_hwError ti_uia_events_UIAErr_hwError__C;

/* hwErrorWithStr */
#define ti_uia_events_UIAErr_hwErrorWithStr (ti_uia_events_UIAErr_hwErrorWithStr__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_hwErrorWithStr;
__extern __FAR__ const CT__ti_uia_events_UIAErr_hwErrorWithStr ti_uia_events_UIAErr_hwErrorWithStr__C;

/* fatal */
#define ti_uia_events_UIAErr_fatal (ti_uia_events_UIAErr_fatal__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_fatal;
__extern __FAR__ const CT__ti_uia_events_UIAErr_fatal ti_uia_events_UIAErr_fatal__C;

/* fatalWithStr */
#define ti_uia_events_UIAErr_fatalWithStr (ti_uia_events_UIAErr_fatalWithStr__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_fatalWithStr;
__extern __FAR__ const CT__ti_uia_events_UIAErr_fatalWithStr ti_uia_events_UIAErr_fatalWithStr__C;

/* critical */
#define ti_uia_events_UIAErr_critical (ti_uia_events_UIAErr_critical__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_critical;
__extern __FAR__ const CT__ti_uia_events_UIAErr_critical ti_uia_events_UIAErr_critical__C;

/* criticalWithStr */
#define ti_uia_events_UIAErr_criticalWithStr (ti_uia_events_UIAErr_criticalWithStr__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_criticalWithStr;
__extern __FAR__ const CT__ti_uia_events_UIAErr_criticalWithStr ti_uia_events_UIAErr_criticalWithStr__C;

/* exception */
#define ti_uia_events_UIAErr_exception (ti_uia_events_UIAErr_exception__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_exception;
__extern __FAR__ const CT__ti_uia_events_UIAErr_exception ti_uia_events_UIAErr_exception__C;

/* uncaughtException */
#define ti_uia_events_UIAErr_uncaughtException (ti_uia_events_UIAErr_uncaughtException__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_uncaughtException;
__extern __FAR__ const CT__ti_uia_events_UIAErr_uncaughtException ti_uia_events_UIAErr_uncaughtException__C;

/* nullPointerException */
#define ti_uia_events_UIAErr_nullPointerException (ti_uia_events_UIAErr_nullPointerException__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_nullPointerException;
__extern __FAR__ const CT__ti_uia_events_UIAErr_nullPointerException ti_uia_events_UIAErr_nullPointerException__C;

/* unexpectedInterrupt */
#define ti_uia_events_UIAErr_unexpectedInterrupt (ti_uia_events_UIAErr_unexpectedInterrupt__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_unexpectedInterrupt;
__extern __FAR__ const CT__ti_uia_events_UIAErr_unexpectedInterrupt ti_uia_events_UIAErr_unexpectedInterrupt__C;

/* memoryAccessFault */
#define ti_uia_events_UIAErr_memoryAccessFault (ti_uia_events_UIAErr_memoryAccessFault__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_memoryAccessFault;
__extern __FAR__ const CT__ti_uia_events_UIAErr_memoryAccessFault ti_uia_events_UIAErr_memoryAccessFault__C;

/* securityException */
#define ti_uia_events_UIAErr_securityException (ti_uia_events_UIAErr_securityException__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_securityException;
__extern __FAR__ const CT__ti_uia_events_UIAErr_securityException ti_uia_events_UIAErr_securityException__C;

/* divisionByZero */
#define ti_uia_events_UIAErr_divisionByZero (ti_uia_events_UIAErr_divisionByZero__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_divisionByZero;
__extern __FAR__ const CT__ti_uia_events_UIAErr_divisionByZero ti_uia_events_UIAErr_divisionByZero__C;

/* overflowException */
#define ti_uia_events_UIAErr_overflowException (ti_uia_events_UIAErr_overflowException__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_overflowException;
__extern __FAR__ const CT__ti_uia_events_UIAErr_overflowException ti_uia_events_UIAErr_overflowException__C;

/* indexOutOfRange */
#define ti_uia_events_UIAErr_indexOutOfRange (ti_uia_events_UIAErr_indexOutOfRange__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_indexOutOfRange;
__extern __FAR__ const CT__ti_uia_events_UIAErr_indexOutOfRange ti_uia_events_UIAErr_indexOutOfRange__C;

/* notImplemented */
#define ti_uia_events_UIAErr_notImplemented (ti_uia_events_UIAErr_notImplemented__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_notImplemented;
__extern __FAR__ const CT__ti_uia_events_UIAErr_notImplemented ti_uia_events_UIAErr_notImplemented__C;

/* stackOverflow */
#define ti_uia_events_UIAErr_stackOverflow (ti_uia_events_UIAErr_stackOverflow__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_stackOverflow;
__extern __FAR__ const CT__ti_uia_events_UIAErr_stackOverflow ti_uia_events_UIAErr_stackOverflow__C;

/* illegalInstruction */
#define ti_uia_events_UIAErr_illegalInstruction (ti_uia_events_UIAErr_illegalInstruction__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_illegalInstruction;
__extern __FAR__ const CT__ti_uia_events_UIAErr_illegalInstruction ti_uia_events_UIAErr_illegalInstruction__C;

/* entryPointNotFound */
#define ti_uia_events_UIAErr_entryPointNotFound (ti_uia_events_UIAErr_entryPointNotFound__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_entryPointNotFound;
__extern __FAR__ const CT__ti_uia_events_UIAErr_entryPointNotFound ti_uia_events_UIAErr_entryPointNotFound__C;

/* moduleNotFound */
#define ti_uia_events_UIAErr_moduleNotFound (ti_uia_events_UIAErr_moduleNotFound__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_moduleNotFound;
__extern __FAR__ const CT__ti_uia_events_UIAErr_moduleNotFound ti_uia_events_UIAErr_moduleNotFound__C;

/* floatingPointError */
#define ti_uia_events_UIAErr_floatingPointError (ti_uia_events_UIAErr_floatingPointError__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_floatingPointError;
__extern __FAR__ const CT__ti_uia_events_UIAErr_floatingPointError ti_uia_events_UIAErr_floatingPointError__C;

/* invalidParameter */
#define ti_uia_events_UIAErr_invalidParameter (ti_uia_events_UIAErr_invalidParameter__C)
typedef xdc_runtime_Log_Event CT__ti_uia_events_UIAErr_invalidParameter;
__extern __FAR__ const CT__ti_uia_events_UIAErr_invalidParameter ti_uia_events_UIAErr_invalidParameter__C;


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct ti_uia_events_UIAErr_Fxns__ {
    xdc_runtime_Types_Base* __base;
    const xdc_runtime_Types_SysFxns2 *__sysp;
    xdc_runtime_Types_SysFxns2 __sfxns;
};

/* Module__FXNS__C */
__extern const ti_uia_events_UIAErr_Fxns__ ti_uia_events_UIAErr_Module__FXNS__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_uia_events_UIAErr_Module_startup( state ) (-1)

/* Module__startupDone__S */
xdc__CODESECT(ti_uia_events_UIAErr_Module__startupDone__S, "ti_uia_events_UIAErr_Module__startupDone__S")
__extern xdc_Bool ti_uia_events_UIAErr_Module__startupDone__S( void );


/*
 * ======== CONVERTORS ========
 */

/* Module_upCast */
static inline ti_uia_events_IUIAEvent_Module ti_uia_events_UIAErr_Module_upCast( void )
{
    return (ti_uia_events_IUIAEvent_Module)&ti_uia_events_UIAErr_Module__FXNS__C;
}

/* Module_to_ti_uia_events_IUIAEvent */
#define ti_uia_events_UIAErr_Module_to_ti_uia_events_IUIAEvent ti_uia_events_UIAErr_Module_upCast


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_uia_events_UIAErr_Module_startupDone() ti_uia_events_UIAErr_Module__startupDone__S()

/* Object_heap */
#define ti_uia_events_UIAErr_Object_heap() ti_uia_events_UIAErr_Object__heap__C

/* Module_heap */
#define ti_uia_events_UIAErr_Module_heap() ti_uia_events_UIAErr_Object__heap__C

/* Module_id */
static inline CT__ti_uia_events_UIAErr_Module__id ti_uia_events_UIAErr_Module_id( void ) 
{
    return ti_uia_events_UIAErr_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_uia_events_UIAErr_Module_hasMask( void ) 
{
    return ti_uia_events_UIAErr_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_uia_events_UIAErr_Module_getMask( void ) 
{
    return ti_uia_events_UIAErr_Module__diagsMask__C != NULL ? *ti_uia_events_UIAErr_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_uia_events_UIAErr_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_uia_events_UIAErr_Module__diagsMask__C != NULL) *ti_uia_events_UIAErr_Module__diagsMask__C = mask;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_uia_events_UIAErr__top__
#undef __nested__
#endif

#endif /* ti_uia_events_UIAErr__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_uia_events_UIAErr__internalaccess))

#ifndef ti_uia_events_UIAErr__include_state
#define ti_uia_events_UIAErr__include_state


#endif /* ti_uia_events_UIAErr__include_state */

#endif

/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_uia_events_UIAErr__nolocalnames)

#ifndef ti_uia_events_UIAErr__localnames__done
#define ti_uia_events_UIAErr__localnames__done

/* module prefix */
#define UIAErr_error ti_uia_events_UIAErr_error
#define UIAErr_errorWithStr ti_uia_events_UIAErr_errorWithStr
#define UIAErr_hwError ti_uia_events_UIAErr_hwError
#define UIAErr_hwErrorWithStr ti_uia_events_UIAErr_hwErrorWithStr
#define UIAErr_fatal ti_uia_events_UIAErr_fatal
#define UIAErr_fatalWithStr ti_uia_events_UIAErr_fatalWithStr
#define UIAErr_critical ti_uia_events_UIAErr_critical
#define UIAErr_criticalWithStr ti_uia_events_UIAErr_criticalWithStr
#define UIAErr_exception ti_uia_events_UIAErr_exception
#define UIAErr_uncaughtException ti_uia_events_UIAErr_uncaughtException
#define UIAErr_nullPointerException ti_uia_events_UIAErr_nullPointerException
#define UIAErr_unexpectedInterrupt ti_uia_events_UIAErr_unexpectedInterrupt
#define UIAErr_memoryAccessFault ti_uia_events_UIAErr_memoryAccessFault
#define UIAErr_securityException ti_uia_events_UIAErr_securityException
#define UIAErr_divisionByZero ti_uia_events_UIAErr_divisionByZero
#define UIAErr_overflowException ti_uia_events_UIAErr_overflowException
#define UIAErr_indexOutOfRange ti_uia_events_UIAErr_indexOutOfRange
#define UIAErr_notImplemented ti_uia_events_UIAErr_notImplemented
#define UIAErr_stackOverflow ti_uia_events_UIAErr_stackOverflow
#define UIAErr_illegalInstruction ti_uia_events_UIAErr_illegalInstruction
#define UIAErr_entryPointNotFound ti_uia_events_UIAErr_entryPointNotFound
#define UIAErr_moduleNotFound ti_uia_events_UIAErr_moduleNotFound
#define UIAErr_floatingPointError ti_uia_events_UIAErr_floatingPointError
#define UIAErr_invalidParameter ti_uia_events_UIAErr_invalidParameter
#define UIAErr_Module_name ti_uia_events_UIAErr_Module_name
#define UIAErr_Module_id ti_uia_events_UIAErr_Module_id
#define UIAErr_Module_startup ti_uia_events_UIAErr_Module_startup
#define UIAErr_Module_startupDone ti_uia_events_UIAErr_Module_startupDone
#define UIAErr_Module_hasMask ti_uia_events_UIAErr_Module_hasMask
#define UIAErr_Module_getMask ti_uia_events_UIAErr_Module_getMask
#define UIAErr_Module_setMask ti_uia_events_UIAErr_Module_setMask
#define UIAErr_Object_heap ti_uia_events_UIAErr_Object_heap
#define UIAErr_Module_heap ti_uia_events_UIAErr_Module_heap
#define UIAErr_Module_upCast ti_uia_events_UIAErr_Module_upCast
#define UIAErr_Module_to_ti_uia_events_IUIAEvent ti_uia_events_UIAErr_Module_to_ti_uia_events_IUIAEvent

#endif /* ti_uia_events_UIAErr__localnames__done */
#endif
