#include "pwm_motor_driver.hpp"
#include "../../../motors/motors.hpp"
#include "../../uc_hw/gpio/gpio.h"
#include "../../uc_hw/tim/tim.h"


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
    /* Log/Debug purposes */
    speed = value;  /* For now value = pwm duty, later when encoders will be implemented value will be calculated to pwm duty */
    
    /* Set pwm only if needed */
    if ((dir != STOP) && (speed > 0))
    {   
        /* HW handling */
        TIM3_update_pwm_duty(pwm_ch, speed);
        TIM3_start_counter();
    }
}

void pwm_motor::set_direction(enum direction value)
{
    /* Log/Debug purposes */
    dir = value;

    /* HW handling */
    if (dir == FORWARD)
    {
       gpio_output_write(gpio_port_dir, gpio_pin_dir, HIGH); 
    }

    else if (dir == BACKWARDS)
    {
        gpio_output_write(gpio_port_dir, gpio_pin_dir, LOW); 
    }
    
}

void pwm_motor::stop()
{
    /* Log/Debug purposes */
    speed = 0;
    dir = STOP;

    /* HW handling */
    TIM3_update_pwm_duty(pwm_ch, speed);
    TIM3_stop_counter();
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





