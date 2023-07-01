cmake_minimum_required(VERSION 3.2.0)

project(MatrixLib VERSION 0.1.0)

option(MATRIX_INCLUDE_TESTS "Enables unit tests with googletest" ON)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(src)
add_subdirectory(lib)

set(CPACK_PROJECT_NAME ${THIS})
set(CPACK_PROJECT_VERSION ${THIS})
include(CPack)

if(NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MATRIX_INCLUDE_TESTS OFF)
endif()

if(MATRIX_INCLUDE_TESTS)
    add_subdirectory(test) 
    enable_testing()
endif()