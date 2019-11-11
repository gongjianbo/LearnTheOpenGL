#if 0
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

class MyShader
{
public:
	//根据着色器代码来初始化
	MyShader(const std::string& vertexCode, const std::string& fragmentCode)
	{
		init(vertexCode, fragmentCode);
	}
	//激活程序
	void useProgram()
	{
		glUseProgram(programID);
	}
	// uniform工具函数
	void setInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
	}

private:
	void init(const std::string& vertexCode, const std::string& fragmentCode)
	{
		const char* vertex_shader_code = vertexCode.c_str();
		const char* fragment_shader_code = fragmentCode.c_str();

		//顶点着色器
		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
		glCompileShader(vertex_shader);
		checkError(vertex_shader, "vertex");

		//片段着色器
		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
		glCompileShader(fragment_shader);
		checkError(fragment_shader,"fragment");

		//着色器程序
		programID = glCreateProgram();
		glAttachShader(programID, vertex_shader);
		glAttachShader(programID, fragment_shader);
		glLinkProgram(programID);
		checkError(programID, "program");

		// 删除着色器，它们已经链接到我们的程序中了
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}

	void checkError(unsigned int name,const std::string &type)
	{
		int check_flag; 
		char check_info[1024];
		if (type != "program") {
			glGetShaderiv(name, GL_COMPILE_STATUS, &check_flag);
			if (!check_flag) {
				glGetShaderInfoLog(name, 1024, NULL, check_info);
				std::cout << type << " error:" << check_info << std::endl;
			}
		}
		else {
			glGetShaderiv(name, GL_LINK_STATUS, &check_flag);
			if (!check_flag) {
				glGetProgramInfoLog(name, 1024, NULL, check_info);
				std::cout << type << " error:" << check_info << std::endl;
			}
		}
	}

private:
	unsigned int programID;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


const std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float aSize;

out vec3 ourColor;

void main()
{
	gl_Position = vec4(aPos*aSize, 1.0);
	ourColor = aColor;
}
)";

//只有一个分量就不是vec2了，直接写float
const std::string fragmentShaderSource = R"(
#version 330 core
in vec3 ourColor;

out vec4 outColor;

void main()
{
	outColor = vec4(ourColor, 1.0);
}
)";

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	// build and compile our shader program
	MyShader a_shader(vertexShaderSource,fragmentShaderSource);

	// 顶点数据
	float vertices[] = {
		// 位置              // 颜色
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
	   -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1); //卸载glVertexAttribPointer前后都可以
	

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	//渲染循环
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// render
		glClear(GL_COLOR_BUFFER_BIT);

		a_shader.useProgram();
		glBindVertexArray(VAO);
		float timeValue = (float)glfwGetTime();
		float sizeValue = sin(timeValue) / 2.0f + 0.5f;
		a_shader.setFloat("aSize", sizeValue);

		// render the triangle
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
#endif