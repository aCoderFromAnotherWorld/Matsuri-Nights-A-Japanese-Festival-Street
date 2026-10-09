#pragma once

#include <glm/glm.hpp>
#include <string>

struct DirLight
{
    glm::vec3 direction{ 0.4f, -0.8f, -0.5f };
    glm::vec3 ambient{ 0.35f, 0.35f, 0.30f };
    glm::vec3 diffuse{ 0.85f, 0.80f, 0.75f };
    glm::vec3 specular{ 0.60f, 0.60f, 0.60f };
};

struct PointLight
{
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 ambient{ 0.05f, 0.05f, 0.05f };
    glm::vec3 diffuse{ 1.0f, 0.7f, 0.3f };
    glm::vec3 specular{ 1.0f, 0.8f, 0.5f };

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    bool active = true;
};

struct SpotLight
{
    glm::vec3 position{ 0.0f, 0.0f, 0.0f };
    glm::vec3 direction{ 0.0f, -1.0f, 0.0f };
    glm::vec3 ambient{ 0.02f, 0.02f, 0.02f };
    glm::vec3 diffuse{ 1.2f, 1.1f, 0.9f };
    glm::vec3 specular{ 1.0f, 1.0f, 0.9f };

    float cutOff = 0.9659f;       // cos(15 deg)
    float outerCutOff = 0.9272f;  // cos(22 deg)

    float constant = 1.0f;
    float linear = 0.07f;
    float quadratic = 0.017f;
    bool active = true;
};

struct Material
{
    float shininess = 32.0f;
    float specularStrength = 0.5f;
};
