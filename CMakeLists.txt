cmake_minimum_required(VERSION 3.12)
project(ini-editor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

add_library(${PROJECT_NAME}
		src/stringUtils.cpp src/stringUtils.h
		src/parser.cpp src/parser.h
		src/Editor.cpp src/Editor.h)

add_subdirectory(test)
