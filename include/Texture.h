#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb_image/stb_image.h>

class Texture {
    public:
        Texture(const char* filepath);
        ~Texture();

        void load_image(const char* filepath);


        void bind() const;
        void unbind() const;

    private:
        int width, height, nrChannels;
        const char* filepath;
        unsigned int ID;
};

#endif // TEXTURE_H