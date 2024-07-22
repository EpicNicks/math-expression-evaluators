cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MathExpressionEvaluator C)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect all source files
file(GLOB_RECURSE SOURCES 
    "${PROJECT_SOURCE_DIR}/src/*.c"
)

# Create the executable
add_executable(math_evaluator ${SOURCES})

# Print the collected source files (optional, for debugging)
message(STATUS "Source files: ${SOURCES}")