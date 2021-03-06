cmake_minimum_required(VERSION 2.8)

project(scv C)

option(BUILD_TEST "Build unit test" ON)
option(BUILD_COVERAGE "Code coverage" OFF)

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

if(MSVC)
  add_definitions(/D_CRT_SECURE_NO_WARNINGS)
  if(NOT MSVC_VERSION LESS 1700)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /analyze")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC)
  set(gcc_warning_flags
    -Wall
    -Wextra
    -pedantic
    -Wshadow
    -Wpointer-arith
    -Wcast-qual
    -Wcast-align
    -Wstrict-prototypes
    -Wmissing-prototypes
  )
  foreach(flag ${gcc_warning_flags})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}")
  endforeach()
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Weverything -pedantic")
endif()

if(BUILD_COVERAGE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 --coverage")
endif()

include_directories(${PROJECT_SOURCE_DIR})

add_library(scv scv.c scv.h)

if(BUILD_TEST)
  add_executable(test_scv test/test_scv.c test/greatest.h scv.h)
  target_link_libraries(test_scv scv)

  enable_testing()
  add_test(test_scv test_scv)
endif()
