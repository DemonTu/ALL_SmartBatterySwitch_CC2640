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
 *     FUNCTION DECLARATIONS
 *     SYSTEM FUNCTIONS
 *     
 *     EPILOGUE
 *     STATE STRUCTURES
 *     PREFIX ALIASES
 */


/*
 * ======== PROLOGUE ========
 */

#ifndef ti_uia_runtime_ServiceMgr__include
#define ti_uia_runtime_ServiceMgr__include

#ifndef __nested__
#define __nested__
#define ti_uia_runtime_ServiceMgr__top__
#endif

#ifdef __cplusplus
#define __extern extern "C"
#else
#define __extern extern
#endif

#define ti_uia_runtime_ServiceMgr___VERS 160


/*
 * ======== INCLUDES ========
 */

#include <xdc/std.h>

#include <xdc/runtime/xdc.h>
#include <xdc/runtime/Types.h>
#include <ti/uia/runtime/ServiceMgr__prologue.h>
#include <ti/uia/runtime/package/package.defs.h>

#include <xdc/runtime/Assert.h>
#include <xdc/runtime/IModule.h>
#include <ti/uia/runtime/UIAPacket.h>
#include <ti/uia/runtime/Transport.h>
#include <ti/uia/runtime/IServiceMgrSupport.h>
#include <ti/uia/runtime/package/ServiceMgr_SupportProxy.h>


/*
 * ======== AUXILIARY DEFINITIONS ========
 */

/* Reason */
enum ti_uia_runtime_ServiceMgr_Reason {
    ti_uia_runtime_ServiceMgr_Reason_PERIODEXPIRED,
    ti_uia_runtime_ServiceMgr_Reason_REQUESTENERGY,
    ti_uia_runtime_ServiceMgr_Reason_INCOMINGMSG
};
typedef enum ti_uia_runtime_ServiceMgr_Reason ti_uia_runtime_ServiceMgr_Reason;

/* Topology */
enum ti_uia_runtime_ServiceMgr_Topology {
    ti_uia_runtime_ServiceMgr_Topology_SINGLECORE,
    ti_uia_runtime_ServiceMgr_Topology_MULTICORE
};
typedef enum ti_uia_runtime_ServiceMgr_Topology ti_uia_runtime_ServiceMgr_Topology;

/* ProcessCallback */
typedef xdc_Void (*ti_uia_runtime_ServiceMgr_ProcessCallback)(ti_uia_runtime_ServiceMgr_Reason, ti_uia_runtime_UIAPacket_Hdr*);

/* WAIT_FOREVER */
#define ti_uia_runtime_ServiceMgr_WAIT_FOREVER (~(0))

/* ServiceId */


/*
 * ======== INTERNAL DEFINITIONS ========
 */


/*
 * ======== MODULE-WIDE CONFIGS ========
 */

/* Module__diagsEnabled */
typedef xdc_Bits32 CT__ti_uia_runtime_ServiceMgr_Module__diagsEnabled;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Module__diagsEnabled ti_uia_runtime_ServiceMgr_Module__diagsEnabled__C;

/* Module__diagsIncluded */
typedef xdc_Bits32 CT__ti_uia_runtime_ServiceMgr_Module__diagsIncluded;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Module__diagsIncluded ti_uia_runtime_ServiceMgr_Module__diagsIncluded__C;

/* Module__diagsMask */
typedef xdc_Bits16 *CT__ti_uia_runtime_ServiceMgr_Module__diagsMask;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Module__diagsMask ti_uia_runtime_ServiceMgr_Module__diagsMask__C;

/* Module__gateObj */
typedef xdc_Ptr CT__ti_uia_runtime_ServiceMgr_Module__gateObj;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Module__gateObj ti_uia_runtime_ServiceMgr_Module__gateObj__C;

/* Module__gatePrms */
typedef xdc_Ptr CT__ti_uia_runtime_ServiceMgr_Module__gatePrms;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Module__gatePrms ti_uia_runtime_ServiceMgr_Module__gatePrms__C;

/* Module__id */
typedef xdc_runtime_Types_ModuleId CT__ti_uia_runtime_ServiceMgr_Module__id;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Module__id ti_uia_runtime_ServiceMgr_Module__id__C;

/* Module__loggerDefined */
typedef xdc_Bool CT__ti_uia_runtime_ServiceMgr_Module__loggerDefined;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Module__loggerDefined ti_uia_runtime_ServiceMgr_Module__loggerDefined__C;

/* Module__loggerObj */
typedef xdc_Ptr CT__ti_uia_runtime_ServiceMgr_Module__loggerObj;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Module__loggerObj ti_uia_runtime_ServiceMgr_Module__loggerObj__C;

/* Module__loggerFxn0 */
typedef xdc_runtime_Types_LoggerFxn0 CT__ti_uia_runtime_ServiceMgr_Module__loggerFxn0;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Module__loggerFxn0 ti_uia_runtime_ServiceMgr_Module__loggerFxn0__C;

/* Module__loggerFxn1 */
typedef xdc_runtime_Types_LoggerFxn1 CT__ti_uia_runtime_ServiceMgr_Module__loggerFxn1;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Module__loggerFxn1 ti_uia_runtime_ServiceMgr_Module__loggerFxn1__C;

/* Module__loggerFxn2 */
typedef xdc_runtime_Types_LoggerFxn2 CT__ti_uia_runtime_ServiceMgr_Module__loggerFxn2;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Module__loggerFxn2 ti_uia_runtime_ServiceMgr_Module__loggerFxn2__C;

/* Module__loggerFxn4 */
typedef xdc_runtime_Types_LoggerFxn4 CT__ti_uia_runtime_ServiceMgr_Module__loggerFxn4;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Module__loggerFxn4 ti_uia_runtime_ServiceMgr_Module__loggerFxn4__C;

/* Module__loggerFxn8 */
typedef xdc_runtime_Types_LoggerFxn8 CT__ti_uia_runtime_ServiceMgr_Module__loggerFxn8;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Module__loggerFxn8 ti_uia_runtime_ServiceMgr_Module__loggerFxn8__C;

/* Module__startupDoneFxn */
typedef xdc_Bool (*CT__ti_uia_runtime_ServiceMgr_Module__startupDoneFxn)(void);
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Module__startupDoneFxn ti_uia_runtime_ServiceMgr_Module__startupDoneFxn__C;

/* Object__count */
typedef xdc_Int CT__ti_uia_runtime_ServiceMgr_Object__count;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Object__count ti_uia_runtime_ServiceMgr_Object__count__C;

/* Object__heap */
typedef xdc_runtime_IHeap_Handle CT__ti_uia_runtime_ServiceMgr_Object__heap;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Object__heap ti_uia_runtime_ServiceMgr_Object__heap__C;

/* Object__sizeof */
typedef xdc_SizeT CT__ti_uia_runtime_ServiceMgr_Object__sizeof;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Object__sizeof ti_uia_runtime_ServiceMgr_Object__sizeof__C;

/* Object__table */
typedef xdc_Ptr CT__ti_uia_runtime_ServiceMgr_Object__table;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_Object__table ti_uia_runtime_ServiceMgr_Object__table__C;

/* A_invalidServiceId */
#define ti_uia_runtime_ServiceMgr_A_invalidServiceId (ti_uia_runtime_ServiceMgr_A_invalidServiceId__C)
typedef xdc_runtime_Assert_Id CT__ti_uia_runtime_ServiceMgr_A_invalidServiceId;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_A_invalidServiceId ti_uia_runtime_ServiceMgr_A_invalidServiceId__C;

/* A_invalidProcessCallbackFxn */
#define ti_uia_runtime_ServiceMgr_A_invalidProcessCallbackFxn (ti_uia_runtime_ServiceMgr_A_invalidProcessCallbackFxn__C)
typedef xdc_runtime_Assert_Id CT__ti_uia_runtime_ServiceMgr_A_invalidProcessCallbackFxn;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_A_invalidProcessCallbackFxn ti_uia_runtime_ServiceMgr_A_invalidProcessCallbackFxn__C;

/* transportFxns */
#define ti_uia_runtime_ServiceMgr_transportFxns (ti_uia_runtime_ServiceMgr_transportFxns__C)
typedef ti_uia_runtime_Transport_FxnSet CT__ti_uia_runtime_ServiceMgr_transportFxns;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_transportFxns ti_uia_runtime_ServiceMgr_transportFxns__C;

/* topology */
#define ti_uia_runtime_ServiceMgr_topology (ti_uia_runtime_ServiceMgr_topology__C)
typedef ti_uia_runtime_ServiceMgr_Topology CT__ti_uia_runtime_ServiceMgr_topology;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_topology ti_uia_runtime_ServiceMgr_topology__C;

/* periodInMs */
#ifdef ti_uia_runtime_ServiceMgr_periodInMs__D
#define ti_uia_runtime_ServiceMgr_periodInMs (ti_uia_runtime_ServiceMgr_periodInMs__D)
#else
#define ti_uia_runtime_ServiceMgr_periodInMs (ti_uia_runtime_ServiceMgr_periodInMs__C)
typedef xdc_Int CT__ti_uia_runtime_ServiceMgr_periodInMs;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_periodInMs ti_uia_runtime_ServiceMgr_periodInMs__C;
#endif

/* maxEventPacketSize */
#ifdef ti_uia_runtime_ServiceMgr_maxEventPacketSize__D
#define ti_uia_runtime_ServiceMgr_maxEventPacketSize (ti_uia_runtime_ServiceMgr_maxEventPacketSize__D)
#else
#define ti_uia_runtime_ServiceMgr_maxEventPacketSize (ti_uia_runtime_ServiceMgr_maxEventPacketSize__C)
typedef xdc_SizeT CT__ti_uia_runtime_ServiceMgr_maxEventPacketSize;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_maxEventPacketSize ti_uia_runtime_ServiceMgr_maxEventPacketSize__C;
#endif

/* numEventPacketBufs */
#ifdef ti_uia_runtime_ServiceMgr_numEventPacketBufs__D
#define ti_uia_runtime_ServiceMgr_numEventPacketBufs (ti_uia_runtime_ServiceMgr_numEventPacketBufs__D)
#else
#define ti_uia_runtime_ServiceMgr_numEventPacketBufs (ti_uia_runtime_ServiceMgr_numEventPacketBufs__C)
typedef xdc_Int CT__ti_uia_runtime_ServiceMgr_numEventPacketBufs;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_numEventPacketBufs ti_uia_runtime_ServiceMgr_numEventPacketBufs__C;
#endif

/* maxCtrlPacketSize */
#ifdef ti_uia_runtime_ServiceMgr_maxCtrlPacketSize__D
#define ti_uia_runtime_ServiceMgr_maxCtrlPacketSize (ti_uia_runtime_ServiceMgr_maxCtrlPacketSize__D)
#else
#define ti_uia_runtime_ServiceMgr_maxCtrlPacketSize (ti_uia_runtime_ServiceMgr_maxCtrlPacketSize__C)
typedef xdc_SizeT CT__ti_uia_runtime_ServiceMgr_maxCtrlPacketSize;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_maxCtrlPacketSize ti_uia_runtime_ServiceMgr_maxCtrlPacketSize__C;
#endif

/* numOutgoingCtrlPacketBufs */
#ifdef ti_uia_runtime_ServiceMgr_numOutgoingCtrlPacketBufs__D
#define ti_uia_runtime_ServiceMgr_numOutgoingCtrlPacketBufs (ti_uia_runtime_ServiceMgr_numOutgoingCtrlPacketBufs__D)
#else
#define ti_uia_runtime_ServiceMgr_numOutgoingCtrlPacketBufs (ti_uia_runtime_ServiceMgr_numOutgoingCtrlPacketBufs__C)
typedef xdc_Int CT__ti_uia_runtime_ServiceMgr_numOutgoingCtrlPacketBufs;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_numOutgoingCtrlPacketBufs ti_uia_runtime_ServiceMgr_numOutgoingCtrlPacketBufs__C;
#endif

/* numIncomingCtrlPacketBufs */
#ifdef ti_uia_runtime_ServiceMgr_numIncomingCtrlPacketBufs__D
#define ti_uia_runtime_ServiceMgr_numIncomingCtrlPacketBufs (ti_uia_runtime_ServiceMgr_numIncomingCtrlPacketBufs__D)
#else
#define ti_uia_runtime_ServiceMgr_numIncomingCtrlPacketBufs (ti_uia_runtime_ServiceMgr_numIncomingCtrlPacketBufs__C)
typedef xdc_Int CT__ti_uia_runtime_ServiceMgr_numIncomingCtrlPacketBufs;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_numIncomingCtrlPacketBufs ti_uia_runtime_ServiceMgr_numIncomingCtrlPacketBufs__C;
#endif

/* supportControl */
#ifdef ti_uia_runtime_ServiceMgr_supportControl__D
#define ti_uia_runtime_ServiceMgr_supportControl (ti_uia_runtime_ServiceMgr_supportControl__D)
#else
#define ti_uia_runtime_ServiceMgr_supportControl (ti_uia_runtime_ServiceMgr_supportControl__C)
typedef xdc_Bool CT__ti_uia_runtime_ServiceMgr_supportControl;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_supportControl ti_uia_runtime_ServiceMgr_supportControl__C;
#endif

/* transferAgentPriority */
#ifdef ti_uia_runtime_ServiceMgr_transferAgentPriority__D
#define ti_uia_runtime_ServiceMgr_transferAgentPriority (ti_uia_runtime_ServiceMgr_transferAgentPriority__D)
#else
#define ti_uia_runtime_ServiceMgr_transferAgentPriority (ti_uia_runtime_ServiceMgr_transferAgentPriority__C)
typedef xdc_Int CT__ti_uia_runtime_ServiceMgr_transferAgentPriority;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_transferAgentPriority ti_uia_runtime_ServiceMgr_transferAgentPriority__C;
#endif

/* transferAgentStackSize */
#ifdef ti_uia_runtime_ServiceMgr_transferAgentStackSize__D
#define ti_uia_runtime_ServiceMgr_transferAgentStackSize (ti_uia_runtime_ServiceMgr_transferAgentStackSize__D)
#else
#define ti_uia_runtime_ServiceMgr_transferAgentStackSize (ti_uia_runtime_ServiceMgr_transferAgentStackSize__C)
typedef xdc_SizeT CT__ti_uia_runtime_ServiceMgr_transferAgentStackSize;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_transferAgentStackSize ti_uia_runtime_ServiceMgr_transferAgentStackSize__C;
#endif

/* rxTaskPriority */
#ifdef ti_uia_runtime_ServiceMgr_rxTaskPriority__D
#define ti_uia_runtime_ServiceMgr_rxTaskPriority (ti_uia_runtime_ServiceMgr_rxTaskPriority__D)
#else
#define ti_uia_runtime_ServiceMgr_rxTaskPriority (ti_uia_runtime_ServiceMgr_rxTaskPriority__C)
typedef xdc_Int CT__ti_uia_runtime_ServiceMgr_rxTaskPriority;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_rxTaskPriority ti_uia_runtime_ServiceMgr_rxTaskPriority__C;
#endif

/* rxTaskStackSize */
#ifdef ti_uia_runtime_ServiceMgr_rxTaskStackSize__D
#define ti_uia_runtime_ServiceMgr_rxTaskStackSize (ti_uia_runtime_ServiceMgr_rxTaskStackSize__D)
#else
#define ti_uia_runtime_ServiceMgr_rxTaskStackSize (ti_uia_runtime_ServiceMgr_rxTaskStackSize__C)
typedef xdc_SizeT CT__ti_uia_runtime_ServiceMgr_rxTaskStackSize;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_rxTaskStackSize ti_uia_runtime_ServiceMgr_rxTaskStackSize__C;
#endif

/* masterProcId */
#ifdef ti_uia_runtime_ServiceMgr_masterProcId__D
#define ti_uia_runtime_ServiceMgr_masterProcId (ti_uia_runtime_ServiceMgr_masterProcId__D)
#else
#define ti_uia_runtime_ServiceMgr_masterProcId (ti_uia_runtime_ServiceMgr_masterProcId__C)
typedef xdc_UInt16 CT__ti_uia_runtime_ServiceMgr_masterProcId;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_masterProcId ti_uia_runtime_ServiceMgr_masterProcId__C;
#endif

/* processCallbackFxn */
#define ti_uia_runtime_ServiceMgr_processCallbackFxn (ti_uia_runtime_ServiceMgr_processCallbackFxn__C)
typedef ti_uia_runtime_ServiceMgr_ProcessCallback __T1_ti_uia_runtime_ServiceMgr_processCallbackFxn;
typedef ti_uia_runtime_ServiceMgr_ProcessCallback *__ARRAY1_ti_uia_runtime_ServiceMgr_processCallbackFxn;
typedef __ARRAY1_ti_uia_runtime_ServiceMgr_processCallbackFxn __TA_ti_uia_runtime_ServiceMgr_processCallbackFxn;
typedef __TA_ti_uia_runtime_ServiceMgr_processCallbackFxn CT__ti_uia_runtime_ServiceMgr_processCallbackFxn;
__extern __FAR__ const CT__ti_uia_runtime_ServiceMgr_processCallbackFxn ti_uia_runtime_ServiceMgr_processCallbackFxn__C;


/*
 * ======== FUNCTION DECLARATIONS ========
 */

/* Module_startup */
#define ti_uia_runtime_ServiceMgr_Module_startup( state ) (-1)

/* Module__startupDone__S */
xdc__CODESECT(ti_uia_runtime_ServiceMgr_Module__startupDone__S, "ti_uia_runtime_ServiceMgr_Module__startupDone__S")
__extern xdc_Bool ti_uia_runtime_ServiceMgr_Module__startupDone__S( void );

/* freePacket__E */
#define ti_uia_runtime_ServiceMgr_freePacket ti_uia_runtime_ServiceMgr_freePacket__E
xdc__CODESECT(ti_uia_runtime_ServiceMgr_freePacket__E, "ti_uia_runtime_ServiceMgr_freePacket")
__extern xdc_Void ti_uia_runtime_ServiceMgr_freePacket__E( ti_uia_runtime_UIAPacket_Hdr *packet );
xdc__CODESECT(ti_uia_runtime_ServiceMgr_freePacket__F, "ti_uia_runtime_ServiceMgr_freePacket")
__extern xdc_Void ti_uia_runtime_ServiceMgr_freePacket__F( ti_uia_runtime_UIAPacket_Hdr *packet );

/* getFreePacket__E */
#define ti_uia_runtime_ServiceMgr_getFreePacket ti_uia_runtime_ServiceMgr_getFreePacket__E
xdc__CODESECT(ti_uia_runtime_ServiceMgr_getFreePacket__E, "ti_uia_runtime_ServiceMgr_getFreePacket")
__extern ti_uia_runtime_UIAPacket_Hdr *ti_uia_runtime_ServiceMgr_getFreePacket__E( ti_uia_runtime_UIAPacket_HdrType type, xdc_UInt timeout );
xdc__CODESECT(ti_uia_runtime_ServiceMgr_getFreePacket__F, "ti_uia_runtime_ServiceMgr_getFreePacket")
__extern ti_uia_runtime_UIAPacket_Hdr *ti_uia_runtime_ServiceMgr_getFreePacket__F( ti_uia_runtime_UIAPacket_HdrType type, xdc_UInt timeout );

/* getNumServices__E */
#define ti_uia_runtime_ServiceMgr_getNumServices ti_uia_runtime_ServiceMgr_getNumServices__E
xdc__CODESECT(ti_uia_runtime_ServiceMgr_getNumServices__E, "ti_uia_runtime_ServiceMgr_getNumServices")
__extern xdc_Int ti_uia_runtime_ServiceMgr_getNumServices__E( void );
xdc__CODESECT(ti_uia_runtime_ServiceMgr_getNumServices__F, "ti_uia_runtime_ServiceMgr_getNumServices")
__extern xdc_Int ti_uia_runtime_ServiceMgr_getNumServices__F( void );

/* processCallback__E */
#define ti_uia_runtime_ServiceMgr_processCallback ti_uia_runtime_ServiceMgr_processCallback__E
xdc__CODESECT(ti_uia_runtime_ServiceMgr_processCallback__E, "ti_uia_runtime_ServiceMgr_processCallback")
__extern xdc_Void ti_uia_runtime_ServiceMgr_processCallback__E( ti_uia_runtime_ServiceMgr_ServiceId id, ti_uia_runtime_ServiceMgr_Reason reason, ti_uia_runtime_UIAPacket_Hdr *packet );
xdc__CODESECT(ti_uia_runtime_ServiceMgr_processCallback__F, "ti_uia_runtime_ServiceMgr_processCallback")
__extern xdc_Void ti_uia_runtime_ServiceMgr_processCallback__F( ti_uia_runtime_ServiceMgr_ServiceId id, ti_uia_runtime_ServiceMgr_Reason reason, ti_uia_runtime_UIAPacket_Hdr *packet );

/* requestEnergy__E */
#define ti_uia_runtime_ServiceMgr_requestEnergy ti_uia_runtime_ServiceMgr_requestEnergy__E
xdc__CODESECT(ti_uia_runtime_ServiceMgr_requestEnergy__E, "ti_uia_runtime_ServiceMgr_requestEnergy")
__extern xdc_Void ti_uia_runtime_ServiceMgr_requestEnergy__E( ti_uia_runtime_ServiceMgr_ServiceId id );
xdc__CODESECT(ti_uia_runtime_ServiceMgr_requestEnergy__F, "ti_uia_runtime_ServiceMgr_requestEnergy")
__extern xdc_Void ti_uia_runtime_ServiceMgr_requestEnergy__F( ti_uia_runtime_ServiceMgr_ServiceId id );

/* sendPacket__E */
#define ti_uia_runtime_ServiceMgr_sendPacket ti_uia_runtime_ServiceMgr_sendPacket__E
xdc__CODESECT(ti_uia_runtime_ServiceMgr_sendPacket__E, "ti_uia_runtime_ServiceMgr_sendPacket")
__extern xdc_Bool ti_uia_runtime_ServiceMgr_sendPacket__E( ti_uia_runtime_UIAPacket_Hdr *packet );
xdc__CODESECT(ti_uia_runtime_ServiceMgr_sendPacket__F, "ti_uia_runtime_ServiceMgr_sendPacket")
__extern xdc_Bool ti_uia_runtime_ServiceMgr_sendPacket__F( ti_uia_runtime_UIAPacket_Hdr *packet );

/* setPeriod__E */
#define ti_uia_runtime_ServiceMgr_setPeriod ti_uia_runtime_ServiceMgr_setPeriod__E
xdc__CODESECT(ti_uia_runtime_ServiceMgr_setPeriod__E, "ti_uia_runtime_ServiceMgr_setPeriod")
__extern xdc_Void ti_uia_runtime_ServiceMgr_setPeriod__E( ti_uia_runtime_ServiceMgr_ServiceId id, xdc_UInt32 periodInMs );
xdc__CODESECT(ti_uia_runtime_ServiceMgr_setPeriod__F, "ti_uia_runtime_ServiceMgr_setPeriod")
__extern xdc_Void ti_uia_runtime_ServiceMgr_setPeriod__F( ti_uia_runtime_ServiceMgr_ServiceId id, xdc_UInt32 periodInMs );


/*
 * ======== SYSTEM FUNCTIONS ========
 */

/* Module_startupDone */
#define ti_uia_runtime_ServiceMgr_Module_startupDone() ti_uia_runtime_ServiceMgr_Module__startupDone__S()

/* Object_heap */
#define ti_uia_runtime_ServiceMgr_Object_heap() ti_uia_runtime_ServiceMgr_Object__heap__C

/* Module_heap */
#define ti_uia_runtime_ServiceMgr_Module_heap() ti_uia_runtime_ServiceMgr_Object__heap__C

/* Module_id */
static inline CT__ti_uia_runtime_ServiceMgr_Module__id ti_uia_runtime_ServiceMgr_Module_id( void ) 
{
    return ti_uia_runtime_ServiceMgr_Module__id__C;
}

/* Module_hasMask */
static inline xdc_Bool ti_uia_runtime_ServiceMgr_Module_hasMask( void ) 
{
    return ti_uia_runtime_ServiceMgr_Module__diagsMask__C != NULL;
}

/* Module_getMask */
static inline xdc_Bits16 ti_uia_runtime_ServiceMgr_Module_getMask( void ) 
{
    return ti_uia_runtime_ServiceMgr_Module__diagsMask__C != NULL ? *ti_uia_runtime_ServiceMgr_Module__diagsMask__C : 0;
}

/* Module_setMask */
static inline xdc_Void ti_uia_runtime_ServiceMgr_Module_setMask( xdc_Bits16 mask ) 
{
    if (ti_uia_runtime_ServiceMgr_Module__diagsMask__C != NULL) *ti_uia_runtime_ServiceMgr_Module__diagsMask__C = mask;
}


/*
 * ======== EPILOGUE ========
 */

#include <ti/uia/runtime/ServiceMgr__epilogue.h>

#ifdef ti_uia_runtime_ServiceMgr__top__
#undef __nested__
#endif

#endif /* ti_uia_runtime_ServiceMgr__include */


/*
 * ======== STATE STRUCTURES ========
 */

#if defined(__config__) || (!defined(__nested__) && defined(ti_uia_runtime_ServiceMgr__internalaccess))

#ifndef ti_uia_runtime_ServiceMgr__include_state
#define ti_uia_runtime_ServiceMgr__include_state

/* Module_State */
struct ti_uia_runtime_ServiceMgr_Module_State {
    xdc_Int runCount;
    xdc_Int numServices;
};

/* Module__state__V */
extern struct ti_uia_runtime_ServiceMgr_Module_State__ ti_uia_runtime_ServiceMgr_Module__state__V;

#endif /* ti_uia_runtime_ServiceMgr__include_state */

#endif


/*
 * ======== PREFIX ALIASES ========
 */

#if !defined(__nested__) && !defined(ti_uia_runtime_ServiceMgr__nolocalnames)

#ifndef ti_uia_runtime_ServiceMgr__localnames__done
#define ti_uia_runtime_ServiceMgr__localnames__done

/* module prefix */
#define ServiceMgr_Reason ti_uia_runtime_ServiceMgr_Reason
#define ServiceMgr_Topology ti_uia_runtime_ServiceMgr_Topology
#define ServiceMgr_ProcessCallback ti_uia_runtime_ServiceMgr_ProcessCallback
#define ServiceMgr_WAIT_FOREVER ti_uia_runtime_ServiceMgr_WAIT_FOREVER
#define ServiceMgr_ServiceId ti_uia_runtime_ServiceMgr_ServiceId
#define ServiceMgr_Module_State ti_uia_runtime_ServiceMgr_Module_State
#define ServiceMgr_Reason_PERIODEXPIRED ti_uia_runtime_ServiceMgr_Reason_PERIODEXPIRED
#define ServiceMgr_Reason_REQUESTENERGY ti_uia_runtime_ServiceMgr_Reason_REQUESTENERGY
#define ServiceMgr_Reason_INCOMINGMSG ti_uia_runtime_ServiceMgr_Reason_INCOMINGMSG
#define ServiceMgr_Topology_SINGLECORE ti_uia_runtime_ServiceMgr_Topology_SINGLECORE
#define ServiceMgr_Topology_MULTICORE ti_uia_runtime_ServiceMgr_Topology_MULTICORE
#define ServiceMgr_A_invalidServiceId ti_uia_runtime_ServiceMgr_A_invalidServiceId
#define ServiceMgr_A_invalidProcessCallbackFxn ti_uia_runtime_ServiceMgr_A_invalidProcessCallbackFxn
#define ServiceMgr_transportFxns ti_uia_runtime_ServiceMgr_transportFxns
#define ServiceMgr_topology ti_uia_runtime_ServiceMgr_topology
#define ServiceMgr_periodInMs ti_uia_runtime_ServiceMgr_periodInMs
#define ServiceMgr_maxEventPacketSize ti_uia_runtime_ServiceMgr_maxEventPacketSize
#define ServiceMgr_numEventPacketBufs ti_uia_runtime_ServiceMgr_numEventPacketBufs
#define ServiceMgr_maxCtrlPacketSize ti_uia_runtime_ServiceMgr_maxCtrlPacketSize
#define ServiceMgr_numOutgoingCtrlPacketBufs ti_uia_runtime_ServiceMgr_numOutgoingCtrlPacketBufs
#define ServiceMgr_numIncomingCtrlPacketBufs ti_uia_runtime_ServiceMgr_numIncomingCtrlPacketBufs
#define ServiceMgr_supportControl ti_uia_runtime_ServiceMgr_supportControl
#define ServiceMgr_transferAgentPriority ti_uia_runtime_ServiceMgr_transferAgentPriority
#define ServiceMgr_transferAgentStackSize ti_uia_runtime_ServiceMgr_transferAgentStackSize
#define ServiceMgr_rxTaskPriority ti_uia_runtime_ServiceMgr_rxTaskPriority
#define ServiceMgr_rxTaskStackSize ti_uia_runtime_ServiceMgr_rxTaskStackSize
#define ServiceMgr_masterProcId ti_uia_runtime_ServiceMgr_masterProcId
#define ServiceMgr_processCallbackFxn ti_uia_runtime_ServiceMgr_processCallbackFxn
#define ServiceMgr_freePacket ti_uia_runtime_ServiceMgr_freePacket
#define ServiceMgr_getFreePacket ti_uia_runtime_ServiceMgr_getFreePacket
#define ServiceMgr_getNumServices ti_uia_runtime_ServiceMgr_getNumServices
#define ServiceMgr_processCallback ti_uia_runtime_ServiceMgr_processCallback
#define ServiceMgr_requestEnergy ti_uia_runtime_ServiceMgr_requestEnergy
#define ServiceMgr_sendPacket ti_uia_runtime_ServiceMgr_sendPacket
#define ServiceMgr_setPeriod ti_uia_runtime_ServiceMgr_setPeriod
#define ServiceMgr_Module_name ti_uia_runtime_ServiceMgr_Module_name
#define ServiceMgr_Module_id ti_uia_runtime_ServiceMgr_Module_id
#define ServiceMgr_Module_startup ti_uia_runtime_ServiceMgr_Module_startup
#define ServiceMgr_Module_startupDone ti_uia_runtime_ServiceMgr_Module_startupDone
#define ServiceMgr_Module_hasMask ti_uia_runtime_ServiceMgr_Module_hasMask
#define ServiceMgr_Module_getMask ti_uia_runtime_ServiceMgr_Module_getMask
#define ServiceMgr_Module_setMask ti_uia_runtime_ServiceMgr_Module_setMask
#define ServiceMgr_Object_heap ti_uia_runtime_ServiceMgr_Object_heap
#define ServiceMgr_Module_heap ti_uia_runtime_ServiceMgr_Module_heap

/* proxies */
#include <ti/uia/runtime/package/ServiceMgr_SupportProxy.h>

#endif /* ti_uia_runtime_ServiceMgr__localnames__done */
#endif
