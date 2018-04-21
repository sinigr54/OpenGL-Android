//
// Created by sinigr on 4/20/18.
//

#ifndef OPENGLCOURSE_MESH_H
#define OPENGLCOURSE_MESH_H

#include <glm.hpp>
#include <vector>
#include <application/renderer/shader/ShaderProgram.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoords;
};

enum class TextureType {
    DIFFUSE, SPECULAR
};

struct Texture {
    unsigned int id;
    TextureType type;
    std::string path;

    bool operator==(const std::string& rhs);
};

class Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    unsigned int VAO, VBO, IBO;

    void initMesh() noexcept;

public:

    Mesh(const std::vector<Vertex> &vertices,
         const std::vector<unsigned int> &indices,
         const std::vector<Texture> &textures) noexcept;

    Mesh(Mesh &&other) noexcept;

    void draw(ShaderProgram shaderProgram) const noexcept;
};


#endif //OPENGLCOURSE_MESH_H
