Example Summary
---------------
Sample application to control on-board LEDs with the PWM driver.

Peripherals Exercised
---------------------
Board_PWM0      PWM instance used to control Board_LED1 brightness
Board_PWM1      PWM instance used to control Board_LED2 brightness

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| CC3200            | Close jumpers J2 and J3                                |
| ----------------- | ------------------------------------------------------ |
| DK-TM4C129X       | N/A                                                    |
| EK-TM4C123GXL     |                                                        |
| EK-TM4C1294XL     |                                                        |
| MSP-EXP430FR5969LP|                                                        |
| ----------------- | ------------------------------------------------------ |

Example Usage
-------------
The example performs general initialization in main().
The pwmLED task varies the intensity of the on-board LEDs at a rate determined
by the arg0 parameter for the Task instance in the .cfg file.  If Board_PWM0
and Board_PWM1 are different (connected to two LEDs), the LED connected to
Board_PWM0 will fade-in while the LED connected to Board_PWM1 will fade-out
and vice versa.

Application Design Details
--------------------------
This application uses one task:
  'pwmLED' performs the following actions:
      Opens and initializes PWM driver objects.

      Uses the PWM driver changed the intensity of the LEDs.

      The task sleeps for 50 system ticks before changing LED intensity again.

For GNU and IAR users, please read the following website for details about
semi-hosting:
    http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting
