/**
 * @file Mesh.cpp
 * @brief Code for the Mesh class, which helps you draw 3D shapes on screen.
 *
 * This file has all the stuff to create and manage a Mesh.
 * A Mesh is like a container for all the info OpenGL needs to draw a shape,
 * like where the points are, how they're connected, colors, and textures.
 */

 #include "Mesh.h"

 /**
  * @brief Constructor for the Mesh class. Sets up a new Mesh object.
  *
  * This is like the "window maker" for your Mesh. It does all the initial setup to
  * get your Mesh ready for drawing.
  *
  * @param vertices  A bunch of numbers (floats) that tell OpenGL where all the points (vertices) of your shape are,
  *                  and also info like colors and texture coordinates for each point.
  *                  Think of it like a list of instructions for each corner of your shape.
  * @param indices   A list of numbers (unsigned ints) that tell OpenGL how to connect the points (vertices)
  *                  to make triangles (or other shapes). It's like saying "connect point A, point B, and point C to make a triangle".
  *
  * @note The vertex data is assumed to be laid out like this for each point:
  *       - Position (3 floats: x, y, z)
  *       - Color    (3 floats: red, green, blue)
  *       - TexCoord (2 floats: texture x, texture y)
  *       So, 8 floats total for each vertex.
  */
 Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
     : indexcount(indices.size()) ///<  We count how many indices there are and save it. This is useful later when we need to draw.
 {
     // ====[ VAO, VBO, EBO -  The Holy Trinity for Drawing ]====
     // VAO: Vertex Array Object -  "Vertex Recipe Card" - remembers how to draw
     // VBO: Vertex Buffer Object  -  "Ingredient Container" - holds the vertex data (points, colors, etc.)
     // EBO: Element Buffer Object -  "Index List"       -  tells OpenGL the order to use points to make triangles
 
     // 1. Make a new "Vertex Recipe Card" (VAO) and get its ID
     glGenVertexArrays(1, &VAO);
     // 2. Make a new "Ingredient Container" (VBO) for vertex data and get its ID
     glGenBuffers(1, &VBO);
     // 3. Make a new "Index List" (EBO) and get its ID
     glGenBuffers(1, &EBO);
 
     // =====[ Get Ready to "Cook" - Binding Time! ]=====
 
     // 4. "Pull out" the "Vertex Recipe Card" (VAO) - make it ACTIVE so we can set it up
     glBindVertexArray(VAO);
 
     // 5. "Select" the "Ingredient Container" (VBO) - make it the active ARRAY_BUFFER (for vertex data)
     glBindBuffer(GL_ARRAY_BUFFER, VBO);
     // 6. "Pour" the raw vertex data (positions, colors, texcoords) from our 'vertices' vector into the VBO container on the GPU
     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), static_cast<const void*>(vertices.data()), GL_STATIC_DRAW);
 
     // 7. "Select" the "Index List" (EBO) - make it the active ELEMENT_ARRAY_BUFFER (for index data)
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     // 8. "Write down" the index order from our 'indices' vector into the EBO Index List on the GPU
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), static_cast<const void*>(indices.data()), GL_STATIC_DRAW);
 
     // =====[ Tell OpenGL the "Vertex Recipe" - Attribute Pointers ]=====
     //  This is like telling OpenGL: "Hey, in our VBO ingredient container, the data is arranged like this..."
 
     // (0,1,2) - > position , color, texcoord  (Attribute locations in shader)
     // (3,3,2) - > 3 floats for position, 3 floats for color, 2 floats for texcoord (Number of components)
     // 8 floats in total (3+3+2) - > Stride: How many floats to jump to get to the NEXT vertex's data
     // void* 0 - > Offset: Start reading position from the VERY beginning of each vertex's data
     // void* 3*sizeof(float) - > Offset: Start reading color from the 4th float (3rd index) of each vertex's data
     // void* 6*sizeof(float) - > Offset: Start reading texcoord from the 7th float (6th index) of each vertex's data
 
     /**
      * @brief Set up attribute pointer for vertex positions (attribute location 0).
      *
      * Tells OpenGL how to read position data from the VBO.
      * - Location 0 in vertex shader (layout (location = 0) in vec3 aPos;).
      * - 3 components (x, y, z).
      * - Data type is GL_FLOAT (floats).
      * - Not normalized (GL_FALSE).
      * - Stride is 8 * sizeof(float) (bytes between starting position of consecutive vertex attributes).
      * - Offset is 0 (position data starts at the beginning of each vertex data block).
      */
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
     glEnableVertexAttribArray(0); // Turn on attribute location 0 - Position Attribute - in our VAO "recipe card"
 
     /**
      * @brief Set up attribute pointer for vertex colors (attribute location 1).
      *
      * Tells OpenGL how to read color data from the VBO.
      * - Location 1 in vertex shader (layout (location = 1) in vec3 aColor;).
      * - 3 components (red, green, blue).
      * - Data type is GL_FLOAT (floats).
      * - Not normalized (GL_FALSE).
      * - Stride is 8 * sizeof(float).
      * - Offset is 3 * sizeof(float) (color data starts after 3 floats from the beginning of each vertex data block).
      */
     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
     glEnableVertexAttribArray(1); // Turn on attribute location 1 - Color Attribute - in our VAO "recipe card"
 
     /**
      * @brief Set up attribute pointer for texture coordinates (attribute location 2).
      *
      * Tells OpenGL how to read texture coordinate data from the VBO.
      * - Location 2 in vertex shader (layout (location = 2) in vec2 aTexCoord;).
      * - 2 components (texture u, texture v).
      * - Data type is GL_FLOAT (floats).
      * - Not normalized (GL_FALSE).
      * - Stride is 8 * sizeof(float).
      * - Offset is 6 * sizeof(float) (texture coordinate data starts after 6 floats from the beginning of each vertex data block).
      */
     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
     glEnableVertexAttribArray(2); // Turn on attribute location 2 - Texture Attribute - in our VAO "recipe card"
 
     // =====[ Clean Up - Unbind Everything ]=====
     //  It's good habit to unbind things when you're done setting them up in a VAO.
     //  This prevents accidentally messing with them later.
 
     glBindBuffer(GL_ARRAY_BUFFER, 0);         // Deselect VBO as ARRAY_BUFFER - Put away the VBO container for now.
     glBindVertexArray(0);                     // Deselect VAO - Put away the Vertex Recipe Card for now.
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Deselect EBO as ELEMENT_ARRAY_BUFFER - Put away the Index List for now.
 }
 
 /**
  * @brief Makes the VAO active for drawing.
  *
  * Before you can draw a Mesh, you need to "bind" its VAO.
  * This is like saying "OpenGL, use this Vertex Recipe Card for the next drawing commands!".
  */
 void Mesh::bind_VAO() const
 {
     // "Make it active" -  Tell OpenGL to use this VAO for drawing from now on.
     glBindVertexArray(VAO);
 }
 
 /**
  * @brief Makes the VAO inactive.
  *
  * After you are done drawing with a Mesh, it's good practice to "unbind" its VAO.
  * This is like saying "OpenGL, stop using this Vertex Recipe Card for now".
  * It's not strictly necessary for simple programs, but good for bigger projects.
  */
 void Mesh::unbind_VAO() const
 {
     // "Make it inactive" - Tell OpenGL to stop using any specific VAO. Go back to default state.
     glBindVertexArray(0);
 }
 
 /**
  * @brief Gets the number of indices used to create this Mesh.
  *
  * This is handy if you need to know how many indices to tell OpenGL to draw with
  * when you call `glDrawElements`.
  *
  * @return The number of indices.
  */
 unsigned int Mesh::getIndexCount() const
 {
     return indexcount; // Returns the number of indices we saved in the constructor.
 }
 
 /**
  * @brief Destructor for the Mesh class. Cleans up OpenGL resources.
  *
  * When a Mesh object is no longer needed, this "destructor" function is automatically called.
  * It's like the "garbage collector" for OpenGL stuff. It makes sure to delete
  * the VAO, VBO, and EBO from the GPU memory so you don't waste resources.
  */
 Mesh::~Mesh()
 {
     // ====[ Clean up OpenGL stuff -  Free GPU Memory ]====
     glDeleteVertexArrays(1, &VAO); // Delete the Vertex Array Object
     glDeleteBuffers(1, &VBO);      // Delete the Vertex Buffer Object
     glDeleteBuffers(1, &EBO);      // Delete the Element Buffer Object
 }