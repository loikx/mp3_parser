cmake_minimum_required(VERSION 3.0.0)

project(
        lab7
        VERSION 1.0
        DESCRIPTION "C++ Year 2023 Lab7"
        LANGUAGES CXX C
)
set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_EXE_LINKER_FLAGS "-static")

add_subdirectory(lib/utils)
add_subdirectory(lib/header)
add_subdirectory(lib/frame)
add_subdirectory(lib/builder)

#add_subdirectory(lib)
add_subdirectory(bin)