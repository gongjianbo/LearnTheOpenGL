#pragma once

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
	unsigned int getID()
	{
		return programID;
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
		checkError(fragment_shader, "fragment");

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
