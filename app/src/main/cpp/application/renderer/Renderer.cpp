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
#include <sstream>

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
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

GLuint gIndices[] = {
        0, 1, 3,
        2, 3, 1
};

Renderer::Renderer(AAssetManager *assetManager) : assetManager(assetManager) {

}

void Renderer::onSurfaceCreated() {
    gShaderProgram = RenderUtils::createProgram(
            assetManager,
            "vertex_shader.glsl",
            "fragment_shader.glsl"
    );

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertices), gVertices, GL_STATIC_DRAW);

    glGenTextures(1, &gTexture1);
    glBindTexture(GL_TEXTURE_2D, gTexture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    RenderUtils::bindTextureSource(GL_TEXTURE_2D, "container.jpg");
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &gTexture2);
    glBindTexture(GL_TEXTURE_2D, gTexture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    RenderUtils::bindTextureSource(GL_TEXTURE_2D, "awesomeface.png");
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gIndices), gIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          8 * sizeof(GLfloat), (GLvoid *) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Renderer::onSurfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
}

void Renderer::onDrawFrame() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gShaderProgram);

    GLint transformLocation = glGetUniformLocation(gShaderProgram, "transform");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTexture1);
    glUniform1i(glGetUniformLocation(gShaderProgram, "texture1"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gTexture2);
    glUniform1i(glGetUniformLocation(gShaderProgram, "texture2"), 1);
    glBindVertexArray(VAO);

    glm::mat4 transform;
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, static_cast<GLfloat>(clock() * 50.0f),
                            glm::vec3(0.0, 0.0, 1.0));

    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    transform = glm::mat4();
    transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
    GLfloat scale = static_cast<GLfloat>(sin(clock()));
    transform = glm::scale(transform, glm::vec3(scale, scale, scale));

    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
