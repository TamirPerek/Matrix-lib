file(GLOB_RECURSE TARGET_SRC "./*.cpp" "./*.hpp" "./*.c" "./*.h")

add_library(${THIS} INTERFACE ${TARGET_SRC})
target_sources(${THIS} PRIVATE ${TARGET_SRC})
target_include_directories(${THIS} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
