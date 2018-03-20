//
// Created by sinigr on 3/21/18.
//

#include "NativeApplication.h"

NativeApplication::NativeApplication(AAssetManager *assetManager)
        : assetManager(assetManager) {

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
    renderer.onSurfaceCreated();
}

void NativeApplication::onSurfaceChanged(int width, int height) {
    renderer.onSurfaceChanged(width, height);
}

void NativeApplication::onDrawFrame() {
    renderer.onDrawFrame();
}
