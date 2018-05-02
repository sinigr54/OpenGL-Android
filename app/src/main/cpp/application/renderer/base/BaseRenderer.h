//
// Created by sinigr on 3/21/18.
//

#ifndef OPENGLCOURSE_RENDERER_H
#define OPENGLCOURSE_RENDERER_H

#include "application/renderer/shader/ShaderProgram.h"

class BaseRenderer {

protected:
    AAssetManager *const assetManager;

public:
    BaseRenderer(AAssetManager *assetManager);

    virtual ~BaseRenderer() = default;

    virtual void onSurfaceCreated() = 0;

    virtual void onSurfaceChanged(int width, int height) = 0;

    virtual void onDrawFrame() = 0;
};


#endif //OPENGLCOURSE_RENDERER_H
