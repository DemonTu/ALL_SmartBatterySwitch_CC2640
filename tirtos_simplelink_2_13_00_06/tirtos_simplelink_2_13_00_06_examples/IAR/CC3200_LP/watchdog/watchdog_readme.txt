Example Summary
---------------
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
| CC3200            | N/A                                                    |
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
The application supplies a callback function to the Watchdog module which
is executed on every watchdog timer time-out (in watchdog interrupt context).
The callback repeatedly clears the watchdog timer, thus preventing a device
reset, and toggles Board_LED0.

When Board_BUTTON0 is pressed, the "watchdogTask" clears a "flag" used by the
callback. The callback sees the flag and quits calling Watchdog_clear().

This application uses one task:

  'watchdogTask' performs the following actions:
      Determine if Board_BUTTON0 had been pressed.  If so, clear the reset flag;
      allow watchdog timer to reset device on next interrupt.

For GNU and IAR users, please read the following website for details about
semi-hosting:
    http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting
