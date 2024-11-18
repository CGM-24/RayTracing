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