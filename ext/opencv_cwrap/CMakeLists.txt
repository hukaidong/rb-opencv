cmake_minimum_required(VERSION 3.16)
project(opencv_cwrap)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(opencv_cwrap SHARED opencv_cwrap.cpp)
target_link_libraries(opencv_cwrap ${OpenCV_LIBS})
