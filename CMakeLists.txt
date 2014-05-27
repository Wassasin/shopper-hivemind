cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/contrib/wubwubcmake/modules")

add_definitions("-Wall -Wextra -Weffc++ -std=c++0x -pedantic -O0 -fno-inline -g")

add_subdirectory("src")
