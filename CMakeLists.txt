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

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.18.0
)
FetchContent_MakeAvailable(googletest)

# 1. Define a custom configuration profile for Sanitizers
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES};TSAN" CACHE STRING "" FORCE)

# 2. Configure flags specifically for the 'TSAN' build type
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # Global compilation flags for all targets
    add_compile_options("$<$<CONFIG:TSAN>:-fsanitize=thread;-fno-omit-frame-pointer;-g>")

    # Global linking flags for all targets
    add_link_options("$<$<CONFIG:TSAN>:-fsanitize=thread>")
endif()

add_library(
    common

    ThreadPool.cpp
    SPSCQueue.cpp
    MutexQueue.cpp
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
    bench-spsc

    bench-spsc.cpp
    spsc_test.cpp
    mutex_queue_test.cpp
)
target_link_libraries(
    bench-spsc

    common
    benchmark::benchmark
    benchmark::benchmark_main
)

add_executable(
    atomic-counter

    AtomicCounter.cpp
)

add_executable(
    atomic-ready

    AtomicReady.cpp
)

add_executable(
    atomic-pointer

    AtomicPointer.cpp
)

add_executable(
    paralel-max

    ParalelMax.cpp
)

add_library(
    fetch-add-vs-cas-counter

    FetchAddCounter.cpp
    CasCounter.cpp
)

add_executable(
    fetch-add-vs-cas-counter-test

    FetchAddCounterTest.cpp
    CasCounterTest.cpp
)
target_link_libraries(
    fetch-add-vs-cas-counter-test

    fetch-add-vs-cas-counter
    GTest::gtest
    GTest::gtest_main
)

add_executable(
    fetch-add-vs-cas-counter-bench

    FetchAddCounterBench.cpp
    CasCounterBench.cpp
)
target_link_libraries(
    fetch-add-vs-cas-counter-bench

    fetch-add-vs-cas-counter
    benchmark::benchmark
    benchmark::benchmark_main
)

add_library(
    atomic-stack

    AtomicStack.cpp
)
add_executable(
    atomic-stack-test

    AtomicStackTest.cpp
)
target_link_libraries(
    atomic-stack-test

    atomic-stack
    GTest::gtest
    GTest::gtest_main
)

add_executable(
    relaxed-vs-seq-cst-counter-bench

    RelaxedVsSeqCstCounterBench.cpp
)
target_link_libraries(
    relaxed-vs-seq-cst-counter-bench

    fetch-add-vs-cas-counter
    benchmark::benchmark
    benchmark::benchmark_main
)

add_library(
    three-transitive-threads

    ThreeTransitiveThreads.cpp
)
add_executable(
    three-transitive-threads-test

    ThreeTransitiveThreadsTest.cpp
)
target_link_libraries(
    three-transitive-threads-test

    three-transitive-threads
    GTest::gtest
    GTest::gtest_main
)

include_directories(.)
