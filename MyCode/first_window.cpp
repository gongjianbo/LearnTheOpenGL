#if 0
//��һ������
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void error_callback(int error, const char* description); //����
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void bufferresize_callback(GLFWwindow* window, int width, int height);

int main()
{
	if (!glfwInit()) {
		return -1; //��ʼ��GLFW��ʧ��
	}
	//ע�����ص���������¼�����ͨ���ص������
	glfwSetErrorCallback(error_callback);
	//����������汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//������ʹΰ汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//����Ϊ����ģʽ
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //macϵͳ����һ��
	//�������ڶ������Ҫ���ٴ�����glfwDestroyWindow(window)
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	//����ʧ���򷵻�NULL���˳�����
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//����λ�ã������²�����������
	glfwSetWindowPos(window, 100, 100);
	//ע�ᰴ�����µĻص�
	glfwSetKeyCallback(window, key_callback);
	//ע��֡��������С�ı�Ļص�
	glfwSetFramebufferSizeCallback(window, bufferresize_callback);
	//֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������
	//�����Ⱦ��е�ǰ��OpenGL�����ģ�Ȼ�����ʹ��OpenGL API�����س�����Ҫ��ǰ�����Ĳ��ܼ���
	glfwMakeContextCurrent(window);

	//�ڵ����κ�OpenGL�ĺ���֮ǰ������Ҫ��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}


	//���GLFW�Ƿ�Ҫ���˳�
	while (!glfwWindowShouldClose(window))
	{
		//���������Ļ���õ���ɫ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//�����ɫ����
		glClear(GL_COLOR_BUFFER_BIT);

		//������ɫ����
		//Ĭ������£�GLFW����ʹ��˫���塣����ζ��ÿ�����ڶ���������Ⱦ��������
		//ǰ�������ͺ󻺳�����ǰ��������Ҫ��ʾ�Ļ��������󻺳�����Ҫ��Ⱦ�Ļ�������
		//��Ⱦ������֡����Ҫ�໥��������������˺󻺳�������Ϊǰ����������֮��Ȼ��
		glfwSwapBuffers(window);
		//�����û�д���ʲô�¼�������������롢����ƶ��ȣ�
		//�����ö�Ӧ�Ļص�����������ͨ���ص������ֶ����ã�
		glfwPollEvents();
	}

	//���ٴ���
	glfwDestroyWindow(window);
	//ʹ��GLFW�Ĳ�������Ҫ��ֹGLFW
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
	//Esc�����򴥷�WindowShouldCloseΪtrue
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void bufferresize_callback(GLFWwindow* window, int width, int height)
{
	//glViewport����ǰ��������������Ⱦ�������½ǵ�λ��
	glViewport(0, 0, width, height);
}

#endif