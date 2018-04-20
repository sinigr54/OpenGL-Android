//
// Created by sinigr on 4/15/18.
//

#include "SceneRenderer.h"
#include <string>
#include <sstream>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

GLfloat gVertices[]{
        // positions          // normals           // cubeTexture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

glm::vec3 cubePositions[]{
        glm::vec3(-0.8f, 0.0f, -3.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(0.8f, 0.0f, -3.0f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
};

SceneRenderer::SceneRenderer(AAssetManager *assetManager,
                             Assimp::IOSystem *ioSystem) : BaseRenderer(assetManager) {

    importer.SetIOHandler(ioSystem);
}

SceneRenderer::~SceneRenderer() {

}

void SceneRenderer::onSurfaceCreated() {
    LOGI("%s", "onSurfaceCreated");

    auto modelPath = "model/nanosuit.blend";
    const aiScene *scene =
            importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOGI("%s::%s", "Error::Assimp", importer.GetErrorString());
    }

    sceneShader.init(assetManager, "cube_vertex_shader.glsl", "cube_fragment_shader.glsl");
    lampShader.init(assetManager, "lamp_vertex_shader.glsl", "lamp_fragment_shader.glsl");

    /*Создаем буфер верши и заполняем его вершинами*/
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertices), gVertices, GL_STATIC_DRAW);

    glGenTextures(1, &cubeTexture);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    RenderUtils::bindTextureSource(GL_TEXTURE_2D, "container2.png");
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &cubeSpecularMap);
    glBindTexture(GL_TEXTURE_2D, cubeSpecularMap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    RenderUtils::bindTextureSource(GL_TEXTURE_2D, "container2_specular.png");
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    /*Создаем объект vertex array для куба и делаем ссылки на вершины*/
    glGenVertexArrays(1, &cubeVao);
    glBindVertexArray(cubeVao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (GLvoid *) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void SceneRenderer::onSurfaceChanged(int width, int height) {
    LOGI("%s", "onSurfaceChanged");

    glViewport(0, 0, width, height);
    screenWidth = static_cast<float>(width);
    screenHeight = static_cast<float>(height);
}

void SceneRenderer::onDrawFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view;
    glm::mat4 projection;

    /* Создаем View матрицу камеры */
    view = camera.getViewMatrix();
    projection = glm::perspective(45.0f, screenWidth / screenHeight, 0.1f, 100.0f);

    sceneShader.use();

    glm::mat4 cubeModel;

    /* Параметры материала */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);

    sceneShader.setUniform("material.diffuse", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, cubeSpecularMap);

    sceneShader.setUniform("material.specular", 1);

    sceneShader.setUniform("material.shininess", 32.0f);

    sceneShader.setUniform("viewPosition", camera.getPosition());

    sceneShader.setUniform("view", view);
    sceneShader.setUniform("projection", projection);

    /* Параметры источников света */

    sceneShader.setUniform("directionLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
    sceneShader.setUniform("directionLight.ambient", ambient);
    sceneShader.setUniform("directionLight.diffuse", diffuse);
    sceneShader.setUniform("directionLight.specular", specular);

    for (int i = 0; i < pointLightPositions.size(); ++i) {
        std::string uniformName = "pointLights[" + std::to_string(i) + "]";

        sceneShader.setUniform(uniformName + ".constant", 1.0f);
        sceneShader.setUniform(uniformName + ".linear", 0.09f);
        sceneShader.setUniform(uniformName + ".quadratic", 0.032f);

        sceneShader.setUniform(uniformName + ".ambient", ambient);
        sceneShader.setUniform(uniformName + ".diffuse", diffuse);
        sceneShader.setUniform(uniformName + ".specular", specular);

        sceneShader.setUniform(".position", pointLightPositions[i]);
    }

    sceneShader.setUniform("spotLight.position", camera.getPosition());
    sceneShader.setUniform("spotLight.direction", camera.getFront());
    sceneShader.setUniform("spotLight.ambient", ambient);
    sceneShader.setUniform("spotLight.diffuse", diffuse);
    sceneShader.setUniform("spotLight.specular", specular);
    sceneShader.setUniform("spotLight.constant", 1.0f);
    sceneShader.setUniform("spotLight.linear", 0.09f);
    sceneShader.setUniform("spotLight.quadratic", 0.032f);
    sceneShader.setUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    sceneShader.setUniform("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

    glBindVertexArray(cubeVao);

    for (int i = 0; i < 10; i++) {
        glm::mat4 model;
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
        sceneShader.setUniform("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(0);

    lampShader.use();

    lampShader.setUniform("view", view);
    lampShader.setUniform("projection", projection);

    glBindVertexArray(lightVao);

    for (auto &position : pointLightPositions) {
        glm::mat4 lightModel;

        lightModel = glm::translate(lightModel, position);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));

        lampShader.setUniform("model", lightModel);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(0);

    camera.getPosition().x = static_cast<float>(sin(delta)) * 10.0f;
    camera.getPosition().z = static_cast<float>(cos(delta)) * 10.0f;

    delta += 0.02;
    if (delta > 10000.0) {
        delta = 0.0;
    }
}
