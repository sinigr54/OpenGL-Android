//
// Created by sinigr on 3/21/18.
//

#include <GLES3/gl3.h>
#include <utils/ShaderUtils.h>
#include "Renderer.h"

constexpr char vertexShaderSource[] = R"(
#version 300 es

layout (location = 0) in vec3 position;

void main() {
   gl_Position = vec4(position.x, position.y, position.z, 1.0f);
})";

constexpr char fragmentShaderSource[] = R"(
#version 300 es

out vec4 color;

void main()
{
	color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
})";

GLuint gShaderProgram = 0;
GLuint VBO = 0;
GLuint VAO = 0;
GLuint IBO = 0;

GLfloat vertices[] = {
        0.5f, 0.5f, 0.0f,  // Верхний правый угол
        0.5f, -0.5f, 0.0f,  // Нижний правый угол
        -0.5f, -0.5f, 0.0f,  // Нижний левый угол
        -0.5f, 0.5f, 0.0f   // Верхний левый угол
};

GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
};

void Renderer::onSurfaceCreated() {
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    gShaderProgram = ShaderUtils::createProgram(vertexShaderSource, fragmentShaderSource);
}

void Renderer::onSurfaceChanged(int width, int height) {
    glViewport(0, 0, width, height);
}

void Renderer::onDrawFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gShaderProgram);
    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}
