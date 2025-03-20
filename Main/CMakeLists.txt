project(ptmk LANGUAGES CXX)

set(CMAKE_BUILD_PARALLEL_LEVEL 16)

set(SOURCE_FILES ./Source_Files/main.cpp ./Source_Files/AppSettings.cpp ./Source_Files/Timer.cpp ./Source_Files/EmployeeModel.cpp ./Source_Files/EmployeeGen.cpp ./Source_Files/Logger.cpp ./Source_Files/Application.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_include_directories(${PROJECT_NAME} PRIVATE ./Header_Files/)
target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra)
target_link_libraries(${PROJECT_NAME} pqxx)
