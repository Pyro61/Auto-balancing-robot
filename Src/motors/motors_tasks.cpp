#include "motors_tasks.hpp"
#include "rtos_wrapper.hpp"
#include "motors.hpp"

extern queue_t motor_steering_data_queue = nullptr;
extern sem_t motor_stop_signal_sem = nullptr;


/* Motor selection is made in task creation by passing correct motor object */
void steer_motor(void *params)
{
    /* Task data */
    motor& mot = *static_cast<motor*>(params);
    struct motor_steering_data data;

    /* Create queues and semaphores if not created yet */
    if (motor_steering_data_queue == nullptr) motor_steering_data_queue = rtos_queue_create((2 * MOTOR_QUANTITY), sizeof(struct motor_steering_data));
    if (motor_stop_signal_sem == nullptr) motor_stop_signal_sem = rtos_sem_counting_create(MOTOR_QUANTITY, 0);
    
    /* Initialise the motor */ 
    mot.init();

    while(1)
    {

        /* Stop signal received, stop the motor */
        if (rtos_sem_take(motor_stop_signal_sem, RTOS_MAX_DELAY) == 1)
        {
            mot.stop();
        }

        /* Steering data received, get data to the motor */
        if (rtos_queue_receive(motor_steering_data_queue, &data, RTOS_MAX_DELAY) == 1)
        {
            mot.set_direction(data.dir);
            mot.set_speed(data.speed);
        }
    }
}
