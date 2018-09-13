cmake_minimum_required(VERSION 3.12)
project(ini_editor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

set(LIB ini-editor)

add_library(${LIB}
		src/StringUtils.cpp src/StringUtils.h
		src/Parser.cpp src/Parser.h
		src/Mapper.cpp src/Mapper.h
		src/Writer.cpp src/Writer.h
		src/IniEditor.cpp src/IniEditor.h)

add_subdirectory(test)
