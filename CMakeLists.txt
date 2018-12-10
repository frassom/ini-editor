cmake_minimum_required(VERSION 3.12)
project(ini-editor)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

add_library(${PROJECT_NAME}
		include/ini/stringUtils.h 		src/stringUtils.cpp 
		include/ini/parser.h			src/parser.cpp 		
		include/ini/Editor.h			src/Editor.cpp 		
		include/ini/ParseException.h)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/ini)

add_subdirectory(test)
