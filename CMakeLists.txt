cmake_minimum_required(VERSION 3.16)

project(thread-pool)

set(CMAKE_CXX_STANDARD 23)

add_executable(
    thread-pool

    main.cpp
    ThreadPool.cpp
)

add_executable(
    par

    par.cpp
    ThreadPool.cpp
)

add_executable(
    seq

    sec.cpp
    ThreadPool.cpp
)

include_directories(.)
