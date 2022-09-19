cmake_minimum_required(VERSION 2.4)

project(lc3_emulator)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_executable(emulator ${SOURCES})
