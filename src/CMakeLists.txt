file(GLOB_RECURSE TARGET_SRC "./*.cpp" "./*.hpp" "./*.h")

add_library(${THIS} STATIC ${TARGET_SRC})
target_sources(${This} PRIVATE ${TARGET_SRC})
target_include_directories(${This} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
