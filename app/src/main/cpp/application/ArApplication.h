//
// Created by Alexander Chekh on 15.05.2018.
//

#ifndef OPENGLCOURSE_ARAPPLICATION_H
#define OPENGLCOURSE_ARAPPLICATION_H


#include <application/renderer/BackgroundRenderer.h>
#include "GraphicalApplication.h"

#include <vector>
#include <unordered_map>
#include <application/renderer/PlaneRenderer.h>

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
    int planeCount = 0;

    BackgroundRenderer backgroundRenderer;
    ObjectRenderer objectRenderer;
    PlaneRenderer planeRenderer;

    Camera camera{glm::vec3(0.0f, 0.0f, 8.0f)};

public:

    ArApplication(AAssetManager *assetManager,
                  const std::string &internalDirectory);

    ~ArApplication();

    void onPause() override;

    void onResume(void *env, void *context, void *activity) override;

    void onSurfaceCreated() override;

    void onDisplayGeometryChanged(int displayRotation, int width, int height) override;

    void onDrawFrame() override;

    void onTouched(float x, float y) override;
};


#endif //OPENGLCOURSE_ARAPPLICATION_H
