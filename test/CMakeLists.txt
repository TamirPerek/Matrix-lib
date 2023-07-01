set(THIS Matrix-Lib-Tests)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


add_executable(${THIS} 
    MatrixTest.cpp
    VectorTest.cpp
)

target_link_libraries(${THIS}
                        gtest_main
                        Matrix-Lib
)

add_test(
    NAME ${THIS}
    COMMAND ${THIS}
)