cmake_minimum_required(VERSION 3.5.1)

project(mqttbroker)

set (CMAKE_CXX_STANDARD 11)

include_directories("include/concurrency")
include_directories("include/logging")
include_directories("include/mqtt")
include_directories("include/tcp")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(mqttbroker ${SOURCES})

target_link_libraries(mqttbroker pthread)
