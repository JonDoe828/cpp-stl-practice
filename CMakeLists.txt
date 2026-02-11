cmake_minimum_required(VERSION 3.16)
project(cpp_stl_parctice LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.4
)

FetchContent_MakeAvailable(Catch2)

# 找到所有 *.test.cpp
file(GLOB_RECURSE TEST_FILES CONFIGURE_DEPENDS "*.test.cpp")

foreach(test_file ${TEST_FILES})
  # 取文件名作为可执行文件名
  get_filename_component(test_name ${test_file} NAME_WE)

  add_executable(${test_name} ${test_file})

  target_link_libraries(${test_name}
    PRIVATE Catch2::Catch2WithMain
  )

  target_include_directories(${test_name}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
  )
endforeach()

# === add app target (just for main) ===
add_executable(app list/list.cpp)
target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/list)

