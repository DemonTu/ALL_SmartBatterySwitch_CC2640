
MEMORY
{
    RTOS_FLASH  : origin = 0x081010, length = 0x0004f0
    RTOS_ROM    : origin = 0x3F7902, length = 0x0027fe
    RTOS_SRAM   : origin = 0x000780, length = 0x000080
}

SECTIONS
{
    sysbios_1: = 0x3f7902
    sysbios_2: = 0x3f9e98
    sysbios_3: = 0x3fa000
}
