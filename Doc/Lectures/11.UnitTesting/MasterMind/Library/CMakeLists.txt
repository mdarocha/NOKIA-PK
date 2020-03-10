cmake_minimum_required(VERSION 3.10)

project(MasterMindLibrary)

cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SRC_LIST)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(${PROJECT_NAME} ${SRC_LIST})

