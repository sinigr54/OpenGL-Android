//
// Created by Alexander Chekh on 15.05.2018.
//

#include "PlaneRenderer.h"

constexpr char gFragmentShader[] = R"(
    precision highp float;
    precision highp int;
    uniform sampler2D texture;
    uniform vec3 color;
    varying vec2 v_textureCoords;
    varying float v_alpha;
    void main() {
      float r = texture2D(texture, v_textureCoords).r;
      gl_FragColor = vec4(color.xyz, r * v_alpha);
    })";

constexpr char gVertexShader[] = R"(
    precision highp float;
    precision highp int;
    attribute vec3 vertex;
    varying vec2 v_textureCoords;
    varying float v_alpha;

    uniform mat4 mvp;
    uniform mat4 model_mat;
    uniform vec3 normal;

    void main() {
      // Vertex Z value is used as the alpha in this shader.
      v_alpha = vertex.z;

      vec4 local_pos = vec4(vertex.x, 0.0, vertex.y, 1.0);
      gl_Position = mvp * local_pos;
      vec4 world_pos = model_mat * local_pos;

      // Construct two vectors that are orthogonal to the normal.
      // This arbitrary choice is not co-linear with either horizontal
      // or vertical plane normals.
      const vec3 arbitrary = vec3(1.0, 1.0, 0.0);
      vec3 vec_u = normalize(cross(normal, arbitrary));
      vec3 vec_v = normalize(cross(normal, vec_u));

      // Project vertices in world frame onto vec_u and vec_v.
      v_textureCoords = vec2(
         dot(world_pos.xyz, vec_u), dot(world_pos.xyz, vec_v));
    })";

void PlaneRenderer::initialize() {
    shaderProgram.init(gVertexShader, gFragmentShader);

    attributeVertices = glGetAttribLocation(shaderProgram.getProgram(), "vertex");

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (!RenderUtils::bindTextureSource(GL_TEXTURE_2D, "models/trigrid.png")) {
        LOGE("Could not load png texture for planes.");
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
}

void PlaneRenderer::draw(const glm::mat4 &projectionMat, const glm::mat4 &viewMat,
                         const ArSession *arSession, const ArPlane *arPlane,
                         const glm::vec3 &color) {

    LOGI("PlaneRenderer::%s", "Draw");

    updateForPlane(arSession, arPlane);

    shaderProgram.use();

    glDepthMask(GL_FALSE);

    glActiveTexture(GL_TEXTURE0);
    shaderProgram.setUniform("texture", 0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    shaderProgram.setUniform("mvp", projectionMat * viewMat * modelMat);
    shaderProgram.setUniform("model_mat", modelMat);
    shaderProgram.setUniform("normal", normal);
    shaderProgram.setUniform("color", color);
    shaderProgram.setUniform("vertex", attributeVertices);

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);

    shaderProgram.unUse();
    glDepthMask(GL_TRUE);
}

void PlaneRenderer::updateForPlane(const ArSession *arSession, const ArPlane *arPlane) {
    // The following code generates a triangle mesh filling a convex polygon,
    // including a feathered edge for blending.
    //
    // The indices shown in the diagram are used in comments below.
    // _______________     0_______________1
    // |             |      |4___________5|
    // |             |      | |         | |
    // |             | =>   | |         | |
    // |             |      | |         | |
    // |             |      |7-----------6|
    // ---------------     3---------------2

    vertices.clear();
    triangles.clear();

    int32_t polygonLength;
    ArPlane_getPolygonSize(arSession, arPlane, &polygonLength);

    if (polygonLength == 0) {
        LOGE("PlaneRenderer::UpdatePlane, no valid plane polygon is found");

        updateRenderConfig();

        return;
    }

    const int32_t verticesSize = polygonLength / 2;
    std::vector<glm::vec2> rawVertices(verticesSize);
    ArPlane_getPolygon(arSession, arPlane,
                       glm::value_ptr(rawVertices.front()));

    // Fill vertex 0 to 3. Note that the vertex.xy are used for x and z
    // position. vertex.z is used for alpha. The outter polygon's alpha
    // is 0.
    for (int32_t i = 0; i < verticesSize; ++i) {
        vertices.push_back(glm::vec3(rawVertices[i].x, rawVertices[i].y, 0.0f));
    }

    ScopedArPose scopedArPose(arSession);
    ArPlane_getCenterPose(arSession, arPlane, scopedArPose.getArPose());
    ArPose_getMatrix(arSession, scopedArPose.getArPose(),
                     glm::value_ptr(modelMat));
    normal = getPlaneNormal(arSession, *scopedArPose.getArPose());

    // Feather distance 0.2 meters.
    const float gFeatherLength = 0.2f;
    // Feather scale over the distance between plane center and vertices.
    const float gFeatherScale = 0.2f;

    // Fill vertex 4 to 7, with alpha set to 1.
    for (int32_t i = 0; i < verticesSize; ++i) {
        // Vector from plane center to current point.
        glm::vec2 v = rawVertices[i];
        const float scale =
                1.0f - std::min((gFeatherLength / glm::length(v)), gFeatherScale);
        const glm::vec2 resultV = scale * v;

        vertices.push_back(glm::vec3(resultV.x, resultV.y, 1.0f));
    }

    const int32_t verticesLength = vertices.size();
    const int32_t halfVerticesLength = verticesLength / 2;

    // Generate triangle (4, 5, 6) and (4, 6, 7).
    for (int i = halfVerticesLength + 1; i < verticesLength - 1; ++i) {
        triangles.push_back(static_cast<unsigned short &&>(halfVerticesLength));
        triangles.push_back(static_cast<unsigned short &&>(i));
        triangles.push_back(static_cast<unsigned short &&>(i + 1));
    }

    // Generate triangle (0, 1, 4), (4, 1, 5), (5, 1, 2), (5, 2, 6),
    // (6, 2, 3), (6, 3, 7), (7, 3, 0), (7, 0, 4)
    for (int i = 0; i < halfVerticesLength; ++i) {
        triangles.push_back(static_cast<unsigned short &&>(i));
        triangles.push_back(static_cast<unsigned short &&>(
                                    (i + 1) % halfVerticesLength));
        triangles.push_back(static_cast<unsigned short &&>(i + halfVerticesLength));

        triangles.push_back(static_cast<unsigned short &&>(i + halfVerticesLength));
        triangles.push_back(static_cast<unsigned short &&>((i + 1) % halfVerticesLength));
        triangles.push_back(static_cast<unsigned short &&>(
                                    (i + halfVerticesLength + 1) % halfVerticesLength +
                                    halfVerticesLength));
    }

    updateRenderConfig();
}

void PlaneRenderer::updateRenderConfig() {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec3)),
                 &vertices[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(triangles.size() * sizeof(GLushort)),
                 &triangles[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(attributeVertices);
    glVertexAttribPointer(attributeVertices, 3, GL_FLOAT, GL_FALSE, 0,
                          vertices.data());

    glBindVertexArray(0);
}
