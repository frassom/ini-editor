cmake_minimum_required(VERSION 3.12)
project(ini_editor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

add_executable(ini_editor main.cpp)