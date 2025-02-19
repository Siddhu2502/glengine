#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <string>
#include <vector>
#include <iostream>

#include "Window.h"
#include "Shader.h"
#include "Utils.h"
#include "Mesh.h"

// float vertices[] = {
//     // positions            // colors
//     0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f, // bottom right
//     -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, // bottom left
//     0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f // top
// };

// int indices[] = {
//     0, 1, 2
// };


int main()
{

    // vertices for the traiangles (now 2 are there !)
    std::vector<float> vertices = {
        // positions          // colors           // texture coords
        // Center point
        0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f,       // 0: Center (white)
                                                              // Hexagon vertices (clockwise from top)
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 1.0f,       // 1: Top (red)
        0.43f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 0.93f, 0.75f,   // 2: Top-right (green)
        0.43f, -0.25f, 0.0f, 0.0f, 0.0f, 1.0f, 0.93f, 0.25f,  // 3: Bottom-right (blue)
        0.0f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.5f, 0.0f,      // 4: Bottom (yellow)
        -0.43f, -0.25f, 0.0f, 0.0f, 1.0f, 1.0f, 0.07f, 0.25f, // 5: Bottom-left (cyan)
        -0.43f, 0.25f, 0.0f, 1.0f, 0.0f, 1.0f, 0.07f, 0.75f,  // 6: Top-left (magenta)
    };

    std::vector<unsigned int> indices = {
        // Triangles to form the hexagon
        // 0, 1, 2, // Center, Top, Top-right
        // 0, 2, 3, // Center, Top-right, Bottom-right
        // 0, 3, 4, // Center, Bottom-right, Bottom
        // 0, 4, 5, // Center, Bottom, Bottom-left
        // 0, 5, 6, // Center, Bottom-left, Top-left
        // 0, 6, 1, // Center, Top-left, Top

        0, 1, 2, 3, 4, 5, 6, 1 // Center, Top, Top-right, Bottom-right, Bottom, Bottom-left, Top-left, Top
    };

    // ---------- Window Initialization ----------
    Window window(800, 600, "Hexagon with Texture");
    
    // ---------- Shader Initialization ----------
    Shader shaderprog((getExecutableDir() + "/vs/vertthing.vert").c_str(), (getExecutableDir() + "/fs/fragthing.frag").c_str());

    // vertex array object (VAO) AND vertex buffer object (VBO)
    Mesh hexagonMesh(vertices, indices);

    // ------------------ Texture generation --------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // loading the image
    int width, height, nrChannels;
    unsigned char *data = stbi_load((getExecutableDir() + "/images/stoneimage.png").c_str(), &width, &height, &nrChannels, 0);

    if (data == nullptr)
    {
        std::cerr << "Failed to load texture" << std::endl;
        return -1;
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);                                                                  // enable the vertex attributes

    // Render loop
    while (!window.windowShouldClose())
    {
        // Input
        window.processInput();

        // Rendering commands
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // linking or using the shader program
        shaderprog.initialize();

        // render the hexagon
        glBindTexture(GL_TEXTURE_2D, texture);
        hexagonMesh.bind_VAO();

        glDrawElements(GL_TRIANGLE_FAN, hexagonMesh.getIndexCount(), GL_UNSIGNED_INT, 0);
        
        // glDrawElements(GL_TRIANGLE_FAN, hexagonMesh.getIndexCount(), GL_UNSIGNED_INT, 0); -> for drawing the hexagon in efficient way
        hexagonMesh.unbind_VAO();

        // Swap buffers and poll events
        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}