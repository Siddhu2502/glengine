/**
 * @file Texture.cpp
 * @brief Code for the Texture class, your versatile tool for painting surfaces in 3D graphics - now with multi-texture support!
 *
 * This file is all about creating and managing Textures, enhancing them to work with multiple texture units.
 * Think of a Texture as a digital image or pattern that you can "stick" onto your 3D shapes to make them visually rich.
 * This updated class still handles loading images from files and preparing them for OpenGL,
 * but now it also allows you to specify which texture unit the texture should be bound to,
 * enabling you to use multiple textures in your shaders simultaneously for more complex effects!
 */

 #include "Texture.h"
 #include <iostream>

 /**
  * @brief Constructor for the Texture class. The enhanced "image loader" and "texture prepper" - now with texture unit assignment!
  *
  * When you create a Texture object, this constructor is the first step in getting your image ready for OpenGL, but now with the ability to specify a texture unit!
  * It's like saying "Hey, I want to load this image from this file, turn it into a Texture, AND assign it to a specific texture unit for use in my shaders!".
  *
  * @param filepath  The path to the image file you want to load as a texture.
  *                  This is like giving the Texture constructor the address of the picture you want to use.
  * @param textureUnit [Optional] The texture unit to which this texture should be bound when `bind()` is called. Default is 0 (GL_TEXTURE0).
  *                    Texture units are like different "texture slots" in your GPU. Using different units allows you to use multiple textures at the same time in your shaders.
  *                    Think of texture units as different paintbrushes, each loaded with a different texture, allowing you to blend and combine them in your artwork!
  *
  * @note This constructor now does the following important things (with additions for texture units and vertical flipping):
  *       - **Generates a Texture ID:**  As before, we get a unique ID from OpenGL for our Texture.
  *       - **Binds the Texture (Initially):**  We still "select" our newly created Texture to configure its initial properties.
  *       - **Sets Texture Parameters (Wrapping and Filtering):**  Texture wrapping and filtering are still configured to control texture behavior.
  *       - **Sets Vertical Flip on Load:**  Crucially, `stbi_set_flip_vertically_on_load(true)` is now called *before* loading the image. This tells the stb_image library to flip the image vertically when loading.
  *         This is often needed because image formats and OpenGL have different conventions for texture coordinate origins (top-left vs. bottom-left). Flipping here ensures textures are oriented correctly in OpenGL.
  *       - **Loads the Image from File:** Calls `load_image()` to read the image data from the file and upload it to OpenGL.
  *       - **Prints Load Confirmation:**  A message is printed to `std::cout` to confirm that the texture has been loaded and to show its filepath for debugging and feedback.
  */
 Texture::Texture(const char* filepath, unsigned int textureUnit = 0)
         : ID(0), unit(textureUnit) ///< Initialize Texture ID to 0 and store the specified texture unit. Default unit is 0.
 {
     // ====[ OpenGL Texture Generation and Initial Binding -  Preparing the Canvas & Selecting Unit 0 Initially ]====

     // 1. Generate a Texture ID -  "Get a blank canvas"
     glGenTextures(1, &ID); // Request and get a unique Texture ID from OpenGL.

     // 2. Bind the Texture - "Select the canvas to work on (initially on unit 0)"
     glBindTexture(GL_TEXTURE_2D, ID); // Bind the Texture to GL_TEXTURE_2D target.  Initially, we bind it on the currently active texture unit (which is unit 0 by default).
                                      // We'll use `bind()` later to bind it to the specified 'unit'.

     // ====[ Texture Parameters -  Setting up the "Canvas Properties" (Still the same wrapping & filtering) ]====
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Repeat texture horizontally.
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Repeat texture vertically.
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear filtering for minification.
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear filtering for magnification.

     // ====[ Set Vertical Flip - "Adjust Image Orientation upon Loading" ]====
     stbi_set_flip_vertically_on_load(true); // Tell stb_image to flip loaded images vertically.
     // This is important because OpenGL expects textures to have their origin at the bottom-left,
     // while many image formats have their origin at the top-left. Flipping corrects this difference.
     // This setting is applied to ALL subsequent image loads using stb_image in your application, until changed.

     // ====[ Load the Image Data -  "Paint on the Canvas" ]====
     load_image(filepath); // Load the image from the specified file path.

     // ====[ Confirmation Message - "Let the User Know Texture Loading was Attempted" ]====
     std::cout << "loaded texture: " << filepath << std::endl; // Print a message to the console confirming texture loading and the filepath.
 }

 /**
  * @brief Destructor for the Texture class.  Still the "resource cleaner-upper"!
  *
  *  Just like before, when a Texture object is no longer needed, this destructor cleans up OpenGL resources,
  *  specifically the Texture ID, to prevent GPU memory leaks.
  */
 Texture::~Texture() {
     glDeleteTextures(1, &ID); // Delete the OpenGL Texture object using its ID.
 }

 /**
  * @brief Loads image data from a file and feeds it to OpenGL -  "Image Loading Engine" (Unchanged from previous version).
  *
  * This function remains the same in its core functionality - it loads the image data using stb_image and uploads it to OpenGL.
  * See previous documentation for detailed explanation of `load_image` function.
  *
  * @param filepath The path to the image file to load.
  */
 void Texture::load_image(const char* filepath) {
     unsigned char *data = stbi_load(filepath, &width, &height, &nrChannels, 0);

     if (data == nullptr)
     {
         std::cerr << "Failed to load texture" << std::endl;
     }
     else
     {
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
         glGenerateMipmap(GL_TEXTURE_2D);
     }

     stbi_image_free(data);
 }

 /**
  * @brief Makes this Texture active for use in rendering - now binds to the specified Texture Unit! "Grab the Paintbrush from the Correct Rack!"
  *
  *  This `bind()` function is updated to handle texture units!
  *  It now makes the Texture active on the specific texture unit that was assigned to it during construction (or unit 0 by default).
  *  This is crucial for multi-texturing - if you want to use multiple textures in your shader, you need to bind each one to a *different* texture unit.
  *  For example, you might bind a diffuse texture to unit 0, a normal map to unit 1, and a specular map to unit 2, and then sample from these units in your shader.
  */
 void Texture::bind() const {
     // ====[ Activate Texture on its Assigned Texture Unit - "Pick up the Paintbrush from the Right Rack" ]====
     glActiveTexture(GL_TEXTURE0 + unit); // Activate the texture unit that this Texture is assigned to.
     // - GL_TEXTURE0 + unit:  Calculates the OpenGL texture unit enum.
     //   GL_TEXTURE0 is the first texture unit, GL_TEXTURE1 is the second, and so on.
     //   Adding 'unit' (which is the texture unit index) to GL_TEXTURE0 selects the desired texture unit.
     //   For example, if unit is 0, it becomes GL_TEXTURE0; if unit is 1, it becomes GL_TEXTURE1, etc.
     //   Texture units are like numbered "slots" where you can bind textures.
     //   Modern GPUs support many texture units (typically at least 16 or more).

     glBindTexture(GL_TEXTURE_2D, ID); // Bind our Texture (identified by 'ID') to the currently active texture unit (set by glActiveTexture).
     // Now, any texture operations on the active texture unit will affect this Texture.
 }

 /**
  * @brief Makes this Texture inactive - "Put down the Paintbrush" (Unchanged unbinding).
  *
  * The `unbind()` function remains the same. It unbinds the texture from the currently active texture unit.
  *  See previous documentation for details.
  */
 void Texture::unbind() const {
     glBindTexture(GL_TEXTURE_2D, 0); // Unbind the currently active GL_TEXTURE_2D texture on the active texture unit.
 }