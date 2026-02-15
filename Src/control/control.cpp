#include "control.hpp"

#include "../position_sensor/position_sensor_tasks.hpp"
#include "../pid/pid_tasks.hpp"
#include "../motors/motors_tasks.hpp"


#define SETPOINT_PITCH_RAD      0.0f

/* Transform pid output data to motor input data */
static void pid_data_to_motor_data(float pid, struct motor_steering_data *motor);

/* Task */
void maintain_balance(void *params)
{
    (void)params;
    rpy_data current_position;
    pid_calc_data pitch_pid_data;
    float pitch_pid_output;
    motor_steering_data motor_steering;

    while (1)
    {
        rtos_delay(5);
        /* Calculate data and wait for it */
        rtos_sem_give(pos_calc_sem);

        if (rtos_queue_receive(pos_data_queue, &current_position, 10) == 1)
        {
            /* Send current pitch angle and pitch setpoint to PID controller */
            pitch_pid_data.current_value = current_position.pitch;
            pitch_pid_data.setpoint = SETPOINT_PITCH_RAD;
            rtos_queue_send(pid_pitch_calc_queue, &pitch_pid_data, RTOS_MAX_DELAY);

            /* Get PID output, set motors duty */
            if (rtos_queue_receive(pid_pitch_ready_queue, &pitch_pid_output, RTOS_MAX_DELAY) == 1)
            {
                /* Calculate motor input data from pid output data */
                pid_data_to_motor_data(pitch_pid_output, &motor_steering);

                /* Stop motors */
                if (motor_steering.dir == STOP)
                {
                    for (uint8_t i = 0; i < MOTOR_QUANTITY; i++)
                    {
                        rtos_sem_give(motor_stop_signal_sem);
                    }
                }

                /* Start (run) motors */
                // Add separate data send to left/right motor
                else
                {
                    for (uint8_t i = 0; i < MOTOR_QUANTITY; i++)
                    {
                        rtos_queue_send(motor_steering_data_queue, &motor_steering, RTOS_MAX_DELAY);
                    }
                }
            }
        }
    }
}



static void pid_data_to_motor_data(float pid, struct motor_steering_data *motor)
{
    /* Run forward */
    if (pid > 0)
    {
        motor -> dir = FORWARD;
        motor -> speed = pid;
    }

    /* Run backwards */
    else if (pid < 0)
    {
        motor -> dir = BACKWARDS;
        motor -> speed = -pid;
    }

    /* Stop */
    else
    {
        motor -> dir = STOP;
        motor -> speed = 0;
    }  
}