cmake_minimum_required(VERSION 3.10.0)
project(bastion)

include_directories(
    src/include
)

add_executable(bastion
    src/main.cpp    
    src/io.cpp
)