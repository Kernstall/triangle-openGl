#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

int main()
{
	constexpr auto vertexShaderSrc = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		uniform vec3 uPosition;
		void main()
		{
			gl_Position = vec4(aPos + uPosition, 1.0);
		}
	)";

	constexpr auto fragmentShaderSrc = R"(
		#version 330 core
		out vec4 FragColor;
		uniform vec3 uColor;
		void main()
		{
			FragColor = vec4(uColor, 1.0);
		}
	)";

	glfwSetErrorCallback([](int code, const char* description)
	{
		std::cout << std::to_string(code) << " " << description;
	});

	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW library!\n";
		glfwTerminate();
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 8);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_MULTISAMPLE);
	
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height)
	{
		glViewport(0, 0, width, height);
	});


	constexpr float vertices[] = {
	   -0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	
	uint32_t VAO{};
	uint32_t VBO{};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
	glEnableVertexAttribArray(0);

	const uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
	glCompileShader(vertexShader);

	const uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
	glCompileShader(fragmentShader);

	const uint32_t shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), 1.0f, 0.5f, 1.0f);
		glUniform3f(glGetUniformLocation(shaderProgram, "uPosition"), 0.0f, 0.0f, 0.0f);
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUniform3f(glGetUniformLocation(shaderProgram, "uColor"), 1.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(shaderProgram, "uPosition"), 0.25f, 0.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}
