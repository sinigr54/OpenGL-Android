//
// Created by sinigr on 4/15/18.
//

#ifndef OPENGLCOURSE_SCENERENDERER_H
#define OPENGLCOURSE_SCENERENDERER_H


#include <application/renderer/base/BaseRenderer.h>
#include <application/renderer/camera/Camera.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <application/renderer/model/Model.h>

class SceneRenderer : public BaseRenderer {
    std::unique_ptr<Assimp::IOSystem> ioSystem;

    double delta{0.0};

    float screenWidth;
    float screenHeight;

    Camera camera{glm::vec3(0.0f, 0.0f, 8.0f)};

    ShaderProgram sceneShader;

    std::vector<Model> models;
public:
    SceneRenderer(AAssetManager *assetManager, Assimp::IOSystem *);

    ~SceneRenderer() override;

    void onSurfaceCreated() override;

    void onSurfaceChanged(int width, int height) override;

    void onDrawFrame() override;
};


#endif //OPENGLCOURSE_SCENERENDERER_H
