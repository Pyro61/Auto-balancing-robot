include(../cmake/compiler_flags.cmake)


set(INCLUDE_DIRS
    #Drivers
    ../Drivers/CMSIS/Include
    ../Drivers/CMSIS/Device/ST/STM32C0xx/Include
    ../Drivers/FreeRTOS/Src/include
    ../Drivers/FreeRTOS/Src/portable/GCC/ARM_CM0
    
    hw/uc_hw
    rtos_wrapper

    #FreeRTOSConfig file
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(C_SRCS
    #Cube generated files
    Core/Src/main.c
    Core/Src/syscalls.c
    Core/Src/sysmem.c

    #Application logic files

    #uC hw files
    hw/uc_hw/core_init/core_init.c
    hw/uc_hw/exceptions/exceptions.c
    hw/uc_hw/gpio/gpio.c
    hw/uc_hw/tim/tim.c

   
)

set (CXX_SRCS
    #Application logic files
    rtos_wrapper/rtos_hooks.cpp
    pid/pid.cpp
    motors/motors.cpp

    #External hw files
    hw/external_hw/pwm_motor_driver/pwm_motor_driver.cpp
)

set(ASM_SRCS
    #Startup file
    Core/Startup/startup_stm32c031c6tx.s
)

set(GLOBAL_DEFINES
    #Board name
    -DSTM32C031xx
)

set(LINKED_LIBS

)

link_directories(

)

include_directories(${INCLUDE_DIRS})
add_definitions(${GLOBAL_DEFINES})

#Generate files
if(DEFINED DEBUG)
    add_arm_executable(${CMAKE_PROJECT_NAME}_debug ${C_SRCS} ${CXX_SRCS} ${ASM_SRCS})
    arm_link_libraries(${CMAKE_PROJECT_NAME}_debug FreeRTOS)
else()
    add_arm_executable(${CMAKE_PROJECT_NAME}_release ${C_SRCS} ${CXX_SRCS} ${ASM_SRCS})
    arm_link_libraries(${CMAKE_PROJECT_NAME}_release FreeRTOS)
endif()