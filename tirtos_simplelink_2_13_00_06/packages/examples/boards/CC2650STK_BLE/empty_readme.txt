Example Summary
----------------
This example is intended to be a starting point for new development where
a fuller set of kernel features and debug capabilities are enabled.

Peripherals Exercised
---------------------
Board_LED0  Indicates that the board was initialized within main()

Example Usage
-------------
The example lights Board_LED0 as part of the initialization in main().
Then a heartBeat task toggles the LED at a rate determined by the arg0 parameter
for the Task instance in the .cfg file.

Application Design Details
--------------------------
This examples is the same as the "Empty (Minimal)" example except many
developement and debug features are enabled. For example:
    - Logging is enabled
    - Assert checking is enabled
    - Kernel Idle task
    - Instrumented driver modules are used
    - Stack overflow checking
    - Default kernel heap is present

Please refer to the "Memory Footprint Reduction" section in the TI-RTOS User
Guide (spruhd4.pdf) for a complete and detailed list of the differences
between the empty minimal and empty projects.

For GNU and IAR users, please read the following website for details about semi-hosting
http://processors.wiki.ti.com/index.php/TI-RTOS_Examples_SemiHosting.
