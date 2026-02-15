#ifndef _MOTORS_
#define _MOTORS_


#include <cstdint>

enum direction
{
    STOP = 0,
    FORWARD = 1,
    BACKWARDS = -1
};

typedef uint32_t speed_t;

class motor
{
    protected:
    speed_t speed;
    enum direction dir;

    public:
    motor();
    virtual void set_speed(speed_t value);
    virtual void set_direction(enum direction value);
    virtual void stop();
    virtual void init();
};


#endif /* _MOTORS_ */

