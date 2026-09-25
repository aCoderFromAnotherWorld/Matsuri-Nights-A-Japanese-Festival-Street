#pragma once

#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include <vector>
#include <string>
#include <memory>

class SceneNode : public std::enable_shared_from_this<SceneNode>
{
public:
    std::string name;
    Transform transform;
    glm::mat4 worldMatrix{ 1.0f };

    SceneNode* parent = nullptr;
    std::vector<std::shared_ptr<SceneNode>> children;

    const Mesh* mesh = nullptr;
    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
    bool isWindow = false;
    bool isSky = false;
    bool isEmissive = false;
    glm::vec3 emissiveColor{ 0.0f, 0.0f, 0.0f };
    bool visible = true;

    SceneNode(const std::string& nodeName = "Node")
        : name(nodeName) {}

    void addChild(std::shared_ptr<SceneNode> child)
    {
        if (child)
        {
            child->parent = this;
            children.push_back(child);
        }
    }

    void updateWorldMatrix(const glm::mat4& parentMatrix = glm::mat4(1.0f))
    {
        worldMatrix = parentMatrix * transform.getLocalMatrix();
        for (auto& child : children)
        {
            child->updateWorldMatrix(worldMatrix);
        }
    }

    void draw(const Shader& shader) const
    {
        if (!visible)
            return;

        if (mesh)
        {
            shader.setMat4("model", worldMatrix);
            shader.setVec4("objectColor", color);
            shader.setBool("isWindow", isWindow);
            shader.setBool("isSky", isSky);
            shader.setBool("isEmissive", isEmissive);
            shader.setVec3("emissiveColor", emissiveColor);

            mesh->Draw();
        }

        for (const auto& child : children)
        {
            child->draw(shader);
        }
    }

    glm::vec3 getWorldPosition() const
    {
        return glm::vec3(worldMatrix[3]);
    }

    std::shared_ptr<SceneNode> findNode(const std::string& targetName)
    {
        if (name == targetName)
            return shared_from_this();
        for (auto& child : children)
        {
            auto found = child->findNode(targetName);
            if (found)
                return found;
        }
        return nullptr;
    }
};
