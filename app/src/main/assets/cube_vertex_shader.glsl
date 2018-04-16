#version 300 es

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 outFragmentPosition;
out vec3 outNormal;
out vec2 outTexCoords;

void main() {
   outNormal = mat3(transpose(inverse(model))) * inNormal;
   outFragmentPosition = vec3(model * vec4(inPosition, 1.0f));
   outTexCoords = inTexCoords;

   gl_Position = projection * view * model * vec4(inPosition, 1.0f);
}
