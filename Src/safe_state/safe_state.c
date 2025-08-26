#include "safe_state.h"
#include "../Drivers/CMSIS/Include/cmsis_gcc.h"


void safe_state()
{
    __disable_irq();
    while (1);
}