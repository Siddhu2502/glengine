cmake_minimum_required(VERSION 3.10)

project(app VERSION 0.1)

# Add the include directory for glad.h
include_directories(include)

# Add glad.c as a source file
add_executable(app 
            src/main.cpp 
            src/glad.c 
            src/Shader.cpp
            src/Window.cpp 
            src/Mesh.cpp
            src/stb_image.cpp
            src/Texture.cpp)

# Link GLFW to the executable
target_link_libraries(app glfw)

# Copy shaders to build directory
file(COPY ${CMAKE_SOURCE_DIR}/src/shaders/fs DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/src/shaders/vs DESTINATION ${CMAKE_BINARY_DIR})

# copy images to build directory
file(COPY ${CMAKE_SOURCE_DIR}/src/images DESTINATION ${CMAKE_BINARY_DIR})