#ifndef _RTOS_WRAPPER_
#define _RTOS_WRAPPER_

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */


/* RTOS includes */
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/**
 * Convert time interval in milliseconds to RTOS ticks.
 *
 * @param ms            Duration in milliseconds.
 *
 * @return              Duration in RTOS ticks .
 */
#define rtos_ms_to_ticks(ms)        ((ms) / portTICK_RATE_MS)

/**
 * Convert time interval in RTOS ticks to milliseconds.
 *
 * @param ms            Duration in RTOS ticks.
 *
 * @return              Duration in milliseconds.
 */
#define rtos_ticks_to_ms(ticks)     ((ticks) * portTICK_RATE_MS)

/**
 * Start RTOS Scheduler
 */
#define rtos_start_scheduler()      vTaskStartScheduler()

/**
 * Create RTOS task.
 *
 * @param ptr           Pointer to task function.
 * @param name          Name string.
 * @param stack         Task stack size.
 * @param prio          Task priority.
 *
 * @return              Task handle.
 */
#define rtos_task_create(ptr, name, stack, params, prio)                               \
   xTaskCreate(ptr, (const char *)name, stack, params, prio, NULL)

/**
 * Set task priority.
 *
 * @param task          Task handle.
 * @param prio          Task priority.
 */
#define rtos_task_priority_set(task, prio)                                     \
   vTaskPrioritySet(task, prio)


/**
 * Delay task for specified amount of milliseconds.
 *
 * @param ms            Number of milliseconds the task should be delayed.
 */
#define rtos_delay(ms)                                                         \
   vTaskDelay(ms/portTICK_RATE_MS)

/**
 * Delay task for specified amount of millisecond since last_ptr.
 *
 * @param last_ptr      Pointer to start ticks.
 * @param ms            Number of milliseconds the task should be delayed.
 */
#define rtos_delay_until(last_ptr, ms)                                         \
    vTaskDelayUntil(last_ptr, ms)

/**
 * Get current system tick count.
 *
 * @return              Tick count.
 */
#define rtos_tick_count_get()                                                  \
   xTaskGetTickCount()

/**
 * Create RTOS queue.
 *
 * @param len           Length of the queue.
 * @param item_size     Size of the single item on the queue.
 *
 * @return              Queue handle.
 */
#define rtos_queue_create(len, item_size)                                      \
   xQueueCreate(len, item_size)

/**
 * Send data to the queue.
 *
 * @param queue_ptr     Queue handle.
 * @param data_ptr      Pointer to data to send.
 * @param ms            Time to wait for queue to be ready.
 *
 * @return              true on success, false on timeout.
 */
#define rtos_queue_send(queue_ptr, data_ptr, ms)                               \
   xQueueSend(queue_ptr, (void *)data_ptr, rtos_ms_to_ticks(ms))

/**
 * Receive data from the queue.
 *
 * @param queue_ptr     Queue handle.
 * @param data_ptr      Pointer to data to receive.
 * @param ms            Time to wait for data to be received.
 *
 * @return              true on success, false on timeout.
 */
#define rtos_queue_receive(queue_ptr, data_ptr, ms)                            \
   xQueueReceive(queue_ptr, (void *)data_ptr, rtos_ms_to_ticks(ms))



/**
 * Create RTOS semaphore.
 *
 * @return              Handle to semaphore to be created.
 */
#define rtos_sem_bin_create()                                           \
   xSemaphoreCreateBinary()

/**
 * Acquire semaphore.
 *
 * @param sem_ptr       Semaphore handle.
 * @param ms            Time to wait for a semaphore.
 *
 * @return              true on success, false on timeout.
 */
#define rtos_sem_take(sem_ptr, ms)                                             \
   xSemaphoreTake(sem_ptr, rtos_ms_to_ticks(ms))

/**
 * Release semaphore.
 *
 * @param sem_ptr       Semaphore handle.
 *
 * @return              true on success, false on error.
 */
#define rtos_sem_give(sem_ptr)                                                 \
   xSemaphoreGive(sem_ptr)

/**
 * Acquire semaphore from the interrupt.
 *
 * @param sem_ptr       Semaphore handle.
 * @param yield         Flag indicating if context switch is needed.
 */
#define rtos_sem_take_isr(sem_ptr, yield)                                      \
   xSemaphoreTakeFromISR(sem_ptr, yield)

/**
 * Release semaphore from the interrupt.
 *
 * @param sem_ptr       Semaphore handle.
 * @param yield         Flag indicating if context switch is needed.
 */
#define rtos_sem_give_isr(sem_ptr, yield)                                      \
   xSemaphoreGiveFromISR(sem_ptr, yield)



/**
 * Create RTOS mutex.
 *
 * @return              Mutex handle.
 */
#define rtos_mutex_create()                                                    \
    xSemaphoreCreateMutex()

/**
 * Acquire mutex.
 *
 * @param mutex_ptr     Mutex handle.
 * @param ms            Time to wait on a mutex.
 *
 * @return              true on success, false on timeout.
 */
#define rtos_mutex_take(mutex_ptr, ms)                                         \
   xSemaphoreTake(mutex_ptr, rtos_ms_to_ticks(ms))

/**
 * Release mutex.
 *
 * @param mutex_ptr     Mutex handle.
 *
 * @return              true on success, false on timeout.
 */
#define rtos_mutex_give(mutex_ptr)                                             \
   xSemaphoreGive(mutex_ptr)
/* Mutexes should not be given/taken from ISR */

/**
 * Enter critical section.
 */
#define rtos_critical_section_enter()                                          \
    taskENTER_CRITICAL()

/**
 * Exit critical section.
 */
#define rtos_critical_section_exit()                                           \
   taskEXIT_CRITICAL()

/** Type of RTOS queue. */
//typedef void * queue_t;
typedef QueueHandle_t queue_t;

/** Type of RTOS semaphore. */
//typedef void * sem_t;
typedef SemaphoreHandle_t sem_t;
/** Type of RTOS mutex. */
//typedef void * mutex_t;
typedef SemaphoreHandle_t mutex_t;

/** Type of RTOS tick. */
typedef TickType_t tick_t;


#define RTOS_TASK_STACKSIZE_MIN     configMINIMAL_STACK_SIZE
#define RTOS_PRIORITY_MIN           tskIDLE_PRIORITY
#define RTOS_MAX_DELAY              portMAX_DELAY

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RTOS_WRAPPER_ */