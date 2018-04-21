#version 300 es

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 outTexCoords;
out vec3 outNormal;
out vec3 outFragmentPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    outTexCoords = aTexCoords;
    outNormal = mat3(transpose(inverse(model))) * aNormal;
    outFragmentPosition = vec3(model * vec4(aPos, 1.0f));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}