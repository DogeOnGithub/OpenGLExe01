#include "CustomShader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>


CustomShader::CustomShader(const char* vertexPath, const char* fragPath)
{
	std::string vertexCode;
	std::string fragCode;

	std::ifstream vertexFile;
	std::ifstream fragFile;

	//保证fstream可以抛出异常
	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexFile.open(vertexPath);
		fragFile.open(fragPath);

		std::stringstream vertexShaderStream, fragShaderStream;

		//读取文件流中的字节到字符串流中，这里的运算符重载，作用是insert until end-of-file from a stream buffer
		vertexShaderStream << vertexFile.rdbuf();
		fragShaderStream << fragFile.rdbuf();

		vertexFile.close();
		fragFile.close();

		vertexCode = vertexShaderStream.str();
		fragCode = fragShaderStream.str();
	}
	catch (const std::exception&)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vertexShaderCode = vertexCode.c_str();
	const char* fragShaderCode = fragCode.c_str();

	unsigned int vertexShader, fragShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	checkCompileErrors(vertexShader, "VERTEX");

	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderCode, NULL);
	glCompileShader(fragShader);
	checkCompileErrors(vertexShader, "FRAGMENT");

	this->ProgramID = glCreateProgram();
	glAttachShader(this->ProgramID, vertexShader);
	glAttachShader(this->ProgramID, fragShader);
	glLinkProgram(this->ProgramID);
	checkCompileErrors(vertexShader, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
}

void CustomShader::use()
{
	glUseProgram(this->ProgramID);
}

void CustomShader::setBool(const std::string &name, bool value)
{
	glUniform1i(glGetUniformLocation(this->ProgramID, name.c_str()), (int)value);
}

void CustomShader::setInt(const std::string &name, int value)
{
	glUniform1i(glGetUniformLocation(this->ProgramID, name.c_str()), value);
}

void CustomShader::setFloat(const std::string &name, float value)
{
	glUniform1f(glGetUniformLocation(this->ProgramID, name.c_str()), value);
}

void CustomShader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}


CustomShader::~CustomShader()
{
}
