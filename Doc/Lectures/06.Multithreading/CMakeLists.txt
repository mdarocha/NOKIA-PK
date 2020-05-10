cmake_minimum_required(VERSION 3.10)
project(multithreading)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/..)

set(CMAKE_CXX_STANDARD 17)

FILE(GLOB children RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/*)
SET(dirlist "")
FOREACH(child ${children})
message (${child})
  IF(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${child})
    LIST(APPEND dirlist ${child})
  ENDIF()
ENDFOREACH()
message(${dirlist})

function(make_exec_for_each_source)
  foreach(f ${ARGN})
      message(${PROJECT_SOURCE_DIR}/${f})
      SET(SRC_LIST "")
    aux_source_directory(${PROJECT_SOURCE_DIR}/${f} SRC_LIST)
    message(${SRC_LIST})
    add_executable(${f} ${SRC_LIST})
    target_link_libraries(${f} Threads::Threads)
  endforeach()
endfunction(make_exec_for_each_source)

make_exec_for_each_source(${dirlist})
