/**
  @file  main.c
  @brief main entry of the BLE stack sample application.

  <!--
  Copyright 2013 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED ``AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
  -->
*/

#include <xdc/std.h>

#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/family/arm/cc26xx/Power.h>
#include <ti/sysbios/family/arm/cc26xx/PowerCC2650.h>
#include <ti/sysbios/BIOS.h>
#ifdef POWER_SAVING
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <stdbool.h>
#include <driverlib/aon_rtc.h>
#include <driverlib/aon_wuc.h>
#include <driverlib/aux_wuc.h>
#endif
#include <driverlib/ioc.h>

#include "ICall.h"
#include "Board.h"
#include <driverlib/sys_ctrl.h>

#include "bcomdef.h"
#include "peripheral.h"
#include "npi_data.h"
#include "simpleNP.h"

#include <inc/hw_memmap.h>
#include <driverlib/vims.h>

#ifndef USE_DEFAULT_USER_CFG

#include "bleUserConfig.h"

// BLE user defined configuration
bleUserCfg_t user0Cfg = BLE_USER_CFG;

#endif // USE_DEFAULT_USER_CFG

/*******************************************************************************
 * CONSTANTS
 */

#define RFC_MODE_BLE                 PRCM_RFCMODESEL_CURR_MODE1
#define RFC_MODE_ANT                 PRCM_RFCMODESEL_CURR_MODE4
#define RFC_MODE_EVERYTHING_BUT_ANT  PRCM_RFCMODESEL_CURR_MODE5
#define RFC_MODE_EVERYTHING          PRCM_RFCMODESEL_CURR_MODE6

/*******************************************************************************
 * MACROS
 */

#define SET_RFC_BLE_MODE(mode) HWREG( PRCM_BASE + PRCM_O_RFCMODESEL ) = (mode)


/**
 * Exception handler
 */
void exceptionHandler()
{
  volatile int j=1;
  while(j);
}

/*
 *  ======== main ========
 */
int main()
{
  PIN_init(BoardGpioInitTable);

  //enable iCache prefetching
  VIMSConfigure(VIMS_BASE, TRUE, TRUE);
   
  // Enable cache
  VIMSModeSet(VIMS_BASE, VIMS_MODE_ENABLED);

#if defined( USE_FPGA )
    SysCtrlPowerEverything();
#endif // USE_FPGA

   //enable iCache prefetching
   VIMSConfigure(VIMS_BASE, TRUE, TRUE);
   
   // Enable cache
   VIMSModeSet(VIMS_BASE, VIMS_MODE_ENABLED);

#ifndef POWER_SAVING
    /* Set constraints for Standby, powerdown and idle mode */
    Power_setConstraint(Power_SB_DISALLOW);
    Power_setConstraint(Power_IDLE_PD_DISALLOW);
#endif // POWER_SAVING


    IOCPortConfigureSet(IOID_29, IOC_PORT_MCU_SWV,
                   IOC_IOMODE_NORMAL);

    /* Initialize ICall module */
    ICall_init();

    /* Start tasks of external images - Priority 5 (highest priority)*/
    ICall_createRemoteTasks();
           
    /* Kick off profile - Priority 3 */
    GAPRole_createTask();

    /* Kick off Simple processor Task - Priority 1 (lowest priority)*/
    SNP_createTask(); 
    
    /* enable interrupts and start SYS/BIOS */
    BIOS_start();
    
    return 0;
}

/**
 * Error handled to be hooked into TI-RTOS
 */
Void smallErrorHook(Error_Block *eb)
{
  for (;;);
}

/**
 * HAL assert handler required by OSAL memory module.
 */
void halAssertHandler(void)
{
  for (;;);
}
