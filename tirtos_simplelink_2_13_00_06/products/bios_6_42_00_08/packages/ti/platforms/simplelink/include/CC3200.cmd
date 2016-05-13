/******************************************************************************
 *
 * Default Linker Command file for the Texas Instruments CC3200
 *
 *****************************************************************************/

--retain=g_pfnVectors

MEMORY
{
    SRAM (RWX) : origin = 0x20004000, length = 0x00030000 - 0x4000
}

/* The following command line options are set as part of the CCS project.    */
/* If you are building using the command line, or for some reason want to    */
/* define them here, you can uncomment and modify these lines as needed.     */
/* If you are using CCS for building, it is probably better to make any such */
/* modifications in your CCS project and leave this file alone.              */
/*                                                                           */
/* --heap_size=0                                                             */
/* --stack_size=256                                                          */
/* --library=rtsv7M4_T_le_eabi.lib                                           */

/* Section allocation in memory */

SECTIONS
{
    .intvecs:   > 0x20004000
    .text   :   > SRAM
    .const  :   > SRAM
    .cinit  :   > SRAM
    .pinit  :   > SRAM
    .init_array : > SRAM

    .vtable :   > 0x20004000
    .data   :   > SRAM
    .bss    :   > SRAM
    .sysmem :   > SRAM
    .stack  :   > SRAM
}

__STACK_TOP = __stack + 512;
