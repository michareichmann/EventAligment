cmake_minimum_required(VERSION 3.3)
project(EventAlignment)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp EventAlignment.h EventAlignment.cc)
add_executable(EventAlignment ${SOURCE_FILES})