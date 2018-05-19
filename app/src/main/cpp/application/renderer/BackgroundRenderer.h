//
// Created by Alexander Chekh on 15.05.2018.
//

#ifndef OPENGLCOURSE_BACKGROUNDRENDERER_H
#define OPENGLCOURSE_BACKGROUNDRENDERER_H

#include <vector>
#include "glm.hpp"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <application/renderer/shader/ShaderProgram.h>

#include "arcore_c_api.h"

using std::vector;

class BackgroundRenderer {
    struct Vertex {
        glm::vec3 position;
        glm::vec2 textureCoords;
    };

    vector<Vertex> vertices;

    static constexpr int VERTEX_SIZE = 3;
    static constexpr int COORDS_SIZE = 2;

    static constexpr int gNumVertices = 4;
    float transformedUvs[gNumVertices * 2];

    ShaderProgram shaderProgram;
    GLuint textureId;

    GLuint attributeVertices;
    GLuint attributeUvs;

    unsigned int VAO;
    unsigned int VBO;

    bool isUvsInitialized = false;

    void updateGraphicContext();

public:

    BackgroundRenderer() = default;
    ~BackgroundRenderer() = default;

    void initialize();

    void draw(const ArSession *arSession, const ArFrame *arFrame);

    GLuint getTextureId() const;
};


#endif //OPENGLCOURSE_BACKGROUNDRENDERER_H
