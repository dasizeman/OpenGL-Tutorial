#ifndef __GL_UTILS_H__
#define __GL_UTILS_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <exception>
#include <string>
#include <fstream>
#include <sstream>

#define __GL_DBG_CALL_(function_call) \
	function_call; \
	std::cout << #function_call << ": glGetError() returned " << \
		GLUtils::checkGlError() << std::endl



namespace GLUtils {
void compileShaderFromString(const GLuint shader, std::string source);

bool compileShaderFromFile(const GLuint shader, std::string path);

void displayBasicText(const float x, const float y, std::string text);

static bool s_initializeBasicShader();
static bool s_intitializeTextPipeline();

std::string checkGlError();

}

#endif
