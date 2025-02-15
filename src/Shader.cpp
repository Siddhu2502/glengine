#include <Shader.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {

    // getting the file paths of vertex and fragment shaders
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        // my code not good enough (GPT told)
        // vertexCode = vShaderFile.rdbuf()->str();
        // fragmentCode = fShaderFile.rdbuf()->str();

        // efficient way to read the file (GPT GAVE)
        vertexCode = std::string((std::istreambuf_iterator<char>(vShaderFile)), std::istreambuf_iterator<char>());
        fragmentCode = std::string((std::istreambuf_iterator<char>(fShaderFile)), std::istreambuf_iterator<char>());
    }  
    catch (std::ifstream::failure e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // compiling the shaders
    unsigned int vertexShader, fragmentShader;
    int success_vertex, success_fragment;
    char LOG_IF_ERROR[512];

    // compiling vertex shader part
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);

    // glGetShaderiv -> https://registry.khronos.org/OpenGL-Refpages/es2.0/xhtml/glGetShaderiv.xml
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success_vertex);

    if (!success_vertex) {
        glGetShaderInfoLog(vertexShader, 512, NULL, LOG_IF_ERROR);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << LOG_IF_ERROR << std::endl;
    }
    // -----------  end ---------------------------------


    // compiling fragment shader part
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);

    // glGetShaderiv -> https://registry.khronos.org/OpenGL-Refpages/es2.0/xhtml/glGetShaderiv.xml
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success_fragment);

    if (!success_fragment) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, LOG_IF_ERROR);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << LOG_IF_ERROR << std::endl;
    }
    // -----------  end ---------------------------------


    int linkersuccess;

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &linkersuccess);

    if (!linkersuccess) {
        glGetProgramInfoLog(ID, 512, NULL, LOG_IF_ERROR);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << LOG_IF_ERROR << std::endl;
    }

    // delete the shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


}

void Shader::initialize() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}