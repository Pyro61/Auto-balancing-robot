#ifndef _POSITION_SENSOR_
#define _POSITION_SENSOR_

#include <cstdint>

typedef uint32_t ms_t; /* miliseconds */
typedef float rad; /* radians */


class position_sensor
{
    protected:
    rad roll;
    rad pitch;
    rad yaw;
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
