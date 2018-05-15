//
// Created by Alexander Chekh on 15.05.2018.
//

#include "SceneApplication.h"

#include <AndroidJNIIOSystem.h>

SceneApplication::SceneApplication(AAssetManager *assetManager,
                                   const std::string &internalDirectory) :
        GraphicalApplication(assetManager, internalDirectory),
        objectRenderer(assetManager, internalDirectory) {

}

void SceneApplication::onPause() {

}

void SceneApplication::onResume(void *env, void *context, void *activity) {

}

void SceneApplication::onSurfaceCreated() {
    objectRenderer.initialize();
}

void SceneApplication::onDisplayGeometryChanged(int displayRotation, int width, int height) {
    glViewport(0, 0, width, height);
    screenWidth = static_cast<float>(width);
    screenHeight = static_cast<float>(height);
}

void SceneApplication::onDrawFrame() {
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;

    model = glm::translate(model, glm::vec3(0.0f, -2.8f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

    /* Создаем View матрицу камеры */
    view = camera.getViewMatrix();

    projection = glm::perspective(45.0f, screenWidth / screenHeight, 0.1f, 100.0f);

    objectRenderer.draw(projection, view, model, camera.getPosition());

    camera.getPosition().x = static_cast<float>(sin(delta)) * 10.0f;
    camera.getPosition().z = static_cast<float>(cos(delta)) * 10.0f;

    delta += 0.02;
    if (delta > 10000.0) {
        delta = 0.0;
    }
}
