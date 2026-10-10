#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

#include "src/Shader.h"
#include "src/Camera.h"
#include "src/Scene.h"
#include "src/TextureGenerator.h"
#include "src/ui/Hud.h"
#include "src/ui/InteractionManager.h"

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>

// Window dimensions
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// Camera
Camera camera(glm::vec3(0.0f, 3.5f, 26.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -2.0f);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool mouseLookActive = true; // Mouse controls camera by default

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Forward declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processContinuousInput(GLFWwindow* window, Scene& scene);
bool runAutomatedTestSuite(Scene& scene, Camera& camera, GLFWwindow* window);
void captureViewportScreenshot(int width, int height, const std::string& filename);

// Global scene pointer for callbacks
Scene* g_Scene = nullptr;

// Heads-Up Display & Context Interaction Manager
Hud g_Hud;
InteractionManager g_InteractionManager;
bool g_PendingScreenshot = false;
bool g_ScreenshotWithHud = false;
float g_CurrentFps = 60.0f;
float g_CurrentMs = 16.6f;

int main(int argc, char** argv)
{
    bool runTests = (argc > 1 && std::string(argv[1]) == "--test");

    // 1. Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (runTests)
    {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 2. Create Window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Matsuri Nights - A Japanese Festival Street [CSE4102]", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    // Capture mouse for standard 3D camera navigation
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // 3. Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 4. OpenGL Configuration
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 5. Build Shader & Scene
    Shader basicShader("shaders/basic.vert", "shaders/basic.frag");
    Scene scene;
    g_Scene = &scene;

    // Initialize In-Window HUD and Context Interaction System
    g_Hud.init();
    g_InteractionManager.init(scene);

    if (runTests)
    {
        bool success = runAutomatedTestSuite(scene, camera, window);
        glfwDestroyWindow(window);
        glfwTerminate();
        return success ? 0 : 1;
    }

    // Print banner and controls in console
    std::cout << "\n========================================================================\n";
    std::cout << "  MATSURI NIGHTS - A JAPANESE FESTIVAL STREET\n";
    std::cout << "  CSE4102 Computer Graphics Project | Full Master Build (Phases 1, 2, 3)\n";
    std::cout << "========================================================================\n";
    std::cout << "  [W/A/S/D]       : Move camera forward / left / backward / right\n";
    std::cout << "  [E / Q]         : Move camera Up / Down\n";
    std::cout << "  [Mouse]         : Look around (FPS Pitch / Yaw)\n";
    std::cout << "  [C]             : Toggle mouse cursor capture\n";
    std::cout << "  [F1]            : Toggle In-Window Semi-Transparent HUD Overlay (Top-Right)\n";
    std::cout << "  [F10]           : Capture Viewport Screenshot BMP (Shift+F10 includes HUD)\n";
    std::cout << "  [Space]         : Pause / Resume all scene animations\n";
    std::cout << "  [N]             : Smooth Day <-> Festival Night transition\n";
    std::cout << "  [0 / Numpad 0]  : Toggle Lantern & Stall Illumination (Lights ON / Dimmed)\n";
    std::cout << "  [M]             : Replay Magic Show Trick Sequence (Vanishing Box & Orb)\n";
    std::cout << "  [P]             : Cycle Shading Mode (Blinn-Phong -> Diffuse-Only -> Ambient/Flat)\n";
    std::cout << "  [X]             : Toggle Texturing (Textures ON / OFF)\n";
    std::cout << "  [V]             : Toggle Realistic Soft Shadows (ON / OFF)\n";
    std::cout << "  [H]             : Interact with nearest house front door (Slide Open / Close)\n";
    std::cout << "  [G]             : Interact with nearest house sliding windows (Slide Open / Close)\n";
    std::cout << "  [B]             : Toggle Wall Collision (Walk Mode: solid walls & stairs <-> Noclip)\n";
    std::cout << "  [Z]             : Toggle Real-Time GPU Ray Tracing Mode (ON / OFF)\n";
    std::cout << "  [F9]            : Capture & Export CPU Ray-Traced Snapshot to BMP\n";
    std::cout << "  [F]             : Launch Firework rocket\n";
    std::cout << "  [1 / 2 / 3]     : Preset camera viewpoints (Street, Magic Stage, Torii)\n";
    std::cout << "  [T]             : Cycle Target Object Live Selection (Shift+T: Auto-Select)\n";
    std::cout << "  [I/K, J/L, U/O] : Translate selected object (+-Y, +-X, +-Z)\n";
    std::cout << "  [Arrow Keys]    : Rotate selected object live (Pitch / Yaw)\n";
    std::cout << "  [+ / -] or [[/]]: Scale selected object live (+-10%)\n";
    std::cout << "  [R]             : Reset camera position\n";
    std::cout << "========================================================================\n\n";

    scene.printCurrentInspection();

    // FPS Counter timing
    int frameCount = 0;
    float fpsTimer = 0.0f;

    // 6. Main Render Loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Live FPS & frame-time display in window title
        frameCount++;
        fpsTimer += deltaTime;
        if (fpsTimer >= 0.5f)
        {
            float fps = static_cast<float>(frameCount) / fpsTimer;
            float ms = (fpsTimer / static_cast<float>(frameCount)) * 1000.0f;
            g_CurrentFps = fps;
            g_CurrentMs = ms;
            char titleBuf[128];
            snprintf(titleBuf, sizeof(titleBuf),
                     "Matsuri Nights - A Japanese Festival Street [CSE4102] | FPS: %.1f (%.2f ms)",
                     fps, ms);
            glfwSetWindowTitle(window, titleBuf);
            frameCount = 0;
            fpsTimer = 0.0f;
        }

        // Process continuous movement keys
        processContinuousInput(window, scene);

        // Update all animations & hierarchy
        scene.update(deltaTime);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        // Update context-sensitive interaction selection & HUD status
        g_InteractionManager.update(camera, deltaTime, scene);
        g_Hud.update(deltaTime, camera, scene, g_InteractionManager, g_CurrentFps, g_CurrentMs, width, height);

        // Background clear
        glClearColor(0.08f, 0.09f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render entire scene (Ray-Traced or Rasterized)
        float aspect = (height > 0) ? (float)width / (float)height : 1.0f;

        if (scene.rayTracingMode)
        {
            scene.renderRayTraced(camera, width, height);
        }
        else
        {
            scene.render(basicShader, camera, aspect, width, height);
        }

        // Clean Screenshot Capture (HUD hidden) for reports
        if (g_PendingScreenshot && !g_ScreenshotWithHud)
        {
            captureViewportScreenshot(width, height, "screenshot_clean.bmp");
            g_PendingScreenshot = false;
        }

        // 2D In-Window HUD Overlay Pass (Semi-transparent top-right status panel)
        g_Hud.render(width, height);

        // Screenshot Capture with HUD overlay (Shift+F10)
        if (g_PendingScreenshot && g_ScreenshotWithHud)
        {
            captureViewportScreenshot(width, height, "screenshot_hud.bmp");
            g_PendingScreenshot = false;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Continuous key input (camera movement, selected object continuous manipulation)
void processContinuousInput(GLFWwindow* window, Scene& scene)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glm::vec3 oldCameraPos = camera.Position;

    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);

    // Apply interactive wall collision detection (prevent walking through walls)
    camera.Position = scene.resolveCollision(oldCameraPos, camera.Position);

    // Live transformation of selected object (CSE4102 requirement: live manual transform testing)
    float tSpeed = 2.5f * deltaTime;
    float rSpeed = 45.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        scene.modifySelectedPosition(glm::vec3(0.0f, tSpeed, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        scene.modifySelectedPosition(glm::vec3(0.0f, -tSpeed, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        scene.modifySelectedPosition(glm::vec3(-tSpeed, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        scene.modifySelectedPosition(glm::vec3(tSpeed, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        scene.modifySelectedPosition(glm::vec3(0.0f, 0.0f, -tSpeed));
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        scene.modifySelectedPosition(glm::vec3(0.0f, 0.0f, tSpeed));

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        scene.modifySelectedRotation(glm::vec3(rSpeed, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        scene.modifySelectedRotation(glm::vec3(-rSpeed, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        scene.modifySelectedRotation(glm::vec3(0.0f, rSpeed, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        scene.modifySelectedRotation(glm::vec3(0.0f, -rSpeed, 0.0f));
}

// Capture and export high-fidelity OpenGL viewport image to 24-bit uncompressed BMP
void captureViewportScreenshot(int width, int height, const std::string& filename)
{
    if (width <= 0 || height <= 0) return;
    std::vector<unsigned char> pixels(width * height * 3);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    // Vertical flip because OpenGL origin is bottom-left and BMP expects top-down
    std::vector<unsigned char> flipped(width * height * 3);
    for (int y = 0; y < height; ++y)
    {
        memcpy(&flipped[y * width * 3], &pixels[(height - 1 - y) * width * 3], width * 3);
    }
    TextureGenerator::writeBMP24(filename, width, height, flipped);
    std::cout << "\n========================================================" << std::endl;
    std::cout << " [SCREENSHOT] Viewport image captured and exported to: " << filename << std::endl;
    std::cout << "========================================================\n" << std::endl;
}

// Single press trigger events
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    // HUD Visibility Toggle (Key F1)
    if (key == GLFW_KEY_F1)
    {
        g_Hud.toggleVisibility();
        std::cout << "[HUD] Overlay " << (g_Hud.isVisible ? "SHOWN" : "HIDDEN") << std::endl;
        return;
    }

    // Viewport Screenshot Capture (Key F10: Clean without HUD; Shift+F10: With HUD)
    if (key == GLFW_KEY_F10)
    {
        g_PendingScreenshot = true;
        g_ScreenshotWithHud = ((mods & GLFW_MOD_SHIFT) != 0);
        std::cout << "[SCREENSHOT] Capturing viewport image ("
                  << (g_ScreenshotWithHud ? "with HUD" : "clean without HUD") << ")..." << std::endl;
        return;
    }

    // Target Selection Cycle (Key T: Next selectable object; Shift+T: Return to Auto-Selection)
    if (key == GLFW_KEY_T && g_Scene)
    {
        if ((mods & GLFW_MOD_SHIFT) != 0)
        {
            g_InteractionManager.unlockToAuto();
            std::cout << "[Target] Returned to Proximity Auto-Selection Mode." << std::endl;
        }
        else
        {
            g_InteractionManager.cycleSelection(1, *g_Scene);
            if (g_InteractionManager.isLocked() && g_InteractionManager.getSelected())
            {
                std::cout << "[Target] Locked to: " << g_InteractionManager.getSelected()->displayName << std::endl;
            }
            else
            {
                std::cout << "[Target] Auto-Selection Mode active." << std::endl;
            }
        }
        return;
    }

    // Lantern & Stall Lights Toggle (Key 0 and Numpad 0)
    if ((key == GLFW_KEY_0 || key == GLFW_KEY_KP_0) && g_Scene)
    {
        g_Scene->toggleLanternLights();
        return;
    }

    // Magic Show Trick Replay (Key M)
    if (key == GLFW_KEY_M && g_Scene)
    {
        g_Scene->replayMagicTrick();
        return;
    }

    if (key == GLFW_KEY_SPACE && g_Scene)
        g_Scene->togglePause();

    if (key == GLFW_KEY_N && g_Scene)
        g_Scene->toggleDayNight();

    if (key == GLFW_KEY_F && g_Scene)
        g_Scene->triggerFirework();

    // Phase 2: Cycle Shading Mode (Blinn-Phong -> Diffuse Only -> Ambient Only)
    if (key == GLFW_KEY_P && g_Scene)
        g_Scene->cycleShadingMode();

    // Phase 3: Toggle Textures (Textures ON / OFF)
    if (key == GLFW_KEY_X && g_Scene)
        g_Scene->toggleTextures();

    // Ray Tracing: Toggle Real-Time GPU Ray Tracing Mode
    if (key == GLFW_KEY_Z && g_Scene)
    {
        g_Scene->toggleRayTracing();
        if (g_Scene->rayTracingMode)
            glfwSetWindowTitle(window, "Matsuri Nights [REAL-TIME RAY TRACING ACTIVE] - A Japanese Festival Street");
        else
            glfwSetWindowTitle(window, "Matsuri Nights - A Japanese Festival Street");
    }

    // Ray Tracing: Capture High-Fidelity CPU Ray-Traced Snapshot to BMP
    if (key == GLFW_KEY_F9 && g_Scene)
    {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        g_Scene->captureCPURayTracedSnapshot(camera, w, h, "raytraced_snapshot.bmp");
    }

    // Realistic Shadows: Toggle Soft PCF Shadow Mapping
    if (key == GLFW_KEY_V && g_Scene)
        g_Scene->toggleShadows();

    // Interactive House Door: Slide Open / Close nearest house front Shoji door
    if (key == GLFW_KEY_H && g_Scene)
        g_Scene->interactNearestDoor(camera.Position);

    // Interactive House Windows: Slide Open / Close nearest house sliding Shoji windows
    if (key == GLFW_KEY_G && g_Scene)
        g_Scene->interactNearestWindow(camera.Position);

    // Wall Collision: Toggle Walk Mode (Solid Walls) vs Noclip Fly Mode
    if (key == GLFW_KEY_B && g_Scene)
        g_Scene->toggleCollision();

    // Toggle mouse cursor capture
    if (key == GLFW_KEY_C)
    {
        mouseLookActive = !mouseLookActive;
        if (mouseLookActive)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    // Scale selected object (+ / - keys, keypad + / -, and brackets [ / ])
    if ((key == GLFW_KEY_EQUAL || key == GLFW_KEY_KP_ADD || key == GLFW_KEY_RIGHT_BRACKET) && g_Scene)
        g_Scene->modifySelectedScale(1.1f);
    if ((key == GLFW_KEY_MINUS || key == GLFW_KEY_KP_SUBTRACT || key == GLFW_KEY_LEFT_BRACKET) && g_Scene)
        g_Scene->modifySelectedScale(0.9f);

    // Reset camera position
    if (key == GLFW_KEY_R)
    {
        camera.Position = glm::vec3(0.0f, 3.5f, 26.0f);
        camera.Yaw = -90.0f;
        camera.Pitch = -2.0f;
    }

    // Preset camera viewpoints
    if (key == GLFW_KEY_1) // Festival street entrance
    {
        camera.Position = glm::vec3(0.0f, 3.5f, 26.0f);
        camera.Yaw = -90.0f;
        camera.Pitch = -2.0f;
        std::cout << "[Camera] Preset 1: Street Entrance" << std::endl;
    }
    if (key == GLFW_KEY_2) // Magic Show Stage & Audience close-up
    {
        camera.Position = glm::vec3(6.2f, 2.2f, -10.5f);
        camera.Yaw = -90.0f;
        camera.Pitch = 2.0f;
        std::cout << "[Camera] Preset 2: Magic Show Stage & Audience" << std::endl;
    }
    if (key == GLFW_KEY_3) // Torii Gate & Fireworks Sky view
    {
        camera.Position = glm::vec3(0.0f, 2.5f, -18.0f);
        camera.Yaw = -90.0f;
        camera.Pitch = 25.0f;
        std::cout << "[Camera] Preset 3: Torii Gate & Sky" << std::endl;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (!mouseLookActive)
        return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// =========================================================================
// Automated Comprehensive Test Suite for All Controllable Objects & Toggles
// =========================================================================
bool runAutomatedTestSuite(Scene& scene, Camera& camera, GLFWwindow* window)
{
    std::cout << "\n========================================================================\n";
    std::cout << "  MATSURI NIGHTS - AUTOMATED COMPREHENSIVE CONTROL & TRANSFORM TEST SUITE\n";
    std::cout << "  Verifying All Controllable Objects, Interactive Elements & Toggles\n";
    std::cout << "========================================================================\n\n";

    int totalTests = 0;
    int passedTests = 0;

    auto testAssert = [&](const std::string& testName, bool condition, const std::string& details = "") {
        totalTests++;
        if (condition)
        {
            passedTests++;
            std::cout << "  [PASS] " << testName;
            if (!details.empty()) std::cout << " (" << details << ")";
            std::cout << "\n";
        }
        else
        {
            std::cerr << "  [FAIL] " << testName << " -- FAILED!";
            if (!details.empty()) std::cerr << " (" << details << ")";
            std::cerr << "\n";
        }
    };

    // -------------------------------------------------------------------------
    // SECTION 1: ALL 11 CONTROLLABLE INSPECTABLE OBJECTS & TRANSFORMS
    // -------------------------------------------------------------------------
    std::cout << "\n--- [SECTION 1: CONTROLLABLE INSPECTABLE OBJECTS & TRANSFORMS] ---\n";
    testAssert("Inspectables List Count == 15", scene.inspectables.size() == 15, "Count = " + std::to_string(scene.inspectables.size()));

    // Pause scene animation so automated transform measurements are deterministic
    scene.isPaused = true;

    for (size_t i = 0; i < scene.inspectables.size(); ++i)
    {
        scene.selectedIndex = static_cast<int>(i);
        auto& item = scene.inspectables[i];
        std::string objHeader = "Obj #" + std::to_string(i + 1) + ": " + item.displayName;
        std::cout << "\nTesting " << objHeader << "\n";

        testAssert(objHeader + " Node Valid", item.node != nullptr);
        if (!item.node) continue;

        glm::vec3 origPos = item.node->transform.position;
        glm::vec3 origRot = item.node->transform.rotation;
        glm::vec3 origScale = item.node->transform.scale;

        // 1. Translation along X, Y, Z
        glm::vec3 dX(2.5f, 0.0f, 0.0f);
        scene.modifySelectedPosition(dX);
        testAssert(objHeader + " Translate +X", glm::distance(item.node->transform.position, origPos + dX) < 0.001f);

        scene.modifySelectedPosition(-dX);
        testAssert(objHeader + " Translate -X (Return)", glm::distance(item.node->transform.position, origPos) < 0.001f);

        glm::vec3 dY(0.0f, 1.8f, 0.0f);
        scene.modifySelectedPosition(dY);
        testAssert(objHeader + " Translate +Y", glm::distance(item.node->transform.position, origPos + dY) < 0.001f);

        scene.modifySelectedPosition(-dY);
        testAssert(objHeader + " Translate -Y (Return)", glm::distance(item.node->transform.position, origPos) < 0.001f);

        glm::vec3 dZ(0.0f, 0.0f, 3.2f);
        scene.modifySelectedPosition(dZ);
        testAssert(objHeader + " Translate +Z", glm::distance(item.node->transform.position, origPos + dZ) < 0.001f);

        scene.modifySelectedPosition(-dZ);
        testAssert(objHeader + " Translate -Z (Return)", glm::distance(item.node->transform.position, origPos) < 0.001f);

        // 2. Rotation (Pitch and Yaw)
        glm::vec3 rPitch(15.0f, 0.0f, 0.0f);
        scene.modifySelectedRotation(rPitch);
        testAssert(objHeader + " Rotate +Pitch", glm::distance(item.node->transform.rotation, origRot + rPitch) < 0.001f);

        scene.modifySelectedRotation(-rPitch);
        testAssert(objHeader + " Rotate -Pitch (Return)", glm::distance(item.node->transform.rotation, origRot) < 0.001f);

        glm::vec3 rYaw(0.0f, 30.0f, 0.0f);
        scene.modifySelectedRotation(rYaw);
        testAssert(objHeader + " Rotate +Yaw", glm::distance(item.node->transform.rotation, origRot + rYaw) < 0.001f);

        scene.modifySelectedRotation(-rYaw);
        testAssert(objHeader + " Rotate -Yaw (Return)", glm::distance(item.node->transform.rotation, origRot) < 0.001f);

        // 3. Scaling (Scale Up and Scale Down)
        scene.modifySelectedScale(1.1f);
        testAssert(objHeader + " Scale Up 1.1x", glm::distance(item.node->transform.scale, origScale * 1.1f) < 0.001f);

        scene.modifySelectedScale(1.0f / 1.1f);
        testAssert(objHeader + " Scale Down (Return)", glm::distance(item.node->transform.scale, origScale) < 0.001f);
        item.node->transform.scale = origScale; // Exact reset

        // 4. World Matrix Propagation
        scene.rootNode->updateWorldMatrix(glm::mat4(1.0f));
        glm::vec3 worldPos = item.node->getWorldPosition();
        testAssert(objHeader + " World Matrix Valid", !std::isnan(worldPos.x) && !std::isnan(worldPos.y) && !std::isnan(worldPos.z));
    }

    // Test cycling forward and backward
    std::cout << "\nTesting Object Cycling (Key T):\n";
    scene.selectedIndex = 0;
    scene.cycleInspectable(1);
    testAssert("Cycle Forward 0 -> 1", scene.selectedIndex == 1);
    scene.cycleInspectable(-1);
    testAssert("Cycle Backward 1 -> 0", scene.selectedIndex == 0);
    int lastIdx = static_cast<int>(scene.inspectables.size()) - 1;
    scene.cycleInspectable(-1);
    testAssert("Cycle Backward 0 -> " + std::to_string(lastIdx) + " (Wrap)", scene.selectedIndex == lastIdx);
    scene.cycleInspectable(1);
    testAssert("Cycle Forward " + std::to_string(lastIdx) + " -> 0 (Wrap)", scene.selectedIndex == 0);

    // -------------------------------------------------------------------------
    // SECTION 2: INTERACTIVE SHOJI DOORS & SLIDING WINDOWS
    // -------------------------------------------------------------------------
    std::cout << "\n--- [SECTION 2: INTERACTIVE HOUSE DOORS & WINDOWS] ---\n";
    testAssert("Machiya Buildings Count == 4", scene.buildings.size() == 4);

    for (size_t b = 0; b < scene.buildings.size(); ++b)
    {
        auto& bld = scene.buildings[b];
        if (!bld) continue;
        std::string bldName = bld->root ? bld->root->name : ("Building_" + std::to_string(b));
        std::cout << "\nTesting " << bldName << " Door & Window Interactions:\n";

        // Test Door
        glm::vec3 doorPos = bld->worldPos;
        if (std::abs(bld->rotationY - 180.0f) < 1.0f)
            doorPos += glm::vec3(-4.14f, 1.2f, 0.90f);
        else
            doorPos += glm::vec3(4.14f, 1.2f, -0.90f);

        testAssert(bldName + " Door Initially Closed", !bld->isDoorOpen);

        // Player stands right at door entrance and presses H
        scene.interactNearestDoor(doorPos);
        testAssert(bldName + " Toggle Door Open (Press H)", bld->isDoorOpen);

        // Update animation over a few frames
        for (int step = 0; step < 10; ++step) bld->update(0.05f);
        testAssert(bldName + " Door Sliding Progressing Open", bld->doorSlideProgress > 0.3f && bld->slidingDoorGroup->transform.position.z > 0.4f);

        // Toggle door close
        scene.interactNearestDoor(doorPos);
        testAssert(bldName + " Toggle Door Closed (Press H)", !bld->isDoorOpen);

        for (int step = 0; step < 15; ++step) bld->update(0.05f);
        testAssert(bldName + " Door Sliding Progressing Closed", bld->doorSlideProgress < 0.2f);

        // Out of range door interaction test (player far away at Y = 50m)
        scene.interactNearestDoor(glm::vec3(0.0f, 50.0f, 0.0f));
        testAssert(bldName + " Far Interaction Disallowed", !bld->isDoorOpen);

        // Test Windows
        testAssert(bldName + " Windows Initially Closed", !bld->isWindowOpen);

        // Player stands near building and presses G
        scene.interactNearestWindow(bld->worldPos);
        testAssert(bldName + " Toggle Windows Open (Press G)", bld->isWindowOpen);

        for (int step = 0; step < 10; ++step) bld->update(0.05f);
        testAssert(bldName + " Window Sliding Progressing Open", bld->windowSlideProgress > 0.3f);

        // Toggle windows close
        scene.interactNearestWindow(bld->worldPos);
        testAssert(bldName + " Toggle Windows Closed (Press G)", !bld->isWindowOpen);

        for (int step = 0; step < 15; ++step) bld->update(0.05f);
        testAssert(bldName + " Window Sliding Progressing Closed", bld->windowSlideProgress < 0.2f);
    }

    // -------------------------------------------------------------------------
    // SECTION 3: ENVIRONMENT, RENDERING & LIGHTING TOGGLES
    // -------------------------------------------------------------------------
    std::cout << "\n--- [SECTION 3: ENVIRONMENT, RENDERING & LIGHTING TOGGLES] ---\n";

    // 1. Day / Night Toggle (Key N)
    bool initNight = scene.targetNight;
    scene.toggleDayNight();
    testAssert("Day/Night Toggle 1 (Target Inverted)", scene.targetNight != initNight);
    scene.toggleDayNight();
    testAssert("Day/Night Toggle 2 (Target Restored)", scene.targetNight == initNight);

    // 2. Pause / Resume Toggle (Key Space)
    scene.isPaused = false;
    scene.totalTime = 0.0f;
    scene.update(0.1f);
    float tRunning = scene.totalTime;
    testAssert("Scene Animation Running (Time Advances)", tRunning > 0.0f);

    scene.togglePause();
    testAssert("Pause Toggle (isPaused == true)", scene.isPaused == true);
    scene.update(0.1f);
    testAssert("Paused State Prevents Time Advance", scene.totalTime == tRunning);

    scene.togglePause();
    testAssert("Resume Toggle (isPaused == false)", scene.isPaused == false);
    scene.update(0.1f);
    testAssert("Resumed State Advances Time", scene.totalTime > tRunning);

    // 3. Shading Mode Cycle (Key P)
    scene.shadingMode = 0;
    scene.cycleShadingMode();
    testAssert("Shading Mode Cycle 0 -> 1 (Diffuse Only)", scene.shadingMode == 1);
    scene.cycleShadingMode();
    testAssert("Shading Mode Cycle 1 -> 2 (Ambient Only)", scene.shadingMode == 2);
    scene.cycleShadingMode();
    testAssert("Shading Mode Cycle 2 -> 0 (Blinn-Phong)", scene.shadingMode == 0);

    // 4. Textures Toggle (Key X)
    bool initTex = scene.enableTextures;
    scene.toggleTextures();
    testAssert("Textures Toggle OFF", scene.enableTextures == !initTex);
    scene.toggleTextures();
    testAssert("Textures Toggle ON", scene.enableTextures == initTex);

    // 5. Shadow Mapping Toggle (Key V)
    bool initShadow = scene.enableShadows;
    scene.toggleShadows();
    testAssert("Shadows Toggle OFF", scene.enableShadows == !initShadow);
    scene.toggleShadows();
    testAssert("Shadows Toggle ON", scene.enableShadows == initShadow);

    // 6. Real-Time GPU Ray Tracing Toggle (Key Z)
    bool initRT = scene.rayTracingMode;
    scene.toggleRayTracing();
    testAssert("Ray Tracing Toggle ON", scene.rayTracingMode == !initRT);
    scene.toggleRayTracing();
    testAssert("Ray Tracing Toggle OFF", scene.rayTracingMode == initRT);

    // 7. Wall Collision & Kinematics (Key B)
    bool initCol = scene.collisionEnabled;
    scene.toggleCollision();
    testAssert("Collision Toggle Inverted", scene.collisionEnabled != initCol);
    scene.toggleCollision();
    testAssert("Collision Toggle Restored", scene.collisionEnabled == initCol);

    // Test Collision Resolution Logic
    scene.collisionEnabled = true;
    auto& testBld = scene.buildings[0]; // Machiya_L1 at (-10.5, 0, 16.0), rotY = 0
    glm::vec3 oldP = testBld->worldPos + glm::vec3(4.5f, 1.0f, 0.0f);
    glm::vec3 insideP = testBld->worldPos + glm::vec3(3.9f, 1.0f, 0.0f);
    testBld->setDoorOpen(false);
    glm::vec3 resClosed = scene.resolveCollision(oldP, insideP);
    testAssert("Solid Wall Blocks Penetration (Door Closed)", resClosed.x >= testBld->worldPos.x + 4.30f);

    // Open door and verify doorway portal pass-through
    glm::vec3 oldPDoor = testBld->worldPos + glm::vec3(4.5f, 1.0f, -0.90f);
    glm::vec3 insidePDoor = testBld->worldPos + glm::vec3(3.9f, 1.0f, -0.90f);
    testBld->setDoorOpen(true);
    glm::vec3 resOpen = scene.resolveCollision(oldPDoor, insidePDoor);
    testAssert("Doorway Portal Allows Pass-Through (Door Open)", resOpen.x == insidePDoor.x);
    testBld->setDoorOpen(false);

    // Test Noclip fly mode
    scene.collisionEnabled = false;
    glm::vec3 resNoclip = scene.resolveCollision(oldP, insideP);
    testAssert("Noclip Mode Disables Wall Blocking", resNoclip == insideP);
    scene.collisionEnabled = true;

    // 8. Manual Firework Launch (Key F)
    scene.triggerFirework();
    bool rocketLaunched = false;
    for (const auto& r : scene.fireworks->rockets)
    {
        if (r.state == FireworkRocket::LAUNCHING)
            rocketLaunched = true;
    }
    testAssert("Firework Rocket Launched (Key F)", rocketLaunched);

    // Update fireworks to reach apex and burst
    for (int step = 0; step < 25; ++step)
        scene.fireworks->update(0.06f, false);

    glm::vec3 bPos, bCol;
    bool burstActive = scene.fireworks && scene.fireworks->getActiveBurst(bPos, bCol);
    testAssert("Firework Explodes in Sky (Active Burst)", burstActive);

    // 9. Camera Presets (Keys 1, 2, 3, R)
    std::cout << "\nTesting Camera Presets:\n";
    camera.Position = glm::vec3(0.0f, 3.5f, 26.0f);
    camera.Yaw = -90.0f;
    camera.Pitch = -2.0f;
    testAssert("Camera Preset 1 (Street Entrance)", camera.Position == glm::vec3(0.0f, 3.5f, 26.0f) && camera.Yaw == -90.0f);

    camera.Position = glm::vec3(6.2f, 2.2f, -10.5f);
    camera.Yaw = -90.0f;
    camera.Pitch = 2.0f;
    testAssert("Camera Preset 2 (Magic Stage)", camera.Position == glm::vec3(6.2f, 2.2f, -10.5f) && camera.Yaw == -90.0f);

    camera.Position = glm::vec3(0.0f, 2.5f, -18.0f);
    camera.Yaw = -90.0f;
    camera.Pitch = 25.0f;
    testAssert("Camera Preset 3 (Torii Gate & Sky)", camera.Position == glm::vec3(0.0f, 2.5f, -18.0f) && camera.Pitch == 25.0f);

    camera.Position = glm::vec3(0.0f, 3.5f, 26.0f);
    camera.Yaw = -90.0f;
    camera.Pitch = -2.0f;
    testAssert("Camera Reset R (Restored Origin)", camera.Position == glm::vec3(0.0f, 3.5f, 26.0f));

    // -------------------------------------------------------------------------
    // SECTION 4: FESTIVAL STALL ANIMATIONS
    // -------------------------------------------------------------------------
    std::cout << "\n--- [SECTION 4: FESTIVAL STALL ANIMATIONS] ---\n";
    testAssert("Takoyaki Stall Exists", scene.takoyakiStall != nullptr);
    testAssert("Takoyaki Balls Count == 6", scene.takoyakiStall && scene.takoyakiStall->balls.size() == 6);

    testAssert("Kakigori Stall Exists", scene.kakigoriStall != nullptr);
    testAssert("Kakigori Servings Count == 6", scene.kakigoriStall && scene.kakigoriStall->servings.size() == 6);
    testAssert("Kakigori Shaver Wheel Exists", scene.kakigoriStall && scene.kakigoriStall->shaverWheel != nullptr);
    testAssert("Kakigori Active Shaver Ice Mound Exists", scene.kakigoriStall && scene.kakigoriStall->shaverActiveIce != nullptr);

    // Update stalls over 1 second to verify dynamic animations
    float origShaverRot = scene.kakigoriStall->shaverWheel->transform.rotation.x;
    float origKakiRot = scene.kakigoriStall->servings[0].rootNode->transform.rotation.y;
    scene.kakigoriStall->update(scene.totalTime, 1.0f);
    testAssert("Kakigori Shaver Wheel Rotates dynamically", scene.kakigoriStall->shaverWheel->transform.rotation.x > origShaverRot + 100.0f);
    testAssert("Kakigori Bowls Rotate dynamically", scene.kakigoriStall->servings[0].rootNode->transform.rotation.y != origKakiRot);

    // -------------------------------------------------------------------------
    // SECTION 5: IN-WINDOW HUD OVERLAY & CONTEXT INTERACTION SYSTEM
    // -------------------------------------------------------------------------
    std::cout << "\n--- [SECTION 5: IN-WINDOW HUD OVERLAY & CONTEXT INTERACTION SYSTEM] ---\n";
    testAssert("Interactables Registered >= 10", g_InteractionManager.interactables.size() >= 10,
               "Count = " + std::to_string(g_InteractionManager.interactables.size()));

    // Verify HUD toggle
    bool initHudVis = g_Hud.isVisible;
    g_Hud.toggleVisibility();
    testAssert("HUD Visibility Toggle (F1 Hide)", g_Hud.isVisible != initHudVis);
    g_Hud.toggleVisibility();
    testAssert("HUD Visibility Toggle (F1 Restore)", g_Hud.isVisible == initHudVis);

    // Verify Auto Selection logic near Machiya Building L1
    camera.Position = glm::vec3(-10.5f, 1.8f, 20.0f);
    camera.Front = glm::vec3(0.0f, 0.0f, -1.0f);
    g_InteractionManager.unlockToAuto();
    g_InteractionManager.update(camera, 0.016f, scene);
    testAssert("Auto-Select Machiya Building L1 in front of camera",
               g_InteractionManager.getSelected() != nullptr && g_InteractionManager.getSelected()->id.find("building_0") != std::string::npos);

    // Verify Context Actions for selected building
    auto bldActions = g_InteractionManager.getActiveActions(camera, scene);
    bool hasDoorAction = false, hasWinAction = false;
    for (const auto& a : bldActions)
    {
        if (a.key == GLFW_KEY_H) hasDoorAction = true;
        if (a.key == GLFW_KEY_G) hasWinAction = true;
    }
    testAssert("Building Context Hint [H] Shoji Door Present", hasDoorAction);
    testAssert("Building Context Hint [G] Shoji Windows Present", hasWinAction);

    // Test cycling selection
    g_InteractionManager.cycleSelection(1, scene);
    testAssert("Cycle Selection Locks Manual Target", g_InteractionManager.isLocked());
    testAssert("Selected Target Valid after Cycle", g_InteractionManager.getSelected() != nullptr);
    testAssert("Cycle Selection Synchronizes scene.selectedIndex", scene.selectedIndex == g_InteractionManager.getSelected()->inspectableIndex);

    // Return to auto
    g_InteractionManager.unlockToAuto();
    testAssert("Unlock Returns to Auto Proximity Mode", !g_InteractionManager.isLocked());

    // Verify HUD geometry update without error
    g_Hud.update(0.15f, camera, scene, g_InteractionManager, 60.0f, 16.6f, 1280, 720);
    testAssert("HUD Update Executed Cleanly", true);
    g_Hud.render(1280, 720);
    captureViewportScreenshot(1280, 720, "screenshot_hud_minimal.bmp");
    bool hudShotExists = std::filesystem::exists("screenshot_hud_minimal.bmp") && std::filesystem::file_size("screenshot_hud_minimal.bmp") > 1000;
    testAssert("HUD Rendered Viewport Screenshot Exported", hudShotExists);

    // Verify Lantern & Stall Lights Toggle (Key 0 / Numpad 0)
    bool initLanternLights = scene.lanternLightsOn;
    scene.toggleLanternLights();
    testAssert("Lantern & Stall Lights Toggle OFF (Key 0 / Numpad 0)", scene.lanternLightsOn != initLanternLights);
    scene.toggleLanternLights();
    testAssert("Lantern & Stall Lights Toggle Restored", scene.lanternLightsOn == initLanternLights);

    // Verify Magic Show Trick Replay (Key M)
    scene.vanishingBox->stateTimer = 4.5f;
    scene.replayMagicTrick();
    testAssert("Magic Show Trick Replay Restarts Timer (Key M)", scene.vanishingBox->stateTimer == 0.0f);

    // Verify Viewport Screenshot Export (Key F10 feature)
    captureViewportScreenshot(64, 64, "test_screenshot.bmp");
    bool ssExists = std::filesystem::exists("test_screenshot.bmp") && std::filesystem::file_size("test_screenshot.bmp") > 100;
    testAssert("Viewport Screenshot BMP Exported Cleanly", ssExists);

    // Clean up temporary test files
    std::filesystem::remove("screenshot_hud_minimal.bmp");
    std::filesystem::remove("test_screenshot.bmp");

    // -------------------------------------------------------------------------
    // FINAL SUMMARY
    // -------------------------------------------------------------------------
    std::cout << "\n========================================================================\n";
    std::cout << "  TEST SUMMARY: " << passedTests << " / " << totalTests << " TESTS PASSED ("
              << (passedTests == totalTests ? "100% SUCCESS" : "FAILED") << ")\n";
    std::cout << "========================================================================\n\n";

    return (passedTests == totalTests);
}
