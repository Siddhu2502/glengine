#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @file Shader.h
 * @brief Header file for the Shader class, which manages OpenGL shader programs.
 *
 * Defines the `Shader` class, responsible for loading, compiling, and using vertex and fragment shaders
 * to create OpenGL shader programs. This class provides an interface to easily manage shaders
 * and set uniform values for rendering.
 */

/**
 * @class Shader
 * @brief Manages an OpenGL Shader Program.
 *
 * The `Shader` class encapsulates an OpenGL shader program, composed of a vertex shader and a fragment shader.
 * It handles the process of reading shader code from files, compiling these shaders, and linking them
 * into a program ready to be used for rendering. It also provides utility functions to set uniform values
 * in the shader program.
 */
class Shader{
public:
    /**
     * @brief ID of the OpenGL Shader Program.
     *
     * This public member variable holds the OpenGL ID (name) of the compiled and linked shader program.
     * This ID is used by OpenGL functions to reference and use this shader program during rendering.
     *
     * @note Users of the `Shader` class typically do not need to interact with this ID directly.
     *       The class methods (`initialize`, `setBool`, `setInt`, `setFloat`) handle the OpenGL operations using this ID internally.
     */
    unsigned int ID;

    /**
     * @brief Constructor for the Shader class. Loads, compiles, and links vertex and fragment shaders.
     *
     * This constructor is responsible for creating an OpenGL shader program from vertex and fragment shader source code files.
     * It performs the following steps:
     *  - Reads the source code of the vertex and fragment shaders from the specified file paths.
     *  - Creates and compiles individual vertex and fragment shader objects.
     *  - Links the compiled shaders into a single shader program.
     *  - Handles error checking for file reading, shader compilation, and program linking, outputting error messages to `std::cerr` if any errors occur.
     *
     * @param vertexPath   Path to the vertex shader source file (e.g., "shaders/vertex.glsl").
     * @param fragmentPath Path to the fragment shader source file (e.g., "shaders/fragment.glsl").
     *
     * @throws std::runtime_error If shader file reading, compilation, or linking fails at any stage. (Though error handling currently uses `std::cerr` within the constructor)
     *
     * @note After successful construction, the shader program is ready to be used by calling `initialize()` and setting uniforms as needed.
     */
    Shader(const char* vertexPath, const char* fragmentPath);

    /**
     * @brief Initializes the shader program for use.
     *
     * This method activates the shader program in OpenGL. Before rendering any geometry using this shader,
     * you must call `initialize()` to tell OpenGL to use this specific shader program for subsequent draw calls.
     *
     * @note Internally, this function calls `glUseProgram(ID)` to set the current active shader program to this Shader object's program ID.
     */
    void initialize();

    /**
     * @brief Sets a boolean uniform value in the shader program.
     *
     * This function sets a uniform variable of boolean type in the shader program. Uniforms are used to pass
     * data from the application to the shaders. This method is for setting boolean (true/false) values, which are
     * represented as integers (1 for true, 0 for false) in OpenGL shaders.
     *
     * @param name  The name of the uniform variable in the shader (as declared in the GLSL code).
     * @param value The boolean value to set for the uniform.
     *
     * @note The uniform variable must be declared in both the vertex and fragment shaders to be accessible.
     * @note If the uniform is not found in the shader program, this call might have no effect or could result in an OpenGL error (depending on OpenGL version and error handling).
     */
    void setBool(const std::string &name, bool value) const;

    /**
     * @brief Sets an integer uniform value in the shader program.
     *
     * Sets a uniform variable of integer type in the shader program. This is used to pass integer values to shaders,
     * for example, to control indexing, counters, or flags in the shader logic.
     *
     * @param name  The name of the integer uniform variable in the shader.
     * @param value The integer value to set for the uniform.
     *
     * @note Similar to `setBool`, the uniform variable must be declared in the shader code.
     */
    void setInt(const std::string &name, int value) const;

    /**
     * @brief Sets a floating-point uniform value in the shader program.
     *
     * Sets a uniform variable of floating-point type in the shader program. Floats are commonly used for uniforms
     * representing colors, positions, scaling factors, and many other types of data in shaders.
     *
     * @param name  The name of the float uniform variable in the shader.
     * @param value The floating-point value to set for the uniform.
     *
     * @note Make sure the uniform variable in your shader is declared as `float` (or `vec2`, `vec3`, `vec4` of floats if you intend to set multiple floats at once using other uniform setting methods).
     */
    void setFloat(const std::string &name, float value) const;

};

#endif // SHADER_H