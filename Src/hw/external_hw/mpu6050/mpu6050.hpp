#ifndef _MPU6050_
#define _MPU6050_

#include "../../../position_sensor/position_sensor.hpp"

class mpu6050 : public position_sensor
{
    private:
    /* Accelerometer data */
    float acc_x;
    float acc_y;
    float acc_z;

    /* Gyroscope data */
    float gyro_x;
    float gyro_y;
    float gyro_z;
    
    public:
    mpu6050();
    void init(ms_t sample_timestamp) override;
    void update_data() override;
    void calculate_xyz_to_rpy();
};

#endif /* _MPU6050_ */
