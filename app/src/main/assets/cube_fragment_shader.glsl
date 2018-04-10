#version 300 es

in vec3 outNormal;
in vec3 outFragmentPosition;

out vec4 outColor;

uniform vec3 viewPosition;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Light light;
uniform Material material;

void main() {
    vec3 ambient = light.ambient * material.ambient;

    vec3 normal = normalize(outNormal);
    vec3 lightDirection = normalize(light.position - outFragmentPosition);

    float diffuseValue = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * (diffuseValue * material.diffuse);

    vec3 viewDirection = normalize(viewPosition - outFragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = light.specular * (specularValue * material.specular);

    vec3 result = ambient + diffuse + specular;
    outColor = vec4(result, 1.0f);
}
