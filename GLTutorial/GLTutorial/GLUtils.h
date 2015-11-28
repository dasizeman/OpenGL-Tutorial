#ifndef __GL_UTILS_H__
#define __GL_UTILS_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <exception>
#include <string>
#include <fstream>
#include <sstream>

namespace GLUtils {
void compileShaderFromString(const GLuint shader, std::string source);

bool compileShaderFromFile(const GLuint shader, std::string path);

}

#endif
