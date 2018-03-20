//
// Created by sinigr on 3/21/18.
//

#ifndef OPENGLCOURSE_NATIVEAPPLICATION_H
#define OPENGLCOURSE_NATIVEAPPLICATION_H

#include <android/asset_manager.h>
#include <memory>

#include "renderer/Renderer.h"

class NativeApplication {
    AAssetManager const* assetManager;
    Renderer renderer;

public:
    NativeApplication() = default;

    NativeApplication(AAssetManager *assetManager);

    ~NativeApplication();

    void onPause();

    void onResume();

    void onSurfaceCreated();

    void onSurfaceChanged(int width, int height);

    void onDrawFrame();
};


#endif //OPENGLCOURSE_NATIVEAPPLICATION_H
