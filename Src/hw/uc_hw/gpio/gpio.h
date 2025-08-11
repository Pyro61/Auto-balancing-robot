#ifndef _GPIO_
#define _GPIO_


#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"


/* EXTI callback function typedef */
typedef void (*cb_t)(void);

/* Enums with gpio config options */
enum gpio_port
{
    START_PORT = 0,
    PORT_A = 0,
    PORT_B = 1,
    PORT_C = 2,
    PORT_D = 3,
    PORT_F = 5,
    END_PORT = 5
};

enum gpio_mode
{
    START_MODE = 0,
    INPUT = 0,
    OUTPUT = 1,
    ALTERNATE_FUN = 2,
    ANALOG = 3,
    END_MODE = 3
};

enum gpio_pp_od
{
    START_PP_OD = 0,
    PUSH_PULL = 0,
    OPEN_DRAIN = 1,
    END_PP_OD = 1
};

enum gpio_speed
{
    START_SPEED = 0,
    SPEED_LOW = 0,
    SPEED_MEDIUM = 1,
    SPEED_HIGH = 2,
    SPEED_VERY_HIGH = 3,
    END_SPEED = 3
};

enum gpio_pull
{
    START_PULL = 0,
    NO_PULL = 0,
    PULL_UP = 1,
    PULL_DOWN = 2,
    END_PULL = 2
};

enum gpio_state
{
    STATE_ERR = -1,
    LOW = 0,
    HIGH = 1
};

enum gpio_err
{
    ERR = -1,
    NO_ERR = 0
};

enum exti_trigger
{
    START_TRIG = 0,
    RISING_EDGE = 0,
    FALLING_EDGE = 1,
    BOTH_EDGES = 2,
    END_TRIG = 2
};


enum gpio_err gpio_set_af(enum gpio_port port, uint8_t pin, uint8_t af);
enum gpio_err gpio_config(enum gpio_port port, uint8_t pin, uint8_t mode, uint8_t pp_od, uint8_t pull, uint8_t speed);
enum gpio_err gpio_set_exti(enum gpio_port port, uint8_t pin, enum exti_trigger trig, cb_t cb);
enum gpio_err gpio_output_write(enum gpio_port port, uint8_t pin, enum gpio_state state);
enum gpio_state gpio_input_read(enum gpio_port port, uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif /* _GPIO_ */
