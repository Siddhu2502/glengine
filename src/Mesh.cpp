#include <Mesh.h>

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
    : indexcount(indices.size())
{
    // Create a new "Vertex Recipe Card" (VAO) and get its ID
    glGenVertexArrays(1, &VAO);
    // Create a new "Ingredient Container" (VBO) for vertex data and get its ID
    glGenBuffers(1, &VBO);
    // Create a new "Index List" (EBO) and get its ID
    glGenBuffers(1, &EBO);

    // "Pull out" the "Vertex Recipe Card" (VAO) - make it active
    glBindVertexArray(VAO);

    // "Select" the "Ingredient Container" (VBO) - make it the active ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // "Pour" the raw vertex data (positions, colors, texcoords) into the VBO container
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), static_cast<const void*>(vertices.data()), GL_STATIC_DRAW);
    
    // "Select" the "Index List" (EBO) - make it the active ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // "Write down" the index order into the EBO Index List
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), static_cast<const void*>(indices.data()), GL_STATIC_DRAW);

    // --- Tell OpenGL how to read the vertex data from the VBO based on the VAO's "recipe" ---

    // Tell OpenGL: "Attribute 0 (position) is 3 floats, starting at the beginning of the VBO, with a stride of 8 floats per vertex"
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    // Enable Attribute 0 (position) in the VAO's recipe
    glEnableVertexAttribArray(0);

    // Tell OpenGL: "Attribute 1 (color) is 3 floats, starting at 3 floats offset into each vertex, stride 8 floats"
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    // Enable Attribute 1 (color) in the VAO's recipe
    glEnableVertexAttribArray(1);

    // Tell OpenGL: "Attribute 2 (texture coordinates) is 2 floats, starting at 6 floats offset into each vertex, stride 8 floats"
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    // Enable Attribute 2 (texture coordinates) in the VAO's recipe
    glEnableVertexAttribArray(2);

    // --- "Put away" or "Deselect" everything after setup - Clear the workspace ---
    glBindBuffer(GL_ARRAY_BUFFER, 0);         // Deselect VBO as ARRAY_BUFFER
    glBindVertexArray(0);                    // Deselect VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Deselect EBO as ELEMENT_ARRAY_BUFFER
}

void Mesh::bind_VAO() const
{
    // "Pull out" the "Vertex Recipe Card" (VAO) - make it active
    glBindVertexArray(VAO);
}

void Mesh::unbind_VAO() const
{
    // "Put away" the "Vertex Recipe Card" (VAO) - make it inactive
    glBindVertexArray(0);

}

unsigned int Mesh::getIndexCount() const
{
    return indexcount;
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}