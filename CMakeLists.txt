cmake_minimum_required(VERSION 2.8)

project(sol)

OPTION(DEBUG "add debug flags" OFF)

if (DEBUG)
    message(STATUS "Configuring build for debug")
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error=unused-result -luuid -ggdb -fsanitize=address -fsanitize=undefined")
else (DEBUG)
    message(STATUS "Configuring build for production")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-error=unused-result -luuid -ggdb -fsanitize=address -fsanitize=undefined")
endif (DEBUG)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

file(GLOB SOURCES src/*.c)

set(AUTHOR "Andrea Giacomo Baldan")
set(LICENSE "BSD2 license")

# Executable
add_executable(sol ${SOURCES})