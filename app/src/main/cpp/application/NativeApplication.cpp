//
// Created by sinigr on 3/21/18.
//

#include <application/renderer/SceneRenderer.h>
#include "NativeApplication.h"

NativeApplication::NativeApplication(AAssetManager *assetManager,
                                     Assimp::IOSystem *ioSystem) :
        pRenderer(std::make_unique<SceneRenderer>(assetManager, ioSystem)) {

}

NativeApplication::~NativeApplication() {

}

void NativeApplication::onPause() {
    /* Nothing */
}

void NativeApplication::onResume() {
    /* Nothing */
}

void NativeApplication::onSurfaceCreated() {
    pRenderer->onSurfaceCreated();
}

void NativeApplication::onSurfaceChanged(int width, int height) {
    pRenderer->onSurfaceChanged(width, height);
}

void NativeApplication::onDrawFrame() {
    pRenderer->onDrawFrame();
}
