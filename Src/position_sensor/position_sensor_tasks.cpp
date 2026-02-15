#include "position_sensor_tasks.hpp"
#include "position_sensor.hpp"

#define SENSOR_READ_SAMPLE_TIME_MS     5

static sem_t pos_calc_finished_sem;

void calc_curr_pos(void *params)
{
    /* Initialisation */
    position_sensor& sensor = *static_cast<position_sensor*>(params);
    sensor.init(SENSOR_READ_SAMPLE_TIME_MS);
    rpy_data data = {0};
    pos_data_queue = rtos_queue_create(3, sizeof(struct rpy_data));
    pos_calc_sem = rtos_sem_bin_create();
    pos_calc_finished_sem = rtos_sem_bin_create();

    /* Data calculations */
    while(1)
    {
        if (rtos_sem_take(pos_calc_sem, RTOS_MAX_DELAY) == 1)
        {
            /* Sensor data update request */
            sensor.update_data(pos_calc_finished_sem);

            /* Data updated, get rpy values */
            if (rtos_sem_take(pos_calc_finished_sem, RTOS_MAX_DELAY) == 1)
            {
                data.roll = sensor.get_roll();
                data.pitch = sensor.get_pitch();
                data.yaw = sensor.get_yaw();
                
                /* Send current position to maintaining balance task */
                rtos_queue_send(pos_data_queue, &data, RTOS_MAX_DELAY);
            }
        }
    }

    /* Task should never reach that point */
}