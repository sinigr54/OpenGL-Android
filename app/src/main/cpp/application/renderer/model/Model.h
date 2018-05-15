//
// Created by sinigr on 4/20/18.
//

#ifndef OPENGLCOURSE_MODEL_H
#define OPENGLCOURSE_MODEL_H

#include <vector>
#include <string>
#include <algorithm>
#include <assimp/scene.h>

#include "Mesh.h"

#include <assimp/IOSystem.hpp>
#include <ostream>

class Model {
    std::vector<Mesh> meshes;
    std::vector<Texture> loadedTextures;
    std::string directory;

    void loadModel(const std::string &path, Assimp::IOSystem *ioSystem = nullptr);

    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial *material,
                                              aiTextureType type,
                                              TextureType textureType);

    unsigned int textureFromFile(const std::string &path,
                                 const std::string &directory,
                                 bool gamma = false);

public:

    Model() = default;

    Model(const char *path, Assimp::IOSystem *ioSystem = nullptr);

    Model(const std::string &path, Assimp::IOSystem *ioSystem = nullptr);

    Model(Model &&other);

    void draw(ShaderProgram shaderProgram) const noexcept;
};


#endif //OPENGLCOURSE_MODEL_H
