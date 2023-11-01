cmake_minimum_required(VERSION 3.16)

project(scv C)

include(CTest)

if(MSVC)
  add_compile_options(/W3)
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -pedantic)
endif()

add_library(scv scv.c scv.h)
target_include_directories(scv PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

if(BUILD_TESTING)
  add_executable(test_scv test/test_scv.c)
  target_link_libraries(test_scv PRIVATE scv)
  if(MSVC)
    target_compile_definitions(test_scv PRIVATE _CRT_SECURE_NO_WARNINGS)
  endif()

  add_test(test_scv test_scv)
endif()
