# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

set(CMAKE_BUILD_TYPE "Debug")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(lab4 LANGUAGES C)
