/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-A71
 */

#ifndef ti_uia_runtime_LogSync__INTERNAL__
#define ti_uia_runtime_LogSync__INTERNAL__

#ifndef ti_uia_runtime_LogSync__internalaccess
#define ti_uia_runtime_LogSync__internalaccess
#endif

#include <ti/uia/runtime/LogSync.h>

#undef xdc_FILE__
#ifndef xdc_FILE
#define xdc_FILE__ NULL
#else
#define xdc_FILE__ xdc_FILE
#endif

/* enable */
#undef ti_uia_runtime_LogSync_enable
#define ti_uia_runtime_LogSync_enable ti_uia_runtime_LogSync_enable__E

/* disable */
#undef ti_uia_runtime_LogSync_disable
#define ti_uia_runtime_LogSync_disable ti_uia_runtime_LogSync_disable__E

/* idleHook */
#undef ti_uia_runtime_LogSync_idleHook
#define ti_uia_runtime_LogSync_idleHook ti_uia_runtime_LogSync_idleHook__E

/* timerHook */
#undef ti_uia_runtime_LogSync_timerHook
#define ti_uia_runtime_LogSync_timerHook ti_uia_runtime_LogSync_timerHook__E

/* putSyncPoint */
#undef ti_uia_runtime_LogSync_putSyncPoint
#define ti_uia_runtime_LogSync_putSyncPoint ti_uia_runtime_LogSync_putSyncPoint__E

/* isSyncEventRequired */
#undef ti_uia_runtime_LogSync_isSyncEventRequired
#define ti_uia_runtime_LogSync_isSyncEventRequired ti_uia_runtime_LogSync_isSyncEventRequired__E

/* Module_startup */
#undef ti_uia_runtime_LogSync_Module_startup
#define ti_uia_runtime_LogSync_Module_startup ti_uia_runtime_LogSync_Module_startup__E

/* Instance_init */
#undef ti_uia_runtime_LogSync_Instance_init
#define ti_uia_runtime_LogSync_Instance_init ti_uia_runtime_LogSync_Instance_init__E

/* Instance_finalize */
#undef ti_uia_runtime_LogSync_Instance_finalize
#define ti_uia_runtime_LogSync_Instance_finalize ti_uia_runtime_LogSync_Instance_finalize__E

/* module */
#define LogSync_module ((ti_uia_runtime_LogSync_Module_State *)(xdc__MODOBJADDR__(ti_uia_runtime_LogSync_Module__state__V)))
#if !defined(__cplusplus) || !defined(ti_uia_runtime_LogSync__cplusplus)
#define module ((ti_uia_runtime_LogSync_Module_State *)(xdc__MODOBJADDR__(ti_uia_runtime_LogSync_Module__state__V)))
#endif
/* per-module runtime symbols */
#undef Module__MID
#define Module__MID ti_uia_runtime_LogSync_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL ti_uia_runtime_LogSync_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB ti_uia_runtime_LogSync_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK ti_uia_runtime_LogSync_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF ti_uia_runtime_LogSync_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ ti_uia_runtime_LogSync_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 ti_uia_runtime_LogSync_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 ti_uia_runtime_LogSync_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 ti_uia_runtime_LogSync_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 ti_uia_runtime_LogSync_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 ti_uia_runtime_LogSync_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ ti_uia_runtime_LogSync_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS ti_uia_runtime_LogSync_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create ti_uia_runtime_LogSync_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_uia_runtime_LogSync_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_uia_runtime_LogSync_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_uia_runtime_LogSync_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_uia_runtime_LogSync_Module_GateProxy_query

/* Object__sizingError */
#line 1 "Error_inconsistent_object_size_in_ti.uia.runtime.LogSync"
typedef char ti_uia_runtime_LogSync_Object__sizingError[sizeof(ti_uia_runtime_LogSync_Object) > sizeof(ti_uia_runtime_LogSync_Struct) ? -1 : 1];


#endif /* ti_uia_runtime_LogSync__INTERNAL____ */
