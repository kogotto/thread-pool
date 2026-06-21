cmake_minimum_required(VERSION 3.16)

project(thread-pool)

set(CMAKE_CXX_STANDARD 23)

add_library(
    common

    ThreadPool.cpp
    SPSCQueue.cpp
)

add_executable(
    thread-pool

    main.cpp
)
target_link_libraries(
    thread-pool

    common
)

add_executable(
    par

    par.cpp
)
target_link_libraries(
    par

    common
)

add_executable(
    seq

    sec.cpp
)
target_link_libraries(
    seq

    common
)

add_executable(
    wait

    wait.cpp
)
target_link_libraries(
    wait

    common
)

include_directories(.)
