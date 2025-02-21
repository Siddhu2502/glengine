#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb_image/stb_image.h>

/**
 * @file Texture.h
 * @brief Header file for the Texture class, managing OpenGL textures.
 *
 * Defines the `Texture` class, which handles loading images from files and creating OpenGL textures.
 * This class simplifies the process of using textures in OpenGL by encapsulating texture creation,
 * parameter setup, and binding/unbinding operations.
 */

/**
 * @class Texture
 * @brief Manages an OpenGL 2D Texture.
 *
 * The `Texture` class is responsible for loading image files from disk and creating OpenGL 2D textures from them.
 * It handles texture parameter settings like wrapping and filtering, and provides methods to bind and unbind the texture
 * for use in rendering.  Textures are used to apply images onto 3D models, adding detail and visual richness.
 */
class Texture {
public:
    /**
     * @brief Constructor for the Texture class. Loads an image from a file and creates an OpenGL texture.
     *
     * This constructor takes a file path to an image, loads the image data, and then creates an OpenGL 2D texture
     * using this image data. It also sets default texture parameters for wrapping and filtering to ensure a reasonable
     * default texture appearance.
     *
     * @param filepath The path to the image file to load (e.g., "textures/wood.png").
     *
     * @note Upon successful construction, the Texture object is ready to be bound and used in rendering operations.
     * @note The constructor uses the stb_image library for image loading. Ensure that stb_image is properly integrated into your project.
     * @note If image loading fails, an error message is printed to `std::cerr`.
     */
    Texture(const char* filepath);

    /**
     * @brief Destructor for the Texture class. Deletes the OpenGL texture.
     *
     * The destructor is responsible for releasing OpenGL resources associated with the Texture object.
     * Specifically, it deletes the OpenGL texture object to free up GPU memory when the `Texture` object is no longer needed.
     */
    ~Texture();

    /**
     * @brief Loads image data from a file and updates the OpenGL texture.
     *
     * This method allows you to load a new image from a file into an existing `Texture` object, effectively
     * changing the texture's image data. This can be useful for dynamically updating textures or reusing Texture objects.
     *
     * @param filepath The path to the new image file to load.
     *
     * @note This function reloads the image data and updates the texture on the GPU. Any previous image data in this Texture object will be replaced.
     * @note Error handling is included: if loading the image fails, an error message is printed to `std::cerr`.
     */
    void load_image(const char* filepath);

    /**
     * @brief Binds the Texture for rendering.
     *
     * Before using this Texture in a shader, you must bind it. Binding makes this Texture the currently active texture
     * in OpenGL's texture unit 0.  Subsequent shader texture lookups (e.g., using texture samplers in shaders) will then
     * sample from this bound Texture.
     *
     * @note Internally, this function calls `glBindTexture(GL_TEXTURE_2D, ID)` to bind the texture.
     * @note Typically, you bind a texture right before drawing a mesh that uses this texture.
     */
    void bind() const;

    /**
     * @brief Unbinds the Texture.
     *
     * Unbinding the texture makes no texture active for the current texture unit (unit 0 in this case, as we are binding to `GL_TEXTURE_2D`).
     * While not strictly necessary for basic programs, unbinding textures after use is good practice for managing OpenGL state
     * and preventing unintended side effects in more complex rendering scenarios.
     *
     * @note Internally, this function calls `glBindTexture(GL_TEXTURE_2D, 0)` to unbind the texture.
     */
    void unbind() const;

private:
    int width, height, nrChannels; ///< Image dimensions (width, height) and number of color channels. Loaded by `stbi_load`.
    const char* filepath;         ///< Filepath of the texture image (stored for potential future use, though not currently used after construction).
    unsigned int ID;             ///< OpenGL ID of the texture object.
};

#endif // TEXTURE_H