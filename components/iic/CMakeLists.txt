target_include_directories(stm32cubemx INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/iic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/iicSoftware.cpp
)