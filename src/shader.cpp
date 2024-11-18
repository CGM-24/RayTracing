#ifndef SPHERE_H
#define SPHERE_H

#include <glm/glm.hpp>
#include "ray.h"

class Sphere {
public:
    Sphere(const glm::vec3& center, float radius, const glm::vec3& color);
    
    bool intersect(const Ray& ray, float& t) const;
    glm::vec3 getNormalAt(const glm::vec3& point) const;
    glm::vec3 getColor() const;

private:
    glm::vec3 center;
    float radius;
    glm::vec3 color;
};

#endif

// sphere.cpp
#include "../include/sphere.h"

Sphere::Sphere(const glm::vec3& center, float radius, const glm::vec3& color)
    : center(center), radius(radius), color(color) {}

bool Sphere::intersect(const Ray& ray, float& t) const {
    glm::vec3 oc = ray.getOrigin() - center;
    float a = glm::dot(ray.getDirection(), ray.getDirection());
    float b = 2.0f * glm::dot(oc, ray.getDirection());
    float c = glm::dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0) return false;
    
    float temp = (-b - sqrt(discriminant)) / (2.0f * a);
    if (temp < 0) {
        temp = (-b + sqrt(discriminant)) / (2.0f * a);
        if (temp < 0) return false;
    }
    
    t = temp;
    return true;
}

glm::vec3 Sphere::getNormalAt(const glm::vec3& point) const {
    return glm::normalize(point - center);
}

glm::vec3 Sphere::getColor() const {
    return color;
}