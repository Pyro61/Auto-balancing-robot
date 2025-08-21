#ifndef _TIM_
#define _TIM_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/* Typedefs */
typedef uint32_t ms_t;


/* ------------------------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------ TIM3 - PWM ------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------------ */

void TIM3_init_pwm(void);
void TIM3_start_counter(void);
void TIM3_stop_counter(void);
void TIM3_update_pwm_duty(uint8_t ch, uint8_t duty);

#ifdef __cplusplus
}
#endif

#endif /* _TIM_ */
