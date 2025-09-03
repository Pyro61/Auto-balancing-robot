#ifndef _MPU6050_
#define _MPU6050_

#include "../../../position_sensor/position_sensor.hpp"

/* Device address */
#define MPU6050_ADDR                        (0x68 << 1) /* 0xD0 */

/* Raw data to xyz scalers */
#define ACCELEROMETER_VALUE_SCALER          16384.0f    /* +/- 2g */
#define GYROSCOPE_VALUE_SCALER              131.0f      /* 250deg/s */

/* MPU6050 data sizes */
#define ACCELEROMETER_DATA_SIZE             6
#define TEMPERATURE_DATA_SIZE               2
#define GYROSCOPE_DATA_SIZE                 6
#define DATA_SIZE                           (ACCELEROMETER_DATA_SIZE + TEMPERATURE_DATA_SIZE + GYROSCOPE_DATA_SIZE)

/* Calculations */
#define RAD_TO_DEG                          (180.0f / 3.14f)
#define MS_TO_S(t)                          ((float)((float)t / 1000.0f))

/* Alpha coefficient */
#define COMPLEMENTARY_FILTER_COEFFICIENT    0.05f

class mpu6050 : public position_sensor
{
    private:
    /* Gyro data accumulated over time (val(n+1) += val(n) * dt) */
    float roll_gyro;
    float pitch_gyro;
    float yaw_gyro;

    /* Data buffer */
    uint8_t read_buffer[DATA_SIZE];

    /* Complementary filter calculation */
    float complementary_filter(float alpha, float acc_val, float gyro_val, float last_val);
    
    public:
    mpu6050();
    void init(ms_t sample_timestamp) override;
    void update_data() override;
    void calculate_xyz_to_rpy();
};

#endif /* _MPU6050_ */
