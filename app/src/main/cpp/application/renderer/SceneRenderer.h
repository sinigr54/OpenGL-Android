//
// Created by sinigr on 4/15/18.
//

#ifndef OPENGLCOURSE_SCENERENDERER_H
#define OPENGLCOURSE_SCENERENDERER_H


#include <application/renderer/base/BaseRenderer.h>
#include <application/renderer/camera/Camera.h>
#include <vector>

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

    Camera camera{glm::vec3(0.0f, 0.0f, 8.0f)};

    glm::vec3 lightPosition{0.0f, 1.0f, 4.0f};
    glm::vec3 lightDirection{0.0f, 0.0f, 0.0f};

    glm::vec3 ambient{glm::vec3(0.1f, 0.1f, 0.1f)};
    glm::vec3 diffuse{glm::vec3(0.6f, 0.6f, 0.6f)};
    glm::vec3 specular{glm::vec3(2.0f, 2.0f, 2.0f)};

    std::vector<glm::vec3> pointLightPositions{
//            glm::vec3(0.7f, 0.2f, 2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f, 2.0f, -12.0f),
            glm::vec3(0.0f, 0.0f, -3.0f)
    };

    ShaderProgram lampShader;
    ShaderProgram sceneShader;

public:
    SceneRenderer(AAssetManager *assetManager);

    ~SceneRenderer() override;

    void onSurfaceCreated() override;

    void onSurfaceChanged(int width, int height) override;

    void onDrawFrame() override;
};


#endif //OPENGLCOURSE_SCENERENDERER_H
