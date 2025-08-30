#ifndef _POSITION_SENSOR_
#define _POSITION_SENSOR_

#include <cstdint>

typedef uint32_t ms_t;


class position_sensor
{
    protected:
    float roll;
    float pitch;
    float yaw;
    ms_t sample_time;

    public:
    position_sensor();
    virtual void init(ms_t sample_timestamp);
    virtual void update_data();
    float get_roll();
    float get_pitch();
    float get_yaw();
};

#endif /* _POSITION_SENSOR_ */
