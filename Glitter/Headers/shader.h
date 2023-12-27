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
    std::vector<std::string> codeStrings(2);
    std::vector<const char *> codeCStrings(2);
    // ensure ifstream objects can throw exceptions:
    for (auto &stream : fileStreams) {
      if (stream && stream->is_open()) {
        stream->exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
          stream->open
        } catch (std::ifstream::failure e) {
          std::cerr << "ERROR::SHADER::FILE_READ_NOT_OK" << std::endl;
        }
      }
    }
    try {
      vFileStream.open(vertexPath);
      fFileStream.open(fragmentPath);
      std::stringstream vShaderStream, fShaderStream;
      // read ifstreams into stringsteams
      vShaderStream << vFileStream.rdbuf();
      fShaderStream << fFileStream.rdbuf();
      // close ifstreams
      vFileStream.close();
      fFileStream.close();
      // convert stringstream to string
      vertexCodeString = vShaderStream.str();
      fragmentCodeString = fShaderStream.str();
    } catch (std::ifstream::failure e) {
      std::cerr << "ERROR::SHADER::FILE_READ_NOT_OK" << std::endl;
    }
    const char *vShaderCode = vertexCodeString.c_str();
    const char *fShaderCode = fragmentCodeString.c_str();
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
