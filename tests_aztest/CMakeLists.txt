# CMakeLists.txt for AZTest-based tests

cmake_minimum_required(VERSION 3.14)

# Test sources for AZTest
set(AZTEST_SOURCES
    TestMain_AZTest.cpp
    PointTests_AZTest.cpp
    VectorTests_AZTest.cpp
    ColorTests_AZTest.cpp
    MatrixTests_AZTest.cpp
    IntersectionTests_AZTest.cpp
    SphereTests_AZTest.cpp
    SceneTests_AZTest.cpp
    CameraTests_AZTest.cpp
    LightSourceTests_AZTest.cpp
    ShadowTests_AZTest.cpp
)

# Create test executable
add_executable(raytracer_aztest ${AZTEST_SOURCES})

# Link AZTest libraries
target_link_libraries(raytracer_aztest
    raytracer_lib
    AZTest
    AZTestMain
)

# Include directories
target_include_directories(raytracer_aztest PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/libs/AZTest-Cpp/include
)

# Add test
add_test(NAME AZTestRun COMMAND raytracer_aztest)
