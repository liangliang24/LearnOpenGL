#pragma once
#include <spdlog/spdlog.h>
#include <iostream>
#include <fstream>

#include "stb_image.h"

namespace LearnOpenGL
{
	class FileLoader
	{
	public:
		inline static std::string ReadFileAsString(const std::string& filepath)
		{
			std::string result;
			std::ifstream in(filepath, std::ios::in | std::ios::binary);
			if (in)
			{
				in.seekg(0, std::ios::end);
				result.resize((size_t)in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());
				in.close();
			}
			else
			{
				spdlog::error("Could not open file '{0}'", filepath);
			}

			return result;
		}

		inline static unsigned char* LoadImageByStbImage(const std::string& filepath, int* width, int* height, int* nrChannels)
		{
			stbi_set_flip_vertically_on_load(true);
			return stbi_load(filepath.c_str(), width, height, nrChannels, 0);
		}
	};
}