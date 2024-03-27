cmake_minimum_required(VERSION 3.27)
project(HSVColorFiltering)

set(CMAKE_CXX_STANDARD 17)

# Find OpenCV library
find_package(OpenCV REQUIRED)

add_executable(HSVColorFiltering HSVColorFiltering.cpp
        HSVRange.h)

# Link OpenCV to your executable
target_link_libraries(HSVColorFiltering ${OpenCV_LIBS})