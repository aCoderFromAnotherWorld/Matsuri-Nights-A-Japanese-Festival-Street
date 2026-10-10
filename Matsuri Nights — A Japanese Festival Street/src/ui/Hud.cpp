#include "Hud.h"
#include "FontAtlasData.h"
#include "InteractionManager.h"
#include "../Camera.h"
#include "../Scene.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iostream>

namespace
{
    const char* HUD_FALLBACK_VERT = R"(#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec4 aColor;

out vec2 TexCoords;
out vec4 Color;

uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    Color = aColor;
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
}
)";

    const char* HUD_FALLBACK_FRAG = R"(#version 330 core
in vec2 TexCoords;
in vec4 Color;

out vec4 FragColor;

uniform sampler2D fontTexture;
uniform int mode; // 0 = solid quad, 1 = font glyph

void main()
{
    if (mode == 1)
    {
        float a = texture(fontTexture, TexCoords).r;
        if (a < 0.08)
            discard;
        // Boost text alpha for 100% solid, crisp, eye-soothing text
        float textAlpha = smoothstep(0.12, 0.45, a) * Color.a;
        FragColor = vec4(Color.rgb, textAlpha);
    }
    else
    {
        FragColor = Color;
    }
}
)";

    std::string truncate(const std::string& str, size_t maxLen)
    {
        if (str.length() <= maxLen) return str;
        return str.substr(0, maxLen - 2) + "..";
    }
}

Hud::Hud()
{
}

Hud::~Hud()
{
    if (vao != 0)
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
    if (vbo != 0)
    {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }
    if (fontTexture != 0)
    {
        glDeleteTextures(1, &fontTexture);
        fontTexture = 0;
    }
    if (shaderProgram != 0)
    {
        glDeleteProgram(shaderProgram);
        shaderProgram = 0;
    }
}

void Hud::init()
{
    // 1. Compile dedicated HUD Shader
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &HUD_FALLBACK_VERT, NULL);
    glCompileShader(vs);

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &HUD_FALLBACK_FRAG, NULL);
    glCompileShader(fs);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);

    // 2. Upload embedded 256x256 font atlas texture
    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, FONT_ATLAS_WIDTH, FONT_ATLAS_HEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, g_HudFontAtlasBytes);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // 3. Setup dynamic VAO & VBO
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Layout 0: Pos (2 floats)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(HudVertex), (void*)offsetof(HudVertex, x));

    // Layout 1: TexCoords (2 floats)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(HudVertex), (void*)offsetof(HudVertex, u));

    // Layout 2: Color (4 floats)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(HudVertex), (void*)offsetof(HudVertex, r));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Hud::addQuad(std::vector<HudVertex>& list, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, const glm::vec4& color)
{
    // Triangle 1
    list.push_back({ x0, y0, u0, v0, color.r, color.g, color.b, color.a });
    list.push_back({ x1, y0, u1, v0, color.r, color.g, color.b, color.a });
    list.push_back({ x0, y1, u0, v1, color.r, color.g, color.b, color.a });

    // Triangle 2
    list.push_back({ x1, y0, u1, v0, color.r, color.g, color.b, color.a });
    list.push_back({ x1, y1, u1, v1, color.r, color.g, color.b, color.a });
    list.push_back({ x0, y1, u0, v1, color.r, color.g, color.b, color.a });
}

void Hud::addRect(float x0, float y0, float x1, float y1, const glm::vec4& color)
{
    addQuad(solidVertices, x0, y0, x1, y1, 0.0f, 0.0f, 1.0f, 1.0f, color);
}

void Hud::addRectOutline(float x0, float y0, float x1, float y1, float thickness, const glm::vec4& color)
{
    addRect(x0, y0, x1, y0 + thickness, color);             // Top
    addRect(x0, y1 - thickness, x1, y1, color);             // Bottom
    addRect(x0, y0, x0 + thickness, y1, color);             // Left
    addRect(x1 - thickness, y0, x1, y1, color);             // Right
}

void Hud::addText(float x, float y, const std::string& text, const glm::vec4& color, float charWidth, float charHeight)
{
    float curX = x;

    for (char c : text)
    {
        if (c < 32 || c > 126)
        {
            curX += charWidth;
            continue;
        }

        int idx = static_cast<int>(c) - 32;
        int col = idx % FONT_GLYPH_COLS;
        int row = idx / FONT_GLYPH_COLS;

        float u0 = (col * FONT_CELL_WIDTH) / static_cast<float>(FONT_ATLAS_WIDTH);
        float u1 = ((col + 1) * FONT_CELL_WIDTH) / static_cast<float>(FONT_ATLAS_WIDTH);
        float v0 = (row * FONT_CELL_HEIGHT) / static_cast<float>(FONT_ATLAS_HEIGHT);
        float v1 = ((row + 1) * FONT_CELL_HEIGHT) / static_cast<float>(FONT_ATLAS_HEIGHT);

        float gx0 = curX;
        float gy0 = y;
        float gx1 = curX + charWidth;
        float gy1 = y + charHeight;

        // Clean solid glyph rendering with anti-aliasing
        addQuad(glyphVertices, gx0, gy0, gx1, gy1, u0, v0, u1, v1, color);

        curX += charWidth;
    }
}

void Hud::buildGeometry(int screenWidth, int screenHeight, const Camera& camera, const Scene& scene, const InteractionManager& interactionMgr, float fps, float frameTimeMs)
{
    solidVertices.clear();
    glyphVertices.clear();

    if (screenWidth < 320 || screenHeight < 240)
        return;

    // Responsive scaling
    float scaleFactor = std::clamp(static_cast<float>(screenHeight) / 720.0f, 0.85f, 1.3f);
    float panelWidth = 225.0f * scaleFactor;
    float margin = 12.0f * scaleFactor;
    float padding = 9.0f * scaleFactor;
    float charW = 8.0f * scaleFactor;
    float charH = 14.0f * scaleFactor;
    float lineH = 17.5f * scaleFactor;

    float px0 = screenWidth - panelWidth - margin;
    float py0 = margin;
    float px1 = screenWidth - margin;

    // Palette: Eye-soothing, modern, minimal dark glass theme
    glm::vec4 colBg(0.06f, 0.08f, 0.12f, 0.72f);        // 72% Translucent dark glass
    glm::vec4 colBorder(0.35f, 0.48f, 0.65f, 0.55f);    // Subtle soft steel-blue border
    glm::vec4 colFps(0.35f, 0.88f, 0.95f, 1.0f);        // Soft cyan for FPS
    glm::vec4 colWhite(0.95f, 0.96f, 0.98f, 1.0f);      // Crisp solid white
    glm::vec4 colTarget(1.0f, 0.86f, 0.30f, 1.0f);      // Warm gold for target
    glm::vec4 colLightOn(1.0f, 0.78f, 0.28f, 1.0f);     // Warm amber when lights ON
    glm::vec4 colLightOff(0.60f, 0.65f, 0.70f, 1.0f);    // Muted slate when lights OFF
    glm::vec4 colAction(0.40f, 0.92f, 0.50f, 1.0f);      // Soft emerald for interactive action
    glm::vec4 colFooter(0.55f, 0.62f, 0.72f, 0.90f);     // Dim footer

    float curY = py0 + padding;
    float curX = px0 + padding;
    size_t maxChars = static_cast<size_t>((panelWidth - 2.0f * padding) / charW);

    // 1. FPS line
    std::ostringstream ssFps;
    ssFps << std::fixed << std::setprecision(1) << "FPS: " << fps;
    addText(curX, curY, ssFps.str(), colFps, charW, charH);
    curY += lineH;

    // 2. Selected Target line
    const Interactable* sel = interactionMgr.getSelected();
    std::string targetName = sel ? truncate(sel->displayName, maxChars - 8) : "None";
    std::string targetLine = "Target: " + targetName;
    addText(curX, curY, targetLine, sel ? colTarget : colWhite, charW, charH);
    curY += lineH;

    // 3. Toggle Target hint
    addText(curX, curY, "[T] Toggle target", colWhite, charW, charH);
    curY += lineH;

    // 4. Lights Status & Toggle hint (Keys 0 / Numpad 0)
    std::string lightLine = scene.lanternLightsOn ? "[0] Lights: ON" : "[0] Lights: OFF";
    addText(curX, curY, lightLine, scene.lanternLightsOn ? colLightOn : colLightOff, charW, charH);
    curY += lineH;

    // 5. Context-Sensitive Action (if target has immediate interaction like Door/Window/Magic)
    if (sel && !sel->actions.empty())
    {
        for (const auto& act : sel->actions)
        {
            if (act.key == GLFW_KEY_0 || act.key == GLFW_KEY_KP_0)
                continue;

            std::string label = act.getLabel ? act.getLabel() : "";
            if (!label.empty())
            {
                std::string actLine = "[" + act.keyName + "] " + truncate(label, maxChars - 5);
                addText(curX, curY, actLine, colAction, charW, charH);
                curY += lineH;
                break; // Keep strictly minimal: only 1 most relevant context action
            }
        }
    }

    // 6. Footer: Hide HUD
    addText(curX, curY, "[F1] Hide HUD", colFooter, charW, charH);
    curY += lineH;

    float py1 = curY + padding - (lineH - charH);

    // Background rect and border outline
    addRect(px0, py0, px1, py1, colBg);
    addRectOutline(px0, py0, px1, py1, 1.2f, colBorder);
}

void Hud::uploadBuffers()
{
    std::vector<HudVertex> allVertices;
    allVertices.reserve(solidVertices.size() + glyphVertices.size());

    solidQuadVertexCount = static_cast<int>(solidVertices.size());
    glyphVertexCount = static_cast<int>(glyphVertices.size());

    allVertices.insert(allVertices.end(), solidVertices.begin(), solidVertices.end());
    allVertices.insert(allVertices.end(), glyphVertices.begin(), glyphVertices.end());

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(HudVertex), allVertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Hud::update(float dt, const Camera& camera, const Scene& scene, const InteractionManager& interactionMgr, float fps, float frameTimeMs, int screenWidth, int screenHeight)
{
    if (screenWidth > 0 && screenHeight > 0)
    {
        lastScreenWidth = screenWidth;
        lastScreenHeight = screenHeight;
    }

    timeSinceLastUpdate += dt;

    // 10 Hz rate limiter
    if (timeSinceLastUpdate >= 0.10f)
    {
        timeSinceLastUpdate = 0.0f;

        const auto* sel = interactionMgr.getSelected();
        std::string currentHash = std::to_string(static_cast<int>(fps * 10)) + "_"
                                + (sel ? sel->id : "none") + "_"
                                + (sel && sel->getStatusString ? sel->getStatusString() : "") + "_"
                                + std::to_string(scene.lanternLightsOn) + "_"
                                + std::to_string(lastScreenWidth) + "_"
                                + std::to_string(lastScreenHeight);

        if (currentHash != cachedContentHash)
        {
            cachedContentHash = currentHash;
            buildGeometry(lastScreenWidth, lastScreenHeight, camera, scene, interactionMgr, fps, frameTimeMs);
            uploadBuffers();
        }
    }
}

void Hud::render(int screenWidth, int screenHeight)
{
    if (!isVisible || screenWidth <= 0 || screenHeight <= 0)
        return;

    if (screenWidth != lastScreenWidth || screenHeight != lastScreenHeight)
    {
        lastScreenWidth = screenWidth;
        lastScreenHeight = screenHeight;
        cachedContentHash = ""; // Force rebuild on window resize
    }

    if (solidQuadVertexCount + glyphVertexCount == 0)
        return;

    // -------------------------------------------------------------
    // Save previous OpenGL states exactly to guarantee zero leaks
    // -------------------------------------------------------------
    GLboolean depthTestWasEnabled = glIsEnabled(GL_DEPTH_TEST);
    GLboolean cullFaceWasEnabled = glIsEnabled(GL_CULL_FACE);
    GLboolean blendWasEnabled = glIsEnabled(GL_BLEND);

    GLint prevBlendSrcRGB, prevBlendDstRGB, prevBlendSrcAlpha, prevBlendDstAlpha;
    glGetIntegerv(GL_BLEND_SRC_RGB, &prevBlendSrcRGB);
    glGetIntegerv(GL_BLEND_DST_RGB, &prevBlendDstRGB);
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &prevBlendSrcAlpha);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &prevBlendDstAlpha);

    GLint prevProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &prevProgram);

    GLint prevVAO = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &prevVAO);

    GLint prevVBO = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &prevVBO);

    GLint prevActiveTexture = 0;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &prevActiveTexture);

    GLint prevTexture2D = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTexture2D);

    GLint prevViewport[4];
    glGetIntegerv(GL_VIEWPORT, prevViewport);

    // -------------------------------------------------------------
    // Setup 2D Orthographic Pass
    // -------------------------------------------------------------
    glViewport(0, 0, screenWidth, screenHeight);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(shaderProgram);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), static_cast<float>(screenHeight), 0.0f, -1.0f, 1.0f);
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    GLint modeLoc = glGetUniformLocation(shaderProgram, "mode");
    GLint texLoc = glGetUniformLocation(shaderProgram, "fontTexture");
    glUniform1i(texLoc, 0);

    glBindVertexArray(vao);

    // Pass 1: Render solid colored background quads and border lines (mode = 0)
    if (solidQuadVertexCount > 0)
    {
        glUniform1i(modeLoc, 0);
        glDrawArrays(GL_TRIANGLES, 0, solidQuadVertexCount);
    }

    // Pass 2: Render textured glyph quads (mode = 1)
    if (glyphVertexCount > 0)
    {
        glUniform1i(modeLoc, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, fontTexture);
        glDrawArrays(GL_TRIANGLES, solidQuadVertexCount, glyphVertexCount);
    }

    // -------------------------------------------------------------
    // Restore previous OpenGL states exactly
    // -------------------------------------------------------------
    glViewport(prevViewport[0], prevViewport[1], prevViewport[2], prevViewport[3]);
    glActiveTexture(prevActiveTexture);
    glBindTexture(GL_TEXTURE_2D, prevTexture2D);
    glBindBuffer(GL_ARRAY_BUFFER, prevVBO);
    glBindVertexArray(prevVAO);
    glUseProgram(prevProgram);

    glBlendFuncSeparate(prevBlendSrcRGB, prevBlendDstRGB, prevBlendSrcAlpha, prevBlendDstAlpha);
    if (!blendWasEnabled) glDisable(GL_BLEND); else glEnable(GL_BLEND);
    if (cullFaceWasEnabled) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    if (depthTestWasEnabled) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
}
