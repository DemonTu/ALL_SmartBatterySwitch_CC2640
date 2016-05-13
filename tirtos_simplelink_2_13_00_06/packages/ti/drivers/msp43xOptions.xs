/*
 *  ======== msp43xOptions.xs ========
 */

/*
 *  ======== getCompilerLinkerOptions ========
 *  Return the *ware compiler and linker include path for a given device and
 *  *ware path
 */
function getCompilerLinkerOptions(part, mspware, target)
{
    var compilerOptions = "";
    var compilerSearchPath = "";
    var linkerLibPath = "";

    /* MSP430FR57xx */
    if (part.match(/MSP430FR57[\d\w]*/g)) {
        compilerSearchPath = " -I" + mspware + "/driverlib/MSP430FR57xx";
        linkerLibPath = " -l" + mspware + "/driverlib" +"/ccs-" + part
                      + "/ccs-" + part + ".lib";
    }
    /* MSP430FR5xx_6xx */
    else if (part.match(/MSP430FR[56]\d\w]*/g)) {
        compilerSearchPath = " -I" + mspware + "/driverlib/MSP430FR5xx_6xx";
        linkerLibPath = " -l" + mspware + "/driverlib" + "/ccs-" + part
                      + "/ccs-" + part + ".lib";
    }
    /* MSP430F5xx_6xx */
    else if (part.match(/MSP430F[56][\d\w]*/g)) {
        compilerSearchPath = " -I" + mspware + "/driverlib/MSP430F5xx_6xx";
        linkerLibPath = " -l" + mspware + "/driverlib" + "/ccs-" + part
                      + "/ccs-" + part + ".lib";
    }
    /* MSP432P4xx */
    else if (part.match(/MSP432P4*/g)) {
        compilerSearchPath = " -I" + mspware + "/driverlib/MSP432P4xx"
                           + " -I" + mspware + "/driverlib/inc";
        linkerLibPath = " -l" + mspware
                      + "/driverlib/MSP432P4xx/ccs/msp432p4xx_driverlib.lib";
    }
    else {
        throw("Invalid part:'" + part + "' supplied");
    }

    compilerOptions = " -D__" + part + "__";

    if (target != undefined) {
        if (target.suffix == "r430XS") {
            compilerSearchPath += " -I" + target.rootDir + "/inc"
                                + " -I" + target.rootDir + "/inc/dlib/c";
        }
        else if (target.suffix == "e430X") {}
        else if (target.suffix == "rm4f") {
            compilerSearchPath += " -I"+ target.rootDir + "/inc"
                                + " -I"+ target.rootDir + "/inc/c"
                                + " -I"+ target.rootDir + "/CMSIS/Include";
        }
        else if (target.suffix == "em4f" || target.suffix == "m4fg") {
            compilerSearchPath += " -I"+ mspware + "/driverlib/inc/CMSIS";
        }
    }

    compilerOptions = compilerOptions + compilerSearchPath;

    return ({
                copts: compilerOptions,
                cIncludePath: compilerSearchPath,
                lIncludeLibPath: linkerLibPath
            });
}
