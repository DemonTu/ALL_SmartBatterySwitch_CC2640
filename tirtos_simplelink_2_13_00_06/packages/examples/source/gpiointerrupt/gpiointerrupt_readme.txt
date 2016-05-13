Example Summary
---------------
Application that toggles a LED using a GPIO pin interrupt.  It also demonstrates
how the UARTMon module can be used with GUI Composer in CCS.

Peripherals Exercised
---------------------
Board_LED0      Indicates that the board was initialized within main()
                Also toggled by Board_BUTTON0
Board_LED1      Toggled by Board_BUTTON1
Board_BUTTON0   Toggles Board_LED0
Board_BUTTON1   Toggles Board_LED1
Board_UART0     Used by UARTMon to communicate with GUI Composer

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| CC3200            | Close Jumpbers J2 and J3                               |
| ----------------- | ------------------------------------------------------ |
| DK-TM4C123G       | N/A                                                    |
| ----------------- | ------------------------------------------------------ |
| DK-TM4C129X       | J37: Close SEL, DOWN, UP (enables Switches)            |
|                   | J36: Close GREEN, BLUE, RED (enables LEDs)             |
| ----------------- | ------------------------------------------------------ |
| EK-TM4C123GXL     | N/A                                                    |
| EK-TM4C1294XL     |                                                        |
| MSP-EXP430F5529   |                                                        |
| MSP-EXP430F5529LP |                                                        |
| ----------------- | ------------------------------------------------------ |
| MSP-EXP430FR5969LP| Close J6                                               |
| ----------------- | ------------------------------------------------------ |
| MSP-EXP432P401RLP | N/A                                                    |
| TMDXDOCK28M36     |                                                        |
| TMDXDOCKH52C1     |                                                        |
| ----------------- | ------------------------------------------------------ |

Example Usage
-------------
Run the example. Board_LED0 turns ON to indicate TI-RTOS driver initialization
is complete.

Board_LED0 is toggled by pushing Board_BUTTON0 (rising edge).
Board_LED1 is toggled by pushing Board_BUTTON1 (falling edge).

Not all boards have more than one button, so Board_LED1 may not be toggled.

Please refer to the TI-RTOS User's Guide for an explanation on how to configure
and use UARTMon with GUI Composer.

Please note, there is no button de-bounce logic in the example. So the
count might increase faster.

Application Design Details
--------------------------
The gpioButtonFxn0/1 functions configured in the "Board.c" file. These functions
are called in the context of the GPIO interrupt.

There is no application source code needed for UARTMon other than UART
configuration in the "Board.c" and the initialization of the TI-RTOS UART driver
which is accomplished by calling Board_initUART().

For GNU and IAR users, please read the following website for details about
semi-hosting:
    http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting
