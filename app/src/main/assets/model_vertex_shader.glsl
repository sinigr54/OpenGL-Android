#version 300 es

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 outTexCoords;
out vec3 outNormal;
out vec3 outFragmentPosition;

out vec3 viewPosition;

uniform mat4 mvMat;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    viewPosition = (mvMat * aPos).xyz;
    outTexCoords = aTexCoords;
    outNormal = mat3(transpose(inverse(model))) * aNormal;
    outFragmentPosition = vec3(model * vec4(aPos));
    gl_Position = projection * view * model * vec4(aPos);
}