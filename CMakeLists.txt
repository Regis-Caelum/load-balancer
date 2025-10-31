cmake_minimum_required(VERSION 3.20)

project(LOAD_BALANCER
    VERSION 1.0
    DESCRIPTION "Simple load balancer"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/std:c++latest)
endif()

file(GLOB_RECURSE LOAD_BALANCER_SOURCES "src/*.cpp")

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add the executable and link the library
add_library(LOAD_BALANCER_LIB ${LOAD_BALANCER_SOURCES})

add_executable(LOAD_BALANCER src/main.cpp)
target_link_libraries(LOAD_BALANCER LOAD_BALANCER_LIB)


if(BUILD_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    )

    # Only build GTest for this project
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    file(GLOB_RECURSE TEST_SOURCES "tests/*.cc")
    add_executable(LOAD_BALANCER_TESTS ${TEST_SOURCES})
    target_link_libraries(LOAD_BALANCER_TESTS LOAD_BALANCER_LIB gtest_main)

    add_test(NAME LOAD_BALANCER_TESTS COMMAND LOAD_BALANCER_TESTS)
endif()
