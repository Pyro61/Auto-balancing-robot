#ifndef _MOTORS_IMPLEMENTATION_
#define _MOTORS_IMPLEMENTATION_

#include "motors.hpp"

/* Motor IDs */
enum motor_id
{
    MOTOR_INVALID = -1,
    MOTOR_LEFT = 0,
    MOTOR_RIGHT = 1,
    MOTOR_QUANTITY = 2
};

motor& get_motor_object(enum motor_id id);

#endif /* _MOTORS_IMPLEMENTATION_ */
