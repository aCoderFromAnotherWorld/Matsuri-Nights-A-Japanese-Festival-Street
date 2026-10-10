#include "InteractionManager.h"
#include "../Scene.h"
#include <GLFW/glfw3.h>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

namespace
{
    std::string formatVec3Compact(const glm::vec3& v, int precision = 1)
    {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(precision)
           << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return ss.str();
    }
}

void InteractionManager::init(Scene& scene)
{
    interactables.clear();

    // ---------------------------------------------------------------------
    // 1. Machiya Buildings (4 units with interactive sliding doors & windows)
    // ---------------------------------------------------------------------
    for (size_t b = 0; b < scene.buildings.size(); ++b)
    {
        auto& bld = scene.buildings[b];
        if (!bld) continue;

        Interactable it;
        std::string bldName = bld->root ? bld->root->name : ("Machiya_" + std::to_string(b));
        it.id = "building_" + std::to_string(b);
        it.displayName = bldName;
        it.category = "Building";
        it.node = bld->root;
        it.inspectableIndex = 11 + static_cast<int>(b);
        it.canManualSelect = true;

        MachiyaBuilding* rawBld = bld.get();

        it.getWorldPosition = [rawBld]() -> glm::vec3 {
            return rawBld->root ? rawBld->root->getWorldPosition() : rawBld->worldPos;
        };
        it.interactionRadius = 14.0f;

        it.getStatusString = [rawBld, bldName]() -> std::string {
            int dPct = static_cast<int>(std::round(rawBld->doorSlideProgress * 100.0f));
            int wPct = static_cast<int>(std::round(rawBld->windowSlideProgress * 100.0f));
            std::string dState = rawBld->isDoorOpen ? ("OPEN (" + std::to_string(dPct) + "%)") : "CLOSED";
            std::string wState = rawBld->isWindowOpen ? ("OPEN (" + std::to_string(wPct) + "%)") : "CLOSED";
            return "Door: " + dState + " | Windows: " + wState;
        };

        it.getTransform = [rawBld](glm::vec3& pos, glm::vec3& rot, glm::vec3& scale) {
            if (rawBld->root)
            {
                pos = rawBld->root->transform.position;
                rot = rawBld->root->transform.rotation;
                scale = rawBld->root->transform.scale;
            }
            else
            {
                pos = rawBld->worldPos;
                rot = glm::vec3(0.0f, rawBld->rotationY, 0.0f);
                scale = glm::vec3(1.0f);
            }
        };

        it.getLightInfo = [b, &scene]() -> std::string {
            int lightIdx = 6 + static_cast<int>(b);
            if (lightIdx < static_cast<int>(scene.pointLights.size()))
            {
                const auto& pl = scene.pointLights[lightIdx];
                return "Interior Lamp: Point | Col (" + std::to_string(static_cast<int>(pl.diffuse.r * 100)) + "%, "
                       + std::to_string(static_cast<int>(pl.diffuse.g * 100)) + "%, "
                       + std::to_string(static_cast<int>(pl.diffuse.b * 100)) + "%)";
            }
            return "";
        };

        // Action [H]: Slide Shoji Door
        ActionHint doorAction;
        doorAction.key = GLFW_KEY_H;
        doorAction.keyName = "H";
        doorAction.getLabel = [rawBld]() -> std::string {
            return rawBld->isDoorOpen ? "Slide Close Shoji Door" : "Slide Open Shoji Door";
        };
        doorAction.isEnabled = [rawBld, &scene]() -> bool {
            glm::vec3 doorLocal(4.14f, 1.2f, -0.90f);
            glm::vec3 doorPos = rawBld->worldPos;
            if (std::abs(rawBld->rotationY - 180.0f) < 1.0f)
                doorPos += glm::vec3(-doorLocal.x, doorLocal.y, -doorLocal.z);
            else
                doorPos += doorLocal;
            return true;
        };
        doorAction.execute = [rawBld, &scene]() {
            rawBld->toggleDoor();
        };
        it.actions.push_back(doorAction);

        // Action [G]: Slide Shoji Windows
        ActionHint winAction;
        winAction.key = GLFW_KEY_G;
        winAction.keyName = "G";
        winAction.getLabel = [rawBld]() -> std::string {
            return rawBld->isWindowOpen ? "Slide Close Windows" : "Slide Open Windows";
        };
        winAction.isEnabled = []() -> bool { return true; };
        winAction.execute = [rawBld, &scene]() {
            rawBld->toggleWindows();
        };
        it.actions.push_back(winAction);

        interactables.push_back(it);
    }

    // ---------------------------------------------------------------------
    // 2. Takoyaki Food Stall
    // ---------------------------------------------------------------------
    if (scene.takoyakiStall)
    {
        Interactable it;
        it.id = "takoyaki_stall";
        it.displayName = "Takoyaki Food Stall";
        it.category = "Stall";
        it.node = scene.takoyakiStall->root;
        it.inspectableIndex = 6; // Index 6 in scene.inspectables (Obj #7)
        it.canManualSelect = true;

        auto* stall = scene.takoyakiStall.get();
        it.getWorldPosition = [stall]() { return stall->root ? stall->root->getWorldPosition() : glm::vec3(-5.2f, 0.0f, 6.0f); };
        it.interactionRadius = 9.0f;
        it.getStatusString = [stall, &scene]() {
            return "Grill: Active (6 sizzling tako balls, amber lantern LIT)";
        };
        it.getTransform = [stall](glm::vec3& pos, glm::vec3& rot, glm::vec3& scale) {
            if (stall->root)
            {
                pos = stall->root->transform.position;
                rot = stall->root->transform.rotation;
                scale = stall->root->transform.scale;
            }
        };
        it.getLightInfo = [&scene]() {
            return "Stall Light: Point #1 | Amber (1.0, 0.60, 0.22)";
        };

        ActionHint lightAction;
        lightAction.key = GLFW_KEY_0;
        lightAction.keyName = "0";
        lightAction.getLabel = [&scene]() {
            return scene.lanternLightsOn ? "Dim Lanterns/Stall Lights" : "Light Lanterns/Stall";
        };
        lightAction.isEnabled = []() { return true; };
        lightAction.execute = [&scene]() { scene.toggleLanternLights(); };
        it.actions.push_back(lightAction);

        ActionHint pauseAction;
        pauseAction.key = GLFW_KEY_SPACE;
        pauseAction.keyName = "Space";
        pauseAction.getLabel = [&scene]() {
            return scene.isPaused ? "Resume Sizzling Animation" : "Pause Sizzling Animation";
        };
        pauseAction.isEnabled = []() { return true; };
        pauseAction.execute = [&scene]() { scene.togglePause(); };
        it.actions.push_back(pauseAction);

        interactables.push_back(it);
    }

    // ---------------------------------------------------------------------
    // 3. Kakigori Shaved Ice Stall
    // ---------------------------------------------------------------------
    if (scene.kakigoriStall)
    {
        Interactable it;
        it.id = "kakigori_stall";
        it.displayName = "Kakigori Ice Stall";
        it.category = "Stall";
        it.node = scene.kakigoriStall->root;
        it.inspectableIndex = 7; // Index 7 in scene.inspectables (Obj #8)
        it.canManualSelect = true;

        auto* stall = scene.kakigoriStall.get();
        it.getWorldPosition = [stall]() { return stall->root ? stall->root->getWorldPosition() : glm::vec3(5.2f, 0.0f, 6.0f); };
        it.interactionRadius = 9.0f;
        it.getStatusString = [stall, &scene]() {
            return "Ice Shaver: Running (6 bowls hopping, festive cyan eaves light)";
        };
        it.getTransform = [stall](glm::vec3& pos, glm::vec3& rot, glm::vec3& scale) {
            if (stall->root)
            {
                pos = stall->root->transform.position;
                rot = stall->root->transform.rotation;
                scale = stall->root->transform.scale;
            }
        };
        it.getLightInfo = [&scene]() {
            return "Stall Light: Point #2 | Cyan (0.35, 0.90, 1.0)";
        };

        ActionHint lightAction;
        lightAction.key = GLFW_KEY_0;
        lightAction.keyName = "0";
        lightAction.getLabel = [&scene]() {
            return scene.lanternLightsOn ? "Dim Lanterns/Stall Lights" : "Light Lanterns/Stall";
        };
        lightAction.isEnabled = []() { return true; };
        lightAction.execute = [&scene]() { scene.toggleLanternLights(); };
        it.actions.push_back(lightAction);

        ActionHint pauseAction;
        pauseAction.key = GLFW_KEY_SPACE;
        pauseAction.keyName = "Space";
        pauseAction.getLabel = [&scene]() {
            return scene.isPaused ? "Resume Shaving Animation" : "Pause Shaving Animation";
        };
        pauseAction.isEnabled = []() { return true; };
        pauseAction.execute = [&scene]() { scene.togglePause(); };
        it.actions.push_back(pauseAction);

        interactables.push_back(it);
    }

    // ---------------------------------------------------------------------
    // 4. Magic Show Stage & Magician Figure
    // ---------------------------------------------------------------------
    if (scene.magician)
    {
        Interactable it;
        it.id = "magician_figure";
        it.displayName = "Magician (Stage)";
        it.category = "Magic Show";
        it.node = scene.magician->root;
        it.inspectableIndex = 3; // Index 3 in scene.inspectables (Obj #4)
        it.canManualSelect = true;

        auto* mag = scene.magician.get();
        it.getWorldPosition = [mag]() { return mag->root ? mag->root->getWorldPosition() : glm::vec3(6.2f, 0.95f, -19.0f); };
        it.interactionRadius = 12.0f;
        it.getStatusString = [mag]() {
            return "Trick: Helical Magic Orb orbit (r=0.45m), Wand gestures";
        };
        it.getTransform = [mag](glm::vec3& pos, glm::vec3& rot, glm::vec3& scale) {
            if (mag->root)
            {
                pos = mag->root->transform.position;
                rot = mag->root->transform.rotation;
                scale = mag->root->transform.scale;
            }
        };
        it.getLightInfo = [&scene]() {
            return "Orb Light: Point #0 | Mystical Cyan (0.35, 0.85, 1.0)";
        };

        ActionHint replayAction;
        replayAction.key = GLFW_KEY_M;
        replayAction.keyName = "M";
        replayAction.getLabel = []() { return "Replay Magic Show Trick"; };
        replayAction.isEnabled = []() { return true; };
        replayAction.execute = [&scene]() { scene.replayMagicTrick(); };
        it.actions.push_back(replayAction);

        ActionHint camAction;
        camAction.key = GLFW_KEY_2;
        camAction.keyName = "2";
        camAction.getLabel = []() { return "Preset Cam 2 (Magic Stage)"; };
        camAction.isEnabled = []() { return true; };
        camAction.execute = []() {};
        it.actions.push_back(camAction);

        interactables.push_back(it);
    }

    // ---------------------------------------------------------------------
    // 5. Vanishing Box Trick (Scale-to-Zero Demo)
    // ---------------------------------------------------------------------
    if (scene.vanishingBox)
    {
        Interactable it;
        it.id = "vanishing_box";
        it.displayName = "Vanishing Box Trick";
        it.category = "Magic Show";
        it.node = scene.vanishingBox->boxNode;
        it.inspectableIndex = 4; // Index 4 in scene.inspectables (Obj #5)
        it.canManualSelect = true;

        auto* box = scene.vanishingBox.get();
        it.getWorldPosition = [box]() {
            return box->boxNode ? box->boxNode->getWorldPosition() : glm::vec3(5.1f, 1.35f, -19.1f);
        };
        it.interactionRadius = 10.0f;

        it.getStatusString = [box]() -> std::string {
            float cycle = std::fmod(box->stateTimer, 7.0f);
            if (cycle < 1.5f) return "Phase 1/6: Silk cloth descending on gold box";
            if (cycle < 2.5f) return "Phase 2/6: Scale-to-zero vanishing in progress";
            if (cycle < 4.0f) return "Phase 3/6: Cloth whipped aside, spot empty!";
            if (cycle < 5.0f) return "Phase 4/6: Silk cloth moving to Spot 2";
            if (cycle < 6.0f) return "Phase 5/6: Box reappears at Spot 2 (Scale 0->1)";
            return "Phase 6/6: Resetting table for next performance";
        };

        it.getTransform = [box](glm::vec3& pos, glm::vec3& rot, glm::vec3& scale) {
            if (box->boxNode)
            {
                pos = box->boxNode->transform.position;
                rot = box->boxNode->transform.rotation;
                scale = box->boxNode->transform.scale;
            }
        };

        ActionHint restartAction;
        restartAction.key = GLFW_KEY_M;
        restartAction.keyName = "M";
        restartAction.getLabel = []() { return "Restart Vanishing Sequence"; };
        restartAction.isEnabled = []() { return true; };
        restartAction.execute = [&scene]() { scene.replayMagicTrick(); };
        it.actions.push_back(restartAction);

        interactables.push_back(it);
    }

    // ---------------------------------------------------------------------
    // 6. Stage Spotlight Rig (Moving Tracking Light)
    // ---------------------------------------------------------------------
    if (scene.spotlightRig)
    {
        Interactable it;
        it.id = "spotlight_rig";
        it.displayName = "Stage Spotlight Rig";
        it.category = "Lighting";
        it.node = scene.spotlightRig->lampHousing;
        it.inspectableIndex = 5; // Index 5 in scene.inspectables (Obj #6)
        it.canManualSelect = true;

        auto* spot = scene.spotlightRig.get();
        it.getWorldPosition = [spot]() {
            return spot->lampHousing ? spot->lampHousing->getWorldPosition() : glm::vec3(6.2f, 5.0f, -14.0f);
        };
        it.interactionRadius = 12.0f;
        it.getStatusString = [&scene]() {
            return scene.spotLight.active ? "Spotlight: Active (Tracking magician, cut-off 15 deg)" : "Spotlight: OFF";
        };
        it.getTransform = [spot](glm::vec3& pos, glm::vec3& rot, glm::vec3& scale) {
            if (spot->lampHousing)
            {
                pos = spot->lampHousing->transform.position;
                rot = spot->lampHousing->transform.rotation;
                scale = spot->lampHousing->transform.scale;
            }
        };
        it.getLightInfo = [&scene]() {
            return "Spotlight: Cone tracking | Diffuse (1.5, 1.35, 1.1)";
        };

        interactables.push_back(it);
    }

    // ---------------------------------------------------------------------
    // 7. Swinging Street Lantern Row
    // ---------------------------------------------------------------------
    if (!scene.lanterns.empty() && scene.lanterns[0])
    {
        Interactable it;
        it.id = "street_lantern";
        it.displayName = "Swinging Street Lantern";
        it.category = "Lantern";
        it.node = scene.lanterns[0]->lanternBody;
        it.inspectableIndex = 0; // Index 0 in scene.inspectables (Obj #1)
        it.canManualSelect = true;

        auto* lantern = scene.lanterns[0];
        it.getWorldPosition = [lantern]() {
            return lantern->lanternBody ? lantern->lanternBody->getWorldPosition() : glm::vec3(0.0f, 4.5f, 11.0f);
        };
        it.interactionRadius = 10.0f;
        it.getStatusString = [lantern, &scene]() {
            float ang = lantern->ropePivot ? lantern->ropePivot->transform.rotation.x : 0.0f;
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(1) << ang;
            std::string litStatus = scene.lanternLightsOn ? "LIT (Paper Emissive)" : "OFF (Dimmed)";
            return "Lantern: " + litStatus + ", swing " + ss.str() + " deg";
        };
        it.getTransform = [lantern](glm::vec3& pos, glm::vec3& rot, glm::vec3& scale) {
            if (lantern->lanternBody)
            {
                pos = lantern->lanternBody->transform.position;
                rot = lantern->lanternBody->transform.rotation;
                scale = lantern->lanternBody->transform.scale;
            }
        };
        it.getLightInfo = [&scene]() {
            return "Street Lights: Points #3 & #4 | Amber (1.0, 0.55, 0.20)";
        };

        ActionHint lightAction;
        lightAction.key = GLFW_KEY_0;
        lightAction.keyName = "0";
        lightAction.getLabel = [&scene]() {
            return scene.lanternLightsOn ? "Dim Street Lantern Lights" : "Light Street Lanterns";
        };
        lightAction.isEnabled = []() { return true; };
        lightAction.execute = [&scene]() { scene.toggleLanternLights(); };
        it.actions.push_back(lightAction);

        interactables.push_back(it);
    }

    // ---------------------------------------------------------------------
    // 8. Grand Vermilion Torii Gate
    // ---------------------------------------------------------------------
    if (scene.toriiGate)
    {
        Interactable it;
        it.id = "torii_gate";
        it.displayName = "Torii Shrine Gate";
        it.category = "Shrine";
        it.node = scene.toriiGate->root;
        it.inspectableIndex = 8; // Index 8 in scene.inspectables (Obj #9)
        it.canManualSelect = true;

        auto* torii = scene.toriiGate.get();
        it.getWorldPosition = [torii]() {
            return torii->root ? torii->root->getWorldPosition() : glm::vec3(0.0f, 0.0f, -22.0f);
        };
        it.interactionRadius = 16.0f;
        it.getStatusString = []() {
            return "Grand Shrine Portal (Curved upward Kasagi & Shimaki arch)";
        };
        it.getTransform = [torii](glm::vec3& pos, glm::vec3& rot, glm::vec3& scale) {
            if (torii->root)
            {
                pos = torii->root->transform.position;
                rot = torii->root->transform.rotation;
                scale = torii->root->transform.scale;
            }
        };

        ActionHint fwAction;
        fwAction.key = GLFW_KEY_F;
        fwAction.keyName = "F";
        fwAction.getLabel = []() { return "Launch Firework Over Torii"; };
        fwAction.isEnabled = []() { return true; };
        fwAction.execute = [&scene]() { scene.triggerFirework(); };
        it.actions.push_back(fwAction);

        ActionHint camAction;
        camAction.key = GLFW_KEY_3;
        camAction.keyName = "3";
        camAction.getLabel = []() { return "Preset Cam 3 (Torii & Sky)"; };
        camAction.isEnabled = []() { return true; };
        camAction.execute = []() {};
        it.actions.push_back(camAction);

        interactables.push_back(it);
    }

    // ---------------------------------------------------------------------
    // 9. Sakura Blossom Tree
    // ---------------------------------------------------------------------
    if (scene.sakuraTree)
    {
        Interactable it;
        it.id = "sakura_tree";
        it.displayName = "Sakura Blossom Tree";
        it.category = "Tree";
        it.node = scene.sakuraTree->root;
        it.inspectableIndex = 9; // Index 9 in scene.inspectables (Obj #10)
        it.canManualSelect = true;

        auto* tree = scene.sakuraTree;
        it.getWorldPosition = [tree]() {
            return tree->root ? tree->root->getWorldPosition() : glm::vec3(-4.5f, 0.0f, -15.0f);
        };
        it.interactionRadius = 12.0f;
        it.getStatusString = [&scene]() {
            return scene.isPaused ? "Blossoms Frozen: Animation PAUSED" : "Blossoms Swaying: Falling petals particle system active";
        };
        it.getTransform = [tree](glm::vec3& pos, glm::vec3& rot, glm::vec3& scale) {
            if (tree->root)
            {
                pos = tree->root->transform.position;
                rot = tree->root->transform.rotation;
                scale = tree->root->transform.scale;
            }
        };

        ActionHint pauseAction;
        pauseAction.key = GLFW_KEY_SPACE;
        pauseAction.keyName = "Space";
        pauseAction.getLabel = [&scene]() {
            return scene.isPaused ? "Resume Petal Falling" : "Freeze Petal Falling";
        };
        pauseAction.isEnabled = []() { return true; };
        pauseAction.execute = [&scene]() { scene.togglePause(); };
        it.actions.push_back(pauseAction);

        interactables.push_back(it);
    }

    // ---------------------------------------------------------------------
    // 10. Crowd Walker #1
    // ---------------------------------------------------------------------
    if (scene.crowd && !scene.crowd->walkers.empty())
    {
        Interactable it;
        it.id = "crowd_walker_1";
        it.displayName = "Crowd Walker #1";
        it.category = "Crowd";
        it.node = scene.crowd->walkers[0].root;
        it.inspectableIndex = 10; // Index 10 in scene.inspectables (Obj #11)
        it.canManualSelect = true;

        auto* walker = &scene.crowd->walkers[0];
        it.getWorldPosition = [walker]() {
            return walker->root ? walker->root->getWorldPosition() : glm::vec3(0.0f);
        };
        it.interactionRadius = 8.0f;
        it.getStatusString = [&scene]() {
            return scene.isPaused ? "Walker Paused (Frozen mid-stride)" : "Walking down festival street with articulated leg cycle";
        };
        it.getTransform = [walker](glm::vec3& pos, glm::vec3& rot, glm::vec3& scale) {
            if (walker->root)
            {
                pos = walker->root->transform.position;
                rot = walker->root->transform.rotation;
                scale = walker->root->transform.scale;
            }
        };

        interactables.push_back(it);
    }

    // ---------------------------------------------------------------------
    // 11. Fireworks System
    // ---------------------------------------------------------------------
    if (scene.fireworks)
    {
        Interactable it;
        it.id = "fireworks_rig";
        it.displayName = "Fireworks Launch Rig";
        it.category = "Sky & FX";
        it.canManualSelect = false;

        it.getWorldPosition = []() { return glm::vec3(0.0f, 15.0f, -24.0f); };
        it.interactionRadius = 35.0f;
        it.getStatusString = [&scene]() {
            glm::vec3 bPos, bCol;
            bool burst = scene.fireworks && scene.fireworks->getActiveBurst(bPos, bCol);
            return burst ? "Sky Flash Active: Detonating multi-colored burst particles" : "Ready: Rockets launching periodically / manually";
        };
        it.getTransform = [](glm::vec3& pos, glm::vec3& rot, glm::vec3& scale) {
            pos = glm::vec3(0.0f, 15.0f, -24.0f);
            rot = glm::vec3(0.0f);
            scale = glm::vec3(1.0f);
        };
        it.getLightInfo = [&scene]() {
            return "Sky Flash Light: Point #5 | Dynamic particle color";
        };

        ActionHint fireAction;
        fireAction.key = GLFW_KEY_F;
        fireAction.keyName = "F";
        fireAction.getLabel = []() { return "Launch Firework Rocket"; };
        fireAction.isEnabled = []() { return true; };
        fireAction.execute = [&scene]() { scene.triggerFirework(); };
        it.actions.push_back(fireAction);

        ActionHint nightAction;
        nightAction.key = GLFW_KEY_N;
        nightAction.keyName = "N";
        nightAction.getLabel = [&scene]() {
            return scene.targetNight ? "Transition to Daylight" : "Transition to Festival Night";
        };
        nightAction.isEnabled = []() { return true; };
        nightAction.execute = [&scene]() { scene.toggleDayNight(); };
        it.actions.push_back(nightAction);

        interactables.push_back(it);
    }
}

void InteractionManager::update(const Camera& camera, float dt, Scene& scene)
{
    // If manual lock is active and index is valid
    if (isManualLocked && manualLockedIndex >= 0 && manualLockedIndex < static_cast<int>(interactables.size()))
    {
        currentSelected = &interactables[manualLockedIndex];
        currentDistance = glm::distance(camera.Position, currentSelected->getWorldPosition());
        return;
    }

    // Auto Selection: Find nearest interactable inside radius AND view-cone (dot >= 0.45)
    const Interactable* bestCandidate = nullptr;
    float bestScore = 99999.0f;

    for (const auto& it : interactables)
    {
        glm::vec3 objPos = it.getWorldPosition();
        float dist = glm::distance(camera.Position, objPos);

        if (dist > it.interactionRadius)
            continue;

        glm::vec3 dirToObj = glm::normalize(objPos - camera.Position);
        float dot = glm::dot(camera.Front, dirToObj);

        // View-cone threshold: ~63 degree half-angle
        if (dot < 0.45f)
            continue;

        // Score balances proximity and direct gaze alignment
        float score = dist / (dot + 0.2f);
        if (score < bestScore)
        {
            bestScore = score;
            bestCandidate = &it;
        }
    }

    if (bestCandidate)
    {
        currentSelected = bestCandidate;
        currentDistance = glm::distance(camera.Position, bestCandidate->getWorldPosition());
        if (currentSelected->inspectableIndex >= 0 && currentSelected->inspectableIndex < static_cast<int>(scene.inspectables.size()))
        {
            scene.selectedIndex = currentSelected->inspectableIndex;
        }
    }
    else
    {
        currentSelected = nullptr;
        currentDistance = 0.0f;
    }
}

void InteractionManager::cycleSelection(int dir, Scene& scene)
{
    if (interactables.empty()) return;

    // Build list of manually selectable interactable indices
    std::vector<int> selectableIndices;
    for (int i = 0; i < static_cast<int>(interactables.size()); ++i)
    {
        if (interactables[i].canManualSelect)
            selectableIndices.push_back(i);
    }

    if (selectableIndices.empty()) return;

    if (!isManualLocked)
    {
        // First press locks to the first selectable
        isManualLocked = true;
        manualLockedIndex = selectableIndices[0];
    }
    else
    {
        // Find current position in selectable list
        int currentPos = 0;
        for (int i = 0; i < static_cast<int>(selectableIndices.size()); ++i)
        {
            if (selectableIndices[i] == manualLockedIndex)
            {
                currentPos = i;
                break;
            }
        }

        int nextPos = currentPos + dir;
        if (nextPos >= static_cast<int>(selectableIndices.size()) || nextPos < 0)
        {
            // Wrap or cycle through auto-select
            if (dir > 0 && nextPos >= static_cast<int>(selectableIndices.size()))
            {
                unlockToAuto();
                return;
            }
            nextPos = (nextPos + static_cast<int>(selectableIndices.size())) % static_cast<int>(selectableIndices.size());
        }
        manualLockedIndex = selectableIndices[nextPos];
    }

    currentSelected = &interactables[manualLockedIndex];
    if (currentSelected && currentSelected->inspectableIndex >= 0 && currentSelected->inspectableIndex < static_cast<int>(scene.inspectables.size()))
    {
        scene.selectedIndex = currentSelected->inspectableIndex;
        scene.printCurrentInspection();
    }
}

void InteractionManager::unlockToAuto()
{
    isManualLocked = false;
    manualLockedIndex = -1;
    currentSelected = nullptr;
}

bool InteractionManager::handleKey(int key, Scene& scene, const Camera& camera)
{
    // 1. If an object is currently selected, check its specific actions first
    if (currentSelected)
    {
        for (const auto& act : currentSelected->actions)
        {
            bool keyMatch = (act.key == key) || (act.key == GLFW_KEY_0 && key == GLFW_KEY_KP_0);
            if (keyMatch && act.isEnabled && act.isEnabled())
            {
                if (act.execute)
                    act.execute();
                return true;
            }
        }
    }

    // 2. Fallbacks for nearest object actions if auto-target wasn't centered
    if (key == GLFW_KEY_H)
    {
        scene.interactNearestDoor(camera.Position);
        return true;
    }
    if (key == GLFW_KEY_G)
    {
        scene.interactNearestWindow(camera.Position);
        return true;
    }
    if (key == GLFW_KEY_0 || key == GLFW_KEY_KP_0)
    {
        scene.toggleLanternLights();
        return true;
    }
    if (key == GLFW_KEY_M)
    {
        scene.replayMagicTrick();
        return true;
    }

    return false;
}

std::vector<ActionHint> InteractionManager::getActiveActions(const Camera& camera, const Scene& scene) const
{
    std::vector<ActionHint> result;

    // 1. Context actions for current selection
    if (currentSelected)
    {
        for (const auto& act : currentSelected->actions)
        {
            if (act.isEnabled && act.isEnabled())
            {
                result.push_back(act);
            }
        }

        // If selected object has transformable node, show compact transform hints
        if (currentSelected->node)
        {
            ActionHint tMove;
            tMove.keyName = "I/K,J/L,U/O";
            tMove.getLabel = []() { return "Move Object Live (+-Y, +-X, +-Z)"; };
            result.push_back(tMove);

            ActionHint tRot;
            tRot.keyName = "Arrows";
            tRot.getLabel = []() { return "Rotate Pitch / Yaw"; };
            result.push_back(tRot);
        }
    }

    // 2. Add top global actions if slot count < 4
    if (result.size() < 4)
    {
        ActionHint hPause;
        hPause.key = GLFW_KEY_SPACE;
        hPause.keyName = "Space";
        hPause.getLabel = [&scene]() {
            return scene.isPaused ? "Resume Animations" : "Pause Animations";
        };
        result.push_back(hPause);
    }

    if (result.size() < 5)
    {
        ActionHint hNight;
        hNight.key = GLFW_KEY_N;
        hNight.keyName = "N";
        hNight.getLabel = [&scene]() {
            return scene.targetNight ? "Switch to Day" : "Switch to Night";
        };
        result.push_back(hNight);
    }

    if (result.size() < 5)
    {
        ActionHint hFire;
        hFire.key = GLFW_KEY_F;
        hFire.keyName = "F";
        hFire.getLabel = []() { return "Launch Firework"; };
        result.push_back(hFire);
    }

    return result;
}
