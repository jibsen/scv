cmake_minimum_required(VERSION 2.6)

project(scv C)

option(BUILD_TEST "Build unit test" ON)

include_directories(${PROJECT_SOURCE_DIR})

add_library(scv scv.c scv.h)

if(BUILD_TEST)
	add_executable(test_scv test/test_scv.c test/greatest.h scv.h)
	target_link_libraries(test_scv scv)

	enable_testing()
	add_test(test_scv test_scv)
endif()
