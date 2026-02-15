#include <stdint.h>
#include "../../rtos_wrapper/rtos_wrapper.hpp"

#include "../../hw/uc_hw/core_init/core_init.h"


void tasks_init(void);


int main(void)
{
   core_init();
   tasks_init();
   rtos_start_scheduler();

   /* No need of infinity loop because rtos scheduler should never return */
}



