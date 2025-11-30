cmake_minimum_required(VERSION 4.0)
project(Project_Boids)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(include/bd)

add_executable(Project_Boids
        src/Vec2.cpp
        include/bd/Vec2.h
        src/DynamicArray.cpp
        include/bd/DynamicArray.h
        src/main.cpp
)
