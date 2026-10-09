#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

// Camera Uniforms
uniform vec3 uCamPos;
uniform vec3 uCamFront;
uniform vec3 uCamUp;
uniform vec3 uCamRight;
uniform vec2 uResolution;
uniform float uFov;
uniform float uAspect;
uniform float uTime;
uniform float uNightFactor;

// Dynamic Scene Transforms
uniform vec3 uOrbPos;
uniform vec3 uBoxPos;
uniform vec3 uBoxScale;
uniform vec3 uSpotPos;
uniform vec3 uSpotDir;
uniform vec3 uFireworksPos;
uniform vec3 uFireworksColor;
uniform float uFireworksActive;
uniform int uMaxBounces;

struct HitInfo
{
    float t;
    vec3 normal;
    vec3 albedo;
    float specularStrength;
    float shininess;
    float reflectivity;
    bool isEmissive;
    vec3 emissiveColor;
};

// Analytical Ray-Sphere Intersection
bool intersectSphere(vec3 ro, vec3 rd, vec3 center, float radius, out float tHit, out vec3 outNorm)
{
    vec3 oc = ro - center;
    float b = dot(oc, rd);
    float c = dot(oc, oc) - radius * radius;
    float disc = b * b - c;
    if (disc < 0.0) return false;
    float sqrtD = sqrt(disc);
    float t = -b - sqrtD;
    if (t < 0.002) t = -b + sqrtD;
    if (t < 0.002) return false;
    tHit = t;
    outNorm = normalize((ro + rd * t) - center);
    return true;
}

// Analytical Ray-AABB Box Intersection (Slab Method)
bool intersectBox(vec3 ro, vec3 rd, vec3 bMin, vec3 bMax, out float tHit, out vec3 outNorm)
{
    vec3 invD = 1.0 / rd;
    vec3 t0 = (bMin - ro) * invD;
    vec3 t1 = (bMax - ro) * invD;
    vec3 tmin = min(t0, t1);
    vec3 tmax = max(t0, t1);
    float tNear = max(max(tmin.x, tmin.y), tmin.z);
    float tFar = min(min(tmax.x, tmax.y), tmax.z);
    if (tNear > tFar || tFar < 0.002) return false;
    float t = (tNear > 0.002) ? tNear : tFar;
    if (t < 0.002) return false;
    tHit = t;

    vec3 hitPt = ro + rd * t;
    vec3 c = (bMin + bMax) * 0.5;
    vec3 d = (bMax - bMin) * 0.5;
    vec3 p = (hitPt - c) / d;
    vec3 absP = abs(p);
    if (absP.x > absP.y && absP.x > absP.z)
        outNorm = vec3(sign(p.x), 0.0, 0.0);
    else if (absP.y > absP.z)
        outNorm = vec3(0.0, sign(p.y), 0.0);
    else
        outNorm = vec3(0.0, 0.0, sign(p.z));
    return true;
}

// Analytical Vertical Cylinder Intersection along Y
bool intersectCylinderY(vec3 ro, vec3 rd, vec3 baseCenter, float radius, float height, out float tHit, out vec3 outNorm)
{
    vec3 oc = ro - baseCenter;
    float a = rd.x * rd.x + rd.z * rd.z;
    if (a < 1e-6) return false;
    float b = 2.0 * (oc.x * rd.x + oc.z * rd.z);
    float c = oc.x * oc.x + oc.z * oc.z - radius * radius;
    float disc = b * b - 4.0 * a * c;
    if (disc < 0.0) return false;
    float sqrtD = sqrt(disc);
    float t = (-b - sqrtD) / (2.0 * a);
    if (t < 0.002) t = (-b + sqrtD) / (2.0 * a);
    if (t < 0.002) return false;
    float yHit = oc.y + rd.y * t;
    if (yHit >= 0.0 && yHit <= height)
    {
        tHit = t;
        vec3 hitPt = ro + rd * t;
        outNorm = normalize(vec3(hitPt.x - baseCenter.x, 0.0, hitPt.z - baseCenter.z));
        return true;
    }
    return false;
}

// Ray-Scene Geometry Traversal
bool traceScene(vec3 ro, vec3 rd, out HitInfo bestHit)
{
    float minT = 1e20;
    bool hitSomething = false;
    float t;
    vec3 norm;

    // 1. Ground Plane (Y = 0)
    if (abs(rd.y) > 1e-5)
    {
        t = -ro.y / rd.y;
        if (t > 0.002 && t < minT)
        {
            vec3 p = ro + rd * t;
            if (abs(p.x) <= 25.0 && abs(p.z) <= 35.0)
            {
                minT = t;
                hitSomething = true;
                bestHit.t = t;
                bestHit.normal = vec3(0.0, 1.0, 0.0);
                bestHit.isEmissive = false;
                bestHit.emissiveColor = vec3(0.0);

                if (abs(p.x) <= 2.2)
                {
                    // Cobblestone stone pavement
                    vec2 uv = p.xz * 1.8;
                    vec2 f = fract(uv);
                    float mortar = step(0.05, f.x) * step(0.05, f.y);
                    vec3 stoneCol = mix(vec3(0.18, 0.18, 0.20), vec3(0.36, 0.36, 0.38), mortar);
                    bestHit.albedo = stoneCol;
                    bestHit.specularStrength = 0.40;
                    bestHit.shininess = 32.0;
                    bestHit.reflectivity = 0.22; // Wet stone road specular reflections
                }
                else if (abs(p.x) <= 2.45)
                {
                    // Stone curbs
                    bestHit.albedo = vec3(0.24, 0.24, 0.26);
                    bestHit.specularStrength = 0.25;
                    bestHit.shininess = 16.0;
                    bestHit.reflectivity = 0.05;
                }
                else
                {
                    // Mossy ground terrain
                    bestHit.albedo = vec3(0.18, 0.24, 0.16);
                    bestHit.specularStrength = 0.08;
                    bestHit.shininess = 8.0;
                    bestHit.reflectivity = 0.0;
                }
            }
        }
    }

    // 2. Torii Shrine Gate (Z = -26.0)
    vec3 toriiCol = vec3(0.85, 0.22, 0.12); // Vermilion
    if (intersectCylinderY(ro, rd, vec3(-3.0, 0.0, -26.0), 0.35, 6.5, t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = toriiCol; bestHit.specularStrength = 0.45; bestHit.shininess = 32.0; bestHit.reflectivity = 0.10; bestHit.isEmissive = false; }
    }
    if (intersectCylinderY(ro, rd, vec3(3.0, 0.0, -26.0), 0.35, 6.5, t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = toriiCol; bestHit.specularStrength = 0.45; bestHit.shininess = 32.0; bestHit.reflectivity = 0.10; bestHit.isEmissive = false; }
    }
    // Torii Pedestals
    if (intersectBox(ro, rd, vec3(-3.4, 0.0, -26.4), vec3(-2.6, 0.45, -25.6), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.45, 0.45, 0.48); bestHit.specularStrength = 0.2; bestHit.shininess = 16.0; bestHit.reflectivity = 0.0; bestHit.isEmissive = false; }
    }
    if (intersectBox(ro, rd, vec3(2.6, 0.0, -26.4), vec3(3.4, 0.45, -25.6), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.45, 0.45, 0.48); bestHit.specularStrength = 0.2; bestHit.shininess = 16.0; bestHit.reflectivity = 0.0; bestHit.isEmissive = false; }
    }
    // Torii Top Beams (Kasagi & Shimaki)
    if (intersectBox(ro, rd, vec3(-4.4, 6.35, -26.35), vec3(4.4, 6.80, -25.65), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = toriiCol; bestHit.specularStrength = 0.45; bestHit.shininess = 32.0; bestHit.reflectivity = 0.10; bestHit.isEmissive = false; }
    }
    if (intersectBox(ro, rd, vec3(-3.9, 5.95, -26.30), vec3(3.9, 6.35, -25.70), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = toriiCol; bestHit.specularStrength = 0.45; bestHit.shininess = 32.0; bestHit.reflectivity = 0.10; bestHit.isEmissive = false; }
    }
    // Nuki Crossbeam
    if (intersectBox(ro, rd, vec3(-3.6, 4.45, -26.25), vec3(3.6, 4.75, -25.75), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = toriiCol; bestHit.specularStrength = 0.45; bestHit.shininess = 32.0; bestHit.reflectivity = 0.10; bestHit.isEmissive = false; }
    }
    // Gakuzuka Plaque
    if (intersectBox(ro, rd, vec3(-0.25, 4.75, -26.15), vec3(0.25, 5.95, -25.85), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.10, 0.10, 0.12); bestHit.specularStrength = 0.5; bestHit.shininess = 48.0; bestHit.reflectivity = 0.15; bestHit.isEmissive = false; }
    }

    // 3. Machiya Traditional Townhouses (4 Buildings)
    vec3 houseWood = vec3(0.36, 0.22, 0.13);
    vec3 roofSlate = vec3(0.16, 0.17, 0.20);
    vec3 shojiCol = mix(vec3(0.90, 0.88, 0.82), vec3(1.0, 0.82, 0.45), uNightFactor);

    // Left Near House
    if (intersectBox(ro, rd, vec3(-10.5, 0.0, 6.0), vec3(-4.5, 4.5, 18.0), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = houseWood; bestHit.specularStrength = 0.25; bestHit.shininess = 16.0; bestHit.reflectivity = 0.04; bestHit.isEmissive = false; }
    }
    if (intersectBox(ro, rd, vec3(-11.0, 4.3, 5.5), vec3(-4.0, 5.8, 18.5), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = roofSlate; bestHit.specularStrength = 0.35; bestHit.shininess = 32.0; bestHit.reflectivity = 0.06; bestHit.isEmissive = false; }
    }
    if (intersectBox(ro, rd, vec3(-4.48, 0.0, 9.5), vec3(-4.44, 2.4, 14.5), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = shojiCol; bestHit.specularStrength = 0.1; bestHit.shininess = 8.0; bestHit.reflectivity = 0.0; bestHit.isEmissive = (uNightFactor > 0.4); bestHit.emissiveColor = shojiCol * (uNightFactor * 0.9); }
    }

    // Left Far House
    if (intersectBox(ro, rd, vec3(-10.5, 0.0, -10.0), vec3(-4.5, 4.5, 2.0), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = houseWood; bestHit.specularStrength = 0.25; bestHit.shininess = 16.0; bestHit.reflectivity = 0.04; bestHit.isEmissive = false; }
    }
    if (intersectBox(ro, rd, vec3(-11.0, 4.3, -10.5), vec3(-4.0, 5.8, 2.5), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = roofSlate; bestHit.specularStrength = 0.35; bestHit.shininess = 32.0; bestHit.reflectivity = 0.06; bestHit.isEmissive = false; }
    }
    if (intersectBox(ro, rd, vec3(-4.48, 0.0, -6.5), vec3(-4.44, 2.4, -1.5), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = shojiCol; bestHit.specularStrength = 0.1; bestHit.shininess = 8.0; bestHit.reflectivity = 0.0; bestHit.isEmissive = (uNightFactor > 0.4); bestHit.emissiveColor = shojiCol * (uNightFactor * 0.9); }
    }

    // Right Near House
    if (intersectBox(ro, rd, vec3(4.5, 0.0, 6.0), vec3(10.5, 4.5, 18.0), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = houseWood; bestHit.specularStrength = 0.25; bestHit.shininess = 16.0; bestHit.reflectivity = 0.04; bestHit.isEmissive = false; }
    }
    if (intersectBox(ro, rd, vec3(4.0, 4.3, 5.5), vec3(11.0, 5.8, 18.5), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = roofSlate; bestHit.specularStrength = 0.35; bestHit.shininess = 32.0; bestHit.reflectivity = 0.06; bestHit.isEmissive = false; }
    }
    if (intersectBox(ro, rd, vec3(4.44, 0.0, 9.5), vec3(4.48, 2.4, 14.5), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = shojiCol; bestHit.specularStrength = 0.1; bestHit.shininess = 8.0; bestHit.reflectivity = 0.0; bestHit.isEmissive = (uNightFactor > 0.4); bestHit.emissiveColor = shojiCol * (uNightFactor * 0.9); }
    }

    // Right Far House
    if (intersectBox(ro, rd, vec3(4.5, 0.0, -10.0), vec3(10.5, 4.5, 2.0), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = houseWood; bestHit.specularStrength = 0.25; bestHit.shininess = 16.0; bestHit.reflectivity = 0.04; bestHit.isEmissive = false; }
    }
    if (intersectBox(ro, rd, vec3(4.0, 4.3, -10.5), vec3(11.0, 5.8, 2.5), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = roofSlate; bestHit.specularStrength = 0.35; bestHit.shininess = 32.0; bestHit.reflectivity = 0.06; bestHit.isEmissive = false; }
    }
    if (intersectBox(ro, rd, vec3(4.44, 0.0, -6.5), vec3(4.48, 2.4, -1.5), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = shojiCol; bestHit.specularStrength = 0.1; bestHit.shininess = 8.0; bestHit.reflectivity = 0.0; bestHit.isEmissive = (uNightFactor > 0.4); bestHit.emissiveColor = shojiCol * (uNightFactor * 0.9); }
    }

    // 4. Food Stalls (Takoyaki & Kakigori)
    // Takoyaki Stall
    if (intersectBox(ro, rd, vec3(-5.2, 0.0, 4.5), vec3(-3.2, 1.1, 7.5), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.28, 0.18, 0.10); bestHit.specularStrength = 0.2; bestHit.shininess = 16.0; bestHit.reflectivity = 0.02; bestHit.isEmissive = false; }
    }
    if (intersectBox(ro, rd, vec3(-5.4, 2.1, 4.3), vec3(-3.0, 2.6, 7.7), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.85, 0.20, 0.18); bestHit.specularStrength = 0.15; bestHit.shininess = 8.0; bestHit.reflectivity = 0.0; bestHit.isEmissive = false; }
    }
    // Stall Lanterns (Emissive)
    if (intersectSphere(ro, rd, vec3(-3.3, 1.9, 5.2), 0.20, t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(1.0, 0.5, 0.15); bestHit.isEmissive = true; bestHit.emissiveColor = vec3(1.3, 0.65, 0.20); }
    }
    if (intersectSphere(ro, rd, vec3(-3.3, 1.9, 6.8), 0.20, t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(1.0, 0.5, 0.15); bestHit.isEmissive = true; bestHit.emissiveColor = vec3(1.3, 0.65, 0.20); }
    }

    // Kakigori Stall
    if (intersectBox(ro, rd, vec3(3.2, 0.0, 4.5), vec3(5.2, 1.1, 7.5), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.28, 0.18, 0.10); bestHit.specularStrength = 0.2; bestHit.shininess = 16.0; bestHit.reflectivity = 0.02; bestHit.isEmissive = false; }
    }
    if (intersectBox(ro, rd, vec3(3.0, 2.1, 4.3), vec3(5.4, 2.6, 7.7), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.20, 0.65, 0.88); bestHit.specularStrength = 0.15; bestHit.shininess = 8.0; bestHit.reflectivity = 0.0; bestHit.isEmissive = false; }
    }
    if (intersectSphere(ro, rd, vec3(3.3, 1.9, 5.2), 0.20, t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.3, 0.8, 1.1); bestHit.isEmissive = true; bestHit.emissiveColor = vec3(0.4, 1.0, 1.4); }
    }
    if (intersectSphere(ro, rd, vec3(3.3, 1.9, 6.8), 0.20, t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.3, 0.8, 1.1); bestHit.isEmissive = true; bestHit.emissiveColor = vec3(0.4, 1.0, 1.4); }
    }

    // 5. Magic Show Stage & Magician
    // Stage Platform (Polished Lacquered Mirror Surface)
    if (intersectBox(ro, rd, vec3(-7.3, 0.0, -21.3), vec3(-2.3, 0.45, -16.7), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.12, 0.10, 0.13); bestHit.specularStrength = 0.70; bestHit.shininess = 64.0; bestHit.reflectivity = 0.40; bestHit.isEmissive = false; }
    }
    // Magician Table
    if (intersectBox(ro, rd, vec3(-5.4, 0.45, -19.4), vec3(-4.2, 1.35, -18.6), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.70, 0.12, 0.15); bestHit.specularStrength = 0.3; bestHit.shininess = 24.0; bestHit.reflectivity = 0.08; bestHit.isEmissive = false; }
    }
    // Golden Byobu Folding Screen (Metallic Gold Mirror)
    if (intersectBox(ro, rd, vec3(-6.8, 0.45, -20.8), vec3(-2.8, 2.65, -20.5), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.92, 0.78, 0.22); bestHit.specularStrength = 0.85; bestHit.shininess = 128.0; bestHit.reflectivity = 0.65; bestHit.isEmissive = false; }
    }
    // Magician Figure
    if (intersectBox(ro, rd, vec3(-4.2, 0.45, -19.2), vec3(-3.6, 1.85, -18.6), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.15, 0.12, 0.22); bestHit.specularStrength = 0.3; bestHit.shininess = 16.0; bestHit.reflectivity = 0.05; bestHit.isEmissive = false; }
    }
    if (intersectSphere(ro, rd, vec3(-3.9, 2.05, -18.9), 0.18, t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.92, 0.78, 0.70); bestHit.specularStrength = 0.25; bestHit.shininess = 24.0; bestHit.reflectivity = 0.0; bestHit.isEmissive = false; }
    }
    if (intersectBox(ro, rd, vec3(-4.1, 2.20, -19.1), vec3(-3.7, 2.65, -18.7), t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.08, 0.08, 0.09); bestHit.specularStrength = 0.4; bestHit.shininess = 32.0; bestHit.reflectivity = 0.10; bestHit.isEmissive = false; }
    }

    // Dynamic Vanishing Box (Gold Foil Trimmed Trick Box)
    if (uBoxScale.x > 0.02)
    {
        vec3 bHalf = vec3(0.18) * uBoxScale;
        if (intersectBox(ro, rd, uBoxPos - bHalf, uBoxPos + bHalf, t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.88, 0.20, 0.18); bestHit.specularStrength = 0.85; bestHit.shininess = 96.0; bestHit.reflectivity = 0.60; bestHit.isEmissive = false; }
        }
    }

    // Dynamic Magic Orb (Helical Orbiting Crystal Glass Sphere)
    if (intersectSphere(ro, rd, uOrbPos, 0.22, t, norm))
    {
        if (t < minT)
        {
            minT = t;
            hitSomething = true;
            bestHit.t = t;
            bestHit.normal = norm;
            bestHit.albedo = vec3(0.3, 0.8, 1.4);
            bestHit.specularStrength = 1.0;
            bestHit.shininess = 128.0;
            bestHit.reflectivity = 0.85;
            bestHit.isEmissive = true;
            bestHit.emissiveColor = vec3(0.3, 0.8, 1.6);
        }
    }

    // 6. Street Lantern Poles & Chochin Lanterns (5 spans along Z)
    float spanZ[5] = float[5](22.0, 11.0, 0.0, -9.0, -24.5);
    for (int i = 0; i < 5; ++i)
    {
        float sz = spanZ[i];
        // Left Pole
        if (intersectCylinderY(ro, rd, vec3(-3.8, 0.0, sz), 0.14, 6.2, t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = houseWood; bestHit.specularStrength = 0.2; bestHit.shininess = 16.0; bestHit.reflectivity = 0.02; bestHit.isEmissive = false; }
        }
        // Right Pole
        if (intersectCylinderY(ro, rd, vec3(3.8, 0.0, sz), 0.14, 6.2, t, norm))
        {
            if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = houseWood; bestHit.specularStrength = 0.2; bestHit.shininess = 16.0; bestHit.reflectivity = 0.02; bestHit.isEmissive = false; }
        }
        // Left Hanging Lantern (Emissive Chochin)
        if (intersectSphere(ro, rd, vec3(-1.9, 5.71, sz), 0.30, t, norm))
        {
            if (t < minT)
            {
                minT = t;
                hitSomething = true;
                bestHit.t = t;
                bestHit.normal = norm;
                bestHit.albedo = vec3(0.92, 0.18, 0.12);
                bestHit.isEmissive = true;
                bestHit.emissiveColor = vec3(1.4, 0.50, 0.18) * mix(0.7, 1.3, uNightFactor);
            }
        }
        // Right Hanging Lantern (Emissive Chochin)
        if (intersectSphere(ro, rd, vec3(1.9, 5.71, sz), 0.30, t, norm))
        {
            if (t < minT)
            {
                minT = t;
                hitSomething = true;
                bestHit.t = t;
                bestHit.normal = norm;
                bestHit.albedo = vec3(0.92, 0.18, 0.12);
                bestHit.isEmissive = true;
                bestHit.emissiveColor = vec3(1.4, 0.50, 0.18) * mix(0.7, 1.3, uNightFactor);
            }
        }
    }

    // 7. Sakura Blossom Tree (Z = -23.0, X = 4.8)
    if (intersectCylinderY(ro, rd, vec3(4.8, 0.0, -23.0), 0.32, 4.0, t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = vec3(0.32, 0.20, 0.14); bestHit.specularStrength = 0.2; bestHit.shininess = 16.0; bestHit.reflectivity = 0.02; bestHit.isEmissive = false; }
    }
    vec3 blossomPink = vec3(0.98, 0.72, 0.82);
    if (intersectSphere(ro, rd, vec3(4.8, 4.6, -23.0), 1.6, t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = blossomPink; bestHit.specularStrength = 0.15; bestHit.shininess = 12.0; bestHit.reflectivity = 0.0; bestHit.isEmissive = false; }
    }
    if (intersectSphere(ro, rd, vec3(4.1, 5.2, -22.4), 1.2, t, norm))
    {
        if (t < minT) { minT = t; hitSomething = true; bestHit.t = t; bestHit.normal = norm; bestHit.albedo = blossomPink; bestHit.specularStrength = 0.15; bestHit.shininess = 12.0; bestHit.reflectivity = 0.0; bestHit.isEmissive = false; }
    }

    return hitSomething;
}

// Shadow Ray Occlusion Test
bool traceShadow(vec3 ro, vec3 rd, float maxDist)
{
    float t;
    vec3 norm;

    // Fast tests against major occluders
    // Torii Gate Pillars
    if (intersectCylinderY(ro, rd, vec3(-3.0, 0.0, -26.0), 0.35, 6.5, t, norm)) if (t < maxDist) return true;
    if (intersectCylinderY(ro, rd, vec3(3.0, 0.0, -26.0), 0.35, 6.5, t, norm)) if (t < maxDist) return true;

    // Houses
    if (intersectBox(ro, rd, vec3(-10.5, 0.0, 6.0), vec3(-4.5, 4.5, 18.0), t, norm)) if (t < maxDist) return true;
    if (intersectBox(ro, rd, vec3(-10.5, 0.0, -10.0), vec3(-4.5, 4.5, 2.0), t, norm)) if (t < maxDist) return true;
    if (intersectBox(ro, rd, vec3(4.5, 0.0, 6.0), vec3(10.5, 4.5, 18.0), t, norm)) if (t < maxDist) return true;
    if (intersectBox(ro, rd, vec3(4.5, 0.0, -10.0), vec3(10.5, 4.5, 2.0), t, norm)) if (t < maxDist) return true;

    // Stalls
    if (intersectBox(ro, rd, vec3(-5.2, 0.0, 4.5), vec3(-3.2, 1.1, 7.5), t, norm)) if (t < maxDist) return true;
    if (intersectBox(ro, rd, vec3(3.2, 0.0, 4.5), vec3(5.2, 1.1, 7.5), t, norm)) if (t < maxDist) return true;

    // Stage
    if (intersectBox(ro, rd, vec3(-7.3, 0.0, -21.3), vec3(-2.3, 0.45, -16.7), t, norm)) if (t < maxDist) return true;
    if (intersectBox(ro, rd, vec3(-6.8, 0.45, -20.8), vec3(-2.8, 2.65, -20.5), t, norm)) if (t < maxDist) return true;

    return false;
}

// Direct Blinn-Phong Illumination + Shadow Rays
vec3 computeDirectLighting(vec3 hitP, vec3 norm, vec3 viewDir, HitInfo hit)
{
    vec3 result = vec3(0.0);

    // 1. Ambient Term
    vec3 dayAmb = vec3(0.35, 0.38, 0.42);
    vec3 nightAmb = vec3(0.06, 0.08, 0.14);
    vec3 ambient = mix(dayAmb, nightAmb, uNightFactor) * hit.albedo;
    result += ambient;

    vec3 shadowOrig = hitP + norm * 0.003;

    // 2. Directional Sun / Moon Light
    vec3 sunDir = normalize(mix(vec3(0.4, 0.8, 0.5), vec3(-0.3, 0.7, -0.4), uNightFactor));
    vec3 sunCol = mix(vec3(1.0, 0.95, 0.80), vec3(0.20, 0.28, 0.48), uNightFactor);

    if (!traceShadow(shadowOrig, sunDir, 300.0))
    {
        float diff = max(dot(norm, sunDir), 0.0);
        vec3 halfDir = normalize(sunDir + viewDir);
        float spec = pow(max(dot(norm, halfDir), 0.0), hit.shininess) * hit.specularStrength;
        result += (hit.albedo * diff + vec3(spec)) * sunCol * mix(0.9, 0.4, uNightFactor);
    }

    // 3. Dynamic Magic Orb Point Light
    vec3 toOrb = uOrbPos - shadowOrig;
    float distOrb = length(toOrb);
    if (distOrb < 25.0)
    {
        vec3 lDir = toOrb / distOrb;
        if (!traceShadow(shadowOrig, lDir, distOrb))
        {
            float atten = 1.0 / (1.0 + 0.15 * distOrb + 0.06 * distOrb * distOrb);
            float diff = max(dot(norm, lDir), 0.0);
            vec3 halfDir = normalize(lDir + viewDir);
            float spec = pow(max(dot(norm, halfDir), 0.0), hit.shininess) * hit.specularStrength;
            vec3 orbLightCol = vec3(0.25, 0.75, 1.4);
            result += (hit.albedo * diff + vec3(spec)) * orbLightCol * atten * 1.8;
        }
    }

    // 4. Takoyaki & Kakigori Stall Point Lights
    vec3 stallPos[2] = vec3[2](vec3(-3.3, 1.9, 6.0), vec3(3.3, 1.9, 6.0));
    vec3 stallCols[2] = vec3[2](vec3(1.2, 0.6, 0.2), vec3(0.3, 0.9, 1.3));
    for (int i = 0; i < 2; ++i)
    {
        vec3 toStall = stallPos[i] - shadowOrig;
        float distS = length(toStall);
        if (distS < 18.0)
        {
            vec3 lDir = toStall / distS;
            if (!traceShadow(shadowOrig, lDir, distS))
            {
                float atten = 1.0 / (1.0 + 0.25 * distS + 0.10 * distS * distS);
                float diff = max(dot(norm, lDir), 0.0);
                result += hit.albedo * diff * stallCols[i] * atten * 1.2;
            }
        }
    }

    // 5. Fireworks Dynamic Flash
    if (uFireworksActive > 0.01)
    {
        vec3 toFw = uFireworksPos - shadowOrig;
        float distFw = length(toFw);
        vec3 lDir = toFw / distFw;
        if (!traceShadow(shadowOrig, lDir, distFw))
        {
            float atten = 1.0 / (1.0 + 0.03 * distFw + 0.005 * distFw * distFw);
            float diff = max(dot(norm, lDir), 0.0);
            result += hit.albedo * diff * uFireworksColor * atten * uFireworksActive * 2.5;
        }
    }

    return result;
}

void main()
{
    // Reconstruct Primary Ray from Camera view plane
    vec2 ndc = (gl_FragCoord.xy / uResolution) * 2.0 - 1.0;
    float tanHalfFov = tan(radians(uFov) * 0.5);
    vec3 rayDir = normalize(uCamFront + (ndc.x * uAspect * tanHalfFov) * uCamRight + (ndc.y * tanHalfFov) * uCamUp);
    vec3 rayOrig = uCamPos;

    vec3 finalColor = vec3(0.0);
    vec3 throughput = vec3(1.0);
    vec3 curRo = rayOrig;
    vec3 curRd = rayDir;

    // Multi-Bounce Whitted Ray Tracing Loop
    for (int bounce = 0; bounce < 3; ++bounce)
    {
        HitInfo hit;
        if (!traceScene(curRo, curRd, hit))
        {
            // Ray missed all geometry -> Sample Dynamic Sky Dome
            float heightRatio = clamp((curRd.y + 0.2) / 1.2, 0.0, 1.0);
            vec3 daySky = mix(vec3(0.55, 0.75, 0.98), vec3(0.82, 0.88, 0.98), heightRatio);
            vec3 nightSky = mix(vec3(0.02, 0.03, 0.08), vec3(0.06, 0.08, 0.18), heightRatio);
            vec3 skyCol = mix(daySky, nightSky, uNightFactor);

            if (uFireworksActive > 0.01)
            {
                vec3 toFw = normalize(uFireworksPos - curRo);
                float fwDot = max(dot(curRd, toFw), 0.0);
                skyCol += uFireworksColor * pow(fwDot, 24.0) * uFireworksActive * 2.2;
            }

            finalColor += throughput * skyCol;
            break;
        }

        vec3 hitP = curRo + curRd * hit.t;

        if (hit.isEmissive)
        {
            finalColor += throughput * hit.emissiveColor;
            break;
        }

        // Direct Blinn-Phong Lighting with analytical Shadow Rays
        vec3 direct = computeDirectLighting(hitP, hit.normal, -curRd, hit);
        finalColor += throughput * direct;

        // Recursive Specular Reflection
        if (hit.reflectivity > 0.02 && bounce < 2)
        {
            throughput *= (hit.albedo * 0.5 + vec3(0.5)) * hit.reflectivity;
            curRd = reflect(curRd, hit.normal);
            curRo = hitP + hit.normal * 0.003;
        }
        else
        {
            break;
        }
    }

    FragColor = vec4(finalColor, 1.0);
}
