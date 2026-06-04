set(STM32CUBEF1_DIR "${CMAKE_SOURCE_DIR}/vendor/STM32CubeF1")
set(HAL_ROOT_DIR "${STM32CUBEF1_DIR}/Drivers/STM32F1xx_HAL_Driver")
set(HAL_SOURCE_DIR "${HAL_ROOT_DIR}/Src")

set(HAL_SOURCES
    "${HAL_SOURCE_DIR}/stm32f1xx_hal.c"
    "${HAL_SOURCE_DIR}/stm32f1xx_hal_rcc.c"
    "${HAL_SOURCE_DIR}/stm32f1xx_hal_rcc_ex.c"
    "${HAL_SOURCE_DIR}/stm32f1xx_hal_gpio.c"
    "${HAL_SOURCE_DIR}/stm32f1xx_hal_cortex.c"
    "${HAL_SOURCE_DIR}/stm32f1xx_hal_uart.c"
    "${HAL_SOURCE_DIR}/stm32f1xx_hal_dma.c"
    "${HAL_SOURCE_DIR}/stm32f1xx_hal_flash.c"
    "${HAL_SOURCE_DIR}/stm32f1xx_hal_flash_ex.c")

add_library(stm32cubef1 STATIC ${HAL_SOURCES})

target_include_directories(stm32cubef1 PUBLIC
    ${STM32CUBEF1_DIR}/Drivers/CMSIS/Core/Include
    ${STM32CUBEF1_DIR}/Drivers/CMSIS/Device/ST/STM32F1xx/Include
    ${HAL_ROOT_DIR}/Inc
    ${CMAKE_SOURCE_DIR}/core)
