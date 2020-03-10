cmake_minimum_required(VERSION 3.10)

project(MasterMind)

set(CMAKE_CXX_STANDARD 17)

# parts of project
add_subdirectory(Application)
add_subdirectory(TextPresentation)
add_subdirectory(Library)
add_subdirectory(Tests)

