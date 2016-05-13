/* Boards capsule is defined by examplegen */
var NDKVer = Boards.NDKVer;

var MSPWareVer = Boards.MSPWareVer;

if (MSPWareVer == undefined){
    MSPWareVer = "MSPWare_2_00_00_35a";
    print("USING unwanted MSPWare dir");
}

var mspWarePath = "../../products/" + MSPWareVer;
var usbStackPath = mspWarePath + "/usblib430/MSP430_USB_Software/MSP430_USB_API/USB_API/";

var msp430USBstackFiles = [

    /* USB_CDC_API */
    {path: usbStackPath + "USB_CDC_API/UsbCdc.c",               targetDirectory: "USB_API/USB_CDC_API"},
    {path: usbStackPath + "USB_CDC_API/UsbCdc.h",               targetDirectory: "USB_API/USB_CDC_API"},

    /* USB_Common */
    {path: usbStackPath + "USB_Common/defMSP430USB.h",          targetDirectory: "USB_API/USB_Common"},
    {path: usbStackPath + "USB_Common/device.h",                targetDirectory: "USB_API/USB_Common"},
    {path: usbStackPath + "USB_Common/types.h",                 targetDirectory: "USB_API/USB_Common"},
    {path: usbStackPath + "USB_Common/usb.c",                   targetDirectory: "USB_API/USB_Common"},
    {path: usbStackPath + "USB_Common/usb.h",                   targetDirectory: "USB_API/USB_Common"},
    {path: usbStackPath + "USB_Common/usbdma.c",                targetDirectory: "USB_API/USB_Common"},
    {path: usbStackPath + "USB_Common/UsbIsr.h",                targetDirectory: "USB_API/USB_Common"},

    /* USB_HID_API */
    {path: usbStackPath + "USB_HID_API/UsbHid.c",               targetDirectory: "USB_API/USB_HID_API"},
    {path: usbStackPath + "USB_HID_API/UsbHid.h",               targetDirectory: "USB_API/USB_HID_API"},
    {path: usbStackPath + "USB_HID_API/UsbHidReq.c",            targetDirectory: "USB_API/USB_HID_API"},
    {path: usbStackPath + "USB_HID_API/UsbHidReq.h",            targetDirectory: "USB_API/USB_HID_API"},

    /* USB_MSC_API */
    {path: usbStackPath + "USB_MSC_API/UsbMsc.h",               targetDirectory: "USB_API/USB_MSC_API"},
    {path: usbStackPath + "USB_MSC_API/UsbMscReq.c",            targetDirectory: "USB_API/USB_MSC_API"},
    {path: usbStackPath + "USB_MSC_API/UsbMscReq.h",            targetDirectory: "USB_API/USB_MSC_API"},
    {path: usbStackPath + "USB_MSC_API/UsbMscScsi.c",           targetDirectory: "USB_API/USB_MSC_API"},
    {path: usbStackPath + "USB_MSC_API/UsbMscScsi.h",           targetDirectory: "USB_API/USB_MSC_API"},
    {path: usbStackPath + "USB_MSC_API/UsbMscStateMachine.c",   targetDirectory: "USB_API/USB_MSC_API"},
    {path: usbStackPath + "USB_MSC_API/UsbMscStateMachine.h",   targetDirectory: "USB_API/USB_MSC_API"},

    /* USB_PHDC_API */
    {path: usbStackPath + "USB_PHDC_API/UsbPHDC.c",             targetDirectory: "USB_API/USB_PHDC_API"},
    {path: usbStackPath + "USB_PHDC_API/UsbPHDC.h",             targetDirectory: "USB_API/USB_PHDC_API"},

    /* USB linker cmd file */
    {path: usbStackPath + "msp430USB.cmd",                      targetDirectory: "USB_API"},
];

var grlibFiles = [
    {path: mspWarePath + "/grlib/grlib/button.c",            targetDirectory: "grlib"},
    {path: mspWarePath + "/grlib/grlib/button.h",            targetDirectory: "grlib"},
    {path: mspWarePath + "/grlib/grlib/checkbox.c",          targetDirectory: "grlib"},
    {path: mspWarePath + "/grlib/grlib/checkbox.h",          targetDirectory: "grlib"},
    {path: mspWarePath + "/grlib/grlib/circle.c",            targetDirectory: "grlib"},
    {path: mspWarePath + "/grlib/grlib/context.c",           targetDirectory: "grlib"},
    {path: mspWarePath + "/grlib/grlib/display.c",           targetDirectory: "grlib"},
    {path: mspWarePath + "/grlib/grlib/grlib.h",             targetDirectory: "grlib"},
    {path: mspWarePath + "/grlib/grlib/image.c",             targetDirectory: "grlib"},
    {path: mspWarePath + "/grlib/grlib/imageButton.c",       targetDirectory: "grlib"},
    {path: mspWarePath + "/grlib/grlib/imageButton.h",       targetDirectory: "grlib"},
    {path: mspWarePath + "/grlib/grlib/line.c",              targetDirectory: "grlib"},
    {path: mspWarePath + "/grlib/grlib/radioButton.c",       targetDirectory: "grlib"},
    {path: mspWarePath + "/grlib/grlib/radioButton.h",       targetDirectory: "grlib"},
    {path: mspWarePath + "/grlib/grlib/rectangle.c",         targetDirectory: "grlib"},
    {path: mspWarePath + "/grlib/grlib/string.c",            targetDirectory: "grlib"},
];

/*
 *
 ********* EMPTY EXAMPLES **********
 * The fileList files reside in the board directory for the empty projects since
 * they are board specific.
 *
 */
var allExamples = [
    {
        title: "Empty Project",
        name: "empty",
        description: "An empty TI-RTOS project",
        cFile: "empty.c",
        cfgFile: "empty.cfg",
        readme: "empty_readme.txt",
        fileList: [],
        requiredProducts: [],
        options: "NPW",
        type: "example",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C123GXL,
            Boards.EK_TM4C1294XL,
            Boards.CC1350DK,
            Boards.CC2650DK,
            Boards.CC2650STK_BLE,
            Boards.MSP_EXP432P401RLP,
            Boards.MSP_EXP430F5529LP,
            Boards.MSP_EXP430F5529,
            Boards.MSP_EXP430FR5969LP
        ],
    },
    {
        title: "Empty Project",
        name: "empty",
        description: "An empty TI-RTOS project",
        cFile: "empty.c",
        cfgFile: "empty.cfg",
        readme: "empty_readme.txt",
        fileList: [{path: "empty.pinmux"},
                   {path: "empty_rom_pin_mux_config.c"},
                   {path: "pin_mux_config.h"},
                ],
        requiredProducts: [],
        options: "NPW",
        type: "example",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.CC3200_LP,
        ],
    },
    {
        title: "Empty (Minimal) Project",
        name: "empty_min",
        description: "An empty TI-RTOS project with minimal footprint",
        cFile: "empty_min.c",
        cfgFile: "empty_min.cfg",
        readme: "empty_min_readme.txt",
        fileList: [],
        requiredProducts: [],
        options: "NPW",
        type: "example",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C123GXL,
            Boards.EK_TM4C1294XL,
            Boards.CC1350DK,
            Boards.CC2650DK,
            Boards.CC2650STK_BLE,
            Boards.MSP_EXP432P401RLP,
            Boards.MSP_EXP430F5529LP,
            Boards.MSP_EXP430F5529,
            Boards.MSP_EXP430FR5969LP,
        ],
    },
    {
        title: "Empty (Minimal) Project",
        name: "empty_min",
        description: "An empty TI-RTOS project with minimal footprint",
        cFile: "empty_min.c",
        cfgFile: "empty_min.cfg",
        readme: "empty_min_readme.txt",
        fileList: [{path: "empty_min.pinmux"},
                   {path: "empty_min_rom_pin_mux_config.c"},
                   {path: "pin_mux_config.h"},
                ],
        requiredProducts: [],
        options: "NPW",
        type: "example",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.CC3200_LP,
        ],
    },

   /*
    *
    ********* Ethernet EXAMPLES **********
    *
    */
    {
        title: "TCP Echo",
        name: "tcpEcho",
        description: "The TCP Echo example showing how to use TCP sockets",
        root: "./source/tcpEcho/",
        cFile: "tcpEcho.c",
        cfgFile: "tcpEcho.cfg",
        fileList:[
            {path: "source/tcpEcho/tcpEchoHooks.c"},
            {path: "source/tcpEcho/tcpEcho_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd ",
            TI_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd ",
            IAR_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd ",
            GNU: " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd -D_POSIX_SOURCE ",
            GNU_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                + "-D_POSIX_SOURCE ",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "Ethernet",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "TCP Echo IPv6",
        name: "tcpEchoIPv6",
        description: "The TCP Echo example showing how to use TCP sockets for IPv6",
        root: "./source/tcpEchoIPv6/",
        cFile: "tcpEchoIPv6.c",
        cfgFile: "tcpEchoIPv6.cfg",
        fileList:[
            {path: "source/tcpEchoIPv6/tcpEchoHooksIPv6.c"},
            {path: "source/tcpEchoIPv6/tcpEchoIPv6_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc "
                + "-I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd "
                + " -D_INCLUDE_IPv6_CODE ",
            TI_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc "
                + " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                + " -D_INCLUDE_IPv6_CODE ",
            IAR_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc "
                + " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                + " -D_INCLUDE_IPv6_CODE ",
            GNU: " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc "
                + "-I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd "
                + " -D_INCLUDE_IPv6_CODE -D_POSIX_SOURCE ",
            GNU_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc "
                + " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                + " -D_INCLUDE_IPv6_CODE -D_POSIX_SOURCE ",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "Ethernet",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "UDP Echo",
        name: "udpEcho",
        description: "The UDP Echo example showing how to use UDP sockets",
        root: "./source/udpEcho/",
        cFile: "udpEcho.c",
        cfgFile: "udpEcho.cfg",
        fileList: [
            {path: "source/udpEcho/udpEchoHooks.c"},
            {path: "source/udpEcho/udpEcho_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd ",
            TI_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd ",
            IAR_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd ",
            GNU: " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd -D_POSIX_SOURCE ",
            GNU_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                + "-D_POSIX_SOURCE ",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "Ethernet",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "UDP Echo IPv6",
        name: "udpEchoIPv6",
        description: "The UDP Echo example showing how to use UDP sockets for IPv6",
        root: "./source/udpEchoIPv6/",
        cFile: "udpEchoIPv6.c",
        cfgFile: "udpEchoIPv6.cfg",
        fileList:[
            {path: "source/udpEchoIPv6/udpEchoHooksIPv6.c"},
            {path: "source/udpEchoIPv6/udpEchoIPv6_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc "
                + "-I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd "
                + " -D_INCLUDE_IPv6_CODE ",
            TI_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc "
                + " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                + " -D_INCLUDE_IPv6_CODE ",
            IAR_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc "
                + " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                + " -D_INCLUDE_IPv6_CODE ",
            GNU: " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc "
                + "-I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd "
                + " -D_INCLUDE_IPv6_CODE -D_POSIX_SOURCE ",
            GNU_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc "
                + " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                + " -D_INCLUDE_IPv6_CODE -D_POSIX_SOURCE ",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "Ethernet",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "TCP Echo",
        name: "tcpEcho",
        description: "The TCP Echo example showing how to use TCP sockets",
        root: "./source/tcpEcho/",
        cFile: "tcpEcho.c",
        cfgFile: "tcpEcho.cfg",
        fileList:[
            {path: "source/tcpEcho/tcpEchoHooks.c"},
            {path: "source/tcpEcho/tcpEcho_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSC2000_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd ",
            TI_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd ",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "Ethernet",
        tools: ["TI"],
        boards:[
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3
        ],
    },
    {
        title: "TCP Echo IPv6",
        name: "tcpEchoIPv6",
        description: "The TCP Echo example showing how to use TCP sockets for IPv6",
        root: "./source/tcpEchoIPv6/",
        cFile: "tcpEchoIPv6.c",
        cfgFile: "tcpEchoIPv6.cfg",
        fileList:[
            {path: "source/tcpEchoIPv6/tcpEchoHooksIPv6.c"},
            {path: "source/tcpEchoIPv6/tcpEchoIPv6_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSC2000_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc "
                + "-I${COM_TI_RTSC_TIRTOSC2000_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd "
                + " -D_INCLUDE_IPv6_CODE ",
            TI_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc "
                + " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                + " -D_INCLUDE_IPv6_CODE ",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "Ethernet",
        tools: ["TI"],
        boards:[
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3
        ],
    },
    {
        title: "UDP Echo",
        name: "udpEcho",
        description: "The UDP Echo example showing how to use UDP sockets",
        root: "./source/udpEcho/",
        cFile: "udpEcho.c",
        cfgFile: "udpEcho.cfg",
        fileList: [
            {path: "source/udpEcho/udpEchoHooks.c"},
            {path: "source/udpEcho/udpEcho_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSC2000_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd ",
            TI_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd ",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "Ethernet",
        tools: ["TI"],
        boards:[
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3
        ],
    },
    {
        title: "UDP Echo IPv6",
        name: "udpEchoIPv6",
        description: "The UDP Echo example showing how to use UDP sockets for IPv6",
        root: "./source/udpEchoIPv6/",
        cFile: "udpEchoIPv6.c",
        cfgFile: "udpEchoIPv6.cfg",
        fileList:[
            {path: "source/udpEchoIPv6/udpEchoHooksIPv6.c"},
            {path: "source/udpEchoIPv6/udpEchoIPv6_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSC2000_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc "
                + "-I${COM_TI_RTSC_TIRTOSC2000_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd "
                + " -D_INCLUDE_IPv6_CODE ",
            TI_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc "
                + " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                + " -D_INCLUDE_IPv6_CODE ",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "Ethernet",
        tools: ["TI"],
        boards:[
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3
        ],
    },
    {
        title: "TCP Echo with TLS",
        name: "tcpEchoTLS",
        description: "The TCP Echo example showing how to use TCP sockets "
                     + "with TLS ",
        root: "./source/tcpEchoTLS/",
        cFile: "tcpEchoTLS.c",
        cfgFile: "tcpEchoTLS.cfg",
        fileList:[
            {path: "source/tcpEchoTLS/tcpEchoHooks.c"},
            {path: "source/tcpEchoTLS/tcpEchoTLS_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -DCYASSL_TIRTOS "
                + "-I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd ",
            TI_makefile: " -DCYASSL_TIRTOS "
                         + "-I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd ",
            IAR_makefile: " -DCYASSL_TIRTOS "
                          + "-I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd ",
            GNU: " -DCYASSL_TIRTOS "
                + "-I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd -D_POSIX_SOURCE ",
            GNU_makefile: " -DCYASSL_TIRTOS "
                          + "-I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                          + "-D_POSIX_SOURCE ",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "Ethernet",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "HTTP Client GET",
        name: "httpget",
        description: "The HTTP Client GET example showing how to use "
                     + "HTTP Client stack ",
        root: "./source/httpget/",
        cFile: "httpget.c",
        cfgFile: "httpget.cfg",
        fileList:[
            {path: "source/httpget/httpget_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -D__NDK__ "
                + "-I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd ",
            TI_makefile: " -D__NDK__ "
                         + "-I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd ",
            IAR_makefile: " -D__NDK__ "
                          + "-I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd ",
            GNU: " -D__NDK__ "
                + "-I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd -D_POSIX_SOURCE ",
            GNU_makefile: " -D__NDK__ "
                          + "-I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                          + "-D_POSIX_SOURCE ",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "Ethernet",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "HTTPS Client GET",
        name: "httpsget",
        description: "The HTTPS Client GET example showing how to use "
                     + "HTTP Client stack with TLS ",
        root: "./source/httpsget/",
        cFile: "httpsget.c",
        cfgFile: "httpsget.cfg",
        fileList:[
            {path: "source/httpsget/httpsget_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -D__NDK__ -D__CYASSL__ -DCYASSL_TIRTOS "
                + "-I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd ",
            TI_makefile: " -D__NDK__ -D__CYASSL__ -DCYASSL_TIRTOS "
                         + "-I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd ",
            IAR_makefile: " -D__NDK__ -D__CYASSL__ -DCYASSL_TIRTOS "
                          + "-I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd ",
            GNU: " -D__NDK__ -D__CYASSL__ -DCYASSL_TIRTOS "
                + "-I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd -D_POSIX_SOURCE ",
            GNU_makefile: " -D__NDK__ -D__CYASSL__ -DCYASSL_TIRTOS "
                          + "-I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                          + "-D_POSIX_SOURCE ",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "Ethernet",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.EK_TM4C1294XL
        ],
    },

   /*
    *
    ********* FatFs EXAMPLES **********
    *
    */
    {
        title: "FatSD",
        name: "fatsd",
        description: "An example using an SDcard",
        root: "./source/fatsd/",
        cFile: "fatsd.c",
        cfgFile: "fatsd.cfg",
        fileList: [
            {path: "source/fatsd/fatsd_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "FatFs",
        tools: ["TI"],
        boards:[
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "FatSD",
        name: "fatsd",
        description: "An example using an SDcard",
        root: "./source/fatsd/",
        cFile: "fatsd.c",
        cfgFile: "fatsd_msp432.cfg",
        fileList: [
            {path: "source/fatsd/fatsd_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "FatFs",
        tools: ["TI"],
        boards:[Boards.MSP_EXP432P401RLP],
    },
    {
        title: "FatSD",
        name: "fatsd",
        description: "An example using an SDcard",
        root: "./source/fatsd/",
        cFile: "fatsd.c",
        cfgFile: "fatsd_cc3200_lp.cfg",
        fileList: [
            {path: "source/fatsd/fatsd_readme.txt"},
            {path: "boards/CC3200_LP/fatsd.pinmux"},
            {path: "boards/CC3200_LP/fatsd_rom_pin_mux_config.c"},
            {path: "boards/CC3200_LP/pin_mux_config.h"},
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "FatFs",
        tools: ["TI"],
        boards:[
            Boards.CC3200_LP,
        ],
    },
    {
        title: "FatSD",
        name: "fatsd",
        description: "An example using an SDcard",
        root: "./source/fatsd/",
        cFile: "fatsd.c",
        cfgFile: "fatsd_msp430.cfg",
        fileList: [
            {path: "source/fatsd/fatsd_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "FatFs",
        tools: ["TI"],
        boards:[
            Boards.MSP_EXP430F5529LP,
            Boards.MSP_EXP430F5529,
            Boards.MSP_EXP430FR5969LP
        ],
    },
    {
        title: "FatSD Raw",
        name: "fatsdraw",
        description: "An example using an SDcard with FatFs API calls",
        root: "./source/fatsdraw/",
        cFile: "fatsdraw.c",
        cfgFile: "fatsd.cfg",
        fileList:[
            {path: "source/fatsdraw/fatsdraw_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "FatFs",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "FatSD Raw",
        name: "fatsdraw",
        description: "An example using an SDcard with FatFs API calls",
        root: "./source/fatsdraw/",
        cFile: "fatsdraw.c",
        cfgFile: "fatsd_msp432.cfg",
        fileList:[
            {path: "source/fatsdraw/fatsdraw_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "FatFs",
        tools: ["TI", "IAR", "GNU"],
        boards:[Boards.MSP_EXP432P401RLP],
    },
    {
        title: "FatSD Raw",
        name: "fatsdraw",
        description: "An example using an SDcard with FatFs API calls",
        root: "./source/fatsdraw/",
        cFile: "fatsdraw.c",
        cfgFile: "fatsd_cc3200_lp.cfg",
        fileList:[
            {path: "source/fatsdraw/fatsdraw_readme.txt"},
            {path: "boards/CC3200_LP/fatsd_raw.pinmux"},
            {path: "boards/CC3200_LP/fatsd_raw_rom_pin_mux_config.c"},
            {path: "boards/CC3200_LP/pin_mux_config.h"},
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "FatFs",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.CC3200_LP,
        ],
    },
    {
        title: "FatSD Raw",
        name: "fatsdraw",
        description: "An example using an SDcard with FatFs API calls",
        root: "./source/fatsdraw/",
        cFile: "fatsdraw.c",
        cfgFile: "fatsd_msp430.cfg",
        fileList:[
            {path: "source/fatsdraw/fatsdraw_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "FatFs",
        tools: ["TI", "IAR"],
        boards:[
            Boards.MSP_EXP430F5529LP,
            Boards.MSP_EXP430F5529,
            Boards.MSP_EXP430FR5969LP
        ],
    },
    {
        title: "FatSD USB Copy",
        name: "fatsdusbcopy",
        description: "An example copying a file from a SD Card to a USB Drive",
        root: "./source/fatsdusbcopy/",
        cFile: "fatsdusbcopy.c",
        cfgFile: "fatsdusbcopy.cfg",
        fileList:[
            {path: "source/fatsdusbcopy/fatsdusbcopy_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "USB",
        tools: ["TI"],
        boards:[
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },

   /*
    *
    ********* GPIO EXAMPLES **********
    *
    */
    {
        title: "GPIO Interrupt",
        name: "gpiointerrupt",
        description: "An example using interrupts to toggle an LED after a button press",
        root: "./source/gpiointerrupt/",
        cFile: "gpiointerrupt.c",
        cfgFile: "gpiointerrupt.cfg",
        fileList:[
            {path: "source/gpiointerrupt/gpiointerrupt_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "GPIO",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "GPIO Interrupt",
        name: "gpiointerrupt",
        description: "An example using interrupts to toggle an LED after a button press",
        root: "./source/gpiointerrupt/",
        cFile: "gpiointerrupt.c",
        cfgFile: "gpiointerrupt_msp432.cfg",
        fileList:[
            {path: "source/gpiointerrupt/gpiointerrupt_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "GPIO",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.MSP_EXP432P401RLP
        ],
    },
    {
        title: "GPIO Interrupt",
        name: "gpiointerrupt",
        description: "An example using interrupts to toggle an LED after a button press",
        root: "./source/gpiointerrupt/",
        cFile: "gpiointerrupt.c",
        cfgFile: "gpiointerrupt_cc3200_lp.cfg",
        fileList:[
            {path: "source/gpiointerrupt/gpiointerrupt_readme.txt"},
            {path: "boards/CC3200_LP/gpiointerrupt.pinmux"},
            {path: "boards/CC3200_LP/gpiointerrupt_rom_pin_mux_config.c"},
            {path: "boards/CC3200_LP/pin_mux_config.h"},
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "GPIO",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.CC3200_LP,
        ],
    },
    {
        title: "GPIO Interrupt",
        name: "gpiointerrupt",
        description: "An example using interrupts to toggle an LED after a button press",
        root: "./source/gpiointerrupt/",
        cFile: "gpiointerrupt.c",
        cfgFile: "gpiointerrupt_msp430f5529.cfg",
        fileList: [
            {path: "source/gpiointerrupt/gpiointerrupt_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "GPIO",
        tools: ["TI", "IAR"],
        boards:[
            Boards.MSP_EXP430F5529LP,
            Boards.MSP_EXP430F5529,
        ],
    },
    {
        title: "GPIO Interrupt",
        name: "gpiointerrupt",
        description: "An example using interrupts to toggle an LED after a button press",
        root: "./source/gpiointerrupt/",
        cFile: "gpiointerrupt.c",
        cfgFile: "gpiointerrupt_msp430fr5969.cfg",
        fileList: [
            {path: "source/gpiointerrupt/gpiointerrupt_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "GPIO",
        tools: ["TI", "IAR"],
        boards:[
            Boards.MSP_EXP430FR5969LP,
        ],
    },

   /*
    *
    ********* PPP EXAMPLES **********
    *
    */
    {
        title: "TCP Echo PPP",
        name: "tcpEchoPPP",
        description: "The TCP Echo example showing how to use TCP sockets on a NULL modem PPP connection",
        root: "./source/tcpEchoPPP/",
        cFile: "tcpEchoPPP.c",
        cfgFile: "tcpEchoPPP.cfg",
        fileList:[
            {path: "source/tcpEchoPPP/tcpEchoHooks.c"},
            {path: "source/tcpEchoPPP/llserial.h"},
            {path: "source/tcpEchoPPP/llserial.c"},
            {path: "source/tcpEchoPPP/usbserialppp.c"},
            {path: "source/usb/USBCDCD.h"},
            {path: "source/usb/USBCDCD.c"},
            {path: "source/tcpEchoPPP/tcpEchoPPP_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc "
                + "-I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd -DTI_EXAMPLES_PPP=1 ",
            TI_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc "
                + "-I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                + "-DTI_EXAMPLES_PPP=1 ",
            IAR_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc "
                + "-I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                + "-DTI_EXAMPLES_PPP=1 ",
            GNU: " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc "
                + "-I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/products/"
                + NDKVer + "/packages/ti/ndk/inc/bsd -D_POSIX_SOURCE "
                + "-DTI_EXAMPLES_PPP=1 ",
            GNU_makefile: " -I$(NDK_INSTALLATION_DIR)/ti/ndk/inc "
                + "-I$(NDK_INSTALLATION_DIR)/ti/ndk/inc/bsd "
                + "-D_POSIX_SOURCE -DTI_EXAMPLES_PPP=1 ",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "PPP",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL,
        ],
    },

   /*
    *
    ********* SPI EXAMPLES **********
    *
    */
    {
        title: "SPI Loopback",
        name: "spiloopback",
        description: "An example that performs an external SPI loopback with master and slave SPI",
        root: "./source/spiloopback/",
        cFile: "spiloopback.c",
        cfgFile: "spiloopback.cfg",
        fileList: [
            {path: "source/spiloopback/spiloopback_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "SPI",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "SPI Loopback",
        name: "spiloopback",
        description: "An example that performs an external SPI loopback with master and slave SPI",
        root: "./source/spiloopback/",
        cFile: "spiloopback.c",
        cfgFile: "spiloopback_msp432.cfg",
        fileList: [
            {path: "source/spiloopback/spiloopback_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "SPI",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.MSP_EXP432P401RLP
        ],
    },
//    {
//        title: "SPI AT45DB161D",
//        name: "spi_at45db161d",
//        description: "This example uses an Atmel AT45Db161D SPI NAND Flash. It performs a page write and read using the AT4DB SPI driver",
//        root: "./source/spi_at45db161d/",
//        cFile: "spi_at45db161d.c",
//        cfgFile: "spi_at45db161d.cfg",
//        fileList: [
//            {path: "source/spi_at45db161d/spi_at45db161d_readme.txt"},
//            {path: "source/spi_at45db161d/AT45DB.h"},
//            {path: "source/spi_at45db161d/AT45DB.c"}
//        ],
//        requiredProducts: [],
//        options: "TREX",
//        type: "example",
//        group: "SPI",
//        tools: ["TI", "IAR"],
//        boards:[
//            Boards.MSP_EXP430F5529LP
//        ],
//    },
//    {
//        title: "SPI AT45DB161D FatFs",
//        name: "spi_at45db161dFatFs",
//        description: "This example uses an Atmel AT45Db161D SPI NAND Flash. It uses the AT45DBFatFs driver to implement a FatFs file system for creating, reading and writing files.",
//        root: "./source/spi_at45db161dFatfs/",
//        cFile: "spi_at45db161dFatFs.c",
//        cfgFile: "spi_at45db161dFatFs.cfg",
//        fileList: [
//            {path: "source/spi_at45db161dFatFs/spi_at45db161dFatFs_readme.txt"},
//            {path: "source/spi_at45db161d/AT45DB.h"},
//            {path: "source/spi_at45db161d/AT45DB.c"},
//            {path: "source/spi_at45db161dFatFs/AT45DBFatFs.h"},
//            {path: "source/spi_at45db161dFatFs/AT45DBFatFs.c"},
//        ],
//        requiredProducts: [],
//        options: "TREX",
//        type: "example",
//        group: "SPI",
//        tools: ["TI", "IAR"],
//        boards:[
//            Boards.MSP_EXP430F5529LP
//        ],
//    },

   /*
    *
    ********* I2C EXAMPLES **********
    *
    */
    {
        title: "I2C EEPROM",
        name: "i2ceeprom",
        description: "An example that performs page reads and writes to the onboard EEPROM",
        root: "./source/i2ceeprom/",
        cFile: "i2ceeprom.c",
        cfgFile: "i2ceeprom.cfg",
        fileList: [
            {path: "source/i2ceeprom/i2ceeprom_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2C",
        tools: ["TI"],
        boards:[
            Boards.TMDXDOCKH52C1_M3
        ],
    },
    {
        title: "I2C TPL0401EVM",
        name: "i2ctpl0401evm",
        description: "An example on how to use the I2C driver with the TPL0401EVM boosterpack",
        root: "./source/i2ctpl0401evm/",
        cFile: "i2ctpl0401evm.c",
        cfgFile: "i2ctpl0401evm.cfg",
        fileList: [
            {path: "source/i2ctpl0401evm/i2ctpl0401evm_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2C",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "I2C TPL0401EVM",
        name: "i2ctpl0401evm",
        description: "An example on how to use the I2C driver with the TPL0401EVM boosterpack",
        root: "./source/i2ctpl0401evm/",
        cFile: "i2ctpl0401evm.c",
        cfgFile: "i2ctpl0401evm_msp432.cfg",
        fileList: [
            {path: "source/i2ctpl0401evm/i2ctpl0401evm_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2C",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.MSP_EXP432P401RLP
        ],
    },
    {
        title: "I2C TPL0401EVM",
        name: "i2ctpl0401evm",
        description: "An example on how to use the I2C driver with the TPL0401EVM boosterpack",
        root: "./source/i2ctpl0401evm/",
        cFile: "i2ctpl0401evm.c",
        cfgFile: "i2ctpl0401evm_msp430f5529.cfg",
        fileList: [
            {path: "source/i2ctpl0401evm/i2ctpl0401evm_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2C",
        tools: ["TI", "IAR"],
        boards:[
            Boards.MSP_EXP430F5529LP
        ],
    },
    {
        title: "I2C TPL0401EVM",
        name: "i2ctpl0401evm",
        description: "An example on how to use the I2C driver with the TPL0401EVM boosterpack",
        root: "./source/i2ctpl0401evm/",
        cFile: "i2ctpl0401evm.c",
        cfgFile: "i2ctpl0401evm_msp430fr5969.cfg",
        fileList: [
            {path: "source/i2ctpl0401evm/i2ctpl0401evm_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2C",
        tools: ["TI", "IAR"],
        boards:[
            Boards.MSP_EXP430FR5969LP
        ],
    },
    {
        title: "I2C TMP006",
        name: "i2ctmp006",
        description: "An example on how to use the I2C driver with the TMP006 boosterpack",
        root: "./source/i2ctmp006/",
        cFile: "i2ctmp006.c",
        cfgFile: "i2ctmp006.cfg",
        fileList: [
            {path: "source/i2ctmp006/i2ctmp006_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2C",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "I2C TMP006",
        name: "i2ctmp006",
        description: "An example on how to use the I2C driver with the TMP006 boosterpack",
        root: "./source/i2ctmp006/",
        cFile: "i2ctmp006.c",
        cfgFile: "i2ctmp006_cc3200_lp.cfg",
        fileList: [
            {path: "source/i2ctmp006/i2ctmp006_readme.txt"},
            {path: "boards/CC3200_LP/i2ctmp006.pinmux"},
            {path: "boards/CC3200_LP/i2ctmp006_rom_pin_mux_config.c"},
            {path: "boards/CC3200_LP/pin_mux_config.h"},
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2C",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.CC3200_LP,
        ],
    },
    {
        title: "I2C TMP006",
        name: "i2ctmp006",
        description: "An example on how to use the I2C driver with the TMP006 boosterpack",
        root: "./source/i2ctmp006/",
        cFile: "i2ctmp006_cc26xx.c",
        cfgFile: "i2ctmp006_cc26xx.cfg",
        fileList: [
            {path: "source/i2ctmp006/i2ctmp006_readme.txt"},
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2C",
        tools: ["TI", "IAR", "GNU"],
        boards: [
            Boards.CC2650STK_BLE,
        ],
    },
    {
        title: "I2C TMP006",
        name: "i2ctmp006",
        description: "An example on how to use the I2C driver with the TMP006 boosterpack",
        root: "./source/i2ctmp006/",
        cFile: "i2ctmp006.c",
        cfgFile: "i2ctmp006_msp432.cfg",
        fileList: [
            {path: "source/i2ctmp006/i2ctmp006_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2C",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.MSP_EXP432P401RLP
        ],
    },
    {
        title: "I2C TMP006",
        name: "i2ctmp006",
        description: "An example on how to use the I2C driver with the TMP006 boosterpack",
        root: "./source/i2ctmp006/",
        cFile: "i2ctmp006.c",
        cfgFile: "i2ctmp006_msp430f5529.cfg",
        fileList: [
            {path: "source/i2ctmp006/i2ctmp006_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2C",
        tools: ["TI", "IAR"],
        boards:[
            Boards.MSP_EXP430F5529LP
        ],
    },
    {
        title: "I2C TMP006",
        name: "i2ctmp006",
        description: "An example on how to use the I2C driver with the TMP006 boosterpack",
        root: "./source/i2ctmp006/",
        cFile: "i2ctmp006.c",
        cfgFile: "i2ctmp006_msp430fr5969.cfg",
        fileList: [
            {path: "source/i2ctmp006/i2ctmp006_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2C",
        tools: ["TI", "IAR"],
        boards:[
            Boards.MSP_EXP430FR5969LP
        ],
    },
    {
        title: "I2C RF430CL330 Load",
        name: "i2crf430cl330_load",
        description: "An example NFC RF430CL330 example that sends the CPU Load",
        root: "./source/i2crf430cl330_load/",
        cFile: "i2crf430cl330_load.c",
        cfgFile: "i2crf430cl330_load.cfg",
        fileList: [
            {path: "source/i2crf430cl330_load/i2crf430cl330_load_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2C",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "I2C RF430CL330 Load",
        name: "i2crf430cl330_load",
        description: "An example NFC RF430CL330 example that sends the CPU Load",
        root: "./source/i2crf430cl330_load/",
        cFile: "i2crf430cl330_load.c",
        cfgFile: "i2crf430cl330_load_msp432.cfg",
        fileList: [
            {path: "source/i2crf430cl330_load/i2crf430cl330_load_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2C",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.MSP_EXP432P401RLP
        ],
    },
    {
        title: "I2C RF430CL330 Load",
        name: "i2crf430cl330_load",
        description: "An example NFC RF430CL330 example that sends the CPU Load",
        root: "./source/i2crf430cl330_load/",
        cFile: "i2crf430cl330_load.c",
        cfgFile: "i2crf430cl330_load_msp430f5529.cfg",
        fileList: [
            {path: "source/i2crf430cl330_load/i2crf430cl330_load_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2C",
        tools: ["TI", "IAR"],
        boards:[
            Boards.MSP_EXP430F5529LP,
            Boards.MSP_EXP430F5529
        ],
    },
    {
        title: "I2C RF430CL330 Load",
        name: "i2crf430cl330_load",
        description: "An example NFC RF430CL330 example that sends the CPU Load",
        root: "./source/i2crf430cl330_load/",
        cFile: "i2crf430cl330_load.c",
        cfgFile: "i2crf430cl330_load_msp430fr5969.cfg",
        fileList: [
            {path: "source/i2crf430cl330_load/i2crf430cl330_load_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2C",
        tools: ["TI", "IAR"],
        boards:[
            Boards.MSP_EXP430FR5969LP
        ],
    },

   /*
    *
    ********* I2S EXAMPLES **********
    *
    */
    {
        title: "I2S Echo",
        name: "i2secho",
        description: "An example that performs audio echo on the CC3200AUDBOOST boosterpack",
        root: "./source/i2secho/",
        cFile: "i2secho.c",
        cfgFile: "i2secho_cc3200_lp.cfg",
        fileList: [
            {path: "source/i2secho/i2secho_readme.txt"},
            {path: "source/i2secho/ti_codec.c"},
            {path: "source/i2secho/ti_codec.h"},
            {path: "boards/CC3200_LP/i2secho.pinmux"},
            {path: "boards/CC3200_LP/i2secho_rom_pin_mux_config.c"},
            {path: "boards/CC3200_LP/pin_mux_config.h"},
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "I2S",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.CC3200_LP
        ],
    },

   /*
    *
    ********* LCD EXAMPLES **********
    *
    */
    {
        title: "LCD SmartRF06EB",
        name: "lcdSmartRF06EB",
        description: "An example demonstrating how to use the LCD on the SmartRF06 Evaluation Board",
        root: "./source/lcdSmartRF06EB/",
        cFile: "lcdSmartRF06EB.c",
        cfgFile: "lcdSmartRF06EB.cfg",
        fileList: [
            {path: "source/lcdSmartRF06EB/lcdSmartRF06EB_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "LCD",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.CC1350DK,
            Boards.CC2650DK
        ],
    },

   /*
    *
    ********* PIN EXAMPLES **********
    *
    */
    {
        title: "Pin Interrupt",
        name: "pinInterrupt",
        description: "An example using interrupts to toggle an LED after a button press",
        root: "./source/pinInterrupt/",
        cFile: "pinInterrupt.c",
        cfgFile: "pinInterrupt.cfg",
        fileList: [
            {path: "source/pinInterrupt/pinInterrupt_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "PIN",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.CC1350DK,
            Boards.CC2650DK,
            Boards.CC2650STK_BLE
        ],
    },

    /*
     *
     ********* PWM EXAMPLES **********
     *
    */
    {
        title: "PWM LED",
        name: "pwmled",
        description: "An example on how to use the PWM driver to control onboard LEDs",
        root: "./source/pwmled/",
        cFile: "pwmled.c",
        cfgFile: "pwmled.cfg",
        fileList: [
            {path: "source/pwmled/pwmled_readme.txt"},
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "PWM",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.DK_TM4C129X,
            Boards.EK_TM4C123GXL,
            Boards.EK_TM4C1294XL,
            // Boards.MSP_EXP432P401RLP
        ],
    },
    {
        title: "PWM LED",
        name: "pwmled",
        description: "An example on how to use the PWM driver to control onboard LEDs",
        root: "./source/pwmled/",
        cFile: "pwmled.c",
        cfgFile: "pwmled_msp432.cfg",
        fileList: [
            {path: "source/pwmled/pwmled_readme.txt"},
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "PWM",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.MSP_EXP432P401RLP
        ],
    },
    {
        title: "PWM LED",
        name: "pwmled",
        description: "An example on how to use the PWM driver to control onboard LEDs",
        root: "./source/pwmled/",
        cFile: "pwmled.c",
        cfgFile: "pwmled_cc3200_lp.cfg",
        fileList: [
            {path: "source/pwmled/pwmled_readme.txt"},
            {path: "boards/CC3200_LP/pwmled.pinmux"},
            {path: "boards/CC3200_LP/pwmled_rom_pin_mux_config.c"},
            {path: "boards/CC3200_LP/pin_mux_config.h"},
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "PWM",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.CC3200_LP,
        ],
    },

    /*
     *
     ********* UART EXAMPLES **********
     *
     */
    {
        title: "UART Console",
        name: "uartconsole",
        description: "A basic console implemented with the UART driver and cstdio",
        root: "./source/uartconsole/",
        cFile: "uartconsole.c",
        cfgFile: "uartconsole.cfg",
        fileList: [
            {path: "source/uartconsole/uartconsole_readme.txt"},
            {path: "source/uart/UARTUtils.c"},
            {path: "source/uart/UARTUtils.h"},
            {path: "source/usb/USBCDCD_LoggerIdle.c"},
            {path: "source/usb/USBCDCD_LoggerIdle.h"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "UART",
        tools: ["TI"],
        boards: [
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL,
        ],
    },
    {
        title: "UART Echo",
        name: "uartecho",
        description: "Echos characters back using the UART driver",
        root: "./source/uartecho/",
        cFile: "uartecho.c",
        cfgFile: "uartecho.cfg",
        fileList: [
            {path: "source/uartecho/uartecho_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "UART",
        tools: ["TI", "IAR", "GNU"],
        boards: [
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "UART Echo",
        name: "uartecho",
        description: "Echos characters back using the UART driver",
        root: "./source/uartecho/",
        cFile: "uartecho_cc26xx.c",
        cfgFile: "uartecho_cc26xx.cfg",
        fileList: [
            {path: "source/uartecho/uartecho_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "UART",
        tools: ["TI", "IAR", "GNU"],
        boards: [
            Boards.CC1350DK,
            Boards.CC2650DK,
            Boards.CC2650STK_BLE
        ],
    },
    {
        title: "UART Echo",
        name: "uartecho",
        description: "Echos characters back using the UART driver",
        root: "./source/uartecho/",
        cFile: "uartecho.c",
        cfgFile: "uartecho_cc3200_lp.cfg",
        fileList: [
            {path: "source/uartecho/uartecho_readme.txt"},
            {path: "boards/CC3200_LP/uartecho_rom_pin_mux_config.c"},
            {path: "boards/CC3200_LP/uartecho.pinmux"},
            {path: "boards/CC3200_LP/pin_mux_config.h"},
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "UART",
        tools: ["TI", "IAR", "GNU"],
        boards: [
            Boards.CC3200_LP,
        ],
    },
    {
        title: "UART Echo",
        name: "uartecho",
        description: "Echos characters back using the UART driver",
        root: "./source/uartecho/",
        cFile: "uartecho.c",
        cfgFile: "uartecho_msp432.cfg",
        fileList: [
            {path: "source/uartecho/uartecho_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "UART",
        tools: ["TI", "IAR", "GNU"],
        boards: [
            Boards.MSP_EXP432P401RLP
        ],
    },
    {
        title: "UART Echo",
        name: "uartecho",
        description: "Echos characters back using the UART driver.",
        root: "./source/uartecho/",
        cFile: "uartecho.c",
        cfgFile: "uartecho_msp430f5529.cfg",
        fileList: [
            {path: "source/uartecho/uartecho_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "UART",
        tools: ["TI", "IAR"],
        boards: [
            Boards.MSP_EXP430F5529LP,
            Boards.MSP_EXP430F5529,
        ],
    },
    {
        title: "UART Echo",
        name: "uartecho",
        description: "Echos characters back using the UART driver.",
        root: "./source/uartecho/",
        cFile: "uartecho.c",
        cfgFile: "uartecho_msp430fr5969.cfg",
        fileList: [
            {path: "source/uartecho/uartecho_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "UART",
        tools: ["TI", "IAR"],
        boards: [
            Boards.MSP_EXP430FR5969LP
        ],
    },
    {
        title: "UART Logging",
        name: "uartlogging",
        description: "A simple example that sends Log data to the UART",
        root: "./source/uartlogging/",
        cFile: "uartlogging.c",
        cfgFile: "uartlogging.cfg",
        fileList: [
            {path: "source/uartlogging/uartlogging_readme.txt",  targetDirectory: "."},
            {path: "source/uart/UARTUtils.c"},
            {path: "source/uart/UARTUtils.h"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "UART",
        tools: ["TI", "GNU"],
        boards: [
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "UART Logging",
        name: "uartlogging",
        description: "A simple example that sends Log data to the UART",
        root: "./source/uartlogging/",
        cFile: "uartlogging.c",
        cfgFile: "uartlogging_cc3200_lp.cfg",
        fileList: [
            {path: "source/uartlogging/uartlogging_readme.txt",  targetDirectory: "."},
            {path: "source/uart/UARTUtils.c"},
            {path: "source/uart/UARTUtils.h"},
            {path: "boards/CC3200_LP/uartlogging.pinmux"},
            {path: "boards/CC3200_LP/uartlogging_rom_pin_mux_config.c"},
            {path: "boards/CC3200_LP/pin_mux_config.h"},
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "UART",
        tools: ["TI", "GNU"],
        boards: [
            Boards.CC3200_LP,
        ],
    },
    {
        title: "UART Logging",
        name: "uartlogging",
        description: "A simple example that sends Log data to the UART",
        root: "./source/uartlogging/",
        cFile: "uartlogging.c",
        cfgFile: "uartlogging_msp432.cfg",
        fileList: [
            {path: "source/uartlogging/uartlogging_readme.txt",  targetDirectory: "."},
            {path: "source/uart/UARTUtils.c"},
            {path: "source/uart/UARTUtils.h"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "UART",
        tools: ["TI", "GNU"],
        boards: [
            Boards.MSP_EXP432P401RLP
        ],
    },
    {
        title: "UART Logging",
        name: "uartlogging",
        description: "A simple example that sends Log data to the UART",
        root: "./source/uartlogging/",
        cFile: "uartlogging.c",
        cfgFile: "uartlogging_msp430f5529.cfg",
        fileList: [
            {path: "source/uartlogging/uartlogging_readme.txt",  targetDirectory: "."},
            {path: "source/uart/UARTUtils.c"},
            {path: "source/uart/UARTUtils.h"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "UART",
        tools: ["TI"],
        boards: [
            Boards.MSP_EXP430F5529LP,
            Boards.MSP_EXP430F5529,
        ],
    },
    {
        title: "UART Logging",
        name: "uartlogging",
        description: "A simple example that sends Log data to the UART",
        root: "./source/uartlogging/",
        cFile: "uartlogging.c",
        cfgFile: "uartlogging_msp430fr5969.cfg",
        fileList: [
            {path: "source/uartlogging/uartlogging_readme.txt",  targetDirectory: "."},
            {path: "source/uart/UARTUtils.c"},
            {path: "source/uart/UARTUtils.h"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "UART",
        tools: ["TI"],
        boards: [
            Boards.MSP_EXP430FR5969LP,
        ],
    },
    {
        title: "UART Console",
        name: "uartconsole",
        description: "A basic console implemented with the UART driver and cstdio",
        root: "./source/uartconsole/",
        cFile: "uartconsole.c",
        cfgFile: "uartconsole_msp430.cfg",
        fileList: msp430USBstackFiles.concat([
            {path: "source/uartconsole/uartconsole_readme.txt"},
            {path: "source/uart/UARTUtils.c"},
            {path: "source/uart/UARTUtils.h"},
            {path: "source/usbmsp430/cdclogging/USBCDCD_LoggerIdle.c"},
            {path: "source/usbmsp430/cdclogging/USBCDCD_LoggerIdle.h"},

            /* USB_config */
            {path: "source/usbmsp430/cdclogging/USB_config/descriptors.c",    targetDirectory: "USB_config"},
            {path: "source/usbmsp430/cdclogging/USB_config/descriptors.h",    targetDirectory: "USB_config"},
            {path: "source/usbmsp430/cdclogging/USB_config/UsbIsr.c",         targetDirectory: "USB_config"},
            {path: "source/usbmsp430/cdclogging/USB_config/MSP430_CDC.inf",   targetDirectory: "USB_config"},
        ]),
        compilerBuildOptions: {
            TI: " -I${workspace_loc:/${ProjName}} -I${workspace_loc:/${ProjName}/USB_config}",
            TI_makefile: " -I ./USB_config"
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "UART",
        tools: ["TI"],
        boards: [
            Boards.MSP_EXP430F5529LP,
            Boards.MSP_EXP430F5529
        ],
    },
    {
        title: "UART Power",
        name: "uartpower",
        description: "UART loopback example with power management",
        root: "./source/uartpower/",
        cFile: "uartpower.c",
        cfgFile: "uartpower.cfg",
        fileList: [
            {path: "source/uartpower/uartpower_readme.txt"},
            {path: "boards/CC3200_LP/uartpower_rom_pin_mux_config.c"},
            {path: "boards/CC3200_LP/uartpower.pinmux"},
            {path: "boards/CC3200_LP/pin_mux_config.h"},
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "UART",
        tools: ["TI", "IAR", "GNU"],
        boards: [
            Boards.CC3200_LP,
        ],
    },

    /*
     *
     ********* USB EXAMPLES **********
     *
     */
    {
        title: "USB Keyboard Device",
        name: "usbkeyboarddevice",
        description: "USB HID Keyboard Device example that simluates a keyboard",
        root: "./source/usbkeyboarddevice/",
        cFile: "usbkeyboarddevice.c",
        cfgFile: "usbkeyboarddevice.cfg",
        fileList: [
            {path: "source/usb/USBKBD.h"},
            {path: "source/usb/USBKBD.c"},
            {path: "source/usbkeyboarddevice/usbkeyboarddevice_readme.txt"}
        ],
        isHybrid: true,
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "USB",
        tools: ["TI", "IAR", "GNU"],
        boards: [
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "USB Mouse Device",
        name: "usbmousedevice",
        description: "USB HID Mouse Device example that moves a mouse cursor and simulates mouse clicks",
        root: "./source/usbmousedevice/",
        cFile: "usbmousedevice.c",
        cfgFile: "usbmousedevice.cfg",
        fileList: [
            {path: "source/usb/USBMD.h"},
            {path: "source/usb/USBMD.c"},
            {path: "source/usbmousedevice/usbmousedevice_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "USB",
        tools: ["TI", "IAR", "GNU"],
        boards: [
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "USB Serial Device",
        name: "usbserialdevice",
        description: "USB CDC Device that allows for serial communications using a virtual serial COM port",
        root: "./source/usbserialdevice/",
        cFile: "usbserialdevice.c",
        cfgFile: "usbserialdevice.cfg",
        fileList: [
            {path: "source/usb/USBCDCD.h"},
            {path: "source/usb/USBCDCD.c"},
            {path: "source/usbserialdevice/usbserialdevice_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "USB",
        tools: ["TI", "IAR", "GNU"],
        boards: [
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "USB Keyboard Host",
        name: "usbkeyboardhost",
        description: "USB HID Keyboard Host example that reads characters from a USB Keyboard",
        root: "./source/usbkeyboardhost/",
        cFile: "usbkeyboardhost.c",
        cfgFile: "usbkeyboardhost.cfg",
        fileList: [
            {path: "source/usb/USBKBH.h"},
            {path: "source/usb/USBKBH.c"},
            {path: "source/usbkeyboardhost/usbkeyboardhost_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "USB",
        tools: ["TI", "IAR", "GNU"],
        boards: [
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "USB Mouse Host",
        name: "usbmousehost",
        description: "USB HID Mouse Host example that displays the status of a USB Mouse",
        root: "./source/usbmousehost/",
        cFile: "usbmousehost.c",
        cfgFile: "usbmousehost.cfg",
        fileList: [
            {path: "source/usb/USBMH.h"},
            {path: "source/usb/USBMH.c"},
            {path: "source/usbmousehost/usbmousehost_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "USB",
        tools: ["TI", "IAR", "GNU"],
        boards: [
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "USB CDC Mouse Device",
        name: "usbcdcmousedevice",
        description: "USB composite example that features a CDC virtual serial port communications along with a HID Mouse device",
        root: "./source/usbcdcmousedevice/",
        cFile: "usbcdcmousedevice.c",
        cfgFile: "usbcdcmousedevice.cfg",
        fileList: [
            {path: "source/usb/USBCDCMOUSE.h"},
            {path: "source/usb/USBCDCMOUSE.c"},
            {path: "source/usbcdcmousedevice/usbcdcmousedevice_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "USB",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "USB Serial Device",
        name: "usbserialdevice",
        description: "USB CDC Device that allows for serial communications using a virtual serial COM port",
        root: "./source/usbserialdevice/",
        cFile: "usbserialdevice.c",
        cfgFile: "usbserialdevice_msp430.cfg",
        fileList: msp430USBstackFiles.concat([
            {path: "source/usbmsp430/cdc/USBCDCD.h"},
            {path: "source/usbmsp430/cdc/USBCDCD.c"},
            {path: "source/usbserialdevice/usbserialdevice_readme.txt"},

            /* USB_config */
            {path: "source/usbmsp430/cdc/USB_config/descriptors.c",    targetDirectory: "USB_config"},
            {path: "source/usbmsp430/cdc/USB_config/descriptors.h",    targetDirectory: "USB_config"},
            {path: "source/usbmsp430/cdc/USB_config/UsbIsr.c",         targetDirectory: "USB_config"},
            {path: "source/usbmsp430/cdc/USB_config/MSP430_CDC.inf",   targetDirectory: "USB_config"},
        ]),
        compilerBuildOptions: {
            TI: " -I${workspace_loc:/${ProjName}} -I${workspace_loc:/${ProjName}/USB_config}",
            TI_makefile: " -I ./USB_config",
            IAR_makefile: " -I ./USB_config"
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "USB",
        tools: ["TI", "IAR"],
        boards:[
            Boards.MSP_EXP430F5529LP,
            Boards.MSP_EXP430F5529
        ],
    },
    {
        title: "USB Mouse Device",
        name: "usbmousedevice",
        description: "USB HID Mouse Device example that moves a mouse cursor and simulates mouse clicks",
        root: "./source/usbmousedevice/",
        cFile: "usbmousedevice.c",
        cfgFile: "usbmousedevice_msp430.cfg",
        fileList: msp430USBstackFiles.concat([
            {path: "source/usbmsp430/hidm/USBMD.h"},
            {path: "source/usbmsp430/hidm/USBMD.c"},
            {path: "source/usbmousedevice/usbmousedevice_readme.txt"},

            /* USB_config */
            {path: "source/usbmsp430/hidm/USB_config/descriptors.c",   targetDirectory: "USB_config"},
            {path: "source/usbmsp430/hidm/USB_config/descriptors.h",   targetDirectory: "USB_config"},
            {path: "source/usbmsp430/hidm/USB_config/UsbIsr.c",        targetDirectory: "USB_config"},
        ]),
        compilerBuildOptions: {
            TI: " -I${workspace_loc:/${ProjName}} -I${workspace_loc:/${ProjName}/USB_config}",
            TI_makefile: " -I ./USB_config",
            IAR_makefile: " -I ./USB_config"
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "USB",
        tools: ["TI", "IAR"],
        boards:[
            Boards.MSP_EXP430F5529LP,
            Boards.MSP_EXP430F5529
        ],
    },
    {
        title: "USB Keyboard Device",
        name: "usbkeyboarddevice",
        description: "USB HID Keyboard Device example that simluates a keyboard",
        root: "./source/usbkeyboarddevice/",
        cFile: "usbkeyboarddevice.c",
        cfgFile: "usbkeyboarddevice_msp430.cfg",
        fileList: msp430USBstackFiles.concat([
            {path: "source/usbmsp430/hidk/USBKBD.h"},
            {path: "source/usbmsp430/hidk/USBKBD.c"},
            {path: "source/usbkeyboarddevice/usbkeyboarddevice_readme.txt"},

            /* USB_config */
            {path: "source/usbmsp430/hidk/USB_config/descriptors.c",   targetDirectory: "USB_config"},
            {path: "source/usbmsp430/hidk/USB_config/descriptors.h",   targetDirectory: "USB_config"},
            {path: "source/usbmsp430/hidk/USB_config/UsbIsr.c",        targetDirectory: "USB_config"},
        ]),
        compilerBuildOptions: {
            TI: " -I${workspace_loc:/${ProjName}} -I${workspace_loc:/${ProjName}/USB_config}",
            TI_makefile: " -I ./USB_config",
            IAR_makefile: " -I ./USB_config"
        },
        isHybrid: true,
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "USB",
        tools: ["TI", "IAR"],
        boards:[
            Boards.MSP_EXP430F5529LP,
            Boards.MSP_EXP430F5529
        ],
    },
    {
        title: "USB CDC Mouse Device",
        name: "usbcdcmousedevice",
        description: "USB composite example that features a CDC virtual serial port communications along with a HID Mouse device",
        root: "./source/usbcdcmousedevice/",
        cFile: "usbcdcmousedevice.c",
        cfgFile: "usbcdcmousedevice_msp430.cfg",
        fileList: msp430USBstackFiles.concat([
            {path: "source/usbmsp430/cdchidm/USBCDCMOUSE.h"},
            {path: "source/usbmsp430/cdchidm/USBCDCMOUSE.c"},
            {path: "source/usbcdcmousedevice/usbcdcmousedevice_readme.txt"},

            /* USB_config */
            {path: "source/usbmsp430/cdchidm/USB_config/descriptors.c", targetDirectory: "USB_config"},
            {path: "source/usbmsp430/cdchidm/USB_config/descriptors.h", targetDirectory: "USB_config"},
            {path: "source/usbmsp430/cdchidm/USB_config/UsbIsr.c",      targetDirectory: "USB_config"},
            {path: "source/usbmsp430/cdchidm/USB_config/MSP430_CDC.inf",targetDirectory: "USB_config"},
        ]),
        compilerBuildOptions: {
            TI: " -I${workspace_loc:/${ProjName}} -I${workspace_loc:/${ProjName}/USB_config}",
            TI_makefile: " -I ./USB_config",
            IAR_makefile: " -I ./USB_config"
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "USB",
        tools: ["TI", "IAR"],
        boards:[
            Boards.MSP_EXP430F5529LP,
            Boards.MSP_EXP430F5529
        ],
    },

   /*
    *
    ********* Watchdog EXAMPLES **********
    *
    */
    {
        title: "Watchdog",
        name: "watchdog",
        description: "Watchdog Timer example that services a watchdog until a button press and release",
        root: "./source/watchdog/",
        cFile: "watchdog.c",
        cfgFile: "watchdog.cfg",
        fileList: [
            {path: "source/watchdog/watchdog_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "Watchdog",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.TMDXDOCK28M36_M3,
            Boards.TMDXDOCKH52C1_M3,
            Boards.DK_TM4C123G,
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
      ],
    },
    {
        title: "Watchdog",
        name: "watchdog",
        description: "Watchdog Timer example that services a watchdog until a button press and release",
        root: "./source/watchdog/",
        cFile: "watchdog_msp430.c",
        cfgFile: "watchdog_msp432.cfg",
        fileList: [
            {path: "source/watchdog/watchdog_msp430_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "Watchdog",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.MSP_EXP432P401RLP
        ],
    },
    {
        title: "Watchdog",
        name: "watchdog",
        description: "Watchdog Timer example that services a watchdog until a button press and release",
        root: "./source/watchdog/",
        cFile: "watchdog.c",
        cfgFile: "watchdog_cc3200_lp.cfg",
        fileList: [
            {path: "source/watchdog/watchdog_readme.txt"},
            {path: "boards/CC3200_LP/watchdog.pinmux"},
            {path: "boards/CC3200_LP/watchdog_rom_pin_mux_config.c"},
            {path: "boards/CC3200_LP/pin_mux_config.h"},
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "Watchdog",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.CC3200_LP
      ],
    },
    {
        title: "Watchdog",
        name: "watchdog",
        description: "Watchdog Timer example that services a watchdog until a button press and release",
        root: "./source/watchdog/",
        cFile: "watchdog_msp430.c",
        cfgFile: "watchdog_msp430.cfg",
        fileList: [
            {path: "source/watchdog/watchdog_msp430_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "Watchdog",
        tools: ["TI", "IAR"],
        boards:[
            Boards.MSP_EXP430F5529LP,
            Boards.MSP_EXP430F5529,
            Boards.MSP_EXP430FR5969LP
        ],
    },

   /*
    *
    ********* WiFi EXAMPLES **********
    *
    */
    /* CC3100 Examples */
    {
        title: "TCP Echo for CC3100",
        name: "tcpEchoCC3100",
        description: "The TCP Echo example showing how to use sockets with the SimpleLink Wi-Fi CC3100",
        root: "./source/tcpEchoCC3100/",
        cFile: "tcpEchoCC3100.c",
        cfgFile: "tcpEchoCC3100.cfg",
        fileList: [
            {path: "source/tcpEchoCC3100/sockets.c"},
            {path: "source/tcpEchoCC3100/sockets.h"},
            {path: "source/tcpEchoCC3100/tcpEchoCC3100_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink"
                + " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink/Include",
            TI_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                         + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
            IAR_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                          + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
            GNU: " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink"
                 + " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink/Include",
            GNU_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                          + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "WiFi",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "UDP Echo for CC3100",
        name: "udpEchoCC3100",
        description: "The UDP Echo example showing how to use sockets with the SimpleLink Wi-Fi CC3100",
        root: "./source/udpEchoCC3100/",
        cFile: "udpEchoCC3100.c",
        cfgFile: "udpEchoCC3100.cfg",
        fileList: [
            {path: "source/udpEchoCC3100/sockets.c"},
            {path: "source/udpEchoCC3100/sockets.h"},
            {path: "source/udpEchoCC3100/udpEchoCC3100_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink"
                + " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink/Include",
            TI_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                         + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
            IAR_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                          + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
            GNU: " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink"
                 + " -I${COM_TI_RTSC_TIRTOSTIVAC_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink/Include",
            GNU_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                          + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "WiFi",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.EK_TM4C123GXL,
            Boards.DK_TM4C129X,
            Boards.EK_TM4C1294XL
        ],
    },
    {
        title: "TCP Echo for CC3100",
        name: "tcpEchoCC3100",
        description: "The TCP Echo example showing how to use sockets with the SimpleLink Wi-Fi CC3100",
        root: "./source/tcpEchoCC3100/",
        cFile: "tcpEchoCC3100.c",
        cfgFile: "tcpEchoCC3100_msp432.cfg",
        fileList: [
            {path: "source/tcpEchoCC3100/sockets.c"},
            {path: "source/tcpEchoCC3100/sockets.h"},
            {path: "source/tcpEchoCC3100/tcpEchoCC3100_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink"
                + " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink/Include",
            TI_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                         + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
            IAR_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                          + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
            GNU: " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink"
                 + " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink/Include",
            GNU_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                          + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "WiFi",
        tools: ["TI", "IAR", "GNU"],
        boards:[
            Boards.MSP_EXP432P401RLP
        ],
    },
    {
        title: "UDP Echo for CC3100",
        name: "udpEchoCC3100",
        description: "The UDP Echo example showing how to use sockets with the SimpleLink Wi-Fi CC3100",
        root: "./source/udpEchoCC3100/",
        cFile: "udpEchoCC3100.c",
        cfgFile: "udpEchoCC3100_msp432.cfg",
        fileList: [
            {path: "source/udpEchoCC3100/sockets.c"},
            {path: "source/udpEchoCC3100/sockets.h"},
            {path: "source/udpEchoCC3100/udpEchoCC3100_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink"
                + " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink/Include",
            TI_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                         + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
            IAR_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                          + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
            GNU: " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink"
                 + " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink/Include",
            GNU_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                          + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "WiFi",
        tools: ["TI", "IAR", "GNU"],
        boards:[Boards.MSP_EXP432P401RLP],
    },
    {
        title: "TCP Echo for CC3100",
        name: "tcpEchoCC3100",
        description: "The TCP Echo example showing how to use sockets with the SimpleLink Wi-Fi CC3100",
        root: "./source/tcpEchoCC3100/",
        cFile: "tcpEchoCC3100.c",
        cfgFile: "tcpEchoCC3100_msp430f5529lp.cfg",
        fileList: [
            {path: "source/tcpEchoCC3100/sockets.c"},
            {path: "source/tcpEchoCC3100/sockets.h"},
            {path: "source/tcpEchoCC3100/tcpEchoCC3100_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink"
                + " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink/Include",
            TI_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                         + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
            IAR_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                          + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "WiFi",
        tools: ["TI", "IAR"],
        boards:[Boards.MSP_EXP430F5529LP],
    },
    {
        title: "UDP Echo for CC3100",
        name: "udpEchoCC3100",
        description: "The UDP Echo example showing how to use sockets with the SimpleLink Wi-Fi CC3100",
        root: "./source/udpEchoCC3100/",
        cFile: "udpEchoCC3100.c",
        cfgFile: "udpEchoCC3100_msp430f5529lp.cfg",
        fileList: [
            {path: "source/udpEchoCC3100/sockets.c"},
            {path: "source/udpEchoCC3100/sockets.h"},
            {path: "source/udpEchoCC3100/udpEchoCC3100_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink"
                + " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink/Include",
            TI_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                         + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
            IAR_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                          + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "WiFi",
        tools: ["TI", "IAR"],
        boards:[Boards.MSP_EXP430F5529LP],
    },
    {
        title: "TCP Echo for CC3100",
        name: "tcpEchoCC3100",
        description: "The TCP Echo example showing how to use sockets with the SimpleLink Wi-Fi CC3100",
        root: "./source/tcpEchoCC3100/",
        cFile: "tcpEchoCC3100.c",
        cfgFile: "tcpEchoCC3100_msp430fr5969lp.cfg",
        fileList: [
            {path: "source/tcpEchoCC3100/sockets.c"},
            {path: "source/tcpEchoCC3100/sockets.h"},
            {path: "source/tcpEchoCC3100/tcpEchoCC3100_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink"
                + " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink/Include",
            TI_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                         + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
            IAR_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                          + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "WiFi",
        tools: ["TI", "IAR"],
        boards:[Boards.MSP_EXP430FR5969LP],
    },
    {
        title: "UDP Echo for CC3100",
        name: "udpEchoCC3100",
        description: "The UDP Echo example showing how to use sockets with the SimpleLink Wi-Fi CC3100",
        root: "./source/udpEchoCC3100/",
        cFile: "udpEchoCC3100.c",
        cfgFile: "udpEchoCC3100_msp430fr5969lp.cfg",
        fileList: [
            {path: "source/udpEchoCC3100/sockets.c"},
            {path: "source/udpEchoCC3100/sockets.h"},
            {path: "source/udpEchoCC3100/udpEchoCC3100_readme.txt"}
        ],
        compilerBuildOptions: {
            TI: " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink"
                + " -I${COM_TI_RTSC_TIRTOSMSP430_INSTALL_DIR}/packages/ti/drivers/wifi/cc3100/Simplelink/Include",
            TI_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                         + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
            IAR_makefile: " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink"
                          + " -I$(TIRTOS_INSTALLATION_DIR)/ti/drivers/wifi/cc3100/Simplelink/Include",
        },
        requiredProducts: [],
        options: "TREX",
        type: "example",
        group: "WiFi",
        tools: ["TI", "IAR"],
        boards:[Boards.MSP_EXP430FR5969LP],
    },

   /*
    *
    ********* DEMO PROJECTS **********
    *
    */
    {
        title: "Demo [M3]",
        name: "demo",
        description: "Demo showcasing SYS/BIOS, Networking, IPC and UIA.  Uses UART, USB, I2C, SDSPI, EMAC and GPIO.",
        root: "./boards/TMDXDOCKH52C1/demo/",
        cFile: "demo.c",
        cfgFile: "demo.cfg",
        fileList: [
            {path: "boards/TMDXDOCKH52C1/demo/webpage.c"},
            {path: "boards/TMDXDOCKH52C1/demo/cmdline.c"},
            {path: "source/usb/USBCDCD_LoggerIdle.c"},
            {path: "boards/TMDXDOCKH52C1/demo/demo.h"},
            {path: "boards/TMDXDOCKH52C1/demo/logobar.h"},
            {path: "boards/TMDXDOCKH52C1/demo/dspchip.h"},
            {path: "boards/TMDXDOCKH52C1/demo/default.h"},
            {path: "source/usb/USBCDCD_LoggerIdle.h"},
            {path: "boards/TMDXDOCKH52C1/demo/jquery.min.h"},
            {path: "boards/TMDXDOCKH52C1/demo/jquery.flot.min.h"},
            {path: "boards/TMDXDOCKH52C1/demo/layout.css.h"},
            {path: "boards/TMDXDOCKH52C1/demo/demo_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "Demo",
        linkeroptions: " --stack_size=1024 --heap_size=0 --warn_sections --rom_model",
        tools: ["TI"],
        boards:[
            Boards.TMDXDOCKH52C1_M3
        ],
    },
    {
        title: "Demo [M3]",
        name: "demo",
        description: "Demo showcasing SYS/BIOS, Networking, IPC and UIA.  Uses UART, USB, I2C, SDSPI, EMAC and GPIO.",
        root: "./boards/TMDXDOCK28M36/demo/",
        cFile: "demo.c",
        cfgFile: "demo.cfg",
        fileList: [
            {path: "boards/TMDXDOCK28M36/demo/webpage.c"},
            {path: "boards/TMDXDOCK28M36/demo/cmdline.c"},
            {path: "boards/TMDXDOCK28M36/demo/demo.h"},
            {path: "boards/TMDXDOCK28M36/demo/logobar.h"},
            {path: "boards/TMDXDOCK28M36/demo/dspchip.h"},
            {path: "boards/TMDXDOCK28M36/demo/default.h"},
            {path: "source/usb/USBCDCD_LoggerIdle.c"},
            {path: "source/usb/USBCDCD_LoggerIdle.h"},
            {path: "boards/TMDXDOCK28M36/demo/jquery.min.h"},
            {path: "boards/TMDXDOCK28M36/demo/jquery.flot.min.h"},
            {path: "boards/TMDXDOCK28M36/demo/layout.css.h"},
            {path: "boards/TMDXDOCK28M36/demo/demo_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "Demo",
        linkeroptions: " --stack_size=1024 --heap_size=0 --warn_sections --rom_model",
        tools: ["TI"],
        boards:[
            Boards.TMDXDOCK28M36_M3
        ],
    },
    {
        title: "Demo [C28]",
        name: "demo",
        description: "Demo showcasing SYS/BIOS, Networking, IPC and UIA.  Uses UART, USB, I2C, SDSPI, EMAC and GPIO.",
        root: "./boards/TMDXDOCKH52C1/demo/",
        cFile: "demo_c28.c",
        cfgFile: "demo_c28.cfg",
        fileList: [
            {path: "boards/TMDXDOCKH52C1/demo/demo.h"},
            {path: "boards/TMDXDOCKH52C1/demo/demo_c28_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "Demo",
        tools: ["TI"],
        boards:[
            Boards.TMDXDOCKH52C1_C28
        ],
    },
    {
        title: "Demo [C28]",
        name: "demo",
        description: "Demo showcasing SYS/BIOS, Networking, IPC and UIA. Uses UART, USB, I2C, SDSPI, EMAC and GPIO.",
        root: "./boards/TMDXDOCK28M36/demo/",
        cFile: "demo_c28.c",
        cfgFile: "demo_c28.cfg",
        fileList: [
            {path: "boards/TMDXDOCK28M36/demo/demo.h"},
            {path: "boards/TMDXDOCK28M36/demo/demo_c28_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "Demo",
        tools: ["TI"],
        boards:[
            Boards.TMDXDOCK28M36_C28
        ],
    },
    {
        title: "IPC SPI Slave",
        name: "spislave",
        description: "Slave side of the MessageQ over SPI example.",
        root: "./boards/TMDXDOCKH52C1/demo/",
        cFile: "spislave.c",
        cfgFile: "spislave.cfg",
        fileList: [
            {path: "boards/TMDXDOCKH52C1/demo/spiDemo.h"},
            {path: "boards/TMDXDOCKH52C1/demo/spislave_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "Demo",
        tools: ["TI"],
        boards:[
            Boards.TMDXDOCKH52C1_M3
        ],
    },
    {
        title: "IPC SPI Master",
        name: "spimaster",
        description: "Master side of the MessageQ over SPI example.",
        root: "./boards/TMDXDOCKH52C1/demo/",
        cFile: "spimaster.c",
        cfgFile: "spimaster.cfg",
        fileList: [
            {path: "boards/TMDXDOCKH52C1/demo/spiDemo.h"},
            {path: "boards/TMDXDOCKH52C1/demo/spimaster_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "Demo",
        tools: ["TI"],
        boards:[
            Boards.TMDXDOCKH52C1_M3
        ],
    },
    {
        title: "Graphic Library Demo",
        name: "grlibdemo",
        description: "Display picture/text on the onboard LED using the graphic library.",
        root: "./boards/DK_TM4C123G/demo/",
        cFile: "grlibdemo.c",
        cfgFile: "grlibdemo.cfg",
        fileList: [
            {path: "boards/DK_TM4C123G/demo/cfal96x64x16.h"},
            {path: "boards/DK_TM4C123G/demo/cfal96x64x16.c"},
            {path: "boards/DK_TM4C123G/demo/images.c"},
            {path: "source/usb/USBCDCD.h"},
            {path: "source/usb/USBCDCD.c"},
            {path: "boards/DK_TM4C123G/demo/grlibdemo_readme.txt"}
        ],
        requiredProducts: [],
        options: "TREX",
        type: "Demo",
        tools: ["TI"],
        boards:[
            Boards.DK_TM4C123G
        ],
    },
    {
        title: "Graphics Library Demo",
        name: "grlibdemo",
        description: "Display picture/text on the onboard LED using the graphic library.",
        root: "./boards/MSP_EXP430F5529/demo/",
        cFile: "grlibdemo.c",
        cfgFile: "grlibdemo.cfg",
        fileList: grlibFiles.concat([
            {path: "boards/MSP_EXP430F5529/demo/images.h", targetDirectory: "images"},
            {path: "boards/MSP_EXP430F5529/demo/LPRocket_96x37.c", targetDirectory: "images"},
            {path: "boards/MSP_EXP430F5529/demo/TI_Logo_69x64.c", targetDirectory: "images"},
            {path: "boards/MSP_EXP430F5529/demo/Dogs102x64_UC1701.h", targetDirectory: "lcdDriver"},
            {path: "boards/MSP_EXP430F5529/demo/Dogs102x64_UC1701.c", targetDirectory: "lcdDriver"},
            {path: "boards/MSP_EXP430F5529/demo/fontfixed6x8.c", targetDirectory: "fonts"},
            {path: "boards/MSP_EXP430F5529/demo/driverlibHeaders.h"},
            {path: "boards/MSP_EXP430F5529/demo/grlibdemo_readme.txt"},
        ]),
        requiredProducts: [],
        options: "TREX",
        type: "Demo",
        tools: ["TI"],
        boards:[
            Boards.MSP_EXP430F5529
        ],
    },
    {
        title: "Graphics Library Demo",
        name: "grlibdemo",
        description: "Display picture/text on the onboard LED using the graphic library.",
        root: "./boards/MSP_EXP430FR5969LP/demo/",
        cFile: "grlibdemo.c",
        cfgFile: "grlibdemo.cfg",
        fileList: grlibFiles.concat([
            {path: "boards/MSP_EXP430FR5969LP/demo/images.h", targetDirectory: "images"},
            {path: "boards/MSP_EXP430FR5969LP/demo/LPRocket_96x37.c", targetDirectory: "images"},
            {path: "boards/MSP_EXP430FR5969LP/demo/TI_Logo_69x64.c", targetDirectory: "images"},
            {path: "boards/MSP_EXP430FR5969LP/demo/Sharp96x96.h", targetDirectory: "lcdDriver"},
            {path: "boards/MSP_EXP430FR5969LP/demo/Sharp96x96.c", targetDirectory: "lcdDriver"},
            {path: "boards/MSP_EXP430FR5969LP/demo/Sharp96x96utils.h", targetDirectory: "lcdDriver"},
            {path: "boards/MSP_EXP430FR5969LP/demo/Sharp96x96utils.asm", targetDirectory: "lcdDriver"},
            {path: "boards/MSP_EXP430FR5969LP/demo/fontfixed6x8.c", targetDirectory: "fonts"},
            {path: "boards/MSP_EXP430FR5969LP/demo/driverlibHeaders.h"},
            {path: "boards/MSP_EXP430FR5969LP/demo/grlibdemo_readme.txt"},
        ]),
        requiredProducts: [],
        options: "TREX",
        type: "Demo",
        tools: ["TI"],
        boards:[
            Boards.MSP_EXP430FR5969LP
        ],
    },
];

/*
 * ======== supportsBoard ========
 */
function supportsBoard(example, board)
{
    for (var i = 0; i < example.boards.length; i++) {
        if(example.boards[i].name == board.name) {
            return true;
        }
    }
    return false;
}

/*
 * ======== supportsTool ========
 */
function supportsTool(example, tool)
{
    for (var i = 0; i < example.tools.length; i++) {
        if(example.tools[i] == tool) {
            return true;
        }
    }
    return false;
}
