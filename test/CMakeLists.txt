find_package(GTest REQUIRED)

add_executable(gtests mainTest.cpp
		ParserTest.cpp EditorTest.cpp)

target_include_directories(gtests PRIVATE
	${PROJECT_SOURCE_DIR}/include/ini
	${GTEST_INCLUDE_DIRS})

target_link_libraries(gtests ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})
