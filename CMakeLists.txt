cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(volvo)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCE_FILES "src/*")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(volvo ${OpenCV_LIBS})
