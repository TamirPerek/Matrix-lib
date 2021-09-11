cmake_minimum_required(VERSION 3.20.0)

set(THIS Matrix)

project(${THIS} VERSION 0.1.0)

enable_testing()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


add_subdirectory(src)
add_subdirectory(lib)

set(CPACK_PROJECT_NAME ${THIS})
set(CPACK_PROJECT_VERSION ${THIS})
include(CPack)

add_subdirectory(test)