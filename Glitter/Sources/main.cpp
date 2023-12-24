#include <GLFW/glfw3.h>
#include <cstring>
#include <glad/glad.h>

// Standard Headers
#include <iostream>
const char *orangeFragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";
const char *yellowFragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\0";

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int genVertexShader();
unsigned int genFragmentShader(const char *source);
bool programOK(unsigned int program);

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
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  float verts1[] = {0.5f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, 0.5f, 0.0f};
  float verts2[] = {0.2f, -0.5f, 0.0f, -0.8f, -0.5f, 0.0f, -0.8f, 0.5f, 0.0f};
  unsigned int vertBuf[2];
  glGenBuffers(2, vertBuf);
  glBindBuffer(GL_ARRAY_BUFFER, vertBuf[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts1), verts1, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, vertBuf[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts2), verts2, GL_STATIC_DRAW);
  unsigned int vertexShader = genVertexShader();

  unsigned int orangeFragShader = genFragmentShader(orangeFragmentShaderSource);
  unsigned int orangeProgram = glCreateProgram();
  glAttachShader(orangeProgram, vertexShader);
  glAttachShader(orangeProgram, orangeFragShader);
  glLinkProgram(orangeProgram);
  if (!programOK(orangeProgram)) {
    std::cout << "shader program failed to compile" << std::endl;
  }
  glDeleteShader(orangeFragShader);

  unsigned int yellowFragShader = genFragmentShader(yellowFragmentShaderSource);
  unsigned int yellowProgram = glCreateProgram();
  glAttachShader(yellowProgram, vertexShader);
  glAttachShader(yellowProgram, yellowFragShader);
  glLinkProgram(yellowProgram);
  if (!programOK(yellowProgram)) {
    std::cout << "shader program failed to compile" << std::endl;
  }
  glDeleteShader(yellowFragShader);

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
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glUseProgram(orangeProgram);
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

unsigned int genVertexShader() {
  const char *vertexShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\0";
  unsigned int ref;
  ref = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(ref, 1, &vertexShaderSource, NULL);
  glCompileShader(ref);
  int ok;
  glGetShaderiv(ref, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    std::cout << "vertex shader failed to compile" << std::endl;
    // glGetShaderInfoLog(shader, strlen(infoLog), NULL, infoLog);
  }
  return ref;
}

unsigned int genFragmentShader(const char *source) {
  unsigned int ref;
  ref = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(ref, 1, &source, NULL);
  glCompileShader(ref);
  int ok;
  glGetShaderiv(ref, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    std::cout << "fragment shader failed to compile" << std::endl;
    // glGetShaderInfoLog(shader, strlen(infoLog), NULL, infoLog);
  }
  return ref;
}

bool programOK(unsigned int program) {
  int success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    // glGetProgramInfoLog(program, strlen(infoLog), NULL, infoLog);
    return false;
  }
  return true;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}
