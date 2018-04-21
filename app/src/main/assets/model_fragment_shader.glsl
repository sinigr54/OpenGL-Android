#version 300 es
out vec4 color;

in vec2 outTexCoords;
in vec3 outNormal;
in vec3 outFragmentPosition;

struct DirectionLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    float shininess;
};

uniform vec3 viewPosition;
uniform DirectionLight directionLight;
uniform Material material;

void main() {
    vec3 normal = normalize(outNormal);
    vec3 viewDirection = normalize(viewPosition - outFragmentPosition);

    vec3 lightDirection = normalize(-directionLight.direction);
    // диффузное освещение
    float diffuseValue = max(dot(normal, lightDirection), 0.0);
    // освещение зеркальных бликов
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    // комбинируем результаты
    vec3 ambient  = directionLight.ambient  * vec3(texture(material.texture_diffuse1, outTexCoords));
    vec3 diffuse  = directionLight.diffuse  * diffuseValue * vec3(texture(material.texture_diffuse1, outTexCoords));
    vec3 specular = directionLight.specular * specularValue * vec3(texture(material.texture_specular1, outTexCoords));

    vec3 directionLightValue = ambient + diffuse + specular;

    color = vec4(directionLightValue, 1.0f);
}
