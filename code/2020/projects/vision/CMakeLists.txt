cmake_minimum_required(VERSION 2.8)

project(capture)

find_package(OpenCV REQUIRED)

message (${OpenCV_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
# include_directories(${CMAKE_SOURCE_DIR}/../ntcore/wpiutil/include)

add_executable(cv_capture capture.cpp Contour.cpp ContourPair.cpp HSV.cpp VisionCalculations.cpp)

SET(GCC_ADDITIONAL_FLAGS "-std=c++17 -g")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_ADDITIONAL_FLAGS}" )

message (${OpenCV_LIBS})
target_link_libraries(cv_capture ${OpenCV_LIBS})

add_library(ntcore SHARED IMPORTED)
set_target_properties(ntcore PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/allwpilib/buildcmake/lib/libntcore.so"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/allwpilib/ntcore/src/main/native/include")

target_link_libraries(cv_capture ntcore)

add_library(cscore SHARED IMPORTED)
set_target_properties(cscore PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/allwpilib/buildcmake/lib/libcscore.so"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/allwpilib/wpiutil/src/main/native/include")

target_link_libraries(cv_capture cscore)
