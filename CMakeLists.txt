cmake_minimum_required(VERSION 3.20)

project(openshmem VERSION 2.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)

option(ENABLE_SHMEM "Enable OpenSHMEM" ON)
option(ENABLE_NVSHMEM "Enable NVSHMEM" OFF)
option(ENABLE_ISHMEM "Enable ISHMEM" OFF)
option(ENABLE_ROCSHMEM "Enable ROCSHMEM" OFF)

#option(BUILD_SHARED_LIBS "Build using shared libraries" ON)


configure_file(${PROJECT_SOURCE_DIR}/openshmem2_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_executable(openshmem2 openshmem2.cpp)

if (ENABLE_SHMEM)
    find_package(SHMEM REQUIRED)
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_STANDARD_REQUIRED True)
    set(CMAKE_CXX_COMPILER "oshc++")
    target_link_libraries(openshmem2 SHMEM)
elseif(ENABLE_NVSHMEM)
    find_package(NVSHMEM REQUIRED)
    set(CMAKE_CXX_COMPILER "nvcc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    target_link_libraries(openshmem2 NVSHMEM)
elseif(ENABLE_ISHMEM)
    find_package(ISHMEM REQUIRED)
    set(CMAKE_CXX_COMPILER "icpx")
    target_link_libraries(openshmem2 ISHMEM)
elseif(ENABLE_ROCSHMEM)
    find_package(ROCSHMEM REQUIRED)
    set(CMAKE_CXX_COMPILER "hipcc")
    target_link_libraries(openshmem2 ROCSHMEM)
endif()

target_include_directories(openshmem2 PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
