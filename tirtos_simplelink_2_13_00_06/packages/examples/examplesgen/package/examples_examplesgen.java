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

public class examples_examplesgen
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
        Global.callFxn("loadPackage", xdcO, "xdc.tools");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("examples.examplesgen.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("examples.examplesgen", new Value.Obj("examples.examplesgen", pkgP));
    }

    void Main$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("examples.examplesgen.Main.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("examples.examplesgen.Main", new Value.Obj("examples.examplesgen.Main", po));
        pkgV.bind("Main", vo);
        // decls 
        // insts 
        Object insP = om.bind("examples.examplesgen.Main.Instance", new Proto.Obj());
        po = (Proto.Obj)om.bind("examples.examplesgen.Main$$Object", new Proto.Obj());
        om.bind("examples.examplesgen.Main.Object", new Proto.Str(po, true));
        po = (Proto.Obj)om.bind("examples.examplesgen.Main$$Params", new Proto.Obj());
        om.bind("examples.examplesgen.Main.Params", new Proto.Str(po, true));
    }

    void Main$$CONSTS()
    {
        // module Main
    }

    void Main$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

        fxn = (Proto.Fxn)om.bind("examples.examplesgen.Main$$create", new Proto.Fxn(om.findStrict("examples.examplesgen.Main.Module", "examples.examplesgen"), om.findStrict("examples.examplesgen.Main.Instance", "examples.examplesgen"), 1, 0, false));
                fxn.addArg(0, "__params", (Proto)om.findStrict("examples.examplesgen.Main.Params", "examples.examplesgen"), Global.newObject());
        sb = new StringBuilder();
        sb.append("examples$examplesgen$Main$$create = function( __params ) {\n");
            sb.append("var __mod = xdc.om['examples.examplesgen.Main'];\n");
            sb.append("var __inst = xdc.om['examples.examplesgen.Main.Instance'].$$make();\n");
            sb.append("__inst.$$bind('$package', xdc.om['examples.examplesgen']);\n");
            sb.append("__inst.$$bind('$index', __mod.$instances.length);\n");
            sb.append("__inst.$$bind('$category', 'Instance');\n");
            sb.append("__inst.$$bind('$args', {});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$instances.$add(__inst);\n");
            sb.append("__inst.tirtosProductDir = __mod.PARAMS.tirtosProductDir;\n");
            sb.append("__inst.toolChain = __mod.PARAMS.toolChain;\n");
            sb.append("__inst.outputDir = __mod.PARAMS.outputDir;\n");
            sb.append("__inst.xdctools = __mod.PARAMS.xdctools;\n");
            sb.append("__inst.bios = __mod.PARAMS.bios;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("var save = xdc.om.$curpkg;\n");
            sb.append("xdc.om.$$bind('$curpkg', __mod.$package.$name);\n");
            sb.append("__mod.instance$meta$init.$fxn.apply(__inst, []);\n");
            sb.append("xdc.om.$$bind('$curpkg', save);\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return __inst;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
        fxn = (Proto.Fxn)om.bind("examples.examplesgen.Main$$construct", new Proto.Fxn(om.findStrict("examples.examplesgen.Main.Module", "examples.examplesgen"), null, 2, 0, false));
                fxn.addArg(0, "__obj", (Proto)om.findStrict("examples.examplesgen.Main$$Object", "examples.examplesgen"), null);
                fxn.addArg(1, "__params", (Proto)om.findStrict("examples.examplesgen.Main.Params", "examples.examplesgen"), Global.newObject());
        sb = new StringBuilder();
        sb.append("examples$examplesgen$Main$$construct = function( __obj, __params ) {\n");
            sb.append("var __mod = xdc.om['examples.examplesgen.Main'];\n");
            sb.append("var __inst = __obj;\n");
            sb.append("__inst.$$bind('$args', {});\n");
            sb.append("__inst.$$bind('$module', __mod);\n");
            sb.append("__mod.$objects.$add(__inst);\n");
            sb.append("__inst.tirtosProductDir = __mod.PARAMS.tirtosProductDir;\n");
            sb.append("__inst.toolChain = __mod.PARAMS.toolChain;\n");
            sb.append("__inst.outputDir = __mod.PARAMS.outputDir;\n");
            sb.append("__inst.xdctools = __mod.PARAMS.xdctools;\n");
            sb.append("__inst.bios = __mod.PARAMS.bios;\n");
            sb.append("for (var __p in __params) __inst[__p] = __params[__p];\n");
            sb.append("__inst.$$bless();\n");
            sb.append("return null;\n");
        sb.append("}\n");
        Global.eval(sb.toString());
    }

    void Main$$FUNCTIONS()
    {
        Proto.Fxn fxn;

        // fxn Main.run
        fxn = (Proto.Fxn)om.bind("examples.examplesgen.Main$$run", new Proto.Fxn(om.findStrict("examples.examplesgen.Main.Instance", "examples.examplesgen"), $$T_Obj, 2, 2, false));
                fxn.addArg(0, "cmdr", (Proto)om.findStrict("xdc.tools.Cmdr.Instance", "examples.examplesgen"), $$UNDEF);
                fxn.addArg(1, "args", new Proto.Arr($$T_Str, false), $$DEFAULT);
    }

    void Main$$SIZES()
    {
    }

    void Main$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "examples/examplesgen/Main.xs");
        om.bind("examples.examplesgen.Main$$capsule", cap);
        po = (Proto.Obj)om.findStrict("examples.examplesgen.Main.Module", "examples.examplesgen");
        po.init("examples.examplesgen.Main.Module", om.findStrict("xdc.tools.ICmd.Module", "examples.examplesgen"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("usage", new Proto.Arr($$T_Str, false), Global.newArray(new Object[]{"[--tirtos           tirtos_product_directory]", "[--toolchain        toolchain(e.g IAR)]", "[--devicefamily     device family (e.g. ARM, MSP430, MSP432, C2000)]", "[--output           output directory]", "[--xdctools         XDC tools install path]", "[--bios             BIOS install path]", "[--uia              UIA install path]", "[--ipc              IPC install path]", "[--ndk              NDK install path]", "[--tivaware         TivaWare install path]", "[--mware            MWare install path]", "[--ccware           CCWare install path]", "[--cc13xxware       CC13XXWare install path]", "[--cc26xxware       CC26XXWare install path]", "[--mspware          MSPWare install path]", "[--codegendir       Codegen install directory]", "[--idefiles         Generates IDE files]", "[--examplelistFile  Override default ExamplesList.xs]", "[--boardsFile       Override default Boards.xs]", "[--testlinkSupport  Add testlink markup text to generated examples]"}), "wh");
                po.addFxn("create", (Proto.Fxn)om.findStrict("examples.examplesgen.Main$$create", "examples.examplesgen"), Global.get("examples$examplesgen$Main$$create"));
                po.addFxn("construct", (Proto.Fxn)om.findStrict("examples.examplesgen.Main$$construct", "examples.examplesgen"), Global.get("examples$examplesgen$Main$$construct"));
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("examples.examplesgen.Main$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("examples.examplesgen.Main$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "instance$meta$init");
        if (fxn != null) om.bind("examples.examplesgen.Main$$instance$meta$init", true);
        if (fxn != null) po.addFxn("instance$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("examples.examplesgen.Main$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
                fxn = Global.get(cap, "main");
                if (fxn != null) po.addFxn("main", (Proto.Fxn)om.findStrict("xdc.tools.ICmd$$main", "examples.examplesgen"), fxn);
                fxn = Global.get(cap, "exec");
                if (fxn != null) po.addFxn("exec", (Proto.Fxn)om.findStrict("xdc.tools.ICmd$$exec", "examples.examplesgen"), fxn);
        po = (Proto.Obj)om.findStrict("examples.examplesgen.Main.Instance", "examples.examplesgen");
        po.init("examples.examplesgen.Main.Instance", om.findStrict("xdc.tools.ICmd.Instance", "examples.examplesgen"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("tirtosProductDir", $$T_Str, $$UNDEF, "wh");
        po.addFld("toolChain", $$T_Str, $$UNDEF, "wh");
        po.addFld("deviceFamily", $$T_Str, null, "wh");
        po.addFld("outputDir", $$T_Str, $$UNDEF, "wh");
        po.addFld("xdctools", $$T_Str, $$UNDEF, "wh");
        po.addFld("bios", $$T_Str, $$UNDEF, "wh");
        po.addFld("uia", $$T_Str, "", "wh");
        po.addFld("ipc", $$T_Str, "", "wh");
        po.addFld("ndk", $$T_Str, "", "wh");
        po.addFld("tivaware", $$T_Str, "", "wh");
        po.addFld("mware", $$T_Str, "", "wh");
        po.addFld("ccware", $$T_Str, "", "wh");
        po.addFld("cc13xxware", $$T_Str, "", "wh");
        po.addFld("cc26xxware", $$T_Str, "", "wh");
        po.addFld("mspware", $$T_Str, "", "wh");
        po.addFld("toolChaindir", $$T_Str, null, "wh");
        po.addFld("optCopts", $$T_Str, "", "wh");
        po.addFld("optLopts", $$T_Str, "", "wh");
        po.addFld("extraXDCPATH", $$T_Str, "", "wh");
        po.addFld("productGroup", $$T_Str, null, "wh");
        po.addFld("idefiles", $$T_Bool, true, "wh");
        po.addFld("examplelistFile", $$T_Str, "", "wh");
        po.addFld("boardsFile", $$T_Str, "", "wh");
        po.addFld("testlinkSupport", $$T_Bool, false, "wh");
                fxn = Global.get(cap, "run");
                if (fxn != null) po.addFxn("run", (Proto.Fxn)om.findStrict("xdc.tools.ICmd$$run", "examples.examplesgen"), fxn);
        po = (Proto.Obj)om.findStrict("examples.examplesgen.Main$$Params", "examples.examplesgen");
        po.init("examples.examplesgen.Main.Params", om.findStrict("xdc.tools.ICmd$$Params", "examples.examplesgen"));
                po.addFld("$hostonly", $$T_Num, 1, "r");
        po.addFld("tirtosProductDir", $$T_Str, $$UNDEF, "wh");
        po.addFld("toolChain", $$T_Str, $$UNDEF, "wh");
        po.addFld("deviceFamily", $$T_Str, null, "wh");
        po.addFld("outputDir", $$T_Str, $$UNDEF, "wh");
        po.addFld("xdctools", $$T_Str, $$UNDEF, "wh");
        po.addFld("bios", $$T_Str, $$UNDEF, "wh");
        po.addFld("uia", $$T_Str, "", "wh");
        po.addFld("ipc", $$T_Str, "", "wh");
        po.addFld("ndk", $$T_Str, "", "wh");
        po.addFld("tivaware", $$T_Str, "", "wh");
        po.addFld("mware", $$T_Str, "", "wh");
        po.addFld("ccware", $$T_Str, "", "wh");
        po.addFld("cc13xxware", $$T_Str, "", "wh");
        po.addFld("cc26xxware", $$T_Str, "", "wh");
        po.addFld("mspware", $$T_Str, "", "wh");
        po.addFld("toolChaindir", $$T_Str, null, "wh");
        po.addFld("optCopts", $$T_Str, "", "wh");
        po.addFld("optLopts", $$T_Str, "", "wh");
        po.addFld("extraXDCPATH", $$T_Str, "", "wh");
        po.addFld("productGroup", $$T_Str, null, "wh");
        po.addFld("idefiles", $$T_Bool, true, "wh");
        po.addFld("examplelistFile", $$T_Str, "", "wh");
        po.addFld("boardsFile", $$T_Str, "", "wh");
        po.addFld("testlinkSupport", $$T_Bool, false, "wh");
        po = (Proto.Obj)om.findStrict("examples.examplesgen.Main$$Object", "examples.examplesgen");
        po.init("examples.examplesgen.Main.Object", om.findStrict("examples.examplesgen.Main.Instance", "examples.examplesgen"));
                fxn = Global.get(cap, "run");
                if (fxn != null) po.addFxn("run", (Proto.Fxn)om.findStrict("xdc.tools.ICmd$$run", "examples.examplesgen"), fxn);
    }

    void Main$$ROV()
    {
    }

    void $$SINGLETONS()
    {
        pkgP.init("examples.examplesgen.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "examples.examplesgen"));
        pkgP.bind("$capsule", $$UNDEF);
        pkgV.init2(pkgP, "examples.examplesgen", Value.DEFAULT, false);
        pkgV.bind("$name", "examples.examplesgen");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "examples.examplesgen.");
        pkgV.bind("$vers", Global.newArray(1, 0, 0));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['examples.examplesgen'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("if ('examples.examplesgen$$stat$base' in xdc.om) {\n");
            sb.append("pkg.packageBase = xdc.om['examples.examplesgen$$stat$base'];\n");
            sb.append("pkg.packageRepository = xdc.om['examples.examplesgen$$stat$root'];\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void Main$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("examples.examplesgen.Main", "examples.examplesgen");
        po = (Proto.Obj)om.findStrict("examples.examplesgen.Main.Module", "examples.examplesgen");
        vo.init2(po, "examples.examplesgen.Main", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("examples.examplesgen.Main$$capsule", "examples.examplesgen"));
        vo.bind("Instance", om.findStrict("examples.examplesgen.Main.Instance", "examples.examplesgen"));
        vo.bind("Params", om.findStrict("examples.examplesgen.Main.Params", "examples.examplesgen"));
        vo.bind("PARAMS", ((Proto.Str)om.findStrict("examples.examplesgen.Main.Params", "examples.examplesgen")).newInstance());
        vo.bind("$package", om.findStrict("examples.examplesgen", "examples.examplesgen"));
        tdefs.clear();
        proxies.clear();
        mcfgs.clear();
        icfgs.clear();
        inherits.clear();
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.tools");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "examples.examplesgen")).add(vo);
        vo.bind("$$instflag", 1);
        vo.bind("$$iobjflag", 1);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 1);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 0);
        Proto.Str ps = (Proto.Str)vo.find("Module_State");
        if (ps != null) vo.bind("$object", ps.newInstance());
        vo.bind("$$meta_iobj", om.has("examples.examplesgen.Main$$instance$static$init", null) ? 1 : 0);
        vo.bind("$$fxntab", Global.newArray());
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.seal("length");
        vo.bind("Object", om.findStrict("examples.examplesgen.Main.Object", "examples.examplesgen"));
        pkgV.bind("Main", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Main");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("examples.examplesgen.Main", "examples.examplesgen"));
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("examples.examplesgen.Main")).bless();
        ((Value.Arr)om.findStrict("$packages", "examples.examplesgen")).add(pkgV);
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
        Main$$OBJECTS();
        Main$$CONSTS();
        Main$$CREATES();
        Main$$FUNCTIONS();
        Main$$SIZES();
        Main$$TYPES();
        if (isROV) {
            Main$$ROV();
        }//isROV
        $$SINGLETONS();
        Main$$SINGLETONS();
        $$INITIALIZATION();
    }
}
