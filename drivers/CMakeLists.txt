target_include_directories(stm32cubemx INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_subdirectory(cmsis)
add_subdirectory(hal)
add_subdirectory(freertos)