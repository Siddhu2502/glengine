cmake_minimum_required(VERSION 3.10)

project(app VERSION 0.1)

# Add the include directory for glad.h
include_directories(include)

# Add glad.c as a source file
add_executable(app src/main.cpp src/glad.c)

# Link GLFW to the executable
target_link_libraries(app glfw)

# Copy shaders to build directory
file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_BINARY_DIR})