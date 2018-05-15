//
// Created by Alexander Chekh on 15.05.2018.
//

#include "ArApplication.h"

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
    backgroundRenderer.initialize();
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

    backgroundRenderer.draw(arSession, arFrame);
}
