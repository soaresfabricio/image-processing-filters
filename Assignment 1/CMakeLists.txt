cmake_minimum_required(VERSION 2.8)
project(pdi)
set(CMAKE_CXX_STANDARD 11)
find_package( OpenCV REQUIRED )
include_directories(.)
add_executable(pdi Main.cpp Color.cpp Color.hpp Common.hpp Filter.cpp Filter.hpp Noise.hpp Noise.cpp)
target_link_libraries( pdi ${OpenCV_LIBS} )

