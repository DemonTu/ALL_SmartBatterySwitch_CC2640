Example Summary
----------------
Example that uses the I2S driver to echo back to the Audio recieved from the
microphone over the speaker.

Peripherals Exercised
---------------------
Board_LED0      Indicates that the board was initialized within main()
Board_I2C0      Used to configure the TI codec on the Audio booster pack
Board_I2S0      Used to echo sounds received on the microphone

Resources & Jumper Settings
---------------------------
Please refer to the development board's specific "Settings and Resources"
section in the Getting Started Guide.

Example Usage
-------------
Run the example. Board_LED0 turns ON to indicate TI-RTOS driver
initialization is complete.

Mount a booster pack with microphone and speaker connected.

The target echoes sounds captured on the microphone.


Application Design Details
--------------------------
This example shows how to initialize the I2S driver in Issue/Reclaim mode
mode with audio echo.

Two tasks, "rxEchoFxn" and "txEchoFxn", are used to read data from the
microphone and write the data back to the speaker.


The drivers by default are non-instrumented. So the following pulls in the
non instrumented I2S,I2C and GPIO libraries
	TIRTOS.useGPIO = true;
	TIRTOS.useI2C  = true;

For GNU and IAR users, please read the following website for details about
semi-hosting:
    http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting

Using the I2S DMA Driver
-------------------------
The CC3200 has DMA support for the I2S driver.  For
these platforms, the I2S example will work with the I2S
driver with DMA support.
