cmake_minimum_required(VERSION 3.9.2)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++11-narrowing -std=c++11 -Wno-deprecated-declarations -Wno-write-strings -Wno-deprecated-register -g")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(JNI REQUIRED)
include_directories(${JAVA_INCLUDE_PATH})
include_directories(${JAVA_INCLUDE_PATH2})

find_path(V8_PATH include/v8.h)
include_directories(${V8_PATH}/include)
link_directories(${V8_PATH}/lib)

add_subdirectory(openrasp_v8)

add_library(openrasp_js_engine SHARED com_baidu_openrasp_plugin_v8_V8.cc utils.cc context.cc)
    
target_link_libraries(openrasp_js_engine openrasp_v8 v8_monolith)  
