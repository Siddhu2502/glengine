#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

/**
 * @class Mesh
 * @brief Represents a 3D mesh for rendering in OpenGL.
 *
 * The `Mesh` class encapsulates the data needed to draw a 3D shape, including vertex positions,
 * colors, texture coordinates, and index information for defining faces.
 * It manages OpenGL Vertex Array Objects (VAOs), Vertex Buffer Objects (VBOs), and Element Buffer Objects (EBOs)
 * to efficiently store and render mesh data on the GPU.
 */
class Mesh {
public:
    /**
     * @brief Constructor for the Mesh class. Initializes a new Mesh object.
     *
     * This constructor prepares a Mesh for rendering by setting up its Vertex Array Object (VAO),
     * Vertex Buffer Object (VBO), and Element Buffer Object (EBO) in OpenGL. It uploads the provided
     * vertex and index data to the GPU.
     *
     * @param vertices  A vector of floats containing vertex data. Each vertex is expected to have
     *                  position (3 floats), color (3 floats), and texture coordinates (2 floats), for a total of 8 floats per vertex.
     * @param indices   A vector of unsigned integers defining the indices of vertices that make up the mesh's faces (triangles).
     *
     * @note The format of the `vertices` vector is crucial. It's assumed to be interleaved:
     *       [posX, posY, posZ, colorR, colorG, colorB, texU, texV,  posX, posY, posZ, ... ] for each vertex.
     */
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    /**
     * @brief Destructor for the Mesh class. Releases OpenGL resources.
     *
     * The destructor is responsible for cleaning up OpenGL resources allocated by the Mesh,
     * specifically deleting the VAO, VBO, and EBO to prevent memory leaks on the GPU.
     */
    ~Mesh();

    /**
     * @brief Binds the Vertex Array Object (VAO) for rendering.
     *
     * Before drawing this Mesh, you must call `bind_VAO()` to make its VAO the active vertex array.
     * This prepares OpenGL to use the vertex attributes and index data associated with this Mesh for subsequent draw calls.
     */
    void bind_VAO() const;

    /**
     * @brief Unbinds the Vertex Array Object (VAO).
     *
     * After you are finished drawing with this Mesh, it is good practice to call `unbind_VAO()` to
     * deselect the VAO. This helps prevent accidental modifications to the VAO state and promotes cleaner OpenGL state management.
     */
    void unbind_VAO() const;

    /**
     * @brief Gets the number of indices used to define this Mesh.
     *
     * This method returns the count of indices stored in the Element Buffer Object (EBO) for this Mesh.
     * This value is essential when calling `glDrawElements` to render the Mesh correctly, as it tells OpenGL how many indices to process.
     *
     * @return The number of indices in the Mesh's index buffer.
     */
    unsigned int getIndexCount() const;


private:
    unsigned int VAO, VBO, EBO, indexcount; ///< OpenGL IDs for VAO, VBO, EBO, and count of indices.
};

#endif // MESH_H