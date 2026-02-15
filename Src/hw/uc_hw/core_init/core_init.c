#include "core_init.h"
#include "stm32c0xx.h"

void core_init(void)
{
    /* Flash access delay */
	FLASH -> ACR |= FLASH_ACR_LATENCY_0;
	while (!(FLASH -> ACR & FLASH_ACR_LATENCY));

	/* Enable HSE clock */
	RCC -> CR |= RCC_CR_HSEON;
	while (!(RCC -> CR & RCC_CR_HSERDY));

	/* Select SYSCLK clock - HSE */
	RCC -> CFGR |= RCC_CFGR_SW_0;

	/* Disable HSI */
	RCC -> CR &= ~RCC_CR_HSION;
}