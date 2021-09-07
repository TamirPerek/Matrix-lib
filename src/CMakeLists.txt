file(GLOB_RECURSE TARGET_SRC "./*.cpp" "./*.hpp" "./*.h")

add_library(${THIS} STATIC ${TARGET_SRC})
