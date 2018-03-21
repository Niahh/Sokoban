cmake_minimum_required(VERSION 3.9)
project(Sokoban)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(Sokoban
        src/main.c src/array_list.c src/array_list.h src/sokoban.h src/sokoban.c)
