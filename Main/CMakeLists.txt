project(Main)

set(CMAKE_BUILD_PARALLEL_LEVEL 16)

set(SOURCE_FILES ${PROJECT_SOURCE_DIR}/Source_Files/)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_include_directories(${PROJECT_NAME} PRIVATE ./Header_Files/)
target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra)
