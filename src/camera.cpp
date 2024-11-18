#include "../include/camera.h"
#include <algorithm>

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float aspect)
    : position(position)
    , worldUp(up)
    , yaw(-90.0f)  // Default looking towards -Z
    , pitch(0.0f)
    , movementSpeed(5.0f)
    , mouseSensitivity(0.1f)
    , fov(fov)
    , aspectRatio(aspect)
{
    // Initialize camera parameters for ray tracing
    float theta = glm::radians(fov);
    float h = tan(theta/2);
    viewportHeight = 2.0f * h;
    viewportWidth = aspectRatio * viewportHeight;
    focalLength = 1.0f;

    // Calculate initial front vector
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // Calculate the new front vector
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    // Recalculate right and up vectors
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

    // Update ray tracing vectors
    horizontal = viewportWidth * right;
    vertical = viewportHeight * up;
    lowerLeftCorner = position - horizontal/2.0f - vertical/2.0f + front * focalLength;
}

Ray Camera::getRay(float u, float v) const {
    return Ray(
        position,
        glm::normalize(lowerLeftCorner + u*horizontal + v*vertical - position)
    );
}

void Camera::moveForward(float deltaTime) {
    position += front * movementSpeed * deltaTime;
    updateCameraVectors();
}

void Camera::moveBackward(float deltaTime) {
    position -= front * movementSpeed * deltaTime;
    updateCameraVectors();
}

void Camera::moveLeft(float deltaTime) {
    position -= right * movementSpeed * deltaTime;
    updateCameraVectors();
}

void Camera::moveRight(float deltaTime) {
    position += right * movementSpeed * deltaTime;
    updateCameraVectors();
}

void Camera::moveUp(float deltaTime) {
    position += worldUp * movementSpeed * deltaTime;
    updateCameraVectors();
}

void Camera::moveDown(float deltaTime) {
    position -= worldUp * movementSpeed * deltaTime;
    updateCameraVectors();
}

void Camera::rotate(float deltaYaw, float deltaPitch) {
    yaw += deltaYaw * mouseSensitivity;
    pitch += deltaPitch * mouseSensitivity;

    // Constrain pitch
    pitch = std::clamp(pitch, -89.0f, 89.0f);

    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}