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

# Include your local headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Fetch libnetcore from GitHub
include(FetchContent)

FetchContent_Declare(
    libnetcore
    GIT_REPOSITORY https://github.com/Regis-Caelum/libnetcore.git
    GIT_TAG main
)

FetchContent_MakeAvailable(libnetcore)

# Collect your source files
file(GLOB_RECURSE LOAD_BALANCER_SOURCES "src/*.cpp")

# Define your executable
add_executable(LOAD_BALANCER ${LOAD_BALANCER_SOURCES})

# Link against NETCORE library from fetched content
target_link_libraries(LOAD_BALANCER PRIVATE NETCORE)

# Include NETCORE headers
target_include_directories(LOAD_BALANCER PRIVATE ${libnetcore_SOURCE_DIR}/include)

# Optional: tests
if(BUILD_TESTS)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
    add_executable(LOAD_BALANCER_TESTS ${TEST_SOURCES})
    target_link_libraries(LOAD_BALANCER_TESTS PRIVATE NETCORE gtest_main)

    add_test(NAME LOAD_BALANCER_TESTS COMMAND LOAD_BALANCER_TESTS)
endif()
