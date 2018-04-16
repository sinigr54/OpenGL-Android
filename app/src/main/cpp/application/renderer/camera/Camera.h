//
// Created by sinigr on 4/7/18.
//

#ifndef OPENGLCOURSE_CAMERA_H
#define OPENGLCOURSE_CAMERA_H

#include <GLES3/gl3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera {
    struct CameraConfiguration {
        constexpr static GLfloat YAW = -90.0f;
        constexpr static GLfloat PITCH = 0.0f;
        constexpr static GLfloat SPEED = 3.0f;
        constexpr static GLfloat SENSITIVITY = 45.0f;
        constexpr static GLfloat ZOOM = 45.0f;
    };

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;

    void updateCameraVectors() {
        glm::vec3 front;
        front.x = static_cast<float>(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
        front.y = static_cast<float>(sin(glm::radians(pitch)));
        front.z = static_cast<float>(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

        this->front = glm::normalize(front);
        this->right = glm::normalize(glm::cross(this->front, this->worldUp));
        this->up = glm::normalize(glm::cross(this->right, this->front));
    }

public:
    enum class CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    Camera(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 3.0f),
           const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f),
           GLfloat yaw = CameraConfiguration::YAW,
           GLfloat pitch = CameraConfiguration::PITCH) : position(position),
                                                         worldUp(up),
                                                         yaw(yaw),
                                                         pitch(pitch) {
        updateCameraVectors();
    }

    Camera(GLfloat posX, GLfloat posY, GLfloat posZ,
           GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) :
            position(posX, posY, posZ),
            worldUp(upX, upY, upZ),
            yaw(yaw),
            pitch(pitch) {

        updateCameraVectors();
    }

    glm::vec3 &getPosition() {
        return position;
    }

    glm::vec3 &getFront() {
        return front;
    }

    glm::vec3 &getUp() {
        return up;
    }

    const glm::vec3 &getPosition() const {
        return position;
    }

    const glm::vec3 &getFront() const {
        return front;
    }

    const glm::vec3 &getUp() const {
        return up;
    }

    glm::mat4 getViewMatrix() const {
//         return glm::lookAt(position, position + front, up);
         return glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), up);
    }

    void processTouch(CameraMovement direction, GLfloat deltaTime) {
        // TODO
    }

    void processAround(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true) {
        // TODO
    }

    void processZoom(GLfloat yOffset) {
        // TODO
    }
};

#endif //OPENGLCOURSE_CAMERA_H
