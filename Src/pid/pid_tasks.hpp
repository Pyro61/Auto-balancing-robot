#ifndef _PID_TASKS_
#define _PID_TASKS_


#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "rtos_wrapper.hpp"

extern queue_t pid_pitch_calc_queue;
extern queue_t pid_pitch_ready_queue;

/* Data needed to perform pid calculation */
struct pid_calc_data
{
    float setpoint;
    float current_value;
};

/* Tasks functions */
void pitch_pid_calculation(void *params);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _PID_TASKS_ */
