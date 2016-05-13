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

#ifndef ti_uia_sysbios_IpcMP__include
#define ti_uia_sysbios_IpcMP__include

#ifndef __nested__
#define __nested__
#define ti_uia_sysbios_IpcMP__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_uia_sysbios_IpcMP___VERS 160


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/uia/sysbios/package/package.defs.h>

#include <xdc/runtime/Assert.h>
#include <ti/uia/runtime/UIAPacket.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/syncs/SyncEvent.h>
#include <ti/uia/runtime/IServiceMgrSupport.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */


/*
 * ======== INTERNAL DEFINITIONS ========
 */

/* Module_State */
typedef xdc_UInt32 __T1_ti_uia_sysbios_IpcMP_Module_State__replyMQ;
typedef xdc_UInt32 *__ARRAY1_ti_uia_sysbios_IpcMP_Module_State__replyMQ;
typedef __ARRAY1_ti_uia_sysbios_IpcMP_Module_State__replyMQ __TA_ti_uia_sysbios_IpcMP_Module_State__replyMQ;


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_uia_sysbios_IpcMP_Module__diagsEnabled;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Module__diagsEnabled ti_uia_sysbios_IpcMP_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_uia_sysbios_IpcMP_Module__diagsIncluded;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Module__diagsIncluded ti_uia_sysbios_IpcMP_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16 *CT__ti_uia_sysbios_IpcMP_Module__diagsMask;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Module__diagsMask ti_uia_sysbios_IpcMP_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_uia_sysbios_IpcMP_Module__gateObj;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Module__gateObj ti_uia_sysbios_IpcMP_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_uia_sysbios_IpcMP_Module__gatePrms;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Module__gatePrms ti_uia_sysbios_IpcMP_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_uia_sysbios_IpcMP_Module__id;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Module__id ti_uia_sysbios_IpcMP_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_uia_sysbios_IpcMP_Module__loggerDefined;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Module__loggerDefined ti_uia_sysbios_IpcMP_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_uia_sysbios_IpcMP_Module__loggerObj;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Module__loggerObj ti_uia_sysbios_IpcMP_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_uia_sysbios_IpcMP_Module__loggerFxn0;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Module__loggerFxn0 ti_uia_sysbios_IpcMP_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_uia_sysbios_IpcMP_Module__loggerFxn1;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Module__loggerFxn1 ti_uia_sysbios_IpcMP_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_uia_sysbios_IpcMP_Module__loggerFxn2;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Module__loggerFxn2 ti_uia_sysbios_IpcMP_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_uia_sysbios_IpcMP_Module__loggerFxn4;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Module__loggerFxn4 ti_uia_sysbios_IpcMP_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_uia_sysbios_IpcMP_Module__loggerFxn8;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Module__loggerFxn8 ti_uia_sysbios_IpcMP_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_uia_sysbios_IpcMP_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Module__startupDoneFxn ti_uia_sysbios_IpcMP_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_uia_sysbios_IpcMP_Object__count;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Object__count ti_uia_sysbios_IpcMP_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_uia_sysbios_IpcMP_Object__heap;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Object__heap ti_uia_sysbios_IpcMP_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_uia_sysbios_IpcMP_Object__sizeof;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Object__sizeof ti_uia_sysbios_IpcMP_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_uia_sysbios_IpcMP_Object__table;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_Object__table ti_uia_sysbios_IpcMP_Object__table__C;

/* A_IpcMPFailure */
#define ti_uia_sysbios_IpcMP_A_IpcMPFailure (ti_uia_sysbios_IpcMP_A_IpcMPFailure__C)
typedef xdc_runtime_Assert_Id CT__ti_uia_sysbios_IpcMP_A_IpcMPFailure;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_A_IpcMPFailure ti_uia_sysbios_IpcMP_A_IpcMPFailure__C;

/* sharedRegionId */
#ifdef ti_uia_sysbios_IpcMP_sharedRegionId__D
#define ti_uia_sysbios_IpcMP_sharedRegionId (ti_uia_sysbios_IpcMP_sharedRegionId__D)
#else
#define ti_uia_sysbios_IpcMP_sharedRegionId (ti_uia_sysbios_IpcMP_sharedRegionId__C)
typedef xdc_Int CT__ti_uia_sysbios_IpcMP_sharedRegionId;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_sharedRegionId ti_uia_sysbios_IpcMP_sharedRegionId__C;
#endif

/* period */
#define ti_uia_sysbios_IpcMP_period (ti_uia_sysbios_IpcMP_period__C)
typedef xdc_UInt32 __T1_ti_uia_sysbios_IpcMP_period;
typedef xdc_UInt32 *__ARRAY1_ti_uia_sysbios_IpcMP_period;
typedef __ARRAY1_ti_uia_sysbios_IpcMP_period __TA_ti_uia_sysbios_IpcMP_period;
typedef __TA_ti_uia_sysbios_IpcMP_period CT__ti_uia_sysbios_IpcMP_period;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_period ti_uia_sysbios_IpcMP_period__C;

/* scheduled */
#define ti_uia_sysbios_IpcMP_scheduled (ti_uia_sysbios_IpcMP_scheduled__C)
typedef xdc_UInt32 __T1_ti_uia_sysbios_IpcMP_scheduled;
typedef xdc_UInt32 *__ARRAY1_ti_uia_sysbios_IpcMP_scheduled;
typedef __ARRAY1_ti_uia_sysbios_IpcMP_scheduled __TA_ti_uia_sysbios_IpcMP_scheduled;
typedef __TA_ti_uia_sysbios_IpcMP_scheduled CT__ti_uia_sysbios_IpcMP_scheduled;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_scheduled ti_uia_sysbios_IpcMP_scheduled__C;

/* reqEnergy */
#define ti_uia_sysbios_IpcMP_reqEnergy (ti_uia_sysbios_IpcMP_reqEnergy__C)
typedef xdc_Bool __T1_ti_uia_sysbios_IpcMP_reqEnergy;
typedef xdc_Bool *__ARRAY1_ti_uia_sysbios_IpcMP_reqEnergy;
typedef __ARRAY1_ti_uia_sysbios_IpcMP_reqEnergy __TA_ti_uia_sysbios_IpcMP_reqEnergy;
typedef __TA_ti_uia_sysbios_IpcMP_reqEnergy CT__ti_uia_sysbios_IpcMP_reqEnergy;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_reqEnergy ti_uia_sysbios_IpcMP_reqEnergy__C;

/* createRxTask */
#ifdef ti_uia_sysbios_IpcMP_createRxTask__D
#define ti_uia_sysbios_IpcMP_createRxTask (ti_uia_sysbios_IpcMP_createRxTask__D)
#else
#define ti_uia_sysbios_IpcMP_createRxTask (ti_uia_sysbios_IpcMP_createRxTask__C)
typedef xdc_Bool CT__ti_uia_sysbios_IpcMP_createRxTask;
__extern __FAR__ const CT__ti_uia_sysbios_IpcMP_createRxTask ti_uia_sysbios_IpcMP_createRxTask__C;
#endif


/*
 * ======== VIRTUAL FUNCTIONS ========
 */

/* Fxns__ */
struct ti_uia_sysbios_IpcMP_Fxns__ {
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
__extern const ti_uia_sysbios_IpcMP_Fxns__ ti_uia_sysbios_IpcMP_Module__FXNS__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_uia_sysbios_IpcMP_Module_startup ti_uia_sysbios_IpcMP_Module_startup__E
xdc__CODESECT(ti_uia_sysbios_IpcMP_Module_startup__E, "ti_uia_sysbios_IpcMP_Module_startup")
__extern xdc_Int ti_uia_sysbios_IpcMP_Module_startup__E( xdc_Int state );
xdc__CODESECT(ti_uia_sysbios_IpcMP_Module_startup__F, "ti_uia_sysbios_IpcMP_Module_startup")
__extern xdc_Int ti_uia_sysbios_IpcMP_Module_startup__F( xdc_Int state );

/* Module__startupDone__S */
xdc__CODESECT(ti_uia_sysbios_IpcMP_Module__startupDone__S, "ti_uia_sysbios_IpcMP_Module__startupDone__S")
__extern xdc_Bool ti_uia_sysbios_IpcMP_Module__startupDone__S( void );

/* freePacket__E */
#define ti_uia_sysbios_IpcMP_freePacket ti_uia_sysbios_IpcMP_freePacket__E
xdc__CODESECT(ti_uia_sysbios_IpcMP_freePacket__E, "ti_uia_sysbios_IpcMP_freePacket")
__extern xdc_Void ti_uia_sysbios_IpcMP_freePacket__E( ti_uia_runtime_UIAPacket_Hdr *packet );

/* getFreePacket__E */
#define ti_uia_sysbios_IpcMP_getFreePacket ti_uia_sysbios_IpcMP_getFreePacket__E
xdc__CODESECT(ti_uia_sysbios_IpcMP_getFreePacket__E, "ti_uia_sysbios_IpcMP_getFreePacket")
__extern ti_uia_runtime_UIAPacket_Hdr *ti_uia_sysbios_IpcMP_getFreePacket__E( ti_uia_runtime_UIAPacket_HdrType type, xdc_UInt timeout );

/* requestEnergy__E */
#define ti_uia_sysbios_IpcMP_requestEnergy ti_uia_sysbios_IpcMP_requestEnergy__E
xdc__CODESECT(ti_uia_sysbios_IpcMP_requestEnergy__E, "ti_uia_sysbios_IpcMP_requestEnergy")
__extern xdc_Void ti_uia_sysbios_IpcMP_requestEnergy__E( xdc_Int id );

/* sendPacket__E */
#define ti_uia_sysbios_IpcMP_sendPacket ti_uia_sysbios_IpcMP_sendPacket__E
xdc__CODESECT(ti_uia_sysbios_IpcMP_sendPacket__E, "ti_uia_sysbios_IpcMP_sendPacket")
__extern xdc_Bool ti_uia_sysbios_IpcMP_sendPacket__E( ti_uia_runtime_UIAPacket_Hdr *packet );

/* setPeriod__E */
#define ti_uia_sysbios_IpcMP_setPeriod ti_uia_sysbios_IpcMP_setPeriod__E
xdc__CODESECT(ti_uia_sysbios_IpcMP_setPeriod__E, "ti_uia_sysbios_IpcMP_setPeriod")
__extern xdc_Void ti_uia_sysbios_IpcMP_setPeriod__E( xdc_Int id, xdc_UInt32 periodInMs );

/* rxTaskFxn__E */
#define ti_uia_sysbios_IpcMP_rxTaskFxn ti_uia_sysbios_IpcMP_rxTaskFxn__E
xdc__CODESECT(ti_uia_sysbios_IpcMP_rxTaskFxn__E, "ti_uia_sysbios_IpcMP_rxTaskFxn")
__extern xdc_Void ti_uia_sysbios_IpcMP_rxTaskFxn__E( xdc_UArg arg0, xdc_UArg arg1 );

/* transferAgentFxn__E */
#define ti_uia_sysbios_IpcMP_transferAgentFxn ti_uia_sysbios_IpcMP_transferAgentFxn__E
xdc__CODESECT(ti_uia_sysbios_IpcMP_transferAgentFxn__E, "ti_uia_sysbios_IpcMP_transferAgentFxn")
__extern xdc_Void ti_uia_sysbios_IpcMP_transferAgentFxn__E( xdc_UArg arg0, xdc_UArg arg1 );

/* clockFxn__E */
#define ti_uia_sysbios_IpcMP_clockFxn ti_uia_sysbios_IpcMP_clockFxn__E
xdc__CODESECT(ti_uia_sysbios_IpcMP_clockFxn__E, "ti_uia_sysbios_IpcMP_clockFxn")
__extern xdc_Void ti_uia_sysbios_IpcMP_clockFxn__E( xdc_UArg arg0 );

/* start__E */
#define ti_uia_sysbios_IpcMP_start ti_uia_sysbios_IpcMP_start__E
xdc__CODESECT(ti_uia_sysbios_IpcMP_start__E, "ti_uia_sysbios_IpcMP_start")
__extern xdc_Int ti_uia_sysbios_IpcMP_start__E( xdc_UArg arg, xdc_UInt16 value );

/* startOrig__I */
#define ti_uia_sysbios_IpcMP_startOrig ti_uia_sysbios_IpcMP_startOrig__I
xdc__CODESECT(ti_uia_sysbios_IpcMP_startOrig__I, "ti_uia_sysbios_IpcMP_startOrig")
__extern xdc_Int ti_uia_sysbios_IpcMP_startOrig__I( xdc_Ptr *ptr, xdc_UInt16 value );

/* giveEnergy__I */
#define ti_uia_sysbios_IpcMP_giveEnergy ti_uia_sysbios_IpcMP_giveEnergy__I
xdc__CODESECT(ti_uia_sysbios_IpcMP_giveEnergy__I, "ti_uia_sysbios_IpcMP_giveEnergy")
__extern xdc_Void ti_uia_sysbios_IpcMP_giveEnergy__I( void );

/* handleMsg__I */
#define ti_uia_sysbios_IpcMP_handleMsg ti_uia_sysbios_IpcMP_handleMsg__I
xdc__CODESECT(ti_uia_sysbios_IpcMP_handleMsg__I, "ti_uia_sysbios_IpcMP_handleMsg")
__extern xdc_Void ti_uia_sysbios_IpcMP_handleMsg__I( xdc_Ptr msg );

/* prime__I */
#define ti_uia_sysbios_IpcMP_prime ti_uia_sysbios_IpcMP_prime__I
xdc__CODESECT(ti_uia_sysbios_IpcMP_prime__I, "ti_uia_sysbios_IpcMP_prime")
__extern xdc_Void ti_uia_sysbios_IpcMP_prime__I( xdc_Ptr handle, xdc_Int size, xdc_Int count );

/* registerWithMaster__I */
#define ti_uia_sysbios_IpcMP_registerWithMaster ti_uia_sysbios_IpcMP_registerWithMaster__I
xdc__CODESECT(ti_uia_sysbios_IpcMP_registerWithMaster__I, "ti_uia_sysbios_IpcMP_registerWithMaster")
__extern xdc_Void ti_uia_sysbios_IpcMP_registerWithMaster__I( void );

/* requestEvents__I */
#define ti_uia_sysbios_IpcMP_requestEvents ti_uia_sysbios_IpcMP_requestEvents__I
xdc__CODESECT(ti_uia_sysbios_IpcMP_requestEvents__I, "ti_uia_sysbios_IpcMP_requestEvents")
__extern xdc_Void ti_uia_sysbios_IpcMP_requestEvents__I( void );


/*
 * ======== CONVERTORS ========
 */

/* Module_upCast */
static inline ti_uia_runtime_IServiceMgrSupport_Module ti_uia_sysbios_IpcMP_Module_upCast( void )
{
    return (ti_uia_runtime_IServiceMgrSupport_Module)&ti_uia_sysbios_IpcMP_Module__FXNS__C;
}

/* Module_to_ti_uia_runtime_IServiceMgrSupport */
#define ti_uia_sysbios_IpcMP_Module_to_ti_uia_runtime_IServiceMgrSupport ti_uia_sysbios_IpcMP_Module_upCast


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_uia_sysbios_IpcMP_Module_startupDone() ti_uia_sysbios_IpcMP_Module__startupDone__S()

/* Object_heap */
#define ti_uia_sysbios_IpcMP_Object_heap() ti_uia_sysbios_IpcMP_Object__heap__C

/* Module_heap */
#define ti_uia_sysbios_IpcMP_Module_heap() ti_uia_sysbios_IpcMP_Object__heap__C

/* Module_id */
static inline CT__ti_uia_sysbios_IpcMP_Module__id ti_uia_sysbios_IpcMP_Module_id( void ) 
{
    return ti_uia_sysbios_IpcMP_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_uia_sysbios_IpcMP_Module_hasMask( void ) 
{
    return ti_uia_sysbios_IpcMP_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_uia_sysbios_IpcMP_Module_getMask( void ) 
{
    return ti_uia_sysbios_IpcMP_Module__diagsMask__C != NULL ? *ti_uia_sysbios_IpcMP_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_uia_sysbios_IpcMP_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_uia_sysbios_IpcMP_Module__diagsMask__C != NULL) *ti_uia_sysbios_IpcMP_Module__diagsMask__C = mask;
}


/*
 * ======== EPILOGUE ========
 */

#ifdef ti_uia_sysbios_IpcMP__top__
#undef __nested__
#endif

#endif /* ti_uia_sysbios_IpcMP__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_uia_sysbios_IpcMP__internalaccess))

#ifndef ti_uia_sysbios_IpcMP__include_state
#define ti_uia_sysbios_IpcMP__include_state

/* Module_State */
struct ti_uia_sysbios_IpcMP_Module_State {
    ti_sysbios_knl_Event_Handle event;
    ti_sysbios_knl_Clock_Handle clock;
    xdc_Ptr freeEventMQ;
    xdc_Ptr freeMsgMQ;
    xdc_Ptr routerMQ;
    xdc_Ptr startedMQ;
    xdc_UInt32 masterMQ;
    xdc_Ptr incomingMsgMQ;
    __TA_ti_uia_sysbios_IpcMP_Module_State__replyMQ replyMQ;
    ti_sysbios_syncs_SyncEvent_Handle syncEvent01;
    ti_sysbios_knl_Task_Handle transferAgentHandle;
    xdc_Ptr transportMsgHandle;
    xdc_Ptr transportEventHandle;
    ti_sysbios_knl_Semaphore_Handle releaseTasksSem;
    xdc_Int numMSGPacketsSent;
    xdc_Int numEventPacketsSent;
    xdc_Bool master;
    xdc_Bool masterRunning;
};

/* Module__state__V */
extern struct ti_uia_sysbios_IpcMP_Module_State__ ti_uia_sysbios_IpcMP_Module__state__V;

#endif /* ti_uia_sysbios_IpcMP__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_uia_sysbios_IpcMP__nolocalnames)

#ifndef ti_uia_sysbios_IpcMP__localnames__done
#define ti_uia_sysbios_IpcMP__localnames__done

/* module prefix */
#define IpcMP_Module_State ti_uia_sysbios_IpcMP_Module_State
#define IpcMP_A_IpcMPFailure ti_uia_sysbios_IpcMP_A_IpcMPFailure
#define IpcMP_sharedRegionId ti_uia_sysbios_IpcMP_sharedRegionId
#define IpcMP_period ti_uia_sysbios_IpcMP_period
#define IpcMP_scheduled ti_uia_sysbios_IpcMP_scheduled
#define IpcMP_reqEnergy ti_uia_sysbios_IpcMP_reqEnergy
#define IpcMP_createRxTask ti_uia_sysbios_IpcMP_createRxTask
#define IpcMP_freePacket ti_uia_sysbios_IpcMP_freePacket
#define IpcMP_getFreePacket ti_uia_sysbios_IpcMP_getFreePacket
#define IpcMP_requestEnergy ti_uia_sysbios_IpcMP_requestEnergy
#define IpcMP_sendPacket ti_uia_sysbios_IpcMP_sendPacket
#define IpcMP_setPeriod ti_uia_sysbios_IpcMP_setPeriod
#define IpcMP_rxTaskFxn ti_uia_sysbios_IpcMP_rxTaskFxn
#define IpcMP_transferAgentFxn ti_uia_sysbios_IpcMP_transferAgentFxn
#define IpcMP_clockFxn ti_uia_sysbios_IpcMP_clockFxn
#define IpcMP_start ti_uia_sysbios_IpcMP_start
#define IpcMP_Module_name ti_uia_sysbios_IpcMP_Module_name
#define IpcMP_Module_id ti_uia_sysbios_IpcMP_Module_id
#define IpcMP_Module_startup ti_uia_sysbios_IpcMP_Module_startup
#define IpcMP_Module_startupDone ti_uia_sysbios_IpcMP_Module_startupDone
#define IpcMP_Module_hasMask ti_uia_sysbios_IpcMP_Module_hasMask
#define IpcMP_Module_getMask ti_uia_sysbios_IpcMP_Module_getMask
#define IpcMP_Module_setMask ti_uia_sysbios_IpcMP_Module_setMask
#define IpcMP_Object_heap ti_uia_sysbios_IpcMP_Object_heap
#define IpcMP_Module_heap ti_uia_sysbios_IpcMP_Module_heap
#define IpcMP_Module_upCast ti_uia_sysbios_IpcMP_Module_upCast
#define IpcMP_Module_to_ti_uia_runtime_IServiceMgrSupport ti_uia_sysbios_IpcMP_Module_to_ti_uia_runtime_IServiceMgrSupport

#endif /* ti_uia_sysbios_IpcMP__localnames__done */
#endif
