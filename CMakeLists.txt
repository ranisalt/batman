cmake_minimum_required(VERSION 3.7)
project(batman)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES batman.cpp)
add_executable(batman ${SOURCE_FILES})
target_link_libraries(batman stdc++fs)
