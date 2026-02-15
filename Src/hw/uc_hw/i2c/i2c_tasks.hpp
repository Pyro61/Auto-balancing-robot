#ifndef _I2C_TASKS_
#define _I2C_TASKS_

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

#include "i2c.h"
#include "rtos_wrapper.hpp"

enum i2c_operation
{
    I2C_OP_SEND_POLLING,
    I2C_OP_RECEIVE_POLLING,
    I2C_OP_SEND_DMA,
    I2C_OP_RECEIVE_DMA
};

struct i2c_queue_data
{
    enum i2c_operation op;
    uint8_t *data_buf;
    uint8_t data_size;
    uint8_t slave_addr;
    uint8_t slave_reg;
    sem_t sem_to_give;
};

/* I2C1 queue handler */
extern queue_t i2c1_queue;


/* Task function */
void i2c1_op_handler(void *params);





#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _I2C_TASKS_ */
