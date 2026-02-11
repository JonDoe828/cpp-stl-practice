cmake_minimum_required(VERSION 3.16)

project(cpp_stl_practice LANGUAGES CXX)

# ===== C++ Standard =====
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ===== Build type default (optional) =====
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ===== Catch2 (FetchContent) =====
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.4
)

FetchContent_MakeAvailable(Catch2)

# ===== Enable CTest =====
include(CTest)
enable_testing()

# ===== Collect all *.test.cpp recursively =====
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/*.test.cpp"
)

# Aggregate build target for all tests
add_custom_target(tests)

# Create one executable per test file
foreach(test_file IN LISTS TEST_FILES)
  # e.g. /path/to/vector.test.cpp -> vector.test
  get_filename_component(test_name_we "${test_file}" NAME_WE)

  # Make a safe CMake target name: replace '.' '-' with '_' etc.
  set(test_target "${test_name_we}")
  string(REPLACE "." "_" test_target "${test_target}")
  string(REPLACE "-" "_" test_target "${test_target}")
  string(REPLACE " " "_" test_target "${test_target}")

  # Optional: prefix all test targets to avoid name collisions
  # e.g. vector_test
  set(test_target "${test_target}")

  add_executable("${test_target}" "${test_file}")

  target_link_libraries("${test_target}"
    PRIVATE
      Catch2::Catch2WithMain
  )

  # If your tests include headers from project root / include/ etc.
  target_include_directories("${test_target}"
    PRIVATE
      "${CMAKE_SOURCE_DIR}"
      "${CMAKE_SOURCE_DIR}/include"
  )

  # Warnings
  if(MSVC)
    target_compile_options("${test_target}" PRIVATE /W4)
  else()
    target_compile_options("${test_target}" PRIVATE -Wall -Wextra -Wpedantic)
  endif()

  # Make `cmake --build build --target tests` build all tests
  add_dependencies(tests "${test_target}")

  # Register with ctest (so `ctest` runs them)
  add_test(NAME "${test_target}" COMMAND "${test_target}")
endforeach()

