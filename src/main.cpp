#include <glad/glad.h>  // Include the glad header
#include <GLFW/glfw3.h> // Include the GLFW header

#include <fstream>
#include <string>
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <cmath>


// vertices for the traiangles (now 2 are there !)
float vertices[] = {
    // Center point
     0.0f,  0.0f, 0.0f, // 0: Center of the hexagon

    // Hexagon vertices (clockwise from top)
     0.0f,  0.5f, 0.0f, // 1: Top
     0.43f,  0.25f, 0.0f, // 2: Top-right
     0.43f, -0.25f, 0.0f, // 3: Bottom-right
     0.0f, -0.5f, 0.0f, // 4: Bottom
    -0.43f, -0.25f, 0.0f, // 5: Bottom-left
    -0.43f,  0.25f, 0.0f, // 6: Top-left
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




// Callback function to handle window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
};

// Process input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void compileshadersuccess(const unsigned int &shader, const std::string & type){
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::"<<type<<"::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void linkershadersuccess(const unsigned int &shader){
    int success;
    char infoLog[512];
    glGetProgramiv(shader, GL_LINK_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

// function to parse the shader file (returns as string) -> give path to the file 
// @todo: make it load like absolute path or something like that [DONE]
std::string parseShader(const std::string &filepath){
    std::string result;
    std::string line;
    std::ifstream file(filepath.c_str());

    if (file.is_open()){
        while (getline(file, line)){
            result += line + "\n";
        }
        file.close();
    } else {
        std::cerr << "ERROR::SHADER::FILE_NOT_READABLE" << std::endl;
    }

    return result;
}

// function to get the executable directory
std::string getExecutableDir() {
    char buffer[1024];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
        return std::filesystem::path(buffer).parent_path().string();
    }
    return "";
}



int main() {


    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    
    // ---------- BOILER PLATE ----------

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



    // ------- TWO SHADERS VERTEX AND FRAGMENT SHADER -------

    // compiling the vertex shader (vertexshadersource up there see)

    // adding the getExecutableDir() to the path of the shader file 
    std::string vertexShaderSourceee = parseShader(getExecutableDir() + "/shaders/vertthing.vert");
    std::string fragmentShaderSourceee = parseShader( getExecutableDir() + "/shaders/fragthing.frag");

    // conversion to const char * 
    // we shd create the std::string and then convert it to const char * because the c_str() method is only available for std::string
    // which means - > we cannot directly chain functions 
    const char * vertexShaderSource = vertexShaderSourceee.c_str();
    const char * fragmentShaderSource = fragmentShaderSourceee.c_str();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    compileshadersuccess(vertexShader, "VERTEX");

    // compiling the fragment shader (fragmentshadersource up there see)
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    compileshadersuccess(fragmentShader, "FRAGMENT");

    // --------------------- SHADER OVER ---------------------



    // --------------- linking shader programs ---------------

    unsigned int shaderprog = glCreateProgram();
    glAttachShader(shaderprog, vertexShader);
    glAttachShader(shaderprog, fragmentShader);
    glLinkProgram(shaderprog);
    linkershadersuccess(shaderprog);

    // --------------- LINKING SHADERS OVER ---------------

    // dont forget to delete the shaders
    glDeleteShader(fragmentShader); 
    glDeleteShader(vertexShader);


    // linking the vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // vertex attributes are linked
    glEnableVertexAttribArray(0); // enable the vertex attributes


    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // linking or using the shader program
        glUseProgram(shaderprog);

        // color change (using the uniform variable in fragment shader)
        float timeValue = glfwGetTime(); // get the time 
        float blueValue = (std::sin(timeValue) / 2.0f) + 0.5f; // add a sin wave to the blue color wrt time
        int vertexColorLocation = glGetUniformLocation(shaderprog, "myColor"); // get the location of the uniform variable
        glUniform4f(vertexColorLocation, 0.0f, 0.0f, blueValue, 1.0f); // set the uniform variable


        // render the hexagon
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

    // Clean up and exit
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}