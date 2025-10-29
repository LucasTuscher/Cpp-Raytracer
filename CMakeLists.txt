cmake_minimum_required(VERSION 3.14)
project(CppRaytracer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include FetchContent module for Google Test
include(FetchContent)

# Fetch Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Source files
set(SOURCES
    src/Main.cpp
)

# Create executable
add_executable(raytracer ${SOURCES})

# Include directories
target_include_directories(raytracer PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

# Source files for library
set(LIB_SOURCES
    src/Vector/Vector.cpp
    src/Point/Point.cpp
    src/Color/Color.cpp
)

# Create a library for shared code
add_library(raytracer_lib ${LIB_SOURCES})
target_include_directories(raytracer_lib PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
)

# Link library to main executable
target_link_libraries(raytracer raytracer_lib)

# Test files
set(TEST_SOURCES
    tests/TestMain.cpp
    tests/PointTests.cpp
    tests/VectorTests.cpp
    tests/ColorTests.cpp
)

# Create test executable
add_executable(raytracer_tests ${TEST_SOURCES})

# Link Google Test and raytracer library to the test executable
target_link_libraries(raytracer_tests
    GTest::gtest_main
    GTest::gmock_main
    raytracer_lib
)

# Include directories for tests
target_include_directories(raytracer_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/src
)

# Add tests
include(GoogleTest)
gtest_discover_tests(raytracer_tests)
