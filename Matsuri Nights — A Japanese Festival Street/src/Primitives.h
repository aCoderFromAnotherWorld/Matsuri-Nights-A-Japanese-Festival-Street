#pragma once

#include "Mesh.h"
#include "Curves.h"
#include <glm/glm.hpp>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Primitives
{
public:
    static Mesh createCube(float size = 1.0f)
    {
        float h = size * 0.5f;
        std::vector<Vertex> vertices = {
            // Front face (+Z)
            { { -h, -h,  h }, {  0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f } },
            { {  h, -h,  h }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f } },
            { {  h,  h,  h }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 1.0f } },
            { { -h,  h,  h }, {  0.0f,  0.0f,  1.0f }, { 0.0f, 1.0f } },

            // Back face (-Z)
            { {  h, -h, -h }, {  0.0f,  0.0f, -1.0f }, { 0.0f, 0.0f } },
            { { -h, -h, -h }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f } },
            { { -h,  h, -h }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f } },
            { {  h,  h, -h }, {  0.0f,  0.0f, -1.0f }, { 0.0f, 1.0f } },

            // Left face (-X)
            { { -h, -h, -h }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f } },
            { { -h, -h,  h }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f } },
            { { -h,  h,  h }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f } },
            { { -h,  h, -h }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f } },

            // Right face (+X)
            { {  h, -h,  h }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f } },
            { {  h, -h, -h }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f } },
            { {  h,  h, -h }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f } },
            { {  h,  h,  h }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f } },

            // Top face (+Y)
            { { -h,  h,  h }, {  0.0f,  1.0f,  0.0f }, { 0.0f, 0.0f } },
            { {  h,  h,  h }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 0.0f } },
            { {  h,  h, -h }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 1.0f } },
            { { -h,  h, -h }, {  0.0f,  1.0f,  0.0f }, { 0.0f, 1.0f } },

            // Bottom face (-Y)
            { { -h, -h, -h }, {  0.0f, -1.0f,  0.0f }, { 0.0f, 0.0f } },
            { {  h, -h, -h }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 0.0f } },
            { {  h, -h,  h }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 1.0f } },
            { { -h, -h,  h }, {  0.0f, -1.0f,  0.0f }, { 0.0f, 1.0f } }
        };

        std::vector<unsigned int> indices;
        for (unsigned int f = 0; f < 6; ++f)
        {
            unsigned int offset = f * 4;
            indices.push_back(offset + 0);
            indices.push_back(offset + 1);
            indices.push_back(offset + 2);
            indices.push_back(offset + 2);
            indices.push_back(offset + 3);
            indices.push_back(offset + 0);
        }

        return Mesh(vertices, indices);
    }

    static Mesh createCylinder(float radius = 0.5f, float height = 1.0f, int segments = 24)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        float halfH = height * 0.5f;

        // Side wall vertices
        for (int i = 0; i <= segments; ++i)
        {
            float theta = (float)i / (float)segments * 2.0f * (float)M_PI;
            float cosT = std::cos(theta);
            float sinT = std::sin(theta);
            float u = (float)i / (float)segments;

            glm::vec3 norm(cosT, 0.0f, sinT);

            // Top ring
            vertices.push_back({ { radius * cosT,  halfH, radius * sinT }, norm, { u, 1.0f } });
            // Bottom ring
            vertices.push_back({ { radius * cosT, -halfH, radius * sinT }, norm, { u, 0.0f } });
        }

        // Side indices
        for (int i = 0; i < segments; ++i)
        {
            unsigned int top1 = i * 2;
            unsigned int bot1 = top1 + 1;
            unsigned int top2 = (i + 1) * 2;
            unsigned int bot2 = top2 + 1;

            indices.push_back(top1);
            indices.push_back(bot1);
            indices.push_back(top2);

            indices.push_back(top2);
            indices.push_back(bot1);
            indices.push_back(bot2);
        }

        // Top cap center
        unsigned int topCenterIndex = static_cast<unsigned int>(vertices.size());
        vertices.push_back({ { 0.0f, halfH, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.5f, 0.5f } });

        unsigned int topRingStart = static_cast<unsigned int>(vertices.size());
        for (int i = 0; i <= segments; ++i)
        {
            float theta = (float)i / (float)segments * 2.0f * (float)M_PI;
            float cosT = std::cos(theta);
            float sinT = std::sin(theta);
            vertices.push_back({ { radius * cosT, halfH, radius * sinT }, { 0.0f, 1.0f, 0.0f }, { 0.5f + 0.5f * cosT, 0.5f + 0.5f * sinT } });
        }
        for (int i = 0; i < segments; ++i)
        {
            indices.push_back(topCenterIndex);
            indices.push_back(topRingStart + i);
            indices.push_back(topRingStart + i + 1);
        }

        // Bottom cap center
        unsigned int botCenterIndex = static_cast<unsigned int>(vertices.size());
        vertices.push_back({ { 0.0f, -halfH, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.5f, 0.5f } });

        unsigned int botRingStart = static_cast<unsigned int>(vertices.size());
        for (int i = 0; i <= segments; ++i)
        {
            float theta = (float)i / (float)segments * 2.0f * (float)M_PI;
            float cosT = std::cos(theta);
            float sinT = std::sin(theta);
            vertices.push_back({ { radius * cosT, -halfH, radius * sinT }, { 0.0f, -1.0f, 0.0f }, { 0.5f + 0.5f * cosT, 0.5f + 0.5f * sinT } });
        }
        for (int i = 0; i < segments; ++i)
        {
            indices.push_back(botCenterIndex);
            indices.push_back(botRingStart + i + 1);
            indices.push_back(botRingStart + i);
        }

        return Mesh(vertices, indices);
    }

    static Mesh createCone(float radius = 0.5f, float height = 1.0f, int segments = 24)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        float halfH = height * 0.5f;

        // Apex vertex
        unsigned int apexIndex = 0;
        // Side wall
        for (int i = 0; i <= segments; ++i)
        {
            float theta = (float)i / (float)segments * 2.0f * (float)M_PI;
            float cosT = std::cos(theta);
            float sinT = std::sin(theta);
            float u = (float)i / (float)segments;

            // Slanted normal for cone mantle
            glm::vec3 norm = glm::normalize(glm::vec3(cosT, radius / height, sinT));

            // Apex copy with matching normal for smooth shading
            vertices.push_back({ { 0.0f, halfH, 0.0f }, norm, { u, 1.0f } });
            // Base ring vertex
            vertices.push_back({ { radius * cosT, -halfH, radius * sinT }, norm, { u, 0.0f } });
        }

        for (int i = 0; i < segments; ++i)
        {
            unsigned int a = i * 2;
            unsigned int b = a + 1;
            unsigned int nextB = (i + 1) * 2 + 1;
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(nextB);
        }

        // Bottom disc
        unsigned int botCenterIndex = static_cast<unsigned int>(vertices.size());
        vertices.push_back({ { 0.0f, -halfH, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.5f, 0.5f } });

        unsigned int botRingStart = static_cast<unsigned int>(vertices.size());
        for (int i = 0; i <= segments; ++i)
        {
            float theta = (float)i / (float)segments * 2.0f * (float)M_PI;
            float cosT = std::cos(theta);
            float sinT = std::sin(theta);
            vertices.push_back({ { radius * cosT, -halfH, radius * sinT }, { 0.0f, -1.0f, 0.0f }, { 0.5f + 0.5f * cosT, 0.5f + 0.5f * sinT } });
        }
        for (int i = 0; i < segments; ++i)
        {
            indices.push_back(botCenterIndex);
            indices.push_back(botRingStart + i + 1);
            indices.push_back(botRingStart + i);
        }

        return Mesh(vertices, indices);
    }

    static Mesh createSphere(float radius = 0.5f, int rings = 20, int sectors = 24)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (int r = 0; r <= rings; ++r)
        {
            float phi = - (float)M_PI * 0.5f + (float)M_PI * ((float)r / (float)rings);
            float cosPhi = std::cos(phi);
            float sinPhi = std::sin(phi);
            float v = (float)r / (float)rings;

            for (int s = 0; s <= sectors; ++s)
            {
                float theta = 2.0f * (float)M_PI * ((float)s / (float)sectors);
                float cosTheta = std::cos(theta);
                float sinTheta = std::sin(theta);
                float u = (float)s / (float)sectors;

                glm::vec3 norm(cosPhi * cosTheta, sinPhi, cosPhi * sinTheta);
                glm::vec3 pos = norm * radius;

                vertices.push_back({ pos, norm, { u, v } });
            }
        }

        for (int r = 0; r < rings; ++r)
        {
            for (int s = 0; s < sectors; ++s)
            {
                unsigned int cur = r * (sectors + 1) + s;
                unsigned int next = cur + sectors + 1;

                indices.push_back(cur);
                indices.push_back(next);
                indices.push_back(cur + 1);

                indices.push_back(cur + 1);
                indices.push_back(next);
                indices.push_back(next + 1);
            }
        }

        return Mesh(vertices, indices);
    }

    static Mesh createPlane(float width = 1.0f, float depth = 1.0f, int gridX = 1, int gridZ = 1)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        float halfW = width * 0.5f;
        float halfD = depth * 0.5f;

        for (int z = 0; z <= gridZ; ++z)
        {
            float fz = (float)z / (float)gridZ;
            float posZ = -halfD + fz * depth;

            for (int x = 0; x <= gridX; ++x)
            {
                float fx = (float)x / (float)gridX;
                float posX = -halfW + fx * width;

                vertices.push_back({ { posX, 0.0f, posZ }, { 0.0f, 1.0f, 0.0f }, { fx, fz } });
            }
        }

        for (int z = 0; z < gridZ; ++z)
        {
            for (int x = 0; x < gridX; ++x)
            {
                unsigned int row1 = z * (gridX + 1);
                unsigned int row2 = (z + 1) * (gridX + 1);

                indices.push_back(row1 + x);
                indices.push_back(row2 + x);
                indices.push_back(row1 + x + 1);

                indices.push_back(row1 + x + 1);
                indices.push_back(row2 + x);
                indices.push_back(row2 + x + 1);
            }
        }

        return Mesh(vertices, indices);
    }

    // -------------------------------------------------------------
    // Curved Geometry Generators (swept tubes, splines, beams, ropes)
    // -------------------------------------------------------------
    static Mesh createSweptTube(
        const std::function<glm::vec3(float)>& evalPos,
        const std::function<glm::vec3(float)>& evalTan,
        const std::function<float(float)>& radiusFunc,
        int lengthSegments = 24,
        int radialSegments = 16,
        float vTiling = 1.0f,
        bool capStart = true,
        bool capEnd = true,
        const glm::vec3& initialUp = glm::vec3(0.0f, 1.0f, 0.0f))
    {
        return Curves::createSweptTube(evalPos, evalTan, radiusFunc, lengthSegments, radialSegments, vTiling, capStart, capEnd, initialUp);
    }

    static Mesh createBezierTube(
        const Curves::Bezier3& bezier,
        float rStart,
        float rEnd,
        int lengthSegments = 24,
        int radialSegments = 16,
        float vTiling = 1.0f,
        bool capStart = true,
        bool capEnd = true,
        const glm::vec3& initialUp = glm::vec3(0.0f, 1.0f, 0.0f))
    {
        return Curves::createBezierTube(bezier, rStart, rEnd, lengthSegments, radialSegments, vTiling, capStart, capEnd, initialUp);
    }

    static Mesh createBezierTube2(
        const Curves::Bezier2& bezier,
        float rStart,
        float rEnd,
        int lengthSegments = 16,
        int radialSegments = 12,
        float vTiling = 1.0f,
        bool capStart = true,
        bool capEnd = true,
        const glm::vec3& initialUp = glm::vec3(0.0f, 1.0f, 0.0f))
    {
        return Curves::createBezierTube2(bezier, rStart, rEnd, lengthSegments, radialSegments, vTiling, capStart, capEnd, initialUp);
    }

    static Mesh createSplineTube(
        const std::vector<glm::vec3>& pts,
        float rStart,
        float rEnd,
        int lengthSegments = 32,
        int radialSegments = 16,
        float vTiling = 2.0f,
        bool capStart = true,
        bool capEnd = true,
        const glm::vec3& initialUp = glm::vec3(0.0f, 1.0f, 0.0f))
    {
        return Curves::createSplineTube(pts, rStart, rEnd, lengthSegments, radialSegments, vTiling, capStart, capEnd, initialUp);
    }

    static Mesh createCurvedBeam(
        float spanX,
        float baseWidthZ,
        float baseHeightY,
        float soriUpward = 0.35f,
        float endFlare = 1.15f,
        int lengthSegments = 32,
        bool roofBevel = true)
    {
        return Curves::createCurvedBeam(spanX, baseWidthZ, baseHeightY, soriUpward, endFlare, lengthSegments, roofBevel);
    }

    static Mesh createCatenaryRope(
        float halfSpan = 3.8f,
        float yPole = 6.20f,
        float sag = 0.65f,
        float radius = 0.035f,
        int lengthSegments = 36,
        int radialSegments = 12)
    {
        return Curves::createCatenaryRope(halfSpan, yPole, sag, radius, lengthSegments, radialSegments);
    }

    static Mesh createCurvedLeaf(
        float length = 0.12f,
        float maxWidth = 0.045f,
        float archY = 0.02f,
        float foldAngle = 25.0f,
        int lengthSegs = 10,
        int widthSegs = 6)
    {
        return Curves::createCurvedLeafMesh(length, maxWidth, archY, foldAngle, lengthSegs, widthSegs);
    }

    static Mesh createCurvedPetal(
        float length = 0.06f,
        float width = 0.045f,
        float cupDepth = 0.014f,
        int segsU = 8,
        int segsV = 6)
    {
        return Curves::createCurvedPetalMesh(length, width, cupDepth, segsU, segsV);
    }

    static Mesh createSakuraBlossomLobe(
        float radius = 1.0f,
        int rings = 18,
        int sectors = 22,
        float billowAmp = 0.24f)
    {
        return Curves::createSakuraBlossomLobe(radius, rings, sectors, billowAmp);
    }

    static Mesh createPineNeedleCluster(
        float widthX = 0.24f,
        float heightY = 0.08f,
        float depthZ = 0.20f,
        int rings = 14,
        int sectors = 18)
    {
        return Curves::createPineNeedleClusterMesh(widthX, heightY, depthZ, rings, sectors);
    }

    static Mesh createHumanHead(float scale = 0.20f)
    {
        return Curves::createHumanHeadMesh(scale);
    }

    static Mesh createHumanTorso(float width = 0.52f, float height = 0.85f, float depth = 0.36f)
    {
        return Curves::createHumanTorsoMesh(width, height, depth);
    }

    static Mesh createArticulatedLimb(float rTop = 0.08f, float rBottom = 0.06f, float length = 0.45f, int lenSegs = 12, int radSegs = 14)
    {
        return Curves::createArticulatedLimbMesh(rTop, rBottom, length, lenSegs, radSegs);
    }

    static Mesh createHand(float length = 0.14f, float width = 0.08f, float thickness = 0.035f)
    {
        return Curves::createHandMesh(length, width, thickness);
    }

    static Mesh createGetaFoot(float length = 0.25f, float width = 0.12f, float height = 0.065f)
    {
        return Curves::createGetaFootMesh(length, width, height);
    }
};
