#include "pch.h"
#include "Texture.h"

namespace LearnOpenGL
{
    Texture::Texture(const std::string& filepath)
    {
        glGenTextures(1, &m_Texture);

        glBindTexture(GL_TEXTURE_2D, m_Texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        {
            int width, height, nrChannels;
            unsigned char* data = FileLoader::LoadImageByStbImage(filepath, &width, &height, &nrChannels);

            if (data)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GetGLChannelByDataChannel(nrChannels), width, height, 0, GetGLChannelByDataChannel(nrChannels), GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                spdlog::error("Load {0} FAILED!", filepath);
            }

            stbi_image_free(data);
        }
    }

    Texture::~Texture()
    {
    }

    void Texture::ActiveTexture(int textureUnit)
    {
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, m_Texture);
    }

    int Texture::GetGLChannelByDataChannel(int channel)
    {
        switch (channel)
        {
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default:
            spdlog::error("Channel {0} is not supported", channel);
            break;
        }
        return GL_RGB;
    }

}
