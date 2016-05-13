Example Summary
---------------
Example that uses the UART driver to echo back to the console.  It also
demonstrates System Analyzer usage via a stop-mode logger.

Peripherals Exercised
---------------------
Board_LED0      Indicates that the board was initialized within main()
Board_UART0     Used to echo characters from host serial session

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| CC2650DK          | N/A                                                   |
| CC2650STE-BLE     |                                                        |
| CC3200            |                                                        |
| DK-TM4C123G       |                                                        |
| DK-TM4C129X       |                                                        |
| EK-TM4C123GXL     |                                                        |
| EK-TM4C1294XL     |                                                        |
| MSP-EXP430F5529   |                                                        |
| MSP-EXP430F5529LP |                                                        |
| MSP-EXP430FR5969LP|                                                        |
| MSP-EXP432P401RLP |                                                        |
| TMDXDOCK28M36     |                                                        |
| TMDXDOCKH52C1     |                                                        |
| ----------------- | ------------------------------------------------------ |

Example Usage
-------------
Run the example. Board_LED0 turns ON to indicate TI-RTOS driver
initialization is complete.

When the application is running, open a serial session (e.g. HyperTerminal,
puTTY, etc.) to the appropriate COM port. Note: the COM port can be determine
via Device Manager in Windows or via ls /dev/tty* in Linux.

The connection should have the following settings
    Baud-rate:    9600
    Data bits:       8
    Stop bits:       1
    Parity:       None
    Flow Control: None

The target echoes back any character that is typed in the serial session.

If the serial session is started before the target completes initialization,
the following is displayed:
    "Echoing characters:"

This example also demonstrates System Analyzer in CCS. This is accomplished
via stop-mode reading of the logs on the target. Halt the target and open
System Analyzer as described in the TI-RTOS User Guide's "Viewing the Logs".

The "Live Session" should have record like the following
   - "LS_cpuLoad: 0%"
   - "Wrote character 0xa"

Application Design Details
--------------------------
This example shows how to initialize the UART driver in blocking read and write
mode with no data processing and echo characters back to a console.

A single task, "echo", reads a character from Board_UART0 and writes it back.

This example uses the default StopMode logger with LoggingSetup. Log records
are stored in internal buffers. They are read by System Analyzer when the target
is halted. If the target is not halted and the buffers are full, the old records
are over-written.

To help fill up the logs, the kernel is configured to generate Log records
via the following from the .cfg file
    BIOS.logsEnabled = true;

Additionally, the drivers instrumentation is turned on to demonstrate logging
in the drivers. For this example, UART logs will show up in the "Main" logger.
    TIRTOS.libType = TIRTOS.LibType_Instrumented;

Note: most of the other examples do not use instrumented libraries to improve
performance and minimize footprint.

For GNU and IAR users, please read the following website for details about semi-hosting
http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting.

Using the UART DMA Driver
-------------------------
The CC3200 and TivaC platforms have DMA support for the UART driver.  For
these platforms, the uartecho example can be built with either the UART
driver with DMA support, or the non-DMA UART driver.  The default configuration
for this example is to use the non-DMA UART driver.  Since only one character
at a time is sent to the UART, this example is not an efficient use of the DMA,
but it still serves to illustrate use of the UART DMA driver.
To use the DMA version of the UART driver, build this example with the
compile flag:

    --define=TI_DRIVERS_UART_DMA=1

The <board>.c file includes either the UART driver with DMA support or the
non-DMA UART driver, based on this compile flag.
