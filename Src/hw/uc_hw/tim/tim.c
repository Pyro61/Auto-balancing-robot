#include "tim.h"
#include "../gpio/gpio.h"
#include "stm32c0xx.h"
#include "uc_specific.h"
#include <stdlib.h>


/* Calculate seconds to miliseconds */
#define S_TO_MS(ms)		(ms * 1000)


/* TIM3 - PWM */
//right - ch 2
//left - ch 1
//apb1 clock source
/* ------------------------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------ TIM3 - PWM ------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------------ */

#define TIM3_CHANNELS          4
#define TIM3_PWM_CHANNELS   2

/* Output compare value register addresses */
static volatile uint32_t *const TIM3_output_compare_value_reg[TIM3_CHANNELS] = {&TIM3->CCR1, &TIM3->CCR2, &TIM3->CCR3, &TIM3->CCR4};

/* 20kHz oen cycle (on and off) */
void TIM3_init_pwm(void)
{
    /* Timer config */
    RCC->APBENR1 |= RCC_APBENR1_TIM3EN; /* Enable TIM3 peripheral */
    TIM3->PSC = (APB_CLOCK_FREQ / 2000000) - 1; /* 500 ns base time */
    TIM3->ARR = 100 - 1; /* PWM duty 0-100 % */
    TIM3->CCR1 = 0; /* PWM duty channel 1 */ 
    TIM3->CCR2 = 0; /* PWM duty channel 2 */
    TIM3->BDTR |= TIM_BDTR_MOE; /* Enable main output */
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE; /* Enable CH1 and CH2 preload */
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2; /* PWM mode 1 - High state until counter reaches pwm duty */
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E; /* Enable CH1 and CH2 */

    TIM3->EGR = TIM_EGR_UG; /* Update registers */

    /* GPIO config */
    gpio_config(PORT_B, 4, ALTERNATE_FUN, PUSH_PULL, NO_PULL, SPEED_MEDIUM); /* TIM3 CH1 */
    gpio_set_af(PORT_B, 4, 1);
    gpio_config(PORT_B, 5, ALTERNATE_FUN, PUSH_PULL, NO_PULL, SPEED_MEDIUM); /* TIM3 CH2 */
    gpio_set_af(PORT_B, 5, 1);
}

void TIM3_start_counter(void)
{
    TIM3->CNT = 0;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void TIM3_stop_counter(void)
{
    TIM3->CR1 &= ~(TIM_CR1_CEN);
}

void TIM3_update_pwm_duty(uint8_t ch, uint8_t duty)
{
    if (ch < TIM3_PWM_CHANNELS)
    {
        *TIM3_output_compare_value_reg[ch-1] = duty;
    }
}

