#version 300 es

in vec3 outNormal;
in vec3 outFragmentPosition;
in vec2 outTexCoords;

out vec4 outColor;

struct DirectionLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform vec3 viewPosition;
uniform DirectionLight directionLight;
uniform PointLight pointLights[3];
uniform SpotLight spotLight;
uniform Material material;

vec3 calculateDirectionLight(DirectionLight light, vec3 normal, vec3 viewDirection);

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDirection);

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection);

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
    vec3 ambient  = directionLight.ambient  * vec3(texture(material.diffuse, outTexCoords));
    vec3 diffuse  = directionLight.diffuse  * diffuseValue * vec3(texture(material.diffuse, outTexCoords));
    vec3 specular = directionLight.specular * specularValue * vec3(texture(material.specular, outTexCoords));

    vec3 directionLightValue = ambient + diffuse + specular;

    vec3 pointLightValue = vec3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 3; i++) {
        vec3 lightDirection = normalize(pointLights[i].position - outFragmentPosition);
        // диффузное освещение
        float diffuseValue = max(dot(normal, lightDirection), 0.0);
        // освещение зеркальных бликов
        vec3 reflectDirection = reflect(-lightDirection, normal);
        float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
        // затухание
        float distance    = length(pointLights[i].position - outFragmentPosition);
        float attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance +
                     pointLights[i].quadratic * (distance * distance));
        // комбинируем результаты
        vec3 ambient  = pointLights[i].ambient  * vec3(texture(material.diffuse, outTexCoords));
        vec3 diffuse  = pointLights[i].diffuse  * diffuseValue * vec3(texture(material.diffuse, outTexCoords));
        vec3 specular = pointLights[i].specular * specularValue * vec3(texture(material.specular, outTexCoords));

        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;

        pointLightValue += ambient + diffuse + specular;
    }

    ambient = spotLight.ambient * vec3(texture(material.diffuse, outTexCoords));

    lightDirection = normalize(spotLight.position - outFragmentPosition);

    diffuseValue = max(dot(normal, lightDirection), 0.0);
    diffuse = spotLight.diffuse * diffuseValue * vec3(texture(material.diffuse, outTexCoords));

    reflectDirection = reflect(-lightDirection, normal);

    specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    specular = spotLight.specular * specularValue * vec3(texture(material.specular, outTexCoords));

    float distance = length(spotLight.position - outFragmentPosition);
    float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance +
                    spotLight.quadratic * (distance * distance));

    float theta = dot(lightDirection, normalize(-spotLight.direction));
    float epsilon = (spotLight.cutOff - spotLight.outerCutOff);
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 spotLightValue = ambient + diffuse + specular;

    outColor = vec4(directionLightValue + pointLightValue + spotLightValue, 1.0f);
}

vec3 calculateDirectionLight(DirectionLight light, vec3 normal, vec3 viewDirection) {
    vec3 lightDirection = normalize(-light.direction);
    // диффузное освещение
    float diffuseValue = max(dot(normal, lightDirection), 0.0);
    // освещение зеркальных бликов
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(dot(viewDirection, reflectionDirection), 0.0), material.shininess);
    // комбинируем результаты
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, outTexCoords));
    vec3 diffuse  = light.diffuse  * diffuseValue * vec3(texture(material.diffuse, outTexCoords));
    vec3 specular = light.specular * specularValue * vec3(texture(material.specular, outTexCoords));

    return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewDirection) {
    vec3 lightDirection = normalize(light.position - outFragmentPosition);
    // диффузное освещение
    float diffuseValue = max(dot(normal, lightDirection), 0.0);
    // освещение зеркальных бликов
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    // затухание
    float distance    = length(light.position - outFragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                 light.quadratic * (distance * distance));
    // комбинируем результаты
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, outTexCoords));
    vec3 diffuse  = light.diffuse  * diffuseValue * vec3(texture(material.diffuse, outTexCoords));
    vec3 specular = light.specular * specularValue * vec3(texture(material.specular, outTexCoords));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragmentPosition, vec3 viewDirection) {
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, outTexCoords));

    vec3 lightDirection = normalize(light.position - fragmentPosition);

    float diffuseValue = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diffuseValue * vec3(texture(material.diffuse, outTexCoords));

    vec3 reflectDirection = reflect(-lightDirection, normal);

    float specularValue = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
    vec3 specular = light.specular * specularValue * vec3(texture(material.specular, outTexCoords));

    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                    light.quadratic * (distance * distance));

    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    diffuse *= intensity;
    specular *= intensity;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}