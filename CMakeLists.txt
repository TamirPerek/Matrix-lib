cmake_minimum_required(VERSION 3.2.0)

project(
    Matrix-Lib 
    VERSION 0.1.0
    DESCRIPTION "Small library for a mathematical correct matrix."
    HOMEPAGE_URL "https://github.com/TamirPerek/Matrix-lib"
)

include(GNUInstallDirs)

option(MATRIX_INCLUDE_TESTS "Enables unit tests with googletest" OFF)

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

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "Matrix-LibConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION
    ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

install(
    EXPORT ${PROJECT_NAME}_Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

install(
    FILES 
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/src DESTINATION include)