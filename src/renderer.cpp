// renderer.h
#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "camera.h"
#include "sphere.h"
#include <glm/glm.hpp>

struct Light {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    
    Light(const glm::vec3& pos, const glm::vec3& col, float intens) 
        : position(pos), color(col), intensity(intens) {}
};

class Renderer {
public:
    Renderer(int width, int height);
    
    // Add objects and lights to the scene
    void addSphere(const Sphere& sphere);
    void addLight(const Light& light);
    
    // Main render function
    void render(const Camera& camera);
    
    // Get the rendered image data
    const std::vector<unsigned char>& getImageData() const;
    
    // Update window dimensions if needed
    void updateDimensions(int width, int height);

private:
    // Core ray tracing functions
    glm::vec3 traceRay(const Ray& ray, int depth = 0);
    bool findNearestIntersection(const Ray& ray, float& t, const Sphere** hitObject);
    
    // Lighting calculation
    glm::vec3 calculateLighting(const glm::vec3& point, const glm::vec3& normal, 
                              const glm::vec3& viewDir, const Sphere& sphere);
    
    // Convert float color to RGB bytes
    void setPixel(int x, int y, const glm::vec3& color);

private:
    int width;
    int height;
    std::vector<unsigned char> imageData;  // RGB format
    std::vector<Sphere> spheres;
    std::vector<Light> lights;
    
    // Rendering parameters
    const int MAX_DEPTH = 3;  // Maximum ray bounce depth
    const float AMBIENT_STRENGTH = 0.1f;
    const float SPECULAR_STRENGTH = 0.5f;
    const int SPECULAR_POWER = 32;
};

#endif

// renderer.cpp
#include "../include/renderer.h"
#include <algorithm>
#include <iostream>

Renderer::Renderer(int w, int h) : width(w), height(h) {
    imageData.resize(width * height * 3);  // RGB format
}

void Renderer::addSphere(const Sphere& sphere) {
    spheres.push_back(sphere);
}

void Renderer::addLight(const Light& light) {
    lights.push_back(light);
}

void Renderer::updateDimensions(int w, int h) {
    width = w;
    height = h;
    imageData.resize(width * height * 3);
}

void Renderer::setPixel(int x, int y, const glm::vec3& color) {
    if (x < 0 || x >= width || y < 0 || y >= height) return;
    
    // Convert floating-point color to byte color with simple tone mapping
    auto gammaCorrect = [](float value) -> unsigned char {
        return static_cast<unsigned char>(std::min(255.0f, std::max(0.0f, 255.0f * std::pow(value, 1.0f/2.2f))));
    };
    
    int index = (y * width + x) * 3;
    imageData[index] = gammaCorrect(color.r);
    imageData[index + 1] = gammaCorrect(color.g);
    imageData[index + 2] = gammaCorrect(color.b);
}

bool Renderer::findNearestIntersection(const Ray& ray, float& t, const Sphere** hitObject) {
    t = std::numeric_limits<float>::infinity();
    *hitObject = nullptr;
    
    for (const auto& sphere : spheres) {
        float currentT;
        if (sphere.intersect(ray, currentT)) {
            if (currentT < t) {
                t = currentT;
                *hitObject = &sphere;
            }
        }
    }
    
    return *hitObject != nullptr;
}

glm::vec3 Renderer::calculateLighting(const glm::vec3& point, const glm::vec3& normal,
                                    const glm::vec3& viewDir, const Sphere& sphere) {
    glm::vec3 finalColor(0.0f);
    glm::vec3 objectColor = sphere.getColor();
    
    // Ambient lighting
    finalColor += AMBIENT_STRENGTH * objectColor;
    
    for (const auto& light : lights) {
        glm::vec3 lightDir = glm::normalize(light.position - point);
        float distance = glm::length(light.position - point);
        
        // Check for shadows
        Ray shadowRay(point + normal * 0.001f, lightDir);
        float shadowT;
        const Sphere* shadowObject;
        bool inShadow = findNearestIntersection(shadowRay, shadowT, &shadowObject) 
                       && shadowT < distance;
        
        if (!inShadow) {
            // Diffuse lighting
            float diff = std::max(glm::dot(normal, lightDir), 0.0f);
            glm::vec3 diffuse = diff * light.color * light.intensity;
            
            // Specular lighting
            glm::vec3 reflectDir = glm::reflect(-lightDir, normal);
            float spec = std::pow(std::max(glm::dot(viewDir, reflectDir), 0.0f), SPECULAR_POWER);
            glm::vec3 specular = SPECULAR_STRENGTH * spec * light.color * light.intensity;
            
            // Attenuation
            float attenuation = 1.0f / (1.0f + 0.09f * distance + 0.032f * distance * distance);
            
            finalColor += (diffuse + specular) * attenuation;
        }
    }
    
    return finalColor * objectColor;
}

glm::vec3 Renderer::traceRay(const Ray& ray, int depth) {
    if (depth >= MAX_DEPTH) {
        return glm::vec3(0.0f);
    }
    
    float t;
    const Sphere* hitObject;
    
    if (findNearestIntersection(ray, t, &hitObject)) {
        glm::vec3 hitPoint = ray.pointAtParameter(t);
        glm::vec3 normal = hitObject->getNormalAt(hitPoint);
        glm::vec3 viewDir = glm::normalize(-ray.getDirection());
        
        return calculateLighting(hitPoint, normal, viewDir, *hitObject);
    }
    
    // Background color - simple gradient
    float t1 = 0.5f * (ray.getDirection().y + 1.0f);
    return (1.0f - t1) * glm::vec3(1.0f) + t1 * glm::vec3(0.5f, 0.7f, 1.0f);
}

void Renderer::render(const Camera& camera) {
    #pragma omp parallel for collapse(2)
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            float u = float(i) / float(width - 1);
            float v = float(j) / float(height - 1);
            
            Ray ray = camera.getRay(u, v);
            glm::vec3 color = traceRay(ray, 0);
            
            setPixel(i, (height - 1 - j), color);  // Flip Y coordinate
        }
    }
}

const std::vector<unsigned char>& Renderer::getImageData() const {
    return imageData;
}