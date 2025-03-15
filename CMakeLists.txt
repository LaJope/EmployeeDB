cmake_minimum_required(VERSION 3.30.0)

project(PTMK VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_BUILD_PARALLEL_LEVEL 16)
set (CMAKE_CXX_STANDARD 20)

#include(vcpkg)
#set(CMAKE_TOOLCHAIN_FILE "$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake")

#include_directories(${CMAKE_SOURCE_DIR}/)

add_subdirectory(Main)

add_custom_target(${PROJECT_NAME} ALL)
add_dependencies(${PROJECT_NAME} Main)

