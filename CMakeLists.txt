cmake_minimum_required(VERSION 3.10)
project(ProjectEmot C)

set(CMAKE_C_STANDARD 99)

add_executable(ProjectEmot  API.h string.c string.h list.c list.h API.c helpers.h helpers.c main.c)