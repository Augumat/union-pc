cmake_minimum_required(VERSION 2.8.11)

project(union-pc)

add_compile_options(-std=c++17)
set(CMAKE_BUILD_TYPE Debug)

find_package(Threads)

include_directories(include/ libs/)

add_executable(
    union-pc
    src/main.cpp
    src/Connection.cpp
    src/Server.cpp
    src/ProgramState.cpp
    src/serialization.cpp
    src/mainTasks.cpp
)

target_link_libraries(union-pc ${CMAKE_THREAD_LIBS_INIT})
