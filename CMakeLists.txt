cmake_minimum_required(VERSION 3.30.0)

project(PTMK VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_BUILD_PARALLEL_LEVEL 16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_library(SQLITE_LIBRARY sqlite3)
find_path(SQLITE_INCLUDE_DIR sqlite3.h)

if (NOT SQLITE_LIBRARY OR NOT SQLITE_INCLUDE_DIR)
    message(FATAL_ERROR "sqlite not found. Please install sqlite using:\n"
                        "ubuntu: sudo apt install libsqlite3-dev\n"
                        "arch: sudo pacman -S sqlite\n"
                        "or your prefered package manager")
endif()

set(SQLITECPP_RUN_CPPCHECK OFF CACHE BOOL "" FORCE)
set(SQLITECPP_RUN_CPPLINT OFF CACHE BOOL "" FORCE)

add_subdirectory(Main)
add_subdirectory(SQLiteCpp)

add_custom_target(${PROJECT_NAME} ALL)
add_dependencies(${PROJECT_NAME} ptmk SQLiteCpp)
