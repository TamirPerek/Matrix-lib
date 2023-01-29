set(THIS MatrixTests)

add_executable(${THIS} 
    MatrixTest.cpp
    VectorTest.cpp
)

target_link_libraries(${THIS}
                        gtest_main
                        Matrix
)

add_test(
    NAME ${THIS}
    COMMAND ${THIS}
)