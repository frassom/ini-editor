find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(gtests mainTest.cpp
		ParserTest.cpp MapperTest.cpp WriterTest.cpp IniEditorTest.cpp)

target_link_libraries(gtests ${LIB} ${GTEST_BOTH_LIBRARIES})
