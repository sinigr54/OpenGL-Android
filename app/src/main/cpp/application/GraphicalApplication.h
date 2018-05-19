//
// Created by sinigr on 3/21/18.
//

#ifndef OPENGLCOURSE_NATIVEAPPLICATION_H
#define OPENGLCOURSE_NATIVEAPPLICATION_H

#include <android/asset_manager.h>
#include <memory>
#include <application/renderer/ObjectRenderer.h>
#include <application/lifecycle/LifecycleListener.h>

class GraphicalApplication : public LifecycleListener {
    AAssetManager *assetManager;
    std::string internalDirectory;

public:

    GraphicalApplication() = delete;

    GraphicalApplication(AAssetManager *assetManager,
                      const std::string &internalDirectory);

    GraphicalApplication(const GraphicalApplication &other) = delete;

    GraphicalApplication &operator=(const GraphicalApplication &other) = delete;

    virtual ~GraphicalApplication();

    virtual void onSurfaceCreated() = 0;

    virtual void onDisplayGeometryChanged(int displayRotation, int width, int height) = 0;

    virtual void onDrawFrame() = 0;

    virtual void onTouched(float x, float y) = 0;
};


#endif //OPENGLCOURSE_NATIVEAPPLICATION_H
