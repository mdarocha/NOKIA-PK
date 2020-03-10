cmake_minimum_required(VERSION 3.10)
project(MasterMindApplication)

project(MasterMindTests)

# set (gtest_disable_pthreads on)

# if (CMAKE_VERSION VERSION_LESS 3.2)
#    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
# else()
#    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
# endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../DownloadProject.cmake)
download_project(PROJ                googletest
                 GIT_REPOSITORY      https://github.com/google/googletest.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

include_directories("${gtest_SOURCE_DIR}/include"
                    "${gmock_SOURCE_DIR}/include")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Library)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


aux_source_directory(. SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} MasterMindTextPresentation)
target_link_libraries(${PROJECT_NAME} MasterMindLibrary)
target_link_libraries(${PROJECT_NAME} gmock_main)
