#ifndef _H_TEXTURE_
#define _H_TEXTURE_

#include "util.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <iostream>
#include <string>

class Texture
{
public:
    unsigned int textureid;
    int width, height;
    const char* path;
    std::string type;

    Texture(const char* _path, std::string _type = "texture_diffuse") // Texture types: texture_diffuse, texture_specular, texture_normal, texture_height
    {
        glGenTextures(1, &textureid);
        glBindTexture(GL_TEXTURE_2D, textureid);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int nrChannels;
        path = _path;
        type = _type;
        unsigned char* data = stbi_load(_path, &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureid);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else
        {
            std::cerr << "ERROR: Couldn't load texture at " << path << std::endl;
        }
        stbi_image_free(data);
    }

    unsigned int getID() const { return textureid; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

#endif