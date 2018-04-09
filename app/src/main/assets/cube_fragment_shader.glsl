#version 300 es

in vec3 outNormal;
in vec3 outFragmentPosition;

out vec4 outColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main() {
	float ambientStrength = 0.2f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 normal = normalize(outNormal);
    vec3 lightDirection = normalize(lightPosition - outFragmentPosition);

    float diffuseValue = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diffuseValue * lightColor;

    float specularStrenght = 1.0f;

    vec3 viewDirection = normalize(viewPosition - outFragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), 32.0f);
    vec3 specular = specularStrenght * specularValue * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    outColor = vec4(result, 1.0f);
}
