cmake_minimum_required(VERSION 3.7)
project(batman)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
find_package(Udev REQUIRED)
include_directories(${Udev_INCLUDE_DIR})

set(SOURCE_FILES batman.cpp)
add_executable(batman ${SOURCE_FILES})
target_link_libraries(batman stdc++fs ${Udev_LIBRARY})
