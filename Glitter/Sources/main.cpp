#include <GLFW/glfw3.h>
#include <cstring>
#include <glad/glad.h>

// Standard Headers
#include <iostream>

// shader string
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
bool shaderOK(unsigned int shader);
bool programOK(unsigned int program);

int main() {
  glfwInit();
  // window hints are like a key-value setter
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // core profile means reduce OpenGL down to the modern subset
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
  // make a triangle w/ verts
  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
  // make 1 vertex buffer object.
  // VBO should be an array of ids to vertex buffer objects
  // But it can be just an int here b/c C++, I guess...
  unsigned int VAO, VBO;
  // generate VAO and VBO
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // bind VAO to GL context
  glBindVertexArray(VAO);
  // bind VBO to GL context as array buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // ingest vertices into the array buffer bound to GL context (VBO)
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // create Vertex shader and compile it
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check if it worked
  if (!shaderOK(vertexShader)) {
    std::cout << "vertex shader failed to compile" << std::endl;
  }
  // create Fragment shader and compile it
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // check if it worked
  if (!shaderOK(fragmentShader)) {
    std::cout << "fragment shader failed to compile" << std::endl;
  }
  // create shader program, attach V and F shader to it.
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  // Link program to GL context
  glLinkProgram(shaderProgram);
  if (!programOK(shaderProgram)) {
    std::cout << "shader program failed to compile" << std::endl;
  }
  // Make it primary program in GL context
  glUseProgram(shaderProgram);
  // Clean up.
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  // Give GL the start index, size, type, normalized?, stride, ptr
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // Make a VAO (vertex array object) which is like an array of
  // (isVertexAttribArrayEnabled, glVertexAttribPointer, VBO)
  while (!glfwWindowShouldClose(window)) {
    // input
    processInput(window);
    // rendering goes here
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // check and call events and swap the buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

bool shaderOK(unsigned int shader) {
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    // glGetShaderInfoLog(shader, strlen(infoLog), NULL, infoLog);
    return false;
  }
  return true;
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
