/**
 * @file Shader.cpp
 * @brief Code for the Shader class, which is the wizard behind the scenes, defining how things are rendered on screen!
 *
 * This file is all about creating and managing Shader Programs in OpenGL.
 * Think of a Shader Program as a set of instructions written in GLSL (OpenGL Shading Language) that tells your graphics card
 * exactly how to process vertices and pixels to draw your 3D objects.
 * This class handles reading shader code from files, compiling them, linking them into a program, and providing ways to set shader parameters (uniforms).
 * Shaders are the heart of modern OpenGL rendering – they control everything from colors and lighting to textures and special effects!
 */

 #include <Shader.h>

 /**
  * @brief Constructor for the Shader class. The "Shader Chef" – prepares and compiles shader recipes!
  *
  * When you create a Shader object, this constructor is like the master chef getting ready to cook up a shader program.
  * It takes the file paths for your vertex and fragment shader code, reads them, compiles them (like checking if the recipe makes sense),
  * and then links them together into a complete shader program that your GPU can execute.
  *
  * @param vertexPath   The file path to the vertex shader source code file (e.g., "shaders/vertex_shader.glsl").
  *                     Think of the vertex shader as the recipe for processing the *geometry* of your 3D models – their vertices (corners).
  * @param fragmentPath The file path to the fragment shader source code file (e.g., "shaders/fragment_shader.glsl").
  *                     Think of the fragment shader as the recipe for determining the *color* of each pixel (fragment) that makes up your 3D models.
  *
  * @note This constructor does the following key steps:
  *       - **Reads Shader Code from Files:** Loads the GLSL code from the specified vertex and fragment shader files.
  *       - **Creates Shader Objects:**  Asks OpenGL to create empty shader objects for both vertex and fragment shaders.
  *       - **Compiles Shader Code:**  Takes the GLSL code and compiles it. If there are errors in your shader code (syntax errors, etc.), compilation will fail, and errors will be reported.
  *       - **Links Shaders into a Program:** After successful compilation, it links the vertex and fragment shaders together into a single Shader Program. This program is what you'll actually use to draw things.
  *       - **Error Handling:** Throughout the process, it includes error checking. If file reading, compilation, or linking fails, it prints detailed error messages to the console, helping you debug your shaders.
  *       - **Deletes Shader Objects (after linking):** Once the shaders are linked into a program, the individual shader objects (vertex and fragment shaders) are no longer needed and are deleted to free up resources. The program itself contains the compiled shader code.
  */
 Shader::Shader(const char* vertexPath, const char* fragmentPath) {

     // ====[ 1. Reading Shader Files - "Getting the Shader Recipes" ]====

     std::string vertexCode;   // To store vertex shader code read from file
     std::string fragmentCode; // To store fragment shader code read from file
     std::ifstream vShaderFile;  // Input file stream for vertex shader file
     std::ifstream fShaderFile;  // Input file stream for fragment shader file

     // Ensure ifstream objects can throw exceptions if something goes wrong during file operations.
     vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit); // Set exception mask for vertex shader file stream
     fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit); // Set exception mask for fragment shader file stream

     try { // Try to read shader files, catch any file reading errors

         // Open shader files
         vShaderFile.open(vertexPath);   // Open the vertex shader file
         fShaderFile.open(fragmentPath);   // Open the fragment shader file
         std::stringstream vShaderStream, fShaderStream; // String streams to read file content

         // Efficiently read file buffer contents into string streams
         vShaderStream << vShaderFile.rdbuf();    // Read vertex shader file buffer into vertex shader string stream
         fShaderStream << fShaderFile.rdbuf();    // Read fragment shader file buffer into fragment shader string stream

         // Convert string streams into actual string code
         vertexCode = vShaderStream.str();    // Get vertex shader code string from string stream
         fragmentCode = fShaderStream.str();  // Get fragment shader code string from string stream
     }
     catch (std::ifstream::failure e) { // Catch any file stream exceptions (e.g., file not found, read error)
         std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl; // Print error message to console if file reading failed.
     }

     // Convert shader code strings to C-style character arrays (const char*) for OpenGL functions.
     const char* vShaderCode = vertexCode.c_str();   // Get C-style string for vertex shader code
     const char* fShaderCode = fragmentCode.c_str(); // Get C-style string for fragment shader code

     // ====[ 2. Compiling the Shaders - "Cooking the Shader Recipes" ]====

     unsigned int vertexShader, fragmentShader; // OpenGL shader object IDs
     int success_vertex, success_fragment;     // Compilation success flags (0 for fail, 1 for success)
     char LOG_IF_ERROR[512];                    // Character array to store error/info logs during compilation/linking

     // ======[ Vertex Shader Compilation - "Cooking the Vertex Recipe" ]======
     vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create an empty vertex shader object in OpenGL and get its ID.

     glShaderSource(vertexShader, 1, &vShaderCode, NULL); // Load the vertex shader source code into the shader object.
     // - vertexShader: Shader object ID.
     // - 1: Number of source code strings (we have one).
     // - &vShaderCode: Pointer to the source code string.
     // - NULL: Lengths of each string (NULL means strings are null-terminated).
     glCompileShader(vertexShader); // Compile the vertex shader code. OpenGL now tries to understand and optimize your vertex shader instructions.

     // Check for compilation errors
     glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success_vertex); // Get compilation status.
     // - vertexShader: Shader object ID.
     // - GL_COMPILE_STATUS: Parameter to query (we want compilation status).
     // - &success_vertex: Integer to store the result (GL_TRUE or GL_FALSE).

     if (!success_vertex) { // If vertex shader compilation failed (success_vertex is GL_FALSE)
         glGetShaderInfoLog(vertexShader, 512, NULL, LOG_IF_ERROR); // Get the compilation error log.
         // - vertexShader: Shader object ID.
         // - 512: Maximum size of the info log buffer.
         // - NULL: Pointer to write the length of the log (NULL means don't need length).
         // - LOG_IF_ERROR: Character array to store the error log.
         std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << LOG_IF_ERROR << std::endl; // Print the error log to the console.
     }
     // -----------  End of Vertex Shader Compilation ---------------------------------


     // ======[ Fragment Shader Compilation - "Cooking the Fragment Recipe" ]======
     fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // Create an empty fragment shader object in OpenGL and get its ID.

     glShaderSource(fragmentShader, 1, &fShaderCode, NULL); // Load the fragment shader source code.
     glCompileShader(fragmentShader); // Compile the fragment shader code.

     // Check for compilation errors (similar to vertex shader error checking)
     glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success_fragment);
     if (!success_fragment) {
         glGetShaderInfoLog(fragmentShader, 512, NULL, LOG_IF_ERROR);
         std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << LOG_IF_ERROR << std::endl;
     }
     // -----------  End of Fragment Shader Compilation ---------------------------------


     // ====[ 3. Shader Program Linking - "Combining Vertex and Fragment Recipes into a Program" ]====

     int linkersuccess; // Linking success flag
     ID = glCreateProgram(); // Create an empty shader program object in OpenGL and get its ID. This is like creating a container to hold our shaders.

     glAttachShader(ID, vertexShader);   // Attach the compiled vertex shader to the program.
     glAttachShader(ID, fragmentShader); // Attach the compiled fragment shader to the program.
     glLinkProgram(ID);                 // Link the attached shaders into a complete shader program. OpenGL combines them to work together.

     // Check for linking errors
     glGetProgramiv(ID, GL_LINK_STATUS, &linkersuccess); // Get program linking status.
     if (!linkersuccess) { // If program linking failed
         glGetProgramInfoLog(ID, 512, NULL, LOG_IF_ERROR); // Get the linking error log.
         std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << LOG_IF_ERROR << std::endl; // Print linking error log.
     }

     // ====[ 4. Shader Cleanup - "Cleaning up the Individual Shader Recipes (Ingredients are now in the Program)" ]====

     // Delete the individual shader objects. Once linked into the program, they are no longer needed separately.
     glDeleteShader(vertexShader);   // Delete vertex shader object.
     glDeleteShader(fragmentShader); // Delete fragment shader object.
 }

 /**
  * @brief Activates (uses) the shader program. "Use this Shader Recipe for Drawing!"
  *
  * Before you can use a shader program to render, you need to "use" it. This function tells OpenGL to use this specific shader program (the one represented by this Shader object) for subsequent drawing commands.
  * It's like selecting a particular set of brushes and paints that you want to use for your next artwork.
  */
 void Shader::initialize() {
     glUseProgram(ID); // Activate the shader program.  All subsequent rendering calls will use this program until another program is used.
 }

 /**
  * @brief Sets a boolean uniform value in the shader. "Set a Shader Parameter (Boolean Type)"
  *
  * Uniforms are variables that you can set in your shader program from your C++ code. They are "uniform" because their value is the same for all vertices and fragments in a single draw call (unless you change it for the next draw call).
  * This function sets a boolean (true/false) uniform variable in your shader.
  *
  * @param name  The name of the uniform variable in your shader (must match exactly).
  * @param value The boolean value to set for the uniform.
  */
 void Shader::setBool(const std::string &name, bool value) const {
     glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); // Set a boolean uniform.
     // - glGetUniformLocation:  Gets the location (index) of the uniform variable in the shader program based on its name.
     // - ID: Shader program ID.
     // - name.c_str():  Name of the uniform variable (converted to C-style string).
     // - glUniform1i: Sets an integer uniform value.  OpenGL represents booleans as integers (0 for false, 1 for true).
     // - (int)value: Cast the boolean 'value' to integer (0 or 1).
 }

 /**
  * @brief Sets an integer uniform value in the shader. "Set a Shader Parameter (Integer Type)"
  *
  * Similar to `setBool`, but this function sets an integer uniform variable in your shader.
  *
  * @param name  The name of the integer uniform variable in your shader.
  * @param value The integer value to set.
  */
 void Shader::setInt(const std::string &name, int value) const {
     glUniform1i(glGetUniformLocation(ID, name.c_str()), value); // Set an integer uniform using glUniform1i.
 }

 /**
  * @brief Sets a floating-point uniform value in the shader. "Set a Shader Parameter (Float Type)"
  *
  * And just like the others, this sets a floating-point uniform variable in your shader. Floats are commonly used for colors, positions, and many other shader parameters.
  *
  * @param name  The name of the float uniform variable in your shader.
  * @param value The floating-point value to set.
  */
 void Shader::setFloat(const std::string &name, float value) const {
     glUniform1f(glGetUniformLocation(ID, name.c_str()), value); // Set a float uniform using glUniform1f.
 }


 // Destructor -  In this simple Shader class, explicit destructor logic isn't strictly necessary for resource cleanup.
 //              OpenGL objects (like shader programs) are usually managed by the OpenGL context and cleaned up when the context is destroyed.
 //              In more complex scenarios, if you were managing resources like dynamically allocated memory within the Shader class, you might need a destructor to free those.
 //              However, for just managing OpenGL shader program IDs, the automatic resource management by OpenGL is generally sufficient.
 //              If you *were* to explicitly delete the program (though not needed in this basic example), you'd do something like:
 //              `glDeleteProgram(ID);`  inside a `Shader::~Shader()` destructor.