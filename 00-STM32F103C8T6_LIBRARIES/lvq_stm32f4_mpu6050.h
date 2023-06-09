/**
  ******************************************************************************

  MPU6050 For SLVQ32F446RE
  Author:   LVQ
  Updated:  23st June 2022

  ******************************************************************************
*/

#ifndef __MPU6050_H
#define __MPU6050_H 

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup LVQ_SLVQ32F4xx_Libraries
 * @{
 */
 
/*
@verbatim
MPU6050     SLVQ32F4xx   Descrption

SCL         PA8         Clock line for I2C
SDA         PC9         Data line for I2C
VCC         3.3V
GND         GND
AD0         -           If pin is low, I2C address is 0xD0, if pin is high, the address is 0xD2

 */

#include "lvq_stm32f4_rccConfig.h"
#include "lvq_stm32f4_delay.h"
#include "lvq_stm32f4_i2c.h"
#include "math.h"

/**
 * @defgroup LVQ_LIB_Macros
 * @brief    Library defines
 * @{
 */

/* Default I2C used */
#ifndef MPU6050_I2C
#define MPU6050_I2C                 I2C3
#define MPU6050_I2C_PINSPACK        LVQ_I2C_PinsPack_1
#endif

/* Default I2C clock */
#ifndef MPU6050_I2C_CLOCK
#define MPU6050_I2C_CLOCK           400000
#endif

/* Default I2C address */
#define MPU6050_I2C_ADDR            0xD0

/* Who I am register value */
#define MPU6050_I_AM                0x68

/* MPU6050 registers */
#define MPU6050_AUX_VDDIO           0x01
#define MPU6050_SMPLRT_DIV          0x19
#define MPU6050_CONFIG              0x1A
#define MPU6050_GYRO_CONFIG         0x1B
#define MPU6050_ACCEL_CONFIG        0x1C
#define MPU6050_MOTION_THRESH       0x1F
#define MPU6050_INT_PIN_CFG         0x37
#define MPU6050_INT_ENABLE          0x38
#define MPU6050_INT_STATUS          0x3A
#define MPU6050_ACCEL_XOUT_H        0x3B
#define MPU6050_ACCEL_XOUT_L        0x3C
#define MPU6050_ACCEL_YOUT_H        0x3D
#define MPU6050_ACCEL_YOUT_L        0x3E
#define MPU6050_ACCEL_ZOUT_H        0x3F
#define MPU6050_ACCEL_ZOUT_L        0x40
#define MPU6050_TEMP_OUT_H          0x41
#define MPU6050_TEMP_OUT_L          0x42
#define MPU6050_GYRO_XOUT_H         0x43
#define MPU6050_GYRO_XOUT_L         0x44
#define MPU6050_GYRO_YOUT_H         0x45
#define MPU6050_GYRO_YOUT_L         0x46
#define MPU6050_GYRO_ZOUT_H         0x47
#define MPU6050_GYRO_ZOUT_L         0x48
#define MPU6050_MOT_DETECT_STATUS   0x61
#define MPU6050_SIGNAL_PATH_RESET   0x68
#define MPU6050_MOT_DETECT_CTRL     0x69
#define MPU6050_USER_CTRL           0x6A
#define MPU6050_PWR_MGMT_1          0x6B
#define MPU6050_PWR_MGMT_2          0x6C
#define MPU6050_FIFO_COUNTH         0x72
#define MPU6050_FIFO_COUNTL         0x73
#define MPU6050_FIFO_R_W            0x74
#define MPU6050_WHO_AM_I            0x75

/* Gyro sensitivities in �/s */
#define MPU6050_GYRO_SENS_250       ((float) 131)
#define MPU6050_GYRO_SENS_500       ((float) 65.5)
#define MPU6050_GYRO_SENS_1000      ((float) 32.8)
#define MPU6050_GYRO_SENS_2000      ((float) 16.4)

/* Acce sensitivities in g */
#define MPU6050_ACCE_SENS_2         ((float) 16384)
#define MPU6050_ACCE_SENS_4         ((float) 8192)
#define MPU6050_ACCE_SENS_8         ((float) 4096)
#define MPU6050_ACCE_SENS_16        ((float) 2048)

/**
 * @}
 */

/**
 * @defgroup LVQ_MPU6050_Typedefs
 * @brief    Library Typedefs
 * @{
 */

/**
 * @brief  MPU6050 can have 2 different slave addresses, depends on it's input AD0 pin
 *         This feature allows you to use 2 different sensors with this library at the same time
 */
typedef enum {
    LVQ_MPU6050_Device_0 = 0,   /*!< AD0 pin is set to low */
    LVQ_MPU6050_Device_1 = 0x02 /*!< AD0 pin is set to high */
} LVQ_MPU6050_Device_t;

/**
 * @brief  MPU6050 result enumeration
 */
typedef enum {
    LVQ_MPU6050_Result_Ok = 0x00,          /*!< Everything OK */
    LVQ_MPU6050_Result_DeviceNotConnected, /*!< There is no device with valid slave address */
    LVQ_MPU6050_Result_DeviceInvalid       /*!< Connected device with address is not MPU6050 */
} LVQ_MPU6050_Result_t;

/**
 * @brief  Parameters for accelerometer range
 */
typedef enum {
    LVQ_MPU6050_Accelerometer_2G = 0x00, /*!< Range is +- 2G */
    LVQ_MPU6050_Accelerometer_4G = 0x01, /*!< Range is +- 4G */
    LVQ_MPU6050_Accelerometer_8G = 0x02, /*!< Range is +- 8G */
    LVQ_MPU6050_Accelerometer_16G = 0x03 /*!< Range is +- 16G */
} LVQ_MPU6050_Accelerometer_t;

/**
 * @brief  Parameters for gyroscope range
 */
typedef enum {
    LVQ_MPU6050_Gyroscope_250s = 0x00,  /*!< Range is +- 250 degrees/s */
    LVQ_MPU6050_Gyroscope_500s = 0x01,  /*!< Range is +- 500 degrees/s */
    LVQ_MPU6050_Gyroscope_1000s = 0x02, /*!< Range is +- 1000 degrees/s */
    LVQ_MPU6050_Gyroscope_2000s = 0x03  /*!< Range is +- 2000 degrees/s */
} LVQ_MPU6050_Gyroscope_t;

/**
 * @brief  Main MPU6050 structure
 */
typedef struct {
    /* Private */
    uint8_t Address;         /*!< I2C address of device. Only for private use */
    float Gyro_Mult;         /*!< Gyroscope corrector from raw data to "degrees/s". Only for private use */
    float Acce_Mult;         /*!< Accelerometer corrector from raw data to "g". Only for private use */
    /* Public */
    int16_t Accelerometer_X; /*!< Accelerometer value X axis */
    int16_t Accelerometer_Y; /*!< Accelerometer value Y axis */
    int16_t Accelerometer_Z; /*!< Accelerometer value Z axis */
    int16_t Gyroscope_X;     /*!< Gyroscope value X axis */
    int16_t Gyroscope_Y;     /*!< Gyroscope value Y axis */
    int16_t Gyroscope_Z;     /*!< Gyroscope value Z axis */
    float Temperature;       /*!< Temperature in degrees */
	
		float Angle_X;
		float Angle_Y;
		float Angle_Z;
} LVQ_MPU6050_t;

/**
 * @}
 */

/**
 * @defgroup LVQ_MPU6050_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes MPU6050 and I2C peripheral
 * @param  *DataStruct: Pointer to empty @ref LVQ_MPU6050_t structure
 * @param   DeviceNumber: MPU6050 has one pin, AD0 which can be used to set address of device.
 *          This feature allows you to use 2 different sensors on the same board with same library.
 *          If you set AD0 pin to low, then this parameter should be LVQ_MPU6050_Device_0,
 *          but if AD0 pin is high, then you should use LVQ_MPU6050_Device_1
 *
 *          Parameter can be a value of @ref LVQ_MPU6050_Device_t enumeration
 * @param  AccelerometerSensitivity: Set accelerometer sensitivity. This parameter can be a value of @ref LVQ_MPU6050_Accelerometer_t enumeration
 * @param  GyroscopeSensitivity: Set gyroscope sensitivity. This parameter can be a value of @ref LVQ_MPU6050_Gyroscope_t enumeration
 * @retval Status:
 *            - LVQ_MPU6050_Result_t: Everything OK
 *            - Other member: in other cases
 */
LVQ_MPU6050_Result_t LVQ_MPU6050_Init(LVQ_MPU6050_t* DataStruct, LVQ_MPU6050_Device_t DeviceNumber, LVQ_MPU6050_Accelerometer_t AccelerometerSensitivity, LVQ_MPU6050_Gyroscope_t GyroscopeSensitivity);

/**
 * @brief  Reads accelerometer data from sensor
 * @param  *DataStruct: Pointer to @ref LVQ_MPU6050_t structure to store data to
 * @retval Member of @ref LVQ_MPU6050_Result_t:
 *            - LVQ_MPU6050_Result_Ok: everything is OK
 *            - Other: in other cases
 */
LVQ_MPU6050_Result_t LVQ_MPU6050_ReadAccelerometer(LVQ_MPU6050_t* DataStruct);

/**
 * @brief  Reads gyroscope data from sensor
 * @param  *DataStruct: Pointer to @ref LVQ_MPU6050_t structure to store data to
 * @retval Member of @ref LVQ_MPU6050_Result_t:
 *            - LVQ_MPU6050_Result_Ok: everything is OK
 *            - Other: in other cases
 */
LVQ_MPU6050_Result_t LVQ_MPU6050_ReadGyroscope(LVQ_MPU6050_t* DataStruct);

/**
 * @brief  Reads temperature data from sensor
 * @param  *DataStruct: Pointer to @ref LVQ_MPU6050_t structure to store data to
 * @retval Member of @ref LVQ_MPU6050_Result_t:
 *            - LVQ_MPU6050_Result_Ok: everything is OK
 *            - Other: in other cases
 */
LVQ_MPU6050_Result_t LVQ_MPU6050_ReadTemperature(LVQ_MPU6050_t* DataStruct);

/**
 * @brief  Reads accelerometer, gyroscope and temperature data from sensor
 * @param  *DataStruct: Pointer to @ref LVQ_MPU6050_t structure to store data to
 * @retval Member of @ref LVQ_MPU6050_Result_t:
 *            - LVQ_MPU6050_Result_Ok: everything is OK
 *            - Other: in other cases
 */
LVQ_MPU6050_Result_t LVQ_MPU6050_ReadAll(LVQ_MPU6050_t* DataStruct);

/**
 * @brief  Reads accelerometer, gyroscope and temperature data from sensor convert angle
 * @param  *DataStruct: Pointer to @ref LVQ_MPU6050_t structure to store data to
 * @retval Member of @ref LVQ_MPU6050_Result_t:
 *            - LVQ_MPU6050_Result_Ok: everything is OK
 *            - Other: in other cases
 */
LVQ_MPU6050_Result_t LVQ_MPU6050_ReadAngles(LVQ_MPU6050_t* DataStruct);

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/* C++ detection */
#ifdef __cplusplus
}
#endif

#endif
