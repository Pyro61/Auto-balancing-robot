#include "i2c.h"
#include "stm32c0xx.h"
#include "../gpio/gpio.h"
#include "uc_specific.h"
#include "../../../safe_state/safe_state.h"
#include <stdlib.h>


/********************************************************************************************/
/*                                           I2C1                                           */
/********************************************************************************************/

static cb_t i2c1_recv_dma_cb;

void i2c1_init(void)
{
	/* GPIO settings
	 * PB8 - SCL, PB9 - SDA */
	if (gpio_config(PORT_B, 8, ALTERNATE_FUN, OPEN_DRAIN, NO_PULL, SPEED_HIGH) == ERR) safe_state();
    if (gpio_set_af(PORT_B, 8, 4) == ERR) safe_state();
    if (gpio_config(PORT_B, 9, ALTERNATE_FUN, OPEN_DRAIN, NO_PULL, SPEED_HIGH) == ERR) safe_state();
    if (gpio_set_af(PORT_B, 9, 4) == ERR) safe_state();

	/* I2C settings
	 * Master mode, timing reg, STOP signal detection IRQ, disable analog filter, enable peripheral and enable own address 1 */
	RCC -> APBENR1 |= RCC_APBENR1_I2C1EN;
	I2C1 -> TIMINGR = 0x20733E55; /* Taken from STM32 CUBE-MX */
	I2C1 -> CR1 |= I2C_CR1_PE | I2C_CR1_STOPIE | I2C_CR1_ANFOFF;
	NVIC_EnableIRQ(I2C1_IRQn);
	I2C1 -> OAR1 |= I2C_OAR1_OA1EN;

	/* DMA settings */
	RCC -> AHBENR |= RCC_AHBENR_DMA1EN;
	/* Receive
	 * Periph addr, priority, memory and periph sizes, mem inc mode, transfer complete and error IRQ, DMA request source */
	DMA1_Channel2 -> CPAR = (uint32_t)&(I2C1 -> RXDR);
	DMA1_Channel2 -> CCR |= DMA_CCR_PL_0 | DMA_CCR_MINC | DMA_CCR_TEIE | DMA_CCR_TCIE;
    NVIC_SetPriority(DMA1_Channel1_IRQn, 1); /* Set low priority because inside irq callback fun is called */
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	DMAMUX1_Channel1 -> CCR |= 10;
}


/* Returns true if i2c1 is ready to use */
bool is_i2c1_bus_free(void)
{
    if (I2C1 -> ISR & I2C_ISR_BUSY)
    {
        return false;
    }

    else
    {
        return true;
    }
}


void i2c1_write_polling(const uint8_t addr, const uint8_t reg, const uint8_t *const data, const uint8_t size)
{
	uint8_t i;
	/* Write reg address to slave */
	I2C1 -> CR2 = addr | ((size + 1) << 16) | I2C_CR2_AUTOEND;
	I2C1 -> TXDR = reg;
	I2C1 -> CR2 |= I2C_CR2_START;

	/* Write data */
	for (i = 0; i < size; i++)
	{
		while (!((I2C1 -> ISR) & I2C_ISR_TXIS));
		I2C1 -> TXDR = data[i];
	}
}


void i2c1_read_polling(const uint8_t addr, const uint8_t reg, uint8_t *const buf, const uint8_t size)
{
	uint8_t i;
	/* Write reg address to slave */
	I2C1 -> CR2 = addr | (1 << 16);
	I2C1 -> TXDR = reg;

	I2C1 -> CR2 |= I2C_CR2_START;
	while (!(I2C1 -> ISR & I2C_ISR_TC));

	/* Read slave reg value */
	I2C1 -> CR2 = addr | (size << 16) | I2C_CR2_RD_WRN;
	I2C1 -> CR2 |= I2C_CR2_START;
	for (i = 0; i < size; i++)
	{
		while (!(I2C1 -> ISR & I2C_ISR_RXNE));
		buf[i] = I2C1 -> RXDR;
	}

	/* Wait for transmission end */
	while (!(I2C1 -> ISR & I2C_ISR_TC));
	I2C1 -> CR2 |= I2C_CR2_STOP;
}


void i2c1_read_dma(const uint8_t addr, const uint8_t reg, uint8_t *const buf, const uint8_t size, const cb_t cb)
{
    /* Write callback address to holder */
    i2c1_recv_dma_cb = cb;

	/* Write reg address to slave */
	I2C1 -> CR2 = addr | (1 << 16);
	I2C1 -> TXDR = reg;

	I2C1 -> CR2 |= I2C_CR2_START;
	while (!(I2C1 -> ISR & I2C_ISR_TC));

	/* DMA transfer settings */
	I2C1 -> CR1 |= I2C_CR1_RXDMAEN;
	DMA1_Channel2 -> CNDTR = size;
	DMA1_Channel2 -> CMAR = (uint32_t)buf;

	/* Read slave reg value */
	I2C1 -> CR2 = addr | (size << 16) | I2C_CR2_RD_WRN;

	/* Start reading */
	DMA1_Channel2 -> CCR |= DMA_CCR_EN;
	I2C1 -> CR2 |= I2C_CR2_START;
}


/* I2C1 DMA reception end irq */
void DMA1_CH2_IRQHandler(void)
{
	if (DMA1 -> ISR & DMA_ISR_TCIF2)
	{
		/* Delete half and full complete transfer flags */
		DMA1 -> IFCR = DMA_IFCR_CTCIF2 | DMA_IFCR_CHTIF2;

		/* Disable DMA CH2 */
		DMA1_Channel2 -> CCR &= ~DMA_CCR_EN;

		/* Disable I2C RX DMA mode */
		I2C1 -> CR1 &= ~I2C_CR1_RXDMAEN;

		/* End I2C transmission */
		I2C1 -> CR2 |= I2C_CR2_STOP;

		/* Reception end callback */
		if (i2c1_recv_dma_cb != NULL)
        {
            i2c1_recv_dma_cb();
        }
	}
}


/* I2C1 Interrupt */
void I2C1_EV_IRQHandler(void)
{
	if (I2C1 -> ISR & I2C_ISR_STOPF)
	{
		I2C1 -> ICR = I2C_ICR_STOPCF;
	}
}