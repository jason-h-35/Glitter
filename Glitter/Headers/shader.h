#ifndef SHADER_H
#define SHADER_H
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
  unsigned int ID;
  // constructor reads and builds shader
  Shader(const char *vertexPath, const char *fragmentPath);
  // use/activate the shader
  void use();
  // utility uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
};

#endif
