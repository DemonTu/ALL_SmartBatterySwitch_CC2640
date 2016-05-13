/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-A71
 */

#ifndef ti_uia_services_Rta__INTERNAL__
#define ti_uia_services_Rta__INTERNAL__

#ifndef ti_uia_services_Rta__internalaccess
#define ti_uia_services_Rta__internalaccess
#endif

#include <ti/uia/services/Rta.h>

#undef xdc_FILE__
#ifndef xdc_FILE
#define xdc_FILE__ NULL
#else
#define xdc_FILE__ xdc_FILE
#endif

/* processCallback */
#undef ti_uia_services_Rta_processCallback
#define ti_uia_services_Rta_processCallback ti_uia_services_Rta_processCallback__F

/* disableAllLogs */
#undef ti_uia_services_Rta_disableAllLogs
#define ti_uia_services_Rta_disableAllLogs ti_uia_services_Rta_disableAllLogs__F

/* enableAllLogs */
#undef ti_uia_services_Rta_enableAllLogs
#define ti_uia_services_Rta_enableAllLogs ti_uia_services_Rta_enableAllLogs__F

/* snapshotAllLogs */
#undef ti_uia_services_Rta_snapshotAllLogs
#define ti_uia_services_Rta_snapshotAllLogs ti_uia_services_Rta_snapshotAllLogs__F

/* resetAllLogs */
#undef ti_uia_services_Rta_resetAllLogs
#define ti_uia_services_Rta_resetAllLogs ti_uia_services_Rta_resetAllLogs__F

/* startDataTx */
#undef ti_uia_services_Rta_startDataTx
#define ti_uia_services_Rta_startDataTx ti_uia_services_Rta_startDataTx__F

/* stopDataTx */
#undef ti_uia_services_Rta_stopDataTx
#define ti_uia_services_Rta_stopDataTx ti_uia_services_Rta_stopDataTx__F

/* sendEvents */
#define Rta_sendEvents ti_uia_services_Rta_sendEvents__I

/* processMsg */
#define Rta_processMsg ti_uia_services_Rta_processMsg__I

/* flushLogger */
#define Rta_flushLogger ti_uia_services_Rta_flushLogger__I

/* acknowledgeCmd */
#define Rta_acknowledgeCmd ti_uia_services_Rta_acknowledgeCmd__I

/* readMask */
#define Rta_readMask ti_uia_services_Rta_readMask__I

/* writeMask */
#define Rta_writeMask ti_uia_services_Rta_writeMask__I

/* enableLog */
#define Rta_enableLog ti_uia_services_Rta_enableLog__I

/* disableLog */
#define Rta_disableLog ti_uia_services_Rta_disableLog__I

/* getCpuSpeed */
#define Rta_getCpuSpeed ti_uia_services_Rta_getCpuSpeed__I

/* resetLog */
#define Rta_resetLog ti_uia_services_Rta_resetLog__I

/* changePeriod */
#define Rta_changePeriod ti_uia_services_Rta_changePeriod__I

/* Module_startup */
#undef ti_uia_services_Rta_Module_startup
#define ti_uia_services_Rta_Module_startup ti_uia_services_Rta_Module_startup__E

/* Instance_init */
#undef ti_uia_services_Rta_Instance_init
#define ti_uia_services_Rta_Instance_init ti_uia_services_Rta_Instance_init__E

/* Instance_finalize */
#undef ti_uia_services_Rta_Instance_finalize
#define ti_uia_services_Rta_Instance_finalize ti_uia_services_Rta_Instance_finalize__E

/* module */
#define Rta_module ((ti_uia_services_Rta_Module_State *)(xdc__MODOBJADDR__(ti_uia_services_Rta_Module__state__V)))
#if !defined(__cplusplus) || !defined(ti_uia_services_Rta__cplusplus)
#define module ((ti_uia_services_Rta_Module_State *)(xdc__MODOBJADDR__(ti_uia_services_Rta_Module__state__V)))
#endif
/* per-module runtime symbols */
#undef Module__MID
#define Module__MID ti_uia_services_Rta_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL ti_uia_services_Rta_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB ti_uia_services_Rta_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK ti_uia_services_Rta_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF ti_uia_services_Rta_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ ti_uia_services_Rta_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 ti_uia_services_Rta_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 ti_uia_services_Rta_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 ti_uia_services_Rta_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 ti_uia_services_Rta_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 ti_uia_services_Rta_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ ti_uia_services_Rta_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS ti_uia_services_Rta_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create ti_uia_services_Rta_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_uia_services_Rta_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_uia_services_Rta_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_uia_services_Rta_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_uia_services_Rta_Module_GateProxy_query


#endif /* ti_uia_services_Rta__INTERNAL____ */
