//
// Created by Alexander Chekh on 15.05.2018.
//

#include "SceneApplication.h"

#include <AndroidJNIIOSystem.h>

SceneApplication::SceneApplication(AAssetManager *assetManager,
                                   const std::string &internalDirectoryPath) :
        GraphicalApplication(assetManager, internalDirectoryPath),
        objectRenderer(assetManager,
                       new Assimp::AndroidJNIIOSystem(assetManager, internalDirectoryPath)) {

}

void SceneApplication::onPause() {

}

void SceneApplication::onResume(void *env, void *context, void *activity) {

}

void SceneApplication::onSurfaceCreated() {
    objectRenderer.initialize();
}

void SceneApplication::onDisplayGeometryChanged(int displayRotation, int width, int height) {
    objectRenderer.onDisplayGeometryChanged(width, height);
}

void SceneApplication::onDrawFrame() {
    objectRenderer.onDraw();
}
