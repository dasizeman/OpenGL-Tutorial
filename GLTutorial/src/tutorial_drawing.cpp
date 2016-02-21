#include <cmath>
#include <chrono>
#define GLEW_STATIC
#include "GLUtils.h"


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

	GLUtils::checkGlError(); // Clear any stale error flags

	// Vertex data
	float vertices[] =
	{
		 0.0f,  0.5f,
		 0.5f, -0.5f,
		-0.5f, -0.5f
	};


	// The VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	
	// Create and compile the shaders
	GLuint vertexShader, fragmentShader;
	try {
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		GLUtils::compileShaderFromFile(vertexShader, "basic_vertex.glsl");

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		GLUtils::compileShaderFromFile(fragmentShader, "basic_fragment.glsl");
	}
	catch (std::exception e)
	{
		system("pause");
		return -1;
	}

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

	// Get the triangle color uniform
	GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
	glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);

	auto t_start = std::chrono::high_resolution_clock::now();


	while (!glfwWindowShouldClose(window))
	{

		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>
			(t_now - t_start).count();

		glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);
		// Draw using the active VAO
		// Args:
		// - The type of primitive to use
		// - Number of vertices to skip at beginning
		// - Number of vertices to process
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//GLUtils::displayBasicText(0, 0, "This is text!");
		//std::cout << glGetError() << std::endl;

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}


	glfwTerminate();
	
}

