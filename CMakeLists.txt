cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project(volvo)

include_directories(src)
include_directories(src/Pose)
include_directories(src/Rendering)
include_directories(src/FeatureExtractor)

find_package(Boost COMPONENTS system filesystem regex REQUIRED)

find_package(Protobuf)

find_package(CUDA REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(Caffe REQUIRED)
include_directories(${Caffe_INCLUDE_DIRS})
add_definitions(${Caffe_DEFINITIONS})    # ex. -DCPU_ONLY
message("Caffe_DIR: ${Caffe_DIR}")	#specify Caffe_DIR in /cmake/Modules/FindCaffe.cmake

set(GLOG_LIB glog)

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
file(GLOB_RECURSE FEATURE_EXTRACTOR_FILES "src/FeatureExtractor/*")

# add_executable(${PROJECT_NAME} ${RENDERING_SOURCE_FILES} "src/main.cpp")
# target_link_libraries(volvo ${OpenCV_LIBS})

add_executable(GeneratePoseFeature ${POSE_SOURCE_FILES} ${RENDERING_SOURCE_FILES} ${PF_SOURCE_FILES} "src/main_pose.cpp")
target_link_libraries(GeneratePoseFeature ${OpenCV_LIBS} ${GSLLIB})

add_executable(PFTracker ${PF_SOURCE_FILES} ${POSE_SOURCE_FILES} ${RENDERING_SOURCE_FILES} ${FEATURE_EXTRACTOR_FILES} "src/main_pf.cpp")
target_link_libraries(PFTracker ${Boost_LIBRARIES} ${PROTOBUF_LIBRARIES} ${GLOG_LIB} ${Caffe_LIBRARIES} ${OpenCV_LIBS} ${GSLLIB})
LINK_LIBRARIES(${GSLLIB})