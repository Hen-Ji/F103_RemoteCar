set(Core_Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

aux_source_directory(src Core_Src) # 将src中的所有文件添加进Core_Src

target_include_directories(stm32cubemx INTERFACE ${Core_Inc})

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${Core_Src})