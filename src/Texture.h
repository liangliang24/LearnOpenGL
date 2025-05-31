#pragma once
#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include "FileLoader.h"

class Texture
{
public:
	Texture(const std::string& filepath);
	virtual ~Texture();

	void ActiveTexture(int textureUnit);
	int GetGLChannelByDataChannel(int channel);
private:


	unsigned int m_Texture;
};