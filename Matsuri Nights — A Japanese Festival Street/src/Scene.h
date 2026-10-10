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
    std::vector<std::unique_ptr<SakuraTree>> sakuraTrees;
    SakuraTree* sakuraTree = nullptr;
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
    Texture texTakoyaki;

    // Interactive Accessibility & Wall Collision System
    bool collisionEnabled = true;

    // Day / Night state machine
    float dayNightFactor = 0.0f; // 0.0 = day, 1.0 = night
    bool targetNight = false;
    float dayNightSpeed = 1.5f;

    // Lantern & Stall lighting control (Key L)
    bool lanternLightsOn = true;

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

    // Realistic Shadows: Directional Depth FBO & Texture
    const unsigned int SHADOW_WIDTH = 2048;
    const unsigned int SHADOW_HEIGHT = 2048;
    unsigned int depthMapFBO = 0;
    unsigned int depthMap = 0;
    std::unique_ptr<Shader> shadowDepthShader;
    bool enableShadows = true;
    glm::mat4 lightSpaceMatrix{ 1.0f };

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
        initShadows();
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
        if (depthMapFBO != 0)
        {
            glDeleteFramebuffers(1, &depthMapFBO);
            depthMapFBO = 0;
        }
        if (depthMap != 0)
        {
            glDeleteTextures(1, &depthMap);
            depthMap = 0;
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
        texTakoyaki.loadFromFile("assets/textures/takoyaki_food.bmp");
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

        // 4. Diverse Cherry Trees (populated on both sides of roads and in the center gaps of Machiya houses)
        struct TreePlacement {
            glm::vec3 pos;
            float scale;
            float rotY;
            float leanAngle;
            float blossomTone;
        };
        std::vector<TreePlacement> treeConfigs = {
            // 1. Grand Shrine Sakura Tree (West side plaza lawn, outside road)
            { glm::vec3(-10.2f, 0.0f, -22.0f), 1.15f,  15.0f,  0.0f,  0.00f },
            // 2. Festival Stage / Plaza Sakura Tree (East side plaza lawn, outside road)
            { glm::vec3( 10.2f, 0.0f, -26.5f), 1.05f, 135.0f, -1.5f,  0.25f },
            // 3. Left Courtyard Garden Gap (between Machiya L1 & L2, outside road)
            { glm::vec3(-10.8f, 0.0f,   5.0f), 0.95f,  75.0f,  1.2f, -0.20f },
            // 4. Right Courtyard Garden Gap (between Machiya R1 & R2, outside road)
            { glm::vec3( 10.8f, 0.0f,   5.0f), 0.95f, 210.0f, -1.0f,  0.30f },
            // 5. South Entrance Avenue West (framing street entrance lawn, outside road)
            { glm::vec3(-10.0f, 0.0f,  28.0f), 0.90f,  40.0f,  1.2f, -0.15f },
            // 6. South Entrance Avenue East (framing street entrance lawn, outside road)
            { glm::vec3( 10.0f, 0.0f,  27.0f), 0.92f, 190.0f, -1.2f,  0.15f },
            // 7. North Torii Sacred Grove Sakura Tree (West of Torii gate lawn, outside road)
            { glm::vec3(-10.2f, 0.0f, -33.5f), 0.85f, 290.0f,  0.5f, -0.10f }
        };

        for (const auto& tc : treeConfigs)
        {
            auto tree = std::make_unique<SakuraTree>(meshes, tc.pos, tc.scale, tc.rotY, tc.leanAngle, tc.blossomTone);
            rootNode->addChild(tree->root);
            sakuraTrees.push_back(std::move(tree));
        }
        sakuraTree = sakuraTrees[0].get();

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

        // 8. Vendor Figures (one inside each stall, standing tall on platform behind counter facing towards road)
        auto vendorTako = std::make_unique<VendorFigure>(meshes, "Vendor_Takoyaki", glm::vec3(-6.65f, 0.0f, 6.0f), 90.0f, glm::vec4(0.18f, 0.35f, 0.75f, 1.0f));
        rootNode->addChild(vendorTako->root);
        vendors.push_back(std::move(vendorTako));

        auto vendorKaki = std::make_unique<VendorFigure>(meshes, "Vendor_Kakigori", glm::vec3(6.65f, 0.0f, 6.0f), -90.0f, glm::vec4(0.85f, 0.25f, 0.22f, 1.0f));
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
                if (node->name.find("Roof") != std::string::npos || node->name.find("Eaves") != std::string::npos)
                {
                    node->texture = &texRoof;
                    node->textureTiling = 5.0f;
                    node->shininess = 32.0f;
                    node->specularStrength = 0.45f;
                }
                else if (node->name.find("Tatami") != std::string::npos)
                {
                    node->texture = &texTatami;
                    node->textureTiling = 4.0f;
                    node->shininess = 16.0f;
                    node->specularStrength = 0.15f;
                }
                else if (node->name.find("Stone") != std::string::npos || node->name.find("Genkan") != std::string::npos)
                {
                    node->texture = &texStone;
                    node->textureTiling = 2.0f;
                    node->shininess = 20.0f;
                    node->specularStrength = 0.20f;
                }
                else if (node->name.find("Flower") != std::string::npos ||
                         node->name.find("Bloom") != std::string::npos ||
                         node->name.find("Petal") != std::string::npos ||
                         node->name.find("Leaf") != std::string::npos ||
                         node->name.find("Leaves") != std::string::npos ||
                         node->name.find("Calyx") != std::string::npos ||
                         node->name.find("Stem") != std::string::npos ||
                         node->name.find("Branch") != std::string::npos ||
                         node->name.find("Moss") != std::string::npos ||
                         node->name.find("Vine") != std::string::npos ||
                         node->name.find("Stamen") != std::string::npos ||
                         node->name.find("Foliage") != std::string::npos ||
                         node->name.find("Soil") != std::string::npos)
                {
                    node->texture = nullptr;
                    node->shininess = 12.0f;
                    node->specularStrength = 0.15f;
                }
                else if (node->name.find("Vase") != std::string::npos || node->name.find("Pot") != std::string::npos)
                {
                    node->texture = nullptr;
                    node->shininess = 64.0f;
                    node->specularStrength = 0.80f;
                }
                else if (node->name.find("Byoubu") != std::string::npos)
                {
                    node->texture = &texGold;
                    node->textureTiling = 1.0f;
                    node->shininess = 48.0f;
                    node->specularStrength = 0.70f;
                }
                else if (node->name.find("Paper") != std::string::npos || node->name.find("Andon") != std::string::npos || node->name.find("Lamp") != std::string::npos || node->name.find("Shade") != std::string::npos || node->isWindow || node->isEmissive)
                {
                    node->texture = nullptr;
                    node->shininess = 8.0f;
                    node->specularStrength = 0.10f;
                }
                else if (node->name.find("Zabuton") != std::string::npos || node->name.find("Futon") != std::string::npos)
                {
                    node->texture = &texTatami;
                    node->textureTiling = 2.0f;
                    node->shininess = 14.0f;
                    node->specularStrength = 0.20f;
                }
                else
                {
                    node->texture = &texWood;
                    node->textureTiling = 2.5f;
                    node->shininess = 20.0f;
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

        // 4. Sakura Blossom Trees
        auto assignTree = [&](auto& self, std::shared_ptr<SceneNode> node) -> void {
            if (!node) return;
            if (node->name.find("Trunk") != std::string::npos || node->name.find("Branch") != std::string::npos || node->name.find("Bough") != std::string::npos || node->name.find("Root") != std::string::npos)
            {
                node->texture = &texBark;
                node->textureTiling = 2.0f;
                node->shininess = 12.0f;
                node->specularStrength = 0.15f;
            }
            else if (node->name.find("Blossom") != std::string::npos || node->name.find("Petal") != std::string::npos || node->name.find("Leaf") != std::string::npos)
            {
                node->texture = nullptr;
            }
            for (auto& ch : node->children)
                self(self, ch);
        };
        for (auto& tree : sakuraTrees)
        {
            if (tree && tree->root)
                assignTree(assignTree, tree->root);
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
                else if (node->name.find("TakoBall") != std::string::npos)
                {
                    node->texture = &texTakoyaki;
                    node->textureTiling = 1.0f;
                    node->shininess = 64.0f;
                    node->specularStrength = 0.70f;
                }
                else if (node->name.find("TakoSauce") != std::string::npos || node->name.find("TakoMayo") != std::string::npos || node->name.find("TakoAonori") != std::string::npos)
                {
                    node->texture = nullptr;
                    node->shininess = 72.0f;
                    node->specularStrength = 0.85f;
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
                else if (node->name.find("Wheel") != std::string::npos || node->name.find("Handle") != std::string::npos)
                {
                    node->texture = nullptr;
                    node->shininess = 48.0f;
                    node->specularStrength = 0.75f;
                }
                else if (node->name.find("Paper") != std::string::npos)
                {
                    node->texture = &texLantern;
                    node->textureTiling = 1.0f;
                }
                else if (node->name.find("Ice") != std::string::npos ||
                         node->name.find("Bowl") != std::string::npos ||
                         node->name.find("Cup") != std::string::npos ||
                         node->name.find("Syrup") != std::string::npos ||
                         node->name.find("Milk") != std::string::npos ||
                         node->name.find("Spoon") != std::string::npos ||
                         node->name.find("Garnish") != std::string::npos ||
                         node->name.find("Topping") != std::string::npos ||
                         node->name.find("Bottle") != std::string::npos ||
                         node->name.find("Pump") != std::string::npos ||
                         node->name.find("Umbrella") != std::string::npos ||
                         node->name.find("Berry") != std::string::npos ||
                         node->name.find("Bean") != std::string::npos ||
                         node->name.find("Mango") != std::string::npos ||
                         node->name.find("Grape") != std::string::npos ||
                         node->name.find("Melon") != std::string::npos ||
                         node->name.find("Stem") != std::string::npos ||
                         node->name.find("Shaver") != std::string::npos ||
                         node->name.find("Tray") != std::string::npos ||
                         node->name.find("Urushi") != std::string::npos ||
                         node->name.find("Kakigori") != std::string::npos)
                {
                    node->texture = nullptr;
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
        pointLights.resize(12);

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

        // Point Light 6: Machiya_L1 Ground Floor Living Room Lamp
        pointLights[6].position = glm::vec3(-10.8f, 2.2f, 17.1f);
        pointLights[6].ambient = glm::vec3(0.12f, 0.09f, 0.05f);
        pointLights[6].diffuse = glm::vec3(1.45f, 1.20f, 0.75f);
        pointLights[6].specular = glm::vec3(0.9f, 0.8f, 0.5f);
        pointLights[6].constant = 1.0f;
        pointLights[6].linear = 0.08f;
        pointLights[6].quadratic = 0.022f;

        // Point Light 7: Machiya_L1 Second Floor Bedroom Lamp
        pointLights[7].position = glm::vec3(-11.0f, 6.0f, 17.0f);
        pointLights[7].ambient = glm::vec3(0.10f, 0.08f, 0.05f);
        pointLights[7].diffuse = glm::vec3(1.35f, 1.10f, 0.70f);
        pointLights[7].specular = glm::vec3(0.8f, 0.7f, 0.45f);
        pointLights[7].constant = 1.0f;
        pointLights[7].linear = 0.08f;
        pointLights[7].quadratic = 0.022f;

        // Point Light 8: Machiya_R1 Ground Floor Living Room Lamp
        pointLights[8].position = glm::vec3(10.8f, 2.2f, 14.9f);
        pointLights[8].ambient = glm::vec3(0.12f, 0.09f, 0.05f);
        pointLights[8].diffuse = glm::vec3(1.45f, 1.20f, 0.75f);
        pointLights[8].specular = glm::vec3(0.9f, 0.8f, 0.5f);
        pointLights[8].constant = 1.0f;
        pointLights[8].linear = 0.08f;
        pointLights[8].quadratic = 0.022f;

        // Point Light 9: Machiya_R1 Second Floor Bedroom Lamp
        pointLights[9].position = glm::vec3(11.0f, 6.0f, 15.0f);
        pointLights[9].ambient = glm::vec3(0.10f, 0.08f, 0.05f);
        pointLights[9].diffuse = glm::vec3(1.35f, 1.10f, 0.70f);
        pointLights[9].specular = glm::vec3(0.8f, 0.7f, 0.45f);
        pointLights[9].constant = 1.0f;
        pointLights[9].linear = 0.08f;
        pointLights[9].quadratic = 0.022f;

        // Point Light 10: Machiya_L2 Ground Floor Living Room Lamp
        pointLights[10].position = glm::vec3(-10.8f, 2.2f, -4.9f);
        pointLights[10].ambient = glm::vec3(0.10f, 0.08f, 0.05f);
        pointLights[10].diffuse = glm::vec3(1.35f, 1.10f, 0.70f);
        pointLights[10].specular = glm::vec3(0.8f, 0.7f, 0.45f);
        pointLights[10].constant = 1.0f;
        pointLights[10].linear = 0.08f;
        pointLights[10].quadratic = 0.022f;

        // Point Light 11: Machiya_R2 Ground Floor Living Room Lamp
        pointLights[11].position = glm::vec3(10.8f, 2.2f, -7.1f);
        pointLights[11].ambient = glm::vec3(0.10f, 0.08f, 0.05f);
        pointLights[11].diffuse = glm::vec3(1.35f, 1.10f, 0.70f);
        pointLights[11].specular = glm::vec3(0.8f, 0.7f, 0.45f);
        pointLights[11].constant = 1.0f;
        pointLights[11].linear = 0.08f;
        pointLights[11].quadratic = 0.022f;

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

        dirLight.ambient = glm::mix(glm::vec3(0.42f, 0.40f, 0.36f), glm::vec3(0.12f, 0.14f, 0.22f), dayNightFactor);
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
        float lightScale = lanternLightsOn ? 1.0f : 0.05f;
        float stallNightBoost = glm::mix(0.25f, 1.15f, dayNightFactor) * lightScale;
        pointLights[1].diffuse = glm::vec3(1.0f, 0.60f, 0.22f) * stallNightBoost;
        pointLights[1].specular = glm::vec3(1.0f, 0.70f, 0.30f) * stallNightBoost;

        pointLights[2].diffuse = glm::vec3(0.35f, 0.90f, 1.0f) * stallNightBoost;
        pointLights[2].specular = glm::vec3(0.5f, 0.95f, 1.0f) * stallNightBoost;

        // 4. Point Lights 3 & 4: Street Lanterns (track swinging lantern bodies!)
        float lanternNightBoost = glm::mix(0.20f, 1.20f, dayNightFactor) * lightScale;
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

        // 6. House Interior Room Point Lights (warm daylight fill, glowing radiant amber at night)
        float roomLightScale = glm::mix(0.85f, 1.45f, dayNightFactor);
        pointLights[6].diffuse = glm::vec3(1.45f, 1.20f, 0.75f) * roomLightScale;
        pointLights[7].diffuse = glm::vec3(1.35f, 1.10f, 0.70f) * roomLightScale;
        pointLights[8].diffuse = glm::vec3(1.45f, 1.20f, 0.75f) * roomLightScale;
        pointLights[9].diffuse = glm::vec3(1.35f, 1.10f, 0.70f) * roomLightScale;
        pointLights[10].diffuse = glm::vec3(1.35f, 1.10f, 0.70f) * roomLightScale;
        pointLights[11].diffuse = glm::vec3(1.35f, 1.10f, 0.70f) * roomLightScale;

        // 7. Spotlight: Tracks spotlight housing orientation in real-time
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
        inspectables.push_back({ "8. Kakigori Stall (Full Unit)", kakigoriStall->root, "Festive stall with spinning/hopping Kakigori dessert bowls & shaved ice machine" });
        inspectables.push_back({ "9. Torii Gate (Grand Entrance)", toriiGate->root, "Static shrine gate anchor at street terminus" });
        inspectables.push_back({ "10. Sakura Blossom Tree", sakuraTree->root, "Tree with hierarchical branches and falling petals" });
        inspectables.push_back({ "11. Crowd Walker #1", crowd->walkers[0].root, "Figure walking down street with leg cycle" });
        if (buildings.size() >= 4)
        {
            inspectables.push_back({ "12. Machiya_L1 (Townhouse Building)", buildings[0]->root, "Traditional townhouse with sliding Shoji door and windows" });
            inspectables.push_back({ "13. Machiya_L2 (Townhouse Building)", buildings[1]->root, "Traditional townhouse with sliding Shoji door and windows" });
            inspectables.push_back({ "14. Machiya_R1 (Townhouse Building)", buildings[2]->root, "Traditional townhouse with sliding Shoji door and windows" });
            inspectables.push_back({ "15. Machiya_R2 (Townhouse Building)", buildings[3]->root, "Traditional townhouse with sliding Shoji door and windows" });
        }
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

    void toggleLanternLights()
    {
        lanternLightsOn = !lanternLightsOn;
        for (auto& l : lanterns)
        {
            if (l && l->lanternBody)
            {
                for (auto& ch : l->lanternBody->children)
                {
                    if (ch && ch->name.find("Paper") != std::string::npos)
                    {
                        ch->isEmissive = lanternLightsOn;
                    }
                }
            }
        }
        std::cout << "\n========================================================" << std::endl;
        std::cout << " [LIGHTS] Lantern & Stall Illuminations: " << (lanternLightsOn ? "LIT (Active Glow)" : "OFF (Dimmed)") << std::endl;
        std::cout << "========================================================\n" << std::endl;
    }

    void replayMagicTrick()
    {
        if (vanishingBox)
        {
            vanishingBox->stateTimer = 0.0f;
        }
        std::cout << "\n========================================================" << std::endl;
        std::cout << " [MAGIC SHOW] Magic Show Trick Sequence RESTARTED from Phase 1!" << std::endl;
        std::cout << "========================================================\n" << std::endl;
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
            for (auto& tree : sakuraTrees)
                tree->update(totalTime, dt);

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

            for (auto& bld : buildings)
            {
                if (bld) bld->update(dt);
            }
        }

        // Recursively compute and propagate world matrices across the scene graph
        rootNode->updateWorldMatrix(glm::mat4(1.0f));

        // Update all dynamic light sources (positions, directions, and day/night intensities)
        updateLighting(dt);
    }

    void interactNearestDoor(const glm::vec3& playerPos)
    {
        float minDist = 999.0f;
        MachiyaBuilding* nearestBld = nullptr;

        for (auto& bld : buildings)
        {
            if (!bld) continue;
            glm::vec3 doorLocal(4.14f, 1.2f, -0.90f);
            glm::vec3 doorWorld;
            if (std::abs(bld->rotationY - 180.0f) < 1.0f)
                doorWorld = bld->worldPos + glm::vec3(-doorLocal.x, doorLocal.y, -doorLocal.z);
            else
                doorWorld = bld->worldPos + doorLocal;

            float d = glm::distance(playerPos, doorWorld);
            if (d < minDist)
            {
                minDist = d;
                nearestBld = bld.get();
            }
        }

        if (nearestBld && minDist < 5.0f)
        {
            nearestBld->toggleDoor();
            std::cout << "\n========================================================" << std::endl;
            std::cout << " [DOOR INTERACTION] " << (nearestBld->isDoorOpen ? "Slid OPEN" : "Slid CLOSED")
                      << " front Shoji door of " << nearestBld->root->name << std::endl;
            std::cout << "========================================================\n" << std::endl;
        }
        else
        {
            std::cout << "\n[DOOR INTERACTION] Walk closer to a house entrance (within 5m) and press [H] to interact.\n" << std::endl;
        }
    }

    void interactNearestWindow(const glm::vec3& playerPos)
    {
        float minDist = 999.0f;
        MachiyaBuilding* nearestBld = nullptr;

        for (auto& bld : buildings)
        {
            if (!bld) continue;
            float d = glm::distance(playerPos, bld->worldPos);
            if (d < minDist)
            {
                minDist = d;
                nearestBld = bld.get();
            }
        }

        if (nearestBld && minDist < 14.0f)
        {
            nearestBld->toggleWindows();
            std::cout << "\n========================================================" << std::endl;
            std::cout << " [WINDOW INTERACTION] " << (nearestBld->isWindowOpen ? "Slid OPEN" : "Slid CLOSED")
                      << " sliding Shoji windows of " << nearestBld->root->name << std::endl;
            std::cout << "========================================================\n" << std::endl;
        }
        else
        {
            std::cout << "\n[WINDOW INTERACTION] Walk closer to a house (within 14m) and press [G] to slide windows.\n" << std::endl;
        }
    }

    void toggleCollision()
    {
        collisionEnabled = !collisionEnabled;
        std::cout << "\n========================================================" << std::endl;
        std::cout << " [WALL COLLISION] " << (collisionEnabled ? "ENABLED (Walk Mode: Solid walls & stairs active, no passing walls)" : "DISABLED (Noclip Fly Mode)") << std::endl;
        std::cout << "========================================================\n" << std::endl;
    }

    glm::vec3 resolveCollision(const glm::vec3& oldPos, const glm::vec3& newPos)
    {
        if (!collisionEnabled)
            return newPos;

        glm::vec3 pos = newPos;
        // Never allow sinking below ground
        pos.y = std::max(pos.y, 0.45f);

        const float radius = 0.35f;

        for (auto& bld : buildings)
        {
            if (!bld) continue;

            glm::vec3 bPos = bld->worldPos;
            float rotY = bld->rotationY;
            bool isRot180 = (std::abs(rotY - 180.0f) < 1.0f);

            auto toLocal = [&](const glm::vec3& wp) -> glm::vec3 {
                glm::vec3 rel = wp - bPos;
                if (isRot180)
                    return glm::vec3(-rel.x, rel.y, -rel.z);
                return rel;
            };

            auto toWorld = [&](const glm::vec3& lp) -> glm::vec3 {
                if (isRot180)
                    return bPos + glm::vec3(-lp.x, lp.y, -lp.z);
                return bPos + lp;
            };

            glm::vec3 oldL = toLocal(oldPos);
            glm::vec3 newL = toLocal(pos);

            // Broadphase check: building footprint is [-4.2, 4.2] x [-4.7, 4.7]
            if (std::abs(newL.x) > 5.8f || std::abs(newL.z) > 6.2f || newL.y > 9.8f)
                continue;

            const float bMinX = -4.0f, bMaxX = 4.0f;
            const float bMinZ = -4.5f, bMaxZ = 4.5f;

            // 1. Back Wall (X = -4.0)
            if (newL.z >= bMinZ && newL.z <= bMaxZ && newL.y <= 8.2f)
            {
                if (oldL.x <= bMinX && newL.x > bMinX - radius)
                    newL.x = bMinX - radius;
                else if (oldL.x >= bMinX && newL.x < bMinX + radius)
                    newL.x = bMinX + radius;
            }

            // 2. Left Wall (Z = +4.5)
            if (newL.x >= bMinX && newL.x <= bMaxX && newL.y <= 8.2f)
            {
                if (oldL.z >= bMaxZ && newL.z < bMaxZ + radius)
                    newL.z = bMaxZ + radius;
                else if (oldL.z <= bMaxZ && newL.z > bMaxZ - radius)
                    newL.z = bMaxZ - radius;
            }

            // 3. Right Wall (Z = -4.5)
            if (newL.x >= bMinX && newL.x <= bMaxX && newL.y <= 8.2f)
            {
                if (oldL.z <= bMinZ && newL.z > bMinZ - radius)
                    newL.z = bMinZ - radius;
                else if (oldL.z >= bMinZ && newL.z < bMinZ + radius)
                    newL.z = bMinZ + radius;
            }

            // 4. Front Wall (X = +4.0) with Doorway portal
            if (newL.z >= bMinZ && newL.z <= bMaxZ && newL.y <= 8.2f)
            {
                const float doorMinZ = -2.05f;
                const float doorMaxZ = 0.25f;
                bool inDoorwayH = (newL.z >= doorMinZ + radius && newL.z <= doorMaxZ - radius);
                bool inDoorwayV = (newL.y >= 0.0f && newL.y <= 2.55f);
                bool canPassDoor = inDoorwayH && inDoorwayV && bld->isDoorOpen;

                // Crossing from street into house
                if (oldL.x >= bMaxX && newL.x < bMaxX + radius)
                {
                    if (!canPassDoor)
                        newL.x = bMaxX + radius;
                }
                // Crossing from house onto street
                else if (oldL.x <= bMaxX && newL.x > bMaxX - radius)
                {
                    if (!canPassDoor)
                        newL.x = bMaxX - radius;
                }
            }

            // 5. Interior Floors, Ceiling & 14-Step Staircase (when inside footprint)
            if (newL.x > bMinX && newL.x < bMaxX && newL.z > bMinZ && newL.z < bMaxZ)
            {
                // Staircase zone: X in [-2.00, +0.85], Z in [-4.15, -2.85]
                if (newL.x >= -2.00f && newL.x <= 0.85f && newL.z >= -4.15f && newL.z <= -2.85f && newL.y <= 6.2f)
                {
                    float stairT = (0.75f - newL.x) / 2.70f;
                    stairT = std::clamp(stairT, 0.0f, 1.0f);
                    float stairFloorY = glm::mix(0.20f, 4.30f, stairT);
                    float targetEyeY = stairFloorY + 1.45f;
                    newL.y = targetEyeY; // Natural stair climbing kinematics (steps up & down smoothly!)
                }
                // Second Floor
                else if (newL.y >= 3.6f)
                {
                    // Guardrail collision along open bedroom stairwell edge (Z = -2.85m, between X = -1.95m and X = +0.75m)
                    if (oldL.z >= -2.85f && newL.z < -2.85f + radius && newL.x >= -1.95f && newL.x <= 0.75f)
                    {
                        newL.z = -2.85f + radius; // Solid guardrail stops player from falling off bedroom edge
                    }

                    // Guardrail collision across the front end of the stairwell (X = +0.75m, between Z = -4.15m and Z = -2.85m)
                    if (oldL.x >= 0.75f && newL.x < 0.75f + radius && newL.z >= -4.15f && newL.z <= -2.85f)
                    {
                        newL.x = 0.75f + radius; // Stops player from walking into stairwell from front floor area
                    }

                    // Stairwell vertical opening is strictly between X = -1.95m and +0.75m
                    bool inStairwellShaft = (newL.x >= -1.95f && newL.x <= 0.75f && newL.z >= -4.15f && newL.z <= -2.85f);
                    if (!inStairwellShaft)
                    {
                        newL.y = std::max(newL.y, 4.30f + 1.45f); // 5.75m standing height on 2nd floor tatami & landing gallery
                    }
                    newL.y = std::min(newL.y, 7.80f);
                }
                else
                {
                    // Ground floor inside: standing height
                    newL.y = std::max(newL.y, 1.65f);
                }
            }

            pos = toWorld(newL);
        }

        return pos;
    }

    void initShadows()
    {
        glGenFramebuffers(1, &depthMapFBO);
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        shadowDepthShader = std::make_unique<Shader>("shaders/shadow_depth.vert", "shaders/shadow_depth.frag");
    }

    void toggleShadows()
    {
        enableShadows = !enableShadows;
        std::cout << "\n========================================================" << std::endl;
        std::cout << " [SHADOWS] Realistic PCF Soft Shadow Mapping "
                  << (enableShadows ? "ENABLED (16-sample PCF filter active)" : "DISABLED (Flat unshadowed lighting)") << std::endl;
        std::cout << "========================================================\n" << std::endl;
    }

    struct LightUniformLocations
    {
        bool initialized = false;
        GLint view = -1;
        GLint projection = -1;
        GLint viewPos = -1;
        GLint dayNightFactor = -1;
        GLint totalTime = -1;
        GLint shadingMode = -1;
        GLint enableTextures = -1;
        GLint enableShadows = -1;
        GLint lightSpaceMatrix = -1;
        GLint shadowMap = -1;

        GLint dirLightDir = -1;
        GLint dirLightAmb = -1;
        GLint dirLightDiff = -1;
        GLint dirLightSpec = -1;

        GLint numActivePointLights = -1;
        struct PointLightLocs {
            GLint pos = -1, amb = -1, diff = -1, spec = -1;
            GLint constant = -1, linear = -1, quadratic = -1;
        };
        std::vector<PointLightLocs> pointLights;

        GLint spotActive = -1, spotPos = -1, spotDir = -1, spotAmb = -1, spotDiff = -1, spotSpec = -1;
        GLint spotCutOff = -1, spotOuterCutOff = -1, spotConstant = -1, spotLinear = -1, spotQuadratic = -1;

        void init(const Shader& shader, size_t numLights)
        {
            view = shader.getUniformLocation("view");
            projection = shader.getUniformLocation("projection");
            viewPos = shader.getUniformLocation("viewPos");
            dayNightFactor = shader.getUniformLocation("dayNightFactor");
            totalTime = shader.getUniformLocation("totalTime");
            shadingMode = shader.getUniformLocation("shadingMode");
            enableTextures = shader.getUniformLocation("enableTextures");
            enableShadows = shader.getUniformLocation("enableShadows");
            lightSpaceMatrix = shader.getUniformLocation("lightSpaceMatrix");
            shadowMap = shader.getUniformLocation("shadowMap");

            dirLightDir = shader.getUniformLocation("dirLight.direction");
            dirLightAmb = shader.getUniformLocation("dirLight.ambient");
            dirLightDiff = shader.getUniformLocation("dirLight.diffuse");
            dirLightSpec = shader.getUniformLocation("dirLight.specular");

            numActivePointLights = shader.getUniformLocation("numActivePointLights");
            pointLights.resize(numLights);
            for (size_t i = 0; i < numLights; ++i)
            {
                std::string prefix = "pointLights[" + std::to_string(i) + "].";
                pointLights[i].pos = shader.getUniformLocation(prefix + "position");
                pointLights[i].amb = shader.getUniformLocation(prefix + "ambient");
                pointLights[i].diff = shader.getUniformLocation(prefix + "diffuse");
                pointLights[i].spec = shader.getUniformLocation(prefix + "specular");
                pointLights[i].constant = shader.getUniformLocation(prefix + "constant");
                pointLights[i].linear = shader.getUniformLocation(prefix + "linear");
                pointLights[i].quadratic = shader.getUniformLocation(prefix + "quadratic");
            }

            spotActive = shader.getUniformLocation("spotLightActive");
            spotPos = shader.getUniformLocation("spotLight.position");
            spotDir = shader.getUniformLocation("spotLight.direction");
            spotAmb = shader.getUniformLocation("spotLight.ambient");
            spotDiff = shader.getUniformLocation("spotLight.diffuse");
            spotSpec = shader.getUniformLocation("spotLight.specular");
            spotCutOff = shader.getUniformLocation("spotLight.cutOff");
            spotOuterCutOff = shader.getUniformLocation("spotLight.outerCutOff");
            spotConstant = shader.getUniformLocation("spotLight.constant");
            spotLinear = shader.getUniformLocation("spotLight.linear");
            spotQuadratic = shader.getUniformLocation("spotLight.quadratic");

            initialized = true;
        }
    };

    LightUniformLocations lightUniforms;
    RenderContext sceneRenderContext;
    DepthRenderContext shadowDepthContext;
    bool sceneContextsInitialized = false;
    GLint shadowLightSpaceLoc = -1;
    bool shadowDepthContextInitialized = false;

    void renderShadowDepth()
    {
        if (!enableShadows || !shadowDepthShader || depthMapFBO == 0) return;

        // Directional light position tracking sun/moon sweep
        glm::vec3 lightDir = glm::normalize(glm::mix(glm::vec3(0.4f, 0.8f, 0.5f), glm::vec3(-0.3f, 0.7f, -0.4f), dayNightFactor));
        glm::vec3 sceneCenter(0.0f, 3.0f, 0.0f);
        glm::vec3 lightPos = sceneCenter + lightDir * 42.0f;

        glm::mat4 lightProjection = glm::ortho(-24.0f, 24.0f, -24.0f, 24.0f, 0.1f, 90.0f);
        glm::mat4 lightView = glm::lookAt(lightPos, sceneCenter, glm::vec3(0.0f, 1.0f, 0.0f));
        lightSpaceMatrix = lightProjection * lightView;

        shadowDepthShader->use();
        if (!shadowDepthContextInitialized)
        {
            shadowLightSpaceLoc = shadowDepthShader->getUniformLocation("lightSpaceMatrix");
            shadowDepthContext.init(*shadowDepthShader);
            shadowDepthContextInitialized = true;
        }

        Shader::setMat4(shadowLightSpaceLoc, lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        // Front-face culling eliminates self-shadow acne on architectural hulls
        glCullFace(GL_FRONT);
        rootNode->drawDepth(*shadowDepthShader, shadowDepthContext);
        glCullFace(GL_BACK);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        Mesh::ResetBoundVAO();
    }

    void render(const Shader& shader, const Camera& camera, float aspectRatio, int screenWidth = 1280, int screenHeight = 720)
    {
        // 1. Render depth map from directional light perspective
        renderShadowDepth();

        // 2. Restore screen viewport and render primary shaded scene
        glViewport(0, 0, screenWidth, screenHeight);

        shader.use();

        if (!sceneContextsInitialized)
        {
            lightUniforms.init(shader, pointLights.size());
            sceneRenderContext.init(shader);
            sceneContextsInitialized = true;
        }

        // Keep sky dome centered at camera position (true infinite distance)
        if (skyDome && skyDome->root)
        {
            skyDome->root->transform.position = camera.Position;
        }

        // Global camera view and projection matrices
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix(aspectRatio);

        Shader::setMat4(lightUniforms.view, view);
        Shader::setMat4(lightUniforms.projection, projection);
        Shader::setVec3(lightUniforms.viewPos, camera.Position);
        Shader::setFloat(lightUniforms.dayNightFactor, dayNightFactor);
        Shader::setFloat(lightUniforms.totalTime, totalTime);

        // Phase 2: Shading mode & Lights
        Shader::setInt(lightUniforms.shadingMode, shadingMode);
        Shader::setBool(lightUniforms.enableTextures, enableTextures);

        // Realistic Shadows: Bind Depth Map to Texture Unit 1
        Shader::setBool(lightUniforms.enableShadows, enableShadows);
        Shader::setMat4(lightUniforms.lightSpaceMatrix, lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        Shader::setInt(lightUniforms.shadowMap, 1);

        // Directional Light
        Shader::setVec3(lightUniforms.dirLightDir, dirLight.direction);
        Shader::setVec3(lightUniforms.dirLightAmb, dirLight.ambient);
        Shader::setVec3(lightUniforms.dirLightDiff, dirLight.diffuse);
        Shader::setVec3(lightUniforms.dirLightSpec, dirLight.specular);

        // Point Lights
        Shader::setInt(lightUniforms.numActivePointLights, (int)pointLights.size());
        for (size_t i = 0; i < pointLights.size(); ++i)
        {
            const auto& locs = lightUniforms.pointLights[i];
            Shader::setVec3(locs.pos, pointLights[i].position);
            Shader::setVec3(locs.amb, pointLights[i].ambient);
            Shader::setVec3(locs.diff, pointLights[i].diffuse);
            Shader::setVec3(locs.spec, pointLights[i].specular);
            Shader::setFloat(locs.constant, pointLights[i].constant);
            Shader::setFloat(locs.linear, pointLights[i].linear);
            Shader::setFloat(locs.quadratic, pointLights[i].quadratic);
        }

        // Spotlight
        Shader::setBool(lightUniforms.spotActive, spotLight.active);
        Shader::setVec3(lightUniforms.spotPos, spotLight.position);
        Shader::setVec3(lightUniforms.spotDir, spotLight.direction);
        Shader::setVec3(lightUniforms.spotAmb, spotLight.ambient);
        Shader::setVec3(lightUniforms.spotDiff, spotLight.diffuse);
        Shader::setVec3(lightUniforms.spotSpec, spotLight.specular);
        Shader::setFloat(lightUniforms.spotCutOff, spotLight.cutOff);
        Shader::setFloat(lightUniforms.spotOuterCutOff, spotLight.outerCutOff);
        Shader::setFloat(lightUniforms.spotConstant, spotLight.constant);
        Shader::setFloat(lightUniforms.spotLinear, spotLight.linear);
        Shader::setFloat(lightUniforms.spotQuadratic, spotLight.quadratic);

        // Reset dynamic draw state cache
        sceneRenderContext.lastShininess = -999.0f;
        sceneRenderContext.lastSpecularStrength = -999.0f;
        sceneRenderContext.lastUseTexture = -1;
        sceneRenderContext.lastTexture = nullptr;
        sceneRenderContext.lastTextureTiling = -999.0f;

        // Traverse scene graph and issue draw calls
        rootNode->draw(shader, sceneRenderContext);

        Mesh::ResetBoundVAO();
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
