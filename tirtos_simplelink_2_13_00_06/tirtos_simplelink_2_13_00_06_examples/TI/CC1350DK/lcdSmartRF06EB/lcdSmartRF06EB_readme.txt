Example Summary
---------------
Sample application to demonstrate how to use the LCD on the SmartRF06 Evaluation
Board.

Peripherals Exercised
---------------------
Board_LED1     Toggled by the low priority task
Board_LED2     Toggled by the high priority task
Board_SPI0     SPI used to control the onboard LCD
Board_BUTTON0  Button used to unblock the high priority task

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| CC2650DK          | The SPI pins from the CC2650EM need to be routed to    |
|                   | the SmartRF06's LCD controller.                        |
|                   | Close SPI_SCK, SPI_MOSI, SPI_MISO, LCD_MODE, LCD_RESET,|
|                   | LCD_CS, and BTN_DOWN                                   |
| ----------------- | ------------------------------------------------------ |

Example Usage
-------------
Run the example. The LCD will be initialized and a task will display message:

    Hello SmartRF06EB!
    Low Priority Task
    Writing to Buffer 1

    Dec   0
    Bin   0
    Hex   0

The values displayed will increment by 1 every 5 seconds.  Every 20 seconds the
display will be refreshed to only show the decimal count:

    Hello SmartRF06EB!
    Low Priority Task
    Writing to Buffer 1

    Count value is:
    xx

By pressing the Board_BUTTON0 button, the high priority task is unblocked and
will print the following message on the LCD:

    This is a LCD example
    High Priority Task
    Writing to Buffer 0

             ***
              *
             ***

Application Design Details
--------------------------
This application uses two tasks:
  'taskLcdLoPriFxn' performs the following actions:
      Opens and initializes a PIN driver object.

      Opens and initializes the LCD driver object.

      Loop forever incrementing the count and refresing the LCD every 5 seconds.

    'lcdHighPriorityFxn' performs the following actions:
      Remains blocked on a semaphore until the Board_BUTTON0 button is pressed.

      Writes a message to the LCD.

      Returns to a blocked state pending on a semaphore.

For GNU and IAR users, please read the following website for details about
semi-hosting:
    http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting
