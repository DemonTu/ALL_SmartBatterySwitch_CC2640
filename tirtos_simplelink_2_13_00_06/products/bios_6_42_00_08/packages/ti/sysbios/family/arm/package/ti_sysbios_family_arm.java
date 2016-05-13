/*
 *  Do not modify this file; it is automatically 
 *  generated and any modifications will be overwritten.
 *
 * @(#) xdc-A71
 */
import java.util.*;
import org.mozilla.javascript.*;
import xdc.services.intern.xsr.*;
import xdc.services.spec.Session;

public class ti_sysbios_family_arm
{
    static final String VERS = "@(#) xdc-A71\n";

    static final Proto.Elm $$T_Bool = Proto.Elm.newBool();
    static final Proto.Elm $$T_Num = Proto.Elm.newNum();
    static final Proto.Elm $$T_Str = Proto.Elm.newStr();
    static final Proto.Elm $$T_Obj = Proto.Elm.newObj();

    static final Proto.Fxn $$T_Met = new Proto.Fxn(null, null, 0, -1, false);
    static final Proto.Map $$T_Map = new Proto.Map($$T_Obj);
    static final Proto.Arr $$T_Vec = new Proto.Arr($$T_Obj);

    static final XScriptO $$DEFAULT = Value.DEFAULT;
    static final Object $$UNDEF = Undefined.instance;

    static final Proto.Obj $$Package = (Proto.Obj)Global.get("$$Package");
    static final Proto.Obj $$Module = (Proto.Obj)Global.get("$$Module");
    static final Proto.Obj $$Instance = (Proto.Obj)Global.get("$$Instance");
    static final Proto.Obj $$Params = (Proto.Obj)Global.get("$$Params");

    static final Object $$objFldGet = Global.get("$$objFldGet");
    static final Object $$objFldSet = Global.get("$$objFldSet");
    static final Object $$proxyGet = Global.get("$$proxyGet");
    static final Object $$proxySet = Global.get("$$proxySet");
    static final Object $$delegGet = Global.get("$$delegGet");
    static final Object $$delegSet = Global.get("$$delegSet");

    Scriptable xdcO;
    Session ses;
    Value.Obj om;

    boolean isROV;
    boolean isCFG;

    Proto.Obj pkgP;
    Value.Obj pkgV;

    ArrayList<Object> imports = new ArrayList<Object>();
    ArrayList<Object> loggables = new ArrayList<Object>();
    ArrayList<Object> mcfgs = new ArrayList<Object>();
    ArrayList<Object> icfgs = new ArrayList<Object>();
    ArrayList<String> inherits = new ArrayList<String>();
    ArrayList<Object> proxies = new ArrayList<Object>();
    ArrayList<Object> sizes = new ArrayList<Object>();
    ArrayList<Object> tdefs = new ArrayList<Object>();

    void $$IMPORTS()
    {
        Global.callFxn("loadPackage", xdcO, "ti.sysbios.interfaces");
        Global.callFxn("loadPackage", xdcO, "xdc");
        Global.callFxn("loadPackage", xdcO, "xdc.corevers");
        Global.callFxn("loadPackage", xdcO, "xdc.runtime");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("ti.sysbios.family.arm.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.sysbios.family.arm", new Value.Obj("ti.sysbios.family.arm", pkgP));
    }

    void HwiCommon$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.HwiCommon.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.arm.HwiCommon", new Value.Obj("ti.sysbios.family.arm.HwiCommon", po));
        pkgV.bind("HwiCommon", vo);
        // decls 
        spo = (Proto.Obj)om.bind("ti.sysbios.family.arm.HwiCommon$$Module_State", new Proto.Obj());
        om.bind("ti.sysbios.family.arm.HwiCommon.Module_State", new Proto.Str(spo, false));
    }

    void IntrinsicsSupport$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.IntrinsicsSupport.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.arm.IntrinsicsSupport", new Value.Obj("ti.sysbios.family.arm.IntrinsicsSupport", po));
        pkgV.bind("IntrinsicsSupport", vo);
        // decls 
    }

    void Settings$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.Settings.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.arm.Settings", new Value.Obj("ti.sysbios.family.arm.Settings", po));
        pkgV.bind("Settings", vo);
        // decls 
    }

    void TaskSupport$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.sysbios.family.arm.TaskSupport.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.sysbios.family.arm.TaskSupport", new Value.Obj("ti.sysbios.family.arm.TaskSupport", po));
        pkgV.bind("TaskSupport", vo);
        // decls 
    }

    void HwiCommon$$CONSTS()
    {
        // module HwiCommon
        om.bind("ti.sysbios.family.arm.HwiCommon.enable", new Extern("ti_sysbios_family_arm_HwiCommon_enable__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.disable", new Extern("ti_sysbios_family_arm_HwiCommon_disable__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.restore", new Extern("ti_sysbios_family_arm_HwiCommon_restore__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.enableFIQ", new Extern("ti_sysbios_family_arm_HwiCommon_enableFIQ__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.disableFIQ", new Extern("ti_sysbios_family_arm_HwiCommon_disableFIQ__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.restoreFIQ", new Extern("ti_sysbios_family_arm_HwiCommon_restoreFIQ__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.enableIRQ", new Extern("ti_sysbios_family_arm_HwiCommon_enableIRQ__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.disableIRQ", new Extern("ti_sysbios_family_arm_HwiCommon_disableIRQ__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.restoreIRQ", new Extern("ti_sysbios_family_arm_HwiCommon_restoreIRQ__E", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.getTaskSP", new Extern("ti_sysbios_family_arm_HwiCommon_getTaskSP__E", "xdc_Char*(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.switchFromBootStack", new Extern("ti_sysbios_family_arm_HwiCommon_switchFromBootStack__E", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.switchToSystemMode", new Extern("ti_sysbios_family_arm_HwiCommon_switchToSystemMode__I", "xdc_Void(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.enableA", new Extern("ti_sysbios_family_arm_HwiCommon_enableA__I", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.disableA", new Extern("ti_sysbios_family_arm_HwiCommon_disableA__I", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.restoreA", new Extern("ti_sysbios_family_arm_HwiCommon_restoreA__I", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.enableFIQA", new Extern("ti_sysbios_family_arm_HwiCommon_enableFIQA__I", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.disableFIQA", new Extern("ti_sysbios_family_arm_HwiCommon_disableFIQA__I", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.restoreFIQA", new Extern("ti_sysbios_family_arm_HwiCommon_restoreFIQA__I", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.enableIRQA", new Extern("ti_sysbios_family_arm_HwiCommon_enableIRQA__I", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.disableIRQA", new Extern("ti_sysbios_family_arm_HwiCommon_disableIRQA__I", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.restoreIRQA", new Extern("ti_sysbios_family_arm_HwiCommon_restoreIRQA__I", "xdc_Void(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.HwiCommon.swiHandler", new Extern("ti_sysbios_family_arm_HwiCommon_swiHandler__I", "xdc_Void(*)(xdc_Void)", true, false));
    }

    void IntrinsicsSupport$$CONSTS()
    {
        // module IntrinsicsSupport
        om.bind("ti.sysbios.family.arm.IntrinsicsSupport.maxbit", new Extern("ti_sysbios_family_arm_IntrinsicsSupport_maxbit__E", "xdc_UInt(*)(xdc_UInt)", true, false));
        om.bind("ti.sysbios.family.arm.IntrinsicsSupport.maxbitA", new Extern("ti_sysbios_family_arm_IntrinsicsSupport_maxbitA__I", "xdc_UInt(*)(xdc_UInt)", true, false));
    }

    void Settings$$CONSTS()
    {
        // module Settings
    }

    void TaskSupport$$CONSTS()
    {
        // module TaskSupport
        om.bind("ti.sysbios.family.arm.TaskSupport.start", new Extern("ti_sysbios_family_arm_TaskSupport_start__E", "xdc_Ptr(*)(xdc_Ptr,xdc_Void(*)(xdc_Void),xdc_Void(*)(xdc_Void),xdc_runtime_Error_Block*)", true, false));
        om.bind("ti.sysbios.family.arm.TaskSupport.swap", new Extern("ti_sysbios_family_arm_TaskSupport_swap__E", "xdc_Void(*)(xdc_Ptr*,xdc_Ptr*)", true, false));
        om.bind("ti.sysbios.family.arm.TaskSupport.checkStack", new Extern("ti_sysbios_family_arm_TaskSupport_checkStack__E", "xdc_Bool(*)(xdc_Char*,xdc_SizeT)", true, false));
        om.bind("ti.sysbios.family.arm.TaskSupport.stackUsed", new Extern("ti_sysbios_family_arm_TaskSupport_stackUsed__E", "xdc_SizeT(*)(xdc_Char*,xdc_SizeT)", true, false));
        om.bind("ti.sysbios.family.arm.TaskSupport.getStackAlignment", new Extern("ti_sysbios_family_arm_TaskSupport_getStackAlignment__E", "xdc_UInt(*)(xdc_Void)", true, false));
        om.bind("ti.sysbios.family.arm.TaskSupport.getDefaultStackSize", new Extern("ti_sysbios_family_arm_TaskSupport_getDefaultStackSize__E", "xdc_SizeT(*)(xdc_Void)", true, false));
    }

    void HwiCommon$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void IntrinsicsSupport$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Settings$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void TaskSupport$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void HwiCommon$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void IntrinsicsSupport$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Settings$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TaskSupport$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void HwiCommon$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.sysbios.family.arm.HwiCommon.Module_State", "ti.sysbios.family.arm");
        sizes.clear();
        sizes.add(Global.newArray("taskSP", "UPtr"));
        sizes.add(Global.newArray("isrStack", "UPtr"));
        sizes.add(Global.newArray("isrStackBase", "UPtr"));
        sizes.add(Global.newArray("isrStackSize", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.sysbios.family.arm.HwiCommon.Module_State']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.sysbios.family.arm.HwiCommon.Module_State']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.sysbios.family.arm.HwiCommon.Module_State'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void IntrinsicsSupport$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void Settings$$SIZES()
    {
    }

    void TaskSupport$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void HwiCommon$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/HwiCommon.xs");
        om.bind("ti.sysbios.family.arm.HwiCommon$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.HwiCommon.Module", "ti.sysbios.family.arm");
        po.init("ti.sysbios.family.arm.HwiCommon.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.sysbios.family.arm"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.arm.HwiCommon$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.HwiCommon$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.HwiCommon$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.arm.HwiCommon$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        // struct HwiCommon.Module_State
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.HwiCommon$$Module_State", "ti.sysbios.family.arm");
        po.init("ti.sysbios.family.arm.HwiCommon.Module_State", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("taskSP", new Proto.Adr("xdc_Char*", "Pn"), $$UNDEF, "w");
                po.addFld("isrStack", new Proto.Adr("xdc_Char*", "Pn"), $$UNDEF, "w");
                po.addFld("isrStackBase", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("isrStackSize", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
    }

    void IntrinsicsSupport$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/IntrinsicsSupport.xs");
        om.bind("ti.sysbios.family.arm.IntrinsicsSupport$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.IntrinsicsSupport.Module", "ti.sysbios.family.arm");
        po.init("ti.sysbios.family.arm.IntrinsicsSupport.Module", om.findStrict("ti.sysbios.interfaces.IIntrinsicsSupport.Module", "ti.sysbios.family.arm"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.arm.IntrinsicsSupport$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.IntrinsicsSupport$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.IntrinsicsSupport$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.arm.IntrinsicsSupport$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
    }

    void Settings$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/Settings.xs");
        om.bind("ti.sysbios.family.arm.Settings$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.Settings.Module", "ti.sysbios.family.arm");
        po.init("ti.sysbios.family.arm.Settings.Module", om.findStrict("ti.sysbios.interfaces.ISettings.Module", "ti.sysbios.family.arm"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.arm.Settings$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.Settings$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.arm.Settings$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                fxn = Global.get(cap, "getDefaultCoreDelegate");
                if (fxn != null) po.addFxn("getDefaultCoreDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultCoreDelegate", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getDefaultHwiDelegate");
                if (fxn != null) po.addFxn("getDefaultHwiDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultHwiDelegate", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getDefaultTimerDelegate");
                if (fxn != null) po.addFxn("getDefaultTimerDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultTimerDelegate", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getDefaultClockTimerDelegate");
                if (fxn != null) po.addFxn("getDefaultClockTimerDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultClockTimerDelegate", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getDefaultTimerSupportDelegate");
                if (fxn != null) po.addFxn("getDefaultTimerSupportDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultTimerSupportDelegate", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getDefaultTimestampDelegate");
                if (fxn != null) po.addFxn("getDefaultTimestampDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultTimestampDelegate", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getDefaultTaskSupportDelegate");
                if (fxn != null) po.addFxn("getDefaultTaskSupportDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultTaskSupportDelegate", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getDefaultIntrinsicsSupportDelegate");
                if (fxn != null) po.addFxn("getDefaultIntrinsicsSupportDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultIntrinsicsSupportDelegate", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getDefaultCacheDelegate");
                if (fxn != null) po.addFxn("getDefaultCacheDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultCacheDelegate", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getDefaultPowerDelegate");
                if (fxn != null) po.addFxn("getDefaultPowerDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultPowerDelegate", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getDefaultSecondsDelegate");
                if (fxn != null) po.addFxn("getDefaultSecondsDelegate", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultSecondsDelegate", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getDefaultBootModule");
                if (fxn != null) po.addFxn("getDefaultBootModule", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultBootModule", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getDefaultMmuModule");
                if (fxn != null) po.addFxn("getDefaultMmuModule", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultMmuModule", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getFamilySettingsXml");
                if (fxn != null) po.addFxn("getFamilySettingsXml", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getFamilySettingsXml", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getDefaultClockTickPeriod");
                if (fxn != null) po.addFxn("getDefaultClockTickPeriod", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ISettings$$getDefaultClockTickPeriod", "ti.sysbios.family.arm"), fxn);
    }

    void TaskSupport$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/TaskSupport.xs");
        om.bind("ti.sysbios.family.arm.TaskSupport$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.TaskSupport.Module", "ti.sysbios.family.arm");
        po.init("ti.sysbios.family.arm.TaskSupport.Module", om.findStrict("ti.sysbios.interfaces.ITaskSupport.Module", "ti.sysbios.family.arm"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("defaultStackSize", Proto.Elm.newCNum("(xdc_SizeT)"), 2048L, "r");
            po.addFld("stackAlignment", Proto.Elm.newCNum("(xdc_UInt)"), 8L, "r");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.sysbios.family.arm.TaskSupport$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.TaskSupport$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.sysbios.family.arm.TaskSupport$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.sysbios.family.arm.TaskSupport$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                fxn = Global.get(cap, "stackUsed$view");
                if (fxn != null) po.addFxn("stackUsed$view", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ITaskSupport$$stackUsed$view", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getCallStack$view");
                if (fxn != null) po.addFxn("getCallStack$view", (Proto.Fxn)om.findStrict("ti.sysbios.interfaces.ITaskSupport$$getCallStack$view", "ti.sysbios.family.arm"), fxn);
    }

    void HwiCommon$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.HwiCommon", "ti.sysbios.family.arm");
        vo.bind("Module_State$fetchDesc", Global.newObject("type", "ti.sysbios.family.arm.HwiCommon.Module_State", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.HwiCommon$$Module_State", "ti.sysbios.family.arm");
    }

    void IntrinsicsSupport$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.IntrinsicsSupport", "ti.sysbios.family.arm");
    }

    void Settings$$ROV()
    {
    }

    void TaskSupport$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.TaskSupport", "ti.sysbios.family.arm");
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.sysbios.family.arm.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.sysbios.family.arm"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/sysbios/family/arm/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "ti.sysbios.family.arm"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "ti.sysbios.family.arm"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "ti.sysbios.family.arm", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.sysbios.family.arm");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.sysbios.family.arm.");
        pkgV.bind("$vers", Global.newArray(2, 0, 0, 0));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        imports.add(Global.newArray("ti.sysbios.interfaces", Global.newArray()));
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.sysbios.family.arm'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("if ('ti.sysbios.family.arm$$stat$base' in xdc.om) {\n");
            sb.append("pkg.packageBase = xdc.om['ti.sysbios.family.arm$$stat$base'];\n");
            sb.append("pkg.packageRepository = xdc.om['ti.sysbios.family.arm$$stat$root'];\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void HwiCommon$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.HwiCommon", "ti.sysbios.family.arm");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.HwiCommon.Module", "ti.sysbios.family.arm");
        vo.init2(po, "ti.sysbios.family.arm.HwiCommon", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.arm.HwiCommon$$capsule", "ti.sysbios.family.arm"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.arm", "ti.sysbios.family.arm"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        mcfgs.add("Module__diagsEnabled");
        icfgs.add("Module__diagsEnabled");
        mcfgs.add("Module__diagsIncluded");
        icfgs.add("Module__diagsIncluded");
        mcfgs.add("Module__diagsMask");
        icfgs.add("Module__diagsMask");
        mcfgs.add("Module__gateObj");
        icfgs.add("Module__gateObj");
        mcfgs.add("Module__gatePrms");
        icfgs.add("Module__gatePrms");
        mcfgs.add("Module__id");
        icfgs.add("Module__id");
        mcfgs.add("Module__loggerDefined");
        icfgs.add("Module__loggerDefined");
        mcfgs.add("Module__loggerObj");
        icfgs.add("Module__loggerObj");
        mcfgs.add("Module__loggerFxn0");
        icfgs.add("Module__loggerFxn0");
        mcfgs.add("Module__loggerFxn1");
        icfgs.add("Module__loggerFxn1");
        mcfgs.add("Module__loggerFxn2");
        icfgs.add("Module__loggerFxn2");
        mcfgs.add("Module__loggerFxn4");
        icfgs.add("Module__loggerFxn4");
        mcfgs.add("Module__loggerFxn8");
        icfgs.add("Module__loggerFxn8");
        mcfgs.add("Module__startupDoneFxn");
        icfgs.add("Module__startupDoneFxn");
        mcfgs.add("Object__count");
        icfgs.add("Object__count");
        mcfgs.add("Object__heap");
        icfgs.add("Object__heap");
        mcfgs.add("Object__sizeof");
        icfgs.add("Object__sizeof");
        mcfgs.add("Object__table");
        icfgs.add("Object__table");
        vo.bind("Module_State", om.findStrict("ti.sysbios.family.arm.HwiCommon.Module_State", "ti.sysbios.family.arm"));
        tdefs.add(om.findStrict("ti.sysbios.family.arm.HwiCommon.Module_State", "ti.sysbios.family.arm"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.arm")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("enable", om.findStrict("ti.sysbios.family.arm.HwiCommon.enable", "ti.sysbios.family.arm"));
        vo.bind("disable", om.findStrict("ti.sysbios.family.arm.HwiCommon.disable", "ti.sysbios.family.arm"));
        vo.bind("restore", om.findStrict("ti.sysbios.family.arm.HwiCommon.restore", "ti.sysbios.family.arm"));
        vo.bind("enableFIQ", om.findStrict("ti.sysbios.family.arm.HwiCommon.enableFIQ", "ti.sysbios.family.arm"));
        vo.bind("disableFIQ", om.findStrict("ti.sysbios.family.arm.HwiCommon.disableFIQ", "ti.sysbios.family.arm"));
        vo.bind("restoreFIQ", om.findStrict("ti.sysbios.family.arm.HwiCommon.restoreFIQ", "ti.sysbios.family.arm"));
        vo.bind("enableIRQ", om.findStrict("ti.sysbios.family.arm.HwiCommon.enableIRQ", "ti.sysbios.family.arm"));
        vo.bind("disableIRQ", om.findStrict("ti.sysbios.family.arm.HwiCommon.disableIRQ", "ti.sysbios.family.arm"));
        vo.bind("restoreIRQ", om.findStrict("ti.sysbios.family.arm.HwiCommon.restoreIRQ", "ti.sysbios.family.arm"));
        vo.bind("getTaskSP", om.findStrict("ti.sysbios.family.arm.HwiCommon.getTaskSP", "ti.sysbios.family.arm"));
        vo.bind("switchFromBootStack", om.findStrict("ti.sysbios.family.arm.HwiCommon.switchFromBootStack", "ti.sysbios.family.arm"));
        vo.bind("switchToSystemMode", om.findStrict("ti.sysbios.family.arm.HwiCommon.switchToSystemMode", "ti.sysbios.family.arm"));
        vo.bind("enableA", om.findStrict("ti.sysbios.family.arm.HwiCommon.enableA", "ti.sysbios.family.arm"));
        vo.bind("disableA", om.findStrict("ti.sysbios.family.arm.HwiCommon.disableA", "ti.sysbios.family.arm"));
        vo.bind("restoreA", om.findStrict("ti.sysbios.family.arm.HwiCommon.restoreA", "ti.sysbios.family.arm"));
        vo.bind("enableFIQA", om.findStrict("ti.sysbios.family.arm.HwiCommon.enableFIQA", "ti.sysbios.family.arm"));
        vo.bind("disableFIQA", om.findStrict("ti.sysbios.family.arm.HwiCommon.disableFIQA", "ti.sysbios.family.arm"));
        vo.bind("restoreFIQA", om.findStrict("ti.sysbios.family.arm.HwiCommon.restoreFIQA", "ti.sysbios.family.arm"));
        vo.bind("enableIRQA", om.findStrict("ti.sysbios.family.arm.HwiCommon.enableIRQA", "ti.sysbios.family.arm"));
        vo.bind("disableIRQA", om.findStrict("ti.sysbios.family.arm.HwiCommon.disableIRQA", "ti.sysbios.family.arm"));
        vo.bind("restoreIRQA", om.findStrict("ti.sysbios.family.arm.HwiCommon.restoreIRQA", "ti.sysbios.family.arm"));
        vo.bind("swiHandler", om.findStrict("ti.sysbios.family.arm.HwiCommon.swiHandler", "ti.sysbios.family.arm"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_arm_HwiCommon_Module__startupDone__E", "ti_sysbios_family_arm_HwiCommon_enable__E", "ti_sysbios_family_arm_HwiCommon_disable__E", "ti_sysbios_family_arm_HwiCommon_restore__E", "ti_sysbios_family_arm_HwiCommon_enableFIQ__E", "ti_sysbios_family_arm_HwiCommon_disableFIQ__E", "ti_sysbios_family_arm_HwiCommon_restoreFIQ__E", "ti_sysbios_family_arm_HwiCommon_enableIRQ__E", "ti_sysbios_family_arm_HwiCommon_disableIRQ__E", "ti_sysbios_family_arm_HwiCommon_restoreIRQ__E", "ti_sysbios_family_arm_HwiCommon_getTaskSP__E", "ti_sysbios_family_arm_HwiCommon_switchFromBootStack__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 1);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("HwiCommon", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("HwiCommon");
    }

    void IntrinsicsSupport$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.IntrinsicsSupport", "ti.sysbios.family.arm");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.IntrinsicsSupport.Module", "ti.sysbios.family.arm");
        vo.init2(po, "ti.sysbios.family.arm.IntrinsicsSupport", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.arm.IntrinsicsSupport$$capsule", "ti.sysbios.family.arm"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.arm", "ti.sysbios.family.arm"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        mcfgs.add("Module__diagsEnabled");
        icfgs.add("Module__diagsEnabled");
        mcfgs.add("Module__diagsIncluded");
        icfgs.add("Module__diagsIncluded");
        mcfgs.add("Module__diagsMask");
        icfgs.add("Module__diagsMask");
        mcfgs.add("Module__gateObj");
        icfgs.add("Module__gateObj");
        mcfgs.add("Module__gatePrms");
        icfgs.add("Module__gatePrms");
        mcfgs.add("Module__id");
        icfgs.add("Module__id");
        mcfgs.add("Module__loggerDefined");
        icfgs.add("Module__loggerDefined");
        mcfgs.add("Module__loggerObj");
        icfgs.add("Module__loggerObj");
        mcfgs.add("Module__loggerFxn0");
        icfgs.add("Module__loggerFxn0");
        mcfgs.add("Module__loggerFxn1");
        icfgs.add("Module__loggerFxn1");
        mcfgs.add("Module__loggerFxn2");
        icfgs.add("Module__loggerFxn2");
        mcfgs.add("Module__loggerFxn4");
        icfgs.add("Module__loggerFxn4");
        mcfgs.add("Module__loggerFxn8");
        icfgs.add("Module__loggerFxn8");
        mcfgs.add("Module__startupDoneFxn");
        icfgs.add("Module__startupDoneFxn");
        mcfgs.add("Object__count");
        icfgs.add("Object__count");
        mcfgs.add("Object__heap");
        icfgs.add("Object__heap");
        mcfgs.add("Object__sizeof");
        icfgs.add("Object__sizeof");
        mcfgs.add("Object__table");
        icfgs.add("Object__table");
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.arm")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("maxbit", om.findStrict("ti.sysbios.family.arm.IntrinsicsSupport.maxbit", "ti.sysbios.family.arm"));
        vo.bind("maxbitA", om.findStrict("ti.sysbios.family.arm.IntrinsicsSupport.maxbitA", "ti.sysbios.family.arm"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_arm_IntrinsicsSupport_Module__startupDone__E", "ti_sysbios_family_arm_IntrinsicsSupport_maxbit__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("IntrinsicsSupport", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("IntrinsicsSupport");
    }

    void Settings$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.Settings", "ti.sysbios.family.arm");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.Settings.Module", "ti.sysbios.family.arm");
        vo.init2(po, "ti.sysbios.family.arm.Settings", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.arm.Settings$$capsule", "ti.sysbios.family.arm"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.arm", "ti.sysbios.family.arm"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.arm")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.sysbios.family.arm.Settings$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        pkgV.bind("Settings", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Settings");
    }

    void TaskSupport$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.sysbios.family.arm.TaskSupport", "ti.sysbios.family.arm");
        po = (Proto.Obj)om.findStrict("ti.sysbios.family.arm.TaskSupport.Module", "ti.sysbios.family.arm");
        vo.init2(po, "ti.sysbios.family.arm.TaskSupport", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.sysbios.family.arm.TaskSupport$$capsule", "ti.sysbios.family.arm"));
        vo.bind("$package", om.findStrict("ti.sysbios.family.arm", "ti.sysbios.family.arm"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        mcfgs.add("Module__diagsEnabled");
        icfgs.add("Module__diagsEnabled");
        mcfgs.add("Module__diagsIncluded");
        icfgs.add("Module__diagsIncluded");
        mcfgs.add("Module__diagsMask");
        icfgs.add("Module__diagsMask");
        mcfgs.add("Module__gateObj");
        icfgs.add("Module__gateObj");
        mcfgs.add("Module__gatePrms");
        icfgs.add("Module__gatePrms");
        mcfgs.add("Module__id");
        icfgs.add("Module__id");
        mcfgs.add("Module__loggerDefined");
        icfgs.add("Module__loggerDefined");
        mcfgs.add("Module__loggerObj");
        icfgs.add("Module__loggerObj");
        mcfgs.add("Module__loggerFxn0");
        icfgs.add("Module__loggerFxn0");
        mcfgs.add("Module__loggerFxn1");
        icfgs.add("Module__loggerFxn1");
        mcfgs.add("Module__loggerFxn2");
        icfgs.add("Module__loggerFxn2");
        mcfgs.add("Module__loggerFxn4");
        icfgs.add("Module__loggerFxn4");
        mcfgs.add("Module__loggerFxn8");
        icfgs.add("Module__loggerFxn8");
        mcfgs.add("Module__startupDoneFxn");
        icfgs.add("Module__startupDoneFxn");
        mcfgs.add("Object__count");
        icfgs.add("Object__count");
        mcfgs.add("Object__heap");
        icfgs.add("Object__heap");
        mcfgs.add("Object__sizeof");
        icfgs.add("Object__sizeof");
        mcfgs.add("Object__table");
        icfgs.add("Object__table");
        vo.bind("FuncPtr", om.findStrict("ti.sysbios.interfaces.ITaskSupport.FuncPtr", "ti.sysbios.family.arm"));
        mcfgs.add("defaultStackSize");
        mcfgs.add("stackAlignment");
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.sysbios.interfaces");
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.sysbios.family.arm")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("start", om.findStrict("ti.sysbios.family.arm.TaskSupport.start", "ti.sysbios.family.arm"));
        vo.bind("swap", om.findStrict("ti.sysbios.family.arm.TaskSupport.swap", "ti.sysbios.family.arm"));
        vo.bind("checkStack", om.findStrict("ti.sysbios.family.arm.TaskSupport.checkStack", "ti.sysbios.family.arm"));
        vo.bind("stackUsed", om.findStrict("ti.sysbios.family.arm.TaskSupport.stackUsed", "ti.sysbios.family.arm"));
        vo.bind("getStackAlignment", om.findStrict("ti.sysbios.family.arm.TaskSupport.getStackAlignment", "ti.sysbios.family.arm"));
        vo.bind("getDefaultStackSize", om.findStrict("ti.sysbios.family.arm.TaskSupport.getDefaultStackSize", "ti.sysbios.family.arm"));
        vo.bind("$$fxntab", Global.newArray("ti_sysbios_family_arm_TaskSupport_Module__startupDone__E", "ti_sysbios_family_arm_TaskSupport_start__E", "ti_sysbios_family_arm_TaskSupport_swap__E", "ti_sysbios_family_arm_TaskSupport_checkStack__E", "ti_sysbios_family_arm_TaskSupport_stackUsed__E", "ti_sysbios_family_arm_TaskSupport_getStackAlignment__E", "ti_sysbios_family_arm_TaskSupport_getDefaultStackSize__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.seal("length");
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("TaskSupport", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TaskSupport");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.arm.HwiCommon", "ti.sysbios.family.arm"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.arm.IntrinsicsSupport", "ti.sysbios.family.arm"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.arm.Settings", "ti.sysbios.family.arm"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.sysbios.family.arm.TaskSupport", "ti.sysbios.family.arm"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.sysbios.family.arm.HwiCommon")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.arm.IntrinsicsSupport")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.arm.Settings")).bless();
        ((Value.Obj)om.getv("ti.sysbios.family.arm.TaskSupport")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.sysbios.family.arm")).add(pkgV);
    }

    public void exec( Scriptable xdcO, Session ses )
    {
        this.xdcO = xdcO;
        this.ses = ses;
        om = (Value.Obj)xdcO.get("om", null);

        Object o = om.geto("$name");
        String s = o instanceof String ? (String)o : null;
        isCFG = s != null && s.equals("cfg");
        isROV = s != null && s.equals("rov");

        $$IMPORTS();
        $$OBJECTS();
        HwiCommon$$OBJECTS();
        IntrinsicsSupport$$OBJECTS();
        Settings$$OBJECTS();
        TaskSupport$$OBJECTS();
        HwiCommon$$CONSTS();
        IntrinsicsSupport$$CONSTS();
        Settings$$CONSTS();
        TaskSupport$$CONSTS();
        HwiCommon$$CREATES();
        IntrinsicsSupport$$CREATES();
        Settings$$CREATES();
        TaskSupport$$CREATES();
        HwiCommon$$FUNCTIONS();
        IntrinsicsSupport$$FUNCTIONS();
        Settings$$FUNCTIONS();
        TaskSupport$$FUNCTIONS();
        HwiCommon$$SIZES();
        IntrinsicsSupport$$SIZES();
        Settings$$SIZES();
        TaskSupport$$SIZES();
        HwiCommon$$TYPES();
        IntrinsicsSupport$$TYPES();
        Settings$$TYPES();
        TaskSupport$$TYPES();
        if (isROV) {
            HwiCommon$$ROV();
            IntrinsicsSupport$$ROV();
            Settings$$ROV();
            TaskSupport$$ROV();
        }//isROV
        $$SINGLETONS();
        HwiCommon$$SINGLETONS();
        IntrinsicsSupport$$SINGLETONS();
        Settings$$SINGLETONS();
        TaskSupport$$SINGLETONS();
        $$INITIALIZATION();
    }
}
