# Matsuri Nights — Comprehensive Technical Documentation (`Details.md`)
### CSE4102: Computer Graphics and Image Processing Laboratory
**Department of Computer Science and Engineering, Khulna University of Engineering & Technology (KUET)**  
**Project Title:** Matsuri Nights — A Japanese Festival Street  
**Author / Student:** MD. Abu Hasanat Soykot | **Roll:** 2107100 | **Group:** B2  
**Implementation Standard:** C++17 / C++20 (`stdcpp20`), Modern OpenGL 3.3 Core Profile, GLFW 3.5.1, GLAD, GLM  

---

## Table of Contents
1. [Project Overview](#1-project-overview)
   - [1.1 Academic Purpose & Scene Summary](#11-academic-purpose--scene-summary)
   - [1.2 Course Requirements Satisfaction](#12-course-requirements-satisfaction)
   - [1.3 Development Phase Status & Code Evidence](#13-development-phase-status--code-evidence)
2. [Tech Stack, Dependencies and Build](#2-tech-stack-dependencies-and-build)
   - [2.1 Technology Stack & Third-Party Libraries](#21-technology-stack--third-party-libraries)
   - [2.2 Step-by-Step Build & Run Instructions](#22-step-by-step-build--run-instructions)
   - [2.3 GLFW Window & Context Configuration](#23-glfw-window--context-configuration)
3. [Repository Structure](#3-repository-structure)
   - [3.1 Annotated Directory Tree](#31-annotated-directory-tree)
   - [3.2 File-by-File Catalog](#32-file-by-file-catalog)
4. [Architecture](#4-architecture)
   - [4.1 Lifecycle & Frame Execution Pipeline](#41-lifecycle--frame-execution-pipeline)
   - [4.2 OpenGL State Machine Management](#42-opengl-state-machine-management)
   - [4.3 GLSL Shader Program Catalog](#43-glsl-shader-program-catalog)
   - [4.4 Architectural Flow Diagrams](#44-architectural-flow-diagrams)
5. [Geometry System](#5-geometry-system)
   - [5.1 Parametric Primitive Mesh Generators](#51-parametric-primitive-mesh-generators)
   - [5.2 Curved & Botanical Geometry Generators (`Curves.h`)](#52-curved--botanical-geometry-generators-curvesh)
   - [5.3 Mesh, VAO/VBO/EBO Wrapper & Draw Dispatch](#53-mesh-vaovboebo-wrapper--draw-dispatch)
   - [5.4 Hierarchical Composition Methodology](#54-hierarchical-composition-methodology)
6. [Transform System and Scene Graph](#6-transform-system-and-scene-graph)
   - [6.1 The `Transform` Class](#61-the-transform-class)
   - [6.2 The `SceneNode` Architecture](#62-the-scenenode-architecture)
   - [6.3 Complete Scene Graph Hierarchy](#63-complete-scene-graph-hierarchy)
   - [6.4 Motion Relative to Another Object's Reference Frame](#64-motion-relative-to-another-objects-reference-frame)
7. [Scene Contents](#7-scene-contents)
   - [7.1 Ground & Pavement](#71-ground--pavement)
   - [7.2 Machiya Townhouse Buildings (x4)](#72-machiya-townhouse-buildings-x4)
   - [7.3 Torii Shrine Gate](#73-torii-shrine-gate)
   - [7.4 Cherry Blossom Trees (x7) & Detached Falling Petals](#74-cherry-blossom-trees-x7--detached-falling-petals)
   - [7.5 Overhead Catenary Lantern Spans (x5) & Swinging Lanterns](#75-overhead-catenary-lantern-spans-x5--swinging-lanterns)
   - [7.6 Takoyaki Food Stall](#76-takoyaki-food-stall)
   - [7.7 Kakigori Shaved Ice Stall](#77-kakigori-shaved-ice-stall)
   - [7.8 Articulated Vendor Figures (x2)](#78-articulated-vendor-figures-x2)
   - [7.9 Magic Show Stage Platform](#79-magic-show-stage-platform)
   - [7.10 Magician Figure](#710-magician-figure)
   - [7.11 Floating Magic Orb (Trick 1)](#711-floating-magic-orb-trick-1)
   - [7.12 Vanishing Box & Silk Cloth (Trick 2)](#712-vanishing-box--silk-cloth-trick-2)
   - [7.13 Stage Tracking Spotlight Rig](#713-stage-tracking-spotlight-rig)
   - [7.14 Seated Audience Figures](#714-seated-audience-figures)
   - [7.15 Walking Crowd Pedestrians](#715-walking-crowd-pedestrians)
   - [7.16 Fireworks Rocket Particle System](#716-fireworks-rocket-particle-system)
   - [7.17 Sky Dome & Atmospheric Canopy](#717-sky-dome--atmospheric-canopy)
   - [7.18 Summary Table of Scene Objects](#718-summary-table-of-scene-objects)
8. [Animation and Motion Catalogue](#8-animation-and-motion-catalogue)
   - [8.1 Hanging Lantern Pendulum Swing](#81-hanging-lantern-pendulum-swing)
   - [8.2 Helical Magic Orb Trajectory](#82-helical-magic-orb-trajectory)
   - [8.3 Vanishing Box 6-Phase State Machine](#83-vanishing-box-6-phase-state-machine)
   - [8.4 Takoyaki Flipping & Hopping Dynamics](#84-takoyaki-flipping--hopping-dynamics)
   - [8.5 Kakigori Shaver Crank & Dessert Bowl Presentation](#85-kakigori-shaver-crank--dessert-bowl-presentation)
   - [8.6 Two-Stage Fireworks Launch & Explosive Burst Physics](#86-two-stage-fireworks-launch--explosive-burst-physics)
   - [8.7 Biomechanical Crowd Walk Cycles](#87-biomechanical-crowd-walk-cycles)
   - [8.8 Falling Sakura Blossom Wind Drift](#88-falling-sakura-blossom-wind-drift)
   - [8.9 Sliding Shoji Doors & Windows Kinematics](#89-sliding-shoji-doors--windows-kinematics)
   - [8.10 Motion Classification & Academic Rubric Matrix](#810-motion-classification--academic-rubric-matrix)
9. [Lighting, Shading and Illumination](#9-lighting-shading-and-illumination)
   - [9.1 Blinn-Phong Shading Pipeline](#91-blinn-phong-shading-pipeline)
   - [9.2 Live Shading Modes](#92-live-shading-modes)
   - [9.3 Complete Light Source Specification (14 Dynamic Point Lights & 16 Total Lights)](#93-complete-light-source-specification-14-dynamic-point-lights--16-total-lights)
   - [9.4 Day/Night Solar & Lunar Blend Machine](#94-daynight-solar--lunar-blend-machine)
   - [9.5 16-Sample PCF Soft Shadow Mapping](#95-16-sample-pcf-soft-shadow-mapping)
   - [9.6 Comprehensive Materials Matrix](#96-comprehensive-materials-matrix)
10. [Texturing](#10-texturing)
    - [10.1 Texture Management & Procedural Fallbacks](#101-texture-management--procedural-fallbacks)
    - [10.2 UV Mapping Parametrization](#102-uv-mapping-parametrization)
    - [10.3 Active Texture Assets Table](#103-active-texture-assets-table)
    - [10.4 Texture & Material Modulation](#104-texture--material-modulation)
11. [Camera, Input and Interaction](#11-camera-input-and-interaction)
    - [11.1 FPS Fly/Walk Camera System](#111-fps-flywalk-camera-system)
    - [11.2 Comprehensive Controls Keymap](#112-comprehensive-controls-keymap)
    - [11.3 In-Window Minimalist HUD System](#113-in-window-minimalist-hud-system)
    - [11.4 Context-Sensitive Interaction & Object Selection](#114-context-sensitive-interaction--object-selection)
    - [11.5 Continuous Collision Detection & Portal Navigation](#115-continuous-collision-detection--portal-navigation)
    - [11.6 Viewport Screenshot Engine](#116-viewport-screenshot-engine)
12. [Advanced Optics: Real-Time GPU Ray Tracer & CPU Snapshot](#12-advanced-optics-real-time-gpu-ray-tracer--cpu-snapshot)
    - [12.1 Real-Time GPU Whitted Ray Tracing Pipeline](#121-real-time-gpu-whitted-ray-tracing-pipeline)
    - [12.2 Multi-Threaded CPU Ray Tracing Snapshot Generator](#122-multi-threaded-cpu-ray-tracing-snapshot-generator)
13. [Live Demonstration Guide](#13-live-demonstration-guide)
    - [13.1 Step-by-Step Examiner Evaluation Walkthrough](#131-step-by-step-examiner-evaluation-walkthrough)
    - [13.2 Rapid Code Customization Cheatsheet](#132-rapid-code-customization-cheatsheet)
14. [Plan vs Implementation](#14-plan-vs-implementation)
    - [14.1 Features Implemented Beyond Original Plan](#141-features-implemented-beyond-original-plan)
    - [14.2 Plan Divergences & Code Harmonization](#142-plan-divergences--code-harmonization)
15. [Performance, Limits and Known Edge Cases](#15-performance-limits-and-known-edge-cases)
    - [15.1 Architectural Bounds & Constant Limits](#151-architectural-bounds--constant-limits)
    - [15.2 Working Directory & Resource Resolution Strategy](#152-working-directory--resource-resolution-strategy)
16. [Extension Guide](#16-extension-guide)
    - [16.1 Adding a New Geometric Primitive](#161-adding-a-new-geometric-primitive)
    - [16.2 Adding a New Scene Object](#162-adding-a-new-scene-object)
    - [16.3 Adding a Dynamic Light Source](#163-adding-a-dynamic-light-source)
    - [16.4 Adding an Interactable & Custom Key Action](#164-adding-an-interactable--custom-key-action)
17. [Glossary and Quick Reference](#17-glossary-and-quick-reference)

---

## 1. Project Overview

### 1.1 Academic Purpose & Scene Summary
**Matsuri Nights — A Japanese Festival Street** is an interactive, real-time 3D computer graphics simulation engineered for **CSE4102: Computer Graphics and Image Processing Laboratory** at Khulna University of Engineering & Technology (KUET). The application is built from first principles in C++17/C++20 using the modern OpenGL Core Profile (3.3+), GLFW, GLAD, and GLM.

The project simulates a traditional Japanese summer evening festival (*matsuri*). A central stone-paved promenade is enclosed by four fully furnished, two-story wooden townhouses (*machiya*), illuminated by five spans of sagging catenary ropes holding swaying paper lanterns (*chochin*), terminating at a monumental vermilion shrine gate (*torii*). Key focal areas include a cherry blossom tree (*sakura*) with wind-drifting petals, two functional food stalls (*takoyaki* and *kakigori*), an elevated magic show stage featuring an articulated magician, floating magic orb, vanishing trick box, and seated spectators, walking pedestrians, and an atmospheric sky dome illuminated by procedural day/night celestial bodies and launching fireworks rockets.

```
========================================================================================
                                     [Torii Gate]
                                    (Z = -32.0m)
                                         |
     [Machiya_L2]                  [Sakura Tree]                   [Machiya_R2]
  (Z = -6.0, X = -10.5)         (Z = -22.0, X = -10.2)          (Z = -6.0, X = +10.5)
           \                             |                               /
            \--- [Street Lantern Span 4 (Z = -9.0m, Swinging Point Light #4)] --/
                 [Street Lantern Span 5 (Z = -24.5m)]
                                         |
    [Takoyaki Food Stall]                |               [Magic Show Stage & Tricks]
     (Amber Point Light #1)              |                  (Spotlight, Orb Light #0)
     (Z = +6.0, X = -5.2)                |                   (Z = -19.0, X = +6.2)
           \                             |                               /
            \--- [Street Lantern Span 3 (Z = 0.0m)] --------------------/
                 [Street Lantern Span 2 (Z = +11.0m, Swinging Point Light #3)]
           /                             |                               \
    [Kakigori Food Stall]                |                      [Audience Semicircle]
      (Cyan Point Light #2)              |                      (Z = -13.5, X = +6.2)
      (Z = +6.0, X = +5.2)               |
                                         |
     [Machiya_L1]             [Walking Pedestrians]                [Machiya_R1]
  (Z = +16.0, X = -10.5)     (Z in [-30.0m, +36.0m])            (Z = +16.0, X = +10.5)
           \                             |                               /
            \--- [Street Lantern Span 1 (Z = +22.0m)] ------------------/
                                         |
                             [Festival Street Entrance]
                             (Camera Preset 1: Z = 26.0m)
========================================================================================
```

### 1.2 Course Requirements Satisfaction
The application is structured to systematically satisfy and exceed every requirement mandated in the CSE4102 curriculum:

1. **Demonstrate Every 3D Model Transformation Type:**
   - **Translation:** Continuous pedestrian locomotion, firework rocket ascent, falling sakura petals, and interactive sliding Shoji doors.
   - **Rotation:** Oscillating pendulum lantern ropes, spinning takoyaki spheres, rotating shaved ice crank wheel, pivoting stage spotlight, and character joint articulation.
   - **Scaling:** Discrete scale-to-zero disappearance and restoration of the magic stage box, exploding firework particle contraction, and live $\pm 10\%$ debug scaling.
   - **Hierarchical/Combined Transforms:** Compound limbs where shoulder, elbow, and wrist transformations concatenate into a single world matrix chain.

2. **Transformations Relative to Another Object's Reference Frame:**
   - **Swinging Lantern Bodies:** The lantern body is a child node of a swinging rope anchor; its world position and orientation update strictly relative to the rope pivot's local rotation.
   - **Floating Magic Orb:** Child node of the magician's articulated hand bone; executes a 3D helical orbit parameterized in hand-local coordinates.
   - **Stage Spotlight Housing:** Conical spotlight housing rotates to track the magician, rotating the dynamic spotlight's world position and cone direction vector.
   - **Sliding Shoji Doors & Windows:** Child nodes of the townhouse building frame; slide open and closed along local axes relative to the building's world orientation.

3. **Complex Moving Objects (Exceeding the Minimum of 5):**
   - The project implements **17 distinct composite objects** (see Section 7), of which 11 are dynamic, articulated, or particle-driven moving objects.

4. **Lighting, Shading, and Color Modulation:**
   - Evaluates Blinn-Phong per-fragment reflection, diffuse Lambertian shading, and ambient baselines switchable live via <kbd>P</kbd>.
   - Features 16 dynamic lights: 1 directional light, 14 point lights, and 1 cone spotlight.

5. **Moving Light Sources & Illumination Feedback:**
   - Point Light #0 tracks the orbiting magic orb.
   - Point Lights #3 & #4 track the oscillating bodies of swinging street lanterns.
   - Point Light #5 flashes dynamically at firework detonation points.
   - Point Lights #12 & #13 dynamically track the Torii shrine gate anchors.
   - The Stage Spotlight dynamically rotates its cutoff cone to follow the magician.

6. **Daylight and Nightlight Views:**
   - Pressing <kbd>N</kbd> smoothly interpolates directional sun/moon angles, sky dome gradients, starfield visibility, lantern intensities, and interior window glows over a 1.5-second transition.

### 1.3 Development Phase Status & Code Evidence
Every planned project phase is **100% complete and verified**:

| Development Phase | Status | Primary Code Evidence |
|---|:---:|---|
| **Phase 1: Geometry, Structure & Motion** | **DONE** | `Primitives.h` (Cube, Cylinder, Cone, Sphere, Plane, Swept Tubes), `Transform.h`, `SceneNode.h`, `Objects.h` (17 composite classes), and hierarchical kinematics in `Scene::update()`. |
| **Phase 2: Illumination, Shading & Colors** | **DONE** | `shaders/basic.vert`, `shaders/basic.frag`, `Light.h`, `Scene::initLighting()`, `Scene::updateLighting()`, 14 dynamic point lights (16 total lights), day/night lerp, and 3-mode shading switch (<kbd>P</kbd>). |
| **Phase 3: Diffuse Texturing & Emissive Maps** | **DONE** | `Texture.h`, `TextureGenerator.h`, procedural BMP generators, active diffuse texture units in `basic.frag`, emissive night glow maps on lanterns and Shoji windows, texture toggle (<kbd>X</kbd>). |
| **Advanced Extension: GPU Whitted Ray Tracing** | **DONE** | Full-screen quad GPU Whitted ray tracer in `shaders/raytrace.vert` & `shaders/raytrace.frag` (<kbd>Z</kbd>) + multi-threaded CPU snapshot exporter in `RayTracer.h` (<kbd>F9</kbd>). |
| **Advanced Extension: 16-Sample PCF Soft Shadows** | **DONE** | $2048 \times 2048$ depth framebuffer FBO in `Scene.h`, `shaders/shadow_depth.vert`, `shaders/shadow_depth.frag`, slope-scaled depth bias in `basic.frag` (<kbd>V</kbd>). |
| **Advanced Extension: Collision & Portal Physics** | **DONE** | Continuous AABB wall collision resolver in `Scene::resolveCollision()` (<kbd>B</kbd>) with doorway portal pass-through when Shoji doors are opened (<kbd>H</kbd>). |
| **Advanced Extension: In-Window Minimal HUD** | **DONE** | 2D orthographic UI pass in `src/ui/Hud.cpp`, embedded Consolas Bold atlas in `FontAtlasData.h`, contextual action telemetry in `InteractionManager.cpp` (<kbd>F1</kbd>). |

---

## 2. Tech Stack, Dependencies and Build

### 2.1 Technology Stack & Third-Party Libraries
The application is compiled natively for 64-bit Windows architectures:
- **Language Standard:** C++20 (`/std:c++20` configured in `.vcxproj`). Code adheres to clean modern C++ standards (`std::unique_ptr`, `std::shared_ptr`, lambdas, structured bindings, `std::clamp`).
- **Graphics API:** OpenGL 3.3 Core Profile. Forward-compatible pipeline utilizing VAOs, VBOs, EBOs, and GLSL `#version 330 core`.
- **GLFW 3.5.1:** Window creation, OpenGL context initialization, high-DPI framebuffer scaling, and input polling. Linked statically via `Libraries/lib/glfw3.lib`.
- **GLAD:** Multi-language OpenGL loader generator. Implemented directly in the compilation unit via `glad.c` and `Libraries/include/glad/glad.h`.
- **GLM (OpenGL Mathematics):** Header-only mathematics library located in `Libraries/include/glm/`. Provides vector (`glm::vec2`, `glm::vec3`, `glm::vec4`), matrix (`glm::mat3`, `glm::mat4`), and transformation functions (`glm::translate`, `glm::rotate`, `glm::scale`, `glm::lookAt`, `glm::perspective`, `glm::ortho`).
- **stb_image.h:** Single-header image decoder by Sean Barrett located in `Libraries/include/stb/stb_image.h`. Used to decode 24-bit uncompressed `.bmp` texture assets.
- **Embedded Consolas Atlas:** Custom $256 \times 256$ single-channel texture atlas defined in `src/ui/FontAtlasData.h`. Requires zero runtime external font libraries (no FreeType or ImGui dependencies).

### 2.2 Step-by-Step Build & Run Instructions

#### Option A: Building via Visual Studio 2022 / Community
1. Open the solution file located at:
   ```
   Matsuri Nights — A Japanese Festival Street/Matsuri Nights - A Japanese Festival Street.slnx
   ```
   *(or open the folder directly in Visual Studio).*
2. In the top toolbar, select the configuration:
   - **Configuration:** `Release` (or `Debug`)
   - **Platform:** `x64`
3. Verify project property include directories point to:
   - `Libraries\include`
   - Library path points to `Libraries\lib`
4. Build the solution using **Build $\to$ Build Solution** (<kbd>Ctrl+Shift+B</kbd>).
5. Launch the application using **Debug $\to$ Start Without Debugging** (<kbd>Ctrl+F5</kbd>) or **Start Debugging** (<kbd>F5</kbd>).

#### Option B: Automated Headless Testing via Command Line
Execute the precompiled binary with the `--test` flag:
```powershell
& ".\Matsuri Nights — A Japanese Festival Street\x64\Release\Matsuri Nights — A Japanese Festival Street.exe" --test
```
*Result:* Runs all 312 automated unit and integration tests (including BMP and PNG screenshot verification) and outputs an exhaustive report to stdout without displaying the GUI window.

#### Working Directory & Asset Requirements
The application resolves shader paths (`shaders/basic.vert`, etc.) and texture assets (`assets/textures/*.bmp`) relative to the executable working directory. If run from an alternate directory, `src/Shader.h` automatically engages embedded fallback GLSL source strings, and `src/TextureGenerator.h` automatically generates procedural BMP files to guarantee zero runtime crashes.

### 2.3 GLFW Window & Context Configuration
Configured in `Main.cpp: main()`:

```cpp
// Main.cpp: lines 61-76
glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

if (runTests)
{
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Headless for test suite
}

GLFWwindow* window = glfwCreateWindow(
    SCR_WIDTH, SCR_HEIGHT, 
    "Matsuri Nights - A Japanese Festival Street [CSE4102]", 
    NULL, NULL
);
```

- **Window Dimensions:** Default width $1280\text{ px}$, height $720\text{ px}$ (`SCR_WIDTH`, `SCR_HEIGHT`).
- **Cursor Mode:** Disabled by default (`glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED)`) for FPS-style mouse look. Pressing <kbd>C</kbd> toggles to `GLFW_CURSOR_NORMAL`.
- **Registered Callbacks:**
  - `glfwSetFramebufferSizeCallback(window, framebuffer_size_callback)`: Updates `glViewport(0, 0, width, height)`.
  - `glfwSetCursorPosCallback(window, mouse_callback)`: Computes mouse offset deltas and passes them to `camera.ProcessMouseMovement()`.
  - `glfwSetScrollCallback(window, scroll_callback)`: Adjusts camera field of view zoom ($1^\circ \le \text{Zoom} \le 60^\circ$).
  - `glfwSetKeyCallback(window, key_callback)`: Handles single-press discrete trigger events.

---

## 3. Repository Structure

### 3.1 Annotated Directory Tree
```
Matsuri-Nights-A-Japanese-Festival-Street/
│
├── .gitignore                                    # Git version control exclusions
├── Plan.md                                       # Master development plan & milestone status
├── Details.md                                    # Comprehensive technical documentation (this file)
├── README.md                                     # Student submission overview & course meta
├── controls.md                                   # Academic controls reference manual
├── color_changes.md                              # Color customization & material reflection guide
├── agy.md                                        # Chronological development changelog
├── run.bat                                       # Windows double-click launch script
│
├── assets/
│   └── textures/                                 # Shared root textures (24-bit BMPs)
│       ├── gold_leaf.bmp                         # Metallic folding screen texture (256x256)
│       ├── lantern_paper.bmp                     # Red washi paper texture with kanji (256x256)
│       ├── roof_tiles.bmp                        # Ceramic Japanese scalloped roof tiles (256x256)
│       ├── sakura_bark.bmp                       # Organic cherry tree trunk bark (256x256)
│       ├── stone_pavement.bmp                    # Cobblestone street paving pattern (256x256)
│       ├── takoyaki_food.bmp                     # Fried batter, sauce, and mayo texture (256x256)
│       ├── tatami_cloth.bmp                      # Woven straw tatami mats and noren cloth (256x256)
│       └── wood_timber.bmp                       # Cedar timber grain for machiya beams (256x256)
│
└── Matsuri Nights — A Japanese Festival Street/  # Primary Visual Studio project directory
    ├── Matsuri Nights - A Japanese Festival Street.slnx      # Solution file (VS 2022/2026 format)
    ├── Matsuri Nights — A Japanese Festival Street.vcxproj   # Visual C++ Project configuration
    ├── Matsuri Nights — A Japanese Festival Street.vcxproj.filters # Solution explorer filters
    ├── glad.c                                    # GLAD OpenGL 3.3 Core Profile loader
    ├── Main.cpp                                  # Main entry point, render loop, tests, input
    │
    ├── Libraries/                                # External headers and static libraries
    │   ├── include/                              # Third-party include files
    │   │   ├── glad/glad.h                       # OpenGL function declarations
    │   │   ├── GLFW/glfw3.h                      # GLFW windowing API
    │   │   ├── glm/...                           # GLM vector/matrix mathematics headers
    │   │   ├── KHR/khrplatform.h                 # Khronos platform abstraction types
    │   │   └── stb/stb_image.h                   # Single-header image loading library
    │   └── lib/
    │       └── glfw3.lib                         # GLFW 3.5.1 64-bit static import library
    │
    ├── shaders/                                  # Runtime GLSL shader programs
    │   ├── basic.vert                            # Core 3D geometry vertex shader
    │   ├── basic.frag                            # Core Blinn-Phong, shadows, texturing fragment shader
    │   ├── shadow_depth.vert                     # Directional shadow depth-pass vertex shader
    │   ├── shadow_depth.frag                     # Directional shadow depth-pass fragment shader
    │   ├── raytrace.vert                         # GPU ray tracing full-screen quad vertex shader
    │   ├── raytrace.frag                         # GPU Whitted analytical ray tracing fragment shader
    │   ├── hud.vert                              # 2D orthographic HUD vertex shader
    │   └── hud.frag                              # 2D orthographic HUD text & card fragment shader
    │
    └── src/                                      # Modular engine source headers & implementations
        ├── Camera.h                              # FPS fly/walk camera with presets and zoom
        ├── Curves.h                              # Bézier curves, splines, and swept geometry
        ├── Light.h                               # DirLight, PointLight, SpotLight data structures
        ├── Mesh.h                                # VAO/VBO/EBO wrapper and indexed draw call issuer
        ├── Objects.h                             # 17 composite scene objects and kinematics
        ├── Primitives.h                          # Cube, Cylinder, Cone, Sphere, Plane generators
        ├── RayTracer.h                           # Multi-threaded CPU Whitted ray tracing engine
        ├── Scene.h                               # Scene coordinator: lighting, shadows, physics
        ├── SceneNode.h                           # Hierarchical scene graph node with world matrices
        ├── Shader.h                              # GLSL compilation, linking, and uniform cache
        ├── Texture.h                             # OpenGL 2D texture wrapper using stb_image
        ├── TextureGenerator.h                    # Procedural BMP fallback texture synthesizer
        ├── Transform.h                           # Local 3D TRS transformation representation
        │
        └── ui/                                   # User interface and context interaction
            ├── FontAtlasData.h                   # Embedded 256x256 Consolas Bold bitmap atlas
            ├── Hud.h / Hud.cpp                   # Minimal 2D heads-up display overlay pass
            ├── Interactable.h                    # Data-driven interactable object definition
            └── InteractionManager.h / .cpp       # Context selection, view cone filter, key dispatch
```

### 3.2 File-by-File Catalog

| File Path | Primary Purpose | Key Classes / Functions | Dependencies |
|---|---|---|---|
| `Main.cpp` | Application entry point, render loop, input routing, screenshot exporter, and test harness. | `main()`, `processContinuousInput()`, `key_callback()`, `mouse_callback()`, `captureViewportScreenshot()`, `runAutomatedTestSuite()` | `glad.h`, `glfw3.h`, `stb_image.h`, `Shader.h`, `Camera.h`, `Scene.h`, `Hud.h`, `InteractionManager.h` |
| `glad.c` | Dynamically retrieves OpenGL function pointers from driver. | `gladLoadGLLoader()` | `glad.h`, `khrplatform.h` |
| `src/Transform.h` | Affine transformation representation (position, Euler rotation, scale). | `struct Transform`, `getLocalMatrix()`, `checkDirty()` | `glm/glm.hpp`, `glm/gtc/matrix_transform.hpp` |
| `src/SceneNode.h` | Hierarchical scene graph node managing parent-child relations and matrix cascades. | `class SceneNode`, `struct RenderContext`, `updateWorldMatrix()`, `draw()`, `drawDepth()` | `Transform.h`, `Mesh.h`, `Shader.h`, `Texture.h` |
| `src/Mesh.h` | OpenGL mesh encapsulation wrapping VAO, VBO, and EBO buffers. | `struct Vertex`, `class Mesh`, `setupMesh()`, `Draw()`, `ResetBoundVAO()` | `glad.h`, `glm/glm.hpp` |
| `src/Primitives.h` | Parametric base mesh generators. | `Primitives::createCube()`, `createCylinder()`, `createCone()`, `createSphere()`, `createPlane()` | `Mesh.h`, `Curves.h`, `glm/glm.hpp` |
| `src/Curves.h` | Parametric curves, Bishop frames, and swept 3D tubes/beams. | `struct Bezier2`, `struct Bezier3`, `struct CatmullRomSpline`, `createSweptTube()`, `createCurvedBeam()`, `createCatenaryRope()` | `Mesh.h`, `glm/glm.hpp` |
| `src/Light.h` | Pod structs defining lighting parameters. | `struct DirLight`, `struct PointLight`, `struct SpotLight`, `struct Material` | `glm/glm.hpp` |
| `src/Shader.h` | GLSL program loader, compiler, linker, and cached uniform setter. | `class Shader`, `use()`, `getUniformLocation()`, `setMat4()`, `setVec3()`, `setFloat()` | `glad.h`, `glm/glm.hpp` |
| `src/Camera.h` | FPS 6-DOF fly/walk camera. | `class Camera`, `GetViewMatrix()`, `GetProjectionMatrix()`, `ProcessKeyboard()`, `ProcessMouseMovement()`, `ProcessMouseScroll()` | `glad.h`, `glm/glm.hpp` |
| `src/Texture.h` | Texture2D abstraction loading images via `stb_image.h`. | `class Texture`, `loadFromFile()`, `loadFromMemory()`, `bind()`, `unbind()` | `glad.h`, `stb_image.h` |
| `src/TextureGenerator.h` | Procedural 24-bit uncompressed BMP synthesizer and writer. | `TextureGenerator::writeBMP24()`, `generateWoodTimber()`, `generateRoofTiles()`, `ensureTextureAssetsExist()` | Standard C++ I/O |
| `src/Objects.h` | Assembles the 17 composite objects from primitives and implements local kinematics. | `SceneMeshes`, `GroundObject`, `MachiyaBuilding`, `ToriiGate`, `SakuraTree`, `LanternObject`, `StreetLanternSpan`, `TakoyakiStall`, `KakigoriStall`, `VendorFigure`, `MagicStage`, `Magician`, `VanishingBoxTrick`, `SpotlightRig`, `AudienceGroup`, `CrowdGroup`, `FireworkSystem`, `SkyDome` | `SceneNode.h`, `Primitives.h`, `Curves.h` |
| `src/RayTracer.h` | Multi-threaded CPU analytical Whitted ray tracing engine. | `CPU_RayTracer::renderSnapshot()`, `traceScene()`, `intersectSphere()`, `intersectBox()`, `intersectCylinderY()` | `Camera.h`, `TextureGenerator.h`, `<thread>` |
| `src/Scene.h` | Master scene coordinator managing objects, lighting, shadows, ray tracing, and collision. | `class Scene`, `buildScene()`, `update()`, `render()`, `renderShadowDepth()`, `renderRayTraced()`, `resolveCollision()`, `interactNearestDoor()` | `SceneNode.h`, `Objects.h`, `Shader.h`, `Camera.h`, `Light.h`, `RayTracer.h` |
| `src/ui/FontAtlasData.h` | Raw byte array containing embedded $256 \times 256$ Consolas Bold font atlas. | `FONT_ATLAS_WIDTH`, `FONT_ATLAS_HEIGHT`, `g_HudFontAtlasBytes[]` | None |
| `src/ui/Hud.h` / `.cpp` | 2D orthographic heads-up display overlay pass. | `class Hud`, `init()`, `update()`, `render()`, `buildGeometry()`, `uploadBuffers()` | `glad.h`, `FontAtlasData.h`, `Camera.h`, `Scene.h`, `Interactable.h` |
| `src/ui/Interactable.h` | Interface record defining interactive scene entities. | `struct ActionHint`, `struct Interactable` | `SceneNode.h`, `glm/glm.hpp` |
| `src/ui/InteractionManager.h` / `.cpp` | Evaluates view cones, manages selection cycles, and routes action keys. | `class InteractionManager`, `init()`, `update()`, `cycleSelection()`, `unlockToAuto()`, `handleKey()` | `Interactable.h`, `Camera.h`, `Scene.h` |

---

## 4. Architecture

### 4.1 Lifecycle & Frame Execution Pipeline
The program executes a deterministic, multi-stage rendering pipeline each frame:

```
+-----------------------------------------------------------------------------------+
|                                  START FRAME                                      |
+-----------------------------------------------------------------------------------+
                                          |
                                          v
+-----------------------------------------------------------------------------------+
| 1. TIME & DELTA COMPUTATION:                                                      |
|    currentFrame = glfwGetTime(); deltaTime = currentFrame - lastFrame;            |
|    Calculate running FPS and average frame-time (updated every 0.5s).            |
+-----------------------------------------------------------------------------------+
                                          |
                                          v
+-----------------------------------------------------------------------------------+
| 2. CONTINUOUS INPUT PROCESSING (Main.cpp: processContinuousInput):                |
|    - WASD/EQ camera translation velocity integration.                             |
|    - Resolve continuous wall collision: camera.Position = resolveCollision(...)   |
|    - Selected object continuous 6-DOF manipulation (I/K, J/L, U/O, Arrows).       |
+-----------------------------------------------------------------------------------+
                                          |
                                          v
+-----------------------------------------------------------------------------------+
| 3. SIMULATION & KINEMATICS UPDATE (Scene.h: Scene::update):                       |
|    - Advance day/night blend factor towards target (dayNightSpeed = 1.5).         |
|    - Advance totalTime (if not paused).                                           |
|    - Update all 17 objects (lantern swings, orb orbit, cloth rippling, etc.).     |
|    - Recalculate scene-graph world matrices: rootNode->updateWorldMatrix().       |
|    - Update dynamic lights (track swinging lanterns, orb position, spotlight).   |
+-----------------------------------------------------------------------------------+
                                          |
                                          v
+-----------------------------------------------------------------------------------+
| 4. INTERACTION & HUD UPDATE:                                                      |
|    - InteractionManager::update(): View-cone dot-product & proximity scoring.     |
|    - Hud::update(): 10 Hz rate-limited string formatting & geometry batching.     |
+-----------------------------------------------------------------------------------+
                                          |
                                          v
+-----------------------------------------------------------------------------------+
| 5. PASS 1 — DIRECTIONAL SHADOW DEPTH PASS (Scene.h: renderShadowDepth):           |
|    - Bind depthMapFBO (2048x2048), glViewport(0, 0, 2048, 2048).                  |
|    - Compute light-space matrix: ortho(-24, 24, -24, 24, 0.1, 90) * lookAt(...).  |
|    - Front-face culling enabled (GL_FRONT) to eliminate surface acne.             |
|    - Traverse scene graph via drawDepth() (skips sky, emissive, non-shadowers).   |
|    - Unbind FBO, restore GL_BACK culling.                                         |
+-----------------------------------------------------------------------------------+
                                          |
                                          v
+-----------------------------------------------------------------------------------+
| 6. PASS 2 — PRIMARY 3D SCENE RENDER PASS:                                         |
|    - glViewport(0, 0, screenWidth, screenHeight).                                 |
|    - glClearColor(0.08, 0.09, 0.14, 1.0); glClear(COLOR_BUFFER | DEPTH_BUFFER).   |
|    - If rayTracingMode == true:                                                   |
|         Draw full-screen quad with raytrace.frag.                                 |
|      Else:                                                                        |
|         Upload view, projection, 14 dynamic point lights, material uniforms.            |
|         Bind shadowMap depth texture to GL_TEXTURE1.                              |
|         Traverse scene graph via draw() (draws meshes with diffuse/emissive).     |
+-----------------------------------------------------------------------------------+
                                          |
                                          v
+-----------------------------------------------------------------------------------+
| 7. PASS 3 — 2D ORTHOGRAPHIC HUD OVERLAY PASS (Hud.cpp: Hud::render):              |
|    - If pending screenshot clean (F10), capture viewport pixels before HUD.       |
|    - Save previous GL state (depth test, blend, culling, bindings).               |
|    - Disable depth test & face culling; enable alpha blend.                       |
|    - Draw Pass A: Solid translucent background rect & border (mode = 0).          |
|    - Draw Pass B: Font glyph quads sampled from fontTexture atlas (mode = 1).     |
|    - Restore previous GL state exactly.                                           |
|    - If pending screenshot with HUD (Shift+F10), capture viewport pixels now.     |
+-----------------------------------------------------------------------------------+
                                          |
                                          v
+-----------------------------------------------------------------------------------+
| 8. BUFFER SWAP & EVENT POLLING:                                                   |
|    - glfwSwapBuffers(window); glfwPollEvents();                                   |
+-----------------------------------------------------------------------------------+
                                          |
                                          v
+-----------------------------------------------------------------------------------+
|                                   END FRAME                                       |
+-----------------------------------------------------------------------------------+
```

### 4.2 OpenGL State Machine Management
To ensure visual fidelity and prevent state corruption between 3D passes and the 2D HUD:
- **Global Initialization (`Main.cpp: lines 100-102`):**
  - `glEnable(GL_DEPTH_TEST)`: Standard less-than depth testing.
  - `glEnable(GL_BLEND)`: Alpha blending enabled.
  - `glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)`: Standard transparency blending.
- **Shadow Depth Pass State (`Scene.h: lines 1386-1390`):**
  - Switches to front-face culling (`glCullFace(GL_FRONT)`) during shadow map creation. This forces depth recording on backfaces, completely eliminating self-shadow surface acne on building walls and roofs without requiring excessive bias.
  - Restores back-face culling (`glCullFace(GL_BACK)`) immediately upon pass completion.
- **HUD Pass State Preservation (`Hud.cpp: lines 360-441`):**
  - Queries and pushes existing GL state via `glGetIntegerv` and `glIsEnabled`: `GL_DEPTH_TEST`, `GL_CULL_FACE`, `GL_BLEND`, blend functions, current program, bound VAO, bound VBO, active texture unit, texture 2D binding, and viewport dimensions.
  - Disables depth test and face culling; sets `glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)`.
  - Upon completion, restores every single queried state to its exact prior value.

### 4.3 GLSL Shader Program Catalog

#### 1. Core Scene Shader (`shaders/basic.vert` & `shaders/basic.frag`)
- **Purpose:** Primary 3D geometry rendering pipeline implementing Blinn-Phong illumination, 14 dynamic point lights (16 total lights), 16-sample PCF soft shadow mapping, diffuse texture mapping, emissive glows, and the celestial sky dome.
- **Vertex Attributes (`layout (location = ...)`):**
  - `0`: `vec3 aPos` (Model-space vertex position)
  - `1`: `vec3 aNormal` (Model-space surface normal)
  - `2`: `vec2 aTexCoords` (Surface UV coordinates)
- **Varying Interface (Vertex to Fragment):**
  - `out vec3 FragPos`: World-space position (`vec3(model * vec4(aPos, 1.0))`)
  - `out vec3 Normal`: Transformed normal (`normalMatrix * aNormal`)
  - `out vec2 TexCoords`: Passed through UV coordinates
  - `out vec4 FragPosLightSpace`: Transformed position in light projection space (`lightSpaceMatrix * vec4(FragPos, 1.0)`)
- **Uniforms Catalog:**
  - `mat4 model`, `mat3 normalMatrix`, `mat4 view`, `mat4 projection`, `mat4 lightSpaceMatrix`
  - `vec4 objectColor`, `float dayNightFactor`, `float totalTime`
  - `bool isSky`, `bool isWindow`, `bool isEmissive`, `vec3 emissiveColor`
  - `vec3 viewPos`, `int shadingMode` ($0 = \text{Blinn-Phong}, 1 = \text{Diffuse Only}, 2 = \text{Ambient Only}$)
  - `bool enableTextures`, `bool useTexture`, `sampler2D diffuseTexture`, `float textureTiling`
  - `sampler2D shadowMap`, `bool enableShadows`
  - `DirLight dirLight` (`direction`, `ambient`, `diffuse`, `specular`)
  - `PointLight pointLights[12]` (`position`, `ambient`, `diffuse`, `specular`, `constant`, `linear`, `quadratic`)
  - `int numActivePointLights`
  - `SpotLight spotLight` (`position`, `direction`, `ambient`, `diffuse`, `specular`, `cutOff`, `outerCutOff`, `constant`, `linear`, `quadratic`)
  - `bool spotLightActive`, `Material material` (`shininess`, `specularStrength`)
- **Objects Using It:** Ground, Machiya buildings, Torii gate, Sakura trees, lanterns, food stalls, vendors, magic stage, magician, audience, crowd, fireworks, and sky dome.

#### 2. Shadow Depth Shader (`shaders/shadow_depth.vert` & `shaders/shadow_depth.frag`)
- **Purpose:** Renders the scene from the directional light's perspective into the $2048 \times 2048$ shadow depth framebuffer.
- **Vertex Attributes:** Location `0`: `vec3 aPos`.
- **Uniforms:** `mat4 lightSpaceMatrix`, `mat4 model`.
- **Fragment Shader:** Empty body; OpenGL hardware automatically writes normalized device coordinate depth values to the attached depth texture.
- **Objects Using It:** All shadow-casting scene geometry (skips sky dome, emissive flames, and non-casting objects).

#### 3. Real-Time GPU Ray Tracer (`shaders/raytrace.vert` & `shaders/raytrace.frag`)
- **Purpose:** Full-screen quad pixel shader performing primary ray generation, analytical primitive ray tracing (spheres, boxes, cylinders, planes), hard shadow rays, and recursive mirror reflections.
- **Vertex Attributes:** Location `0`: `vec2 aPos` (Clip-space coordinates $[-1, 1]$).
- **Uniforms:**
  - `vec3 uCamPos`, `vec3 uCamFront`, `vec3 uCamUp`, `vec3 uCamRight`, `vec2 uResolution`
  - `float uFov`, `float uAspect`, `float uTime`, `float uNightFactor`
  - `vec3 uOrbPos`, `vec3 uBoxPos`, `vec3 uBoxScale`
  - `vec3 uSpotPos`, `vec3 uSpotDir`
  - `vec3 uFireworksPos`, `vec3 uFireworksColor`, `float uFireworksActive`
  - `int uMaxBounces` (default 3)
- **Objects Using It:** Rendered when `scene.rayTracingMode` is toggled on via <kbd>Z</kbd>.

#### 4. Heads-Up Display Shader (`shaders/hud.vert` & `shaders/hud.frag`)
- **Purpose:** Dedicated 2D orthographic UI shader rendering solid translucent background panels, border lines, and anti-aliased font glyphs.
- **Vertex Attributes:**
  - `0`: `vec2 aPos` (Screen pixel coordinates $[x, y]$)
  - `1`: `vec2 aTexCoords` (Font atlas UV coordinates $[u, v]$)
  - `2`: `vec4 aColor` (Vertex RGBA color)
- **Uniforms:**
  - `mat4 projection`: 2D pixel-coordinate orthographic matrix (`glm::ortho(0, width, height, 0, -1, 1)`)
  - `sampler2D fontTexture`: Bound to the $256 \times 256$ Consolas Bold atlas (`GL_TEXTURE0`)
  - `int mode`: $0 = \text{solid colored quad}$, $1 = \text{textured font glyph}$
- **Fragment Functionality:** In mode 1, samples glyph coverage from `fontTexture.r` and applies a `smoothstep(0.12, 0.45, a)` curve to eliminate alpha bleeding while producing solid, crisp text.

---

## 5. Geometry System

### 5.1 Parametric Primitive Mesh Generators
All base geometric shapes are generated algorithmically in `src/Primitives.h`. No external 3D model files (.obj, .gltf) are loaded; every object is synthesized from code.

```
       +-----------------+             +-----------------+             +-----------------+
       |      CUBE       |             |    CYLINDER     |             |      CONE       |
       |  24 Vertices    |             |  100 Vertices   |             |   76 Vertices   |
       |  36 Indices     |             |  144 Indices    |             |  144 Indices    |
       |  12 Triangles   |             |  48 Triangles   |             |  48 Triangles   |
       +-----------------+             +-----------------+             +-----------------+
               |                               |                               |
               v                               v                               v
       +-----------------+             +-----------------+             +-----------------+
       |     SPHERE      |             |      PLANE      |             |   SWEPT TUBES   |
       |  525 Vertices   |             |   25 Vertices   |             |   (Bezier /     |
       |  960 Indices    |             |   32 Indices    |             |  Catmull-Rom)   |
       |  320 Triangles  |             |  16 Triangles   |             |  Dynamic Mesh   |
       +-----------------+             +-----------------+             +-----------------+
```

#### 1. Cube (`Primitives::createCube(float size)`)
- **Algorithm:** Constructs 6 distinct square faces. Each face generates 4 unique vertices with constant orthogonal normal vectors ($+Z, -Z, -X, +X, +Y, -Y$) and standard $[0, 1]$ UV coordinates.
- **Counts:** Exactly 24 vertices, 36 indices, 12 triangles.
- **Parameters:** `size = 1.0f` (extents from $-0.5\cdot \text{size}$ to $+0.5\cdot \text{size}$).

#### 2. Cylinder (`Primitives::createCylinder(float radius, float height, int segments)`)
- **Algorithm:** Generates a cylindrical mantle using two rings of vertices (top at $+h/2$, bottom at $-h/2$) parameterized by $\theta \in [0, 2\pi]$. Mantle normals are purely horizontal radial vectors $(\cos\theta, 0, \sin\theta)$. Top and bottom disc caps are constructed as triangle fans with independent vertical normals $(0, 1, 0)$ and $(0, -1, 0)$ and circular UV mapping.
- **Counts (at `segments = 24`):**
  - Mantle: 50 vertices, 144 indices.
  - Caps: 2 center vertices + 50 ring vertices, 144 indices.
  - Total: 102 vertices, 288 indices, 96 triangles.

#### 3. Cone (`Primitives::createCone(float radius, float height, int segments)`)
- **Algorithm:** Generates a conical mantle parameterized by circumferential segments. Apex duplicates share slanted surface normals normalized from $(\cos\theta, \text{radius}/\text{height}, \sin\theta)$ to ensure smooth Gouraud/Phong normal interpolation across the tip. Circular base cap constructed with downward normal $(0, -1, 0)$.
- **Counts (at `segments = 24`):** 77 vertices, 216 indices, 72 triangles.

#### 4. Sphere (`Primitives::createSphere(float radius, int rings, int sectors)`)
- **Algorithm:** UV latitude/longitude tessellation. Latitude angle $\phi \in [-\pi/2, \pi/2]$ (`rings = 20`); longitude angle $\theta \in [0, 2\pi]$ (`sectors = 24`). Normal vectors are identical to unitized vertex positions: $\mathbf{N} = (\cos\phi\cos\theta, \sin\phi, \cos\phi\sin\theta)$.
- **Counts:** $(20 + 1) \times (24 + 1) = 525$ vertices; $20 \times 24 \times 6 = 2880$ indices, 960 triangles.

#### 5. Plane (`Primitives::createPlane(float width, float depth, int gridX, int gridZ)`)
- **Algorithm:** Subdivided horizontal quad grid along the $XZ$ plane ($Y = 0$). Upward normals $(0, 1, 0)$.
- **Counts (at `gridX = 4, gridZ = 4`):** $(4+1) \times (4+1) = 25$ vertices, $4 \times 4 \times 6 = 96$ indices, 32 triangles.

### 5.2 Curved & Botanical Geometry Generators (`Curves.h`)
`src/Curves.h` provides specialized procedural modeling functions utilizing continuous mathematics:
- **`Curves::Bezier3` & `Bezier2`:** Parametric cubic and quadratic Bézier evaluation computing positions and $C^1$ continuous unit tangents:
  $$\mathbf{B}(t) = (1-t)^3 \mathbf{P}_0 + 3(1-t)^2 t \mathbf{P}_1 + 3(1-t) t^2 \mathbf{P}_2 + t^3 \mathbf{P}_3$$
- **Bishop Parallel Transport Frame:** Generates continuous rotation-minimizing local coordinate frames along arbitrary 3D curves without gimbal-lock or twisting discontinuities.
- **`createSweptTube()`:** Sweeps variable-radius circular cross-sections along 3D space curves. Used for windswept bonsai trunks, ikebana flower stems, and kokedama vines.
- **`createCurvedBeam()`:** Sweeps an architectural cross-section with Japanese *sori* (upward concave arch curvature) and roof beveling. Used for the Torii gate *kasagi* and *shimaki* lintels.
- **`createCatenaryRope()`:** Generates the catenary sag curve $y(x) = y_{\text{pole}} - \text{sag} \cdot (1 - (x/L)^2)$ holding street lanterns.
- **`createSakuraBlossomLobe()`:** Generates organic, billowed hemisphere lobes with radial sine pertubations ($A = 0.24$) for cherry blossom foliage clusters.
- **`createCurvedLeafMesh()` & `createCurvedPetalMesh()`:** Cupped botanical meshes with longitudinal arching and transverse V-folds.

### 5.3 Mesh, VAO/VBO/EBO Wrapper & Draw Dispatch
`src/Mesh.h` wraps the low-level OpenGL buffer state:
```cpp
// Mesh.h: setupMesh()
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glGenBuffers(1, &EBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

// Layout 0: Position (3 floats)
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

// Layout 1: Normal (3 floats)
glEnableVertexAttribArray(1);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

// Layout 2: TexCoords (2 floats)
glEnableVertexAttribArray(2);
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
```
- **Redundant State Elimination:** `Mesh::s_CurrentBoundVAO` tracks the currently bound VAO. If consecutive draw calls share the same underlying primitive (e.g., hundreds of cubes in a building), `glBindVertexArray` driver calls are bypassed.

### 5.4 Hierarchical Composition Methodology
All 17 composite scene objects are constructed by instantiating shared primitive meshes within a tree of `SceneNode` instances. Primitives are never regenerated per instance; a single `SceneMeshes` struct (instantiated in `Scene.h`) holds one master copy of each primitive mesh, which is referenced via pointers (`node->mesh = &meshes.cube`). Node transforms provide individual position, orientation, and scaling.

---

## 6. Transform System and Scene Graph

### 6.1 The `Transform` Class
Defined in `src/Transform.h`. Encapsulates local 3D affine transformation parameters:
- `glm::vec3 position`: Local translation offsets $(t_x, t_y, t_z)$.
- `glm::vec3 rotation`: Euler angles in degrees representing $(pitch_x, yaw_y, roll_z)$.
- `glm::vec3 scale`: Non-uniform scaling factors $(s_x, s_y, s_z)$.

#### Local Matrix Composition Order
The local affine matrix $\mathbf{M}_{\text{local}}$ is constructed using standard TRS concatenation with Yaw $\to$ Pitch $\to$ Roll Euler rotation order:
$$\mathbf{M}_{\text{local}} = \mathbf{T}(p_x, p_y, p_z) \times \mathbf{R}_y(\theta_y) \times \mathbf{R}_x(\theta_x) \times \mathbf{R}_z(\theta_z) \times \mathbf{S}(s_x, s_y, s_z)$$

```cpp
// Transform.h: lines 36-46
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, position);
if (rotation.y != 0.0f)
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
if (rotation.x != 0.0f)
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
if (rotation.z != 0.0f)
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::scale(model, scale);
```
- **Matrix Caching & Dirty Flags:** `Transform` caches `cachedLocalMatrix`. `checkDirty()` compares current vectors against `lastPosition`, `lastRotation`, and `lastScale`. Matrix re-multiplication is skipped if transforms did not change.

### 6.2 The `SceneNode` Architecture
Defined in `src/SceneNode.h`. Implements an n-ary scene graph tree:
- `std::string name`: Human-readable debug identifier.
- `Transform transform`: Local TRS transform.
- `glm::mat4 worldMatrix`: Fully accumulated world transformation matrix.
- `glm::mat3 normalMatrix`: Precomputed normal transformation matrix:
  $$\mathbf{N}_{\text{matrix}} = \left( (\mathbf{M}_{\text{world}}^{3 \times 3})^{-1} \right)^T$$
- `SceneNode* parent`: Raw pointer to parent node (null if root).
- `std::vector<std::shared_ptr<SceneNode>> children`: Owning list of child nodes.
- `const Mesh* mesh`: Non-owning pointer to shared primitive mesh.
- `glm::vec4 color`, `float shininess`, `float specularStrength`, `const Texture* texture`, `float textureTiling`.
- `bool isStatic`: When `true`, matrix cascade skips traversal if parent did not move.

#### World Matrix Cascade (`updateWorldMatrix`)
```cpp
// SceneNode.h: lines 141-157
void updateWorldMatrix(const glm::mat4& parentMatrix = glm::mat4(1.0f))
{
    bool localChanged = transform.checkDirty();
    bool parentChanged = (parentMatrix != lastParentMatrix);

    if (localChanged || parentChanged || matrixDirty)
    {
        worldMatrix = parentMatrix * transform.getLocalMatrix();
        normalMatrix = glm::transpose(glm::inverse(glm::mat3(worldMatrix)));
        lastParentMatrix = parentMatrix;
        matrixDirty = false;

        for (auto& child : children)
            child->updateWorldMatrix(worldMatrix);
    }
    else if (!isStatic)
    {
        for (auto& child : children)
            child->updateWorldMatrix(worldMatrix);
    }
}
```

### 6.3 Complete Scene Graph Hierarchy

```
World_Root (pos: 0, 0, 0)
│
├── Ground_Street (pos: 0, 0, 0) [Plane]
│   ├── Curb_Left (pos: -4.5, 0.08, 0) [Cube]
│   └── Curb_Right (pos: 4.5, 0.08, 0) [Cube]
│
├── Machiya_L1 (pos: -10.5, 0, 16.0, rotY: 0)
│   ├── Machiya_L1_Frame [Cube]
│   ├── Machiya_L1_Roof_Eaves [Cube, rotX: 12]
│   ├── Machiya_L1_SlidingDoorGroup (pos: 4.14, 1.2, Z_slide)
│   │   └── Machiya_L1_FrontDoorPanel [Cube]
│   ├── Machiya_L1_SlidingWindowSash_1 (pos: 4.14, 5.2, X_slide)
│   ├── Machiya_L1_LivingRoom_Tatami [Plane]
│   └── Machiya_L1_Stairs (14 steps) [Cubes]
│
├── Machiya_L2 (pos: -10.5, 0, -6.0, rotY: 0)
├── Machiya_R1 (pos: 10.5, 0, 16.0, rotY: 180)
├── Machiya_R2 (pos: 10.5, 0, -6.0, rotY: 180)
│
├── Torii_Gate (pos: 0, 0, -32.0)
│   ├── Torii_Column_L (pos: -5.2, 5.0, 0, rotZ: 1.5) [Cylinder]
│   ├── Torii_Column_R (pos: 5.2, 5.0, 0, rotZ: -1.5) [Cylinder]
│   ├── Torii_Nuki_Beam (pos: 0, 7.8, 0) [Cube]
│   ├── Torii_Shimaki (pos: 0, 9.4, 0) [CurvedBeam]
│   └── Torii_Kasagi (pos: 0, 9.9, 0) [CurvedBeam]
│
├── SakuraTree_0 .. 6 (Placements across street lawns)
│   ├── Tree_Trunk [Cylinder / SweptTube]
│   ├── Bough_Primary .. Secondary [Cylinders]
│   ├── Canopy_Lobes (x16) [SakuraBlossomLobe]
│   └── Petal_0 .. 7 (Animated wind particles) [Spheres]
│
├── Span_0 .. 4 (Overhead street catenary rope spans)
│   ├── Span_Pole_L (pos: -4.5, 3.2, Z) [Cylinder]
│   ├── Span_Pole_R (pos: 4.5, 3.2, Z) [Cylinder]
│   ├── Span_Catenary_Rope [CatenaryRope]
│   └── Lantern_0 .. 3 (Per span)
│       └── RopePivot (pos: X, Y, Z, rotX: swingAngle)
│           └── LanternBody (pos: 0, -0.65, 0) [Sphere]
│               ├── CapTop [Cylinder]
│               ├── CapBot [Cylinder]
│               └── Tassel [Cone]
│
├── Takoyaki_Stall (pos: -5.2, 0, 6.0, rotY: 90)
│   ├── Stall_Frame [Cube]
│   ├── Awning_Noren [Plane, rotX: 18]
│   ├── Griddle_Plate [Cube]
│   └── TakoBall_0 .. 5 (pos: X, Y_hop, Z, rotY: spin) [Spheres]
│
├── Kakigori_Stall (pos: 5.2, 0, 6.0, rotY: -90)
│   ├── Stall_Frame [Cube]
│   ├── Shaver_Machine_Box [Cube]
│   │   ├── Shaver_Crank_Wheel (rotX: spin) [Cylinder]
│   │   └── Shaver_Active_Ice (scale: pulse) [Sphere]
│   └── Serving_0 .. 5 (pos: X, Y_hop, Z) [Glass Cup + Ice Mound]
│
├── Vendor_Takoyaki & Vendor_Kakigori (pos behind stall counters)
│   ├── Vendor_Torso [Cylinder]
│   ├── Vendor_Head [Sphere]
│   └── Vendor_UpperArm (rotX: stir) [Cylinder]
│       └── Vendor_Forearm (rotX: stir) [Cylinder]
│           └── Vendor_Ladle [Cylinder]
│
├── Magic_Show_Stage (pos: 6.2, 0, -19.0)
│   ├── Stage_Platform [Cube]
│   ├── Tatami_Carpet [Plane]
│   └── Byobu_Screen [Plane]
│
├── Magician (pos: 6.2, 0.95, -19.0)
│   ├── Magician_Torso [Cylinder]
│   ├── Magician_Head & TopHat [Sphere + Cylinders]
│   └── Magician_RightArm (Articulated spellcasting pose)
│       └── Magician_RightForearm
│           └── Magician_RightHand
│               ├── Wand [Cylinder]
│               ├── MagicOrb (pos: r*cos, y, r*sin) [Sphere]
│               └── CometTrail_0 .. 2 (Delayed phase) [Spheres]
│
├── Magic_VanishingBox (pos: 6.2, 0.95, -19.0)
│   ├── Magic_GoldenBox (scale: 0.65 -> 0.0 -> 0.65) [Cube]
│   └── Magic_SilkCloth (pos: descent -> whip aside) [Cube]
│
├── Spotlight_Rig (pos: 4.0, 0, -16.5)
│   ├── Spotlight_Pole [Cylinder]
│   └── LampHousing (rotY: trackMagicianYaw, rotX: pitch) [Cone]
│       └── Spotlight_Lens (emissive) [Cylinder]
│
├── Audience_Group (pos in front of stage)
│   └── Spectator_0 .. 5 (Yukata bodies, nodding heads)
│
├── Crowd_Group (Walking pedestrians traversing street)
│   └── Walker_0 .. 5 (posZ: translate, rotY: laneSway)
│       ├── Thigh_L (rotX: walkAngle) -> Shin_L (rotX: kneeFlex)
│       ├── Thigh_R (rotX: -walkAngle) -> Shin_R (rotX: kneeFlex)
│       └── Arm_L -> Arm_R (Counter-swing)
│
├── Firework_System (Upper sky dome)
│   └── Rocket_0 .. 3
│       ├── Shell [Sphere]
│       └── Particle_0 .. 34 (Explosion burst pool) [Spheres]
│
└── Sky_Dome (pos: camera.Position) [Inverted Sphere]
```

### 6.4 Motion Relative to Another Object's Reference Frame
A central requirement of CSE4102 is demonstrating transformations relative to another object's moving reference frame. The project achieves this via the scene graph hierarchy, without manually hard-coding world-space offsets:

#### Example 1: Hanging Paper Lanterns (`Objects.h: LanternObject`)
```cpp
// Objects.h: lines 2529-2536
void update(float time)
{
    float angle = swingAmp * std::sin(time * swingFreq + phaseOffset);
    ropePivot->transform.rotation.x = angle;
    ropePivot->transform.rotation.z = angle * 0.25f;
}
```
- `ropePivot` is placed at the catenary rope attachment point.
- `lanternBody` is added as a child of `ropePivot` at local position $(0.0, -0.65, 0.0)$.
- When `ropePivot` rotates, `lanternBody`, the top cap, bottom cap, and tassel all swing together.
- In `Scene.h: updateLighting()`, Point Light #3 queries `lanternBody->getWorldPosition()`, causing the light source to swing with the lantern.

#### Example 2: Floating Magic Orb (`Objects.h: Magician`)
```cpp
// Objects.h: lines 3898-3905
float r = 0.45f;
float speed = 3.2f;
float orbX = r * std::cos(time * speed);
float orbY = 0.18f + 0.16f * std::sin(time * speed * 2.0f);
float orbZ = 0.50f + r * std::sin(time * speed);

orbNode->transform.position = glm::vec3(orbX, orbY, orbZ);
```
- `orbNode` is parented to `rightHand`, which is parented to `rightForearm`, which is parented to `rightArm`, which is parented to `magician->root`.
- The magician's arm raises, sways, and articulates. The orb's helical equation $(r\cos t, y(t), r\sin t)$ is evaluated entirely in hand-local space.
- Point Light #0 queries `magician->orbNode->getWorldPosition()`, casting dynamic light onto the magician's face as the orb orbits his moving hand.

---

## 7. Scene Contents

### 7.1 Ground & Pavement
- **File & Class:** `src/Objects.h: GroundObject`
- **Primitives Used:** Scaled `Plane` ($14.0\text{m} \times 74.0\text{m}$ street, $65.0\text{m} \times 90.0\text{m}$ earth), `Cube` curbs.
- **Position & Scale:** Positioned at $(0.0, 0.0, 0.0)$. Pavement spans $X \in [-4.5, 4.5]$, $Z \in [-37.0, 37.0]$.
- **Materials & Textures:** Flagstone pavement uses `texStone` (`assets/textures/stone_pavement.bmp`) with 14x tiling, shininess $16.0$, specular strength $0.20$. Curbs use dark granite stone color.

### 7.2 Machiya Townhouse Buildings (x4)
- **File & Class:** `src/Objects.h: MachiyaBuilding`
- **Count:** 4 instanced buildings:
  - `Machiya_L1`: Left side, $Z = 16.0\text{m}$, $X = -10.5\text{m}$, rotation $Y = 0^\circ$.
  - `Machiya_L2`: Left side, $Z = -6.0\text{m}$, $X = -10.5\text{m}$, rotation $Y = 0^\circ$.
  - `Machiya_R1`: Right side, $Z = 16.0\text{m}$, $X = 10.5\text{m}$, rotation $Y = 180^\circ$.
  - `Machiya_R2`: Right side, $Z = -6.0\text{m}$, $X = 10.5\text{m}$, rotation $Y = 180^\circ$.
- **Primitives Used:** ~180 `Cube` and `Plane` primitives per building (timber framework, sloped roof tiles, second-floor veranda, interior tatami flooring, 14-step interior staircase, sliding Shoji door, sliding second-floor windows).
- **Interactive Doors & Windows:** Front sliding door translates $+1.35\text{m}$ along $Z$ when <kbd>H</kbd> is pressed. Upper window sashes translate $\pm 0.65\text{m}$ along $X$ when <kbd>G</kbd> is pressed.
- **Lighting:** Houses 6 interior amber point lights (Lights #6 through #11) in ground-floor living rooms and upper bedrooms.

### 7.3 Torii Shrine Gate & Decorative Illuminations
- **File & Class:** `src/Objects.h: ToriiGate`
- **Position:** Anchored at $(0.0, 0.0, -32.0\text{m})$ framing the street terminus.
- **Primitives Used:** Two vertical `Cylinder` columns (*hashira*) with $1.5^\circ$ inward entasis tilt, two granite *daiishi* base stones, one horizontal *nuki* tie-beam, central *gakuzuka* tablet plaque with gilded gold leaf finish and circular crest disk, curved *shimaki* sub-lintel (`Curves::createCurvedBeam`), and upper curved *kasagi* roof beam with upward *sori* arch ($0.40\text{m}$ rise) and beveled cap.
- **Festive Decorative Additions:**
  - **4 Grand Hanging Chochin Lanterns:** Suspended under the Nuki crossbeam ($X = \pm 3.2, \pm 1.15$) with bronze cords, black caps, radiant vermilion washi paper bodies (`isEmissive = true`, glowing `(2.6, 1.4, 0.5)`), white kanji bands, and golden silk tassels.
  - **2 Pillar-Mounted Cantilever Bracket Lanterns (*Tsuri-Doro*):** Forged bronze bracket arms at $Y = 4.8\text{m}$ on each pillar, hexagonal pagoda roof canopies, glowing warm amber washi diffuser cylinders (`(3.0, 1.8, 0.6)`), and teardrop finials.
  - **2 Traditional Japanese Stone Lanterns (*Ishi-Doro*):** Flanking the entrance path at $X = \pm 3.8\text{m}, Z = 2.4\text{m}$ with stepped *Kiso* plinths, fluted *Sao* shafts, lotus *Chudai* shelves, hollow *Hibukuro* chambers with radiant sacred flame cores (`(3.5, 2.4, 1.0)`), flared pagoda *Kasa* roofs, and lotus *Hoju* jewel finials.
  - **Sacred Straw Rope (*Shimenawa*) & Streamers (*Shide*):** Braided straw rope with 3 hanging tassels and 4 folded white zigzag paper streamers spanning beneath the Nuki crossbeam.
  - **2 Dynamic Point Light Anchors:** Left and right anchors tracking Point Lights #12 and #13.
- **Color & Material:** Brilliant vermilion red (`glm::vec4(0.85, 0.22, 0.12, 1.0)`), black caps, gold plaque, granite stone, and straw wheat.

### 7.4 Cherry Blossom Trees (x7) & Detached Falling Petals
- **File & Class:** `src/Objects.h: SakuraTree`
- **Count:** 7 trees placed along street borders and courtyard gaps (e.g., Grand Shrine tree at $(-10.2, 0, -22.0)$, Plaza tree at $(10.2, 0, -26.5)$).
- **Primitives Used:** Tapered `Cylinder` trunks, primary and secondary branches, 16 billowed hemisphere foliage clusters (`Curves::createSakuraBlossomLobe`), and 8 detached falling petal spheres.
- **Animation:** Falling petals detach and fall at $0.65\text{ m/s}$, drifting horizontally with sinusoidal wind sway ($\text{amp} = 0.55\text{m}$), respawning at tree apex ($Y \approx 8.5\text{m}$) when touching ground.

### 7.5 Overhead Catenary Lantern Spans (x5) & Swinging Lanterns
- **File & Class:** `src/Objects.h: StreetLanternSpan`, `LanternObject`
- **Count:** 5 spans across the street at $Z = 22.0, 11.0, 0.0, -9.0, -24.5\text{m}$. Each span contains 4 lanterns (20 lanterns total).
- **Primitives Used:** Two vertical cedar poles (`Cylinder`), one sagging catenary rope (`Curves::createCatenaryRope`), and 4 lanterns (`Sphere` body, `Cylinder` top/bottom caps, `Cone` tassel).
- **Animation:** Harmonic pendulum swing driven by rope pivot rotation $\theta = 12.0^\circ \sin(2.0 t + \phi)$. Point Lights #3 & #4 dynamically track lantern bodies on Spans 1 and 3.

### 7.6 Takoyaki Food Stall
- **File & Class:** `src/Objects.h: TakoyakiStall`
- **Position:** West curb at $(-5.2, 0.0, 6.0\text{m})$, rotated $90^\circ$ facing the street.
- **Primitives Used:** Cedar stall frame (`Cube`), red/white striped noren awning (`Plane`), iron griddle plate (`Cube`), counter shelves, hanging paper lanterns, and 6 takoyaki spheres.
- **Animation:** 6 takoyaki balls rotate continuously in place ($120^\circ/\text{s}$). Every 3.0 seconds, each ball executes a parabolic hopping arc ($\Delta y_{\text{apex}} = 0.35\text{m}$) flipping between grill cavities with a $360^\circ$ mid-air spin.

### 7.7 Kakigori Shaved Ice Stall
- **File & Class:** `src/Objects.h: KakigoriStall`
- **Position:** East curb at $(5.2, 0.0, 6.0\text{m})$, rotated $-90^\circ$ facing the street.
- **Primitives Used:** Timber cart frame, blue/white wave noren banner, vintage iron shaved ice machine ("The Box") with hand crank wheel, and 6 multi-flavored Kakigori dessert bowls in shaved ice glass cups (matcha, strawberry, blue hawaii, mango, grape, sweet milk).
- **Animation:** Shaver crank wheel rotates continuously ($240^\circ/\text{s}$). Active ice mound pulses under the chute. 6 dessert bowls rotate continuously ($90^\circ/\text{s}$) and periodically execute joyful parabolic hopping arcs across the counter.

### 7.8 Articulated Vendor Figures (x2)
- **File & Class:** `src/Objects.h: VendorFigure`
- **Positions:** Standing behind food stall counters at $(-6.65, 0.0, 6.0\text{m})$ and $(6.65, 0.0, 6.0\text{m})$.
- **Primitives Used:** Articulated rig: torso (`Cylinder`), head (`Sphere`), chef *hachimaki* headband, upper arm, forearm, hand, and cooking utensil (takoyaki pick / ice scoop).
- **Animation:** Compound shoulder and elbow articulation: upper arm pitches and yaws ($\sin(4.0t) \times 14^\circ$), forearm bends ($\sin(4.0t + 0.5) \times 16^\circ$), executing continuous cooking/serving motions.

### 7.9 Magic Show Stage Platform
- **File & Class:** `src/Objects.h: MagicStage`
- **Position:** Plaza area at $(6.2, 0.0, -19.0\text{m})$.
- **Primitives Used:** Elevated wooden platform deck (`Cube`, $4.8\text{m} \times 0.9\text{m} \times 5.6\text{m}$), red tatami carpet trim (`Plane`), and a three-panel gold leaf folding screen (*byobu*) using `texGold` (`assets/textures/gold_leaf.bmp`).

### 7.10 Magician Figure
- **File & Class:** `src/Objects.h: Magician`
- **Position:** Center stage at $(6.2, 0.95, -19.0\text{m})$.
- **Primitives Used:** Articulated character rig: kimono torso, head, wizard top hat, flowing cape, left resting arm, right spellcasting arm with wand.
- **Animation:** Spellcasting arm articulation, head scanning the audience, and holding the reference frame for the floating orb trick.

### 7.11 Floating Magic Orb (Trick 1)
- **File & Class:** `src/Objects.h: Magician::orbNode`
- **Hierarchy:** Child node of the magician's right hand bone.
- **Primitives Used:** Glowing cyan sphere (`radius = 0.16m`), plus 3 trailing comet-tail spheres.
- **Animation:** 3D helical orbit in hand space ($r = 0.45\text{m}, \omega = 3.2\text{ rad/s}$). Point Light #0 tracks the orb's world position.

### 7.12 Vanishing Box & Silk Cloth (Trick 2)
- **File & Class:** `src/Objects.h: VanishingBoxTrick`
- **Position:** Magic stage table at $(6.2, 0.95, -19.0\text{m})$.
- **Primitives Used:** Gold-trimmed box (`Cube`) and crimson silk cloth (`Cube`).
- **Animation:** Looping 7.0-second 6-phase state machine demonstrating scale-to-zero transform and position swap (see Section 8.3).

### 7.13 Stage Tracking Spotlight Rig
- **File & Class:** `src/Objects.h: SpotlightRig`
- **Position:** Beside magic stage at $(4.0, 0.0, -16.5\text{m})$.
- **Primitives Used:** Vertical steel pole (`Cylinder`), mounting gimbal, conical lamp housing (`Cone`), and emissive lens (`Cylinder`).
- **Animation:** Housing pans ($yaw = -35^\circ + 20^\circ\sin(1.5t)$) and tilts ($pitch = 30^\circ + 10^\circ\cos(1.2t)$). The dynamic SpotLight updates position and direction from this housing.

### 7.14 Seated Audience Figures
- **File & Class:** `src/Objects.h: AudienceGroup`
- **Position:** Semicircular festival benches facing the magic stage at $Z \approx -13.5\text{m}$.
- **Primitives Used:** 6 seated figures wearing distinct colored yukatas (indigo, red, green, gold, purple).
- **Animation:** Heads nod and turn with individual phase offsets watching the performance.

### 7.15 Walking Crowd Pedestrians
- **File & Class:** `src/Objects.h: CrowdGroup`, `WalkingPerson`
- **Count:** 6 articulated pedestrians walking down the street lanes.
- **Primitives Used:** Multi-joint rig: torso, head, left/right thighs, shins with geta sandals, and arms.
- **Animation:** Linear translation along $Z$, lane sway, vertical step bobbing, and alternating hip/knee walk cycles. Looping wrap-around between $Z = -30.0\text{m}$ and $+36.0\text{m}$.

### 7.16 Fireworks Rocket Particle System
- **File & Class:** `src/Objects.h: FireworkSystem`, `FireworkRocket`
- **Count:** Pool of 4 rockets.
- **Primitives Used:** Glowing shell sphere during launch; 35 explosion particle spheres per rocket during burst.
- **Animation:** Two-stage particle physics: ease-out upward launch followed by spherical burst with gravity ($g = 7.5\text{ m/s}^2$) and size decay. Triggers Point Light #5.

### 7.17 Sky Dome & Atmospheric Canopy
- **File & Class:** `src/Objects.h: SkyDome`
- **Primitives Used:** Inverted `Sphere` (`radius = 160.0m`, 20 rings, 24 sectors) centered at `camera.Position`.
- **Shader Features:** Procedural celestial sun with solar flare corona, silvery moon with lunar craters, 160-cell primary twinkling star constellations, and faint Milky Way star dust.

### 7.18 Summary Table of Scene Objects

| # | Composite Object | Count / Instances | Base Primitives | Motion Type | Transform Types Demonstrated |
|:---:|---|:---:|---|:---:|---|
| **1** | Ground & Pavement | 1 unit | `Plane`, `Cube` | Static | Large-scale texture mapping, spatial anchor |
| **2** | Machiya Townhouses | 4 units | `Cube`, `Plane` | Animated | **Interactive local translation** (Shoji doors & windows) |
| **3** | Torii Shrine Gate | 1 unit | `Cylinder`, `Cube`, `CurvedBeam` | Static | Sori curvature, entasis pillar tilt |
| **4** | Sakura Trees & Petals | 7 trees, 56 petals | `Cylinder`, `Sphere`, `SweptTube` | Animated | **Hierarchical translation, sinusoidal drift, looping** |
| **5** | Street Lantern Spans | 5 spans, 20 lanterns | `Cylinder`, `Sphere`, `Cone`, `CatenaryRope` | Animated | **Harmonic pendulum rotation, relative-frame transform** |
| **6** | Takoyaki Food Stall | 1 unit, 6 balls | `Cube`, `Plane`, `Sphere` | Animated | **Continuous spin, parabolic hopping arc, flipping** |
| **7** | Kakigori Food Stall | 1 unit, 6 bowls | `Cube`, `Plane`, `Sphere`, `Cylinder` | Animated | **Continuous rotation, pulse scaling, hopping translation** |
| **8** | Vendor Figures | 2 figures | `Cylinder`, `Sphere` | Animated | **Hierarchical multi-joint shoulder & elbow rotation** |
| **9** | Magic Show Stage | 1 unit | `Cube`, `Plane` | Static | Elevated platform anchor, gold byobu folding screen |
| **10** | Magician Figure | 1 figure | `Cylinder`, `Sphere` | Animated | **Articulated spellcasting pose, moving parent frame** |
| **11** | Floating Magic Orb | 1 orb, 3 trails | `Sphere` | Animated | **3D helical orbit relative to moving hand bone** |
| **12** | Vanishing Box & Cloth | 1 box, 1 cloth | `Cube` | Animated | **Scale-to-zero disappearance, translation swap, restore** |
| **13** | Spotlight Rig | 1 rig | `Cylinder`, `Cone` | Animated | **Dynamic pan & tilt rotation tracking moving target** |
| **14** | Seated Audience | 6 figures | `Cube`, `Cylinder`, `Sphere` | Animated | **Periodic head nod & yaw rotation** |
| **15** | Crowd Pedestrians | 6 figures | `Cylinder`, `Sphere` | Animated | **Linear translation, lane sway, vertical bob, walk cycle** |
| **16** | Fireworks System | 4 rockets, 140 particles | `Sphere` | Animated | **Two-stage particle physics, gravity falloff, fade** |
| **17** | Sky Dome | 1 unit | Inverted `Sphere` | Dynamic | Infinite distance tracking, celestial day/night shader |

---

## 8. Animation and Motion Catalogue

### 8.1 Hanging Lantern Pendulum Swing
- **File & Function:** `src/Objects.h: LanternObject::update(float time)`
- **Node Driven:** `ropePivot` (parent of `lanternBody`).
- **Relative To:** Catenary rope anchor point on overhead span.
- **Mathematical Formula:**
  $$\theta_x(t) = A \cdot \sin(\omega t + \phi)$$
  $$\theta_z(t) = 0.25 \cdot \theta_x(t)$$
- **Constants:** Amplitude $A = 12.0^\circ$, angular frequency $\omega = 2.0\text{ rad/s}$, phase offset $\phi = \text{spanIndex} \times 0.7\text{ rad}$.
- **Transform Demonstrated:** Rotation $\to$ Hierarchical relative-frame motion.

### 8.2 Helical Magic Orb Trajectory
- **File & Function:** `src/Objects.h: Magician::update(float time)`
- **Node Driven:** `orbNode` and 3 trailing comet spheres.
- **Relative To:** Magician's articulated right hand bone (`rightHand`).
- **Mathematical Formula:**
  $$x(t) = r \cdot \cos(\omega t)$$
  $$y(t) = y_0 + h \cdot \sin(2\omega t)$$
  $$z(t) = z_0 + r \cdot \sin(\omega t)$$
- **Constants:** Radius $r = 0.45\text{m}$, speed $\omega = 3.2\text{ rad/s}$, base height $y_0 = 0.18\text{m}$, vertical amplitude $h = 0.16\text{m}$, forward offset $z_0 = 0.50\text{m}$. Trailing particles lag by $\Delta t = (i + 1) \times 0.14\text{s}$.
- **Transform Demonstrated:** Compound 3D Translation & Rotation relative to a moving articulated character bone.

### 8.3 Vanishing Box 6-Phase State Machine
- **File & Function:** `src/Objects.h: VanishingBoxTrick::update(float dt)`
- **Nodes Driven:** `boxNode` and `clothNode`.
- **Relative To:** Magic stage platform origin $(6.2, 0.95, -19.0)$.
- **Cycle Period:** $T = 7.0\text{ seconds}$ looping (`cycle = fmod(stateTimer, 7.0)`).
- **State Progression:**
  1. **Phase 1 ($0.0\text{s} \le t < 1.5\text{s}$):** Box rests at Spot 1 $(s = 0.65)$. Cloth descends onto box: $y_{\text{cloth}} = y_1 + 0.45 - 0.15 \cdot (t / 1.5)$.
  2. **Phase 2 ($1.5\text{s} \le t < 2.5\text{s}$) — SCALE-TO-ZERO:** Box scales down smoothly:
     $$s(t) = \text{lerp}(0.65, 0.001, (t - 1.5) / 1.0)$$
  3. **Phase 3 ($2.5\text{s} \le t < 4.0\text{s}$):** Box scale is $0.0$ (invisible). Cloth is yanked aside dramatically: translates $\Delta x = +1.2\text{m}, \Delta y = +0.4\text{m}$ and rotates $(\theta_x, \theta_y, \theta_z) = (45^\circ, 60^\circ, 30^\circ) \cdot \tau$.
  4. **Phase 4 ($4.0\text{s} \le t < 5.0\text{s}$):** Cloth flies to Spot 2. Box position swaps to Spot 2 $(s = 0.0)$.
  5. **Phase 5 ($5.0\text{s} \le t < 6.0\text{s}$) — REAPPEARANCE:** Box scales up from zero:
     $$s(t) = \text{lerp}(0.001, 0.65, (t - 5.0) / 1.0)$$
  6. **Phase 6 ($6.0\text{s} \le t < 7.0\text{s}$):** Reset transition: cloth lifts away and resets back to Spot 1.
- **Transform Demonstrated:** Non-uniform Scaling to Zero $\to$ Position Swap $\to$ Scaling Restoration.

### 8.4 Takoyaki Flipping & Hopping Dynamics
- **File & Function:** `src/Objects.h: TakoyakiStall::update(float time, float dt)`
- **Nodes Driven:** 6 `TakoBall` nodes.
- **Continuous In-Place Spin:** $\Delta \theta_y = 120.0^\circ/\text{s} \cdot dt$.
- **Periodic Parabolic Hop:** Every $3.0\text{s}$, balls hop between griddle molds over duration $t_{\text{hop}} = 0.6\text{s}$. Normalized progress $\tau = t / t_{\text{hop}} \in [0, 1]$:
  $$\mathbf{p}(\tau) = \text{lerp}(\mathbf{p}_1, \mathbf{p}_2, \tau) + \begin{pmatrix} 0 \\ 4 h_{\text{apex}} \tau (1 - \tau) \\ 0 \end{pmatrix}$$
  $$\Delta \theta_x(\tau) = 360.0^\circ/\text{s} \cdot dt$$
- **Constants:** Jump apex height $h_{\text{apex}} = 0.35\text{m}$.
- **Transform Demonstrated:** Parabolic Translation $\to$ High-speed Flip Rotation.

### 8.5 Kakigori Shaver Crank & Dessert Bowl Presentation
- **File & Function:** `src/Objects.h: KakigoriStall::update(float time, float dt)`
- **Shaver Hand Wheel:** Constant crank spin: $\Delta \theta_x = 240.0^\circ/\text{s} \cdot dt$.
- **Active Shaved Ice Mound:** Continuous rotation $\Delta \theta_y = 90.0^\circ/\text{s} \cdot dt$; breathing scale:
  $$s(t) = 0.28 + 0.02 \cdot \sin(3.5 t)$$
- **6 Dessert Servings:** Continuous presentation spin: $\Delta \theta_y = 75.0^\circ/\text{s} \cdot dt$. Periodic hopping swap every $3.2\text{s}$ with parabolic arc ($h_{\text{apex}} = 0.32\text{m}$), $360^\circ$ mid-air spin, and playful tilt $\theta_z = 12.0^\circ \sin(\pi \tau)$.
- **Transform Demonstrated:** Combined Translation, Rotation, and Scaling.

### 8.6 Two-Stage Fireworks Launch & Explosive Burst Physics
- **File & Function:** `src/Objects.h: FireworkRocket::update(float dt)`
- **Stage 1 (Launch Ascent, $t_{\text{launch}} = 1.1\text{s}$):**
  $$y(t) = y(t) + (y_{\text{apex}} - y(t)) \cdot 4.0 \cdot dt$$
- **Stage 2 (Spherical Burst Explosion):** At apex ($Y \approx 28\text{m} - 43\text{m}$), 35 particles spawn with uniform spherical velocity vectors:
  $$\mathbf{v}_i = s \begin{pmatrix} \sin\theta \cos\phi \\ \cos\theta \\ \sin\theta \sin\phi \end{pmatrix}, \quad s \in [6.0, 14.0]\text{ m/s}$$
  Under constant downward gravity acceleration:
  $$\mathbf{p}_i(t + dt) = \mathbf{p}_i(t) + \mathbf{v}_i \cdot dt, \quad v_{i,y}(t + dt) = v_{i,y}(t) - 7.5 \cdot dt$$
  $$\text{scale}_i = 0.35 \cdot \left(1.0 - \frac{t_{\text{life}}}{t_{\text{max}}}\right)$$
- **Transform Demonstrated:** Particle Translation under Gravity $\to$ Dynamic Scaling Contraction.

### 8.7 Biomechanical Crowd Walk Cycles
- **File & Function:** `src/Objects.h: CrowdGroup::update(float time, float dt)`
- **Nodes Driven:** Root pedestrian, thighs, shins, arms.
- **Linear Translation:** $z(t + dt) = z(t) + \text{dir} \cdot v_{\text{walk}} \cdot dt$, wrapping between $[-30.0\text{m}, +36.0\text{m}]$.
- **Lateral Lane Sway:** $x(t) = x_{\text{lane}} + 0.08\text{m} \cdot \sin(0.8 t + \phi)$.
- **Vertical Step Bobbing:** $y(t) = 0.04\text{m} \cdot |\sin(6.0 t + \phi)|$.
- **Hip Joint Swing:** $\theta_{\text{thigh, L}} = 26.0^\circ \cdot \sin(6.0 t + \phi)$, $\theta_{\text{thigh, R}} = -\theta_{\text{thigh, L}}$.
- **Knee Flexion Kinematics:** When thigh swings backward ($\theta < 0$), knee bends backward to lift sandal ($\theta_{\text{shin}} = |\theta_{\text{thigh}}| \cdot 0.8$); when swinging forward, knee straightens for heel strike ($\theta_{\text{shin}} = 0^\circ$).

### 8.8 Falling Sakura Blossom Wind Drift
- **File & Function:** `src/Objects.h: SakuraTree::update(float time, float dt)`
- **Vertical Descent:** $y(t + dt) = y(t) - v_{\text{fall}} \cdot dt$ ($v_{\text{fall}} \in [0.45, 0.75]\text{ m/s}$).
- **Horizontal Wind Drift:**
  $$x(t) = x_{\text{base}} + A_{\text{sway}} \cdot \sin(1.8 t + \phi)$$
  $$z(t) = z_{\text{base}} + A_{\text{sway}} \cdot \cos(1.4 t + \phi)$$
- **Tumbling Rotation:** $(\theta_x, \theta_y, \theta_z) = (50 t + 30\phi, 65 t, 35 t)^\circ$.

### 8.9 Sliding Shoji Doors & Windows Kinematics
- **File & Function:** `src/Objects.h: MachiyaBuilding::update(float dt)`
- **Front Shoji Door:** Smooth exponential ease-in-out interpolation:
  $$p_{\text{door}}(t + dt) = \text{lerp}(p_{\text{door}}, \text{target}, \text{clamp}(5.0 \cdot dt, 0, 1))$$
  $$z_{\text{local}} = p_{\text{door}} \cdot 1.35\text{m}$$
- **Second-Floor Sliding Windows:**
  $$p_{\text{win}}(t + dt) = \text{lerp}(p_{\text{win}}, \text{target}, \text{clamp}(4.5 \cdot dt, 0, 1))$$
  $$\mathbf{p}_{\text{sash}} = \mathbf{p}_{\text{base}} + \Delta \mathbf{p} \cdot p_{\text{win}}$$

### 8.10 Motion Classification & Academic Rubric Matrix

| Animation Feature | Translation | Rotation | Scaling | Hierarchical Chaining | Relative-Frame Motion | Particle Physics |
|---|:---:|:---:|:---:|:---:|:---:|:---:|
| **Hanging Lantern Swing** | | Yes | | Yes | **Yes** | |
| **Floating Magic Orb** | Yes | Yes | | Yes | **Yes** | |
| **Vanishing Box Trick** | Yes | Yes | **Yes** | Yes | | |
| **Takoyaki Flipping** | Yes | Yes | | Yes | | |
| **Kakigori Shaver & Bowls** | Yes | Yes | Yes | Yes | | |
| **Fireworks Rockets & Bursts** | Yes | | Yes | | | **Yes** |
| **Crowd Walk Cycles** | Yes | Yes | | Yes | | |
| **Falling Sakura Petals** | Yes | Yes | | | | **Yes** |
| **Sliding Shoji Doors & Windows** | **Yes** | | | Yes | **Yes** | |
| **Vendor Cooking Articulation** | | Yes | | Yes | | |
| **Spotlight Tracking Rig** | | Yes | | Yes | **Yes** | |

---

## 9. Lighting, Shading and Illumination

### 9.1 Blinn-Phong Shading Pipeline
The lighting model is computed per-fragment in `shaders/basic.frag` using the **Blinn-Phong** reflection model. The specular term calculates the half-vector $\mathbf{H}$ between the light direction $\mathbf{L}$ and the view direction $\mathbf{V}$:
$$\mathbf{H} = \frac{\mathbf{L} + \mathbf{V}}{\|\mathbf{L} + \mathbf{V}\|}$$

For each active light source $i$, the fragment shader accumulates:
$$\mathbf{I}_i = \mathbf{I}_{\text{ambient}, i} + (1.0 - \text{shadow}) \cdot \left[ \mathbf{I}_{\text{diffuse}, i} \cdot \max(\mathbf{N} \cdot \mathbf{L}_i, 0) + \mathbf{I}_{\text{specular}, i} \cdot k_s \cdot (\max(\mathbf{N} \cdot \mathbf{H}_i, 0))^\alpha \right] \cdot \text{attenuation}_i$$
where $\alpha$ is `material.shininess` and $k_s$ is `material.specularStrength`.

- **Indoor Indirect Skylight Bounce:** To prevent pitch-black ambient shadow artifacts inside the townhouses, `CalcDirLight` incorporates an ambient bounce:
  $$\text{indoorBounce} = \text{mix}(\text{vec3}(0.48, 0.45, 0.40), \text{vec3}(0.18, 0.16, 0.14), \text{dayNightFactor}) \cdot \mathbf{C}_{\text{diffuse}}$$
  $$\mathbf{I}_{\text{ambient}} = \max(\mathbf{L}_{\text{ambient}} \cdot \mathbf{C}_{\text{diffuse}}, \text{indoorBounce})$$

### 9.2 Live Shading Modes
Pressing <kbd>P</kbd> cycles the uniform `shadingMode` live in `basic.frag`:
1. `shadingMode = 0` (**Blinn-Phong Illumination**): Full Ambient + Lambertian Diffuse + Specular highlights with PCF soft shadows.
2. `shadingMode = 1` (**Diffuse Only**): Ambient + Diffuse terms only. Specular highlights disabled to demonstrate pure matte Lambertian reflectance.
3. `shadingMode = 2` (**Ambient Only**): Flat ambient baseline illumination for report visual comparisons.

### 9.3 Complete Light Source Specification (14 Dynamic Point Lights & 16 Total Lights)

```
+-------------------------------------------------------------------------------------------------------+
| Light ID | Type        | Color / Diffuse (RGB)    | World Position / Origin     | Attenuation / Range     |
+-------------------------------------------------------------------------------------------------------+
| DirLight | Directional | Lerp: Sun (0.85,0.82,0.76)| Celestial Sweeping Arc      | Infinite (Normalized)   |
|          |             |   to Moon (0.20,0.25,0.38)| (dir tracked per frame)     |                         |
+-------------------------------------------------------------------------------------------------------+
| Point #0 | Point       | Cyan: (0.35, 0.85, 1.0)  | Tracks Floating Magic Orb   | Kc=1.0, Kl=0.14, Kq=0.07|
| Point #1 | Point       | Amber: (1.0, 0.60, 0.22) | Takoyaki Stall Eaves (-5.2) | Kc=1.0, Kl=0.10, Kq=0.04|
| Point #2 | Point       | Cyan: (0.35, 0.90, 1.0)  | Kakigori Stall Eaves (+5.2) | Kc=1.0, Kl=0.10, Kq=0.04|
| Point #3 | Point       | Orange: (1.0, 0.55, 0.20)| Swinging Lantern Span 1     | Kc=1.0, Kl=0.09, Kq=0.03|
| Point #4 | Point       | Orange: (1.0, 0.55, 0.20)| Swinging Lantern Span 3     | Kc=1.0, Kl=0.09, Kq=0.03|
| Point #5 | Point       | Dynamic Burst Color      | Fireworks Detonation Apex   | Kc=1.0, Kl=0.04, Kq=0.01|
| Point #6 | Point       | Warm Amber: (1.45,1.20,0)| Machiya_L1 Living Room      | Kc=1.0, Kl=0.08, Kq=0.02|
| Point #7 | Point       | Warm Amber: (1.35,1.10,0)| Machiya_L1 Upper Bedroom    | Kc=1.0, Kl=0.08, Kq=0.02|
| Point #8 | Point       | Warm Amber: (1.45,1.20,0)| Machiya_R1 Living Room      | Kc=1.0, Kl=0.08, Kq=0.02|
| Point #9 | Point       | Warm Amber: (1.35,1.10,0)| Machiya_R1 Upper Bedroom    | Kc=1.0, Kl=0.08, Kq=0.02|
| Point #10| Point       | Cozy Glow: (1.35,1.10,0) | Machiya_L2 Living Room      | Kc=1.0, Kl=0.08, Kq=0.02|
| Point #11| Point       | Cozy Glow: (1.35,1.10,0) | Machiya_R2 Living Room      | Kc=1.0, Kl=0.08, Kq=0.02|
| Point #12| Point       | Radiant Gold: (1.50,1.05)| Torii Gate Left Lantern     | Kc=1.0, Kl=0.07, Kq=0.018|
| Point #13| Point       | Radiant Gold: (1.50,1.05)| Torii Gate Right Lantern    | Kc=1.0, Kl=0.07, Kq=0.018|
+-------------------------------------------------------------------------------------------------------+
| SpotLight| Spotlight   | Warm Stage: (1.2,1.1,0.9)| Stage Rig Cone Housing      | Inner: 15°, Outer: 22°  |
|          |             |                          | (tracks Magician yaw/pitch) | Kc=1.0, Kl=0.06, Kq=0.01|
+-------------------------------------------------------------------------------------------------------+
```

#### Attenuation Formulation
Point and spotlights apply quadratic physical distance attenuation:
$$\text{attenuation} = \frac{1.0}{k_c + k_l \cdot d + k_q \cdot d^2}$$
If distance squared exceeds cutoff threshold ($d^2 > 700\text{ m}^2$), lighting evaluation early-outs to conserve GPU fragment cycles.

#### Spotlight Cutoff Formulation
$$\theta = \mathbf{L} \cdot (-\mathbf{D}_{\text{spot}})$$
$$\epsilon = \cos(\theta_{\text{inner}}) - \cos(\theta_{\text{outer}})$$
$$\text{spotIntensity} = \text{clamp}\left(\frac{\theta - \cos(\theta_{\text{outer}})}{\epsilon}, 0.0, 1.0\right)$$
where $\cos(\theta_{\text{inner}}) = 0.9659$ ($15^\circ$) and $\cos(\theta_{\text{outer}}) = 0.9272$ ($22^\circ$).

### 9.4 Day/Night Solar & Lunar Blend Machine
Triggered via <kbd>N</kbd>, `dayNightFactor` ($0.0 = \text{Day}, 1.0 = \text{Night}$) advances at speed $1.5\text{ s}^{-1}$ in `Scene::update()`:
- **Directional Light Direction:** Interpolates between Sun direction $(0.40, -0.85, -0.50)$ and Moon direction $(-0.35, -0.75, 0.40)$.
- **Sky Dome Gradient:** Interpolates daytime cyan-azure $(0.52, 0.72, 0.96) \to (0.78, 0.86, 0.98)$ to midnight indigo $(0.015, 0.02, 0.06) \to (0.04, 0.06, 0.15)$.
- **Shoji Rice Paper Windows:** Blends neutral off-white paper in daylight $(0.90, 0.88, 0.82)$ to glowing amber at night $(1.0, 0.82, 0.45)$.
- **Lantern & Stall Lights:** Intensities scale dynamically: $\text{scale} = \text{mix}(0.20, 1.20, \text{dayNightFactor})$. Pressing <kbd>0</kbd> or <kbd>KP_0</kbd> manually dims/brightens festival lights.

### 9.5 16-Sample PCF Soft Shadow Mapping
- **Framebuffer Architecture:** Dedicated $2048 \times 2048$ 32-bit depth texture (`depthMap`) bound to `depthMapFBO`.
- **Projection:** Orthographic box $[-24, 24] \times [-24, 24]$ with near $0.1\text{m}$, far $90.0\text{m}$.
- **Adaptive Slope-Scale Depth Bias:**
  $$\text{bias} = \max(0.0035 \cdot (1.0 - \mathbf{N} \cdot \mathbf{L}), 0.0006)$$
- **16-Sample Poisson/Grid Kernel:** Evaluates a $4 \times 4$ kernel centered at projected light coordinates with texel radius $1.2 / 2048$, producing smooth penumbra transitions across street curbs, building walls, and stall awnings.

### 9.6 Comprehensive Materials Matrix

| Object / Sub-Part | Ambient | Diffuse / Tint | Specular | Shininess ($\alpha$) | Specular Strength ($k_s$) | Emissive Color | Texture Asset |
|---|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| **Street Cobblestones** | Auto | $(1.0, 1.0, 1.0)$ | $(0.2, 0.2, 0.2)$ | 16.0 | 0.20 | None | `stone_pavement.bmp` (14x) |
| **Machiya Beams & Pillars** | Auto | $(0.45, 0.32, 0.22)$ | $(0.3, 0.3, 0.3)$ | 20.0 | 0.25 | None | `wood_timber.bmp` (2.5x) |
| **Machiya Ceramic Roof Tiles**| Auto | $(0.35, 0.38, 0.42)$ | $(0.5, 0.5, 0.5)$ | 32.0 | 0.45 | None | `roof_tiles.bmp` (5.0x) |
| **Machiya Interior Tatami** | Auto | $(0.55, 0.58, 0.45)$ | $(0.2, 0.2, 0.2)$ | 16.0 | 0.15 | None | `tatami_cloth.bmp` (4.0x) |
| **Shoji Window Rice Paper** | 1.0 | Window Lerp | $(0.1, 0.1, 0.1)$ | 8.0 | 0.10 | Amber Glow | None (`isWindow = true`) |
| **Paper Lantern Bodies** | 1.0 | $(1.0, 0.28, 0.15)$ | $(0.4, 0.4, 0.4)$ | 16.0 | 0.30 | $(1.0, 0.45, 0.15)$ | `lantern_paper.bmp` (1.0x) |
| **Torii Gate Vermilion Wood**| Auto | $(0.85, 0.22, 0.12)$ | $(0.4, 0.4, 0.4)$ | 28.0 | 0.35 | None | `wood_timber.bmp` (3.0x) |
| **Torii Tablet Plaque** | Auto | $(0.95, 0.82, 0.25)$ | $(0.9, 0.9, 0.9)$ | 64.0 | 0.90 | None | `gold_leaf.bmp` (1.0x) |
| **Torii Hanging Lanterns** | 1.0 | $(0.92, 0.18, 0.12)$ | $(0.4, 0.4, 0.4)$ | 32.0 | 0.40 | $(2.6, 1.4, 0.5)$ | `lantern_paper.bmp` (1.0x) |
| **Torii Stone Lantern Flame**| 1.0 | $(1.0, 0.85, 0.40)$ | $(0.8, 0.8, 0.8)$ | 64.0 | 0.90 | $(3.5, 2.4, 1.0)$ | None (`isEmissive = true`) |
| **Sakura Tree Bark** | Auto | $(0.38, 0.28, 0.22)$ | $(0.2, 0.2, 0.2)$ | 12.0 | 0.15 | None | `sakura_bark.bmp` (2.0x) |
| **Sakura Petals & Foliage** | Auto | $(0.98, 0.72, 0.82)$ | $(0.2, 0.2, 0.2)$ | 12.0 | 0.15 | None | None |
| **Takoyaki Cast Iron Griddle**| Auto | $(0.18, 0.18, 0.20)$ | $(0.9, 0.9, 0.9)$ | 64.0 | 0.90 | None | None |
| **Takoyaki Food Balls** | Auto | $(0.82, 0.55, 0.25)$ | $(0.7, 0.7, 0.7)$ | 64.0 | 0.70 | None | `takoyaki_food.bmp` (1.0x) |
| **Magic Stage Gold Screen** | Auto | $(0.95, 0.82, 0.25)$ | $(0.9, 0.9, 0.9)$ | 48.0 | 0.85 | None | `gold_leaf.bmp` (2.0x) |
| **Magic Orb** | 1.0 | $(0.25, 0.85, 1.0)$ | $(1.0, 1.0, 1.0)$ | 128.0 | 1.00 | $(0.35, 0.85, 1.0)$| None (`isEmissive = true`)|
| **Magic Crimson Silk Cloth** | Auto | $(0.85, 0.15, 0.25)$ | $(0.3, 0.3, 0.3)$ | 16.0 | 0.30 | None | `tatami_cloth.bmp` (2.0x) |
| **Spotlight Cone Lens** | 1.0 | $(1.0, 1.0, 0.9)$ | $(1.0, 1.0, 1.0)$ | 96.0 | 1.00 | $(1.2, 1.1, 0.9)$ | None (`isEmissive = true`)|

---

## 10. Texturing

### 10.1 Texture Management & Procedural Fallbacks
- **Implementation:** `src/Texture.h` encapsulates OpenGL 2D texture operations.
- **Image Decoding:** `stbi_load` decodes 24-bit uncompressed `.bmp` assets from disk with vertical flip enabled (`stbi_set_flip_vertically_on_load(true)`).
- **Filtering & Mipmaps:** Linear minification filtering with trilinear mipmap interpolation (`GL_LINEAR_MIPMAP_LINEAR`) and linear magnification filtering (`GL_LINEAR`).
- **Wrap Modes:** Repeating textures (timber, pavement, tatami, roof tiles) use `GL_REPEAT`; clamped assets use `GL_CLAMP_TO_EDGE`.
- **Procedural Synthesizer (`TextureGenerator.h`):** If any `.bmp` asset is missing upon boot, `ensureTextureAssetsExist()` generates authentic procedurally synthesized 24-bit BMP assets on disk using sinusoidal grain math and pseudo-noise formulas.

### 10.2 UV Mapping Parametrization
Base primitives supply normalized UV coordinates:
- **Cube:** Each face maps $[0, 0]$ (bottom-left) to $[1, 1]$ (top-right).
- **Cylinder:** Mantle maps $u = \theta / 2\pi \in [0, 1]$ and $v \in [0, 1]$. End disc caps map radially: $(u, v) = (0.5 + 0.5\cos\theta, 0.5 + 0.5\sin\theta)$.
- **Cone:** Mantle maps $u = \theta / 2\pi$, apex at $v = 1$, base at $v = 0$.
- **Sphere:** Equirectangular projection: $u = \theta / 2\pi$, $v = (\phi + \pi/2) / \pi$.
- **Plane:** Grid subdivision maps $u = x / \text{width}$, $v = z / \text{depth}$.

### 10.3 Active Texture Assets Table

| Texture File | Format & Resolution | Texture Unit | Bound Scene Geometry | Tiling Scale |
|---|:---:|:---:|---|:---:|
| `wood_timber.bmp` | 24-bit RGB, $256 \times 256$ | `GL_TEXTURE0` | Machiya beams, stall frames, vendor stands, Torii posts | $2.5\text{x} - 3.0\text{x}$ |
| `roof_tiles.bmp` | 24-bit RGB, $256 \times 256$ | `GL_TEXTURE0` | Machiya townhouse sloped roofs and eaves | $5.0\text{x}$ |
| `stone_pavement.bmp` | 24-bit RGB, $256 \times 256$ | `GL_TEXTURE0` | Ground street pavement, curb stones, lantern bases | $14.0\text{x}$ |
| `lantern_paper.bmp` | 24-bit RGB, $256 \times 256$ | `GL_TEXTURE0` | Overhead street lanterns, stall lanterns | $1.0\text{x}$ |
| `tatami_cloth.bmp` | 24-bit RGB, $256 \times 256$ | `GL_TEXTURE0` | Interior floors, magic stage carpet, stall awnings | $2.0\text{x} - 4.0\text{x}$ |
| `gold_leaf.bmp` | 24-bit RGB, $256 \times 256$ | `GL_TEXTURE0` | Magic stage folding screen (*byobu*), vanishing box | $1.0\text{x} - 2.0\text{x}$ |
| `sakura_bark.bmp` | 24-bit RGB, $256 \times 256$ | `GL_TEXTURE0` | Cherry blossom tree trunks and heavy boughs | $2.0\text{x}$ |
| `takoyaki_food.bmp` | 24-bit RGB, $256 \times 256$ | `GL_TEXTURE0` | 6 takoyaki spheres on the griddle plate | $1.0\text{x}$ |
| `hud_font.bmp` | 8-bit Alpha, $256 \times 256$ | `GL_TEXTURE0` (UI) | Consolas Bold font atlas for 2D HUD text quads | $1.0\text{x}$ |
| `shadowMap` (FBO) | 32-bit Depth, $2048 \times 2048$ | `GL_TEXTURE1` | Directional light shadow depth buffer | $1.0\text{x}$ |

### 10.4 Texture & Material Modulation
In `shaders/basic.frag`, texture samples are multiplied by the object's base color:
```glsl
// basic.frag: lines 324-329
vec4 baseColor = objectColor;
if (enableTextures && useTexture)
{
    vec4 texColor = texture(diffuseTexture, TexCoords * textureTiling);
    baseColor = texColor * objectColor;
}
```
This enables tinting textured surfaces (e.g., staining wood beams dark or golden) while preserving surface grain. Pressing <kbd>X</kbd> toggles `enableTextures` off, reverting surfaces to clean untextured Phong materials.

---

## 11. Camera, Input and Interaction

### 11.1 FPS Fly/Walk Camera System
Implemented in `src/Camera.h`:
- **Position & Vectors:** Maintains `Position`, `Front`, `Up`, `Right`, and `WorldUp`.
- **Projection:** Perspective matrix generated via `glm::perspective(glm::radians(Zoom), aspectRatio, 0.1f, 300.0f)`.
- **Speed & Sensitivity:** Movement speed $= 12.0\text{ m/s}$, mouse sensitivity $= 0.1^\circ/\text{pixel}$.
- **Pitch Clamping:** Constrained between $-89.0^\circ$ and $+89.0^\circ$ to prevent viewport flipping.
- **Field of View Zoom:** Mouse scroll adjusts `Zoom` between $1.0^\circ$ (telephoto) and $60.0^\circ$ (wide angle).
- **Camera Viewpoint Presets:**
  - <kbd>1</kbd>: **Festival Street Entrance** $\to$ `Pos(0.0, 3.5, 26.0)`, `Yaw -90.0°`, `Pitch -2.0°`.
  - <kbd>2</kbd>: **Magic Show Stage** $\to$ `Pos(6.2, 2.2, -10.5)`, `Yaw -90.0°`, `Pitch 2.0°`.
  - <kbd>3</kbd>: **Torii Gate & Sky** $\to$ `Pos(0.0, 2.5, -18.0)`, `Yaw -90.0°`, `Pitch 25.0°`.
  - <kbd>R</kbd>: **Reset Origin** $\to$ Restores camera back to entrance coordinates.

### 11.2 Comprehensive Controls Keymap

| Key / Input | Category | Function / Action | Code Location |
|:---:|:---:|---|---|
| **<kbd>W</kbd> / <kbd>A</kbd> / <kbd>S</kbd> / <kbd>D</kbd>** | Navigation | Fly camera Forward / Strafe Left / Backward / Strafe Right | `Main.cpp: processContinuousInput` |
| **<kbd>E</kbd> / <kbd>Q</kbd>** | Navigation | Ascend camera Vertically Up / Descend Down | `Main.cpp: processContinuousInput` |
| **Mouse Move** | Navigation | Rotate camera Pitch and Yaw (FPS-style mouse look) | `Main.cpp: mouse_callback` |
| **Mouse Scroll** | Navigation | Zoom in / Zoom out ($1.0^\circ \le \text{FOV} \le 60.0^\circ$) | `Main.cpp: scroll_callback` |
| **<kbd>C</kbd>** | Navigation | Toggle mouse cursor lock / unlock | `Main.cpp: key_callback` |
| **<kbd>1</kbd> / <kbd>2</kbd> / <kbd>3</kbd>** | Navigation | Preset Camera Viewpoints (Street, Stage, Torii & Sky) | `Main.cpp: key_callback` |
| **<kbd>R</kbd>** | Navigation | Reset camera position to street entrance origin | `Main.cpp: key_callback` |
| **<kbd>B</kbd>** | Physics | Toggle **Wall Collision Mode** (Solid walls & stairs $\longleftrightarrow$ Noclip) | `Scene.h: toggleCollision()` |
| **<kbd>F1</kbd>** | Interface | **Toggle In-Window Semi-Transparent HUD Overlay** (Top-right) | `Hud.h: toggleVisibility()` |
| **<kbd>F10</kbd>** | Capture | **Capture Viewport Screenshot** (`screenshot_clean.bmp`; <kbd>Shift+F10</kbd> with HUD) | `Main.cpp: key_callback` |
| **<kbd>Space</kbd>** | Simulation | Pause / Resume all kinematics and animations | `Scene.h: togglePause()` |
| **<kbd>N</kbd>** | Lighting | Smooth Day $\longleftrightarrow$ Festival Night transition | `Scene.h: toggleDayNight()` |
| **<kbd>0</kbd> / <kbd>KP_0</kbd>** | Lighting | **Toggle Lantern & Stall Illumination** (Lights ON / Dimmed) | `Scene.h: toggleLanternLights()` |
| **<kbd>P</kbd>** | Shading | Cycle Shading Model (Blinn-Phong $\to$ Diffuse Only $\to$ Ambient Only) | `Scene.h: cycleShadingMode()` |
| **<kbd>X</kbd>** | Texturing | Toggle Texturing (Textures ON / OFF) | `Scene.h: toggleTextures()` |
| **<kbd>V</kbd>** | Shadows | Toggle Realistic 16-Sample PCF Soft Shadows (ON / OFF) | `Scene.h: toggleShadows()` |
| **<kbd>Z</kbd>** | Ray Tracing | Toggle Real-Time GPU Whitted Ray Tracing Mode (ON / OFF) | `Scene.h: toggleRayTracing()` |
| **<kbd>F9</kbd>** | Ray Tracing | Capture & Export CPU Ray-Traced Snapshot to `raytraced_snapshot.bmp`| `Scene.h: captureCPURayTracedSnapshot()`|
| **<kbd>H</kbd>** | Interaction | **Slide Open / Close Nearest Shoji Door** (Smooth local translation) | `Scene.h: interactNearestDoor()` |
| **<kbd>G</kbd>** | Interaction | **Slide Open / Close Nearest Shoji Windows** (Smooth local translation)| `Scene.h: interactNearestWindow()` |
| **<kbd>M</kbd>** | Interaction | **Replay Magic Show Trick Sequence** (Restart vanishing box & orb) | `Scene.h: replayMagicTrick()` |
| **<kbd>F</kbd>** | Effects | Manually trigger a Firework rocket launch & apex burst | `Scene.h: triggerFirework()` |
| **<kbd>T</kbd>** | Inspection | **Cycle Selected Target Object** (<kbd>Shift+T</kbd>: Proximity Auto-Select)| `InteractionManager.cpp: cycleSelection()` |
| **<kbd>J</kbd> / <kbd>L</kbd>** | Transform | Translate selected object along $\pm X$ (Left / Right) | `Main.cpp: processContinuousInput` |
| **<kbd>I</kbd> / <kbd>K</kbd>** | Transform | Translate selected object along $\pm Y$ (Up / Down) | `Main.cpp: processContinuousInput` |
| **<kbd>U</kbd> / <kbd>O</kbd>** | Transform | Translate selected object along $\pm Z$ (Forward / Backward) | `Main.cpp: processContinuousInput` |
| **<kbd>↑</kbd> / <kbd>↓</kbd>** | Transform | Rotate selected object Pitch (around X axis) | `Main.cpp: processContinuousInput` |
| **<kbd>←</kbd> / <kbd>→</kbd>** | Transform | Rotate selected object Yaw (around Y axis) | `Main.cpp: processContinuousInput` |
| **<kbd>+</kbd> / <kbd>-</kbd>** or **<kbd>[</kbd> / <kbd>]</kbd>** | Transform | Scale selected object up (+10%) / down (-10%) | `Main.cpp: key_callback` |
| **<kbd>Esc</kbd>** | System | Exit application cleanly | `Main.cpp: processContinuousInput` |

### 11.3 In-Window Minimalist HUD System
Implemented in `src/ui/Hud.h` and `src/ui/Hud.cpp`:
- **Placement & Styling:** Anchored to the top-right corner using viewport coordinates ($x \in [\text{width} - 240, \text{width} - 12]$, $y \in [12, 115]$). Features a sleek, semi-transparent dark glass card ($72\%$ alpha, soft steel-blue border line, rounded padding).
- **Embedded Consolas Bold Atlas:** Samples glyphs from an embedded $256 \times 256$ texture atlas (`FontAtlasData.h`) with a dedicated GLSL shader (`hud.vert`, `hud.frag`). Text alpha is boosted via `smoothstep(0.12, 0.45, a)` to render crisp, fully opaque characters.
- **10 Hz Rate Limiting:** Geometry rebuilds at a maximum of 10 Hz (`timeSinceLastUpdate >= 0.10f`), only when content changes (`currentHash != cachedContentHash`). Eliminates per-frame CPU/GPU memory churning.
- **Telemetry Readout (5 Minimal Lines):**
  1. `FPS: xx.x` (Real-time framerate)
  2. `Target: [Object Name]` (Currently focused inspectable)
  3. `[T] Toggle target` (Selection cycle instruction)
  4. `[0] Lights: ON/OFF` (Live lighting status)
  5. Context-sensitive action hint (e.g., `[H] Slide Open Shoji Door`, `[M] Replay Magic Trick`)
  6. `[F1] Hide HUD` (Visibility toggle footer)

### 11.4 Context-Sensitive Interaction & Object Selection
Implemented in `src/ui/InteractionManager.cpp`:
- **Dual Selection Modes:**
  1. **Automatic Proximity & View-Cone Selection:** When the camera moves, the manager calculates a gaze dot product:
     $$\text{dot} = \mathbf{V}_{\text{front}} \cdot \frac{\mathbf{P}_{\text{obj}} - \mathbf{P}_{\text{cam}}}{\|\mathbf{P}_{\text{obj}} - \mathbf{P}_{\text{cam}}\|}$$
     If within interaction radius ($d \le R_{\text{interact}}$) and within view cone ($\text{dot} \ge 0.45$, $\approx 63^\circ$ half-angle), selects the best candidate based on composite score:
     $$\text{score} = \frac{d}{\text{dot} + 0.2}$$
  2. **Manual Locked Selection:** Pressing <kbd>T</kbd> cycles sequentially across all 15 inspectables, synchronizing `scene.selectedIndex`. Pressing <kbd>Shift+T</kbd> unlocks manual mode and returns to automatic proximity selection.

#### The 15 Controllable Inspectables (`Scene::inspectables`):
1. `1. Lantern [Body]` (Child of Swinging Rope Pivot)
2. `2. Lantern [Rope Pivot]` (Parent Anchor Node)
3. `3. Magic Orb` (Child of Magician's Hand Bone)
4. `4. Magician Figure (Root)`
5. `5. Vanishing Box` (Scale-to-zero demo)
6. `6. Stage Spotlight Housing` (Aiming pivot)
7. `7. Takoyaki Stall` (Full Unit)
8. `8. Kakigori Stall` (Full Unit + Shaved Ice Machine)
9. `9. Torii Gate` (Grand Entrance)
10. `10. Sakura Blossom Tree`
11. `11. Crowd Walker #1`
12. `12. Machiya_L1` (Townhouse with Door & Windows)
13. `13. Machiya_L2` (Townhouse with Door & Windows)
14. `14. Machiya_R1` (Townhouse with Door & Windows)
15. `15. Machiya_R2` (Townhouse with Door & Windows)

### 11.5 Continuous Collision Detection & Portal Navigation
Implemented in `Scene::resolveCollision(oldPos, newPos)`:
- **AABB Wall Physics:** Projects player movement into local coordinates for each townhouse building. Solves collisions against exterior walls: Back wall ($X = -4.0\text{m}$), Left wall ($Z = +4.5\text{m}$), Right wall ($Z = -4.5\text{m}$), and Front wall ($X = +4.0\text{m}$).
- **Interactive Doorway Portals:** On the front wall ($Z \in [-2.05, 0.25]$, $Y \in [0.0, 2.55]$), the collision barrier checks `bld->isDoorOpen`. When the door is closed, the wall blocks player penetration; when the Shoji door is slid open (<kbd>H</kbd>), the portal opens, allowing the player to walk seamlessly inside.
- **Interior Navigation & Staircases:** When inside the building footprint, evaluates staircase bounds ($X \in [-2.0, 0.85]$, $Z \in [-4.15, -2.85]$), adjusting camera ground height step-by-step ($Y \in [0.45\text{m}, 4.25\text{m}]$) to climb up to the second-floor tatami bedroom.
- **Noclip Fly Mode:** Pressing <kbd>B</kbd> disables collision checking for aerial inspection.

### 11.6 Viewport Screenshot Engine
- **Clean Screenshots (<kbd>F10</kbd>):** Dispatches `captureViewportScreenshot()` immediately before the HUD overlay renders. Reads back raw pixels via `glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE)`, flips rows vertically, and writes an uncompressed 24-bit BMP image directly to `screenshot_clean.bmp`.
- **HUD Screenshots (<kbd>Shift+F10</kbd>):** Captures after the HUD pass, writing to `screenshot_hud.bmp`.

---

## 12. Advanced Optics: Real-Time GPU Ray Tracer & CPU Snapshot

### 12.1 Real-Time GPU Whitted Ray Tracing Pipeline
Implemented in `shaders/raytrace.vert` and `shaders/raytrace.frag`, engaged via <kbd>Z</kbd>:
- **Ray Generation:** Full-screen quad pixel shader synthesizes primary camera rays:
  $$\mathbf{R}_{\text{dir}} = \text{normalize}(\mathbf{V}_{\text{front}} + (2u - 1)\tan(\text{FOV}/2)\cdot\text{aspect}\cdot\mathbf{V}_{\text{right}} + (1 - 2v)\tan(\text{FOV}/2)\cdot\mathbf{V}_{\text{up}})$$
- **Analytical Intersections:**
  - *Ray-Sphere:* Evaluates quadratic discriminant $\Delta = b^2 - c$.
  - *Ray-Box (Slab Method):* Evaluates near/far slab intersections $t_0, t_1 = (\mathbf{B}_{\text{min, max}} - \mathbf{R}_o) / \mathbf{R}_d$.
  - *Ray-Cylinder:* Intersects radial quadratic on $XZ$ plane and clips against vertical height interval $[0, h]$.
  - *Ray-Plane:* Evaluates horizontal plane intersection $t = -R_{o, y} / R_{d, y}$.
- **Shadow Rays:** Evaluates occlusion rays towards the directional sun/moon and point lights.
- **Recursive Specular Reflections:** Traces up to 3 recursive reflection bounces ($\mathbf{R}_{\text{refl}} = \text{reflect}(\mathbf{R}_d, \mathbf{N})$) off the polished magic orb, stage platform, and gold-trimmed vanishing box.
- **Performance:** Runs smoothly at 60+ FPS on modern desktop GPUs.

### 12.2 Multi-Threaded CPU Ray Tracing Snapshot Generator
Implemented in `src/RayTracer.h`, triggered via <kbd>F9</kbd>:
- **Multi-Threading:** Distributes scanlines across `std::thread::hardware_concurrency()` threads.
- **High-Precision Image Output:** Renders a clean $1920 \times 1080$ frame and exports it directly to `raytraced_snapshot.bmp` without rasterization artifacts.

---

## 13. Live Demonstration Guide

### 13.1 Step-by-Step Examiner Evaluation Walkthrough

```
========================================================================================
                      CSE4102 LIVE EXAMINER EVALUATION SCRIPT
========================================================================================

1. INITIAL SCENE INSPECTION & CAMERA NAVIGATION:
   - Run the application. Camera starts at Preset 1 (Street Entrance, Z = 26.0m).
   - Use W/A/S/D to walk down the stone-paved street; mouse to look around.
   - Press [1], [2], [3] to demonstrate instant camera presets (Entrance, Stage, Torii).
   - Press [F1] to show/hide the HUD overlay in the top-right corner.

2. RELATIVE REFERENCE FRAME TRANSFORMATION DEMONSTRATION:
   - Walk up to Street Lantern Span 2 (Z = 11.0m) or press [T] until "Lantern [Body]" is selected.
   - Point out that the lantern body's pendulum swing is a child rotation relative
     to the catenary rope anchor node.
   - Walk up to the Magic Stage (press [2]). Frame the Magician:
     - Point out the glowing cyan Magic Orb orbiting the magician's articulated hand bone.
     - The orb's helical equation executes strictly relative to the moving hand frame.
     - Note that Point Light #0 dynamically tracks the orb, lighting the magician's face.

3. SCALE-TO-ZERO & POSITION SWAP DEMONSTRATION:
   - Press [M] to restart the Magic Show sequence from Phase 1.
   - Watch the table: the silk cloth descends -> the golden box smoothly scales down to 0
     (disappears completely) -> cloth whips aside -> box reappears at Spot 2 (scales from 0 to 1).

4. MOVING LIGHT SOURCE & DYNAMIC ILLUMINATION DEMONSTRATION:
   - Observe the Stage Spotlight above the platform: the conical housing rotates and tilts
     in real-time to track the magician, continuously shifting specular highlights across the stage.
   - Look up at the swinging lanterns at night: Point Lights #3 & #4 physically swing with the
     lantern bodies, modulating specular reflections across the stone cobblestones.

5. DAYLIGHT <-> FESTIVAL NIGHT TRANSITION:
   - Press [N]. Watch the 1.5-second smooth environmental transition:
     - Directional sunlight sweeps into cool blue moonlight.
     - Sky dome shifts from bright blue to dark indigo with twinkling stars.
     - Paper lanterns and Shoji windows begin radiating an intense warm golden emissive glow.
     - Fireworks automatically begin launching and detonating in the sky.
   - Press [0] or [Numpad 0] to toggle the festival lanterns and stall lights on and off live.

6. MATERIAL & SHADING MODEL COMPARISON:
   - Stand near the takoyaki griddle plate or Torii gate.
   - Press [P] to cycle shading modes:
     - Mode 0: Full Blinn-Phong (specular highlights visible).
     - Mode 1: Diffuse Only (matte Lambertian reflectance, no specular).
     - Mode 2: Ambient Only (flat ambient baseline).
   - Press [X] to toggle textures on/off (compare raw Phong materials vs textured surfaces).
   - Press [V] to toggle 16-sample PCF soft shadows on/off.

7. INTERACTIVE SHOJI DOORS, WINDOWS & WALL COLLISION:
   - Walk up to the front entrance of Machiya_L1 (left building).
   - Try walking through the front wall: collision blocks penetration (solid wall).
   - Press [H]: the Shoji front door smoothly slides open along its track.
   - Walk through the opened doorway: portal pass-through allows seamless entry inside.
   - Explore the tatami interior; walk up the 14-step staircase to the upper floor.
   - Press [G] to slide the upper windows open. Press [B] to toggle noclip fly mode.

8. LIVE MANUAL 6-DOF TRANSFORMATION OF CONTROLLABLE OBJECTS:
   - Press [T] to select any object (e.g., Takoyaki Stall, Torii Gate, or Magician).
   - Use [J] / [L] to translate along X, [I] / [K] along Y, [U] / [O] along Z.
   - Use [Arrow Keys] to rotate Pitch and Yaw live.
   - Use [+] / [-] to scale the selected object up or down by 10%.
   - Press [Shift+T] to return to proximity auto-selection mode.

9. ADVANCED OPTICS: REAL-TIME GPU RAY TRACER:
   - Press [Z] to toggle the real-time GPU Whitted ray tracer at 60+ FPS.
   - Observe analytical sphere/box intersections, hard shadow rays, and recursive reflections.
   - Press [F9] to export a high-resolution CPU ray-traced snapshot to raytraced_snapshot.bmp.
   - Press [F10] to export a clean rasterized screenshot to screenshot_clean.bmp.
========================================================================================
```

### 13.2 Rapid Code Customization Cheatsheet
If an examiner requests modifying a parameter live during the evaluation:

- **Change Lantern Swing Speed or Angle:** Edit `src/Objects.h: LanternObject`
  - Amplitude: `swingAmp = 12.0f;` (line 2471)
  - Frequency: `swingFreq = 2.0f;` (line 2470)
- **Change Magic Orb Orbit Radius or Speed:** Edit `src/Objects.h: Magician::update()`
  - Radius: `float r = 0.45f;` (line 3899)
  - Speed: `float speed = 3.2f;` (line 3900)
- **Change Day/Night Transition Speed:** Edit `src/Scene.h: Scene`
  - Speed: `float dayNightSpeed = 1.5f;` (line 73)
- **Change Spotlight Cutoff Angles:** Edit `src/Light.h: SpotLight`
  - Inner cone: `cutOff = 0.9659f;` (line 35, $\cos 15^\circ$)
  - Outer cone: `outerCutOff = 0.9272f;` (line 36, $\cos 22^\circ$)
- **Change Walking Crowd Speed:** Edit `src/Objects.h: CrowdGroup`
  - Speed: `wp.speed = 2.2f;` (line 4505)

---

## 14. Plan vs Implementation

### 14.1 Features Implemented Beyond Original Plan
The completed codebase significantly exceeds the scope outlined in the initial `Plan.md`:
1. **Real-Time GPU Whitted Ray Tracing (<kbd>Z</kbd>):** Implemented via a dedicated full-screen fragment shader (`shaders/raytrace.frag`) supporting primary ray generation, analytical intersections, shadows, and recursive reflections at 60+ FPS.
2. **Multi-Threaded CPU Ray Tracer Snapshot Generator (<kbd>F9</kbd>):** Implemented in `src/RayTracer.h` utilizing standard C++ threads to export high-precision BMP images.
3. **16-Sample PCF Soft Shadow Mapping (<kbd>V</kbd>):** $2048 \times 2048$ depth framebuffer FBO with slope-scaled depth bias and Poisson penumbra filtering (`shaders/shadow_depth.*`).
4. **Continuous Collision Detection & Interactive Portals (<kbd>B</kbd>):** Full AABB wall physics with dynamic doorway portals when Shoji doors are opened (<kbd>H</kbd>).
5. **Interactive Shoji Doors (<kbd>H</kbd>) & Sliding Windows (<kbd>G</kbd>):** Smooth ease-in-out animated child node translations relative to Machiya townhouse frames.
6. **In-Window Minimalist HUD Overlay (<kbd>F1</kbd>):** Embedded $256 \times 256$ Consolas Bold font atlas with dedicated UI shaders, responsive layout, and zero external library dependencies.
7. **15 Controllable Inspectables & Dual Selection Engine (<kbd>T</kbd> / <kbd>Shift+T</kbd>):** Context-sensitive view-cone auto-selection combined with 6-DOF manual transformation controls.
8. **Automated Verification Harness (`Main.cpp --test`):** Comprehensive automated test harness asserting 312 unit and integration conditions with 100% success.
9. **Rich Curved Geometry Generators (`Curves.h`):** Bézier tubes, Catmull-Rom splines, Bishop frames, and curved architectural *sori* beams.
10. **Academic LaTeX Report & Automated Capture (`--capture-report`):** Concise 40-page (34-page core) formal laboratory report (`report/main.pdf`) with 6 vector PDF diagrams, 20 automated high-resolution viewport screenshots, and Gemini 3.8 Flash development acknowledgments.

### 14.2 Plan Divergences & Code Harmonization
- **Light Toggle Keybinding:** Originally proposed as <kbd>L</kbd>. Remapped to <kbd>0</kbd> and <kbd>Numpad 0</kbd> in code to avoid conflicting with the $+X$ manual translation key (<kbd>J</kbd>/<kbd>L</kbd>).
- **Inspectables Count:** Expanded from 11 objects to 15 objects (added the 4 Machiya townhouse buildings as inspectables 12–15).
- **Point Light Count:** Expanded from 6 point lights to 14 dynamic point lights (16 total lights: 1 Directional, 1 Spotlight, 14 Point Lights including 6 interior house lights and 2 Torii shrine lanterns).

---

## 15. Performance, Limits and Known Edge Cases

### 15.1 Architectural Bounds & Constant Limits
The following hard-coded limits exist in the source code:
- **Maximum Point Lights:** Capped at `12` in `shaders/basic.frag` (`#define NR_POINT_LIGHTS 12`) and `Scene.h` (`pointLights.resize(12)`). Increasing this requires updating both the GLSL macro and `Scene::initLighting()`.
- **Directional Shadow Map Resolution:** Fixed at $2048 \times 2048$ pixels (`SHADOW_WIDTH`, `SHADOW_HEIGHT` in `Scene.h: lines 93-94`).
- **GPU Ray Tracer Bounces:** Fixed at `uMaxBounces = 3` in `Scene::renderRayTraced()`.
- **Fireworks Particle Pool:** Fixed at 4 simultaneous rockets (`rockets.size() == 4`), each holding 35 explosion particles (140 particles total).
- **Inspectables Pool:** Exactly 15 controllable objects registered in `Scene::inspectables`.
- **HUD Frame Rate Limiter:** Geometry updates are rate-limited to 10 Hz (`timeSinceLastUpdate >= 0.10f`) to prevent CPU-GPU buffer churn.

### 15.2 Working Directory & Resource Resolution Strategy
- **Texture Paths:** If texture assets are missing from `assets/textures/`, `TextureGenerator::ensureTextureAssetsExist()` automatically generates procedural BMP files to prevent null texture bindings.
- **Shader Paths:** If external `.vert` or `.frag` files cannot be read from disk (e.g., if launched from an unexpected working directory), `Shader.h` catches the `ifstream` exception and compiles hard-coded embedded fallback GLSL source strings.

---

## 16. Extension Guide

### 16.1 Adding a New Geometric Primitive
1. Open `src/Primitives.h`.
2. Add a new static method returning a `Mesh`:
   ```cpp
   static Mesh createTorus(float rMajor, float rMinor, int segMajor = 24, int segMinor = 16)
   {
       std::vector<Vertex> vertices;
       std::vector<unsigned int> indices;
       // Compute torus vertices (pos, normal, UVs) and triangle indices...
       return Mesh(vertices, indices);
   }
   ```
3. Add a `Mesh torus;` member to `struct SceneMeshes` in `src/Objects.h` and initialize it in `SceneMeshes::init()`.

### 16.2 Adding a New Scene Object
1. Open `src/Objects.h`.
2. Define a new class constructing a `SceneNode` hierarchy:
   ```cpp
   class FestivalBanner
   {
   public:
       std::shared_ptr<SceneNode> root;
       FestivalBanner(SceneMeshes& meshes, const glm::vec3& pos)
       {
           root = std::make_shared<SceneNode>("Festival_Banner");
           root->transform.position = pos;
           auto pole = std::make_shared<SceneNode>("Pole");
           pole->mesh = &meshes.cylinder;
           pole->transform.scale = glm::vec3(0.08f, 4.0f, 0.08f);
           root->addChild(pole);
       }
   };
   ```
3. Open `src/Scene.h`. Add a member `std::unique_ptr<FestivalBanner> banner;` and instantiate it inside `Scene::buildScene()`.

### 16.3 Adding a Dynamic Light Source
1. Open `shaders/basic.frag` and increase `#define NR_POINT_LIGHTS 12` to `13`.
2. Open `src/Scene.h: initLighting()`. Change `pointLights.resize(12);` to `13`.
3. Configure the new point light parameters:
   ```cpp
   pointLights[12].position = glm::vec3(0.0f, 3.0f, 0.0f);
   pointLights[12].diffuse = glm::vec3(1.0f, 0.2f, 0.8f);
   pointLights[12].constant = 1.0f;
   pointLights[12].linear = 0.09f;
   pointLights[12].quadratic = 0.032f;
   ```
4. Update its dynamic position or intensity inside `Scene::updateLighting(float dt)`.

### 16.4 Adding an Interactable & Custom Key Action
1. Open `src/ui/InteractionManager.cpp: init()`.
2. Register the new object in `interactables`:
   ```cpp
   Interactable it;
   it.id = "custom_banner";
   it.displayName = "Festival Banner";
   it.category = "Decoration";
   it.getWorldPosition = []() { return glm::vec3(0, 0, 0); };
   it.interactionRadius = 5.0f;
   
   ActionHint action;
   action.key = GLFW_KEY_K;
   action.keyName = "K";
   action.getLabel = []() { return "Wave Banner"; };
   action.execute = []() { /* Trigger animation */ };
   it.actions.push_back(action);
   
   interactables.push_back(it);
   ```

---

## 17. Glossary and Quick Reference

### Glossary of Computer Graphics Terms
- **Scene Graph:** An n-ary tree data structure organizing visual and spatial entities into parent-child hierarchies.
- **World Transformation Matrix:** An affine $4 \times 4$ matrix accumulating all ancestor transformations to position a vertex in global world space: $\mathbf{M}_{\text{world}} = \mathbf{M}_{\text{parent}} \times \mathbf{M}_{\text{local}}$.
- **Normal Matrix:** The transpose of the inverse of the upper-left $3 \times 3$ model matrix: $\mathbf{N} = ((\mathbf{M}^{3 \times 3})^{-1})^T$. Preserves surface normal perpendicularity under non-uniform scaling.
- **Blinn-Phong Illumination:** An approximation of Phong shading utilizing the halfway vector $\mathbf{H} = \frac{\mathbf{L}+\mathbf{V}}{\|\mathbf{L}+\mathbf{V}\|}$, reducing trigonometric evaluation overhead while generating realistic specular highlights.
- **Percentage-Closer Filtering (PCF):** A shadow anti-aliasing technique that samples multiple depth values around a shadow coordinate and averages the boolean depth comparison results to produce soft penumbras.
- **Whitted Ray Tracing:** A recursive optical ray-casting algorithm that traces primary camera rays, shadow rays towards light sources, and recursive reflection/refraction rays off specular surfaces.
- **Bishop Parallel Transport Frame:** A rotation-minimizing local coordinate frame defined along 3D space curves that eliminates twisting discontinuities common to standard Frenet-Serret frames.
- **Continuous Collision Detection (CCD):** Physics resolution calculating object trajectories between frames to prevent high-speed tunneling through thin surfaces.

### Quick Reference Cheatsheet
- **Key Bindings Summary:**
  - Movement: <kbd>W</kbd>/<kbd>A</kbd>/<kbd>S</kbd>/<kbd>D</kbd> + <kbd>E</kbd>/<kbd>Q</kbd> | Look: Mouse | Zoom: Scroll
  - HUD: <kbd>F1</kbd> | Screenshot Clean: <kbd>F10</kbd> | Screenshot HUD: <kbd>Shift+F10</kbd>
  - Lighting Toggle: <kbd>0</kbd> / <kbd>KP_0</kbd> | Day/Night: <kbd>N</kbd> | Shading Cycle: <kbd>P</kbd>
  - Textures: <kbd>X</kbd> | Shadows: <kbd>V</kbd> | Collision: <kbd>B</kbd> | GPU Ray Tracer: <kbd>Z</kbd> | CPU Snapshot: <kbd>F9</kbd>
  - Doors: <kbd>H</kbd> | Windows: <kbd>G</kbd> | Magic Trick: <kbd>M</kbd> | Firework: <kbd>F</kbd>
  - Select Target: <kbd>T</kbd> (<kbd>Shift+T</kbd> Auto) | Transform: <kbd>I/K</kbd>, <kbd>J/L</kbd>, <kbd>U/O</kbd>, <kbd>Arrows</kbd>, <kbd>+/-</kbd>
- **Primary Source Files:**
  - Entry & Tests: `Main.cpp`
  - Scene Coordinator: `src/Scene.h`
  - 17 Objects & Kinematics: `src/Objects.h`
  - Mesh Primitives: `src/Primitives.h` & `src/Curves.h`
  - Shaders: `shaders/basic.vert`, `shaders/basic.frag`, `shaders/raytrace.frag`
  - UI & Interaction: `src/ui/Hud.cpp`, `src/ui/InteractionManager.cpp`
