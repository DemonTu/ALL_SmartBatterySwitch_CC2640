/*******************************************************************************
*  Filename:       sensor_mpu9250.c
*  Revised:        $Date: 2015-02-05 10:47:02 +0100 (on, 05 feb 2015) $
*  Revision:       $Revision: 12066 $
*
*  Description:    Driver for the Invensys MPU9250 Motion Porcessing Unit
*
*  Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/* -----------------------------------------------------------------------------
*                                          Includes
* ------------------------------------------------------------------------------
*/
#include "board.h"
#include "sensor_mpu9250.h"
#include "sensor.h"
#include "bsp_i2c.h"

/* -----------------------------------------------------------------------------
*                                           Constants
* ------------------------------------------------------------------------------
*/
// Sensor I2C address
#define SENSOR_I2C_ADDRESS            0x68
#define SENSOR_MAG_I2_ADDRESS         0x0C

// Registers
#define SELF_TEST_X_GYRO              0x00 // R/W
#define SELF_TEST_Y_GYRO              0x01 // R/W
#define SELF_TEST_Z_GYRO              0x02 // R/W
#define SELF_TEST_X_ACCEL             0x0D // R/W
#define SELF_TEST_Z_ACCEL             0x0E // R/W
#define SELF_TEST_Y_ACCEL             0x0F // R/W

#define XG_OFFSET_H                   0x13 // R/W
#define XG_OFFSET_L                   0x14 // R/W
#define YG_OFFSET_H                   0x15 // R/W
#define YG_OFFSET_L                   0x16 // R/W
#define ZG_OFFSET_H                   0x17 // R/W
#define ZG_OFFSET_L                   0x18 // R/W

#define SMPLRT_DIV                    0x19 // R/W
#define CONFIG                        0x1A // R/W
#define GYRO_CONFIG                   0x1B // R/W
#define ACCEL_CONFIG                  0x1C // R/W
#define ACCEL_CONFIG_2                0x1D // R/W
#define LP_ACCEL_ODR                  0x1E // R/W
#define WOM_THR                       0x1F // R/W
#define FIFO_EN                       0x23 // R/W

// .. registers 0x24 - 0x36 are not applicable to the SensorTag HW configuration

#define INT_PIN_CFG                   0x37 // R/W
#define INT_ENABLE                    0x38 // R/W
#define INT_STATUS                    0x3A // R
#define ACCEL_XOUT_H                  0x3B // R
#define ACCEL_XOUT_L                  0x3C // R
#define ACCEL_YOUT_H                  0x3D // R
#define ACCEL_YOUT_L                  0x3E // R
#define ACCEL_ZOUT_H                  0x3F // R
#define ACCEL_ZOUT_L                  0x40 // R
#define TEMP_OUT_H                    0x41 // R
#define TEMP_OUT_L                    0x42 // R
#define GYRO_XOUT_H                   0x43 // R
#define GYRO_XOUT_L                   0x44 // R
#define GYRO_YOUT_H                   0x45 // R
#define GYRO_YOUT_L                   0x46 // R
#define GYRO_ZOUT_H                   0x47 // R
#define GYRO_ZOUT_L                   0x48 // R

// .. registers 0x49 - 0x60 are not applicable to the SensorTag HW configuration
// .. registers 0x63 - 0x67 are not applicable to the SensorTag HW configuration

#define SIGNAL_PATH_RESET             0x68 // R/W
#define ACCEL_INTEL_CTRL              0x69 // R/W
#define USER_CTRL                     0x6A // R/W
#define PWR_MGMT_1                    0x6B // R/W
#define PWR_MGMT_2                    0x6C // R/W
#define FIFO_COUNT_H                  0x72 // R/W
#define FIFO_COUNT_L                  0x73 // R/W
#define FIFO_R_W                      0x74 // R/W
#define WHO_AM_I                      0x75 // R/W

// Masks is mpuConfig valiable
#define ACC_CONFIG_MASK               0x38
#define GYRO_CONFIG_MASK              0x07

// Values PWR_MGMT_1
#define MPU_SLEEP                     0x4F  // Sleep + stop all clocks
#define MPU_WAKE_UP                   0x09  // Disable temp. + intern osc

// Values PWR_MGMT_2
#define ALL_AXES                      0x3F
#define GYRO_AXES                     0x07
#define ACC_AXES                      0x38

// Data sizes
#define DATA_SIZE                     6

// Output data rates
#define INV_LPA_0_3125HZ              0
#define INV_LPA_0_625HZ               1
#define INV_LPA_1_25HZ                2
#define INV_LPA_2_5HZ                 3
#define INV_LPA_5HZ                   4
#define INV_LPA_10HZ                  5
#define INV_LPA_20HZ                  6
#define INV_LPA_40HZ                  7
#define INV_LPA_80HZ                  8
#define INV_LPA_160HZ                 9
#define INV_LPA_320HZ                 10
#define INV_LPA_640HZ                 11
#define INV_LPA_STOPPED               255

// Bit values
#define BIT_ANY_RD_CLR                0x10
#define BIT_RAW_RDY_EN                0x01
#define BIT_WOM_EN                    0x40
#define BIT_LPA_CYCLE                 0x20
#define BIT_STBY_XA                   0x20
#define BIT_STBY_YA                   0x10
#define BIT_STBY_ZA                   0x08
#define BIT_STBY_XG                   0x04
#define BIT_STBY_YG                   0x02
#define BIT_STBY_ZG                   0x01
#define BIT_STBY_XYZA                 (BIT_STBY_XA | BIT_STBY_YA | BIT_STBY_ZA)
#define BIT_STBY_XYZG                 (BIT_STBY_XG | BIT_STBY_YG | BIT_STBY_ZG)

// User control register
#define BIT_ACTL                      0x80
#define BIT_LATCH_EN                  0x20

// INT Pin / Bypass Enable Configuration
#define BIT_AUX_IF_EN                 0x20  // I2C_MST_EN
#define BIT_BYPASS_EN                 0x02


// Magnetometer registers
#define MAG_WHO_AM_I                  0x00  // should return 0x48
#define MAG_INFO                      0x01
#define MAG_ST1                       0x02  // data ready status bit 0
#define MAG_XOUT_L	                  0x03  // data
#define MAG_XOUT_H	                  0x04
#define MAG_YOUT_L	                  0x05
#define MAG_YOUT_H	                  0x06
#define MAG_ZOUT_L	                  0x07
#define MAG_ZOUT_H	                  0x08
#define MAG_ST2                       0x09  // Data overflow bit 3 and data read error status bit 2
#define MAG_CNTL                      0x0A  // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
#define MAG_ASTC                      0x0C  // Self test control
#define MAG_I2CDIS                    0x0F  // I2C disable
#define MAG_ASAX                      0x10  // Fuse ROM x-axis sensitivity adjustment value
#define MAG_ASAY                      0x11  // Fuse ROM y-axis sensitivity adjustment value
#define MAG_ASAZ                      0x12  // Fuse ROM z-axis sensitivity adjustment value

#define MAG_DEVICE_ID                 0x48

#define MFS_14BITS                    0     // 0.6 mG per LSB
#define MFS_16BITS                    1     // 0.15 mG per LSB

// Sensor selection/deselection
#define SENSOR_SELECT()               bspI2cSelect(BSP_I2C_INTERFACE_1,SENSOR_I2C_ADDRESS)
#define SENSOR_SELECT_MAG()           bspI2cSelect(BSP_I2C_INTERFACE_1,SENSOR_MAG_I2_ADDRESS)
#define SENSOR_DESELECT()             bspI2cDeselect()

/* -----------------------------------------------------------------------------
*                           Typedefs
* ------------------------------------------------------------------------------
*/

/* -----------------------------------------------------------------------------
*                           Macros
* ------------------------------------------------------------------------------
*/

/* -----------------------------------------------------------------------------
*                           Local Functions
* ------------------------------------------------------------------------------
*/
static void sensorMpuSleep(void);
static void sensorMpu9250WakeUp(void);
static void sensorMpu9250SelectAxes(void);
#ifdef FEATURE_MAGNETOMETER
static void sensorMagInit(void);
static void sensorMagEnable(bool);
static bool sensorMpu9250SetBypass(void);
#endif
/* -----------------------------------------------------------------------------
*                           Local Variables
* ------------------------------------------------------------------------------
*/
static uint8_t mpuConfig;
static uint8_t accRange;
static uint8_t accRangeReg;
static uint8_t val;
#ifdef WOM_ENABLE
static bool isWomEnabled;
#endif

#ifdef FEATURE_MAGNETOMETER
static float calX;
static float calY;
static float calZ;
#endif

/*******************************************************************************
* @fn          sensorMpu9250PowerOn
*
* @brief       This function turns on the power supply to MPU9250
*
* @return      none
*/
void sensorMpu9250PowerOn(void)
{
  extern PIN_Handle hGpioPin;

  // Make sure I2C interface is in a known state
  bspI2cSelect(BSP_I2C_INTERFACE_0,0);
  bspI2cDeselect();

  // Turn on power supply
  PIN_setOutputValue(hGpioPin,Board_MPU_POWER, Board_MPU_POWER_ON);
  delay_ms(100);
  sensorMpu9250Init();
}

/*******************************************************************************
* @fn          sensorMpu9250PowerOff
*
* @brief       This function turns off the power supply to MPU9250
*
* @return      none
*/
void sensorMpu9250PowerOff(void)
{
  extern PIN_Handle hGpioPin;

  // Make sure I2C interface is in a known state
  bspI2cSelect(BSP_I2C_INTERFACE_0,0);
  bspI2cDeselect();

  // Turn off power supply
  PIN_setOutputValue(hGpioPin,Board_MPU_POWER, Board_MPU_POWER_OFF);
}

/*******************************************************************************
* @fn          sensorMpu9250PowerIsOn
*
* @brief       Return true if MPU power is on
*
* @return      none
*/
bool sensorMpu9250PowerIsOn(void)
{
 return PIN_getOutputValue(Board_MPU_POWER) == Board_MPU_POWER_ON;
}

/*******************************************************************************
* @fn          sensorMpu9250Init
*
* @brief       This function initializes the MPU abstraction layer.
*
* @return      True if success
*/
bool sensorMpu9250Init(void)
{
  bool ret;

#ifdef WOM_ENABLE
  isWomEnabled = false;
#endif
  accRange = ACC_RANGE_INVALID;
  mpuConfig = 0;   // All axes off

  if (!SENSOR_SELECT())
  {
    return false;
  }

  // Device reset
  val = 0x80;
  sensorWriteReg(PWR_MGMT_1, &val, 1);
  SENSOR_DESELECT();

  delay_ms(100);

  ret =  sensorMpu9250Test();
  if (ret)
  {
    // Initial configuration
    sensorMpu9250AccSetRange(ACC_RANGE_8G);
#ifdef FEATURE_MAGNETOMETER
    sensorMagInit();
#endif
    // Power save
    sensorMpuSleep();
  }

  return ret;
}


/*******************************************************************************
* @fn          sensorMpu9250IntEnable
*
* @brief       This function enables interrupt output
*
* @param       enable - true if interrupt is to be generated
*
* @return      True if success
*/
bool sensorMpu9250IntEnable(bool enable)
{
  bool ret;

  if (!SENSOR_SELECT())
  {
    return false;
  }

  // Configure INT pin
  val = enable ? BIT_RAW_RDY_EN : 0;
  ret = sensorWriteReg(INT_ENABLE, &val, 1);

  SENSOR_DESELECT();

  return ret;
}

#ifdef WOM_ENABLE
/*******************************************************************************
* @fn          sensorMpu9250WomEnable
*
* @brief       Enable Wake On Motion functionality
*
* @return      True if success
*/
bool sensorMpu9250WomEnable(void)
{
  if (!SENSOR_SELECT())
  {
    return false;
  }

  // Make sure accelerometer is running
  val = 0x09;
  ST_ASSERT(sensorWriteReg(PWR_MGMT_1, &val, 1));

  // Enable accelerometer, disable gyro
  val = 0x07;
  ST_ASSERT(sensorWriteReg(PWR_MGMT_2, &val, 1));

  // Set Accel LPF setting to 184 Hz Bandwidth
  val = 0x01;
  ST_ASSERT(sensorWriteReg(ACCEL_CONFIG_2, &val, 1));

  // Enable Motion Interrupt
  val = BIT_WOM_EN;
  ST_ASSERT(sensorWriteReg(INT_ENABLE, &val, 1));

  // Enable Accel Hardware Intelligence
  val = 0xC0;
  ST_ASSERT(sensorWriteReg(ACCEL_INTEL_CTRL, &val, 1));

  // Set Motion Threshold
  val = 3;
  ST_ASSERT(sensorWriteReg(WOM_THR, &val, 1));

  // Set Frequency of Wake-up
  val = INV_LPA_5HZ;
  ST_ASSERT(sensorWriteReg(LP_ACCEL_ODR, &val, 1));

  // Enable Cycle Mode (Accel Low Power Mode)
  val = 0x29;
  ST_ASSERT(sensorWriteReg(PWR_MGMT_1, &val, 1));

  // Select the current range
  ST_ASSERT(sensorWriteReg(ACCEL_CONFIG, &accRangeReg, 1));

  SENSOR_DESELECT();
  isWomEnabled = true;
  mpuConfig = BIT_STBY_XYZA;

  return true;
}
#endif

/*******************************************************************************
* @fn          sensorMpu9250IntStatus
*
* @brief       Check whether a data or wake on motion interrupt has occurred
*
* @return      Return interrupt status
*/
uint8_t sensorMpu9250IntStatus(void)
{
  uint8_t intStatus;

  if (SENSOR_SELECT())
  {
    sensorReadReg(INT_STATUS,&intStatus,1);
    SENSOR_DESELECT();
  }
  else
  {
    intStatus = 0;
  }

  return intStatus;
}

/*******************************************************************************
* @fn          sensorMpu9250Enable
*
* @brief       Enable acceleromter readout
*
* @param       Axes: Gyro bitmap [0..2], X = 1, Y = 2, Z = 4. 0 = gyro off
* @                  Acc  bitmap [3..5], X = 8, Y = 16, Z = 32. 0 = accelerometer off
*
* @return      None
*/
void sensorMpu9250Enable(uint16_t axes)
{
  if (mpuConfig == 0 && axes != 0)
  {
    // Wake up the sensor if it was off
    sensorMpu9250WakeUp();
  }

  mpuConfig = axes;

  if (mpuConfig != 0)
  {
    // Enable gyro + accelerometer + magnetometer readout
    sensorMpu9250SelectAxes();
  }
  else if (mpuConfig == 0)
  {
     sensorMpuSleep();
  }
}


/*******************************************************************************
* @fn          sensorMpu9250AccSetRange
*
* @brief       Set the range of the accelerometer
*
* @param       newRange: ACC_RANGE_2G, ACC_RANGE_4G, ACC_RANGE_8G, ACC_RANGE_16G
*
* @return      true if write succeeded
*/
bool sensorMpu9250AccSetRange(uint8_t newRange)
{
  bool success;

  if (newRange == accRange)
    return true;

  if (!SENSOR_SELECT())
  {
    return false;
  }

  success = false;

  accRangeReg = (newRange << 3);

  // Apply the range
  success = sensorWriteReg(ACCEL_CONFIG, &accRangeReg, 1);
  SENSOR_DESELECT();

  if (success)
    accRange = newRange;

  return success;
}

/*******************************************************************************
* @fn          sensorMpu9250AccReadRange
*
* @brief       Set the range of the accelerometer
*
* @param       none
*
* @return      ange: ACC_RANGE_2G, ACC_RANGE_4G, ACC_RANGE_8G, ACC_RANGE_16G
*/
uint8_t sensorMpu9250AccReadRange(void)
{
  if (!SENSOR_SELECT())
  {
    return false;
  }

  // Apply the range
  sensorReadReg(ACCEL_CONFIG, &accRangeReg, 1);
  SENSOR_DESELECT();

  accRange = (accRangeReg>>3) & 3;

  return accRange;
}


/*******************************************************************************
* @fn          sensorMpu9250AccRead
*
* @brief       Read data from the accelerometer - X, Y, Z - 3 words
*
* @return      TRUE if valid data, FALSE if not
*/
bool sensorMpu9250AccRead(uint16_t *data )
{
  bool success;

  // Burst read of all accelerometer values
  if (!SENSOR_SELECT())
  {
    return false;
  }

  success = sensorReadReg(ACCEL_XOUT_H, (uint8_t*)data, DATA_SIZE);
  SENSOR_DESELECT();

  if (success)
  {
    convertToLe((uint8_t*)data,DATA_SIZE);
  }
  else
  {
    sensorSetErrorData((uint8_t*)data,DATA_SIZE);
  }

  return success;
}

/*******************************************************************************
* @fn          sensorMpu9250GyroRead
*
* @brief       Read data from the gyroscope - X, Y, Z - 3 words
*
* @return      TRUE if valid data, FALSE if not
*/
bool sensorMpu9250GyroRead(uint16_t *data )
{
  bool success;

  // Select this sensor
  if (!SENSOR_SELECT())
  {
    return false;
  }

  // Burst read of all gyroscope values
  success = sensorReadReg(GYRO_XOUT_H, (uint8_t*)data, DATA_SIZE);

  SENSOR_DESELECT();

  if (success)
  {
    convertToLe((uint8_t*)data,DATA_SIZE);
  }
  else
  {
    sensorSetErrorData((uint8_t*)data,DATA_SIZE);
  }

  return success;
}


/*******************************************************************************
 * @fn          sensorMpu9250Test
 *
 * @brief       Run a sensor self-test
 *
 * @return      TRUE if passed, FALSE if failed
 */
bool sensorMpu9250Test(void)
{
  static bool first = true;

  // Select Gyro/Accelerometer
  if (!SENSOR_SELECT())
  {
    return false;
  }

  // Make sure power is ramped up
  if (first)
  {
    delay_ms(100);
    first = false;
  }

  // Check the WHO AM I register
  ST_ASSERT(sensorReadReg(WHO_AM_I, &val, 1));
  ST_ASSERT(val == 0x71);

  SENSOR_DESELECT();

  return true;
}


/*******************************************************************************
 * @fn          sensorMpu9250AccelConvert
 *
 * @brief       Convert raw data to G units
 *
 * @param       rawData - raw data from sensor
 *
 * @return      Converted value
 ******************************************************************************/
float sensorMpu9250AccelConvert(int16_t rawData)
{
  float v;

  switch (accRange)
  {
  case ACC_RANGE_2G:
    //-- calculate acceleration, unit G, range -2, +2
    v = (rawData * 1.0) / (32768/2);
    break;

  case ACC_RANGE_4G:
    //-- calculate acceleration, unit G, range -4, +4
    v = (rawData * 1.0) / (32768/4);
    break;

  case ACC_RANGE_8G:
    //-- calculate acceleration, unit G, range -8, +8
    v = (rawData * 1.0) / (32768/8);
    break;

  case ACC_RANGE_16G:
    //-- calculate acceleration, unit G, range -16, +16
    v = (rawData * 1.0) / (32768/16);
    break;
  }

  return v;
}

/*******************************************************************************
 * @fn          sensorMpu9250GyroConvert
 *
 * @brief       Convert raw data to deg/sec units
 *
 * @param       data - raw data from sensor
 *
 * @return      none
 ******************************************************************************/
float sensorMpu9250GyroConvert(int16_t data)
{
  //-- calculate rotation, unit deg/s, range -250, +250
  return (data * 1.0) / (65536 / 500);
}



/*******************************************************************************
* @fn          sensorMpuSleep
*
* @brief       Place the MPU in low power mode
*
* @return
*/
static void sensorMpuSleep(void)
{
  bool success;

  if (!SENSOR_SELECT())
  {
    return;
  }

  val = ALL_AXES;
  success = sensorWriteReg(PWR_MGMT_2, &val, 1);
  if (success)
  {
    val = MPU_SLEEP;
    success = sensorWriteReg(PWR_MGMT_1, &val, 1);
  }

  SENSOR_DESELECT();
}


/*******************************************************************************
* @fn          sensorMpu9250WakeUp
*
* @brief       Exit low power mode
*
* @return      none
*/

static void sensorMpu9250WakeUp(void)
{
  bool success;

  if (!SENSOR_SELECT())
  {
    return;
  }

  val = MPU_WAKE_UP;
  success = sensorWriteReg(PWR_MGMT_1, &val, 1);

  if (success)
  {
    // All axis initially disabled
    val = ALL_AXES;
    success = sensorWriteReg(PWR_MGMT_2, &val, 1);
    mpuConfig = 0;
  }

  if (success)
  {
    // Restore the range
    success = sensorWriteReg(ACCEL_CONFIG, &accRangeReg, 1);

    if (success)
    {
      // Clear interrupts
      success = sensorReadReg(INT_STATUS,&val,1);
    }
  }

  SENSOR_DESELECT();
}


/*******************************************************************************
* @fn          sensorMpu9250SelectAxes
*
* @brief       Select gyro, acceleromter, magnetometer
*
* @return      none
*/
static void sensorMpu9250SelectAxes(void)
{
  val = ~mpuConfig;
  sensorWriteReg(PWR_MGMT_2, &val, 1);
#ifdef FEATURE_MAGNETOMETER
  sensorMagEnable(!!(mpuConfig & MPU_AX_MAG));
#endif
}

#ifdef FEATURE_MAGNETOMETER
/*******************************************************************************
* @fn          sensorMpu9250SetBypass
*
* @brief       Allow the I2C bus to control the compass
*
* @return      none
*/
static bool sensorMpu9250SetBypass(void)
{
  SENSOR_SELECT();

  val = BIT_BYPASS_EN;
  sensorWriteReg(INT_PIN_CFG, &val, 1);
  delay_ms(100);

  SENSOR_DESELECT();

  return true;
}


/*******************************************************************************
* @fn          sensorMagInit
*
* @brief       Initialise the compass
*
* @return      none
*/
static void sensorMagInit(void)
{
  uint8_t rawData[3];

  sensorMpu9250SetBypass();

  SENSOR_SELECT_MAG();

  // Enter Fuse ROM access mode
  val = 0x0F;
  sensorWriteReg(MAG_CNTL, &val, 1);
  delay_ms(10);

  // Get calibration data
  sensorReadReg(MAG_ASAX, &rawData[0], 3);  // Read the x-, y-, and z-axis calibration values
  calX =  (float)(rawData[0] - 128)/256.0 + 1.0;   // Return x-axis sensitivity adjustment values, etc.
  calY =  (float)(rawData[1] - 128)/256.0 + 1.0;
  calZ =  (float)(rawData[2] - 128)/256.0 + 1.0;

  // Turn off the sensor again
  val = 0x00;
  sensorWriteReg(MAG_CNTL, &val, 1);
  delay_ms(10);

  SENSOR_DESELECT();
}

/*******************************************************************************
 * @fn          sensorMagEnable
 *
 * @brief       Enable or disable the compass part of the MPU9250
 *
 * @return      none
 */
static void sensorMagEnable(bool enable)
{
  uint8_t val;

  sensorMpu9250SetBypass();

  SENSOR_SELECT_MAG();

  if (enable)
  {
    uint8_t scale = MFS_16BITS; // Choose either 14-bit or 16-bit magnetometer resolution
    uint8_t mode = 0x02;        // 2 for 8 Hz, 6 for 100 Hz continuous magnetometer data read
    // Configure the magnetometer for continuous read and highest resolution
    // set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL register,
    // and enable continuous mode data acquisition Mmode (bits [3:0]), 0010 for 8 Hz and 0110 for 100 Hz sample rates
    val = (scale << 4) | mode;
    sensorWriteReg(MAG_CNTL, &val, 1); // Set magnetometer data resolution and sample ODR
  }
  else
  {
    val = 0x00;
    sensorWriteReg(MAG_CNTL, &val, 1); // Power down magnetometer
  }
  delay_ms(10);

  SENSOR_DESELECT();
}

/*******************************************************************************
 * @fn          sensorMpu9250MagTest
 *
 * @brief       Run a magnetometer self test
 *
 * @return      TRUE if passed, FALSE if failed
 */
bool sensorMpu9250MagTest(void)
{
  // Connect magnetometer internally in MPU9250
  sensorMpu9250SetBypass();

  // Select magnetometer
  SENSOR_SELECT_MAG();

  // Check the WHO AM I register
  val = 0xFF;
  ST_ASSERT(sensorReadReg(MAG_WHO_AM_I, &val, 1));
  ST_ASSERT(val == MAG_DEVICE_ID);

  SENSOR_DESELECT();

  return true;
}

/*******************************************************************************
* @fn          sensorMpu9250MagRead
*
* @brief       Read data from the compass - X, Y, Z - 3 words
*
* @return      TRUE if valid data, FALSE if not
*/
bool sensorMpu9250MagRead(int16_t *data)
{
  bool success;
  uint8_t val;
  uint8_t rawData[7];  // x/y/z compass register data, ST2 register stored here,
                       // must read ST2 at end of data acquisition

  // Connect magnetometer internally in MPU9250
  SENSOR_SELECT();
  val = BIT_BYPASS_EN;
  sensorWriteReg(INT_PIN_CFG, &val, 1);
  SENSOR_DESELECT();

  // Select this sensor
  SENSOR_SELECT_MAG();

  success = sensorReadReg(MAG_ST1,&val,1);
  if (success)
  {
    // Check magnetometer data ready bit
    if (val & 0x01)
    {
      // Burst read of all compass  values + ST2 register
      sensorReadReg(MAG_XOUT_L, &rawData[0],7);
      val = rawData[6]; // ST2 register

      // Check if magnetic sensor overflow set, if not then report data
      if(!(val & 0x08))
      {
        data[0] = ((int16_t)rawData[1] << 8) | rawData[0];  // Turn the MSB and LSB into a signed 16-bit value
        data[1] = ((int16_t)rawData[3] << 8) | rawData[2];  // Data stored as little Endian
        data[2] = ((int16_t)rawData[5] << 8) | rawData[4];
      }
      else
      {
        success = false;
      }
    }
  }

  if (!success)
  {
    sensorSetErrorData((uint8_t*)data,DATA_SIZE);
  }

  SENSOR_DESELECT();

  return success;
}

/*******************************************************************************
* @fn          sensorMpu9250MagConvert
*
* @brief       Convert raw magnetometer data to signed integers (unit uT)
*
* @return      none
*/
void sensorMpu9250MagConvert(int16_t *pRawData, SensorMagData_t *pMagData)
{
  pMagData->x = (int16_t)(pRawData[0] * calX);
  pMagData->y = (int16_t)(pRawData[1] * calY);
  pMagData->z = (int16_t)(pRawData[2] * calZ);
}


#endif

//
// Temporary test function to read all registers
//
uint8_t sensorMpu9250ReadRegs(uint8_t *data, uint8_t start, uint8_t nRegs)
{
  uint8_t reg;
  SENSOR_SELECT();

  for (reg = start; reg < nRegs; reg++)
  {
    if (sensorReadReg(reg, data+reg, 1) == FALSE)
      break;
  }
  SENSOR_DESELECT();

  return reg;
}

/*******************************************************************************
*******************************************************************************/
