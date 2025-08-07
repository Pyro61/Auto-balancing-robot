include(../../cmake/compiler_flags.cmake)

add_library(FreeRTOS STATIC
    #uC specific files
	Src/portable/GCC/ARM_CM0/mpu_wrappers_v2_asm.c
    Src/portable/GCC/ARM_CM0/port.c
    Src/portable/GCC/ARM_CM0/portasm.c
    #FreeRTOS kernel files
    Src/croutine.c
    Src/event_groups.c
    Src/list.c
    Src/queue.c
    Src/stream_buffer.c
    Src/tasks.c
    Src/timers.c


    #Heap implementation file
    Src/portable/MemMang/heap_4.c
)

target_include_directories(FreeRTOS PUBLIC
    Src/include
    Src/portable/GCC/ARM_CM0
    ../../Src
    ../../Src/hw/uc_hw
    ../CMSIS/Device/ST/STM32C0xx/Include
    ../CMSIS/Include
)
# Board name
add_definitions(-DSTM32C031xx)

target_include_directories(FreeRTOS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(FreeRTOS PRIVATE "-Wall")