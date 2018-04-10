//
// Created by sinigr on 3/21/18.
//

#ifndef OPENGLCOURSE_RENDERER_H
#define OPENGLCOURSE_RENDERER_H

#include "shader/Shader.h"

class Renderer {
    AAssetManager *const assetManager;

    // TODO update with concrete renderer
    Shader cubeShader;
    Shader lampShader;
public:
    Renderer(AAssetManager *assetManager);

    void onSurfaceCreated();

    void onSurfaceChanged(int width, int height);

    void onDrawFrame();
};


#endif //OPENGLCOURSE_RENDERER_H
