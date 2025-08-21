#include "motors_implementation.hpp"


/* Implementation */
/* PWM steering */
#include "../hw/external_hw/pwm_motor_driver/pwm_motor_driver.hpp"

/* Motor objects */
static pwm_motor left_motor(PORT_B, 10, PORT_B, 11, PORT_B, 12, 1, PWM_PLUS_DIR);
static pwm_motor right_motor(PORT_B, 13, PORT_B, 14, PORT_B, 15, 2, PWM_PLUS_DIR);

static motor* const motor_list[MOTOR_QUANTITY] = {&left_motor, &right_motor};


/* Return motor object */
motor& get_motor_object(enum motor_id id)
{
    /* Check id */
    if ((id >= MOTOR_QUANTITY) || id <= MOTOR_INVALID)
    {
        while (1);
    }

    return *motor_list[id];
}
