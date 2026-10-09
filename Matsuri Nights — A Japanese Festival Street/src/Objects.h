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
#include <algorithm>

// Shared primitive meshes created once and reused across all scene nodes
struct SceneMeshes
{
    Mesh cube;
    Mesh cylinder;
    Mesh cone;
    Mesh sphere;
    Mesh plane;

    // Curved Geometry Primitives
    Mesh curvedKasagi;
    Mesh curvedShimaki;
    Mesh catenaryRope;
    Mesh bonsaiTrunkCurved;
    Mesh bonsaiBranchCurved1;
    Mesh bonsaiBranchCurved2;
    Mesh ikebanaStemShin;
    Mesh ikebanaStemSoe;
    Mesh ikebanaStemHikae;
    Mesh ikebanaStemAccent;
    Mesh planterStemCurvedA;
    Mesh planterStemCurvedB;
    Mesh kokedamaVineCurved1;
    Mesh kokedamaVineCurved2;

    // Botanical Curve Meshes
    Mesh curvedLeaf;
    Mesh curvedPetal;
    Mesh sakuraCanopyLobe;
    Mesh pineCluster;

    // Anatomical Articulated Human Meshes
    Mesh humanHead;
    Mesh humanTorso;
    Mesh limbThigh;
    Mesh limbShin;
    Mesh limbUpperArm;
    Mesh limbForearm;
    Mesh humanHand;
    Mesh getaFoot;

    void init()
    {
        cube = Primitives::createCube(1.0f);
        cylinder = Primitives::createCylinder(0.5f, 1.0f, 24);
        cone = Primitives::createCone(0.5f, 1.0f, 24);
        sphere = Primitives::createSphere(0.5f, 20, 24);
        plane = Primitives::createPlane(1.0f, 1.0f, 4, 4);

        // 1. Curved Torii Gate Kasagi & Shimaki (Iconic upward sori arch & beveled roof cap)
        curvedKasagi = Primitives::createCurvedBeam(15.4f, 1.25f, 0.55f, 0.40f, 1.15f, 32, true);
        curvedShimaki = Primitives::createCurvedBeam(14.2f, 0.95f, 0.65f, 0.28f, 1.08f, 32, false);

        // 2. Continuous Catenary Rope
        catenaryRope = Primitives::createCatenaryRope(3.8f, 6.20f, 0.65f, 0.035f, 36, 12);

        // 3. Curved Bonsai Trunk & Branches (Windswept Moyogi style spline)
        bonsaiTrunkCurved = Primitives::createSplineTube(
            { { 0.0f, 0.0f, 0.0f }, { -0.06f, 0.12f, 0.02f }, { -0.02f, 0.26f, 0.04f }, { 0.04f, 0.38f, -0.01f }, { 0.01f, 0.48f, 0.01f } },
            0.065f, 0.030f, 24, 14, 2.0f, true, true);
        bonsaiBranchCurved1 = Primitives::createBezierTube(
            Curves::Bezier3({ 0.0f, 0.0f, 0.0f }, { -0.05f, 0.03f, 0.02f }, { -0.11f, 0.05f, 0.03f }, { -0.16f, 0.04f, 0.05f }),
            0.030f, 0.016f, 16, 12, 1.0f, true, true);
        bonsaiBranchCurved2 = Primitives::createBezierTube(
            Curves::Bezier3({ 0.0f, 0.0f, 0.0f }, { 0.05f, 0.03f, -0.02f }, { 0.11f, 0.05f, -0.04f }, { 0.16f, 0.04f, -0.06f }),
            0.028f, 0.015f, 16, 12, 1.0f, true, true);

        // 4. Curved Ikebana Stems (Living lines: Shin, Soe, Hikae, Accent)
        ikebanaStemShin = Primitives::createBezierTube(
            Curves::Bezier3({ 0.0f, 0.0f, 0.0f }, { 0.02f, 0.14f, -0.02f }, { 0.05f, 0.26f, -0.04f }, { 0.07f, 0.38f, -0.05f }),
            0.018f, 0.011f, 20, 14, 1.0f, true, true);
        ikebanaStemSoe = Primitives::createBezierTube(
            Curves::Bezier3({ 0.0f, 0.0f, 0.0f }, { -0.05f, 0.10f, 0.04f }, { -0.12f, 0.20f, 0.09f }, { -0.15f, 0.30f, 0.12f }),
            0.016f, 0.010f, 20, 14, 1.0f, true, true);
        ikebanaStemHikae = Primitives::createBezierTube(
            Curves::Bezier3({ 0.0f, 0.0f, 0.0f }, { 0.07f, 0.08f, -0.05f }, { 0.15f, 0.16f, -0.12f }, { 0.20f, 0.22f, -0.18f }),
            0.015f, 0.009f, 20, 14, 1.0f, true, true);
        ikebanaStemAccent = Primitives::createBezierTube(
            Curves::Bezier3({ 0.0f, 0.0f, 0.0f }, { -0.04f, 0.06f, -0.04f }, { -0.08f, 0.13f, -0.08f }, { -0.11f, 0.20f, -0.11f }),
            0.014f, 0.008f, 18, 12, 1.0f, true, true);

        // 5. Curved Window Planter Flower Stems
        planterStemCurvedA = Primitives::createBezierTube(
            Curves::Bezier3({ 0.0f, 0.0f, 0.0f }, { 0.02f, 0.05f, 0.01f }, { 0.035f, 0.10f, 0.02f }, { 0.03f, 0.15f, 0.025f }),
            0.011f, 0.008f, 16, 12, 1.0f, true, true);
        planterStemCurvedB = Primitives::createBezierTube(
            Curves::Bezier3({ 0.0f, 0.0f, 0.0f }, { -0.015f, 0.05f, 0.015f }, { -0.03f, 0.10f, 0.025f }, { -0.025f, 0.15f, 0.03f }),
            0.011f, 0.008f, 16, 12, 1.0f, true, true);

        // 6. Curved Hanging Kokedama Vines
        kokedamaVineCurved1 = Primitives::createBezierTube(
            Curves::Bezier3({ 0.0f, 0.0f, 0.0f }, { 0.03f, -0.08f, 0.02f }, { 0.06f, -0.18f, 0.04f }, { 0.04f, -0.28f, 0.05f }),
            0.016f, 0.009f, 18, 12, 1.0f, true, true);
        kokedamaVineCurved2 = Primitives::createBezierTube(
            Curves::Bezier3({ 0.0f, 0.0f, 0.0f }, { -0.02f, -0.07f, 0.03f }, { -0.05f, -0.17f, 0.05f }, { -0.03f, -0.27f, 0.06f }),
            0.016f, 0.009f, 18, 12, 1.0f, true, true);

        // 7. Botanical Curve Meshes (curved leaves, cupped petals, organic blossom billows, pine pads)
        curvedLeaf = Primitives::createCurvedLeaf(0.12f, 0.045f, 0.02f, 25.0f, 10, 6);
        curvedPetal = Primitives::createCurvedPetal(0.065f, 0.048f, 0.016f, 8, 6);
        sakuraCanopyLobe = Primitives::createSakuraBlossomLobe(1.0f, 18, 22, 0.24f);
        pineCluster = Primitives::createPineNeedleCluster(0.24f, 0.08f, 0.20f, 14, 18);

        // 8. Anatomical Articulated Human Meshes (contoured face, kimono collar torso, articulated limbs, geta)
        humanHead = Primitives::createHumanHead(0.20f);
        humanTorso = Primitives::createHumanTorso(0.52f, 0.85f, 0.36f);
        limbThigh = Primitives::createArticulatedLimb(0.09f, 0.07f, 0.42f, 12, 14);
        limbShin = Primitives::createArticulatedLimb(0.07f, 0.055f, 0.42f, 12, 14);
        limbUpperArm = Primitives::createArticulatedLimb(0.075f, 0.06f, 0.38f, 10, 14);
        limbForearm = Primitives::createArticulatedLimb(0.06f, 0.048f, 0.36f, 10, 14);
        humanHand = Primitives::createHand(0.14f, 0.08f, 0.035f);
        getaFoot = Primitives::createGetaFoot(0.25f, 0.12f, 0.065f);
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
// -------------------------------------------------------------
// Helper Object: Traditional Japanese Bonsai Tree (盆栽)
// -------------------------------------------------------------
inline std::shared_ptr<SceneNode> createBonsaiTree(SceneMeshes& meshes, const std::string& prefix, const glm::vec3& pos, float scale = 1.0f, float rotY = 0.0f)
{
    auto root = std::make_shared<SceneNode>(prefix + "_Bonsai");
    root->transform.position = pos;
    root->transform.rotation.y = rotY;
    root->transform.scale = glm::vec3(scale);

    glm::vec4 ceramicPot(0.18f, 0.28f, 0.42f, 1.0f); // dark cobalt glazed ceramic pot
    glm::vec4 darkSoil(0.14f, 0.10f, 0.07f, 1.0f);   // rich dark mossy potting soil
    glm::vec4 trunkWood(0.32f, 0.20f, 0.12f, 1.0f);  // gnarled weathered bark
    glm::vec4 foliagePine(0.12f, 0.36f, 0.18f, 1.0f); // deep evergreen juniper
    glm::vec4 foliageLight(0.18f, 0.46f, 0.24f, 1.0f);// fresh pine tip green
    glm::vec4 rockColor(0.38f, 0.38f, 0.40f, 1.0f);  // miniature suiseki rock

    // Shallow Glazed Ceramic Pot (Tray)
    auto pot = std::make_shared<SceneNode>(prefix + "_Pot");
    pot->mesh = &meshes.cube;
    pot->transform.position = glm::vec3(0.0f, 0.06f, 0.0f);
    pot->transform.scale = glm::vec3(0.46f, 0.12f, 0.34f);
    pot->color = ceramicPot;
    root->addChild(pot);

    // 4 Tiny Pot Feet
    float fx[2] = { -0.19f, 0.19f };
    float fz[2] = { -0.13f, 0.13f };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            auto foot = std::make_shared<SceneNode>(prefix + "_Foot_" + std::to_string(i) + "_" + std::to_string(j));
            foot->mesh = &meshes.cube;
            foot->transform.position = glm::vec3(fx[i], 0.015f, fz[j]);
            foot->transform.scale = glm::vec3(0.04f, 0.03f, 0.04f);
            foot->color = ceramicPot;
            root->addChild(foot);
        }
    }

    // Soil Bed
    auto soil = std::make_shared<SceneNode>(prefix + "_Soil");
    soil->mesh = &meshes.cube;
    soil->transform.position = glm::vec3(0.0f, 0.12f, 0.0f);
    soil->transform.scale = glm::vec3(0.42f, 0.02f, 0.30f);
    soil->color = darkSoil;
    root->addChild(soil);

    // Decorative Miniature Accent Stone (Suiseki)
    auto rock = std::make_shared<SceneNode>(prefix + "_Suiseki");
    rock->mesh = &meshes.sphere;
    rock->transform.position = glm::vec3(0.12f, 0.15f, -0.06f);
    rock->transform.scale = glm::vec3(0.08f, 0.06f, 0.07f);
    rock->color = rockColor;
    root->addChild(rock);

    // Gnarled Twisting Bonsai Trunk (continuous organic swept 3D spline curve)
    auto trunk = std::make_shared<SceneNode>(prefix + "_TrunkCurved");
    trunk->mesh = &meshes.bonsaiTrunkCurved;
    trunk->transform.position = glm::vec3(0.0f, 0.12f, 0.0f);
    trunk->color = trunkWood;
    root->addChild(trunk);

    // Miniature Bonsai Branches (swept curved limbs arching gracefully under foliage pads)
    auto br1 = std::make_shared<SceneNode>(prefix + "_Branch1");
    br1->mesh = &meshes.bonsaiBranchCurved1;
    br1->transform.position = glm::vec3(-0.02f, 0.38f, 0.04f);
    br1->color = trunkWood;
    root->addChild(br1);

    auto br2 = std::make_shared<SceneNode>(prefix + "_Branch2");
    br2->mesh = &meshes.bonsaiBranchCurved2;
    br2->transform.position = glm::vec3(0.04f, 0.44f, -0.01f);
    br2->color = trunkWood;
    root->addChild(br2);

    // Sculpted Cloud Foliage Pads (Evergreen Pine tiers)
    struct PadDef { glm::vec3 offset; glm::vec3 scale; glm::vec4 col; };
    PadDef pads[5] = {
        { { -0.16f, 0.38f,  0.06f }, { 0.20f, 0.07f, 0.16f }, foliagePine },
        { {  0.15f, 0.46f, -0.05f }, { 0.22f, 0.08f, 0.18f }, foliageLight },
        { {  0.02f, 0.55f,  0.02f }, { 0.24f, 0.09f, 0.20f }, foliagePine },
        { {  0.07f, 0.60f,  0.04f }, { 0.16f, 0.06f, 0.14f }, foliageLight },
        { { -0.05f, 0.46f, -0.08f }, { 0.17f, 0.06f, 0.15f }, foliagePine }
    };
    for (int p = 0; p < 5; ++p) {
        auto pad = std::make_shared<SceneNode>(prefix + "_FoliagePad_" + std::to_string(p));
        pad->mesh = &meshes.pineCluster;
        pad->transform.position = pads[p].offset;
        pad->transform.scale = glm::vec3(pads[p].scale.x / 0.24f, pads[p].scale.y / 0.08f, pads[p].scale.z / 0.20f);
        pad->color = pads[p].col;
        root->addChild(pad);
    }

    return root;
}

// -------------------------------------------------------------
// Helper Object: Window Planter Box / Flower Tub (窓の植木鉢 / 花壇)
// -------------------------------------------------------------
inline std::shared_ptr<SceneNode> createWindowPlanterBox(SceneMeshes& meshes, const std::string& prefix, const glm::vec3& pos, float width = 1.6f, float rotY = 0.0f)
{
    auto root = std::make_shared<SceneNode>(prefix + "_Planter");
    root->transform.position = pos;
    root->transform.rotation.y = rotY;

    glm::vec4 cedarWood(0.28f, 0.17f, 0.10f, 1.0f);
    glm::vec4 soilCol(0.12f, 0.09f, 0.06f, 1.0f);
    glm::vec4 foliage(0.16f, 0.45f, 0.18f, 1.0f);
    glm::vec4 flowerRed(0.88f, 0.15f, 0.15f, 1.0f);
    glm::vec4 flowerYellow(0.98f, 0.85f, 0.18f, 1.0f);
    glm::vec4 flowerPink(0.96f, 0.55f, 0.72f, 1.0f);
    glm::vec4 flowerWhite(0.98f, 0.96f, 0.92f, 1.0f);
    glm::vec4 flowerPurple(0.55f, 0.22f, 0.75f, 1.0f);

    // Cedar Planter Trough
    auto trough = std::make_shared<SceneNode>(prefix + "_Trough");
    trough->mesh = &meshes.cube;
    trough->transform.position = glm::vec3(0.0f, 0.10f, 0.0f);
    trough->transform.scale = glm::vec3(0.30f, 0.18f, width);
    trough->color = cedarWood;
    root->addChild(trough);

    // Soil Bed
    auto soil = std::make_shared<SceneNode>(prefix + "_Soil");
    soil->mesh = &meshes.cube;
    soil->transform.position = glm::vec3(0.0f, 0.18f, 0.0f);
    soil->transform.scale = glm::vec3(0.26f, 0.04f, width * 0.95f);
    soil->color = soilCol;
    root->addChild(soil);

    // Green Foliage Leaves / Shrub Mounds (Sculpted Evergreen Pine Pads & Botanical clusters)
    int numMounds = (int)(width / 0.32f) + 1;
    for (int m = 0; m < numMounds; ++m) {
        float mz = -width * 0.45f + (float)m * (width * 0.90f / (float)(std::max(1, numMounds - 1)));
        auto leafMound = std::make_shared<SceneNode>(prefix + "_Leaves_" + std::to_string(m));
        leafMound->mesh = &meshes.pineCluster;
        leafMound->transform.position = glm::vec3(0.0f, 0.22f, mz);
        leafMound->transform.scale = glm::vec3(1.1f, 1.3f, 1.2f);
        leafMound->color = foliage;
        root->addChild(leafMound);
    }

    // Diverse Colorful Flower Blooms & Stems (Fully Connected Floral Anatomy with Curves)
    glm::vec4 flowerPalette[5] = { flowerRed, flowerYellow, flowerPink, flowerWhite, flowerPurple };
    glm::vec4 stemGreen(0.20f, 0.48f, 0.18f, 1.0f);
    glm::vec4 stamenGold(0.98f, 0.88f, 0.22f, 1.0f);
    int numFlowers = (int)(width * 5.0f);
    for (int f = 0; f < numFlowers; ++f) {
        float fz = -width * 0.42f + (float)f * (width * 0.84f / (float)(std::max(1, numFlowers - 1)));
        float fx = ((f % 3) - 1) * 0.06f;
        float bloomY = 0.30f + ((f % 2) * 0.04f);
        float stemH = bloomY - 0.18f;

        // Group entire floral specimen under parent node for 100% rigid connection
        auto flowerGroup = std::make_shared<SceneNode>(prefix + "_FlowerGroup_" + std::to_string(f));
        flowerGroup->transform.position = glm::vec3(fx, 0.18f, fz);
        root->addChild(flowerGroup);

        // Natural curved green stem rooted directly in the soil
        auto stem = std::make_shared<SceneNode>(prefix + "_FlowerStem_" + std::to_string(f));
        stem->mesh = (f % 2 == 0) ? &meshes.planterStemCurvedA : &meshes.planterStemCurvedB;
        stem->transform.scale = glm::vec3(1.0f, stemH / 0.15f, 1.0f);
        stem->color = stemGreen;
        flowerGroup->addChild(stem);

        // 3D Curved Botanical Leaves sprouting naturally from the stem
        auto leaf1 = std::make_shared<SceneNode>(prefix + "_FlowerLeafA_" + std::to_string(f));
        leaf1->mesh = &meshes.curvedLeaf;
        leaf1->transform.position = glm::vec3(0.015f, 0.04f, 0.01f);
        leaf1->transform.rotation = glm::vec3(20.0f, (float)(f * 60 % 360), -35.0f);
        leaf1->transform.scale = glm::vec3(0.55f);
        leaf1->color = foliage;
        flowerGroup->addChild(leaf1);

        auto leaf2 = std::make_shared<SceneNode>(prefix + "_FlowerLeafB_" + std::to_string(f));
        leaf2->mesh = &meshes.curvedLeaf;
        leaf2->transform.position = glm::vec3(-0.015f, 0.07f, -0.01f);
        leaf2->transform.rotation = glm::vec3(-15.0f, (float)((f * 60 + 180) % 360), 30.0f);
        leaf2->transform.scale = glm::vec3(0.48f);
        leaf2->color = foliage;
        flowerGroup->addChild(leaf2);

        // Blossom tip at local Y = stemH
        glm::vec3 tipLocal(0.0f, stemH, 0.0f);

        // Green calyx cup cradling the base of the flower bloom
        auto calyx = std::make_shared<SceneNode>(prefix + "_FlowerCalyx_" + std::to_string(f));
        calyx->mesh = &meshes.cylinder;
        calyx->transform.position = tipLocal - glm::vec3(0.0f, 0.01f, 0.0f);
        calyx->transform.scale = glm::vec3(0.035f, 0.02f, 0.035f);
        calyx->color = stemGreen;
        flowerGroup->addChild(calyx);

        // 4 Cupped curved flower petals arranged in a rosette
        for (int p = 0; p < 4; ++p) {
            float pAngle = (float)p * 90.0f;
            float pr = glm::radians(pAngle);
            auto petal = std::make_shared<SceneNode>(prefix + "_Petal_" + std::to_string(f) + "_" + std::to_string(p));
            petal->mesh = &meshes.curvedPetal;
            petal->transform.position = tipLocal + glm::vec3(std::cos(pr) * 0.015f, 0.005f, std::sin(pr) * 0.015f);
            petal->transform.rotation = glm::vec3(25.0f, pAngle, 0.0f);
            petal->transform.scale = glm::vec3(0.85f);
            petal->color = flowerPalette[f % 5];
            flowerGroup->addChild(petal);
        }

        // Golden stamen center nestled atop the petals
        auto stamen = std::make_shared<SceneNode>(prefix + "_FlowerStamen_" + std::to_string(f));
        stamen->mesh = &meshes.sphere;
        stamen->transform.position = tipLocal + glm::vec3(0.0f, 0.015f, 0.0f);
        stamen->transform.scale = glm::vec3(0.028f, 0.022f, 0.028f);
        stamen->color = stamenGold;
        flowerGroup->addChild(stamen);
    }

    return root;
}

// -------------------------------------------------------------
// Helper Object: Hanging Kokedama / Hanging Flower Basket (苔玉吊り鉢)
// -------------------------------------------------------------
inline std::shared_ptr<SceneNode> createHangingKokedama(SceneMeshes& meshes, const std::string& prefix, const glm::vec3& ceilingAnchor, float cordLen = 0.85f)
{
    auto root = std::make_shared<SceneNode>(prefix + "_HangingKokedama");
    root->transform.position = ceilingAnchor;

    glm::vec4 cordCol(0.18f, 0.16f, 0.14f, 1.0f);
    glm::vec4 mossBallCol(0.18f, 0.38f, 0.15f, 1.0f);
    glm::vec4 ivyFoliage(0.22f, 0.52f, 0.20f, 1.0f);

    // Hanging Cord
    auto cord = std::make_shared<SceneNode>(prefix + "_Cord");
    cord->mesh = &meshes.cylinder;
    cord->transform.position = glm::vec3(0.0f, -cordLen * 0.5f, 0.0f);
    cord->transform.scale = glm::vec3(0.015f, cordLen, 0.015f);
    cord->color = cordCol;
    root->addChild(cord);

    // Spherical Moss Ball Body
    float ballY = -cordLen;
    auto mossBall = std::make_shared<SceneNode>(prefix + "_MossBall");
    mossBall->mesh = &meshes.sphere;
    mossBall->transform.position = glm::vec3(0.0f, ballY, 0.0f);
    mossBall->transform.scale = glm::vec3(0.26f, 0.24f, 0.26f);
    mossBall->color = mossBallCol;
    root->addChild(mossBall);

    // Cascading Foliage Sprays & Ivy Trailing Downward along graceful 3D Bézier curves
    for (int v = 0; v < 4; ++v) {
        float angle = (float)v * 90.0f;
        float rad = glm::radians(angle);
        auto vineNode = std::make_shared<SceneNode>(prefix + "_VineNode_" + std::to_string(v));
        vineNode->transform.position = glm::vec3(std::cos(rad) * 0.11f, ballY - 0.04f, std::sin(rad) * 0.11f);
        vineNode->transform.rotation.y = angle;
        root->addChild(vineNode);

        auto vine = std::make_shared<SceneNode>(prefix + "_VineMesh_" + std::to_string(v));
        vine->mesh = (v % 2 == 0) ? &meshes.kokedamaVineCurved1 : &meshes.kokedamaVineCurved2;
        vine->color = ivyFoliage;
        vineNode->addChild(vine);

        // Curved leaves attached along the vine
        auto vLeaf1 = std::make_shared<SceneNode>(prefix + "_VineLeaf1_" + std::to_string(v));
        vLeaf1->mesh = &meshes.curvedLeaf;
        vLeaf1->transform.position = glm::vec3(0.02f, -0.10f, 0.02f);
        vLeaf1->transform.rotation = glm::vec3(35.0f, 40.0f, -20.0f);
        vLeaf1->transform.scale = glm::vec3(0.55f);
        vLeaf1->color = ivyFoliage;
        vineNode->addChild(vLeaf1);

        auto vLeaf2 = std::make_shared<SceneNode>(prefix + "_VineLeaf2_" + std::to_string(v));
        vLeaf2->mesh = &meshes.curvedLeaf;
        vLeaf2->transform.position = glm::vec3(0.04f, -0.20f, 0.03f);
        vLeaf2->transform.rotation = glm::vec3(-25.0f, 130.0f, 30.0f);
        vLeaf2->transform.scale = glm::vec3(0.50f);
        vLeaf2->color = ivyFoliage;
        vineNode->addChild(vLeaf2);

        // Blossom at tip of vine with curved cupped petals
        glm::vec3 blossomTip(0.04f, -0.28f, 0.05f);
        for (int p = 0; p < 4; ++p) {
            float pAngle = (float)p * 90.0f;
            float pr = glm::radians(pAngle);
            auto petal = std::make_shared<SceneNode>(prefix + "_VinePetal_" + std::to_string(v) + "_" + std::to_string(p));
            petal->mesh = &meshes.curvedPetal;
            petal->transform.position = blossomTip + glm::vec3(std::cos(pr) * 0.012f, 0.0f, std::sin(pr) * 0.012f);
            petal->transform.rotation = glm::vec3(30.0f, pAngle, 0.0f);
            petal->transform.scale = glm::vec3(0.70f);
            petal->color = (v % 2 == 0) ? glm::vec4(0.98f, 0.42f, 0.62f, 1.0f) : glm::vec4(0.98f, 0.88f, 0.25f, 1.0f);
            vineNode->addChild(petal);
        }
    }

    return root;
}

// -------------------------------------------------------------
// Helper Object: Traditional Japanese Ikebana Flower Vase (華道 生け花)
// -------------------------------------------------------------
inline std::shared_ptr<SceneNode> createIkebanaVase(SceneMeshes& meshes, const std::string& prefix, const glm::vec3& pos, float scale = 1.0f, int colorTheme = 0)
{
    auto root = std::make_shared<SceneNode>(prefix + "_Ikebana");
    root->transform.position = pos;
    root->transform.scale = glm::vec3(scale);

    // Ceramic Vase Colors: 0 = Celadon Jade, 1 = Cobalt Blue Porcelain, 2 = Terracotta Stoneware
    glm::vec4 vaseColor;
    if (colorTheme == 0)
        vaseColor = glm::vec4(0.35f, 0.62f, 0.52f, 1.0f);
    else if (colorTheme == 1)
        vaseColor = glm::vec4(0.18f, 0.28f, 0.68f, 1.0f);
    else
        vaseColor = glm::vec4(0.55f, 0.32f, 0.22f, 1.0f);

    glm::vec4 stemColor(0.20f, 0.48f, 0.18f, 1.0f);
    glm::vec4 leafColor(0.16f, 0.44f, 0.16f, 1.0f);

    // Ceramic Foot Base
    auto foot = std::make_shared<SceneNode>(prefix + "_VaseFoot");
    foot->mesh = &meshes.cylinder;
    foot->transform.position = glm::vec3(0.0f, 0.03f, 0.0f);
    foot->transform.scale = glm::vec3(0.18f, 0.06f, 0.18f);
    foot->color = vaseColor;
    foot->shininess = 64.0f;
    foot->specularStrength = 0.80f;
    root->addChild(foot);

    // Bulbous Ceramic Body
    auto body = std::make_shared<SceneNode>(prefix + "_VaseBody");
    body->mesh = &meshes.sphere;
    body->transform.position = glm::vec3(0.0f, 0.22f, 0.0f);
    body->transform.scale = glm::vec3(0.26f, 0.32f, 0.26f);
    body->color = vaseColor;
    body->shininess = 64.0f;
    body->specularStrength = 0.80f;
    root->addChild(body);

    // Slender Neck & Flared Lip Rim
    auto neck = std::make_shared<SceneNode>(prefix + "_VaseNeck");
    neck->mesh = &meshes.cylinder;
    neck->transform.position = glm::vec3(0.0f, 0.42f, 0.0f);
    neck->transform.scale = glm::vec3(0.11f, 0.16f, 0.11f);
    neck->color = vaseColor;
    neck->shininess = 64.0f;
    neck->specularStrength = 0.80f;
    root->addChild(neck);

    auto rim = std::make_shared<SceneNode>(prefix + "_VaseRim");
    rim->mesh = &meshes.cylinder;
    rim->transform.position = glm::vec3(0.0f, 0.51f, 0.0f);
    rim->transform.scale = glm::vec3(0.16f, 0.03f, 0.16f);
    rim->color = vaseColor;
    rim->shininess = 64.0f;
    rim->specularStrength = 0.80f;
    root->addChild(rim);

    // Asymmetric Ikebana Flower Stems & Blossoms (Living Curves: Shin, Soe, Hikae, Accent)
    struct FlowerSpec {
        glm::vec3 anchorPos;
        glm::vec3 stemRot;
        const Mesh* stemMesh;
        glm::vec3 tipPos;
        glm::vec3 leaf1Pos;
        glm::vec3 leaf2Pos;
        glm::vec4 bloomColor;
        float bloomScale;
    };
    std::vector<FlowerSpec> flowers = {
        // 1. Tall Shin (Truth/Heaven) line: Crimson Camellia (Tsubaki)
        { glm::vec3(0.01f, 0.48f, -0.01f), glm::vec3(5.0f, 0.0f, -4.0f), &meshes.ikebanaStemShin,
          glm::vec3(0.07f, 0.38f, -0.05f), glm::vec3(0.03f, 0.18f, -0.02f), glm::vec3(0.05f, 0.29f, -0.04f),
          glm::vec4(0.88f, 0.14f, 0.18f, 1.0f), 0.12f },
        // 2. Medium Soe (Supporting/Man) line: Soft Pink Peony (Botan)
        { glm::vec3(-0.02f, 0.48f, 0.02f), glm::vec3(-6.0f, 20.0f, 10.0f), &meshes.ikebanaStemSoe,
          glm::vec3(-0.15f, 0.30f, 0.12f), glm::vec3(-0.07f, 0.14f, 0.06f), glm::vec3(-0.12f, 0.22f, 0.10f),
          glm::vec4(0.96f, 0.58f, 0.74f, 1.0f), 0.11f },
        // 3. Slanted Hikae (Restrained/Earth) line: Golden Plum Blossom (Ume)
        { glm::vec3(0.03f, 0.48f, 0.02f), glm::vec3(12.0f, -25.0f, -10.0f), &meshes.ikebanaStemHikae,
          glm::vec3(0.20f, 0.22f, -0.18f), glm::vec3(0.10f, 0.11f, -0.08f), glm::vec3(0.16f, 0.17f, -0.13f),
          glm::vec4(0.98f, 0.82f, 0.16f, 1.0f), 0.09f },
        // 4. Low accent line: Imperial Violet Iris (Ayame)
        { glm::vec3(-0.02f, 0.48f, -0.03f), glm::vec3(-8.0f, -60.0f, 8.0f), &meshes.ikebanaStemAccent,
          glm::vec3(-0.11f, 0.20f, -0.11f), glm::vec3(-0.05f, 0.09f, -0.05f), glm::vec3(-0.09f, 0.15f, -0.09f),
          glm::vec4(0.55f, 0.20f, 0.78f, 1.0f), 0.10f }
    };

    for (size_t i = 0; i < flowers.size(); ++i) {
        const auto& fl = flowers[i];

        // Hierarchical Flower Branch root anchored at the vase rim
        auto branchRoot = std::make_shared<SceneNode>(prefix + "_Branch_" + std::to_string(i));
        branchRoot->transform.position = fl.anchorPos;
        branchRoot->transform.rotation = fl.stemRot;
        root->addChild(branchRoot);

        // Swept curved 3D stem tube extending gracefully along the Ikebana living line
        auto stem = std::make_shared<SceneNode>(prefix + "_Stem_" + std::to_string(i));
        stem->mesh = fl.stemMesh;
        stem->color = stemColor;
        branchRoot->addChild(stem);

        // Lower green curved leaf sprouting naturally off the curved stem
        auto leaf1 = std::make_shared<SceneNode>(prefix + "_Leaf1_" + std::to_string(i));
        leaf1->mesh = &meshes.curvedLeaf;
        leaf1->transform.position = fl.leaf1Pos;
        leaf1->transform.rotation = glm::vec3(15.0f, 30.0f, -42.0f);
        leaf1->transform.scale = glm::vec3(0.95f);
        leaf1->color = leafColor;
        branchRoot->addChild(leaf1);

        // Upper green curved leaf branching out opposite the lower leaf along the curve
        auto leaf2 = std::make_shared<SceneNode>(prefix + "_Leaf2_" + std::to_string(i));
        leaf2->mesh = &meshes.curvedLeaf;
        leaf2->transform.position = fl.leaf2Pos;
        leaf2->transform.rotation = glm::vec3(-15.0f, 195.0f, -38.0f);
        leaf2->transform.scale = glm::vec3(0.80f);
        leaf2->color = leafColor;
        branchRoot->addChild(leaf2);

        // Green calyx cup firmly attached to the curved stem tip cradling the blossom
        auto calyx = std::make_shared<SceneNode>(prefix + "_Calyx_" + std::to_string(i));
        calyx->mesh = &meshes.cylinder;
        calyx->transform.position = fl.tipPos;
        calyx->transform.scale = glm::vec3(fl.bloomScale * 0.45f, 0.025f, fl.bloomScale * 0.45f);
        calyx->color = stemColor;
        branchRoot->addChild(calyx);

        // 5 Cupped curved flower petals arranged in a floral rosette
        for (int p = 0; p < 5; ++p) {
            float pAngle = (float)p * 72.0f;
            float pr = glm::radians(pAngle);
            auto petal = std::make_shared<SceneNode>(prefix + "_Petal_" + std::to_string(i) + "_" + std::to_string(p));
            petal->mesh = &meshes.curvedPetal;
            petal->transform.position = fl.tipPos + glm::vec3(std::cos(pr) * fl.bloomScale * 0.22f, fl.bloomScale * 0.05f, std::sin(pr) * fl.bloomScale * 0.22f);
            petal->transform.rotation = glm::vec3(35.0f, pAngle, 0.0f);
            petal->transform.scale = glm::vec3(fl.bloomScale * 1.5f);
            petal->color = fl.bloomColor;
            branchRoot->addChild(petal);
        }

        // Golden stamen center firmly embedded in the middle of the blossom
        auto stamen = std::make_shared<SceneNode>(prefix + "_Stamen_" + std::to_string(i));
        stamen->mesh = &meshes.sphere;
        stamen->transform.position = fl.tipPos + glm::vec3(0.0f, fl.bloomScale * 0.12f, 0.0f);
        stamen->transform.scale = glm::vec3(fl.bloomScale * 0.28f, fl.bloomScale * 0.22f, fl.bloomScale * 0.28f);
        stamen->color = glm::vec4(0.98f, 0.88f, 0.20f, 1.0f);
        branchRoot->addChild(stamen);
    }

    return root;
}

// -------------------------------------------------------------
// Helper Object: Authentic Traditional Andon Floor Lamp (行灯)
// -------------------------------------------------------------
inline std::shared_ptr<SceneNode> createAndonFloorLamp(SceneMeshes& meshes, const std::string& prefix, const glm::vec3& pos, float scale = 1.0f)
{
    auto root = std::make_shared<SceneNode>(prefix + "_AndonLamp");
    root->transform.position = pos;
    root->transform.scale = glm::vec3(scale);

    glm::vec4 cedar(0.20f, 0.12f, 0.07f, 1.0f);
    glm::vec4 paper(0.96f, 0.92f, 0.82f, 1.0f);

    // Plinth
    auto plinth = std::make_shared<SceneNode>(prefix + "_Plinth");
    plinth->mesh = &meshes.cube;
    plinth->transform.position = glm::vec3(0.0f, 0.06f, 0.0f);
    plinth->transform.scale = glm::vec3(0.44f, 0.04f, 0.44f);
    plinth->color = cedar;
    root->addChild(plinth);

    // 4 Carved Feet & 4 Corner Posts
    float lx[2] = { -0.18f, 0.18f };
    float lz[2] = { -0.18f, 0.18f };
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            auto leg = std::make_shared<SceneNode>(prefix + "_Leg_" + std::to_string(i) + "_" + std::to_string(j));
            leg->mesh = &meshes.cube;
            leg->transform.position = glm::vec3(lx[i], 0.025f, lz[j]);
            leg->transform.scale = glm::vec3(0.05f, 0.05f, 0.05f);
            leg->color = cedar;
            root->addChild(leg);

            auto post = std::make_shared<SceneNode>(prefix + "_Post_" + std::to_string(i) + "_" + std::to_string(j));
            post->mesh = &meshes.cube;
            post->transform.position = glm::vec3(lx[i], 0.52f, lz[j]);
            post->transform.scale = glm::vec3(0.04f, 0.88f, 0.04f);
            post->color = cedar;
            root->addChild(post);
        }
    }

    // Translucent Washi Paper Diffuser Body
    auto paperBody = std::make_shared<SceneNode>(prefix + "_PaperBody");
    paperBody->mesh = &meshes.cube;
    paperBody->transform.position = glm::vec3(0.0f, 0.52f, 0.0f);
    paperBody->transform.scale = glm::vec3(0.36f, 0.84f, 0.36f);
    paperBody->color = paper;
    paperBody->isEmissive = true;
    paperBody->emissiveColor = glm::vec3(1.60f, 1.25f, 0.65f); // warm radiant lamp glow
    root->addChild(paperBody);

    // Horizontal Kumiko Lattice Ribs
    for (int r = 0; r < 3; ++r) {
        float ry = 0.28f + (float)r * 0.24f;
        auto ribH = std::make_shared<SceneNode>(prefix + "_RibH_" + std::to_string(r));
        ribH->mesh = &meshes.cube;
        ribH->transform.position = glm::vec3(0.0f, ry, 0.0f);
        ribH->transform.scale = glm::vec3(0.38f, 0.025f, 0.38f);
        ribH->color = cedar;
        root->addChild(ribH);
    }

    // Top Cap & Carrying Handle
    auto topCap = std::make_shared<SceneNode>(prefix + "_TopCap");
    topCap->mesh = &meshes.cube;
    topCap->transform.position = glm::vec3(0.0f, 0.98f, 0.0f);
    topCap->transform.scale = glm::vec3(0.42f, 0.04f, 0.42f);
    topCap->color = cedar;
    root->addChild(topCap);

    auto handle = std::make_shared<SceneNode>(prefix + "_Handle");
    handle->mesh = &meshes.cylinder;
    handle->transform.position = glm::vec3(0.0f, 1.06f, 0.0f);
    handle->transform.rotation.z = 90.0f;
    handle->transform.scale = glm::vec3(0.035f, 0.28f, 0.035f);
    handle->color = cedar;
    root->addChild(handle);

    return root;
}

// -------------------------------------------------------------
// Helper Object: Hanging Ceiling Pendant Washi Lantern (吊り行灯)
// -------------------------------------------------------------
inline std::shared_ptr<SceneNode> createCeilingPendantLamp(SceneMeshes& meshes, const std::string& prefix, const glm::vec3& ceilingPos, float dropLen = 0.80f)
{
    auto root = std::make_shared<SceneNode>(prefix + "_PendantLamp");
    root->transform.position = ceilingPos;

    glm::vec4 cedar(0.20f, 0.12f, 0.07f, 1.0f);
    glm::vec4 cord(0.12f, 0.12f, 0.12f, 1.0f);
    glm::vec4 paper(0.96f, 0.92f, 0.82f, 1.0f);

    // Ceiling Mount Rosette
    auto rosette = std::make_shared<SceneNode>(prefix + "_Rosette");
    rosette->mesh = &meshes.cylinder;
    rosette->transform.position = glm::vec3(0.0f, -0.02f, 0.0f);
    rosette->transform.scale = glm::vec3(0.18f, 0.04f, 0.18f);
    rosette->color = cedar;
    root->addChild(rosette);

    // Suspension Cord
    auto cordNode = std::make_shared<SceneNode>(prefix + "_Cord");
    cordNode->mesh = &meshes.cylinder;
    cordNode->transform.position = glm::vec3(0.0f, -dropLen * 0.5f, 0.0f);
    cordNode->transform.scale = glm::vec3(0.02f, dropLen, 0.02f);
    cordNode->color = cord;
    root->addChild(cordNode);

    // Lantern Body Frame & Paper Shade
    float bodyY = -dropLen - 0.25f;
    auto shade = std::make_shared<SceneNode>(prefix + "_Shade");
    shade->mesh = &meshes.cube;
    shade->transform.position = glm::vec3(0.0f, bodyY, 0.0f);
    shade->transform.scale = glm::vec3(0.48f, 0.50f, 0.48f);
    shade->color = paper;
    shade->isEmissive = true;
    shade->emissiveColor = glm::vec3(1.70f, 1.35f, 0.70f); // bright radiant lantern core
    root->addChild(shade);

    auto shadeTop = std::make_shared<SceneNode>(prefix + "_ShadeTop");
    shadeTop->mesh = &meshes.cube;
    shadeTop->transform.position = glm::vec3(0.0f, bodyY + 0.26f, 0.0f);
    shadeTop->transform.scale = glm::vec3(0.52f, 0.04f, 0.52f);
    shadeTop->color = cedar;
    root->addChild(shadeTop);

    auto shadeBot = std::make_shared<SceneNode>(prefix + "_ShadeBot");
    shadeBot->mesh = &meshes.cube;
    shadeBot->transform.position = glm::vec3(0.0f, bodyY - 0.26f, 0.0f);
    shadeBot->transform.scale = glm::vec3(0.52f, 0.04f, 0.52f);
    shadeBot->color = cedar;
    root->addChild(shadeBot);

    return root;
}

// -------------------------------------------------------------
// 2. Machiya Building (Traditional 2-Storied Japanese Townhouse)
// -------------------------------------------------------------
struct WindowSashItem
{
    std::shared_ptr<SceneNode> node;
    glm::vec3 basePos;
    glm::vec3 slideDelta;
};

class MachiyaBuilding
{
public:
    std::shared_ptr<SceneNode> root;
    std::vector<std::shared_ptr<SceneNode>> windows;
    std::shared_ptr<SceneNode> slidingDoorGroup;
    std::vector<WindowSashItem> slidingWindowSashes;

    glm::vec3 worldPos;
    float rotationY;
    bool isDoorOpen = false;
    float doorSlideProgress = 0.0f; // 0.0 = closed, 1.0 = open
    bool isWindowOpen = false;
    float windowSlideProgress = 0.0f; // 0.0 = closed, 1.0 = open

    MachiyaBuilding(SceneMeshes& meshes, const std::string& name, const glm::vec3& pos, float rotY, const glm::vec3& scale = glm::vec3(1.0f))
        : worldPos(pos), rotationY(rotY)
    {
        root = std::make_shared<SceneNode>(name);
        root->transform.position = pos;
        root->transform.rotation.y = rotY;
        root->transform.scale = scale;

        glm::vec4 timber(0.36f, 0.22f, 0.13f, 1.0f);
        glm::vec4 darkWood(0.18f, 0.11f, 0.06f, 1.0f);
        glm::vec4 plaster(0.88f, 0.86f, 0.82f, 1.0f);
        glm::vec4 roofSlate(0.16f, 0.17f, 0.20f, 1.0f);
        glm::vec4 paperColor(0.92f, 0.88f, 0.80f, 1.0f);
        glm::vec4 stone(0.38f, 0.38f, 0.40f, 1.0f);
        glm::vec4 tatamiColor(0.80f, 0.78f, 0.60f, 1.0f);
        glm::vec4 tableWood(0.16f, 0.09f, 0.05f, 1.0f);

        // =========================================================
        // 1. GROUND FLOOR PERIMETER WALLS (Hollow Interior Shell)
        // =========================================================
        // Back Wall (Local X = -3.95) with Garden Window Opening (Z in [-1.2, 1.2])
        auto wallBackL = std::make_shared<SceneNode>(name + "_WallBackL");
        wallBackL->mesh = &meshes.cube;
        wallBackL->transform.position = glm::vec3(-3.95f, 2.15f, 2.80f);
        wallBackL->transform.scale = glm::vec3(0.20f, 4.30f, 3.30f);
        wallBackL->color = timber;
        root->addChild(wallBackL);

        auto wallBackR = std::make_shared<SceneNode>(name + "_WallBackR");
        wallBackR->mesh = &meshes.cube;
        wallBackR->transform.position = glm::vec3(-3.95f, 2.15f, -2.80f);
        wallBackR->transform.scale = glm::vec3(0.20f, 4.30f, 3.30f);
        wallBackR->color = timber;
        root->addChild(wallBackR);

        auto wallBackHeader = std::make_shared<SceneNode>(name + "_WallBackHeader");
        wallBackHeader->mesh = &meshes.cube;
        wallBackHeader->transform.position = glm::vec3(-3.95f, 3.45f, 0.0f);
        wallBackHeader->transform.scale = glm::vec3(0.20f, 1.70f, 2.50f);
        wallBackHeader->color = timber;
        root->addChild(wallBackHeader);

        auto wallBackSill = std::make_shared<SceneNode>(name + "_WallBackSill");
        wallBackSill->mesh = &meshes.cube;
        wallBackSill->transform.position = glm::vec3(-3.95f, 0.45f, 0.0f);
        wallBackSill->transform.scale = glm::vec3(0.20f, 0.90f, 2.50f);
        wallBackSill->color = timber;
        root->addChild(wallBackSill);

        // Left Side Wall (Local Z = +4.45) with Side Window Opening (X in [-1.0, 1.0])
        auto wallLeftF = std::make_shared<SceneNode>(name + "_WallLeftF");
        wallLeftF->mesh = &meshes.cube;
        wallLeftF->transform.position = glm::vec3(2.50f, 2.15f, 4.45f);
        wallLeftF->transform.scale = glm::vec3(3.00f, 4.30f, 0.20f);
        wallLeftF->color = timber;
        root->addChild(wallLeftF);

        auto wallLeftB = std::make_shared<SceneNode>(name + "_WallLeftB");
        wallLeftB->mesh = &meshes.cube;
        wallLeftB->transform.position = glm::vec3(-2.50f, 2.15f, 4.45f);
        wallLeftB->transform.scale = glm::vec3(3.00f, 4.30f, 0.20f);
        wallLeftB->color = timber;
        root->addChild(wallLeftB);

        auto wallLeftHeader = std::make_shared<SceneNode>(name + "_WallLeftHeader");
        wallLeftHeader->mesh = &meshes.cube;
        wallLeftHeader->transform.position = glm::vec3(0.0f, 3.45f, 4.45f);
        wallLeftHeader->transform.scale = glm::vec3(2.10f, 1.70f, 0.20f);
        wallLeftHeader->color = timber;
        root->addChild(wallLeftHeader);

        auto wallLeftSill = std::make_shared<SceneNode>(name + "_WallLeftSill");
        wallLeftSill->mesh = &meshes.cube;
        wallLeftSill->transform.position = glm::vec3(0.0f, 0.55f, 4.45f);
        wallLeftSill->transform.scale = glm::vec3(2.10f, 1.10f, 0.20f);
        wallLeftSill->color = timber;
        root->addChild(wallLeftSill);

        // Right Side Wall (Local Z = -4.45, solid wall bounding staircase)
        auto wallRight = std::make_shared<SceneNode>(name + "_WallRight");
        wallRight->mesh = &meshes.cube;
        wallRight->transform.position = glm::vec3(0.0f, 2.15f, -4.45f);
        wallRight->transform.scale = glm::vec3(8.00f, 4.30f, 0.20f);
        wallRight->color = timber;
        root->addChild(wallRight);

        // Front Wall Left Section (with Shoji window, Z in [0.25, 4.45])
        auto wallFrontFarL = std::make_shared<SceneNode>(name + "_WallFrontFarL");
        wallFrontFarL->mesh = &meshes.cube;
        wallFrontFarL->transform.position = glm::vec3(3.95f, 2.15f, 3.80f);
        wallFrontFarL->transform.scale = glm::vec3(0.20f, 4.30f, 1.30f);
        wallFrontFarL->color = timber;
        root->addChild(wallFrontFarL);

        auto wallFrontMidL = std::make_shared<SceneNode>(name + "_WallFrontMidL");
        wallFrontMidL->mesh = &meshes.cube;
        wallFrontMidL->transform.position = glm::vec3(3.95f, 2.15f, 0.55f);
        wallFrontMidL->transform.scale = glm::vec3(0.20f, 4.30f, 0.60f);
        wallFrontMidL->color = timber;
        root->addChild(wallFrontMidL);

        auto wallFrontWinH = std::make_shared<SceneNode>(name + "_WallFrontWinH");
        wallFrontWinH->mesh = &meshes.cube;
        wallFrontWinH->transform.position = glm::vec3(3.95f, 3.45f, 2.00f);
        wallFrontWinH->transform.scale = glm::vec3(0.20f, 1.70f, 2.40f);
        wallFrontWinH->color = timber;
        root->addChild(wallFrontWinH);

        auto wallFrontWinS = std::make_shared<SceneNode>(name + "_WallFrontWinS");
        wallFrontWinS->mesh = &meshes.cube;
        wallFrontWinS->transform.position = glm::vec3(3.95f, 0.45f, 2.00f);
        wallFrontWinS->transform.scale = glm::vec3(0.20f, 0.90f, 2.40f);
        wallFrontWinS->color = timber;
        root->addChild(wallFrontWinS);

        // Front Wall Right Section (Z in [-4.45, -2.05])
        auto wallFrontR = std::make_shared<SceneNode>(name + "_WallFrontR");
        wallFrontR->mesh = &meshes.cube;
        wallFrontR->transform.position = glm::vec3(3.95f, 2.15f, -3.25f);
        wallFrontR->transform.scale = glm::vec3(0.20f, 4.30f, 2.40f);
        wallFrontR->color = timber;
        root->addChild(wallFrontR);

        // Front Doorway Header / Lintel Wall
        auto wallFrontHeader = std::make_shared<SceneNode>(name + "_WallFrontHeader");
        wallFrontHeader->mesh = &meshes.cube;
        wallFrontHeader->transform.position = glm::vec3(3.95f, 3.40f, -0.90f);
        wallFrontHeader->transform.scale = glm::vec3(0.20f, 1.80f, 2.30f);
        wallFrontHeader->color = timber;
        root->addChild(wallFrontHeader);

        // Timber Corner Structural Studs
        float cx[2] = { -3.95f, 3.95f };
        float cz[2] = { -4.45f, 4.45f };
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                auto post = std::make_shared<SceneNode>(name + "_Post_" + std::to_string(i) + "_" + std::to_string(j));
                post->mesh = &meshes.cube;
                post->transform.position = glm::vec3(cx[i], 2.15f, cz[j]);
                post->transform.scale = glm::vec3(0.28f, 4.30f, 0.28f);
                post->color = darkWood;
                root->addChild(post);
            }
        }

        // =========================================================
        // 2. ENTRANCE DOOR FRAME & INTERACTIVE SLIDING SHOJI DOOR
        // =========================================================
        auto doorJambL = std::make_shared<SceneNode>(name + "_DoorJambL");
        doorJambL->mesh = &meshes.cube;
        doorJambL->transform.position = glm::vec3(4.14f, 1.35f, 0.26f);
        doorJambL->transform.scale = glm::vec3(0.24f, 2.70f, 0.12f);
        doorJambL->color = darkWood;
        root->addChild(doorJambL);

        auto doorJambR = std::make_shared<SceneNode>(name + "_DoorJambR");
        doorJambR->mesh = &meshes.cube;
        doorJambR->transform.position = glm::vec3(4.14f, 1.35f, -2.06f);
        doorJambR->transform.scale = glm::vec3(0.24f, 2.70f, 0.12f);
        doorJambR->color = darkWood;
        root->addChild(doorJambR);

        auto doorLintel = std::make_shared<SceneNode>(name + "_DoorLintel");
        doorLintel->mesh = &meshes.cube;
        doorLintel->transform.position = glm::vec3(4.14f, 2.62f, -0.90f);
        doorLintel->transform.scale = glm::vec3(0.26f, 0.16f, 2.44f);
        doorLintel->color = darkWood;
        root->addChild(doorLintel);

        auto doorSill = std::make_shared<SceneNode>(name + "_DoorSill");
        doorSill->mesh = &meshes.cube;
        doorSill->transform.position = glm::vec3(4.14f, 0.08f, -0.90f);
        doorSill->transform.scale = glm::vec3(0.26f, 0.16f, 2.44f);
        doorSill->color = darkWood;
        root->addChild(doorSill);

        auto stoneStep = std::make_shared<SceneNode>(name + "_StoneStep");
        stoneStep->mesh = &meshes.cube;
        stoneStep->transform.position = glm::vec3(4.35f, 0.08f, -0.90f);
        stoneStep->transform.scale = glm::vec3(0.60f, 0.16f, 2.50f);
        stoneStep->color = stone;
        root->addChild(stoneStep);

        auto doorCanopy = std::make_shared<SceneNode>(name + "_DoorCanopy");
        doorCanopy->mesh = &meshes.cube;
        doorCanopy->transform.position = glm::vec3(4.60f, 2.80f, -0.90f);
        doorCanopy->transform.rotation.z = -7.0f;
        doorCanopy->transform.scale = glm::vec3(1.10f, 0.12f, 2.70f);
        doorCanopy->color = roofSlate;
        root->addChild(doorCanopy);

        auto noren = std::make_shared<SceneNode>(name + "_EntranceNoren");
        noren->mesh = &meshes.cube;
        noren->transform.position = glm::vec3(4.26f, 2.45f, -0.90f);
        noren->transform.scale = glm::vec3(0.06f, 0.45f, 1.90f);
        noren->color = glm::vec4(0.16f, 0.22f, 0.45f, 1.0f);
        root->addChild(noren);

        // SLIDING DOOR GROUP
        slidingDoorGroup = std::make_shared<SceneNode>(name + "_SlidingDoorGroup");
        root->addChild(slidingDoorGroup);

        auto doorPanel1 = std::make_shared<SceneNode>(name + "_DoorPanel1");
        doorPanel1->mesh = &meshes.cube;
        doorPanel1->transform.position = glm::vec3(4.08f, 1.35f, -1.45f);
        doorPanel1->transform.scale = glm::vec3(0.04f, 2.40f, 1.05f);
        doorPanel1->isWindow = true;
        doorPanel1->color = paperColor;
        slidingDoorGroup->addChild(doorPanel1);
        windows.push_back(doorPanel1);

        auto doorFrame1_Top = std::make_shared<SceneNode>(name + "_DoorF1_Top");
        doorFrame1_Top->mesh = &meshes.cube;
        doorFrame1_Top->transform.position = glm::vec3(4.10f, 2.48f, -1.45f);
        doorFrame1_Top->transform.scale = glm::vec3(0.05f, 0.12f, 1.05f);
        doorFrame1_Top->color = darkWood;
        slidingDoorGroup->addChild(doorFrame1_Top);

        auto doorFrame1_Bot = std::make_shared<SceneNode>(name + "_DoorF1_Bot");
        doorFrame1_Bot->mesh = &meshes.cube;
        doorFrame1_Bot->transform.position = glm::vec3(4.10f, 0.22f, -1.45f);
        doorFrame1_Bot->transform.scale = glm::vec3(0.05f, 0.12f, 1.05f);
        doorFrame1_Bot->color = darkWood;
        slidingDoorGroup->addChild(doorFrame1_Bot);

        auto doorRib1 = std::make_shared<SceneNode>(name + "_DoorRib1");
        doorRib1->mesh = &meshes.cube;
        doorRib1->transform.position = glm::vec3(4.11f, 1.35f, -1.45f);
        doorRib1->transform.scale = glm::vec3(0.04f, 2.36f, 0.06f);
        doorRib1->color = darkWood;
        slidingDoorGroup->addChild(doorRib1);

        auto doorPanel2 = std::make_shared<SceneNode>(name + "_DoorPanel2");
        doorPanel2->mesh = &meshes.cube;
        doorPanel2->transform.position = glm::vec3(4.17f, 1.35f, -0.35f);
        doorPanel2->transform.scale = glm::vec3(0.04f, 2.40f, 1.05f);
        doorPanel2->isWindow = true;
        doorPanel2->color = paperColor;
        slidingDoorGroup->addChild(doorPanel2);
        windows.push_back(doorPanel2);

        auto doorFrame2_Top = std::make_shared<SceneNode>(name + "_DoorF2_Top");
        doorFrame2_Top->mesh = &meshes.cube;
        doorFrame2_Top->transform.position = glm::vec3(4.19f, 2.48f, -0.35f);
        doorFrame2_Top->transform.scale = glm::vec3(0.05f, 0.12f, 1.05f);
        doorFrame2_Top->color = darkWood;
        slidingDoorGroup->addChild(doorFrame2_Top);

        auto doorFrame2_Bot = std::make_shared<SceneNode>(name + "_DoorF2_Bot");
        doorFrame2_Bot->mesh = &meshes.cube;
        doorFrame2_Bot->transform.position = glm::vec3(4.19f, 0.22f, -0.35f);
        doorFrame2_Bot->transform.scale = glm::vec3(0.05f, 0.12f, 1.05f);
        doorFrame2_Bot->color = darkWood;
        slidingDoorGroup->addChild(doorFrame2_Bot);

        auto doorRib2 = std::make_shared<SceneNode>(name + "_DoorRib2");
        doorRib2->mesh = &meshes.cube;
        doorRib2->transform.position = glm::vec3(4.20f, 1.35f, -0.35f);
        doorRib2->transform.scale = glm::vec3(0.04f, 2.36f, 0.06f);
        doorRib2->color = darkWood;
        slidingDoorGroup->addChild(doorRib2);

        // =========================================================
        // 2B. GROUND FLOOR WINDOWS (Authentic Shoji Sliding Windows)
        // =========================================================
        // Front Street Sliding Window (at X = 4.05, Z in [0.90, 3.10])
        // Window frame: 4-piece open border (top, bottom, left, right bars) so sashes are visible
        auto winF1_FrameTop = std::make_shared<SceneNode>(name + "_WinF1_FrameTop");
        winF1_FrameTop->mesh = &meshes.cube;
        winF1_FrameTop->transform.position = glm::vec3(4.08f, 2.84f, 2.00f);
        winF1_FrameTop->transform.scale = glm::vec3(0.12f, 0.12f, 2.44f);
        winF1_FrameTop->color = darkWood;
        root->addChild(winF1_FrameTop);

        auto winF1_FrameBot = std::make_shared<SceneNode>(name + "_WinF1_FrameBot");
        winF1_FrameBot->mesh = &meshes.cube;
        winF1_FrameBot->transform.position = glm::vec3(4.08f, 0.88f, 2.00f);
        winF1_FrameBot->transform.scale = glm::vec3(0.12f, 0.12f, 2.44f);
        winF1_FrameBot->color = darkWood;
        root->addChild(winF1_FrameBot);

        auto winF1_FrameL = std::make_shared<SceneNode>(name + "_WinF1_FrameL");
        winF1_FrameL->mesh = &meshes.cube;
        winF1_FrameL->transform.position = glm::vec3(4.08f, 1.85f, 3.18f);
        winF1_FrameL->transform.scale = glm::vec3(0.12f, 1.84f, 0.10f);
        winF1_FrameL->color = darkWood;
        root->addChild(winF1_FrameL);

        auto winF1_FrameR = std::make_shared<SceneNode>(name + "_WinF1_FrameR");
        winF1_FrameR->mesh = &meshes.cube;
        winF1_FrameR->transform.position = glm::vec3(4.08f, 1.85f, 0.82f);
        winF1_FrameR->transform.scale = glm::vec3(0.12f, 1.84f, 0.10f);
        winF1_FrameR->color = darkWood;
        root->addChild(winF1_FrameR);

        // Window Sill Ledge (exterior shelf)
        auto winF1_SillLedge = std::make_shared<SceneNode>(name + "_WinF1_SillLedge");
        winF1_SillLedge->mesh = &meshes.cube;
        winF1_SillLedge->transform.position = glm::vec3(4.22f, 0.88f, 2.00f);
        winF1_SillLedge->transform.scale = glm::vec3(0.35f, 0.08f, 2.50f);
        winF1_SillLedge->color = darkWood;
        root->addChild(winF1_SillLedge);

        // Ground Floor Window Sashes (Inner Left & Outer Right on dual sill tracks)
        // Left sash: inner track at X=4.05, Right sash: outer track at X=4.11
        auto winF1_SashL = std::make_shared<SceneNode>(name + "_WinF1_SashL");
        winF1_SashL->mesh = &meshes.cube;
        winF1_SashL->transform.position = glm::vec3(4.05f, 1.85f, 1.48f);
        winF1_SashL->transform.scale = glm::vec3(0.035f, 1.84f, 1.05f);
        winF1_SashL->isWindow = true;
        winF1_SashL->color = paperColor;
        root->addChild(winF1_SashL);
        windows.push_back(winF1_SashL);

        // Left sash vertical kumiko rib (slightly in front of paper)
        auto winF1_RibL = std::make_shared<SceneNode>(name + "_WinF1_RibL");
        winF1_RibL->mesh = &meshes.cube;
        winF1_RibL->transform.position = glm::vec3(4.07f, 1.85f, 1.48f);
        winF1_RibL->transform.scale = glm::vec3(0.025f, 1.80f, 0.05f);
        winF1_RibL->color = darkWood;
        root->addChild(winF1_RibL);

        // Left sash horizontal kumiko bars
        for (int hb = 0; hb < 3; ++hb) {
            auto hbar = std::make_shared<SceneNode>(name + "_WinF1_HBarL_" + std::to_string(hb));
            hbar->mesh = &meshes.cube;
            hbar->transform.position = glm::vec3(4.07f, 1.25f + (float)hb * 0.60f, 1.48f);
            hbar->transform.scale = glm::vec3(0.025f, 0.025f, 1.02f);
            hbar->color = darkWood;
            root->addChild(hbar);
        }

        auto winF1_SashR = std::make_shared<SceneNode>(name + "_WinF1_SashR");
        winF1_SashR->mesh = &meshes.cube;
        winF1_SashR->transform.position = glm::vec3(4.11f, 1.85f, 2.52f);
        winF1_SashR->transform.scale = glm::vec3(0.035f, 1.84f, 1.05f);
        winF1_SashR->isWindow = true;
        winF1_SashR->color = paperColor;
        root->addChild(winF1_SashR);
        windows.push_back(winF1_SashR);

        // Right sash vertical kumiko rib (slightly in front of paper)
        auto winF1_RibR = std::make_shared<SceneNode>(name + "_WinF1_RibR");
        winF1_RibR->mesh = &meshes.cube;
        winF1_RibR->transform.position = glm::vec3(4.13f, 1.85f, 2.52f);
        winF1_RibR->transform.scale = glm::vec3(0.025f, 1.80f, 0.05f);
        winF1_RibR->color = darkWood;
        root->addChild(winF1_RibR);

        // Right sash horizontal kumiko bars (also slide with the sash)
        std::vector<std::shared_ptr<SceneNode>> winF1_HBarsR;
        for (int hb = 0; hb < 3; ++hb) {
            auto hbarR = std::make_shared<SceneNode>(name + "_WinF1_HBarR_" + std::to_string(hb));
            hbarR->mesh = &meshes.cube;
            hbarR->transform.position = glm::vec3(4.13f, 1.25f + (float)hb * 0.60f, 2.52f);
            hbarR->transform.scale = glm::vec3(0.025f, 0.025f, 1.02f);
            hbarR->color = darkWood;
            root->addChild(hbarR);
            winF1_HBarsR.push_back(hbarR);
        }

        // Register interactive sliding animation for right sash (slides along -Z by 0.95m behind left sash)
        slidingWindowSashes.push_back({ winF1_SashR, winF1_SashR->transform.position, glm::vec3(0.0f, 0.0f, -0.95f) });
        slidingWindowSashes.push_back({ winF1_RibR, winF1_RibR->transform.position, glm::vec3(0.0f, 0.0f, -0.95f) });
        for (auto& hb : winF1_HBarsR)
            slidingWindowSashes.push_back({ hb, hb->transform.position, glm::vec3(0.0f, 0.0f, -0.95f) });

        // Ground Floor Window Garden Decorations:
        // Window Flower Planter Box on exterior window sill
        auto planterF1 = createWindowPlanterBox(meshes, name + "_GFPlanter", glm::vec3(4.30f, 0.92f, 2.00f), 2.10f, 0.0f);
        root->addChild(planterF1);

        // Bonsai Tree on the Window Sill Ledge
        auto bonsaiF1 = createBonsaiTree(meshes, name + "_GFBonsai", glm::vec3(3.95f, 0.92f, 1.15f), 0.75f, 30.0f);
        root->addChild(bonsaiF1);

        // Hanging Kokedama moss ball outside the ground floor window
        auto kokedamaF1 = createHangingKokedama(meshes, name + "_GFKokedama", glm::vec3(4.55f, 2.75f, 2.80f), 0.65f);
        root->addChild(kokedamaF1);

        // Back Wall Garden Sliding Window (at X = -3.95, Z = 0.0)
        // 4-piece open border frame (top, bottom, left, right bars)
        auto winBack_FrameTop = std::make_shared<SceneNode>(name + "_WinBack_FrameTop");
        winBack_FrameTop->mesh = &meshes.cube;
        winBack_FrameTop->transform.position = glm::vec3(-3.95f, 2.86f, 0.0f);
        winBack_FrameTop->transform.scale = glm::vec3(0.12f, 0.12f, 2.44f);
        winBack_FrameTop->color = darkWood;
        root->addChild(winBack_FrameTop);

        auto winBack_FrameBot = std::make_shared<SceneNode>(name + "_WinBack_FrameBot");
        winBack_FrameBot->mesh = &meshes.cube;
        winBack_FrameBot->transform.position = glm::vec3(-3.95f, 0.94f, 0.0f);
        winBack_FrameBot->transform.scale = glm::vec3(0.12f, 0.12f, 2.44f);
        winBack_FrameBot->color = darkWood;
        root->addChild(winBack_FrameBot);

        auto winBack_FrameL = std::make_shared<SceneNode>(name + "_WinBack_FrameL");
        winBack_FrameL->mesh = &meshes.cube;
        winBack_FrameL->transform.position = glm::vec3(-3.95f, 1.90f, 1.18f);
        winBack_FrameL->transform.scale = glm::vec3(0.12f, 1.80f, 0.10f);
        winBack_FrameL->color = darkWood;
        root->addChild(winBack_FrameL);

        auto winBack_FrameR = std::make_shared<SceneNode>(name + "_WinBack_FrameR");
        winBack_FrameR->mesh = &meshes.cube;
        winBack_FrameR->transform.position = glm::vec3(-3.95f, 1.90f, -1.18f);
        winBack_FrameR->transform.scale = glm::vec3(0.12f, 1.80f, 0.10f);
        winBack_FrameR->color = darkWood;
        root->addChild(winBack_FrameR);

        // Left sash (inner track at X = -3.94)
        auto winBack_SashL = std::make_shared<SceneNode>(name + "_WinBack_SashL");
        winBack_SashL->mesh = &meshes.cube;
        winBack_SashL->transform.position = glm::vec3(-3.94f, 1.90f, -0.52f);
        winBack_SashL->transform.scale = glm::vec3(0.035f, 1.85f, 1.05f);
        winBack_SashL->isWindow = true;
        winBack_SashL->color = paperColor;
        root->addChild(winBack_SashL);
        windows.push_back(winBack_SashL);

        auto winBack_RibL = std::make_shared<SceneNode>(name + "_WinBack_RibL");
        winBack_RibL->mesh = &meshes.cube;
        winBack_RibL->transform.position = glm::vec3(-3.92f, 1.90f, -0.52f);
        winBack_RibL->transform.scale = glm::vec3(0.025f, 1.80f, 0.05f);
        winBack_RibL->color = darkWood;
        root->addChild(winBack_RibL);

        // Right sash (outer track at X = -3.98)
        auto winBack_SashR = std::make_shared<SceneNode>(name + "_WinBack_SashR");
        winBack_SashR->mesh = &meshes.cube;
        winBack_SashR->transform.position = glm::vec3(-3.98f, 1.90f, 0.52f);
        winBack_SashR->transform.scale = glm::vec3(0.035f, 1.85f, 1.05f);
        winBack_SashR->isWindow = true;
        winBack_SashR->color = paperColor;
        root->addChild(winBack_SashR);
        windows.push_back(winBack_SashR);

        auto winBack_RibR = std::make_shared<SceneNode>(name + "_WinBack_RibR");
        winBack_RibR->mesh = &meshes.cube;
        winBack_RibR->transform.position = glm::vec3(-4.00f, 1.90f, 0.52f);
        winBack_RibR->transform.scale = glm::vec3(0.025f, 1.80f, 0.05f);
        winBack_RibR->color = darkWood;
        root->addChild(winBack_RibR);

        // Right sash horizontal kumiko bars
        std::vector<std::shared_ptr<SceneNode>> winBack_HBarsR;
        for (int hb = 0; hb < 3; ++hb) {
            auto hbarR = std::make_shared<SceneNode>(name + "_WinBack_HBarR_" + std::to_string(hb));
            hbarR->mesh = &meshes.cube;
            hbarR->transform.position = glm::vec3(-4.00f, 1.30f + (float)hb * 0.60f, 0.52f);
            hbarR->transform.scale = glm::vec3(0.025f, 0.025f, 1.02f);
            hbarR->color = darkWood;
            root->addChild(hbarR);
            winBack_HBarsR.push_back(hbarR);
        }

        // Register interactive sliding animation for back right sash (slides along -Z by 0.95m behind left sash)
        slidingWindowSashes.push_back({ winBack_SashR, winBack_SashR->transform.position, glm::vec3(0.0f, 0.0f, -0.95f) });
        slidingWindowSashes.push_back({ winBack_RibR, winBack_RibR->transform.position, glm::vec3(0.0f, 0.0f, -0.95f) });
        for (auto& hb : winBack_HBarsR)
            slidingWindowSashes.push_back({ hb, hb->transform.position, glm::vec3(0.0f, 0.0f, -0.95f) });

        // Planter box at back garden window
        auto planterBack = createWindowPlanterBox(meshes, name + "_BackPlanter", glm::vec3(-4.18f, 0.90f, 0.0f), 2.00f, 180.0f);
        root->addChild(planterBack);

        // =========================================================
        // 3. GROUND FLOOR INTERIOR: GENKAN, LIVING ROOM & TEA ROOM
        // =========================================================
        // Genkan Sunken Stone Floor
        auto genkanFloor = std::make_shared<SceneNode>(name + "_GenkanFloor");
        genkanFloor->mesh = &meshes.cube;
        genkanFloor->transform.position = glm::vec3(3.20f, 0.05f, -0.90f);
        genkanFloor->transform.scale = glm::vec3(1.40f, 0.10f, 2.40f);
        genkanFloor->color = stone;
        root->addChild(genkanFloor);

        // Agari-kamachi Wooden Threshold
        auto agariKamachi = std::make_shared<SceneNode>(name + "_AgariKamachi");
        agariKamachi->mesh = &meshes.cube;
        agariKamachi->transform.position = glm::vec3(2.45f, 0.14f, -0.90f);
        agariKamachi->transform.scale = glm::vec3(0.16f, 0.20f, 2.40f);
        agariKamachi->color = darkWood;
        root->addChild(agariKamachi);

        // Geta-bako Shoe Cabinet in Genkan
        auto shoeCabinet = std::make_shared<SceneNode>(name + "_ShoeCabinet");
        shoeCabinet->mesh = &meshes.cube;
        shoeCabinet->transform.position = glm::vec3(3.25f, 0.45f, 0.12f);
        shoeCabinet->transform.scale = glm::vec3(0.55f, 0.70f, 0.35f);
        shoeCabinet->color = darkWood;
        root->addChild(shoeCabinet);

        // Small Ikebana Flower Vase atop the Shoe Cabinet
        auto genkanIkebana = createIkebanaVase(meshes, name + "_GenkanIkebana", glm::vec3(3.25f, 0.80f, 0.12f), 0.55f, 1);
        root->addChild(genkanIkebana);

        // Main Living Room Raised Tatami Floor (spanning X in [-3.8, 2.4], Z in [-4.3, 4.3])
        auto tatamiFloor = std::make_shared<SceneNode>(name + "_TatamiFloorF1");
        tatamiFloor->mesh = &meshes.cube;
        tatamiFloor->transform.position = glm::vec3(-0.75f, 0.18f, 0.65f);
        tatamiFloor->transform.scale = glm::vec3(6.20f, 0.14f, 7.30f);
        tatamiFloor->color = tatamiColor;
        root->addChild(tatamiFloor);

        // Chabudai Low Mahogany Floor Table
        auto chabudaiTable = std::make_shared<SceneNode>(name + "_ChabudaiTable");
        chabudaiTable->mesh = &meshes.cube;
        chabudaiTable->transform.position = glm::vec3(-0.30f, 0.52f, 1.10f);
        chabudaiTable->transform.scale = glm::vec3(1.70f, 0.08f, 1.30f);
        chabudaiTable->color = tableWood;
        chabudaiTable->shininess = 48.0f;
        chabudaiTable->specularStrength = 0.60f;
        root->addChild(chabudaiTable);

        // 4 Table Legs
        float lx[2] = { -0.95f, 0.35f };
        float lz[2] = { 0.60f, 1.60f };
        for (int x = 0; x < 2; ++x) {
            for (int z = 0; z < 2; ++z) {
                auto leg = std::make_shared<SceneNode>(name + "_TableLeg_" + std::to_string(x) + "_" + std::to_string(z));
                leg->mesh = &meshes.cylinder;
                leg->transform.position = glm::vec3(lx[x], 0.28f, lz[z]);
                leg->transform.scale = glm::vec3(0.08f, 0.44f, 0.08f);
                leg->color = tableWood;
                root->addChild(leg);
            }
        }

        // 4 Silk Zabuton Floor Cushions
        auto zabutonF = std::make_shared<SceneNode>(name + "_Zabuton_F");
        zabutonF->mesh = &meshes.cube;
        zabutonF->transform.position = glm::vec3(-0.30f, 0.23f, 0.22f);
        zabutonF->transform.scale = glm::vec3(0.60f, 0.08f, 0.55f);
        zabutonF->color = glm::vec4(0.78f, 0.16f, 0.16f, 1.0f);
        root->addChild(zabutonF);

        auto zabutonB = std::make_shared<SceneNode>(name + "_Zabuton_B");
        zabutonB->mesh = &meshes.cube;
        zabutonB->transform.position = glm::vec3(-0.30f, 0.23f, 1.98f);
        zabutonB->transform.scale = glm::vec3(0.60f, 0.08f, 0.55f);
        zabutonB->color = glm::vec4(0.78f, 0.16f, 0.16f, 1.0f);
        root->addChild(zabutonB);

        auto zabutonL = std::make_shared<SceneNode>(name + "_Zabuton_L");
        zabutonL->mesh = &meshes.cube;
        zabutonL->transform.position = glm::vec3(-1.35f, 0.23f, 1.10f);
        zabutonL->transform.scale = glm::vec3(0.55f, 0.08f, 0.60f);
        zabutonL->color = glm::vec4(0.18f, 0.26f, 0.55f, 1.0f);
        root->addChild(zabutonL);

        auto zabutonR = std::make_shared<SceneNode>(name + "_Zabuton_R");
        zabutonR->mesh = &meshes.cube;
        zabutonR->transform.position = glm::vec3(0.75f, 0.23f, 1.10f);
        zabutonR->transform.scale = glm::vec3(0.55f, 0.08f, 0.60f);
        zabutonR->color = glm::vec4(0.18f, 0.26f, 0.55f, 1.0f);
        root->addChild(zabutonR);

        // Bamboo Tea Serving Tray
        auto teaTray = std::make_shared<SceneNode>(name + "_TeaTray");
        teaTray->mesh = &meshes.cube;
        teaTray->transform.position = glm::vec3(-0.30f, 0.58f, 1.10f);
        teaTray->transform.scale = glm::vec3(0.60f, 0.04f, 0.42f);
        teaTray->color = glm::vec4(0.48f, 0.36f, 0.20f, 1.0f);
        root->addChild(teaTray);

        // Ceramic Kyusu Teapot & 2 Celadon Yunomi Cups
        auto teapot = std::make_shared<SceneNode>(name + "_KyusuPot");
        teapot->mesh = &meshes.sphere;
        teapot->transform.position = glm::vec3(-0.38f, 0.66f, 1.10f);
        teapot->transform.scale = glm::vec3(0.18f, 0.14f, 0.18f);
        teapot->color = glm::vec4(0.18f, 0.16f, 0.16f, 1.0f);
        root->addChild(teapot);

        auto potSpout = std::make_shared<SceneNode>(name + "_KyusuSpout");
        potSpout->mesh = &meshes.cylinder;
        potSpout->transform.position = glm::vec3(-0.48f, 0.68f, 1.10f);
        potSpout->transform.rotation.z = 45.0f;
        potSpout->transform.scale = glm::vec3(0.04f, 0.12f, 0.04f);
        potSpout->color = glm::vec4(0.18f, 0.16f, 0.16f, 1.0f);
        root->addChild(potSpout);

        auto potHandle = std::make_shared<SceneNode>(name + "_KyusuHandle");
        potHandle->mesh = &meshes.cylinder;
        potHandle->transform.position = glm::vec3(-0.38f, 0.68f, 1.22f);
        potHandle->transform.rotation.x = 90.0f;
        potHandle->transform.scale = glm::vec3(0.03f, 0.14f, 0.03f);
        potHandle->color = darkWood;
        root->addChild(potHandle);

        auto cup1 = std::make_shared<SceneNode>(name + "_Yunomi_1");
        cup1->mesh = &meshes.cylinder;
        cup1->transform.position = glm::vec3(-0.18f, 0.64f, 1.02f);
        cup1->transform.scale = glm::vec3(0.08f, 0.10f, 0.08f);
        cup1->color = glm::vec4(0.38f, 0.58f, 0.48f, 1.0f);
        root->addChild(cup1);

        auto cup2 = std::make_shared<SceneNode>(name + "_Yunomi_2");
        cup2->mesh = &meshes.cylinder;
        cup2->transform.position = glm::vec3(-0.18f, 0.64f, 1.18f);
        cup2->transform.scale = glm::vec3(0.08f, 0.10f, 0.08f);
        cup2->color = glm::vec4(0.38f, 0.58f, 0.48f, 1.0f);
        root->addChild(cup2);

        // REALISTIC GROUND FLOOR LAMPS:
        // 1. Standing Andon Floor Lamp with Carved Legs & Lattice
        auto andonFloor = createAndonFloorLamp(meshes, name + "_GFAndon", glm::vec3(-3.30f, 0.22f, 3.60f), 1.0f);
        root->addChild(andonFloor);

        // 2. Ceiling Pendant Washi Chandelier Lantern above the Tea Table
        auto pendantGF = createCeilingPendantLamp(meshes, name + "_GFPendant", glm::vec3(-0.30f, 4.15f, 1.10f), 0.70f);
        root->addChild(pendantGF);

        // Tokonoma Sacred Alcove Raised Platform (Toko-kamachi)
        auto tokonomaPlinth = std::make_shared<SceneNode>(name + "_TokonomaPlinth");
        tokonomaPlinth->mesh = &meshes.cube;
        tokonomaPlinth->transform.position = glm::vec3(-3.45f, 0.26f, 2.00f);
        tokonomaPlinth->transform.scale = glm::vec3(0.90f, 0.12f, 1.60f);
        tokonomaPlinth->color = darkWood;
        root->addChild(tokonomaPlinth);

        // Decorative Hanging Calligraphy Wall Scroll (Kakemono)
        auto scroll = std::make_shared<SceneNode>(name + "_WallScroll");
        scroll->mesh = &meshes.cube;
        scroll->transform.position = glm::vec3(-3.83f, 2.20f, 2.00f);
        scroll->transform.scale = glm::vec3(0.04f, 1.60f, 0.70f);
        scroll->color = glm::vec4(0.18f, 0.22f, 0.42f, 1.0f);
        root->addChild(scroll);

        // Detailed Colorful Ikebana Flower Vase in the Tokonoma Corner
        auto tokonomaIkebana = createIkebanaVase(meshes, name + "_TokonomaIkebana", glm::vec3(-3.40f, 0.32f, 2.45f), 0.75f, 0);
        root->addChild(tokonomaIkebana);

        // =========================================================
        // 4. AUTHENTIC 14-STEP WOODEN STAIRCASE (Hakokaidan)
        // =========================================================
        // Ascends along right wall from X = +0.75 (ground living area) to X = -1.95 (upper landing gallery)
        // Leaving a generous 1.75m spacious landing gallery between the top step and the back corner wall
        const int numSteps = 14;
        const float stairX0 = 0.75f;
        const float stairX1 = -1.95f;
        const float stairY0 = 0.20f;
        const float stairY1 = 4.30f;
        const float stepWidth = 1.05f;
        const float stepRun = (stairX0 - stairX1) / (float)numSteps;
        const float stepRise = (stairY1 - stairY0) / (float)numSteps;

        for (int k = 0; k < numSteps; ++k)
        {
            float stepX = stairX0 - (float)k * stepRun - stepRun * 0.5f;
            float stepY = stairY0 + (float)(k + 1) * stepRise;

            // Polished Japanese Cedar Step Tread (with rounded bullnose overhang)
            auto tread = std::make_shared<SceneNode>(name + "_StairTread_" + std::to_string(k));
            tread->mesh = &meshes.cube;
            tread->transform.position = glm::vec3(stepX, stepY, -3.50f);
            tread->transform.scale = glm::vec3(stepRun * 1.15f, 0.055f, stepWidth);
            tread->color = darkWood;
            tread->shininess = 32.0f;
            tread->specularStrength = 0.35f;
            root->addChild(tread);

            // Step Riser Board
            auto riser = std::make_shared<SceneNode>(name + "_StairRiser_" + std::to_string(k));
            riser->mesh = &meshes.cube;
            riser->transform.position = glm::vec3(stepX + stepRun * 0.5f, stepY - stepRise * 0.5f, -3.50f);
            riser->transform.scale = glm::vec3(0.04f, stepRise, stepWidth);
            riser->color = timber;
            root->addChild(riser);

            // Under-stair Hakokaidan Solid Cabinet & Drawer Panels
            auto underBlock = std::make_shared<SceneNode>(name + "_StairCabinet_" + std::to_string(k));
            underBlock->mesh = &meshes.cube;
            float underH = stepY - stepRise;
            if (underH > 0.05f)
            {
                underBlock->transform.position = glm::vec3(stepX, underH * 0.5f, -3.50f);
                underBlock->transform.scale = glm::vec3(stepRun, underH, stepWidth);
                underBlock->color = timber;
                root->addChild(underBlock);

                // Brass Drawer Pull on Cabinet Side facing room
                if (k % 2 == 0 && underH > 0.4f)
                {
                    auto pull = std::make_shared<SceneNode>(name + "_DrawerPull_" + std::to_string(k));
                    pull->mesh = &meshes.cylinder;
                    pull->transform.position = glm::vec3(stepX, underH * 0.5f, -3.50f + stepWidth * 0.51f);
                    pull->transform.rotation.x = 90.0f;
                    pull->transform.scale = glm::vec3(0.035f, 0.03f, 0.035f);
                    pull->color = glm::vec4(0.85f, 0.72f, 0.28f, 1.0f); // polished brass ring
                    root->addChild(pull);
                }
            }

            // Turned Balusters every 2 steps on open side of stairs
            if (k % 2 == 1 && k < numSteps - 1)
            {
                auto baluster = std::make_shared<SceneNode>(name + "_Baluster_" + std::to_string(k));
                baluster->mesh = &meshes.cylinder;
                baluster->transform.position = glm::vec3(stepX, stepY + 0.45f, -3.50f + stepWidth * 0.48f);
                baluster->transform.scale = glm::vec3(0.035f, 0.85f, 0.035f);
                baluster->color = darkWood;
                root->addChild(baluster);
            }
        }

        // Slanted Wooden Stair Handrail Balustrade & Newel Posts
        float hrMx = (stairX0 + stairX1) * 0.5f;
        float hrMy = (stairY0 + stairY1) * 0.5f + 0.90f;
        float hrDx = stairX1 - stairX0;
        float hrDy = stairY1 - stairY0;
        float hrLen = std::sqrt(hrDx * hrDx + hrDy * hrDy);
        float hrAngle = glm::degrees(std::atan2(hrDy, -hrDx));

        auto handrail = std::make_shared<SceneNode>(name + "_StairHandrail");
        handrail->mesh = &meshes.cylinder;
        handrail->transform.position = glm::vec3(hrMx, hrMy, -3.50f + stepWidth * 0.48f);
        handrail->transform.rotation.z = -hrAngle;
        handrail->transform.scale = glm::vec3(0.055f, hrLen, 0.055f);
        handrail->color = darkWood;
        root->addChild(handrail);

        // Bottom Newel Post
        auto newelBot = std::make_shared<SceneNode>(name + "_NewelBot");
        newelBot->mesh = &meshes.cube;
        newelBot->transform.position = glm::vec3(stairX0, stairY0 + 0.50f, -3.50f + stepWidth * 0.48f);
        newelBot->transform.scale = glm::vec3(0.08f, 1.00f, 0.08f);
        newelBot->color = darkWood;
        root->addChild(newelBot);

        // Top Newel Post at the Landing Entrance
        auto newelTop = std::make_shared<SceneNode>(name + "_NewelTop");
        newelTop->mesh = &meshes.cube;
        newelTop->transform.position = glm::vec3(stairX1, stairY1 + 0.50f, -3.50f + stepWidth * 0.48f);
        newelTop->transform.scale = glm::vec3(0.08f, 1.00f, 0.08f);
        newelTop->color = darkWood;
        root->addChild(newelTop);

        // Intermediate Eaves Roof dividing 1st & 2nd floors (Exterior Hisashi overhangs)
        // Exterior sloped eaves protecting outer walls, leaving the interior and stairwell completely open
        auto eavesFront = std::make_shared<SceneNode>(name + "_EavesFront");
        eavesFront->mesh = &meshes.cube;
        eavesFront->transform.position = glm::vec3(4.25f, 4.35f, 0.0f);
        eavesFront->transform.rotation.z = -14.0f;
        eavesFront->transform.scale = glm::vec3(0.90f, 0.18f, 9.40f);
        eavesFront->color = roofSlate;
        root->addChild(eavesFront);

        auto eavesBack = std::make_shared<SceneNode>(name + "_EavesBack");
        eavesBack->mesh = &meshes.cube;
        eavesBack->transform.position = glm::vec3(-4.25f, 4.35f, 0.0f);
        eavesBack->transform.rotation.z = 14.0f;
        eavesBack->transform.scale = glm::vec3(0.90f, 0.18f, 9.40f);
        eavesBack->color = roofSlate;
        root->addChild(eavesBack);

        auto eavesLeft = std::make_shared<SceneNode>(name + "_EavesLeft");
        eavesLeft->mesh = &meshes.cube;
        eavesLeft->transform.position = glm::vec3(0.0f, 4.35f, 4.70f);
        eavesLeft->transform.rotation.x = 14.0f;
        eavesLeft->transform.scale = glm::vec3(8.20f, 0.18f, 0.90f);
        eavesLeft->color = roofSlate;
        root->addChild(eavesLeft);

        auto eavesRight = std::make_shared<SceneNode>(name + "_EavesRight");
        eavesRight->mesh = &meshes.cube;
        eavesRight->transform.position = glm::vec3(0.0f, 4.35f, -4.70f);
        eavesRight->transform.rotation.x = -14.0f;
        eavesRight->transform.scale = glm::vec3(8.20f, 0.18f, 0.90f);
        eavesRight->color = roofSlate;
        root->addChild(eavesRight);

        // =========================================================
        // 5. SECOND FLOOR: BEDROOM (Shinshitsu) & FURNISHINGS
        // =========================================================
        // Full Second Floor Tatami Floor System (top walking surface exactly Y = 4.30m)
        // 1. Main Bedroom Tatami Slab (spanning Z from -2.85m to +4.15m across full X from -3.70m to +3.70m)
        auto floor2Main = std::make_shared<SceneNode>(name + "_Floor2TatamiMain");
        floor2Main->mesh = &meshes.cube;
        floor2Main->transform.position = glm::vec3(0.0f, 4.22f, 0.65f);
        floor2Main->transform.scale = glm::vec3(7.40f, 0.16f, 7.00f);
        floor2Main->color = tatamiColor;
        root->addChild(floor2Main);

        // 2. Spacious Upper Stair Landing Gallery (from X = -3.70m to -1.95m across Z in [-4.15m, -2.85m])
        // Generous 1.75m x 1.30m open landing providing ample standing room with clear walkthrough into bedroom!
        auto floor2Landing = std::make_shared<SceneNode>(name + "_Floor2TatamiLanding");
        floor2Landing->mesh = &meshes.cube;
        floor2Landing->transform.position = glm::vec3(-2.825f, 4.22f, -3.50f);
        floor2Landing->transform.scale = glm::vec3(1.75f, 0.16f, 1.30f);
        floor2Landing->color = tatamiColor;
        root->addChild(floor2Landing);

        // 3. Front Floor Area ahead of stairwell opening (from X = +0.75m to +3.70m along Z = -3.50m)
        auto floor2Front = std::make_shared<SceneNode>(name + "_Floor2TatamiFront");
        floor2Front->mesh = &meshes.cube;
        floor2Front->transform.position = glm::vec3(2.225f, 4.22f, -3.50f);
        floor2Front->transform.scale = glm::vec3(2.95f, 0.16f, 1.30f);
        floor2Front->color = tatamiColor;
        root->addChild(floor2Front);

        // Authentic Japanese Balustrade Guardrail around Upper Stairwell Opening
        // Long side guardrail along bedroom edge (Z = -2.88m, from X = -1.95m to X = +0.75m, length = 2.70m)
        auto guardTopRail = std::make_shared<SceneNode>(name + "_StairGuardTopRail");
        guardTopRail->mesh = &meshes.cube;
        guardTopRail->transform.position = glm::vec3(-0.60f, 5.15f, -2.88f);
        guardTopRail->transform.scale = glm::vec3(2.72f, 0.07f, 0.07f);
        guardTopRail->color = darkWood;
        root->addChild(guardTopRail);

        auto guardBotRail = std::make_shared<SceneNode>(name + "_StairGuardBotRail");
        guardBotRail->mesh = &meshes.cube;
        guardBotRail->transform.position = glm::vec3(-0.60f, 4.36f, -2.88f);
        guardBotRail->transform.scale = glm::vec3(2.72f, 0.07f, 0.07f);
        guardBotRail->color = darkWood;
        root->addChild(guardBotRail);

        // Corner newel post at the landing entrance
        auto guardPostLanding = std::make_shared<SceneNode>(name + "_StairGuardPostLanding");
        guardPostLanding->mesh = &meshes.cube;
        guardPostLanding->transform.position = glm::vec3(-1.95f, 4.75f, -2.88f);
        guardPostLanding->transform.scale = glm::vec3(0.08f, 0.88f, 0.08f);
        guardPostLanding->color = darkWood;
        root->addChild(guardPostLanding);

        // Corner newel post at the front edge of the stairwell
        auto guardPostFront = std::make_shared<SceneNode>(name + "_StairGuardPostFront");
        guardPostFront->mesh = &meshes.cube;
        guardPostFront->transform.position = glm::vec3(0.75f, 4.75f, -2.88f);
        guardPostFront->transform.scale = glm::vec3(0.08f, 0.88f, 0.08f);
        guardPostFront->color = darkWood;
        root->addChild(guardPostFront);

        // Vertical cedar balusters along the bedroom guardrail
        for (int gb = 0; gb < 6; ++gb) {
            float gbx = -1.60f + (float)gb * 0.40f;
            auto bal = std::make_shared<SceneNode>(name + "_StairGuardBal_" + std::to_string(gb));
            bal->mesh = &meshes.cylinder;
            bal->transform.position = glm::vec3(gbx, 4.75f, -2.88f);
            bal->transform.scale = glm::vec3(0.035f, 0.72f, 0.035f);
            bal->color = darkWood;
            root->addChild(bal);
        }

        // Front end guardrail across the front opening (at X = +0.75m, from Z = -2.88m to Z = -4.15m)
        auto guardFrontEndTop = std::make_shared<SceneNode>(name + "_StairGuardFrontTop");
        guardFrontEndTop->mesh = &meshes.cube;
        guardFrontEndTop->transform.position = glm::vec3(0.75f, 5.15f, -3.515f);
        guardFrontEndTop->transform.scale = glm::vec3(0.07f, 0.07f, 1.25f);
        guardFrontEndTop->color = darkWood;
        root->addChild(guardFrontEndTop);

        auto guardFrontEndBot = std::make_shared<SceneNode>(name + "_StairGuardFrontBot");
        guardFrontEndBot->mesh = &meshes.cube;
        guardFrontEndBot->transform.position = glm::vec3(0.75f, 4.36f, -3.515f);
        guardFrontEndBot->transform.scale = glm::vec3(0.07f, 0.07f, 1.25f);
        guardFrontEndBot->color = darkWood;
        root->addChild(guardFrontEndBot);

        auto guardPostWall = std::make_shared<SceneNode>(name + "_StairGuardPostWall");
        guardPostWall->mesh = &meshes.cube;
        guardPostWall->transform.position = glm::vec3(0.75f, 4.75f, -4.14f);
        guardPostWall->transform.scale = glm::vec3(0.08f, 0.88f, 0.08f);
        guardPostWall->color = darkWood;
        root->addChild(guardPostWall);

        for (int fb = 0; fb < 3; ++fb) {
            float fbz = -3.15f - (float)fb * 0.35f;
            auto balF = std::make_shared<SceneNode>(name + "_StairGuardFrontBal_" + std::to_string(fb));
            balF->mesh = &meshes.cylinder;
            balF->transform.position = glm::vec3(0.75f, 4.75f, fbz);
            balF->transform.scale = glm::vec3(0.035f, 0.72f, 0.035f);
            balF->color = darkWood;
            root->addChild(balF);
        }

        // Cedar Threshold Beam at front edge of stairwell
        auto stairSill = std::make_shared<SceneNode>(name + "_StairSill");
        stairSill->mesh = &meshes.cube;
        stairSill->transform.position = glm::vec3(0.75f, 4.32f, -3.515f);
        stairSill->transform.scale = glm::vec3(0.10f, 0.08f, 1.25f);
        stairSill->color = darkWood;
        root->addChild(stairSill);

        // Second floor exterior walls
        auto floor2WallBack = std::make_shared<SceneNode>(name + "_F2WallBack");
        floor2WallBack->mesh = &meshes.cube;
        floor2WallBack->transform.position = glm::vec3(-3.70f, 6.10f, 0.0f);
        floor2WallBack->transform.scale = glm::vec3(0.20f, 3.60f, 8.40f);
        floor2WallBack->color = timber;
        root->addChild(floor2WallBack);

        auto floor2WallLeft = std::make_shared<SceneNode>(name + "_F2WallLeft");
        floor2WallLeft->mesh = &meshes.cube;
        floor2WallLeft->transform.position = glm::vec3(0.0f, 6.10f, 4.20f);
        floor2WallLeft->transform.scale = glm::vec3(7.40f, 3.60f, 0.20f);
        floor2WallLeft->color = timber;
        root->addChild(floor2WallLeft);

        auto floor2WallRight = std::make_shared<SceneNode>(name + "_F2WallRight");
        floor2WallRight->mesh = &meshes.cube;
        floor2WallRight->transform.position = glm::vec3(0.0f, 6.10f, -4.20f);
        floor2WallRight->transform.scale = glm::vec3(7.40f, 3.60f, 0.20f);
        floor2WallRight->color = timber;
        root->addChild(floor2WallRight);

        // 2nd Floor Front Facade: Framed Walls with Open Window Bay Cutouts
        // Central wall section between the two windows
        auto f2WallFrontCenter = std::make_shared<SceneNode>(name + "_F2WallFrontCenter");
        f2WallFrontCenter->mesh = &meshes.cube;
        f2WallFrontCenter->transform.position = glm::vec3(3.70f, 6.10f, 0.0f);
        f2WallFrontCenter->transform.scale = glm::vec3(0.20f, 3.60f, 2.00f);
        f2WallFrontCenter->color = timber;
        root->addChild(f2WallFrontCenter);

        // Far-left front wall section
        auto f2WallFrontLeft = std::make_shared<SceneNode>(name + "_F2WallFrontLeft");
        f2WallFrontLeft->mesh = &meshes.cube;
        f2WallFrontLeft->transform.position = glm::vec3(3.70f, 6.10f, 3.70f);
        f2WallFrontLeft->transform.scale = glm::vec3(0.20f, 3.60f, 1.00f);
        f2WallFrontLeft->color = timber;
        root->addChild(f2WallFrontLeft);

        // Far-right front wall section
        auto f2WallFrontRight = std::make_shared<SceneNode>(name + "_F2WallFrontRight");
        f2WallFrontRight->mesh = &meshes.cube;
        f2WallFrontRight->transform.position = glm::vec3(3.70f, 6.10f, -3.70f);
        f2WallFrontRight->transform.scale = glm::vec3(0.20f, 3.60f, 1.00f);
        f2WallFrontRight->color = timber;
        root->addChild(f2WallFrontRight);

        // Wall sections below and above the two windows
        float f2WinZ[2] = { -2.10f, 2.10f };
        for (int w = 0; w < 2; ++w) {
            float wz = f2WinZ[w];

            // Sub-window sill wall
            auto f2WallSill = std::make_shared<SceneNode>(name + "_F2WallSill_" + std::to_string(w));
            f2WallSill->mesh = &meshes.cube;
            f2WallSill->transform.position = glm::vec3(3.70f, 4.725f, wz);
            f2WallSill->transform.scale = glm::vec3(0.20f, 0.85f, 2.20f);
            f2WallSill->color = timber;
            root->addChild(f2WallSill);

            // Header wall above window
            auto f2WallHeader = std::make_shared<SceneNode>(name + "_F2WallHeader_" + std::to_string(w));
            f2WallHeader->mesh = &meshes.cube;
            f2WallHeader->transform.position = glm::vec3(3.70f, 7.575f, wz);
            f2WallHeader->transform.scale = glm::vec3(0.20f, 0.65f, 2.20f);
            f2WallHeader->color = timber;
            root->addChild(f2WallHeader);
        }

        // Exposed Ceiling Crossbeams across the Second Floor
        for (int b = -1; b <= 1; ++b) {
            auto beam = std::make_shared<SceneNode>(name + "_F2Beam_" + std::to_string(b));
            beam->mesh = &meshes.cube;
            beam->transform.position = glm::vec3(0.0f, 7.80f, (float)b * 2.6f);
            beam->transform.scale = glm::vec3(7.20f, 0.22f, 0.22f);
            beam->color = darkWood;
            root->addChild(beam);
        }

        // SECOND FLOOR SLIDING WINDOWS (2 pairs facing festival street, at Z = -2.1 and Z = 2.1)
        // Authentic Shoji Windows with 4-piece open border frames, dual-track sashes, and kumiko lattice mullions
        for (int w = 0; w < 2; ++w)
        {
            float wz = f2WinZ[w];

            // 4-piece open border timber frame (top, bottom, left, right border bars)
            auto winFrameTop = std::make_shared<SceneNode>(name + "_F2WinFrameTop_" + std::to_string(w));
            winFrameTop->mesh = &meshes.cube;
            winFrameTop->transform.position = glm::vec3(3.74f, 7.20f, wz);
            winFrameTop->transform.scale = glm::vec3(0.12f, 0.10f, 2.24f);
            winFrameTop->color = darkWood;
            root->addChild(winFrameTop);

            auto winFrameBot = std::make_shared<SceneNode>(name + "_F2WinFrameBot_" + std::to_string(w));
            winFrameBot->mesh = &meshes.cube;
            winFrameBot->transform.position = glm::vec3(3.74f, 5.20f, wz);
            winFrameBot->transform.scale = glm::vec3(0.12f, 0.10f, 2.24f);
            winFrameBot->color = darkWood;
            root->addChild(winFrameBot);

            auto winFrameL = std::make_shared<SceneNode>(name + "_F2WinFrameL_" + std::to_string(w));
            winFrameL->mesh = &meshes.cube;
            winFrameL->transform.position = glm::vec3(3.74f, 6.20f, wz + 1.07f);
            winFrameL->transform.scale = glm::vec3(0.12f, 1.90f, 0.10f);
            winFrameL->color = darkWood;
            root->addChild(winFrameL);

            auto winFrameR = std::make_shared<SceneNode>(name + "_F2WinFrameR_" + std::to_string(w));
            winFrameR->mesh = &meshes.cube;
            winFrameR->transform.position = glm::vec3(3.74f, 6.20f, wz - 1.07f);
            winFrameR->transform.scale = glm::vec3(0.12f, 1.90f, 0.10f);
            winFrameR->color = darkWood;
            root->addChild(winFrameR);

            // Exterior Window Sill Ledge
            auto winSillLedge = std::make_shared<SceneNode>(name + "_F2WinSillLedge_" + std::to_string(w));
            winSillLedge->mesh = &meshes.cube;
            winSillLedge->transform.position = glm::vec3(3.88f, 5.16f, wz);
            winSillLedge->transform.scale = glm::vec3(0.30f, 0.08f, 2.30f);
            winSillLedge->color = darkWood;
            root->addChild(winSillLedge);

            // Left Sliding Sash (Inner track at X = 3.71)
            auto sashL = std::make_shared<SceneNode>(name + "_F2SashL_" + std::to_string(w));
            sashL->mesh = &meshes.cube;
            sashL->transform.position = glm::vec3(3.71f, 6.20f, wz - 0.48f);
            sashL->transform.scale = glm::vec3(0.035f, 1.88f, 1.00f);
            sashL->isWindow = true;
            sashL->color = paperColor;
            root->addChild(sashL);
            windows.push_back(sashL);

            // Left sash vertical kumiko rib (offset slightly outward to eliminate Z-fighting)
            auto ribL = std::make_shared<SceneNode>(name + "_F2RibL_" + std::to_string(w));
            ribL->mesh = &meshes.cube;
            ribL->transform.position = glm::vec3(3.73f, 6.20f, wz - 0.48f);
            ribL->transform.scale = glm::vec3(0.025f, 1.84f, 0.05f);
            ribL->color = darkWood;
            root->addChild(ribL);

            // Left sash horizontal kumiko lattice bars
            for (int hb = 0; hb < 3; ++hb) {
                auto hbarL = std::make_shared<SceneNode>(name + "_F2HBarL_" + std::to_string(w) + "_" + std::to_string(hb));
                hbarL->mesh = &meshes.cube;
                hbarL->transform.position = glm::vec3(3.73f, 5.60f + (float)hb * 0.60f, wz - 0.48f);
                hbarL->transform.scale = glm::vec3(0.025f, 0.025f, 0.98f);
                hbarL->color = darkWood;
                root->addChild(hbarL);
            }

            // Right Sliding Sash (Outer track at X = 3.76)
            auto sashR = std::make_shared<SceneNode>(name + "_F2SashR_" + std::to_string(w));
            sashR->mesh = &meshes.cube;
            sashR->transform.position = glm::vec3(3.76f, 6.20f, wz + 0.48f);
            sashR->transform.scale = glm::vec3(0.035f, 1.88f, 1.00f);
            sashR->isWindow = true;
            sashR->color = paperColor;
            root->addChild(sashR);
            windows.push_back(sashR);

            // Right sash vertical kumiko rib (offset slightly outward to eliminate Z-fighting)
            auto ribR = std::make_shared<SceneNode>(name + "_F2RibR_" + std::to_string(w));
            ribR->mesh = &meshes.cube;
            ribR->transform.position = glm::vec3(3.78f, 6.20f, wz + 0.48f);
            ribR->transform.scale = glm::vec3(0.025f, 1.84f, 0.05f);
            ribR->color = darkWood;
            root->addChild(ribR);

            // Right sash horizontal kumiko lattice bars (also slide when window opens)
            std::vector<std::shared_ptr<SceneNode>> f2HBarsR;
            for (int hb = 0; hb < 3; ++hb) {
                auto hbarR = std::make_shared<SceneNode>(name + "_F2HBarR_" + std::to_string(w) + "_" + std::to_string(hb));
                hbarR->mesh = &meshes.cube;
                hbarR->transform.position = glm::vec3(3.78f, 5.60f + (float)hb * 0.60f, wz + 0.48f);
                hbarR->transform.scale = glm::vec3(0.025f, 0.025f, 0.98f);
                hbarR->color = darkWood;
                root->addChild(hbarR);
                f2HBarsR.push_back(hbarR);
            }

            // Register right sash to slide open neatly behind left sash (slides along -Z by 0.90m)
            slidingWindowSashes.push_back({ sashR, sashR->transform.position, glm::vec3(0.0f, 0.0f, -0.90f) });
            slidingWindowSashes.push_back({ ribR, ribR->transform.position, glm::vec3(0.0f, 0.0f, -0.90f) });
            for (auto& hb : f2HBarsR)
                slidingWindowSashes.push_back({ hb, hb->transform.position, glm::vec3(0.0f, 0.0f, -0.90f) });

            // Exterior Window Sill Planter Box on 2nd Floor
            auto planterF2 = createWindowPlanterBox(meshes, name + "_F2Planter_" + std::to_string(w), glm::vec3(3.96f, 5.25f, wz), 2.10f, 0.0f);
            root->addChild(planterF2);

            // Hanging Kokedama under second floor eaves
            auto kokedamaF2 = createHangingKokedama(meshes, name + "_F2Kokedama_" + std::to_string(w), glm::vec3(4.25f, 7.30f, wz + 0.75f), 0.70f);
            root->addChild(kokedamaF2);
        }

        // Bonsai tree placed on the 2nd Floor window sill ledge (visible from room & street)
        auto bonsaiF2 = createBonsaiTree(meshes, name + "_F2Bonsai", glm::vec3(3.68f, 5.28f, -1.25f), 0.70f, -25.0f);
        root->addChild(bonsaiF2);

        // ---------------------------------------------------------
        // Second Floor Bedroom: Traditional Japanese Futon Bed
        // ---------------------------------------------------------
        auto futonBase = std::make_shared<SceneNode>(name + "_FutonBase");
        futonBase->mesh = &meshes.cube;
        futonBase->transform.position = glm::vec3(0.0f, 4.38f, 1.20f);
        futonBase->transform.scale = glm::vec3(2.10f, 0.12f, 1.50f);
        futonBase->color = glm::vec4(0.95f, 0.94f, 0.90f, 1.0f);
        root->addChild(futonBase);

        auto futonQuilt = std::make_shared<SceneNode>(name + "_FutonQuilt");
        futonQuilt->mesh = &meshes.cube;
        futonQuilt->transform.position = glm::vec3(0.28f, 4.47f, 1.20f);
        futonQuilt->transform.scale = glm::vec3(1.40f, 0.16f, 1.54f);
        futonQuilt->color = glm::vec4(0.75f, 0.14f, 0.14f, 1.0f);
        root->addChild(futonQuilt);

        auto futonPillow = std::make_shared<SceneNode>(name + "_FutonPillow");
        futonPillow->mesh = &meshes.cylinder;
        futonPillow->transform.position = glm::vec3(-0.85f, 4.48f, 1.20f);
        futonPillow->transform.rotation.x = 90.0f;
        futonPillow->transform.scale = glm::vec3(0.18f, 0.55f, 0.18f);
        futonPillow->color = glm::vec4(0.14f, 0.18f, 0.38f, 1.0f);
        root->addChild(futonPillow);

        // Traditional 3-Panel Folding Screen (Byoubu 屏風 with Gold Accents)
        for (int p = 0; p < 3; ++p) {
            auto panel = std::make_shared<SceneNode>(name + "_ByoubuPanel_" + std::to_string(p));
            panel->mesh = &meshes.cube;
            float px = -1.30f + (float)p * 0.12f;
            float pz = 0.40f + (float)p * 0.75f;
            float rotY = (p % 2 == 0) ? -18.0f : 18.0f;
            panel->transform.position = glm::vec3(px, 5.15f, pz);
            panel->transform.rotation.y = rotY;
            panel->transform.scale = glm::vec3(0.04f, 1.65f, 0.72f);
            panel->color = glm::vec4(0.85f, 0.72f, 0.35f, 1.0f); // gold-leaf lacquer
            panel->shininess = 48.0f;
            panel->specularStrength = 0.70f;
            root->addChild(panel);
        }

        // Traditional Low Japanese Floor Study Desk (Tsukue / Bunjindukue)
        auto desk = std::make_shared<SceneNode>(name + "_StudyDesk");
        desk->mesh = &meshes.cube;
        desk->transform.position = glm::vec3(1.20f, 4.65f, -1.20f);
        desk->transform.scale = glm::vec3(1.10f, 0.08f, 0.70f);
        desk->color = tableWood;
        desk->shininess = 48.0f;
        desk->specularStrength = 0.55f;
        root->addChild(desk);

        // Desk Legs
        float dx[2] = { 0.75f, 1.65f };
        float dz[2] = { -1.45f, -0.95f };
        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                auto leg = std::make_shared<SceneNode>(name + "_DeskLeg_" + std::to_string(i) + "_" + std::to_string(j));
                leg->mesh = &meshes.cylinder;
                leg->transform.position = glm::vec3(dx[i], 4.48f, dz[j]);
                leg->transform.scale = glm::vec3(0.05f, 0.28f, 0.05f);
                leg->color = tableWood;
                root->addChild(leg);
            }
        }

        // Ceramic Inkstone (Suzuri) & Manuscript Scroll on Desk
        auto suzuri = std::make_shared<SceneNode>(name + "_Inkstone");
        suzuri->mesh = &meshes.cube;
        suzuri->transform.position = glm::vec3(1.05f, 4.72f, -1.20f);
        suzuri->transform.scale = glm::vec3(0.18f, 0.04f, 0.12f);
        suzuri->color = glm::vec4(0.12f, 0.12f, 0.14f, 1.0f);
        root->addChild(suzuri);

        auto scrollDoc = std::make_shared<SceneNode>(name + "_ManuscriptScroll");
        scrollDoc->mesh = &meshes.cylinder;
        scrollDoc->transform.position = glm::vec3(1.38f, 4.72f, -1.20f);
        scrollDoc->transform.rotation.z = 90.0f;
        scrollDoc->transform.scale = glm::vec3(0.04f, 0.35f, 0.04f);
        scrollDoc->color = glm::vec4(0.92f, 0.90f, 0.82f, 1.0f);
        root->addChild(scrollDoc);

        // Traditional Stepped Tansu Cabinet / Chest of Drawers
        auto tansuChest = std::make_shared<SceneNode>(name + "_TansuChest");
        tansuChest->mesh = &meshes.cube;
        tansuChest->transform.position = glm::vec3(-3.25f, 4.90f, 2.70f);
        tansuChest->transform.scale = glm::vec3(0.65f, 1.20f, 1.60f);
        tansuChest->color = darkWood;
        root->addChild(tansuChest);

        // Dedicated Corner Flower Stand (Kada) & Ikebana Vase on Second Floor
        auto flowerStand = std::make_shared<SceneNode>(name + "_F2FlowerStand");
        flowerStand->mesh = &meshes.cube;
        flowerStand->transform.position = glm::vec3(-3.25f, 4.60f, -0.20f);
        flowerStand->transform.scale = glm::vec3(0.45f, 0.60f, 0.45f);
        flowerStand->color = darkWood;
        root->addChild(flowerStand);

        auto f2Ikebana = createIkebanaVase(meshes, name + "_F2Ikebana", glm::vec3(-3.25f, 4.90f, -0.20f), 0.70f, 2);
        root->addChild(f2Ikebana);

        // REALISTIC SECOND FLOOR LAMPS:
        // 1. Bedside Paper Night Lamp (Andon)
        auto bedroomLamp = createAndonFloorLamp(meshes, name + "_F2BedsideAndon", glm::vec3(-0.85f, 4.30f, 2.30f), 0.75f);
        root->addChild(bedroomLamp);

        // 2. Second Floor Ceiling Pendant Chandelier Lantern
        auto pendantF2 = createCeilingPendantLamp(meshes, name + "_F2Pendant", glm::vec3(0.0f, 7.80f, 0.50f), 0.75f);
        root->addChild(pendantF2);

        // =========================================================
        // 6. UPPER MAIN PITCHED ROOF: GABLE ROOF
        // =========================================================
        auto roofLeft = std::make_shared<SceneNode>(name + "_RoofLeft");
        roofLeft->mesh = &meshes.cube;
        roofLeft->transform.position = glm::vec3(-2.0f, 8.6f, 0.0f);
        roofLeft->transform.rotation.z = 22.0f;
        roofLeft->transform.scale = glm::vec3(4.8f, 0.35f, 9.6f);
        roofLeft->color = roofSlate;
        root->addChild(roofLeft);

        auto roofRight = std::make_shared<SceneNode>(name + "_RoofRight");
        roofRight->mesh = &meshes.cube;
        roofRight->transform.position = glm::vec3(2.0f, 8.6f, 0.0f);
        roofRight->transform.rotation.z = -22.0f;
        roofRight->transform.scale = glm::vec3(4.8f, 0.35f, 9.6f);
        roofRight->color = roofSlate;
        root->addChild(roofRight);

        auto ridge = std::make_shared<SceneNode>(name + "_RoofRidge");
        ridge->mesh = &meshes.cube;
        ridge->transform.position = glm::vec3(0.0f, 9.45f, 0.0f);
        ridge->transform.scale = glm::vec3(0.65f, 0.38f, 9.8f);
        ridge->color = glm::vec4(0.11f, 0.11f, 0.13f, 1.0f);
        root->addChild(ridge);

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

    void toggleDoor()
    {
        isDoorOpen = !isDoorOpen;
    }

    void setDoorOpen(bool open)
    {
        isDoorOpen = open;
    }

    void toggleWindows()
    {
        isWindowOpen = !isWindowOpen;
    }

    void setWindowsOpen(bool open)
    {
        isWindowOpen = open;
    }

    void update(float dt)
    {
        // Smoothly animate the sliding Shoji front door along its track
        doorSlideProgress = glm::mix(doorSlideProgress, isDoorOpen ? 1.0f : 0.0f, glm::clamp(dt * 5.0f, 0.0f, 1.0f));
        if (slidingDoorGroup)
        {
            slidingDoorGroup->transform.position.z = doorSlideProgress * 1.35f;
        }

        // Smoothly animate all sliding window sashes
        windowSlideProgress = glm::mix(windowSlideProgress, isWindowOpen ? 1.0f : 0.0f, glm::clamp(dt * 4.5f, 0.0f, 1.0f));
        for (auto& sash : slidingWindowSashes)
        {
            if (sash.node)
            {
                sash.node->transform.position = sash.basePos + sash.slideDelta * windowSlideProgress;
            }
        }
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

        // Sub-top curved crossbeam (Shimaki) with authentic Japanese upward sori
        auto shimaki = std::make_shared<SceneNode>("Torii_Shimaki");
        shimaki->mesh = &meshes.curvedShimaki;
        shimaki->transform.position = glm::vec3(0.0f, 10.1f, 0.0f);
        shimaki->color = vermilion;
        root->addChild(shimaki);

        // Upper main curved crossbeam (Kasagi) with dramatic upward sori and beveled roof cap
        auto kasagi = std::make_shared<SceneNode>("Torii_Kasagi");
        kasagi->mesh = &meshes.curvedKasagi;
        kasagi->transform.position = glm::vec3(0.0f, 10.65f, 0.0f);
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

    // Organic 3D Curved Geometry Meshes (Swept Spline, Bishop frames, Bézier branches)
    std::shared_ptr<Mesh> trunkMesh;
    std::vector<std::shared_ptr<Mesh>> rootMeshes;
    std::vector<std::shared_ptr<Mesh>> boughMeshes;
    std::vector<std::shared_ptr<Mesh>> branchMeshes;

    SakuraTree(SceneMeshes& meshes, const glm::vec3& pos, float scale = 1.0f, float rotY = 0.0f, float leanAngle = 0.0f, float blossomTone = 0.0f)
    {
        root = std::make_shared<SceneNode>("Sakura_Tree");
        root->transform.position = pos;
        root->transform.rotation.y = rotY;
        root->transform.rotation.z = leanAngle;
        root->transform.scale = glm::vec3(scale);

        glm::vec4 bark(0.32f, 0.20f, 0.14f, 1.0f);
        glm::vec4 barkDark(0.24f, 0.15f, 0.10f, 1.0f);

        // 1. Organic Buttress Root Spurs anchoring trunk into ground along 3D Bézier curves
        float rootAngles[5] = { 15.0f, 85.0f, 160.0f, 230.0f, 305.0f };
        float rootDist[5]   = { 2.2f,  2.0f,  2.4f,   1.9f,   2.3f };
        for (int r = 0; r < 5; ++r) {
            float rad = glm::radians(rootAngles[r]);
            float cosR = std::cos(rad);
            float sinR = std::sin(rad);
            float len = rootDist[r];

            glm::vec3 p0(cosR * 0.55f, 0.45f, sinR * 0.55f);
            glm::vec3 p1(cosR * 1.10f, 0.22f, sinR * 1.10f);
            glm::vec3 p2(cosR * 1.65f, 0.08f, sinR * 1.65f);
            glm::vec3 p3(cosR * len,  -0.05f, sinR * len);

            auto rMesh = std::make_shared<Mesh>(Curves::createBezierTube(
                Curves::Bezier3(p0, p1, p2, p3), 0.32f, 0.10f, 16, 12, 1.0f, true, true));
            rootMeshes.push_back(rMesh);

            auto rootSpur = std::make_shared<SceneNode>("Sakura_Root_" + std::to_string(r));
            rootSpur->mesh = rMesh.get();
            rootSpur->color = barkDark;
            root->addChild(rootSpur);
        }

        // 2. Organic Continuous Swept 3D Spline Trunk (natural flaring base, gnarled twists, tapers to crown)
        std::vector<glm::vec3> trunkSpline = {
            {  0.00f, -0.25f,  0.00f }, // root flare base in earth
            {  0.00f,  0.00f,  0.00f }, // ground level
            { -0.16f,  1.50f,  0.10f }, // gentle forward-left organic sweep
            { -0.18f,  3.10f, -0.06f }, // counter-curve backward-left
            {  0.06f,  4.50f,  0.04f }, // gentle return towards center
            {  0.00f,  5.50f,  0.00f }  // crown fork junction
        };
        trunkMesh = std::make_shared<Mesh>(Curves::createSplineTube(trunkSpline, 0.76f, 0.42f, 32, 18, 3.0f, true, true));
        auto trunk = std::make_shared<SceneNode>("Sakura_Trunk_Curved");
        trunk->mesh = trunkMesh.get();
        trunk->color = bark;
        root->addChild(trunk);

        // 3. Five Primary Spreading Scaffold Boughs (swept 3D Bézier curves arching into canopy)
        struct BoughCurve { glm::vec3 p0, p1, p2, p3; float rStart, rEnd; };
        BoughCurve boughCurves[5] = {
            // Northwest bough
            { { -0.05f, 5.35f,  0.05f }, { -0.50f, 5.60f,  0.40f }, { -1.10f, 5.90f,  0.80f }, { -1.75f, 6.20f,  1.20f }, 0.28f, 0.14f },
            // Northeast bough
            { {  0.05f, 5.30f, -0.05f }, {  0.50f, 5.55f, -0.40f }, {  1.10f, 5.85f, -0.85f }, {  1.75f, 6.10f, -1.30f }, 0.28f, 0.14f },
            // Southwest bough
            { { -0.05f, 5.40f, -0.05f }, { -0.45f, 5.70f, -0.45f }, { -0.95f, 6.05f, -0.85f }, { -1.50f, 6.40f, -1.25f }, 0.26f, 0.13f },
            // Southeast bough
            { {  0.05f, 5.35f,  0.05f }, {  0.45f, 5.65f,  0.45f }, {  0.95f, 6.00f,  0.85f }, {  1.55f, 6.30f,  1.15f }, 0.26f, 0.13f },
            // Central crown bough
            { {  0.00f, 5.45f,  0.00f }, {  0.05f, 6.00f,  0.05f }, { -0.05f, 6.60f, -0.05f }, {  0.05f, 7.20f,  0.00f }, 0.30f, 0.15f }
        };
        for (int b = 0; b < 5; ++b) {
            auto bMesh = std::make_shared<Mesh>(Curves::createBezierTube(
                Curves::Bezier3(boughCurves[b].p0, boughCurves[b].p1, boughCurves[b].p2, boughCurves[b].p3),
                boughCurves[b].rStart, boughCurves[b].rEnd, 20, 14, 1.5f, true, true));
            boughMeshes.push_back(bMesh);

            auto bough = std::make_shared<SceneNode>("Sakura_Bough_" + std::to_string(b));
            bough->mesh = bMesh.get();
            bough->color = bark;
            root->addChild(bough);
        }

        // 4. Secondary Curved Branches Forking Towards Blossom Clouds
        struct BranchCurve { glm::vec3 p0, p1, p2, p3; float rStart, rEnd; };
        BranchCurve branchCurves[8] = {
            { { -1.75f, 6.20f,  1.20f }, { -2.20f, 6.50f,  1.40f }, { -2.60f, 6.80f,  1.50f }, { -3.00f, 7.10f,  1.60f }, 0.14f, 0.08f },
            { { -1.50f, 6.40f, -1.25f }, { -1.90f, 6.70f, -1.50f }, { -2.30f, 7.00f, -1.70f }, { -2.60f, 7.30f, -1.80f }, 0.13f, 0.08f },
            { {  1.75f, 6.10f, -1.30f }, {  2.15f, 6.45f, -1.50f }, {  2.55f, 6.75f, -1.70f }, {  2.90f, 7.10f, -1.80f }, 0.14f, 0.08f },
            { {  1.55f, 6.30f,  1.15f }, {  1.95f, 6.65f,  1.35f }, {  2.35f, 6.95f,  1.45f }, {  2.70f, 7.25f,  1.55f }, 0.13f, 0.08f },
            { {  0.05f, 7.20f,  0.00f }, {  0.15f, 7.60f,  0.40f }, {  0.18f, 8.00f,  0.80f }, {  0.20f, 8.40f,  1.20f }, 0.15f, 0.08f },
            { {  0.05f, 7.20f,  0.00f }, { -0.15f, 7.60f, -0.40f }, { -0.20f, 8.00f, -0.80f }, { -0.25f, 8.40f, -1.20f }, 0.15f, 0.08f },
            { { -0.80f, 6.80f,  0.10f }, { -1.10f, 7.20f,  0.15f }, { -1.30f, 7.60f,  0.20f }, { -1.50f, 8.00f,  0.25f }, 0.12f, 0.07f },
            { {  0.80f, 6.70f, -0.10f }, {  1.10f, 7.10f, -0.15f }, {  1.30f, 7.50f, -0.20f }, {  1.50f, 7.90f, -0.25f }, 0.12f, 0.07f }
        };
        for (int br = 0; br < 8; ++br) {
            auto brMesh = std::make_shared<Mesh>(Curves::createBezierTube(
                Curves::Bezier3(branchCurves[br].p0, branchCurves[br].p1, branchCurves[br].p2, branchCurves[br].p3),
                branchCurves[br].rStart, branchCurves[br].rEnd, 16, 12, 1.0f, true, true));
            branchMeshes.push_back(brMesh);

            auto branch = std::make_shared<SceneNode>("Sakura_Branch_" + std::to_string(br));
            branch->mesh = brMesh.get();
            branch->color = bark;
            root->addChild(branch);
        }

        // 5. Volumetric Multi-Tiered Cherry Blossom Canopy (22 Sculpted Multi-Lobed Billow Clusters)
        glm::vec4 deepPink(0.92f + blossomTone * 0.04f, 0.48f - blossomTone * 0.10f, 0.66f + blossomTone * 0.12f, 1.0f);
        glm::vec4 midPink(0.98f + blossomTone * 0.02f, 0.70f - blossomTone * 0.08f, 0.82f + blossomTone * 0.10f, 1.0f);
        glm::vec4 lightPink(1.00f, 0.86f - blossomTone * 0.05f, 0.90f + blossomTone * 0.06f, 1.0f);
        glm::vec4 whitePink(1.00f, 0.93f - blossomTone * 0.03f, 0.96f + blossomTone * 0.03f, 1.0f);

        struct ClusterDef { glm::vec3 offset; glm::vec3 scl; glm::vec4 col; };
        std::vector<ClusterDef> clusters = {
            // Core central canopy
            { {  0.0f, 6.8f,  0.0f },  { 4.4f, 3.2f, 4.4f }, deepPink },
            { {  0.0f, 8.4f,  0.0f },  { 3.6f, 2.6f, 3.6f }, lightPink },
            { {  0.0f, 9.6f,  0.0f },  { 2.6f, 1.9f, 2.6f }, whitePink },
            // North / Northwest lobes
            { { -2.2f, 6.2f,  1.4f },  { 3.4f, 2.6f, 3.2f }, midPink },
            { { -3.2f, 6.6f,  1.8f },  { 2.8f, 2.2f, 2.8f }, lightPink },
            { { -1.8f, 7.8f,  1.6f },  { 2.9f, 2.2f, 2.9f }, midPink },
            // Northeast lobes
            { {  2.1f, 6.1f,  1.3f },  { 3.2f, 2.5f, 3.1f }, midPink },
            { {  3.0f, 6.4f,  1.7f },  { 2.6f, 2.0f, 2.6f }, whitePink },
            { {  1.7f, 7.6f,  1.5f },  { 2.8f, 2.1f, 2.8f }, lightPink },
            // South / Southwest lobes
            { { -1.9f, 6.0f, -1.8f },  { 3.3f, 2.5f, 3.2f }, deepPink },
            { { -2.8f, 6.3f, -2.4f },  { 2.7f, 2.0f, 2.7f }, midPink },
            { { -1.5f, 7.5f, -1.9f },  { 2.8f, 2.2f, 2.8f }, lightPink },
            // Southeast lobes
            { {  2.2f, 6.0f, -1.7f },  { 3.3f, 2.5f, 3.3f }, midPink },
            { {  3.1f, 6.3f, -2.2f },  { 2.7f, 2.0f, 2.7f }, lightPink },
            { {  1.8f, 7.5f, -1.6f },  { 2.8f, 2.1f, 2.8f }, whitePink },
            // Upper dome crown
            { {  0.9f, 8.8f,  0.8f },  { 2.5f, 1.8f, 2.5f }, whitePink },
            { { -0.8f, 8.7f, -0.7f },  { 2.5f, 1.8f, 2.5f }, lightPink },
            { { -0.9f, 8.8f,  0.7f },  { 2.4f, 1.8f, 2.4f }, whitePink },
            { {  0.8f, 8.7f, -0.8f },  { 2.4f, 1.8f, 2.4f }, lightPink },
            // Weeping lower hanging blossom sprays
            { { -2.4f, 5.0f,  1.5f },  { 1.8f, 1.4f, 1.8f }, midPink },
            { {  2.3f, 4.9f, -1.6f },  { 1.8f, 1.4f, 1.8f }, midPink },
            { {  0.0f, 5.2f,  2.4f },  { 1.9f, 1.5f, 1.9f }, deepPink }
        };

        for (size_t i = 0; i < clusters.size(); ++i)
        {
            auto cluster = std::make_shared<SceneNode>("BlossomCluster_" + std::to_string(i));
            cluster->mesh = &meshes.sakuraCanopyLobe;
            cluster->transform.position = clusters[i].offset;
            cluster->transform.scale = clusters[i].scl;
            cluster->color = clusters[i].col;
            root->addChild(cluster);
        }

        // 5b. Botanical curved cherry leaves (fresh young spring foliage sprigs)
        struct LeafSprigDef { glm::vec3 offset; glm::vec3 rot; float scl; };
        std::vector<LeafSprigDef> leafSprigs = {
            { { -2.8f, 6.0f,  1.6f }, { 25.0f,  45.0f, -15.0f }, 1.3f },
            { { -1.6f, 6.4f,  1.8f }, { -20.0f, 120.0f, 20.0f }, 1.2f },
            { {  2.6f, 5.9f, -1.5f }, { 15.0f, -60.0f, -25.0f }, 1.4f },
            { {  1.8f, 6.3f, -1.8f }, { -15.0f, 150.0f, 15.0f }, 1.2f },
            { { -2.4f, 4.8f,  1.4f }, { 35.0f,  20.0f, -30.0f }, 1.5f },
            { {  2.2f, 4.7f, -1.5f }, { 30.0f, -40.0f,  25.0f }, 1.5f },
            { {  0.1f, 5.0f,  2.2f }, { 25.0f,  90.0f, -10.0f }, 1.4f },
            { { -1.3f, 7.8f,  0.2f }, { -10.0f, -30.0f, 20.0f }, 1.3f },
            { {  1.4f, 7.7f, -0.2f }, { 15.0f, 160.0f, -20.0f }, 1.3f }
        };
        for (size_t l = 0; l < leafSprigs.size(); ++l) {
            auto sprig = std::make_shared<SceneNode>("Sakura_LeafSprig_" + std::to_string(l));
            sprig->mesh = &meshes.curvedLeaf;
            sprig->transform.position = leafSprigs[l].offset;
            sprig->transform.rotation = leafSprigs[l].rot;
            sprig->transform.scale = glm::vec3(leafSprigs[l].scl);
            sprig->color = glm::vec4(0.28f, 0.58f, 0.22f, 1.0f); // vibrant spring leaf green
            root->addChild(sprig);
        }

        // 6. Fallen Blossom Petal Patches on Street Cobblestones
        for (int p = 0; p < 7; ++p)
        {
            float pAngle = (float)p * 51.4f;
            float pRad = glm::radians(pAngle);
            float pDist = 1.4f + (float)(p % 3) * 0.9f;
            auto patch = std::make_shared<SceneNode>("GroundPetals_" + std::to_string(p));
            patch->mesh = &meshes.cylinder;
            patch->transform.position = glm::vec3(std::cos(pRad) * pDist, 0.03f, std::sin(pRad) * pDist);
            patch->transform.scale = glm::vec3(0.55f + (p % 2) * 0.25f, 0.02f, 0.55f + (p % 2) * 0.25f);
            patch->color = glm::vec4(0.96f, 0.68f, 0.78f, 0.85f);
            root->addChild(patch);
        }

        // 7. Dynamic Cascading Petal Particle System (32 Falling Curved Petals)
        petals.reserve(32);
        for (int i = 0; i < 32; ++i)
        {
            auto petal = std::make_shared<SceneNode>("Petal_" + std::to_string(i));
            petal->mesh = &meshes.curvedPetal;
            petal->transform.scale = glm::vec3(1.2f);
            petal->color = (i % 2 == 0) ? glm::vec4(0.99f, 0.70f, 0.82f, 1.0f) : glm::vec4(1.0f, 0.85f, 0.90f, 1.0f);

            FallingPetal fp;
            fp.node = petal;
            fp.basePos = glm::vec3(((rand() % 100) / 100.0f - 0.5f) * 6.5f,
                                   4.5f + (rand() % 55) / 10.0f,
                                   ((rand() % 100) / 100.0f - 0.5f) * 6.5f);
            fp.speed = 0.65f + (rand() % 60) / 100.0f;
            fp.phase = (float)i * 0.45f;
            fp.swayAmp = 0.30f + (rand() % 40) / 100.0f;

            petal->transform.position = fp.basePos;
            root->addChild(petal);
            petals.push_back(fp);
        }
    }

    void update(float time, float dt)
    {
        for (auto& fp : petals)
        {
            fp.basePos.y -= fp.speed * dt;
            if (fp.basePos.y < 0.08f)
            {
                fp.basePos.y = 8.5f + (rand() % 25) / 10.0f;
                fp.basePos.x = ((rand() % 100) / 100.0f - 0.5f) * 6.5f;
                fp.basePos.z = ((rand() % 100) / 100.0f - 0.5f) * 6.5f;
            }

            float xOffset = std::sin(time * 1.8f + fp.phase) * fp.swayAmp;
            float zOffset = std::cos(time * 1.4f + fp.phase) * fp.swayAmp;
            fp.node->transform.position = glm::vec3(fp.basePos.x + xOffset, fp.basePos.y, fp.basePos.z + zOffset);
            fp.node->transform.rotation = glm::vec3(time * 50.0f + fp.phase * 30.0f, time * 65.0f, time * 35.0f);
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

        // 2. Realistic Continuous Swept Catenary Rope connecting left and right poles
        // Span: X in [-3.8, +3.8]. Continuous 3D swept mesh along mathematical curve
        const float yPole = 6.20f;
        const float sag = 0.65f;
        const float halfSpan = 3.8f;

        auto catenaryY = [&](float x) -> float {
            float normX = x / halfSpan;
            return yPole - sag * (1.0f - normX * normX);
        };

        auto rope = std::make_shared<SceneNode>(prefix + "_Rope");
        rope->mesh = &meshes.catenaryRope;
        rope->color = ropeColor;
        root->addChild(rope);

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
                ballNode->color = glm::vec4(0.92f, 0.72f, 0.38f, 1.0f); // golden browned crispy batter
                ballNode->shininess = 64.0f;
                ballNode->specularStrength = 0.65f;

                // Rich Glossy Dark Savory Takoyaki Sauce Glaze (Otafuku Sauce)
                auto sauce = std::make_shared<SceneNode>("TakoSauce_" + std::to_string(i) + "_" + std::to_string(j));
                sauce->mesh = &meshes.sphere;
                sauce->transform.position = glm::vec3(0.0f, 0.06f, 0.0f);
                sauce->transform.scale = glm::vec3(0.96f, 0.46f, 0.96f);
                sauce->color = glm::vec4(0.18f, 0.08f, 0.03f, 1.0f);
                sauce->shininess = 72.0f;
                sauce->specularStrength = 0.85f;
                ballNode->addChild(sauce);

                // Creamy Kewpie Mayonnaise Zig-Zag Drizzle
                auto mayo = std::make_shared<SceneNode>("TakoMayo_" + std::to_string(i) + "_" + std::to_string(j));
                mayo->mesh = &meshes.cylinder;
                mayo->transform.position = glm::vec3(0.0f, 0.12f, 0.0f);
                mayo->transform.rotation.z = 90.0f;
                mayo->transform.scale = glm::vec3(0.05f, 0.72f, 0.05f);
                mayo->color = glm::vec4(0.96f, 0.94f, 0.84f, 1.0f);
                ballNode->addChild(mayo);

                // Emerald Green Aonori (crushed dried seaweed flakes)
                auto aonori = std::make_shared<SceneNode>("TakoAonori_" + std::to_string(i) + "_" + std::to_string(j));
                aonori->mesh = &meshes.cube;
                aonori->transform.position = glm::vec3(0.05f, 0.14f, 0.04f);
                aonori->transform.scale = glm::vec3(0.18f, 0.04f, 0.18f);
                aonori->color = glm::vec4(0.10f, 0.50f, 0.16f, 1.0f);
                ballNode->addChild(aonori);

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
    std::shared_ptr<SceneNode> hand;

    VendorFigure(SceneMeshes& meshes, const std::string& name, const glm::vec3& pos, float rotY, const glm::vec4& robeColor)
    {
        root = std::make_shared<SceneNode>(name);
        root->transform.position = pos;
        root->transform.rotation.y = rotY;

        glm::vec4 skin(0.92f, 0.76f, 0.64f, 1.0f);
        glm::vec4 wood(0.35f, 0.24f, 0.15f, 1.0f);
        glm::vec4 darkPants(0.18f, 0.18f, 0.22f, 1.0f);
        glm::vec4 whiteBand(0.95f, 0.95f, 0.95f, 1.0f);

        // Raised wooden platform behind counter so the vendor stands tall and visible
        auto platform = std::make_shared<SceneNode>(name + "_Platform");
        platform->mesh = &meshes.cube;
        platform->transform.position = glm::vec3(0.0f, 0.10f, 0.0f);
        platform->transform.scale = glm::vec3(2.2f, 0.20f, 1.2f);
        platform->color = wood;
        root->addChild(platform);

        // Hierarchical Articulated Legs standing on platform (Platform top is at Y = 0.20)
        float hipX[2] = { -0.16f, 0.16f };
        for (int l = 0; l < 2; ++l)
        {
            std::string side = (l == 0) ? "L" : "R";
            auto thigh = std::make_shared<SceneNode>(name + "_Thigh" + side);
            thigh->mesh = &meshes.limbThigh;
            thigh->transform.position = glm::vec3(hipX[l], 1.04f, 0.0f);
            thigh->color = darkPants;
            root->addChild(thigh);

            auto shin = std::make_shared<SceneNode>(name + "_Shin" + side);
            shin->mesh = &meshes.limbShin;
            shin->transform.position = glm::vec3(0.0f, -0.42f, 0.0f);
            shin->color = darkPants;
            thigh->addChild(shin);

            auto foot = std::make_shared<SceneNode>(name + "_Geta" + side);
            foot->mesh = &meshes.getaFoot;
            foot->transform.position = glm::vec3(0.0f, -0.42f, 0.04f);
            foot->color = wood;
            shin->addChild(foot);
        }

        // Anatomically Contoured Human Torso (Wearing festive Happi coat with Eri collar)
        auto torso = std::make_shared<SceneNode>(name + "_Torso");
        torso->mesh = &meshes.humanTorso;
        torso->transform.position = glm::vec3(0.0f, 1.48f, 0.0f);
        torso->transform.scale = glm::vec3(1.15f, 1.05f, 1.15f);
        torso->color = robeColor;
        root->addChild(torso);

        // Obi sash belt
        auto obi = std::make_shared<SceneNode>(name + "_Obi");
        obi->mesh = &meshes.cube;
        obi->transform.position = glm::vec3(0.0f, 1.30f, 0.0f);
        obi->transform.scale = glm::vec3(0.68f, 0.20f, 0.48f);
        obi->color = glm::vec4(0.88f, 0.82f, 0.35f, 1.0f);
        root->addChild(obi);

        // Sculpted Anatomical Human Head & Face
        auto head = std::make_shared<SceneNode>(name + "_Head");
        head->mesh = &meshes.humanHead;
        head->transform.position = glm::vec3(0.0f, 2.05f, 0.0f);
        head->transform.scale = glm::vec3(1.18f);
        head->color = skin;
        root->addChild(head);

        // Headband (Hachimaki) knotted across the forehead
        auto band = std::make_shared<SceneNode>(name + "_Hachimaki");
        band->mesh = &meshes.cube;
        band->transform.position = glm::vec3(0.0f, 2.12f, 0.02f);
        band->transform.scale = glm::vec3(0.48f, 0.08f, 0.46f);
        band->color = whiteBand;
        root->addChild(band);

        // Left Arm: Shoulder -> UpperArm -> Elbow -> Forearm -> Hand (resting naturally)
        auto leftArm = std::make_shared<SceneNode>(name + "_LeftArm");
        leftArm->mesh = &meshes.limbUpperArm;
        leftArm->transform.position = glm::vec3(-0.36f, 1.82f, 0.0f);
        leftArm->transform.rotation.z = -18.0f;
        leftArm->transform.rotation.x = 10.0f;
        leftArm->color = robeColor;
        root->addChild(leftArm);

        auto leftForearm = std::make_shared<SceneNode>(name + "_LeftForearm");
        leftForearm->mesh = &meshes.limbForearm;
        leftForearm->transform.position = glm::vec3(0.0f, -0.38f, 0.0f);
        leftForearm->transform.rotation.x = -25.0f;
        leftForearm->color = skin;
        leftArm->addChild(leftForearm);

        auto leftHand = std::make_shared<SceneNode>(name + "_LeftHand");
        leftHand->mesh = &meshes.humanHand;
        leftHand->transform.position = glm::vec3(0.0f, -0.36f, 0.0f);
        leftHand->transform.rotation.z = 90.0f;
        leftHand->color = skin;
        leftForearm->addChild(leftHand);

        // Right Arm (Hierarchical Shoulder -> UpperArm -> Elbow -> Forearm -> Hand -> Utensil)
        upperArm = std::make_shared<SceneNode>(name + "_UpperArm");
        upperArm->transform.position = glm::vec3(0.36f, 1.82f, 0.05f);
        root->addChild(upperArm);

        auto upperMesh = std::make_shared<SceneNode>(name + "_UpperMesh");
        upperMesh->mesh = &meshes.limbUpperArm;
        upperMesh->color = robeColor;
        upperArm->addChild(upperMesh);

        // Forearm node (child of upper arm at elbow condyle)
        foreArm = std::make_shared<SceneNode>(name + "_ForeArm");
        foreArm->transform.position = glm::vec3(0.0f, -0.38f, 0.0f);
        upperArm->addChild(foreArm);

        auto foreMesh = std::make_shared<SceneNode>(name + "_ForeMesh");
        foreMesh->mesh = &meshes.limbForearm;
        foreMesh->color = skin;
        foreArm->addChild(foreMesh);

        // Anatomical Hand
        hand = std::make_shared<SceneNode>(name + "_Hand");
        hand->mesh = &meshes.humanHand;
        hand->transform.position = glm::vec3(0.0f, -0.36f, 0.0f);
        hand->transform.rotation.x = 45.0f;
        hand->color = skin;
        foreArm->addChild(hand);

        // Cooking turner utensil held firmly in hand
        auto utensil = std::make_shared<SceneNode>(name + "_Utensil");
        utensil->mesh = &meshes.cylinder;
        utensil->transform.position = glm::vec3(0.0f, -0.06f, 0.18f);
        utensil->transform.rotation.x = -75.0f;
        utensil->transform.scale = glm::vec3(0.03f, 0.45f, 0.03f);
        utensil->color = glm::vec4(0.75f, 0.75f, 0.78f, 1.0f); // steel pick
        hand->addChild(utensil);
    }

    void update(float time)
    {
        // Stirring / cooking arm joint kinematics reaching over counter
        upperArm->transform.rotation.x = 22.0f + std::sin(time * 4.0f) * 16.0f;
        upperArm->transform.rotation.y = std::cos(time * 4.0f) * 14.0f;
        foreArm->transform.rotation.x = 35.0f + std::sin(time * 4.0f + 0.5f) * 18.0f;
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

        // Hierarchical Articulated Legs standing firmly on stage (Y = 0.0 is stage floor)
        float hipX[2] = { -0.16f, 0.16f };
        for (int l = 0; l < 2; ++l)
        {
            std::string side = (l == 0) ? "L" : "R";
            auto thigh = std::make_shared<SceneNode>("Magician_Thigh" + side);
            thigh->mesh = &meshes.limbThigh;
            thigh->transform.position = glm::vec3(hipX[l], 0.84f, 0.0f);
            thigh->color = darkPants;
            root->addChild(thigh);

            auto shin = std::make_shared<SceneNode>("Magician_Shin" + side);
            shin->mesh = &meshes.limbShin;
            shin->transform.position = glm::vec3(0.0f, -0.42f, 0.0f);
            shin->color = darkPants;
            thigh->addChild(shin);

            // Formal magician boots resting flush on the stage surface (Y in [0.0, 0.10])
            auto boot = std::make_shared<SceneNode>("Magician_Boot" + side);
            boot->mesh = &meshes.cube;
            boot->transform.position = glm::vec3(0.0f, -0.38f, 0.05f);
            boot->transform.scale = glm::vec3(0.18f, 0.10f, 0.28f);
            boot->color = bootBlack;
            shin->addChild(boot);
        }

        // Cape tail draping behind magician
        auto capeTail = std::make_shared<SceneNode>("Magician_CapeTail");
        capeTail->mesh = &meshes.cube;
        capeTail->transform.position = glm::vec3(0.0f, 0.75f, -0.22f);
        capeTail->transform.rotation.x = 4.0f;
        capeTail->transform.scale = glm::vec3(0.82f, 1.35f, 0.06f);
        capeTail->color = robe;
        root->addChild(capeTail);

        // Body robe (contoured human torso with kimono collar Eri)
        auto torso = std::make_shared<SceneNode>("Magician_Torso");
        torso->mesh = &meshes.humanTorso;
        torso->transform.position = glm::vec3(0.0f, 1.28f, 0.0f);
        torso->transform.scale = glm::vec3(1.22f, 1.05f, 1.22f);
        torso->color = robe;
        root->addChild(torso);

        // Head (sculpted human head with facial features)
        auto head = std::make_shared<SceneNode>("Magician_Head");
        head->mesh = &meshes.humanHead;
        head->transform.position = glm::vec3(0.0f, 1.88f, 0.0f);
        head->transform.scale = glm::vec3(1.20f);
        head->color = skin;
        root->addChild(head);

        // Top Hat / Wizard hat
        auto hatBrim = std::make_shared<SceneNode>("Hat_Brim");
        hatBrim->mesh = &meshes.cylinder;
        hatBrim->transform.position = glm::vec3(0.0f, 2.15f, 0.0f);
        hatBrim->transform.scale = glm::vec3(1.0f, 0.06f, 1.0f);
        hatBrim->color = robe;
        root->addChild(hatBrim);

        auto hatCone = std::make_shared<SceneNode>("Hat_Cone");
        hatCone->mesh = &meshes.cone;
        hatCone->transform.position = glm::vec3(0.0f, 2.70f, 0.0f);
        hatCone->transform.scale = glm::vec3(0.65f, 1.1f, 0.65f);
        hatCone->color = robe;
        root->addChild(hatCone);

        // Left Arm: Shoulder -> UpperArm -> Elbow -> Forearm -> Hand (bent gracefully at side / hip)
        auto leftArm = std::make_shared<SceneNode>("Magician_LeftArm");
        leftArm->mesh = &meshes.limbUpperArm;
        leftArm->transform.position = glm::vec3(-0.36f, 1.62f, 0.05f);
        leftArm->transform.rotation.z = 22.0f;
        leftArm->transform.rotation.x = -15.0f;
        leftArm->color = robe;
        root->addChild(leftArm);

        auto leftForearm = std::make_shared<SceneNode>("Magician_LeftForearm");
        leftForearm->mesh = &meshes.limbForearm;
        leftForearm->transform.position = glm::vec3(0.0f, -0.38f, 0.0f);
        leftForearm->transform.rotation.x = 35.0f;
        leftForearm->color = skin;
        leftArm->addChild(leftForearm);

        auto leftHand = std::make_shared<SceneNode>("Magician_LeftHand");
        leftHand->mesh = &meshes.humanHand;
        leftHand->transform.position = glm::vec3(0.0f, -0.36f, 0.0f);
        leftHand->transform.rotation.z = 90.0f;
        leftHand->color = skin;
        leftForearm->addChild(leftHand);

        // Raised Right Arm commanding the magic orb
        auto rightArm = std::make_shared<SceneNode>("Magician_RightArm");
        rightArm->mesh = &meshes.limbUpperArm;
        rightArm->transform.position = glm::vec3(0.36f, 1.62f, 0.05f);
        rightArm->transform.rotation = glm::vec3(-45.0f, 20.0f, -25.0f);
        rightArm->color = robe;
        root->addChild(rightArm);

        auto rightForearm = std::make_shared<SceneNode>("Magician_RightForearm");
        rightForearm->mesh = &meshes.limbForearm;
        rightForearm->transform.position = glm::vec3(0.0f, -0.38f, 0.0f);
        rightForearm->transform.rotation.x = 25.0f;
        rightForearm->color = skin;
        rightArm->addChild(rightForearm);

        // Right Hand Node (serves as the moving reference frame for the orb!)
        rightHand = std::make_shared<SceneNode>("Magician_RightHand");
        rightHand->transform.position = glm::vec3(0.9f, 2.3f, 0.5f);
        root->addChild(rightHand);

        // Hand palm contoured mesh
        auto palm = std::make_shared<SceneNode>("Magician_Palm");
        palm->mesh = &meshes.humanHand;
        palm->transform.rotation.x = -30.0f;
        palm->color = skin;
        rightHand->addChild(palm);

        // Magic Wand
        auto wand = std::make_shared<SceneNode>("Magic_Wand");
        wand->mesh = &meshes.cylinder;
        wand->transform.position = glm::vec3(0.04f, 0.12f, 0.10f);
        wand->transform.rotation.x = -50.0f;
        wand->transform.scale = glm::vec3(0.035f, 0.65f, 0.035f);
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

                // Seated lap / lower body (contoured thighs resting horizontally on bench felt)
                auto thighL = std::make_shared<SceneNode>("Aud_ThighL_" + std::to_string(personIndex));
                thighL->mesh = &meshes.limbThigh;
                thighL->transform.position = glm::vec3(-0.16f, 0.55f, 0.05f);
                thighL->transform.rotation.x = -85.0f; // horizontal resting forward on bench
                thighL->color = yukata;
                person->addChild(thighL);

                auto shinL = std::make_shared<SceneNode>("Aud_ShinL_" + std::to_string(personIndex));
                shinL->mesh = &meshes.limbShin;
                shinL->transform.position = glm::vec3(0.0f, -0.40f, 0.0f);
                shinL->transform.rotation.x = 85.0f; // bends down to ground
                shinL->color = skin;
                thighL->addChild(shinL);

                auto getaL = std::make_shared<SceneNode>("Aud_GetaL_" + std::to_string(personIndex));
                getaL->mesh = &meshes.getaFoot;
                getaL->transform.position = glm::vec3(0.0f, -0.40f, 0.05f);
                getaL->color = benchWood;
                shinL->addChild(getaL);

                auto thighR = std::make_shared<SceneNode>("Aud_ThighR_" + std::to_string(personIndex));
                thighR->mesh = &meshes.limbThigh;
                thighR->transform.position = glm::vec3(0.16f, 0.55f, 0.05f);
                thighR->transform.rotation.x = -85.0f;
                thighR->color = yukata;
                person->addChild(thighR);

                auto shinR = std::make_shared<SceneNode>("Aud_ShinR_" + std::to_string(personIndex));
                shinR->mesh = &meshes.limbShin;
                shinR->transform.position = glm::vec3(0.0f, -0.40f, 0.0f);
                shinR->transform.rotation.x = 85.0f;
                shinR->color = skin;
                thighR->addChild(shinR);

                auto getaR = std::make_shared<SceneNode>("Aud_GetaR_" + std::to_string(personIndex));
                getaR->mesh = &meshes.getaFoot;
                getaR->transform.position = glm::vec3(0.0f, -0.40f, 0.05f);
                getaR->color = benchWood;
                shinR->addChild(getaR);

                // Torso upright in Yukata (contoured human torso with kimono collar Eri)
                auto torso = std::make_shared<SceneNode>("Aud_Torso_" + std::to_string(personIndex));
                torso->mesh = &meshes.humanTorso;
                torso->transform.position = glm::vec3(0.0f, 0.95f, 0.0f);
                torso->transform.scale = glm::vec3(1.05f, 0.95f, 1.05f);
                torso->color = yukata;
                person->addChild(torso);

                // Obi sash belt
                auto obi = std::make_shared<SceneNode>("Aud_Obi_" + std::to_string(personIndex));
                obi->mesh = &meshes.cube;
                obi->transform.position = glm::vec3(0.0f, 0.85f, 0.0f);
                obi->transform.scale = glm::vec3(0.55f, 0.16f, 0.40f);
                obi->color = goldObi;
                person->addChild(obi);

                // Articulated Arms resting gently on lap/knees
                auto armL = std::make_shared<SceneNode>("Aud_ArmL_" + std::to_string(personIndex));
                armL->mesh = &meshes.limbUpperArm;
                armL->transform.position = glm::vec3(-0.30f, 1.25f, 0.05f);
                armL->transform.rotation.x = -25.0f;
                armL->transform.rotation.z = -10.0f;
                armL->color = yukata;
                person->addChild(armL);

                auto forearmL = std::make_shared<SceneNode>("Aud_ForearmL_" + std::to_string(personIndex));
                forearmL->mesh = &meshes.limbForearm;
                forearmL->transform.position = glm::vec3(0.0f, -0.36f, 0.0f);
                forearmL->transform.rotation.x = 45.0f;
                forearmL->color = skin;
                armL->addChild(forearmL);

                auto handL = std::make_shared<SceneNode>("Aud_HandL_" + std::to_string(personIndex));
                handL->mesh = &meshes.humanHand;
                handL->transform.position = glm::vec3(0.0f, -0.32f, 0.0f);
                handL->color = skin;
                forearmL->addChild(handL);

                auto armR = std::make_shared<SceneNode>("Aud_ArmR_" + std::to_string(personIndex));
                armR->mesh = &meshes.limbUpperArm;
                armR->transform.position = glm::vec3(0.30f, 1.25f, 0.05f);
                armR->transform.rotation.x = -25.0f;
                armR->transform.rotation.z = 10.0f;
                armR->color = yukata;
                person->addChild(armR);

                auto forearmR = std::make_shared<SceneNode>("Aud_ForearmR_" + std::to_string(personIndex));
                forearmR->mesh = &meshes.limbForearm;
                forearmR->transform.position = glm::vec3(0.0f, -0.36f, 0.0f);
                forearmR->transform.rotation.x = 45.0f;
                forearmR->color = skin;
                armR->addChild(forearmR);

                auto handR = std::make_shared<SceneNode>("Aud_HandR_" + std::to_string(personIndex));
                handR->mesh = &meshes.humanHand;
                handR->transform.position = glm::vec3(0.0f, -0.32f, 0.0f);
                handR->color = skin;
                forearmR->addChild(handR);

                // Sculpted Human Head with facial contours
                auto head = std::make_shared<SceneNode>("Aud_Head_" + std::to_string(personIndex));
                head->mesh = &meshes.humanHead;
                head->transform.position = glm::vec3(0.0f, 1.50f, 0.0f);
                head->transform.scale = glm::vec3(1.10f);
                head->color = skin;
                person->addChild(head);
                heads.push_back(head);

                // Traditional festival hair / top knot
                auto hair = std::make_shared<SceneNode>("Aud_Hair_" + std::to_string(personIndex));
                hair->mesh = &meshes.sphere;
                hair->transform.position = glm::vec3(0.0f, 1.62f, -0.04f);
                hair->transform.scale = glm::vec3(0.36f, 0.16f, 0.36f);
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
    std::shared_ptr<SceneNode> thighL;
    std::shared_ptr<SceneNode> shinL;
    std::shared_ptr<SceneNode> thighR;
    std::shared_ptr<SceneNode> shinR;
    std::shared_ptr<SceneNode> armL;
    std::shared_ptr<SceneNode> forearmL;
    std::shared_ptr<SceneNode> armR;
    std::shared_ptr<SceneNode> forearmR;
    std::shared_ptr<SceneNode> head;
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

            glm::vec4 yukataColor = yukataTones[i % yukataTones.size()];
            glm::vec4 skinColor(0.92f, 0.76f, 0.64f, 1.0f);
            glm::vec4 pantsColor(0.18f, 0.18f, 0.22f, 1.0f);
            glm::vec4 woodColor(0.35f, 0.22f, 0.14f, 1.0f);

            // Contoured Human Torso wearing traditional Yukata with Kimono Eri collar
            auto torso = std::make_shared<SceneNode>("Walk_Torso_" + std::to_string(i));
            torso->mesh = &meshes.humanTorso;
            torso->transform.position = glm::vec3(0.0f, 1.25f, 0.0f);
            torso->transform.scale = glm::vec3(1.10f, 1.0f, 1.10f);
            torso->color = yukataColor;
            wp.root->addChild(torso);

            // Sash (Obi belt)
            auto obi = std::make_shared<SceneNode>("Walk_Obi_" + std::to_string(i));
            obi->mesh = &meshes.cube;
            obi->transform.position = glm::vec3(0.0f, 1.08f, 0.0f);
            obi->transform.scale = glm::vec3(0.60f, 0.20f, 0.42f);
            obi->color = glm::vec4(0.88f, 0.82f, 0.35f, 1.0f);
            wp.root->addChild(obi);

            // Sculpted Anatomical Human Head & Face
            wp.head = std::make_shared<SceneNode>("Walk_Head_" + std::to_string(i));
            wp.head->mesh = &meshes.humanHead;
            wp.head->transform.position = glm::vec3(0.0f, 1.82f, 0.0f);
            wp.head->transform.scale = glm::vec3(1.12f);
            wp.head->color = skinColor;
            wp.root->addChild(wp.head);

            // Left Leg: Hip Pivot -> Thigh -> Knee Condyle -> Shin -> Ankle -> Geta Sandal
            wp.thighL = std::make_shared<SceneNode>("Walk_ThighL_" + std::to_string(i));
            wp.thighL->mesh = &meshes.limbThigh;
            wp.thighL->transform.position = glm::vec3(-0.16f, 0.84f, 0.0f);
            wp.thighL->color = pantsColor;
            wp.root->addChild(wp.thighL);

            wp.shinL = std::make_shared<SceneNode>("Walk_ShinL_" + std::to_string(i));
            wp.shinL->mesh = &meshes.limbShin;
            wp.shinL->transform.position = glm::vec3(0.0f, -0.42f, 0.0f);
            wp.shinL->color = skinColor;
            wp.thighL->addChild(wp.shinL);

            auto getaL = std::make_shared<SceneNode>("Walk_GetaL_" + std::to_string(i));
            getaL->mesh = &meshes.getaFoot;
            getaL->transform.position = glm::vec3(0.0f, -0.42f, 0.04f);
            getaL->color = woodColor;
            wp.shinL->addChild(getaL);

            // Right Leg: Hip Pivot -> Thigh -> Knee Condyle -> Shin -> Ankle -> Geta Sandal
            wp.thighR = std::make_shared<SceneNode>("Walk_ThighR_" + std::to_string(i));
            wp.thighR->mesh = &meshes.limbThigh;
            wp.thighR->transform.position = glm::vec3(0.16f, 0.84f, 0.0f);
            wp.thighR->color = pantsColor;
            wp.root->addChild(wp.thighR);

            wp.shinR = std::make_shared<SceneNode>("Walk_ShinR_" + std::to_string(i));
            wp.shinR->mesh = &meshes.limbShin;
            wp.shinR->transform.position = glm::vec3(0.0f, -0.42f, 0.0f);
            wp.shinR->color = skinColor;
            wp.thighR->addChild(wp.shinR);

            auto getaR = std::make_shared<SceneNode>("Walk_GetaR_" + std::to_string(i));
            getaR->mesh = &meshes.getaFoot;
            getaR->transform.position = glm::vec3(0.0f, -0.42f, 0.04f);
            getaR->color = woodColor;
            wp.shinR->addChild(getaR);

            // Left Arm: Shoulder Pivot -> Upper Arm / Sleeve -> Elbow Condyle -> Forearm -> Wrist -> Hand
            wp.armL = std::make_shared<SceneNode>("Walk_ArmL_" + std::to_string(i));
            wp.armL->mesh = &meshes.limbUpperArm;
            wp.armL->transform.position = glm::vec3(-0.34f, 1.58f, 0.0f);
            wp.armL->color = yukataColor;
            wp.root->addChild(wp.armL);

            wp.forearmL = std::make_shared<SceneNode>("Walk_ForearmL_" + std::to_string(i));
            wp.forearmL->mesh = &meshes.limbForearm;
            wp.forearmL->transform.position = glm::vec3(0.0f, -0.38f, 0.0f);
            wp.forearmL->color = skinColor;
            wp.armL->addChild(wp.forearmL);

            auto handL = std::make_shared<SceneNode>("Walk_HandL_" + std::to_string(i));
            handL->mesh = &meshes.humanHand;
            handL->transform.position = glm::vec3(0.0f, -0.36f, 0.0f);
            handL->transform.rotation.z = 90.0f;
            handL->color = skinColor;
            wp.forearmL->addChild(handL);

            // Right Arm: Shoulder Pivot -> Upper Arm / Sleeve -> Elbow Condyle -> Forearm -> Wrist -> Hand
            wp.armR = std::make_shared<SceneNode>("Walk_ArmR_" + std::to_string(i));
            wp.armR->mesh = &meshes.limbUpperArm;
            wp.armR->transform.position = glm::vec3(0.34f, 1.58f, 0.0f);
            wp.armR->color = yukataColor;
            wp.root->addChild(wp.armR);

            wp.forearmR = std::make_shared<SceneNode>("Walk_ForearmR_" + std::to_string(i));
            wp.forearmR->mesh = &meshes.limbForearm;
            wp.forearmR->transform.position = glm::vec3(0.0f, -0.38f, 0.0f);
            wp.forearmR->color = skinColor;
            wp.armR->addChild(wp.forearmR);

            auto handR = std::make_shared<SceneNode>("Walk_HandR_" + std::to_string(i));
            handR->mesh = &meshes.humanHand;
            handR->transform.position = glm::vec3(0.0f, -0.36f, 0.0f);
            handR->transform.rotation.z = -90.0f;
            handR->color = skinColor;
            wp.forearmR->addChild(handR);

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

            // Alternating hip swing walk-cycle
            float walkAngle = std::sin(time * 6.0f + wp.phase) * 26.0f;
            wp.thighL->transform.rotation.x = walkAngle;
            wp.thighR->transform.rotation.x = -walkAngle;

            // Biomechanical knee flexion kinematics:
            // When thigh swings backward (< 0), knee bends backward (+X angle) to lift sandal
            // When thigh swings forward (> 0), knee straightens for heel strike
            float kneeL = std::max(0.0f, -walkAngle * 1.35f);
            float kneeR = std::max(0.0f, walkAngle * 1.35f);
            wp.shinL->transform.rotation.x = kneeL;
            wp.shinR->transform.rotation.x = kneeR;

            // Realistic walking arm swing: arms swing in opposite phase to legs
            float armAngle = -walkAngle * 0.75f;
            wp.armL->transform.rotation.x = armAngle;
            wp.armR->transform.rotation.x = -armAngle;
            wp.armL->transform.rotation.z = -6.0f;
            wp.armR->transform.rotation.z = 6.0f;

            // Biomechanical elbow kinematics:
            // Resting flex ~ 18 deg, bends further on forward swing
            wp.forearmL->transform.rotation.x = 18.0f + std::max(0.0f, armAngle * 0.65f);
            wp.forearmR->transform.rotation.x = 18.0f + std::max(0.0f, -armAngle * 0.65f);

            // Subtle head tilt / gaze sway during motion
            if (wp.head)
            {
                wp.head->transform.rotation.y = std::sin(time * 3.0f + wp.phase) * 4.0f;
                wp.head->transform.rotation.z = std::cos(time * 6.0f + wp.phase) * 2.0f;
            }
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

    bool getActiveBurst(glm::vec3& outPos, glm::vec3& outColor) const
    {
        for (const auto& r : rockets)
        {
            if (r.state == FireworkRocket::BURSTING && r.timer < 0.85f)
            {
                outPos = r.burstPos;
                outColor = r.color;
                return true;
            }
        }
        return false;
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
