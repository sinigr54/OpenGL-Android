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
#include <AndroidJNIIOSystem.h>

ObjectRenderer::ObjectRenderer(AAssetManager *assetManager,
                               const std::string &path) :
        assetManager(assetManager),
        ioSystem(std::make_unique<Assimp::AndroidJNIIOSystem>(assetManager, path)) {
}

void ObjectRenderer::initialize() {
    LOGI("%s", "initialize");

    model = Model{"model/nanosuit.obj", ioSystem.get()};

    shaderProgram.init(assetManager, "model_vertex_shader.glsl", "model_fragment_shader.glsl");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void ObjectRenderer::draw(const glm::mat4 &projectionMat, const glm::mat4 &viewMat,
                          const glm::mat4 &modelMat, const glm::vec3 &viewPosition) {
    shaderProgram.use();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 ambient{glm::vec3{0.4f, 0.4f, 0.4f}};
    glm::vec3 diffuse{glm::vec3{1.0f, 1.0f, 1.0f}};
    glm::vec3 specular{glm::vec3{1.0f, 1.0f, 1.0f}};

    shaderProgram.setUniform("viewPosition", viewPosition);

    shaderProgram.setUniform("directionLight.direction", glm::vec3(0.0f, -0.8f, -1.0f));
    shaderProgram.setUniform("directionLight.ambient", ambient);
    shaderProgram.setUniform("directionLight.diffuse", diffuse);
    shaderProgram.setUniform("directionLight.specular", specular);

    shaderProgram.setUniform("model", modelMat);
    shaderProgram.setUniform("view", viewMat);
    shaderProgram.setUniform("projection", projectionMat);

    model.draw(shaderProgram);
}
