#include "Texture.h"
#include <iostream>

Texture::Texture(const char* filepath, unsigned int textureUnit = 0)
        : ID(0), unit(textureUnit)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    load_image(filepath);

    std::cout << "loaded texture: " << filepath << std::endl;
}

Texture::~Texture() {
    glDeleteTextures(1, &ID);
}

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

void Texture::bind() const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}