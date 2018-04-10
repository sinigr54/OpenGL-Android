#version 300 es

in vec3 outNormal;
in vec3 outFragmentPosition;
in vec2 outTexCoords;

out vec4 outColor;

uniform vec3 viewPosition;

struct Light {
    // vec3 position;

    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Light light;
uniform Material material;

void main() {
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, outTexCoords));

    vec3 normal = normalize(outNormal);
    vec3 lightDirection = normalize(-light.direction);

    float diffuseValue = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diffuseValue * vec3(texture(material.diffuse, outTexCoords));

    vec3 viewDirection = normalize(viewPosition - outFragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);

    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = light.specular * specularValue * vec3(texture(material.specular, outTexCoords));

    vec3 result = ambient + diffuse + specular;
    outColor = vec4(result, 1.0f);
}
