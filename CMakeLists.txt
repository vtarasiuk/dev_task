cmake_minimum_required (VERSION 3.22.1)
project (DEVTASK)

add_executable (devtask main.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")