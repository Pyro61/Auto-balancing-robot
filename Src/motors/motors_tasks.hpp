#ifndef _MOTORS_TASKS_
#define _MOTORS_TASKS_

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "rtos_wrapper.hpp"
#include "motors.hpp"

#define MOTOR_QUANTITY      2

extern queue_t motor_steering_data_queue;
extern sem_t motor_stop_signal_sem;

struct motor_steering_data
{
    enum direction dir;
    speed_t speed;
};


/* Tasks functions */
void steer_motor(void *params);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* _MOTORS_TASKS_ */
