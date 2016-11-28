cmake_minimum_required(VERSION 3.5.1)

project(mqttbroker)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DELPP_THREAD_SAFE -DELPP_STACKTRACE_ON_CRASH -DELPP_NO_DEFAULT_LOG_FILE")

include_directories("include/concurrency")
include_directories("include/logging")
include_directories("include/mqtt")
include_directories("include/tcp")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(mqttbroker ${SOURCES})

target_link_libraries(mqttbroker pthread)
