#include "pwm_motor_driver.hpp"
#include "../../../motors/motors.hpp"
#include "../../uc_hw/gpio/gpio.h"
#include "../../uc_hw/tim/tim.h"


/* Motor objects */
static pwm_motor left_motor(PORT_B, 10, PORT_B, 11, PORT_B, 12, 1, PWM_PLUS_DIR);
static pwm_motor right_motor(PORT_B, 13, PORT_B, 14, PORT_B, 15, 2, PWM_PLUS_DIR); //move timer gpio to timer module, init all there, choose between motors by channels


pwm_motor::pwm_motor(enum gpio_port port_en, uint8_t pin_en, enum gpio_port port_mode, uint8_t pin_mode, \
                     enum gpio_port port_dir, uint8_t pin_dir, uint8_t ch, enum motor_driver_mode mode)
{
    gpio_port_driver_enable = port_en;
    gpio_pin_driver_enable = pin_en;
    gpio_port_mode = port_mode;
    gpio_pin_mode = pin_mode;
    gpio_port_dir = port_dir;
    gpio_pin_dir = pin_dir;
    pwm_ch = ch;

    /* GPIO config */
}

void pwm_motor::set_speed(speed_t value)
{
    /* For now value = pwm duty, later when encoders will be implemented value will be calculated to pwm duty */
    speed = value;
    //if dir = stop activate tim first then pwm duty
    
}

void pwm_motor::set_direction(enum direction value)
{
    dir = value;
}

void pwm_motor::stop()
{
    speed = 0;
    dir = STOP;
}





//timer
//tim 3 right ch2
//left ch1
//gpio timer af
//right - pb5 af1
//left - pb4 af1





