target_include_directories(stm32cubemx INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/os.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/osGroup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/osList.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/osMutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/osTask.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/osAtomic.cpp
)