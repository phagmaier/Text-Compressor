# Set the minimum CMake version required
cmake_minimum_required(VERSION 3.16)

# Project name and version
project(MyProject VERSION 1.0 LANGUAGES CXX)

# Set the C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set optimization flags for performance (targeted for release builds)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -flto")
    elseif(MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /GL")
    endif()
endif()

# Enable warnings (without treating them as errors)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
    add_compile_options(/W4)
endif()

# Include all .cpp files in the src folder
file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Add the main source file and all other sources from src folder
add_executable(runme ${SRC_FILES})

# Include the src folder so headers can be found
target_include_directories(runme PRIVATE "${CMAKE_SOURCE_DIR}/src")
