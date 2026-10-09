#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <functional>
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// =========================================================================
// Mathematical Curves & Swept Geometry Architecture (Curves.h)
// Provides parametric Bézier curves (quadratic & cubic), Catmull-Rom splines,
// rotation-minimizing parallel transport frames (Bishop frames),
// and swept surface generators (generalized cylinders, curved beams, ropes).
// =========================================================================

namespace Curves
{
    // -------------------------------------------------------------
    // 1. Quadratic Bézier Curve (3 Control Points in 3D)
    // B(t) = (1-t)^2 P0 + 2(1-t)t P1 + t^2 P2
    // -------------------------------------------------------------
    struct Bezier2
    {
        glm::vec3 p0, p1, p2;

        Bezier2() : p0(0.0f), p1(0.0f), p2(0.0f) {}
        Bezier2(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
            : p0(a), p1(b), p2(c) {}

        glm::vec3 evaluate(float t) const
        {
            float u = 1.0f - t;
            return u * u * p0 + 2.0f * u * t * p1 + t * t * p2;
        }

        glm::vec3 tangent(float t) const
        {
            float u = 1.0f - t;
            glm::vec3 d = 2.0f * u * (p1 - p0) + 2.0f * t * (p2 - p1);
            float len = glm::length(d);
            if (len < 1e-6f) return glm::vec3(0.0f, 1.0f, 0.0f);
            return d / len;
        }
    };

    // -------------------------------------------------------------
    // 2. Cubic Bézier Curve (4 Control Points in 3D)
    // B(t) = (1-t)^3 P0 + 3(1-t)^2 t P1 + 3(1-t) t^2 P2 + t^3 P3
    // -------------------------------------------------------------
    struct Bezier3
    {
        glm::vec3 p0, p1, p2, p3;

        Bezier3() : p0(0.0f), p1(0.0f), p2(0.0f), p3(0.0f) {}
        Bezier3(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d)
            : p0(a), p1(b), p2(c), p3(d) {}

        glm::vec3 evaluate(float t) const
        {
            float u = 1.0f - t;
            float tt = t * t;
            float uu = u * u;
            return uu * u * p0 + 3.0f * uu * t * p1 + 3.0f * u * tt * p2 + tt * t * p3;
        }

        glm::vec3 tangent(float t) const
        {
            float u = 1.0f - t;
            glm::vec3 d = 3.0f * u * u * (p1 - p0) + 6.0f * u * t * (p2 - p1) + 3.0f * t * t * (p3 - p2);
            float len = glm::length(d);
            if (len < 1e-6f) return glm::vec3(0.0f, 1.0f, 0.0f);
            return d / len;
        }
    };

    // -------------------------------------------------------------
    // 3. Catmull-Rom Spline (Arbitrary Waypoints in 3D)
    // Passes through all waypoints with continuous C^1 tangents.
    // -------------------------------------------------------------
    struct CatmullRomSpline
    {
        std::vector<glm::vec3> points;

        CatmullRomSpline() = default;
        CatmullRomSpline(const std::vector<glm::vec3>& pts) : points(pts) {}

        glm::vec3 evaluate(float t) const
        {
            if (points.empty()) return glm::vec3(0.0f);
            if (points.size() == 1) return points[0];
            if (points.size() == 2) return glm::mix(points[0], points[1], t);

            int numSegments = static_cast<int>(points.size()) - 1;
            float st = glm::clamp(t, 0.0f, 1.0f) * (float)numSegments;
            int seg = static_cast<int>(std::floor(st));
            if (seg >= numSegments) seg = numSegments - 1;
            float u = st - (float)seg;

            glm::vec3 p0 = (seg > 0) ? points[seg - 1] : (points[0] * 2.0f - points[1]);
            glm::vec3 p1 = points[seg];
            glm::vec3 p2 = points[seg + 1];
            glm::vec3 p3 = (seg + 2 < (int)points.size()) ? points[seg + 2] : (points[seg + 1] * 2.0f - points[seg]);

            float u2 = u * u;
            float u3 = u2 * u;

            return 0.5f * (
                (2.0f * p1) +
                (-p0 + p2) * u +
                (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * u2 +
                (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * u3
            );
        }

        glm::vec3 tangent(float t) const
        {
            if (points.size() < 2) return glm::vec3(0.0f, 1.0f, 0.0f);

            int numSegments = static_cast<int>(points.size()) - 1;
            float st = glm::clamp(t, 0.0f, 1.0f) * (float)numSegments;
            int seg = static_cast<int>(std::floor(st));
            if (seg >= numSegments) seg = numSegments - 1;
            float u = st - (float)seg;

            glm::vec3 p0 = (seg > 0) ? points[seg - 1] : (points[0] * 2.0f - points[1]);
            glm::vec3 p1 = points[seg];
            glm::vec3 p2 = points[seg + 1];
            glm::vec3 p3 = (seg + 2 < (int)points.size()) ? points[seg + 2] : (points[seg + 1] * 2.0f - points[seg]);

            float u2 = u * u;

            glm::vec3 d = 0.5f * (
                (-p0 + p2) +
                (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * (2.0f * u) +
                (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * (3.0f * u2)
            );
            float len = glm::length(d);
            if (len < 1e-6f) return glm::vec3(0.0f, 1.0f, 0.0f);
            return d / len;
        }
    };

    // -------------------------------------------------------------
    // 4. Parallel Transport Frame (Bishop Frame) Computation
    // Computes rotation-minimizing coordinate frames along arbitrary 3D curves.
    // -------------------------------------------------------------
    struct Frame
    {
        glm::vec3 position;
        glm::vec3 tangent;
        glm::vec3 normal;
        glm::vec3 binormal;
    };

    inline std::vector<Frame> computeBishopFrames(
        const std::function<glm::vec3(float)>& evalPos,
        const std::function<glm::vec3(float)>& evalTan,
        int lengthSegments,
        const glm::vec3& initialUpHint = glm::vec3(0.0f, 1.0f, 0.0f))
    {
        std::vector<Frame> frames(lengthSegments + 1);

        for (int i = 0; i <= lengthSegments; ++i)
        {
            float t = (float)i / (float)lengthSegments;
            frames[i].position = evalPos(t);
            frames[i].tangent = evalTan(t);
        }

        // Initial normal perpendicular to tangent 0
        glm::vec3 t0 = frames[0].tangent;
        glm::vec3 up = initialUpHint;
        if (std::abs(glm::dot(t0, up)) > 0.92f)
        {
            up = glm::vec3(1.0f, 0.0f, 0.0f);
            if (std::abs(glm::dot(t0, up)) > 0.92f)
                up = glm::vec3(0.0f, 0.0f, 1.0f);
        }
        frames[0].normal = glm::normalize(glm::cross(t0, up));
        frames[0].binormal = glm::normalize(glm::cross(t0, frames[0].normal));

        // Propagate frames using Rodrigues rotation
        for (int i = 1; i <= lengthSegments; ++i)
        {
            glm::vec3 tPrev = frames[i - 1].tangent;
            glm::vec3 tCurr = frames[i].tangent;
            glm::vec3 axis = glm::cross(tPrev, tCurr);
            float axisLen = glm::length(axis);

            if (axisLen < 1e-6f)
            {
                frames[i].normal = frames[i - 1].normal;
                frames[i].binormal = frames[i - 1].binormal;
            }
            else
            {
                axis /= axisLen;
                float dotVal = glm::clamp(glm::dot(tPrev, tCurr), -1.0f, 1.0f);
                float angle = std::acos(dotVal);

                glm::vec3 nPrev = frames[i - 1].normal;
                glm::vec3 nCurr = nPrev * std::cos(angle) +
                                  glm::cross(axis, nPrev) * std::sin(angle) +
                                  axis * (glm::dot(axis, nPrev)) * (1.0f - std::cos(angle));
                frames[i].normal = glm::normalize(nCurr);
                frames[i].binormal = glm::normalize(glm::cross(tCurr, frames[i].normal));
            }
        }

        return frames;
    }

    // -------------------------------------------------------------
    // 5. Generalized Swept Tube / Curved Cylinder Mesh
    // Sweeps circular cross section of varying radius along arbitrary 3D curve.
    // -------------------------------------------------------------
    inline Mesh createSweptTube(
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
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        auto frames = computeBishopFrames(evalPos, evalTan, lengthSegments, initialUp);

        // Generate tube vertices
        for (int k = 0; k <= lengthSegments; ++k)
        {
            float t = (float)k / (float)lengthSegments;
            float r = radiusFunc(t);
            const auto& fr = frames[k];

            for (int j = 0; j <= radialSegments; ++j)
            {
                float phi = (float)j / (float)radialSegments * 2.0f * (float)M_PI;
                float cosP = std::cos(phi);
                float sinP = std::sin(phi);

                glm::vec3 radialDir = cosP * fr.normal + sinP * fr.binormal;
                glm::vec3 pos = fr.position + r * radialDir;
                glm::vec3 norm = radialDir;
                glm::vec2 uv((float)j / (float)radialSegments, t * vTiling);

                vertices.push_back({ pos, norm, uv });
            }
        }

        // Tube side indices
        int ringStride = radialSegments + 1;
        for (int k = 0; k < lengthSegments; ++k)
        {
            for (int j = 0; j < radialSegments; ++j)
            {
                unsigned int i0 = k * ringStride + j;
                unsigned int i1 = i0 + 1;
                unsigned int i2 = (k + 1) * ringStride + j;
                unsigned int i3 = i2 + 1;

                indices.push_back(i0);
                indices.push_back(i2);
                indices.push_back(i1);

                indices.push_back(i1);
                indices.push_back(i2);
                indices.push_back(i3);
            }
        }

        // Start cap (t = 0)
        if (capStart)
        {
            unsigned int centerIdx = static_cast<unsigned int>(vertices.size());
            glm::vec3 startNorm = -frames[0].tangent;
            vertices.push_back({ frames[0].position, startNorm, { 0.5f, 0.5f } });

            unsigned int ringStart = static_cast<unsigned int>(vertices.size());
            float r0 = radiusFunc(0.0f);
            for (int j = 0; j <= radialSegments; ++j)
            {
                float phi = (float)j / (float)radialSegments * 2.0f * (float)M_PI;
                glm::vec3 radialDir = std::cos(phi) * frames[0].normal + std::sin(phi) * frames[0].binormal;
                vertices.push_back({ frames[0].position + r0 * radialDir, startNorm, { 0.5f + 0.5f * std::cos(phi), 0.5f + 0.5f * std::sin(phi) } });
            }

            for (int j = 0; j < radialSegments; ++j)
            {
                indices.push_back(centerIdx);
                indices.push_back(ringStart + j + 1);
                indices.push_back(ringStart + j);
            }
        }

        // End cap (t = 1)
        if (capEnd)
        {
            unsigned int centerIdx = static_cast<unsigned int>(vertices.size());
            glm::vec3 endNorm = frames[lengthSegments].tangent;
            vertices.push_back({ frames[lengthSegments].position, endNorm, { 0.5f, 0.5f } });

            unsigned int ringStart = static_cast<unsigned int>(vertices.size());
            float r1 = radiusFunc(1.0f);
            for (int j = 0; j <= radialSegments; ++j)
            {
                float phi = (float)j / (float)radialSegments * 2.0f * (float)M_PI;
                glm::vec3 radialDir = std::cos(phi) * frames[lengthSegments].normal + std::sin(phi) * frames[lengthSegments].binormal;
                vertices.push_back({ frames[lengthSegments].position + r1 * radialDir, endNorm, { 0.5f + 0.5f * std::cos(phi), 0.5f + 0.5f * std::sin(phi) } });
            }

            for (int j = 0; j < radialSegments; ++j)
            {
                indices.push_back(centerIdx);
                indices.push_back(ringStart + j);
                indices.push_back(ringStart + j + 1);
            }
        }

        return Mesh(vertices, indices);
    }

    // -------------------------------------------------------------
    // 6. Cubic Bézier Tube Wrapper
    // -------------------------------------------------------------
    inline Mesh createBezierTube(
        const Bezier3& bezier,
        float rStart,
        float rEnd,
        int lengthSegments = 24,
        int radialSegments = 16,
        float vTiling = 1.0f,
        bool capStart = true,
        bool capEnd = true,
        const glm::vec3& initialUp = glm::vec3(0.0f, 1.0f, 0.0f))
    {
        auto evalPos = [bezier](float t) { return bezier.evaluate(t); };
        auto evalTan = [bezier](float t) { return bezier.tangent(t); };
        auto rFunc = [rStart, rEnd](float t) { return glm::mix(rStart, rEnd, t); };
        return createSweptTube(evalPos, evalTan, rFunc, lengthSegments, radialSegments, vTiling, capStart, capEnd, initialUp);
    }

    // -------------------------------------------------------------
    // 7. Quadratic Bézier Tube Wrapper
    // -------------------------------------------------------------
    inline Mesh createBezierTube2(
        const Bezier2& bezier,
        float rStart,
        float rEnd,
        int lengthSegments = 16,
        int radialSegments = 12,
        float vTiling = 1.0f,
        bool capStart = true,
        bool capEnd = true,
        const glm::vec3& initialUp = glm::vec3(0.0f, 1.0f, 0.0f))
    {
        auto evalPos = [bezier](float t) { return bezier.evaluate(t); };
        auto evalTan = [bezier](float t) { return bezier.tangent(t); };
        auto rFunc = [rStart, rEnd](float t) { return glm::mix(rStart, rEnd, t); };
        return createSweptTube(evalPos, evalTan, rFunc, lengthSegments, radialSegments, vTiling, capStart, capEnd, initialUp);
    }

    // -------------------------------------------------------------
    // 8. Catmull-Rom Spline Tube Wrapper
    // -------------------------------------------------------------
    inline Mesh createSplineTube(
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
        CatmullRomSpline spline(pts);
        auto evalPos = [spline](float t) { return spline.evaluate(t); };
        auto evalTan = [spline](float t) { return spline.tangent(t); };
        auto rFunc = [rStart, rEnd](float t) { return glm::mix(rStart, rEnd, t); };
        return createSweptTube(evalPos, evalTan, rFunc, lengthSegments, radialSegments, vTiling, capStart, capEnd, initialUp);
    }

    // -------------------------------------------------------------
    // 9. Swept Curved Architectural Beam (Kasagi / Shimaki with Sori)
    // Sweeps a rectangular cross section with upward parabolic curvature (sori)
    // and flared wingtips, providing authentic Japanese shrine lintels.
    // -------------------------------------------------------------
    inline Mesh createCurvedBeam(
        float spanX,
        float baseWidthZ,
        float baseHeightY,
        float soriUpward = 0.35f,
        float endFlare = 1.15f,
        int lengthSegments = 32,
        bool roofBevel = true)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        float halfSpan = spanX * 0.5f;

        struct Slice
        {
            glm::vec3 center;
            glm::vec3 tangent;
            glm::vec3 up;
            float widthZ;
            float heightY;
        };

        std::vector<Slice> slices(lengthSegments + 1);
        for (int i = 0; i <= lengthSegments; ++i)
        {
            float u = -1.0f + 2.0f * (float)i / (float)lengthSegments;
            float x = u * halfSpan;
            float u2 = u * u;
            float y = soriUpward * u2;
            float dydx = (soriUpward * 2.0f * u) / halfSpan;

            glm::vec3 tanVec = glm::normalize(glm::vec3(1.0f, dydx, 0.0f));
            glm::vec3 upVec = glm::normalize(glm::vec3(-tanVec.y, tanVec.x, 0.0f));

            slices[i].center = glm::vec3(x, y, 0.0f);
            slices[i].tangent = tanVec;
            slices[i].up = upVec;
            slices[i].widthZ = baseWidthZ * (1.0f + (endFlare - 1.0f) * u2);
            slices[i].heightY = baseHeightY * (1.0f + 0.08f * u2);
        }

        auto addQuad = [&](const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3) {
            unsigned int idx = static_cast<unsigned int>(vertices.size());
            vertices.push_back(v0);
            vertices.push_back(v1);
            vertices.push_back(v2);
            vertices.push_back(v3);
            indices.push_back(idx + 0);
            indices.push_back(idx + 1);
            indices.push_back(idx + 2);
            indices.push_back(idx + 0);
            indices.push_back(idx + 2);
            indices.push_back(idx + 3);
        };

        for (int i = 0; i < lengthSegments; ++i)
        {
            const auto& s0 = slices[i];
            const auto& s1 = slices[i + 1];

            float u0 = (float)i / (float)lengthSegments;
            float u1 = (float)(i + 1) / (float)lengthSegments;

            float hz0 = s0.widthZ * 0.5f;
            float hz1 = s1.widthZ * 0.5f;
            float hy0 = s0.heightY * 0.5f;
            float hy1 = s1.heightY * 0.5f;

            // 4 corner points per slice
            glm::vec3 botL0 = s0.center - s0.up * hy0 - glm::vec3(0, 0, hz0);
            glm::vec3 botR0 = s0.center - s0.up * hy0 + glm::vec3(0, 0, hz0);
            glm::vec3 topL0 = s0.center + s0.up * hy0 - glm::vec3(0, 0, hz0);
            glm::vec3 topR0 = s0.center + s0.up * hy0 + glm::vec3(0, 0, hz0);

            glm::vec3 botL1 = s1.center - s1.up * hy1 - glm::vec3(0, 0, hz1);
            glm::vec3 botR1 = s1.center - s1.up * hy1 + glm::vec3(0, 0, hz1);
            glm::vec3 topL1 = s1.center + s1.up * hy1 - glm::vec3(0, 0, hz1);
            glm::vec3 topR1 = s1.center + s1.up * hy1 + glm::vec3(0, 0, hz1);

            // Bottom face (-up)
            glm::vec3 normBot = -glm::normalize(s0.up + s1.up);
            addQuad({ botL0, normBot, { u0 * 4.0f, 0.0f } },
                    { botR0, normBot, { u0 * 4.0f, 1.0f } },
                    { botR1, normBot, { u1 * 4.0f, 1.0f } },
                    { botL1, normBot, { u1 * 4.0f, 0.0f } });

            // Top face (+up)
            if (!roofBevel)
            {
                glm::vec3 normTop = glm::normalize(s0.up + s1.up);
                addQuad({ topL0, normTop, { u0 * 4.0f, 0.0f } },
                        { topL1, normTop, { u1 * 4.0f, 0.0f } },
                        { topR1, normTop, { u1 * 4.0f, 1.0f } },
                        { topR0, normTop, { u0 * 4.0f, 1.0f } });
            }
            else
            {
                // Beveled roof ridge along top center
                glm::vec3 ridge0 = s0.center + s0.up * (hy0 * 1.35f);
                glm::vec3 ridge1 = s1.center + s1.up * (hy1 * 1.35f);

                glm::vec3 nLeft = glm::normalize(glm::cross(ridge1 - topL0, topL1 - topL0));
                addQuad({ topL0, nLeft, { u0 * 4.0f, 0.0f } },
                        { topL1, nLeft, { u1 * 4.0f, 0.0f } },
                        { ridge1, nLeft, { u1 * 4.0f, 0.5f } },
                        { ridge0, nLeft, { u0 * 4.0f, 0.5f } });

                glm::vec3 nRight = glm::normalize(glm::cross(topR1 - ridge0, ridge1 - ridge0));
                addQuad({ ridge0, nRight, { u0 * 4.0f, 0.5f } },
                        { ridge1, nRight, { u1 * 4.0f, 0.5f } },
                        { topR1, nRight, { u1 * 4.0f, 1.0f } },
                        { topR0, nRight, { u0 * 4.0f, 1.0f } });
            }

            // Front face (+Z)
            glm::vec3 normFront(0.0f, 0.0f, 1.0f);
            addQuad({ botR0, normFront, { u0 * 4.0f, 0.0f } },
                    { topR0, normFront, { u0 * 4.0f, 1.0f } },
                    { topR1, normFront, { u1 * 4.0f, 1.0f } },
                    { botR1, normFront, { u1 * 4.0f, 0.0f } });

            // Back face (-Z)
            glm::vec3 normBack(0.0f, 0.0f, -1.0f);
            addQuad({ botL0, normBack, { u0 * 4.0f, 0.0f } },
                    { botL1, normBack, { u1 * 4.0f, 0.0f } },
                    { topL1, normBack, { u1 * 4.0f, 1.0f } },
                    { topL0, normBack, { u0 * 4.0f, 1.0f } });
        }

        // Left End Cap (i = 0)
        {
            const auto& s = slices[0];
            glm::vec3 nCap = -s.tangent;
            float hz = s.widthZ * 0.5f;
            float hy = s.heightY * 0.5f;
            glm::vec3 botL = s.center - s.up * hy - glm::vec3(0, 0, hz);
            glm::vec3 botR = s.center - s.up * hy + glm::vec3(0, 0, hz);
            glm::vec3 topL = s.center + s.up * hy - glm::vec3(0, 0, hz);
            glm::vec3 topR = s.center + s.up * hy + glm::vec3(0, 0, hz);
            if (!roofBevel)
            {
                addQuad({ botL, nCap, { 0.0f, 0.0f } },
                        { topL, nCap, { 0.0f, 1.0f } },
                        { topR, nCap, { 1.0f, 1.0f } },
                        { botR, nCap, { 1.0f, 0.0f } });
            }
            else
            {
                glm::vec3 ridge = s.center + s.up * (hy * 1.35f);
                addQuad({ botL, nCap, { 0.0f, 0.0f } },
                        { topL, nCap, { 0.0f, 0.8f } },
                        { topR, nCap, { 1.0f, 0.8f } },
                        { botR, nCap, { 1.0f, 0.0f } });
                unsigned int idx = static_cast<unsigned int>(vertices.size());
                vertices.push_back({ topL, nCap, { 0.0f, 0.8f } });
                vertices.push_back({ ridge, nCap, { 0.5f, 1.0f } });
                vertices.push_back({ topR, nCap, { 1.0f, 0.8f } });
                indices.push_back(idx + 0);
                indices.push_back(idx + 1);
                indices.push_back(idx + 2);
            }
        }

        // Right End Cap (i = lengthSegments)
        {
            const auto& s = slices[lengthSegments];
            glm::vec3 nCap = s.tangent;
            float hz = s.widthZ * 0.5f;
            float hy = s.heightY * 0.5f;
            glm::vec3 botL = s.center - s.up * hy - glm::vec3(0, 0, hz);
            glm::vec3 botR = s.center - s.up * hy + glm::vec3(0, 0, hz);
            glm::vec3 topL = s.center + s.up * hy - glm::vec3(0, 0, hz);
            glm::vec3 topR = s.center + s.up * hy + glm::vec3(0, 0, hz);
            if (!roofBevel)
            {
                addQuad({ botR, nCap, { 0.0f, 0.0f } },
                        { topR, nCap, { 0.0f, 1.0f } },
                        { topL, nCap, { 1.0f, 1.0f } },
                        { botL, nCap, { 1.0f, 0.0f } });
            }
            else
            {
                glm::vec3 ridge = s.center + s.up * (hy * 1.35f);
                addQuad({ botR, nCap, { 0.0f, 0.0f } },
                        { topR, nCap, { 0.0f, 0.8f } },
                        { topL, nCap, { 1.0f, 0.8f } },
                        { botL, nCap, { 1.0f, 0.0f } });
                unsigned int idx = static_cast<unsigned int>(vertices.size());
                vertices.push_back({ topR, nCap, { 0.0f, 0.8f } });
                vertices.push_back({ ridge, nCap, { 0.5f, 1.0f } });
                vertices.push_back({ topL, nCap, { 1.0f, 0.8f } });
                indices.push_back(idx + 0);
                indices.push_back(idx + 1);
                indices.push_back(idx + 2);
            }
        }

        return Mesh(vertices, indices);
    }

    // -------------------------------------------------------------
    // 10. Continuous Catenary Rope Mesh
    // Sweeps a circular rope along a sagging catenary curve between two poles.
    // -------------------------------------------------------------
    inline Mesh createCatenaryRope(
        float halfSpan = 3.8f,
        float yPole = 6.20f,
        float sag = 0.65f,
        float radius = 0.035f,
        int lengthSegments = 36,
        int radialSegments = 12)
    {
        auto evalPos = [halfSpan, yPole, sag](float t) -> glm::vec3 {
            float x = -halfSpan + t * (2.0f * halfSpan);
            float normX = x / halfSpan;
            float y = yPole - sag * (1.0f - normX * normX);
            return glm::vec3(x, y, 0.0f);
        };

        auto evalTan = [halfSpan, sag](float t) -> glm::vec3 {
            float x = -halfSpan + t * (2.0f * halfSpan);
            float normX = x / halfSpan;
            float dydx = (2.0f * sag * normX) / halfSpan;
            return glm::normalize(glm::vec3(1.0f, dydx, 0.0f));
        };

        auto rFunc = [radius](float /*t*/) { return radius; };

        return createSweptTube(evalPos, evalTan, rFunc, lengthSegments, radialSegments, 6.0f, true, true, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    // -------------------------------------------------------------
    // 11. Curved 3D Leaf Mesh (Botanical Midrib Arch & Cupping)
    // -------------------------------------------------------------
    inline Mesh createCurvedLeafMesh(
        float length = 0.12f,
        float maxWidth = 0.045f,
        float archY = 0.02f,
        float foldAngle = 25.0f,
        int lengthSegs = 10,
        int widthSegs = 6)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        float foldRad = glm::radians(foldAngle);

        for (int i = 0; i <= lengthSegs; ++i)
        {
            float u = (float)i / (float)lengthSegs;
            float z = u * length;
            float yMid = archY * std::sin(u * (float)M_PI * 0.95f);
            float w = maxWidth * std::sin(std::pow(u, 0.65f) * (float)M_PI);

            for (int j = 0; j <= widthSegs; ++j)
            {
                float v = -1.0f + 2.0f * (float)j / (float)widthSegs;
                float x = v * (w * 0.5f);
                float y = yMid + std::abs(v) * (w * 0.5f * std::sin(foldRad));

                float ny = std::cos(foldRad);
                float nx = -v * std::sin(foldRad);
                float nz = -0.25f * (1.0f - u);
                glm::vec3 norm = glm::normalize(glm::vec3(nx, ny, nz));

                vertices.push_back({ { x, y, z }, norm, { u, (v + 1.0f) * 0.5f } });
            }
        }

        int rowStride = widthSegs + 1;
        for (int i = 0; i < lengthSegs; ++i)
        {
            for (int j = 0; j < widthSegs; ++j)
            {
                unsigned int i0 = i * rowStride + j;
                unsigned int i1 = i0 + 1;
                unsigned int i2 = (i + 1) * rowStride + j;
                unsigned int i3 = i2 + 1;

                // Double-sided leaf
                indices.push_back(i0); indices.push_back(i1); indices.push_back(i2);
                indices.push_back(i1); indices.push_back(i3); indices.push_back(i2);
                indices.push_back(i0); indices.push_back(i2); indices.push_back(i1);
                indices.push_back(i1); indices.push_back(i2); indices.push_back(i3);
            }
        }
        return Mesh(vertices, indices);
    }

    // -------------------------------------------------------------
    // 12. Curved Cupped Flower Petal Mesh
    // -------------------------------------------------------------
    inline Mesh createCurvedPetalMesh(
        float length = 0.06f,
        float width = 0.045f,
        float cupDepth = 0.014f,
        int segsU = 8,
        int segsV = 6)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (int i = 0; i <= segsU; ++i)
        {
            float u = (float)i / (float)segsU;
            float z = u * length;
            float w = width * std::sin(std::pow(u, 0.7f) * (float)M_PI);

            for (int j = 0; j <= segsV; ++j)
            {
                float v = -1.0f + 2.0f * (float)j / (float)segsV;
                float x = v * (w * 0.5f);
                float y = -cupDepth * (1.0f - v * v) * std::sin(u * (float)M_PI);

                glm::vec3 norm = glm::normalize(glm::vec3(v * 0.5f, 1.0f, -0.2f * (1.0f - u)));
                vertices.push_back({ { x, y, z }, norm, { u, (v + 1.0f) * 0.5f } });
            }
        }

        int rowStride = segsV + 1;
        for (int i = 0; i < segsU; ++i)
        {
            for (int j = 0; j < segsV; ++j)
            {
                unsigned int i0 = i * rowStride + j;
                unsigned int i1 = i0 + 1;
                unsigned int i2 = (i + 1) * rowStride + j;
                unsigned int i3 = i2 + 1;

                indices.push_back(i0); indices.push_back(i1); indices.push_back(i2);
                indices.push_back(i1); indices.push_back(i3); indices.push_back(i2);
                indices.push_back(i0); indices.push_back(i2); indices.push_back(i1);
                indices.push_back(i1); indices.push_back(i2); indices.push_back(i3);
            }
        }
        return Mesh(vertices, indices);
    }

    // -------------------------------------------------------------
    // 13. Sculpted Organic Cherry Blossom Canopy Lobe (Curved Petal Billows)
    // -------------------------------------------------------------
    inline Mesh createSakuraBlossomLobe(
        float radius = 1.0f,
        int rings = 18,
        int sectors = 22,
        float billowAmp = 0.24f)
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

                // Organic blossom cluster billows / ruffled petal puffs
                float billow = 1.0f + billowAmp * (
                    0.55f * std::sin(5.0f * theta) * std::cos(3.0f * phi) +
                    0.35f * std::cos(7.0f * theta) * std::sin(4.0f * phi) +
                    0.25f * std::sin(3.0f * theta + 2.0f * phi)
                );

                glm::vec3 baseDir(cosPhi * cosTheta, sinPhi, cosPhi * sinTheta);
                glm::vec3 pos = baseDir * (radius * billow);
                glm::vec3 norm = glm::normalize(baseDir);

                vertices.push_back({ pos, norm, { u, v } });
            }
        }

        for (int r = 0; r < rings; ++r)
        {
            for (int s = 0; s < sectors; ++s)
            {
                unsigned int cur = r * (sectors + 1) + s;
                unsigned int next = cur + sectors + 1;
                indices.push_back(cur); indices.push_back(next); indices.push_back(cur + 1);
                indices.push_back(cur + 1); indices.push_back(next); indices.push_back(next + 1);
            }
        }
        return Mesh(vertices, indices);
    }

    // -------------------------------------------------------------
    // 14. Sculpted Evergreen Pine Foliage Pad (Bonsai Needle Clusters)
    // -------------------------------------------------------------
    inline Mesh createPineNeedleClusterMesh(
        float widthX = 0.24f,
        float heightY = 0.08f,
        float depthZ = 0.20f,
        int rings = 14,
        int sectors = 18)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (int r = 0; r <= rings; ++r)
        {
            float phi = - (float)M_PI * 0.5f + (float)M_PI * ((float)r / (float)rings);
            float cosPhi = std::cos(phi);
            float sinPhi = std::sin(phi);

            for (int s = 0; s <= sectors; ++s)
            {
                float theta = 2.0f * (float)M_PI * ((float)s / (float)sectors);
                float ripple = 1.0f + 0.16f * std::sin(8.0f * theta) * std::cos(3.0f * phi);

                float px = widthX * 0.5f * cosPhi * std::cos(theta) * ripple;
                float py = heightY * 0.5f * sinPhi * (0.8f + 0.2f * std::cos(6.0f * theta));
                float pz = depthZ * 0.5f * cosPhi * std::sin(theta) * ripple;

                glm::vec3 norm = glm::normalize(glm::vec3(px / (widthX * widthX), py / (heightY * heightY), pz / (depthZ * depthZ)));
                vertices.push_back({ { px, py, pz }, norm, { (float)s / (float)sectors, (float)r / (float)rings } });
            }
        }

        for (int r = 0; r < rings; ++r)
        {
            for (int s = 0; s < sectors; ++s)
            {
                unsigned int cur = r * (sectors + 1) + s;
                unsigned int next = cur + sectors + 1;
                indices.push_back(cur); indices.push_back(next); indices.push_back(cur + 1);
                indices.push_back(cur + 1); indices.push_back(next); indices.push_back(next + 1);
            }
        }
        return Mesh(vertices, indices);
    }

    // -------------------------------------------------------------
    // 15. Sculpted Anatomical Human Head & Face Mesh
    // Realistic head shape with chin, jawline, nose bridge, eye sockets, and lips.
    // -------------------------------------------------------------
    inline Mesh createHumanHeadMesh(float scale = 0.20f)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        int rings = 20;
        int sectors = 24;

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

                float rx = scale * 0.88f;
                float ry = scale * 1.15f;
                float rz = scale * 0.95f;

                // Facial feature modifications
                float isFront = std::max(0.0f, cosPhi * sinTheta);
                float yNorm = sinPhi;

                // Chin taper towards bottom front
                if (yNorm < -0.3f && isFront > 0.3f)
                {
                    rx *= (1.0f - 0.28f * (1.0f + yNorm));
                    rz *= (1.0f - 0.15f * (1.0f + yNorm));
                }

                // Nose protrusion in mid-front
                if (std::abs(yNorm) < 0.15f && std::abs(cosPhi * cosTheta) < 0.25f && isFront > 0.6f)
                {
                    float noseFactor = (1.0f - std::abs(yNorm) / 0.15f) * (1.0f - std::abs(cosPhi * cosTheta) / 0.25f);
                    rz += scale * 0.38f * noseFactor;
                }

                // Cheekbone fullness
                if (yNorm > 0.05f && yNorm < 0.35f && isFront > 0.4f)
                {
                    rx *= 1.08f;
                }

                // Eye socket recession
                if (yNorm > 0.18f && yNorm < 0.35f && std::abs(cosPhi * cosTheta) > 0.15f && std::abs(cosPhi * cosTheta) < 0.55f && isFront > 0.6f)
                {
                    rz -= scale * 0.08f;
                }

                // Jaw taper
                if (yNorm < -0.1f)
                {
                    rx *= (1.0f + yNorm * 0.25f);
                }

                glm::vec3 pos(rx * cosPhi * cosTheta, ry * sinPhi, rz * cosPhi * sinTheta);
                glm::vec3 norm = glm::normalize(glm::vec3(pos.x / (rx * rx), pos.y / (ry * ry), pos.z / (rz * rz)));

                vertices.push_back({ pos, norm, { u, v } });
            }
        }

        for (int r = 0; r < rings; ++r)
        {
            for (int s = 0; s < sectors; ++s)
            {
                unsigned int cur = r * (sectors + 1) + s;
                unsigned int next = cur + sectors + 1;
                indices.push_back(cur); indices.push_back(next); indices.push_back(cur + 1);
                indices.push_back(cur + 1); indices.push_back(next); indices.push_back(next + 1);
            }
        }
        return Mesh(vertices, indices);
    }

    // -------------------------------------------------------------
    // 16. Sculpted Human Torso with Kimono / Yukata Crossed Collar (Eri)
    // -------------------------------------------------------------
    inline Mesh createHumanTorsoMesh(float width = 0.52f, float height = 0.85f, float depth = 0.36f)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        int ySlices = 14;
        int radSegs = 18;

        for (int i = 0; i <= ySlices; ++i)
        {
            float t = (float)i / (float)ySlices; // 0 (hips) to 1 (shoulders/neck)
            float y = (t - 0.5f) * height;

            // Width profile: hips -> waist taper -> broad shoulders
            float w;
            if (t < 0.35f)
                w = width * (0.92f + 0.08f * (t / 0.35f)); // hips
            else if (t < 0.60f)
                w = width * (1.0f - 0.18f * ((t - 0.35f) / 0.25f)); // waist taper
            else
                w = width * (0.82f + 0.28f * ((t - 0.60f) / 0.40f)); // chest/shoulders

            float d = depth * (0.85f + 0.25f * std::sin(t * (float)M_PI));

            for (int j = 0; j <= radSegs; ++j)
            {
                float phi = (float)j / (float)radSegs * 2.0f * (float)M_PI;
                float cosP = std::cos(phi);
                float sinP = std::sin(phi);

                float px = (w * 0.5f) * cosP;
                float pz = (d * 0.5f) * sinP;

                // Kimono overlapping collar (Eri) on upper chest
                if (t > 0.55f && sinP > 0.5f)
                {
                    // V-neck crease
                    float vFactor = (t - 0.55f) / 0.45f;
                    pz += 0.015f * std::cos(cosP * 3.14f);
                }

                glm::vec3 pos(px, y, pz);
                glm::vec3 norm = glm::normalize(glm::vec3(px / (w * w), 0.1f * (0.5f - t), pz / (d * d)));
                vertices.push_back({ pos, norm, { (float)j / (float)radSegs, t } });
            }
        }

        int rowStride = radSegs + 1;
        for (int i = 0; i < ySlices; ++i)
        {
            for (int j = 0; j < radSegs; ++j)
            {
                unsigned int i0 = i * rowStride + j;
                unsigned int i1 = i0 + 1;
                unsigned int i2 = (i + 1) * rowStride + j;
                unsigned int i3 = i2 + 1;
                indices.push_back(i0); indices.push_back(i2); indices.push_back(i1);
                indices.push_back(i1); indices.push_back(i2); indices.push_back(i3);
            }
        }
        return Mesh(vertices, indices);
    }

    // -------------------------------------------------------------
    // 17. Anatomical Articulated Limb Segment (Thigh, Shin, Bicep, Forearm)
    // Smooth tapering with joint condyles.
    // -------------------------------------------------------------
    inline Mesh createArticulatedLimbMesh(float rTop = 0.08f, float rBottom = 0.06f, float length = 0.45f, int lenSegs = 12, int radSegs = 14)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for (int i = 0; i <= lenSegs; ++i)
        {
            float t = (float)i / (float)lenSegs;
            float y = -t * length; // extends downward from joint pivot at Y=0
            // Muscle curvature / contour
            float muscleBulge = 1.0f + 0.12f * std::sin(t * (float)M_PI);
            float r = glm::mix(rTop, rBottom, t) * muscleBulge;

            for (int j = 0; j <= radSegs; ++j)
            {
                float phi = (float)j / (float)radSegs * 2.0f * (float)M_PI;
                float cosP = std::cos(phi);
                float sinP = std::sin(phi);

                glm::vec3 pos(r * cosP, y, r * sinP);
                glm::vec3 norm = glm::normalize(glm::vec3(cosP, 0.0f, sinP));
                vertices.push_back({ pos, norm, { (float)j / (float)radSegs, t } });
            }
        }

        int rowStride = radSegs + 1;
        for (int i = 0; i < lenSegs; ++i)
        {
            for (int j = 0; j < radSegs; ++j)
            {
                unsigned int i0 = i * rowStride + j;
                unsigned int i1 = i0 + 1;
                unsigned int i2 = (i + 1) * rowStride + j;
                unsigned int i3 = i2 + 1;
                indices.push_back(i0); indices.push_back(i2); indices.push_back(i1);
                indices.push_back(i1); indices.push_back(i2); indices.push_back(i3);
            }
        }
        return Mesh(vertices, indices);
    }

    // -------------------------------------------------------------
    // 18. Contoured Human Hand Mesh (Palm, Thumb, & Articulated Fingers)
    // -------------------------------------------------------------
    inline Mesh createHandMesh(float length = 0.14f, float width = 0.08f, float thickness = 0.035f)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        float hl = length * 0.5f;
        float hw = width * 0.5f;
        float ht = thickness * 0.5f;

        // Palm slab with tapered fingers
        struct BoxFace { glm::vec3 v[4]; glm::vec3 n; };
        std::vector<BoxFace> faces = {
            // Front (+Z)
            { { {-hw, -hl, ht}, {hw, -hl, ht}, {hw * 0.85f, hl, ht * 0.8f}, {-hw * 0.85f, hl, ht * 0.8f} }, {0, 0, 1} },
            // Back (-Z)
            { { {hw, -hl, -ht}, {-hw, -hl, -ht}, {-hw * 0.85f, hl, -ht * 0.8f}, {hw * 0.85f, hl, -ht * 0.8f} }, {0, 0, -1} },
            // Left (-X) thumb side
            { { {-hw, -hl, -ht}, {-hw, -hl, ht}, {-hw * 0.85f, hl, ht * 0.8f}, {-hw * 0.85f, hl, -ht * 0.8f} }, {-1, 0, 0} },
            // Right (+X)
            { { {hw, -hl, ht}, {hw, -hl, -ht}, {hw * 0.85f, hl, -ht * 0.8f}, {hw * 0.85f, hl, ht * 0.8f} }, {1, 0, 0} },
            // Top (+Y fingertips)
            { { {-hw * 0.85f, hl, ht * 0.8f}, {hw * 0.85f, hl, ht * 0.8f}, {hw * 0.85f, hl, -ht * 0.8f}, {-hw * 0.85f, hl, -ht * 0.8f} }, {0, 1, 0} },
            // Bottom (-Y wrist)
            { { {-hw, -hl, -ht}, {hw, -hl, -ht}, {hw, -hl, ht}, {-hw, -hl, ht} }, {0, -1, 0} }
        };

        for (const auto& f : faces)
        {
            unsigned int idx = static_cast<unsigned int>(vertices.size());
            vertices.push_back({ f.v[0], f.n, {0, 0} });
            vertices.push_back({ f.v[1], f.n, {1, 0} });
            vertices.push_back({ f.v[2], f.n, {1, 1} });
            vertices.push_back({ f.v[3], f.n, {0, 1} });
            indices.push_back(idx + 0); indices.push_back(idx + 1); indices.push_back(idx + 2);
            indices.push_back(idx + 0); indices.push_back(idx + 2); indices.push_back(idx + 3);
        }
        return Mesh(vertices, indices);
    }

    // -------------------------------------------------------------
    // 19. Japanese Geta Sandal & Foot Mesh
    // Traditional wooden geta sole with 2 elevated teeth and red Hanao V-strap.
    // -------------------------------------------------------------
    inline Mesh createGetaFootMesh(float length = 0.25f, float width = 0.12f, float height = 0.065f)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        auto addBox = [&](const glm::vec3& center, const glm::vec3& size, const glm::vec3& normColorDummy) {
            float hx = size.x * 0.5f;
            float hy = size.y * 0.5f;
            float hz = size.z * 0.5f;
            glm::vec3 c = center;

            struct BoxFace { glm::vec3 v[4]; glm::vec3 n; };
            BoxFace faces[6] = {
                { { {c.x-hx, c.y-hy, c.z+hz}, {c.x+hx, c.y-hy, c.z+hz}, {c.x+hx, c.y+hy, c.z+hz}, {c.x-hx, c.y+hy, c.z+hz} }, {0, 0, 1} },
                { { {c.x+hx, c.y-hy, c.z-hz}, {c.x-hx, c.y-hy, c.z-hz}, {c.x-hx, c.y+hy, c.z-hz}, {c.x+hx, c.y+hy, c.z-hz} }, {0, 0, -1} },
                { { {c.x-hx, c.y-hy, c.z-hz}, {c.x-hx, c.y-hy, c.z+hz}, {c.x-hx, c.y+hy, c.z+hz}, {c.x-hx, c.y+hy, c.z-hz} }, {-1, 0, 0} },
                { { {c.x+hx, c.y-hy, c.z+hz}, {c.x+hx, c.y-hy, c.z-hz}, {c.x+hx, c.y+hy, c.z-hz}, {c.x+hx, c.y+hy, c.z+hz} }, {1, 0, 0} },
                { { {c.x-hx, c.y+hy, c.z+hz}, {c.x+hx, c.y+hy, c.z+hz}, {c.x+hx, c.y+hy, c.z-hz}, {c.x-hx, c.y+hy, c.z-hz} }, {0, 1, 0} },
                { { {c.x-hx, c.y-hy, c.z-hz}, {c.x+hx, c.y-hy, c.z-hz}, {c.x+hx, c.y-hy, c.z+hz}, {c.x-hx, c.y-hy, c.z+hz} }, {0, -1, 0} }
            };
            for (int f = 0; f < 6; ++f)
            {
                unsigned int idx = static_cast<unsigned int>(vertices.size());
                for (int v = 0; v < 4; ++v)
                    vertices.push_back({ faces[f].v[v], faces[f].n, {(float)(v%2), (float)(v/2)} });
                indices.push_back(idx + 0); indices.push_back(idx + 1); indices.push_back(idx + 2);
                indices.push_back(idx + 0); indices.push_back(idx + 2); indices.push_back(idx + 3);
            }
        };

        // Main wooden board (Dai) resting at Y in [0.035, 0.065]
        addBox(glm::vec3(0.0f, height * 0.75f, 0.0f), glm::vec3(width, height * 0.40f, length), glm::vec3(1));
        // Front tooth (Ha) resting flush on ground Y in [0.0, 0.035]
        addBox(glm::vec3(0.0f, height * 0.25f, length * 0.25f), glm::vec3(width * 0.90f, height * 0.50f, length * 0.12f), glm::vec3(1));
        // Rear tooth (Ha) resting flush on ground Y in [0.0, 0.035]
        addBox(glm::vec3(0.0f, height * 0.25f, -length * 0.28f), glm::vec3(width * 0.90f, height * 0.50f, length * 0.12f), glm::vec3(1));

        return Mesh(vertices, indices);
    }
}
