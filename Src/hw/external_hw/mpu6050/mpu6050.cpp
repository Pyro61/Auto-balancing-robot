#include "mpu6050.hpp"
#include "../../uc_hw/i2c/i2c.h"
#include "../../uc_hw/i2c/i2c_tasks.hpp"
#include <cmath>

struct i2c_queue_data i2c_data;


mpu6050::mpu6050()
{
    is_xyz_calculated_to_rpy_flag = true; /* No data so far, so calculating is not useful */
}

void mpu6050::init(ms_t sample_timestamp)
{
    sample_time = sample_timestamp;

    /* Temporarily directly calling i2c fucntion instead of rtos task */

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

void mpu6050::update_data(void* cb)
{
    i2c_data.op = I2C_OP_RECEIVE_DMA;
    i2c_data.data_buf = read_buffer;
    i2c_data.data_size = DATA_SIZE;
    i2c_data.slave_addr = MPU6050_ADDR;
    i2c_data.slave_reg = 0x3B;
    i2c_data.sem_to_give = static_cast<sem_t>(cb);

    if (rtos_queue_send(i2c1_queue, &i2c_data, sample_time) == 1)
    {
        is_xyz_calculated_to_rpy_flag = false;
    }
}

void mpu6050::calculate_xyz_to_rpy()
{
    /* Calculate raw to xyz */
    m_per_s2 acc_x = (float)(read_buffer[0] << 8 | read_buffer[1]) / ACCELEROMETER_VALUE_SCALER;    //add offset substraction, measure how much it is
    m_per_s2 acc_y = (float)(read_buffer[2] << 8 | read_buffer[3]) / ACCELEROMETER_VALUE_SCALER;
    m_per_s2 acc_z = (float)(read_buffer[4] << 8 | read_buffer[5]) / ACCELEROMETER_VALUE_SCALER;
    rad_per_s gyro_x = (float)((read_buffer[8] << 8 | read_buffer[9]) / GYROSCOPE_VALUE_SCALER) * DEG_TO_RAD;   /* Need to trasform from deg/s to rad/s */
    rad_per_s gyro_y = (float)((read_buffer[10] << 8 | read_buffer[11]) / GYROSCOPE_VALUE_SCALER) * DEG_TO_RAD; /* Need to trasform from deg/s to rad/s */
    rad_per_s gyro_z = (float)((read_buffer[12] << 8 | read_buffer[13]) / GYROSCOPE_VALUE_SCALER) * DEG_TO_RAD; /* Need to trasform from deg/s to rad/s */

    /* Calculate xyz to rpy */
    /* Accelerometer */
    rad roll_acc = atan2f(acc_y, acc_z);
    rad pitch_acc = atan2f(-acc_x, sqrtf(acc_y * acc_y + acc_z * acc_z));

    /* Gyroscope */
    rad_per_s roll_gyro_now = gyro_x + tanf(pitch) * sinf(roll) * gyro_y   + cosf(roll) * tanf(pitch) * gyro_z;
    rad_per_s pitch_gyro_now =         cosf(roll) * gyro_y                 -  sinf(roll) * gyro_z;
    rad_per_s yaw_gyro_now =           (sinf(roll) / cosf(pitch)) * gyro_y + (cosf(roll) / cosf(pitch)) * gyro_z;
    
    roll_gyro += roll_gyro_now * MS_TO_S(sample_time);
    pitch_gyro += pitch_gyro_now * MS_TO_S(sample_time);
    yaw_gyro += yaw_gyro_now * MS_TO_S(sample_time);

    /* Combine both sensor readings with complementary filter */
    roll = complementary_filter(COMPLEMENTARY_FILTER_COEFFICIENT, roll_acc, roll_gyro, roll);
    pitch = complementary_filter(COMPLEMENTARY_FILTER_COEFFICIENT, pitch_acc, pitch_gyro, pitch);
    yaw = yaw_gyro; /* Not accurate - gyro drift (temporary solution) */

    is_xyz_calculated_to_rpy_flag = true;
}

float mpu6050::complementary_filter(float alpha, m_per_s2 acc_val, rad_per_s gyro_val, rad last_val)
{
    return alpha * acc_val + (1.0f - alpha) * (last_val + gyro_val);
}

float mpu6050::get_roll()
{
    if (is_xyz_calculated_to_rpy_flag == false)
    {
        mpu6050::calculate_xyz_to_rpy();
    }
    return roll;
}

float mpu6050::get_pitch()
{
    if (is_xyz_calculated_to_rpy_flag == false)
    {
        mpu6050::calculate_xyz_to_rpy();
    }
    return pitch;
}

float mpu6050::get_yaw()
{
    if (is_xyz_calculated_to_rpy_flag == false)
    {
        mpu6050::calculate_xyz_to_rpy();
    }
    return yaw;
}

