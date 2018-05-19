//
// Created by sinigr on 4/15/18.
//

#ifndef OPENGLCOURSE_SCENERENDERER_H
#define OPENGLCOURSE_SCENERENDERER_H

#include <application/renderer/camera/Camera.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <application/renderer/model/Model.h>

class ObjectRenderer {
    std::unique_ptr<Assimp::IOSystem> ioSystem;

    ShaderProgram shaderProgram;

    Model model;

    AAssetManager *const assetManager;

public:
    ObjectRenderer(AAssetManager *assetManager, const std::string &path);

    ~ObjectRenderer() = default;

    void initialize();

    void draw(const glm::mat4 &projectionMat, const glm::mat4 &viewMat,
              const glm::mat4 &modelMat, const glm::vec3 &viewPosition);

    void drawAr(const glm::mat4 &projectionMat, const glm::mat4 &viewMat,
                const glm::mat4 &modelMat);
};


#endif //OPENGLCOURSE_SCENERENDERER_H
