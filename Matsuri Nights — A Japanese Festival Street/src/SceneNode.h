#pragma once

#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
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
    std::shared_ptr<Mesh> ownedMesh = nullptr;

    void setMesh(const Mesh* m)
    {
        mesh = m;
        ownedMesh = nullptr;
    }

    void setMesh(std::shared_ptr<Mesh> m)
    {
        ownedMesh = m;
        mesh = ownedMesh ? ownedMesh.get() : nullptr;
    }

    glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
    bool isWindow = false;
    bool isSky = false;
    bool isEmissive = false;
    glm::vec3 emissiveColor{ 0.0f, 0.0f, 0.0f };
    bool visible = true;

    // Phase 2: Material properties for Blinn-Phong Illumination
    float shininess = 32.0f;
    float specularStrength = 0.5f;

    // Phase 3: Texture mapping
    const Texture* texture = nullptr;
    float textureTiling = 1.0f;

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

            // Phase 2: Material parameters
            shader.setFloat("material.shininess", shininess);
            shader.setFloat("material.specularStrength", specularStrength);

            // Phase 3: Texture parameters
            if (texture && texture->id != 0)
            {
                shader.setBool("useTexture", true);
                texture->bind(0);
                shader.setInt("diffuseTexture", 0);
                shader.setFloat("textureTiling", textureTiling);
            }
            else
            {
                shader.setBool("useTexture", false);
            }

            mesh->Draw();
        }

        for (const auto& child : children)
        {
            child->draw(shader);
        }
    }

    void drawDepth(const Shader& depthShader) const
    {
        if (!visible || isSky || isEmissive)
            return;

        if (mesh)
        {
            depthShader.setMat4("model", worldMatrix);
            mesh->Draw();
        }

        for (const auto& child : children)
        {
            child->drawDepth(depthShader);
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
