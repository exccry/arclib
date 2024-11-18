cmake_minimum_required(VERSION 3.10)
project(arclib C)

set(CMAKE_C_STANDARD 17)

include_directories(include)

file(GLOB TP_SOURCES
  src/*.c
)

add_library(arclib STATIC ${TP_SOURCES})
set_target_properties(arclib PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)

add_executable(test test/main.c)
target_link_libraries(test arclib)

