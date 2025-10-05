cmake_minimum_required(VERSION 3.10)
project(RPI_Arduino_Shim LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(LIB_PATH "Path to finani/ICM42688 repository (root)" "../ICM42688")
option(USE_SPI "Build with SPI support" ON)

include_directories(${PROJECT_SOURCE_DIR}/compat)
include_directories(${LIB_PATH}/src)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(lib_test ${SRC_FILES})
target_link_libraries(lib_test pthread)
