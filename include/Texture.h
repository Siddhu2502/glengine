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
        Texture(const char* filepath, unsigned int textureUnit);
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
        int width, height, nrChannels;
        const char* filepath;
        unsigned int ID, unit;
};

#endif // TEXTURE_H