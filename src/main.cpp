#include <glad/glad.h>  // Include the glad header
#include <GLFW/glfw3.h> // Include the GLFW header

#include <iostream>
#include <fstream>
#include <string>




// globally saying off this thing what ever this code is not sure yet tho

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";


// fagment shader content
const char *fragmentShaderSource = "#version 330 core\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                    "}\0";

// vertices for the traiangles (now 2 are there !)

float vertices[] = {
// first triangle
0.5f, 0.5f, 0.0f, // top right
0.5f, -0.5f, 0.0f, // bottom right
-0.5f, 0.5f, 0.0f, // top left
// second triangle
0.5f, -0.5f, 0.0f, // bottom right
-0.5f, -0.5f, 0.0f, // bottom left
-0.5f, 0.5f, 0.0f // top left
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

std::string glslreader(const std::string & filepath) {
    std::ifstream file;
    file.open(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return "";
    }
    
    std::string content;
    std::string line;
    
    while(getline(file, line)){
        content += line + "\n";
    }
    content += "\0";

    file.close();
    return content;
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


    /* 
    
    -------- SO THIS WAS WHAT WAS INTRODUCED PRIOR TO THE VAO I THOUGHT WE SHD INITIATE THE VBO BEFORE 
    THE SHADER COMPILATION BUT I WAS WRONG SO I COMMENTED IT OUT AND MOVED IT TO THE BOTTOM OF THE CODE
    SO THAT IT CAN BE INITIATED AFTER THE SHADER COMPILATION AND LINKING IS DONE --------

    // vertex buffer object (VBO) 
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    */



    // ------- TWO SHADERS VERTEX AND FRAGMENT SHADER -------

    // compiling the vertex shader (vertexshadersource up there see)
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
    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO); // create the vertex array which stores the vertex buffer object
    glGenBuffers(1, &VBO); // create the vertex buffer object (there can be multiple VBOs)

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // vertices are given up in the code
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // vertex attributes are linked
    glEnableVertexAttribArray(0); // enable the vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind the buffer
    glBindVertexArray(0); // unbind the vertex array



    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        glUseProgram(shaderprog);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);



        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up and exit
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}