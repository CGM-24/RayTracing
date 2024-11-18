#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ray.h"

class Camera {
public:
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f),
        glm::vec3 target = glm::vec3(0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float fov = 45.0f,
        float aspect = 16.0f/9.0f
    );

    // Ray generation
    Ray getRay(float u, float v) const;

    // Camera movement
    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);

    // Camera rotation
    void rotate(float yaw, float pitch);

    // Update camera vectors
    void updateCameraVectors();

    // Getters
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getDirection() const { return front; }
    glm::mat4 getViewMatrix() const;

private:
    // Camera position and orientation
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Camera parameters
    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
    float fov;  // Field of view in degrees
    float aspectRatio;

    // Viewport parameters
    float viewportHeight;
    float viewportWidth;
    float focalLength;

    // Camera basis vectors for ray tracing
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 lowerLeftCorner;
};

#endif