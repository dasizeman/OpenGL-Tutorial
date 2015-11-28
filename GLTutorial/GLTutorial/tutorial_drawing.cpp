#define GLEW_STATIC
#include <iostream>
#include <exception>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


void compileShader(const GLuint shader, GLchar const * const source);

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL); //Windowed
	//GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), NULL); //Full screen

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	// Vertex data
	float vertices[] =
	{
		 0.0f,  0.5f,
		 0.5f, -0.5f,
		-0.5f, -0.5f
	};

	// Shaders
	GLchar const *vertexShaderSource =
		"#version 150\n"
		"in vec2 position;\n"
		"void main()\n"
		"{\n"
			"gl_Position = vec4(position, 0.0, 1.0);\n"
		"}\n";

	GLchar const *fragmentShaderSource =
		"#version 150\n"
		"out vec4 outColor;\n"
		"void main()\n"
		"{\n"
			"outColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
		"}\n";

	// The VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	
	// Create and compile the shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	compileShader(vertexShader, vertexShaderSource);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	compileShader(fragmentShader, fragmentShaderSource);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	// Create and link a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Map how the data in our VBO is used by the shader program.  This info
	// is maintained in the active VAO

	// Get a reference to the position input in the vertex shader
	GLuint posAttrib = glGetAttribLocation(shaderProgram, "position");

	// Actually set up the vertex attribute array
	// Args:
	// - Shader input to link to
	// - The number of values in that input
	// - The type of these values
	// - Whether or not the values should be normalized as floats
	//   if they are not already
	// - Stride: The number of bytes between each attribute in the array
	// - Offset: Distance from front of array to the start of the relevant data
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Actually enable the VAA
	glEnableVertexAttribArray(posAttrib);

	while (!glfwWindowShouldClose(window))
	{
		// Draw using the active VAO
		// Args:
		// - The type of primitive to use
		// - Number of vertices to skip at beginning
		// - Number of vertices to process
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}


	glfwTerminate();
	
}

void compileShader(const GLuint shader, GLchar const * const source)
{
	GLchar *logBuffer;

	std::cout << "Trying to compile shader...";
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	// Check the the compile status
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (GL_TRUE == status)
	{
		std::cout << "Success!" << std::endl;
		return;
	}

	// Dump the shader info log and throw an exception if compilation was not
	// sucessful
	GLint logLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

	logBuffer = (GLchar*)malloc(logLength);
	glGetShaderInfoLog(shader, logLength, NULL, logBuffer);

	std::cout << std::string(logBuffer) << std::endl;
	throw std::exception("Shader compilation failed.");

}