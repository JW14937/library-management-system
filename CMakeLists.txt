cmake_minimum_required(VERSION 2.8)

project(library)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_C_FLAGS "-std=c99")

add_executable(library main.c user_management.c book_management.c)