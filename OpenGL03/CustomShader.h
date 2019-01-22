#pragma once

#ifndef CUSTOM_SHADER_H
#define CUSTOM_SHADER_H

#include <string>

class CustomShader
{
public:

	unsigned int ProgramID;

	CustomShader(const char* vertexPath, const char* fragPath);
	~CustomShader();

	void use();

	void setBool(const std::string &name, bool value);

	void setInt(const std::string &name, int value);

	void setFloat(const std::string &name, float value);


private:
	void checkCompileErrors(unsigned int shader, std::string type);
};


#endif // !CUSTOM_SHADER_H
