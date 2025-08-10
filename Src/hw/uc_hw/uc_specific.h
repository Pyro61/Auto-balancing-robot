#ifndef _UC_SPECIFIC_
#define _UC_SPECIFIC_

#include "stm32c0xx.h"

/* HSE frequency */
#define HSE_FREQ              48000000ULL
/* Core clock frequency */
#define MCU_CLOCK_FREQ        48000000ULL
/* AHB bus frequency */
#define AHB_CLOCK_FREQ        (MCU_CLOCK_FREQ)
/* APB bus frequency */
#define APB_CLOCK_FREQ        (MCU_CLOCK_FREQ)

/* SysTick frequency */
#define SYS_TICK_FREQ          (AHB_CLOCK_FREQ / 8)


#endif /* _UC_SPECIFIC_ */
