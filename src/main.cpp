#include <GLFW/glfw3.h> // Window management
#include <glad/glad.h>  // OpenGL function loader

#include <iostream>
#include <vector>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main() {
  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW\n";
    return -1;
  }

  // Set OpenGL version to 3.3 Core
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create GLFW window
  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Ray Tracer Starter", NULL, NULL);
  if (!window) {
    std::cerr << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Load OpenGL functions with GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD\n";
    return -1;
  }

  // Set up basic OpenGL settings
  glViewport(0, 0, WIDTH, HEIGHT);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

  // Temporary buffer to store color values (for now, we'll use a gradient)
  std::vector<float> pixels(WIDTH * HEIGHT * 3); // RGB values for each pixel

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // Generate a simple gradient across the screen (temporary)
    for (int y = 0; y < HEIGHT; ++y) {
      for (int x = 0; x < WIDTH; ++x) {
        int pixelIndex = (y * WIDTH + x) * 3;
        pixels[pixelIndex + 0] = (float)x / WIDTH;  // Red component
        pixels[pixelIndex + 1] = (float)y / HEIGHT; // Green component
        pixels[pixelIndex + 2] = 0.2f;              // Blue component (constant)
      }
    }

    // Render the gradient as a simple color buffer
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, pixels.data()); // Directly draw color buffer

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup and close
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}