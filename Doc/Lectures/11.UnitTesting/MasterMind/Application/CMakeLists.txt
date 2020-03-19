cmake_minimum_required(VERSION 3.10)
project(MasterMindApplication)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Library)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../TextPresentation)

aux_source_directory(. SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} MasterMindTextPresentation)
target_link_libraries(${PROJECT_NAME} MasterMindLibrary)

