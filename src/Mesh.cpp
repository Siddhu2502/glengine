#include <Mesh.h>

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
    : indexcount(indices.size())
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // make it active
    glBindVertexArray(VAO);

    // make it the active ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // "Pour" the raw vertex data (positions, colors, texcoords) into the VBO container
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), static_cast<const void*>(vertices.data()), GL_STATIC_DRAW);
    
    // "Select" the "Index List" (EBO) - make it the active ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // "Write down" the index order into the EBO Index List
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), static_cast<const void*>(indices.data()), GL_STATIC_DRAW);

    // --- Tell OpenGL how to read the vertex data from the VBO based on the VAO's "recipe" ---

    // (0,1,2) - > position , color, texcoord
    // (3,3,2) - > 3 floats for position, 3 floats for color, 2 floats for texcoord
    // 8 floats in total (3+3+2)
    // 8 * sizeof(float) - > jump 8 floats to get to the next vertex data
    // void* 0 - > start from the beginning of the vertex data
    // void* 3*sizeof(float) - > start from the 4th float (3rd index) of the vertex data
    // void* 6*sizeof(float) - > start from the 7th float (6th index) of the vertex data

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);         // Deselect VBO as ARRAY_BUFFER
    glBindVertexArray(0);                     // Deselect VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Deselect EBO as ELEMENT_ARRAY_BUFFER
}

void Mesh::bind_VAO() const
{
    // make it active
    glBindVertexArray(VAO);
}

void Mesh::unbind_VAO() const
{
    //make it inactive
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