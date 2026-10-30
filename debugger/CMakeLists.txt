cmake_minimum_required(VERSION 3.20)

project(debugger_demo LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
set(HEADERS)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_compile_options(${PROJECT_NAME} PRIVATE -g -O0 -Wall -Wextra -Werror -Wpedantic)
