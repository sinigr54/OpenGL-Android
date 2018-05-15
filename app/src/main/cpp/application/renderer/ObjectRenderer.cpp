//
// Created by sinigr on 4/15/18.
//

#include "ObjectRenderer.h"
#include "application/renderer/model/Model.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

ObjectRenderer::ObjectRenderer(AAssetManager *assetManager,
                             Assimp::IOSystem *ioSystem) :
        assetManager(assetManager),
        ioSystem(ioSystem) {
}

void ObjectRenderer::initialize() {
    LOGI("%s", "initialize");

    models.clear();
    models.emplace_back(Model{"model/nanosuit.obj", ioSystem.get()});

    sceneShader.init(assetManager, "model_vertex_shader.glsl", "model_fragment_shader.glsl");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void ObjectRenderer::onDisplayGeometryChanged(int width, int height) {
    LOGI("%s", "onDisplayGeometryChanged");

    glViewport(0, 0, width, height);
    screenWidth = static_cast<float>(width);
    screenHeight = static_cast<float>(height);
}

void ObjectRenderer::onDraw() {
    sceneShader.use();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 model;

    model = glm::translate(model, glm::vec3(0.0f, -2.8f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

    /* Создаем View матрицу камеры */
    view = camera.getViewMatrix();

    projection = glm::perspective(45.0f, screenWidth / screenHeight, 0.1f, 100.0f);

    glm::vec3 ambient{glm::vec3{0.4f, 0.4f, 0.4f}};
    glm::vec3 diffuse{glm::vec3{1.0f, 1.0f, 1.0f}};
    glm::vec3 specular{glm::vec3{1.0f, 1.0f, 1.0f}};

    sceneShader.setUniform("viewPosition", camera.getPosition());

    sceneShader.setUniform("directionLight.direction", glm::vec3(0.0f, -0.8f, -1.0f));
    sceneShader.setUniform("directionLight.ambient", ambient);
    sceneShader.setUniform("directionLight.diffuse", diffuse);
    sceneShader.setUniform("directionLight.specular", specular);

    sceneShader.setUniform("model", model);
    sceneShader.setUniform("view", view);
    sceneShader.setUniform("projection", projection);

    std::for_each(models.cbegin(), models.cend(), [this](const auto &model) {
        model.draw(this->sceneShader);
    });

    camera.getPosition().x = static_cast<float>(sin(delta)) * 10.0f;
    camera.getPosition().z = static_cast<float>(cos(delta)) * 10.0f;

    delta += 0.02;
    if (delta > 10000.0) {
        delta = 0.0;
    }
}
