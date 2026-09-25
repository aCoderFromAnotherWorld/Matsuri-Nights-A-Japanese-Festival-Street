#pragma once

#include "SceneNode.h"
#include "Primitives.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>
#include <cmath>
#include <cstdlib>
#include <string>

// Shared primitive meshes created once and reused across all scene nodes
struct SceneMeshes
{
    Mesh cube;
    Mesh cylinder;
    Mesh cone;
    Mesh sphere;
    Mesh plane;

    void init()
    {
        cube = Primitives::createCube(1.0f);
        cylinder = Primitives::createCylinder(0.5f, 1.0f, 24);
        cone = Primitives::createCone(0.5f, 1.0f, 24);
        sphere = Primitives::createSphere(0.5f, 20, 24);
        plane = Primitives::createPlane(1.0f, 1.0f, 4, 4);
    }
};

// -------------------------------------------------------------
// 1. Ground & Street
// -------------------------------------------------------------
class GroundObject
{
public:
    std::shared_ptr<SceneNode> root;

    GroundObject(SceneMeshes& meshes)
    {
        root = std::make_shared<SceneNode>("Ground_System");

        // Main street stone pavement
        auto street = std::make_shared<SceneNode>("Street_Pavement");
        street->mesh = &meshes.cube;
        street->transform.position = glm::vec3(0.0f, -0.1f, 0.0f);
        street->transform.scale = glm::vec3(14.0f, 0.2f, 100.0f);
        street->color = glm::vec4(0.35f, 0.35f, 0.37f, 1.0f); // dark stone grey
        root->addChild(street);

        // Cobblestone border left
        auto borderL = std::make_shared<SceneNode>("Border_Left");
        borderL->mesh = &meshes.cube;
        borderL->transform.position = glm::vec3(-7.2f, 0.05f, 0.0f);
        borderL->transform.scale = glm::vec3(0.5f, 0.3f, 100.0f);
        borderL->color = glm::vec4(0.22f, 0.22f, 0.24f, 1.0f);
        root->addChild(borderL);

        // Cobblestone border right
        auto borderR = std::make_shared<SceneNode>("Border_Right");
        borderR->mesh = &meshes.cube;
        borderR->transform.position = glm::vec3(7.2f, 0.05f, 0.0f);
        borderR->transform.scale = glm::vec3(0.5f, 0.3f, 100.0f);
        borderR->color = glm::vec4(0.22f, 0.22f, 0.24f, 1.0f);
        root->addChild(borderR);

        // Surrounding festival grass / earth ground
        auto dirt = std::make_shared<SceneNode>("Earth_Ground");
        dirt->mesh = &meshes.cube;
        dirt->transform.position = glm::vec3(0.0f, -0.25f, 0.0f);
        dirt->transform.scale = glm::vec3(90.0f, 0.2f, 120.0f);
        dirt->color = glm::vec4(0.18f, 0.24f, 0.16f, 1.0f); // dark mossy ground
        root->addChild(dirt);
    }
};

// -------------------------------------------------------------
// 2. Machiya Building (Traditional Japanese Townhouse)
// -------------------------------------------------------------
class MachiyaBuilding
{
public:
    std::shared_ptr<SceneNode> root;
    std::vector<std::shared_ptr<SceneNode>> windows;

    MachiyaBuilding(SceneMeshes& meshes, const std::string& name, const glm::vec3& pos, float rotY, const glm::vec3& scale = glm::vec3(1.0f))
    {
        root = std::make_shared<SceneNode>(name);
        root->transform.position = pos;
        root->transform.rotation.y = rotY;
        root->transform.scale = scale;

        glm::vec4 timber(0.36f, 0.22f, 0.13f, 1.0f);
        glm::vec4 darkWood(0.18f, 0.11f, 0.06f, 1.0f);
        glm::vec4 roofSlate(0.16f, 0.17f, 0.20f, 1.0f);
        glm::vec4 paperColor(0.92f, 0.88f, 0.80f, 1.0f);
        glm::vec4 stone(0.42f, 0.42f, 0.44f, 1.0f);

        // Ground floor wooden body
        auto floor1 = std::make_shared<SceneNode>(name + "_Floor1");
        floor1->mesh = &meshes.cube;
        floor1->transform.position = glm::vec3(0.0f, 2.2f, 0.0f);
        floor1->transform.scale = glm::vec3(8.0f, 4.4f, 9.0f);
        floor1->color = timber;
        root->addChild(floor1);

        // ---------------------------------------------------------
        // FRONT ENTRANCE DOOR (facing road on local +X facade)
        // ---------------------------------------------------------
        // Dark interior recess backing (sits flush against front wall X = 4.00, prevents see-through)
        auto doorBacking = std::make_shared<SceneNode>(name + "_DoorBacking");
        doorBacking->mesh = &meshes.cube;
        doorBacking->transform.position = glm::vec3(4.015f, 1.35f, -0.90f);
        doorBacking->transform.scale = glm::vec3(0.03f, 2.50f, 2.25f);
        doorBacking->color = glm::vec4(0.12f, 0.08f, 0.05f, 1.0f);
        root->addChild(doorBacking);

        // Outer Timber Frame: Left Jamb Post (at Z = 0.25)
        auto doorJambL = std::make_shared<SceneNode>(name + "_DoorJambL");
        doorJambL->mesh = &meshes.cube;
        doorJambL->transform.position = glm::vec3(4.14f, 1.35f, 0.26f);
        doorJambL->transform.scale = glm::vec3(0.24f, 2.70f, 0.12f);
        doorJambL->color = darkWood;
        root->addChild(doorJambL);

        // Outer Timber Frame: Right Jamb Post (at Z = -2.05)
        auto doorJambR = std::make_shared<SceneNode>(name + "_DoorJambR");
        doorJambR->mesh = &meshes.cube;
        doorJambR->transform.position = glm::vec3(4.14f, 1.35f, -2.06f);
        doorJambR->transform.scale = glm::vec3(0.24f, 2.70f, 0.12f);
        doorJambR->color = darkWood;
        root->addChild(doorJambR);

        // Top Lintel Header Beam (Kamoi)
        auto doorLintel = std::make_shared<SceneNode>(name + "_DoorLintel");
        doorLintel->mesh = &meshes.cube;
        doorLintel->transform.position = glm::vec3(4.14f, 2.62f, -0.90f);
        doorLintel->transform.scale = glm::vec3(0.26f, 0.16f, 2.44f);
        doorLintel->color = darkWood;
        root->addChild(doorLintel);

        // Bottom Threshold Sill Beam (Shikii)
        auto doorSill = std::make_shared<SceneNode>(name + "_DoorSill");
        doorSill->mesh = &meshes.cube;
        doorSill->transform.position = glm::vec3(4.14f, 0.08f, -0.90f);
        doorSill->transform.scale = glm::vec3(0.26f, 0.16f, 2.44f);
        doorSill->color = darkWood;
        root->addChild(doorSill);

        // Inner Sliding Shoji Door Panel (Right side: Z = -1.45, Layer X = 4.08)
        auto doorPanel1 = std::make_shared<SceneNode>(name + "_DoorPanel1");
        doorPanel1->mesh = &meshes.cube;
        doorPanel1->transform.position = glm::vec3(4.08f, 1.35f, -1.45f);
        doorPanel1->transform.scale = glm::vec3(0.04f, 2.40f, 1.05f);
        doorPanel1->isWindow = true;
        doorPanel1->color = paperColor;
        root->addChild(doorPanel1);
        windows.push_back(doorPanel1);

        // Inner Door Timber Frame Rails & Stiles
        auto doorFrame1_Top = std::make_shared<SceneNode>(name + "_DoorF1_Top");
        doorFrame1_Top->mesh = &meshes.cube;
        doorFrame1_Top->transform.position = glm::vec3(4.10f, 2.48f, -1.45f);
        doorFrame1_Top->transform.scale = glm::vec3(0.05f, 0.12f, 1.05f);
        doorFrame1_Top->color = darkWood;
        root->addChild(doorFrame1_Top);

        auto doorFrame1_Bot = std::make_shared<SceneNode>(name + "_DoorF1_Bot");
        doorFrame1_Bot->mesh = &meshes.cube;
        doorFrame1_Bot->transform.position = glm::vec3(4.10f, 0.22f, -1.45f);
        doorFrame1_Bot->transform.scale = glm::vec3(0.05f, 0.12f, 1.05f);
        doorFrame1_Bot->color = darkWood;
        root->addChild(doorFrame1_Bot);

        // Inner Door lattice wood ribs (X = 4.11)
        auto doorRib1 = std::make_shared<SceneNode>(name + "_DoorRib1");
        doorRib1->mesh = &meshes.cube;
        doorRib1->transform.position = glm::vec3(4.11f, 1.35f, -1.45f);
        doorRib1->transform.scale = glm::vec3(0.04f, 2.36f, 0.06f);
        doorRib1->color = darkWood;
        root->addChild(doorRib1);

        // Outer Sliding Shoji Door Panel (Left side: Z = -0.35, Layer X = 4.17)
        auto doorPanel2 = std::make_shared<SceneNode>(name + "_DoorPanel2");
        doorPanel2->mesh = &meshes.cube;
        doorPanel2->transform.position = glm::vec3(4.17f, 1.35f, -0.35f);
        doorPanel2->transform.scale = glm::vec3(0.04f, 2.40f, 1.05f);
        doorPanel2->isWindow = true;
        doorPanel2->color = paperColor;
        root->addChild(doorPanel2);
        windows.push_back(doorPanel2);

        // Outer Door Timber Frame Rails & Stiles
        auto doorFrame2_Top = std::make_shared<SceneNode>(name + "_DoorF2_Top");
        doorFrame2_Top->mesh = &meshes.cube;
        doorFrame2_Top->transform.position = glm::vec3(4.19f, 2.48f, -0.35f);
        doorFrame2_Top->transform.scale = glm::vec3(0.05f, 0.12f, 1.05f);
        doorFrame2_Top->color = darkWood;
        root->addChild(doorFrame2_Top);

        auto doorFrame2_Bot = std::make_shared<SceneNode>(name + "_DoorF2_Bot");
        doorFrame2_Bot->mesh = &meshes.cube;
        doorFrame2_Bot->transform.position = glm::vec3(4.19f, 0.22f, -0.35f);
        doorFrame2_Bot->transform.scale = glm::vec3(0.05f, 0.12f, 1.05f);
        doorFrame2_Bot->color = darkWood;
        root->addChild(doorFrame2_Bot);

        // Outer Door lattice wood ribs (X = 4.20)
        auto doorRib2 = std::make_shared<SceneNode>(name + "_DoorRib2");
        doorRib2->mesh = &meshes.cube;
        doorRib2->transform.position = glm::vec3(4.20f, 1.35f, -0.35f);
        doorRib2->transform.scale = glm::vec3(0.04f, 2.36f, 0.06f);
        doorRib2->color = darkWood;
        root->addChild(doorRib2);

        // Entrance Stone Step Threshold (Kutsunugi-ishi) sitting on ground level
        auto stoneStep = std::make_shared<SceneNode>(name + "_StoneStep");
        stoneStep->mesh = &meshes.cube;
        stoneStep->transform.position = glm::vec3(4.35f, 0.08f, -0.90f);
        stoneStep->transform.scale = glm::vec3(0.60f, 0.16f, 2.50f);
        stoneStep->color = stone;
        root->addChild(stoneStep);

        // Doorway Eave Canopy (Hisashi) projecting outward toward road
        auto doorCanopy = std::make_shared<SceneNode>(name + "_DoorCanopy");
        doorCanopy->mesh = &meshes.cube;
        doorCanopy->transform.position = glm::vec3(4.60f, 2.80f, -0.90f);
        doorCanopy->transform.rotation.z = -7.0f;
        doorCanopy->transform.scale = glm::vec3(1.10f, 0.12f, 2.70f);
        doorCanopy->color = roofSlate;
        root->addChild(doorCanopy);

        // Entrance Fabric Noren Curtain
        auto noren = std::make_shared<SceneNode>(name + "_EntranceNoren");
        noren->mesh = &meshes.cube;
        noren->transform.position = glm::vec3(4.26f, 2.45f, -0.90f);
        noren->transform.scale = glm::vec3(0.06f, 0.45f, 1.90f);
        noren->color = glm::vec4(0.16f, 0.22f, 0.45f, 1.0f); // indigo dye
        root->addChild(noren);

        // Ground Floor Window (alongside door on local +X facade)
        auto winF1_Frame = std::make_shared<SceneNode>(name + "_ShojiF1_Frame");
        winF1_Frame->mesh = &meshes.cube;
        winF1_Frame->transform.position = glm::vec3(4.08f, 1.90f, 2.20f);
        winF1_Frame->transform.scale = glm::vec3(0.18f, 2.36f, 2.56f);
        winF1_Frame->color = darkWood;
        root->addChild(winF1_Frame);

        auto winF1 = std::make_shared<SceneNode>(name + "_ShojiF1_Right");
        winF1->mesh = &meshes.cube;
        winF1->transform.position = glm::vec3(4.06f, 1.90f, 2.20f);
        winF1->transform.scale = glm::vec3(0.04f, 2.20f, 2.40f);
        winF1->isWindow = true;
        winF1->color = paperColor;
        root->addChild(winF1);
        windows.push_back(winF1);

        auto winF1_RibV = std::make_shared<SceneNode>(name + "_WinF1_RibV");
        winF1_RibV->mesh = &meshes.cube;
        winF1_RibV->transform.position = glm::vec3(4.09f, 1.90f, 2.20f);
        winF1_RibV->transform.scale = glm::vec3(0.04f, 2.24f, 0.08f);
        winF1_RibV->color = darkWood;
        root->addChild(winF1_RibV);

        auto winF1_RibH = std::make_shared<SceneNode>(name + "_WinF1_RibH");
        winF1_RibH->mesh = &meshes.cube;
        winF1_RibH->transform.position = glm::vec3(4.09f, 1.90f, 2.20f);
        winF1_RibH->transform.scale = glm::vec3(0.04f, 0.08f, 2.30f);
        winF1_RibH->color = darkWood;
        root->addChild(winF1_RibH);

        // First floor eaves roof
        auto eaves1 = std::make_shared<SceneNode>(name + "_Eaves1");
        eaves1->mesh = &meshes.cube;
        eaves1->transform.position = glm::vec3(0.0f, 4.4f, 0.0f);
        eaves1->transform.scale = glm::vec3(8.8f, 0.35f, 10.0f);
        eaves1->color = roofSlate;
        root->addChild(eaves1);

        // Second floor wooden body
        auto floor2 = std::make_shared<SceneNode>(name + "_Floor2");
        floor2->mesh = &meshes.cube;
        floor2->transform.position = glm::vec3(0.0f, 6.2f, 0.0f);
        floor2->transform.scale = glm::vec3(7.4f, 3.8f, 8.4f);
        floor2->color = glm::vec4(0.32f, 0.20f, 0.12f, 1.0f);
        root->addChild(floor2);

        // Floor 2 Windows (3 windows facing road on local +X facade)
        float winZ[3] = { -2.4f, 0.0f, 2.4f };
        for (int i = 0; i < 3; ++i)
        {
            auto win2 = std::make_shared<SceneNode>(name + "_ShojiF2_" + std::to_string(i));
            win2->mesh = &meshes.cube;
            win2->transform.position = glm::vec3(3.74f, 6.2f, winZ[i]);
            win2->transform.scale = glm::vec3(0.05f, 1.9f, 1.4f);
            win2->isWindow = true;
            win2->color = paperColor;
            root->addChild(win2);
            windows.push_back(win2);

            auto rib = std::make_shared<SceneNode>(name + "_WinF2Rib_" + std::to_string(i));
            rib->mesh = &meshes.cube;
            rib->transform.position = glm::vec3(3.78f, 6.2f, winZ[i]);
            rib->transform.scale = glm::vec3(0.04f, 2.0f, 0.10f);
            rib->color = darkWood;
            root->addChild(rib);
        }

        // ---------------------------------------------------------
        // UPPER MAIN PITCHED ROOF: TRUE UPSIDE-DOWN "V" (^) GABLE
        // ---------------------------------------------------------
        // Left slope (slopes down toward -X): +22.0 deg rotation
        auto roofLeft = std::make_shared<SceneNode>(name + "_RoofLeft");
        roofLeft->mesh = &meshes.cube;
        roofLeft->transform.position = glm::vec3(-2.0f, 8.6f, 0.0f);
        roofLeft->transform.rotation.z = 22.0f; // Slopes DOWN toward outer edge
        roofLeft->transform.scale = glm::vec3(4.8f, 0.35f, 9.6f);
        roofLeft->color = roofSlate;
        root->addChild(roofLeft);

        // Right slope (slopes down toward +X): -22.0 deg rotation
        auto roofRight = std::make_shared<SceneNode>(name + "_RoofRight");
        roofRight->mesh = &meshes.cube;
        roofRight->transform.position = glm::vec3(2.0f, 8.6f, 0.0f);
        roofRight->transform.rotation.z = -22.0f; // Slopes DOWN toward street eave
        roofRight->transform.scale = glm::vec3(4.8f, 0.35f, 9.6f);
        roofRight->color = roofSlate;
        root->addChild(roofRight);

        // Apex Ridge Cap Beam connecting the two slopes at the top peak
        auto ridge = std::make_shared<SceneNode>(name + "_RoofRidge");
        ridge->mesh = &meshes.cube;
        ridge->transform.position = glm::vec3(0.0f, 9.45f, 0.0f);
        ridge->transform.scale = glm::vec3(0.65f, 0.38f, 9.8f);
        ridge->color = glm::vec4(0.11f, 0.11f, 0.13f, 1.0f);
        root->addChild(ridge);

        // Gable wall timber filling under roof peak
        auto gableFront = std::make_shared<SceneNode>(name + "_GableFront");
        gableFront->mesh = &meshes.cube;
        gableFront->transform.position = glm::vec3(0.0f, 8.4f, 4.2f);
        gableFront->transform.scale = glm::vec3(4.0f, 1.1f, 0.15f);
        gableFront->color = timber;
        root->addChild(gableFront);

        auto gableBack = std::make_shared<SceneNode>(name + "_GableBack");
        gableBack->mesh = &meshes.cube;
        gableBack->transform.position = glm::vec3(0.0f, 8.4f, -4.2f);
        gableBack->transform.scale = glm::vec3(4.0f, 1.1f, 0.15f);
        gableBack->color = timber;
        root->addChild(gableBack);
    }
};

// -------------------------------------------------------------
// 3. Torii Gate (Grand Shrine Portal at Street End)
// -------------------------------------------------------------
class ToriiGate
{
public:
    std::shared_ptr<SceneNode> root;

    ToriiGate(SceneMeshes& meshes, const glm::vec3& pos)
    {
        root = std::make_shared<SceneNode>("Torii_Gate");
        root->transform.position = pos;

        glm::vec4 vermilion(0.85f, 0.22f, 0.12f, 1.0f);
        glm::vec4 black(0.12f, 0.12f, 0.14f, 1.0f);
        glm::vec4 stone(0.45f, 0.45f, 0.48f, 1.0f);

        // Stone pedestal bases (Kamebara)
        auto baseL = std::make_shared<SceneNode>("Torii_BaseL");
        baseL->mesh = &meshes.cylinder;
        baseL->transform.position = glm::vec3(-4.5f, 0.5f, 0.0f);
        baseL->transform.scale = glm::vec3(1.6f, 1.0f, 1.6f);
        baseL->color = stone;
        root->addChild(baseL);

        auto baseR = std::make_shared<SceneNode>("Torii_BaseR");
        baseR->mesh = &meshes.cylinder;
        baseR->transform.position = glm::vec3(4.5f, 0.5f, 0.0f);
        baseR->transform.scale = glm::vec3(1.6f, 1.0f, 1.6f);
        baseR->color = stone;
        root->addChild(baseR);

        // Vertical pillars (Hashira)
        auto colL = std::make_shared<SceneNode>("Torii_ColL");
        colL->mesh = &meshes.cylinder;
        colL->transform.position = glm::vec3(-4.5f, 5.5f, 0.0f);
        colL->transform.scale = glm::vec3(0.9f, 9.5f, 0.9f);
        colL->transform.rotation.z = -1.5f; // subtle authentic inward tilt
        colL->color = vermilion;
        root->addChild(colL);

        auto colR = std::make_shared<SceneNode>("Torii_ColR");
        colR->mesh = &meshes.cylinder;
        colR->transform.position = glm::vec3(4.5f, 5.5f, 0.0f);
        colR->transform.scale = glm::vec3(0.9f, 9.5f, 0.9f);
        colR->transform.rotation.z = 1.5f;
        colR->color = vermilion;
        root->addChild(colR);

        // Lower horizontal crossbeam (Nuki)
        auto nuki = std::make_shared<SceneNode>("Torii_Nuki");
        nuki->mesh = &meshes.cube;
        nuki->transform.position = glm::vec3(0.0f, 8.2f, 0.0f);
        nuki->transform.scale = glm::vec3(12.5f, 0.65f, 0.75f);
        nuki->color = vermilion;
        root->addChild(nuki);

        // Central vertical tablet / strut (Gakuzuka)
        auto gakuzuka = std::make_shared<SceneNode>("Torii_Gakuzuka");
        gakuzuka->mesh = &meshes.cube;
        gakuzuka->transform.position = glm::vec3(0.0f, 9.25f, 0.0f);
        gakuzuka->transform.scale = glm::vec3(1.2f, 1.4f, 0.35f);
        gakuzuka->color = black;
        root->addChild(gakuzuka);

        // Sub-top crossbeam (Shimaki)
        auto shimaki = std::make_shared<SceneNode>("Torii_Shimaki");
        shimaki->mesh = &meshes.cube;
        shimaki->transform.position = glm::vec3(0.0f, 10.1f, 0.0f);
        shimaki->transform.scale = glm::vec3(14.0f, 0.65f, 0.95f);
        shimaki->color = vermilion;
        root->addChild(shimaki);

        // Upper main curved crossbeam (Kasagi) with black roof cap
        auto kasagi = std::make_shared<SceneNode>("Torii_Kasagi");
        kasagi->mesh = &meshes.cube;
        kasagi->transform.position = glm::vec3(0.0f, 10.65f, 0.0f);
        kasagi->transform.scale = glm::vec3(15.2f, 0.5f, 1.2f);
        kasagi->color = black;
        root->addChild(kasagi);
    }
};

// -------------------------------------------------------------
// 4. Sakura Tree with Falling Blossom Petals
// -------------------------------------------------------------
struct FallingPetal
{
    std::shared_ptr<SceneNode> node;
    glm::vec3 basePos;
    float speed;
    float phase;
    float swayAmp;
};

class SakuraTree
{
public:
    std::shared_ptr<SceneNode> root;
    std::vector<FallingPetal> petals;

    SakuraTree(SceneMeshes& meshes, const glm::vec3& pos)
    {
        root = std::make_shared<SceneNode>("Sakura_Tree");
        root->transform.position = pos;

        // Trunk
        auto trunk = std::make_shared<SceneNode>("Trunk");
        trunk->mesh = &meshes.cylinder;
        trunk->transform.position = glm::vec3(0.0f, 3.0f, 0.0f);
        trunk->transform.scale = glm::vec3(1.1f, 6.0f, 1.1f);
        trunk->transform.rotation.z = 4.0f;
        trunk->color = glm::vec4(0.32f, 0.22f, 0.16f, 1.0f);
        root->addChild(trunk);

        // Main boughs / branches
        auto bough1 = std::make_shared<SceneNode>("Bough_1");
        bough1->mesh = &meshes.cylinder;
        bough1->transform.position = glm::vec3(-0.8f, 5.2f, 0.5f);
        bough1->transform.rotation = glm::vec3(20.0f, 30.0f, -40.0f);
        bough1->transform.scale = glm::vec3(0.55f, 3.5f, 0.55f);
        bough1->color = glm::vec4(0.32f, 0.22f, 0.16f, 1.0f);
        root->addChild(bough1);

        auto bough2 = std::make_shared<SceneNode>("Bough_2");
        bough2->mesh = &meshes.cylinder;
        bough2->transform.position = glm::vec3(0.9f, 5.0f, -0.6f);
        bough2->transform.rotation = glm::vec3(-15.0f, -40.0f, 35.0f);
        bough2->transform.scale = glm::vec3(0.55f, 3.5f, 0.55f);
        bough2->color = glm::vec4(0.32f, 0.22f, 0.16f, 1.0f);
        root->addChild(bough2);

        // Blossom foliage clusters (soft cherry pink spheres)
        glm::vec4 pink1(0.98f, 0.72f, 0.82f, 1.0f);
        glm::vec4 pink2(0.95f, 0.62f, 0.76f, 1.0f);
        glm::vec4 pink3(1.00f, 0.82f, 0.88f, 1.0f);

        struct FoliageDef { glm::vec3 offset; glm::vec3 scl; glm::vec4 col; };
        std::vector<FoliageDef> clusters = {
            { { 0.0f, 6.8f, 0.0f },    { 4.2f, 3.2f, 4.2f }, pink1 },
            { { -1.8f, 6.2f, 1.2f },   { 3.2f, 2.6f, 3.0f }, pink2 },
            { {  1.9f, 6.0f, -1.1f },  { 3.2f, 2.5f, 3.2f }, pink3 },
            { {  0.8f, 7.5f,  1.4f },  { 2.8f, 2.2f, 2.8f }, pink1 },
            { { -1.2f, 7.2f, -1.5f },  { 2.9f, 2.3f, 2.9f }, pink2 },
            { {  0.0f, 8.4f,  0.0f },  { 2.5f, 2.0f, 2.5f }, pink3 }
        };

        for (size_t i = 0; i < clusters.size(); ++i)
        {
            auto cluster = std::make_shared<SceneNode>("BlossomCluster_" + std::to_string(i));
            cluster->mesh = &meshes.sphere;
            cluster->transform.position = clusters[i].offset;
            cluster->transform.scale = clusters[i].scl;
            cluster->color = clusters[i].col;
            root->addChild(cluster);
        }

        // 8 Falling Blossom Petals (hierarchical moving objects)
        for (int i = 0; i < 8; ++i)
        {
            auto petal = std::make_shared<SceneNode>("Petal_" + std::to_string(i));
            petal->mesh = &meshes.sphere;
            petal->transform.scale = glm::vec3(0.18f, 0.08f, 0.18f);
            petal->color = glm::vec4(0.99f, 0.68f, 0.80f, 1.0f);

            FallingPetal fp;
            fp.node = petal;
            fp.basePos = glm::vec3(((rand() % 100) / 100.0f - 0.5f) * 4.5f,
                                   5.5f + (rand() % 30) / 10.0f,
                                   ((rand() % 100) / 100.0f - 0.5f) * 4.5f);
            fp.speed = 0.8f + (rand() % 50) / 100.0f;
            fp.phase = (float)i * 0.8f;
            fp.swayAmp = 0.35f + (rand() % 30) / 100.0f;

            petal->transform.position = fp.basePos;
            root->addChild(petal);
            petals.push_back(fp);
        }
    }

    void update(float time, float dt)
    {
        for (auto& fp : petals)
        {
            // Drifts downward
            fp.basePos.y -= fp.speed * dt;
            // Looping respawn at tree crown
            if (fp.basePos.y < 0.1f)
            {
                fp.basePos.y = 7.0f + (rand() % 20) / 10.0f;
            }

            // Swaying sinusoidal drift + tumbling rotation
            float xOffset = std::sin(time * 1.8f + fp.phase) * fp.swayAmp;
            float zOffset = std::cos(time * 1.4f + fp.phase) * fp.swayAmp;
            fp.node->transform.position = glm::vec3(fp.basePos.x + xOffset, fp.basePos.y, fp.basePos.z + zOffset);
            fp.node->transform.rotation = glm::vec3(time * 45.0f + fp.phase * 20.0f, time * 60.0f, time * 30.0f);
        }
    }
};

// -------------------------------------------------------------
// 5. Lantern with Pendulum Swing (Relative Transform Demo Object)
// -------------------------------------------------------------
class LanternObject
{
public:
    std::shared_ptr<SceneNode> ropePivot; // Parent node whose rotation swings the lantern
    std::shared_ptr<SceneNode> lanternBody; // Child node inheriting rope transform
    float swingFreq = 2.0f;
    float swingAmp = 12.0f;
    float phaseOffset = 0.0f;

    LanternObject(SceneMeshes& meshes, const std::string& name, const glm::vec3& anchorPos, float phase = 0.0f)
        : phaseOffset(phase)
    {
        // 1. Rope pivot node anchored at overhead line
        ropePivot = std::make_shared<SceneNode>(name + "_RopePivot");
        ropePivot->transform.position = anchorPos;

        // Rope cord (descending from pivot)
        auto cord = std::make_shared<SceneNode>(name + "_Cord");
        cord->mesh = &meshes.cylinder;
        cord->transform.position = glm::vec3(0.0f, -0.4f, 0.0f);
        cord->transform.scale = glm::vec3(0.04f, 0.8f, 0.04f);
        cord->color = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);
        ropePivot->addChild(cord);

        // 2. Lantern Body Node (child of rope pivot!)
        lanternBody = std::make_shared<SceneNode>(name + "_Body");
        lanternBody->transform.position = glm::vec3(0.0f, -0.85f, 0.0f);
        ropePivot->addChild(lanternBody);

        // Top black cap
        auto capTop = std::make_shared<SceneNode>(name + "_CapTop");
        capTop->mesh = &meshes.cylinder;
        capTop->transform.position = glm::vec3(0.0f, 0.42f, 0.0f);
        capTop->transform.scale = glm::vec3(0.48f, 0.14f, 0.48f);
        capTop->color = glm::vec4(0.12f, 0.12f, 0.12f, 1.0f);
        lanternBody->addChild(capTop);

        // Glowing red paper body (chochin)
        auto paper = std::make_shared<SceneNode>(name + "_Paper");
        paper->mesh = &meshes.sphere;
        paper->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        paper->transform.scale = glm::vec3(0.85f, 1.05f, 0.85f);
        paper->color = glm::vec4(0.92f, 0.18f, 0.12f, 1.0f);
        paper->isEmissive = true;
        paper->emissiveColor = glm::vec3(1.0f, 0.35f, 0.15f); // warm festive glow
        lanternBody->addChild(paper);

        // Central white festival kanji band
        auto band = std::make_shared<SceneNode>(name + "_Band");
        band->mesh = &meshes.cylinder;
        band->transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
        band->transform.scale = glm::vec3(0.87f, 0.35f, 0.87f);
        band->color = glm::vec4(0.95f, 0.92f, 0.85f, 1.0f);
        lanternBody->addChild(band);

        // Bottom black cap
        auto capBot = std::make_shared<SceneNode>(name + "_CapBot");
        capBot->mesh = &meshes.cylinder;
        capBot->transform.position = glm::vec3(0.0f, -0.42f, 0.0f);
        capBot->transform.scale = glm::vec3(0.45f, 0.12f, 0.45f);
        capBot->color = glm::vec4(0.12f, 0.12f, 0.12f, 1.0f);
        lanternBody->addChild(capBot);
    }

    void update(float time)
    {
        // Core hierarchical relative-transform demonstration:
        // Changing local rotation of rope node swings the lantern along with all its children!
        float angle = swingAmp * std::sin(time * swingFreq + phaseOffset);
        ropePivot->transform.rotation.x = angle;
        ropePivot->transform.rotation.z = angle * 0.25f;
    }
};

// -------------------------------------------------------------
// 5B. Street Lantern Span with Poles, Sagging Catenary Rope, and Aligned Lanterns
// -------------------------------------------------------------
class StreetLanternSpan
{
public:
    std::shared_ptr<SceneNode> root;
    std::vector<std::shared_ptr<LanternObject>> lanterns;

    StreetLanternSpan(SceneMeshes& meshes, const std::string& prefix, float z, float phaseBase = 0.0f)
    {
        root = std::make_shared<SceneNode>(prefix + "_Span");
        root->transform.position = glm::vec3(0.0f, 0.0f, z);

        glm::vec4 wood(0.32f, 0.20f, 0.12f, 1.0f);
        glm::vec4 stone(0.42f, 0.42f, 0.45f, 1.0f);
        glm::vec4 ropeColor(0.48f, 0.38f, 0.25f, 1.0f);

        float poleX[2] = { -3.8f, 3.8f };

        // 1. Poles on both sides of the street along curb line (X = +/- 3.8)
        // Completely clear of all pedestrians (X in [-1.6, 1.4]), stalls, and stage
        for (int p = 0; p < 2; ++p)
        {
            float px = poleX[p];
            std::string pName = prefix + ((p == 0) ? "_PoleL" : "_PoleR");

            // Stone base pedestal collar (resting flush on ground: Y in [0.0, 0.40])
            auto base = std::make_shared<SceneNode>(pName + "_Base");
            base->mesh = &meshes.cube;
            base->transform.position = glm::vec3(px, 0.20f, 0.0f);
            base->transform.scale = glm::vec3(0.45f, 0.40f, 0.45f);
            base->color = stone;
            root->addChild(base);

            // Tall wooden cedar pole (flush on ground: Y in [0.0, 6.50])
            auto shaft = std::make_shared<SceneNode>(pName + "_Shaft");
            shaft->mesh = &meshes.cylinder;
            shaft->transform.position = glm::vec3(px, 3.25f, 0.0f);
            shaft->transform.scale = glm::vec3(0.14f, 6.50f, 0.14f);
            shaft->color = wood;
            root->addChild(shaft);

            // Top decorative crossarm peg / finial
            auto peg = std::make_shared<SceneNode>(pName + "_Peg");
            peg->mesh = &meshes.cube;
            peg->transform.position = glm::vec3(px, 6.20f, 0.0f);
            peg->transform.scale = glm::vec3(0.35f, 0.08f, 0.14f);
            peg->color = wood;
            root->addChild(peg);
        }

        // 2. Realistic Sagging Catenary Rope connecting left and right poles
        // Span: X in [-3.8, +3.8]. Equation: Y(x) = Y_pole - sag * (1.0 - (x/3.8)^2)
        const float yPole = 6.20f;
        const float sag = 0.65f;
        const float halfSpan = 3.8f;
        const int numSegments = 10;

        auto catenaryY = [&](float x) -> float {
            float normX = x / halfSpan;
            return yPole - sag * (1.0f - normX * normX);
        };

        for (int i = 0; i < numSegments; ++i)
        {
            float x0 = -halfSpan + (float)i / (float)numSegments * (2.0f * halfSpan);
            float x1 = -halfSpan + (float)(i + 1) / (float)numSegments * (2.0f * halfSpan);
            float y0 = catenaryY(x0);
            float y1 = catenaryY(x1);

            float mx = (x0 + x1) * 0.5f;
            float my = (y0 + y1) * 0.5f;

            float dx = x1 - x0;
            float dy = y1 - y0;
            float len = std::sqrt(dx * dx + dy * dy);
            float angleDeg = glm::degrees(std::atan2(dy, dx));

            auto seg = std::make_shared<SceneNode>(prefix + "_RopeSeg_" + std::to_string(i));
            seg->mesh = &meshes.cylinder;
            seg->transform.position = glm::vec3(mx, my, 0.0f);
            seg->transform.rotation.z = angleDeg - 90.0f; // Align cylinder along segment direction
            seg->transform.scale = glm::vec3(0.035f, len, 0.035f);
            seg->color = ropeColor;
            root->addChild(seg);
        }

        // 3. Lanterns anchored precisely onto the sagging catenary curve
        float lanternX[2] = { -1.90f, 1.90f };
        for (int l = 0; l < 2; ++l)
        {
            float lx = lanternX[l];
            float ly = catenaryY(lx); // Exact height on the sagging rope!
            float phase = phaseBase + (float)l * 0.5f;

            auto lantern = std::make_shared<LanternObject>(meshes, prefix + "_Lantern_" + std::to_string(l), glm::vec3(lx, ly, 0.0f), phase);
            root->addChild(lantern->ropePivot);
            lanterns.push_back(lantern);
        }
    }

    void update(float time)
    {
        for (auto& l : lanterns)
            l->update(time);
    }
};

// -------------------------------------------------------------
// 6. Takoyaki Stall with Flipping Balls
// -------------------------------------------------------------
struct TakoyakiBall
{
    std::shared_ptr<SceneNode> node;
    glm::vec3 holePos1;
    glm::vec3 holePos2;
    float flipTimer = 0.0f;
    float flipDuration = 0.65f;
    bool isFlipping = false;
};

class TakoyakiStall
{
public:
    std::shared_ptr<SceneNode> root;
    std::vector<TakoyakiBall> balls;
    std::vector<std::shared_ptr<SceneNode>> stallLanterns;

    TakoyakiStall(SceneMeshes& meshes, const glm::vec3& pos, float rotY = 0.0f)
    {
        root = std::make_shared<SceneNode>("Takoyaki_Stall");
        root->transform.position = pos;
        root->transform.rotation.y = rotY;

        glm::vec4 wood(0.42f, 0.28f, 0.18f, 1.0f);
        glm::vec4 red(0.88f, 0.20f, 0.15f, 1.0f);
        glm::vec4 white(0.92f, 0.90f, 0.88f, 1.0f);

        // Counter base table (sits at ground level Y in [0, 1.1])
        auto base = std::make_shared<SceneNode>("Stall_Base");
        base->mesh = &meshes.cube;
        base->transform.position = glm::vec3(0.0f, 0.55f, 0.0f);
        base->transform.scale = glm::vec3(3.6f, 1.1f, 1.9f);
        base->color = wood;
        root->addChild(base);

        // Counter top board (waist height at Y = 1.15)
        auto counterTop = std::make_shared<SceneNode>("Counter_Top");
        counterTop->mesh = &meshes.cube;
        counterTop->transform.position = glm::vec3(0.0f, 1.15f, 0.0f);
        counterTop->transform.scale = glm::vec3(4.0f, 0.10f, 2.2f);
        counterTop->color = glm::vec4(0.32f, 0.20f, 0.12f, 1.0f);
        root->addChild(counterTop);

        // 4 Canopy support corner poles
        float px[2] = { -1.7f, 1.7f };
        float pz[2] = { -0.9f, 0.9f };
        for (int x = 0; x < 2; ++x) {
            for (int z = 0; z < 2; ++z) {
                auto pole = std::make_shared<SceneNode>("Pole_" + std::to_string(x) + "_" + std::to_string(z));
                pole->mesh = &meshes.cylinder;
                pole->transform.position = glm::vec3(px[x], 2.2f, pz[z]);
                pole->transform.scale = glm::vec3(0.08f, 2.0f, 0.08f);
                pole->color = wood;
                root->addChild(pole);
            }
        }

        // Slanted striped awning roof
        auto awning = std::make_shared<SceneNode>("Awning_Roof");
        awning->mesh = &meshes.cube;
        awning->transform.position = glm::vec3(0.0f, 3.25f, 0.0f);
        awning->transform.rotation.x = -8.0f;
        awning->transform.scale = glm::vec3(4.4f, 0.18f, 2.6f);
        awning->color = red;
        root->addChild(awning);

        // Noren front banner
        auto noren = std::make_shared<SceneNode>("Takoyaki_Banner");
        noren->mesh = &meshes.cube;
        noren->transform.position = glm::vec3(0.0f, 2.90f, 1.18f);
        noren->transform.scale = glm::vec3(4.0f, 0.50f, 0.06f);
        noren->color = white;
        root->addChild(noren);

        // Hanging Front Stall Lanterns (Ko-Chochin emitting warm night light)
        float slX[2] = { -1.75f, 1.75f };
        for (int l = 0; l < 2; ++l)
        {
            auto pivot = std::make_shared<SceneNode>("TakoStall_LanternPivot_" + std::to_string(l));
            pivot->transform.position = glm::vec3(slX[l], 3.05f, 1.15f);

            auto cord = std::make_shared<SceneNode>("TakoStall_Cord_" + std::to_string(l));
            cord->mesh = &meshes.cylinder;
            cord->transform.position = glm::vec3(0.0f, -0.15f, 0.0f);
            cord->transform.scale = glm::vec3(0.02f, 0.30f, 0.02f);
            cord->color = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);
            pivot->addChild(cord);

            auto cTop = std::make_shared<SceneNode>("TakoStall_CapT_" + std::to_string(l));
            cTop->mesh = &meshes.cylinder;
            cTop->transform.position = glm::vec3(0.0f, -0.32f, 0.0f);
            cTop->transform.scale = glm::vec3(0.26f, 0.06f, 0.26f);
            cTop->color = glm::vec4(0.12f, 0.12f, 0.12f, 1.0f);
            pivot->addChild(cTop);

            auto paper = std::make_shared<SceneNode>("TakoStall_Paper_" + std::to_string(l));
            paper->mesh = &meshes.sphere;
            paper->transform.position = glm::vec3(0.0f, -0.56f, 0.0f);
            paper->transform.scale = glm::vec3(0.42f, 0.50f, 0.42f);
            paper->color = glm::vec4(0.95f, 0.20f, 0.14f, 1.0f);
            paper->isEmissive = true;
            paper->emissiveColor = glm::vec3(1.20f, 0.50f, 0.18f); // Warm night light!
            pivot->addChild(paper);

            auto band = std::make_shared<SceneNode>("TakoStall_Band_" + std::to_string(l));
            band->mesh = &meshes.cylinder;
            band->transform.position = glm::vec3(0.0f, -0.56f, 0.0f);
            band->transform.scale = glm::vec3(0.44f, 0.16f, 0.44f);
            band->color = glm::vec4(0.95f, 0.92f, 0.88f, 1.0f);
            pivot->addChild(band);

            auto cBot = std::make_shared<SceneNode>("TakoStall_CapB_" + std::to_string(l));
            cBot->mesh = &meshes.cylinder;
            cBot->transform.position = glm::vec3(0.0f, -0.80f, 0.0f);
            cBot->transform.scale = glm::vec3(0.24f, 0.06f, 0.24f);
            cBot->color = glm::vec4(0.12f, 0.12f, 0.12f, 1.0f);
            pivot->addChild(cBot);

            root->addChild(pivot);
            stallLanterns.push_back(pivot);
        }

        // Cast iron takoyaki grill plate
        auto grill = std::make_shared<SceneNode>("Grill_Plate");
        grill->mesh = &meshes.cube;
        grill->transform.position = glm::vec3(0.0f, 1.24f, 0.0f);
        grill->transform.scale = glm::vec3(2.2f, 0.08f, 1.3f);
        grill->color = glm::vec4(0.14f, 0.14f, 0.15f, 1.0f);
        root->addChild(grill);

        // 6 Takoyaki balls in 2 rows of 3
        float bx[3] = { -0.65f, 0.0f, 0.65f };
        float bz[2] = { -0.32f, 0.32f };
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                auto ballNode = std::make_shared<SceneNode>("TakoBall_" + std::to_string(i) + "_" + std::to_string(j));
                ballNode->mesh = &meshes.sphere;
                ballNode->transform.scale = glm::vec3(0.28f, 0.28f, 0.28f);
                ballNode->color = glm::vec4(0.85f, 0.62f, 0.28f, 1.0f); // golden browned octopus ball

                TakoyakiBall tb;
                tb.node = ballNode;
                tb.holePos1 = glm::vec3(bx[i], 1.36f, bz[j]);
                tb.holePos2 = glm::vec3(bx[(i + 1) % 3], 1.36f, bz[j]);
                tb.node->transform.position = tb.holePos1;
                tb.flipTimer = (float)(i * 2 + j) * 0.9f;

                root->addChild(ballNode);
                balls.push_back(tb);
            }
        }
    }

    void update(float time, float dt)
    {
        for (auto& tb : balls)
        {
            // Spin constantly in place
            tb.node->transform.rotation.y += 120.0f * dt;

            tb.flipTimer += dt;
            if (tb.flipTimer > 3.0f)
            {
                tb.isFlipping = true;
                tb.flipTimer = 0.0f;
            }

            if (tb.isFlipping)
            {
                float t = tb.flipTimer / tb.flipDuration;
                if (t >= 1.0f)
                {
                    tb.isFlipping = false;
                    tb.node->transform.position = tb.holePos1;
                }
                else
                {
                    // Parabolic hopping arc: translation along X and parabolic height Y
                    glm::vec3 curPos = glm::mix(tb.holePos1, tb.holePos2, t);
                    curPos.y += 4.0f * 0.35f * t * (1.0f - t); // Parabolic hop formula
                    tb.node->transform.position = curPos;
                    tb.node->transform.rotation.x += 360.0f * dt; // flip spin
                }
            }
        }

        // Gentle wind sway on front stall lanterns
        for (size_t i = 0; i < stallLanterns.size(); ++i)
        {
            stallLanterns[i]->transform.rotation.z = std::sin(time * 2.8f + (float)i * 1.5f) * 6.0f;
        }
    }
};

// -------------------------------------------------------------
// 7. Kakigori (Shaved Ice) Stall with Rippling Cloth & Fluttering Flag
// -------------------------------------------------------------
class KakigoriStall
{
public:
    std::shared_ptr<SceneNode> root;
    std::shared_ptr<SceneNode> bannerCloth; // Noren cloth with ripple
    std::shared_ptr<SceneNode> noboriFlag;   // Vertical fluttering banner
    std::shared_ptr<SceneNode> shaverWheel;  // Ice shaver crank wheel
    std::vector<std::shared_ptr<SceneNode>> stallLanterns;

    KakigoriStall(SceneMeshes& meshes, const glm::vec3& pos, float rotY = 0.0f)
    {
        root = std::make_shared<SceneNode>("Kakigori_Stall");
        root->transform.position = pos;
        root->transform.rotation.y = rotY;

        glm::vec4 wood(0.40f, 0.26f, 0.16f, 1.0f);
        glm::vec4 cyan(0.20f, 0.65f, 0.88f, 1.0f);
        glm::vec4 white(0.94f, 0.94f, 0.96f, 1.0f);

        // Counter base table
        auto base = std::make_shared<SceneNode>("Kakigori_Base");
        base->mesh = &meshes.cube;
        base->transform.position = glm::vec3(0.0f, 0.55f, 0.0f);
        base->transform.scale = glm::vec3(3.6f, 1.1f, 1.9f);
        base->color = wood;
        root->addChild(base);

        // Counter board (waist height at Y = 1.15)
        auto counterTop = std::make_shared<SceneNode>("Kakigori_Top");
        counterTop->mesh = &meshes.cube;
        counterTop->transform.position = glm::vec3(0.0f, 1.15f, 0.0f);
        counterTop->transform.scale = glm::vec3(4.0f, 0.10f, 2.2f);
        counterTop->color = glm::vec4(0.30f, 0.18f, 0.10f, 1.0f);
        root->addChild(counterTop);

        // Awning roof
        auto awning = std::make_shared<SceneNode>("Kakigori_Awning");
        awning->mesh = &meshes.cube;
        awning->transform.position = glm::vec3(0.0f, 3.25f, 0.0f);
        awning->transform.rotation.x = -8.0f;
        awning->transform.scale = glm::vec3(4.4f, 0.18f, 2.6f);
        awning->color = cyan;
        root->addChild(awning);

        // Rippling Noren Banner
        bannerCloth = std::make_shared<SceneNode>("Kakigori_NorenCloth");
        bannerCloth->mesh = &meshes.cube;
        bannerCloth->transform.position = glm::vec3(0.0f, 2.90f, 1.18f);
        bannerCloth->transform.scale = glm::vec3(4.0f, 0.50f, 0.06f);
        bannerCloth->color = white;
        root->addChild(bannerCloth);

        // Traditional Kakigori "Ice" Kanji logo accent
        auto kanjiSign = std::make_shared<SceneNode>("Kakigori_IceSign");
        kanjiSign->mesh = &meshes.cube;
        kanjiSign->transform.position = glm::vec3(0.0f, 2.90f, 1.22f);
        kanjiSign->transform.scale = glm::vec3(0.9f, 0.42f, 0.04f);
        kanjiSign->color = glm::vec4(0.85f, 0.15f, 0.15f, 1.0f); // red ice symbol
        root->addChild(kanjiSign);

        // Shaved ice machine replica on counter
        auto shaverBody = std::make_shared<SceneNode>("Shaver_Body");
        shaverBody->mesh = &meshes.cube;
        shaverBody->transform.position = glm::vec3(0.4f, 1.55f, 0.1f);
        shaverBody->transform.scale = glm::vec3(0.55f, 0.65f, 0.55f);
        shaverBody->color = cyan;
        root->addChild(shaverBody);

        shaverWheel = std::make_shared<SceneNode>("Shaver_Wheel");
        shaverWheel->mesh = &meshes.cylinder;
        shaverWheel->transform.position = glm::vec3(0.70f, 1.72f, 0.1f);
        shaverWheel->transform.rotation.z = 90.0f;
        shaverWheel->transform.scale = glm::vec3(0.32f, 0.07f, 0.32f);
        shaverWheel->color = glm::vec4(0.85f, 0.20f, 0.20f, 1.0f);
        root->addChild(shaverWheel);

        // Shaved ice bowl with colored syrup
        auto bowl = std::make_shared<SceneNode>("Ice_Bowl");
        bowl->mesh = &meshes.cone;
        bowl->transform.position = glm::vec3(0.4f, 1.35f, 0.1f);
        bowl->transform.rotation.x = 180.0f;
        bowl->transform.scale = glm::vec3(0.35f, 0.20f, 0.35f);
        bowl->color = glm::vec4(0.9f, 0.95f, 1.0f, 1.0f);
        root->addChild(bowl);

        auto iceMound = std::make_shared<SceneNode>("Ice_Mound");
        iceMound->mesh = &meshes.sphere;
        iceMound->transform.position = glm::vec3(0.4f, 1.48f, 0.1f);
        iceMound->transform.scale = glm::vec3(0.32f, 0.28f, 0.32f);
        iceMound->color = glm::vec4(0.20f, 0.85f, 0.95f, 1.0f); // Blue Hawaii syrup!
        root->addChild(iceMound);

        // Fluttering Nobori Flag on a bamboo pole next to stall
        auto flagPole = std::make_shared<SceneNode>("Nobori_Pole");
        flagPole->mesh = &meshes.cylinder;
        flagPole->transform.position = glm::vec3(2.2f, 2.5f, 0.9f);
        flagPole->transform.scale = glm::vec3(0.08f, 5.0f, 0.08f);
        flagPole->color = glm::vec4(0.55f, 0.45f, 0.25f, 1.0f); // bamboo
        root->addChild(flagPole);

        noboriFlag = std::make_shared<SceneNode>("Nobori_FlagCloth");
        noboriFlag->mesh = &meshes.cube;
        // Anchor edge at pole: offset center by half width
        noboriFlag->transform.position = glm::vec3(2.65f, 3.5f, 0.9f);
        noboriFlag->transform.scale = glm::vec3(0.80f, 2.6f, 0.05f);
        noboriFlag->color = cyan;
        root->addChild(noboriFlag);

        // Hanging Front Stall Lanterns (Ko-Chochin emitting warm night light)
        float slX[2] = { -1.75f, 1.75f };
        for (int l = 0; l < 2; ++l)
        {
            auto pivot = std::make_shared<SceneNode>("KakiStall_LanternPivot_" + std::to_string(l));
            pivot->transform.position = glm::vec3(slX[l], 3.05f, 1.15f);

            auto cord = std::make_shared<SceneNode>("KakiStall_Cord_" + std::to_string(l));
            cord->mesh = &meshes.cylinder;
            cord->transform.position = glm::vec3(0.0f, -0.15f, 0.0f);
            cord->transform.scale = glm::vec3(0.02f, 0.30f, 0.02f);
            cord->color = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);
            pivot->addChild(cord);

            auto cTop = std::make_shared<SceneNode>("KakiStall_CapT_" + std::to_string(l));
            cTop->mesh = &meshes.cylinder;
            cTop->transform.position = glm::vec3(0.0f, -0.32f, 0.0f);
            cTop->transform.scale = glm::vec3(0.26f, 0.06f, 0.26f);
            cTop->color = glm::vec4(0.12f, 0.12f, 0.12f, 1.0f);
            pivot->addChild(cTop);

            auto paper = std::make_shared<SceneNode>("KakiStall_Paper_" + std::to_string(l));
            paper->mesh = &meshes.sphere;
            paper->transform.position = glm::vec3(0.0f, -0.56f, 0.0f);
            paper->transform.scale = glm::vec3(0.42f, 0.50f, 0.42f);
            paper->color = glm::vec4(0.22f, 0.70f, 0.95f, 1.0f); // Festive ice cyan paper
            paper->isEmissive = true;
            paper->emissiveColor = glm::vec3(0.45f, 0.95f, 1.25f); // Bright night glow
            pivot->addChild(paper);

            auto band = std::make_shared<SceneNode>("KakiStall_Band_" + std::to_string(l));
            band->mesh = &meshes.cylinder;
            band->transform.position = glm::vec3(0.0f, -0.56f, 0.0f);
            band->transform.scale = glm::vec3(0.44f, 0.16f, 0.44f);
            band->color = glm::vec4(0.95f, 0.92f, 0.88f, 1.0f);
            pivot->addChild(band);

            auto cBot = std::make_shared<SceneNode>("KakiStall_CapB_" + std::to_string(l));
            cBot->mesh = &meshes.cylinder;
            cBot->transform.position = glm::vec3(0.0f, -0.80f, 0.0f);
            cBot->transform.scale = glm::vec3(0.24f, 0.06f, 0.24f);
            cBot->color = glm::vec4(0.12f, 0.12f, 0.12f, 1.0f);
            pivot->addChild(cBot);

            root->addChild(pivot);
            stallLanterns.push_back(pivot);
        }
    }

    void update(float time, float dt)
    {
        // 1. Shaver wheel crank spin
        shaverWheel->transform.rotation.x += 240.0f * dt;

        // 2. Noren banner cloth ripples in the wind
        bannerCloth->transform.rotation.x = std::sin(time * 3.5f) * 6.0f;

        // 3. Nobori flag flutters about its vertical pole
        noboriFlag->transform.rotation.y = std::sin(time * 4.5f) * 15.0f;

        // 4. Gentle wind sway on front stall lanterns
        for (size_t i = 0; i < stallLanterns.size(); ++i)
        {
            stallLanterns[i]->transform.rotation.z = std::sin(time * 2.8f + (float)i * 1.5f) * 6.0f;
        }
    }
};

// -------------------------------------------------------------
// 8. Vendor Figures (Hierarchical Stirring/Serving Rig)
// -------------------------------------------------------------
class VendorFigure
{
public:
    std::shared_ptr<SceneNode> root;
    std::shared_ptr<SceneNode> upperArm;
    std::shared_ptr<SceneNode> foreArm;

    VendorFigure(SceneMeshes& meshes, const std::string& name, const glm::vec3& pos, float rotY, const glm::vec4& robeColor)
    {
        root = std::make_shared<SceneNode>(name);
        root->transform.position = pos;
        root->transform.rotation.y = rotY;

        glm::vec4 skin(0.92f, 0.76f, 0.64f, 1.0f);
        glm::vec4 wood(0.35f, 0.24f, 0.15f, 1.0f);
        glm::vec4 darkPants(0.18f, 0.18f, 0.22f, 1.0f);

        // Raised wooden platform behind counter so the vendor stands tall and visible
        auto platform = std::make_shared<SceneNode>(name + "_Platform");
        platform->mesh = &meshes.cube;
        platform->transform.position = glm::vec3(0.0f, 0.10f, 0.0f);
        platform->transform.scale = glm::vec3(2.2f, 0.20f, 1.2f);
        platform->color = wood;
        root->addChild(platform);

        // Legs standing on platform
        auto legL = std::make_shared<SceneNode>(name + "_LegL");
        legL->mesh = &meshes.cylinder;
        legL->transform.position = glm::vec3(-0.18f, 0.65f, 0.0f);
        legL->transform.scale = glm::vec3(0.16f, 0.90f, 0.16f);
        legL->color = darkPants;
        root->addChild(legL);

        auto legR = std::make_shared<SceneNode>(name + "_LegR");
        legR->mesh = &meshes.cylinder;
        legR->transform.position = glm::vec3(0.18f, 0.65f, 0.0f);
        legR->transform.scale = glm::vec3(0.16f, 0.90f, 0.16f);
        legR->color = darkPants;
        root->addChild(legR);

        // Torso (wearing festive Happi coat)
        auto torso = std::make_shared<SceneNode>(name + "_Torso");
        torso->mesh = &meshes.cube;
        torso->transform.position = glm::vec3(0.0f, 1.55f, 0.0f);
        torso->transform.scale = glm::vec3(0.75f, 0.95f, 0.48f);
        torso->color = robeColor;
        root->addChild(torso);

        // Obi sash belt
        auto obi = std::make_shared<SceneNode>(name + "_Obi");
        obi->mesh = &meshes.cube;
        obi->transform.position = glm::vec3(0.0f, 1.35f, 0.0f);
        obi->transform.scale = glm::vec3(0.78f, 0.22f, 0.52f);
        obi->color = glm::vec4(0.88f, 0.82f, 0.35f, 1.0f);
        root->addChild(obi);

        // Head
        auto head = std::make_shared<SceneNode>(name + "_Head");
        head->mesh = &meshes.sphere;
        head->transform.position = glm::vec3(0.0f, 2.30f, 0.0f);
        head->transform.scale = glm::vec3(0.48f, 0.52f, 0.48f);
        head->color = skin;
        root->addChild(head);

        // Headband (Hachimaki)
        auto band = std::make_shared<SceneNode>(name + "_Hachimaki");
        band->mesh = &meshes.cube;
        band->transform.position = glm::vec3(0.0f, 2.38f, 0.0f);
        band->transform.scale = glm::vec3(0.52f, 0.12f, 0.52f);
        band->color = glm::vec4(0.95f, 0.95f, 0.95f, 1.0f);
        root->addChild(band);

        // Left arm resting naturally near hip
        auto leftArm = std::make_shared<SceneNode>(name + "_LeftArm");
        leftArm->mesh = &meshes.cylinder;
        leftArm->transform.position = glm::vec3(-0.48f, 1.60f, 0.0f);
        leftArm->transform.rotation.z = -22.0f;
        leftArm->transform.scale = glm::vec3(0.15f, 0.75f, 0.15f);
        leftArm->color = robeColor;
        root->addChild(leftArm);

        // Hierarchical Right Arm (Shoulder -> UpperArm -> Elbow -> Forearm -> Utensil)
        upperArm = std::make_shared<SceneNode>(name + "_UpperArm");
        upperArm->transform.position = glm::vec3(0.45f, 1.90f, 0.05f);
        root->addChild(upperArm);

        auto upperMesh = std::make_shared<SceneNode>(name + "_UpperMesh");
        upperMesh->mesh = &meshes.cylinder;
        upperMesh->transform.position = glm::vec3(0.0f, -0.28f, 0.0f);
        upperMesh->transform.scale = glm::vec3(0.16f, 0.55f, 0.16f);
        upperMesh->color = robeColor;
        upperArm->addChild(upperMesh);

        // Forearm node (child of upper arm)
        foreArm = std::make_shared<SceneNode>(name + "_ForeArm");
        foreArm->transform.position = glm::vec3(0.0f, -0.55f, 0.0f);
        upperArm->addChild(foreArm);

        auto foreMesh = std::make_shared<SceneNode>(name + "_ForeMesh");
        foreMesh->mesh = &meshes.cylinder;
        foreMesh->transform.position = glm::vec3(0.0f, -0.25f, 0.15f);
        foreMesh->transform.rotation.x = -45.0f;
        foreMesh->transform.scale = glm::vec3(0.14f, 0.55f, 0.14f);
        foreMesh->color = skin;
        foreArm->addChild(foreMesh);

        // Cooking turner utensil in hand
        auto utensil = std::make_shared<SceneNode>(name + "_Utensil");
        utensil->mesh = &meshes.cylinder;
        utensil->transform.position = glm::vec3(0.0f, -0.42f, 0.35f);
        utensil->transform.rotation.x = -75.0f;
        utensil->transform.scale = glm::vec3(0.04f, 0.50f, 0.04f);
        utensil->color = glm::vec4(0.75f, 0.75f, 0.78f, 1.0f); // steel pick
        foreArm->addChild(utensil);
    }

    void update(float time)
    {
        // Stirring / cooking arm joint kinematics reaching over counter
        upperArm->transform.rotation.x = 22.0f + std::sin(time * 4.0f) * 16.0f;
        upperArm->transform.rotation.y = std::cos(time * 4.0f) * 14.0f;
        foreArm->transform.rotation.x = -28.0f + std::sin(time * 4.0f + 0.5f) * 18.0f;
    }
};

// -------------------------------------------------------------
// 9. Magic Show Stage
// -------------------------------------------------------------
class MagicStage
{
public:
    std::shared_ptr<SceneNode> root;

    MagicStage(SceneMeshes& meshes, const glm::vec3& pos)
    {
        root = std::make_shared<SceneNode>("Magic_Show_Stage");
        root->transform.position = pos;

        // Elevated platform deck
        auto deck = std::make_shared<SceneNode>("Stage_Platform");
        deck->mesh = &meshes.cube;
        deck->transform.position = glm::vec3(0.0f, 0.45f, 0.0f);
        deck->transform.scale = glm::vec3(4.8f, 0.9f, 5.6f);
        deck->color = glm::vec4(0.28f, 0.18f, 0.12f, 1.0f); // dark stage wood
        root->addChild(deck);

        // Red festive carpet trim
        auto carpet = std::make_shared<SceneNode>("Stage_Carpet");
        carpet->mesh = &meshes.cube;
        carpet->transform.position = glm::vec3(0.0f, 0.92f, 0.0f);
        carpet->transform.scale = glm::vec3(4.2f, 0.05f, 5.0f);
        carpet->color = glm::vec4(0.85f, 0.15f, 0.18f, 1.0f);
        root->addChild(carpet);

        // Traditional folding backdrop screen (Byobu) behind magician
        auto screen = std::make_shared<SceneNode>("Stage_Byobu");
        screen->mesh = &meshes.cube;
        screen->transform.position = glm::vec3(0.0f, 2.6f, -2.5f);
        screen->transform.scale = glm::vec3(4.4f, 3.4f, 0.15f);
        screen->color = glm::vec4(0.88f, 0.78f, 0.42f, 1.0f); // gold-leaf screen
        root->addChild(screen);
    }
};

// -------------------------------------------------------------
// 10 & 11. Magician and Floating Magic Orb (Helical Orbit Demo)
// -------------------------------------------------------------
class Magician
{
public:
    std::shared_ptr<SceneNode> root;
    std::shared_ptr<SceneNode> rightHand;   // Moving reference frame
    std::shared_ptr<SceneNode> orbNode;     // Child node in helical orbit relative to hand!
    std::vector<std::shared_ptr<SceneNode>> trailOrbs;

    Magician(SceneMeshes& meshes, const glm::vec3& pos)
    {
        root = std::make_shared<SceneNode>("Magician");
        root->transform.position = pos;

        glm::vec4 robe(0.15f, 0.12f, 0.22f, 1.0f); // mystical midnight indigo
        glm::vec4 gold(0.92f, 0.80f, 0.25f, 1.0f);
        glm::vec4 skin(0.92f, 0.78f, 0.65f, 1.0f);
        glm::vec4 darkPants(0.12f, 0.10f, 0.16f, 1.0f);
        glm::vec4 bootBlack(0.08f, 0.08f, 0.10f, 1.0f);

        // Legs (standing firmly on stage: Magician local Y = 0.0 is stage floor)
        auto legL = std::make_shared<SceneNode>("Magician_LegL");
        legL->mesh = &meshes.cylinder;
        legL->transform.position = glm::vec3(-0.20f, 0.45f, 0.0f);
        legL->transform.scale = glm::vec3(0.18f, 0.80f, 0.18f); // Y in [0.05, 0.85]
        legL->color = darkPants;
        root->addChild(legL);

        auto legR = std::make_shared<SceneNode>("Magician_LegR");
        legR->mesh = &meshes.cylinder;
        legR->transform.position = glm::vec3(0.20f, 0.45f, 0.0f);
        legR->transform.scale = glm::vec3(0.18f, 0.80f, 0.18f); // Y in [0.05, 0.85]
        legR->color = darkPants;
        root->addChild(legR);

        // Formal magician boots resting flush on the stage surface (Y in [0.0, 0.10])
        auto bootL = std::make_shared<SceneNode>("Magician_BootL");
        bootL->mesh = &meshes.cube;
        bootL->transform.position = glm::vec3(-0.20f, 0.05f, 0.05f);
        bootL->transform.scale = glm::vec3(0.20f, 0.10f, 0.32f); // bottom sits at 0.05 - 0.05 = 0.0f!
        bootL->color = bootBlack;
        root->addChild(bootL);

        auto bootR = std::make_shared<SceneNode>("Magician_BootR");
        bootR->mesh = &meshes.cube;
        bootR->transform.position = glm::vec3(0.20f, 0.05f, 0.05f);
        bootR->transform.scale = glm::vec3(0.20f, 0.10f, 0.32f); // bottom sits at 0.05 - 0.05 = 0.0f!
        bootR->color = bootBlack;
        root->addChild(bootR);

        // Cape tail draping behind magician
        auto capeTail = std::make_shared<SceneNode>("Magician_CapeTail");
        capeTail->mesh = &meshes.cube;
        capeTail->transform.position = glm::vec3(0.0f, 0.75f, -0.22f);
        capeTail->transform.rotation.x = 4.0f;
        capeTail->transform.scale = glm::vec3(0.82f, 1.35f, 0.06f);
        capeTail->color = robe;
        root->addChild(capeTail);

        // Body robe
        auto torso = std::make_shared<SceneNode>("Magician_Torso");
        torso->mesh = &meshes.cylinder;
        torso->transform.position = glm::vec3(0.0f, 1.4f, 0.0f);
        torso->transform.scale = glm::vec3(0.8f, 1.3f, 0.6f);
        torso->color = robe;
        root->addChild(torso);

        // Head
        auto head = std::make_shared<SceneNode>("Magician_Head");
        head->mesh = &meshes.sphere;
        head->transform.position = glm::vec3(0.0f, 2.3f, 0.0f);
        head->transform.scale = glm::vec3(0.5f, 0.55f, 0.5f);
        head->color = skin;
        root->addChild(head);

        // Top Hat / Wizard hat
        auto hatBrim = std::make_shared<SceneNode>("Hat_Brim");
        hatBrim->mesh = &meshes.cylinder;
        hatBrim->transform.position = glm::vec3(0.0f, 2.58f, 0.0f);
        hatBrim->transform.scale = glm::vec3(1.1f, 0.08f, 1.1f);
        hatBrim->color = robe;
        root->addChild(hatBrim);

        auto hatCone = std::make_shared<SceneNode>("Hat_Cone");
        hatCone->mesh = &meshes.cone;
        hatCone->transform.position = glm::vec3(0.0f, 3.2f, 0.0f);
        hatCone->transform.scale = glm::vec3(0.7f, 1.2f, 0.7f);
        hatCone->color = robe;
        root->addChild(hatCone);

        // Left Arm (bent gracefully at side / hip)
        auto leftArm = std::make_shared<SceneNode>("Magician_LeftArm");
        leftArm->mesh = &meshes.cylinder;
        leftArm->transform.position = glm::vec3(-0.50f, 1.55f, 0.05f);
        leftArm->transform.rotation.z = 22.0f;
        leftArm->transform.rotation.x = -15.0f;
        leftArm->transform.scale = glm::vec3(0.18f, 0.85f, 0.18f);
        leftArm->color = robe;
        root->addChild(leftArm);

        auto leftHand = std::make_shared<SceneNode>("Magician_LeftHand");
        leftHand->mesh = &meshes.sphere;
        leftHand->transform.position = glm::vec3(-0.65f, 1.20f, 0.15f);
        leftHand->transform.scale = glm::vec3(0.18f, 0.18f, 0.18f);
        leftHand->color = skin;
        root->addChild(leftHand);

        // Raised Right Arm commanding the magic orb
        auto rightArm = std::make_shared<SceneNode>("Magician_RightArm");
        rightArm->mesh = &meshes.cylinder;
        rightArm->transform.position = glm::vec3(0.55f, 1.9f, 0.2f);
        rightArm->transform.rotation = glm::vec3(-40.0f, 20.0f, -30.0f);
        rightArm->transform.scale = glm::vec3(0.18f, 0.9f, 0.18f);
        rightArm->color = robe;
        root->addChild(rightArm);

        // Right Hand Node (serves as the moving reference frame for the orb!)
        rightHand = std::make_shared<SceneNode>("Magician_RightHand");
        rightHand->transform.position = glm::vec3(0.9f, 2.3f, 0.5f);
        root->addChild(rightHand);

        // Hand palm sphere
        auto palm = std::make_shared<SceneNode>("Magician_Palm");
        palm->mesh = &meshes.sphere;
        palm->transform.scale = glm::vec3(0.2f, 0.2f, 0.2f);
        palm->color = skin;
        rightHand->addChild(palm);

        // Magic Wand
        auto wand = std::make_shared<SceneNode>("Magic_Wand");
        wand->mesh = &meshes.cylinder;
        wand->transform.position = glm::vec3(0.1f, 0.25f, 0.0f);
        wand->transform.rotation.z = -35.0f;
        wand->transform.scale = glm::vec3(0.05f, 0.7f, 0.05f);
        wand->color = gold;
        rightHand->addChild(wand);

        // 11. MAGIC TRICK 1: FLOATING ORB (CHILD OF HAND NODE!)
        // This is a direct implementation of "object transformed relative to another object's reference frame"
        orbNode = std::make_shared<SceneNode>("Magic_Orb");
        orbNode->mesh = &meshes.sphere;
        orbNode->transform.scale = glm::vec3(0.35f, 0.35f, 0.35f);
        orbNode->color = glm::vec4(0.3f, 0.85f, 1.0f, 1.0f);
        orbNode->isEmissive = true;
        orbNode->emissiveColor = glm::vec3(0.3f, 0.9f, 1.0f);
        rightHand->addChild(orbNode);

        // 3 trailing comet-tail spheres
        for (int i = 0; i < 3; ++i)
        {
            auto tail = std::make_shared<SceneNode>("Orb_Tail_" + std::to_string(i));
            tail->mesh = &meshes.sphere;
            float s = 0.25f - i * 0.06f;
            tail->transform.scale = glm::vec3(s, s, s);
            tail->color = glm::vec4(0.4f, 0.7f, 1.0f, 1.0f);
            tail->isEmissive = true;
            tail->emissiveColor = glm::vec3(0.4f, 0.7f, 1.0f);
            rightHand->addChild(tail);
            trailOrbs.push_back(tail);
        }
    }

    void update(float time)
    {
        // Hand sways gently
        rightHand->transform.position.y = 2.3f + std::sin(time * 2.0f) * 0.12f;

        // Magic Orb executes a helical 3D orbit around the hand's reference frame:
        // offset = (radius * cos(t), height + sin(2t)*0.2, radius * sin(t))
        float r = 0.8f;
        float speed = 3.2f;
        float orbX = r * std::cos(time * speed);
        float orbZ = r * std::sin(time * speed);
        float orbY = 0.45f + 0.35f * std::sin(time * speed * 2.0f);

        orbNode->transform.position = glm::vec3(orbX, orbY, orbZ);
        orbNode->transform.rotation.y = time * 180.0f;

        // Trailing particles follow with delayed phase
        for (size_t i = 0; i < trailOrbs.size(); ++i)
        {
            float delay = (float)(i + 1) * 0.15f;
            float tx = r * std::cos((time - delay) * speed);
            float tz = r * std::sin((time - delay) * speed);
            float ty = 0.45f + 0.35f * std::sin((time - delay) * speed * 2.0f);
            trailOrbs[i]->transform.position = glm::vec3(tx, ty, tz);
        }
    }
};

// -------------------------------------------------------------
// 12. Magic Trick 2: Vanishing Box
// -------------------------------------------------------------
class VanishingBoxTrick
{
public:
    std::shared_ptr<SceneNode> root;
    std::shared_ptr<SceneNode> boxNode;
    std::shared_ptr<SceneNode> clothNode;

    glm::vec3 spot1{ -1.1f, 1.35f, 0.45f };
    glm::vec3 spot2{ -1.1f, 1.35f, -0.65f };
    float stateTimer = 0.0f;

    VanishingBoxTrick(SceneMeshes& meshes, const glm::vec3& stagePos)
    {
        root = std::make_shared<SceneNode>("Vanishing_Box_Trick");
        root->transform.position = stagePos;

        // Magic Table on stage
        auto table = std::make_shared<SceneNode>("Magic_Table");
        table->mesh = &meshes.cube;
        table->transform.position = glm::vec3(-1.1f, 0.55f, -0.1f);
        table->transform.scale = glm::vec3(1.2f, 1.1f, 2.2f);
        table->color = glm::vec4(0.20f, 0.15f, 0.12f, 1.0f);
        root->addChild(table);

        // The Vanishing Box (scales to 0, teleports to spot 2, reappears!)
        boxNode = std::make_shared<SceneNode>("Vanishing_Box");
        boxNode->mesh = &meshes.cube;
        boxNode->transform.position = spot1;
        boxNode->transform.scale = glm::vec3(0.55f, 0.55f, 0.55f);
        boxNode->color = glm::vec4(0.88f, 0.72f, 0.15f, 1.0f); // shimmering gold leaf
        root->addChild(boxNode);

        // Silk Cloth
        clothNode = std::make_shared<SceneNode>("Magic_Silk_Cloth");
        clothNode->mesh = &meshes.cube;
        clothNode->transform.position = glm::vec3(spot1.x, spot1.y + 0.45f, spot1.z);
        clothNode->transform.scale = glm::vec3(0.7f, 0.08f, 0.7f);
        clothNode->color = glm::vec4(0.85f, 0.15f, 0.25f, 1.0f); // rich crimson silk
        root->addChild(clothNode);
    }

    void update(float dt)
    {
        stateTimer += dt;
        float cycle = std::fmod(stateTimer, 7.0f);

        // State Machine:
        // 0.0s - 1.5s: Box rests at Spot 1, cloth descends onto box
        // 1.5s - 2.5s: Box scales down to 0 (disappears!)
        // 2.5s - 4.0s: Cloth yanks aside and reveals empty spot
        // 4.0s - 5.0s: Cloth moves to Spot 2
        // 5.0s - 6.0s: Box scales up from 0 to 1 at Spot 2 (reappearance!)
        // 6.0s - 7.0s: Reset transition back to Spot 1

        if (cycle < 1.5f)
        {
            // Box visible at Spot 1
            boxNode->transform.position = spot1;
            boxNode->transform.scale = glm::vec3(0.65f, 0.65f, 0.65f);

            // Cloth hovers down over box
            float t = cycle / 1.5f;
            clothNode->transform.position = glm::vec3(spot1.x, spot1.y + 0.45f - t * 0.15f, spot1.z);
            clothNode->transform.rotation = glm::vec3(0.0f);
        }
        else if (cycle < 2.5f)
        {
            // SCALE-TO-ZERO TRANSFORM DEMO
            float t = (cycle - 1.5f) / 1.0f;
            float s = glm::mix(0.65f, 0.001f, t);
            boxNode->transform.scale = glm::vec3(s, s, s);
            clothNode->transform.position = glm::vec3(spot1.x, spot1.y + 0.3f, spot1.z);
        }
        else if (cycle < 4.0f)
        {
            // Box is invisible
            boxNode->transform.scale = glm::vec3(0.0f);

            // Cloth is whipped aside dramatically (fast translate + rotate)
            float t = (cycle - 2.5f) / 1.5f;
            clothNode->transform.position = glm::vec3(spot1.x + t * 1.2f, spot1.y + 0.8f + t * 0.4f, spot1.z);
            clothNode->transform.rotation = glm::vec3(t * 45.0f, t * 60.0f, t * 30.0f);
        }
        else if (cycle < 5.0f)
        {
            // Cloth flies over to Spot 2
            float t = (cycle - 4.0f) / 1.0f;
            clothNode->transform.position = glm::mix(glm::vec3(spot1.x + 1.2f, spot1.y + 1.2f, spot1.z),
                                                      glm::vec3(spot2.x, spot2.y + 0.4f, spot2.z), t);
            clothNode->transform.rotation = glm::mix(glm::vec3(45.0f, 60.0f, 30.0f), glm::vec3(0.0f), t);
            boxNode->transform.position = spot2;
            boxNode->transform.scale = glm::vec3(0.0f);
        }
        else if (cycle < 6.0f)
        {
            // REAPPEARANCE TRANSFORM (Scale from 0 back to full 1.0)
            float t = (cycle - 5.0f) / 1.0f;
            float s = glm::mix(0.001f, 0.65f, t);
            boxNode->transform.position = spot2;
            boxNode->transform.scale = glm::vec3(s, s, s);
            clothNode->transform.position = glm::vec3(spot2.x + t * 0.8f, spot2.y + 0.5f + t * 0.3f, spot2.z);
        }
        else
        {
            // Transition reset
            float t = (cycle - 6.0f) / 1.0f;
            boxNode->transform.position = glm::mix(spot2, spot1, t);
            clothNode->transform.position = glm::mix(clothNode->transform.position, glm::vec3(spot1.x, spot1.y + 0.5f, spot1.z), t);
        }
    }
};

// -------------------------------------------------------------
// 13. Spotlight Rig (Tracking Moving Light Demo)
// -------------------------------------------------------------
class SpotlightRig
{
public:
    std::shared_ptr<SceneNode> root;
    std::shared_ptr<SceneNode> lampHousing;

    SpotlightRig(SceneMeshes& meshes, const glm::vec3& pos)
    {
        root = std::make_shared<SceneNode>("Stage_Spotlight_Rig");
        root->transform.position = pos;

        // Tall steel pole
        auto pole = std::make_shared<SceneNode>("Spotlight_Pole");
        pole->mesh = &meshes.cylinder;
        pole->transform.position = glm::vec3(0.0f, 3.5f, 0.0f);
        pole->transform.scale = glm::vec3(0.12f, 7.0f, 0.12f);
        pole->color = glm::vec4(0.2f, 0.2f, 0.22f, 1.0f);
        root->addChild(pole);

        // Cross arm
        auto arm = std::make_shared<SceneNode>("Spotlight_Arm");
        arm->mesh = &meshes.cylinder;
        arm->transform.position = glm::vec3(0.5f, 6.8f, 0.0f);
        arm->transform.rotation.z = 90.0f;
        arm->transform.scale = glm::vec3(0.08f, 1.0f, 0.08f);
        arm->color = glm::vec4(0.2f, 0.2f, 0.22f, 1.0f);
        root->addChild(arm);

        // Movable lamp housing (Cone + emissive lens)
        lampHousing = std::make_shared<SceneNode>("Lamp_Housing");
        lampHousing->transform.position = glm::vec3(0.9f, 6.8f, 0.0f);
        root->addChild(lampHousing);

        auto cone = std::make_shared<SceneNode>("Housing_Cone");
        cone->mesh = &meshes.cone;
        cone->transform.rotation.x = -90.0f;
        cone->transform.scale = glm::vec3(0.55f, 0.85f, 0.55f);
        cone->color = glm::vec4(0.15f, 0.15f, 0.15f, 1.0f);
        lampHousing->addChild(cone);

        auto lens = std::make_shared<SceneNode>("Lamp_Lens");
        lens->mesh = &meshes.cylinder;
        lens->transform.position = glm::vec3(0.0f, 0.0f, 0.42f);
        lens->transform.rotation.x = 90.0f;
        lens->transform.scale = glm::vec3(0.52f, 0.05f, 0.52f);
        lens->color = glm::vec4(1.0f, 0.95f, 0.8f, 1.0f);
        lens->isEmissive = true;
        lens->emissiveColor = glm::vec3(1.2f, 1.1f, 0.9f);
        lampHousing->addChild(lens);
    }

    void update(float time)
    {
        // Spotlight pans and tilts smoothly tracking the stage
        lampHousing->transform.rotation.y = -35.0f + std::sin(time * 1.5f) * 20.0f;
        lampHousing->transform.rotation.x = 30.0f + std::cos(time * 1.2f) * 10.0f;
    }
};

// -------------------------------------------------------------
// 14. Audience Figures (Seated on Benches in Front of Magic Stage)
// -------------------------------------------------------------
class AudienceGroup
{
public:
    std::shared_ptr<SceneNode> root;
    std::vector<std::shared_ptr<SceneNode>> heads;

    AudienceGroup(SceneMeshes& meshes, const glm::vec3& stageCenter)
    {
        root = std::make_shared<SceneNode>("Audience_Group");

        std::vector<glm::vec4> yukataColors = {
            { 0.20f, 0.45f, 0.65f, 1.0f }, // indigo blue
            { 0.78f, 0.25f, 0.35f, 1.0f }, // festive cherry blossom red
            { 0.30f, 0.55f, 0.35f, 1.0f }, // matcha green
            { 0.85f, 0.65f, 0.20f, 1.0f }, // golden yellow
            { 0.48f, 0.30f, 0.62f, 1.0f }, // royal purple
            { 0.25f, 0.55f, 0.58f, 1.0f }  // turquoise teal
        };

        glm::vec4 benchWood(0.35f, 0.22f, 0.14f, 1.0f);
        glm::vec4 redFelt(0.82f, 0.16f, 0.18f, 1.0f); // traditional Mousen festival cloth
        glm::vec4 skin(0.92f, 0.76f, 0.64f, 1.0f);
        glm::vec4 darkHair(0.12f, 0.12f, 0.14f, 1.0f);
        glm::vec4 goldObi(0.88f, 0.80f, 0.35f, 1.0f);

        // 2 spectator festival benches placed in front of the stage (in +Z direction from stageCenter)
        // Completely clear of the central street (X <= 3.5), directly facing the magician (-Z direction)
        // All elements strictly sit at Y >= 0.0f (no mesh penetrates below ground level!)
        struct BenchConfig {
            float relZ; // Offset in +Z from stageCenter
            float rotY; // Facing angle toward stage
        };

        BenchConfig benches[2] = {
            { 4.5f, 180.0f }, // Front row bench (at Z = stageCenter.z + 4.5f)
            { 6.3f, 180.0f }  // Back row bench  (at Z = stageCenter.z + 6.3f)
        };

        float seatOffsetsX[3] = { -1.15f, 0.0f, 1.15f };
        int personIndex = 0;

        for (int b = 0; b < 2; ++b)
        {
            glm::vec3 benchCenter = stageCenter + glm::vec3(0.0f, 0.0f, benches[b].relZ);

            auto benchNode = std::make_shared<SceneNode>("Spectator_Bench_" + std::to_string(b));
            benchNode->transform.position = benchCenter;
            benchNode->transform.rotation.y = benches[b].rotY;

            // 4 Bench wooden support legs (resting flush on ground: Y in [0.0, 0.4])
            float legX[2] = { -1.35f, 1.35f };
            float legZ[2] = { -0.22f, 0.22f };
            for (int lx = 0; lx < 2; ++lx)
            {
                for (int lz = 0; lz < 2; ++lz)
                {
                    auto leg = std::make_shared<SceneNode>("BenchLeg_" + std::to_string(b) + "_" + std::to_string(lx) + "_" + std::to_string(lz));
                    leg->mesh = &meshes.cylinder;
                    leg->transform.position = glm::vec3(legX[lx], 0.20f, legZ[lz]);
                    leg->transform.scale = glm::vec3(0.10f, 0.40f, 0.10f); // Y in [0.0, 0.4]
                    leg->color = benchWood;
                    benchNode->addChild(leg);
                }
            }

            // Sturdy wooden seat plank (Y in [0.40, 0.50])
            auto plank = std::make_shared<SceneNode>("BenchPlank_" + std::to_string(b));
            plank->mesh = &meshes.cube;
            plank->transform.position = glm::vec3(0.0f, 0.45f, 0.0f);
            plank->transform.scale = glm::vec3(3.3f, 0.10f, 0.65f); // Y in [0.40, 0.50]
            plank->color = benchWood;
            benchNode->addChild(plank);

            // Red Mousen fabric cover drape (Y in [0.50, 0.52])
            auto mousen = std::make_shared<SceneNode>("BenchMousen_" + std::to_string(b));
            mousen->mesh = &meshes.cube;
            mousen->transform.position = glm::vec3(0.0f, 0.51f, 0.0f);
            mousen->transform.scale = glm::vec3(3.35f, 0.02f, 0.68f); // Y in [0.50, 0.52]
            mousen->color = redFelt;
            benchNode->addChild(mousen);

            root->addChild(benchNode);

            // Seat 3 spectators on this bench
            for (int s = 0; s < 3; ++s)
            {
                auto person = std::make_shared<SceneNode>("Audience_" + std::to_string(personIndex));
                // Position relative to stageCenter: benchCenter + offset along local X
                person->transform.position = benchCenter + glm::vec3(seatOffsetsX[s], 0.0f, 0.0f);

                // Subtle inward facing angle so edge spectators look towards center stage
                float inwardYaw = (benches[b].rotY) + seatOffsetsX[s] * 7.0f;
                person->transform.rotation.y = inwardYaw;

                glm::vec4 yukata = yukataColors[personIndex % yukataColors.size()];

                // Seated lap / lower body resting on bench felt (Y in [0.52, 0.72])
                auto lap = std::make_shared<SceneNode>("Aud_Lap_" + std::to_string(personIndex));
                lap->mesh = &meshes.cube;
                lap->transform.position = glm::vec3(0.0f, 0.62f, 0.06f);
                lap->transform.scale = glm::vec3(0.50f, 0.20f, 0.44f);
                lap->color = yukata;
                person->addChild(lap);

                // Lower Legs extending from seat down to ground (Y in [0.06, 0.50])
                auto legL = std::make_shared<SceneNode>("Aud_LegL_" + std::to_string(personIndex));
                legL->mesh = &meshes.cylinder;
                legL->transform.position = glm::vec3(-0.16f, 0.28f, 0.25f);
                legL->transform.scale = glm::vec3(0.13f, 0.44f, 0.13f);
                legL->color = skin;
                person->addChild(legL);

                auto legR = std::make_shared<SceneNode>("Aud_LegR_" + std::to_string(personIndex));
                legR->mesh = &meshes.cylinder;
                legR->transform.position = glm::vec3(0.16f, 0.28f, 0.25f);
                legR->transform.scale = glm::vec3(0.13f, 0.44f, 0.13f);
                legR->color = skin;
                person->addChild(legR);

                // Traditional Japanese Geta sandals (resting flush on ground: Y in [0.0, 0.07])
                auto getaL = std::make_shared<SceneNode>("Aud_GetaL_" + std::to_string(personIndex));
                getaL->mesh = &meshes.cube;
                getaL->transform.position = glm::vec3(-0.16f, 0.035f, 0.28f);
                getaL->transform.scale = glm::vec3(0.14f, 0.07f, 0.26f); // Lowest face = 0.035 - 0.035 = 0.0f!
                getaL->color = benchWood;
                person->addChild(getaL);

                auto getaR = std::make_shared<SceneNode>("Aud_GetaR_" + std::to_string(personIndex));
                getaR->mesh = &meshes.cube;
                getaR->transform.position = glm::vec3(0.16f, 0.035f, 0.28f);
                getaR->transform.scale = glm::vec3(0.14f, 0.07f, 0.26f); // Lowest face = 0.0f!
                getaR->color = benchWood;
                person->addChild(getaR);

                // Red sandal straps (Hanao)
                auto strapL = std::make_shared<SceneNode>("Aud_StrapL_" + std::to_string(personIndex));
                strapL->mesh = &meshes.cube;
                strapL->transform.position = glm::vec3(-0.16f, 0.075f, 0.31f);
                strapL->transform.scale = glm::vec3(0.12f, 0.02f, 0.08f);
                strapL->color = redFelt;
                person->addChild(strapL);

                auto strapR = std::make_shared<SceneNode>("Aud_StrapR_" + std::to_string(personIndex));
                strapR->mesh = &meshes.cube;
                strapR->transform.position = glm::vec3(0.16f, 0.075f, 0.31f);
                strapR->transform.scale = glm::vec3(0.12f, 0.02f, 0.08f);
                strapR->color = redFelt;
                person->addChild(strapR);

                // Torso upright in Yukata (Y in [0.72, 1.48])
                auto torso = std::make_shared<SceneNode>("Aud_Torso_" + std::to_string(personIndex));
                torso->mesh = &meshes.cube;
                torso->transform.position = glm::vec3(0.0f, 1.10f, 0.0f);
                torso->transform.scale = glm::vec3(0.52f, 0.76f, 0.38f);
                torso->color = yukata;
                person->addChild(torso);

                // Obi sash belt (Y in [0.82, 0.98])
                auto obi = std::make_shared<SceneNode>("Aud_Obi_" + std::to_string(personIndex));
                obi->mesh = &meshes.cube;
                obi->transform.position = glm::vec3(0.0f, 0.90f, 0.0f);
                obi->transform.scale = glm::vec3(0.55f, 0.16f, 0.40f);
                obi->color = goldObi;
                person->addChild(obi);

                // Left Arm resting on knee
                auto armL = std::make_shared<SceneNode>("Aud_ArmL_" + std::to_string(personIndex));
                armL->mesh = &meshes.cylinder;
                armL->transform.position = glm::vec3(-0.31f, 1.00f, 0.10f);
                armL->transform.rotation.x = -32.0f;
                armL->transform.scale = glm::vec3(0.12f, 0.52f, 0.12f);
                armL->color = yukata;
                person->addChild(armL);

                auto handL = std::make_shared<SceneNode>("Aud_HandL_" + std::to_string(personIndex));
                handL->mesh = &meshes.sphere;
                handL->transform.position = glm::vec3(-0.31f, 0.74f, 0.26f);
                handL->transform.scale = glm::vec3(0.12f, 0.12f, 0.12f);
                handL->color = skin;
                person->addChild(handL);

                // Right Arm resting on knee
                auto armR = std::make_shared<SceneNode>("Aud_ArmR_" + std::to_string(personIndex));
                armR->mesh = &meshes.cylinder;
                armR->transform.position = glm::vec3(0.31f, 1.00f, 0.10f);
                armR->transform.rotation.x = -32.0f;
                armR->transform.scale = glm::vec3(0.12f, 0.52f, 0.12f);
                armR->color = yukata;
                person->addChild(armR);

                auto handR = std::make_shared<SceneNode>("Aud_HandR_" + std::to_string(personIndex));
                handR->mesh = &meshes.sphere;
                handR->transform.position = glm::vec3(0.31f, 0.74f, 0.26f);
                handR->transform.scale = glm::vec3(0.12f, 0.12f, 0.12f);
                handR->color = skin;
                person->addChild(handR);

                // Head (animated sway / nod) (Y in [1.41, 1.83])
                auto head = std::make_shared<SceneNode>("Aud_Head_" + std::to_string(personIndex));
                head->mesh = &meshes.sphere;
                head->transform.position = glm::vec3(0.0f, 1.62f, 0.0f);
                head->transform.scale = glm::vec3(0.38f, 0.42f, 0.38f);
                head->color = skin;
                person->addChild(head);
                heads.push_back(head);

                // Traditional festival hair / top knot (Y in [1.66, 1.86])
                auto hair = std::make_shared<SceneNode>("Aud_Hair_" + std::to_string(personIndex));
                hair->mesh = &meshes.sphere;
                hair->transform.position = glm::vec3(0.0f, 1.76f, -0.04f);
                hair->transform.scale = glm::vec3(0.40f, 0.20f, 0.40f);
                hair->color = darkHair;
                person->addChild(hair);

                root->addChild(person);
                personIndex++;
            }
        }
    }

    void update(float time)
    {
        for (size_t i = 0; i < heads.size(); ++i)
        {
            // Spectators attentively nod and turn heads watching the magic performance
            heads[i]->transform.rotation.y = std::sin(time * 2.2f + (float)i * 1.3f) * 7.0f;
            heads[i]->transform.rotation.x = -6.0f + std::cos(time * 1.8f + (float)i * 0.9f) * 4.0f;
        }
    }
};

// -------------------------------------------------------------
// 15. Crowd Figures (Walking Down Street with Collision-Free Paths)
// -------------------------------------------------------------
struct WalkingPerson
{
    std::shared_ptr<SceneNode> root;
    std::shared_ptr<SceneNode> legL;
    std::shared_ptr<SceneNode> legR;
    std::shared_ptr<SceneNode> armL;
    std::shared_ptr<SceneNode> armR;
    float laneX;
    float speed;
    float direction; // -1.0f = toward Torii gate (-Z), +1.0f = toward entrance (+Z)
    float phase;
};

class CrowdGroup
{
public:
    std::shared_ptr<SceneNode> root;
    std::vector<WalkingPerson> walkers;

    CrowdGroup(SceneMeshes& meshes)
    {
        root = std::make_shared<SceneNode>("Crowd_Walking_Group");

        std::vector<glm::vec4> yukataTones = {
            { 0.18f, 0.38f, 0.62f, 1.0f },
            { 0.78f, 0.28f, 0.38f, 1.0f },
            { 0.28f, 0.58f, 0.48f, 1.0f },
            { 0.88f, 0.70f, 0.25f, 1.0f },
            { 0.52f, 0.28f, 0.58f, 1.0f },
            { 0.75f, 0.45f, 0.25f, 1.0f },
            { 0.35f, 0.65f, 0.75f, 1.0f },
            { 0.82f, 0.35f, 0.25f, 1.0f }
        };

        // 8 walkers assigned strictly to the open festival street promenade [-1.6f, +1.4f]
        // This guarantees zero collision with any stall, stage, audience, sakura tree, or houses!
        struct PathDef {
            float laneX;
            float startZ;
            float speed;
            float dir; // -1 = Northbound (to Torii), +1 = Southbound (to entrance)
        };

        std::vector<PathDef> paths = {
            // Northbound walkers (heading toward Torii Gate)
            { -1.3f,  32.0f, 2.3f, -1.0f },
            {  0.7f,  20.0f, 2.0f, -1.0f },
            { -0.4f,   8.0f, 2.4f, -1.0f },
            {  1.3f,  -6.0f, 2.1f, -1.0f },

            // Southbound walkers (heading toward entrance)
            { -0.8f, -26.0f, 2.2f,  1.0f },
            {  0.3f, -14.0f, 2.4f,  1.0f },
            { -1.5f,  -2.0f, 2.0f,  1.0f },
            {  1.1f,  12.0f, 2.1f,  1.0f }
        };

        for (size_t i = 0; i < paths.size(); ++i)
        {
            WalkingPerson wp;
            wp.root = std::make_shared<SceneNode>("Walker_" + std::to_string(i));
            wp.laneX = paths[i].laneX;
            wp.speed = paths[i].speed;
            wp.direction = paths[i].dir;
            wp.phase = (float)i * 1.25f;

            wp.root->transform.position = glm::vec3(wp.laneX, 0.0f, paths[i].startZ);
            // Face travel direction
            wp.root->transform.rotation.y = (wp.direction < 0.0f) ? 180.0f : 0.0f;

            // Torso
            auto torso = std::make_shared<SceneNode>("Walk_Torso_" + std::to_string(i));
            torso->mesh = &meshes.cube;
            torso->transform.position = glm::vec3(0.0f, 1.25f, 0.0f);
            torso->transform.scale = glm::vec3(0.55f, 0.9f, 0.38f);
            torso->color = yukataTones[i % yukataTones.size()];
            wp.root->addChild(torso);

            // Sash (Obi belt)
            auto obi = std::make_shared<SceneNode>("Walk_Obi_" + std::to_string(i));
            obi->mesh = &meshes.cube;
            obi->transform.position = glm::vec3(0.0f, 1.15f, 0.0f);
            obi->transform.scale = glm::vec3(0.58f, 0.22f, 0.42f);
            obi->color = glm::vec4(0.88f, 0.82f, 0.35f, 1.0f);
            wp.root->addChild(obi);

            // Head
            auto head = std::make_shared<SceneNode>("Walk_Head_" + std::to_string(i));
            head->mesh = &meshes.sphere;
            head->transform.position = glm::vec3(0.0f, 1.95f, 0.0f);
            head->transform.scale = glm::vec3(0.40f, 0.44f, 0.40f);
            head->color = glm::vec4(0.92f, 0.76f, 0.64f, 1.0f);
            wp.root->addChild(head);

            // Left Leg
            wp.legL = std::make_shared<SceneNode>("Walk_LegL_" + std::to_string(i));
            wp.legL->mesh = &meshes.cylinder;
            wp.legL->transform.position = glm::vec3(-0.16f, 0.4f, 0.0f);
            wp.legL->transform.scale = glm::vec3(0.14f, 0.8f, 0.14f);
            wp.legL->color = glm::vec4(0.2f, 0.2f, 0.25f, 1.0f);
            wp.root->addChild(wp.legL);

            // Right Leg
            wp.legR = std::make_shared<SceneNode>("Walk_LegR_" + std::to_string(i));
            wp.legR->mesh = &meshes.cylinder;
            wp.legR->transform.position = glm::vec3(0.16f, 0.4f, 0.0f);
            wp.legR->transform.scale = glm::vec3(0.14f, 0.8f, 0.14f);
            wp.legR->color = glm::vec4(0.2f, 0.2f, 0.25f, 1.0f);
            wp.root->addChild(wp.legR);

            // Left Arm Pivot (at left shoulder: Y = 1.55)
            wp.armL = std::make_shared<SceneNode>("Walk_ArmPivotL_" + std::to_string(i));
            wp.armL->transform.position = glm::vec3(-0.35f, 1.55f, 0.0f);

            // Left Yukata Sleeve
            auto sleeveL = std::make_shared<SceneNode>("Walk_SleeveL_" + std::to_string(i));
            sleeveL->mesh = &meshes.cylinder;
            sleeveL->transform.position = glm::vec3(0.0f, -0.22f, 0.0f);
            sleeveL->transform.scale = glm::vec3(0.14f, 0.44f, 0.14f);
            sleeveL->color = yukataTones[i % yukataTones.size()];
            wp.armL->addChild(sleeveL);

            // Left Hand (skin colored)
            auto handL = std::make_shared<SceneNode>("Walk_HandL_" + std::to_string(i));
            handL->mesh = &meshes.sphere;
            handL->transform.position = glm::vec3(0.0f, -0.48f, 0.0f);
            handL->transform.scale = glm::vec3(0.12f, 0.14f, 0.12f);
            handL->color = glm::vec4(0.92f, 0.76f, 0.64f, 1.0f);
            wp.armL->addChild(handL);
            wp.root->addChild(wp.armL);

            // Right Arm Pivot (at right shoulder: Y = 1.55)
            wp.armR = std::make_shared<SceneNode>("Walk_ArmPivotR_" + std::to_string(i));
            wp.armR->transform.position = glm::vec3(0.35f, 1.55f, 0.0f);

            // Right Yukata Sleeve
            auto sleeveR = std::make_shared<SceneNode>("Walk_SleeveR_" + std::to_string(i));
            sleeveR->mesh = &meshes.cylinder;
            sleeveR->transform.position = glm::vec3(0.0f, -0.22f, 0.0f);
            sleeveR->transform.scale = glm::vec3(0.14f, 0.44f, 0.14f);
            sleeveR->color = yukataTones[i % yukataTones.size()];
            wp.armR->addChild(sleeveR);

            // Right Hand (skin colored)
            auto handR = std::make_shared<SceneNode>("Walk_HandR_" + std::to_string(i));
            handR->mesh = &meshes.sphere;
            handR->transform.position = glm::vec3(0.0f, -0.48f, 0.0f);
            handR->transform.scale = glm::vec3(0.12f, 0.14f, 0.12f);
            handR->color = glm::vec4(0.92f, 0.76f, 0.64f, 1.0f);
            wp.armR->addChild(handR);
            wp.root->addChild(wp.armR);

            root->addChild(wp.root);
            walkers.push_back(wp);
        }
    }

    void update(float time, float dt)
    {
        for (auto& wp : walkers)
        {
            // Translate along the festival street in assigned direction
            wp.root->transform.position.z += wp.direction * wp.speed * dt;

            // Seamless loop wrap-around
            if (wp.direction < 0.0f && wp.root->transform.position.z < -30.0f)
            {
                wp.root->transform.position.z = 36.0f;
            }
            else if (wp.direction > 0.0f && wp.root->transform.position.z > 36.0f)
            {
                wp.root->transform.position.z = -30.0f;
            }

            // Gentle natural lane sway while strictly maintaining clearance
            wp.root->transform.position.x = wp.laneX + std::sin(time * 0.8f + wp.phase) * 0.08f;

            // Vertical step bobbing
            wp.root->transform.position.y = std::abs(std::sin(time * 6.0f + wp.phase)) * 0.04f;

            // Facing angle with subtle walking sway
            float baseYaw = (wp.direction < 0.0f) ? 180.0f : 0.0f;
            float turnSway = std::sin(time * 3.0f + wp.phase) * 3.0f;
            wp.root->transform.rotation.y = baseYaw + turnSway;

            // Alternating leg swing walk-cycle
            float walkAngle = std::sin(time * 6.0f + wp.phase) * 26.0f;
            wp.legL->transform.rotation.x = walkAngle;
            wp.legR->transform.rotation.x = -walkAngle;

            // Realistic walking arm swing: arms swing in opposite phase to legs
            float armAngle = -walkAngle * 0.85f;
            wp.armL->transform.rotation.x = armAngle;
            wp.armR->transform.rotation.x = -armAngle;
            wp.armL->transform.rotation.z = -5.0f;
            wp.armR->transform.rotation.z = 5.0f;
        }
    }
};

// -------------------------------------------------------------
// 16. Fireworks Particle System (Night Sky)
// -------------------------------------------------------------
struct Particle
{
    std::shared_ptr<SceneNode> node;
    glm::vec3 velocity;
    float lifetime = 0.0f;
    float maxLife = 1.6f;
};

class FireworkRocket
{
public:
    enum State { LAUNCHING, BURSTING, INACTIVE };
    State state = INACTIVE;

    std::shared_ptr<SceneNode> rocketShell;
    std::vector<Particle> particles;
    glm::vec3 burstPos;
    glm::vec3 color;
    float timer = 0.0f;
    float delay = 0.0f;

    void init(SceneMeshes& meshes, const std::string& prefix, std::shared_ptr<SceneNode> parentNode)
    {
        rocketShell = std::make_shared<SceneNode>(prefix + "_Shell");
        rocketShell->mesh = &meshes.sphere;
        rocketShell->transform.scale = glm::vec3(0.4f, 0.4f, 0.4f);
        rocketShell->isEmissive = true;
        rocketShell->visible = false;
        parentNode->addChild(rocketShell);

        // Pool of 24 burst particles
        for (int i = 0; i < 24; ++i)
        {
            Particle p;
            p.node = std::make_shared<SceneNode>(prefix + "_P_" + std::to_string(i));
            p.node->mesh = &meshes.sphere;
            p.node->isEmissive = true;
            p.node->visible = false;
            parentNode->addChild(p.node);
            particles.push_back(p);
        }
    }

    void launch(const glm::vec3& startPos, const glm::vec3& burstTarget, const glm::vec3& col)
    {
        burstPos = burstTarget;
        color = col;
        rocketShell->transform.position = startPos;
        rocketShell->emissiveColor = color * 1.5f;
        rocketShell->visible = true;
        state = LAUNCHING;
        timer = 0.0f;
    }

    void update(float dt)
    {
        if (state == LAUNCHING)
        {
            timer += dt;
            float t = timer / 1.1f; // launch takes 1.1 seconds
            if (t >= 1.0f)
            {
                // Trigger burst
                state = BURSTING;
                timer = 0.0f;
                rocketShell->visible = false;

                for (size_t i = 0; i < particles.size(); ++i)
                {
                    particles[i].node->transform.position = burstPos;
                    particles[i].node->visible = true;
                    particles[i].node->emissiveColor = color;
                    particles[i].lifetime = 0.0f;

                    // Uniform spherical explosion velocity
                    float phi = (float)rand() / (float)RAND_MAX * 2.0f * (float)M_PI;
                    float cosTheta = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
                    float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);
                    float spd = 6.0f + ((float)rand() / (float)RAND_MAX) * 8.0f;

                    particles[i].velocity = glm::vec3(spd * sinTheta * std::cos(phi),
                                                      spd * cosTheta,
                                                      spd * sinTheta * std::sin(phi));
                }
            }
            else
            {
                // Ease out upward trajectory
                rocketShell->transform.position.y += (burstPos.y - rocketShell->transform.position.y) * 4.0f * dt;
            }
        }
        else if (state == BURSTING)
        {
            timer += dt;
            bool allDead = true;
            for (auto& p : particles)
            {
                p.lifetime += dt;
                if (p.lifetime < p.maxLife)
                {
                    allDead = false;
                    // Apply velocity and gravity
                    p.node->transform.position += p.velocity * dt;
                    p.velocity.y -= 7.5f * dt; // gravity

                    // Scale shrinks over time
                    float lifeRatio = 1.0f - (p.lifetime / p.maxLife);
                    float s = 0.35f * lifeRatio;
                    p.node->transform.scale = glm::vec3(s, s, s);
                    p.node->emissiveColor = color * (lifeRatio * 1.5f);
                }
                else
                {
                    p.node->visible = false;
                }
            }

            if (allDead)
            {
                state = INACTIVE;
            }
        }
    }
};

class FireworkSystem
{
public:
    std::shared_ptr<SceneNode> root;
    std::vector<FireworkRocket> rockets;
    float spawnCooldown = 1.0f;

    FireworkSystem(SceneMeshes& meshes)
    {
        root = std::make_shared<SceneNode>("Firework_System");

        for (int i = 0; i < 4; ++i)
        {
            FireworkRocket r;
            r.init(meshes, "Fw_" + std::to_string(i), root);
            r.delay = (float)i * 1.2f;
            rockets.push_back(r);
        }
    }

    void triggerBurstNow()
    {
        for (auto& r : rockets)
        {
            if (r.state == FireworkRocket::INACTIVE)
            {
                float x = ((rand() % 100) / 100.0f - 0.5f) * 40.0f;
                float z = -15.0f - (rand() % 30);
                float y = 28.0f + (rand() % 15);
                glm::vec3 col = getNextColor();
                r.launch(glm::vec3(x, 1.0f, z), glm::vec3(x, y, z), col);
                break;
            }
        }
    }

    void update(float dt, bool isNight)
    {
        for (auto& r : rockets)
        {
            r.update(dt);
        }

        // Periodically launch fireworks automatically during night mode
        if (isNight)
        {
            spawnCooldown -= dt;
            if (spawnCooldown <= 0.0f)
            {
                triggerBurstNow();
                spawnCooldown = 1.5f + ((float)rand() / (float)RAND_MAX) * 1.5f;
            }
        }
    }

private:
    glm::vec3 getNextColor()
    {
        static const glm::vec3 cols[] = {
            { 1.0f, 0.25f, 0.25f }, // crimson
            { 1.0f, 0.85f, 0.20f }, // gold
            { 0.25f, 0.90f, 1.0f }, // cyan
            { 0.85f, 0.35f, 1.0f }, // violet
            { 0.35f, 1.0f, 0.45f }  // emerald
        };
        int idx = rand() % 5;
        return cols[idx];
    }
};

// -------------------------------------------------------------
// 17. Sky Dome (Day / Night Atmospheric Gradient)
// -------------------------------------------------------------
class SkyDome
{
public:
    std::shared_ptr<SceneNode> root;

    SkyDome(SceneMeshes& meshes)
    {
        root = std::make_shared<SceneNode>("Sky_Dome");
        root->mesh = &meshes.sphere;
        root->transform.scale = glm::vec3(180.0f, 180.0f, 180.0f);
        root->isSky = true;
    }
};
