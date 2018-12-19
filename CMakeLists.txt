cmake_minimum_required(VERSION 2.8)

project(ScenarioCpp)

set(CMAKE_CXX_STANDARD 11)

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

include_directories(include)

set(SRC_FILES src/main.cpp src/example.cpp src/scenariocpp-all.cpp)
file(GLOB TEST_FILES test/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${TEST_FILES})

target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

