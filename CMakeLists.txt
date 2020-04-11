cmake_minimum_required(VERSION 3.12)
project(CPP_Workshop)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include("${CMAKE_CURRENT_SOURCE_DIR}/internal_commands.cmake")

set(SQLITE_ORM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/sqlite_orm")
option(SqliteOrm_BuildTests OFF)

set_compiler_options()

add_subdirectory(googletest)
add_subdirectory(UE)
add_subdirectory(BTS)
add_subdirectory(COMMON)
