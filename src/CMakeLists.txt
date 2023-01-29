add_library(${THIS} INTERFACE 
    vector.h
    matrix.h
    Types/row.h
    Types/column.h
    Types/BasicStrongType_Functionalities.h
    Types/BasicStrongType.h
)
target_sources(${THIS} PRIVATE ${TARGET_SRC})
target_include_directories(${THIS} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
