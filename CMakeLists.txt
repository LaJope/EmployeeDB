cmake_minimum_required(VERSION 3.20.0)

project(PTMK VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_BUILD_PARALLEL_LEVEL 16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(Main)
add_subdirectory(libpqxx)

add_custom_target(${PROJECT_NAME} ALL)
add_dependencies(${PROJECT_NAME} ptmk pqxx)
