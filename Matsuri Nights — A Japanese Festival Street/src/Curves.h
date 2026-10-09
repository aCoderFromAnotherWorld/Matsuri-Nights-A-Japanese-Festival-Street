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
}
