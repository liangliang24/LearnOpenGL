#pragma once
#include "pch.h"
#include "Shader.h"

namespace LearnOpenGL
{
	Shader::Shader(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath)
	{
		Shader(vertexShaderFilepath.c_str(), fragmentShaderFilepath.c_str());
	}

	Shader::Shader(const char* vertexShaderFilepath, const char* fragmentShaderFilepath)
	{
		m_ShaderProgram = glCreateProgram();

		std::string fragmentShaderSource = FileLoader::ReadFileAsString(fragmentShaderFilepath);

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		{
			std::string vertexShaderSource = FileLoader::ReadFileAsString(vertexShaderFilepath);
			unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

			const GLchar* vertexShaderSourceStr = vertexShaderSource.c_str();
			glShaderSource(vertexShader, 1, &vertexShaderSourceStr, nullptr);
			glCompileShader(vertexShader);

			int success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
				spdlog::error("VertexShader compile FAILED!{0}", infoLog);
			}
			else
			{
				spdlog::info("VertexShader compile SUCCESS!");
			}

			glAttachShader(m_ShaderProgram, vertexShader);
			glDeleteShader(vertexShader);
		}

		{
			std::string fragmentShaderSource = FileLoader::ReadFileAsString(fragmentShaderFilepath);
			unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

			const GLchar* fragmentShaderSourceStr = fragmentShaderSource.c_str();
			glShaderSource(fragmentShader, 1, &fragmentShaderSourceStr, nullptr);
			glCompileShader(fragmentShader);

			int success;
			char infoLog[512];
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
				spdlog::error("FragmentShader compile FAILED!{0}", infoLog);
			}
			else
			{
				spdlog::info("FragmentShader compile SUCCESS!");
			}

			glAttachShader(m_ShaderProgram, fragmentShader);
			glDeleteShader(fragmentShader);
		}

		glLinkProgram(m_ShaderProgram);

		{
			int success;
			char infoLog[512];
			glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
				spdlog::error("ShaderProgram Link FAILED!{0}", infoLog);
			}
			else
			{
				spdlog::info("ShaderProgram Link Success!");
			}
		}
	}

	Shader::~Shader()
	{
	}

	int Shader::GetUniformLocation(std::string uniformName)
	{
		return glGetUniformLocation(m_ShaderProgram, uniformName.c_str());
	}

	int Shader::GetUniformLocation(char* uniformName)
	{
		return glGetUniformLocation(m_ShaderProgram, uniformName);
	}

	void Shader::SetUniform1f(char* uniformName, float x)
	{
		glUniform1f(GetUniformLocation(uniformName), x);
	}

	void Shader::SetUniform2f(char* uniformName, float x, float y)
	{
		glUniform2f(GetUniformLocation(uniformName), x, y);
	}

	void Shader::SetUniform3f(char* uniformName, float x, float y, float z)
	{
		glUniform3f(GetUniformLocation(uniformName), x, y, z);
	}

	void Shader::SetUniform4f(char* uniformName, float x, float y, float z, float w)
	{
		glUniform4f(GetUniformLocation(uniformName), x, y, z, w);
	}

	void Shader::SetUniform1i(char* uniformName, int x)
	{
		glUniform1i(GetUniformLocation(uniformName), x);
	}

	void Shader::SetUniform2i(char* uniformName, int x, int y)
	{
		glUniform2i(GetUniformLocation(uniformName), x, y);
	}

	void Shader::SetUniform3i(char* uniformName, int x, int y, int z)
	{
		glUniform3i(GetUniformLocation(uniformName), x, y, z);
	}

	void Shader::SetUniform4i(char* uniformName, int x, int y, int z, int w)
	{
		glUniform4i(GetUniformLocation(uniformName), x, y, z, w);
	}

	void Shader::SetUniformMatrix4fv(char* uniformName, int matSize, bool transpose, const float* trans)
	{
		glUniformMatrix4fv(GetUniformLocation(uniformName), matSize, transpose, trans);
	}

	void Shader::SetUniform1iv(char* uniformName, int size, const int* value)
	{
		glUniform1iv(GetUniformLocation(uniformName), size, value);
	}

	void Shader::UseShader()
	{
		glUseProgram(m_ShaderProgram);
	}

}

