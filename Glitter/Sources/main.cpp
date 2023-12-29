#include <GLFW/glfw3.h>
#include <cstring>
#include <glad/glad.h>
#include <shader.h>

#include <iostream>

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
  Shader ourShader("shader.vert", "shader.frag");
  glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
  // clang-format off
  float vertices[] = {
    // positions          // colors
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, // bottom left
     0.0f, 0.5f,  0.0f,   0.0f, 0.0f, 1.0f  // top
  };
  // clang-format on
  unsigned int vertBuf[2];
  glGenBuffers(2, vertBuf);
  glBindBuffer(GL_ARRAY_BUFFER, vertBuf[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts1), verts1, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vertBuf[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts2), verts2, GL_STATIC_DRAW);
  unsigned int vertAttr[2];
  glGenVertexArrays(2, vertAttr);
  glBindVertexArray(vertAttr[0]);
  glBindBuffer(GL_ARRAY_BUFFER, vertBuf[0]);
  // Give GL the start index, size, type, normalized?, stride, ptr
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(vertAttr[1]);
  glBindBuffer(GL_ARRAY_BUFFER, vertBuf[1]);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glUseProgram(program);
    glBindVertexArray(vertAttr[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(yellowProgram);
    glBindVertexArray(vertAttr[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
