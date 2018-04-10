//
// Created by sinigr on 3/21/18.
//

#include <GLES3/gl3.h>
#include "utils/RenderUtils.h"
#include "utils/Random.h"
#include "Renderer.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <ctime>
#include <vector>
#include <sstream>
#include "application/renderer/camera/Camera.h"
#include "shader/Shader.h"
#include <memory>

const std::string TAG = "Renderer";

GLuint VBO = 0;
GLuint cubeVao = 0;
GLuint lightVao = 0;
GLuint IBO = 0;

GLfloat gVertices[] = {
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

glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
};

float screenWidth = 0.0f;
float screenHeight = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

glm::vec3 lightPosition(0.0f, 1.0f, 4.0f);

double delta = 0.0;

GLuint cubeTexture;
GLuint cubeSpecularMap;

Renderer::Renderer(AAssetManager *assetManager) :
        assetManager(assetManager) {

}

void Renderer::onSurfaceCreated() {
    cubeShader.init(assetManager, "cube_vertex_shader.glsl", "cube_fragment_shader.glsl");
    lampShader.init(assetManager, "lamp_vertex_shader.glsl", "light_fragment_shader.glsl");

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
}

void Renderer::onSurfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
    screenWidth = static_cast<float>(width);
    screenHeight = static_cast<float>(height);
}

void Renderer::onDrawFrame() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view;
    glm::mat4 projection;

    /* Создаем View матрицу камеры */
    view = camera.getViewMatrix();
    projection = glm::perspective(45.0f, screenWidth / screenHeight, 0.1f, 100.0f);

    cubeShader.use();

    glm::mat4 cubeModel;

    /* Параметры источника света */
    cubeShader.setUniform("light.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
    cubeShader.setUniform("light.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));
    cubeShader.setUniform("light.specular", glm::vec3(2.0f, 2.0f, 2.0f));

    /* Параметры материала */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);

    cubeShader.setUniform("material.diffuse", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, cubeSpecularMap);

    cubeShader.setUniform("material.specular", 1);
    cubeShader.setUniform("material.shininess", 64.0f);

    cubeShader.setUniform("light.position", lightPosition);
    cubeShader.setUniform("light.direction", glm::vec3(0.0f, 0.2f, -0.8f));

    cubeShader.setUniform("viewPosition", camera.getPosition());

    cubeShader.setUniform("view", view);
    cubeShader.setUniform("projection", projection);

    cubeShader.setUniform("light.constant", 1.0f);
    cubeShader.setUniform("light.linear", 0.09f);
    cubeShader.setUniform("light.quadratic", 0.032f);

    glBindVertexArray(cubeVao);

    for (int i = 0; i < 10; i++) {
        glm::mat4 model;
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
        cubeShader.setUniform("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(0);

    lampShader.use();

    glm::mat4 lightModel;

    lightModel = glm::translate(lightModel, lightPosition);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));

    lampShader.setUniform("view", view);
    lampShader.setUniform("projection", projection);
    lampShader.setUniform("model", lightModel);

    glBindVertexArray(lightVao);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);

    camera.getPosition().x = static_cast<float>(sin(delta)) * 10.0f;
    camera.getPosition().z = static_cast<float>(cos(delta)) * 10.0f;

    delta += 0.02;
    if (delta > 10000.0) {
        delta = 0.0;
    }
}
