cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_PROJECT_NAME MultiTask_System)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(${CMAKE_PROJECT_NAME})
message("Build type: ${CMAKE_BUILD_TYPE}")

enable_language(C ASM)
add_executable(${CMAKE_PROJECT_NAME})
add_subdirectory(cmake/stm32cubemx)

set(FREERTOS_DIR ${CMAKE_SOURCE_DIR}/Middlewares/Third_Party/FreeRTOS/Source)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Core/Src/app_freertos.c
    Core/Src/hcsr04.c
    ${FREERTOS_DIR}/tasks.c
    ${FREERTOS_DIR}/queue.c
    ${FREERTOS_DIR}/list.c
    ${FREERTOS_DIR}/portable/MemMang/heap_4.c
    ${FREERTOS_DIR}/portable/GCC/ARM_CM4F/port.c
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${FREERTOS_DIR}/include
    ${FREERTOS_DIR}/portable/GCC/ARM_CM4F
    Core/Inc
)

list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
)
