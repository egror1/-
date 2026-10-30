cmake_minimum_required(VERSION 3.20)

project(clock LANGUAGES CXX)

option(ENABLE_DEBUG "Build with debugging symbols" OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy clang-tidy-18 REQUIRED)
find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format clang-format-18 REQUIRED)
set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE}")

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
set(HEADERS)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wpedantic)

if(ENABLE_DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE -g -O0)
endif()

add_custom_target(clang-format
    COMMAND ${CLANG_FORMAT_EXECUTABLE} --dry-run -Werror ${SOURCES} ${HEADERS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Checking C++ formatting"
    VERBATIM
)
