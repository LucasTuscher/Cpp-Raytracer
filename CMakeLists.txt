cmake_minimum_required(VERSION 3.14)
project(CppRaytracer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_TESTS "Build unit tests" OFF)

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
    src/Canvas/Canvas.cpp
    src/Ray/Ray.cpp
    src/Matrix/Matrix.cpp
    src/Intersection/Intersection.cpp
    src/Intersection/Intersections.cpp
    src/Shape/Shape.cpp
    src/Shape/Sphere.cpp
    src/Shape/Plane.cpp
    src/Scene/Scene.cpp
    src/Camera/Camera.cpp
    src/RayTracer/RayTracer.cpp
    src/Material/Material.cpp
)

# Create a library for shared code
add_library(raytracer_lib ${LIB_SOURCES})
target_include_directories(raytracer_lib PUBLIC 
    ${CMAKE_SOURCE_DIR}/src
)

# Link library to main executable
target_link_libraries(raytracer raytracer_lib)

# Transformation demo executable
add_executable(transformation_demo src/TransformationDemo.cpp)
target_link_libraries(transformation_demo raytracer_lib)
target_include_directories(transformation_demo PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Lighting demo executable (Kapitel 7: Beleuchtung)
add_executable(raytracer_lighting src/MainLighting.cpp)
target_link_libraries(raytracer_lighting raytracer_lib)
target_include_directories(raytracer_lighting PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Light sources demo executable (Kapitel 9: Lichtquellen)
add_executable(raytracer_lightsources src/MainLightSources.cpp)
target_link_libraries(raytracer_lightsources raytracer_lib)
target_include_directories(raytracer_lightsources PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Shadows demo executable (Kapitel 10: Schattenberechnung)
add_executable(raytracer_shadows src/MainShadows.cpp)
target_link_libraries(raytracer_shadows raytracer_lib)
target_include_directories(raytracer_shadows PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Reflection demo executable (Kapitel 11: Reflexion)
add_executable(raytracer_reflection src/MainReflection.cpp)
target_link_libraries(raytracer_reflection raytracer_lib)
target_include_directories(raytracer_reflection PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Test simple lighting executable
add_executable(test_simple_lighting src/TestSimpleLighting.cpp)
target_link_libraries(test_simple_lighting raytracer_lib)
target_include_directories(test_simple_lighting PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Test max light executable
add_executable(test_max_light src/TestMaxLight.cpp)
target_link_libraries(test_max_light raytracer_lib)
target_include_directories(test_max_light PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Reflection debug executable
add_executable(test_reflection_debug src/TestReflectionDebug.cpp)
target_link_libraries(test_reflection_debug raytracer_lib)
target_include_directories(test_reflection_debug PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

# Tests (disabled by default to avoid network fetch for googletest)
if(ENABLE_TESTS)
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

    # Test files
    set(TEST_SOURCES
        tests/TestMain.cpp
        tests/PointTests.cpp
        tests/VectorTests.cpp
        tests/ColorTests.cpp
        tests/MatrixTests.cpp
        tests/IntersectionTests.cpp
        tests/SphereTests.cpp
        tests/SceneTests.cpp
        tests/CameraTests.cpp
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
endif()
# Add tests
include(GoogleTest)
if(ENABLE_TESTS)
gtest_discover_tests(raytracer_tests)
endif()

# ============================================================================
# AZTest-Cpp Framework Integration
# ============================================================================

# Add AZTest library
add_subdirectory(libs/AZTest-Cpp)

# Add AZTest tests subdirectory
add_subdirectory(tests_aztest)
