#include "i2c_tasks.hpp"

#define MAX_I2C_QUEUE_QUANTITY  10

static sem_t i2c1_op_finished_sem;

extern queue_t i2c1_queue = 0; //Without it mpu6050::update_data() gives linker error


static void give_sem_if_not_nullptr(sem_t sem);
static void i2c1_operation_finished();
static void exec_op_i2c1(struct i2c_queue_data& i2c_op);

void i2c1_op_handler(void *params)
{
    (void)params;

    i2c1_init();
    struct i2c_queue_data op_data;
    i2c1_queue = rtos_queue_create(MAX_I2C_QUEUE_QUANTITY, sizeof(struct i2c_queue_data));
    i2c1_op_finished_sem = rtos_sem_bin_create();

    while(1)
    {
        /* Operation request */
        if (rtos_queue_receive(i2c1_queue, &op_data, RTOS_MAX_DELAY) == 1)
        {
            /* Execute operation requested by other task */
            exec_op_i2c1(op_data);

            /* Operation finished */
            if (rtos_sem_take(i2c1_op_finished_sem, RTOS_MAX_DELAY) == 1)
            {
                give_sem_if_not_nullptr(op_data.sem_to_give);
            }
        }
    }

}

static void give_sem_if_not_nullptr(sem_t sem)
{
    if (sem != nullptr)
    {
        rtos_sem_give(sem);
    }
}

static void i2c1_operation_finished()
{
    give_sem_if_not_nullptr(i2c1_op_finished_sem);
}

static void exec_op_i2c1(struct i2c_queue_data& i2c_op)
{
    switch (i2c_op.op)
    {
        case I2C_OP_SEND_POLLING:
        {
            /* Check if buffer is not null pointer, if so abort */
            if (i2c_op.data_buf == nullptr) break;

            /* Execute operation */
            rtos_critical_section_enter();
            i2c1_write_polling(i2c_op.slave_addr, i2c_op.slave_reg, i2c_op.data_buf, i2c_op.data_size);
            rtos_critical_section_exit();

            /* Operation finished, return control to op_handler task */
            i2c1_operation_finished();
            break;
        }

        case I2C_OP_RECEIVE_POLLING:
        {
            /* Check if buffer is not null pointer, if so abort */
            if (i2c_op.data_buf == nullptr) break;

            /* Execute operation */
            rtos_critical_section_enter();
            i2c1_read_polling(i2c_op.slave_addr, i2c_op.slave_reg, i2c_op.data_buf, i2c_op.data_size);
            rtos_critical_section_exit();

            /* Operation finished, return control to op_handler task */
            i2c1_operation_finished();
            break;
        }

        
        case I2C_OP_SEND_DMA:
        {
            /* No implementation of dma send so far, abort operation */
            break;

            /* Check if buffer is not null pointer, if so abort */
            if (i2c_op.data_buf == nullptr) break;
            //i2c1_write_dma(i2c_op.slave_addr, i2c_op.slave_reg, i2c_op.data_buf, i2c_op.data_size, i2c1_operation_finished);
            break;
        }

        case I2C_OP_RECEIVE_DMA:
        {
            /* Check if buffer is not null pointer, if so abort */
            if (i2c_op.data_buf == nullptr) break;
            i2c1_read_dma(i2c_op.slave_addr, i2c_op.slave_reg, i2c_op.data_buf, i2c_op.data_size, i2c1_operation_finished);
            break;
        }
    }
}

