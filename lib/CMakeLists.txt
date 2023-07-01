include(FetchContent)

if(MATRIX_INCLUDE_TESTS)
  # ------------- GoogleTest --------------

  set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
  set(BUILD_GTEST ON CACHE BOOL "" FORCE)
  
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.11.0
  )

  FetchContent_MakeAvailable(googletest)
endif()