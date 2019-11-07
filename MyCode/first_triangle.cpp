#if 0
//第一个三角形
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


void error_callback(int error, const char* description); //声明
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void bufferresize_callback(GLFWwindow* window, int width, int height);

//顶点着色器程序，着色器语言GLSL下一节才学
//为了设置顶点着色器的输出，我们必须把位置数据赋值给预定义的gl_Position变量
//layout对应glVertexAttribPointer中的参数1
static const char* vertex_code = R"(
#version 330 core
layout(location=0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos,1.0f);
}
)";

//片段着色器代码
//片段着色器所做的是计算像素最后的颜色输出
static const char* frag_code = R"(
#version 330 core
out vec4 FragColor;

void main()
{
	FragColor = vec4(0.1f,1.0f,0.1f,1.0f);
}
)";

int main()
{
	if (!glfwInit()) {
		return -1; //初始化GLFW库失败
	}
	//注册错误回调，大多数事件都是通过回调报告的
	glfwSetErrorCallback(error_callback);
	//设置最低主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//设置最低次版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//设置为核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //mac系统加这一句
	//创建窗口对象，如果要销毁窗口则glfwDestroyWindow(window)
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	//创建失败则返回NULL，退出程序
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//窗口位置，试了下不包含标题栏
	glfwSetWindowPos(window, 100, 100);
	//注册按键按下的回调
	glfwSetKeyCallback(window, key_callback);
	//注册帧缓冲区大小改变的回调
	glfwSetFramebufferSizeCallback(window, bufferresize_callback);
	//通知GLFW将我们窗口的上下文设置为当前线程的主上下文
	//必须先具有当前的OpenGL上下文，然后才能使用OpenGL API，加载程序需要当前上下文才能加载
	glfwMakeContextCurrent(window);

	//在调用任何OpenGL的函数之前我们需要初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	//创建顶点着色器对象
	//GLuint glCreateShader(GLenum shaderType​);
	//参数为着色器类型
	//GL_VERTEX_SHADER：顶点着色器
	//GL_FRAGMENT_SHADER：片段着色器	
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//把着色器代码附加到着色器对象上
	//void glShaderSource(GLuint shader​, GLsizei count​, const GLchar **string​, const GLint *length​);
	//参数1着色器对象，参数2指定字符串和长度数组中的元素数
	//参数3指定指向要加载到着色器的源码字符串的指针数组（二级指针）
	//参数4指定字符串长度的数组（暂时还没明白第四个参数）
	glShaderSource(vertexShader, 1, &vertex_code, NULL);
	//编译着色器
	glCompileShader(vertexShader);

	//检测是否编译成功
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << std::endl;
	}

	//创建片段着色器对象 
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//把着色器代码附加到着色器对象上，并编译
	glShaderSource(fragmentShader, 1, &frag_code, NULL);
	glCompileShader(fragmentShader);

	//检测是否编译成功
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << std::endl;
	}

	//现在两个着色器都编译了
	//接下来把两个着色器对象连接到一个用来渲染的着色器程序中

	//着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本
	//创建程序对象
	//GLuint glCreateProgram(void​);
	//glCreateProgram创建一个空的程序对象，并返回一个非零值，该值可以被引用
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//把之前编译的着色器附加到程序对象上
	//void glAttachShader(GLuint program​, GLuint shader​);
	//参数1程序对象，参数2待附加的着色器对象
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//链接程序对象
	//void glLinkProgram(GLuint program​);
	//参数为程序对象
	//成功链接程序对象后，可以通过调用glUseProgram使该程序对象成为当前状态的一部分
	glLinkProgram(shaderProgram);

	//检测是否链接成功
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << infoLog << std::endl;
	}

	//把着色器对象链接到程序对象以后，
	//记得删除着色器对象，我们不再需要它们了
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//VAO是一个对象，其中包含一个或者更多的Vertex Buffer Objects。
	//而VBO是Graphics Card中的一个内存缓冲区，用来保存顶点信息，颜色信息，法线信息，纹理坐标信息和索引信息等等。
	//一个VAO有多个VBO，它们之间也是通过上下文，只有唯一的激活VAO，在VAO后创建的VBO都属于该VAO。
	//关联VBO数据用取得当前激活的缓存区对象偏移来指定。

	//开始绘制图形之前，我们必须先给OpenGL输入一些顶点数据
	//我们希望渲染一个三角形，我们一共要指定三个顶点，每个顶点都有一个3D位置
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};


	unsigned int VAO;
	//生成顶点数组对象名称
	//void glGenVertexArrays(GLsizei n​, GLuint *arrays​);
	//参数1为生成名称数量，参数2为顶点数组 数组
	glGenVertexArrays(1, &VAO);

	unsigned int VBO;
	//生成缓冲区对象名称
	//void glGenBuffers(GLsizei n​, GLuint * buffers​);
	//参数1为生成名称数量，参数2为缓冲区数组
	glGenBuffers(1, &VBO);

	//先绑定顶点数组对象，然后绑定并设置顶点缓冲区，然后配置顶点属性
	//绑定一个命名的顶点数组对象
	//void glBindVertexArray(GLuint array​);
	//参数为顶点数组的名称
	glBindVertexArray(VAO);

	//OpenGL有很多缓冲对象类型，顶点缓冲对象的缓冲类型是GL_ARRAY_BUFFER
	//OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。
	//绑定一个命名的缓冲区对象：
	//void glBindBuffer(GLenum target​, GLuint buffer​);
	//参数1缓冲对象类型，参数2缓冲对象名称
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//接下来把之前定义的顶点数据复制到缓冲的内存中
	//glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数
	//void glBufferData(GLenum target​, GLsizeiptr size​, const GLvoid * data​, GLenum usage​);
	//参数1缓冲对象类型，参数2数据字节大小，参数3数据指针，没数据则为NULL
	//参数4指定了我们希望显卡如何管理给定的数据，他有三种形式：
	//GL_STATIC_DRAW/READ/COPY ：数据不会或几乎不会改变。
	//GL_DYNAMIC_DRAW/READ/COPY：数据会被改变很多。
	//GL_STREAM_DRAW/READ/COPY ：数据每次绘制时都会改变。	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//定义通用顶点属性数据的数组
	//void glVertexAttribPointer(GLuint index​, GLint size​, GLenum type​, GLboolean normalized​, GLsizei stride​, const GLvoid * pointer​);
	//参数1指定要配置的通用顶点属性的索引，对应顶点着色器中的（layout(location = 0)）
	//参数2指定顶点属性的大小，1-4，这里顶点属性是vec3，所以填3
	//参数3指定数据类型
	//参数4定义我们是否希望数据被标准化，为true则数据被归一化0-1
	//参数5为字节步长，告诉我们在连续的顶点属性组之间的间隔
	//由于下个组位于3个float之后，所以置为3 * sizeof(float)
	//参数6表示位置数据在缓冲中起始位置的偏移量(Offset)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//使能顶点属性数组
	//void glEnableVertexAttribArray(GLuint index​);
	//参数为顶点属性的索引
	glEnableVertexAttribArray(0);

	//对glVertexAttribPointer的调用将VBO注册为顶点属性的绑定顶点缓冲区对象，
	//这样之后我们可以安全地解除绑定
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//可以解绑VAO，这样其他VAO调用就不会以外地修改这个VAO
	glBindVertexArray(0);

	//设置清空屏幕所用的颜色
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//检查GLFW是否被要求退出
	while (!glfwWindowShouldClose(window))
	{
		//清空颜色缓冲
		glClear(GL_COLOR_BUFFER_BIT);

		//安装所指定的程序对象程序作为当前再现状态的一部分
		//void glUseProgram(GLuint program​);
		glUseProgram(shaderProgram);

		//绑定一个顶点数组对象
		//void glBindVertexArray(GLuint array​);
		//参数为要绑定的顶点数组的名称
		glBindVertexArray(VAO);

		//使用当前激活的着色器和顶点属性配置和VBO（通过VAO简介绑定）来绘制图元
		//void glDrawArrays(GLenum mode​, GLint first​, GLsizei count​);
		//参数1为图元类型
		//参数2指定顶点数组的其实索引
		//参数3指定顶点个数
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//交换颜色缓冲
		//默认情况下，GLFW窗口使用双缓冲。这意味着每个窗口都有两个渲染缓冲区。
		//前缓冲区和后缓冲区。前缓冲区是要显示的缓冲区，后缓冲区是要渲染的缓冲区。
		//渲染完整个帧后，需要相互交换缓冲区，因此后缓冲区将变为前缓冲区，反之亦然。
		glfwSwapBuffers(window);
		//检查有没有触发什么事件（比如键盘输入、鼠标移动等）
		//并调用对应的回调函数（可以通过回调方法手动设置）
		glfwPollEvents();
	}

	//删除顶点数组对象
	//void glDeleteVertexArrays(GLsizei n​, const GLuint *arrays​);
	//参数1为数量，参数2为顶点数组 数组
	glDeleteVertexArrays(1, &VAO);
	//删除命名的缓冲区对象
	//void glDeleteBuffers(GLsizei n​, const GLuint * buffers​);
	//参数1为数量，参数2为缓冲区数组
	glDeleteBuffers(1, &VBO);

	//销毁窗口
	glfwDestroyWindow(window);
	//使用GLFW的操作后，需要终止GLFW
	glfwTerminate();
	return 0;
}

void error_callback(int error, const char* description)
{
	std::cout << "error:" << error << std::endl;
	std::cout << "description:" << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Esc按下则触发WindowShouldClose为true
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void bufferresize_callback(GLFWwindow* window, int width, int height)
{
	//glViewport函数前两个参数控制渲染窗口左下角的位置
	glViewport(0, 0, width, height);
}

#endif