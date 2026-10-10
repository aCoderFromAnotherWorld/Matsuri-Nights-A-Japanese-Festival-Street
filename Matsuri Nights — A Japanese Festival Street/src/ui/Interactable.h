#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <glm/glm.hpp>
#include "../SceneNode.h"

// Context-sensitive action hint associated with an interactable or global scene feature
struct ActionHint
{
    int key = 0;                               // GLFW key code (e.g., GLFW_KEY_H)
    std::string keyName;                       // Human-readable key label (e.g., "H")
    std::function<std::string()> getLabel;     // Action description callback (e.g., "Open Shoji door")
    std::function<bool()> isEnabled;           // Validity condition callback
    std::function<void()> execute;             // Action execution callback
};

// Data-driven interactable entity representing inspectable/controllable objects
struct Interactable
{
    std::string id;
    std::string displayName;
    std::string category;                      // "Building", "Stall", "Stage", "Lantern", "Shrine", etc.

    // Spatial queries
    std::function<glm::vec3()> getWorldPosition;
    float interactionRadius = 8.0f;

    // Live status readout for HUD
    std::function<std::string()> getStatusString;

    // Compact transform queries (position, rotation, scale)
    std::function<void(glm::vec3& pos, glm::vec3& rot, glm::vec3& scale)> getTransform;

    // Optional lighting status readout (type, position, color/intensity)
    std::function<std::string()> getLightInfo;

    // Context-sensitive action hints
    std::vector<ActionHint> actions;

    // Optional link to underlying SceneNode & inspectable index
    std::shared_ptr<SceneNode> node = nullptr;
    int inspectableIndex = -1;
    bool canManualSelect = true;
};
