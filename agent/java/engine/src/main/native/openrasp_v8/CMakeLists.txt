cmake_minimum_required(VERSION 3.13)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++11-narrowing -std=c++11 -Wno-deprecated-declarations -Wno-write-strings -Wno-deprecated-register -g")

set(CMAKE_CXX_STANDARD 11)

find_path(V8_PATH include/v8.h)
include_directories(${V8_PATH}/include)
link_directories(${V8_PATH}/lib)

aux_source_directory(. DIR_SRCS)

add_library(openrasp_v8 ${DIR_SRCS})

target_link_libraries(openrasp_v8 v8_monolith)       