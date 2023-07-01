set(THIS Matrix-Lib-Tests)

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