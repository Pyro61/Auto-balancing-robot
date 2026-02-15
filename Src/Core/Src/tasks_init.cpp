#include "stm32c0xx.h"

#include "../../rtos_wrapper/rtos_wrapper.hpp"

#include "../../hw/uc_hw/core_init/core_init.h"
#include "../../hw/external_hw/mpu6050/mpu6050.hpp"

#include "../../hw/uc_hw/i2c/i2c_tasks.hpp"
#include "../../control/control.hpp"
#include "../../motors/motors_tasks.hpp"
#include "../../motors/motors_implementation.hpp"
#include "../../pid/pid_tasks.hpp"
#include "../../position_sensor/position_sensor_tasks.hpp"
#include "../../position_sensor/position_sensor_implementation.hpp"

extern "C" {
void tasks_init(void)
{
    rtos_task_create(maintain_balance, "Maintain balance", RTOS_TASK_STACKSIZE_MIN, nullptr, 2);
    rtos_task_create(i2c1_op_handler, "I2C task manager", RTOS_TASK_STACKSIZE_MIN, nullptr, 4);
    rtos_task_create(steer_motor, "Left motor task", RTOS_TASK_STACKSIZE_MIN, static_cast<void*>(&get_motor_object(MOTOR_LEFT)), 3);
    rtos_task_create(steer_motor, "Right motor task", RTOS_TASK_STACKSIZE_MIN, static_cast<void*>(&get_motor_object(MOTOR_RIGHT)), 3);
    rtos_task_create(pitch_pid_calculation, "Pitch PID calculation task", RTOS_TASK_STACKSIZE_MIN, nullptr, 1);
    rtos_task_create(calc_curr_pos, "Position sensor task", RTOS_TASK_STACKSIZE_MIN, static_cast<void*>(&get_position_sensor_object()), 2);
}


}




//mpu6050, docelowo osobny moduł gyro, klasa gyro, podklasa mpu6050, tak samo jak motor i pwm_motor, w balanse_control(main task) odczytanie danych z nadklasy gyro
//do tego task i2c co bedzie zarzadzał użyciem, zrobić structa z potrzebnymi danymi i czy read/write, komunikacja z taskami za pomocą queue

//może osobny task do czytania danych z czujnika, i tam wywoływanie position sensor, wysyłka danych do main task za pomocą queue, rozpoczęcie czytania danych semafor binarny
