//
// Created by Alexander Chekh on 15.05.2018.
//

#ifndef OPENGLCOURSE_BACKGROUNDRENDERER_H
#define OPENGLCOURSE_BACKGROUNDRENDERER_H


#include <application/renderer/model/Model.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "arcore_c_api.h"

class BackgroundRenderer {
    static constexpr int gNumVertices = 4;

    ShaderProgram shaderProgram;
    GLuint textureId;

    GLuint attributeVertices;
    GLuint attributeUvs;

    float transformedUvs[gNumVertices * 2];
    bool isUvsInitialized = false;

public:

    BackgroundRenderer() = default;
    ~BackgroundRenderer() = default;

    void initialize();

    void draw(const ArSession *arSession, const ArFrame *arFrame);

    GLuint getTextureId() const;
};


#endif //OPENGLCOURSE_BACKGROUNDRENDERER_H
