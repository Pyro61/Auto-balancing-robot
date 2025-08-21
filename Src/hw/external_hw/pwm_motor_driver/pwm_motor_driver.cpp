#include "pwm_motor_driver.hpp"
#include "../../../motors/motors.hpp"
#include "../../uc_hw/gpio/gpio.h"
#include "../../uc_hw/tim/tim.h"


/* Motor objects */
static pwm_motor left_motor(PORT_B, 10, PORT_B, 11, PORT_B, 12, 1, PWM_PLUS_DIR);
static pwm_motor right_motor(PORT_B, 13, PORT_B, 14, PORT_B, 15, 2, PWM_PLUS_DIR);


pwm_motor::pwm_motor(enum gpio_port port_en, uint8_t pin_en, enum gpio_port port_mode, uint8_t pin_mode, \
                     enum gpio_port port_dir, uint8_t pin_dir, uint8_t ch, enum motor_driver_mode mode)
{
    gpio_port_driver_enable = port_en;
    gpio_pin_driver_enable = pin_en;
    selected_mode = mode;
    gpio_port_mode = port_mode;
    gpio_pin_mode = pin_mode;
    gpio_port_dir = port_dir;
    gpio_pin_dir = pin_dir;
    pwm_ch = ch;
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

void pwm_motor::init()
{
    /* GPIO config */
    switch(selected_mode)
    {
        case PWM_PLUS_DIR:
        {
            gpio_config(gpio_port_mode, gpio_pin_mode, OUTPUT, PUSH_PULL, NO_PULL, SPEED_LOW);
            gpio_output_write(gpio_port_mode, gpio_pin_mode, LOW);

            gpio_config(gpio_port_driver_enable, gpio_pin_driver_enable, OUTPUT, PUSH_PULL, NO_PULL, SPEED_LOW);
            gpio_output_write(gpio_port_driver_enable, gpio_pin_driver_enable, HIGH);
            break;
        }

        case PWM_PLUS_PWM:
        {
            gpio_config(gpio_port_mode, gpio_pin_mode, OUTPUT, PUSH_PULL, NO_PULL, SPEED_LOW);
            gpio_output_write(gpio_port_mode, gpio_pin_mode, HIGH);

            gpio_config(gpio_port_driver_enable, gpio_pin_driver_enable, OUTPUT, PUSH_PULL, NO_PULL, SPEED_LOW);
            gpio_output_write(gpio_port_driver_enable, gpio_pin_driver_enable, HIGH);
            break;
        }

        case HALF_BRIDGE:
        {
            gpio_config(gpio_port_mode, gpio_pin_mode, OUTPUT, OPEN_DRAIN, NO_PULL, SPEED_LOW);
            gpio_output_write(gpio_port_mode, gpio_pin_mode, HIGH); /* Hi-Z state */

            gpio_config(gpio_port_driver_enable, gpio_pin_driver_enable, OUTPUT, PUSH_PULL, NO_PULL, SPEED_LOW);
            gpio_output_write(gpio_port_driver_enable, gpio_pin_driver_enable, HIGH);
            break;
        }

        default: /* Do nothing */
        break;
    }
}





