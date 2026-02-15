#include "position_sensor_implementation.hpp"


/* Implementation */
/* MPU6050 */
#include "../hw/external_hw/mpu6050/mpu6050.hpp"

/* Position sensor object */
static mpu6050 pos_sensor;


/* Return motor object */
position_sensor& get_position_sensor_object()
{
    return pos_sensor;
}
