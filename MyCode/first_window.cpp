#if 0
//第一个窗口
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void error_callback(int error, const char* description); //声明
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void bufferresize_callback(GLFWwindow* window, int width, int height);

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


	//检查GLFW是否被要求退出
	while (!glfwWindowShouldClose(window))
	{
		//设置清空屏幕所用的颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//清空颜色缓冲
		glClear(GL_COLOR_BUFFER_BIT);

		//交换颜色缓冲
		//默认情况下，GLFW窗口使用双缓冲。这意味着每个窗口都有两个渲染缓冲区。
		//前缓冲区和后缓冲区。前缓冲区是要显示的缓冲区，后缓冲区是要渲染的缓冲区。
		//渲染完整个帧后，需要相互交换缓冲区，因此后缓冲区将变为前缓冲区，反之亦然。
		glfwSwapBuffers(window);
		//检查有没有触发什么事件（比如键盘输入、鼠标移动等）
		//并调用对应的回调函数（可以通过回调方法手动设置）
		glfwPollEvents();
	}

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