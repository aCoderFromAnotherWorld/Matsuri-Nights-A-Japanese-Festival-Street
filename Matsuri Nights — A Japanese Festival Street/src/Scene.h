#pragma once

#include "SceneNode.h"
#include "Objects.h"
#include "Shader.h"
#include "Camera.h"
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

    Scene()
    {
        meshes.init();
        rootNode = std::make_shared<SceneNode>("World_Root");
        buildScene();
        setupInspectables();
    }

    void buildScene()
    {
        // 1. Ground & Street
        ground = std::make_unique<GroundObject>(meshes);
        rootNode->addChild(ground->root);

        // 2. Machiya Buildings (x4: 2 on Left side, 2 on Right side)
        // Street runs along Z (-35 to +35)
        // Building 1 (Left near): rotY = 0.0f faces road (+X)
        buildings.push_back(std::make_unique<MachiyaBuilding>(meshes, "Machiya_L1", glm::vec3(-10.5f, 0.0f, 16.0f), 0.0f));
        // Building 2 (Left far): rotY = 0.0f faces road (+X)
        buildings.push_back(std::make_unique<MachiyaBuilding>(meshes, "Machiya_L2", glm::vec3(-10.5f, 0.0f, -6.0f), 0.0f));
        // Building 3 (Right near): rotY = 180.0f faces road (-X)
        buildings.push_back(std::make_unique<MachiyaBuilding>(meshes, "Machiya_R1", glm::vec3(10.5f, 0.0f, 16.0f), 180.0f));
        // Building 4 (Right far): rotY = 180.0f faces road (-X)
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

    void setupInspectables()
    {
        inspectables.clear();
        // Register key nodes that the user / teacher may want to live-transform
        inspectables.push_back({ "1. Lantern [Body] (Child of Swinging Rope Pivot)", lanterns[0]->lanternBody, "Demonstrates transform relative to another object's reference frame" });
        inspectables.push_back({ "2. Lantern [Rope Pivot] (Parent Anchor Node)", lanterns[0]->ropePivot, "Parent node whose local rotation swings child lantern" });
        inspectables.push_back({ "3. Magic Orb (Child of Magician's Hand Bone)", magician->orbNode, "Orb transformed relative to moving hand reference frame" });
        inspectables.push_back({ "4. Magician Figure (Root)", magician->root, "Articulated character rig standing on stage" });
        inspectables.push_back({ "5. Vanishing Box (Scale-to-zero demo)", vanishingBox->boxNode, "Box experiencing scale and translation swap" });
        inspectables.push_back({ "6. Stage Spotlight Housing", spotlightRig->lampHousing, "Cone lamp housing rotating to track magician" });
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
    }

    void render(const Shader& shader, const Camera& camera, float aspectRatio)
    {
        shader.use();

        // Global camera view and projection matrices
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = camera.GetProjectionMatrix(aspectRatio);

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setFloat("dayNightFactor", dayNightFactor);

        // Traverse scene graph and issue draw calls
        rootNode->draw(shader);
    }
};
