Example Summary
---------------
Sample application to send a string to a USB host as a keyboard HID device.

Peripherals Exercised
---------------------
Board_LED0      Caps Lock LED
Board_LED1      Scroll Lock LED
Board_BUTTON0   Used to send a "typed" string
Board_USBDEVICE Used as HID Mouse device*

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| DK-TM4C123G       | Please ensure that the board is connected to your host |
| DK-TM4C129X       | via a USB cable. A VCOM (virtual COM) port driver may  |
| EK-TM4C123GXL     | need to be installed.                                  |
| EK-TM4C1294XL     |                                                        |
| MSP-EXP430F5529   |                                                        |
| MSP-EXP430F5529LP |                                                        |
| TMDXDOCK28M36     |                                                        |
| ----------------- | ------------------------------------------------------ |
| TMDXDOCKH52C1     | Please ensure that the board is connected to your host |
|                   | via a USB cable. A VCOM (virtual COM) port driver may  |
|                   | need to be installed.                                  |
|                   | Connectivity Bank 30-31: B-C position (USB)            |
| ----------------- | ------------------------------------------------------ |

Example Usage
-------------
Run the example. Board_LED0 turns ON to indicate TI-RTOS driver
initialization is complete.

Once the enumeration occurs, Board_LED0 and Board_LED1 show the status
of Caps Lock and Scroll lock respectively. For example, press Caps Lock
on your keyboard and Board_LED0 will toggle.

Board_BUTTON0 simulates the typing of the following string:

   "TI-RTOS controls USB.\n"

An easy way to see this is to open an empty text file in edit mode and press
Board_BUTTON0. The text will be "typed" in.

USB drivers can be found at the following locations:
    Tiva USB Drivers:
    Windows USB drivers are located in the products directory:
    <tirtos_install_dir>\products\<TivaWare_install_dir>\windows_drivers

    MSP430 USB Drivers:
    Windows USB drivers are located within the CCS project's USB_config/
    directory.

    Concerto Family USB Drivers:
    Windows USB drivers are located in the products directory:
    <tirtos_install_dir>\products\<MWare_install_dir>\windows_drivers

Application Design Details
--------------------------
This application uses one tasks:

  'keyboardTask' performs the following actions:
      Waits for the device to connected to a USB host.

      Gets the status of the host's keyboard and updates the LEDs accordingly.

      Then it polls to detect a button push. When the Board_BUTTON0 is pushed,
      text is sent to the host.

For GNU and IAR users, please read the following website for details about
semi-hosting:
    http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting
