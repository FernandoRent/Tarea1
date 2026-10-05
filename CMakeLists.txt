cmake_minimum_required(VERSION 3.16)
project(Tarea1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_library(structures INTERFACE)
target_include_directories(structures INTERFACE include)
add_executable(demo src/main.cpp)
target_link_libraries(demo PRIVATE structures)
if(MSVC)
target_compile_options(demo PRIVATE /W4)
else()
target_compile_options(demo PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Pruebas con GoogleTest (se descarga solo la primera vez que se configura)
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests
    tests/test_stack.cpp
    tests/test_queue.cpp
    tests/test_dictionary.cpp
)
target_link_libraries(tests PRIVATE structures GTest::gtest_main)
if(MSVC)
target_compile_options(tests PRIVATE /W4)
else()
target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic)
endif()
include(GoogleTest)
gtest_discover_tests(tests)

# Tarea 2: generador de cuadruplos con Flex/Bison (reutiliza structures)
add_subdirectory(Tarea2)
