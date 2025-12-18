set(Start_Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

set(Start_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/stm32f1xx_it.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sysmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/syscalls.c
)

target_include_directories(stm32cubemx INTERFACE ${Start_Inc})

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${Start_Src})
target_sources(STM32_Drivers PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/system_stm32f1xx.c)