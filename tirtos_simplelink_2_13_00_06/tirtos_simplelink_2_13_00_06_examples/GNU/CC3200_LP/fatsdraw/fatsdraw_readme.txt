Example Summary
---------------
Sample application to read and write data onto a SD Card (SPI interface)

Board Overview
--------------
Board_LED0    Indicates that the board was initialized within main()
Board_SDSPI0  Connection to SD card

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| CC3200            | Requires an external SD Card boosterpack               |
| EK-TM4C123GXL     |                                                        |
| ----------------- | ------------------------------------------------------ |
| DK-TM4C123G       | It uses the onboard SD Card slot.                      |
| DK-TM4C129X       |                                                        |
| ----------------- | ------------------------------------------------------ |
| EK-TM4C1294XL     | Requires an external SD Card boosterpack               |
| ----------------- | ------------------------------------------------------ |
| MSP-EXP430F5529   | It uses the onboard SD Card slot.                      |
| ----------------- | ------------------------------------------------------ |
| MSP-EXP430F5529LP | Requires an external SD Card boosterpack               |
| MSP-EXP430FR5969LP|                                                        |
| MSP-EXP432P401RLP |                                                        |
| ----------------- | ------------------------------------------------------ |
| TMDXDOCK28M36     | It uses the onboard SD Card slot.                      |
| ----------------- | ------------------------------------------------------ |
| TMDXDOCKH52C1     | It uses the onboard SD Card slot.                      |
|                   | Connectivity Bank 22-25: B-C position (SD card slot)   |
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
    "***********************************************************************\n"

The "input.txt" file is then read and it's contents are written to a new file
called "output.txt". If the file already exists on the SD card, it will be
overwritten.

The contents of the "output.txt" file are then written to the console.

Application Design Details
--------------------------
This application demonstrates how to use TI-RTOS's SD Card driver to read and
write data onto a SD Card using FatFS API calls (f_open, f_read, f_write, etc).

This application uses SysStd instead of SysMin. This was done because
real-time was not a concern and the size of the internal SysMin had to
be large to hold the output. Please refer to the TI-RTOS User Guide's
"Generating printf Output" for a comparison of the different System
Support implementations.

This application uses one task:
  'fatSDTask' performs the following actions:
      Create and initialize SDSPI driver object.

      Prints the disk size and the amount of free space on it.

      Open the "input.txt" file.  If not found, create the file for reading and
      writing.  Write the default message to the file.

      Open the "output.txt" file.  Content will be overwritten if found.

      Copy the contents of "input.txt" to "output.txt".  Close both files
      afterwards.

      Open "output.txt", read the file and print the contents to
      System_printf. And closes "output.txt".

      Again, prints the disk size and the amount of free space on it. If the
      disk was empty, the difference in free space should can be observed.

      The SDSPI driver is closed and the example is terminated.

For GNU and IAR users, please read the following website for details about
semi-hosting:
    http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting
