//
// Created by Alexander Chekh on 15.05.2018.
//

#ifndef OPENGLCOURSE_SCENEAPPLICATION_H
#define OPENGLCOURSE_SCENEAPPLICATION_H


#include "GraphicalApplication.h"

class SceneApplication : public GraphicalApplication {
    ObjectRenderer objectRenderer;

public:
    SceneApplication(AAssetManager *assetManager,
                     const std::string &internalDirectoryPath);

    void onPause() override;

    void onResume(void *env, void *context, void *activity) override;

    void onSurfaceCreated() override;

    void onDisplayGeometryChanged(int displayRotation, int width, int height) override;

    void onDrawFrame() override;
};


#endif //OPENGLCOURSE_SCENEAPPLICATION_H
