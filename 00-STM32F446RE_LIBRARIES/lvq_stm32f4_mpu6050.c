/**
  ******************************************************************************

  MPU6050 For STM32F446RE
  Author:   LVQ
  Updated:  23st June 2022

  ******************************************************************************
*/

#include "lvq_stm32f4_mpu6050.h"

#define PI 3.14
#define RAD_TO_DEG 59.29577951

static uint32_t prevMillis = 0;
static double accX, accY, gyroX, gyroY, gyroZ;
static double gyroXOffset, gyroYOffset, gyroZOffset;

LVQ_MPU6050_Result_t LVQ_MPU6050_Init(LVQ_MPU6050_t* DataStruct, LVQ_MPU6050_Device_t DeviceNumber, LVQ_MPU6050_Accelerometer_t AccelerometerSensitivity, LVQ_MPU6050_Gyroscope_t GyroscopeSensitivity) 
{
    uint8_t temp;

    /* Format I2C address */
    DataStruct->Address = MPU6050_I2C_ADDR | (uint8_t)DeviceNumber;

    /* Initialize I2C */
    LVQ_I2C_Init(MPU6050_I2C, MPU6050_I2C_PINSPACK, MPU6050_I2C_CLOCK);

    /* Check if device is connected */
    if (!LVQ_I2C_IsDeviceConnected(MPU6050_I2C, DataStruct->Address)) 
		{
        /* Return error */
        return LVQ_MPU6050_Result_DeviceNotConnected;
    }

    /* Check who I am */
    if (LVQ_I2C_Read(MPU6050_I2C, DataStruct->Address, MPU6050_WHO_AM_I) != MPU6050_I_AM) 
		{
        /* Return error */
        return LVQ_MPU6050_Result_DeviceInvalid;
    }

    /* Wakeup MPU6050 */
    LVQ_I2C_Write(MPU6050_I2C, DataStruct->Address, MPU6050_PWR_MGMT_1, 0x00);
		
		/* 0x07 Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz */
    LVQ_I2C_Write(MPU6050_I2C, DataStruct->Address, MPU6050_SMPLRT_DIV, 0x07);
		
		/* Config MPU6050 */
    LVQ_I2C_Write(MPU6050_I2C, DataStruct->Address, MPU6050_CONFIG, 0x00);

    /* Config accelerometer */
    temp = LVQ_I2C_Read(MPU6050_I2C, DataStruct->Address, MPU6050_ACCEL_CONFIG);
    temp = (temp & 0xE7) | (uint8_t)AccelerometerSensitivity << 3;
    LVQ_I2C_Write(MPU6050_I2C, DataStruct->Address, MPU6050_ACCEL_CONFIG, temp);

    /* Config gyroscope */
    temp = LVQ_I2C_Read(MPU6050_I2C, DataStruct->Address, MPU6050_GYRO_CONFIG);
    temp = (temp & 0xE7) | (uint8_t)GyroscopeSensitivity << 3;
    LVQ_I2C_Write(MPU6050_I2C, DataStruct->Address, MPU6050_GYRO_CONFIG, temp);
		
		/* User control MPU6050 */
    LVQ_I2C_Write(MPU6050_I2C, DataStruct->Address, MPU6050_USER_CTRL, 0x00);
		
		/* Wakeup MPU6050 */
    LVQ_I2C_Write(MPU6050_I2C, DataStruct->Address, MPU6050_PWR_MGMT_1, 0x01);

    /* Set sensitivities for multiplying gyro and accelerometer data */
    switch (AccelerometerSensitivity) 
		{
        case LVQ_MPU6050_Accelerometer_2G:
            DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_2;
            break;
        case LVQ_MPU6050_Accelerometer_4G:
            DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_4;
            break;
        case LVQ_MPU6050_Accelerometer_8G:
            DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_8;
            break;
        case LVQ_MPU6050_Accelerometer_16G:
            DataStruct->Acce_Mult = (float)1 / MPU6050_ACCE_SENS_16;
        default:
            break;
    }

    switch (GyroscopeSensitivity) 
		{
        case LVQ_MPU6050_Gyroscope_250s:
            DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_250;
            break;
        case LVQ_MPU6050_Gyroscope_500s:
            DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_500;
            break;
        case LVQ_MPU6050_Gyroscope_1000s:
            DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_1000;
            break;
        case LVQ_MPU6050_Gyroscope_2000s:
            DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_2000;
        default:
            break;
    }

    /* Return OK */
    return LVQ_MPU6050_Result_Ok;
}

LVQ_MPU6050_Result_t LVQ_MPU6050_ReadAccelerometer(LVQ_MPU6050_t* DataStruct) 
{
    uint8_t data[6];

    /* Read accelerometer data */
    LVQ_I2C_ReadMulti(MPU6050_I2C, DataStruct->Address, MPU6050_ACCEL_XOUT_H, data, 6);

    /* Format */
    DataStruct->Accelerometer_X = (int16_t)(data[0] << 8 | data[1]);
    DataStruct->Accelerometer_Y = (int16_t)(data[2] << 8 | data[3]);
    DataStruct->Accelerometer_Z = (int16_t)(data[4] << 8 | data[5]);

    /* Return OK */
    return LVQ_MPU6050_Result_Ok;
}

LVQ_MPU6050_Result_t LVQ_MPU6050_ReadGyroscope(LVQ_MPU6050_t* DataStruct) 
{
    uint8_t data[6];

    /* Read gyroscope data */
    LVQ_I2C_ReadMulti(MPU6050_I2C, DataStruct->Address, MPU6050_GYRO_XOUT_H, data, 6);

    /* Format */
    DataStruct->Gyroscope_X = (int16_t)(data[0] << 8 | data[1]);
    DataStruct->Gyroscope_Y = (int16_t)(data[2] << 8 | data[3]);
    DataStruct->Gyroscope_Z = (int16_t)(data[4] << 8 | data[5]);

    /* Return OK */
    return LVQ_MPU6050_Result_Ok;
}

LVQ_MPU6050_Result_t LVQ_MPU6050_ReadTemperature(LVQ_MPU6050_t* DataStruct) 
{
    uint8_t data[2];
    int16_t temp;

    /* Read temperature */
    LVQ_I2C_ReadMulti(MPU6050_I2C, DataStruct->Address, MPU6050_TEMP_OUT_H, data, 2);

    /* Format temperature */
    temp = (data[0] << 8 | data[1]);
    DataStruct->Temperature = (float)((int16_t)temp / (float)340.0 + (float)36.53);

    /* Return OK */
    return LVQ_MPU6050_Result_Ok;
}

LVQ_MPU6050_Result_t LVQ_MPU6050_ReadAll(LVQ_MPU6050_t* DataStruct) 
{
    uint8_t data[14];
    int16_t temp;

    /* Read full raw data, 14bytes */
    LVQ_I2C_ReadMulti(MPU6050_I2C, DataStruct->Address, MPU6050_ACCEL_XOUT_H, data, 14);

    /* Format accelerometer data */
    DataStruct->Accelerometer_X = (int16_t)(data[0] << 8 | data[1]);
    DataStruct->Accelerometer_Y = (int16_t)(data[2] << 8 | data[3]);
    DataStruct->Accelerometer_Z = (int16_t)(data[4] << 8 | data[5]);

    /* Format temperature */
    temp = (data[6] << 8 | data[7]);
    DataStruct->Temperature = (float)((float)((int16_t)temp) / (float)340.0 + (float)36.53);

    /* Format gyroscope data */
    DataStruct->Gyroscope_X = (int16_t)(data[8] << 8 | data[9]);
    DataStruct->Gyroscope_Y = (int16_t)(data[10] << 8 | data[11]);
    DataStruct->Gyroscope_Z = (int16_t)(data[12] << 8 | data[13]);

    /* Return OK */
    return LVQ_MPU6050_Result_Ok;
}

LVQ_MPU6050_Result_t LVQ_MPU6050_ReadAngles(LVQ_MPU6050_t* DataStruct) 
{
    uint8_t data[14];
    int16_t temp;

    /* Read full raw data, 14bytes */
    LVQ_I2C_ReadMulti(MPU6050_I2C, DataStruct->Address, MPU6050_ACCEL_XOUT_H, data, 14);

    /* Format accelerometer data */
    DataStruct->Accelerometer_X = (int16_t)(data[0] << 8 | data[1]);
    DataStruct->Accelerometer_Y = (int16_t)(data[2] << 8 | data[3]);
    DataStruct->Accelerometer_Z = (int16_t)(data[4] << 8 | data[5]);

    /* Format temperature */
    temp = (data[6] << 8 | data[7]);
    DataStruct->Temperature = (float)((float)((int16_t)temp) / (float)340.0 + (float)36.53);

    /* Format gyroscope data */
    DataStruct->Gyroscope_X = (int16_t)(data[8] << 8 | data[9]);
    DataStruct->Gyroscope_Y = (int16_t)(data[10] << 8 | data[11]);
    DataStruct->Gyroscope_Z = (int16_t)(data[12] << 8 | data[13]);
	
		accX = atan((DataStruct->Accelerometer_Y * DataStruct->Acce_Mult) / sqrt(pow((DataStruct->Accelerometer_X * DataStruct->Acce_Mult), 2) + pow((DataStruct->Accelerometer_Z * DataStruct->Acce_Mult), 2))) * RAD_TO_DEG;
    accY = atan(-1 * (DataStruct->Accelerometer_X * DataStruct->Acce_Mult) / sqrt(pow((DataStruct->Accelerometer_Y * DataStruct->Acce_Mult), 2) + pow((DataStruct->Accelerometer_Z * DataStruct->Acce_Mult), 2))) * RAD_TO_DEG;

		gyroX = (DataStruct->Gyroscope_X + gyroXOffset) * DataStruct->Gyro_Mult;
    gyroY = (DataStruct->Gyroscope_Y + gyroYOffset) * DataStruct->Gyro_Mult;
    gyroZ = (DataStruct->Gyroscope_Z + gyroZOffset) * DataStruct->Gyro_Mult;
		
		uint32_t curMillis = Timer_read_ms();
    double duration = (curMillis - prevMillis) * 1e-3;
    prevMillis = curMillis;
		
		DataStruct->Angle_X = 0.98 * (DataStruct->Angle_X + gyroX * duration) + 0.02 * accX;
    DataStruct->Angle_Y = 0.98 * (DataStruct->Angle_Y + gyroY * duration) + 0.02 * accY;
    DataStruct->Angle_Z = DataStruct->Angle_Z + gyroZ * duration;
		
    /* Return OK */
    return LVQ_MPU6050_Result_Ok;
}
