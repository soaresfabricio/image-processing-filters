
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")


cmake_minimum_required(VERSION 3.12)
project(pdi)
set(CMAKE_CXX_STANDARD 11)
find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED )

find_package(OpenMP REQUIRED)
if(NOT TARGET OpenMP::OpenMP_CXX)
    add_library(OpenMP_TARGET INTERFACE)
    add_library(OpenMP::OpenMP_CXX ALIAS OpenMP_TARGET)
    target_compile_options(OpenMP_TARGET INTERFACE ${OpenMP_CXX_FLAGS})
    find_package(Threads REQUIRED)
    target_link_libraries(OpenMP_TARGET INTERFACE Threads::Threads)
    target_link_libraries(OpenMP_TARGET INTERFACE ${OpenMP_CXX_FLAGS})
endif()


include_directories(.)
add_executable(pdi Main.cpp Color.cpp Color.hpp Common.hpp Filter.cpp Filter.hpp Noise.hpp Noise.cpp)
target_link_libraries( pdi ${OpenCV_LIBS} OpenMP::OpenMP_CXX)
