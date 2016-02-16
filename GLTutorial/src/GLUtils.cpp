#include "GLUtils.h"

// Thanks to Sean Barret
// https://github.com/nothings/stb
#include "stb_easy_font.h"


// Hardcoding some barebones shaders, mostly for use with text display right now.
// Supports color through the uniform basicColor
static std::string s_basicVertexShaderSource =
"#version 150\n"
"in vec2 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position, 0.0, 1.0);\n"
"}\n";

static std::string s_basicFragmentShaderSource =
"#version 150\n"
"uniform vec3 basicColor;\n"
"out vec4 outColor\n;"
"void main()\n"
"{\n"
	"outColor = vec4(basicColor, 1.0);\n"
"}\n";

static bool s_basicShaderInitialized = false, s_textPipelineInitialized = false;

static GLuint s_textVAO, s_textVertexBuffer;
static GLuint s_basicVertexShader, s_basicFragmentShader, s_basicShaderProgram;

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
	if (fin.rdstate() & std::fstream::failbit)
	{
		std::cout << "Failed to open shader file: " << path <<
			std::endl;
		return false;
	}

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

void GLUtils::displayBasicText(const float x, const float y, std::string text)
{
	// First we will allocate a buffer that will be used as our VBO for vertex data.
	// Sean Barret says this will be ~270 bytes per character, so we will use 300 for good measure
	GLuint rawBufferSize = text.length() * 300;
	void *rawTextVertexBuffer = (void*)malloc(rawBufferSize);

	// Call into the stb_easy_font code to get the vertex data for the given text
	GLuint numQuadPrimitives = stb_easy_font_print(x, y, (char*)(text.c_str()), NULL, rawTextVertexBuffer, (int)rawBufferSize);


	// Set everything up to draw (these bindings will have to be restored by the caller)
	if (!s_textPipelineInitialized)
		s_textPipelineInitialized = s_intitializeTextPipeline();
	if (!s_textPipelineInitialized) return;

	// Copy the data into the VBO
	glBufferData(GL_ARRAY_BUFFER, rawBufferSize, rawTextVertexBuffer, GL_STREAM_DRAW);

	// Set the color of the text via the uniform
	GLuint uniColor = glGetUniformLocation(s_basicShaderProgram, "basicColor");
	glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);


	// Draw the text
	glDrawArrays(GL_QUADS, 0, 4 * numQuadPrimitives);

	// Unset the pipeline flag because we assume other bindings will happen before this is called again
	s_textPipelineInitialized = false;


}

bool GLUtils::s_initializeBasicShader()
{
	s_basicVertexShader = __GL_DBG_CALL_(glCreateShader(GL_VERTEX_SHADER));
	s_basicFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glGenBuffers(1, &s_textVertexBuffer);
	s_basicShaderProgram = glCreateProgram();

	try {
		GLUtils::compileShaderFromString(s_basicVertexShader, s_basicVertexShaderSource);
		GLUtils::compileShaderFromString(s_basicFragmentShader, s_basicFragmentShaderSource);
	}
	catch (std::exception) {
		std::cout << "GLUtils failed to compile its internal shaders" << std::endl;
		return false;
	}

	glAttachShader(s_basicShaderProgram, s_basicVertexShader);
	glAttachShader(s_basicShaderProgram, s_basicFragmentShader);

	glLinkProgram(s_basicShaderProgram);
	return true;
}

bool GLUtils::s_intitializeTextPipeline()
{
	if (!s_basicShaderInitialized)
		s_basicShaderInitialized = s_initializeBasicShader();

	if (!s_basicShaderInitialized) return false;
	glGenVertexArrays(1, &s_textVAO);
	glBindVertexArray(s_textVAO);

	// Use the internal shader program
	glUseProgram(s_basicShaderProgram);

	// Bind and set VBO data
	glBindBuffer(GL_ARRAY_BUFFER, s_textVertexBuffer);

	// Set up the attribute pointer
	GLuint posAttrib = glGetAttribLocation(s_basicShaderProgram, "position");
	GLuint strideSize = (3 * sizeof(float)) + (4 * sizeof(unsigned char));
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, strideSize, 0);
	glEnableVertexAttribArray(posAttrib);
	return true;
}

std::string GLUtils::checkGlError()
{
	GLenum error;
	std::string errMsg = "GL_NO_ERROR";
	
	while ((error = glGetError()) != GL_NO_ERROR)
	{
		switch (error)
		{
		case GL_INVALID_ENUM:
			errMsg = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			errMsg = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			errMsg = "GL_INVALID_OPERATION";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			errMsg = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		case GL_OUT_OF_MEMORY:
			errMsg = "GL_OUT_OF_MEMORY";
			break;
		default:
			errMsg = "UNDEFINED_ERROR";
			break;
		}
	}

	return errMsg;
}