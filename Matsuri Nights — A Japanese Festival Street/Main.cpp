#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "src/Shader.h"
#include "src/Camera.h"
#include "src/Scene.h"

#include <iostream>

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

// Global scene pointer for callbacks
Scene* g_Scene = nullptr;

int main()
{
    // 1. Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    // Print banner and controls in console
    std::cout << "\n========================================================================\n";
    std::cout << "  MATSURI NIGHTS - A JAPANESE FESTIVAL STREET\n";
    std::cout << "  CSE4102 Computer Graphics Project | Phase 1: Kinematics & Structure\n";
    std::cout << "========================================================================\n";
    std::cout << "  [W/A/S/D]       : Move camera forward / left / backward / right\n";
    std::cout << "  [E / Q]         : Move camera Up / Down\n";
    std::cout << "  [Mouse]         : Look around\n";
    std::cout << "  [C]             : Toggle mouse cursor capture\n";
    std::cout << "  [Space]         : Pause / Resume all scene animations\n";
    std::cout << "  [N]             : Smooth Day <-> Festival Night transition\n";
    std::cout << "  [F]             : Launch Firework rocket\n";
    std::cout << "  [1 / 2 / 3]     : Preset camera viewpoints (Street, Magic Stage, Torii)\n";
    std::cout << "  [T]             : Cycle object for Live In-Class Inspection & Transform\n";
    std::cout << "  [I/K, J/L, U/O] : Translate selected object (+-Y, +-X, +-Z)\n";
    std::cout << "  [Arrow Keys]    : Rotate selected object live\n";
    std::cout << "  [ [ / ] ]       : Scale selected object live down / up\n";
    std::cout << "  [R]             : Reset camera position\n";
    std::cout << "========================================================================\n\n";

    scene.printCurrentInspection();

    // 6. Main Render Loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process continuous movement keys
        processContinuousInput(window, scene);

        // Update all animations & hierarchy
        scene.update(deltaTime);

        // Background clear
        glClearColor(0.08f, 0.09f, 0.14f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render entire scene
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        float aspect = (height > 0) ? (float)width / (float)height : 1.0f;
        scene.render(basicShader, camera, aspect);

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

// Single press trigger events
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    if (key == GLFW_KEY_SPACE && g_Scene)
        g_Scene->togglePause();

    if (key == GLFW_KEY_N && g_Scene)
        g_Scene->toggleDayNight();

    if (key == GLFW_KEY_F && g_Scene)
        g_Scene->triggerFirework();

    if (key == GLFW_KEY_T && g_Scene)
        g_Scene->cycleInspectable(1);

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
