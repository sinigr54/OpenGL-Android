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

    double delta{0.0};

    float screenWidth;
    float screenHeight;

    Camera camera{glm::vec3(0.0f, 0.0f, 8.0f)};

    ShaderProgram sceneShader;

    std::vector<Model> models;

    AAssetManager *const assetManager;

public:
    ObjectRenderer(AAssetManager *assetManager, Assimp::IOSystem *);

    ~ObjectRenderer() = default;

    void initialize();

    void onDisplayGeometryChanged(int width, int height);

    void onDraw();
};


#endif //OPENGLCOURSE_SCENERENDERER_H
