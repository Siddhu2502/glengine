#include <glad/glad.h>  // Include the glad header
#include <GLFW/glfw3.h> // Include the GLFW header

#include "Shader.h++"
#include "Util.h++"

#include <string>
#include <iostream>

#include <stb_image/stb_image.h>

// float vertices[] = {
//     // positions            // colors
//     0.5f, -0.5f, 0.0f,      1.0f, 0.0f, 0.0f, // bottom right
//     -0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f, // bottom left
//     0.0f, 0.5f, 0.0f,       0.0f, 0.0f, 1.0f // top
// };

// int indices[] = {
//     0, 1, 2
// };

// vertices for the traiangles (now 2 are there !)
float vertices[] = {
    // positions          // colors           // texture coords
    // Center point
     0.0f,  0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.5f, 0.5f, // 0: Center (white)
    // Hexagon vertices (clockwise from top)
     0.0f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.5f, 1.0f, // 1: Top (red)
     0.43f,  0.25f, 0.0f, 0.0f, 1.0f, 0.0f,   0.93f, 0.75f, // 2: Top-right (green)
     0.43f, -0.25f, 0.0f, 0.0f, 0.0f, 1.0f,   0.93f, 0.25f, // 3: Bottom-right (blue)
     0.0f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.5f, 0.0f, // 4: Bottom (yellow)
    -0.43f, -0.25f, 0.0f, 0.0f, 1.0f, 1.0f,   0.07f, 0.25f, // 5: Bottom-left (cyan)
    -0.43f,  0.25f, 0.0f, 1.0f, 0.0f, 1.0f,   0.07f, 0.75f, // 6: Top-left (magenta)
};

int indices[] = {
    // Triangles to form the hexagon
    0, 1, 2, // Center, Top, Top-right
    0, 2, 3, // Center, Top-right, Bottom-right
    0, 3, 4, // Center, Bottom-right, Bottom
    0, 4, 5, // Center, Bottom, Bottom-left
    0, 5, 6, // Center, Bottom-left, Top-left
    0, 6, 1, // Center, Top-left, Top
};


int main() {

    // ---------- BOILER PLATE ----------

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW: Set OpenGL version to 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creating the Window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OPENGL TAB", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load all the OpenGL functions using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // ---------- END OF BOILER PLATE ----------


    Shader shaderprog((getExecutableDir() + "/shaders/vs/vertthing.vert").c_str(), (getExecutableDir() + "/shaders/fs/fragthing.frag").c_str());


    // vertex array object (VAO) AND vertex buffer object (VBO)
    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO); // create the vertex array which stores the vertex buffer object
    glGenBuffers(1, &VBO); // create the vertex buffer object (there can be multiple VBOs)
    glGenBuffers(1, &EBO); // create the element buffer object (EBO)

    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // vertices are given up in the code

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // linking the vertex attributes (positions and colors)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // vertex attributes are linked
    glEnableVertexAttribArray(0); // enable the vertex attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // vertex attributes are linked
    glEnableVertexAttribArray(1); // enable the vertex attributes


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
    unsigned char * data = stbi_load((getExecutableDir() + "/images/stoneimage.png").c_str(), &width, &height, &nrChannels, 0);

    if (data == nullptr) {
        std::cerr << "Failed to load texture" << std::endl;
        return -1;
    }else{
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // vertex attributes are linked
    glEnableVertexAttribArray(2); // enable the vertex attributes




    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // linking or using the shader program
        shaderprog.initialize();

        // render the hexagon
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Delete the VAO, VBO, EBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}