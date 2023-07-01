add_library(${PROJECT_NAME} INTERFACE 
    vector.h
    matrix.h
    Types/row.h
    Types/column.h
    Types/BasicStrongType_Functionalities.h
    Types/BasicStrongType.h
)
target_sources(${PROJECT_NAME} PRIVATE ${TARGET_SRC})
target_include_directories(${PROJECT_NAME} INTERFACE .)