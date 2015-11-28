#include "GLUtils.h"

// Thanks to Sean Barret
// https://github.com/nothings/stb
#include "stb_easy_font.h"

void GLUtils::compileShaderFromString(const GLuint shader, std::string source)
{
	const GLchar * const sourceArr = source.c_str();
	GLchar *logBuffer;


	std::cout << "Trying to compile shader...";
	glShaderSource(shader, 1, &sourceArr, NULL);
	glCompileShader(shader);

	// Check the the compile status
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (GL_TRUE == status)
	{
		std::cout << "Success!" << std::endl;
		return;
	}

	std::cout << "Failed!\nSee compilation log below:" << std::endl;
	// Dump the shader info log and throw an exception if compilation was not
	// sucessful
	GLint logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

	logBuffer = (GLchar*)malloc(logLength);
	glGetShaderInfoLog(shader, logLength, NULL, logBuffer);

	
	std::cout << std::string(logBuffer) << std::endl;
	throw std::exception("Shader compilation failed.");

}

bool GLUtils::compileShaderFromFile(const GLuint shader,std::string path)
{
	std::fstream fin;
	std::stringstream ss;

	fin.open(path.c_str(), std::fstream::in);
	if (fin.rdstate() & std::fstream::failbit) return false;

	std::string str;
	while (!fin.eof())
	{
		std::getline(fin, str);
		ss << str;
		ss << "\n";
	}

	GLUtils::compileShaderFromString(shader, ss.str());

	fin.close();

	return true;
}
