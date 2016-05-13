Example Summary
---------------
Sample application to get updates from a mouse HID device.

Peripherals Exercised
---------------------
Board_LED0      Primary Button Indicator LED
Board_LED1      Secondary Button Indicator LED
Board_USBHOST   Used as HID mouse host*

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| DK-TM4C123G       | Please ensure that the HID mouse device is connected   |
| DK-TM4C129X       | to your board's USB Host port.                         |
| EK-TM4C1294XL     |                                                        |
| TMDXDOCK28M36     |                                                        |
| ----------------- | ------------------------------------------------------ |
| TMDXDOCKH52C1     | Please ensure that the HID keyboard device is connected|
|                   | to your board's USB Host port.                         |
|                   | Connectivity Bank 30-31: B-C position (USB)            |
|                   | The TMDXDOCKH52C1 board has an KNOWN issue with USB    |
|                   | HOST operations. See the Getting Started Guide for   |
|                   | necessary board modifications.
| ----------------- | ------------------------------------------------------ |

If an unknown USB device or a power fault on the USB bus has been detected, a
System_printf message is generated.

Example Usage
-------------
Run the example. Board_LED0 turns ON to indicate TI-RTOS driver
initialization is complete.

Once the enumeration occurs, Board_LED0 and Board_LED1 to show the status
of the Primary and Secondary HID mouse buttons respectively. For example, if
the Primary button is pressed Board_LED0 will turn ON.  Once Primary button is
release, Board_LED0 will turn OFF.

The HID mouse's movements are tracked as offsets.  The offsets are printed to
the SysMin internal buffer.  These can  be viewed by stopping the target and
looking at SysMin in ROV.

Application Design Details
--------------------------
This application uses one tasks:
  'mouseHostTask' performs the following actions:
      Waits for a HID mouse device to be connected to the USB host port.

      Gets the status of the device's buttons and updates the LEDs accordingly.

      Prints the mouse's movement offsets via System_printf.  SysMin is used
      for this example; the output is redirected to the internal buffer.

For GNU and IAR users, please read the following website for details about
semi-hosting:
    http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting
