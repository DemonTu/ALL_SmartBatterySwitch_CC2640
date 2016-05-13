Example Summary
----------------
This application demonstrates how to use the TI-RTOS Watchdog driver to cause a
reset.

Peripherals Exercised
---------------------
Board_LED0      Indicator LED
Board_BUTTON0   Used to control the application
Board_WATCHDOG0 Timer to resets device

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| MSP-EXP430F5529   | N/A                                                    |
| MSP-EXP430F5529LP |                                                        |
| ----------------- | ------------------------------------------------------ |

Example Usage
-------------
The application turns ON Board_LED0 to indicate TI-RTOS driver initialization
is complete.

Board_LED0 is toggled repeatedly until Board_BUTTON0 is pressed. A device reset
occurs once Board_BUTTON0 is pushed.  What happens after the button push depends
on whether there is a debugger attached or not.

In the debugger case, the reset will either cause the application to restart or
cause it to halt, depending on your device. Again depending on your device,
you can set a breakpoint at the beginning of main() to see that the device was
reset after pressing the button.

If the debugger is not in use and Board_BUTTON0 is pressed, there may be a brief
flicker in the blinking of Board_LED0 as the reset occurs.

Application Design Details
--------------------------
The application's task opens a Watchdog driver object. The task calls
Watchdog_clear() to prevent a reset until the Board_BUTTON0 is pushed. When
the button was pushed, Watchdog_clear() is not called and the watchdog timer
expires, causing a reset.

For GNU and IAR users, please read the following website for details about semi-hosting
http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting.
