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
    std::vector<std::string> codeStrings(numPaths);
    std::cout << "Vecs allocated" << std::endl;
    // ensure ifstream objects can throw exceptions:
    for (size_t i = 0; i < filePaths.size(); i++) {
      std::ifstream fileStream;
      std::stringstream stringStream;
      fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      try {
        fileStream.open(filePaths[i]);
        stringStream << fileStream.rdbuf();
        fileStream.close();
      } catch (std::ifstream::failure e) {
        std::cerr << "ERROR::SHADER::FILE_READ_NOT_OK\n" << std::endl;
      }
      codeStrings[i] = stringStream.str();
    }

    std::cout << "Retrieved shader code from file path" << std::endl;
    // 2. compile shaders
    std::vector<unsigned int> glRefs(numPaths);
    std::vector<GLenum> shaderTypes = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
    int ok;
    const size_t logMaxLen = 512;
    char infoLog[logMaxLen];
    std::cout << "1" << std::endl;
    for (size_t i = 0; i < glRefs.size(); i++) {
      std::cout << "2" << std::endl;
      glRefs[i] = glCreateShader(shaderTypes[i]);
      std::cout << "3" << std::endl;
      std::cout << codeStrings[i] << std::endl;
      const char *source = codeStrings[i].c_str();
      glShaderSource(glRefs[i], 1, &source, nullptr);
      std::cout << "4" << std::endl;
      glCompileShader(glRefs[i]);
      std::cout << "5" << std::endl;
      glGetShaderiv(glRefs[i], GL_COMPILE_STATUS, &ok);
      if (!ok) {
        glGetShaderInfoLog(glRefs[i], logMaxLen, nullptr, infoLog);
        std::cerr << "Shader compile failed" << infoLog << std::endl;
      } else {
        std::cout << "Compiled shader" << std::endl;
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
