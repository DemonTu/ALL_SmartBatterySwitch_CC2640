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

#ifndef ti_uia_sysbios_Adaptor__include
#define ti_uia_sysbios_Adaptor__include

#ifndef __nested__
#define __nested__
#define ti_uia_sysbios_Adaptor__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_uia_sysbios_Adaptor___VERS 160


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/uia/sysbios/package/package.defs.h>

#include <xdc/runtime/Assert.h>
#include <ti/uia/runtime/UIAPacket.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/syncs/SyncEvent.h>
#include <ti/uia/runtime/IServiceMgrSupport.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */


/*
 * ======== INTERNAL DEFINITIONS ========
 */

/* Entry */
struct ti_uia_sysbios_Adaptor_Entry {
    ti_sysbios_knl_Queue_Elem elem;
    ti_uia_runtime_UIAPacket_Hdr packet;
};


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_uia_sysbios_Adaptor_Module__diagsEnabled;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Module__diagsEnabled ti_uia_sysbios_Adaptor_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_uia_sysbios_Adaptor_Module__diagsIncluded;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Module__diagsIncluded ti_uia_sysbios_Adaptor_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16 *CT__ti_uia_sysbios_Adaptor_Module__diagsMask;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Module__diagsMask ti_uia_sysbios_Adaptor_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_uia_sysbios_Adaptor_Module__gateObj;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Module__gateObj ti_uia_sysbios_Adaptor_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_uia_sysbios_Adaptor_Module__gatePrms;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Module__gatePrms ti_uia_sysbios_Adaptor_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_uia_sysbios_Adaptor_Module__id;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Module__id ti_uia_sysbios_Adaptor_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_uia_sysbios_Adaptor_Module__loggerDefined;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Module__loggerDefined ti_uia_sysbios_Adaptor_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_uia_sysbios_Adaptor_Module__loggerObj;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Module__loggerObj ti_uia_sysbios_Adaptor_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_uia_sysbios_Adaptor_Module__loggerFxn0;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Module__loggerFxn0 ti_uia_sysbios_Adaptor_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_uia_sysbios_Adaptor_Module__loggerFxn1;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Module__loggerFxn1 ti_uia_sysbios_Adaptor_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_uia_sysbios_Adaptor_Module__loggerFxn2;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Module__loggerFxn2 ti_uia_sysbios_Adaptor_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_uia_sysbios_Adaptor_Module__loggerFxn4;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Module__loggerFxn4 ti_uia_sysbios_Adaptor_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_uia_sysbios_Adaptor_Module__loggerFxn8;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Module__loggerFxn8 ti_uia_sysbios_Adaptor_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_uia_sysbios_Adaptor_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Module__startupDoneFxn ti_uia_sysbios_Adaptor_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_uia_sysbios_Adaptor_Object__count;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Object__count ti_uia_sysbios_Adaptor_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_uia_sysbios_Adaptor_Object__heap;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Object__heap ti_uia_sysbios_Adaptor_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_uia_sysbios_Adaptor_Object__sizeof;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Object__sizeof ti_uia_sysbios_Adaptor_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_uia_sysbios_Adaptor_Object__table;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_Object__table ti_uia_sysbios_Adaptor_Object__table__C;

/* eventBuf */
#define ti_uia_sysbios_Adaptor_eventBuf (ti_uia_sysbios_Adaptor_eventBuf__C)
typedef xdc_Char __T1_ti_uia_sysbios_Adaptor_eventBuf;
typedef xdc_Char *__ARRAY1_ti_uia_sysbios_Adaptor_eventBuf;
typedef __ARRAY1_ti_uia_sysbios_Adaptor_eventBuf __TA_ti_uia_sysbios_Adaptor_eventBuf;
typedef __TA_ti_uia_sysbios_Adaptor_eventBuf CT__ti_uia_sysbios_Adaptor_eventBuf;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_eventBuf ti_uia_sysbios_Adaptor_eventBuf__C;

/* msgBuf */
#define ti_uia_sysbios_Adaptor_msgBuf (ti_uia_sysbios_Adaptor_msgBuf__C)
typedef xdc_Char __T1_ti_uia_sysbios_Adaptor_msgBuf;
typedef xdc_Char *__ARRAY1_ti_uia_sysbios_Adaptor_msgBuf;
typedef __ARRAY1_ti_uia_sysbios_Adaptor_msgBuf __TA_ti_uia_sysbios_Adaptor_msgBuf;
typedef __TA_ti_uia_sysbios_Adaptor_msgBuf CT__ti_uia_sysbios_Adaptor_msgBuf;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_msgBuf ti_uia_sysbios_Adaptor_msgBuf__C;

/* period */
#define ti_uia_sysbios_Adaptor_period (ti_uia_sysbios_Adaptor_period__C)
typedef xdc_UInt32 __T1_ti_uia_sysbios_Adaptor_period;
typedef xdc_UInt32 *__ARRAY1_ti_uia_sysbios_Adaptor_period;
typedef __ARRAY1_ti_uia_sysbios_Adaptor_period __TA_ti_uia_sysbios_Adaptor_period;
typedef __TA_ti_uia_sysbios_Adaptor_period CT__ti_uia_sysbios_Adaptor_period;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_period ti_uia_sysbios_Adaptor_period__C;

/* scheduled */
#define ti_uia_sysbios_Adaptor_scheduled (ti_uia_sysbios_Adaptor_scheduled__C)
typedef xdc_UInt32 __T1_ti_uia_sysbios_Adaptor_scheduled;
typedef xdc_UInt32 *__ARRAY1_ti_uia_sysbios_Adaptor_scheduled;
typedef __ARRAY1_ti_uia_sysbios_Adaptor_scheduled __TA_ti_uia_sysbios_Adaptor_scheduled;
typedef __TA_ti_uia_sysbios_Adaptor_scheduled CT__ti_uia_sysbios_Adaptor_scheduled;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_scheduled ti_uia_sysbios_Adaptor_scheduled__C;

/* reqEnergy */
#define ti_uia_sysbios_Adaptor_reqEnergy (ti_uia_sysbios_Adaptor_reqEnergy__C)
typedef xdc_Bool __T1_ti_uia_sysbios_Adaptor_reqEnergy;
typedef xdc_Bool *__ARRAY1_ti_uia_sysbios_Adaptor_reqEnergy;
typedef __ARRAY1_ti_uia_sysbios_Adaptor_reqEnergy __TA_ti_uia_sysbios_Adaptor_reqEnergy;
typedef __TA_ti_uia_sysbios_Adaptor_reqEnergy CT__ti_uia_sysbios_Adaptor_reqEnergy;
__extern __FAR__ const CT__ti_uia_sysbios_Adaptor_reqEnergy ti_uia_sysbios_Adaptor_reqEnergy__C;


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct ti_uia_sysbios_Adaptor_Fxns__ {
    xdc_runtime_Types_Base* __base;
    const xdc_runtime_Types_SysFxns2 *__sysp;
    xdc_Void (*freePacket)(ti_uia_runtime_UIAPacket_Hdr*);
    ti_uia_runtime_UIAPacket_Hdr *(*getFreePacket)(ti_uia_runtime_UIAPacket_HdrType, xdc_UInt);
    xdc_Void (*requestEnergy)(xdc_Int);
    xdc_Bool (*sendPacket)(ti_uia_runtime_UIAPacket_Hdr*);
    xdc_Void (*setPeriod)(xdc_Int, xdc_UInt32);
    xdc_runtime_Types_SysFxns2 __sfxns;
};

/* Module__FXNS__C */
__extern const ti_uia_sysbios_Adaptor_Fxns__ ti_uia_sysbios_Adaptor_Module__FXNS__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_uia_sysbios_Adaptor_Module_startup ti_uia_sysbios_Adaptor_Module_startup__E
xdc__CODESECT(ti_uia_sysbios_Adaptor_Module_startup__E, "ti_uia_sysbios_Adaptor_Module_startup")
__extern xdc_Int ti_uia_sysbios_Adaptor_Module_startup__E( xdc_Int state );
xdc__CODESECT(ti_uia_sysbios_Adaptor_Module_startup__F, "ti_uia_sysbios_Adaptor_Module_startup")
__extern xdc_Int ti_uia_sysbios_Adaptor_Module_startup__F( xdc_Int state );

/* Module__startupDone__S */
xdc__CODESECT(ti_uia_sysbios_Adaptor_Module__startupDone__S, "ti_uia_sysbios_Adaptor_Module__startupDone__S")
__extern xdc_Bool ti_uia_sysbios_Adaptor_Module__startupDone__S( void );

/* freePacket__E */
#define ti_uia_sysbios_Adaptor_freePacket ti_uia_sysbios_Adaptor_freePacket__E
xdc__CODESECT(ti_uia_sysbios_Adaptor_freePacket__E, "ti_uia_sysbios_Adaptor_freePacket")
__extern xdc_Void ti_uia_sysbios_Adaptor_freePacket__E( ti_uia_runtime_UIAPacket_Hdr *packet );

/* getFreePacket__E */
#define ti_uia_sysbios_Adaptor_getFreePacket ti_uia_sysbios_Adaptor_getFreePacket__E
xdc__CODESECT(ti_uia_sysbios_Adaptor_getFreePacket__E, "ti_uia_sysbios_Adaptor_getFreePacket")
__extern ti_uia_runtime_UIAPacket_Hdr *ti_uia_sysbios_Adaptor_getFreePacket__E( ti_uia_runtime_UIAPacket_HdrType type, xdc_UInt timeout );

/* requestEnergy__E */
#define ti_uia_sysbios_Adaptor_requestEnergy ti_uia_sysbios_Adaptor_requestEnergy__E
xdc__CODESECT(ti_uia_sysbios_Adaptor_requestEnergy__E, "ti_uia_sysbios_Adaptor_requestEnergy")
__extern xdc_Void ti_uia_sysbios_Adaptor_requestEnergy__E( xdc_Int id );

/* sendPacket__E */
#define ti_uia_sysbios_Adaptor_sendPacket ti_uia_sysbios_Adaptor_sendPacket__E
xdc__CODESECT(ti_uia_sysbios_Adaptor_sendPacket__E, "ti_uia_sysbios_Adaptor_sendPacket")
__extern xdc_Bool ti_uia_sysbios_Adaptor_sendPacket__E( ti_uia_runtime_UIAPacket_Hdr *packet );

/* setPeriod__E */
#define ti_uia_sysbios_Adaptor_setPeriod ti_uia_sysbios_Adaptor_setPeriod__E
xdc__CODESECT(ti_uia_sysbios_Adaptor_setPeriod__E, "ti_uia_sysbios_Adaptor_setPeriod")
__extern xdc_Void ti_uia_sysbios_Adaptor_setPeriod__E( xdc_Int id, xdc_UInt32 periodInMs );

/* rxTaskFxn__E */
#define ti_uia_sysbios_Adaptor_rxTaskFxn ti_uia_sysbios_Adaptor_rxTaskFxn__E
xdc__CODESECT(ti_uia_sysbios_Adaptor_rxTaskFxn__E, "ti_uia_sysbios_Adaptor_rxTaskFxn")
__extern xdc_Void ti_uia_sysbios_Adaptor_rxTaskFxn__E( xdc_UArg arg0, xdc_UArg arg1 );

/* transferAgentTaskFxn__E */
#define ti_uia_sysbios_Adaptor_transferAgentTaskFxn ti_uia_sysbios_Adaptor_transferAgentTaskFxn__E
xdc__CODESECT(ti_uia_sysbios_Adaptor_transferAgentTaskFxn__E, "ti_uia_sysbios_Adaptor_transferAgentTaskFxn")
__extern xdc_Void ti_uia_sysbios_Adaptor_transferAgentTaskFxn__E( xdc_UArg arg0, xdc_UArg arg1 );

/* clockFxn__E */
#define ti_uia_sysbios_Adaptor_clockFxn ti_uia_sysbios_Adaptor_clockFxn__E
xdc__CODESECT(ti_uia_sysbios_Adaptor_clockFxn__E, "ti_uia_sysbios_Adaptor_clockFxn")
__extern xdc_Void ti_uia_sysbios_Adaptor_clockFxn__E( xdc_UArg arg0 );

/* giveEnergy__I */
#define ti_uia_sysbios_Adaptor_giveEnergy ti_uia_sysbios_Adaptor_giveEnergy__I
xdc__CODESECT(ti_uia_sysbios_Adaptor_giveEnergy__I, "ti_uia_sysbios_Adaptor_giveEnergy")
__extern xdc_Void ti_uia_sysbios_Adaptor_giveEnergy__I( void );

/* sendToHost__I */
#define ti_uia_sysbios_Adaptor_sendToHost ti_uia_sysbios_Adaptor_sendToHost__I
xdc__CODESECT(ti_uia_sysbios_Adaptor_sendToHost__I, "ti_uia_sysbios_Adaptor_sendToHost")
__extern xdc_Bool ti_uia_sysbios_Adaptor_sendToHost__I( ti_uia_runtime_UIAPacket_Hdr *packet );

/* sendToService__I */
#define ti_uia_sysbios_Adaptor_sendToService ti_uia_sysbios_Adaptor_sendToService__I
xdc__CODESECT(ti_uia_sysbios_Adaptor_sendToService__I, "ti_uia_sysbios_Adaptor_sendToService")
__extern xdc_Void ti_uia_sysbios_Adaptor_sendToService__I( ti_uia_sysbios_Adaptor_Entry *entry );

/* runScheduledServices__I */
#define ti_uia_sysbios_Adaptor_runScheduledServices ti_uia_sysbios_Adaptor_runScheduledServices__I
xdc__CODESECT(ti_uia_sysbios_Adaptor_runScheduledServices__I, "ti_uia_sysbios_Adaptor_runScheduledServices")
__extern xdc_Void ti_uia_sysbios_Adaptor_runScheduledServices__I( void );


/*
 * ======== CONVERTORS ========
 */

/* Module_upCast */
static inline ti_uia_runtime_IServiceMgrSupport_Module ti_uia_sysbios_Adaptor_Module_upCast( void )
{
    return (ti_uia_runtime_IServiceMgrSupport_Module)&ti_uia_sysbios_Adaptor_Module__FXNS__C;
}

/* Module_to_ti_uia_runtime_IServiceMgrSupport */
#define ti_uia_sysbios_Adaptor_Module_to_ti_uia_runtime_IServiceMgrSupport ti_uia_sysbios_Adaptor_Module_upCast


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_uia_sysbios_Adaptor_Module_startupDone() ti_uia_sysbios_Adaptor_Module__startupDone__S()

/* Object_heap */
#define ti_uia_sysbios_Adaptor_Object_heap() ti_uia_sysbios_Adaptor_Object__heap__C

/* Module_heap */
#define ti_uia_sysbios_Adaptor_Module_heap() ti_uia_sysbios_Adaptor_Object__heap__C

/* Module_id */
static inline CT__ti_uia_sysbios_Adaptor_Module__id ti_uia_sysbios_Adaptor_Module_id( void ) 
{
    return ti_uia_sysbios_Adaptor_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_uia_sysbios_Adaptor_Module_hasMask( void ) 
{
    return ti_uia_sysbios_Adaptor_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_uia_sysbios_Adaptor_Module_getMask( void ) 
{
    return ti_uia_sysbios_Adaptor_Module__diagsMask__C != NULL ? *ti_uia_sysbios_Adaptor_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_uia_sysbios_Adaptor_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_uia_sysbios_Adaptor_Module__diagsMask__C != NULL) *ti_uia_sysbios_Adaptor_Module__diagsMask__C = mask;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_uia_sysbios_Adaptor__top__
#undef __nested__
#endif

#endif /* ti_uia_sysbios_Adaptor__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_uia_sysbios_Adaptor__internalaccess))

#ifndef ti_uia_sysbios_Adaptor__include_state
#define ti_uia_sysbios_Adaptor__include_state

/* Module_State */
struct ti_uia_sysbios_Adaptor_Module_State {
    ti_sysbios_knl_Event_Handle event;
    ti_sysbios_knl_Clock_Handle clock;
    ti_sysbios_knl_Semaphore_Handle freeEventSem;
    ti_sysbios_knl_Semaphore_Handle freeMsgSem;
    ti_sysbios_knl_Semaphore_Handle incomingSem;
    ti_sysbios_knl_Queue_Handle freeEventQ;
    ti_sysbios_knl_Queue_Handle freeMsgQ;
    ti_sysbios_knl_Queue_Handle incomingQ;
    ti_sysbios_knl_Queue_Handle outgoingQ;
    ti_sysbios_syncs_SyncEvent_Handle syncEvent01;
    ti_sysbios_knl_Task_Handle transferAgentHandle;
    xdc_Ptr transportMsgHandle;
    xdc_Ptr transportEventHandle;
    xdc_Int numMsgPacketsSent;
    xdc_Int numMsgPacketsFailed;
    xdc_Int numEventPacketsSent;
    xdc_Int numEventPacketsFailed;
};

/* Module__state__V */
extern struct ti_uia_sysbios_Adaptor_Module_State__ ti_uia_sysbios_Adaptor_Module__state__V;

#endif /* ti_uia_sysbios_Adaptor__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_uia_sysbios_Adaptor__nolocalnames)

#ifndef ti_uia_sysbios_Adaptor__localnames__done
#define ti_uia_sysbios_Adaptor__localnames__done

/* module prefix */
#define Adaptor_Entry ti_uia_sysbios_Adaptor_Entry
#define Adaptor_Module_State ti_uia_sysbios_Adaptor_Module_State
#define Adaptor_eventBuf ti_uia_sysbios_Adaptor_eventBuf
#define Adaptor_msgBuf ti_uia_sysbios_Adaptor_msgBuf
#define Adaptor_period ti_uia_sysbios_Adaptor_period
#define Adaptor_scheduled ti_uia_sysbios_Adaptor_scheduled
#define Adaptor_reqEnergy ti_uia_sysbios_Adaptor_reqEnergy
#define Adaptor_freePacket ti_uia_sysbios_Adaptor_freePacket
#define Adaptor_getFreePacket ti_uia_sysbios_Adaptor_getFreePacket
#define Adaptor_requestEnergy ti_uia_sysbios_Adaptor_requestEnergy
#define Adaptor_sendPacket ti_uia_sysbios_Adaptor_sendPacket
#define Adaptor_setPeriod ti_uia_sysbios_Adaptor_setPeriod
#define Adaptor_rxTaskFxn ti_uia_sysbios_Adaptor_rxTaskFxn
#define Adaptor_transferAgentTaskFxn ti_uia_sysbios_Adaptor_transferAgentTaskFxn
#define Adaptor_clockFxn ti_uia_sysbios_Adaptor_clockFxn
#define Adaptor_Module_name ti_uia_sysbios_Adaptor_Module_name
#define Adaptor_Module_id ti_uia_sysbios_Adaptor_Module_id
#define Adaptor_Module_startup ti_uia_sysbios_Adaptor_Module_startup
#define Adaptor_Module_startupDone ti_uia_sysbios_Adaptor_Module_startupDone
#define Adaptor_Module_hasMask ti_uia_sysbios_Adaptor_Module_hasMask
#define Adaptor_Module_getMask ti_uia_sysbios_Adaptor_Module_getMask
#define Adaptor_Module_setMask ti_uia_sysbios_Adaptor_Module_setMask
#define Adaptor_Object_heap ti_uia_sysbios_Adaptor_Object_heap
#define Adaptor_Module_heap ti_uia_sysbios_Adaptor_Module_heap
#define Adaptor_Module_upCast ti_uia_sysbios_Adaptor_Module_upCast
#define Adaptor_Module_to_ti_uia_runtime_IServiceMgrSupport ti_uia_sysbios_Adaptor_Module_to_ti_uia_runtime_IServiceMgrSupport

#endif /* ti_uia_sysbios_Adaptor__localnames__done */
#endif
