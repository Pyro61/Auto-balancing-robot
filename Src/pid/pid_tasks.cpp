#include "pid_tasks.hpp"
#include "pid.hpp"

#define PID_P_GAIN              1
#define PID_I_GAIN              0
#define PID_D_GAIN              0

#define PID_MAX_OUTPUT          (100 - 1)
#define PID_MIN_OUTPUT          (-PID_MAX_OUTPUT)

#define PID_TAU                 1
#define PID_SAMPLE_TIME_S       0.005f

#define PID_MAX_INTEGRATOR      100
#define PID_MIN_INTEGRATOR      (-PID_MAX_INTEGRATOR)

extern queue_t pid_pitch_calc_queue = nullptr;
extern queue_t pid_pitch_ready_queue = nullptr;



void pitch_pid_calculation(void *params)
{
    (void)params;
    pid pitch_pid;
    struct pid_calc_data data = {0, 0};

    /* RTOS queues init */
    pid_pitch_calc_queue = rtos_queue_create(3, sizeof(struct pid_calc_data));
    pid_pitch_ready_queue = rtos_queue_create(3, sizeof(float));

    /* PID initialisation */
    pitch_pid.pid_reset();
    pitch_pid.pid_set_gains(PID_P_GAIN, PID_I_GAIN, PID_D_GAIN);
    pitch_pid.pid_set_output_limits(PID_MIN_OUTPUT, PID_MAX_OUTPUT);
    pitch_pid.pid_set_integrator_limits(PID_MIN_INTEGRATOR, PID_MAX_INTEGRATOR);
    pitch_pid.pid_set_tau(PID_TAU);
    pitch_pid.pid_set_sample_time(PID_SAMPLE_TIME_S);


    while(1)
    {
        /* Get current angle, calculate motors pwm duty, send pid output to balancing task */
        if (rtos_queue_receive(pid_pitch_calc_queue, &data, RTOS_MAX_DELAY) == 1)
        {
            float pid_output = pitch_pid.pid_calculate(data.setpoint, data.current_value);
            rtos_queue_send(pid_pitch_ready_queue, &pid_output, RTOS_MAX_DELAY);
        }
    }
}