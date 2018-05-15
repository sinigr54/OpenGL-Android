//
// Created by sinigr on 4/20/18.
//

#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/IOSystem.hpp>

Model::Model(const char *path, Assimp::IOSystem *ioSystem) : Model(std::string(path), ioSystem) {

}

Model::Model(const std::string &path, Assimp::IOSystem *ioSystem) {
    loadModel(path, ioSystem);
}

Model::Model(Model &&other) :
        meshes(std::move(other.meshes)),
        loadedTextures(std::move(other.loadedTextures)),
        directory(std::move(other.directory)) {

}

void Model::draw(ShaderProgram shaderProgram) const noexcept {
    for (const auto &mesh : meshes) {
        mesh.draw(shaderProgram);
    }
}

void Model::loadModel(const std::string &path, Assimp::IOSystem *ioSystem) {
    Assimp::Importer importer{};
    if (ioSystem != nullptr) {
        importer.SetIOHandler(ioSystem);
    }

    const aiScene *scene = importer.ReadFile(path,
                                             aiProcess_Triangulate | aiProcess_FlipUVs);

    if (scene == nullptr ||
        scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        scene->mRootNode == nullptr) {
        LOGI("%s::%s", "Error::Assimp", importer.GetErrorString());

        return;
    }

    directory = path.substr(0, path.find_last_of('/')); // TODO fix platform delimiter

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    // обработать все полигональные сетки в узле(если есть)
    for (auto i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // выполнить ту же обработку и для каждого потомка узла
    for (auto i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (auto i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;

        auto assimpVertex = mesh->mVertices[i];
        vertex.position.x = assimpVertex.x;
        vertex.position.y = assimpVertex.y;
        vertex.position.z = assimpVertex.z;

        auto assimpNormal = mesh->mNormals[i];
        vertex.normal.x = assimpNormal.x;
        vertex.normal.y = assimpNormal.y;
        vertex.normal.z = assimpNormal.z;

        if (mesh->HasTextureCoords(0)) {
            vertex.textureCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.textureCoords.y = mesh->mTextureCoords[0][i].y;
        } else {
            vertex.textureCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (auto i = 0; i < mesh->mNumFaces; ++i) {
        auto face = mesh->mFaces[i];
        for (auto j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
                                                                aiTextureType_DIFFUSE,
                                                                TextureType::DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Texture> specularMaps = loadMaterialTextures(material,
                                                                 aiTextureType_SPECULAR,
                                                                 TextureType::SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture>
Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, TextureType textureType) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < material->GetTextureCount(type); i++) {
        aiString textureFilePath;
        material->GetTexture(type, i, &textureFilePath);

        auto iterator = std::find(loadedTextures.begin(),
                                  loadedTextures.end(),
                                  std::string(textureFilePath.C_Str()));

        if (iterator == loadedTextures.end()) {
            Texture texture;
            texture.id = textureFromFile(textureFilePath.C_Str(), directory);
            texture.type = textureType;
            textures.push_back(texture);
            loadedTextures.push_back(texture);
        } else {
            textures.push_back(*iterator);
        }
    }

    return textures;
}

unsigned int Model::textureFromFile(const std::string &path,
                                    const std::string &directory, bool gamma) {
    std::string fileName = directory + "/" + path;

    unsigned int textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    bool result = RenderUtils::bindTextureSource(GL_TEXTURE_2D, fileName);

    if (result) {
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        LOGI("%s: %s", "Texture failed to load at path", fileName.c_str());
    }

    return textureId;
}

Model &Model::operator=(Model &&other) {
    if (this != &other) {
        meshes = std::move(other.meshes);
        loadedTextures = std::move(other.loadedTextures);
        directory = std::move(other.directory);
    }

    return *this;
}
