#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include "Interactable.h"

class Camera;
class Scene;
class InteractionManager;

struct HudVertex
{
    float x, y;
    float u, v;
    float r, g, b, a;
};

class Hud
{
public:
    bool isVisible = true;

    Hud();
    ~Hud();

    void init();
    void update(float dt, const Camera& camera, const Scene& scene, const InteractionManager& interactionMgr, float fps, float frameTimeMs, int screenWidth = 0, int screenHeight = 0);
    void render(int screenWidth, int screenHeight);
    void toggleVisibility() { isVisible = !isVisible; }

private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
    unsigned int fontTexture = 0;
    unsigned int shaderProgram = 0;

    int solidQuadVertexCount = 0;
    int glyphVertexCount = 0;

    std::vector<HudVertex> solidVertices;
    std::vector<HudVertex> glyphVertices;

    float timeSinceLastUpdate = 0.0f;
    std::string cachedContentHash;
    int lastScreenWidth = 1280;
    int lastScreenHeight = 720;

    void addQuad(std::vector<HudVertex>& list, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, const glm::vec4& color);
    void addRect(float x0, float y0, float x1, float y1, const glm::vec4& color);
    void addRectOutline(float x0, float y0, float x1, float y1, float thickness, const glm::vec4& color);
    void addText(float x, float y, const std::string& text, const glm::vec4& color, float charWidth = 8.5f, float charHeight = 15.0f);

    void buildGeometry(int screenWidth, int screenHeight, const Camera& camera, const Scene& scene, const InteractionManager& interactionMgr, float fps, float frameTimeMs);
    void uploadBuffers();
};
