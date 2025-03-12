cmake_minimum_required(VERSION 3.10)

project(app VERSION 0.1)

include(FetchContent)

# Add the include directory for glad.h
include_directories(include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)


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
target_link_libraries(app PRIVATE glfw)
target_link_libraries(app PRIVATE glm::glm)

# Copy shaders to build directory
file(COPY ${CMAKE_SOURCE_DIR}/src/shaders/fs DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/src/shaders/vs DESTINATION ${CMAKE_BINARY_DIR})

# copy images to build directory
file(COPY ${CMAKE_SOURCE_DIR}/src/images DESTINATION ${CMAKE_BINARY_DIR})