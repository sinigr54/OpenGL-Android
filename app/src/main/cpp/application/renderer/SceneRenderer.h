//
// Created by sinigr on 4/15/18.
//

#ifndef OPENGLCOURSE_SCENERENDERER_H
#define OPENGLCOURSE_SCENERENDERER_H


#include <application/renderer/base/BaseRenderer.h>
#include <application/renderer/camera/Camera.h>

class SceneRenderer : public BaseRenderer {
    GLuint VBO{0};
    GLuint cubeVao{0};
    GLuint lightVao{0};
    GLuint IBO{0};

    double delta{0.0};

    GLuint cubeTexture;
    GLuint cubeSpecularMap;

    float screenWidth;
    float screenHeight;

    Camera camera;

    glm::vec3 lightPosition{0.0f, 1.0f, 4.0f};

    ShaderProgram lampShader;
    ShaderProgram sceneShader;

public:
    SceneRenderer(AAssetManager *assetManager);

    ~SceneRenderer() override;

    void onSurfaceCreated() noexcept override;

    void onSurfaceChanged(int width, int height) noexcept override;

    void onDrawFrame() noexcept override;
};


#endif //OPENGLCOURSE_SCENERENDERER_H
