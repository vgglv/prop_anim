cmake_minimum_required (VERSION 3.18)
project(prop_anim)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (PROJECT_IS_TOP_LEVEL)
	add_subdirectory(sample)
endif()

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

add_library(prop_anim STATIC ${SOURCES})

target_include_directories(prop_anim PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
