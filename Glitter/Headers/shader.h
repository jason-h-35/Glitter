#ifndef SHADER_H
#define SHADER_H
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class Shader {
public:
  unsigned int ID;
  // constructor reads and builds shader
  Shader(const char *vertexPath, const char *fragmentPath) {
    // 1. retrieve shader source code from file path
    std::vector<const char *> filePaths = {vertexPath, fragmentPath};
    std::vector<std::unique_ptr<std::ifstream>> fileStreams(2);
    std::vector<std::unique_ptr<std::stringstream>> stringStreams(2);
    std::vector<const char *> codeCStrings(2);
    // ensure ifstream objects can throw exceptions:
    for (size_t i = 0; i < fileStreams.size(); i++) {
      if (fileStreams[i] && fileStreams[i]->is_open()) {
        fileStreams[i]->exceptions(std::ifstream::failbit |
                                   std::ifstream::badbit);
        try {
          fileStreams[i]->open(vertexPath);
          *stringStreams[i] << fileStreams[i]->rdbuf();
          fileStreams[i]->close();
        } catch (std::ifstream::failure e) {
          std::cerr << "ERROR::SHADER::FILE_READ_NOT_OK" << std::endl;
        }
        codeCStrings[i] = stringStreams[i]->str().c_str();
      }
    }
    // 2. compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // vertexShader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
  }
  // use/activate the shader
  void use();
  // utility uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
};

#endif
