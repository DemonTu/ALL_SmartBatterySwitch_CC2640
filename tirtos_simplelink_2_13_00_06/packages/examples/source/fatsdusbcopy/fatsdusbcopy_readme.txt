Example Summary
---------------
Sample application to read data from a SD Card (SPI interface) and onto a
USB flash drive (USB Host Mass Storage Class).

Peripherals Exercised
---------------------
Board_LED0           Indicates that the board was initialized within main()
Board_SDSPI0         Interface to communicate with SD Card
Board_USBMSCHFatFs0  USB mass storage driver instance

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| DK-TM4C123G       | It uses the onboard SD Card slot and an external USB   |
| DK-TM4C129X       | OTG dongle.                                            |
| ----------------- | ------------------------------------------------------ |
| EK-TM4C1294XL     | Requires an external SD Card boosterpack and USB OTG   |
|                   | dongle.                                                |
| ----------------- | ------------------------------------------------------ |
| TMDXDOCK28M36     | It uses the onboard SD Card slot and an external USB   |
|                   | OTG dongle.                                            |
|                   | J2-J7: 1-2 position (USB connector)                    |
| ----------------- | ------------------------------------------------------ |
| TMDXDOCKH52C1     | It uses the onboard SD Card slot and an external USB   |
|                   | OTG dongle.                                            |
|                   | Connectivity Bank 22-25: B-C position (SD card slot)   |
|                   | Connectivity Bank 30-31: B-C position (USB connector)  |
| ----------------- | ------------------------------------------------------ |


Example Usage
-------------
Run the example. Board_LED0 turns ON to indicate TI-RTOS driver initialization
is complete.

The example proceeds to read the SD card. If an "input.txt" file is not found,
the file is created on the SD card with the following text:

    "***********************************************************************\n"
    "0         1         2         3         4         5         6         7\n"
    "01234567890123456789012345678901234567890123456789012345678901234567890\n"
    "This is some text to be inserted into the inputfile if there isn't     \n"
    "already an existing file located on the SDCard.                        \n"
    "If an inputfile already exists, or if the file was already once        \n"
    "generated, then the inputfile will NOT be modified.                    \n"
    "\n"
    "This output file is copied from the SDCard to the USB Thumb Drive      \n"
    "End of Demo. Now, get back to work!                                    \n"
    "***********************************************************************\n"

The "input.txt" file on the SD card is read and it's contents are written to a
new file called "output.txt" on the Flash Drive. If the file already exists on
the Flash Drive, it will be overwritten.

The contents of the "output.txt" file are then written to the console.

Application Design Details
--------------------------
This application demonstrates how to use TI-RTOS's SD Card driver to read and
write data onto a SD Card or a USB Flash Drive using C I/O API calls
(fopen, fread, fwrite, etc...).

The TMDXDOCKH52C1 development board has a KNOWN issue with USB HOST operation.
If you are using Rev 1.0, you must remove AND short R230 on the control card!!
Please refer to the Getting Started Guide's Board section for more details.

This application uses one task:
  'fatsdUSBCopyTask' performs the following actions:
      Create and initialize SDSPI and USBMSCHFatFS driver objects.

      Block execution until the USB Flash Drive is enumerated.

      Open the "input.txt" file on the SD Card.  If not found, create the file
      for reading and writing.  Write the default message to the file.

      Open a "output.txt" file on the USB Drive.  Content will be overwritten
      if found.

      Copy the contents of "input.txt" on the SD Card to "output.txt" on the
      USB Drive.  Close both files afterwards.

      Open "output.txt", read the file and print the contents to System_printf.

      The "output.txt" file, SDSPI driver, and USBMSCHFatFS driver are closed
      and the example is terminated.

For GNU and IAR users, please read the following website for details about
semi-hosting:
    http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting
