#include <iostream>
#include <fstream>
#include <spdlog/spdlog.h>
#include <glad/glad.h>

class Shader
{
public:
	Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath);
	Shader(const char* vertexShaderFilepath, const char* fragmentShaderFilepath);
	virtual ~Shader();

	static std::string ReadFileAsString(const std::string& filepath);
	int GetUniformLocation(std::string uniformName);
	int GetUniformLocation(char* uniformName);
	void SetUniform1f(char* uniformName, float x);
	void SetUniform2f(char* uniformName, float x, float y);
	void SetUniform3f(char* uniformName, float x, float y, float z);
	void SetUniform4f(char* uniformName, float x, float y, float z, float w);
	void UseShader();

private:
	unsigned int m_ShaderProgram;
};