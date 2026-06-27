cmake_minimum_required(VERSION 3.16)

project(thread-pool)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

# Prevent the library from building its own
# internal tests/docs to speed up your build
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.9.5
)

FetchContent_MakeAvailable(googlebenchmark)

add_library(
    common

    ThreadPool.cpp
    SPSCQueue.cpp
)
target_compile_options(
    common
    PUBLIC
    -Wno-interference-size
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

add_executable(
    bench-spsc

    bench-spsc.cpp
    spsc_test.cpp
)
target_link_libraries(
    bench-spsc

    common
    benchmark::benchmark
    benchmark::benchmark_main
)

include_directories(.)
