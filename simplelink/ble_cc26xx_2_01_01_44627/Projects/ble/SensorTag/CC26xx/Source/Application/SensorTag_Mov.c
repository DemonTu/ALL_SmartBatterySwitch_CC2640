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
#include "sensor_mpu9250.h"
#include "sensor_h3lisXXXdl.h"
#include "sensor.h"
#include "util.h"
#include "string.h"

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
static Clock_Struct periodicClock;
static uint16_t sensorPeriod;
static volatile bool sensorReadScheduled;
static uint8_t sensorData[SENSOR_DATA_LEN];

// Application state variables

// MPU config:
// bit 0-2:   accelerometer enable(z,y,x)
// bit 3-5:   gyroscope enable (z,y,x)
// bit 6:     magnetometer enable
// bit 7:     WOM enable
// bit 8-9:   accelerometer range (2,4,8,16)
static uint16_t mpuConfig;

static volatile bool mpuDataRdy;
static uint8_t mpuIntStatus;

static volatile bool h3lisDataRdy;

static uint8_t nMotions;
static volatile bool sensorWomEnabled;

// RTC
static uint32_t currentTimeMSec;
static uint32_t prevClockTicks;
static uint16_t remUsTicks;

static uint8_t sensorDataMemCache[SENSOR_DATA_CACHE_SIZE] = {0xFF};
static uint8_t *pMemCacheCurrent = sensorDataMemCache;
static uint8_t *pMemCacheEnd = sensorDataMemCache + SENSOR_DATA_CACHE_SIZE;
// Number of valid data available in memory cache for reading
static volatile uint16_t validDataCounts;
// Number of valid time counts for recording sensor data
static volatile int32_t validTimeCounts;

static uint16_t h3lisMeasure[3] = {0};
static uint16_t mpuMeasuer[6] = {0};

/*********************************************************************
 * LOCAL FUNCTIONS
 */
static void sensorChangeCB( uint8_t paramID);
static void initCharacteristicValue( uint8_t paramID, uint8_t value,
                                    uint8_t paramLen);
static void SensorTagMov_clockHandler(UArg arg);
static void setCurrentTimestamp(uint16_t *sensorData);

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

	SensorTagMov_reset();


	// Create continuous clock for internal periodic events.
	Util_constructClock(&periodicClock, SensorTagMov_clockHandler,
	                  1000, sensorPeriod, false, 0);
}

/*********************************************************************
 * PUBLIC FUNCTIONS
 */

/*********************************************************************
 * @fn      SensorTagMov_processInterrupt
 *
 * @brief   Interrupt handler for MPU
 *
 */
void SensorTagMov_processInterrupt(void)
{
  // Wake up the application thread
  mpuDataRdy = true;
  sensorReadScheduled = true;
  Semaphore_post(sem);
}

/*********************************************************************
 * @fn      SensorTagH3lis_processInterrupt
 *
 * @brief   Interrupt handler for STM H3LISxxx
 *
 */
void SensorTagH3lis_processInterrupt(void)
{
  // Wake up the application thread
  h3lisDataRdy = true;
  sensorReadScheduled = true;
  Semaphore_post(sem);
}

/*********************************************************************
 * @fn      SensorTagMov_processSensorEvent
 *
 * @brief   SensorTag Gyroscope sensor event processor.
 *
 */
void SensorTagMov_processSensorEvent(void)
{
  uint16_t sensorBuf[6] = {0};
  uint8_t dataBuf[12] = {0};
  uint8_t static dFlag = 0;
  uint16_t dBuf = 0; 
  
  if (sensorReadScheduled)
  {
    uint8_t axes;
    
    axes = mpuConfig & MPU_AX_ALL;
    
    if ((mpuConfig & MOV_WOM_ENABLE) || ((axes != ST_CFG_SENSOR_DISABLE) && (axes != ST_CFG_ERROR)))
    {
      // Get interrupt status (clears interrupt)
      mpuIntStatus = sensorMpu9250IntStatus();
      
      // Process gyro and accelerometer
      if (mpuDataRdy)
      {
        if (mpuIntStatus & MPU_MOVEMENT)
        {
          // Motion detected (small filter)
          nMotions++;
          if (2 == nMotions)
          {
            uint8_t advEnable = TRUE;
            SensorTagMov_setSensorWom(false);
            SensorTag_checkBatteryInfo(true);
            GAPRole_SetParameter(GAPROLE_ADVERT_ENABLED, sizeof(uint8_t), &advEnable);
            // SensorTag_resetClockPeriod();
            nMotions = 0;
          } else 
          {
            SensorTagMov_setSensorWom(false);
            delay_ms(100);
            SensorTagMov_setSensorWom(true);
          }
        }
        else if (mpuIntStatus & MPU_DATA_READY)
        {
          // Clear data
          //memset(sensorData,0,SENSOR_DATA_LEN);
          
          // Read accelerometer data                    
          sensorMpu9250AccRead(sensorBuf);          
          
          // Read gyro data
          sensorMpu9250GyroRead(&sensorBuf[3]);
          
          for(int i=0; i<6; i++)
          {
            sensorBuf[i] = (sensorBuf[i] & 0xFFF0) | (sensorBuf[i+1]>>12);
            i++;
            sensorBuf[i] = sensorBuf[i]<<4;
          }
          
          memcpy(&dataBuf[0], &sensorBuf[0], 12);
          convertToLe(dataBuf, 12);
          for(int j=0; j<3; j++)
          {
            memcpy(&sensorData[j*3 + dFlag*9], &dataBuf[j*4], 3);
          }
          
          dFlag += 1;
          if(dFlag == 2)
          {
            // Set current timestamp
            setCurrentTimestamp((uint16_t*)&sensorData[18]);
            
            // Send data
            Movement_setParameter(SENSOR_DATA, SENSOR_DATA_LEN, sensorData);
            
            // Check if we need to write flash
            Movement_getParameter(SENSOR_OPMODE, &axes);
            if (SENSOR_OPMODE_FSM_WF == axes || SENSOR_OPMODE_FSM_RTWF == axes)
            {
              // Make sure we have enough space
              if ((pMemCacheEnd - pMemCacheCurrent) >= SENSOR_DATA_LEN)
              {
                // Copy data to memory cache
                memcpy(pMemCacheCurrent, sensorData, SENSOR_DATA_LEN);
                pMemCacheCurrent += SENSOR_DATA_LEN;
                
                // Check if memory cache is full
                if (pMemCacheEnd == pMemCacheCurrent)
                {
                  // Write flash
                  SensorTag_manipulateEflData(true, SENSOR_DATA_CACHE_SIZE, sensorDataMemCache);
                  pMemCacheCurrent = sensorDataMemCache;
                }
                
              } else
              {
                // Error handling.
                // TODO
                ;
              }
            }
            memset(sensorData,0,SENSOR_DATA_LEN);
            dFlag = 0;
          }
        }     
        mpuDataRdy = false;
      }
    }
    
    sensorReadScheduled = false;
  }
  sensorH3lisEnableInt(true);
}


/*********************************************************************
 * @fn      SensorTagMov_processCharChangeEvt
 *
 * @brief   SensorTag Movement event handling
 *
 */
void SensorTagMov_processCharChangeEvt(uint8_t paramID)
{
  uint8_t newValue8;

  switch (paramID)
  {
    case SENSOR_DATA:
      // Should not get here
      break;
      
    case SENSOR_CONF:
      newValue8 = SENSOR_CONF_INVALID;
      Movement_getParameter(SENSOR_CONF, &newValue8);
      
      switch (newValue8)
      {
      case SENSOR_CONF_RTC:
        {
          uint16_t newValue16 = SENSOR_CONF_RTC << 8;
          Movement_getParameter(SENSOR_CONF, &newValue16);
          currentTimeMSec = newValue16;
        }
        break;
        
      case SENSOR_CONF_FREQ:
        newValue8 = SENSOR_CONF_FREQ;
        Movement_getParameter(SENSOR_CONF, &newValue8);
        sensorPeriod = newValue8;
        Util_rescheduleClock(&periodicClock,sensorPeriod);  
        break;
        
      case SENSOR_CONF_OPMODE:
        SensorTagMov_updateFsm(SENSOR_CONF_OPMODE_INVALID);
        break;
        
      case SENSOR_CONF_NAME:
        {
          // GAP_DEVICE_NAME_LEN is 21
          uint8_t newDevName[SENSOR_CONFIG_NAME_PAYLOAD_DATA_LEN_MAX];
          newValue8 = SENSOR_CONF_NAME_LEN;
          Movement_getParameter(SENSOR_CONF, &newValue8);
          newDevName[0] = SENSOR_CONF_NAME;
          Movement_getParameter(SENSOR_CONF, newDevName);
          
          SensorTag_updateDeviceName(true, newValue8, newDevName);
        }
        break;      

      case SENSOR_CONF_VENDOR:
        
        // Get vendor command
        newValue8 = SENSOR_CONF_VENDOR;
        Movement_getParameter(SENSOR_CONF, &newValue8);
        
        switch (newValue8)
        {
        case SENSOR_VENDOR_SET_PREFIX_NAME:
          {
            // GAP_DEVICE_NAME_LEN is 21
            uint8_t newDevPrefixName[SENSOR_CONFIG_VENDOR_PREFIX_NAME_LEN_MAX];
            newValue8 = SENSOR_CONF_NAME_LEN;
            Movement_getParameter(SENSOR_CONF, &newValue8);
            newDevPrefixName[0] = SENSOR_CONF_NAME;
            Movement_getParameter(SENSOR_CONF, newDevPrefixName);
            SensorTag_updateDeviceName(false, newValue8, newDevPrefixName);          
          }
          break;
          
        case SENSOR_VENDOR_SET_CALI:
          {
            uint8_t sensorCaliData[SENSOR_CONFIG_VENDOR_CALI_PAYLOAD_DATA_LEN];
            sensorCaliData[0] = SENSOR_CONF_VENDOR_DATA;
            Movement_getParameter(SENSOR_CONF, sensorCaliData);
            SensorTag_updateSensorCaliData(sensorCaliData);          
          }          
          break;
          
        case SENSOR_VENDOR_ERASE_DATA:
          SensorTag_erasDeviceConfigData();
          break;
          
        case SENSOR_VENDOR_RESET_HW:
          SensorTag_resetHardware();
          break;
          
        default:
          // Should not get here
          break;
        }
        
        break; 
      }      
      break;     
      
    case SENSOR_OPMODE:
      // Should not get here
      break;
        
    case SENSOR_FREQ:
      // Should not get here
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
 * @fn      SensorTagMov_reset
 *
 * @brief   Reset characteristics and disable sensor
 *
 * @param   none
 *
 * @return  none
 */
void SensorTagMov_reset (void)
{
  if (sensorWomEnabled)
    return;
  
  initCharacteristicValue(SENSOR_DATA, 0, SENSOR_DATA_LEN);

  

  // Stop scheduled data measurements
  Util_stopClock(&periodicClock);
}

/*********************************************************************
 * @fn      SensorTagMov_setSensorInt
 *
 * @brief   Enable or Disable MPU-9250's interrupt.
 *
 * @param   isEnable - true for enabling MPU-9250's interrupt register.
 *
 * @return  none
 */
void SensorTagMov_setSensorInt(bool isEnable)
{

  if(!isEnable)
  {
    mpuDataRdy = false;
  }

}

/*********************************************************************
 * @fn      SensorTagMov_setSensorPower
 *
 * @brief   Power On or Off MPU-9250.
 *          By default, we would NOT enable MPU-9250's interrupt register on 
 *          power-on request, but we would disable it on power-off request.
 *
 * @param   isPowerOn - true for powering on MPU-9250 and enabling each sensor.
 *
 * @return  none
 */
void SensorTagMov_setSensorPower(bool isPowerOn)
{
  uint16_t newCfg;
  
  // Power on and enable sensors
  if (isPowerOn)
  {
    // Power on and initialize the device
    sensorMpu9250PowerOn();

    // 0x3F : Enable XYZ axis of both Accelerometer and Gyroscope
    newCfg = ((GYRO_RANGE_DEFAULT << 10)|(ACC_RANGE_DEFAULT << 8)|(0x7F));
    
    // Bitmap tells which axis to enable (bit 0: X, bit 1: Y, bit 2: Z)
    sensorMpu9250Enable(newCfg & 0xFF);
    
    // Start scheduled data measurements
    Util_startClock(&periodicClock);

    // Set the accelerometer range
    sensorMpu9250AccSetRange(newCfg>>8);
    
    // Set the gyroscope range
    sensorMpu9250GyroReadRange();
    sensorMpu9250GyroSetRange(newCfg>>10);
    
    // We do NOT enable data ready interrupt
    // sensorMpu9250IntEnable(true);
    
#if defined(H3LIS200DL) || defined(H3LIS331DL)  
    // Powering on STM H3LIS also enables axes automatically !
    sensorH3lisPowerOn();
    sensorH3lisEnable(true);
#endif  
    
  } 
  else 
  {
    newCfg = ST_CFG_SENSOR_DISABLE;

    // All axes off, power off MPU
    if (mpuConfig != ST_CFG_SENSOR_DISABLE)
    {
      // Stop scheduled data measurements
      Util_stopClock(&periodicClock);
      
      // Disable data ready interrupt
      sensorMpu9250IntEnable(false);
      
      // Disable all axes
      sensorMpu9250Enable(0);
      
      // Set internal state
      mpuConfig = newCfg;
      initCharacteristicValue(SENSOR_DATA, 0, SENSOR_DATA_LEN);
      
      // Power off the device
      sensorMpu9250PowerOff();   
      
#if defined(H3LIS200DL) || defined(H3LIS331DL)  
      sensorH3lisPowerOff();
#endif       
    }
  }

  delay_ms(5);
  mpuConfig = newCfg;
}

/*********************************************************************
 * @fn      SensorTagMov_updateFsm
 *
 * @brief   Update FSM due to event from characteristic changed callback or 
 *          application layer.
 *          Application layer would call this API mainly for two situations,
 *              1) SENSOR_CONF_OPMODE_RF_STOP : When reading flash is out of data.
 *              2) SENSOR_CONF_OPMODE_WF_STOP : When requested time period is reached in writing flash mode
 *
 * @return  none
 */
void SensorTagMov_updateFsm(uint8_t event)
{
  uint8_t currentState;
  Movement_getParameter(SENSOR_OPMODE, &currentState); 
  
  // Check if caller is from SensorTagMov_ProcessCharChangeEvt()
  if (SENSOR_CONF_OPMODE_INVALID == event) {
    event = SENSOR_CONF_OPMODE;
    Movement_getParameter(SENSOR_CONF, &event);
    
    // NOTE : Get extra data from movementservice in CORRECT operation ONLY !
  }

  if (event < SENSOR_CONF_OPMODE_INVALID && currentState < SENSOR_OPMODE_FSM_INVALID)
    fsm_op_table[currentState][event]();  
}

/*********************************************************************
 * @fn      SensorTagMov_getFsmState
 *
 * @brief   Get current FSM state from movementservice.
 *
 * @return  Current FSM state
 */
uint8_t SensorTagMov_getFsmState(void)
{
  uint8_t currentState;
  Movement_getParameter(SENSOR_OPMODE, &currentState); 
  return currentState;
}

/*********************************************************************
 * @fn      SensorTagMov_handleFsmEvent
 *
 * @brief   Handle FSM related events.
 *
 * @param   param - event to handle.
 *
 * @return  none
 */
void SensorTagMov_handleFsmEvent(uint8_t param)
{
  switch (param)
  {
  case SENSOR_FSM_CB_EVENT_RF:
    SensorTagMov_handleFsmRfMode();
    break;
  case SENSOR_FSM_CB_EVENT_WF:
    SensorTagMov_handleFsmWfMode();
    break;
  case SENSOR_FSM_CB_EVENT_NOTIFY_ALL:
    Movement_notifyOpmodeUpdated(true);
    break;
  case SENSOR_FSM_CB_EVENT_NOTIFY_OB:
    Movement_notifyOpmodeUpdated(false);
    break;
  default:
    // Should not get here
    break;
  }
  return;
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
 * @fn      SensorTagMov_verifyConnection
 *
 * @brief   Verify current connection by valid read/write characteristic operations.
 *
 * @return  true if there was/were valid operations.
 */
bool SensorTagMov_verifyConnection(void)
{
  return Movement_verifyConnection();
}


/*********************************************************************
* Private functions
*/

/*********************************************************************
 * @fn      SensorTagMov_clockHandler
 *
 * @brief   Handler function for clock time-outs.
 *
 * @param   arg - event type
 *
 * @return  none
 */
static void SensorTagMov_clockHandler(UArg arg)
{
  uint8_t currentState;
  
  // Schedule readout periodically
  sensorReadScheduled = true;
  Movement_getParameter(SENSOR_OPMODE, &currentState); 
  
  if (SENSOR_OPMODE_FSM_RF == currentState)
  {
    SensorTag_fsmModeCB(SENSOR_FSM_CB_EVENT_RF);
    return;
  }
  
  Semaphore_post(sem);
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

/*********************************************************************
 * @fn      initCharacteristicValue
 *
 * @brief   Initialize a characteristic value
 *
 * @param   paramID - parameter ID of the value is to be cleared
 *
 * @param   value - value to initialize with
 *
 * @param   paramLen - length of the parameter
 *
 * @return  none
 */
static void initCharacteristicValue( uint8_t paramID, uint8_t value,
                                    uint8_t paramLen )
{
  memset(sensorData,value,paramLen);
  Movement_setParameter(paramID, paramLen, sensorData);
}

/*********************************************************************
 * @fn      resetTimestampVars
 *
 * @brief   Reset timestamp related variables including clock ticks and logical
 *          current time.
 *
 * @param   none
 *
 * @return  none
 */
static void resetTimestampVars()
{
  currentTimeMSec = 0;
  prevClockTicks = Clock_getTicks();
  remUsTicks = 0;
}

/*********************************************************************
 * @fn      setCurrentTimestamp
 *
 * @brief   Get current timestamp in milliseconds from OSAL and store it to 
 *          input pointer. We only get lower 16-bit value.
 *
 * @param   Pointer to position of timestamp in sensor data array
 *
 * @return  none
 */
static void setCurrentTimestamp(uint16_t *sensorData)
{
  uint32_t currentClockTicks, elapsedClockTicks;
  uint32_t elapsedMSec = 0;
  
  currentClockTicks = Clock_getTicks();
  
  // Check that time has passed.
  if (currentClockTicks != prevClockTicks)
  {
    // To make sure time has passed and that a negative difference is not 
    // calculated, check if the tick count is greater than the previous
    // measurement's.
    if (currentClockTicks > prevClockTicks)
    {
      // Get the elapsed clock ticks.
      elapsedClockTicks = currentClockTicks - prevClockTicks;
    }
    // Else tick count rolled over.
    else
    {
      // Get the elapsed clock ticks, accounting for the roll over.
      elapsedClockTicks = (0xFFFFFFFF - prevClockTicks) + currentClockTicks + 1;
    }
    
    // Convert to milliseconds.
    elapsedMSec = (elapsedClockTicks * Clock_tickPeriod) / 1000;
    
    // Find remainder.
    remUsTicks += (elapsedClockTicks * Clock_tickPeriod) % 1000;

    // If the running total of remaining microseconds is greater than or equal 
    // to one millisecond.
    if (remUsTicks >= 1000)
    {
      // Add in the extra millisecond.
      // Note: the remainder has an open upper limit of 2 milliseconds.
      elapsedMSec += 1;
      
      // Adjust the remainder.
      remUsTicks %= 1000;
    }
  }

  // If time has passed
  if (elapsedMSec)
  {
    // Store the tick count for the next iteration through this function.
    prevClockTicks = currentClockTicks;
    currentTimeMSec += elapsedMSec;
  }    
  
  // In unit of millisecond
  *sensorData = (uint16_t)(currentTimeMSec);
  // *sensorData = (uint16_t)(ICall_getTicks() * ICall_getTickPeriod() / 1000) - sensorRtcOffset;
  
  // OSAL is removed out of application.
  // *sensorData = (uint16_t)osal_getClockMilliseconds();
  // *sensorData = (uint16_t)(osal_getClockMilliseconds() >> 16);
}



