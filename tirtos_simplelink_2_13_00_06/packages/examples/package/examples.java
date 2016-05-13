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

public class examples
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
        Global.callFxn("loadPackage", xdcO, "xdc");
        Global.callFxn("loadPackage", xdcO, "xdc.corevers");
        Global.callFxn("loadPackage", xdcO, "xdc.tools.product");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("examples.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("examples", new Value.Obj("examples", pkgP));
    }

    void TrexExamples$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("examples.TrexExamples.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("examples.TrexExamples", new Value.Obj("examples.TrexExamples", po));
        pkgV.bind("TrexExamples", vo);
        // decls 
        om.bind("examples.TrexExamples.FileDesc", om.findStrict("xdc.tools.product.IProductTemplate.FileDesc", "examples"));
        om.bind("examples.TrexExamples.Filter", om.findStrict("xdc.tools.product.IProductTemplate.Filter", "examples"));
        om.bind("examples.TrexExamples.TemplateInfo", om.findStrict("xdc.tools.product.IProductTemplate.TemplateInfo", "examples"));
        om.bind("examples.TrexExamples.TemplateGroup", om.findStrict("xdc.tools.product.IProductTemplate.TemplateGroup", "examples"));
    }

    void TrexExamples$$CONSTS()
    {
        // module TrexExamples
    }

    void TrexExamples$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void TrexExamples$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void TrexExamples$$SIZES()
    {
    }

    void TrexExamples$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "examples/TrexExamples.xs");
        om.bind("examples.TrexExamples$$capsule", cap);
        po = (Proto.Obj)om.findStrict("examples.TrexExamples.Module", "examples");
        po.init("examples.TrexExamples.Module", om.findStrict("xdc.tools.product.IProductTemplate.Module", "examples"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("examples.TrexExamples$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("examples.TrexExamples$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("examples.TrexExamples$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
    }

    void TrexExamples$$ROV()
    {
    }

    void $$SINGLETONS()
    {
        pkgP.init("examples.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "examples"));
        pkgP.bind("$capsule", $$UNDEF);
        pkgV.init2(pkgP, "examples", Value.DEFAULT, false);
        pkgV.bind("$name", "examples");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "examples.");
        pkgV.bind("$vers", Global.newArray());
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['examples'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("if ('examples$$stat$base' in xdc.om) {\n");
            sb.append("pkg.packageBase = xdc.om['examples$$stat$base'];\n");
            sb.append("pkg.packageRepository = xdc.om['examples$$stat$root'];\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void TrexExamples$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("examples.TrexExamples", "examples");
        po = (Proto.Obj)om.findStrict("examples.TrexExamples.Module", "examples");
        vo.init2(po, "examples.TrexExamples", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("examples.TrexExamples$$capsule", "examples"));
        vo.bind("$package", om.findStrict("examples", "examples"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("FileDesc", om.findStrict("xdc.tools.product.IProductTemplate.FileDesc", "examples"));
        tdefs.add(om.findStrict("xdc.tools.product.IProductTemplate.FileDesc", "examples"));
        vo.bind("Filter", om.findStrict("xdc.tools.product.IProductTemplate.Filter", "examples"));
        tdefs.add(om.findStrict("xdc.tools.product.IProductTemplate.Filter", "examples"));
        vo.bind("TemplateInfo", om.findStrict("xdc.tools.product.IProductTemplate.TemplateInfo", "examples"));
        tdefs.add(om.findStrict("xdc.tools.product.IProductTemplate.TemplateInfo", "examples"));
        vo.bind("TemplateGroup", om.findStrict("xdc.tools.product.IProductTemplate.TemplateGroup", "examples"));
        tdefs.add(om.findStrict("xdc.tools.product.IProductTemplate.TemplateGroup", "examples"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.tools.product");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "examples")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("examples.TrexExamples$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        pkgV.bind("TrexExamples", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("TrexExamples");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("examples.TrexExamples", "examples"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("examples.TrexExamples")).bless();
        ((Value.Arr)om.findStrict("$packages", "examples")).add(pkgV);
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
        TrexExamples$$OBJECTS();
        TrexExamples$$CONSTS();
        TrexExamples$$CREATES();
        TrexExamples$$FUNCTIONS();
        TrexExamples$$SIZES();
        TrexExamples$$TYPES();
        if (isROV) {
            TrexExamples$$ROV();
        }//isROV
        $$SINGLETONS();
        TrexExamples$$SINGLETONS();
        $$INITIALIZATION();
    }
}
