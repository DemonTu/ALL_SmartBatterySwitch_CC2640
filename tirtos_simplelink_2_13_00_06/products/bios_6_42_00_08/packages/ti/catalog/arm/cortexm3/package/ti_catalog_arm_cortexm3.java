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

public class ti_catalog_arm_cortexm3
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
        Global.callFxn("loadPackage", xdcO, "xdc.platform");
        Global.callFxn("loadPackage", xdcO, "ti.catalog.peripherals.hdvicp2");
        Global.callFxn("loadPackage", xdcO, "xdc");
        Global.callFxn("loadPackage", xdcO, "xdc.corevers");
        Global.callFxn("loadPackage", xdcO, "ti.catalog");
        Global.callFxn("loadPackage", xdcO, "ti.catalog.arm.peripherals.timers");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.catalog.arm.cortexm3", new Value.Obj("ti.catalog.arm.cortexm3", pkgP));
    }

    void OMAP4430$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.OMAP4430.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.OMAP4430", new Value.Obj("ti.catalog.arm.cortexm3.OMAP4430", po));
        pkgV.bind("OMAP4430", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.OMAP4430.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.OMAP4430$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.OMAP4430.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.OMAP4430$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.OMAP4430.Params", new Proto.Str(po, true));
    }

    void ILM3Sx7xx$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ILM3Sx7xx.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.ILM3Sx7xx", new Value.Obj("ti.catalog.arm.cortexm3.ILM3Sx7xx", po));
        pkgV.bind("ILM3Sx7xx", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.ILM3Sx7xx.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ILM3Sx7xx$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.ILM3Sx7xx.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ILM3Sx7xx$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.ILM3Sx7xx.Params", new Proto.Str(po, true));
    }

    void LM3S9792$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9792.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9792", new Value.Obj("ti.catalog.arm.cortexm3.LM3S9792", po));
        pkgV.bind("LM3S9792", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.LM3S9792.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9792$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S9792.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9792$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S9792.Params", new Proto.Str(po, true));
    }

    void LM3S9790$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9790.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9790", new Value.Obj("ti.catalog.arm.cortexm3.LM3S9790", po));
        pkgV.bind("LM3S9790", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.LM3S9790.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9790$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S9790.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9790$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S9790.Params", new Proto.Str(po, true));
    }

    void LM3S5791$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S5791.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S5791", new Value.Obj("ti.catalog.arm.cortexm3.LM3S5791", po));
        pkgV.bind("LM3S5791", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.LM3S5791.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S5791$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S5791.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S5791$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S5791.Params", new Proto.Str(po, true));
    }

    void LM3S2793$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S2793.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S2793", new Value.Obj("ti.catalog.arm.cortexm3.LM3S2793", po));
        pkgV.bind("LM3S2793", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.LM3S2793.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S2793$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S2793.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S2793$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S2793.Params", new Proto.Str(po, true));
    }

    void ILM3SxBxx$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ILM3SxBxx.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.ILM3SxBxx", new Value.Obj("ti.catalog.arm.cortexm3.ILM3SxBxx", po));
        pkgV.bind("ILM3SxBxx", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.ILM3SxBxx.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ILM3SxBxx$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.ILM3SxBxx.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ILM3SxBxx$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.ILM3SxBxx.Params", new Proto.Str(po, true));
    }

    void LM3S9B96$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B96.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B96", new Value.Obj("ti.catalog.arm.cortexm3.LM3S9B96", po));
        pkgV.bind("LM3S9B96", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.LM3S9B96.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B96$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S9B96.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B96$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S9B96.Params", new Proto.Str(po, true));
    }

    void LM3S9B95$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B95.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B95", new Value.Obj("ti.catalog.arm.cortexm3.LM3S9B95", po));
        pkgV.bind("LM3S9B95", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.LM3S9B95.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B95$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S9B95.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B95$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S9B95.Params", new Proto.Str(po, true));
    }

    void LM3S9B92$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B92.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B92", new Value.Obj("ti.catalog.arm.cortexm3.LM3S9B92", po));
        pkgV.bind("LM3S9B92", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.LM3S9B92.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B92$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S9B92.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B92$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S9B92.Params", new Proto.Str(po, true));
    }

    void LM3S5B91$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S5B91.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S5B91", new Value.Obj("ti.catalog.arm.cortexm3.LM3S5B91", po));
        pkgV.bind("LM3S5B91", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.LM3S5B91.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S5B91$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S5B91.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S5B91$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S5B91.Params", new Proto.Str(po, true));
    }

    void LM3S9B90$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B90.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B90", new Value.Obj("ti.catalog.arm.cortexm3.LM3S9B90", po));
        pkgV.bind("LM3S9B90", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.LM3S9B90.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B90$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S9B90.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S9B90$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S9B90.Params", new Proto.Str(po, true));
    }

    void LM3S2B93$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S2B93.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S2B93", new Value.Obj("ti.catalog.arm.cortexm3.LM3S2B93", po));
        pkgV.bind("LM3S2B93", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.LM3S2B93.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S2B93$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S2B93.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S2B93$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S2B93.Params", new Proto.Str(po, true));
    }

    void ILM3Sx9xx$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ILM3Sx9xx.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.ILM3Sx9xx", new Value.Obj("ti.catalog.arm.cortexm3.ILM3Sx9xx", po));
        pkgV.bind("ILM3Sx9xx", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.ILM3Sx9xx.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ILM3Sx9xx$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.ILM3Sx9xx.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ILM3Sx9xx$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.ILM3Sx9xx.Params", new Proto.Str(po, true));
    }

    void LM3S8962$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S8962.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S8962", new Value.Obj("ti.catalog.arm.cortexm3.LM3S8962", po));
        pkgV.bind("LM3S8962", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.LM3S8962.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S8962$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S8962.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.LM3S8962$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.LM3S8962.Params", new Proto.Str(po, true));
    }

    void ITI8168$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ITI8168.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.ITI8168", new Value.Obj("ti.catalog.arm.cortexm3.ITI8168", po));
        pkgV.bind("ITI8168", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.ITI8168.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ITI8168$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.ITI8168.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ITI8168$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.ITI8168.Params", new Proto.Str(po, true));
    }

    void TMS320DM8168$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320DM8168.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320DM8168", new Value.Obj("ti.catalog.arm.cortexm3.TMS320DM8168", po));
        pkgV.bind("TMS320DM8168", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.TMS320DM8168.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320DM8168$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320DM8168.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320DM8168$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320DM8168.Params", new Proto.Str(po, true));
    }

    void TMS320TI816X$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI816X.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI816X", new Value.Obj("ti.catalog.arm.cortexm3.TMS320TI816X", po));
        pkgV.bind("TMS320TI816X", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.TMS320TI816X.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI816X$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320TI816X.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI816X$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320TI816X.Params", new Proto.Str(po, true));
    }

    void TMS320C6A8168$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320C6A8168.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320C6A8168", new Value.Obj("ti.catalog.arm.cortexm3.TMS320C6A8168", po));
        pkgV.bind("TMS320C6A8168", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.TMS320C6A8168.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320C6A8168$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320C6A8168.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320C6A8168$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320C6A8168.Params", new Proto.Str(po, true));
    }

    void ITI8148$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ITI8148.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.ITI8148", new Value.Obj("ti.catalog.arm.cortexm3.ITI8148", po));
        pkgV.bind("ITI8148", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.ITI8148.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ITI8148$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.ITI8148.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ITI8148$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.ITI8148.Params", new Proto.Str(po, true));
    }

    void TMS320DM8148$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320DM8148.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320DM8148", new Value.Obj("ti.catalog.arm.cortexm3.TMS320DM8148", po));
        pkgV.bind("TMS320DM8148", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.TMS320DM8148.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320DM8148$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320DM8148.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320DM8148$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320DM8148.Params", new Proto.Str(po, true));
    }

    void TMS320TI814X$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI814X.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI814X", new Value.Obj("ti.catalog.arm.cortexm3.TMS320TI814X", po));
        pkgV.bind("TMS320TI814X", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.TMS320TI814X.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI814X$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320TI814X.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI814X$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320TI814X.Params", new Proto.Str(po, true));
    }

    void CC13xx$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.CC13xx.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.CC13xx", new Value.Obj("ti.catalog.arm.cortexm3.CC13xx", po));
        pkgV.bind("CC13xx", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.CC13xx.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.CC13xx$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.CC13xx.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.CC13xx$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.CC13xx.Params", new Proto.Str(po, true));
    }

    void CC26xx$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.CC26xx.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.CC26xx", new Value.Obj("ti.catalog.arm.cortexm3.CC26xx", po));
        pkgV.bind("CC26xx", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.CC26xx.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.CC26xx$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.CC26xx.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.CC26xx$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.CC26xx.Params", new Proto.Str(po, true));
    }

    void F28M35x$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.F28M35x.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.F28M35x", new Value.Obj("ti.catalog.arm.cortexm3.F28M35x", po));
        pkgV.bind("F28M35x", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.F28M35x.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.F28M35x$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.F28M35x.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.F28M35x$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.F28M35x.Params", new Proto.Str(po, true));
    }

    void Stellaris$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.Stellaris.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.Stellaris", new Value.Obj("ti.catalog.arm.cortexm3.Stellaris", po));
        pkgV.bind("Stellaris", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.Stellaris.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.Stellaris$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.Stellaris.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.Stellaris$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.Stellaris.Params", new Proto.Str(po, true));
    }

    void Tiva$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.Tiva.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.Tiva", new Value.Obj("ti.catalog.arm.cortexm3.Tiva", po));
        pkgV.bind("Tiva", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.Tiva.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.Tiva$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.Tiva.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.Tiva$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.Tiva.Params", new Proto.Str(po, true));
    }

    void CortexM3$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.CortexM3.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.CortexM3", new Value.Obj("ti.catalog.arm.cortexm3.CortexM3", po));
        pkgV.bind("CortexM3", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.CortexM3.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.CortexM3$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.CortexM3.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.CortexM3$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.CortexM3.Params", new Proto.Str(po, true));
    }

    void ITI813X$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ITI813X.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.ITI813X", new Value.Obj("ti.catalog.arm.cortexm3.ITI813X", po));
        pkgV.bind("ITI813X", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.ITI813X.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ITI813X$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.ITI813X.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ITI813X$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.ITI813X.Params", new Proto.Str(po, true));
    }

    void TMS320TI813X$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI813X.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI813X", new Value.Obj("ti.catalog.arm.cortexm3.TMS320TI813X", po));
        pkgV.bind("TMS320TI813X", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.TMS320TI813X.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI813X$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320TI813X.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI813X$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320TI813X.Params", new Proto.Str(po, true));
    }

    void ITI811X$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ITI811X.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.ITI811X", new Value.Obj("ti.catalog.arm.cortexm3.ITI811X", po));
        pkgV.bind("ITI811X", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.ITI811X.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ITI811X$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.ITI811X.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.ITI811X$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.ITI811X.Params", new Proto.Str(po, true));
    }

    void TMS320TI811X$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI811X.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI811X", new Value.Obj("ti.catalog.arm.cortexm3.TMS320TI811X", po));
        pkgV.bind("TMS320TI811X", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.TMS320TI811X.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI811X$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320TI811X.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320TI811X$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320TI811X.Params", new Proto.Str(po, true));
    }

    void TMS320C6A8149$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320C6A8149.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320C6A8149", new Value.Obj("ti.catalog.arm.cortexm3.TMS320C6A8149", po));
        pkgV.bind("TMS320C6A8149", vo);
        // decls 
        // insts 
        Object insP = om.bind("ti.catalog.arm.cortexm3.TMS320C6A8149.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320C6A8149$$Object", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320C6A8149.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("ti.catalog.arm.cortexm3.TMS320C6A8149$$Params", new Proto.Obj());
        om.bind("ti.catalog.arm.cortexm3.TMS320C6A8149.Params", new Proto.Str(po, true));
    }

    void OMAP4430$$CONSTS()
    {
        // module OMAP4430
    }

    void ILM3Sx7xx$$CONSTS()
    {
        // interface ILM3Sx7xx
    }

    void LM3S9792$$CONSTS()
    {
        // module LM3S9792
    }

    void LM3S9790$$CONSTS()
    {
        // module LM3S9790
    }

    void LM3S5791$$CONSTS()
    {
        // module LM3S5791
    }

    void LM3S2793$$CONSTS()
    {
        // module LM3S2793
    }

    void ILM3SxBxx$$CONSTS()
    {
        // interface ILM3SxBxx
    }

    void LM3S9B96$$CONSTS()
    {
        // module LM3S9B96
    }

    void LM3S9B95$$CONSTS()
    {
        // module LM3S9B95
    }

    void LM3S9B92$$CONSTS()
    {
        // module LM3S9B92
    }

    void LM3S5B91$$CONSTS()
    {
        // module LM3S5B91
    }

    void LM3S9B90$$CONSTS()
    {
        // module LM3S9B90
    }

    void LM3S2B93$$CONSTS()
    {
        // module LM3S2B93
    }

    void ILM3Sx9xx$$CONSTS()
    {
        // interface ILM3Sx9xx
    }

    void LM3S8962$$CONSTS()
    {
        // module LM3S8962
    }

    void ITI8168$$CONSTS()
    {
        // interface ITI8168
    }

    void TMS320DM8168$$CONSTS()
    {
        // module TMS320DM8168
    }

    void TMS320TI816X$$CONSTS()
    {
        // module TMS320TI816X
    }

    void TMS320C6A8168$$CONSTS()
    {
        // module TMS320C6A8168
    }

    void ITI8148$$CONSTS()
    {
        // interface ITI8148
    }

    void TMS320DM8148$$CONSTS()
    {
        // module TMS320DM8148
    }

    void TMS320TI814X$$CONSTS()
    {
        // module TMS320TI814X
    }

    void CC13xx$$CONSTS()
    {
        // module CC13xx
    }

    void CC26xx$$CONSTS()
    {
        // module CC26xx
    }

    void F28M35x$$CONSTS()
    {
        // module F28M35x
    }

    void Stellaris$$CONSTS()
    {
        // module Stellaris
    }

    void Tiva$$CONSTS()
    {
        // module Tiva
    }

    void CortexM3$$CONSTS()
    {
        // module CortexM3
    }

    void ITI813X$$CONSTS()
    {
        // interface ITI813X
    }

    void TMS320TI813X$$CONSTS()
    {
        // module TMS320TI813X
    }

    void ITI811X$$CONSTS()
    {
        // interface ITI811X
    }

    void TMS320TI811X$$CONSTS()
    {
        // module TMS320TI811X
    }

    void TMS320C6A8149$$CONSTS()
    {
        // module TMS320C6A8149
    }

    void OMAP4430$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.OMAP4430$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.OMAP4430.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.OMAP4430.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.OMAP4430.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$OMAP4430$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.OMAP4430'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.OMAP4430.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.OMAP4430$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.OMAP4430.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.OMAP4430$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.OMAP4430.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$OMAP4430$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.OMAP4430'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void ILM3Sx7xx$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void LM3S9792$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S9792$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S9792.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.LM3S9792.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9792.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S9792$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S9792'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.LM3S9792.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S9792$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S9792.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9792$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9792.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S9792$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S9792'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void LM3S9790$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S9790$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S9790.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.LM3S9790.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9790.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S9790$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S9790'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.LM3S9790.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S9790$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S9790.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9790$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9790.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S9790$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S9790'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void LM3S5791$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S5791$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S5791.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.LM3S5791.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S5791.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S5791$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S5791'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.LM3S5791.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S5791$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S5791.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S5791$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S5791.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S5791$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S5791'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void LM3S2793$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S2793$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S2793.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.LM3S2793.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S2793.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S2793$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S2793'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.LM3S2793.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S2793$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S2793.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S2793$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S2793.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S2793$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S2793'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void ILM3SxBxx$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void LM3S9B96$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S9B96$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S9B96$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S9B96'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.LM3S9B96.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S9B96$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S9B96$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S9B96'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void LM3S9B95$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S9B95$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S9B95$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S9B95'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.LM3S9B95.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S9B95$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S9B95$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S9B95'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void LM3S9B92$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S9B92$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S9B92$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S9B92'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.LM3S9B92.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S9B92$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S9B92$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S9B92'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void LM3S5B91$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S5B91$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S5B91$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S5B91'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.LM3S5B91.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S5B91$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S5B91$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S5B91'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void LM3S9B90$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S9B90$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S9B90$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S9B90'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.LM3S9B90.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S9B90$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S9B90$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S9B90'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void LM3S2B93$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S2B93$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S2B93$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S2B93'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.LM3S2B93.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S2B93$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S2B93$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S2B93'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void ILM3Sx9xx$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void LM3S8962$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S8962$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S8962.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.LM3S8962.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S8962.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S8962$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S8962'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.LM3S8962.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.LM3S8962$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.LM3S8962.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S8962$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.LM3S8962.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$LM3S8962$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.LM3S8962'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void ITI8168$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void TMS320DM8168$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320DM8168$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320DM8168$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320DM8168'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.TMS320DM8168.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("__inst.hdvicp1 = __mod.PARAMS.hdvicp1;\n");
            sb.append("__inst.hdvicp2 = __mod.PARAMS.hdvicp2;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320DM8168$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320DM8168$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320DM8168'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("__inst.hdvicp1 = __mod.PARAMS.hdvicp1;\n");
            sb.append("__inst.hdvicp2 = __mod.PARAMS.hdvicp2;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void TMS320TI816X$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320TI816X$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320TI816X$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320TI816X'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.TMS320TI816X.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("__inst.hdvicp1 = __mod.PARAMS.hdvicp1;\n");
            sb.append("__inst.hdvicp2 = __mod.PARAMS.hdvicp2;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320TI816X$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320TI816X$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320TI816X'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("__inst.hdvicp1 = __mod.PARAMS.hdvicp1;\n");
            sb.append("__inst.hdvicp2 = __mod.PARAMS.hdvicp2;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void TMS320C6A8168$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320C6A8168$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320C6A8168$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320C6A8168'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.TMS320C6A8168.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("__inst.hdvicp1 = __mod.PARAMS.hdvicp1;\n");
            sb.append("__inst.hdvicp2 = __mod.PARAMS.hdvicp2;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320C6A8168$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320C6A8168$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320C6A8168'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("__inst.hdvicp1 = __mod.PARAMS.hdvicp1;\n");
            sb.append("__inst.hdvicp2 = __mod.PARAMS.hdvicp2;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void ITI8148$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void TMS320DM8148$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320DM8148$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320DM8148$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320DM8148'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.TMS320DM8148.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320DM8148$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320DM8148$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320DM8148'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void TMS320TI814X$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320TI814X$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320TI814X$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320TI814X'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.TMS320TI814X.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320TI814X$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320TI814X$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320TI814X'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void CC13xx$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.CC13xx$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.CC13xx.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.CC13xx.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.CC13xx.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$CC13xx$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.CC13xx'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.CC13xx.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.CC13xx$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.CC13xx.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.CC13xx$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.CC13xx.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$CC13xx$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.CC13xx'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void CC26xx$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.CC26xx$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.CC26xx.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.CC26xx.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.CC26xx.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$CC26xx$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.CC26xx'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.CC26xx.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.CC26xx$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.CC26xx.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.CC26xx$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.CC26xx.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$CC26xx$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.CC26xx'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void F28M35x$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.F28M35x$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.F28M35x.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.F28M35x.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.F28M35x.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$F28M35x$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.F28M35x'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.F28M35x.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.F28M35x$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.F28M35x.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.F28M35x$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.F28M35x.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$F28M35x$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.F28M35x'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.timer0 = __mod.PARAMS.timer0;\n");
            sb.append("__inst.timer1 = __mod.PARAMS.timer1;\n");
            sb.append("__inst.timer2 = __mod.PARAMS.timer2;\n");
            sb.append("__inst.timer3 = __mod.PARAMS.timer3;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void Stellaris$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.Stellaris$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.Stellaris.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.Stellaris.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.Stellaris.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$Stellaris$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.Stellaris'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.Stellaris.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.Stellaris$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.Stellaris.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.Stellaris$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.Stellaris.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$Stellaris$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.Stellaris'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void Tiva$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.Tiva$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.Tiva.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.Tiva.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.Tiva.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$Tiva$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.Tiva'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.Tiva.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.Tiva$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.Tiva.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.Tiva$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.Tiva.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$Tiva$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.Tiva'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void CortexM3$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.CortexM3$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.CortexM3.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.CortexM3.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.CortexM3.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$CortexM3$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.CortexM3'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.CortexM3.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.CortexM3$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.CortexM3.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.CortexM3$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.CortexM3.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$CortexM3$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.CortexM3'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void ITI813X$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void TMS320TI813X$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320TI813X$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320TI813X$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320TI813X'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.TMS320TI813X.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320TI813X$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320TI813X$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320TI813X'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void ITI811X$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void TMS320TI811X$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320TI811X$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320TI811X$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320TI811X'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.TMS320TI811X.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320TI811X$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320TI811X$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320TI811X'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void TMS320C6A8149$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320C6A8149$$create", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149.Module", "ti.catalog.arm.cortexm3"), om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149.Instance", "ti.catalog.arm.cortexm3"), 2, 1, false));
                fxn.addArg(0, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(1, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320C6A8149$$create = function( revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320C6A8149'];\n");
            sb.append("var __inst = xdc.om['ti.catalog.arm.cortexm3.TMS320C6A8149.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['ti.catalog.arm.cortexm3']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, [revision]);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("ti.catalog.arm.cortexm3.TMS320C6A8149$$construct", new Proto.Fxn(om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149.Module", "ti.catalog.arm.cortexm3"), null, 3, 1, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149$$Object", "ti.catalog.arm.cortexm3"), null);
                fxn.addArg(1, "revision", $$T_Str, $$UNDEF);
                fxn.addArg(2, "__params", (Proto)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149.Params", "ti.catalog.arm.cortexm3"), Global.newObject());
        sb = new StringBuilder();
        sb.append("ti$catalog$arm$cortexm3$TMS320C6A8149$$construct = function( __obj, revision, __params ) {\n");
            sb.append("var __mod = xdc.om['ti.catalog.arm.cortexm3.TMS320C6A8149'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {revision:revision});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.cpuCore = __mod.PARAMS.cpuCore;\n");
            sb.append("__inst.cpuCoreRevision = __mod.PARAMS.cpuCoreRevision;\n");
            sb.append("__inst.minProgUnitSize = __mod.PARAMS.minProgUnitSize;\n");
            sb.append("__inst.minDataUnitSize = __mod.PARAMS.minDataUnitSize;\n");
            sb.append("__inst.dataWordSize = __mod.PARAMS.dataWordSize;\n");
            sb.append("__inst.peripherals = __mod.PARAMS.peripherals;\n");
            sb.append("__inst.deviceHeader = __mod.PARAMS.deviceHeader;\n");
            sb.append("__inst.isa = __mod.PARAMS.isa;\n");
            sb.append("__inst.hdvicp0 = __mod.PARAMS.hdvicp0;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void OMAP4430$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void ILM3Sx7xx$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LM3S9792$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LM3S9790$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LM3S5791$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LM3S2793$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void ILM3SxBxx$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LM3S9B96$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LM3S9B95$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LM3S9B92$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LM3S5B91$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LM3S9B90$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LM3S2B93$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void ILM3Sx9xx$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void LM3S8962$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void ITI8168$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TMS320DM8168$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TMS320TI816X$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TMS320C6A8168$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void ITI8148$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TMS320DM8148$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TMS320TI814X$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void CC13xx$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void CC26xx$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void F28M35x$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Stellaris$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Tiva$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void CortexM3$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void ITI813X$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TMS320TI813X$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void ITI811X$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TMS320TI811X$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TMS320C6A8149$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void OMAP4430$$SIZES()
    {
    }

    void ILM3Sx7xx$$SIZES()
    {
    }

    void LM3S9792$$SIZES()
    {
    }

    void LM3S9790$$SIZES()
    {
    }

    void LM3S5791$$SIZES()
    {
    }

    void LM3S2793$$SIZES()
    {
    }

    void ILM3SxBxx$$SIZES()
    {
    }

    void LM3S9B96$$SIZES()
    {
    }

    void LM3S9B95$$SIZES()
    {
    }

    void LM3S9B92$$SIZES()
    {
    }

    void LM3S5B91$$SIZES()
    {
    }

    void LM3S9B90$$SIZES()
    {
    }

    void LM3S2B93$$SIZES()
    {
    }

    void ILM3Sx9xx$$SIZES()
    {
    }

    void LM3S8962$$SIZES()
    {
    }

    void ITI8168$$SIZES()
    {
    }

    void TMS320DM8168$$SIZES()
    {
    }

    void TMS320TI816X$$SIZES()
    {
    }

    void TMS320C6A8168$$SIZES()
    {
    }

    void ITI8148$$SIZES()
    {
    }

    void TMS320DM8148$$SIZES()
    {
    }

    void TMS320TI814X$$SIZES()
    {
    }

    void CC13xx$$SIZES()
    {
    }

    void CC26xx$$SIZES()
    {
    }

    void F28M35x$$SIZES()
    {
    }

    void Stellaris$$SIZES()
    {
    }

    void Tiva$$SIZES()
    {
    }

    void CortexM3$$SIZES()
    {
    }

    void ITI813X$$SIZES()
    {
    }

    void TMS320TI813X$$SIZES()
    {
    }

    void ITI811X$$SIZES()
    {
    }

    void TMS320TI811X$$SIZES()
    {
    }

    void TMS320C6A8149$$SIZES()
    {
    }

    void OMAP4430$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/arm/cortexm3/OMAP4430.xs");
        om.bind("ti.catalog.arm.cortexm3.OMAP4430$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.OMAP4430.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.OMAP4430.Module", om.findStrict("ti.catalog.ICpuDataSheet.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.OMAP4430$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$OMAP4430$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.OMAP4430$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$OMAP4430$$construct"));
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.OMAP4430$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.OMAP4430$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.OMAP4430$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.OMAP4430$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.OMAP4430.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.OMAP4430.Instance", om.findStrict("ti.catalog.ICpuDataSheet.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("hdvicp0", (Proto)om.findStrict("ti.catalog.peripherals.hdvicp2.HDVICP2.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"L2_ROM", Global.newObject("name", "L2_ROM", "base", 0x00000000L, "len", 0x00004000L)}), Global.newArray(new Object[]{"L2_RAM", Global.newObject("name", "L2_RAM", "base", 0x20000000L, "len", 0x00010000L)})}), "wh");
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.OMAP4430$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.OMAP4430.Params", om.findStrict("ti.catalog.ICpuDataSheet$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("hdvicp0", (Proto)om.findStrict("ti.catalog.peripherals.hdvicp2.HDVICP2.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"L2_ROM", Global.newObject("name", "L2_ROM", "base", 0x00000000L, "len", 0x00004000L)}), Global.newArray(new Object[]{"L2_RAM", Global.newObject("name", "L2_RAM", "base", 0x20000000L, "len", 0x00010000L)})}), "wh");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.OMAP4430$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.OMAP4430.Object", om.findStrict("ti.catalog.arm.cortexm3.OMAP4430.Instance", "ti.catalog.arm.cortexm3"));
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
    }

    void ILM3Sx7xx$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/arm/cortexm3/ILM3Sx7xx.xs");
        om.bind("ti.catalog.arm.cortexm3.ILM3Sx7xx$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ILM3Sx7xx.Module", om.findStrict("ti.catalog.ICpuDataSheet.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ILM3Sx7xx$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ILM3Sx7xx$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ILM3Sx7xx$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ILM3Sx7xx$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ILM3Sx7xx.Instance", om.findStrict("ti.catalog.ICpuDataSheet.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("timer0", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer1", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer2", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer3", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"FRAM", Global.newObject("comment", "128KB FRAM", "name", "FRAM", "base", 0x00000000L, "len", 0x00020000L)}), Global.newArray(new Object[]{"IRAM", Global.newObject("comment", "64KB IRAM", "name", "IRAM", "base", 0x20000000L, "len", 0x00010000L)})}), "wh");
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ILM3Sx7xx.Params", om.findStrict("ti.catalog.ICpuDataSheet$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("timer0", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer1", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer2", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer3", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"FRAM", Global.newObject("comment", "128KB FRAM", "name", "FRAM", "base", 0x00000000L, "len", 0x00020000L)}), Global.newArray(new Object[]{"IRAM", Global.newObject("comment", "64KB IRAM", "name", "IRAM", "base", 0x20000000L, "len", 0x00010000L)})}), "wh");
    }

    void LM3S9792$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9792.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9792.Module", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S9792$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S9792$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S9792$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S9792$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9792.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9792.Instance", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9792$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9792.Params", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9792$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9792.Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S9792.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void LM3S9790$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9790.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9790.Module", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S9790$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S9790$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S9790$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S9790$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9790.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9790.Instance", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9790$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9790.Params", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9790$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9790.Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S9790.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void LM3S5791$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S5791.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S5791.Module", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S5791$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S5791$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S5791$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S5791$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S5791.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S5791.Instance", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S5791$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S5791.Params", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S5791$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S5791.Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S5791.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void LM3S2793$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S2793.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S2793.Module", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S2793$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S2793$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S2793$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S2793$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S2793.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S2793.Instance", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S2793$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S2793.Params", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S2793$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S2793.Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S2793.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void ILM3SxBxx$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/arm/cortexm3/ILM3SxBxx.xs");
        om.bind("ti.catalog.arm.cortexm3.ILM3SxBxx$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ILM3SxBxx.Module", om.findStrict("ti.catalog.ICpuDataSheet.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ILM3SxBxx$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ILM3SxBxx$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ILM3SxBxx$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ILM3SxBxx$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ILM3SxBxx.Instance", om.findStrict("ti.catalog.ICpuDataSheet.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("timer0", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer1", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer2", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer3", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"FRAM", Global.newObject("comment", "256KB FRAM", "name", "FRAM", "base", 0x00000000L, "len", 0x00040000L)}), Global.newArray(new Object[]{"IRAM", Global.newObject("comment", "96KB IRAM", "name", "IRAM", "base", 0x20000000L, "len", 0x00018000L)})}), "wh");
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ILM3SxBxx.Params", om.findStrict("ti.catalog.ICpuDataSheet$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("timer0", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer1", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer2", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer3", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"FRAM", Global.newObject("comment", "256KB FRAM", "name", "FRAM", "base", 0x00000000L, "len", 0x00040000L)}), Global.newArray(new Object[]{"IRAM", Global.newObject("comment", "96KB IRAM", "name", "IRAM", "base", 0x20000000L, "len", 0x00018000L)})}), "wh");
    }

    void LM3S9B96$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B96.Module", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S9B96$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S9B96$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B96.Instance", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B96.Params", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B96.Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void LM3S9B95$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B95.Module", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S9B95$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S9B95$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B95.Instance", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B95.Params", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B95.Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void LM3S9B92$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B92.Module", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S9B92$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S9B92$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B92.Instance", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B92.Params", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B92.Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void LM3S5B91$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S5B91.Module", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S5B91$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S5B91$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S5B91.Instance", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S5B91.Params", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S5B91.Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void LM3S9B90$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B90.Module", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S9B90$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S9B90$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B90.Instance", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B90.Params", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S9B90.Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void LM3S2B93$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S2B93.Module", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S2B93$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S2B93$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S2B93.Instance", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S2B93.Params", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S2B93.Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void ILM3Sx9xx$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/arm/cortexm3/ILM3Sx9xx.xs");
        om.bind("ti.catalog.arm.cortexm3.ILM3Sx9xx$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx9xx.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ILM3Sx9xx.Module", om.findStrict("ti.catalog.ICpuDataSheet.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ILM3Sx9xx$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ILM3Sx9xx$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ILM3Sx9xx$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ILM3Sx9xx$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx9xx.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ILM3Sx9xx.Instance", om.findStrict("ti.catalog.ICpuDataSheet.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("timer0", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer1", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer2", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer3", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"FRAM", Global.newObject("comment", "256KB FRAM", "name", "FRAM", "base", 0x00000000L, "len", 0x00040000L)}), Global.newArray(new Object[]{"IRAM", Global.newObject("comment", "64KB IRAM", "name", "IRAM", "base", 0x20000000L, "len", 0x00010000L)})}), "wh");
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx9xx$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ILM3Sx9xx.Params", om.findStrict("ti.catalog.ICpuDataSheet$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("timer0", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer1", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer2", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer3", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"FRAM", Global.newObject("comment", "256KB FRAM", "name", "FRAM", "base", 0x00000000L, "len", 0x00040000L)}), Global.newArray(new Object[]{"IRAM", Global.newObject("comment", "64KB IRAM", "name", "IRAM", "base", 0x20000000L, "len", 0x00010000L)})}), "wh");
    }

    void LM3S8962$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S8962.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S8962.Module", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx9xx.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S8962$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S8962$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.LM3S8962$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$LM3S8962$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S8962.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S8962.Instance", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx9xx.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S8962$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S8962.Params", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx9xx$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S8962$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.LM3S8962.Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S8962.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void ITI8168$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/arm/cortexm3/ITI8168.xs");
        om.bind("ti.catalog.arm.cortexm3.ITI8168$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI8168.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ITI8168.Module", om.findStrict("ti.catalog.ICpuDataSheet.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI8168$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI8168$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI8168$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI8168$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI8168.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ITI8168.Instance", om.findStrict("ti.catalog.ICpuDataSheet.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("hdvicp0", (Proto)om.findStrict("ti.catalog.peripherals.hdvicp2.HDVICP2.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("hdvicp1", (Proto)om.findStrict("ti.catalog.peripherals.hdvicp2.HDVICP2.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("hdvicp2", (Proto)om.findStrict("ti.catalog.peripherals.hdvicp2.HDVICP2.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"L2_BOOT", Global.newObject("name", "L2_BOOT", "base", 0x00000000L, "len", 0x4000L)}), Global.newArray(new Object[]{"L2_RAM", Global.newObject("name", "L2_RAM", "base", 0x20004000L, "len", 0x3C000L)}), Global.newArray(new Object[]{"OCMC_0", Global.newObject("name", "OCMC_0", "base", 0x00300000L, "len", 0x40000L)}), Global.newArray(new Object[]{"OCMC_1", Global.newObject("name", "OCMC_1", "base", 0x00400000L, "len", 0x40000L)})}), "wh");
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI8168$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ITI8168.Params", om.findStrict("ti.catalog.ICpuDataSheet$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("hdvicp0", (Proto)om.findStrict("ti.catalog.peripherals.hdvicp2.HDVICP2.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("hdvicp1", (Proto)om.findStrict("ti.catalog.peripherals.hdvicp2.HDVICP2.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("hdvicp2", (Proto)om.findStrict("ti.catalog.peripherals.hdvicp2.HDVICP2.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"L2_BOOT", Global.newObject("name", "L2_BOOT", "base", 0x00000000L, "len", 0x4000L)}), Global.newArray(new Object[]{"L2_RAM", Global.newObject("name", "L2_RAM", "base", 0x20004000L, "len", 0x3C000L)}), Global.newArray(new Object[]{"OCMC_0", Global.newObject("name", "OCMC_0", "base", 0x00300000L, "len", 0x40000L)}), Global.newArray(new Object[]{"OCMC_1", Global.newObject("name", "OCMC_1", "base", 0x00400000L, "len", 0x40000L)})}), "wh");
    }

    void TMS320DM8168$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320DM8168.Module", om.findStrict("ti.catalog.arm.cortexm3.ITI8168.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320DM8168$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320DM8168$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320DM8168.Instance", om.findStrict("ti.catalog.arm.cortexm3.ITI8168.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320DM8168.Params", om.findStrict("ti.catalog.arm.cortexm3.ITI8168$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320DM8168.Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void TMS320TI816X$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI816X.Module", om.findStrict("ti.catalog.arm.cortexm3.ITI8168.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320TI816X$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320TI816X$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI816X.Instance", om.findStrict("ti.catalog.arm.cortexm3.ITI8168.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI816X.Params", om.findStrict("ti.catalog.arm.cortexm3.ITI8168$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI816X.Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void TMS320C6A8168$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320C6A8168.Module", om.findStrict("ti.catalog.arm.cortexm3.ITI8168.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320C6A8168$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320C6A8168$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320C6A8168.Instance", om.findStrict("ti.catalog.arm.cortexm3.ITI8168.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320C6A8168.Params", om.findStrict("ti.catalog.arm.cortexm3.ITI8168$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320C6A8168.Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void ITI8148$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/arm/cortexm3/ITI8148.xs");
        om.bind("ti.catalog.arm.cortexm3.ITI8148$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI8148.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ITI8148.Module", om.findStrict("ti.catalog.ICpuDataSheet.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI8148$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI8148$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI8148$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI8148$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI8148.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ITI8148.Instance", om.findStrict("ti.catalog.ICpuDataSheet.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("hdvicp0", (Proto)om.findStrict("ti.catalog.peripherals.hdvicp2.HDVICP2.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"L2_BOOT", Global.newObject("name", "L2_BOOT", "base", 0x00000000L, "len", 0x4000L)}), Global.newArray(new Object[]{"L2_RAM", Global.newObject("name", "L2_RAM", "base", 0x20004000L, "len", 0x3C000L)}), Global.newArray(new Object[]{"OCMC", Global.newObject("name", "OCMC", "base", 0x00300000L, "len", 0x20000L)})}), "wh");
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI8148$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ITI8148.Params", om.findStrict("ti.catalog.ICpuDataSheet$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("hdvicp0", (Proto)om.findStrict("ti.catalog.peripherals.hdvicp2.HDVICP2.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"L2_BOOT", Global.newObject("name", "L2_BOOT", "base", 0x00000000L, "len", 0x4000L)}), Global.newArray(new Object[]{"L2_RAM", Global.newObject("name", "L2_RAM", "base", 0x20004000L, "len", 0x3C000L)}), Global.newArray(new Object[]{"OCMC", Global.newObject("name", "OCMC", "base", 0x00300000L, "len", 0x20000L)})}), "wh");
    }

    void TMS320DM8148$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320DM8148.Module", om.findStrict("ti.catalog.arm.cortexm3.ITI8148.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320DM8148$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320DM8148$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320DM8148.Instance", om.findStrict("ti.catalog.arm.cortexm3.ITI8148.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320DM8148.Params", om.findStrict("ti.catalog.arm.cortexm3.ITI8148$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320DM8148.Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void TMS320TI814X$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI814X.Module", om.findStrict("ti.catalog.arm.cortexm3.ITI8148.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320TI814X$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320TI814X$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI814X.Instance", om.findStrict("ti.catalog.arm.cortexm3.ITI8148.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI814X.Params", om.findStrict("ti.catalog.arm.cortexm3.ITI8148$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI814X.Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void CC13xx$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/arm/cortexm3/CC13xx.xs");
        om.bind("ti.catalog.arm.cortexm3.CC13xx$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.CC13xx.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.CC13xx.Module", om.findStrict("ti.catalog.ICpuDataSheet.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.CC13xx$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$CC13xx$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.CC13xx$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$CC13xx$$construct"));
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.CC13xx$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.CC13xx$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.CC13xx$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.CC13xx$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.CC13xx.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.CC13xx.Instance", om.findStrict("ti.catalog.ICpuDataSheet.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.CC13xx$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.CC13xx.Params", om.findStrict("ti.catalog.ICpuDataSheet$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.CC13xx$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.CC13xx.Object", om.findStrict("ti.catalog.arm.cortexm3.CC13xx.Instance", "ti.catalog.arm.cortexm3"));
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
    }

    void CC26xx$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/arm/cortexm3/CC26xx.xs");
        om.bind("ti.catalog.arm.cortexm3.CC26xx$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.CC26xx.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.CC26xx.Module", om.findStrict("ti.catalog.ICpuDataSheet.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.CC26xx$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$CC26xx$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.CC26xx$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$CC26xx$$construct"));
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.CC26xx$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.CC26xx$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.CC26xx$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.CC26xx$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.CC26xx.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.CC26xx.Instance", om.findStrict("ti.catalog.ICpuDataSheet.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.CC26xx$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.CC26xx.Params", om.findStrict("ti.catalog.ICpuDataSheet$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.CC26xx$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.CC26xx.Object", om.findStrict("ti.catalog.arm.cortexm3.CC26xx.Instance", "ti.catalog.arm.cortexm3"));
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
    }

    void F28M35x$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/arm/cortexm3/F28M35x.xs");
        om.bind("ti.catalog.arm.cortexm3.F28M35x$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.F28M35x.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.F28M35x.Module", om.findStrict("ti.catalog.ICpuDataSheet.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.F28M35x$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$F28M35x$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.F28M35x$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$F28M35x$$construct"));
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.F28M35x$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.F28M35x$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.F28M35x$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.F28M35x$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.F28M35x.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.F28M35x.Instance", om.findStrict("ti.catalog.ICpuDataSheet.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("timer0", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer1", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer2", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer3", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"BOOTROM", Global.newObject("comment", "On-Chip Boot ROM", "name", "BOOTROM", "base", 0x0L, "len", 0x10000L, "space", "code", "access", "RX")}), Global.newArray(new Object[]{"FLASH_BOOT", Global.newObject("comment", "Branch for boot from Flash", "name", "FLASH_BOOT", "base", 0x200030L, "len", 0x4L, "space", "code/data", "access", "RWX")}), Global.newArray(new Object[]{"FLASH", Global.newObject("comment", "512KB Flash memory", "name", "FLASH", "base", 0x200034L, "len", 0x7FFC8L, "space", "code/data", "access", "RWX")}), Global.newArray(new Object[]{"OTP", Global.newObject("comment", "4KB OTP memory", "name", "OTP", "base", 0x680000L, "len", 0x1000L, "space", "code", "access", "RWX")}), Global.newArray(new Object[]{"C03SRAM", Global.newObject("comment", "32KB On-Chip RAM Memory", "name", "C03SRAM", "base", 0x20000000L, "len", 0x8000L, "space", "code/data", "access", "RWX")}), Global.newArray(new Object[]{"S07SHRAM", Global.newObject("comment", "64KB On-Chip Shared RAM Memory", "name", "S07SHRAM", "base", 0x20008000L, "len", 0x10000L, "space", "code/data", "access", "RWX")}), Global.newArray(new Object[]{"CTOMMSGRAM", Global.newObject("comment", "C28 to M3 MSG RAM Memory", "name", "CTOMMSGRAM", "base", 0x2007F000L, "len", 0x800L, "space", "data", "access", "R")}), Global.newArray(new Object[]{"MTOCMSGRAM", Global.newObject("comment", "M3 to C28 MSG RAM Memory", "name", "MTOCMSGRAM", "base", 0x2007F800L, "len", 0x800L, "space", "data", "access", "RW")})}), "wh");
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.F28M35x$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.F28M35x.Params", om.findStrict("ti.catalog.ICpuDataSheet$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("timer0", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer1", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer2", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("timer3", (Proto)om.findStrict("ti.catalog.arm.peripherals.timers.Timer.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"BOOTROM", Global.newObject("comment", "On-Chip Boot ROM", "name", "BOOTROM", "base", 0x0L, "len", 0x10000L, "space", "code", "access", "RX")}), Global.newArray(new Object[]{"FLASH_BOOT", Global.newObject("comment", "Branch for boot from Flash", "name", "FLASH_BOOT", "base", 0x200030L, "len", 0x4L, "space", "code/data", "access", "RWX")}), Global.newArray(new Object[]{"FLASH", Global.newObject("comment", "512KB Flash memory", "name", "FLASH", "base", 0x200034L, "len", 0x7FFC8L, "space", "code/data", "access", "RWX")}), Global.newArray(new Object[]{"OTP", Global.newObject("comment", "4KB OTP memory", "name", "OTP", "base", 0x680000L, "len", 0x1000L, "space", "code", "access", "RWX")}), Global.newArray(new Object[]{"C03SRAM", Global.newObject("comment", "32KB On-Chip RAM Memory", "name", "C03SRAM", "base", 0x20000000L, "len", 0x8000L, "space", "code/data", "access", "RWX")}), Global.newArray(new Object[]{"S07SHRAM", Global.newObject("comment", "64KB On-Chip Shared RAM Memory", "name", "S07SHRAM", "base", 0x20008000L, "len", 0x10000L, "space", "code/data", "access", "RWX")}), Global.newArray(new Object[]{"CTOMMSGRAM", Global.newObject("comment", "C28 to M3 MSG RAM Memory", "name", "CTOMMSGRAM", "base", 0x2007F000L, "len", 0x800L, "space", "data", "access", "R")}), Global.newArray(new Object[]{"MTOCMSGRAM", Global.newObject("comment", "M3 to C28 MSG RAM Memory", "name", "MTOCMSGRAM", "base", 0x2007F800L, "len", 0x800L, "space", "data", "access", "RW")})}), "wh");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.F28M35x$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.F28M35x.Object", om.findStrict("ti.catalog.arm.cortexm3.F28M35x.Instance", "ti.catalog.arm.cortexm3"));
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
    }

    void Stellaris$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/arm/cortexm3/Stellaris.xs");
        om.bind("ti.catalog.arm.cortexm3.Stellaris$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.Stellaris.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.Stellaris.Module", om.findStrict("ti.catalog.ICpuDataSheet.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.Stellaris$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$Stellaris$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.Stellaris$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$Stellaris$$construct"));
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.Stellaris$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.Stellaris$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.Stellaris$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.Stellaris$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.Stellaris.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.Stellaris.Instance", om.findStrict("ti.catalog.ICpuDataSheet.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.Stellaris$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.Stellaris.Params", om.findStrict("ti.catalog.ICpuDataSheet$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.Stellaris$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.Stellaris.Object", om.findStrict("ti.catalog.arm.cortexm3.Stellaris.Instance", "ti.catalog.arm.cortexm3"));
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
    }

    void Tiva$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/arm/cortexm3/Tiva.xs");
        om.bind("ti.catalog.arm.cortexm3.Tiva$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.Tiva.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.Tiva.Module", om.findStrict("ti.catalog.ICpuDataSheet.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.Tiva$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$Tiva$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.Tiva$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$Tiva$$construct"));
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.Tiva$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.Tiva$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.Tiva$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.Tiva$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.Tiva.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.Tiva.Instance", om.findStrict("ti.catalog.ICpuDataSheet.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.Tiva$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.Tiva.Params", om.findStrict("ti.catalog.ICpuDataSheet$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.Tiva$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.Tiva.Object", om.findStrict("ti.catalog.arm.cortexm3.Tiva.Instance", "ti.catalog.arm.cortexm3"));
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
    }

    void CortexM3$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/arm/cortexm3/CortexM3.xs");
        om.bind("ti.catalog.arm.cortexm3.CortexM3$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.CortexM3.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.CortexM3.Module", om.findStrict("ti.catalog.ICpuDataSheet.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.CortexM3$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$CortexM3$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.CortexM3$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$CortexM3$$construct"));
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.CortexM3$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.CortexM3$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.CortexM3$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.CortexM3$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.CortexM3.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.CortexM3.Instance", om.findStrict("ti.catalog.ICpuDataSheet.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{}), "wh");
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.CortexM3$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.CortexM3.Params", om.findStrict("ti.catalog.ICpuDataSheet$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{}), "wh");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.CortexM3$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.CortexM3.Object", om.findStrict("ti.catalog.arm.cortexm3.CortexM3.Instance", "ti.catalog.arm.cortexm3"));
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
    }

    void ITI813X$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/arm/cortexm3/ITI813X.xs");
        om.bind("ti.catalog.arm.cortexm3.ITI813X$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI813X.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ITI813X.Module", om.findStrict("ti.catalog.ICpuDataSheet.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI813X$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI813X$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI813X$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI813X$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI813X.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ITI813X.Instance", om.findStrict("ti.catalog.ICpuDataSheet.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("hdvicp0", (Proto)om.findStrict("ti.catalog.peripherals.hdvicp2.HDVICP2.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"L2_BOOT", Global.newObject("name", "L2_BOOT", "base", 0x00000000L, "len", 0x4000L)}), Global.newArray(new Object[]{"L2_RAM", Global.newObject("name", "L2_RAM", "base", 0x20004000L, "len", 0x3C000L)}), Global.newArray(new Object[]{"OCMC", Global.newObject("name", "OCMC", "base", 0x00300000L, "len", 0x20000L)})}), "wh");
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI813X$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ITI813X.Params", om.findStrict("ti.catalog.ICpuDataSheet$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("hdvicp0", (Proto)om.findStrict("ti.catalog.peripherals.hdvicp2.HDVICP2.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"L2_BOOT", Global.newObject("name", "L2_BOOT", "base", 0x00000000L, "len", 0x4000L)}), Global.newArray(new Object[]{"L2_RAM", Global.newObject("name", "L2_RAM", "base", 0x20004000L, "len", 0x3C000L)}), Global.newArray(new Object[]{"OCMC", Global.newObject("name", "OCMC", "base", 0x00300000L, "len", 0x20000L)})}), "wh");
    }

    void TMS320TI813X$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI813X.Module", om.findStrict("ti.catalog.arm.cortexm3.ITI813X.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320TI813X$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320TI813X$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI813X.Instance", om.findStrict("ti.catalog.arm.cortexm3.ITI813X.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI813X.Params", om.findStrict("ti.catalog.arm.cortexm3.ITI813X$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI813X.Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void ITI811X$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/catalog/arm/cortexm3/ITI811X.xs");
        om.bind("ti.catalog.arm.cortexm3.ITI811X$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI811X.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ITI811X.Module", om.findStrict("ti.catalog.ICpuDataSheet.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI811X$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI811X$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI811X$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.catalog.arm.cortexm3.ITI811X$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI811X.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ITI811X.Instance", om.findStrict("ti.catalog.ICpuDataSheet.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("hdvicp0", (Proto)om.findStrict("ti.catalog.peripherals.hdvicp2.HDVICP2.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"L2_BOOT", Global.newObject("name", "L2_BOOT", "base", 0x00000000L, "len", 0x4000L)}), Global.newArray(new Object[]{"L2_RAM", Global.newObject("name", "L2_RAM", "base", 0x20004000L, "len", 0x3C000L)}), Global.newArray(new Object[]{"OCMC", Global.newObject("name", "OCMC", "base", 0x00300000L, "len", 0x20000L)})}), "wh");
                fxn = Global.get(cap, "getMemoryMap");
                if (fxn != null) po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), fxn);
                fxn = Global.get(cap, "getRegisterSet");
                if (fxn != null) po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), fxn);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI811X$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.ITI811X.Params", om.findStrict("ti.catalog.ICpuDataSheet$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("hdvicp0", (Proto)om.findStrict("ti.catalog.peripherals.hdvicp2.HDVICP2.Instance", "ti.catalog.arm.cortexm3"), $$UNDEF, "wh");
        po.addFld("cpuCore", $$T_Str, "CM3", "wh");
        po.addFld("isa", $$T_Str, "v7M", "wh");
        po.addFld("cpuCoreRevision", $$T_Str, "1.0", "wh");
        po.addFld("minProgUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("minDataUnitSize", Proto.Elm.newCNum("(xdc_Int)"), 1L, "wh");
        po.addFld("dataWordSize", Proto.Elm.newCNum("(xdc_Int)"), 4L, "wh");
        po.addFld("memMap", new Proto.Map((Proto)om.findStrict("xdc.platform.IPlatform.Memory", "ti.catalog.arm.cortexm3")), Global.newArray(new Object[]{Global.newArray(new Object[]{"L2_BOOT", Global.newObject("name", "L2_BOOT", "base", 0x00000000L, "len", 0x4000L)}), Global.newArray(new Object[]{"L2_RAM", Global.newObject("name", "L2_RAM", "base", 0x20004000L, "len", 0x3C000L)}), Global.newArray(new Object[]{"OCMC", Global.newObject("name", "OCMC", "base", 0x00300000L, "len", 0x20000L)})}), "wh");
    }

    void TMS320TI811X$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI811X.Module", om.findStrict("ti.catalog.arm.cortexm3.ITI811X.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320TI811X$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320TI811X$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI811X.Instance", om.findStrict("ti.catalog.arm.cortexm3.ITI811X.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI811X.Params", om.findStrict("ti.catalog.arm.cortexm3.ITI811X$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320TI811X.Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void TMS320C6A8149$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149.Module", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320C6A8149.Module", om.findStrict("ti.catalog.arm.cortexm3.ITI8148.Module", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("create", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149$$create", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320C6A8149$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149$$construct", "ti.catalog.arm.cortexm3"), Global.get("ti$catalog$arm$cortexm3$TMS320C6A8149$$construct"));
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149.Instance", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320C6A8149.Instance", om.findStrict("ti.catalog.arm.cortexm3.ITI8148.Instance", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149$$Params", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320C6A8149.Params", om.findStrict("ti.catalog.arm.cortexm3.ITI8148$$Params", "ti.catalog.arm.cortexm3"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149$$Object", "ti.catalog.arm.cortexm3");
        po.init("ti.catalog.arm.cortexm3.TMS320C6A8149.Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149.Instance", "ti.catalog.arm.cortexm3"));
                po.addFxn("getMemoryMap", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getMemoryMap", "ti.catalog.arm.cortexm3"), $$UNDEF);
                po.addFxn("getRegisterSet", (Proto.Fxn)om.findStrict("xdc.platform.ICpuDataSheet$$getRegisterSet", "ti.catalog.arm.cortexm3"), $$UNDEF);
    }

    void OMAP4430$$ROV()
    {
    }

    void ILM3Sx7xx$$ROV()
    {
    }

    void LM3S9792$$ROV()
    {
    }

    void LM3S9790$$ROV()
    {
    }

    void LM3S5791$$ROV()
    {
    }

    void LM3S2793$$ROV()
    {
    }

    void ILM3SxBxx$$ROV()
    {
    }

    void LM3S9B96$$ROV()
    {
    }

    void LM3S9B95$$ROV()
    {
    }

    void LM3S9B92$$ROV()
    {
    }

    void LM3S5B91$$ROV()
    {
    }

    void LM3S9B90$$ROV()
    {
    }

    void LM3S2B93$$ROV()
    {
    }

    void ILM3Sx9xx$$ROV()
    {
    }

    void LM3S8962$$ROV()
    {
    }

    void ITI8168$$ROV()
    {
    }

    void TMS320DM8168$$ROV()
    {
    }

    void TMS320TI816X$$ROV()
    {
    }

    void TMS320C6A8168$$ROV()
    {
    }

    void ITI8148$$ROV()
    {
    }

    void TMS320DM8148$$ROV()
    {
    }

    void TMS320TI814X$$ROV()
    {
    }

    void CC13xx$$ROV()
    {
    }

    void CC26xx$$ROV()
    {
    }

    void F28M35x$$ROV()
    {
    }

    void Stellaris$$ROV()
    {
    }

    void Tiva$$ROV()
    {
    }

    void CortexM3$$ROV()
    {
    }

    void ITI813X$$ROV()
    {
    }

    void TMS320TI813X$$ROV()
    {
    }

    void ITI811X$$ROV()
    {
    }

    void TMS320TI811X$$ROV()
    {
    }

    void TMS320C6A8149$$ROV()
    {
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.catalog.arm.cortexm3.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.catalog.arm.cortexm3"));
        pkgP.bind("$capsule", $$UNDEF);
        pkgV.init2(pkgP, "ti.catalog.arm.cortexm3", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.catalog.arm.cortexm3");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.catalog.arm.cortexm3.");
        pkgV.bind("$vers", Global.newArray(1, 0, 0));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        imports.add(Global.newArray("xdc.platform", Global.newArray()));
        imports.add(Global.newArray("ti.catalog.peripherals.hdvicp2", Global.newArray()));
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.catalog.arm.cortexm3'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("if ('ti.catalog.arm.cortexm3$$stat$base' in xdc.om) {\n");
            sb.append("pkg.packageBase = xdc.om['ti.catalog.arm.cortexm3$$stat$base'];\n");
            sb.append("pkg.packageRepository = xdc.om['ti.catalog.arm.cortexm3$$stat$root'];\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void OMAP4430$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.OMAP4430", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.OMAP4430.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.OMAP4430", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.catalog.arm.cortexm3.OMAP4430$$capsule", "ti.catalog.arm.cortexm3"));
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.OMAP4430.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.OMAP4430.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.OMAP4430.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.OMAP4430$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.OMAP4430.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("OMAP4430", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("OMAP4430");
    }

    void ILM3Sx7xx$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.ILM3Sx7xx", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Interface");
        vo.bind("$capsule", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx$$capsule", "ti.catalog.arm.cortexm3"));
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx7xx.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$interfaces")).add(vo);
        pkgV.bind("ILM3Sx7xx", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("ILM3Sx7xx");
        vo.seal(null);
    }

    void LM3S9792$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9792", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9792.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.LM3S9792", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.LM3S9792.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.LM3S9792.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.LM3S9792.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.LM3S9792$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S9792.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("LM3S9792", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LM3S9792");
    }

    void LM3S9790$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9790", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9790.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.LM3S9790", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.LM3S9790.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.LM3S9790.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.LM3S9790.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.LM3S9790$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S9790.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("LM3S9790", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LM3S9790");
    }

    void LM3S5791$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S5791", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S5791.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.LM3S5791", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.LM3S5791.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.LM3S5791.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.LM3S5791.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.LM3S5791$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S5791.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("LM3S5791", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LM3S5791");
    }

    void LM3S2793$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S2793", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S2793.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.LM3S2793", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.LM3S2793.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.LM3S2793.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.LM3S2793.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.LM3S2793$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S2793.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("LM3S2793", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LM3S2793");
    }

    void ILM3SxBxx$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.ILM3SxBxx", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Interface");
        vo.bind("$capsule", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx$$capsule", "ti.catalog.arm.cortexm3"));
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.ILM3SxBxx.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$interfaces")).add(vo);
        pkgV.bind("ILM3SxBxx", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("ILM3SxBxx");
        vo.seal(null);
    }

    void LM3S9B96$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.LM3S9B96", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.LM3S9B96$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("LM3S9B96", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LM3S9B96");
    }

    void LM3S9B95$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.LM3S9B95", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.LM3S9B95$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("LM3S9B95", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LM3S9B95");
    }

    void LM3S9B92$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.LM3S9B92", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.LM3S9B92$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("LM3S9B92", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LM3S9B92");
    }

    void LM3S5B91$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.LM3S5B91", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.LM3S5B91$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("LM3S5B91", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LM3S5B91");
    }

    void LM3S9B90$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.LM3S9B90", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.LM3S9B90$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("LM3S9B90", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LM3S9B90");
    }

    void LM3S2B93$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.LM3S2B93", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.LM3S2B93$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("LM3S2B93", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LM3S2B93");
    }

    void ILM3Sx9xx$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx9xx", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx9xx.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.ILM3Sx9xx", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Interface");
        vo.bind("$capsule", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx9xx$$capsule", "ti.catalog.arm.cortexm3"));
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx9xx.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx9xx.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.ILM3Sx9xx.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$interfaces")).add(vo);
        pkgV.bind("ILM3Sx9xx", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("ILM3Sx9xx");
        vo.seal(null);
    }

    void LM3S8962$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S8962", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.LM3S8962.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.LM3S8962", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.LM3S8962.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.LM3S8962.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.LM3S8962.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.LM3S8962$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.LM3S8962.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("LM3S8962", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("LM3S8962");
    }

    void ITI8168$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI8168", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI8168.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.ITI8168", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Interface");
        vo.bind("$capsule", om.findStrict("ti.catalog.arm.cortexm3.ITI8168$$capsule", "ti.catalog.arm.cortexm3"));
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.ITI8168.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.ITI8168.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.ITI8168.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$interfaces")).add(vo);
        pkgV.bind("ITI8168", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("ITI8168");
        vo.seal(null);
    }

    void TMS320DM8168$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.TMS320DM8168", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.TMS320DM8168$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("TMS320DM8168", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TMS320DM8168");
    }

    void TMS320TI816X$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.TMS320TI816X", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.TMS320TI816X$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("TMS320TI816X", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TMS320TI816X");
    }

    void TMS320C6A8168$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.TMS320C6A8168", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.TMS320C6A8168$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("TMS320C6A8168", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TMS320C6A8168");
    }

    void ITI8148$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI8148", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI8148.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.ITI8148", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Interface");
        vo.bind("$capsule", om.findStrict("ti.catalog.arm.cortexm3.ITI8148$$capsule", "ti.catalog.arm.cortexm3"));
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.ITI8148.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.ITI8148.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.ITI8148.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$interfaces")).add(vo);
        pkgV.bind("ITI8148", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("ITI8148");
        vo.seal(null);
    }

    void TMS320DM8148$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.TMS320DM8148", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.TMS320DM8148$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("TMS320DM8148", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TMS320DM8148");
    }

    void TMS320TI814X$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.TMS320TI814X", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.TMS320TI814X$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("TMS320TI814X", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TMS320TI814X");
    }

    void CC13xx$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.CC13xx", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.CC13xx.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.CC13xx", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.catalog.arm.cortexm3.CC13xx$$capsule", "ti.catalog.arm.cortexm3"));
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.CC13xx.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.CC13xx.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.CC13xx.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.CC13xx$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.CC13xx.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("CC13xx", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("CC13xx");
    }

    void CC26xx$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.CC26xx", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.CC26xx.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.CC26xx", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.catalog.arm.cortexm3.CC26xx$$capsule", "ti.catalog.arm.cortexm3"));
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.CC26xx.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.CC26xx.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.CC26xx.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.CC26xx$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.CC26xx.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("CC26xx", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("CC26xx");
    }

    void F28M35x$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.F28M35x", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.F28M35x.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.F28M35x", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.catalog.arm.cortexm3.F28M35x$$capsule", "ti.catalog.arm.cortexm3"));
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.F28M35x.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.F28M35x.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.F28M35x.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.F28M35x$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.F28M35x.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("F28M35x", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("F28M35x");
    }

    void Stellaris$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.Stellaris", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.Stellaris.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.Stellaris", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.catalog.arm.cortexm3.Stellaris$$capsule", "ti.catalog.arm.cortexm3"));
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.Stellaris.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.Stellaris.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.Stellaris.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.Stellaris$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.Stellaris.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("Stellaris", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Stellaris");
    }

    void Tiva$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.Tiva", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.Tiva.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.Tiva", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.catalog.arm.cortexm3.Tiva$$capsule", "ti.catalog.arm.cortexm3"));
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.Tiva.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.Tiva.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.Tiva.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.Tiva$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.Tiva.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("Tiva", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Tiva");
    }

    void CortexM3$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.CortexM3", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.CortexM3.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.CortexM3", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.catalog.arm.cortexm3.CortexM3$$capsule", "ti.catalog.arm.cortexm3"));
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.CortexM3.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.CortexM3.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.CortexM3.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.CortexM3$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.CortexM3.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("CortexM3", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("CortexM3");
    }

    void ITI813X$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI813X", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI813X.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.ITI813X", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Interface");
        vo.bind("$capsule", om.findStrict("ti.catalog.arm.cortexm3.ITI813X$$capsule", "ti.catalog.arm.cortexm3"));
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.ITI813X.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.ITI813X.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.ITI813X.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$interfaces")).add(vo);
        pkgV.bind("ITI813X", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("ITI813X");
        vo.seal(null);
    }

    void TMS320TI813X$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.TMS320TI813X", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.TMS320TI813X$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("TMS320TI813X", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TMS320TI813X");
    }

    void ITI811X$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI811X", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.ITI811X.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.ITI811X", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Interface");
        vo.bind("$capsule", om.findStrict("ti.catalog.arm.cortexm3.ITI811X$$capsule", "ti.catalog.arm.cortexm3"));
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.ITI811X.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.ITI811X.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.ITI811X.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$interfaces")).add(vo);
        pkgV.bind("ITI811X", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("ITI811X");
        vo.seal(null);
    }

    void TMS320TI811X$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.TMS320TI811X", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.TMS320TI811X$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("TMS320TI811X", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TMS320TI811X");
    }

    void TMS320C6A8149$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149", "ti.catalog.arm.cortexm3");
        po = (Proto.Obj)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149.Module", "ti.catalog.arm.cortexm3");
        vo.init2(po, "ti.catalog.arm.cortexm3.TMS320C6A8149", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", $$UNDEF);
        vo.bind("Instance", om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149.Instance", "ti.catalog.arm.cortexm3"));
        vo.bind("Params", om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149.Params", "ti.catalog.arm.cortexm3"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149.Params", "ti.catalog.arm.cortexm3")).newInstance());
        vo.bind("$package", om.findStrict("ti.catalog.arm.cortexm3", "ti.catalog.arm.cortexm3"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("ti.catalog.arm.cortexm3");
        inherits.add("ti.catalog");
        inherits.add("xdc.platform");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.catalog.arm.cortexm3")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("ti.catalog.arm.cortexm3.TMS320C6A8149$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149.Object", "ti.catalog.arm.cortexm3"));
        pkgV.bind("TMS320C6A8149", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TMS320C6A8149");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.OMAP4430", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.LM3S9792", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.LM3S9790", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.LM3S5791", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.LM3S2793", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B96", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B95", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B92", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.LM3S5B91", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.LM3S9B90", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.LM3S2B93", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.LM3S8962", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8168", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI816X", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8168", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.TMS320DM8148", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI814X", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.CC13xx", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.CC26xx", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.F28M35x", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.Stellaris", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.Tiva", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.CortexM3", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI813X", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.TMS320TI811X", "ti.catalog.arm.cortexm3"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.catalog.arm.cortexm3.TMS320C6A8149", "ti.catalog.arm.cortexm3"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.OMAP4430")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.ILM3Sx7xx")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.LM3S9792")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.LM3S9790")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.LM3S5791")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.LM3S2793")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.ILM3SxBxx")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.LM3S9B96")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.LM3S9B95")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.LM3S9B92")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.LM3S5B91")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.LM3S9B90")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.LM3S2B93")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.ILM3Sx9xx")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.LM3S8962")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.ITI8168")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.TMS320DM8168")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.TMS320TI816X")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.TMS320C6A8168")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.ITI8148")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.TMS320DM8148")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.TMS320TI814X")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.CC13xx")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.CC26xx")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.F28M35x")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.Stellaris")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.Tiva")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.CortexM3")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.ITI813X")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.TMS320TI813X")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.ITI811X")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.TMS320TI811X")).bless();
        ((Value.Obj)om.getv("ti.catalog.arm.cortexm3.TMS320C6A8149")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.catalog.arm.cortexm3")).add(pkgV);
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
        OMAP4430$$OBJECTS();
        ILM3Sx7xx$$OBJECTS();
        LM3S9792$$OBJECTS();
        LM3S9790$$OBJECTS();
        LM3S5791$$OBJECTS();
        LM3S2793$$OBJECTS();
        ILM3SxBxx$$OBJECTS();
        LM3S9B96$$OBJECTS();
        LM3S9B95$$OBJECTS();
        LM3S9B92$$OBJECTS();
        LM3S5B91$$OBJECTS();
        LM3S9B90$$OBJECTS();
        LM3S2B93$$OBJECTS();
        ILM3Sx9xx$$OBJECTS();
        LM3S8962$$OBJECTS();
        ITI8168$$OBJECTS();
        TMS320DM8168$$OBJECTS();
        TMS320TI816X$$OBJECTS();
        TMS320C6A8168$$OBJECTS();
        ITI8148$$OBJECTS();
        TMS320DM8148$$OBJECTS();
        TMS320TI814X$$OBJECTS();
        CC13xx$$OBJECTS();
        CC26xx$$OBJECTS();
        F28M35x$$OBJECTS();
        Stellaris$$OBJECTS();
        Tiva$$OBJECTS();
        CortexM3$$OBJECTS();
        ITI813X$$OBJECTS();
        TMS320TI813X$$OBJECTS();
        ITI811X$$OBJECTS();
        TMS320TI811X$$OBJECTS();
        TMS320C6A8149$$OBJECTS();
        OMAP4430$$CONSTS();
        ILM3Sx7xx$$CONSTS();
        LM3S9792$$CONSTS();
        LM3S9790$$CONSTS();
        LM3S5791$$CONSTS();
        LM3S2793$$CONSTS();
        ILM3SxBxx$$CONSTS();
        LM3S9B96$$CONSTS();
        LM3S9B95$$CONSTS();
        LM3S9B92$$CONSTS();
        LM3S5B91$$CONSTS();
        LM3S9B90$$CONSTS();
        LM3S2B93$$CONSTS();
        ILM3Sx9xx$$CONSTS();
        LM3S8962$$CONSTS();
        ITI8168$$CONSTS();
        TMS320DM8168$$CONSTS();
        TMS320TI816X$$CONSTS();
        TMS320C6A8168$$CONSTS();
        ITI8148$$CONSTS();
        TMS320DM8148$$CONSTS();
        TMS320TI814X$$CONSTS();
        CC13xx$$CONSTS();
        CC26xx$$CONSTS();
        F28M35x$$CONSTS();
        Stellaris$$CONSTS();
        Tiva$$CONSTS();
        CortexM3$$CONSTS();
        ITI813X$$CONSTS();
        TMS320TI813X$$CONSTS();
        ITI811X$$CONSTS();
        TMS320TI811X$$CONSTS();
        TMS320C6A8149$$CONSTS();
        OMAP4430$$CREATES();
        ILM3Sx7xx$$CREATES();
        LM3S9792$$CREATES();
        LM3S9790$$CREATES();
        LM3S5791$$CREATES();
        LM3S2793$$CREATES();
        ILM3SxBxx$$CREATES();
        LM3S9B96$$CREATES();
        LM3S9B95$$CREATES();
        LM3S9B92$$CREATES();
        LM3S5B91$$CREATES();
        LM3S9B90$$CREATES();
        LM3S2B93$$CREATES();
        ILM3Sx9xx$$CREATES();
        LM3S8962$$CREATES();
        ITI8168$$CREATES();
        TMS320DM8168$$CREATES();
        TMS320TI816X$$CREATES();
        TMS320C6A8168$$CREATES();
        ITI8148$$CREATES();
        TMS320DM8148$$CREATES();
        TMS320TI814X$$CREATES();
        CC13xx$$CREATES();
        CC26xx$$CREATES();
        F28M35x$$CREATES();
        Stellaris$$CREATES();
        Tiva$$CREATES();
        CortexM3$$CREATES();
        ITI813X$$CREATES();
        TMS320TI813X$$CREATES();
        ITI811X$$CREATES();
        TMS320TI811X$$CREATES();
        TMS320C6A8149$$CREATES();
        OMAP4430$$FUNCTIONS();
        ILM3Sx7xx$$FUNCTIONS();
        LM3S9792$$FUNCTIONS();
        LM3S9790$$FUNCTIONS();
        LM3S5791$$FUNCTIONS();
        LM3S2793$$FUNCTIONS();
        ILM3SxBxx$$FUNCTIONS();
        LM3S9B96$$FUNCTIONS();
        LM3S9B95$$FUNCTIONS();
        LM3S9B92$$FUNCTIONS();
        LM3S5B91$$FUNCTIONS();
        LM3S9B90$$FUNCTIONS();
        LM3S2B93$$FUNCTIONS();
        ILM3Sx9xx$$FUNCTIONS();
        LM3S8962$$FUNCTIONS();
        ITI8168$$FUNCTIONS();
        TMS320DM8168$$FUNCTIONS();
        TMS320TI816X$$FUNCTIONS();
        TMS320C6A8168$$FUNCTIONS();
        ITI8148$$FUNCTIONS();
        TMS320DM8148$$FUNCTIONS();
        TMS320TI814X$$FUNCTIONS();
        CC13xx$$FUNCTIONS();
        CC26xx$$FUNCTIONS();
        F28M35x$$FUNCTIONS();
        Stellaris$$FUNCTIONS();
        Tiva$$FUNCTIONS();
        CortexM3$$FUNCTIONS();
        ITI813X$$FUNCTIONS();
        TMS320TI813X$$FUNCTIONS();
        ITI811X$$FUNCTIONS();
        TMS320TI811X$$FUNCTIONS();
        TMS320C6A8149$$FUNCTIONS();
        OMAP4430$$SIZES();
        ILM3Sx7xx$$SIZES();
        LM3S9792$$SIZES();
        LM3S9790$$SIZES();
        LM3S5791$$SIZES();
        LM3S2793$$SIZES();
        ILM3SxBxx$$SIZES();
        LM3S9B96$$SIZES();
        LM3S9B95$$SIZES();
        LM3S9B92$$SIZES();
        LM3S5B91$$SIZES();
        LM3S9B90$$SIZES();
        LM3S2B93$$SIZES();
        ILM3Sx9xx$$SIZES();
        LM3S8962$$SIZES();
        ITI8168$$SIZES();
        TMS320DM8168$$SIZES();
        TMS320TI816X$$SIZES();
        TMS320C6A8168$$SIZES();
        ITI8148$$SIZES();
        TMS320DM8148$$SIZES();
        TMS320TI814X$$SIZES();
        CC13xx$$SIZES();
        CC26xx$$SIZES();
        F28M35x$$SIZES();
        Stellaris$$SIZES();
        Tiva$$SIZES();
        CortexM3$$SIZES();
        ITI813X$$SIZES();
        TMS320TI813X$$SIZES();
        ITI811X$$SIZES();
        TMS320TI811X$$SIZES();
        TMS320C6A8149$$SIZES();
        OMAP4430$$TYPES();
        ILM3Sx7xx$$TYPES();
        LM3S9792$$TYPES();
        LM3S9790$$TYPES();
        LM3S5791$$TYPES();
        LM3S2793$$TYPES();
        ILM3SxBxx$$TYPES();
        LM3S9B96$$TYPES();
        LM3S9B95$$TYPES();
        LM3S9B92$$TYPES();
        LM3S5B91$$TYPES();
        LM3S9B90$$TYPES();
        LM3S2B93$$TYPES();
        ILM3Sx9xx$$TYPES();
        LM3S8962$$TYPES();
        ITI8168$$TYPES();
        TMS320DM8168$$TYPES();
        TMS320TI816X$$TYPES();
        TMS320C6A8168$$TYPES();
        ITI8148$$TYPES();
        TMS320DM8148$$TYPES();
        TMS320TI814X$$TYPES();
        CC13xx$$TYPES();
        CC26xx$$TYPES();
        F28M35x$$TYPES();
        Stellaris$$TYPES();
        Tiva$$TYPES();
        CortexM3$$TYPES();
        ITI813X$$TYPES();
        TMS320TI813X$$TYPES();
        ITI811X$$TYPES();
        TMS320TI811X$$TYPES();
        TMS320C6A8149$$TYPES();
        if (isROV) {
            OMAP4430$$ROV();
            ILM3Sx7xx$$ROV();
            LM3S9792$$ROV();
            LM3S9790$$ROV();
            LM3S5791$$ROV();
            LM3S2793$$ROV();
            ILM3SxBxx$$ROV();
            LM3S9B96$$ROV();
            LM3S9B95$$ROV();
            LM3S9B92$$ROV();
            LM3S5B91$$ROV();
            LM3S9B90$$ROV();
            LM3S2B93$$ROV();
            ILM3Sx9xx$$ROV();
            LM3S8962$$ROV();
            ITI8168$$ROV();
            TMS320DM8168$$ROV();
            TMS320TI816X$$ROV();
            TMS320C6A8168$$ROV();
            ITI8148$$ROV();
            TMS320DM8148$$ROV();
            TMS320TI814X$$ROV();
            CC13xx$$ROV();
            CC26xx$$ROV();
            F28M35x$$ROV();
            Stellaris$$ROV();
            Tiva$$ROV();
            CortexM3$$ROV();
            ITI813X$$ROV();
            TMS320TI813X$$ROV();
            ITI811X$$ROV();
            TMS320TI811X$$ROV();
            TMS320C6A8149$$ROV();
        }//isROV
        $$SINGLETONS();
        OMAP4430$$SINGLETONS();
        ILM3Sx7xx$$SINGLETONS();
        LM3S9792$$SINGLETONS();
        LM3S9790$$SINGLETONS();
        LM3S5791$$SINGLETONS();
        LM3S2793$$SINGLETONS();
        ILM3SxBxx$$SINGLETONS();
        LM3S9B96$$SINGLETONS();
        LM3S9B95$$SINGLETONS();
        LM3S9B92$$SINGLETONS();
        LM3S5B91$$SINGLETONS();
        LM3S9B90$$SINGLETONS();
        LM3S2B93$$SINGLETONS();
        ILM3Sx9xx$$SINGLETONS();
        LM3S8962$$SINGLETONS();
        ITI8168$$SINGLETONS();
        TMS320DM8168$$SINGLETONS();
        TMS320TI816X$$SINGLETONS();
        TMS320C6A8168$$SINGLETONS();
        ITI8148$$SINGLETONS();
        TMS320DM8148$$SINGLETONS();
        TMS320TI814X$$SINGLETONS();
        CC13xx$$SINGLETONS();
        CC26xx$$SINGLETONS();
        F28M35x$$SINGLETONS();
        Stellaris$$SINGLETONS();
        Tiva$$SINGLETONS();
        CortexM3$$SINGLETONS();
        ITI813X$$SINGLETONS();
        TMS320TI813X$$SINGLETONS();
        ITI811X$$SINGLETONS();
        TMS320TI811X$$SINGLETONS();
        TMS320C6A8149$$SINGLETONS();
        $$INITIALIZATION();
    }
}
