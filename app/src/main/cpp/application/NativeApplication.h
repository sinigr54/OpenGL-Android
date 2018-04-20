//
// Created by sinigr on 3/21/18.
//

#ifndef OPENGLCOURSE_NATIVEAPPLICATION_H
#define OPENGLCOURSE_NATIVEAPPLICATION_H

#include <android/asset_manager.h>
#include <memory>
#include <assimp/IOSystem.hpp>

#include "application/renderer/base/BaseRenderer.h"

class NativeApplication {
    std::unique_ptr<BaseRenderer> pRenderer;

public:
    NativeApplication() = default;

    NativeApplication(AAssetManager *assetManager, Assimp::IOSystem *ioSystem);

    ~NativeApplication();

    void onPause();

    void onResume();

    void onSurfaceCreated();

    void onSurfaceChanged(int width, int height);

    void onDrawFrame();
};


#endif //OPENGLCOURSE_NATIVEAPPLICATION_H
