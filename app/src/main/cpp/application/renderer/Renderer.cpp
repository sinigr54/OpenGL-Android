//
// Created by sinigr on 3/21/18.
//

#include <GLES3/gl3.h>
#include <utils/RenderUtils.h>
#include <utils/Random.h>
#include "Renderer.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <ctime>
#include <vector>
#include <sstream>
#include <application/renderer/camera/Camera.h>

const std::string TAG = "Rendered";

GLuint gShaderProgram = 0;

GLuint VBO = 0;
GLuint VAO = 0;
GLuint IBO = 0;

GLuint gTexture1 = 0;
GLuint gTexture2 = 0;

/*
 * 4 x (3 координаты вершин, 3 параметра цвета, 2 координаты текстуры)
 * */
GLfloat gVertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

std::vector<glm::vec3> gCubePosition = {
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

GLuint gIndices[] = {
        0, 1, 3,
        2, 3, 1
};

float screenWidth = 0.0f;
float screenHeight = 0.0f;

const Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

Renderer::Renderer(AAssetManager *assetManager) : assetManager(assetManager) {

}

void Renderer::onSurfaceCreated() {
    gShaderProgram = RenderUtils::createProgram(
            assetManager,
            "vertex_shader.glsl",
            "fragment_shader.glsl"
    );

    /*Создаем буфер верши и заполняем его вершинами*/
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertices), gVertices, GL_STATIC_DRAW);

    /*Создаем текстуру, подключаемся к ней и инициализируем*/
    glGenTextures(1, &gTexture1);
    glBindTexture(GL_TEXTURE_2D, gTexture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    RenderUtils::bindTextureSource(GL_TEXTURE_2D, "container.jpg");
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    /*Создаем текстуру, подключаемся к ней и инициализируем*/
    glGenTextures(1, &gTexture2);
    glBindTexture(GL_TEXTURE_2D, gTexture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    RenderUtils::bindTextureSource(GL_TEXTURE_2D, "awesomeface.png");
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    /*Создаем объект vertex array и делаем ссылки на вершины*/
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

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

    glUseProgram(gShaderProgram);

    GLint modelLocation = glGetUniformLocation(gShaderProgram, "model");
    GLint viewLocation = glGetUniformLocation(gShaderProgram, "view");
    GLint projectionLocation = glGetUniformLocation(gShaderProgram, "projection");

    /*Активируем текстуру 0 и 1 подключаем каждую к шейдеру*/
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexture1);
    glUniform1i(glGetUniformLocation(gShaderProgram, "texture1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gTexture2);
    glUniform1i(glGetUniformLocation(gShaderProgram, "texture2"), 1);

    glm::mat4 view;
    glm::mat4 projection;
//    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));

    /* Создаем View матрицу камеры */
    view = camera.getViewMatrix();

    projection = glm::perspective(45.0f, screenWidth / screenHeight,
                                  0.1f, 100.0f);

    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(VAO);

    /* Для каждого объекта создаем свою матрицу модели и рисуем объект по очереди */
    for (int i = 0; i < gCubePosition.size(); ++i) {
        glm::mat4 model;
        model = glm::translate(model, gCubePosition[i]);
        GLfloat angle = 20.0f * i;
        if ((i + 1) != 1 && (i + 1) % 3 != 0) {
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.5f));
        } else {
            model = glm::rotate(model, clock() * 50.0f, glm::vec3(1.0f, 0.0f, 0.5f));
        }
        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(0);
}
