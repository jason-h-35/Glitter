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
    unsigned int numPaths = 2;
    // 1. retrieve shader source code from file path
    std::vector<const char *> filePaths = {vertexPath, fragmentPath};
    std::vector<std::unique_ptr<std::ifstream>> fileStreams(numPaths);
    std::vector<std::unique_ptr<std::stringstream>> stringStreams(numPaths);
    std::vector<const char *> codeCStrings(2);
    std::cout << "Vecs allocated" << std::endl;
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
          std::cerr << "ERROR::SHADER::FILE_READ_NOT_OK\n" << std::endl;
        }
        codeCStrings[i] = stringStreams[i]->str().c_str();
      }
    }
    std::cout << "Retrieved shader code from file path" << std::endl;
    // 2. compile shaders
    std::vector<unsigned int> glRefs(numPaths);
    std::vector<GLenum> shaderTypes = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
    int ok;
    const size_t logMaxLen = 512;
    char infoLog[logMaxLen];
    for (size_t i = 0; i < glRefs.size(); i++) {
      glRefs[i] = glCreateShader(shaderTypes[i]);
      glShaderSource(glRefs[i], 1, &codeCStrings[i], nullptr);
      glCompileShader(glRefs[i]);
      glGetShaderiv(glRefs[i], GL_COMPILE_STATUS, &ok);
      if (!ok) {
        glGetShaderInfoLog(glRefs[i], logMaxLen, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
      }
    }
    std::cout << "Compiled shaders" << std::endl;
    // 3. shader program
    ID = glCreateProgram();
    for (auto glRef : glRefs) {
      glAttachShader(ID, glRef);
    }
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &ok);
    if (!ok) {
      glGetProgramInfoLog(ID, logMaxLen, nullptr, infoLog);
      std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                << infoLog << std::endl;
    }
    for (auto glRef : glRefs) {
      glDeleteShader(glRef);
    }
  }
  // use/activate the shader
  void use() { glUseProgram(ID); }
  // utility uniform functions
  void setBool(const std::string &name, bool value) const {
    setInt(name, (int)value);
  }
  void setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }
  void setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }
};

#endif
