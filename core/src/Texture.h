#pragma once
#include <glad/glad.h>

#include "FileLoader.h"

namespace LearnOpenGL
{

	class Texture
	{
	public:
		Texture() 
			:m_Texture(0) { }
		Texture(const std::string& filepath);
		virtual ~Texture();

		void ActiveTexture(int textureUnit);
		int GetGLChannelByDataChannel(int channel);
	private:


		unsigned int m_Texture;
	};
}
