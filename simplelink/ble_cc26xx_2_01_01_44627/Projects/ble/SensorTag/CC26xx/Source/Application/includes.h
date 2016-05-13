#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INCLUDE_KEY


#include <xdc/runtime/System.h>
#include <ti/sysbios/BIOS.h>

#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>

#include "ICall.h"
#include "gatt.h"
#include "hci.h"
#include "gapgattserver.h"
#include "gattservapp.h"
#include "gapbondmgr.h"
#include "osal_snv.h"
#include "ICallBleAPIMSG.h"
#include "util.h"

#include "peripheral.h"


#include "Board.h"

#include "bsp_i2c.h"
#include "bsp_spi.h"
#include "bsp_uart.h"

#include "keyScan.h"
#include "oled.h"
#include "SmBus_Driver.h"
#include "ChargeDetection.h"



#endif

