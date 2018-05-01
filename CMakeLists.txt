cmake_minimum_required (VERSION 2.6)

project (CT_MAIN)

include_directories (timer utils)
add_definitions("-std=c++11 -g")
set(CMAKE_EXE_LINKER_FLAGS "-pthread")
add_executable(test main.cpp timer/ct_timer.cpp)
