//
// Created by Alexander Chekh on 15.05.2018.
//

#include <array>
#include "ArApplication.h"

#include "glm.hpp"

constexpr size_t gMaxNumberOfAndroidsToRender = 20;
const glm::vec3 gWhite = {255, 255, 255};

constexpr int32_t gPlaneColorRgbaSize = 16;

constexpr std::array<uint32_t, gPlaneColorRgbaSize> gPlaneColorRgba = {
        {0xFFFFFFFF, 0xF44336FF, 0xE91E63FF, 0x9C27B0FF, 0x673AB7FF, 0x3F51B5FF,
                0x2196F3FF, 0x03A9F4FF, 0x00BCD4FF, 0x009688FF, 0x4CAF50FF, 0x8BC34AFF,
                0xCDDC39FF, 0xFFEB3BFF, 0xFFC107FF, 0xFF9800FF}};

inline glm::vec3 getRandomPlaneColor() {
    const int32_t colorRgba = gPlaneColorRgba[std::rand() % gPlaneColorRgbaSize];
    return glm::vec3(((colorRgba >> 24) & 0xff) / 255.0f,
                     ((colorRgba >> 16) & 0xff) / 255.0f,
                     ((colorRgba >> 8) & 0xff) / 255.0f);
};

ArApplication::ArApplication(AAssetManager *assetManager,
                             const std::string &internalDirectory) :
        GraphicalApplication(assetManager, internalDirectory),
        objectRenderer(assetManager, internalDirectory) {

}

ArApplication::~ArApplication() {
    if (arSession != nullptr && arFrame != nullptr) {
        ArSession_destroy(arSession);
        ArFrame_destroy(arFrame);
    }
}

void ArApplication::onPause() {
    if (arSession != nullptr) {
        ArSession_pause(arSession);
    }
}

void ArApplication::onResume(void *env, void *context, void *activity) {
    if (arSession == nullptr) {
        ArInstallStatus installStatus;
        // If install was not yet requested, that means that we are resuming the
        // activity first time because of explicit user interaction (such as
        // launching the application)
        bool userRequestedInstall = !isInstallRequested;

        // === ATTENTION!  ATTENTION!  ATTENTION! ===
        // This method can and will fail in user-facing situations.  Your
        // application must handle these cases at least somewhat gracefully.  See
        // HelloAR Java sample code for reasonable behavior.
        CHECK(ArCoreApk_requestInstall(env, activity, userRequestedInstall,
                                       &installStatus) == AR_SUCCESS);

        ArCoreApk_requestInstall(env, activity, userRequestedInstall, &installStatus);

        switch (installStatus) {
            case AR_INSTALL_STATUS_INSTALLED:
                break;
            case AR_INSTALL_STATUS_INSTALL_REQUESTED:
                isInstallRequested = true;
                return;
        }

        // === ATTENTION!  ATTENTION!  ATTENTION! ===
        // This method can and will fail in user-facing situations.  Your
        // application must handle these cases at least somewhat gracefully.  See
        // HelloAR Java sample code for reasonable behavior.
        ArSession_create(env, context, &arSession);
        CHECK(arSession);

        ArFrame_create(arSession, &arFrame);
        CHECK(arFrame);

        ArSession_setDisplayGeometry(arSession, displayRotation, screenWidth,
                                     screenHeight);
    }

    const ArStatus status = ArSession_resume(arSession);
    CHECK(status == AR_SUCCESS);
}

void ArApplication::onSurfaceCreated() {
    objectRenderer.initialize();
    backgroundRenderer.initialize();
    planeRenderer.initialize();
}

void ArApplication::onDisplayGeometryChanged(int displayRotation, int width, int height) {
    glViewport(0, 0, width, height);
    this->displayRotation = displayRotation;
    screenWidth = width;
    screenHeight = height;

    if (arSession != nullptr) {
        ArSession_setDisplayGeometry(arSession, displayRotation, screenWidth,
                                     screenHeight);
    }
}

void ArApplication::onDrawFrame() {
//    LOGI("%s", "ArApplication::onDrawFrame");

    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (arSession == nullptr) {
        return;
    }

    ArSession_setCameraTextureName(arSession, backgroundRenderer.getTextureId());

    if (ArSession_update(arSession, arFrame) != AR_SUCCESS) {
        LOGE("ArApplication::onDrawFrame ArSession_update error!");
    }

    ArCamera *arCamera;
    ArFrame_acquireCamera(arSession, arFrame, &arCamera);

    glm::mat4 viewMat;
    glm::mat4 projectionMat;

    ArCamera_getViewMatrix(arSession, arCamera, glm::value_ptr(viewMat));
    ArCamera_getProjectionMatrix(arSession, arCamera,
                                 0.1f, 100.0f,
                                 glm::value_ptr(projectionMat));

    ArTrackingState cameraTrackingState;
    ArCamera_getTrackingState(arSession, arCamera, &cameraTrackingState);
    ArCamera_release(arCamera);

    backgroundRenderer.draw(arSession, arFrame);

    // If the camera isn't tracking don't bother rendering other objects.
    if (cameraTrackingState != AR_TRACKING_STATE_TRACKING) {
        return;
    }

    // Get light estimation value.
    ArLightEstimate *arLightEstimate;
    ArLightEstimateState arLightEstimateState;
    ArLightEstimate_create(arSession, &arLightEstimate);

    ArFrame_getLightEstimate(arSession, arFrame, arLightEstimate);
    ArLightEstimate_getState(arSession, arLightEstimate,
                             &arLightEstimateState);

    // Set light intensity to default. Intensity value ranges from 0.0f to 1.0f.
    // The first three components are color scaling factors.
    // The last one is the average pixel intensity in gamma space.
    float colorCorrection[4] = {1.f, 1.f, 1.f, 1.f};
    if (arLightEstimateState == AR_LIGHT_ESTIMATE_STATE_VALID) {
        ArLightEstimate_getColorCorrection(arSession, arLightEstimate,
                                           colorCorrection);
    }

    ArLightEstimate_destroy(arLightEstimate);
    arLightEstimate = nullptr;

    // Render objects.
    glm::mat4 modelMat(1.0f);

    for (const auto &iterator : trackedObjects) {

        ArTrackingState tracking_state = AR_TRACKING_STATE_STOPPED;
        ArAnchor_getTrackingState(arSession, iterator, &tracking_state);
        if (tracking_state == AR_TRACKING_STATE_TRACKING) {
            // Render object only if the tracking state is AR_TRACKING_STATE_TRACKING.
            getTransformMatrixFromAnchor(arSession, iterator, &modelMat);
            modelMat = glm::scale(modelMat, glm::vec3(0.05f, 0.05f, 0.05f));
            objectRenderer.drawAr(projectionMat, viewMat, modelMat
/*, colorCorrection */);
        }
    }

    // Update and render planes.
    ArTrackableList *planeList = nullptr;
    ArTrackableList_create(arSession, &planeList);
    CHECK(planeList != nullptr);

    ArTrackableType planeTrackedType = AR_TRACKABLE_PLANE;
    ArSession_getAllTrackables(arSession, planeTrackedType, planeList);

    int32_t planeListSize = 0;
    ArTrackableList_getSize(arSession, planeList, &planeListSize);
    planeCount = planeListSize;

    for (int i = 0; i < planeListSize; ++i) {
        ArTrackable *arTrackable = nullptr;
        ArTrackableList_acquireItem(arSession, planeList, i, &arTrackable);
        ArPlane *arPlane = ArAsPlane(arTrackable);
        ArTrackingState outTrackingState;
        ArTrackable_getTrackingState(arSession, arTrackable,
                                     &outTrackingState);

        ArPlane *subsumePlane;
        ArPlane_acquireSubsumedBy(arSession, arPlane, &subsumePlane);
        if (subsumePlane != nullptr) {
            ArTrackable_release(ArAsTrackable(subsumePlane));
            continue;
        }

        if (ArTrackingState::AR_TRACKING_STATE_TRACKING != outTrackingState) {
            continue;
        }

        ArTrackingState planeTrackingState;
        ArTrackable_getTrackingState(arSession, ArAsTrackable(arPlane),
                                     &planeTrackingState);

        if (planeTrackingState == AR_TRACKING_STATE_TRACKING) {
            const auto iterator = planeColorMap.find(arPlane);
            glm::vec3 color;
            if (iterator != planeColorMap.end()) {
                color = iterator->second;

                // If this is an already observed trackable release it so it doesn't
                // leave an additional reference dangling.
                ArTrackable_release(arTrackable);
            } else {
                // The first plane is always white.
                if (!isFistPlaneHasBeenDetected) {
                    isFistPlaneHasBeenDetected = true;
                    color = gWhite;
                } else {
                    color = getRandomPlaneColor();
                }
                planeColorMap.insert({arPlane, color});
            }

            planeRenderer.draw(projectionMat, viewMat, arSession, arPlane,
                               color);
        }
    }

    ArTrackableList_destroy(planeList);
    planeList = nullptr;
}

void ArApplication::onTouched(float x, float y) {
    LOGI("%s: (%f, %f)", "onTouched", x, y);

    if (arFrame != nullptr && arSession != nullptr) {
        ArHitResultList *hitResultList = nullptr;
        ArHitResultList_create(arSession, &hitResultList);
        CHECK(hitResultList);
        ArFrame_hitTest(arSession, arFrame, x, y, hitResultList);

        int32_t hitResultListSize = 0;
        ArHitResultList_getSize(arSession, hitResultList,
                                &hitResultListSize);

        // The hitTest method sorts the resulting list by distance from the camera,
        // increasing.  The first hit result will usually be the most relevant when
        // responding to user input.

        ArHitResult *arHitResult = nullptr;
        for (int32_t i = 0; i < hitResultListSize; ++i) {
            ArHitResult *arHit = nullptr;
            ArHitResult_create(arSession, &arHit);
            ArHitResultList_getItem(arSession, hitResultList, i, arHit);

            if (arHit == nullptr) {
                LOGE("HelloArApplication::OnTouched ArHitResultList_getItem error");
                return;
            }

            ArTrackable *arTrackable = nullptr;
            ArHitResult_acquireTrackable(arSession, arHit, &arTrackable);
            ArTrackableType arTrackableType = AR_TRACKABLE_NOT_VALID;
            ArTrackable_getType(arSession, arTrackable, &arTrackableType);
            // Creates an anchor if a plane or an oriented point was hit.
            if (AR_TRACKABLE_PLANE == arTrackableType) {
                ArPose *hitPose = nullptr;
                ArPose_create(arSession, nullptr, &hitPose);
                ArHitResult_getHitPose(arSession, arHit, hitPose);
                int32_t inPolygon = 0;
                ArPlane *arPlane = ArAsPlane(arTrackable);
                ArPlane_isPoseInPolygon(arSession, arPlane, hitPose, &inPolygon);

                // Use hit pose and camera pose to check if hittest is from the
                // back of the plane, if it is, no need to create the anchor.
                ArPose *cameraPose = nullptr;
                ArPose_create(arSession, nullptr, &cameraPose);
                ArCamera *arCamera;
                ArFrame_acquireCamera(arSession, arFrame, &arCamera);
                ArCamera_getPose(arSession, arCamera, cameraPose);
                ArCamera_release(arCamera);
                float normalDistanceToPlane = calculateDistanceToPlane(
                        arSession, *hitPose, *cameraPose);

                ArPose_destroy(hitPose);
                ArPose_destroy(cameraPose);

                if (!inPolygon || normalDistanceToPlane < 0) {
                    continue;
                }

                arHitResult = arHit;
                break;
            } else if (AR_TRACKABLE_POINT == arTrackableType) {
                ArPoint *arPoint = ArAsPoint(arTrackable);
                ArPointOrientationMode mode;
                ArPoint_getOrientationMode(arSession, arPoint, &mode);
                if (AR_POINT_ORIENTATION_ESTIMATED_SURFACE_NORMAL == mode) {
                    arHitResult = arHit;
                    break;
                }
            }
        }

        if (arHitResult) {
            // Note that the application is responsible for releasing the anchor
            // pointer after using it. Call ArAnchor_release(anchor) to release.
            ArAnchor *anchor = nullptr;
            if (ArHitResult_acquireNewAnchor(arSession, arHitResult, &anchor) !=
                AR_SUCCESS) {
                LOGE(
                        "HelloArApplication::OnTouched ArHitResult_acquireNewAnchor error");
                return;
            }

            ArTrackingState trackingState = AR_TRACKING_STATE_STOPPED;
            ArAnchor_getTrackingState(arSession, anchor, &trackingState);
            if (trackingState != AR_TRACKING_STATE_TRACKING) {
                ArAnchor_release(anchor);
                return;
            }

            if (trackedObjects.size() >= gMaxNumberOfAndroidsToRender) {
                ArAnchor_release(trackedObjects[0]);
                trackedObjects.erase(trackedObjects.begin());
            }

            trackedObjects.push_back(anchor);
            ArHitResult_destroy(arHitResult);
            arHitResult = nullptr;

            ArHitResultList_destroy(hitResultList);
            hitResultList = nullptr;
        }
    }
}
