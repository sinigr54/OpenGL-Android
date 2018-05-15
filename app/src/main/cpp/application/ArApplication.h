//
// Created by Alexander Chekh on 15.05.2018.
//

#ifndef OPENGLCOURSE_ARAPPLICATION_H
#define OPENGLCOURSE_ARAPPLICATION_H


#include <application/renderer/BackgroundRenderer.h>
#include "GraphicalApplication.h"

#include <vector>
#include <unordered_map>

class ArApplication : public GraphicalApplication {
    ArSession *arSession = nullptr;
    ArFrame *arFrame = nullptr;

    bool isInstallRequested = false;

    int screenWidth;
    int screenHeight;

    int displayRotation = 0;

    std::vector<ArAnchor*> trackedObjects;

    std::unordered_map<ArPlane*, glm::vec3> planeColorMap;

    bool isFistPlaneHasBeenDetected = false;

    BackgroundRenderer backgroundRenderer;
    ObjectRenderer objectRenderer;

public:

    ArApplication(AAssetManager *assetManager,
                  const std::string &internalDirectory);

    ~ArApplication();

    void onPause() override;

    void onResume(void *env, void *context, void *activity) override;

    void onSurfaceCreated() override;

    void onDisplayGeometryChanged(int displayRotation, int width, int height) override;

    void onDrawFrame() override;
};


#endif //OPENGLCOURSE_ARAPPLICATION_H
