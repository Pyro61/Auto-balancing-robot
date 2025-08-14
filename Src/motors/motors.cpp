#include "motors.hpp"


//1 main task for balanse control
//get current angle via imu reading task, calculate motors steering via pid, send pwm duty to motors via queue

//2 separate tasks for left/right motor control
//get class objects via task params

//main motor class, subclass motor_pwm, subclass motor_mock (unit tests), funs steer(speed, dir), get_speed, get_dir
//subclass motor_pwm in hw/external_hw/motor_pwm_driver, in constructor initialise hw

motor::motor()
{
    speed = 0;
    dir = STOP;
}

void motor::set_speed(speed_t value)
{
    speed = value;
}

void motor::set_direction(enum direction value)
{
    dir = value;
}

void motor::stop()
{
    speed = 0;
    dir = STOP;
}