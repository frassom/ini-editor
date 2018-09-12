find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
add_executable(gtests mainTest.cpp IniParserTest.cpp)
target_link_libraries(gtests ${LIB} ${GTEST_BOTH_LIBRARIES})
