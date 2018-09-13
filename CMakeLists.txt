cmake_minimum_required(VERSION 3.12)
project(ini_editor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

set(LIB ini_lib)

add_library(${LIB}
		src/StringUtils.cpp src/StringUtils.h
		src/Parser.cpp src/Parser.h
		src/IniMapper.cpp src/IniMapper.h
		src/IniWriter.cpp src/IniWriter.h
		src/IniEditor.cpp src/IniEditor.h)

add_subdirectory(test)
