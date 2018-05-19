//
// Created by sinigr on 4/15/18.
//

#include "ObjectRenderer.h"
#include "application/renderer/model/Model.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <AndroidJNIIOSystem.h>

/* --------------- TEMPORARY --------------- */

const glm::vec4 gLightDirection(0.0f, 1.0f, 0.0f, 0.0f);

constexpr char gVertexShader[] = R"(
uniform mat4 u_ModelView;
uniform mat4 u_ModelViewProjection;

attribute vec4 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_TexCoord;

varying vec3 v_ViewPosition;
varying vec3 v_ViewNormal;
varying vec2 v_TexCoord;

void main() {
    v_ViewPosition = (u_ModelView * a_Position).xyz;
    v_ViewNormal = normalize((u_ModelView * vec4(a_Normal, 0.0)).xyz);
    v_TexCoord = a_TexCoord;
    gl_Position = u_ModelViewProjection * a_Position;
})";

constexpr char gFragmentShader[] = R"(
precision mediump float;

uniform sampler2D u_Texture;

uniform vec4 u_LightingParameters;
uniform vec4 u_MaterialParameters;
uniform vec4 u_ColorCorrectionParameters;

varying vec3 v_ViewPosition;
varying vec3 v_ViewNormal;
varying vec2 v_TexCoord;

void main() {
    // We support approximate sRGB gamma.
    const float kGamma = 0.4545454;
    const float kInverseGamma = 2.2;
    const float kMiddleGrayGamma = 0.466;

    // Unpack lighting and material parameters for better naming.
    vec3 viewLightDirection = u_LightingParameters.xyz;
    vec3 colorShift = u_ColorCorrectionParameters.rgb;
    float averagePixelIntensity = u_ColorCorrectionParameters.a;

    float materialAmbient = u_MaterialParameters.x;
    float materialDiffuse = u_MaterialParameters.y;
    float materialSpecular = u_MaterialParameters.z;
    float materialSpecularPower = u_MaterialParameters.w;

    // Normalize varying parameters, because they are linearly interpolated in
    // the vertex shader.
    vec3 viewFragmentDirection = normalize(v_ViewPosition);
    vec3 viewNormal = normalize(v_ViewNormal);

    // Apply inverse SRGB gamma to the texture before making lighting
    // calculations.
    // Flip the y-texture coordinate to address the texture from top-left.
    vec4 objectColor = texture2D(u_Texture,
            vec2(v_TexCoord.x, 1.0 - v_TexCoord.y));
    objectColor.rgb = pow(objectColor.rgb, vec3(kInverseGamma));

    // Ambient light is unaffected by the light intensity.
    float ambient = materialAmbient;

    // Approximate a hemisphere light (not a harsh directional light).
    float diffuse = materialDiffuse *
            0.5 * (dot(viewNormal, viewLightDirection) + 1.0);

    // Compute specular light.
    vec3 reflectedLightDirection = reflect(viewLightDirection, viewNormal);
    float specularStrength = max(0.0, dot(viewFragmentDirection,
            reflectedLightDirection));
    float specular = materialSpecular *
            pow(specularStrength, materialSpecularPower);

    vec3 color = objectColor.rgb * (ambient + diffuse) + specular;
    // Apply SRGB gamma before writing the fragment color.
    color.rgb = pow(color, vec3(kGamma));
    // Apply average pixel intensity and color shift
    color *= colorShift * (averagePixelIntensity/kMiddleGrayGamma);
    gl_FragColor.rgb = color;
    gl_FragColor.a = objectColor.a;
}
)";

/* --------------- TEMPORARY --------------- */


ObjectRenderer::ObjectRenderer(AAssetManager *assetManager,
                               const std::string &path) :
        assetManager(assetManager),
        ioSystem(std::make_unique<Assimp::AndroidJNIIOSystem>(assetManager, path)) {
}

void ObjectRenderer::initialize() {
    LOGI("%s", "Object start initialize");

    model = Model{"model/nanosuit.obj", ioSystem.get()};

    shaderProgram.init(assetManager, "model_vertex_shader.glsl", "model_fragment_shader.glsl");
//    shaderProgram.init(assetManager, gVertexShader, gFragmentShader);

    LOGI("%s", "Object end initialize");
}

void ObjectRenderer::draw(const glm::mat4 &projectionMat, const glm::mat4 &viewMat,
                          const glm::mat4 &modelMat, const glm::vec3 &viewPosition) {

    shaderProgram.use();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 ambient{glm::vec3{0.4f, 0.4f, 0.4f}};
    glm::vec3 diffuse{glm::vec3{1.0f, 1.0f, 1.0f}};
    glm::vec3 specular{glm::vec3{1.0f, 1.0f, 1.0f}};

    glm::mat4 mvMap = viewMat * modelMat;
    shaderProgram.setUniform("mvMap", mvMap);
    shaderProgram.setUniform("viewPosition", viewPosition);

    shaderProgram.setUniform("directionLight.direction", glm::vec3(0.0f, -0.8f, -1.0f));
    shaderProgram.setUniform("directionLight.ambient", ambient);
    shaderProgram.setUniform("directionLight.diffuse", diffuse);
    shaderProgram.setUniform("directionLight.specular", specular);

    shaderProgram.setUniform("model", modelMat);
    shaderProgram.setUniform("view", viewMat);
    shaderProgram.setUniform("projection", projectionMat);

    model.draw(shaderProgram);

    shaderProgram.unUse();
}

void ObjectRenderer::drawAr(const glm::mat4 &projectionMat, const glm::mat4 &viewMat,
                            const glm::mat4 &modelMat) {
    LOGI("%s", "ObjectRenderer::drawAr");

    shaderProgram.use();

    glm::vec3 ambient{glm::vec3{0.4f, 0.4f, 0.4f}};
    glm::vec3 diffuse{glm::vec3{1.0f, 1.0f, 1.0f}};
    glm::vec3 specular{glm::vec3{1.0f, 1.0f, 1.0f}};

    /*glm::mat4 mvpMat = projectionMat * viewMat * modelMat;
    glm::mat4 mvMat = viewMat * modelMat;
    glm::vec4 viewLightDirection = glm::normalize(mvMat * gLightDirection);

    shaderProgram.setUniform("u_ModelViewProjection", mvpMat);
    shaderProgram.setUniform("u_ModelView", mvMat);
    shaderProgram.setUniform("u_Texture", )

    shaderProgram.setUniform("u_LightingParameters", viewLightDirection);
    shaderProgram.setUniform("u_MaterialParameters", )
    shaderProgram.setUniform("u_ColorCorrectionParameters", )

    shaderProgram.setUniform("u_ModelViewProjection", )
    shaderProgram.setUniform("u_ModelViewProjection", )
    shaderProgram.setUniform("u_ModelViewProjection", )*/

    glm::mat4 mvMap = viewMat * modelMat;
    shaderProgram.setUniform("mvMap", mvMap);
//    shaderProgram.setUniform("viewPosition", viewPosition);

    shaderProgram.setUniform("directionLight.direction", glm::vec3(0.0f, -0.8f, -1.0f));
    shaderProgram.setUniform("directionLight.ambient", ambient);
    shaderProgram.setUniform("directionLight.diffuse", diffuse);
    shaderProgram.setUniform("directionLight.specular", specular);

    shaderProgram.setUniform("model", modelMat);
    shaderProgram.setUniform("view", viewMat);
    shaderProgram.setUniform("projection", projectionMat);

    model.draw(shaderProgram);

    shaderProgram.unUse();
}
