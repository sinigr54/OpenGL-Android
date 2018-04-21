//
// Created by sinigr on 4/20/18.
//

#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex> &vertices,
           const std::vector<unsigned int> &indices,
           const std::vector<Texture> &textures) noexcept :
        vertices(vertices),
        indices(indices),
        textures(textures) {

    initMesh();
}

Mesh::Mesh(Mesh &&other) noexcept :
        vertices(std::move(other.vertices)),
        indices(std::move(other.indices)),
        textures(std::move(other.textures)),
        VBO(other.VBO),
        VAO(other.VAO),
        IBO(other.IBO) {

}

void Mesh::draw(ShaderProgram shaderProgram) const noexcept {
    unsigned int diffuse = 1;
    unsigned int specular = 1;

    for (unsigned int i = 0; i < textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string name;
        std::string number;

        auto type = textures[i].type;
        if (type == TextureType::DIFFUSE) {
            name = "texture_diffuse";
            number = std::to_string(diffuse);
            ++diffuse;
        } else if (type == TextureType::SPECULAR) {
            name = "texture_specular";
            number = std::to_string(specular);
            ++specular;
        }

        shaderProgram.setUniform("material." + name + number, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Mesh::initMesh() noexcept {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
                 &vertices[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
                 &indices[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *) offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *) offsetof(Vertex, textureCoords));

    glBindVertexArray(0);
}

bool Texture::operator==(const std::string &rhs) {
    return path == rhs;
}
