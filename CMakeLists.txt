cmake_minimum_required(VERSION 3.5.1)

project(mqttbroker)

# Setting CMAKE variables
# C++11 Standard
set(CMAKE_CXX_STANDARD 11)
# G++ glags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DELPP_THREAD_SAFE -DELPP_STACKTRACE_ON_CRASH -DELPP_NO_DEFAULT_LOG_FILE")

# Include dirs
include_directories("include/concurrency")
include_directories("include/config")
include_directories("include/logging")
include_directories("include/mqtt")
include_directories("include/tcp")

# Copyng configuration to the build directory
file(COPY "config" DESTINATION ${CMAKE_BINARY_DIR})

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Executable
add_executable(mqttbroker ${SOURCES})

# Linking
set ( PROJECT_LINK_LIBS pthread libconfig++.so )
target_link_libraries(mqttbroker ${PROJECT_LINK_LIBS})
