//
// Created by Alexander Chekh on 15.05.2018.
//

#ifndef OPENGLCOURSE_PLANERENDERER_H
#define OPENGLCOURSE_PLANERENDERER_H


#include <android/asset_manager.h>
#include <detail/type_mat.hpp>
#include <arcore_c_api.h>
#include <glm.hpp>
#include <vector>
#include <GLES3/gl3.h>
#include <application/renderer/shader/ShaderProgram.h>

class PlaneRenderer {
public:
    PlaneRenderer() = default;
    ~PlaneRenderer() = default;

    // Sets up OpenGL state used by the plane renderer. Must be called on the
    // OpenGL thread.
    void initialize();

    // Draws the provided plane.
    void draw(const glm::mat4& projectionMat, const glm::mat4& viewMat,
              const ArSession* arSession, const ArPlane* arPlane,
              const glm::vec3& color);

private:

    void updateRenderConfig();

    void updateForPlane(const ArSession* arSession, const ArPlane* arPlane);

    std::vector<glm::vec3> vertices;
    std::vector<GLushort> triangles;
    glm::mat4 modelMat = glm::mat4(1.0f);
    glm::vec3 normal = glm::vec3(0.0f);

    GLuint textureId;

    unsigned int VAO;
    unsigned int VBO;
    unsigned int IBO;

    ShaderProgram shaderProgram;
    GLint attributeVertices;
};


#endif //OPENGLCOURSE_PLANERENDERER_H
