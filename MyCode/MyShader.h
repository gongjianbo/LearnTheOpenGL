#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

class MyShader
{
public:
	//������ɫ����������ʼ��
	MyShader(const std::string& vertexCode, const std::string& fragmentCode)
	{
		init(vertexCode, fragmentCode);
	}
	//�������
	void useProgram()
	{
		glUseProgram(programID);
	}
	unsigned int getID()
	{
		return programID;
	}
	// uniform���ߺ���
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

		//������ɫ��
		unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_code, NULL);
		glCompileShader(vertex_shader);
		checkError(vertex_shader, "vertex");

		//Ƭ����ɫ��
		unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_code, NULL);
		glCompileShader(fragment_shader);
		checkError(fragment_shader, "fragment");

		//��ɫ������
		programID = glCreateProgram();
		glAttachShader(programID, vertex_shader);
		glAttachShader(programID, fragment_shader);
		glLinkProgram(programID);
		checkError(programID, "program");

		// ɾ����ɫ���������Ѿ����ӵ����ǵĳ�������
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}

	void checkError(unsigned int name, const std::string& type)
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
