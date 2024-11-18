// ray.h
#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class Ray {
public:
    Ray(const glm::vec3& origin, const glm::vec3& direction);
    
    glm::vec3 getOrigin() const;
    glm::vec3 getDirection() const;
    glm::vec3 pointAtParameter(float t) const;

private:
    glm::vec3 orig;
    glm::vec3 dir;
};

#endif

