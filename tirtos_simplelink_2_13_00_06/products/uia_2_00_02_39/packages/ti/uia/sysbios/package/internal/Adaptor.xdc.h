/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-A71
 */

#ifndef ti_uia_sysbios_Adaptor__INTERNAL__
#define ti_uia_sysbios_Adaptor__INTERNAL__

#ifndef ti_uia_sysbios_Adaptor__internalaccess
#define ti_uia_sysbios_Adaptor__internalaccess
#endif

#include <ti/uia/sysbios/Adaptor.h>

#undef xdc_FILE__
#ifndef xdc_FILE
#define xdc_FILE__ NULL
#else
#define xdc_FILE__ xdc_FILE
#endif

/* freePacket */
#undef ti_uia_sysbios_Adaptor_freePacket
#define ti_uia_sysbios_Adaptor_freePacket ti_uia_sysbios_Adaptor_freePacket__E

/* getFreePacket */
#undef ti_uia_sysbios_Adaptor_getFreePacket
#define ti_uia_sysbios_Adaptor_getFreePacket ti_uia_sysbios_Adaptor_getFreePacket__E

/* requestEnergy */
#undef ti_uia_sysbios_Adaptor_requestEnergy
#define ti_uia_sysbios_Adaptor_requestEnergy ti_uia_sysbios_Adaptor_requestEnergy__E

/* sendPacket */
#undef ti_uia_sysbios_Adaptor_sendPacket
#define ti_uia_sysbios_Adaptor_sendPacket ti_uia_sysbios_Adaptor_sendPacket__E

/* setPeriod */
#undef ti_uia_sysbios_Adaptor_setPeriod
#define ti_uia_sysbios_Adaptor_setPeriod ti_uia_sysbios_Adaptor_setPeriod__E

/* rxTaskFxn */
#undef ti_uia_sysbios_Adaptor_rxTaskFxn
#define ti_uia_sysbios_Adaptor_rxTaskFxn ti_uia_sysbios_Adaptor_rxTaskFxn__E

/* transferAgentTaskFxn */
#undef ti_uia_sysbios_Adaptor_transferAgentTaskFxn
#define ti_uia_sysbios_Adaptor_transferAgentTaskFxn ti_uia_sysbios_Adaptor_transferAgentTaskFxn__E

/* clockFxn */
#undef ti_uia_sysbios_Adaptor_clockFxn
#define ti_uia_sysbios_Adaptor_clockFxn ti_uia_sysbios_Adaptor_clockFxn__E

/* giveEnergy */
#define Adaptor_giveEnergy ti_uia_sysbios_Adaptor_giveEnergy__I

/* sendToHost */
#define Adaptor_sendToHost ti_uia_sysbios_Adaptor_sendToHost__I

/* sendToService */
#define Adaptor_sendToService ti_uia_sysbios_Adaptor_sendToService__I

/* runScheduledServices */
#define Adaptor_runScheduledServices ti_uia_sysbios_Adaptor_runScheduledServices__I

/* Module_startup */
#undef ti_uia_sysbios_Adaptor_Module_startup
#define ti_uia_sysbios_Adaptor_Module_startup ti_uia_sysbios_Adaptor_Module_startup__E

/* Instance_init */
#undef ti_uia_sysbios_Adaptor_Instance_init
#define ti_uia_sysbios_Adaptor_Instance_init ti_uia_sysbios_Adaptor_Instance_init__E

/* Instance_finalize */
#undef ti_uia_sysbios_Adaptor_Instance_finalize
#define ti_uia_sysbios_Adaptor_Instance_finalize ti_uia_sysbios_Adaptor_Instance_finalize__E

/* module */
#define Adaptor_module ((ti_uia_sysbios_Adaptor_Module_State *)(xdc__MODOBJADDR__(ti_uia_sysbios_Adaptor_Module__state__V)))
#if !defined(__cplusplus) || !defined(ti_uia_sysbios_Adaptor__cplusplus)
#define module ((ti_uia_sysbios_Adaptor_Module_State *)(xdc__MODOBJADDR__(ti_uia_sysbios_Adaptor_Module__state__V)))
#endif
/* per-module runtime symbols */
#undef Module__MID
#define Module__MID ti_uia_sysbios_Adaptor_Module__id__C
#undef Module__DGSINCL
#define Module__DGSINCL ti_uia_sysbios_Adaptor_Module__diagsIncluded__C
#undef Module__DGSENAB
#define Module__DGSENAB ti_uia_sysbios_Adaptor_Module__diagsEnabled__C
#undef Module__DGSMASK
#define Module__DGSMASK ti_uia_sysbios_Adaptor_Module__diagsMask__C
#undef Module__LOGDEF
#define Module__LOGDEF ti_uia_sysbios_Adaptor_Module__loggerDefined__C
#undef Module__LOGOBJ
#define Module__LOGOBJ ti_uia_sysbios_Adaptor_Module__loggerObj__C
#undef Module__LOGFXN0
#define Module__LOGFXN0 ti_uia_sysbios_Adaptor_Module__loggerFxn0__C
#undef Module__LOGFXN1
#define Module__LOGFXN1 ti_uia_sysbios_Adaptor_Module__loggerFxn1__C
#undef Module__LOGFXN2
#define Module__LOGFXN2 ti_uia_sysbios_Adaptor_Module__loggerFxn2__C
#undef Module__LOGFXN4
#define Module__LOGFXN4 ti_uia_sysbios_Adaptor_Module__loggerFxn4__C
#undef Module__LOGFXN8
#define Module__LOGFXN8 ti_uia_sysbios_Adaptor_Module__loggerFxn8__C
#undef Module__G_OBJ
#define Module__G_OBJ ti_uia_sysbios_Adaptor_Module__gateObj__C
#undef Module__G_PRMS
#define Module__G_PRMS ti_uia_sysbios_Adaptor_Module__gatePrms__C
#undef Module__GP_create
#define Module__GP_create ti_uia_sysbios_Adaptor_Module_GateProxy_create
#undef Module__GP_delete
#define Module__GP_delete ti_uia_sysbios_Adaptor_Module_GateProxy_delete
#undef Module__GP_enter
#define Module__GP_enter ti_uia_sysbios_Adaptor_Module_GateProxy_enter
#undef Module__GP_leave
#define Module__GP_leave ti_uia_sysbios_Adaptor_Module_GateProxy_leave
#undef Module__GP_query
#define Module__GP_query ti_uia_sysbios_Adaptor_Module_GateProxy_query


#endif /* ti_uia_sysbios_Adaptor__INTERNAL____ */
