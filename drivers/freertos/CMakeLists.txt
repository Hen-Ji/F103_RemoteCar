target_include_directories(stm32cubemx INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/port
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/croutine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/event_groups.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tasks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/port/heap_4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/port/port.c
)