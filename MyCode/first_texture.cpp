#if 0

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "MyShader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
)";

const std::string fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1992;

void main()
{
	FragColor = texture(texture1992, TexCoord)*vec4(ourColor,1.0f);
}
)";

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	// build and compile our shader zprogram
	MyShader ourShader(vertexShaderSource, fragmentShaderSource);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	float vertices[] = {
		// positions          // colors           // texture coords
		 1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f, // top right
		 1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// 创建一个纹理对象
	unsigned int texture;
	glGenTextures(1, &texture);
	// 将命名纹理绑定到纹理目标，很明显这又是状态机系统的体现
	// void glBindTexture(GLenum target​, GLuint texture​);
	// 参数1指定纹理绑定的目标，参数2指定纹理名称
	glBindTexture(GL_TEXTURE_2D, texture); 

	// 设置纹理参数
	// void glTexParameterf(GLenum target​, GLenum pname​, GLfloat param​);
	// void glTexParameteri(GLenum target​, GLenum pname​, GLint param​);
	//参数1纹理目标，参数2指定单值纹理参数的名称，参数3指定值的参数

	//设置纹理环绕参数
	//默认为GL_REPEAT，重复纹理图像（坐标的S\T\R等效于xyz）
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); //GL_CLAMP_TO_EDGE
	//如果纹理环绕设置为GL_CLAMP_TO_BORDER，还需要设置一个边框颜色
	//把纹理坐标设置在范围之外才看得到
	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	
	//设置纹理过滤参数
	//默认为GL_NEAREST临近过滤，选择附近那个像素，
	//GL_LINEAR线性过滤则做插值计算。（Min Mag分别制定放大和缩小的过滤方式）
	//当要纹理化的像素映射到大于一个纹理元素的区域时，就使用纹理最小化功能:GL_TEXTURE_MIN_FILTER
	//当被纹理化的像素映射到小于或等于一个纹理元素的区域时，将使用纹理放大功能:GL_TEXTURE_MAG_FILTER
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载图像，创建纹理并生成mipmap
	int width, height, nrChannels;
	//OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部，于是造成了上下翻转。
	//stb_image.h能够在图像加载时帮助我们翻转y轴，使用stbi_set_flip_vertically_on_load(true);
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(R"(F:\Src\textures\brickwall.jpg)", &width, &height, &nrChannels, 0);
	if (data)
	{
		//指定二位纹理图像
		//void glTexImage2D(GLenum target​, GLint level​, GLint internalFormat​, GLsizei width​, GLsizei height​, GLint border​, GLenum format​, GLenum type​, const GLvoid * data​);
		//参数1指定目标纹理，参数2指定详细级别编号，参数3纹理中颜色分量的数量
		//参数4指定纹理图像的宽度，参数2指定纹理图像的高度或纹理阵列中的层数
		//参数5边界为0，参数6指定像素数据格式
		//参数7指定像素数据的数据类型，参数8图像数据
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//为指定的纹理目标生成mipmap
		//void glGenerateMipmap(GLenum target​);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClear(GL_COLOR_BUFFER_BIT);

		// bind Texture
		glBindTexture(GL_TEXTURE_2D, texture);

		// render container
		ourShader.useProgram();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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