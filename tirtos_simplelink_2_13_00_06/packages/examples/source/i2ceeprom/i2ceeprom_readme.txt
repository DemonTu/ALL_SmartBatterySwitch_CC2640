Example Summary
---------------
Sample application to read and write onto an on-board I2C EEPROM IC.

Peripherals Exercised
---------------------
Board_LED0        Indicator LED
Board_I2C_EEPROM  I2C used to communicate with I2C EEPROM.

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide. For convenience, a short summary is also
shown below.

| Development board | Notes                                                  |
| ================= | ====================================================== |
| TMDXDOCKH52C1     | It uses the onboard I2C EEPROM                         |
|                   | Connectivity Bank 20-21: B-C position (I2C EEPROM)     |
|                   | J6: 2-3 position (I2C EEPROM Write protection off)     |
| ----------------- | ------------------------------------------------------ |

Example Usage
-------------
Run the example. Board_LED0 turns ON to indicate TI-RTOS driver
initialization is complete.

A confirmation message will be printed on System_printf after the EEPROM is
erased.

A page with incremental values is written to the EEPROM.  The memory is compared
to ensure the data was written correctly.  A confirmation message will
be written to System_printf.  The following message should appear:

    "Page was successfully written with data\n"

The example exits afterwards.

Application Design Details
--------------------------
This application uses one task:
  'eepromTask' performs the following actions:
      Create and initialize I2C driver object.

      Clear EEPROM memory by writing 0xFF's to memory.

      Wait for the I2C EEPROM to acknowledge, indicating the page write
      operation was completed.

      Read the EEPROM page and verify was erased.

      Write incrementing data into the "Erased" page.

      Wait for the I2C EEPROM to acknowledge, indicating the page write
      operation has completed.

      Read the EEPROM page to verify it was written correctly.

      Close the I2C object and terminate execution.

For GNU and IAR users, please read the following website for details about
semi-hosting:
    http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting
