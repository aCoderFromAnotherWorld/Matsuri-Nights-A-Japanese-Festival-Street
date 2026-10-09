#pragma once

#include "SceneNode.h"
#include "Objects.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Texture.h"
#include "TextureGenerator.h"
#include "RayTracer.h"

#include <vector>
#include <memory>
#include <iostream>
#include <iomanip>

struct InspectableObject
{
    std::string displayName;
    std::shared_ptr<SceneNode> node;
    std::string description;
};

class Scene
{
public:
    SceneMeshes meshes;
    std::shared_ptr<SceneNode> rootNode;

    // The 17 Scene Objects
    std::unique_ptr<GroundObject> ground;
    std::vector<std::unique_ptr<MachiyaBuilding>> buildings;
    std::unique_ptr<ToriiGate> toriiGate;
    std::unique_ptr<SakuraTree> sakuraTree;
    std::vector<std::unique_ptr<StreetLanternSpan>> lanternSpans;
    std::vector<LanternObject*> lanterns;
    std::unique_ptr<TakoyakiStall> takoyakiStall;
    std::unique_ptr<KakigoriStall> kakigoriStall;
    std::vector<std::unique_ptr<VendorFigure>> vendors;
    std::unique_ptr<MagicStage> magicStage;
    std::unique_ptr<Magician> magician;
    std::unique_ptr<VanishingBoxTrick> vanishingBox;
    std::unique_ptr<SpotlightRig> spotlightRig;
    std::unique_ptr<AudienceGroup> audience;
    std::unique_ptr<CrowdGroup> crowd;
    std::unique_ptr<FireworkSystem> fireworks;
    std::unique_ptr<SkyDome> skyDome;

    // Phase 2: Lighting & Illumination
    DirLight dirLight;
    std::vector<PointLight> pointLights;
    SpotLight spotLight;
    int shadingMode = 0; // 0 = Blinn-Phong, 1 = Diffuse Only (Lambert), 2 = Ambient Only (Flat)

    // Phase 3: Texturing
    bool enableTextures = true;
    Texture texWood;
    Texture texRoof;
    Texture texStone;
    Texture texLantern;
    Texture texTatami;
    Texture texGold;
    Texture texBark;

    // Day / Night state machine
    float dayNightFactor = 0.0f; // 0.0 = day, 1.0 = night
    bool targetNight = false;
    float dayNightSpeed = 1.5f;

    // Animation control
    bool isPaused = false;
    float totalTime = 0.0f;

    // Live In-Class Inspection & Manual Transform System
    std::vector<InspectableObject> inspectables;
    int selectedIndex = 0;

    // Ray Tracing Engine
    bool rayTracingMode = false;
    std::unique_ptr<Shader> rayTraceShader;
    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;

    Scene()
    {
        meshes.init();
        rootNode = std::make_shared<SceneNode>("World_Root");

        loadTextures();
        buildScene();
        applyTexturesAndMaterials();
        initLighting();
        setupInspectables();
        initRayTracing();
    }

    ~Scene()
    {
        if (quadVAO != 0)
        {
            glDeleteVertexArrays(1, &quadVAO);
            quadVAO = 0;
        }
        if (quadVBO != 0)
        {
            glDeleteBuffers(1, &quadVBO);
            quadVBO = 0;
        }
    }

    void loadTextures()
    {
        TextureGenerator::ensureTextureAssetsExist("assets/textures");

        texWood.loadFromFile("assets/textures/wood_timber.bmp");
        texRoof.loadFromFile("assets/textures/roof_tiles.bmp");
        texStone.loadFromFile("assets/textures/stone_pavement.bmp");
        texLantern.loadFromFile("assets/textures/lantern_paper.bmp");
        texTatami.loadFromFile("assets/textures/tatami_cloth.bmp");
        texGold.loadFromFile("assets/textures/gold_leaf.bmp");
        texBark.loadFromFile("assets/textures/sakura_bark.bmp");
    }

    void buildScene()
    {
        // 1. Ground & Street
        ground = std::make_unique<GroundObject>(meshes);
        rootNode->addChild(ground->root);

        // 2. Machiya Buildings (x4: 2 on Left side, 2 on Right side)
        // Street runs along Z (-35 to +35)
        buildings.push_back(std::make_unique<MachiyaBuilding>(meshes, "Machiya_L1", glm::vec3(-10.5f, 0.0f, 16.0f), 0.0f));
        buildings.push_back(std::make_unique<MachiyaBuilding>(meshes, "Machiya_L2", glm::vec3(-10.5f, 0.0f, -6.0f), 0.0f));
        buildings.push_back(std::make_unique<MachiyaBuilding>(meshes, "Machiya_R1", glm::vec3(10.5f, 0.0f, 16.0f), 180.0f));
        buildings.push_back(std::make_unique<MachiyaBuilding>(meshes, "Machiya_R2", glm::vec3(10.5f, 0.0f, -6.0f), 180.0f));

        for (auto& b : buildings)
            rootNode->addChild(b->root);

        // 3. Torii Gate (at far end of the street)
        toriiGate = std::make_unique<ToriiGate>(meshes, glm::vec3(0.0f, 0.0f, -32.0f));
        rootNode->addChild(toriiGate->root);

        // 4. Sakura Tree (with falling petals)
        sakuraTree = std::make_unique<SakuraTree>(meshes, glm::vec3(-5.8f, 0.0f, -22.0f));
        rootNode->addChild(sakuraTree->root);

        // 5. Street Lantern Spans (Cedar poles on both curbs, sagging catenary ropes, and aligned lanterns)
        // Clear of stalls (Z = 6.0), magic stage (Z in [-21.8, -16.2]), and audience (Z in [-14.5, -12.7])
        float spanZPositions[5] = { 22.0f, 11.0f, 0.0f, -9.0f, -24.5f };
        for (int r = 0; r < 5; ++r)
        {
            float z = spanZPositions[r];
            auto span = std::make_unique<StreetLanternSpan>(meshes, "Span_" + std::to_string(r), z, (float)r * 0.7f);
            for (auto& l : span->lanterns)
                lanterns.push_back(l.get());
            rootNode->addChild(span->root);
            lanternSpans.push_back(std::move(span));
        }

        // 6. Takoyaki Stall (Left side of street, along curb)
        takoyakiStall = std::make_unique<TakoyakiStall>(meshes, glm::vec3(-5.2f, 0.0f, 6.0f), 90.0f);
        rootNode->addChild(takoyakiStall->root);

        // 7. Kakigori Stall (Right side of street, along curb)
        kakigoriStall = std::make_unique<KakigoriStall>(meshes, glm::vec3(5.2f, 0.0f, 6.0f), -90.0f);
        rootNode->addChild(kakigoriStall->root);

        // 8. Vendor Figures (one inside each stall, standing tall on platform behind counter)
        auto vendorTako = std::make_unique<VendorFigure>(meshes, "Vendor_Takoyaki", glm::vec3(-6.05f, 0.0f, 6.0f), 90.0f, glm::vec4(0.18f, 0.35f, 0.75f, 1.0f));
        rootNode->addChild(vendorTako->root);
        vendors.push_back(std::move(vendorTako));

        auto vendorKaki = std::make_unique<VendorFigure>(meshes, "Vendor_Kakigori", glm::vec3(6.05f, 0.0f, 6.0f), -90.0f, glm::vec4(0.85f, 0.25f, 0.22f, 1.0f));
        rootNode->addChild(vendorKaki->root);
        vendors.push_back(std::move(vendorKaki));

        // 9. Magic Show Stage (Right side plaza)
        glm::vec3 stagePos(6.2f, 0.0f, -19.0f);
        magicStage = std::make_unique<MagicStage>(meshes, stagePos);
        rootNode->addChild(magicStage->root);

        // 10 & 11. Magician & Floating Orb (on stage)
        magician = std::make_unique<Magician>(meshes, stagePos + glm::vec3(0.0f, 0.95f, 0.0f));
        rootNode->addChild(magician->root);

        // 12. Vanishing Box Trick (on stage table)
        vanishingBox = std::make_unique<VanishingBoxTrick>(meshes, stagePos);
        rootNode->addChild(vanishingBox->root);

        // 13. Spotlight Rig (mounted next to stage)
        spotlightRig = std::make_unique<SpotlightRig>(meshes, stagePos + glm::vec3(-2.2f, 0.0f, 2.5f));
        rootNode->addChild(spotlightRig->root);

        // 14. Audience Group (seated on festival benches in front of stage facing magician)
        audience = std::make_unique<AudienceGroup>(meshes, stagePos);
        rootNode->addChild(audience->root);

        // 15. Crowd Walking Figures (translating along street)
        crowd = std::make_unique<CrowdGroup>(meshes);
        rootNode->addChild(crowd->root);

        // 16. Fireworks System (night sky bursts)
        fireworks = std::make_unique<FireworkSystem>(meshes);
        rootNode->addChild(fireworks->root);

        // 17. Sky Dome
        skyDome = std::make_unique<SkyDome>(meshes);
        rootNode->addChild(skyDome->root);
    }

    void applyTexturesAndMaterials()
    {
        // 1. Ground & Street pavement
        if (ground && ground->root)
        {
            ground->root->texture = &texStone;
            ground->root->textureTiling = 14.0f;
            ground->root->shininess = 16.0f;
            ground->root->specularStrength = 0.20f;
        }

        // 2. Machiya Townhouse Buildings
        for (auto& b : buildings)
        {
            if (!b || !b->root) continue;
            auto assignBuilding = [&](auto& self, std::shared_ptr<SceneNode> node) -> void {
                if (!node) return;
                if (node->name.find("Roof") != std::string::npos)
                {
                    node->texture = &texRoof;
                    node->textureTiling = 5.0f;
                    node->shininess = 32.0f;
                    node->specularStrength = 0.45f;
                }
                else if (node->name.find("Window") != std::string::npos)
                {
                    node->shininess = 8.0f;
                    node->specularStrength = 0.10f;
                }
                else
                {
                    node->texture = &texWood;
                    node->textureTiling = 2.5f;
                    node->shininess = 16.0f;
                    node->specularStrength = 0.25f;
                }
                for (auto& ch : node->children)
                    self(self, ch);
            };
            assignBuilding(assignBuilding, b->root);
        }

        // 3. Torii Gate
        if (toriiGate && toriiGate->root)
        {
            auto assignTorii = [&](auto& self, std::shared_ptr<SceneNode> node) -> void {
                if (!node) return;
                if (node->name.find("Gakuzuka") != std::string::npos)
                {
                    node->texture = &texGold;
                    node->textureTiling = 1.0f;
                    node->shininess = 48.0f;
                    node->specularStrength = 0.85f;
                }
                else
                {
                    node->texture = &texWood;
                    node->textureTiling = 3.0f;
                    node->shininess = 28.0f;
                    node->specularStrength = 0.35f;
                }
                for (auto& ch : node->children)
                    self(self, ch);
            };
            assignTorii(assignTorii, toriiGate->root);
        }

        // 4. Sakura Blossom Tree
        if (sakuraTree && sakuraTree->root)
        {
            auto assignTree = [&](auto& self, std::shared_ptr<SceneNode> node) -> void {
                if (!node) return;
                if (node->name.find("Trunk") != std::string::npos || node->name.find("Branch") != std::string::npos)
                {
                    node->texture = &texBark;
                    node->textureTiling = 2.0f;
                    node->shininess = 12.0f;
                    node->specularStrength = 0.15f;
                }
                for (auto& ch : node->children)
                    self(self, ch);
            };
            assignTree(assignTree, sakuraTree->root);
        }

        // 5. Street Lantern Spans
        for (auto& span : lanternSpans)
        {
            if (!span || !span->root) continue;
            auto assignSpan = [&](auto& self, std::shared_ptr<SceneNode> node) -> void {
                if (!node) return;
                if (node->name.find("Shaft") != std::string::npos || node->name.find("Peg") != std::string::npos)
                {
                    node->texture = &texWood;
                    node->textureTiling = 3.0f;
                    node->shininess = 20.0f;
                    node->specularStrength = 0.25f;
                }
                else if (node->name.find("Base") != std::string::npos)
                {
                    node->texture = &texStone;
                    node->textureTiling = 2.0f;
                    node->shininess = 16.0f;
                    node->specularStrength = 0.20f;
                }
                else if (node->name.find("Paper") != std::string::npos)
                {
                    node->texture = &texLantern;
                    node->textureTiling = 1.0f;
                    node->shininess = 16.0f;
                    node->specularStrength = 0.30f;
                }
                for (auto& ch : node->children)
                    self(self, ch);
            };
            assignSpan(assignSpan, span->root);
        }

        // 6. Takoyaki Stall
        if (takoyakiStall && takoyakiStall->root)
        {
            auto assignTako = [&](auto& self, std::shared_ptr<SceneNode> node) -> void {
                if (!node) return;
                if (node->name.find("Awning") != std::string::npos || node->name.find("Banner") != std::string::npos)
                {
                    node->texture = &texTatami;
                    node->textureTiling = 3.0f;
                    node->shininess = 12.0f;
                    node->specularStrength = 0.20f;
                }
                else if (node->name.find("Grill") != std::string::npos)
                {
                    node->shininess = 64.0f;
                    node->specularStrength = 0.90f;
                }
                else if (node->name.find("Paper") != std::string::npos)
                {
                    node->texture = &texLantern;
                    node->textureTiling = 1.0f;
                }
                else
                {
                    node->texture = &texWood;
                    node->textureTiling = 2.0f;
                    node->shininess = 20.0f;
                    node->specularStrength = 0.25f;
                }
                for (auto& ch : node->children)
                    self(self, ch);
            };
            assignTako(assignTako, takoyakiStall->root);
        }

        // 7. Kakigori Stall
        if (kakigoriStall && kakigoriStall->root)
        {
            auto assignKaki = [&](auto& self, std::shared_ptr<SceneNode> node) -> void {
                if (!node) return;
                if (node->name.find("Flag") != std::string::npos || node->name.find("Noren") != std::string::npos)
                {
                    node->texture = &texTatami;
                    node->textureTiling = 3.0f;
                    node->shininess = 12.0f;
                    node->specularStrength = 0.20f;
                }
                else if (node->name.find("Wheel") != std::string::npos)
                {
                    node->shininess = 48.0f;
                    node->specularStrength = 0.75f;
                }
                else if (node->name.find("Paper") != std::string::npos)
                {
                    node->texture = &texLantern;
                    node->textureTiling = 1.0f;
                }
                else
                {
                    node->texture = &texWood;
                    node->textureTiling = 2.0f;
                    node->shininess = 20.0f;
                    node->specularStrength = 0.25f;
                }
                for (auto& ch : node->children)
                    self(self, ch);
            };
            assignKaki(assignKaki, kakigoriStall->root);
        }

        // 8. Magic Show Stage & Tricks
        if (magicStage && magicStage->root)
        {
            auto assignStage = [&](auto& self, std::shared_ptr<SceneNode> node) -> void {
                if (!node) return;
                if (node->name.find("Carpet") != std::string::npos)
                {
                    node->texture = &texTatami;
                    node->textureTiling = 4.0f;
                    node->shininess = 10.0f;
                    node->specularStrength = 0.15f;
                }
                else if (node->name.find("Byobu") != std::string::npos)
                {
                    node->texture = &texGold;
                    node->textureTiling = 2.0f;
                    node->shininess = 48.0f;
                    node->specularStrength = 0.85f;
                }
                else
                {
                    node->texture = &texWood;
                    node->textureTiling = 3.0f;
                    node->shininess = 20.0f;
                    node->specularStrength = 0.30f;
                }
                for (auto& ch : node->children)
                    self(self, ch);
            };
            assignStage(assignStage, magicStage->root);
        }

        if (vanishingBox && vanishingBox->root)
        {
            if (vanishingBox->boxNode)
            {
                vanishingBox->boxNode->texture = &texGold;
                vanishingBox->boxNode->textureTiling = 1.0f;
                vanishingBox->boxNode->shininess = 48.0f;
                vanishingBox->boxNode->specularStrength = 0.85f;
            }
            if (vanishingBox->clothNode)
            {
                vanishingBox->clothNode->texture = &texTatami;
                vanishingBox->clothNode->textureTiling = 2.0f;
                vanishingBox->clothNode->shininess = 16.0f;
                vanishingBox->clothNode->specularStrength = 0.30f;
            }
        }

        // 9. Audience benches
        if (audience && audience->root)
        {
            auto assignAud = [&](auto& self, std::shared_ptr<SceneNode> node) -> void {
                if (!node) return;
                if (node->name.find("BenchPlank") != std::string::npos || node->name.find("BenchLeg") != std::string::npos)
                {
                    node->texture = &texWood;
                    node->textureTiling = 2.0f;
                }
                else if (node->name.find("Mousen") != std::string::npos)
                {
                    node->texture = &texTatami;
                    node->textureTiling = 3.0f;
                }
                for (auto& ch : node->children)
                    self(self, ch);
            };
            assignAud(assignAud, audience->root);
        }
    }

    void initLighting()
    {
        pointLights.resize(6);

        // Point Light 0: Magic Orb (Cyan/mystical blue moving light)
        pointLights[0].ambient = glm::vec3(0.05f, 0.10f, 0.15f);
        pointLights[0].diffuse = glm::vec3(0.35f, 0.85f, 1.0f);
        pointLights[0].specular = glm::vec3(0.5f, 0.9f, 1.0f);
        pointLights[0].constant = 1.0f;
        pointLights[0].linear = 0.14f;
        pointLights[0].quadratic = 0.07f;

        // Point Light 1: Takoyaki Stall (Warm golden-amber light under front eaves)
        pointLights[1].position = glm::vec3(-5.2f, 2.85f, 6.0f);
        pointLights[1].ambient = glm::vec3(0.08f, 0.04f, 0.02f);
        pointLights[1].diffuse = glm::vec3(1.0f, 0.60f, 0.22f);
        pointLights[1].specular = glm::vec3(1.0f, 0.70f, 0.30f);
        pointLights[1].constant = 1.0f;
        pointLights[1].linear = 0.10f;
        pointLights[1].quadratic = 0.045f;

        // Point Light 2: Kakigori Stall (Festive cyan light under front eaves)
        pointLights[2].position = glm::vec3(5.2f, 2.85f, 6.0f);
        pointLights[2].ambient = glm::vec3(0.03f, 0.07f, 0.10f);
        pointLights[2].diffuse = glm::vec3(0.35f, 0.90f, 1.0f);
        pointLights[2].specular = glm::vec3(0.5f, 0.95f, 1.0f);
        pointLights[2].constant = 1.0f;
        pointLights[2].linear = 0.10f;
        pointLights[2].quadratic = 0.045f;

        // Point Light 3: Street Overhead Lantern Span 1 (near entrance, Z = 11.0)
        pointLights[3].ambient = glm::vec3(0.06f, 0.03f, 0.01f);
        pointLights[3].diffuse = glm::vec3(1.0f, 0.55f, 0.20f);
        pointLights[3].specular = glm::vec3(1.0f, 0.65f, 0.25f);
        pointLights[3].constant = 1.0f;
        pointLights[3].linear = 0.09f;
        pointLights[3].quadratic = 0.032f;

        // Point Light 4: Street Overhead Lantern Span 3 (near stage/mid, Z = -9.0)
        pointLights[4].ambient = glm::vec3(0.06f, 0.03f, 0.01f);
        pointLights[4].diffuse = glm::vec3(1.0f, 0.55f, 0.20f);
        pointLights[4].specular = glm::vec3(1.0f, 0.65f, 0.25f);
        pointLights[4].constant = 1.0f;
        pointLights[4].linear = 0.09f;
        pointLights[4].quadratic = 0.032f;

        // Point Light 5: Fireworks Sky Flash Light
        pointLights[5].ambient = glm::vec3(0.0f);
        pointLights[5].diffuse = glm::vec3(0.0f);
        pointLights[5].specular = glm::vec3(0.0f);
        pointLights[5].constant = 1.0f;
        pointLights[5].linear = 0.04f;
        pointLights[5].quadratic = 0.009f;

        // Spotlight: Stage tracking spotlight
        spotLight.ambient = glm::vec3(0.05f, 0.05f, 0.04f);
        spotLight.diffuse = glm::vec3(1.5f, 1.35f, 1.1f);
        spotLight.specular = glm::vec3(1.2f, 1.1f, 0.9f);
        spotLight.cutOff = std::cos(glm::radians(15.0f));
        spotLight.outerCutOff = std::cos(glm::radians(23.0f));
        spotLight.constant = 1.0f;
        spotLight.linear = 0.06f;
        spotLight.quadratic = 0.014f;
        spotLight.active = true;
    }

    void updateLighting(float dt)
    {
        // 1. Directional Sun/Moonlight
        glm::vec3 sunDir = glm::normalize(glm::vec3(0.40f, -0.85f, -0.50f));
        glm::vec3 moonDir = glm::normalize(glm::vec3(-0.35f, -0.75f, 0.40f));
        dirLight.direction = glm::normalize(glm::mix(sunDir, moonDir, dayNightFactor));

        dirLight.ambient = glm::mix(glm::vec3(0.38f, 0.36f, 0.32f), glm::vec3(0.07f, 0.09f, 0.16f), dayNightFactor);
        dirLight.diffuse = glm::mix(glm::vec3(0.85f, 0.82f, 0.76f), glm::vec3(0.20f, 0.25f, 0.38f), dayNightFactor);
        dirLight.specular = glm::mix(glm::vec3(0.60f, 0.60f, 0.55f), glm::vec3(0.30f, 0.35f, 0.45f), dayNightFactor);

        // 2. Point Light 0: Magic Orb (tracks orbiting reference frame in real-time)
        if (magician && magician->orbNode)
        {
            pointLights[0].position = magician->orbNode->getWorldPosition();
            float orbIntensity = glm::mix(0.4f, 1.25f, dayNightFactor);
            pointLights[0].diffuse = glm::vec3(0.35f, 0.85f, 1.0f) * orbIntensity;
            pointLights[0].specular = glm::vec3(0.5f, 0.9f, 1.0f) * orbIntensity;
        }

        // 3. Point Lights 1 & 2: Stalls
        float stallNightBoost = glm::mix(0.25f, 1.15f, dayNightFactor);
        pointLights[1].diffuse = glm::vec3(1.0f, 0.60f, 0.22f) * stallNightBoost;
        pointLights[1].specular = glm::vec3(1.0f, 0.70f, 0.30f) * stallNightBoost;

        pointLights[2].diffuse = glm::vec3(0.35f, 0.90f, 1.0f) * stallNightBoost;
        pointLights[2].specular = glm::vec3(0.5f, 0.95f, 1.0f) * stallNightBoost;

        // 4. Point Lights 3 & 4: Street Lanterns (track swinging lantern bodies!)
        float lanternNightBoost = glm::mix(0.20f, 1.20f, dayNightFactor);
        if (lanternSpans.size() > 1 && !lanternSpans[1]->lanterns.empty())
        {
            pointLights[3].position = lanternSpans[1]->lanterns[0]->lanternBody->getWorldPosition();
            pointLights[3].diffuse = glm::vec3(1.0f, 0.55f, 0.20f) * lanternNightBoost;
            pointLights[3].specular = glm::vec3(1.0f, 0.65f, 0.25f) * lanternNightBoost;
        }
        if (lanternSpans.size() > 3 && !lanternSpans[3]->lanterns.empty())
        {
            pointLights[4].position = lanternSpans[3]->lanterns[0]->lanternBody->getWorldPosition();
            pointLights[4].diffuse = glm::vec3(1.0f, 0.55f, 0.20f) * lanternNightBoost;
            pointLights[4].specular = glm::vec3(1.0f, 0.65f, 0.25f) * lanternNightBoost;
        }

        // 5. Point Light 5: Fireworks Sky Flash
        glm::vec3 burstPos, burstColor;
        if (fireworks && fireworks->getActiveBurst(burstPos, burstColor))
        {
            pointLights[5].position = burstPos;
            pointLights[5].diffuse = burstColor * 1.8f;
            pointLights[5].specular = burstColor * 1.5f;
            pointLights[5].active = true;
        }
        else
        {
            pointLights[5].diffuse = glm::vec3(0.0f);
            pointLights[5].specular = glm::vec3(0.0f);
            pointLights[5].active = false;
        }

        // 6. Spotlight: Tracks spotlight housing orientation in real-time
        if (spotlightRig && spotlightRig->lampHousing)
        {
            spotLight.position = spotlightRig->lampHousing->getWorldPosition();
            // Transform local forward vector (+Z) by the housing's world matrix
            glm::vec4 forwardLocal(0.0f, 0.0f, 1.0f, 0.0f);
            glm::vec3 worldDir = glm::normalize(glm::vec3(spotlightRig->lampHousing->worldMatrix * forwardLocal));
            spotLight.direction = worldDir;

            float spotBoost = glm::mix(0.35f, 1.4f, dayNightFactor);
            spotLight.diffuse = glm::vec3(1.5f, 1.35f, 1.1f) * spotBoost;
            spotLight.specular = glm::vec3(1.2f, 1.1f, 0.9f) * spotBoost;
        }
    }

    void setupInspectables()
    {
        inspectables.clear();
        inspectables.push_back({ "1. Lantern [Body] (Child of Swinging Rope Pivot)", lanterns[0]->lanternBody, "Demonstrates transform relative to another object's reference frame" });
        inspectables.push_back({ "2. Lantern [Rope Pivot] (Parent Anchor Node)", lanterns[0]->ropePivot, "Parent node whose local rotation swings child lantern & point light" });
        inspectables.push_back({ "3. Magic Orb (Child of Magician's Hand Bone)", magician->orbNode, "Orb transformed relative to moving hand reference frame with point light" });
        inspectables.push_back({ "4. Magician Figure (Root)", magician->root, "Articulated character rig standing on stage" });
        inspectables.push_back({ "5. Vanishing Box (Scale-to-zero demo)", vanishingBox->boxNode, "Box experiencing scale and translation swap" });
        inspectables.push_back({ "6. Stage Spotlight Housing", spotlightRig->lampHousing, "Cone lamp housing rotating to track magician & aim dynamic spotlight" });
        inspectables.push_back({ "7. Takoyaki Stall (Full Unit)", takoyakiStall->root, "Complex object with spinning/hopping takoyaki" });
        inspectables.push_back({ "8. Torii Gate (Grand Entrance)", toriiGate->root, "Static shrine gate anchor at street terminus" });
        inspectables.push_back({ "9. Sakura Blossom Tree", sakuraTree->root, "Tree with hierarchical branches and falling petals" });
        inspectables.push_back({ "10. Crowd Walker #1", crowd->walkers[0].root, "Figure walking down street with leg cycle" });
    }

    void cycleInspectable(int dir = 1)
    {
        if (inspectables.empty()) return;
        selectedIndex = (selectedIndex + dir + (int)inspectables.size()) % (int)inspectables.size();
        printCurrentInspection();
    }

    void printCurrentInspection() const
    {
        if (selectedIndex < 0 || selectedIndex >= (int)inspectables.size()) return;
        const auto& item = inspectables[selectedIndex];
        std::cout << "\n========================================================" << std::endl;
        std::cout << " [INSPECT SELECTED] " << item.displayName << std::endl;
        std::cout << " Description : " << item.description << std::endl;
        if (item.node)
        {
            const auto& t = item.node->transform;
            std::cout << " Local Pos   : (" << std::fixed << std::setprecision(2) << t.position.x << ", " << t.position.y << ", " << t.position.z << ")" << std::endl;
            std::cout << " Local Rot   : (" << t.rotation.x << ", " << t.rotation.y << ", " << t.rotation.z << ") deg" << std::endl;
            std::cout << " Local Scale : (" << t.scale.x << ", " << t.scale.y << ", " << t.scale.z << ")" << std::endl;
            glm::vec3 worldPos = item.node->getWorldPosition();
            std::cout << " World Pos   : (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ")" << std::endl;
            std::cout << " Parent Node : " << (item.node->parent ? item.node->parent->name : "None (Root)") << std::endl;
        }
        std::cout << " Controls: [I/K] Y-pos, [J/L] X-pos, [U/O] Z-pos | [Arrows] Rot | [+/-] Scale" << std::endl;
        std::cout << "========================================================\n" << std::endl;
    }

    void modifySelectedPosition(const glm::vec3& delta)
    {
        if (selectedIndex >= 0 && selectedIndex < (int)inspectables.size() && inspectables[selectedIndex].node)
        {
            inspectables[selectedIndex].node->transform.position += delta;
            printCurrentInspection();
        }
    }

    void modifySelectedRotation(const glm::vec3& delta)
    {
        if (selectedIndex >= 0 && selectedIndex < (int)inspectables.size() && inspectables[selectedIndex].node)
        {
            inspectables[selectedIndex].node->transform.rotation += delta;
            printCurrentInspection();
        }
    }

    void modifySelectedScale(float factor)
    {
        if (selectedIndex >= 0 && selectedIndex < (int)inspectables.size() && inspectables[selectedIndex].node)
        {
            inspectables[selectedIndex].node->transform.scale *= factor;
            printCurrentInspection();
        }
    }

    void toggleDayNight()
    {
        targetNight = !targetNight;
        std::cout << "[Scene] Day/Night Mode toggled: Target = " << (targetNight ? "Festival Night" : "Daylight") << std::endl;
    }

    void togglePause()
    {
        isPaused = !isPaused;
        std::cout << "[Scene] Animation " << (isPaused ? "PAUSED" : "RESUMED") << std::endl;
    }

    void triggerFirework()
    {
        fireworks->triggerBurstNow();
        std::cout << "[Scene] Manual Firework Launched!" << std::endl;
    }

    void cycleShadingMode()
    {
        shadingMode = (shadingMode + 1) % 3;
        std::cout << "\n========================================================" << std::endl;
        if (shadingMode == 0)
            std::cout << " [SHADING MODE] Blinn-Phong Illumination (Ambient + Diffuse + Specular)" << std::endl;
        else if (shadingMode == 1)
            std::cout << " [SHADING MODE] Diffuse Only (Lambertian - No Specular Highlights)" << std::endl;
        else
            std::cout << " [SHADING MODE] Ambient Only (Flat / Ambient Illumination)" << std::endl;
        std::cout << "========================================================\n" << std::endl;
    }

    void toggleTextures()
    {
        enableTextures = !enableTextures;
        std::cout << "\n========================================================" << std::endl;
        std::cout << " [TEXTURES] Texturing " << (enableTextures ? "ENABLED (Diffuse Texture Maps Active)" : "DISABLED (Showing Clean Material Colors)") << std::endl;
        std::cout << "========================================================\n" << std::endl;
    }

    void update(float dt)
    {
        // Smooth day/night blend transition
        float targetFactor = targetNight ? 1.0f : 0.0f;
        if (dayNightFactor < targetFactor)
        {
            dayNightFactor = std::min(targetFactor, dayNightFactor + dayNightSpeed * dt);
        }
        else if (dayNightFactor > targetFactor)
        {
            dayNightFactor = std::max(targetFactor, dayNightFactor - dayNightSpeed * dt);
        }

        if (!isPaused)
        {
            totalTime += dt;

            // Update all complex moving objects
            sakuraTree->update(totalTime, dt);

            for (auto& span : lanternSpans)
                span->update(totalTime);

            takoyakiStall->update(totalTime, dt);
            kakigoriStall->update(totalTime, dt);

            for (auto& vendor : vendors)
                vendor->update(totalTime);

            magician->update(totalTime);
            vanishingBox->update(dt);
            spotlightRig->update(totalTime);
            audience->update(totalTime);
            crowd->update(totalTime, dt);
            fireworks->update(dt, dayNightFactor > 0.6f);
        }

        // Recursively compute and propagate world matrices across the scene graph
        rootNode->updateWorldMatrix(glm::mat4(1.0f));

        // Update all dynamic light sources (positions, directions, and day/night intensities)
        updateLighting(dt);
    }

    void render(const Shader& shader, const Camera& camera, float aspectRatio)
    {
        shader.use();

        // Global camera view and projection matrices
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix(aspectRatio);

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setVec3("viewPos", camera.Position);
        shader.setFloat("dayNightFactor", dayNightFactor);

        // Phase 2: Shading mode & Lights
        shader.setInt("shadingMode", shadingMode);
        shader.setBool("enableTextures", enableTextures);

        // Directional Light
        shader.setVec3("dirLight.direction", dirLight.direction);
        shader.setVec3("dirLight.ambient", dirLight.ambient);
        shader.setVec3("dirLight.diffuse", dirLight.diffuse);
        shader.setVec3("dirLight.specular", dirLight.specular);

        // Point Lights
        shader.setInt("numActivePointLights", (int)pointLights.size());
        for (size_t i = 0; i < pointLights.size(); ++i)
        {
            std::string prefix = "pointLights[" + std::to_string(i) + "].";
            shader.setVec3(prefix + "position", pointLights[i].position);
            shader.setVec3(prefix + "ambient", pointLights[i].ambient);
            shader.setVec3(prefix + "diffuse", pointLights[i].diffuse);
            shader.setVec3(prefix + "specular", pointLights[i].specular);
            shader.setFloat(prefix + "constant", pointLights[i].constant);
            shader.setFloat(prefix + "linear", pointLights[i].linear);
            shader.setFloat(prefix + "quadratic", pointLights[i].quadratic);
        }

        // Spotlight
        shader.setBool("spotLightActive", spotLight.active);
        shader.setVec3("spotLight.position", spotLight.position);
        shader.setVec3("spotLight.direction", spotLight.direction);
        shader.setVec3("spotLight.ambient", spotLight.ambient);
        shader.setVec3("spotLight.diffuse", spotLight.diffuse);
        shader.setVec3("spotLight.specular", spotLight.specular);
        shader.setFloat("spotLight.cutOff", spotLight.cutOff);
        shader.setFloat("spotLight.outerCutOff", spotLight.outerCutOff);
        shader.setFloat("spotLight.constant", spotLight.constant);
        shader.setFloat("spotLight.linear", spotLight.linear);
        shader.setFloat("spotLight.quadratic", spotLight.quadratic);

        // Traverse scene graph and issue draw calls
        rootNode->draw(shader);
    }

    void initRayTracing()
    {
        // Setup Full-Screen Quad VAO & VBO
        float quadVertices[] = {
            -1.0f,  1.0f,
            -1.0f, -1.0f,
             1.0f, -1.0f,

            -1.0f,  1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f
        };
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glBindVertexArray(0);

        // Load Ray Tracing Shader Program
        rayTraceShader = std::make_unique<Shader>("shaders/raytrace.vert", "shaders/raytrace.frag");
    }

    void toggleRayTracing()
    {
        rayTracingMode = !rayTracingMode;
        std::cout << "\n========================================================" << std::endl;
        if (rayTracingMode)
        {
            std::cout << " [RAY TRACING] Real-Time GPU Ray Tracer: ENABLED (Press 'Z' to toggle)" << std::endl;
            std::cout << " - Primary camera rays traced per-pixel in real-time" << std::endl;
            std::cout << " - Analytical ray-primitive intersections across festival street" << std::endl;
            std::cout << " - Real-time hard shadow rays for all active light sources" << std::endl;
            std::cout << " - Multi-bounce recursive specular mirror reflections (Orb, Stage, Gold Box)" << std::endl;
        }
        else
        {
            std::cout << " [RAY TRACING] Standard Rasterization Pipeline (Blinn-Phong) RESTORED" << std::endl;
        }
        std::cout << "========================================================\n" << std::endl;
    }

    void renderRayTraced(const Camera& camera, int screenWidth, int screenHeight)
    {
        if (!rayTraceShader || quadVAO == 0) return;

        rayTraceShader->use();
        float aspect = (screenHeight > 0) ? (float)screenWidth / (float)screenHeight : 1.0f;

        rayTraceShader->setVec3("uCamPos", camera.Position);
        rayTraceShader->setVec3("uCamFront", camera.Front);
        rayTraceShader->setVec3("uCamUp", camera.Up);
        rayTraceShader->setVec3("uCamRight", camera.Right);
        rayTraceShader->setVec2("uResolution", glm::vec2((float)screenWidth, (float)screenHeight));
        rayTraceShader->setFloat("uFov", camera.Zoom);
        rayTraceShader->setFloat("uAspect", aspect);
        rayTraceShader->setFloat("uTime", totalTime);
        rayTraceShader->setFloat("uNightFactor", dayNightFactor);

        // Dynamic Magic Orb position
        glm::vec3 orbPos = pointLights.empty() ? glm::vec3(-4.0f, 1.8f, -19.0f) : pointLights[0].position;
        rayTraceShader->setVec3("uOrbPos", orbPos);

        // Dynamic Vanishing Box
        if (vanishingBox && vanishingBox->boxNode)
        {
            rayTraceShader->setVec3("uBoxPos", vanishingBox->boxNode->getWorldPosition());
            rayTraceShader->setVec3("uBoxScale", vanishingBox->boxNode->transform.scale);
        }
        else
        {
            rayTraceShader->setVec3("uBoxPos", glm::vec3(-4.8f, 1.45f, -19.0f));
            rayTraceShader->setVec3("uBoxScale", glm::vec3(1.0f));
        }

        // Spotlight
        rayTraceShader->setVec3("uSpotPos", spotLight.position);
        rayTraceShader->setVec3("uSpotDir", spotLight.direction);

        // Fireworks
        glm::vec3 fwPos(0.0f);
        glm::vec3 fwCol(1.0f);
        bool fwActive = fireworks && fireworks->getActiveBurst(fwPos, fwCol);
        rayTraceShader->setVec3("uFireworksPos", fwPos);
        rayTraceShader->setVec3("uFireworksColor", fwCol);
        rayTraceShader->setFloat("uFireworksActive", fwActive ? 1.0f : 0.0f);

        rayTraceShader->setInt("uMaxBounces", 3);

        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);
    }

    void captureCPURayTracedSnapshot(const Camera& camera, int width = 1280, int height = 720, const std::string& filename = "raytraced_snapshot.bmp")
    {
        CPU_RayTracer::SceneSnapshotData snapData;
        snapData.dayNightFactor = dayNightFactor;
        snapData.orbPos = pointLights.empty() ? glm::vec3(-4.0f, 1.8f, -19.0f) : pointLights[0].position;
        if (vanishingBox && vanishingBox->boxNode)
        {
            snapData.boxPos = vanishingBox->boxNode->getWorldPosition();
            snapData.boxScale = vanishingBox->boxNode->transform.scale;
        }
        snapData.spotPos = spotLight.position;
        snapData.spotDir = spotLight.direction;

        glm::vec3 fwPos(0.0f);
        glm::vec3 fwCol(1.0f);
        if (fireworks && fireworks->getActiveBurst(fwPos, fwCol))
        {
            snapData.fireworkPos = fwPos;
            snapData.fireworkColor = fwCol;
            snapData.fireworkActive = 1.0f;
        }

        CPU_RayTracer::renderSnapshot(camera, width, height, snapData, filename, 3);
    }
};
