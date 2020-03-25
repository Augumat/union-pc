cmake_minimum_required(VERSION 2.8.11)

project(union-pc)

set_property(GLOBAL PROPERTY CXX_STANDARD 11)

find_package(Threads)

include_directories(src/includes)

add_executable(
    union-pc
    src/main.cpp
    src/Connection.cpp
    src/Server.cpp
)

target_link_libraries(union-pc ${CMAKE_THREAD_LIBS_INIT})
