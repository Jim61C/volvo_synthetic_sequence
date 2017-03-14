cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(volvo)

include_directories(src)
include_directories(src/Pose)
include_directories(src/Rendering)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

FIND_PACKAGE(GSL REQUIRED)

if (GSL_FOUND)
	message("--- found GSL, include directories.")
	INCLUDE_DIRECTORIES(${GSL_INCLUDE_DIRS} ${GSLCBLAS_INCLUDE_DIRS})
	SET(GSLLIB ${LIBS} ${GSL_LIBRARIES} ${GSLCBLAS_LIBRARIES})
else ()
	message("--- could not find GSL")
endif ()


file(GLOB_RECURSE RENDERING_SOURCE_FILES "src/Rendering/*")
file(GLOB_RECURSE POSE_SOURCE_FILES "src/Pose/*")
file(GLOB_RECURSE PF_SOURCE_FILES "src/ParticleFilter/*")

# add_executable(${PROJECT_NAME} ${RENDERING_SOURCE_FILES} "src/main.cpp")
# target_link_libraries(volvo ${OpenCV_LIBS})

add_executable(GeneratePoseFeature ${POSE_SOURCE_FILES} ${RENDERING_SOURCE_FILES} "src/main_pose.cpp")
target_link_libraries(GeneratePoseFeature ${OpenCV_LIBS})

add_executable(PFTracker ${PF_SOURCE_FILES} ${POSE_SOURCE_FILES} ${RENDERING_SOURCE_FILES} "src/main_pf.cpp")
target_link_libraries(PFTracker ${OpenCV_LIBS} ${GSLLIB})
LINK_LIBRARIES(${GSLLIB})