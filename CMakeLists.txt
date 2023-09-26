cmake_minimum_required(VERSION 3.22)

project(chestnut)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()
add_subdirectory(chestnut)
add_subdirectory(static)
