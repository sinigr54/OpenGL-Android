//
// Created by sinigr on 3/21/18.
//

#include <application/renderer/SceneRenderer.h>
#include "NativeApplication.h"

NativeApplication::NativeApplication(AAssetManager *assetManager,
                                     Assimp::IOSystem *ioSystem) :
        pRenderer(new SceneRenderer(assetManager, ioSystem)) {
    LOGI("%s", "NativeApplication");
}

NativeApplication::~NativeApplication() {
    LOGI("%s", "~NativeApplication");

    delete pRenderer;
}

void NativeApplication::onPause() {
    /* Nothing */
}

void NativeApplication::onResume(void *env, void *context, void *activity) {
    /* Nothing */
}

void NativeApplication::onSurfaceCreated() {
    pRenderer->onSurfaceCreated();
}

void NativeApplication::onDisplayGeometryChanged(int displayRotation, int width, int height) {
    pRenderer->onSurfaceChanged(displayRotation, width, height);
}

void NativeApplication::onDrawFrame() {
    pRenderer->onDrawFrame();
}
