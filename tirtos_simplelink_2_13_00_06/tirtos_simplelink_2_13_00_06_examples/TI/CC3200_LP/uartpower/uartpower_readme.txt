Example Summary
---------------
UART example using power management.  This example has one task that
repeatedly writes a buffer of to the UART for a fixed number of times,
and then sleeps for 5 seconds.  While the task is asleep, the processor
goes into a low power mode (eg, LPDS for CC3200).  The power consumption
can be measured by connecting an ammeter to the appropriat pins on the
LaunchPad.

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| CC3200            | N/A                                                    |
| ----------------- | ------------------------------------------------------ |

Peripherals Exercised
---------------------
Board_LED0,                         Used to indicate status.
Board_UART0                         Used to send data to the UART.

Example Usage
-------------
Attach an ammeter across jumper J12 (CC3200 Launchpad) or jumper 3V3
(MSP432 LaunchPad) to measure the current draw.

Run the example.  Open a serial session (eg, HyperTerminal, puTTY, etc)
to the appropriate COM port.  Note: the COM port can be determined
via Device Manager in Windows or via ls /dev/tty* in Linux.
You should see a line containing the letters of the alphabet continually
written to the serial session for a few seconds, followed by a line that
says "Going to sleep for 5 seconds...".  This repeats until you halt
the program.  By connecting an ammeter to the appropriate jumper, you
can see the current usage drop while the task is asleep.
