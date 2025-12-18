target_include_directories(stm32cubemx INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_sources(STM32_Drivers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32f1xx_ll_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32f1xx_ll_i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32f1xx_ll_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32f1xx_ll_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32f1xx_ll_utils.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32f1xx_ll_exti.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32f1xx_ll_spi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32f1xx_ll_pwr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32f1xx_ll_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32f1xx_ll_usart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32f1xx_ll_adc.c
)