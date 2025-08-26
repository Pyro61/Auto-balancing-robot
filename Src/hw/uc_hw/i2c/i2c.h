#ifndef _I2C_
#define _I2C_


#include <stdbool.h>
#include <stdint.h>


typedef void (*cb_t)(void);

/********************************************************************************************/
/*                                           I2C1                                           */
/********************************************************************************************/

void i2c1_init(void);
void i2c1_write_polling(const uint8_t addr, const uint8_t reg, const uint8_t *const data, const uint8_t size);
void i2c1_read_polling(const uint8_t addr, const uint8_t reg, uint8_t *const buf, const uint8_t size);
void i2c1_read_dma(const uint8_t addr, const uint8_t reg, uint8_t *const buf, const uint8_t size, const cb_t cb);
bool is_i2c1_bus_free(void);


#endif /* _I2C_ */
