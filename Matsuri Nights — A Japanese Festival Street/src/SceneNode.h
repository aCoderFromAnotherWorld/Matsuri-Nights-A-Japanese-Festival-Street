#pragma once

#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include <vector>
#include <string>
#include <memory>

struct RenderContext
{
    GLint locModel = -1;
    GLint locNormalMatrix = -1;
    GLint locColor = -1;
    GLint locIsWindow = -1;
    GLint locIsSky = -1;
    GLint locIsEmissive = -1;
    GLint locEmissiveColor = -1;
    GLint locShininess = -1;
    GLint locSpecularStrength = -1;
    GLint locUseTexture = -1;
    GLint locDiffuseTexture = -1;
    GLint locTextureTiling = -1;

    mutable float lastShininess = -999.0f;
    mutable float lastSpecularStrength = -999.0f;
    mutable int lastUseTexture = -1;
    mutable const Texture* lastTexture = nullptr;
    mutable float lastTextureTiling = -999.0f;

    void init(const Shader& shader)
    {
        locModel = shader.getUniformLocation("model");
        locNormalMatrix = shader.getUniformLocation("normalMatrix");
        locColor = shader.getUniformLocation("objectColor");
        locIsWindow = shader.getUniformLocation("isWindow");
        locIsSky = shader.getUniformLocation("isSky");
        locIsEmissive = shader.getUniformLocation("isEmissive");
        locEmissiveColor = shader.getUniformLocation("emissiveColor");
        locShininess = shader.getUniformLocation("material.shininess");
        locSpecularStrength = shader.getUniformLocation("material.specularStrength");
        locUseTexture = shader.getUniformLocation("useTexture");
        locDiffuseTexture = shader.getUniformLocation("diffuseTexture");
        locTextureTiling = shader.getUniformLocation("textureTiling");

        lastShininess = -999.0f;
        lastSpecularStrength = -999.0f;
        lastUseTexture = -1;
        lastTexture = nullptr;
        lastTextureTiling = -999.0f;
    }
};

struct DepthRenderContext
{
    GLint locModel = -1;

    void init(const Shader& depthShader)
    {
        locModel = depthShader.getUniformLocation("model");
    }
};

class SceneNode : public std::enable_shared_from_this<SceneNode>
{
public:
    std::string name;
    Transform transform;
    glm::mat4 worldMatrix{ 1.0f };
    glm::mat3 normalMatrix{ 1.0f };

    glm::mat4 lastParentMatrix{ 0.0f };
    bool isStatic = false;
    bool castShadow = true;
    bool matrixDirty = true;

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

    void setStaticRecursive(bool stat)
    {
        isStatic = stat;
        for (auto& child : children)
        {
            child->setStaticRecursive(stat);
        }
    }

    void setCastShadowRecursive(bool cast)
    {
        castShadow = cast;
        for (auto& child : children)
        {
            child->setCastShadowRecursive(cast);
        }
    }

    void updateWorldMatrix(const glm::mat4& parentMatrix = glm::mat4(1.0f))
    {
        bool localChanged = transform.checkDirty();
        bool parentChanged = (parentMatrix != lastParentMatrix);

        if (localChanged || parentChanged || matrixDirty)
        {
            worldMatrix = parentMatrix * transform.getLocalMatrix();
            normalMatrix = glm::transpose(glm::inverse(glm::mat3(worldMatrix)));
            lastParentMatrix = parentMatrix;
            matrixDirty = false;

            for (auto& child : children)
            {
                child->updateWorldMatrix(worldMatrix);
            }
        }
        else
        {
            // Node and parent did not change. If node is marked static, no child can have changed.
            if (!isStatic)
            {
                for (auto& child : children)
                {
                    child->updateWorldMatrix(worldMatrix);
                }
            }
        }
    }

    void draw(const Shader& shader, const RenderContext& ctx) const
    {
        if (!visible)
            return;

        if (mesh)
        {
            Shader::setMat4(ctx.locModel, worldMatrix);
            if (ctx.locNormalMatrix >= 0)
                Shader::setMat3(ctx.locNormalMatrix, normalMatrix);
            Shader::setVec4(ctx.locColor, color);
            Shader::setBool(ctx.locIsWindow, isWindow);
            Shader::setBool(ctx.locIsSky, isSky);
            Shader::setBool(ctx.locIsEmissive, isEmissive);
            if (isEmissive)
                Shader::setVec3(ctx.locEmissiveColor, emissiveColor);

            if (shininess != ctx.lastShininess)
            {
                Shader::setFloat(ctx.locShininess, shininess);
                ctx.lastShininess = shininess;
            }
            if (specularStrength != ctx.lastSpecularStrength)
            {
                Shader::setFloat(ctx.locSpecularStrength, specularStrength);
                ctx.lastSpecularStrength = specularStrength;
            }

            bool hasTex = (texture && texture->id != 0);
            if ((int)hasTex != ctx.lastUseTexture)
            {
                Shader::setBool(ctx.locUseTexture, hasTex);
                ctx.lastUseTexture = (int)hasTex;
            }
            if (hasTex)
            {
                if (texture != ctx.lastTexture)
                {
                    texture->bind(0);
                    Shader::setInt(ctx.locDiffuseTexture, 0);
                    ctx.lastTexture = texture;
                }
                if (textureTiling != ctx.lastTextureTiling)
                {
                    Shader::setFloat(ctx.locTextureTiling, textureTiling);
                    ctx.lastTextureTiling = textureTiling;
                }
            }

            mesh->Draw();
        }

        for (const auto& child : children)
        {
            child->draw(shader, ctx);
        }
    }

    void draw(const Shader& shader) const
    {
        RenderContext ctx;
        ctx.init(shader);
        draw(shader, ctx);
    }

    void drawDepth(const Shader& depthShader, const DepthRenderContext& ctx) const
    {
        if (!visible || isSky || isEmissive || !castShadow)
            return;

        if (mesh)
        {
            Shader::setMat4(ctx.locModel, worldMatrix);
            mesh->Draw();
        }

        for (const auto& child : children)
        {
            child->drawDepth(depthShader, ctx);
        }
    }

    void drawDepth(const Shader& depthShader) const
    {
        DepthRenderContext ctx;
        ctx.init(depthShader);
        drawDepth(depthShader, ctx);
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
