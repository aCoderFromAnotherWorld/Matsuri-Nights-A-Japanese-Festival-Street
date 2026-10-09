#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

class Shader
{
public:
    unsigned int ID = 0;

    Shader() = default;

    Shader(const char* vertexPath, const char* fragmentPath)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        bool loadedFromFile = false;
        try
        {
            // Try standard relative path
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            loadedFromFile = true;
        }
        catch (std::ifstream::failure&)
        {
            // Try alternate path (e.g. if run from solution dir or output dir)
            try
            {
                std::string altV = std::string("shaders/") + vertexPath;
                std::string altF = std::string("shaders/") + fragmentPath;
                vShaderFile.open(altV.c_str());
                fShaderFile.open(altF.c_str());
                std::stringstream vStream, fStream;
                vStream << vShaderFile.rdbuf();
                fStream << fShaderFile.rdbuf();
                vShaderFile.close();
                fShaderFile.close();
                vertexCode = vStream.str();
                fragmentCode = fStream.str();
                loadedFromFile = true;
            }
            catch (std::ifstream::failure&)
            {
                std::cout << "[Shader] Note: Could not read shader files (" << vertexPath << "), using embedded fallback shaders." << std::endl;
            }
        }

        if (!loadedFromFile)
        {
            // Embedded fallback shaders to guarantee execution under any working directory
            vertexCode = R"(
                #version 330 core
                layout (location = 0) in vec3 aPos;
                layout (location = 1) in vec3 aNormal;
                layout (location = 2) in vec2 aTexCoords;

                out vec3 FragPos;
                out vec3 Normal;
                out vec2 TexCoords;

                uniform mat4 model;
                uniform mat4 view;
                uniform mat4 projection;

                void main()
                {
                    FragPos = vec3(model * vec4(aPos, 1.0));
                    Normal = mat3(transpose(inverse(model))) * aNormal;
                    TexCoords = aTexCoords;
                    gl_Position = projection * view * vec4(FragPos, 1.0);
                }
            )";

            fragmentCode = R"(
                #version 330 core
                out vec4 FragColor;

                in vec3 FragPos;
                in vec3 Normal;
                in vec2 TexCoords;

                uniform vec4 objectColor;
                uniform float dayNightFactor;
                uniform bool isSky;
                uniform bool isWindow;
                uniform bool isEmissive;
                uniform vec3 emissiveColor;

                void main()
                {
                    if (isSky) {
                        float heightRatio = clamp((FragPos.y + 10.0) / 100.0, 0.0, 1.0);
                        vec3 daySky = mix(vec3(0.55, 0.75, 0.98), vec3(0.82, 0.88, 0.98), heightRatio);
                        vec3 nightSky = mix(vec3(0.02, 0.03, 0.08), vec3(0.06, 0.08, 0.18), heightRatio);
                        FragColor = vec4(mix(daySky, nightSky, dayNightFactor), 1.0);
                        return;
                    }
                    if (isEmissive) {
                        vec3 col = emissiveColor * mix(0.85, 1.35, dayNightFactor);
                        FragColor = vec4(col, objectColor.a);
                        return;
                    }
                    if (isWindow) {
                        vec3 dayPaper = vec3(0.90, 0.88, 0.82);
                        vec3 nightGlow = vec3(1.0, 0.82, 0.45);
                        FragColor = vec4(mix(dayPaper, nightGlow, dayNightFactor), 1.0);
                        return;
                    }

                    vec3 sunDir = normalize(vec3(0.4, 0.8, 0.5));
                    vec3 norm = normalize(Normal);
                    float diff = max(dot(norm, sunDir), 0.0);
                    float ambientStrength = mix(0.45, 0.18, dayNightFactor);
                    float lightIntensity = mix(0.65, 0.35, dayNightFactor);
                    float lighting = ambientStrength + diff * lightIntensity;

                    FragColor = vec4(objectColor.rgb * lighting, objectColor.a);
                }
            )";
        }

        const char* vShaderSource = vertexCode.c_str();
        const char* fShaderSource = fragmentCode.c_str();

        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderSource, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderSource, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() const
    {
        glUseProgram(ID);
    }

    mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;

    GLint getUniformLocation(const std::string& name) const
    {
        auto it = m_UniformLocationCache.find(name);
        if (it != m_UniformLocationCache.end())
            return it->second;

        GLint location = glGetUniformLocation(ID, name.c_str());
        m_UniformLocationCache[name] = location;
        return location;
    }

    // Direct location setters (zero string hashing/driver lookup overhead)
    static void setBool(GLint loc, bool value)
    {
        if (loc >= 0) glUniform1i(loc, (int)value);
    }
    static void setInt(GLint loc, int value)
    {
        if (loc >= 0) glUniform1i(loc, value);
    }
    static void setFloat(GLint loc, float value)
    {
        if (loc >= 0) glUniform1f(loc, value);
    }
    static void setVec2(GLint loc, const glm::vec2& value)
    {
        if (loc >= 0) glUniform2fv(loc, 1, &value[0]);
    }
    static void setVec3(GLint loc, const glm::vec3& value)
    {
        if (loc >= 0) glUniform3fv(loc, 1, &value[0]);
    }
    static void setVec4(GLint loc, const glm::vec4& value)
    {
        if (loc >= 0) glUniform4fv(loc, 1, &value[0]);
    }
    static void setMat3(GLint loc, const glm::mat3& mat)
    {
        if (loc >= 0) glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
    }
    static void setMat4(GLint loc, const glm::mat4& mat)
    {
        if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
    }

    // Cached string-name setters
    void setBool(const std::string& name, bool value) const
    {
        setBool(getUniformLocation(name), value);
    }
    void setInt(const std::string& name, int value) const
    {
        setInt(getUniformLocation(name), value);
    }
    void setFloat(const std::string& name, float value) const
    {
        setFloat(getUniformLocation(name), value);
    }
    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        setVec2(getUniformLocation(name), value);
    }
    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        setVec3(getUniformLocation(name), value);
    }
    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        setVec4(getUniformLocation(name), value);
    }
    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        setMat3(getUniformLocation(name), mat);
    }
    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        setMat4(getUniformLocation(name), mat);
    }

private:
    static void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
            }
        }
    }
};
