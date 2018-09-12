cmake_minimum_required(VERSION 3.12)
project(ini_editor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

set(LIB ini_lib)

add_library(${LIB}
		src/StringUtils.cpp src/StringUtils.h
		src/IniParser.cpp src/IniParser.h
		src/IniMapper.h src/IniMapper.cpp)

add_subdirectory(test)

add_executable(ini main.cpp)
target_link_libraries(ini ${LIB})
