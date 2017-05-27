cmake_minimum_required(VERSION 3.7)
project(List)

set(CMAKE_C_FLAGS "-std=c99 -Wall -Werror -pedantic-errors -DNDEBUG")

set(SOURCE_FILES
        list_mtm1.h
        list_mtm1.c
        #list.h
        list test_utilities.h
        list-test_1_alex.c)
add_executable(List ${SOURCE_FILES})
#target_link_libraries(List ${CMAKE_SOURCE_DIR}/libmtm.a)