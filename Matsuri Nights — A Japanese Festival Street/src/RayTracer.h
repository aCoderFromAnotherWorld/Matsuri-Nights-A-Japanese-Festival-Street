#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "Camera.h"
#include "TextureGenerator.h"

namespace CPU_RayTracer
{
    struct Ray
    {
        glm::vec3 origin;
        glm::vec3 direction;

        Ray() : origin(0.0f), direction(0.0f, 0.0f, -1.0f) {}
        Ray(const glm::vec3& o, const glm::vec3& d) : origin(o), direction(glm::normalize(d)) {}
        glm::vec3 at(float t) const { return origin + direction * t; }
    };

    struct HitRecord
    {
        float t = 1e20f;
        glm::vec3 p = glm::vec3(0.0f);
        glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 albedo = glm::vec3(0.8f);
        float specularStrength = 0.3f;
        float shininess = 32.0f;
        float reflectivity = 0.0f;
        bool isEmissive = false;
        glm::vec3 emissiveColor = glm::vec3(0.0f);
    };

    inline bool intersectSphere(const Ray& ray, const glm::vec3& center, float radius, float& tHit, glm::vec3& outNorm)
    {
        glm::vec3 oc = ray.origin - center;
        float b = glm::dot(oc, ray.direction);
        float c = glm::dot(oc, oc) - radius * radius;
        float disc = b * b - c;
        if (disc < 0.0f) return false;
        float sqrtD = std::sqrt(disc);
        float t = -b - sqrtD;
        if (t < 0.002f) t = -b + sqrtD;
        if (t < 0.002f) return false;
        tHit = t;
        outNorm = glm::normalize((ray.origin + ray.direction * t) - center);
        return true;
    }

    inline bool intersectBox(const Ray& ray, const glm::vec3& bMin, const glm::vec3& bMax, float& tHit, glm::vec3& outNorm)
    {
        glm::vec3 invD = 1.0f / ray.direction;
        glm::vec3 t0 = (bMin - ray.origin) * invD;
        glm::vec3 t1 = (bMax - ray.origin) * invD;
        glm::vec3 tmin = glm::min(t0, t1);
        glm::vec3 tmax = glm::max(t0, t1);
        float tNear = std::max(std::max(tmin.x, tmin.y), tmin.z);
        float tFar = std::min(std::min(tmax.x, tmax.y), tmax.z);
        if (tNear > tFar || tFar < 0.002f) return false;
        float t = (tNear > 0.002f) ? tNear : tFar;
        if (t < 0.002f) return false;
        tHit = t;

        glm::vec3 hitPt = ray.origin + ray.direction * t;
        glm::vec3 c = (bMin + bMax) * 0.5f;
        glm::vec3 d = (bMax - bMin) * 0.5f;
        glm::vec3 p = (hitPt - c) / d;
        glm::vec3 absP = glm::abs(p);
        if (absP.x > absP.y && absP.x > absP.z)
            outNorm = glm::vec3((p.x > 0.0f) ? 1.0f : -1.0f, 0.0f, 0.0f);
        else if (absP.y > absP.z)
            outNorm = glm::vec3(0.0f, (p.y > 0.0f) ? 1.0f : -1.0f, 0.0f);
        else
            outNorm = glm::vec3(0.0f, 0.0f, (p.z > 0.0f) ? 1.0f : -1.0f);
        return true;
    }

    inline bool intersectCylinderY(const Ray& ray, const glm::vec3& baseCenter, float radius, float height, float& tHit, glm::vec3& outNorm)
    {
        glm::vec3 oc = ray.origin - baseCenter;
        float a = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z;
        if (a < 1e-6f) return false;
        float b = 2.0f * (oc.x * ray.direction.x + oc.z * ray.direction.z);
        float c = oc.x * oc.x + oc.z * oc.z - radius * radius;
        float disc = b * b - 4.0f * a * c;
        if (disc < 0.0f) return false;
        float sqrtD = std::sqrt(disc);
        float t = (-b - sqrtD) / (2.0f * a);
        if (t < 0.002f) t = (-b + sqrtD) / (2.0f * a);
        if (t < 0.002f) return false;
        float yHit = oc.y + ray.direction.y * t;
        if (yHit >= 0.0f && yHit <= height)
        {
            tHit = t;
            glm::vec3 hitPt = ray.origin + ray.direction * t;
            outNorm = glm::normalize(glm::vec3(hitPt.x - baseCenter.x, 0.0f, hitPt.z - baseCenter.z));
            return true;
        }
        return false;
    }

    struct SceneSnapshotData
    {
        float dayNightFactor = 0.0f;
        glm::vec3 orbPos = glm::vec3(-4.0f, 1.8f, -19.0f);
        glm::vec3 boxPos = glm::vec3(-4.8f, 1.45f, -19.0f);
        glm::vec3 boxScale = glm::vec3(1.0f);
        glm::vec3 spotPos = glm::vec3(-7.0f, 4.5f, -16.0f);
        glm::vec3 spotDir = glm::vec3(0.5f, -0.6f, -0.5f);
        glm::vec3 fireworkPos = glm::vec3(0.0f, 25.0f, -28.0f);
        glm::vec3 fireworkColor = glm::vec3(1.0f, 0.4f, 0.2f);
        float fireworkActive = 0.0f;
    };

    inline bool traceScene(const Ray& ray, const SceneSnapshotData& data, HitRecord& bestHit)
    {
        float minT = 1e20f;
        bool hitSomething = false;
        float t;
        glm::vec3 norm;

        // 1. Ground Plane (Y = 0)
        if (std::abs(ray.direction.y) > 1e-5f)
        {
            t = -ray.origin.y / ray.direction.y;
            if (t > 0.002f && t < minT)
            {
                glm::vec3 p = ray.at(t);
                if (std::abs(p.x) <= 25.0f && std::abs(p.z) <= 35.0f)
                {
                    minT = t;
                    hitSomething = true;
                    bestHit.t = t;
                    bestHit.p = p;
                    bestHit.normal = glm::vec3(0.0f, 1.0f, 0.0f);
                    bestHit.isEmissive = false;

                    if (std::abs(p.x) <= 2.2f)
                    {
                        // Stone pavement with procedural cobblestones
                        float uvX = p.x * 1.8f;
                        float uvZ = p.z * 1.8f;
                        float fx = uvX - std::floor(uvX);
                        float fz = uvZ - std::floor(uvZ);
                        float mortar = (fx > 0.05f && fz > 0.05f) ? 1.0f : 0.0f;
                        bestHit.albedo = glm::mix(glm::vec3(0.18f, 0.18f, 0.20f), glm::vec3(0.36f, 0.36f, 0.38f), mortar);
                        bestHit.specularStrength = 0.40f;
                        bestHit.shininess = 32.0f;
                        bestHit.reflectivity = 0.22f; // Wet cobblestone reflection
                    }
                    else if (std::abs(p.x) <= 2.45f)
                    {
                        bestHit.albedo = glm::vec3(0.24f, 0.24f, 0.26f);
                        bestHit.specularStrength = 0.25f;
                        bestHit.shininess = 16.0f;
                        bestHit.reflectivity = 0.05f;
                    }
                    else
                    {
                        bestHit.albedo = glm::vec3(0.18f, 0.24f, 0.16f); // Mossy ground
                        bestHit.specularStrength = 0.08f;
                        bestHit.shininess = 8.0f;
                        bestHit.reflectivity = 0.0f;
                    }
                }
            }
        }

        // 2. Torii Shrine Gate
        glm::vec3 toriiCol(0.85f, 0.22f, 0.12f);
        if (intersectCylinderY(ray, glm::vec3(-3.0f, 0.0f, -26.0f), 0.35f, 6.5f, t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = toriiCol; bestHit.specularStrength = 0.45f; bestHit.shininess = 32.0f; bestHit.reflectivity = 0.10f; bestHit.isEmissive = false; }
        }
        if (intersectCylinderY(ray, glm::vec3(3.0f, 0.0f, -26.0f), 0.35f, 6.5f, t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = toriiCol; bestHit.specularStrength = 0.45f; bestHit.shininess = 32.0f; bestHit.reflectivity = 0.10f; bestHit.isEmissive = false; }
        }
        if (intersectBox(ray, glm::vec3(-3.4f, 0.0f, -26.4f), glm::vec3(-2.6f, 0.45f, -25.6f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = glm::vec3(0.45f, 0.45f, 0.48f); bestHit.specularStrength = 0.2f; bestHit.shininess = 16.0f; bestHit.reflectivity = 0.0f; bestHit.isEmissive = false; }
        }
        if (intersectBox(ray, glm::vec3(2.6f, 0.0f, -26.4f), glm::vec3(3.4f, 0.45f, -25.6f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = glm::vec3(0.45f, 0.45f, 0.48f); bestHit.specularStrength = 0.2f; bestHit.shininess = 16.0f; bestHit.reflectivity = 0.0f; bestHit.isEmissive = false; }
        }
        if (intersectBox(ray, glm::vec3(-4.4f, 6.35f, -26.35f), glm::vec3(4.4f, 6.80f, -25.65f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = toriiCol; bestHit.specularStrength = 0.45f; bestHit.shininess = 32.0f; bestHit.reflectivity = 0.10f; bestHit.isEmissive = false; }
        }
        if (intersectBox(ray, glm::vec3(-3.9f, 5.95f, -26.30f), glm::vec3(3.9f, 6.35f, -25.70f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = toriiCol; bestHit.specularStrength = 0.45f; bestHit.shininess = 32.0f; bestHit.reflectivity = 0.10f; bestHit.isEmissive = false; }
        }
        if (intersectBox(ray, glm::vec3(-3.6f, 4.45f, -26.25f), glm::vec3(3.6f, 4.75f, -25.75f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = toriiCol; bestHit.specularStrength = 0.45f; bestHit.shininess = 32.0f; bestHit.reflectivity = 0.10f; bestHit.isEmissive = false; }
        }

        // 3. Machiya Townhouses (4 Buildings)
        glm::vec3 houseWood(0.36f, 0.22f, 0.13f);
        glm::vec3 roofSlate(0.16f, 0.17f, 0.20f);
        glm::vec3 shojiCol = glm::mix(glm::vec3(0.90f, 0.88f, 0.82f), glm::vec3(1.0f, 0.82f, 0.45f), data.dayNightFactor);

        // House 1
        if (intersectBox(ray, glm::vec3(-10.5f, 0.0f, 6.0f), glm::vec3(-4.5f, 4.5f, 18.0f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = houseWood; bestHit.specularStrength = 0.25f; bestHit.shininess = 16.0f; bestHit.reflectivity = 0.04f; bestHit.isEmissive = false; }
        }
        if (intersectBox(ray, glm::vec3(-11.0f, 4.3f, 5.5f), glm::vec3(-4.0f, 5.8f, 18.5f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = roofSlate; bestHit.specularStrength = 0.35f; bestHit.shininess = 32.0f; bestHit.reflectivity = 0.06f; bestHit.isEmissive = false; }
        }
        if (intersectBox(ray, glm::vec3(-4.48f, 0.0f, 9.5f), glm::vec3(-4.44f, 2.4f, 14.5f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = shojiCol; bestHit.specularStrength = 0.1f; bestHit.shininess = 8.0f; bestHit.reflectivity = 0.0f; bestHit.isEmissive = (data.dayNightFactor > 0.4f); bestHit.emissiveColor = shojiCol * (data.dayNightFactor * 0.9f); }
        }

        // House 2
        if (intersectBox(ray, glm::vec3(-10.5f, 0.0f, -10.0f), glm::vec3(-4.5f, 4.5f, 2.0f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = houseWood; bestHit.specularStrength = 0.25f; bestHit.shininess = 16.0f; bestHit.reflectivity = 0.04f; bestHit.isEmissive = false; }
        }
        if (intersectBox(ray, glm::vec3(-11.0f, 4.3f, -10.5f), glm::vec3(-4.0f, 5.8f, 2.5f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = roofSlate; bestHit.specularStrength = 0.35f; bestHit.shininess = 32.0f; bestHit.reflectivity = 0.06f; bestHit.isEmissive = false; }
        }

        // House 3
        if (intersectBox(ray, glm::vec3(4.5f, 0.0f, 6.0f), glm::vec3(10.5f, 4.5f, 18.0f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = houseWood; bestHit.specularStrength = 0.25f; bestHit.shininess = 16.0f; bestHit.reflectivity = 0.04f; bestHit.isEmissive = false; }
        }
        if (intersectBox(ray, glm::vec3(4.0f, 4.3f, 5.5f), glm::vec3(11.0f, 5.8f, 18.5f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = roofSlate; bestHit.specularStrength = 0.35f; bestHit.shininess = 32.0f; bestHit.reflectivity = 0.06f; bestHit.isEmissive = false; }
        }

        // House 4
        if (intersectBox(ray, glm::vec3(4.5f, 0.0f, -10.0f), glm::vec3(10.5f, 4.5f, 2.0f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = houseWood; bestHit.specularStrength = 0.25f; bestHit.shininess = 16.0f; bestHit.reflectivity = 0.04f; bestHit.isEmissive = false; }
        }
        if (intersectBox(ray, glm::vec3(4.0f, 4.3f, -10.5f), glm::vec3(11.0f, 5.8f, 2.5f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = roofSlate; bestHit.specularStrength = 0.35f; bestHit.shininess = 32.0f; bestHit.reflectivity = 0.06f; bestHit.isEmissive = false; }
        }

        // 4. Stalls
        // Takoyaki
        if (intersectBox(ray, glm::vec3(-5.2f, 0.0f, 4.5f), glm::vec3(-3.2f, 1.1f, 7.5f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = glm::vec3(0.28f, 0.18f, 0.10f); bestHit.specularStrength = 0.2f; bestHit.shininess = 16.0f; bestHit.reflectivity = 0.02f; bestHit.isEmissive = false; }
        }
        if (intersectBox(ray, glm::vec3(-5.4f, 2.1f, 4.3f), glm::vec3(-3.0f, 2.6f, 7.7f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = glm::vec3(0.85f, 0.20f, 0.18f); bestHit.specularStrength = 0.15f; bestHit.shininess = 8.0f; bestHit.reflectivity = 0.0f; bestHit.isEmissive = false; }
        }
        // Kakigori
        if (intersectBox(ray, glm::vec3(3.2f, 0.0f, 4.5f), glm::vec3(5.2f, 1.1f, 7.5f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = glm::vec3(0.28f, 0.18f, 0.10f); bestHit.specularStrength = 0.2f; bestHit.shininess = 16.0f; bestHit.reflectivity = 0.02f; bestHit.isEmissive = false; }
        }
        if (intersectBox(ray, glm::vec3(3.0f, 2.1f, 4.3f), glm::vec3(5.4f, 2.6f, 7.7f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = glm::vec3(0.20f, 0.65f, 0.88f); bestHit.specularStrength = 0.15f; bestHit.shininess = 8.0f; bestHit.reflectivity = 0.0f; bestHit.isEmissive = false; }
        }

        // 5. Magic Stage & Props
        // Stage Platform (Polished Mirror Lacquer Floor)
        if (intersectBox(ray, glm::vec3(-7.3f, 0.0f, -21.3f), glm::vec3(-2.3f, 0.45f, -16.7f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = glm::vec3(0.12f, 0.10f, 0.13f); bestHit.specularStrength = 0.70f; bestHit.shininess = 64.0f; bestHit.reflectivity = 0.40f; bestHit.isEmissive = false; }
        }
        // Magician Table
        if (intersectBox(ray, glm::vec3(-5.4f, 0.45f, -19.4f), glm::vec3(-4.2f, 1.35f, -18.6f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = glm::vec3(0.70f, 0.12f, 0.15f); bestHit.specularStrength = 0.3f; bestHit.shininess = 24.0f; bestHit.reflectivity = 0.08f; bestHit.isEmissive = false; }
        }
        // Golden Byobu Folding Screen (High Gold Metallic Mirror)
        if (intersectBox(ray, glm::vec3(-6.8f, 0.45f, -20.8f), glm::vec3(-2.8f, 2.65f, -20.5f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = glm::vec3(0.92f, 0.78f, 0.22f); bestHit.specularStrength = 0.85f; bestHit.shininess = 128.0f; bestHit.reflectivity = 0.65f; bestHit.isEmissive = false; }
        }
        // Magician
        if (intersectBox(ray, glm::vec3(-4.2f, 0.45f, -19.2f), glm::vec3(-3.6f, 1.85f, -18.6f), t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = glm::vec3(0.15f, 0.12f, 0.22f); bestHit.specularStrength = 0.3f; bestHit.shininess = 16.0f; bestHit.reflectivity = 0.05f; bestHit.isEmissive = false; }
        }
        if (intersectSphere(ray, glm::vec3(-3.9f, 2.05f, -18.9f), 0.18f, t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = glm::vec3(0.92f, 0.78f, 0.70f); bestHit.specularStrength = 0.25f; bestHit.shininess = 24.0f; bestHit.reflectivity = 0.0f; bestHit.isEmissive = false; }
        }

        // Vanishing Box (Dynamic Transform Trick)
        if (data.boxScale.x > 0.02f)
        {
            glm::vec3 bHalf = glm::vec3(0.18f) * data.boxScale;
            if (intersectBox(ray, data.boxPos - bHalf, data.boxPos + bHalf, t, norm))
            {
                if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = glm::vec3(0.88f, 0.20f, 0.18f); bestHit.specularStrength = 0.85f; bestHit.shininess = 96.0f; bestHit.reflectivity = 0.60f; bestHit.isEmissive = false; }
            }
        }

        // Dynamic Magic Orb (Crystal Mirror Cyan Sphere)
        if (intersectSphere(ray, data.orbPos, 0.22f, t, norm))
        {
            if (t < minT)
            {
                minT = t;
                hitSomething = true;
                bestHit.t = t;
                bestHit.p = ray.at(t);
                bestHit.normal = norm;
                bestHit.albedo = glm::vec3(0.3f, 0.8f, 1.4f);
                bestHit.specularStrength = 1.0f;
                bestHit.shininess = 128.0f;
                bestHit.reflectivity = 0.85f;
                bestHit.isEmissive = true;
                bestHit.emissiveColor = glm::vec3(0.3f, 0.8f, 1.6f);
            }
        }

        // 6. Street Lantern Poles & Lanterns (5 spans along Z)
        float spanZ[5] = { 22.0f, 11.0f, 0.0f, -9.0f, -24.5f };
        for (int i = 0; i < 5; ++i)
        {
            float sz = spanZ[i];
            if (intersectCylinderY(ray, glm::vec3(-3.8f, 0.0f, sz), 0.14f, 6.2f, t, norm))
            {
                if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = houseWood; bestHit.specularStrength = 0.2f; bestHit.shininess = 16.0f; bestHit.reflectivity = 0.02f; bestHit.isEmissive = false; }
            }
            if (intersectCylinderY(ray, glm::vec3(3.8f, 0.0f, sz), 0.14f, 6.2f, t, norm))
            {
                if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = houseWood; bestHit.specularStrength = 0.2f; bestHit.shininess = 16.0f; bestHit.reflectivity = 0.02f; bestHit.isEmissive = false; }
            }
            // Hanging Lanterns
            if (intersectSphere(ray, glm::vec3(-1.9f, 5.71f, sz), 0.30f, t, norm))
            {
                if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = glm::vec3(0.92f, 0.18f, 0.12f); bestHit.isEmissive = true; bestHit.emissiveColor = glm::vec3(1.4f, 0.50f, 0.18f) * glm::mix(0.7f, 1.3f, data.dayNightFactor); }
            }
            if (intersectSphere(ray, glm::vec3(1.9f, 5.71f, sz), 0.30f, t, norm))
            {
                if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = glm::vec3(0.92f, 0.18f, 0.12f); bestHit.isEmissive = true; bestHit.emissiveColor = glm::vec3(1.4f, 0.50f, 0.18f) * glm::mix(0.7f, 1.3f, data.dayNightFactor); }
            }
        }

        // 7. Sakura Blossom Tree
        if (intersectCylinderY(ray, glm::vec3(4.8f, 0.0f, -23.0f), 0.32f, 4.0f, t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = glm::vec3(0.32f, 0.20f, 0.14f); bestHit.specularStrength = 0.2f; bestHit.shininess = 16.0f; bestHit.reflectivity = 0.02f; bestHit.isEmissive = false; }
        }
        glm::vec3 blossomPink(0.98f, 0.72f, 0.82f);
        if (intersectSphere(ray, glm::vec3(4.8f, 4.6f, -23.0f), 1.6f, t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.p = ray.at(t); bestHit.normal = norm; bestHit.albedo = blossomPink; bestHit.specularStrength = 0.15f; bestHit.shininess = 12.0f; bestHit.reflectivity = 0.0f; bestHit.isEmissive = false; }
        }

        return hitSomething;
    }

    inline bool traceShadow(const Ray& shadowRay, const SceneSnapshotData& data, float maxDist)
    {
        float t;
        glm::vec3 norm;

        // Fast occlusion tests against dominant silhouettes
        if (intersectCylinderY(shadowRay, glm::vec3(-3.0f, 0.0f, -26.0f), 0.35f, 6.5f, t, norm)) if (t < maxDist) return true;
        if (intersectCylinderY(shadowRay, glm::vec3(3.0f, 0.0f, -26.0f), 0.35f, 6.5f, t, norm)) if (t < maxDist) return true;

        if (intersectBox(shadowRay, glm::vec3(-10.5f, 0.0f, 6.0f), glm::vec3(-4.5f, 4.5f, 18.0f), t, norm)) if (t < maxDist) return true;
        if (intersectBox(shadowRay, glm::vec3(-10.5f, 0.0f, -10.0f), glm::vec3(-4.5f, 4.5f, 2.0f), t, norm)) if (t < maxDist) return true;
        if (intersectBox(shadowRay, glm::vec3(4.5f, 0.0f, 6.0f), glm::vec3(10.5f, 4.5f, 18.0f), t, norm)) if (t < maxDist) return true;
        if (intersectBox(shadowRay, glm::vec3(4.5f, 0.0f, -10.0f), glm::vec3(10.5f, 4.5f, 2.0f), t, norm)) if (t < maxDist) return true;

        if (intersectBox(shadowRay, glm::vec3(-5.2f, 0.0f, 4.5f), glm::vec3(-3.2f, 1.1f, 7.5f), t, norm)) if (t < maxDist) return true;
        if (intersectBox(shadowRay, glm::vec3(3.2f, 0.0f, 4.5f), glm::vec3(5.2f, 1.1f, 7.5f), t, norm)) if (t < maxDist) return true;

        if (intersectBox(shadowRay, glm::vec3(-7.3f, 0.0f, -21.3f), glm::vec3(-2.3f, 0.45f, -16.7f), t, norm)) if (t < maxDist) return true;
        if (intersectBox(shadowRay, glm::vec3(-6.8f, 0.45f, -20.8f), glm::vec3(-2.8f, 2.65f, -20.5f), t, norm)) if (t < maxDist) return true;

        return false;
    }

    inline glm::vec3 computeDirectLighting(const HitRecord& hit, const glm::vec3& viewDir, const SceneSnapshotData& data)
    {
        glm::vec3 result(0.0f);

        // 1. Ambient Term with Contact Ambient Occlusion
        glm::vec3 dayAmb(0.35f, 0.38f, 0.42f);
        glm::vec3 nightAmb(0.06f, 0.08f, 0.14f);
        float contactAO = std::clamp(hit.p.y * 1.6f + 0.40f, 0.40f, 1.0f);
        glm::vec3 ambient = glm::mix(dayAmb, nightAmb, data.dayNightFactor) * hit.albedo * contactAO;
        result += ambient;

        glm::vec3 shadowOrig = hit.p + hit.normal * 0.003f;

        // 2. Directional Sun / Moon with Soft Penumbra Shadows
        glm::vec3 sunDir = glm::normalize(glm::mix(glm::vec3(0.4f, 0.8f, 0.5f), glm::vec3(-0.3f, 0.7f, -0.4f), data.dayNightFactor));
        glm::vec3 sunCol = glm::mix(glm::vec3(1.0f, 0.95f, 0.80f), glm::vec3(0.20f, 0.28f, 0.48f), data.dayNightFactor);

        glm::vec3 uAxis = glm::normalize(glm::cross(sunDir, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 vAxis = glm::cross(sunDir, uAxis);
        float sunShadow = 0.0f;
        glm::vec2 pcfOffsets[4] = { glm::vec2(-0.02f, -0.02f), glm::vec2(0.02f, -0.02f), glm::vec2(-0.02f, 0.02f), glm::vec2(0.02f, 0.02f) };
        for (int s = 0; s < 4; ++s)
        {
            glm::vec3 jitteredDir = glm::normalize(sunDir + uAxis * pcfOffsets[s].x + vAxis * pcfOffsets[s].y);
            Ray sunShadowRay(shadowOrig, jitteredDir);
            if (traceShadow(sunShadowRay, data, 300.0f))
                sunShadow += 0.25f;
        }

        if (sunShadow < 0.99f)
        {
            float diff = std::max(glm::dot(hit.normal, sunDir), 0.0f);
            glm::vec3 halfDir = glm::normalize(sunDir + viewDir);
            float spec = std::pow(std::max(glm::dot(hit.normal, halfDir), 0.0f), hit.shininess) * hit.specularStrength;
            result += (1.0f - sunShadow) * (hit.albedo * diff + glm::vec3(spec)) * sunCol * glm::mix(0.9f, 0.4f, data.dayNightFactor);
        }

        // 3. Dynamic Magic Orb Light
        glm::vec3 toOrb = data.orbPos - shadowOrig;
        float distOrb = glm::length(toOrb);
        if (distOrb < 25.0f && distOrb > 0.01f)
        {
            glm::vec3 lDir = toOrb / distOrb;
            Ray orbShadowRay(shadowOrig, lDir);
            if (!traceShadow(orbShadowRay, data, distOrb))
            {
                float atten = 1.0f / (1.0f + 0.15f * distOrb + 0.06f * distOrb * distOrb);
                float diff = std::max(glm::dot(hit.normal, lDir), 0.0f);
                glm::vec3 halfDir = glm::normalize(lDir + viewDir);
                float spec = std::pow(std::max(glm::dot(hit.normal, halfDir), 0.0f), hit.shininess) * hit.specularStrength;
                glm::vec3 orbCol(0.25f, 0.75f, 1.4f);
                result += (hit.albedo * diff + glm::vec3(spec)) * orbCol * atten * 1.8f;
            }
        }

        return result;
    }

    inline glm::vec3 traceRay(const Ray& initialRay, const SceneSnapshotData& data, int maxBounces = 3)
    {
        glm::vec3 finalColor(0.0f);
        glm::vec3 throughput(1.0f);
        Ray curRay = initialRay;

        for (int bounce = 0; bounce < maxBounces; ++bounce)
        {
            HitRecord hit;
            if (!traceScene(curRay, data, hit))
            {
                // Hit Sky
                float heightRatio = std::clamp((curRay.direction.y + 0.2f) / 1.2f, 0.0f, 1.0f);
                glm::vec3 daySky = glm::mix(glm::vec3(0.55f, 0.75f, 0.98f), glm::vec3(0.82f, 0.88f, 0.98f), heightRatio);
                glm::vec3 nightSky = glm::mix(glm::vec3(0.02f, 0.03f, 0.08f), glm::vec3(0.06f, 0.08f, 0.18f), heightRatio);
                glm::vec3 skyCol = glm::mix(daySky, nightSky, data.dayNightFactor);

                if (data.fireworkActive > 0.01f)
                {
                    glm::vec3 toFw = glm::normalize(data.fireworkPos - curRay.origin);
                    float fwDot = std::max(glm::dot(curRay.direction, toFw), 0.0f);
                    skyCol += data.fireworkColor * std::pow(fwDot, 24.0f) * data.fireworkActive * 2.2f;
                }

                finalColor += throughput * skyCol;
                break;
            }

            if (hit.isEmissive)
            {
                finalColor += throughput * hit.emissiveColor;
                break;
            }

            glm::vec3 viewDir = -curRay.direction;
            glm::vec3 direct = computeDirectLighting(hit, viewDir, data);
            finalColor += throughput * direct;

            if (hit.reflectivity > 0.02f && bounce < maxBounces - 1)
            {
                throughput *= (hit.albedo * 0.5f + glm::vec3(0.5f)) * hit.reflectivity;
                glm::vec3 reflDir = glm::reflect(curRay.direction, hit.normal);
                curRay = Ray(hit.p + hit.normal * 0.003f, reflDir);
            }
            else
            {
                break;
            }
        }

        return finalColor;
    }

    // High-Performance Multi-Threaded CPU Snapshot Renderer
    inline bool renderSnapshot(
        const Camera& camera,
        int width,
        int height,
        const SceneSnapshotData& data,
        const std::string& outputFile,
        int maxBounces = 3)
    {
        auto startTime = std::chrono::high_resolution_clock::now();

        unsigned int numThreads = std::max(1u, std::thread::hardware_concurrency());
        std::cout << "\n========================================================================\n";
        std::cout << " [CPU RAY TRACER] Starting Multi-Threaded Ray Tracing Snapshot...\n";
        std::cout << "  Resolution: " << width << " x " << height << "\n";
        std::cout << "  CPU Threads: " << numThreads << "\n";
        std::cout << "  Max Specular Bounces: " << maxBounces << "\n";
        std::cout << "  Output File: " << outputFile << "\n";
        std::cout << "========================================================================\n";

        std::vector<unsigned char> rgb(width * height * 3, 0);

        float aspect = (float)width / (float)height;
        float tanHalfFov = std::tan(glm::radians(camera.Zoom) * 0.5f);
        glm::vec3 camPos = camera.Position;
        glm::vec3 camFront = camera.Front;
        glm::vec3 camUp = camera.Up;
        glm::vec3 camRight = camera.Right;

        auto worker = [&](int startY, int endY)
        {
            for (int y = startY; y < endY; ++y)
            {
                float v = (float)y / (float)height;
                float ndcY = v * 2.0f - 1.0f;

                for (int x = 0; x < width; ++x)
                {
                    float u = (float)x / (float)width;
                    float ndcX = u * 2.0f - 1.0f;

                    glm::vec3 rayDir = glm::normalize(camFront + (ndcX * aspect * tanHalfFov) * camRight + (ndcY * tanHalfFov) * camUp);
                    Ray ray(camPos, rayDir);

                    glm::vec3 color = traceRay(ray, data, maxBounces);

                    // Clamp & tone-map to [0, 255]
                    color = glm::clamp(color, 0.0f, 1.0f);
                    // Standard gamma correction 2.2
                    color = glm::pow(color, glm::vec3(1.0f / 2.2f));

                    int pixelIdx = (y * width + x) * 3;
                    rgb[pixelIdx + 0] = static_cast<unsigned char>(color.r * 255.0f);
                    rgb[pixelIdx + 1] = static_cast<unsigned char>(color.g * 255.0f);
                    rgb[pixelIdx + 2] = static_cast<unsigned char>(color.b * 255.0f);
                }
            }
        };

        std::vector<std::thread> threads;
        int rowsPerThread = height / numThreads;
        for (unsigned int i = 0; i < numThreads; ++i)
        {
            int startY = i * rowsPerThread;
            int endY = (i == numThreads - 1) ? height : (i + 1) * rowsPerThread;
            threads.emplace_back(worker, startY, endY);
        }

        for (auto& t : threads)
        {
            if (t.joinable())
                t.join();
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = endTime - startTime;

        bool written = TextureGenerator::writeBMP24(outputFile, width, height, rgb);
        if (written)
        {
            std::cout << "[CPU RAY TRACER] Completed successfully in " << std::fixed << std::setprecision(2)
                      << elapsed.count() << " seconds!" << std::endl;
            std::cout << "[CPU RAY TRACER] Saved snapshot image to: " << outputFile << "\n\n";
        }
        else
        {
            std::cerr << "[CPU RAY TRACER] Failed to write BMP file to disk!" << std::endl;
        }

        return written;
    }
}
