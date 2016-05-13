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

public class ti_drivers
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
        Global.callFxn("loadPackage", xdcO, "xdc.rov");
        Global.callFxn("loadPackage", xdcO, "xdc.runtime");
        Global.callFxn("loadPackage", xdcO, "ti.sysbios.knl");
        Global.callFxn("loadPackage", xdcO, "ti.sysbios.hal");
    }

    void $$OBJECTS()
    {
        pkgP = (Proto.Obj)om.bind("ti.drivers.Package", new Proto.Obj());
        pkgV = (Value.Obj)om.bind("ti.drivers", new Value.Obj("ti.drivers", pkgP));
    }

    void EMAC$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.drivers.EMAC.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.drivers.EMAC", new Value.Obj("ti.drivers.EMAC", po));
        pkgV.bind("EMAC", vo);
        // decls 
        om.bind("ti.drivers.EMAC.LibType", new Proto.Enm("ti.drivers.EMAC.LibType"));
        spo = (Proto.Obj)om.bind("ti.drivers.EMAC$$EMAC_Config", new Proto.Obj());
        om.bind("ti.drivers.EMAC.EMAC_Config", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.EMAC$$EMACTiva_Object", new Proto.Obj());
        om.bind("ti.drivers.EMAC.EMACTiva_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.EMAC$$EMACTiva_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.EMAC.EMACTiva_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.EMAC$$EMACSnow_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.EMAC.EMACSnow_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.EMAC$$EMACSnow_Data", new Proto.Obj());
        om.bind("ti.drivers.EMAC.EMACSnow_Data", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.EMAC$$EMACSnow_Object", new Proto.Obj());
        om.bind("ti.drivers.EMAC.EMACSnow_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.EMAC$$BasicView", new Proto.Obj());
        om.bind("ti.drivers.EMAC.BasicView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.EMAC$$StatsView", new Proto.Obj());
        om.bind("ti.drivers.EMAC.StatsView", new Proto.Str(spo, false));
    }

    void GPIO$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.drivers.GPIO.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.drivers.GPIO", new Value.Obj("ti.drivers.GPIO", po));
        pkgV.bind("GPIO", vo);
        // decls 
        om.bind("ti.drivers.GPIO.LibType", new Proto.Enm("ti.drivers.GPIO.LibType"));
        spo = (Proto.Obj)om.bind("ti.drivers.GPIO$$GPIO_Config", new Proto.Obj());
        om.bind("ti.drivers.GPIO.GPIO_Config", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.GPIO$$BasicView", new Proto.Obj());
        om.bind("ti.drivers.GPIO.BasicView", new Proto.Str(spo, false));
    }

    void I2C$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.drivers.I2C.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.drivers.I2C", new Value.Obj("ti.drivers.I2C", po));
        pkgV.bind("I2C", vo);
        // decls 
        om.bind("ti.drivers.I2C.LibType", new Proto.Enm("ti.drivers.I2C.LibType"));
        spo = (Proto.Obj)om.bind("ti.drivers.I2C$$I2C_Config", new Proto.Obj());
        om.bind("ti.drivers.I2C.I2C_Config", new Proto.Str(spo, false));
        om.bind("ti.drivers.I2C.I2C_TransferMode", new Proto.Enm("ti.drivers.I2C.I2C_TransferMode"));
        spo = (Proto.Obj)om.bind("ti.drivers.I2C$$I2C_Transaction", new Proto.Obj());
        om.bind("ti.drivers.I2C.I2C_Transaction", new Proto.Str(spo, false));
        om.bind("ti.drivers.I2C.I2C_Mode", new Proto.Enm("ti.drivers.I2C.I2C_Mode"));
        spo = (Proto.Obj)om.bind("ti.drivers.I2C$$I2C_Params", new Proto.Obj());
        om.bind("ti.drivers.I2C.I2C_Params", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.I2C$$I2CTiva_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.I2C.I2CTiva_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.I2C$$I2CTiva_Object", new Proto.Obj());
        om.bind("ti.drivers.I2C.I2CTiva_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.I2C$$I2CCC3200_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.I2C.I2CCC3200_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.I2C$$I2CCC3200_Object", new Proto.Obj());
        om.bind("ti.drivers.I2C.I2CCC3200_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.I2C$$I2CMSP432_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.I2C.I2CMSP432_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.I2C$$I2CMSP432_Object", new Proto.Obj());
        om.bind("ti.drivers.I2C.I2CMSP432_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.I2C$$I2CUSCIB_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.I2C.I2CUSCIB_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.I2C$$I2CEUSCIB_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.I2C.I2CEUSCIB_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.I2C$$I2CUSCIB_Object", new Proto.Obj());
        om.bind("ti.drivers.I2C.I2CUSCIB_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.I2C$$I2CEUSCIB_Object", new Proto.Obj());
        om.bind("ti.drivers.I2C.I2CEUSCIB_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.I2C$$BasicView", new Proto.Obj());
        om.bind("ti.drivers.I2C.BasicView", new Proto.Str(spo, false));
    }

    void Power$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.drivers.Power.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.drivers.Power", new Value.Obj("ti.drivers.Power", po));
        pkgV.bind("Power", vo);
        // decls 
    }

    void PWM$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.drivers.PWM.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.drivers.PWM", new Value.Obj("ti.drivers.PWM", po));
        pkgV.bind("PWM", vo);
        // decls 
        om.bind("ti.drivers.PWM.LibType", new Proto.Enm("ti.drivers.PWM.LibType"));
        spo = (Proto.Obj)om.bind("ti.drivers.PWM$$PWM_Config", new Proto.Obj());
        om.bind("ti.drivers.PWM.PWM_Config", new Proto.Str(spo, false));
        om.bind("ti.drivers.PWM.PWM_DutyMode", new Proto.Enm("ti.drivers.PWM.PWM_DutyMode"));
        om.bind("ti.drivers.PWM.PWM_Polarity", new Proto.Enm("ti.drivers.PWM.PWM_Polarity"));
        spo = (Proto.Obj)om.bind("ti.drivers.PWM$$PWM_Params", new Proto.Obj());
        om.bind("ti.drivers.PWM.PWM_Params", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.PWM$$PWMTimerCC3200_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.PWM.PWMTimerCC3200_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.PWM$$PWMTimerCC3200_Object", new Proto.Obj());
        om.bind("ti.drivers.PWM.PWMTimerCC3200_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.PWM$$PWMTiva_Status", new Proto.Obj());
        om.bind("ti.drivers.PWM.PWMTiva_Status", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.PWM$$PWMTiva_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.PWM.PWMTiva_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.PWM$$PWMTiva_Object", new Proto.Obj());
        om.bind("ti.drivers.PWM.PWMTiva_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.PWM$$PWMTimerTiva_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.PWM.PWMTimerTiva_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.PWM$$PWMTimerTiva_Object", new Proto.Obj());
        om.bind("ti.drivers.PWM.PWMTimerTiva_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.PWM$$PWMTimerMSP432_Status", new Proto.Obj());
        om.bind("ti.drivers.PWM.PWMTimerMSP432_Status", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.PWM$$PWMTimerMSP432_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.PWM.PWMTimerMSP432_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.PWM$$PWMTimerMSP432_Object", new Proto.Obj());
        om.bind("ti.drivers.PWM.PWMTimerMSP432_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.PWM$$BasicView", new Proto.Obj());
        om.bind("ti.drivers.PWM.BasicView", new Proto.Str(spo, false));
    }

    void SDSPI$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.drivers.SDSPI.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.drivers.SDSPI", new Value.Obj("ti.drivers.SDSPI", po));
        pkgV.bind("SDSPI", vo);
        // decls 
        om.bind("ti.drivers.SDSPI.LibType", new Proto.Enm("ti.drivers.SDSPI.LibType"));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$SDSPI_Config", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.SDSPI_Config", new Proto.Str(spo, false));
        om.bind("ti.drivers.SDSPI.SDSPITiva_CardType", new Proto.Enm("ti.drivers.SDSPI.SDSPITiva_CardType"));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$SDSPITiva_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.SDSPITiva_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$SDSPITiva_Object", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.SDSPITiva_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$SDSPIUSCIA_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.SDSPIUSCIA_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$SDSPIUSCIB_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.SDSPIUSCIB_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$SDSPIUSCIA_Object", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.SDSPIUSCIA_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$SDSPIUSCIB_Object", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.SDSPIUSCIB_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$SDSPIEUSCIA_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.SDSPIEUSCIA_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$SDSPIEUSCIB_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.SDSPIEUSCIB_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$SDSPIEUSCIA_Object", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.SDSPIEUSCIA_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$SDSPIEUSCIB_Object", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.SDSPIEUSCIB_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$SDSPICC3200_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.SDSPICC3200_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$SDSPICC3200_Object", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.SDSPICC3200_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$SDSPIMSP432_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.SDSPIMSP432_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$SDSPIMSP432_Object", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.SDSPIMSP432_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SDSPI$$BasicView", new Proto.Obj());
        om.bind("ti.drivers.SDSPI.BasicView", new Proto.Str(spo, false));
    }

    void SPI$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.drivers.SPI.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.drivers.SPI", new Value.Obj("ti.drivers.SPI", po));
        pkgV.bind("SPI", vo);
        // decls 
        om.bind("ti.drivers.SPI.LibType", new Proto.Enm("ti.drivers.SPI.LibType"));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPI_Config", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPI_Config", new Proto.Str(spo, false));
        om.bind("ti.drivers.SPI.TransferMode", new Proto.Enm("ti.drivers.SPI.TransferMode"));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPI_Transaction", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPI_Transaction", new Proto.Str(spo, false));
        om.bind("ti.drivers.SPI.SPITivaDMA_FrameSize", new Proto.Enm("ti.drivers.SPI.SPITivaDMA_FrameSize"));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPITivaDMA_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPITivaDMA_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPITivaDMA_Object", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPITivaDMA_Object", new Proto.Str(spo, false));
        om.bind("ti.drivers.SPI.SPICC3200DMA_FrameSize", new Proto.Enm("ti.drivers.SPI.SPICC3200DMA_FrameSize"));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPICC3200DMA_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPICC3200DMA_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPICC3200DMA_Object", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPICC3200DMA_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPIUSCIADMA_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPIUSCIADMA_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPIUSCIADMA_Object", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPIUSCIADMA_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPIUSCIBDMA_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPIUSCIBDMA_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPIUSCIBDMA_Object", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPIUSCIBDMA_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPIEUSCIADMA_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPIEUSCIADMA_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPIEUSCIADMA_Object", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPIEUSCIADMA_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPIEUSCIBDMA_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPIEUSCIBDMA_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPIEUSCIBDMA_Object", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPIEUSCIBDMA_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPIMSP432DMA_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPIMSP432DMA_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$SPIMSP432DMA_Object", new Proto.Obj());
        om.bind("ti.drivers.SPI.SPIMSP432DMA_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.SPI$$BasicView", new Proto.Obj());
        om.bind("ti.drivers.SPI.BasicView", new Proto.Str(spo, false));
    }

    void UART$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.drivers.UART.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.drivers.UART", new Value.Obj("ti.drivers.UART", po));
        pkgV.bind("UART", vo);
        // decls 
        om.bind("ti.drivers.UART.LibType", new Proto.Enm("ti.drivers.UART.LibType"));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$UART_Config", new Proto.Obj());
        om.bind("ti.drivers.UART.UART_Config", new Proto.Str(spo, false));
        om.bind("ti.drivers.UART.UART_Mode", new Proto.Enm("ti.drivers.UART.UART_Mode"));
        om.bind("ti.drivers.UART.UART_ReturnMode", new Proto.Enm("ti.drivers.UART.UART_ReturnMode"));
        om.bind("ti.drivers.UART.UART_DataMode", new Proto.Enm("ti.drivers.UART.UART_DataMode"));
        om.bind("ti.drivers.UART.UART_Echo", new Proto.Enm("ti.drivers.UART.UART_Echo"));
        om.bind("ti.drivers.UART.UART_LEN", new Proto.Enm("ti.drivers.UART.UART_LEN"));
        om.bind("ti.drivers.UART.UART_STOP", new Proto.Enm("ti.drivers.UART.UART_STOP"));
        om.bind("ti.drivers.UART.UART_PAR", new Proto.Enm("ti.drivers.UART.UART_PAR"));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$UARTTiva_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.UART.UARTTiva_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$UARTTivaDMA_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.UART.UARTTivaDMA_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$UARTTiva_Object", new Proto.Obj());
        om.bind("ti.drivers.UART.UARTTiva_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$UARTCC3200_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.UART.UARTCC3200_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$UARTCC3200DMA_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.UART.UARTCC3200DMA_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$UARTCC3200_Object", new Proto.Obj());
        om.bind("ti.drivers.UART.UARTCC3200_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$UARTTivaDMA_Object", new Proto.Obj());
        om.bind("ti.drivers.UART.UARTTivaDMA_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$UARTCC3200DMA_Object", new Proto.Obj());
        om.bind("ti.drivers.UART.UARTCC3200DMA_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$UARTUSCIA_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.UART.UARTUSCIA_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$UARTUSCIA_Object", new Proto.Obj());
        om.bind("ti.drivers.UART.UARTUSCIA_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$UARTEUSCIA_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.UART.UARTEUSCIA_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$UARTEUSCIA_Object", new Proto.Obj());
        om.bind("ti.drivers.UART.UARTEUSCIA_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$UARTMSP432_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.UART.UARTMSP432_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$UARTMSP432_Object", new Proto.Obj());
        om.bind("ti.drivers.UART.UARTMSP432_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$BasicView", new Proto.Obj());
        om.bind("ti.drivers.UART.BasicView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$ConfigView", new Proto.Obj());
        om.bind("ti.drivers.UART.ConfigView", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.UART$$BufferView", new Proto.Obj());
        om.bind("ti.drivers.UART.BufferView", new Proto.Str(spo, false));
    }

    void USBMSCHFatFs$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.drivers.USBMSCHFatFs.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.drivers.USBMSCHFatFs", new Value.Obj("ti.drivers.USBMSCHFatFs", po));
        pkgV.bind("USBMSCHFatFs", vo);
        // decls 
        om.bind("ti.drivers.USBMSCHFatFs.LibType", new Proto.Enm("ti.drivers.USBMSCHFatFs.LibType"));
        spo = (Proto.Obj)om.bind("ti.drivers.USBMSCHFatFs$$USBMSCHFatFs_Config", new Proto.Obj());
        om.bind("ti.drivers.USBMSCHFatFs.USBMSCHFatFs_Config", new Proto.Str(spo, false));
        om.bind("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_USBState", new Proto.Enm("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_USBState"));
        spo = (Proto.Obj)om.bind("ti.drivers.USBMSCHFatFs$$USBMSCHFatFsTiva_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.USBMSCHFatFs$$USBMSCHFatFsTiva_Object", new Proto.Obj());
        om.bind("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.USBMSCHFatFs$$BasicView", new Proto.Obj());
        om.bind("ti.drivers.USBMSCHFatFs.BasicView", new Proto.Str(spo, false));
    }

    void Watchdog$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.drivers.Watchdog.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.drivers.Watchdog", new Value.Obj("ti.drivers.Watchdog", po));
        pkgV.bind("Watchdog", vo);
        // decls 
        om.bind("ti.drivers.Watchdog.LibType", new Proto.Enm("ti.drivers.Watchdog.LibType"));
        spo = (Proto.Obj)om.bind("ti.drivers.Watchdog$$WatchdogTiva_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.Watchdog.WatchdogTiva_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.Watchdog$$WatchdogTiva_Object", new Proto.Obj());
        om.bind("ti.drivers.Watchdog.WatchdogTiva_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.Watchdog$$WatchdogCC3200_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.Watchdog.WatchdogCC3200_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.Watchdog$$WatchdogCC3200_Object", new Proto.Obj());
        om.bind("ti.drivers.Watchdog.WatchdogCC3200_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.Watchdog$$WatchdogMSP430_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.Watchdog.WatchdogMSP430_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.Watchdog$$WatchdogMSP430_Object", new Proto.Obj());
        om.bind("ti.drivers.Watchdog.WatchdogMSP430_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.Watchdog$$WatchdogMSP432_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.Watchdog.WatchdogMSP432_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.Watchdog$$WatchdogMSP432_Object", new Proto.Obj());
        om.bind("ti.drivers.Watchdog.WatchdogMSP432_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.Watchdog$$Watchdog_Config", new Proto.Obj());
        om.bind("ti.drivers.Watchdog.Watchdog_Config", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.Watchdog$$BasicView", new Proto.Obj());
        om.bind("ti.drivers.Watchdog.BasicView", new Proto.Str(spo, false));
    }

    void WiFi$$OBJECTS()
    {
        Proto.Obj po, spo;
        Value.Obj vo;

        po = (Proto.Obj)om.bind("ti.drivers.WiFi.Module", new Proto.Obj());
        vo = (Value.Obj)om.bind("ti.drivers.WiFi", new Value.Obj("ti.drivers.WiFi", po));
        pkgV.bind("WiFi", vo);
        // decls 
        om.bind("ti.drivers.WiFi.DeviceType", new Proto.Enm("ti.drivers.WiFi.DeviceType"));
        om.bind("ti.drivers.WiFi.LibType", new Proto.Enm("ti.drivers.WiFi.LibType"));
        spo = (Proto.Obj)om.bind("ti.drivers.WiFi$$WiFi_Config", new Proto.Obj());
        om.bind("ti.drivers.WiFi.WiFi_Config", new Proto.Str(spo, false));
        om.bind("ti.drivers.WiFi.WiFiCC3100_SPI_State", new Proto.Enm("ti.drivers.WiFi.WiFiCC3100_SPI_State"));
        spo = (Proto.Obj)om.bind("ti.drivers.WiFi$$WiFiCC3100_HWAttrs", new Proto.Obj());
        om.bind("ti.drivers.WiFi.WiFiCC3100_HWAttrs", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.WiFi$$WiFiCC3100_Object", new Proto.Obj());
        om.bind("ti.drivers.WiFi.WiFiCC3100_Object", new Proto.Str(spo, false));
        spo = (Proto.Obj)om.bind("ti.drivers.WiFi$$BasicView", new Proto.Obj());
        om.bind("ti.drivers.WiFi.BasicView", new Proto.Str(spo, false));
    }

    void EMAC$$CONSTS()
    {
        // module EMAC
        om.bind("ti.drivers.EMAC.LibType_Instrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.EMAC.LibType", "ti.drivers"), "ti.drivers.EMAC.LibType_Instrumented", 0));
        om.bind("ti.drivers.EMAC.LibType_NonInstrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.EMAC.LibType", "ti.drivers"), "ti.drivers.EMAC.LibType_NonInstrumented", 1));
    }

    void GPIO$$CONSTS()
    {
        // module GPIO
        om.bind("ti.drivers.GPIO.LibType_Instrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.GPIO.LibType", "ti.drivers"), "ti.drivers.GPIO.LibType_Instrumented", 0));
        om.bind("ti.drivers.GPIO.LibType_NonInstrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.GPIO.LibType", "ti.drivers"), "ti.drivers.GPIO.LibType_NonInstrumented", 1));
    }

    void I2C$$CONSTS()
    {
        // module I2C
        om.bind("ti.drivers.I2C.LibType_Instrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.I2C.LibType", "ti.drivers"), "ti.drivers.I2C.LibType_Instrumented", 0));
        om.bind("ti.drivers.I2C.LibType_NonInstrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.I2C.LibType", "ti.drivers"), "ti.drivers.I2C.LibType_NonInstrumented", 1));
        om.bind("ti.drivers.I2C.I2C_MODE_BLOCKING", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.I2C.I2C_TransferMode", "ti.drivers"), "ti.drivers.I2C.I2C_MODE_BLOCKING", 0));
        om.bind("ti.drivers.I2C.I2C_MODE_CALLBACK", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.I2C.I2C_TransferMode", "ti.drivers"), "ti.drivers.I2C.I2C_MODE_CALLBACK", 1));
        om.bind("ti.drivers.I2C.IDLE_MODE", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.I2C.I2C_Mode", "ti.drivers"), "ti.drivers.I2C.IDLE_MODE", xdc.services.intern.xsr.Enum.intValue(0x00L)+0));
        om.bind("ti.drivers.I2C.TX_MODE", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.I2C.I2C_Mode", "ti.drivers"), "ti.drivers.I2C.TX_MODE", xdc.services.intern.xsr.Enum.intValue(om.findStrict("ti.drivers.I2C.IDLE_MODE", "ti.drivers"))+1));
        om.bind("ti.drivers.I2C.RX_MODE", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.I2C.I2C_Mode", "ti.drivers"), "ti.drivers.I2C.RX_MODE", xdc.services.intern.xsr.Enum.intValue(om.findStrict("ti.drivers.I2C.IDLE_MODE", "ti.drivers"))+2));
        om.bind("ti.drivers.I2C.ERROR", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.I2C.I2C_Mode", "ti.drivers"), "ti.drivers.I2C.ERROR", xdc.services.intern.xsr.Enum.intValue(0xFFL)+0));
    }

    void Power$$CONSTS()
    {
        // module Power
    }

    void PWM$$CONSTS()
    {
        // module PWM
        om.bind("ti.drivers.PWM.LibType_Instrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.PWM.LibType", "ti.drivers"), "ti.drivers.PWM.LibType_Instrumented", 0));
        om.bind("ti.drivers.PWM.LibType_NonInstrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.PWM.LibType", "ti.drivers"), "ti.drivers.PWM.LibType_NonInstrumented", 1));
        om.bind("ti.drivers.PWM.PWM_DUTY_COUNTS", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.PWM.PWM_DutyMode", "ti.drivers"), "ti.drivers.PWM.PWM_DUTY_COUNTS", 0));
        om.bind("ti.drivers.PWM.PWM_DUTY_SCALAR", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.PWM.PWM_DutyMode", "ti.drivers"), "ti.drivers.PWM.PWM_DUTY_SCALAR", 1));
        om.bind("ti.drivers.PWM.PWM_DUTY_TIME", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.PWM.PWM_DutyMode", "ti.drivers"), "ti.drivers.PWM.PWM_DUTY_TIME", 2));
        om.bind("ti.drivers.PWM.PWM_POL_ACTIVE_HIGH", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.PWM.PWM_Polarity", "ti.drivers"), "ti.drivers.PWM.PWM_POL_ACTIVE_HIGH", 0));
        om.bind("ti.drivers.PWM.PWM_POL_ACTIVE_LOW", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.PWM.PWM_Polarity", "ti.drivers"), "ti.drivers.PWM.PWM_POL_ACTIVE_LOW", 1));
    }

    void SDSPI$$CONSTS()
    {
        // module SDSPI
        om.bind("ti.drivers.SDSPI.LibType_Instrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.SDSPI.LibType", "ti.drivers"), "ti.drivers.SDSPI.LibType_Instrumented", 0));
        om.bind("ti.drivers.SDSPI.LibType_NonInstrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.SDSPI.LibType", "ti.drivers"), "ti.drivers.SDSPI.LibType_NonInstrumented", 1));
        om.bind("ti.drivers.SDSPI.NOCARD", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.SDSPI.SDSPITiva_CardType", "ti.drivers"), "ti.drivers.SDSPI.NOCARD", xdc.services.intern.xsr.Enum.intValue(0x0L)+0));
        om.bind("ti.drivers.SDSPI.MMC", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.SDSPI.SDSPITiva_CardType", "ti.drivers"), "ti.drivers.SDSPI.MMC", xdc.services.intern.xsr.Enum.intValue(1L)+0));
        om.bind("ti.drivers.SDSPI.SDSC", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.SDSPI.SDSPITiva_CardType", "ti.drivers"), "ti.drivers.SDSPI.SDSC", xdc.services.intern.xsr.Enum.intValue(2L)+0));
        om.bind("ti.drivers.SDSPI.SDHC", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.SDSPI.SDSPITiva_CardType", "ti.drivers"), "ti.drivers.SDSPI.SDHC", xdc.services.intern.xsr.Enum.intValue(3L)+0));
    }

    void SPI$$CONSTS()
    {
        // module SPI
        om.bind("ti.drivers.SPI.LibType_Instrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.SPI.LibType", "ti.drivers"), "ti.drivers.SPI.LibType_Instrumented", 0));
        om.bind("ti.drivers.SPI.LibType_NonInstrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.SPI.LibType", "ti.drivers"), "ti.drivers.SPI.LibType_NonInstrumented", 1));
        om.bind("ti.drivers.SPI.SPI_MODE_BLOCKING", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.SPI.TransferMode", "ti.drivers"), "ti.drivers.SPI.SPI_MODE_BLOCKING", 0));
        om.bind("ti.drivers.SPI.SPI_MODE_CALLBACK", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.SPI.TransferMode", "ti.drivers"), "ti.drivers.SPI.SPI_MODE_CALLBACK", 1));
        om.bind("ti.drivers.SPI.SPITivaDMA_8bit", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.SPI.SPITivaDMA_FrameSize", "ti.drivers"), "ti.drivers.SPI.SPITivaDMA_8bit", xdc.services.intern.xsr.Enum.intValue(0L)+0));
        om.bind("ti.drivers.SPI.SPITivaDMA_16bit", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.SPI.SPITivaDMA_FrameSize", "ti.drivers"), "ti.drivers.SPI.SPITivaDMA_16bit", xdc.services.intern.xsr.Enum.intValue(1L)+0));
        om.bind("ti.drivers.SPI.SPICC3200DMA_8bit", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.SPI.SPICC3200DMA_FrameSize", "ti.drivers"), "ti.drivers.SPI.SPICC3200DMA_8bit", xdc.services.intern.xsr.Enum.intValue(0L)+0));
        om.bind("ti.drivers.SPI.SPICC3200DMA_16bit", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.SPI.SPICC3200DMA_FrameSize", "ti.drivers"), "ti.drivers.SPI.SPICC3200DMA_16bit", xdc.services.intern.xsr.Enum.intValue(1L)+0));
        om.bind("ti.drivers.SPI.SPICC3200DMA_32bit", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.SPI.SPICC3200DMA_FrameSize", "ti.drivers"), "ti.drivers.SPI.SPICC3200DMA_32bit", xdc.services.intern.xsr.Enum.intValue(2L)+0));
    }

    void UART$$CONSTS()
    {
        // module UART
        om.bind("ti.drivers.UART.LibType_Instrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.LibType", "ti.drivers"), "ti.drivers.UART.LibType_Instrumented", 0));
        om.bind("ti.drivers.UART.LibType_NonInstrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.LibType", "ti.drivers"), "ti.drivers.UART.LibType_NonInstrumented", 1));
        om.bind("ti.drivers.UART.UART_MODE_BLOCKING", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), "ti.drivers.UART.UART_MODE_BLOCKING", 0));
        om.bind("ti.drivers.UART.UART_MODE_CALLBACK", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), "ti.drivers.UART.UART_MODE_CALLBACK", 1));
        om.bind("ti.drivers.UART.UART_MODE_ISSUERECLAIM", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), "ti.drivers.UART.UART_MODE_ISSUERECLAIM", 2));
        om.bind("ti.drivers.UART.UART_RETURN_FULL", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_ReturnMode", "ti.drivers"), "ti.drivers.UART.UART_RETURN_FULL", 0));
        om.bind("ti.drivers.UART.UART_RETURN_NEWLINE", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_ReturnMode", "ti.drivers"), "ti.drivers.UART.UART_RETURN_NEWLINE", 1));
        om.bind("ti.drivers.UART.UART_DATA_BINARY", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), "ti.drivers.UART.UART_DATA_BINARY", 0));
        om.bind("ti.drivers.UART.UART_DATA_TEXT", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), "ti.drivers.UART.UART_DATA_TEXT", 1));
        om.bind("ti.drivers.UART.UART_ECHO_OFF", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_Echo", "ti.drivers"), "ti.drivers.UART.UART_ECHO_OFF", xdc.services.intern.xsr.Enum.intValue(0L)+0));
        om.bind("ti.drivers.UART.UART_ECHO_ON", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_Echo", "ti.drivers"), "ti.drivers.UART.UART_ECHO_ON", xdc.services.intern.xsr.Enum.intValue(1L)+0));
        om.bind("ti.drivers.UART.UART_LEN_5", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_LEN", "ti.drivers"), "ti.drivers.UART.UART_LEN_5", 0));
        om.bind("ti.drivers.UART.UART_LEN_6", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_LEN", "ti.drivers"), "ti.drivers.UART.UART_LEN_6", 1));
        om.bind("ti.drivers.UART.UART_LEN_7", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_LEN", "ti.drivers"), "ti.drivers.UART.UART_LEN_7", 2));
        om.bind("ti.drivers.UART.UART_LEN_8", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_LEN", "ti.drivers"), "ti.drivers.UART.UART_LEN_8", 3));
        om.bind("ti.drivers.UART.UART_STOP_ONE", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_STOP", "ti.drivers"), "ti.drivers.UART.UART_STOP_ONE", 0));
        om.bind("ti.drivers.UART.UART_STOP_TWO", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_STOP", "ti.drivers"), "ti.drivers.UART.UART_STOP_TWO", 1));
        om.bind("ti.drivers.UART.UART_PAR_NONE", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_PAR", "ti.drivers"), "ti.drivers.UART.UART_PAR_NONE", 0));
        om.bind("ti.drivers.UART.UART_PAR_EVEN", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_PAR", "ti.drivers"), "ti.drivers.UART.UART_PAR_EVEN", 1));
        om.bind("ti.drivers.UART.UART_PAR_ODD", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_PAR", "ti.drivers"), "ti.drivers.UART.UART_PAR_ODD", 2));
        om.bind("ti.drivers.UART.UART_PAR_ZERO", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_PAR", "ti.drivers"), "ti.drivers.UART.UART_PAR_ZERO", 3));
        om.bind("ti.drivers.UART.UART_PAR_ONE", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.UART.UART_PAR", "ti.drivers"), "ti.drivers.UART.UART_PAR_ONE", 4));
    }

    void USBMSCHFatFs$$CONSTS()
    {
        // module USBMSCHFatFs
        om.bind("ti.drivers.USBMSCHFatFs.LibType_Instrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.USBMSCHFatFs.LibType", "ti.drivers"), "ti.drivers.USBMSCHFatFs.LibType_Instrumented", 0));
        om.bind("ti.drivers.USBMSCHFatFs.LibType_NonInstrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.USBMSCHFatFs.LibType", "ti.drivers"), "ti.drivers.USBMSCHFatFs.LibType_NonInstrumented", 1));
        om.bind("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_NO_DEVICE", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_USBState", "ti.drivers"), "ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_NO_DEVICE", 0));
        om.bind("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_CONNECTED", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_USBState", "ti.drivers"), "ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_CONNECTED", 1));
        om.bind("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_UNKNOWN", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_USBState", "ti.drivers"), "ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_UNKNOWN", 2));
        om.bind("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_POWER_FAULT", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_USBState", "ti.drivers"), "ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_POWER_FAULT", 3));
    }

    void Watchdog$$CONSTS()
    {
        // module Watchdog
        om.bind("ti.drivers.Watchdog.LibType_Instrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.Watchdog.LibType", "ti.drivers"), "ti.drivers.Watchdog.LibType_Instrumented", 0));
        om.bind("ti.drivers.Watchdog.LibType_NonInstrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.Watchdog.LibType", "ti.drivers"), "ti.drivers.Watchdog.LibType_NonInstrumented", 1));
    }

    void WiFi$$CONSTS()
    {
        // module WiFi
        om.bind("ti.drivers.WiFi.DeviceType_SimpleLink_CC3100", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.WiFi.DeviceType", "ti.drivers"), "ti.drivers.WiFi.DeviceType_SimpleLink_CC3100", 0));
        om.bind("ti.drivers.WiFi.LibType_Instrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.WiFi.LibType", "ti.drivers"), "ti.drivers.WiFi.LibType_Instrumented", 0));
        om.bind("ti.drivers.WiFi.LibType_NonInstrumented", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.WiFi.LibType", "ti.drivers"), "ti.drivers.WiFi.LibType_NonInstrumented", 1));
        om.bind("ti.drivers.WiFi.WiFiCC3100_SPI_UNINITIALIZED", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.WiFi.WiFiCC3100_SPI_State", "ti.drivers"), "ti.drivers.WiFi.WiFiCC3100_SPI_UNINITIALIZED", 0));
        om.bind("ti.drivers.WiFi.WiFiCC3100_SPI_IDLE", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.WiFi.WiFiCC3100_SPI_State", "ti.drivers"), "ti.drivers.WiFi.WiFiCC3100_SPI_IDLE", 1));
        om.bind("ti.drivers.WiFi.WiFiCC3100_SPI_READ_EOT", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.WiFi.WiFiCC3100_SPI_State", "ti.drivers"), "ti.drivers.WiFi.WiFiCC3100_SPI_READ_EOT", 2));
        om.bind("ti.drivers.WiFi.WiFiCC3100_SPI_READ_MSG", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.WiFi.WiFiCC3100_SPI_State", "ti.drivers"), "ti.drivers.WiFi.WiFiCC3100_SPI_READ_MSG", 3));
        om.bind("ti.drivers.WiFi.WiFiCC3100_SPI_WRITE_EOT", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.WiFi.WiFiCC3100_SPI_State", "ti.drivers"), "ti.drivers.WiFi.WiFiCC3100_SPI_WRITE_EOT", 4));
        om.bind("ti.drivers.WiFi.WiFiCC3100_SPI_WRITE_MSG", xdc.services.intern.xsr.Enum.make((Proto.Enm)om.findStrict("ti.drivers.WiFi.WiFiCC3100_SPI_State", "ti.drivers"), "ti.drivers.WiFi.WiFiCC3100_SPI_WRITE_MSG", 5));
    }

    void EMAC$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void GPIO$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void I2C$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Power$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void PWM$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void SDSPI$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void SPI$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void UART$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void USBMSCHFatFs$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void Watchdog$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void WiFi$$CREATES()
    {
        Proto.Fxn fxn;
        StringBuilder sb;

    }

    void EMAC$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void GPIO$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void I2C$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Power$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void PWM$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void SDSPI$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void SPI$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void UART$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void USBMSCHFatFs$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void Watchdog$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void WiFi$$FUNCTIONS()
    {
        Proto.Fxn fxn;

    }

    void EMAC$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.drivers.EMAC.EMAC_Config", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("fxnTablePtr", "UPtr"));
        sizes.add(Global.newArray("object", "UPtr"));
        sizes.add(Global.newArray("hwAttrs", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.EMAC.EMAC_Config']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.EMAC.EMAC_Config']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.EMAC.EMAC_Config'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.EMAC.EMACTiva_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("swi", "UPtr"));
        sizes.add(Global.newArray("hwi", "UPtr"));
        sizes.add(Global.newArray("rxCount", "UInt"));
        sizes.add(Global.newArray("rxDropped", "UInt"));
        sizes.add(Global.newArray("txSent", "UInt"));
        sizes.add(Global.newArray("txDropped", "UInt"));
        sizes.add(Global.newArray("linkUp", "UInt"));
        sizes.add(Global.newArray("pTxDescList", "UPtr"));
        sizes.add(Global.newArray("pRxDescList", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.EMAC.EMACTiva_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.EMAC.EMACTiva_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.EMAC.EMACTiva_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.EMAC.EMACTiva_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("intNum", "UInt"));
        sizes.add(Global.newArray("macAddress", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.EMAC.EMACTiva_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.EMAC.EMACTiva_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.EMAC.EMACTiva_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.EMAC.EMACSnow_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt"));
        sizes.add(Global.newArray("intNum", "UInt"));
        sizes.add(Global.newArray("macAddress", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.EMAC.EMACSnow_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.EMAC.EMACSnow_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.EMAC.EMACSnow_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.EMAC.EMACSnow_Data", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("hEvent", "UPtr"));
        sizes.add(Global.newArray("PBM_tx", "A12;UInt8"));
        sizes.add(Global.newArray("PBM_rx", "A12;UInt8"));
        sizes.add(Global.newArray("rxCount", "UInt"));
        sizes.add(Global.newArray("rxDropped", "UInt"));
        sizes.add(Global.newArray("txSent", "UInt"));
        sizes.add(Global.newArray("txDropped", "UInt"));
        sizes.add(Global.newArray("abnormalInts", "UInt"));
        sizes.add(Global.newArray("isrCount", "UInt"));
        sizes.add(Global.newArray("linkUp", "UInt"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.EMAC.EMACSnow_Data']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.EMAC.EMACSnow_Data']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.EMAC.EMACSnow_Data'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.EMAC.EMACSnow_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("swi", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.EMAC.EMACSnow_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.EMAC.EMACSnow_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.EMAC.EMACSnow_Object'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void GPIO$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.drivers.GPIO.GPIO_Config", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("pinConfigs", "UPtr"));
        sizes.add(Global.newArray("callbacks", "UPtr"));
        sizes.add(Global.newArray("numberOfPinConfigs", "UInt32"));
        sizes.add(Global.newArray("numberOfCallbacks", "UInt32"));
        sizes.add(Global.newArray("intPriority", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.GPIO.GPIO_Config']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.GPIO.GPIO_Config']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.GPIO.GPIO_Config'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void I2C$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.drivers.I2C.I2C_Config", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("fxnTablePtr", "UPtr"));
        sizes.add(Global.newArray("object", "UPtr"));
        sizes.add(Global.newArray("hwAttrs", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.I2C.I2C_Config']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.I2C.I2C_Config']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.I2C.I2C_Config'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.I2C.I2C_Transaction", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("txBuff", "UPtr"));
        sizes.add(Global.newArray("txCount", "USize"));
        sizes.add(Global.newArray("rxBuff", "UPtr"));
        sizes.add(Global.newArray("rxCount", "USize"));
        sizes.add(Global.newArray("slaveAddress", "UInt8"));
        sizes.add(Global.newArray("arg", "UPtr"));
        sizes.add(Global.newArray("nextPtr", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.I2C.I2C_Transaction']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.I2C.I2C_Transaction']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.I2C.I2C_Transaction'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.I2C.I2C_Params", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("transferMode", "Nti.drivers.I2C.I2C_TransferMode;;;"));
        sizes.add(Global.newArray("transferCallback", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.I2C.I2C_Params']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.I2C.I2C_Params']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.I2C.I2C_Params'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.I2C.I2CTiva_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt"));
        sizes.add(Global.newArray("intNum", "UInt"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.I2C.I2CTiva_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.I2C.I2CTiva_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.I2C.I2CTiva_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.I2C.I2CTiva_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("mutex", "UPtr"));
        sizes.add(Global.newArray("transferComplete", "UPtr"));
        sizes.add(Global.newArray("i2cParams", "Sti.drivers.I2C;I2C_Params"));
        sizes.add(Global.newArray("hwi", "UPtr"));
        sizes.add(Global.newArray("mode", "Nti.drivers.I2C.I2C_Mode;;0x00;;;0xFF"));
        sizes.add(Global.newArray("currentTransaction", "UPtr"));
        sizes.add(Global.newArray("writeBufIdx", "UPtr"));
        sizes.add(Global.newArray("writeCountIdx", "USize"));
        sizes.add(Global.newArray("readBufIdx", "UPtr"));
        sizes.add(Global.newArray("readCountIdx", "USize"));
        sizes.add(Global.newArray("headPtr", "UPtr"));
        sizes.add(Global.newArray("tailPtr", "UPtr"));
        sizes.add(Global.newArray("isOpen", "UShort"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.I2C.I2CTiva_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.I2C.I2CTiva_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.I2C.I2CTiva_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.I2C.I2CCC3200_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt"));
        sizes.add(Global.newArray("intNum", "UInt"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.I2C.I2CCC3200_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.I2C.I2CCC3200_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.I2C.I2CCC3200_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.I2C.I2CCC3200_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("mutex", "UPtr"));
        sizes.add(Global.newArray("transferComplete", "UPtr"));
        sizes.add(Global.newArray("i2cParams", "Sti.drivers.I2C;I2C_Params"));
        sizes.add(Global.newArray("hwi", "UPtr"));
        sizes.add(Global.newArray("mode", "Nti.drivers.I2C.I2C_Mode;;0x00;;;0xFF"));
        sizes.add(Global.newArray("currentTransaction", "UPtr"));
        sizes.add(Global.newArray("writeBufIdx", "UPtr"));
        sizes.add(Global.newArray("writeCountIdx", "USize"));
        sizes.add(Global.newArray("readBufIdx", "UPtr"));
        sizes.add(Global.newArray("readCountIdx", "USize"));
        sizes.add(Global.newArray("headPtr", "UPtr"));
        sizes.add(Global.newArray("tailPtr", "UPtr"));
        sizes.add(Global.newArray("isOpen", "UShort"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.I2C.I2CCC3200_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.I2C.I2CCC3200_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.I2C.I2CCC3200_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.I2C.I2CMSP432_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("intNum", "UInt32"));
        sizes.add(Global.newArray("clockSource", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.I2C.I2CMSP432_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.I2C.I2CMSP432_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.I2C.I2CMSP432_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.I2C.I2CMSP432_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("mutex", "UPtr"));
        sizes.add(Global.newArray("transferComplete", "UPtr"));
        sizes.add(Global.newArray("transferMode", "Nti.drivers.I2C.I2C_TransferMode;;;"));
        sizes.add(Global.newArray("transferCallbackFxn", "UPtr"));
        sizes.add(Global.newArray("mode", "Nti.drivers.I2C.I2C_Mode;;0x00;;;0xFF"));
        sizes.add(Global.newArray("currentTransaction", "UPtr"));
        sizes.add(Global.newArray("writeBufIdx", "UPtr"));
        sizes.add(Global.newArray("writeCountIdx", "UInt"));
        sizes.add(Global.newArray("readBufIdx", "UPtr"));
        sizes.add(Global.newArray("readCountIdx", "UInt"));
        sizes.add(Global.newArray("headPtr", "UPtr"));
        sizes.add(Global.newArray("tailPtr", "UPtr"));
        sizes.add(Global.newArray("isOpen", "UShort"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.I2C.I2CMSP432_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.I2C.I2CMSP432_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.I2C.I2CMSP432_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.I2C.I2CUSCIB_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("clockSource", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.I2C.I2CUSCIB_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.I2C.I2CUSCIB_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.I2C.I2CUSCIB_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.I2C.I2CEUSCIB_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("clockSource", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.I2C.I2CEUSCIB_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.I2C.I2CEUSCIB_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.I2C.I2CEUSCIB_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.I2C.I2CUSCIB_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("isOpen", "UShort"));
        sizes.add(Global.newArray("mutex", "UPtr"));
        sizes.add(Global.newArray("transferComplete", "UPtr"));
        sizes.add(Global.newArray("i2cParams", "Sti.drivers.I2C;I2C_Params"));
        sizes.add(Global.newArray("mode", "Nti.drivers.I2C.I2C_Mode;;0x00;;;0xFF"));
        sizes.add(Global.newArray("currentTransaction", "UPtr"));
        sizes.add(Global.newArray("writeBufIdx", "UPtr"));
        sizes.add(Global.newArray("writeCountIdx", "UInt"));
        sizes.add(Global.newArray("readBufIdx", "UPtr"));
        sizes.add(Global.newArray("readCountIdx", "UInt"));
        sizes.add(Global.newArray("headPtr", "UPtr"));
        sizes.add(Global.newArray("tailPtr", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.I2C.I2CUSCIB_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.I2C.I2CUSCIB_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.I2C.I2CUSCIB_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.I2C.I2CEUSCIB_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("isOpen", "UShort"));
        sizes.add(Global.newArray("mutex", "UPtr"));
        sizes.add(Global.newArray("transferComplete", "UPtr"));
        sizes.add(Global.newArray("i2cParams", "Sti.drivers.I2C;I2C_Params"));
        sizes.add(Global.newArray("mode", "Nti.drivers.I2C.I2C_Mode;;0x00;;;0xFF"));
        sizes.add(Global.newArray("currentTransaction", "UPtr"));
        sizes.add(Global.newArray("writeBufIdx", "UPtr"));
        sizes.add(Global.newArray("writeCountIdx", "USize"));
        sizes.add(Global.newArray("readBufIdx", "UPtr"));
        sizes.add(Global.newArray("readCountIdx", "USize"));
        sizes.add(Global.newArray("headPtr", "UPtr"));
        sizes.add(Global.newArray("tailPtr", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.I2C.I2CEUSCIB_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.I2C.I2CEUSCIB_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.I2C.I2CEUSCIB_Object'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void Power$$SIZES()
    {
        Proto.Str so;
        Object fxn;

    }

    void PWM$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.drivers.PWM.PWM_Config", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("fxnTablePtr", "UPtr"));
        sizes.add(Global.newArray("object", "UPtr"));
        sizes.add(Global.newArray("hwAttrs", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.PWM.PWM_Config']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.PWM.PWM_Config']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.PWM.PWM_Config'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.PWM.PWM_Params", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("period", "UInt32"));
        sizes.add(Global.newArray("dutyMode", "Nti.drivers.PWM.PWM_DutyMode;;;;"));
        sizes.add(Global.newArray("polarity", "Nti.drivers.PWM.PWM_Polarity;;;"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.PWM.PWM_Params']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.PWM.PWM_Params']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.PWM.PWM_Params'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.PWM.PWMTimerCC3200_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("timer", "UInt16"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.PWM.PWMTimerCC3200_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.PWM.PWMTimerCC3200_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.PWM.PWMTimerCC3200_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.PWM.PWMTimerCC3200_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("period", "UInt32"));
        sizes.add(Global.newArray("duty", "UInt32"));
        sizes.add(Global.newArray("dutyMode", "UInt8"));
        sizes.add(Global.newArray("cyclesPerMicroSec", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.PWM.PWMTimerCC3200_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.PWM.PWMTimerCC3200_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.PWM.PWMTimerCC3200_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.PWM.PWMTiva_Status", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("genPeriods", "A4;UInt16"));
        sizes.add(Global.newArray("cyclesPerMicroSec", "UInt8"));
        sizes.add(Global.newArray("prescalar", "UInt8"));
        sizes.add(Global.newArray("activeOutputs", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.PWM.PWMTiva_Status']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.PWM.PWMTiva_Status']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.PWM.PWMTiva_Status'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.PWM.PWMTiva_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("pwmOutput", "UInt32"));
        sizes.add(Global.newArray("pwmGenOpts", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.PWM.PWMTiva_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.PWM.PWMTiva_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.PWM.PWMTiva_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.PWM.PWMTiva_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("pwmStatus", "UPtr"));
        sizes.add(Global.newArray("pwmDuty", "UInt16"));
        sizes.add(Global.newArray("pwmOutputBit", "UInt8"));
        sizes.add(Global.newArray("dutyMode", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.PWM.PWMTiva_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.PWM.PWMTiva_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.PWM.PWMTiva_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.PWM.PWMTimerTiva_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("timer", "UInt16"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.PWM.PWMTimerTiva_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.PWM.PWMTimerTiva_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.PWM.PWMTimerTiva_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.PWM.PWMTimerTiva_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("period", "UInt32"));
        sizes.add(Global.newArray("duty", "UInt32"));
        sizes.add(Global.newArray("dutyMode", "UInt8"));
        sizes.add(Global.newArray("cyclesPerMicroSec", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.PWM.PWMTimerTiva_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.PWM.PWMTimerTiva_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.PWM.PWMTimerTiva_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.PWM.PWMTimerMSP432_Status", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("period", "UInt16"));
        sizes.add(Global.newArray("prescalar", "UInt8"));
        sizes.add(Global.newArray("cyclesPerMicroSec", "UInt8"));
        sizes.add(Global.newArray("activeOutputs", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.PWM.PWMTimerMSP432_Status']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.PWM.PWMTimerMSP432_Status']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.PWM.PWMTimerMSP432_Status'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.PWM.PWMTimerMSP432_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("compareRegister", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.PWM.PWMTimerMSP432_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.PWM.PWMTimerMSP432_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.PWM.PWMTimerMSP432_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.PWM.PWMTimerMSP432_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("pwmStatus", "UPtr"));
        sizes.add(Global.newArray("pwmDuty", "UInt16"));
        sizes.add(Global.newArray("pwmCompareRegisterBit", "UInt8"));
        sizes.add(Global.newArray("dutyMode", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.PWM.PWMTimerMSP432_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.PWM.PWMTimerMSP432_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.PWM.PWMTimerMSP432_Object'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void SDSPI$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.drivers.SDSPI.SDSPI_Config", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("fxnTablePtr", "UPtr"));
        sizes.add(Global.newArray("object", "UPtr"));
        sizes.add(Global.newArray("hwAttrs", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SDSPI.SDSPI_Config']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SDSPI.SDSPI_Config']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SDSPI.SDSPI_Config'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SDSPI.SDSPITiva_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("portSPI", "UInt32"));
        sizes.add(Global.newArray("pinSCK", "UInt32"));
        sizes.add(Global.newArray("pinMISO", "UInt32"));
        sizes.add(Global.newArray("pinMOSI", "UInt32"));
        sizes.add(Global.newArray("portCS", "UInt32"));
        sizes.add(Global.newArray("pinCS", "UInt32"));
        sizes.add(Global.newArray("portTX", "UInt32"));
        sizes.add(Global.newArray("pinTX", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SDSPI.SDSPITiva_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SDSPI.SDSPITiva_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SDSPI.SDSPITiva_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SDSPI.SDSPITiva_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("driveNumber", "UInt32"));
        sizes.add(Global.newArray("diskState", "UChar"));
        sizes.add(Global.newArray("cardType", "Nti.drivers.SDSPI.SDSPITiva_CardType;;0x0;1;2;3"));
        sizes.add(Global.newArray("bitRate", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SDSPI.SDSPITiva_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SDSPI.SDSPITiva_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SDSPI.SDSPITiva_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SDSPI.SDSPIUSCIA_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("clockSource", "UInt8"));
        sizes.add(Global.newArray("portSPI", "UInt8"));
        sizes.add(Global.newArray("pinSCK", "UInt32"));
        sizes.add(Global.newArray("pinMISO", "UInt32"));
        sizes.add(Global.newArray("pinMOSI", "UInt32"));
        sizes.add(Global.newArray("portCS", "UInt8"));
        sizes.add(Global.newArray("pinCS", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SDSPI.SDSPIUSCIA_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SDSPI.SDSPIUSCIA_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SDSPI.SDSPIUSCIA_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SDSPI.SDSPIUSCIB_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("clockSource", "UInt8"));
        sizes.add(Global.newArray("portSPI", "UInt8"));
        sizes.add(Global.newArray("pinSCK", "UInt32"));
        sizes.add(Global.newArray("pinMISO", "UInt32"));
        sizes.add(Global.newArray("pinMOSI", "UInt32"));
        sizes.add(Global.newArray("portCS", "UInt8"));
        sizes.add(Global.newArray("pinCS", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SDSPI.SDSPIUSCIB_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SDSPI.SDSPIUSCIB_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SDSPI.SDSPIUSCIB_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SDSPI.SDSPIUSCIA_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("driveNumber", "UInt16"));
        sizes.add(Global.newArray("diskState", "UChar"));
        sizes.add(Global.newArray("cardType", "Nti.drivers.SDSPI.SDSPITiva_CardType;;0x0;1;2;3"));
        sizes.add(Global.newArray("bitRate", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SDSPI.SDSPIUSCIA_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SDSPI.SDSPIUSCIA_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SDSPI.SDSPIUSCIA_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SDSPI.SDSPIUSCIB_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("driveNumber", "UInt16"));
        sizes.add(Global.newArray("diskState", "UChar"));
        sizes.add(Global.newArray("cardType", "Nti.drivers.SDSPI.SDSPITiva_CardType;;0x0;1;2;3"));
        sizes.add(Global.newArray("bitRate", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SDSPI.SDSPIUSCIB_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SDSPI.SDSPIUSCIB_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SDSPI.SDSPIUSCIB_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SDSPI.SDSPIEUSCIA_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("clockSource", "UInt8"));
        sizes.add(Global.newArray("portSCK", "UInt8"));
        sizes.add(Global.newArray("pinSCK", "UInt32"));
        sizes.add(Global.newArray("portMISO", "UInt8"));
        sizes.add(Global.newArray("pinMISO", "UInt32"));
        sizes.add(Global.newArray("portMOSI", "UInt8"));
        sizes.add(Global.newArray("pinMOSI", "UInt32"));
        sizes.add(Global.newArray("portCS", "UInt8"));
        sizes.add(Global.newArray("pinCS", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SDSPI.SDSPIEUSCIA_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SDSPI.SDSPIEUSCIA_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SDSPI.SDSPIEUSCIA_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SDSPI.SDSPIEUSCIB_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("clockSource", "UInt8"));
        sizes.add(Global.newArray("portSCK", "UInt8"));
        sizes.add(Global.newArray("pinSCK", "UInt32"));
        sizes.add(Global.newArray("portMISO", "UInt8"));
        sizes.add(Global.newArray("pinMISO", "UInt32"));
        sizes.add(Global.newArray("portMOSI", "UInt8"));
        sizes.add(Global.newArray("pinMOSI", "UInt32"));
        sizes.add(Global.newArray("portCS", "UInt8"));
        sizes.add(Global.newArray("pinCS", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SDSPI.SDSPIEUSCIB_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SDSPI.SDSPIEUSCIB_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SDSPI.SDSPIEUSCIB_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SDSPI.SDSPIEUSCIA_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("driveNumber", "UInt16"));
        sizes.add(Global.newArray("diskState", "UChar"));
        sizes.add(Global.newArray("cardType", "Nti.drivers.SDSPI.SDSPITiva_CardType;;0x0;1;2;3"));
        sizes.add(Global.newArray("bitRate", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SDSPI.SDSPIEUSCIA_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SDSPI.SDSPIEUSCIA_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SDSPI.SDSPIEUSCIA_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SDSPI.SDSPIEUSCIB_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("driveNumber", "UInt16"));
        sizes.add(Global.newArray("diskState", "UChar"));
        sizes.add(Global.newArray("cardType", "Nti.drivers.SDSPI.SDSPITiva_CardType;;0x0;1;2;3"));
        sizes.add(Global.newArray("bitRate", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SDSPI.SDSPIEUSCIB_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SDSPI.SDSPIEUSCIB_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SDSPI.SDSPIEUSCIB_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SDSPI.SDSPICC3200_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("spiPRCM", "UInt32"));
        sizes.add(Global.newArray("csGPIOBase", "UInt32"));
        sizes.add(Global.newArray("csGPIOPin", "UInt32"));
        sizes.add(Global.newArray("txGPIOBase", "UInt32"));
        sizes.add(Global.newArray("txGPIOPin", "UInt32"));
        sizes.add(Global.newArray("txGPIOMode", "UInt32"));
        sizes.add(Global.newArray("txSPIMode", "UInt32"));
        sizes.add(Global.newArray("txPackPin", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SDSPI.SDSPICC3200_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SDSPI.SDSPICC3200_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SDSPI.SDSPICC3200_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SDSPI.SDSPICC3200_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("driveNumber", "UInt32"));
        sizes.add(Global.newArray("diskState", "UChar"));
        sizes.add(Global.newArray("cardType", "Nti.drivers.SDSPI.SDSPITiva_CardType;;0x0;1;2;3"));
        sizes.add(Global.newArray("bitRate", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SDSPI.SDSPICC3200_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SDSPI.SDSPICC3200_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SDSPI.SDSPICC3200_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SDSPI.SDSPIMSP432_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("clockSource", "UInt8"));
        sizes.add(Global.newArray("portSCK", "UInt8"));
        sizes.add(Global.newArray("pinSCK", "UInt16"));
        sizes.add(Global.newArray("sckMode", "UInt8"));
        sizes.add(Global.newArray("portMISO", "UInt8"));
        sizes.add(Global.newArray("pinMISO", "UInt32"));
        sizes.add(Global.newArray("misoMode", "UInt8"));
        sizes.add(Global.newArray("portMOSI", "UInt8"));
        sizes.add(Global.newArray("pinMOSI", "UInt16"));
        sizes.add(Global.newArray("mosiMode", "UInt8"));
        sizes.add(Global.newArray("portCS", "UInt8"));
        sizes.add(Global.newArray("pinCS", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SDSPI.SDSPIMSP432_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SDSPI.SDSPIMSP432_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SDSPI.SDSPIMSP432_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SDSPI.SDSPIMSP432_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("driveNumber", "UInt16"));
        sizes.add(Global.newArray("diskState", "UChar"));
        sizes.add(Global.newArray("cardType", "Nti.drivers.SDSPI.SDSPITiva_CardType;;0x0;1;2;3"));
        sizes.add(Global.newArray("bitRate", "UInt32"));
        sizes.add(Global.newArray("filesystem", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SDSPI.SDSPIMSP432_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SDSPI.SDSPIMSP432_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SDSPI.SDSPIMSP432_Object'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void SPI$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPI_Config", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("fxnTablePtr", "UPtr"));
        sizes.add(Global.newArray("object", "UPtr"));
        sizes.add(Global.newArray("hwAttrs", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPI_Config']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPI_Config']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPI_Config'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPI_Transaction", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("count", "UInt"));
        sizes.add(Global.newArray("txBuf", "UPtr"));
        sizes.add(Global.newArray("rxBuf", "UPtr"));
        sizes.add(Global.newArray("arg", "UIArg"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPI_Transaction']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPI_Transaction']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPI_Transaction'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPITivaDMA_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("intNum", "UInt32"));
        sizes.add(Global.newArray("rxChannelIndex", "UInt32"));
        sizes.add(Global.newArray("txChannelIndex", "UInt32"));
        sizes.add(Global.newArray("channelMappingFxn", "UFxn"));
        sizes.add(Global.newArray("rxChannelMappingFxnArg", "UInt32"));
        sizes.add(Global.newArray("txChannelMappingFxnArg", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPITivaDMA_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPITivaDMA_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPITivaDMA_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPITivaDMA_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("transferComplete", "UPtr"));
        sizes.add(Global.newArray("hwi", "UPtr"));
        sizes.add(Global.newArray("transferMode", "Nti.drivers.SPI.TransferMode;;;"));
        sizes.add(Global.newArray("transferCallbackFxn", "UFxn"));
        sizes.add(Global.newArray("transaction", "UPtr"));
        sizes.add(Global.newArray("frameSize", "Nti.drivers.SPI.SPITivaDMA_FrameSize;;0;1"));
        sizes.add(Global.newArray("isOpen", "UShort"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPITivaDMA_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPITivaDMA_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPITivaDMA_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPICC3200DMA_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("intNum", "UInt32"));
        sizes.add(Global.newArray("spiPRCM", "UInt32"));
        sizes.add(Global.newArray("csControl", "UInt32"));
        sizes.add(Global.newArray("csPolarity", "UInt32"));
        sizes.add(Global.newArray("pinMode", "UInt32"));
        sizes.add(Global.newArray("turboMode", "UInt32"));
        sizes.add(Global.newArray("scratchBufPtr", "UPtr"));
        sizes.add(Global.newArray("defaultTxBufValue", "UInt32"));
        sizes.add(Global.newArray("rxChannelIndex", "UInt32"));
        sizes.add(Global.newArray("txChannelIndex", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPICC3200DMA_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPICC3200DMA_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPICC3200DMA_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPICC3200DMA_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("transferComplete", "UPtr"));
        sizes.add(Global.newArray("hwi", "UPtr"));
        sizes.add(Global.newArray("transferMode", "Nti.drivers.SPI.TransferMode;;;"));
        sizes.add(Global.newArray("transferCallbackFxn", "UFxn"));
        sizes.add(Global.newArray("transaction", "UPtr"));
        sizes.add(Global.newArray("frameSize", "Nti.drivers.SPI.SPICC3200DMA_FrameSize;;0;1;2"));
        sizes.add(Global.newArray("spiMode", "ULong"));
        sizes.add(Global.newArray("rxFifoTrigger", "ULong"));
        sizes.add(Global.newArray("txFifoTrigger", "ULong"));
        sizes.add(Global.newArray("isOpen", "UShort"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPICC3200DMA_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPICC3200DMA_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPICC3200DMA_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPIUSCIADMA_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("clockSource", "UInt8"));
        sizes.add(Global.newArray("bitOrder", "UInt8"));
        sizes.add(Global.newArray("dmaBaseAddr", "UInt32"));
        sizes.add(Global.newArray("rxDMAChannelIndex", "UInt8"));
        sizes.add(Global.newArray("rxDMASourceTrigger", "UInt8"));
        sizes.add(Global.newArray("txDMAChannelIndex", "UInt8"));
        sizes.add(Global.newArray("txDMASourceTrigger", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPIUSCIADMA_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPIUSCIADMA_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPIUSCIADMA_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPIUSCIADMA_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("isOpen", "UShort"));
        sizes.add(Global.newArray("transferComplete", "UPtr"));
        sizes.add(Global.newArray("transferMode", "Nti.drivers.SPI.TransferMode;;;"));
        sizes.add(Global.newArray("transferCallbackFxn", "UFxn"));
        sizes.add(Global.newArray("transaction", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPIUSCIADMA_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPIUSCIADMA_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPIUSCIADMA_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPIUSCIBDMA_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("clockSource", "UInt8"));
        sizes.add(Global.newArray("bitOrder", "UInt8"));
        sizes.add(Global.newArray("dmaBaseAddr", "UInt32"));
        sizes.add(Global.newArray("rxDMAChannelIndex", "UInt8"));
        sizes.add(Global.newArray("rxDMASourceTrigger", "UInt8"));
        sizes.add(Global.newArray("txDMAChannelIndex", "UInt8"));
        sizes.add(Global.newArray("txDMASourceTrigger", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPIUSCIBDMA_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPIUSCIBDMA_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPIUSCIBDMA_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPIUSCIBDMA_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("isOpen", "UShort"));
        sizes.add(Global.newArray("transferComplete", "UPtr"));
        sizes.add(Global.newArray("transferMode", "Nti.drivers.SPI.TransferMode;;;"));
        sizes.add(Global.newArray("transferCallbackFxn", "UFxn"));
        sizes.add(Global.newArray("transaction", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPIUSCIBDMA_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPIUSCIBDMA_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPIUSCIBDMA_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPIEUSCIADMA_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("clockSource", "UInt8"));
        sizes.add(Global.newArray("BitOrder", "UInt16"));
        sizes.add(Global.newArray("dmaBaseAddr", "UInt32"));
        sizes.add(Global.newArray("rxDMAChannelIndex", "UInt8"));
        sizes.add(Global.newArray("rxDMASourceTrigger", "UInt8"));
        sizes.add(Global.newArray("txDMAChannelIndex", "UInt8"));
        sizes.add(Global.newArray("txDMASourceTrigger", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPIEUSCIADMA_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPIEUSCIADMA_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPIEUSCIADMA_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPIEUSCIADMA_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("isOpen", "UShort"));
        sizes.add(Global.newArray("transferComplete", "UPtr"));
        sizes.add(Global.newArray("transferMode", "Nti.drivers.SPI.TransferMode;;;"));
        sizes.add(Global.newArray("transferCallbackFxn", "UFxn"));
        sizes.add(Global.newArray("transaction", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPIEUSCIADMA_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPIEUSCIADMA_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPIEUSCIADMA_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPIEUSCIBDMA_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("clockSource", "UInt8"));
        sizes.add(Global.newArray("BitOrder", "UInt16"));
        sizes.add(Global.newArray("dmaBaseAddr", "UInt32"));
        sizes.add(Global.newArray("rxDMAChannelIndex", "UInt8"));
        sizes.add(Global.newArray("rxDMASourceTrigger", "UInt8"));
        sizes.add(Global.newArray("txDMAChannelIndex", "UInt8"));
        sizes.add(Global.newArray("txDMASourceTrigger", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPIEUSCIBDMA_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPIEUSCIBDMA_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPIEUSCIBDMA_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPIEUSCIBDMA_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("isOpen", "UShort"));
        sizes.add(Global.newArray("transferComplete", "UPtr"));
        sizes.add(Global.newArray("transferMode", "Nti.drivers.SPI.TransferMode;;;"));
        sizes.add(Global.newArray("transferCallbackFxn", "UFxn"));
        sizes.add(Global.newArray("transaction", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPIEUSCIBDMA_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPIEUSCIBDMA_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPIEUSCIBDMA_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPIMSP432DMA_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("bitOrder", "UInt16"));
        sizes.add(Global.newArray("clockSource", "UInt8"));
        sizes.add(Global.newArray("scratchBufPtr", "UPtr"));
        sizes.add(Global.newArray("defaultTxBufValue", "UInt8"));
        sizes.add(Global.newArray("dmaIntNum", "UInt8"));
        sizes.add(Global.newArray("rxDMAChannelIndex", "UInt32"));
        sizes.add(Global.newArray("txDMAChannelIndex", "UInt32"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPIMSP432DMA_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPIMSP432DMA_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPIMSP432DMA_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.SPI.SPIMSP432DMA_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("transferComplete", "UPtr"));
        sizes.add(Global.newArray("transferMode", "Nti.drivers.SPI.TransferMode;;;"));
        sizes.add(Global.newArray("transferCallbackFxn", "UFxn"));
        sizes.add(Global.newArray("transaction", "UPtr"));
        sizes.add(Global.newArray("isOpen", "UShort"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.SPI.SPIMSP432DMA_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.SPI.SPIMSP432DMA_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.SPI.SPIMSP432DMA_Object'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void UART$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.drivers.UART.UART_Config", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("fxnTablePtr", "UPtr"));
        sizes.add(Global.newArray("object", "UPtr"));
        sizes.add(Global.newArray("hwAttrs", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.UART.UART_Config']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.UART.UART_Config']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.UART.UART_Config'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.UART.UARTTiva_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt"));
        sizes.add(Global.newArray("intNum", "UInt"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.UART.UARTTiva_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.UART.UARTTiva_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.UART.UARTTiva_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.UART.UARTTivaDMA_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt"));
        sizes.add(Global.newArray("intNum", "UInt"));
        sizes.add(Global.newArray("rxChannelIndex", "ULong"));
        sizes.add(Global.newArray("txChannelIndex", "ULong"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.UART.UARTTivaDMA_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.UART.UARTTivaDMA_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.UART.UARTTivaDMA_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.UART.UARTTiva_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("isOpen", "UShort"));
        sizes.add(Global.newArray("readMode", "Nti.drivers.UART.UART_Mode;;;;"));
        sizes.add(Global.newArray("writeMode", "Nti.drivers.UART.UART_Mode;;;;"));
        sizes.add(Global.newArray("readTimeout", "UInt"));
        sizes.add(Global.newArray("writeTimeout", "UInt"));
        sizes.add(Global.newArray("readCallback", "UFxn"));
        sizes.add(Global.newArray("writeCallback", "UFxn"));
        sizes.add(Global.newArray("readReturnMode", "Nti.drivers.UART.UART_ReturnMode;;;"));
        sizes.add(Global.newArray("readDataMode", "Nti.drivers.UART.UART_DataMode;;;"));
        sizes.add(Global.newArray("writeDataMode", "Nti.drivers.UART.UART_DataMode;;;"));
        sizes.add(Global.newArray("readEcho", "Nti.drivers.UART.UART_Echo;;0;1"));
        sizes.add(Global.newArray("writeBuf", "UPtr"));
        sizes.add(Global.newArray("writeCount", "USize"));
        sizes.add(Global.newArray("writeSize", "USize"));
        sizes.add(Global.newArray("writeCR", "UShort"));
        sizes.add(Global.newArray("readBuf", "UPtr"));
        sizes.add(Global.newArray("readCount", "USize"));
        sizes.add(Global.newArray("readSize", "USize"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.UART.UARTTiva_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.UART.UARTTiva_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.UART.UARTTiva_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.UART.UARTCC3200_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt"));
        sizes.add(Global.newArray("intNum", "UInt"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.UART.UARTCC3200_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.UART.UARTCC3200_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.UART.UARTCC3200_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.UART.UARTCC3200DMA_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt"));
        sizes.add(Global.newArray("intNum", "UInt"));
        sizes.add(Global.newArray("rxChannelIndex", "ULong"));
        sizes.add(Global.newArray("txChannelIndex", "ULong"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.UART.UARTCC3200DMA_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.UART.UARTCC3200DMA_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.UART.UARTCC3200DMA_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.UART.UARTCC3200_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("isOpen", "UShort"));
        sizes.add(Global.newArray("readMode", "Nti.drivers.UART.UART_Mode;;;;"));
        sizes.add(Global.newArray("writeMode", "Nti.drivers.UART.UART_Mode;;;;"));
        sizes.add(Global.newArray("readTimeout", "UInt"));
        sizes.add(Global.newArray("writeTimeout", "UInt"));
        sizes.add(Global.newArray("readCallback", "UFxn"));
        sizes.add(Global.newArray("writeCallback", "UFxn"));
        sizes.add(Global.newArray("readReturnMode", "Nti.drivers.UART.UART_ReturnMode;;;"));
        sizes.add(Global.newArray("readDataMode", "Nti.drivers.UART.UART_DataMode;;;"));
        sizes.add(Global.newArray("writeDataMode", "Nti.drivers.UART.UART_DataMode;;;"));
        sizes.add(Global.newArray("readEcho", "Nti.drivers.UART.UART_Echo;;0;1"));
        sizes.add(Global.newArray("writeBuf", "UPtr"));
        sizes.add(Global.newArray("writeCount", "USize"));
        sizes.add(Global.newArray("writeSize", "USize"));
        sizes.add(Global.newArray("writeCR", "UShort"));
        sizes.add(Global.newArray("readBuf", "UPtr"));
        sizes.add(Global.newArray("readCount", "USize"));
        sizes.add(Global.newArray("readSize", "USize"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.UART.UARTCC3200_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.UART.UARTCC3200_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.UART.UARTCC3200_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.UART.UARTTivaDMA_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("opened", "UShort"));
        sizes.add(Global.newArray("readMode", "Nti.drivers.UART.UART_Mode;;;;"));
        sizes.add(Global.newArray("writeMode", "Nti.drivers.UART.UART_Mode;;;;"));
        sizes.add(Global.newArray("readTimeout", "UInt"));
        sizes.add(Global.newArray("writeTimeout", "UInt"));
        sizes.add(Global.newArray("readCallback", "UFxn"));
        sizes.add(Global.newArray("writeCallback", "UFxn"));
        sizes.add(Global.newArray("readReturnMode", "Nti.drivers.UART.UART_ReturnMode;;;"));
        sizes.add(Global.newArray("readDataMode", "Nti.drivers.UART.UART_DataMode;;;"));
        sizes.add(Global.newArray("writeDataMode", "Nti.drivers.UART.UART_DataMode;;;"));
        sizes.add(Global.newArray("readEcho", "Nti.drivers.UART.UART_Echo;;0;1"));
        sizes.add(Global.newArray("writeBuf", "UPtr"));
        sizes.add(Global.newArray("writeCount", "USize"));
        sizes.add(Global.newArray("writeSize", "USize"));
        sizes.add(Global.newArray("readBuf", "UPtr"));
        sizes.add(Global.newArray("readCount", "USize"));
        sizes.add(Global.newArray("readSize", "USize"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.UART.UARTTivaDMA_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.UART.UARTTivaDMA_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.UART.UARTTivaDMA_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.UART.UARTCC3200DMA_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("opened", "UShort"));
        sizes.add(Global.newArray("readMode", "Nti.drivers.UART.UART_Mode;;;;"));
        sizes.add(Global.newArray("writeMode", "Nti.drivers.UART.UART_Mode;;;;"));
        sizes.add(Global.newArray("readTimeout", "UInt"));
        sizes.add(Global.newArray("writeTimeout", "UInt"));
        sizes.add(Global.newArray("readCallback", "UFxn"));
        sizes.add(Global.newArray("writeCallback", "UFxn"));
        sizes.add(Global.newArray("readReturnMode", "Nti.drivers.UART.UART_ReturnMode;;;"));
        sizes.add(Global.newArray("readDataMode", "Nti.drivers.UART.UART_DataMode;;;"));
        sizes.add(Global.newArray("writeDataMode", "Nti.drivers.UART.UART_DataMode;;;"));
        sizes.add(Global.newArray("readEcho", "Nti.drivers.UART.UART_Echo;;0;1"));
        sizes.add(Global.newArray("writeBuf", "UPtr"));
        sizes.add(Global.newArray("writeCount", "USize"));
        sizes.add(Global.newArray("writeSize", "USize"));
        sizes.add(Global.newArray("readBuf", "UPtr"));
        sizes.add(Global.newArray("readCount", "USize"));
        sizes.add(Global.newArray("readSize", "USize"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.UART.UARTCC3200DMA_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.UART.UARTCC3200DMA_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.UART.UARTCC3200DMA_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.UART.UARTUSCIA_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt"));
        sizes.add(Global.newArray("clockSouce", "UInt8"));
        sizes.add(Global.newArray("bitOrder", "UInt32"));
        sizes.add(Global.newArray("numBaudrateEntries", "UInt"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.UART.UARTUSCIA_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.UART.UARTUSCIA_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.UART.UARTUSCIA_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.UART.UARTUSCIA_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("isOpen", "UShort"));
        sizes.add(Global.newArray("readMode", "Nti.drivers.UART.UART_Mode;;;;"));
        sizes.add(Global.newArray("writeMode", "Nti.drivers.UART.UART_Mode;;;;"));
        sizes.add(Global.newArray("readTimeout", "UInt"));
        sizes.add(Global.newArray("writeTimeout", "UInt"));
        sizes.add(Global.newArray("readCallback", "UFxn"));
        sizes.add(Global.newArray("writeCallback", "UFxn"));
        sizes.add(Global.newArray("readReturnMode", "Nti.drivers.UART.UART_ReturnMode;;;"));
        sizes.add(Global.newArray("readDataMode", "Nti.drivers.UART.UART_DataMode;;;"));
        sizes.add(Global.newArray("writeDataMode", "Nti.drivers.UART.UART_DataMode;;;"));
        sizes.add(Global.newArray("readEcho", "Nti.drivers.UART.UART_Echo;;0;1"));
        sizes.add(Global.newArray("writeBuf", "UPtr"));
        sizes.add(Global.newArray("writeCount", "USize"));
        sizes.add(Global.newArray("writeSize", "USize"));
        sizes.add(Global.newArray("writeCR", "UShort"));
        sizes.add(Global.newArray("readBuf", "UPtr"));
        sizes.add(Global.newArray("readCount", "USize"));
        sizes.add(Global.newArray("readSize", "USize"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.UART.UARTUSCIA_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.UART.UARTUSCIA_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.UART.UARTUSCIA_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.UART.UARTEUSCIA_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt"));
        sizes.add(Global.newArray("clockSouce", "UInt8"));
        sizes.add(Global.newArray("bitOrder", "UInt32"));
        sizes.add(Global.newArray("numBaudrateEntries", "UInt"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.UART.UARTEUSCIA_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.UART.UARTEUSCIA_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.UART.UARTEUSCIA_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.UART.UARTEUSCIA_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("isOpen", "UShort"));
        sizes.add(Global.newArray("readMode", "Nti.drivers.UART.UART_Mode;;;;"));
        sizes.add(Global.newArray("writeMode", "Nti.drivers.UART.UART_Mode;;;;"));
        sizes.add(Global.newArray("readTimeout", "UInt"));
        sizes.add(Global.newArray("writeTimeout", "UInt"));
        sizes.add(Global.newArray("readCallback", "UFxn"));
        sizes.add(Global.newArray("writeCallback", "UFxn"));
        sizes.add(Global.newArray("readReturnMode", "Nti.drivers.UART.UART_ReturnMode;;;"));
        sizes.add(Global.newArray("readDataMode", "Nti.drivers.UART.UART_DataMode;;;"));
        sizes.add(Global.newArray("writeDataMode", "Nti.drivers.UART.UART_DataMode;;;"));
        sizes.add(Global.newArray("readEcho", "Nti.drivers.UART.UART_Echo;;0;1"));
        sizes.add(Global.newArray("writeBuf", "UPtr"));
        sizes.add(Global.newArray("writeCount", "USize"));
        sizes.add(Global.newArray("writeSize", "USize"));
        sizes.add(Global.newArray("writeCR", "UShort"));
        sizes.add(Global.newArray("readBuf", "UPtr"));
        sizes.add(Global.newArray("readCount", "USize"));
        sizes.add(Global.newArray("readSize", "USize"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.UART.UARTEUSCIA_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.UART.UARTEUSCIA_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.UART.UARTEUSCIA_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.UART.UARTMSP432_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("intNum", "UInt8"));
        sizes.add(Global.newArray("clockSouce", "UInt8"));
        sizes.add(Global.newArray("bitOrder", "UInt32"));
        sizes.add(Global.newArray("numBaudrateEntries", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.UART.UARTMSP432_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.UART.UARTMSP432_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.UART.UARTMSP432_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.UART.UARTMSP432_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("readMode", "Nti.drivers.UART.UART_Mode;;;;"));
        sizes.add(Global.newArray("writeMode", "Nti.drivers.UART.UART_Mode;;;;"));
        sizes.add(Global.newArray("readTimeout", "UInt"));
        sizes.add(Global.newArray("writeTimeout", "UInt"));
        sizes.add(Global.newArray("readCallback", "UFxn"));
        sizes.add(Global.newArray("writeCallback", "UFxn"));
        sizes.add(Global.newArray("readReturnMode", "Nti.drivers.UART.UART_ReturnMode;;;"));
        sizes.add(Global.newArray("readDataMode", "Nti.drivers.UART.UART_DataMode;;;"));
        sizes.add(Global.newArray("writeDataMode", "Nti.drivers.UART.UART_DataMode;;;"));
        sizes.add(Global.newArray("readEcho", "Nti.drivers.UART.UART_Echo;;0;1"));
        sizes.add(Global.newArray("writeBuf", "UPtr"));
        sizes.add(Global.newArray("writeCount", "USize"));
        sizes.add(Global.newArray("writeSize", "USize"));
        sizes.add(Global.newArray("writeCR", "UShort"));
        sizes.add(Global.newArray("readBuf", "UPtr"));
        sizes.add(Global.newArray("readCount", "USize"));
        sizes.add(Global.newArray("readSize", "USize"));
        sizes.add(Global.newArray("writeSem", "UPtr"));
        sizes.add(Global.newArray("readSem", "UPtr"));
        sizes.add(Global.newArray("isOpen", "UShort"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.UART.UARTMSP432_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.UART.UARTMSP432_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.UART.UARTMSP432_Object'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void USBMSCHFatFs$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFs_Config", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("fxnTablePtr", "UPtr"));
        sizes.add(Global.newArray("object", "UPtr"));
        sizes.add(Global.newArray("hwAttrs", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.USBMSCHFatFs.USBMSCHFatFs_Config']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.USBMSCHFatFs.USBMSCHFatFs_Config']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.USBMSCHFatFs.USBMSCHFatFs_Config'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "ULong"));
        sizes.add(Global.newArray("portSPI", "ULong"));
        sizes.add(Global.newArray("pinSCK", "ULong"));
        sizes.add(Global.newArray("pinMISO", "ULong"));
        sizes.add(Global.newArray("pinMOSI", "ULong"));
        sizes.add(Global.newArray("portCS", "ULong"));
        sizes.add(Global.newArray("pinCS", "ULong"));
        sizes.add(Global.newArray("portTX", "ULong"));
        sizes.add(Global.newArray("pinTX", "ULong"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("driveNumber", "UInt32"));
        sizes.add(Global.newArray("state", "Nti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_USBState;;;;;"));
        sizes.add(Global.newArray("hwi", "UPtr"));
        sizes.add(Global.newArray("taskHCDMain", "UPtr"));
        sizes.add(Global.newArray("gateUSBWait", "UPtr"));
        sizes.add(Global.newArray("gateUSBLibAccess", "UPtr"));
        sizes.add(Global.newArray("semUSBConnected", "UPtr"));
        sizes.add(Global.newArray("MSCInstance", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_Object'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void Watchdog$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.drivers.Watchdog.WatchdogTiva_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "ULong"));
        sizes.add(Global.newArray("intNum", "UInt"));
        sizes.add(Global.newArray("reloadValue", "ULong"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.Watchdog.WatchdogTiva_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.Watchdog.WatchdogTiva_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.Watchdog.WatchdogTiva_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.Watchdog.WatchdogTiva_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("isOpen", "UShort"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.Watchdog.WatchdogTiva_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.Watchdog.WatchdogTiva_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.Watchdog.WatchdogTiva_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.Watchdog.WatchdogCC3200_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "ULong"));
        sizes.add(Global.newArray("intNum", "UInt"));
        sizes.add(Global.newArray("reloadValue", "ULong"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.Watchdog.WatchdogCC3200_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.Watchdog.WatchdogCC3200_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.Watchdog.WatchdogCC3200_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.Watchdog.WatchdogCC3200_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("isOpen", "UShort"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.Watchdog.WatchdogCC3200_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.Watchdog.WatchdogCC3200_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.Watchdog.WatchdogCC3200_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.Watchdog.WatchdogMSP430_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "ULong"));
        sizes.add(Global.newArray("sfrAddr", "ULong"));
        sizes.add(Global.newArray("clockSource", "UInt"));
        sizes.add(Global.newArray("clockDivider", "UInt"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.Watchdog.WatchdogMSP430_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.Watchdog.WatchdogMSP430_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.Watchdog.WatchdogMSP430_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.Watchdog.WatchdogMSP430_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("isOpen", "UShort"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.Watchdog.WatchdogMSP430_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.Watchdog.WatchdogMSP430_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.Watchdog.WatchdogMSP430_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.Watchdog.WatchdogMSP432_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("baseAddr", "UInt32"));
        sizes.add(Global.newArray("intNum", "UInt8"));
        sizes.add(Global.newArray("clockSource", "UInt8"));
        sizes.add(Global.newArray("clockDivider", "UInt8"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.Watchdog.WatchdogMSP432_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.Watchdog.WatchdogMSP432_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.Watchdog.WatchdogMSP432_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.Watchdog.WatchdogMSP432_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("isOpen", "UShort"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.Watchdog.WatchdogMSP432_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.Watchdog.WatchdogMSP432_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.Watchdog.WatchdogMSP432_Object'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.Watchdog.Watchdog_Config", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("fxnTablePtr", "UPtr"));
        sizes.add(Global.newArray("object", "UPtr"));
        sizes.add(Global.newArray("hwAttrs", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.Watchdog.Watchdog_Config']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.Watchdog.Watchdog_Config']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.Watchdog.Watchdog_Config'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void WiFi$$SIZES()
    {
        Proto.Str so;
        Object fxn;

        so = (Proto.Str)om.findStrict("ti.drivers.WiFi.WiFi_Config", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("fxnTablePtr", "UPtr"));
        sizes.add(Global.newArray("object", "UPtr"));
        sizes.add(Global.newArray("hwAttrs", "UPtr"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.WiFi.WiFi_Config']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.WiFi.WiFi_Config']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.WiFi.WiFi_Config'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.WiFi.WiFiCC3100_HWAttrs", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("irqPort", "ULong"));
        sizes.add(Global.newArray("irqPin", "ULong"));
        sizes.add(Global.newArray("irqIntNum", "ULong"));
        sizes.add(Global.newArray("csPort", "ULong"));
        sizes.add(Global.newArray("csPin", "ULong"));
        sizes.add(Global.newArray("enPort", "ULong"));
        sizes.add(Global.newArray("enPin", "ULong"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.WiFi.WiFiCC3100_HWAttrs']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.WiFi.WiFiCC3100_HWAttrs']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.WiFi.WiFiCC3100_HWAttrs'], fld); }");
        so.bind("$offsetof", fxn);
        so = (Proto.Str)om.findStrict("ti.drivers.WiFi.WiFiCC3100_Object", "ti.drivers");
        sizes.clear();
        sizes.add(Global.newArray("wifiHwi", "UPtr"));
        sizes.add(Global.newArray("spiHandle", "UPtr"));
        sizes.add(Global.newArray("wifiIntFxn", "UFxn"));
        sizes.add(Global.newArray("readSemaphore", "UPtr"));
        sizes.add(Global.newArray("writeSemaphore", "UPtr"));
        sizes.add(Global.newArray("spiState", "Nti.drivers.WiFi.WiFiCC3100_SPI_State;;;;;;;"));
        sizes.add(Global.newArray("isOpen", "UShort"));
        so.bind("$$sizes", Global.newArray(sizes.toArray()));
        fxn = Global.eval("function() { return $$sizeof(xdc.om['ti.drivers.WiFi.WiFiCC3100_Object']); }");
        so.bind("$sizeof", fxn);
        fxn = Global.eval("function() { return $$alignof(xdc.om['ti.drivers.WiFi.WiFiCC3100_Object']); }");
        so.bind("$alignof", fxn);
        fxn = Global.eval("function(fld) { return $$offsetof(xdc.om['ti.drivers.WiFi.WiFiCC3100_Object'], fld); }");
        so.bind("$offsetof", fxn);
    }

    void EMAC$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/drivers/EMAC.xs");
        om.bind("ti.drivers.EMAC$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC.Module", "ti.drivers");
        po.init("ti.drivers.EMAC.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.drivers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.drivers"), $$UNDEF, "wh");
            po.addFld("libType", (Proto)om.findStrict("ti.drivers.EMAC.LibType", "ti.drivers"), om.find("ti.drivers.EMAC.LibType_Instrumented"), "wh");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.drivers.EMAC$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.drivers.EMAC$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.drivers.EMAC$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.drivers.EMAC$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        // struct EMAC.EMAC_Config
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC$$EMAC_Config", "ti.drivers");
        po.init("ti.drivers.EMAC.EMAC_Config", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("fxnTablePtr", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("object", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("hwAttrs", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        // struct EMAC.EMACTiva_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC$$EMACTiva_Object", "ti.drivers");
        po.init("ti.drivers.EMAC.EMACTiva_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("swi", (Proto)om.findStrict("ti.sysbios.knl.Swi.Handle", "ti.drivers"), $$UNDEF, "w");
                po.addFld("hwi", (Proto)om.findStrict("ti.sysbios.hal.Hwi.Handle", "ti.drivers"), $$UNDEF, "w");
                po.addFld("rxCount", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("rxDropped", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("txSent", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("txDropped", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("linkUp", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("pTxDescList", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("pRxDescList", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        // struct EMAC.EMACTiva_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC$$EMACTiva_HWAttrs", "ti.drivers");
        po.init("ti.drivers.EMAC.EMACTiva_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("macAddress", $$T_Str, $$UNDEF, "w");
        // struct EMAC.EMACSnow_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC$$EMACSnow_HWAttrs", "ti.drivers");
        po.init("ti.drivers.EMAC.EMACSnow_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("macAddress", $$T_Str, $$UNDEF, "w");
        // struct EMAC.EMACSnow_Data
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC$$EMACSnow_Data", "ti.drivers");
        po.init("ti.drivers.EMAC.EMACSnow_Data", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("hEvent", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("PBM_tx", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt8)"), false, xdc.services.intern.xsr.Enum.intValue(12L)), $$DEFAULT, "w");
                po.addFld("PBM_rx", new Proto.Arr(Proto.Elm.newCNum("(xdc_UInt8)"), false, xdc.services.intern.xsr.Enum.intValue(12L)), $$DEFAULT, "w");
                po.addFld("rxCount", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("rxDropped", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("txSent", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("txDropped", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("abnormalInts", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("isrCount", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("linkUp", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
        // struct EMAC.EMACSnow_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC$$EMACSnow_Object", "ti.drivers");
        po.init("ti.drivers.EMAC.EMACSnow_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("swi", (Proto)om.findStrict("ti.sysbios.knl.Swi.Handle", "ti.drivers"), $$UNDEF, "w");
        // struct EMAC.BasicView
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC$$BasicView", "ti.drivers");
        po.init("ti.drivers.EMAC.BasicView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("macAddress", $$T_Str, $$UNDEF, "w");
                po.addFld("functionTable", $$T_Str, $$UNDEF, "w");
                po.addFld("libType", $$T_Str, $$UNDEF, "w");
                po.addFld("emacHandle", $$T_Str, $$UNDEF, "w");
                po.addFld("linkUp", $$T_Bool, $$UNDEF, "w");
        // struct EMAC.StatsView
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC$$StatsView", "ti.drivers");
        po.init("ti.drivers.EMAC.StatsView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("rxCount", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("rxDropped", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("txSent", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("txDropped", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
    }

    void GPIO$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/drivers/GPIO.xs");
        om.bind("ti.drivers.GPIO$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.drivers.GPIO.Module", "ti.drivers");
        po.init("ti.drivers.GPIO.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.drivers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("libType", (Proto)om.findStrict("ti.drivers.GPIO.LibType", "ti.drivers"), om.find("ti.drivers.GPIO.LibType_Instrumented"), "wh");
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.drivers"), $$UNDEF, "wh");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.drivers.GPIO$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.drivers.GPIO$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.drivers.GPIO$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.drivers.GPIO$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        // typedef GPIO.GPIO_PinConfig
        om.bind("ti.drivers.GPIO.GPIO_PinConfig", Proto.Elm.newCNum("(xdc_UInt32)"));
        // typedef GPIO.GPIO_CallbackFxn
        om.bind("ti.drivers.GPIO.GPIO_CallbackFxn", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"));
        // struct GPIO.GPIO_Config
        po = (Proto.Obj)om.findStrict("ti.drivers.GPIO$$GPIO_Config", "ti.drivers");
        po.init("ti.drivers.GPIO.GPIO_Config", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("pinConfigs", new Proto.Adr("xdc_UInt32*", "Pn"), $$UNDEF, "w");
                po.addFld("callbacks", new Proto.Adr("xdc_Void(*)(xdc_Void)*", "PPFv"), $$UNDEF, "w");
                po.addFld("numberOfPinConfigs", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("numberOfCallbacks", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
                po.addFld("intPriority", Proto.Elm.newCNum("(xdc_UInt32)"), $$UNDEF, "w");
        // struct GPIO.BasicView
        po = (Proto.Obj)om.findStrict("ti.drivers.GPIO$$BasicView", "ti.drivers");
        po.init("ti.drivers.GPIO.BasicView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("Index", $$T_Str, $$UNDEF, "w");
                po.addFld("Port", $$T_Str, $$UNDEF, "w");
                po.addFld("Pin", $$T_Str, $$UNDEF, "w");
                po.addFld("Direction", $$T_Str, $$UNDEF, "w");
                po.addFld("Value", $$T_Str, $$UNDEF, "w");
    }

    void I2C$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/drivers/I2C.xs");
        om.bind("ti.drivers.I2C$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C.Module", "ti.drivers");
        po.init("ti.drivers.I2C.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.drivers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("libType", (Proto)om.findStrict("ti.drivers.I2C.LibType", "ti.drivers"), om.find("ti.drivers.I2C.LibType_Instrumented"), "wh");
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.drivers"), $$UNDEF, "wh");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.drivers.I2C$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.drivers.I2C$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.drivers.I2C$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.drivers.I2C$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        // struct I2C.I2C_Config
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2C_Config", "ti.drivers");
        po.init("ti.drivers.I2C.I2C_Config", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("fxnTablePtr", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("object", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("hwAttrs", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
        // typedef I2C.CallbackFxn
        om.bind("ti.drivers.I2C.CallbackFxn", new Proto.Adr("xdc_Void(*)(ti_drivers_I2C_I2C_Config*,ti_drivers_I2C_I2C_Transaction*,xdc_Bool)", "PFv"));
        // struct I2C.I2C_Transaction
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2C_Transaction", "ti.drivers");
        po.init("ti.drivers.I2C.I2C_Transaction", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("txBuff", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("txCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("rxBuff", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("rxCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("slaveAddress", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("arg", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("nextPtr", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        // struct I2C.I2C_Params
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2C_Params", "ti.drivers");
        po.init("ti.drivers.I2C.I2C_Params", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("transferMode", (Proto)om.findStrict("ti.drivers.I2C.I2C_TransferMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("transferCallback", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        // struct I2C.I2CTiva_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CTiva_HWAttrs", "ti.drivers");
        po.init("ti.drivers.I2C.I2CTiva_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
        // struct I2C.I2CTiva_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CTiva_Object", "ti.drivers");
        po.init("ti.drivers.I2C.I2CTiva_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("mutex", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("transferComplete", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("i2cParams", (Proto)om.findStrict("ti.drivers.I2C.I2C_Params", "ti.drivers"), $$DEFAULT, "w");
                po.addFld("hwi", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("mode", (Proto)om.findStrict("ti.drivers.I2C.I2C_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("currentTransaction", new Proto.Adr("ti_drivers_I2C_I2C_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("writeBufIdx", new Proto.Adr("xdc_Bits8*", "Pn"), $$UNDEF, "w");
                po.addFld("writeCountIdx", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("readBufIdx", new Proto.Adr("xdc_Bits8*", "Pn"), $$UNDEF, "w");
                po.addFld("readCountIdx", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("headPtr", new Proto.Adr("ti_drivers_I2C_I2C_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("tailPtr", new Proto.Adr("ti_drivers_I2C_I2C_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
        // struct I2C.I2CCC3200_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CCC3200_HWAttrs", "ti.drivers");
        po.init("ti.drivers.I2C.I2CCC3200_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
        // struct I2C.I2CCC3200_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CCC3200_Object", "ti.drivers");
        po.init("ti.drivers.I2C.I2CCC3200_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("mutex", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("transferComplete", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("i2cParams", (Proto)om.findStrict("ti.drivers.I2C.I2C_Params", "ti.drivers"), $$DEFAULT, "w");
                po.addFld("hwi", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("mode", (Proto)om.findStrict("ti.drivers.I2C.I2C_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("currentTransaction", new Proto.Adr("ti_drivers_I2C_I2C_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("writeBufIdx", new Proto.Adr("xdc_Bits8*", "Pn"), $$UNDEF, "w");
                po.addFld("writeCountIdx", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("readBufIdx", new Proto.Adr("xdc_Bits8*", "Pn"), $$UNDEF, "w");
                po.addFld("readCountIdx", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("headPtr", new Proto.Adr("ti_drivers_I2C_I2C_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("tailPtr", new Proto.Adr("ti_drivers_I2C_I2C_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
        // struct I2C.I2CMSP432_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CMSP432_HWAttrs", "ti.drivers");
        po.init("ti.drivers.I2C.I2CMSP432_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("clockSource", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct I2C.I2CMSP432_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CMSP432_Object", "ti.drivers");
        po.init("ti.drivers.I2C.I2CMSP432_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("mutex", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("transferComplete", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("transferMode", (Proto)om.findStrict("ti.drivers.I2C.I2C_TransferMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("transferCallbackFxn", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("mode", (Proto)om.findStrict("ti.drivers.I2C.I2C_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("currentTransaction", new Proto.Adr("ti_drivers_I2C_I2C_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("writeBufIdx", new Proto.Adr("xdc_UChar*", "Pn"), $$UNDEF, "w");
                po.addFld("writeCountIdx", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("readBufIdx", new Proto.Adr("xdc_UChar*", "Pn"), $$UNDEF, "w");
                po.addFld("readCountIdx", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("headPtr", new Proto.Adr("ti_drivers_I2C_I2C_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("tailPtr", new Proto.Adr("ti_drivers_I2C_I2C_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
        // struct I2C.I2CUSCIB_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CUSCIB_HWAttrs", "ti.drivers");
        po.init("ti.drivers.I2C.I2CUSCIB_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("clockSource", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct I2C.I2CEUSCIB_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CEUSCIB_HWAttrs", "ti.drivers");
        po.init("ti.drivers.I2C.I2CEUSCIB_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("clockSource", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct I2C.I2CUSCIB_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CUSCIB_Object", "ti.drivers");
        po.init("ti.drivers.I2C.I2CUSCIB_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
                po.addFld("mutex", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("transferComplete", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("i2cParams", (Proto)om.findStrict("ti.drivers.I2C.I2C_Params", "ti.drivers"), $$DEFAULT, "w");
                po.addFld("mode", (Proto)om.findStrict("ti.drivers.I2C.I2C_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("currentTransaction", new Proto.Adr("ti_drivers_I2C_I2C_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("writeBufIdx", new Proto.Adr("xdc_UChar*", "Pn"), $$UNDEF, "w");
                po.addFld("writeCountIdx", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("readBufIdx", new Proto.Adr("xdc_UChar*", "Pn"), $$UNDEF, "w");
                po.addFld("readCountIdx", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("headPtr", new Proto.Adr("ti_drivers_I2C_I2C_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("tailPtr", new Proto.Adr("ti_drivers_I2C_I2C_Transaction*", "PS"), $$UNDEF, "w");
        // struct I2C.I2CEUSCIB_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CEUSCIB_Object", "ti.drivers");
        po.init("ti.drivers.I2C.I2CEUSCIB_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
                po.addFld("mutex", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("transferComplete", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("i2cParams", (Proto)om.findStrict("ti.drivers.I2C.I2C_Params", "ti.drivers"), $$DEFAULT, "w");
                po.addFld("mode", (Proto)om.findStrict("ti.drivers.I2C.I2C_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("currentTransaction", new Proto.Adr("ti_drivers_I2C_I2C_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("writeBufIdx", new Proto.Adr("xdc_Bits8*", "Pn"), $$UNDEF, "w");
                po.addFld("writeCountIdx", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("readBufIdx", new Proto.Adr("xdc_Bits8*", "Pn"), $$UNDEF, "w");
                po.addFld("readCountIdx", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("headPtr", new Proto.Adr("ti_drivers_I2C_I2C_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("tailPtr", new Proto.Adr("ti_drivers_I2C_I2C_Transaction*", "PS"), $$UNDEF, "w");
        // struct I2C.BasicView
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$BasicView", "ti.drivers");
        po.init("ti.drivers.I2C.BasicView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("i2cHandle", $$T_Str, $$UNDEF, "w");
                po.addFld("baseAddr", $$T_Str, $$UNDEF, "w");
                po.addFld("functionTable", $$T_Str, $$UNDEF, "w");
    }

    void Power$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/drivers/Power.xs");
        om.bind("ti.drivers.Power$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.drivers.Power.Module", "ti.drivers");
        po.init("ti.drivers.Power.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.drivers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.drivers.Power$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.drivers.Power$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.drivers.Power$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.drivers.Power$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
    }

    void PWM$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/drivers/PWM.xs");
        om.bind("ti.drivers.PWM$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM.Module", "ti.drivers");
        po.init("ti.drivers.PWM.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.drivers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("libType", (Proto)om.findStrict("ti.drivers.PWM.LibType", "ti.drivers"), om.find("ti.drivers.PWM.LibType_Instrumented"), "wh");
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.drivers"), $$UNDEF, "wh");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.drivers.PWM$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.drivers.PWM$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.drivers.PWM$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.drivers.PWM$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        // struct PWM.PWM_Config
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWM_Config", "ti.drivers");
        po.init("ti.drivers.PWM.PWM_Config", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("fxnTablePtr", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("object", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("hwAttrs", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
        // struct PWM.PWM_Params
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWM_Params", "ti.drivers");
        po.init("ti.drivers.PWM.PWM_Params", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("period", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("dutyMode", (Proto)om.findStrict("ti.drivers.PWM.PWM_DutyMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("polarity", (Proto)om.findStrict("ti.drivers.PWM.PWM_Polarity", "ti.drivers"), $$UNDEF, "w");
        // struct PWM.PWMTimerCC3200_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTimerCC3200_HWAttrs", "ti.drivers");
        po.init("ti.drivers.PWM.PWMTimerCC3200_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("timer", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
        // struct PWM.PWMTimerCC3200_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTimerCC3200_Object", "ti.drivers");
        po.init("ti.drivers.PWM.PWMTimerCC3200_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("period", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("duty", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("dutyMode", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("cyclesPerMicroSec", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct PWM.PWMTiva_Status
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTiva_Status", "ti.drivers");
        po.init("ti.drivers.PWM.PWMTiva_Status", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("genPeriods", new Proto.Arr(Proto.Elm.newCNum("(xdc_Bits16)"), false, xdc.services.intern.xsr.Enum.intValue(4L)), $$DEFAULT, "w");
                po.addFld("cyclesPerMicroSec", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("prescalar", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("activeOutputs", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct PWM.PWMTiva_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTiva_HWAttrs", "ti.drivers");
        po.init("ti.drivers.PWM.PWMTiva_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("pwmOutput", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("pwmGenOpts", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct PWM.PWMTiva_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTiva_Object", "ti.drivers");
        po.init("ti.drivers.PWM.PWMTiva_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("pwmStatus", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("pwmDuty", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("pwmOutputBit", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("dutyMode", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct PWM.PWMTimerTiva_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTimerTiva_HWAttrs", "ti.drivers");
        po.init("ti.drivers.PWM.PWMTimerTiva_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("timer", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
        // struct PWM.PWMTimerTiva_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTimerTiva_Object", "ti.drivers");
        po.init("ti.drivers.PWM.PWMTimerTiva_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("period", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("duty", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("dutyMode", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("cyclesPerMicroSec", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct PWM.PWMTimerMSP432_Status
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTimerMSP432_Status", "ti.drivers");
        po.init("ti.drivers.PWM.PWMTimerMSP432_Status", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("period", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("prescalar", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("cyclesPerMicroSec", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("activeOutputs", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct PWM.PWMTimerMSP432_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTimerMSP432_HWAttrs", "ti.drivers");
        po.init("ti.drivers.PWM.PWMTimerMSP432_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("compareRegister", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct PWM.PWMTimerMSP432_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTimerMSP432_Object", "ti.drivers");
        po.init("ti.drivers.PWM.PWMTimerMSP432_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("pwmStatus", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("pwmDuty", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("pwmCompareRegisterBit", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("dutyMode", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct PWM.BasicView
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$BasicView", "ti.drivers");
        po.init("ti.drivers.PWM.BasicView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("pwmHandle", $$T_Str, $$UNDEF, "w");
                po.addFld("baseAddr", $$T_Str, $$UNDEF, "w");
                po.addFld("functionTable", $$T_Str, $$UNDEF, "w");
    }

    void SDSPI$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/drivers/SDSPI.xs");
        om.bind("ti.drivers.SDSPI$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI.Module", "ti.drivers");
        po.init("ti.drivers.SDSPI.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.drivers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("libType", (Proto)om.findStrict("ti.drivers.SDSPI.LibType", "ti.drivers"), om.find("ti.drivers.SDSPI.LibType_Instrumented"), "wh");
            po.addFld("maxPeripheralCount", Proto.Elm.newCNum("(xdc_UInt)"), 32L, "wh");
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.drivers"), $$UNDEF, "wh");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.drivers.SDSPI$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.drivers.SDSPI$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.drivers.SDSPI$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.drivers.SDSPI$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        // struct SDSPI.SDSPI_Config
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPI_Config", "ti.drivers");
        po.init("ti.drivers.SDSPI.SDSPI_Config", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("fxnTablePtr", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("object", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("hwAttrs", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
        // struct SDSPI.SDSPITiva_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPITiva_HWAttrs", "ti.drivers");
        po.init("ti.drivers.SDSPI.SDSPITiva_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("portSPI", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("pinSCK", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("pinMISO", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("pinMOSI", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("portCS", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("pinCS", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("portTX", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("pinTX", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SDSPI.SDSPITiva_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPITiva_Object", "ti.drivers");
        po.init("ti.drivers.SDSPI.SDSPITiva_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("driveNumber", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("diskState", Proto.Elm.newCNum("(xdc_UChar)"), $$UNDEF, "w");
                po.addFld("cardType", (Proto)om.findStrict("ti.drivers.SDSPI.SDSPITiva_CardType", "ti.drivers"), $$UNDEF, "w");
                po.addFld("bitRate", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SDSPI.SDSPIUSCIA_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIUSCIA_HWAttrs", "ti.drivers");
        po.init("ti.drivers.SDSPI.SDSPIUSCIA_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("clockSource", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("portSPI", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinSCK", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("pinMISO", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("pinMOSI", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("portCS", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinCS", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SDSPI.SDSPIUSCIB_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIUSCIB_HWAttrs", "ti.drivers");
        po.init("ti.drivers.SDSPI.SDSPIUSCIB_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("clockSource", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("portSPI", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinSCK", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("pinMISO", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("pinMOSI", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("portCS", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinCS", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SDSPI.SDSPIUSCIA_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIUSCIA_Object", "ti.drivers");
        po.init("ti.drivers.SDSPI.SDSPIUSCIA_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("driveNumber", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("diskState", Proto.Elm.newCNum("(xdc_UChar)"), $$UNDEF, "w");
                po.addFld("cardType", (Proto)om.findStrict("ti.drivers.SDSPI.SDSPITiva_CardType", "ti.drivers"), $$UNDEF, "w");
                po.addFld("bitRate", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SDSPI.SDSPIUSCIB_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIUSCIB_Object", "ti.drivers");
        po.init("ti.drivers.SDSPI.SDSPIUSCIB_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("driveNumber", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("diskState", Proto.Elm.newCNum("(xdc_UChar)"), $$UNDEF, "w");
                po.addFld("cardType", (Proto)om.findStrict("ti.drivers.SDSPI.SDSPITiva_CardType", "ti.drivers"), $$UNDEF, "w");
                po.addFld("bitRate", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SDSPI.SDSPIEUSCIA_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIEUSCIA_HWAttrs", "ti.drivers");
        po.init("ti.drivers.SDSPI.SDSPIEUSCIA_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("clockSource", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("portSCK", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinSCK", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("portMISO", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinMISO", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("portMOSI", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinMOSI", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("portCS", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinCS", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SDSPI.SDSPIEUSCIB_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIEUSCIB_HWAttrs", "ti.drivers");
        po.init("ti.drivers.SDSPI.SDSPIEUSCIB_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("clockSource", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("portSCK", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinSCK", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("portMISO", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinMISO", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("portMOSI", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinMOSI", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("portCS", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinCS", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SDSPI.SDSPIEUSCIA_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIEUSCIA_Object", "ti.drivers");
        po.init("ti.drivers.SDSPI.SDSPIEUSCIA_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("driveNumber", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("diskState", Proto.Elm.newCNum("(xdc_UChar)"), $$UNDEF, "w");
                po.addFld("cardType", (Proto)om.findStrict("ti.drivers.SDSPI.SDSPITiva_CardType", "ti.drivers"), $$UNDEF, "w");
                po.addFld("bitRate", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SDSPI.SDSPIEUSCIB_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIEUSCIB_Object", "ti.drivers");
        po.init("ti.drivers.SDSPI.SDSPIEUSCIB_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("driveNumber", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("diskState", Proto.Elm.newCNum("(xdc_UChar)"), $$UNDEF, "w");
                po.addFld("cardType", (Proto)om.findStrict("ti.drivers.SDSPI.SDSPITiva_CardType", "ti.drivers"), $$UNDEF, "w");
                po.addFld("bitRate", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SDSPI.SDSPICC3200_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPICC3200_HWAttrs", "ti.drivers");
        po.init("ti.drivers.SDSPI.SDSPICC3200_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("spiPRCM", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("csGPIOBase", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("csGPIOPin", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("txGPIOBase", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("txGPIOPin", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("txGPIOMode", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("txSPIMode", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("txPackPin", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SDSPI.SDSPICC3200_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPICC3200_Object", "ti.drivers");
        po.init("ti.drivers.SDSPI.SDSPICC3200_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("driveNumber", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("diskState", Proto.Elm.newCNum("(xdc_UChar)"), $$UNDEF, "w");
                po.addFld("cardType", (Proto)om.findStrict("ti.drivers.SDSPI.SDSPITiva_CardType", "ti.drivers"), $$UNDEF, "w");
                po.addFld("bitRate", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SDSPI.SDSPIMSP432_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIMSP432_HWAttrs", "ti.drivers");
        po.init("ti.drivers.SDSPI.SDSPIMSP432_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("clockSource", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("portSCK", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinSCK", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("sckMode", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("portMISO", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinMISO", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("misoMode", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("portMOSI", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinMOSI", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("mosiMode", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("portCS", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("pinCS", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SDSPI.SDSPIMSP432_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIMSP432_Object", "ti.drivers");
        po.init("ti.drivers.SDSPI.SDSPIMSP432_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("driveNumber", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("diskState", Proto.Elm.newCNum("(xdc_UChar)"), $$UNDEF, "w");
                po.addFld("cardType", (Proto)om.findStrict("ti.drivers.SDSPI.SDSPITiva_CardType", "ti.drivers"), $$UNDEF, "w");
                po.addFld("bitRate", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("filesystem", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SDSPI.BasicView
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$BasicView", "ti.drivers");
        po.init("ti.drivers.SDSPI.BasicView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("sdspiHandle", $$T_Str, $$UNDEF, "w");
                po.addFld("baseAddr", $$T_Str, $$UNDEF, "w");
                po.addFld("functionTable", $$T_Str, $$UNDEF, "w");
    }

    void SPI$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/drivers/SPI.xs");
        om.bind("ti.drivers.SPI$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI.Module", "ti.drivers");
        po.init("ti.drivers.SPI.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.drivers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("libType", (Proto)om.findStrict("ti.drivers.SPI.LibType", "ti.drivers"), om.find("ti.drivers.SPI.LibType_Instrumented"), "wh");
            po.addFld("maxPeripheralCount", Proto.Elm.newCNum("(xdc_UInt)"), 32L, "wh");
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.drivers"), $$UNDEF, "wh");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.drivers.SPI$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.drivers.SPI$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.drivers.SPI$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.drivers.SPI$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        // struct SPI.SPI_Config
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPI_Config", "ti.drivers");
        po.init("ti.drivers.SPI.SPI_Config", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("fxnTablePtr", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("object", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("hwAttrs", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
        // typedef SPI.CallbackFxn
        om.bind("ti.drivers.SPI.CallbackFxn", new Proto.Adr("xdc_Void(*)(ti_drivers_SPI_SPI_Config*,ti_drivers_SPI_SPI_Transaction*)", "PFv"));
        // struct SPI.SPI_Transaction
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPI_Transaction", "ti.drivers");
        po.init("ti.drivers.SPI.SPI_Transaction", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("count", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("txBuf", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("rxBuf", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("arg", new Proto.Adr("xdc_UArg", "Pv"), $$UNDEF, "w");
        // struct SPI.SPITivaDMA_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPITivaDMA_HWAttrs", "ti.drivers");
        po.init("ti.drivers.SPI.SPITivaDMA_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("rxChannelIndex", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("txChannelIndex", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("channelMappingFxn", new Proto.Adr("xdc_Void(*)(xdc_ULong)", "PFv"), $$UNDEF, "w");
                po.addFld("rxChannelMappingFxnArg", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("txChannelMappingFxnArg", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SPI.SPITivaDMA_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPITivaDMA_Object", "ti.drivers");
        po.init("ti.drivers.SPI.SPITivaDMA_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("transferComplete", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("hwi", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("transferMode", (Proto)om.findStrict("ti.drivers.SPI.TransferMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("transferCallbackFxn", new Proto.Adr("xdc_Void(*)(ti_drivers_SPI_SPI_Config*,ti_drivers_SPI_SPI_Transaction*)", "PFv"), $$UNDEF, "w");
                po.addFld("transaction", new Proto.Adr("ti_drivers_SPI_SPI_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("frameSize", (Proto)om.findStrict("ti.drivers.SPI.SPITivaDMA_FrameSize", "ti.drivers"), $$UNDEF, "w");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
        // struct SPI.SPICC3200DMA_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPICC3200DMA_HWAttrs", "ti.drivers");
        po.init("ti.drivers.SPI.SPICC3200DMA_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("spiPRCM", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("csControl", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("csPolarity", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("pinMode", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("turboMode", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("scratchBufPtr", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("defaultTxBufValue", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("rxChannelIndex", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("txChannelIndex", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SPI.SPICC3200DMA_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPICC3200DMA_Object", "ti.drivers");
        po.init("ti.drivers.SPI.SPICC3200DMA_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("transferComplete", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("hwi", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("transferMode", (Proto)om.findStrict("ti.drivers.SPI.TransferMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("transferCallbackFxn", new Proto.Adr("xdc_Void(*)(ti_drivers_SPI_SPI_Config*,ti_drivers_SPI_SPI_Transaction*)", "PFv"), $$UNDEF, "w");
                po.addFld("transaction", new Proto.Adr("ti_drivers_SPI_SPI_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("frameSize", (Proto)om.findStrict("ti.drivers.SPI.SPICC3200DMA_FrameSize", "ti.drivers"), $$UNDEF, "w");
                po.addFld("spiMode", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("rxFifoTrigger", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("txFifoTrigger", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
        // struct SPI.SPIUSCIADMA_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIUSCIADMA_HWAttrs", "ti.drivers");
        po.init("ti.drivers.SPI.SPIUSCIADMA_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("clockSource", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("bitOrder", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("dmaBaseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("rxDMAChannelIndex", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("rxDMASourceTrigger", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("txDMAChannelIndex", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("txDMASourceTrigger", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct SPI.SPIUSCIADMA_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIUSCIADMA_Object", "ti.drivers");
        po.init("ti.drivers.SPI.SPIUSCIADMA_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
                po.addFld("transferComplete", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("transferMode", (Proto)om.findStrict("ti.drivers.SPI.TransferMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("transferCallbackFxn", new Proto.Adr("xdc_Void(*)(ti_drivers_SPI_SPI_Config*,ti_drivers_SPI_SPI_Transaction*)", "PFv"), $$UNDEF, "w");
                po.addFld("transaction", new Proto.Adr("ti_drivers_SPI_SPI_Transaction*", "PS"), $$UNDEF, "w");
        // struct SPI.SPIUSCIBDMA_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIUSCIBDMA_HWAttrs", "ti.drivers");
        po.init("ti.drivers.SPI.SPIUSCIBDMA_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("clockSource", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("bitOrder", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("dmaBaseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("rxDMAChannelIndex", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("rxDMASourceTrigger", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("txDMAChannelIndex", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("txDMASourceTrigger", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct SPI.SPIUSCIBDMA_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIUSCIBDMA_Object", "ti.drivers");
        po.init("ti.drivers.SPI.SPIUSCIBDMA_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
                po.addFld("transferComplete", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("transferMode", (Proto)om.findStrict("ti.drivers.SPI.TransferMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("transferCallbackFxn", new Proto.Adr("xdc_Void(*)(ti_drivers_SPI_SPI_Config*,ti_drivers_SPI_SPI_Transaction*)", "PFv"), $$UNDEF, "w");
                po.addFld("transaction", new Proto.Adr("ti_drivers_SPI_SPI_Transaction*", "PS"), $$UNDEF, "w");
        // struct SPI.SPIEUSCIADMA_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIEUSCIADMA_HWAttrs", "ti.drivers");
        po.init("ti.drivers.SPI.SPIEUSCIADMA_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("clockSource", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("BitOrder", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("dmaBaseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("rxDMAChannelIndex", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("rxDMASourceTrigger", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("txDMAChannelIndex", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("txDMASourceTrigger", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct SPI.SPIEUSCIADMA_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIEUSCIADMA_Object", "ti.drivers");
        po.init("ti.drivers.SPI.SPIEUSCIADMA_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
                po.addFld("transferComplete", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("transferMode", (Proto)om.findStrict("ti.drivers.SPI.TransferMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("transferCallbackFxn", new Proto.Adr("xdc_Void(*)(ti_drivers_SPI_SPI_Config*,ti_drivers_SPI_SPI_Transaction*)", "PFv"), $$UNDEF, "w");
                po.addFld("transaction", new Proto.Adr("ti_drivers_SPI_SPI_Transaction*", "PS"), $$UNDEF, "w");
        // struct SPI.SPIEUSCIBDMA_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIEUSCIBDMA_HWAttrs", "ti.drivers");
        po.init("ti.drivers.SPI.SPIEUSCIBDMA_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("clockSource", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("BitOrder", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("dmaBaseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("rxDMAChannelIndex", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("rxDMASourceTrigger", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("txDMAChannelIndex", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("txDMASourceTrigger", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct SPI.SPIEUSCIBDMA_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIEUSCIBDMA_Object", "ti.drivers");
        po.init("ti.drivers.SPI.SPIEUSCIBDMA_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
                po.addFld("transferComplete", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("transferMode", (Proto)om.findStrict("ti.drivers.SPI.TransferMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("transferCallbackFxn", new Proto.Adr("xdc_Void(*)(ti_drivers_SPI_SPI_Config*,ti_drivers_SPI_SPI_Transaction*)", "PFv"), $$UNDEF, "w");
                po.addFld("transaction", new Proto.Adr("ti_drivers_SPI_SPI_Transaction*", "PS"), $$UNDEF, "w");
        // struct SPI.SPIMSP432DMA_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIMSP432DMA_HWAttrs", "ti.drivers");
        po.init("ti.drivers.SPI.SPIMSP432DMA_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("bitOrder", Proto.Elm.newCNum("(xdc_Bits16)"), $$UNDEF, "w");
                po.addFld("clockSource", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("scratchBufPtr", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("defaultTxBufValue", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("dmaIntNum", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("rxDMAChannelIndex", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("txDMAChannelIndex", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
        // struct SPI.SPIMSP432DMA_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIMSP432DMA_Object", "ti.drivers");
        po.init("ti.drivers.SPI.SPIMSP432DMA_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("transferComplete", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("transferMode", (Proto)om.findStrict("ti.drivers.SPI.TransferMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("transferCallbackFxn", new Proto.Adr("xdc_Void(*)(ti_drivers_SPI_SPI_Config*,ti_drivers_SPI_SPI_Transaction*)", "PFv"), $$UNDEF, "w");
                po.addFld("transaction", new Proto.Adr("ti_drivers_SPI_SPI_Transaction*", "PS"), $$UNDEF, "w");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
        // struct SPI.BasicView
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$BasicView", "ti.drivers");
        po.init("ti.drivers.SPI.BasicView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("spiHandle", $$T_Str, $$UNDEF, "w");
                po.addFld("baseAddr", $$T_Str, $$UNDEF, "w");
                po.addFld("functionTable", $$T_Str, $$UNDEF, "w");
    }

    void UART$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/drivers/UART.xs");
        om.bind("ti.drivers.UART$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.drivers.UART.Module", "ti.drivers");
        po.init("ti.drivers.UART.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.drivers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("libType", (Proto)om.findStrict("ti.drivers.UART.LibType", "ti.drivers"), om.find("ti.drivers.UART.LibType_Instrumented"), "wh");
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.drivers"), $$UNDEF, "wh");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.drivers.UART$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.drivers.UART$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.drivers.UART$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.drivers.UART$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        // struct UART.UART_Config
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UART_Config", "ti.drivers");
        po.init("ti.drivers.UART.UART_Config", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("fxnTablePtr", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("object", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("hwAttrs", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
        // typedef UART.CallbackFxn
        om.bind("ti.drivers.UART.CallbackFxn", new Proto.Adr("xdc_Void(*)(xdc_Ptr,xdc_Int)", "PFv"));
        // struct UART.UARTTiva_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTTiva_HWAttrs", "ti.drivers");
        po.init("ti.drivers.UART.UARTTiva_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
        // struct UART.UARTTivaDMA_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTTivaDMA_HWAttrs", "ti.drivers");
        po.init("ti.drivers.UART.UARTTivaDMA_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("rxChannelIndex", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("txChannelIndex", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
        // struct UART.UARTTiva_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTTiva_Object", "ti.drivers");
        po.init("ti.drivers.UART.UARTTiva_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
                po.addFld("readMode", (Proto)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeMode", (Proto)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readTimeout", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("writeTimeout", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("readCallback", new Proto.Adr("xdc_Void(*)(xdc_Ptr,xdc_Int)", "PFv"), $$UNDEF, "w");
                po.addFld("writeCallback", new Proto.Adr("xdc_Void(*)(xdc_Ptr,xdc_Int)", "PFv"), $$UNDEF, "w");
                po.addFld("readReturnMode", (Proto)om.findStrict("ti.drivers.UART.UART_ReturnMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readDataMode", (Proto)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeDataMode", (Proto)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readEcho", (Proto)om.findStrict("ti.drivers.UART.UART_Echo", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeBuf", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("writeCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("writeSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("writeCR", $$T_Bool, $$UNDEF, "w");
                po.addFld("readBuf", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("readCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("readSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
        // struct UART.UARTCC3200_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTCC3200_HWAttrs", "ti.drivers");
        po.init("ti.drivers.UART.UARTCC3200_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
        // struct UART.UARTCC3200DMA_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTCC3200DMA_HWAttrs", "ti.drivers");
        po.init("ti.drivers.UART.UARTCC3200DMA_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("rxChannelIndex", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("txChannelIndex", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
        // struct UART.UARTCC3200_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTCC3200_Object", "ti.drivers");
        po.init("ti.drivers.UART.UARTCC3200_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
                po.addFld("readMode", (Proto)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeMode", (Proto)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readTimeout", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("writeTimeout", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("readCallback", new Proto.Adr("xdc_Void(*)(xdc_Ptr,xdc_Int)", "PFv"), $$UNDEF, "w");
                po.addFld("writeCallback", new Proto.Adr("xdc_Void(*)(xdc_Ptr,xdc_Int)", "PFv"), $$UNDEF, "w");
                po.addFld("readReturnMode", (Proto)om.findStrict("ti.drivers.UART.UART_ReturnMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readDataMode", (Proto)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeDataMode", (Proto)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readEcho", (Proto)om.findStrict("ti.drivers.UART.UART_Echo", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeBuf", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("writeCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("writeSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("writeCR", $$T_Bool, $$UNDEF, "w");
                po.addFld("readBuf", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("readCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("readSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
        // struct UART.UARTTivaDMA_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTTivaDMA_Object", "ti.drivers");
        po.init("ti.drivers.UART.UARTTivaDMA_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("opened", $$T_Bool, $$UNDEF, "w");
                po.addFld("readMode", (Proto)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeMode", (Proto)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readTimeout", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("writeTimeout", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("readCallback", new Proto.Adr("xdc_Void(*)(xdc_Ptr,xdc_Int)", "PFv"), $$UNDEF, "w");
                po.addFld("writeCallback", new Proto.Adr("xdc_Void(*)(xdc_Ptr,xdc_Int)", "PFv"), $$UNDEF, "w");
                po.addFld("readReturnMode", (Proto)om.findStrict("ti.drivers.UART.UART_ReturnMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readDataMode", (Proto)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeDataMode", (Proto)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readEcho", (Proto)om.findStrict("ti.drivers.UART.UART_Echo", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeBuf", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("writeCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("writeSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("readBuf", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("readCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("readSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
        // struct UART.UARTCC3200DMA_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTCC3200DMA_Object", "ti.drivers");
        po.init("ti.drivers.UART.UARTCC3200DMA_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("opened", $$T_Bool, $$UNDEF, "w");
                po.addFld("readMode", (Proto)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeMode", (Proto)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readTimeout", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("writeTimeout", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("readCallback", new Proto.Adr("xdc_Void(*)(xdc_Ptr,xdc_Int)", "PFv"), $$UNDEF, "w");
                po.addFld("writeCallback", new Proto.Adr("xdc_Void(*)(xdc_Ptr,xdc_Int)", "PFv"), $$UNDEF, "w");
                po.addFld("readReturnMode", (Proto)om.findStrict("ti.drivers.UART.UART_ReturnMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readDataMode", (Proto)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeDataMode", (Proto)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readEcho", (Proto)om.findStrict("ti.drivers.UART.UART_Echo", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeBuf", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("writeCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("writeSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("readBuf", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("readCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("readSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
        // struct UART.UARTUSCIA_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTUSCIA_HWAttrs", "ti.drivers");
        po.init("ti.drivers.UART.UARTUSCIA_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("clockSouce", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("bitOrder", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("numBaudrateEntries", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
        // struct UART.UARTUSCIA_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTUSCIA_Object", "ti.drivers");
        po.init("ti.drivers.UART.UARTUSCIA_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
                po.addFld("readMode", (Proto)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeMode", (Proto)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readTimeout", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("writeTimeout", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("readCallback", new Proto.Adr("xdc_Void(*)(xdc_Ptr,xdc_Int)", "PFv"), $$UNDEF, "w");
                po.addFld("writeCallback", new Proto.Adr("xdc_Void(*)(xdc_Ptr,xdc_Int)", "PFv"), $$UNDEF, "w");
                po.addFld("readReturnMode", (Proto)om.findStrict("ti.drivers.UART.UART_ReturnMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readDataMode", (Proto)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeDataMode", (Proto)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readEcho", (Proto)om.findStrict("ti.drivers.UART.UART_Echo", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeBuf", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("writeCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("writeSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("writeCR", $$T_Bool, $$UNDEF, "w");
                po.addFld("readBuf", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("readCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("readSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
        // struct UART.UARTEUSCIA_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTEUSCIA_HWAttrs", "ti.drivers");
        po.init("ti.drivers.UART.UARTEUSCIA_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("clockSouce", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("bitOrder", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("numBaudrateEntries", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
        // struct UART.UARTEUSCIA_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTEUSCIA_Object", "ti.drivers");
        po.init("ti.drivers.UART.UARTEUSCIA_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
                po.addFld("readMode", (Proto)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeMode", (Proto)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readTimeout", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("writeTimeout", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("readCallback", new Proto.Adr("xdc_Void(*)(xdc_Ptr,xdc_Int)", "PFv"), $$UNDEF, "w");
                po.addFld("writeCallback", new Proto.Adr("xdc_Void(*)(xdc_Ptr,xdc_Int)", "PFv"), $$UNDEF, "w");
                po.addFld("readReturnMode", (Proto)om.findStrict("ti.drivers.UART.UART_ReturnMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readDataMode", (Proto)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeDataMode", (Proto)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readEcho", (Proto)om.findStrict("ti.drivers.UART.UART_Echo", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeBuf", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("writeCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("writeSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("writeCR", $$T_Bool, $$UNDEF, "w");
                po.addFld("readBuf", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("readCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("readSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
        // struct UART.UARTMSP432_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTMSP432_HWAttrs", "ti.drivers");
        po.init("ti.drivers.UART.UARTMSP432_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("clockSouce", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("bitOrder", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("numBaudrateEntries", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct UART.UARTMSP432_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTMSP432_Object", "ti.drivers");
        po.init("ti.drivers.UART.UARTMSP432_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("readMode", (Proto)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeMode", (Proto)om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readTimeout", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("writeTimeout", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("readCallback", new Proto.Adr("xdc_Void(*)(xdc_Ptr,xdc_Int)", "PFv"), $$UNDEF, "w");
                po.addFld("writeCallback", new Proto.Adr("xdc_Void(*)(xdc_Ptr,xdc_Int)", "PFv"), $$UNDEF, "w");
                po.addFld("readReturnMode", (Proto)om.findStrict("ti.drivers.UART.UART_ReturnMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readDataMode", (Proto)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeDataMode", (Proto)om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"), $$UNDEF, "w");
                po.addFld("readEcho", (Proto)om.findStrict("ti.drivers.UART.UART_Echo", "ti.drivers"), $$UNDEF, "w");
                po.addFld("writeBuf", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("writeCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("writeSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("writeCR", $$T_Bool, $$UNDEF, "w");
                po.addFld("readBuf", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("readCount", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("readSize", Proto.Elm.newCNum("(xdc_SizeT)"), $$UNDEF, "w");
                po.addFld("writeSem", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("readSem", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
        // struct UART.BasicView
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$BasicView", "ti.drivers");
        po.init("ti.drivers.UART.BasicView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("uartHandle", $$T_Str, $$UNDEF, "w");
                po.addFld("baseAddr", $$T_Str, $$UNDEF, "w");
                po.addFld("functionTable", $$T_Str, $$UNDEF, "w");
        // struct UART.ConfigView
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$ConfigView", "ti.drivers");
        po.init("ti.drivers.UART.ConfigView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("baseAddr", $$T_Str, $$UNDEF, "w");
                po.addFld("writeMode", $$T_Str, $$UNDEF, "w");
                po.addFld("readMode", $$T_Str, $$UNDEF, "w");
                po.addFld("writeTimeout", $$T_Str, $$UNDEF, "w");
                po.addFld("readTimeout", $$T_Str, $$UNDEF, "w");
                po.addFld("readReturnMode", $$T_Str, $$UNDEF, "w");
                po.addFld("readDataMode", $$T_Str, $$UNDEF, "w");
                po.addFld("writeDataMode", $$T_Str, $$UNDEF, "w");
                po.addFld("readEcho", $$T_Str, $$UNDEF, "w");
        // struct UART.BufferView
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$BufferView", "ti.drivers");
        po.init("ti.drivers.UART.BufferView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("Base", $$T_Str, $$UNDEF, "w");
                po.addFld("Format", $$T_Str, $$UNDEF, "w");
                po.addFld("Contents", $$T_Str, $$UNDEF, "w");
    }

    void USBMSCHFatFs$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/drivers/USBMSCHFatFs.xs");
        om.bind("ti.drivers.USBMSCHFatFs$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.drivers.USBMSCHFatFs.Module", "ti.drivers");
        po.init("ti.drivers.USBMSCHFatFs.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.drivers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("libType", (Proto)om.findStrict("ti.drivers.USBMSCHFatFs.LibType", "ti.drivers"), om.find("ti.drivers.USBMSCHFatFs.LibType_Instrumented"), "wh");
            po.addFld("maxPeripheralCount", Proto.Elm.newCNum("(xdc_UInt)"), 32L, "wh");
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.drivers"), $$UNDEF, "wh");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.drivers.USBMSCHFatFs$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.drivers.USBMSCHFatFs$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.drivers.USBMSCHFatFs$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.drivers.USBMSCHFatFs$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        // struct USBMSCHFatFs.USBMSCHFatFs_Config
        po = (Proto.Obj)om.findStrict("ti.drivers.USBMSCHFatFs$$USBMSCHFatFs_Config", "ti.drivers");
        po.init("ti.drivers.USBMSCHFatFs.USBMSCHFatFs_Config", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("fxnTablePtr", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("object", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("hwAttrs", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
        // struct USBMSCHFatFs.USBMSCHFatFsTiva_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.USBMSCHFatFs$$USBMSCHFatFsTiva_HWAttrs", "ti.drivers");
        po.init("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("portSPI", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("pinSCK", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("pinMISO", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("pinMOSI", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("portCS", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("pinCS", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("portTX", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("pinTX", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
        // struct USBMSCHFatFs.USBMSCHFatFsTiva_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.USBMSCHFatFs$$USBMSCHFatFsTiva_Object", "ti.drivers");
        po.init("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("driveNumber", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("state", (Proto)om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_USBState", "ti.drivers"), $$UNDEF, "w");
                po.addFld("hwi", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("taskHCDMain", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("gateUSBWait", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("gateUSBLibAccess", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("semUSBConnected", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("MSCInstance", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        // struct USBMSCHFatFs.BasicView
        po = (Proto.Obj)om.findStrict("ti.drivers.USBMSCHFatFs$$BasicView", "ti.drivers");
        po.init("ti.drivers.USBMSCHFatFs.BasicView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("usbmschfatfsHandle", $$T_Str, $$UNDEF, "w");
                po.addFld("baseAddr", $$T_Str, $$UNDEF, "w");
                po.addFld("functionTable", $$T_Str, $$UNDEF, "w");
    }

    void Watchdog$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/drivers/Watchdog.xs");
        om.bind("ti.drivers.Watchdog$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog.Module", "ti.drivers");
        po.init("ti.drivers.Watchdog.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.drivers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("libType", (Proto)om.findStrict("ti.drivers.Watchdog.LibType", "ti.drivers"), om.find("ti.drivers.Watchdog.LibType_Instrumented"), "wh");
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.drivers"), $$UNDEF, "wh");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.drivers.Watchdog$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.drivers.Watchdog$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.drivers.Watchdog$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.drivers.Watchdog$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        // struct Watchdog.WatchdogTiva_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogTiva_HWAttrs", "ti.drivers");
        po.init("ti.drivers.Watchdog.WatchdogTiva_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("reloadValue", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
        // struct Watchdog.WatchdogTiva_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogTiva_Object", "ti.drivers");
        po.init("ti.drivers.Watchdog.WatchdogTiva_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
        // struct Watchdog.WatchdogCC3200_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogCC3200_HWAttrs", "ti.drivers");
        po.init("ti.drivers.Watchdog.WatchdogCC3200_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("reloadValue", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
        // struct Watchdog.WatchdogCC3200_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogCC3200_Object", "ti.drivers");
        po.init("ti.drivers.Watchdog.WatchdogCC3200_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
        // struct Watchdog.WatchdogMSP430_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogMSP430_HWAttrs", "ti.drivers");
        po.init("ti.drivers.Watchdog.WatchdogMSP430_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("sfrAddr", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("clockSource", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
                po.addFld("clockDivider", Proto.Elm.newCNum("(xdc_UInt)"), $$UNDEF, "w");
        // struct Watchdog.WatchdogMSP430_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogMSP430_Object", "ti.drivers");
        po.init("ti.drivers.Watchdog.WatchdogMSP430_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
        // struct Watchdog.WatchdogMSP432_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogMSP432_HWAttrs", "ti.drivers");
        po.init("ti.drivers.Watchdog.WatchdogMSP432_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("baseAddr", Proto.Elm.newCNum("(xdc_Bits32)"), $$UNDEF, "w");
                po.addFld("intNum", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("clockSource", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
                po.addFld("clockDivider", Proto.Elm.newCNum("(xdc_Bits8)"), $$UNDEF, "w");
        // struct Watchdog.WatchdogMSP432_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogMSP432_Object", "ti.drivers");
        po.init("ti.drivers.Watchdog.WatchdogMSP432_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
        // struct Watchdog.Watchdog_Config
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$Watchdog_Config", "ti.drivers");
        po.init("ti.drivers.Watchdog.Watchdog_Config", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("fxnTablePtr", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("object", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("hwAttrs", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
        // struct Watchdog.BasicView
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$BasicView", "ti.drivers");
        po.init("ti.drivers.Watchdog.BasicView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("watchdogHandle", $$T_Str, $$UNDEF, "w");
                po.addFld("baseAddr", $$T_Str, $$UNDEF, "w");
                po.addFld("functionTable", $$T_Str, $$UNDEF, "w");
    }

    void WiFi$$TYPES()
    {
        Scriptable cap;
        Proto.Obj po;
        Proto.Str ps;
        Proto.Typedef pt;
        Object fxn;

        cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/drivers/WiFi.xs");
        om.bind("ti.drivers.WiFi$$capsule", cap);
        po = (Proto.Obj)om.findStrict("ti.drivers.WiFi.Module", "ti.drivers");
        po.init("ti.drivers.WiFi.Module", om.findStrict("xdc.runtime.IModule.Module", "ti.drivers"));
                po.addFld("$hostonly", $$T_Num, 0, "r");
        if (isCFG) {
            po.addFld("deviceType", (Proto)om.findStrict("ti.drivers.WiFi.DeviceType", "ti.drivers"), om.find("ti.drivers.WiFi.DeviceType_SimpleLink_CC3100"), "wh");
            po.addFld("libType", (Proto)om.findStrict("ti.drivers.WiFi.LibType", "ti.drivers"), om.find("ti.drivers.WiFi.LibType_Instrumented"), "wh");
            po.addFld("spawnTaskPriority", Proto.Elm.newCNum("(xdc_Int)"), 3L, "wh");
            po.addFld("rovViewInfo", (Proto)om.findStrict("xdc.rov.ViewInfo.Instance", "ti.drivers"), $$UNDEF, "wh");
        }//isCFG
        fxn = Global.get(cap, "module$use");
        if (fxn != null) om.bind("ti.drivers.WiFi$$module$use", true);
        if (fxn != null) po.addFxn("module$use", $$T_Met, fxn);
        fxn = Global.get(cap, "module$meta$init");
        if (fxn != null) om.bind("ti.drivers.WiFi$$module$meta$init", true);
        if (fxn != null) po.addFxn("module$meta$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$static$init");
        if (fxn != null) om.bind("ti.drivers.WiFi$$module$static$init", true);
        if (fxn != null) po.addFxn("module$static$init", $$T_Met, fxn);
        fxn = Global.get(cap, "module$validate");
        if (fxn != null) om.bind("ti.drivers.WiFi$$module$validate", true);
        if (fxn != null) po.addFxn("module$validate", $$T_Met, fxn);
        // struct WiFi.WiFi_Config
        po = (Proto.Obj)om.findStrict("ti.drivers.WiFi$$WiFi_Config", "ti.drivers");
        po.init("ti.drivers.WiFi.WiFi_Config", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("fxnTablePtr", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("object", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
                po.addFld("hwAttrs", new Proto.Adr("xdc_Void*", "Pv"), $$UNDEF, "w");
        // struct WiFi.WiFiCC3100_HWAttrs
        po = (Proto.Obj)om.findStrict("ti.drivers.WiFi$$WiFiCC3100_HWAttrs", "ti.drivers");
        po.init("ti.drivers.WiFi.WiFiCC3100_HWAttrs", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("irqPort", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("irqPin", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("irqIntNum", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("csPort", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("csPin", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("enPort", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
                po.addFld("enPin", Proto.Elm.newCNum("(xdc_ULong)"), $$UNDEF, "w");
        // struct WiFi.WiFiCC3100_Object
        po = (Proto.Obj)om.findStrict("ti.drivers.WiFi$$WiFiCC3100_Object", "ti.drivers");
        po.init("ti.drivers.WiFi.WiFiCC3100_Object", null);
                po.addFld("$hostonly", $$T_Num, 0, "r");
                po.addFld("wifiHwi", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("spiHandle", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("wifiIntFxn", new Proto.Adr("xdc_Void(*)(xdc_Void)", "PFv"), $$UNDEF, "w");
                po.addFld("readSemaphore", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("writeSemaphore", new Proto.Adr("xdc_Ptr", "Pv"), $$UNDEF, "w");
                po.addFld("spiState", (Proto)om.findStrict("ti.drivers.WiFi.WiFiCC3100_SPI_State", "ti.drivers"), $$UNDEF, "w");
                po.addFld("isOpen", $$T_Bool, $$UNDEF, "w");
        // struct WiFi.BasicView
        po = (Proto.Obj)om.findStrict("ti.drivers.WiFi$$BasicView", "ti.drivers");
        po.init("ti.drivers.WiFi.BasicView", null);
                po.addFld("$hostonly", $$T_Num, 1, "r");
                po.addFld("functionTable", $$T_Str, $$UNDEF, "w");
                po.addFld("wifiHandle", $$T_Str, $$UNDEF, "w");
                po.addFld("spiHandle", $$T_Str, $$UNDEF, "w");
                po.addFld("spiState", $$T_Str, $$UNDEF, "w");
    }

    void EMAC$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.EMAC", "ti.drivers");
        vo.bind("EMAC_Config$fetchDesc", Global.newObject("type", "ti.drivers.EMAC.EMAC_Config", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC$$EMAC_Config", "ti.drivers");
        vo.bind("EMACTiva_Object$fetchDesc", Global.newObject("type", "ti.drivers.EMAC.EMACTiva_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC$$EMACTiva_Object", "ti.drivers");
        vo.bind("EMACTiva_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.EMAC.EMACTiva_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC$$EMACTiva_HWAttrs", "ti.drivers");
        vo.bind("EMACSnow_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.EMAC.EMACSnow_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC$$EMACSnow_HWAttrs", "ti.drivers");
        vo.bind("EMACSnow_Data$fetchDesc", Global.newObject("type", "ti.drivers.EMAC.EMACSnow_Data", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC$$EMACSnow_Data", "ti.drivers");
        vo.bind("EMACSnow_Object$fetchDesc", Global.newObject("type", "ti.drivers.EMAC.EMACSnow_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC$$EMACSnow_Object", "ti.drivers");
    }

    void GPIO$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.GPIO", "ti.drivers");
        vo.bind("GPIO_Config$fetchDesc", Global.newObject("type", "ti.drivers.GPIO.GPIO_Config", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.GPIO$$GPIO_Config", "ti.drivers");
    }

    void I2C$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.I2C", "ti.drivers");
        vo.bind("I2C_Config$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Config", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2C_Config", "ti.drivers");
        vo.bind("I2C_Transaction$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2C_Transaction", "ti.drivers");
        vo.bind("I2C_Params$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Params", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2C_Params", "ti.drivers");
        vo.bind("I2CTiva_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2CTiva_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CTiva_HWAttrs", "ti.drivers");
        vo.bind("I2CTiva_Object$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2CTiva_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CTiva_Object", "ti.drivers");
        po.bind("currentTransaction$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
        po.bind("headPtr$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
        po.bind("tailPtr$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
        vo.bind("I2CCC3200_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2CCC3200_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CCC3200_HWAttrs", "ti.drivers");
        vo.bind("I2CCC3200_Object$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2CCC3200_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CCC3200_Object", "ti.drivers");
        po.bind("currentTransaction$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
        po.bind("headPtr$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
        po.bind("tailPtr$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
        vo.bind("I2CMSP432_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2CMSP432_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CMSP432_HWAttrs", "ti.drivers");
        vo.bind("I2CMSP432_Object$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2CMSP432_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CMSP432_Object", "ti.drivers");
        po.bind("currentTransaction$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
        po.bind("headPtr$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
        po.bind("tailPtr$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
        vo.bind("I2CUSCIB_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2CUSCIB_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CUSCIB_HWAttrs", "ti.drivers");
        vo.bind("I2CEUSCIB_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2CEUSCIB_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CEUSCIB_HWAttrs", "ti.drivers");
        vo.bind("I2CUSCIB_Object$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2CUSCIB_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CUSCIB_Object", "ti.drivers");
        po.bind("currentTransaction$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
        po.bind("headPtr$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
        po.bind("tailPtr$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
        vo.bind("I2CEUSCIB_Object$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2CEUSCIB_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C$$I2CEUSCIB_Object", "ti.drivers");
        po.bind("currentTransaction$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
        po.bind("headPtr$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
        po.bind("tailPtr$fetchDesc", Global.newObject("type", "ti.drivers.I2C.I2C_Transaction", "isScalar", false));
    }

    void Power$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.Power", "ti.drivers");
    }

    void PWM$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.PWM", "ti.drivers");
        vo.bind("PWM_Config$fetchDesc", Global.newObject("type", "ti.drivers.PWM.PWM_Config", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWM_Config", "ti.drivers");
        vo.bind("PWM_Params$fetchDesc", Global.newObject("type", "ti.drivers.PWM.PWM_Params", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWM_Params", "ti.drivers");
        vo.bind("PWMTimerCC3200_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.PWM.PWMTimerCC3200_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTimerCC3200_HWAttrs", "ti.drivers");
        vo.bind("PWMTimerCC3200_Object$fetchDesc", Global.newObject("type", "ti.drivers.PWM.PWMTimerCC3200_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTimerCC3200_Object", "ti.drivers");
        vo.bind("PWMTiva_Status$fetchDesc", Global.newObject("type", "ti.drivers.PWM.PWMTiva_Status", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTiva_Status", "ti.drivers");
        vo.bind("PWMTiva_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.PWM.PWMTiva_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTiva_HWAttrs", "ti.drivers");
        vo.bind("PWMTiva_Object$fetchDesc", Global.newObject("type", "ti.drivers.PWM.PWMTiva_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTiva_Object", "ti.drivers");
        vo.bind("PWMTimerTiva_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.PWM.PWMTimerTiva_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTimerTiva_HWAttrs", "ti.drivers");
        vo.bind("PWMTimerTiva_Object$fetchDesc", Global.newObject("type", "ti.drivers.PWM.PWMTimerTiva_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTimerTiva_Object", "ti.drivers");
        vo.bind("PWMTimerMSP432_Status$fetchDesc", Global.newObject("type", "ti.drivers.PWM.PWMTimerMSP432_Status", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTimerMSP432_Status", "ti.drivers");
        vo.bind("PWMTimerMSP432_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.PWM.PWMTimerMSP432_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTimerMSP432_HWAttrs", "ti.drivers");
        vo.bind("PWMTimerMSP432_Object$fetchDesc", Global.newObject("type", "ti.drivers.PWM.PWMTimerMSP432_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM$$PWMTimerMSP432_Object", "ti.drivers");
    }

    void SDSPI$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.SDSPI", "ti.drivers");
        vo.bind("SDSPI_Config$fetchDesc", Global.newObject("type", "ti.drivers.SDSPI.SDSPI_Config", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPI_Config", "ti.drivers");
        vo.bind("SDSPITiva_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.SDSPI.SDSPITiva_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPITiva_HWAttrs", "ti.drivers");
        vo.bind("SDSPITiva_Object$fetchDesc", Global.newObject("type", "ti.drivers.SDSPI.SDSPITiva_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPITiva_Object", "ti.drivers");
        vo.bind("SDSPIUSCIA_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.SDSPI.SDSPIUSCIA_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIUSCIA_HWAttrs", "ti.drivers");
        vo.bind("SDSPIUSCIB_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.SDSPI.SDSPIUSCIB_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIUSCIB_HWAttrs", "ti.drivers");
        vo.bind("SDSPIUSCIA_Object$fetchDesc", Global.newObject("type", "ti.drivers.SDSPI.SDSPIUSCIA_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIUSCIA_Object", "ti.drivers");
        vo.bind("SDSPIUSCIB_Object$fetchDesc", Global.newObject("type", "ti.drivers.SDSPI.SDSPIUSCIB_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIUSCIB_Object", "ti.drivers");
        vo.bind("SDSPIEUSCIA_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.SDSPI.SDSPIEUSCIA_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIEUSCIA_HWAttrs", "ti.drivers");
        vo.bind("SDSPIEUSCIB_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.SDSPI.SDSPIEUSCIB_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIEUSCIB_HWAttrs", "ti.drivers");
        vo.bind("SDSPIEUSCIA_Object$fetchDesc", Global.newObject("type", "ti.drivers.SDSPI.SDSPIEUSCIA_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIEUSCIA_Object", "ti.drivers");
        vo.bind("SDSPIEUSCIB_Object$fetchDesc", Global.newObject("type", "ti.drivers.SDSPI.SDSPIEUSCIB_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIEUSCIB_Object", "ti.drivers");
        vo.bind("SDSPICC3200_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.SDSPI.SDSPICC3200_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPICC3200_HWAttrs", "ti.drivers");
        vo.bind("SDSPICC3200_Object$fetchDesc", Global.newObject("type", "ti.drivers.SDSPI.SDSPICC3200_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPICC3200_Object", "ti.drivers");
        vo.bind("SDSPIMSP432_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.SDSPI.SDSPIMSP432_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIMSP432_HWAttrs", "ti.drivers");
        vo.bind("SDSPIMSP432_Object$fetchDesc", Global.newObject("type", "ti.drivers.SDSPI.SDSPIMSP432_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI$$SDSPIMSP432_Object", "ti.drivers");
    }

    void SPI$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.SPI", "ti.drivers");
        vo.bind("SPI_Config$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPI_Config", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPI_Config", "ti.drivers");
        vo.bind("SPI_Transaction$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPI_Transaction", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPI_Transaction", "ti.drivers");
        vo.bind("SPITivaDMA_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPITivaDMA_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPITivaDMA_HWAttrs", "ti.drivers");
        vo.bind("SPITivaDMA_Object$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPITivaDMA_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPITivaDMA_Object", "ti.drivers");
        po.bind("transaction$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPI_Transaction", "isScalar", false));
        vo.bind("SPICC3200DMA_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPICC3200DMA_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPICC3200DMA_HWAttrs", "ti.drivers");
        vo.bind("SPICC3200DMA_Object$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPICC3200DMA_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPICC3200DMA_Object", "ti.drivers");
        po.bind("transaction$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPI_Transaction", "isScalar", false));
        vo.bind("SPIUSCIADMA_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPIUSCIADMA_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIUSCIADMA_HWAttrs", "ti.drivers");
        vo.bind("SPIUSCIADMA_Object$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPIUSCIADMA_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIUSCIADMA_Object", "ti.drivers");
        po.bind("transaction$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPI_Transaction", "isScalar", false));
        vo.bind("SPIUSCIBDMA_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPIUSCIBDMA_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIUSCIBDMA_HWAttrs", "ti.drivers");
        vo.bind("SPIUSCIBDMA_Object$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPIUSCIBDMA_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIUSCIBDMA_Object", "ti.drivers");
        po.bind("transaction$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPI_Transaction", "isScalar", false));
        vo.bind("SPIEUSCIADMA_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPIEUSCIADMA_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIEUSCIADMA_HWAttrs", "ti.drivers");
        vo.bind("SPIEUSCIADMA_Object$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPIEUSCIADMA_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIEUSCIADMA_Object", "ti.drivers");
        po.bind("transaction$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPI_Transaction", "isScalar", false));
        vo.bind("SPIEUSCIBDMA_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPIEUSCIBDMA_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIEUSCIBDMA_HWAttrs", "ti.drivers");
        vo.bind("SPIEUSCIBDMA_Object$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPIEUSCIBDMA_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIEUSCIBDMA_Object", "ti.drivers");
        po.bind("transaction$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPI_Transaction", "isScalar", false));
        vo.bind("SPIMSP432DMA_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPIMSP432DMA_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIMSP432DMA_HWAttrs", "ti.drivers");
        vo.bind("SPIMSP432DMA_Object$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPIMSP432DMA_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI$$SPIMSP432DMA_Object", "ti.drivers");
        po.bind("transaction$fetchDesc", Global.newObject("type", "ti.drivers.SPI.SPI_Transaction", "isScalar", false));
    }

    void UART$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.UART", "ti.drivers");
        vo.bind("UART_Config$fetchDesc", Global.newObject("type", "ti.drivers.UART.UART_Config", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UART_Config", "ti.drivers");
        vo.bind("UARTTiva_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.UART.UARTTiva_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTTiva_HWAttrs", "ti.drivers");
        vo.bind("UARTTivaDMA_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.UART.UARTTivaDMA_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTTivaDMA_HWAttrs", "ti.drivers");
        vo.bind("UARTTiva_Object$fetchDesc", Global.newObject("type", "ti.drivers.UART.UARTTiva_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTTiva_Object", "ti.drivers");
        vo.bind("UARTCC3200_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.UART.UARTCC3200_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTCC3200_HWAttrs", "ti.drivers");
        vo.bind("UARTCC3200DMA_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.UART.UARTCC3200DMA_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTCC3200DMA_HWAttrs", "ti.drivers");
        vo.bind("UARTCC3200_Object$fetchDesc", Global.newObject("type", "ti.drivers.UART.UARTCC3200_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTCC3200_Object", "ti.drivers");
        vo.bind("UARTTivaDMA_Object$fetchDesc", Global.newObject("type", "ti.drivers.UART.UARTTivaDMA_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTTivaDMA_Object", "ti.drivers");
        vo.bind("UARTCC3200DMA_Object$fetchDesc", Global.newObject("type", "ti.drivers.UART.UARTCC3200DMA_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTCC3200DMA_Object", "ti.drivers");
        vo.bind("UARTUSCIA_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.UART.UARTUSCIA_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTUSCIA_HWAttrs", "ti.drivers");
        vo.bind("UARTUSCIA_Object$fetchDesc", Global.newObject("type", "ti.drivers.UART.UARTUSCIA_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTUSCIA_Object", "ti.drivers");
        vo.bind("UARTEUSCIA_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.UART.UARTEUSCIA_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTEUSCIA_HWAttrs", "ti.drivers");
        vo.bind("UARTEUSCIA_Object$fetchDesc", Global.newObject("type", "ti.drivers.UART.UARTEUSCIA_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTEUSCIA_Object", "ti.drivers");
        vo.bind("UARTMSP432_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.UART.UARTMSP432_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTMSP432_HWAttrs", "ti.drivers");
        vo.bind("UARTMSP432_Object$fetchDesc", Global.newObject("type", "ti.drivers.UART.UARTMSP432_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.UART$$UARTMSP432_Object", "ti.drivers");
    }

    void USBMSCHFatFs$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.USBMSCHFatFs", "ti.drivers");
        vo.bind("USBMSCHFatFs_Config$fetchDesc", Global.newObject("type", "ti.drivers.USBMSCHFatFs.USBMSCHFatFs_Config", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.USBMSCHFatFs$$USBMSCHFatFs_Config", "ti.drivers");
        vo.bind("USBMSCHFatFsTiva_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.USBMSCHFatFs$$USBMSCHFatFsTiva_HWAttrs", "ti.drivers");
        vo.bind("USBMSCHFatFsTiva_Object$fetchDesc", Global.newObject("type", "ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.USBMSCHFatFs$$USBMSCHFatFsTiva_Object", "ti.drivers");
    }

    void Watchdog$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.Watchdog", "ti.drivers");
        vo.bind("WatchdogTiva_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.Watchdog.WatchdogTiva_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogTiva_HWAttrs", "ti.drivers");
        vo.bind("WatchdogTiva_Object$fetchDesc", Global.newObject("type", "ti.drivers.Watchdog.WatchdogTiva_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogTiva_Object", "ti.drivers");
        vo.bind("WatchdogCC3200_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.Watchdog.WatchdogCC3200_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogCC3200_HWAttrs", "ti.drivers");
        vo.bind("WatchdogCC3200_Object$fetchDesc", Global.newObject("type", "ti.drivers.Watchdog.WatchdogCC3200_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogCC3200_Object", "ti.drivers");
        vo.bind("WatchdogMSP430_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.Watchdog.WatchdogMSP430_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogMSP430_HWAttrs", "ti.drivers");
        vo.bind("WatchdogMSP430_Object$fetchDesc", Global.newObject("type", "ti.drivers.Watchdog.WatchdogMSP430_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogMSP430_Object", "ti.drivers");
        vo.bind("WatchdogMSP432_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.Watchdog.WatchdogMSP432_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogMSP432_HWAttrs", "ti.drivers");
        vo.bind("WatchdogMSP432_Object$fetchDesc", Global.newObject("type", "ti.drivers.Watchdog.WatchdogMSP432_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$WatchdogMSP432_Object", "ti.drivers");
        vo.bind("Watchdog_Config$fetchDesc", Global.newObject("type", "ti.drivers.Watchdog.Watchdog_Config", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog$$Watchdog_Config", "ti.drivers");
    }

    void WiFi$$ROV()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.WiFi", "ti.drivers");
        vo.bind("WiFi_Config$fetchDesc", Global.newObject("type", "ti.drivers.WiFi.WiFi_Config", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.WiFi$$WiFi_Config", "ti.drivers");
        vo.bind("WiFiCC3100_HWAttrs$fetchDesc", Global.newObject("type", "ti.drivers.WiFi.WiFiCC3100_HWAttrs", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.WiFi$$WiFiCC3100_HWAttrs", "ti.drivers");
        vo.bind("WiFiCC3100_Object$fetchDesc", Global.newObject("type", "ti.drivers.WiFi.WiFiCC3100_Object", "isScalar", false));
        po = (Proto.Obj)om.findStrict("ti.drivers.WiFi$$WiFiCC3100_Object", "ti.drivers");
    }

    void $$SINGLETONS()
    {
        pkgP.init("ti.drivers.Package", (Proto.Obj)om.findStrict("xdc.IPackage.Module", "ti.drivers"));
        Scriptable cap = (Scriptable)Global.callFxn("loadCapsule", xdcO, "ti/drivers/package.xs");
        om.bind("xdc.IPackage$$capsule", cap);
        Object fxn;
                fxn = Global.get(cap, "init");
                if (fxn != null) pkgP.addFxn("init", (Proto.Fxn)om.findStrict("xdc.IPackage$$init", "ti.drivers"), fxn);
                fxn = Global.get(cap, "close");
                if (fxn != null) pkgP.addFxn("close", (Proto.Fxn)om.findStrict("xdc.IPackage$$close", "ti.drivers"), fxn);
                fxn = Global.get(cap, "validate");
                if (fxn != null) pkgP.addFxn("validate", (Proto.Fxn)om.findStrict("xdc.IPackage$$validate", "ti.drivers"), fxn);
                fxn = Global.get(cap, "exit");
                if (fxn != null) pkgP.addFxn("exit", (Proto.Fxn)om.findStrict("xdc.IPackage$$exit", "ti.drivers"), fxn);
                fxn = Global.get(cap, "getLibs");
                if (fxn != null) pkgP.addFxn("getLibs", (Proto.Fxn)om.findStrict("xdc.IPackage$$getLibs", "ti.drivers"), fxn);
                fxn = Global.get(cap, "getSects");
                if (fxn != null) pkgP.addFxn("getSects", (Proto.Fxn)om.findStrict("xdc.IPackage$$getSects", "ti.drivers"), fxn);
        pkgP.bind("$capsule", cap);
        pkgV.init2(pkgP, "ti.drivers", Value.DEFAULT, false);
        pkgV.bind("$name", "ti.drivers");
        pkgV.bind("$category", "Package");
        pkgV.bind("$$qn", "ti.drivers.");
        pkgV.bind("$vers", Global.newArray(1, 0, 0));
        Value.Map atmap = (Value.Map)pkgV.getv("$attr");
        atmap.seal("length");
        imports.clear();
        pkgV.bind("$imports", imports);
        StringBuilder sb = new StringBuilder();
        sb.append("var pkg = xdc.om['ti.drivers'];\n");
        sb.append("if (pkg.$vers.length >= 3) {\n");
            sb.append("pkg.$vers.push(Packages.xdc.services.global.Vers.getDate(xdc.csd() + '/..'));\n");
        sb.append("}\n");
        sb.append("if ('ti.drivers$$stat$base' in xdc.om) {\n");
            sb.append("pkg.packageBase = xdc.om['ti.drivers$$stat$base'];\n");
            sb.append("pkg.packageRepository = xdc.om['ti.drivers$$stat$root'];\n");
        sb.append("}\n");
        sb.append("pkg.build.libraries = [\n");
            sb.append("'lib/drivers_ccware.aem4',\n");
            sb.append("'lib/drivers_ccware_instrumented.aem4',\n");
            sb.append("'lib/drivers_ccware.am4g',\n");
            sb.append("'lib/drivers_ccware_instrumented.am4g',\n");
            sb.append("'lib/drivers_ccware.arm4',\n");
            sb.append("'lib/drivers_ccware_instrumented.arm4',\n");
            sb.append("'lib/power_cc3200_tirtos.aem4',\n");
            sb.append("'lib/power_cc3200_tirtos_instrumented.aem4',\n");
            sb.append("'lib/power_cc3200_tirtos.am4g',\n");
            sb.append("'lib/power_cc3200_tirtos_instrumented.am4g',\n");
            sb.append("'lib/power_cc3200_tirtos.arm4',\n");
            sb.append("'lib/power_cc3200_tirtos_instrumented.arm4',\n");
            sb.append("'lib/drivers_cc26xxware.aem3',\n");
            sb.append("'lib/drivers_cc26xxware_instrumented.aem3',\n");
            sb.append("'lib/drivers_cc26xxware.arm3',\n");
            sb.append("'lib/drivers_cc26xxware_instrumented.arm3',\n");
            sb.append("'lib/drivers_cc13xxware.aem3',\n");
            sb.append("'lib/drivers_cc13xxware_instrumented.aem3',\n");
            sb.append("'lib/drivers_cc13xxware.arm3',\n");
            sb.append("'lib/drivers_cc13xxware_instrumented.arm3',\n");
        sb.append("];\n");
        sb.append("pkg.build.libDesc = [\n");
            sb.append("['lib/drivers_ccware.aem4', {target: 'ti.targets.arm.elf.M4', suffix: 'em4'}],\n");
            sb.append("['lib/drivers_ccware_instrumented.aem4', {target: 'ti.targets.arm.elf.M4', suffix: 'em4'}],\n");
            sb.append("['lib/drivers_ccware.am4g', {target: 'gnu.targets.arm.M4', suffix: 'm4g'}],\n");
            sb.append("['lib/drivers_ccware_instrumented.am4g', {target: 'gnu.targets.arm.M4', suffix: 'm4g'}],\n");
            sb.append("['lib/drivers_ccware.arm4', {target: 'iar.targets.arm.M4', suffix: 'rm4'}],\n");
            sb.append("['lib/drivers_ccware_instrumented.arm4', {target: 'iar.targets.arm.M4', suffix: 'rm4'}],\n");
            sb.append("['lib/power_cc3200_tirtos.aem4', {target: 'ti.targets.arm.elf.M4', suffix: 'em4'}],\n");
            sb.append("['lib/power_cc3200_tirtos_instrumented.aem4', {target: 'ti.targets.arm.elf.M4', suffix: 'em4'}],\n");
            sb.append("['lib/power_cc3200_tirtos.am4g', {target: 'gnu.targets.arm.M4', suffix: 'm4g'}],\n");
            sb.append("['lib/power_cc3200_tirtos_instrumented.am4g', {target: 'gnu.targets.arm.M4', suffix: 'm4g'}],\n");
            sb.append("['lib/power_cc3200_tirtos.arm4', {target: 'iar.targets.arm.M4', suffix: 'rm4'}],\n");
            sb.append("['lib/power_cc3200_tirtos_instrumented.arm4', {target: 'iar.targets.arm.M4', suffix: 'rm4'}],\n");
            sb.append("['lib/drivers_cc26xxware.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/drivers_cc26xxware_instrumented.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/drivers_cc26xxware.arm3', {target: 'iar.targets.arm.M3', suffix: 'rm3'}],\n");
            sb.append("['lib/drivers_cc26xxware_instrumented.arm3', {target: 'iar.targets.arm.M3', suffix: 'rm3'}],\n");
            sb.append("['lib/drivers_cc13xxware.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/drivers_cc13xxware_instrumented.aem3', {target: 'ti.targets.arm.elf.M3', suffix: 'em3'}],\n");
            sb.append("['lib/drivers_cc13xxware.arm3', {target: 'iar.targets.arm.M3', suffix: 'rm3'}],\n");
            sb.append("['lib/drivers_cc13xxware_instrumented.arm3', {target: 'iar.targets.arm.M3', suffix: 'rm3'}],\n");
        sb.append("];\n");
        Global.eval(sb.toString());
    }

    void EMAC$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.EMAC", "ti.drivers");
        po = (Proto.Obj)om.findStrict("ti.drivers.EMAC.Module", "ti.drivers");
        vo.init2(po, "ti.drivers.EMAC", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.drivers.EMAC$$capsule", "ti.drivers"));
        vo.bind("$package", om.findStrict("ti.drivers", "ti.drivers"));
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
        vo.bind("LibType", om.findStrict("ti.drivers.EMAC.LibType", "ti.drivers"));
        vo.bind("EMAC_Config", om.findStrict("ti.drivers.EMAC.EMAC_Config", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.EMAC.EMAC_Config", "ti.drivers"));
        vo.bind("EMACTiva_Object", om.findStrict("ti.drivers.EMAC.EMACTiva_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.EMAC.EMACTiva_Object", "ti.drivers"));
        vo.bind("EMACTiva_HWAttrs", om.findStrict("ti.drivers.EMAC.EMACTiva_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.EMAC.EMACTiva_HWAttrs", "ti.drivers"));
        vo.bind("EMACSnow_HWAttrs", om.findStrict("ti.drivers.EMAC.EMACSnow_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.EMAC.EMACSnow_HWAttrs", "ti.drivers"));
        vo.bind("EMACSnow_Data", om.findStrict("ti.drivers.EMAC.EMACSnow_Data", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.EMAC.EMACSnow_Data", "ti.drivers"));
        vo.bind("EMACSnow_Object", om.findStrict("ti.drivers.EMAC.EMACSnow_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.EMAC.EMACSnow_Object", "ti.drivers"));
        vo.bind("BasicView", om.findStrict("ti.drivers.EMAC.BasicView", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.EMAC.BasicView", "ti.drivers"));
        vo.bind("StatsView", om.findStrict("ti.drivers.EMAC.StatsView", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.EMAC.StatsView", "ti.drivers"));
        vo.bind("LibType_Instrumented", om.findStrict("ti.drivers.EMAC.LibType_Instrumented", "ti.drivers"));
        vo.bind("LibType_NonInstrumented", om.findStrict("ti.drivers.EMAC.LibType_NonInstrumented", "ti.drivers"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.drivers")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 1);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("$$fxntab", Global.newArray("ti_drivers_EMAC_Module__startupDone__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", "");
        atmap.seal("length");
        if (isCFG) {
            vo.put("common$", vo, Global.get((Proto.Obj)om.find("xdc.runtime.Defaults.Module"), "noRuntimeCommon$"));
            ((Value.Obj)vo.geto("common$")).seal(null);
        }//isCFG
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("EMAC", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("EMAC");
    }

    void GPIO$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.GPIO", "ti.drivers");
        po = (Proto.Obj)om.findStrict("ti.drivers.GPIO.Module", "ti.drivers");
        vo.init2(po, "ti.drivers.GPIO", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.drivers.GPIO$$capsule", "ti.drivers"));
        vo.bind("$package", om.findStrict("ti.drivers", "ti.drivers"));
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
        vo.bind("LibType", om.findStrict("ti.drivers.GPIO.LibType", "ti.drivers"));
        vo.bind("GPIO_PinConfig", om.findStrict("ti.drivers.GPIO.GPIO_PinConfig", "ti.drivers"));
        vo.bind("GPIO_CallbackFxn", om.findStrict("ti.drivers.GPIO.GPIO_CallbackFxn", "ti.drivers"));
        vo.bind("GPIO_Config", om.findStrict("ti.drivers.GPIO.GPIO_Config", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.GPIO.GPIO_Config", "ti.drivers"));
        vo.bind("BasicView", om.findStrict("ti.drivers.GPIO.BasicView", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.GPIO.BasicView", "ti.drivers"));
        vo.bind("LibType_Instrumented", om.findStrict("ti.drivers.GPIO.LibType_Instrumented", "ti.drivers"));
        vo.bind("LibType_NonInstrumented", om.findStrict("ti.drivers.GPIO.LibType_NonInstrumented", "ti.drivers"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.drivers")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 1);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("$$fxntab", Global.newArray("ti_drivers_GPIO_Module__startupDone__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", "");
        atmap.seal("length");
        if (isCFG) {
            vo.put("common$", vo, Global.get((Proto.Obj)om.find("xdc.runtime.Defaults.Module"), "noRuntimeCommon$"));
            ((Value.Obj)vo.geto("common$")).seal(null);
        }//isCFG
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("GPIO", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("GPIO");
    }

    void I2C$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.I2C", "ti.drivers");
        po = (Proto.Obj)om.findStrict("ti.drivers.I2C.Module", "ti.drivers");
        vo.init2(po, "ti.drivers.I2C", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.drivers.I2C$$capsule", "ti.drivers"));
        vo.bind("$package", om.findStrict("ti.drivers", "ti.drivers"));
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
        vo.bind("LibType", om.findStrict("ti.drivers.I2C.LibType", "ti.drivers"));
        vo.bind("I2C_Config", om.findStrict("ti.drivers.I2C.I2C_Config", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.I2C.I2C_Config", "ti.drivers"));
        vo.bind("CallbackFxn", om.findStrict("ti.drivers.I2C.CallbackFxn", "ti.drivers"));
        vo.bind("I2C_TransferMode", om.findStrict("ti.drivers.I2C.I2C_TransferMode", "ti.drivers"));
        vo.bind("I2C_Transaction", om.findStrict("ti.drivers.I2C.I2C_Transaction", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.I2C.I2C_Transaction", "ti.drivers"));
        vo.bind("I2C_Mode", om.findStrict("ti.drivers.I2C.I2C_Mode", "ti.drivers"));
        vo.bind("I2C_Params", om.findStrict("ti.drivers.I2C.I2C_Params", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.I2C.I2C_Params", "ti.drivers"));
        vo.bind("I2CTiva_HWAttrs", om.findStrict("ti.drivers.I2C.I2CTiva_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.I2C.I2CTiva_HWAttrs", "ti.drivers"));
        vo.bind("I2CTiva_Object", om.findStrict("ti.drivers.I2C.I2CTiva_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.I2C.I2CTiva_Object", "ti.drivers"));
        vo.bind("I2CCC3200_HWAttrs", om.findStrict("ti.drivers.I2C.I2CCC3200_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.I2C.I2CCC3200_HWAttrs", "ti.drivers"));
        vo.bind("I2CCC3200_Object", om.findStrict("ti.drivers.I2C.I2CCC3200_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.I2C.I2CCC3200_Object", "ti.drivers"));
        vo.bind("I2CMSP432_HWAttrs", om.findStrict("ti.drivers.I2C.I2CMSP432_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.I2C.I2CMSP432_HWAttrs", "ti.drivers"));
        vo.bind("I2CMSP432_Object", om.findStrict("ti.drivers.I2C.I2CMSP432_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.I2C.I2CMSP432_Object", "ti.drivers"));
        vo.bind("I2CUSCIB_HWAttrs", om.findStrict("ti.drivers.I2C.I2CUSCIB_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.I2C.I2CUSCIB_HWAttrs", "ti.drivers"));
        vo.bind("I2CEUSCIB_HWAttrs", om.findStrict("ti.drivers.I2C.I2CEUSCIB_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.I2C.I2CEUSCIB_HWAttrs", "ti.drivers"));
        vo.bind("I2CUSCIB_Object", om.findStrict("ti.drivers.I2C.I2CUSCIB_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.I2C.I2CUSCIB_Object", "ti.drivers"));
        vo.bind("I2CEUSCIB_Object", om.findStrict("ti.drivers.I2C.I2CEUSCIB_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.I2C.I2CEUSCIB_Object", "ti.drivers"));
        vo.bind("BasicView", om.findStrict("ti.drivers.I2C.BasicView", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.I2C.BasicView", "ti.drivers"));
        vo.bind("LibType_Instrumented", om.findStrict("ti.drivers.I2C.LibType_Instrumented", "ti.drivers"));
        vo.bind("LibType_NonInstrumented", om.findStrict("ti.drivers.I2C.LibType_NonInstrumented", "ti.drivers"));
        vo.bind("I2C_MODE_BLOCKING", om.findStrict("ti.drivers.I2C.I2C_MODE_BLOCKING", "ti.drivers"));
        vo.bind("I2C_MODE_CALLBACK", om.findStrict("ti.drivers.I2C.I2C_MODE_CALLBACK", "ti.drivers"));
        vo.bind("IDLE_MODE", om.findStrict("ti.drivers.I2C.IDLE_MODE", "ti.drivers"));
        vo.bind("TX_MODE", om.findStrict("ti.drivers.I2C.TX_MODE", "ti.drivers"));
        vo.bind("RX_MODE", om.findStrict("ti.drivers.I2C.RX_MODE", "ti.drivers"));
        vo.bind("ERROR", om.findStrict("ti.drivers.I2C.ERROR", "ti.drivers"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.drivers")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 1);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("$$fxntab", Global.newArray("ti_drivers_I2C_Module__startupDone__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", "");
        atmap.seal("length");
        if (isCFG) {
            vo.put("common$", vo, Global.get((Proto.Obj)om.find("xdc.runtime.Defaults.Module"), "noRuntimeCommon$"));
            ((Value.Obj)vo.geto("common$")).seal(null);
        }//isCFG
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("I2C", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("I2C");
    }

    void Power$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.Power", "ti.drivers");
        po = (Proto.Obj)om.findStrict("ti.drivers.Power.Module", "ti.drivers");
        vo.init2(po, "ti.drivers.Power", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.drivers.Power$$capsule", "ti.drivers"));
        vo.bind("$package", om.findStrict("ti.drivers", "ti.drivers"));
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
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.drivers")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 1);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("$$fxntab", Global.newArray("ti_drivers_Power_Module__startupDone__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", "");
        atmap.seal("length");
        if (isCFG) {
            vo.put("common$", vo, Global.get((Proto.Obj)om.find("xdc.runtime.Defaults.Module"), "noRuntimeCommon$"));
            ((Value.Obj)vo.geto("common$")).seal(null);
        }//isCFG
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("Power", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Power");
    }

    void PWM$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.PWM", "ti.drivers");
        po = (Proto.Obj)om.findStrict("ti.drivers.PWM.Module", "ti.drivers");
        vo.init2(po, "ti.drivers.PWM", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.drivers.PWM$$capsule", "ti.drivers"));
        vo.bind("$package", om.findStrict("ti.drivers", "ti.drivers"));
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
        vo.bind("LibType", om.findStrict("ti.drivers.PWM.LibType", "ti.drivers"));
        vo.bind("PWM_Config", om.findStrict("ti.drivers.PWM.PWM_Config", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.PWM.PWM_Config", "ti.drivers"));
        vo.bind("PWM_DutyMode", om.findStrict("ti.drivers.PWM.PWM_DutyMode", "ti.drivers"));
        vo.bind("PWM_Polarity", om.findStrict("ti.drivers.PWM.PWM_Polarity", "ti.drivers"));
        vo.bind("PWM_Params", om.findStrict("ti.drivers.PWM.PWM_Params", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.PWM.PWM_Params", "ti.drivers"));
        vo.bind("PWMTimerCC3200_HWAttrs", om.findStrict("ti.drivers.PWM.PWMTimerCC3200_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.PWM.PWMTimerCC3200_HWAttrs", "ti.drivers"));
        vo.bind("PWMTimerCC3200_Object", om.findStrict("ti.drivers.PWM.PWMTimerCC3200_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.PWM.PWMTimerCC3200_Object", "ti.drivers"));
        vo.bind("PWMTiva_Status", om.findStrict("ti.drivers.PWM.PWMTiva_Status", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.PWM.PWMTiva_Status", "ti.drivers"));
        vo.bind("PWMTiva_HWAttrs", om.findStrict("ti.drivers.PWM.PWMTiva_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.PWM.PWMTiva_HWAttrs", "ti.drivers"));
        vo.bind("PWMTiva_Object", om.findStrict("ti.drivers.PWM.PWMTiva_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.PWM.PWMTiva_Object", "ti.drivers"));
        vo.bind("PWMTimerTiva_HWAttrs", om.findStrict("ti.drivers.PWM.PWMTimerTiva_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.PWM.PWMTimerTiva_HWAttrs", "ti.drivers"));
        vo.bind("PWMTimerTiva_Object", om.findStrict("ti.drivers.PWM.PWMTimerTiva_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.PWM.PWMTimerTiva_Object", "ti.drivers"));
        vo.bind("PWMTimerMSP432_Status", om.findStrict("ti.drivers.PWM.PWMTimerMSP432_Status", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.PWM.PWMTimerMSP432_Status", "ti.drivers"));
        vo.bind("PWMTimerMSP432_HWAttrs", om.findStrict("ti.drivers.PWM.PWMTimerMSP432_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.PWM.PWMTimerMSP432_HWAttrs", "ti.drivers"));
        vo.bind("PWMTimerMSP432_Object", om.findStrict("ti.drivers.PWM.PWMTimerMSP432_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.PWM.PWMTimerMSP432_Object", "ti.drivers"));
        vo.bind("BasicView", om.findStrict("ti.drivers.PWM.BasicView", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.PWM.BasicView", "ti.drivers"));
        vo.bind("LibType_Instrumented", om.findStrict("ti.drivers.PWM.LibType_Instrumented", "ti.drivers"));
        vo.bind("LibType_NonInstrumented", om.findStrict("ti.drivers.PWM.LibType_NonInstrumented", "ti.drivers"));
        vo.bind("PWM_DUTY_COUNTS", om.findStrict("ti.drivers.PWM.PWM_DUTY_COUNTS", "ti.drivers"));
        vo.bind("PWM_DUTY_SCALAR", om.findStrict("ti.drivers.PWM.PWM_DUTY_SCALAR", "ti.drivers"));
        vo.bind("PWM_DUTY_TIME", om.findStrict("ti.drivers.PWM.PWM_DUTY_TIME", "ti.drivers"));
        vo.bind("PWM_POL_ACTIVE_HIGH", om.findStrict("ti.drivers.PWM.PWM_POL_ACTIVE_HIGH", "ti.drivers"));
        vo.bind("PWM_POL_ACTIVE_LOW", om.findStrict("ti.drivers.PWM.PWM_POL_ACTIVE_LOW", "ti.drivers"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.drivers")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 1);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("$$fxntab", Global.newArray("ti_drivers_PWM_Module__startupDone__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", "");
        atmap.seal("length");
        if (isCFG) {
            vo.put("common$", vo, Global.get((Proto.Obj)om.find("xdc.runtime.Defaults.Module"), "noRuntimeCommon$"));
            ((Value.Obj)vo.geto("common$")).seal(null);
        }//isCFG
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("PWM", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("PWM");
    }

    void SDSPI$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.SDSPI", "ti.drivers");
        po = (Proto.Obj)om.findStrict("ti.drivers.SDSPI.Module", "ti.drivers");
        vo.init2(po, "ti.drivers.SDSPI", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.drivers.SDSPI$$capsule", "ti.drivers"));
        vo.bind("$package", om.findStrict("ti.drivers", "ti.drivers"));
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
        vo.bind("LibType", om.findStrict("ti.drivers.SDSPI.LibType", "ti.drivers"));
        vo.bind("SDSPI_Config", om.findStrict("ti.drivers.SDSPI.SDSPI_Config", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.SDSPI_Config", "ti.drivers"));
        vo.bind("SDSPITiva_CardType", om.findStrict("ti.drivers.SDSPI.SDSPITiva_CardType", "ti.drivers"));
        vo.bind("SDSPITiva_HWAttrs", om.findStrict("ti.drivers.SDSPI.SDSPITiva_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.SDSPITiva_HWAttrs", "ti.drivers"));
        vo.bind("SDSPITiva_Object", om.findStrict("ti.drivers.SDSPI.SDSPITiva_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.SDSPITiva_Object", "ti.drivers"));
        vo.bind("SDSPIUSCIA_HWAttrs", om.findStrict("ti.drivers.SDSPI.SDSPIUSCIA_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.SDSPIUSCIA_HWAttrs", "ti.drivers"));
        vo.bind("SDSPIUSCIB_HWAttrs", om.findStrict("ti.drivers.SDSPI.SDSPIUSCIB_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.SDSPIUSCIB_HWAttrs", "ti.drivers"));
        vo.bind("SDSPIUSCIA_Object", om.findStrict("ti.drivers.SDSPI.SDSPIUSCIA_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.SDSPIUSCIA_Object", "ti.drivers"));
        vo.bind("SDSPIUSCIB_Object", om.findStrict("ti.drivers.SDSPI.SDSPIUSCIB_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.SDSPIUSCIB_Object", "ti.drivers"));
        vo.bind("SDSPIEUSCIA_HWAttrs", om.findStrict("ti.drivers.SDSPI.SDSPIEUSCIA_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.SDSPIEUSCIA_HWAttrs", "ti.drivers"));
        vo.bind("SDSPIEUSCIB_HWAttrs", om.findStrict("ti.drivers.SDSPI.SDSPIEUSCIB_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.SDSPIEUSCIB_HWAttrs", "ti.drivers"));
        vo.bind("SDSPIEUSCIA_Object", om.findStrict("ti.drivers.SDSPI.SDSPIEUSCIA_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.SDSPIEUSCIA_Object", "ti.drivers"));
        vo.bind("SDSPIEUSCIB_Object", om.findStrict("ti.drivers.SDSPI.SDSPIEUSCIB_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.SDSPIEUSCIB_Object", "ti.drivers"));
        vo.bind("SDSPICC3200_HWAttrs", om.findStrict("ti.drivers.SDSPI.SDSPICC3200_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.SDSPICC3200_HWAttrs", "ti.drivers"));
        vo.bind("SDSPICC3200_Object", om.findStrict("ti.drivers.SDSPI.SDSPICC3200_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.SDSPICC3200_Object", "ti.drivers"));
        vo.bind("SDSPIMSP432_HWAttrs", om.findStrict("ti.drivers.SDSPI.SDSPIMSP432_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.SDSPIMSP432_HWAttrs", "ti.drivers"));
        vo.bind("SDSPIMSP432_Object", om.findStrict("ti.drivers.SDSPI.SDSPIMSP432_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.SDSPIMSP432_Object", "ti.drivers"));
        vo.bind("BasicView", om.findStrict("ti.drivers.SDSPI.BasicView", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SDSPI.BasicView", "ti.drivers"));
        vo.bind("LibType_Instrumented", om.findStrict("ti.drivers.SDSPI.LibType_Instrumented", "ti.drivers"));
        vo.bind("LibType_NonInstrumented", om.findStrict("ti.drivers.SDSPI.LibType_NonInstrumented", "ti.drivers"));
        vo.bind("NOCARD", om.findStrict("ti.drivers.SDSPI.NOCARD", "ti.drivers"));
        vo.bind("MMC", om.findStrict("ti.drivers.SDSPI.MMC", "ti.drivers"));
        vo.bind("SDSC", om.findStrict("ti.drivers.SDSPI.SDSC", "ti.drivers"));
        vo.bind("SDHC", om.findStrict("ti.drivers.SDSPI.SDHC", "ti.drivers"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.drivers")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 1);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("$$fxntab", Global.newArray("ti_drivers_SDSPI_Module__startupDone__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", "");
        atmap.seal("length");
        if (isCFG) {
            vo.put("common$", vo, Global.get((Proto.Obj)om.find("xdc.runtime.Defaults.Module"), "noRuntimeCommon$"));
            ((Value.Obj)vo.geto("common$")).seal(null);
        }//isCFG
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("SDSPI", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("SDSPI");
    }

    void SPI$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.SPI", "ti.drivers");
        po = (Proto.Obj)om.findStrict("ti.drivers.SPI.Module", "ti.drivers");
        vo.init2(po, "ti.drivers.SPI", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.drivers.SPI$$capsule", "ti.drivers"));
        vo.bind("$package", om.findStrict("ti.drivers", "ti.drivers"));
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
        vo.bind("LibType", om.findStrict("ti.drivers.SPI.LibType", "ti.drivers"));
        vo.bind("SPI_Config", om.findStrict("ti.drivers.SPI.SPI_Config", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPI_Config", "ti.drivers"));
        vo.bind("CallbackFxn", om.findStrict("ti.drivers.SPI.CallbackFxn", "ti.drivers"));
        vo.bind("TransferMode", om.findStrict("ti.drivers.SPI.TransferMode", "ti.drivers"));
        vo.bind("SPI_Transaction", om.findStrict("ti.drivers.SPI.SPI_Transaction", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPI_Transaction", "ti.drivers"));
        vo.bind("SPITivaDMA_FrameSize", om.findStrict("ti.drivers.SPI.SPITivaDMA_FrameSize", "ti.drivers"));
        vo.bind("SPITivaDMA_HWAttrs", om.findStrict("ti.drivers.SPI.SPITivaDMA_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPITivaDMA_HWAttrs", "ti.drivers"));
        vo.bind("SPITivaDMA_Object", om.findStrict("ti.drivers.SPI.SPITivaDMA_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPITivaDMA_Object", "ti.drivers"));
        vo.bind("SPICC3200DMA_FrameSize", om.findStrict("ti.drivers.SPI.SPICC3200DMA_FrameSize", "ti.drivers"));
        vo.bind("SPICC3200DMA_HWAttrs", om.findStrict("ti.drivers.SPI.SPICC3200DMA_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPICC3200DMA_HWAttrs", "ti.drivers"));
        vo.bind("SPICC3200DMA_Object", om.findStrict("ti.drivers.SPI.SPICC3200DMA_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPICC3200DMA_Object", "ti.drivers"));
        vo.bind("SPIUSCIADMA_HWAttrs", om.findStrict("ti.drivers.SPI.SPIUSCIADMA_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPIUSCIADMA_HWAttrs", "ti.drivers"));
        vo.bind("SPIUSCIADMA_Object", om.findStrict("ti.drivers.SPI.SPIUSCIADMA_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPIUSCIADMA_Object", "ti.drivers"));
        vo.bind("SPIUSCIBDMA_HWAttrs", om.findStrict("ti.drivers.SPI.SPIUSCIBDMA_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPIUSCIBDMA_HWAttrs", "ti.drivers"));
        vo.bind("SPIUSCIBDMA_Object", om.findStrict("ti.drivers.SPI.SPIUSCIBDMA_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPIUSCIBDMA_Object", "ti.drivers"));
        vo.bind("SPIEUSCIADMA_HWAttrs", om.findStrict("ti.drivers.SPI.SPIEUSCIADMA_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPIEUSCIADMA_HWAttrs", "ti.drivers"));
        vo.bind("SPIEUSCIADMA_Object", om.findStrict("ti.drivers.SPI.SPIEUSCIADMA_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPIEUSCIADMA_Object", "ti.drivers"));
        vo.bind("SPIEUSCIBDMA_HWAttrs", om.findStrict("ti.drivers.SPI.SPIEUSCIBDMA_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPIEUSCIBDMA_HWAttrs", "ti.drivers"));
        vo.bind("SPIEUSCIBDMA_Object", om.findStrict("ti.drivers.SPI.SPIEUSCIBDMA_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPIEUSCIBDMA_Object", "ti.drivers"));
        vo.bind("SPIMSP432DMA_HWAttrs", om.findStrict("ti.drivers.SPI.SPIMSP432DMA_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPIMSP432DMA_HWAttrs", "ti.drivers"));
        vo.bind("SPIMSP432DMA_Object", om.findStrict("ti.drivers.SPI.SPIMSP432DMA_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.SPIMSP432DMA_Object", "ti.drivers"));
        vo.bind("BasicView", om.findStrict("ti.drivers.SPI.BasicView", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.SPI.BasicView", "ti.drivers"));
        vo.bind("LibType_Instrumented", om.findStrict("ti.drivers.SPI.LibType_Instrumented", "ti.drivers"));
        vo.bind("LibType_NonInstrumented", om.findStrict("ti.drivers.SPI.LibType_NonInstrumented", "ti.drivers"));
        vo.bind("SPI_MODE_BLOCKING", om.findStrict("ti.drivers.SPI.SPI_MODE_BLOCKING", "ti.drivers"));
        vo.bind("SPI_MODE_CALLBACK", om.findStrict("ti.drivers.SPI.SPI_MODE_CALLBACK", "ti.drivers"));
        vo.bind("SPITivaDMA_8bit", om.findStrict("ti.drivers.SPI.SPITivaDMA_8bit", "ti.drivers"));
        vo.bind("SPITivaDMA_16bit", om.findStrict("ti.drivers.SPI.SPITivaDMA_16bit", "ti.drivers"));
        vo.bind("SPICC3200DMA_8bit", om.findStrict("ti.drivers.SPI.SPICC3200DMA_8bit", "ti.drivers"));
        vo.bind("SPICC3200DMA_16bit", om.findStrict("ti.drivers.SPI.SPICC3200DMA_16bit", "ti.drivers"));
        vo.bind("SPICC3200DMA_32bit", om.findStrict("ti.drivers.SPI.SPICC3200DMA_32bit", "ti.drivers"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.drivers")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 1);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("$$fxntab", Global.newArray("ti_drivers_SPI_Module__startupDone__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", "");
        atmap.seal("length");
        if (isCFG) {
            vo.put("common$", vo, Global.get((Proto.Obj)om.find("xdc.runtime.Defaults.Module"), "noRuntimeCommon$"));
            ((Value.Obj)vo.geto("common$")).seal(null);
        }//isCFG
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("SPI", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("SPI");
    }

    void UART$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.UART", "ti.drivers");
        po = (Proto.Obj)om.findStrict("ti.drivers.UART.Module", "ti.drivers");
        vo.init2(po, "ti.drivers.UART", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.drivers.UART$$capsule", "ti.drivers"));
        vo.bind("$package", om.findStrict("ti.drivers", "ti.drivers"));
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
        vo.bind("LibType", om.findStrict("ti.drivers.UART.LibType", "ti.drivers"));
        vo.bind("UART_Config", om.findStrict("ti.drivers.UART.UART_Config", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.UART_Config", "ti.drivers"));
        vo.bind("CallbackFxn", om.findStrict("ti.drivers.UART.CallbackFxn", "ti.drivers"));
        vo.bind("UART_Mode", om.findStrict("ti.drivers.UART.UART_Mode", "ti.drivers"));
        vo.bind("UART_ReturnMode", om.findStrict("ti.drivers.UART.UART_ReturnMode", "ti.drivers"));
        vo.bind("UART_DataMode", om.findStrict("ti.drivers.UART.UART_DataMode", "ti.drivers"));
        vo.bind("UART_Echo", om.findStrict("ti.drivers.UART.UART_Echo", "ti.drivers"));
        vo.bind("UART_LEN", om.findStrict("ti.drivers.UART.UART_LEN", "ti.drivers"));
        vo.bind("UART_STOP", om.findStrict("ti.drivers.UART.UART_STOP", "ti.drivers"));
        vo.bind("UART_PAR", om.findStrict("ti.drivers.UART.UART_PAR", "ti.drivers"));
        vo.bind("UARTTiva_HWAttrs", om.findStrict("ti.drivers.UART.UARTTiva_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.UARTTiva_HWAttrs", "ti.drivers"));
        vo.bind("UARTTivaDMA_HWAttrs", om.findStrict("ti.drivers.UART.UARTTivaDMA_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.UARTTivaDMA_HWAttrs", "ti.drivers"));
        vo.bind("UARTTiva_Object", om.findStrict("ti.drivers.UART.UARTTiva_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.UARTTiva_Object", "ti.drivers"));
        vo.bind("UARTCC3200_HWAttrs", om.findStrict("ti.drivers.UART.UARTCC3200_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.UARTCC3200_HWAttrs", "ti.drivers"));
        vo.bind("UARTCC3200DMA_HWAttrs", om.findStrict("ti.drivers.UART.UARTCC3200DMA_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.UARTCC3200DMA_HWAttrs", "ti.drivers"));
        vo.bind("UARTCC3200_Object", om.findStrict("ti.drivers.UART.UARTCC3200_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.UARTCC3200_Object", "ti.drivers"));
        vo.bind("UARTTivaDMA_Object", om.findStrict("ti.drivers.UART.UARTTivaDMA_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.UARTTivaDMA_Object", "ti.drivers"));
        vo.bind("UARTCC3200DMA_Object", om.findStrict("ti.drivers.UART.UARTCC3200DMA_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.UARTCC3200DMA_Object", "ti.drivers"));
        vo.bind("UARTUSCIA_HWAttrs", om.findStrict("ti.drivers.UART.UARTUSCIA_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.UARTUSCIA_HWAttrs", "ti.drivers"));
        vo.bind("UARTUSCIA_Object", om.findStrict("ti.drivers.UART.UARTUSCIA_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.UARTUSCIA_Object", "ti.drivers"));
        vo.bind("UARTEUSCIA_HWAttrs", om.findStrict("ti.drivers.UART.UARTEUSCIA_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.UARTEUSCIA_HWAttrs", "ti.drivers"));
        vo.bind("UARTEUSCIA_Object", om.findStrict("ti.drivers.UART.UARTEUSCIA_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.UARTEUSCIA_Object", "ti.drivers"));
        vo.bind("UARTMSP432_HWAttrs", om.findStrict("ti.drivers.UART.UARTMSP432_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.UARTMSP432_HWAttrs", "ti.drivers"));
        vo.bind("UARTMSP432_Object", om.findStrict("ti.drivers.UART.UARTMSP432_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.UARTMSP432_Object", "ti.drivers"));
        vo.bind("BasicView", om.findStrict("ti.drivers.UART.BasicView", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.BasicView", "ti.drivers"));
        vo.bind("ConfigView", om.findStrict("ti.drivers.UART.ConfigView", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.ConfigView", "ti.drivers"));
        vo.bind("BufferView", om.findStrict("ti.drivers.UART.BufferView", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.UART.BufferView", "ti.drivers"));
        vo.bind("LibType_Instrumented", om.findStrict("ti.drivers.UART.LibType_Instrumented", "ti.drivers"));
        vo.bind("LibType_NonInstrumented", om.findStrict("ti.drivers.UART.LibType_NonInstrumented", "ti.drivers"));
        vo.bind("UART_MODE_BLOCKING", om.findStrict("ti.drivers.UART.UART_MODE_BLOCKING", "ti.drivers"));
        vo.bind("UART_MODE_CALLBACK", om.findStrict("ti.drivers.UART.UART_MODE_CALLBACK", "ti.drivers"));
        vo.bind("UART_MODE_ISSUERECLAIM", om.findStrict("ti.drivers.UART.UART_MODE_ISSUERECLAIM", "ti.drivers"));
        vo.bind("UART_RETURN_FULL", om.findStrict("ti.drivers.UART.UART_RETURN_FULL", "ti.drivers"));
        vo.bind("UART_RETURN_NEWLINE", om.findStrict("ti.drivers.UART.UART_RETURN_NEWLINE", "ti.drivers"));
        vo.bind("UART_DATA_BINARY", om.findStrict("ti.drivers.UART.UART_DATA_BINARY", "ti.drivers"));
        vo.bind("UART_DATA_TEXT", om.findStrict("ti.drivers.UART.UART_DATA_TEXT", "ti.drivers"));
        vo.bind("UART_ECHO_OFF", om.findStrict("ti.drivers.UART.UART_ECHO_OFF", "ti.drivers"));
        vo.bind("UART_ECHO_ON", om.findStrict("ti.drivers.UART.UART_ECHO_ON", "ti.drivers"));
        vo.bind("UART_LEN_5", om.findStrict("ti.drivers.UART.UART_LEN_5", "ti.drivers"));
        vo.bind("UART_LEN_6", om.findStrict("ti.drivers.UART.UART_LEN_6", "ti.drivers"));
        vo.bind("UART_LEN_7", om.findStrict("ti.drivers.UART.UART_LEN_7", "ti.drivers"));
        vo.bind("UART_LEN_8", om.findStrict("ti.drivers.UART.UART_LEN_8", "ti.drivers"));
        vo.bind("UART_STOP_ONE", om.findStrict("ti.drivers.UART.UART_STOP_ONE", "ti.drivers"));
        vo.bind("UART_STOP_TWO", om.findStrict("ti.drivers.UART.UART_STOP_TWO", "ti.drivers"));
        vo.bind("UART_PAR_NONE", om.findStrict("ti.drivers.UART.UART_PAR_NONE", "ti.drivers"));
        vo.bind("UART_PAR_EVEN", om.findStrict("ti.drivers.UART.UART_PAR_EVEN", "ti.drivers"));
        vo.bind("UART_PAR_ODD", om.findStrict("ti.drivers.UART.UART_PAR_ODD", "ti.drivers"));
        vo.bind("UART_PAR_ZERO", om.findStrict("ti.drivers.UART.UART_PAR_ZERO", "ti.drivers"));
        vo.bind("UART_PAR_ONE", om.findStrict("ti.drivers.UART.UART_PAR_ONE", "ti.drivers"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.drivers")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 1);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("$$fxntab", Global.newArray("ti_drivers_UART_Module__startupDone__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", "");
        atmap.seal("length");
        if (isCFG) {
            vo.put("common$", vo, Global.get((Proto.Obj)om.find("xdc.runtime.Defaults.Module"), "noRuntimeCommon$"));
            ((Value.Obj)vo.geto("common$")).seal(null);
        }//isCFG
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("UART", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("UART");
    }

    void USBMSCHFatFs$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.USBMSCHFatFs", "ti.drivers");
        po = (Proto.Obj)om.findStrict("ti.drivers.USBMSCHFatFs.Module", "ti.drivers");
        vo.init2(po, "ti.drivers.USBMSCHFatFs", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.drivers.USBMSCHFatFs$$capsule", "ti.drivers"));
        vo.bind("$package", om.findStrict("ti.drivers", "ti.drivers"));
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
        vo.bind("LibType", om.findStrict("ti.drivers.USBMSCHFatFs.LibType", "ti.drivers"));
        vo.bind("USBMSCHFatFs_Config", om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFs_Config", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFs_Config", "ti.drivers"));
        vo.bind("USBMSCHFatFsTiva_USBState", om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_USBState", "ti.drivers"));
        vo.bind("USBMSCHFatFsTiva_HWAttrs", om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_HWAttrs", "ti.drivers"));
        vo.bind("USBMSCHFatFsTiva_Object", om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_Object", "ti.drivers"));
        vo.bind("BasicView", om.findStrict("ti.drivers.USBMSCHFatFs.BasicView", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.USBMSCHFatFs.BasicView", "ti.drivers"));
        vo.bind("LibType_Instrumented", om.findStrict("ti.drivers.USBMSCHFatFs.LibType_Instrumented", "ti.drivers"));
        vo.bind("LibType_NonInstrumented", om.findStrict("ti.drivers.USBMSCHFatFs.LibType_NonInstrumented", "ti.drivers"));
        vo.bind("USBMSCHFatFsTiva_NO_DEVICE", om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_NO_DEVICE", "ti.drivers"));
        vo.bind("USBMSCHFatFsTiva_CONNECTED", om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_CONNECTED", "ti.drivers"));
        vo.bind("USBMSCHFatFsTiva_UNKNOWN", om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_UNKNOWN", "ti.drivers"));
        vo.bind("USBMSCHFatFsTiva_POWER_FAULT", om.findStrict("ti.drivers.USBMSCHFatFs.USBMSCHFatFsTiva_POWER_FAULT", "ti.drivers"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.drivers")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 1);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("$$fxntab", Global.newArray("ti_drivers_USBMSCHFatFs_Module__startupDone__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", "");
        atmap.seal("length");
        if (isCFG) {
            vo.put("common$", vo, Global.get((Proto.Obj)om.find("xdc.runtime.Defaults.Module"), "noRuntimeCommon$"));
            ((Value.Obj)vo.geto("common$")).seal(null);
        }//isCFG
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("USBMSCHFatFs", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("USBMSCHFatFs");
    }

    void Watchdog$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.Watchdog", "ti.drivers");
        po = (Proto.Obj)om.findStrict("ti.drivers.Watchdog.Module", "ti.drivers");
        vo.init2(po, "ti.drivers.Watchdog", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.drivers.Watchdog$$capsule", "ti.drivers"));
        vo.bind("$package", om.findStrict("ti.drivers", "ti.drivers"));
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
        vo.bind("LibType", om.findStrict("ti.drivers.Watchdog.LibType", "ti.drivers"));
        vo.bind("WatchdogTiva_HWAttrs", om.findStrict("ti.drivers.Watchdog.WatchdogTiva_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.Watchdog.WatchdogTiva_HWAttrs", "ti.drivers"));
        vo.bind("WatchdogTiva_Object", om.findStrict("ti.drivers.Watchdog.WatchdogTiva_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.Watchdog.WatchdogTiva_Object", "ti.drivers"));
        vo.bind("WatchdogCC3200_HWAttrs", om.findStrict("ti.drivers.Watchdog.WatchdogCC3200_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.Watchdog.WatchdogCC3200_HWAttrs", "ti.drivers"));
        vo.bind("WatchdogCC3200_Object", om.findStrict("ti.drivers.Watchdog.WatchdogCC3200_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.Watchdog.WatchdogCC3200_Object", "ti.drivers"));
        vo.bind("WatchdogMSP430_HWAttrs", om.findStrict("ti.drivers.Watchdog.WatchdogMSP430_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.Watchdog.WatchdogMSP430_HWAttrs", "ti.drivers"));
        vo.bind("WatchdogMSP430_Object", om.findStrict("ti.drivers.Watchdog.WatchdogMSP430_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.Watchdog.WatchdogMSP430_Object", "ti.drivers"));
        vo.bind("WatchdogMSP432_HWAttrs", om.findStrict("ti.drivers.Watchdog.WatchdogMSP432_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.Watchdog.WatchdogMSP432_HWAttrs", "ti.drivers"));
        vo.bind("WatchdogMSP432_Object", om.findStrict("ti.drivers.Watchdog.WatchdogMSP432_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.Watchdog.WatchdogMSP432_Object", "ti.drivers"));
        vo.bind("Watchdog_Config", om.findStrict("ti.drivers.Watchdog.Watchdog_Config", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.Watchdog.Watchdog_Config", "ti.drivers"));
        vo.bind("BasicView", om.findStrict("ti.drivers.Watchdog.BasicView", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.Watchdog.BasicView", "ti.drivers"));
        vo.bind("LibType_Instrumented", om.findStrict("ti.drivers.Watchdog.LibType_Instrumented", "ti.drivers"));
        vo.bind("LibType_NonInstrumented", om.findStrict("ti.drivers.Watchdog.LibType_NonInstrumented", "ti.drivers"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.drivers")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 1);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("$$fxntab", Global.newArray("ti_drivers_Watchdog_Module__startupDone__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", "");
        atmap.seal("length");
        if (isCFG) {
            vo.put("common$", vo, Global.get((Proto.Obj)om.find("xdc.runtime.Defaults.Module"), "noRuntimeCommon$"));
            ((Value.Obj)vo.geto("common$")).seal(null);
        }//isCFG
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        pkgV.bind("Watchdog", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("Watchdog");
    }

    void WiFi$$SINGLETONS()
    {
        Proto.Obj po;
        Value.Obj vo;

        vo = (Value.Obj)om.findStrict("ti.drivers.WiFi", "ti.drivers");
        po = (Proto.Obj)om.findStrict("ti.drivers.WiFi.Module", "ti.drivers");
        vo.init2(po, "ti.drivers.WiFi", $$DEFAULT, false);
        vo.bind("Module", po);
        vo.bind("$category", "Module");
        vo.bind("$capsule", om.findStrict("ti.drivers.WiFi$$capsule", "ti.drivers"));
        vo.bind("$package", om.findStrict("ti.drivers", "ti.drivers"));
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
        vo.bind("DeviceType", om.findStrict("ti.drivers.WiFi.DeviceType", "ti.drivers"));
        vo.bind("LibType", om.findStrict("ti.drivers.WiFi.LibType", "ti.drivers"));
        vo.bind("WiFi_Config", om.findStrict("ti.drivers.WiFi.WiFi_Config", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.WiFi.WiFi_Config", "ti.drivers"));
        vo.bind("WiFiCC3100_SPI_State", om.findStrict("ti.drivers.WiFi.WiFiCC3100_SPI_State", "ti.drivers"));
        vo.bind("WiFiCC3100_HWAttrs", om.findStrict("ti.drivers.WiFi.WiFiCC3100_HWAttrs", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.WiFi.WiFiCC3100_HWAttrs", "ti.drivers"));
        vo.bind("WiFiCC3100_Object", om.findStrict("ti.drivers.WiFi.WiFiCC3100_Object", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.WiFi.WiFiCC3100_Object", "ti.drivers"));
        vo.bind("BasicView", om.findStrict("ti.drivers.WiFi.BasicView", "ti.drivers"));
        tdefs.add(om.findStrict("ti.drivers.WiFi.BasicView", "ti.drivers"));
        vo.bind("DeviceType_SimpleLink_CC3100", om.findStrict("ti.drivers.WiFi.DeviceType_SimpleLink_CC3100", "ti.drivers"));
        vo.bind("LibType_Instrumented", om.findStrict("ti.drivers.WiFi.LibType_Instrumented", "ti.drivers"));
        vo.bind("LibType_NonInstrumented", om.findStrict("ti.drivers.WiFi.LibType_NonInstrumented", "ti.drivers"));
        vo.bind("WiFiCC3100_SPI_UNINITIALIZED", om.findStrict("ti.drivers.WiFi.WiFiCC3100_SPI_UNINITIALIZED", "ti.drivers"));
        vo.bind("WiFiCC3100_SPI_IDLE", om.findStrict("ti.drivers.WiFi.WiFiCC3100_SPI_IDLE", "ti.drivers"));
        vo.bind("WiFiCC3100_SPI_READ_EOT", om.findStrict("ti.drivers.WiFi.WiFiCC3100_SPI_READ_EOT", "ti.drivers"));
        vo.bind("WiFiCC3100_SPI_READ_MSG", om.findStrict("ti.drivers.WiFi.WiFiCC3100_SPI_READ_MSG", "ti.drivers"));
        vo.bind("WiFiCC3100_SPI_WRITE_EOT", om.findStrict("ti.drivers.WiFi.WiFiCC3100_SPI_WRITE_EOT", "ti.drivers"));
        vo.bind("WiFiCC3100_SPI_WRITE_MSG", om.findStrict("ti.drivers.WiFi.WiFiCC3100_SPI_WRITE_MSG", "ti.drivers"));
        vo.bind("$$tdefs", Global.newArray(tdefs.toArray()));
        vo.bind("$$proxies", Global.newArray(proxies.toArray()));
        vo.bind("$$mcfgs", Global.newArray(mcfgs.toArray()));
        vo.bind("$$icfgs", Global.newArray(icfgs.toArray()));
        inherits.add("xdc.runtime");
        vo.bind("$$inherits", Global.newArray(inherits.toArray()));
        ((Value.Arr)pkgV.getv("$modules")).add(vo);
        ((Value.Arr)om.findStrict("$modules", "ti.drivers")).add(vo);
        vo.bind("$$instflag", 0);
        vo.bind("$$iobjflag", 0);
        vo.bind("$$sizeflag", 1);
        vo.bind("$$dlgflag", 0);
        vo.bind("$$iflag", 0);
        vo.bind("$$romcfgs", "|");
        vo.bind("$$nortsflag", 1);
        if (isCFG) {
            Proto.Str ps = (Proto.Str)vo.find("Module_State");
            if (ps != null) vo.bind("$object", ps.newInstance());
            vo.bind("$$meta_iobj", 1);
        }//isCFG
        vo.bind("$$fxntab", Global.newArray("ti_drivers_WiFi_Module__startupDone__E"));
        vo.bind("$$logEvtCfgs", Global.newArray());
        vo.bind("$$errorDescCfgs", Global.newArray());
        vo.bind("$$assertDescCfgs", Global.newArray());
        Value.Map atmap = (Value.Map)vo.getv("$attr");
        atmap.setElem("", true);
        atmap.setElem("", "./WiFi.xdt");
        atmap.setElem("", "");
        atmap.seal("length");
        if (isCFG) {
            vo.put("common$", vo, Global.get((Proto.Obj)om.find("xdc.runtime.Defaults.Module"), "noRuntimeCommon$"));
            ((Value.Obj)vo.geto("common$")).seal(null);
        }//isCFG
        vo.bind("MODULE_STARTUP$", 0);
        vo.bind("PROXY$", 0);
        loggables.clear();
        vo.bind("$$loggables", loggables.toArray());
        vo.bind("TEMPLATE$", "./WiFi.xdt");
        pkgV.bind("WiFi", vo);
        ((Value.Arr)pkgV.getv("$unitNames")).add("WiFi");
    }

    void $$INITIALIZATION()
    {
        Value.Obj vo;

        if (isCFG) {
        }//isCFG
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.drivers.EMAC", "ti.drivers"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.drivers.GPIO", "ti.drivers"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.drivers.I2C", "ti.drivers"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.drivers.Power", "ti.drivers"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.drivers.PWM", "ti.drivers"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.drivers.SDSPI", "ti.drivers"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.drivers.SPI", "ti.drivers"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.drivers.UART", "ti.drivers"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.drivers.USBMSCHFatFs", "ti.drivers"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.drivers.Watchdog", "ti.drivers"));
        Global.callFxn("module$meta$init", (Scriptable)om.findStrict("ti.drivers.WiFi", "ti.drivers"));
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.drivers.EMAC", "ti.drivers");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Basic", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE_DATA"), "viewInitFxn", "viewInitBasic", "structName", "BasicView")}), Global.newArray(new Object[]{"Statistics", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE_DATA"), "viewInitFxn", "viewInitStats", "structName", "StatsView")})}))));
        }//isCFG
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.drivers.GPIO", "ti.drivers");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Basic", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE_DATA"), "viewInitFxn", "viewInitBasic", "structName", "BasicView")})}))));
        }//isCFG
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.drivers.I2C", "ti.drivers");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Basic", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE_DATA"), "viewInitFxn", "viewInitBasic", "structName", "BasicView")})}))));
        }//isCFG
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.drivers.PWM", "ti.drivers");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Basic", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE_DATA"), "viewInitFxn", "viewInitBasic", "structName", "BasicView")})}))));
        }//isCFG
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.drivers.SDSPI", "ti.drivers");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Basic", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE_DATA"), "viewInitFxn", "viewInitBasic", "structName", "BasicView")})}))));
        }//isCFG
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.drivers.SPI", "ti.drivers");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Basic", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE_DATA"), "viewInitFxn", "viewInitBasic", "structName", "BasicView")})}))));
        }//isCFG
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.drivers.UART", "ti.drivers");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Basic", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE_DATA"), "viewInitFxn", "viewInitBasic", "structName", "BasicView")})}))));
        }//isCFG
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.drivers.USBMSCHFatFs", "ti.drivers");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Basic", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE_DATA"), "viewInitFxn", "viewInitBasic", "structName", "BasicView")})}))));
        }//isCFG
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.drivers.Watchdog", "ti.drivers");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Basic", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE_DATA"), "viewInitFxn", "viewInitBasic", "structName", "BasicView")})}))));
        }//isCFG
        if (isCFG) {
            vo = (Value.Obj)om.findStrict("ti.drivers.WiFi", "ti.drivers");
            Global.put(vo, "rovViewInfo", Global.callFxn("create", (Scriptable)om.find("xdc.rov.ViewInfo"), Global.newObject("viewMap", Global.newArray(new Object[]{Global.newArray(new Object[]{"Basic", Global.newObject("type", om.find("xdc.rov.ViewInfo.MODULE_DATA"), "viewInitFxn", "viewInitBasic", "structName", "BasicView")})}))));
        }//isCFG
        Global.callFxn("init", pkgV);
        ((Value.Obj)om.getv("ti.drivers.EMAC")).bless();
        ((Value.Obj)om.getv("ti.drivers.GPIO")).bless();
        ((Value.Obj)om.getv("ti.drivers.I2C")).bless();
        ((Value.Obj)om.getv("ti.drivers.Power")).bless();
        ((Value.Obj)om.getv("ti.drivers.PWM")).bless();
        ((Value.Obj)om.getv("ti.drivers.SDSPI")).bless();
        ((Value.Obj)om.getv("ti.drivers.SPI")).bless();
        ((Value.Obj)om.getv("ti.drivers.UART")).bless();
        ((Value.Obj)om.getv("ti.drivers.USBMSCHFatFs")).bless();
        ((Value.Obj)om.getv("ti.drivers.Watchdog")).bless();
        ((Value.Obj)om.getv("ti.drivers.WiFi")).bless();
        ((Value.Arr)om.findStrict("$packages", "ti.drivers")).add(pkgV);
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
        EMAC$$OBJECTS();
        GPIO$$OBJECTS();
        I2C$$OBJECTS();
        Power$$OBJECTS();
        PWM$$OBJECTS();
        SDSPI$$OBJECTS();
        SPI$$OBJECTS();
        UART$$OBJECTS();
        USBMSCHFatFs$$OBJECTS();
        Watchdog$$OBJECTS();
        WiFi$$OBJECTS();
        EMAC$$CONSTS();
        GPIO$$CONSTS();
        I2C$$CONSTS();
        Power$$CONSTS();
        PWM$$CONSTS();
        SDSPI$$CONSTS();
        SPI$$CONSTS();
        UART$$CONSTS();
        USBMSCHFatFs$$CONSTS();
        Watchdog$$CONSTS();
        WiFi$$CONSTS();
        EMAC$$CREATES();
        GPIO$$CREATES();
        I2C$$CREATES();
        Power$$CREATES();
        PWM$$CREATES();
        SDSPI$$CREATES();
        SPI$$CREATES();
        UART$$CREATES();
        USBMSCHFatFs$$CREATES();
        Watchdog$$CREATES();
        WiFi$$CREATES();
        EMAC$$FUNCTIONS();
        GPIO$$FUNCTIONS();
        I2C$$FUNCTIONS();
        Power$$FUNCTIONS();
        PWM$$FUNCTIONS();
        SDSPI$$FUNCTIONS();
        SPI$$FUNCTIONS();
        UART$$FUNCTIONS();
        USBMSCHFatFs$$FUNCTIONS();
        Watchdog$$FUNCTIONS();
        WiFi$$FUNCTIONS();
        EMAC$$SIZES();
        GPIO$$SIZES();
        I2C$$SIZES();
        Power$$SIZES();
        PWM$$SIZES();
        SDSPI$$SIZES();
        SPI$$SIZES();
        UART$$SIZES();
        USBMSCHFatFs$$SIZES();
        Watchdog$$SIZES();
        WiFi$$SIZES();
        EMAC$$TYPES();
        GPIO$$TYPES();
        I2C$$TYPES();
        Power$$TYPES();
        PWM$$TYPES();
        SDSPI$$TYPES();
        SPI$$TYPES();
        UART$$TYPES();
        USBMSCHFatFs$$TYPES();
        Watchdog$$TYPES();
        WiFi$$TYPES();
        if (isROV) {
            EMAC$$ROV();
            GPIO$$ROV();
            I2C$$ROV();
            Power$$ROV();
            PWM$$ROV();
            SDSPI$$ROV();
            SPI$$ROV();
            UART$$ROV();
            USBMSCHFatFs$$ROV();
            Watchdog$$ROV();
            WiFi$$ROV();
        }//isROV
        $$SINGLETONS();
        EMAC$$SINGLETONS();
        GPIO$$SINGLETONS();
        I2C$$SINGLETONS();
        Power$$SINGLETONS();
        PWM$$SINGLETONS();
        SDSPI$$SINGLETONS();
        SPI$$SINGLETONS();
        UART$$SINGLETONS();
        USBMSCHFatFs$$SINGLETONS();
        Watchdog$$SINGLETONS();
        WiFi$$SINGLETONS();
        $$INITIALIZATION();
    }
}
