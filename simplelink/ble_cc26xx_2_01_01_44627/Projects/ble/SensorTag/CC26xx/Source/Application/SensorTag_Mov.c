/*******************************************************************************
  Filename:       SensorTag_Mov.c
  Revised:        $Date: 2013-11-06 17:27:44 +0100 (on, 06 nov 2013) $
  Revision:       $Revision: 35922 $

  Description:    This file contains the Movement Processor sub-application.
                  It uses the MPU-9250 Wake-on-movement feature to allow the
                  MPU to turn off the gyroscope and magnetometer when no activity 
				  is detected.

  Copyright 2015  Texas Instruments Incorporated. All rights reserved.

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
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
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
*******************************************************************************/

/*********************************************************************
 * INCLUDES
 */
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Queue.h>
#include <ICall.h>

#include "gatt.h"
#include "gattservapp.h"

#include "Board.h"
#include "movementservice.h"
#include "SensorTag_Mov.h"
#include "sensor.h"
#include "util.h"
#include "string.h"
#include "bsp_uart.h"
#include "userAppTask.h"
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * CONSTANTS and MACROS
 */
// Length of the data for this sensor
#define SENSOR_DATA_LEN         MOVEMENT_DATA_LEN

// Event flag for this sensor
#define SENSOR_EVT              ST_GYROSCOPE_SENSOR_EVT

// Default value is 10.
// Value which is less than 15 would trigger interrupt even though the device 
// remains unmoved persistently.
// Max value is 255.
#define WOM_THR                  128

// Local memory cache for sensor data read/write external flash
// SPI operation in current driver would handle 256 bytes only each time.
// Our one complete data payload is 20 bytes so 240 bytes might be a good choice.
#define SENSOR_DATA_CACHE_SIZE  240

#define MOV_WOM_ENABLE          0x0080

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL VARIABLES
 */

/*********************************************************************
 * EXTERNAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

// Application state variables

// MPU config:
// bit 0-2:   accelerometer enable(z,y,x)
// bit 3-5:   gyroscope enable (z,y,x)
// bit 6:     magnetometer enable
// bit 7:     WOM enable
// bit 8-9:   accelerometer range (2,4,8,16)


/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void sensorChangeCB( uint8_t paramID);

/*********************************************************************
 * PROFILE CALLBACKS
 */
static sensorCBs_t sensorCallbacks =
{
  sensorChangeCB,  // Characteristic value change callback
};

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SensorTagMov_init
 *
 * @brief   Initialization function for the SensorTag movement sub-application
 *
 */
void SensorTagMov_init( void )
{
	// Add service
	Movement_addService();

	// Register callbacks with profile
	Movement_registerAppCBs(&sensorCallbacks);
	
	Movement_setParameter(SENSOR_CONF, 4, (void *)&sysPara.showChar);
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SensorTagMov_processCharChangeEvt
 *
 * @brief   SensorTag Movement event handling
 *
 */
void SensorTagMov_processCharChangeEvt(uint8_t paramID)
{
  //uint8_t newValue8;

  switch (paramID)
  {
    case SENSOR_DATA:
      // Should not get here
      break;
      
    case SENSOR_CONF:
		{
			uint8_t temp[4]={0};
			Movement_getParameter(SENSOR_CONF, temp);
			uartWriteDebug(temp, 4);
			if (temp[2]&0x80)
			{
				/* ´ó³¡ */
				if (temp[1]<7 &&temp[0]<=0x46 && temp[0]>=0x41)
				{
					sysPara.coordinateBac = sysPara.coordinate;
					sysPara.showChar      = temp[0];
					sysPara.coordinate    = temp[1];
					sysPara.deviceNum[0]  = temp[2];
					sysPara.deviceNum[1]  =	temp[3];
					userSystemParaSave(&sysPara);
				}
			}
			else
			{
				/* ´ó³¡ */
				if (temp[1]<5 &&temp[0]<=0x44 && temp[0]>=0x41)
				{
					sysPara.coordinateBac = sysPara.coordinate;
					sysPara.showChar      = temp[0];
					sysPara.coordinate    = temp[1];
					sysPara.deviceNum[0]  = temp[2];
					sysPara.deviceNum[1]  =	temp[3];
					userSystemParaSave(&sysPara);
				}
			}
						
		}
      break;     
      
      
    case SENSOR_BATTERY:
      // Should not get here
      break;  
        
    default:
       // Should not get here
       break;
  }
}






/*********************************************************************
 * @fn      SensorTagMov_updateBatteryInfo
 *
 * @brief   Update battery info if there is change. 
 *          In unit of percentage.
 *
 * @return  none
 */
void SensorTagMov_updateBatteryInfo(uint16_t value)
{
  uint8_t currentValue = 0;
  Movement_getParameter(SENSOR_BATTERY, &currentValue);
  
  if (currentValue != (uint8_t)value)
  {
    Movement_setParameter(SENSOR_BATTERY, 1, &value);
  }
}


/*********************************************************************
 * @fn      sensorChangeCB
 *
 * @brief   Callback from Movement Service indicating a value change
 *
 * @param   paramID - parameter ID of the value that was changed.
 *
 * @return  none
 */
static void sensorChangeCB(uint8_t paramID)
{
  // Wake up the application thread
  SensorTag_charValueChangeCB(SERVICE_ID_MOV, paramID);
}

