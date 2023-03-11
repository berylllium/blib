cmake_minimum_required(VERSION 3.21)

project(blib C)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

set(BLIB_HEADERS
	container/darray.h
	container/hashmap.h
)
list(TRANSFORM BLIB_HEADERS PREPEND "include/")

set(BLIB_SOURCES
	container/darray.c
	container/hashmap.c
)
list(TRANSFORM BLIB_SOURCES PREPEND "src/")

add_library(blib SHARED ${BLIB_SOURCES} ${BLIB_HEADERS})
target_include_directories(blib PUBLIC include/)

# Don't add the test subdirectory if not in the top level.
if(PROJECT_IS_TOP_LEVEL)
	add_subdirectory(test)
endif()
