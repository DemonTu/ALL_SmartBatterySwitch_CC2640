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
 *     VIRTUAL FUNCTIONS
 *     FUNCTION STUBS
 *     FUNCTION SELECTORS
 *     
 *     EPILOGUE
 *     PREFIX ALIASES
 */


/*
 * ======== PROLOGUE ========
 */

#ifndef ti_uia_events_IUIAEvent__include
#define ti_uia_events_IUIAEvent__include

#ifndef __nested__
#define __nested__
#define ti_uia_events_IUIAEvent__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_uia_events_IUIAEvent___VERS 160


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/uia/events/package/package.defs.h>

#include <xdc/runtime/IModule.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct ti_uia_events_IUIAEvent_Fxns__ {
    xdc_runtime_Types_Base* __base;
    const xdc_runtime_Types_SysFxns2 *__sysp;
    xdc_runtime_Types_SysFxns2 __sfxns;
};

/* Interface__BASE__C */
__extern const xdc_runtime_Types_Base ti_uia_events_IUIAEvent_Interface__BASE__C;


/*
 * ======== FUNCTION STUBS ========
 */

/* Module_id */
static inline xdc_runtime_Types_ModuleId ti_uia_events_IUIAEvent_Module_id( ti_uia_events_IUIAEvent_Module mod )
{
    return mod->__sysp->__mid;
}


/*
 * ======== FUNCTION SELECTORS ========
 */


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_uia_events_IUIAEvent__top__
#undef __nested__
#endif

#endif /* ti_uia_events_IUIAEvent__include */


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_uia_events_IUIAEvent__nolocalnames)

#ifndef ti_uia_events_IUIAEvent__localnames__done
#define ti_uia_events_IUIAEvent__localnames__done

/* module prefix */
#define IUIAEvent_Module ti_uia_events_IUIAEvent_Module
#define IUIAEvent_Module_name ti_uia_events_IUIAEvent_Module_name

#endif /* ti_uia_events_IUIAEvent__localnames__done */
#endif
