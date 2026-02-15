#ifndef _POSITION_SENSOR_TASKS_
#define _POSITION_SENSOR_TASKS_

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "../rtos_wrapper/rtos_wrapper.hpp"
#include "position_sensor.hpp"

struct rpy_data
{
    rad roll;
    rad pitch;
    rad yaw;
};

extern queue_t pos_data_queue;
extern sem_t pos_calc_sem;

/* Task function */
void calc_curr_pos(void *params);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _POSITION_SENSOR_TASKS_ */
