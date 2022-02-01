cmake_minimum_required(VERSION 3.21)

project(scv C)

include(CTest)

add_library(scv scv.c scv.h)
target_include_directories(scv PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)

if(BUILD_TESTING)
  add_executable(test_scv test/test_scv.c)
  target_link_libraries(test_scv PRIVATE scv)

  add_test(test_scv test_scv)
endif()
