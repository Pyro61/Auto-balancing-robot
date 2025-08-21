#ifndef _PWM_MOTOR_DRIVER_
#define _PWM_MOTOR_DRIVER_

#include <cstdint>
#include "../../uc_hw/gpio/gpio.h"
#include "../../../motors/motors.hpp"


/* MOTOR STEERING MODES */
enum motor_driver_mode
{
    PWM_PLUS_DIR = 0,
    PWM_PLUS_PWM = 1,
    HALF_BRIDGE  = 2,
};


class pwm_motor : public motor
{
    private:
    enum gpio_port gpio_port_driver_enable;     
    uint8_t gpio_pin_driver_enable;             /* 1 - activate driver and  latch selected mode */
                                                /* 0 - enable low power mode */
                                                
    enum motor_driver_mode selected_mode;
    enum gpio_port gpio_port_mode;              
    uint8_t gpio_pin_mode;                      /* Selected mode */

    enum gpio_port gpio_port_dir;               
    uint8_t gpio_pin_dir;                       /* 1 - forward */
                                                /* 0 - reverse */

    uint8_t pwm_ch;                             /* Timer pwm channel */
    /* uint8_t pwm_tim;   One timer with pwm and with no option to create another pwm timers and pwm_motor objects,
                           maybe in future there will be creating objects on different timers in run-time tim2_setpwmduty(ch, duty) */
    

    public:
    pwm_motor(enum gpio_port port_en, uint8_t pin_en, enum gpio_port port_mode, uint8_t pin_mode, \
              enum gpio_port port_dir, uint8_t pin_dir, uint8_t ch, enum motor_driver_mode mode);
    void set_speed(speed_t value) override;
    void set_direction(enum direction value) override;
    void stop() override;
    void init() override;
};


#endif /* _PWM_MOTOR_DRIVER_ */
