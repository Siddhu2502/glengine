#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Mesh { 
    public:
        Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
        ~Mesh();

        void bind_VAO() const;
        void unbind_VAO() const;

        unsigned int getIndexCount() const;


    private:
        unsigned int VAO, VBO, EBO, indexcount;
};

#endif // MESH_H