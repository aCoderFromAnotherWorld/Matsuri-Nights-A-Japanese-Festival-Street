#pragma once

#include "Interactable.h"
#include "../Camera.h"
#include <vector>
#include <string>
#include <memory>

class Scene; // Forward declaration

class InteractionManager
{
public:
    std::vector<Interactable> interactables;
    int manualLockedIndex = -1; // -1 = Automatic proximity & view-cone selection; >= 0 = Locked
    const Interactable* currentSelected = nullptr;
    float currentDistance = 0.0f;
    bool isManualLocked = false;

    InteractionManager() = default;

    // Registers all real scene objects, building doors/windows, stalls, tricks, and lighting
    void init(Scene& scene);

    // Evaluates view cone (dot >= 0.45) & interaction radius each frame
    void update(const Camera& camera, float dt, Scene& scene);

    // Manual cycle through interactables (synchronized with Scene::inspectables)
    void cycleSelection(int dir, Scene& scene);
    void unlockToAuto();

    // Context-sensitive key trigger dispatcher
    bool handleKey(int key, Scene& scene, const Camera& camera);

    // Queries for HUD rendering
    const Interactable* getSelected() const { return currentSelected; }
    float getSelectedDistance() const { return currentDistance; }
    bool isLocked() const { return isManualLocked; }

    // Gathers 2-5 context-sensitive hints for currently selected object + relevant global hints
    std::vector<ActionHint> getActiveActions(const Camera& camera, const Scene& scene) const;
};
