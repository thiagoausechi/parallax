cmake_minimum_required(VERSION 3.5)
project(parallax)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/src/*.h")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

message("LIBRARIES = ${LIBRARIES}")
