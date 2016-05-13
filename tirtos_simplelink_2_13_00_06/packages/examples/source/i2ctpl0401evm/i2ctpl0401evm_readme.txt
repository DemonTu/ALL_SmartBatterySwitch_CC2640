Example Summary
---------------
Sample application to communicate with the I2C peripherals on a TPL0401EVM
Booster Pack.

Peripherals Exercised
---------------------
Board_LED0         Indicator LED
Board_LED1         Indicator LED
Board_I2C_TPL0401  I2C used to communicate with I2C peripherals on TPL0401EVM.*

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| DK-TM4C129X       | This example was designed to use the TPL0401 Booster   |
| EK-TM4C123GXL     | Pack (TPL0401EVM). The booster pack is required to     |
| EK-TM4C1294XL     | successfully complete this example.                    |
| MSP-EXP430F5529LP | The I2C GPIO driver on the TPL0401 requires external   |
| MSP-EXP430FR5969LP| current sourcing to power the RGB LEDs.                |
| MSP-EXP432P401RLP |                                                        |
| ----------------- | ------------------------------------------------------ |

Example Usage
-------------
Connect the TPL0401EVM Booster Pack before powering the hardware.

Run the example. Board_LED0 turns ON to indicate TI-RTOS driver
initialization is complete.

The TLC59108 will read the color patterns in the rgbcmd variable and toggle
the RGB LEDs ON/OFF.  Board_LED1 will turn ON if there has been a problem
with I2C communication.

Application Design Details
--------------------------
This application uses one tasks:
  'cycleLED' performs the following actions:
      Opens and initializes an I2C driver object.

      Uses the I2C driver to initialize the PWM oscillator on the TLC59108 and
      set the LED outputs to PWM mode.

      A RGBCMD structure (contains LED address and individual color values for
      Red, Green and Blue) is read from the rgbcmd array and sent via I2C
      driver to the TLC59108.

      If the end of the array is reached, the index is reset to 0 and the Task
      sleeps for 100 system ticks before starting the next iteration.

For GNU and IAR users, please read the following website for details about
semi-hosting:
    http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting
