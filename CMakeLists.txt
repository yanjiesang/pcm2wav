cmake_minimum_required(VERSION 3.0.0)

project(pcm2wav)

set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Release)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(
    ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCE_FILE_DIR
)

add_executable(${PROJECT_NAME} ${SOURCE_FILE_DIR})