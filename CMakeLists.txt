cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(volvo)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB_RECURSE RENDERING_SOURCE_FILES "src/Rendering/*")
# file(GLOB_RECURSE POSE_SOURCE_FILES "src/Pose/*")

add_executable(${PROJECT_NAME} ${RENDERING_SOURCE_FILES} "src/main.cpp")
target_link_libraries(volvo ${OpenCV_LIBS})

# add_executable(GeneratePoseFeature ${POSE_SOURCE_FILES})
# target_link_libraries(GeneratePoseFeature ${OpenCV_LIBS})
