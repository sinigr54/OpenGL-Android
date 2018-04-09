#version 300 es

in vec3 outNormal;
in vec3 outFragmentPosition;

out vec4 outColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;

void main() {
	float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 normal = normalize(outNormal);
    vec3 lightDirection = normalize(lightPosition - outFragmentPosition);

    float diffuseValue = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diffuseValue * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    outColor = vec4(result, 1.0f);
}
