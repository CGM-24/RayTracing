// ray.cpp
#include "../include/ray.h"

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction) 
    : orig(origin), dir(glm::normalize(direction)) {}

glm::vec3 Ray::getOrigin() const {
    return orig;
}

glm::vec3 Ray::getDirection() const {
    return dir;
}

glm::vec3 Ray::pointAtParameter(float t) const {
    return orig + t * dir;
}