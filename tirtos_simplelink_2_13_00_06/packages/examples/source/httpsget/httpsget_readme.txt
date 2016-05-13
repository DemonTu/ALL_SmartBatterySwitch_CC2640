Example Summary
----------------
This application demonstrates how to use HTTP Client with TLS.

Peripherals Exercised
---------------------
Board_LED0      Indicates that the board was initialized within main()
Board_EMAC      Connection to network

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| EK-TM4C1294XL     | N/A                                                    |
| ----------------- | ------------------------------------------------------ |

Build Details
-------------

Before building the application:

Change the application code in "httpsget.c" marked by "USER STEP" as needed.

This example needs a root certificate to build. The root certificate has to be
a C header file containing byte buffer of the public key. The steps to
generate a "certificate.h" are documented below. The generated file should
be placed next to the "httpsget.c" file for the application to build.

Also, this example needs CyaSSL libraries for TLS support. Read the following
wiki for details on using CyaSSL with TI-RTOS:
    http://processors.wiki.ti.com/index.php?title=Using_CyaSSL_with_TI-RTOS

After building CyaSSL:
    Add '<CyaSSL root directory>' path to the compiler include path.
    Add '<CyaSSL root directory>/tirtos/packages/ti/net/cyassl/lib/ti.net.cyassl.a<target>' to the linker options.
    For GNU, add '<CyaSSL root directory>/tirtos/packages/ti/net/cyassl/lib' to
    the library search path options and ":ti.net.cyassl.am4fg" to the library
    options.

    NOTE: Make sure the library ordering is correct on the linker command line
          to avoid linker errors.

Certificate generation
----------------------
Download "VeriSign Class 3 Public Primary CA" certificate (.pem format) from:
    http://www.symantec.com/page.jsp?id=roots

NOTE: This webpage will have several downloads which all appear to have
this certificate name.  Many of them have a suffix in the format "-G#" (for
example, "VeriSign Class 3 Public Primary CA-G5", which is not the correct
certificate to download). The correct download will be exactly titled
"VeriSign Class 3 Public Primary CA" without any "-G#" suffix at the end.

Rename the file to "ca_cert.pem".

Generate the "certificate.h" from the downloaded certificate using the
following command:

    <xdctools_dir>/xs --xdcpath=<tirtos_dir>/packages ti.net.tools.gencertbuf --certs "ca_cert.pem"

    where,
          <xdctools_dir> with XDCtools installation directory path.
          <tirtos_dir> with TI-RTOS insallation directory path.

Example Usage
-------------
The device must be connected to internet to run this example successfully.

The example turns ON Board_LED0 and starts the network stack. When the stack
receives an IP address from the DHCP server, the IP address is written to the
console.

The example then makes an HTTPS GET call to "download.finance.yahoo.com" to
the stock information for the ticker defined and print it to the console.

Application Design Details
--------------------------
This application uses a task for HTTPS communication:

httpsTask  Initializes the system clock, creates and initializes a TLS
           context with the certificate loaded and creates a TLS connection
           to the HTTPS server. When the connection is established, makes an
           HTTP GET method call using the request URI. The response status code
           header fields and body from the HTTP server are processed to extract
           the stock information. The connection is closed and resources
           are freed before the task exits.

For GNU and IAR users, please read the following website for details about semi-hosting
http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting.
