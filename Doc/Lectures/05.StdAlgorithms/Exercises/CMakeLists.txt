cmake_minimum_required(VERSION 3.10)
project(StdAlgorithms)

set(CMAKE_BUILD_TYPE Debug)

include_directories(${PROJECT_SOURCE_DIR})

find_package(Threads)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-D_GLIBCXX_CONCEPT_CHECKS)

function(make_exec_for_each_source)
  foreach(f ${ARGN})
    file(RELATIVE_PATH filename ${PROJECT_SOURCE_DIR} ${f})
    set(exec_name ${filename}.out)
    add_executable(${exec_name} ${filename})
    target_link_libraries(${exec_name} ${CMAKE_THREAD_LIBS_INIT})
  endforeach()
endfunction(make_exec_for_each_source)

file(GLOB SRCS ${PROJECT_SOURCE_DIR}/*.cpp)
make_exec_for_each_source(${SRCS})


