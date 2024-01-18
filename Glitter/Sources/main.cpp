#include <GLFW/glfw3.h>
#include <cstring>
#include <glad/glad.h>
#include <shader.h>

#include <iostream>

// TODO: Ex 2: horizontal offset via uniform
// TODO: Ex 3:
//            - output vertex position to fragment shader using `out`
//            - set fragment's color equal to this vertex position
//            - why is the bottom-left side of triangle black?

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  Shader shader("/home/jason/code/Glitter/Glitter/Shaders/shader.vert",
                "/home/jason/code/Glitter/Glitter/Shaders/shader.frag");
  std::cout << "Shader created" << std::endl;
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  // clang-format off
  float verts[] = {
    // positions          // colors
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // bottom left
     0.0f, 0.5f,  0.0f,   0.0f, 0.0f, 1.0f  // top
  };
  // clang-format on
  unsigned int VBO[1], VAO[1];
  glGenBuffers(1, VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
  glGenVertexArrays(1, VAO);
  glBindVertexArray(VAO[0]);
  // Give GL the start index, size, type, normalized?, stride, ptr
  // position vertex attrib on 0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color vertex attrib on 1
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    shader.use();
    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    shader.use();
    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glDeleteVertexArrays(1, &VAO[0]);
  glDeleteBuffers(1, &VBO[0]);
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow * /* win */, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
