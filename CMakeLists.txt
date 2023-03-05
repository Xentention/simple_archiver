cmake_minimum_required(VERSION 3.24)
project(simple_archiver C)

set(CMAKE_C_STANDARD 17)

add_executable(simple_archiver src/c_files/main.c src/headers/main.h src/c_files/archiver.c src/c_files/unarchiver.c src/headers/archiver.h src/headers/unarchiver.h)
