#include "mpu6050.hpp"
#include "../../uc_hw/i2c/i2c.h"
#include <cmath>

/* Create RTOS task inside class in future */
/* Flow: balance task -> position sensor::update data->semaphore mpu6050 task calc data -> sleep */
/* Scheduler -> mpu6050 task ->  mpu6050::read raw -> sleep  */



#define MPU6050_ADDR                    (0xD0)      /* 0x68 << 1 */
#define ACCELEROMETER_VALUE_SCALER      16384.0f    /* +/- 2g */
#define GYROSCOPE_VALUE_SCALER          131.0f      /* 250deg/s */

#define ACCELEROMETER_DATA_SIZE         6
#define TEMPERATURE_DATA_SIZE           2
#define GYROSCOPE_DATA_SIZE             6

#define DATA_SIZE                       (ACCELEROMETER_DATA_SIZE + TEMPERATURE_DATA_SIZE + GYROSCOPE_DATA_SIZE)

#define DEGREE_CONVERTER                (180.0f / 3.14f)
#define MS_TO_S(t)                      ((float)((float)t / 1000.0f))

/* Data read buffer */
static uint8_t read_buffer[DATA_SIZE]; /* Temporary read temperature too, with dedicated task
                                          readings from gyro and accelerometer  will be seperated */


mpu6050::mpu6050()
{
    /* Do nothing (for now) */
}

void mpu6050::init(ms_t sample_timestamp)
{
    sample_time = sample_timestamp;

    while (!is_i2c1_bus_free());
    uint8_t tmp = 0x08;
    i2c1_write_polling(MPU6050_ADDR, 0x6B, &tmp, 1); /* Exit sleep mode, disable temp sensor */
    while (!is_i2c1_bus_free());
    tmp = 0x04;
    i2c1_write_polling(MPU6050_ADDR, 0x1A, &tmp, 1); /* Set value of low pass filter */
    while (!is_i2c1_bus_free());
    tmp = 0x00;
    i2c1_write_polling(MPU6050_ADDR, 0x1C, &tmp, 1); /* Set accelerometer range: +/- 2g */
    while (!is_i2c1_bus_free()); 
    i2c1_write_polling(MPU6050_ADDR, 0x1B, &tmp, 1); /* Set gyroscope range: 250deg/s */
}

void mpu6050::update_data()
{
    while (!is_i2c1_bus_free()) // bez tej linii działa, z wysypuje się wtf
    i2c1_read_dma(MPU6050_ADDR, 0x3B, read_buffer, DATA_SIZE, nullptr);
}

void mpu6050::calculate_xyz_to_rpy()
{
    /* Calculate raw to xyz */
    acc_x = (float)(read_buffer[0] << 8 | read_buffer[1]) / ACCELEROMETER_VALUE_SCALER * DEGREE_CONVERTER;
    acc_y = (float)(read_buffer[2] << 8 | read_buffer[3]) / ACCELEROMETER_VALUE_SCALER * DEGREE_CONVERTER;
    acc_z = (float)(read_buffer[4] << 8 | read_buffer[5]) / ACCELEROMETER_VALUE_SCALER * DEGREE_CONVERTER;
    gyro_x = (float)(read_buffer[8] << 8 | read_buffer[9]) / GYROSCOPE_VALUE_SCALER * DEGREE_CONVERTER;
    gyro_y = (float)(read_buffer[10] << 8 | read_buffer[11]) / GYROSCOPE_VALUE_SCALER * DEGREE_CONVERTER;
    gyro_z = (float)(read_buffer[12] << 8 | read_buffer[13]) / GYROSCOPE_VALUE_SCALER * DEGREE_CONVERTER;

    /* Calculate xyz to rpy */
    roll = atan2(acc_y, acc_z);
    pitch = atan2(-acc_x, sqrt(acc_y * acc_y + acc_z * acc_z));
    yaw += gyro_z * MS_TO_S(sample_time); /* Gyro drift - temporary solution */
}

