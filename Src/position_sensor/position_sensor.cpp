#include "position_sensor.hpp"


position_sensor::position_sensor()
{
    roll = 0.0f;
    pitch = 0.0f;
    yaw = 0.0f;
}

void position_sensor::init(ms_t sample_timestamp)
{
    sample_time = sample_timestamp;
}

void position_sensor::update_data()
{
    /* Do nothing */
    /* In hw implementation read sensor registers and eventually calculate to rpy */
}

float position_sensor::get_roll()
{
    return roll;
}

float position_sensor::get_pitch()
{
    return pitch;
}

float position_sensor::get_yaw()
{
    return yaw;
}