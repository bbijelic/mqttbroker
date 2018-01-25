cmake_minimum_required(VERSION 3.5.1)

project(mqttbroker)

# Setting CMAKE variables
# C++11 Standard
set(CMAKE_CXX_STANDARD 11)
# G++ glags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DELPP_THREAD_SAFE -DELPP_STACKTRACE_ON_CRASH -DELPP_NO_DEFAULT_LOG_FILE")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# Include dirs
include_directories("include")

# Copyng configuration to the build directory
file(COPY "config" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Executable
add_executable(mqttbroker ${SOURCES})

# Linking
set ( PROJECT_LINK_LIBS pthread libconfig++.so ssl crypto )
target_link_libraries(mqttbroker ${PROJECT_LINK_LIBS})
