#pragma once
#include <glad/glad.h>

#include "FileLoader.h"

namespace LearnOpenGL
{

	class Shader
	{
	public:
		Shader()
		: m_ShaderProgram(0) { }
		Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath);
		Shader(const char* vertexShaderFilepath, const char* fragmentShaderFilepath);
		virtual ~Shader();

		int GetUniformLocation(std::string uniformName);
		int GetUniformLocation(char* uniformName);
		void SetUniform1f(char* uniformName, float x);
		void SetUniform2f(char* uniformName, float x, float y);
		void SetUniform3f(char* uniformName, float x, float y, float z);
		void SetUniform4f(char* uniformName, float x, float y, float z, float w);
		void SetUniform1i(char* uniformName, int x);
		void SetUniform2i(char* uniformName, int x, int y);
		void SetUniform3i(char* uniformName, int x, int y, int z);
		void SetUniform4i(char* uniformName, int x, int y, int z, int w);
		void SetUniformMatrix4fv(char* uniformName, int matSize, bool transpose, const float* trans);
		void UseShader();

	private:
		unsigned int m_ShaderProgram;
	};
}
