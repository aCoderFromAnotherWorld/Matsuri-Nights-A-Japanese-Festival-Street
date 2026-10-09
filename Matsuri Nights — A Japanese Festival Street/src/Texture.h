#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <vector>
#include "stb_image.h"

class Texture
{
public:
    unsigned int id = 0;
    int width = 0;
    int height = 0;
    int channels = 0;
    std::string type = "diffuse";

    Texture() = default;

    ~Texture()
    {
        if (id != 0)
        {
            glDeleteTextures(1, &id);
            id = 0;
        }
    }

    Texture(Texture&& other) noexcept
        : id(other.id), width(other.width), height(other.height), channels(other.channels), type(std::move(other.type))
    {
        other.id = 0;
    }

    Texture& operator=(Texture&& other) noexcept
    {
        if (this != &other)
        {
            if (id != 0)
                glDeleteTextures(1, &id);
            id = other.id;
            width = other.width;
            height = other.height;
            channels = other.channels;
            type = std::move(other.type);
            other.id = 0;
        }
        return *this;
    }

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    bool loadFromFile(const std::string& path, bool repeat = true)
    {
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
        if (!data)
        {
            std::cerr << "[Texture] Failed to load image: " << path << std::endl;
            return false;
        }

        if (id == 0)
            glGenTextures(1, &id);

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        std::cout << "[Texture] Successfully loaded: " << path << " (" << width << "x" << height << ")" << std::endl;
        return true;
    }

    bool loadFromMemory(int w, int h, const unsigned char* rgbaData, bool repeat = true)
    {
        width = w;
        height = h;
        channels = 4;

        if (id == 0)
            glGenTextures(1, &id);

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaData);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return true;
    }

    void bind(unsigned int unit = 0) const
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
};
