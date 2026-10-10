# Matsuri Nights — Development & Change Log (`agy.md`)
**Project:** Matsuri Nights — A Japanese Festival Street  
**Course:** CSE4102 Computer Graphics & Image Processing Laboratory  
**Author:** MD. Abu Hasanat Soykot | **Roll:** 2107100 | **Group:** B2  

---

## Overview
This document tracks all features, additions, bug fixes, transformations, and architectural updates implemented in the project. Whenever any component, asset, animation, control, or shader is modified, added, or fixed, a detailed entry is logged here.

---

## Log Entries

### [2026-10-10] — Academic Project Report: Comprehensive LaTeX Documentation (`report/main.pdf`), Automated Capture Suite (`--capture-report`), & PNG Screenshot Export

#### 1. Native PNG Screenshot Export & Automated Report Capture Harness (`Main.cpp`)
* **Files Added/Modified:** `Libraries/include/stb_image_write.h`, `Libraries/include/stb/stb_image_write.h`, [`Main.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Main.cpp)
* **Goal & Scope:** Enable lossless PNG image capture alongside uncompressed BMP export and implement a headless/automated CLI flag `--capture-report` that systematically triggers and saves all 20 required academic figures across every rendering mode, lighting state, and animation view into `report/figures/`.
* **Technical Details:**
  * Added Sean Barrett's single-header `stb_image_write.h` (`STB_IMAGE_WRITE_IMPLEMENTATION`) to `Libraries/include/` and `Libraries/include/stb/`.
  * Updated `captureViewportScreenshot(const std::string& filename)` to automatically detect `.png` extension and dispatch to `stbi_write_png(..., 3, flippedData, width * 3)`.
  * Added `--capture-report` argument handler in `Main.cpp` driving camera coordinates, day/night cycles, lighting toggles, shading modes (Blinn-Phong, Diffuse, Ambient), PCF shadow states, GPU ray tracing, HUD overlays, and multi-object inspectables across 20 frames without manual user intervention.
  * Augmented automated test suite in `Main.cpp: runAutomatedTests()` to assert PNG screenshot file generation, raising test count from 311 to **312 assertions (100% SUCCESS)**.

#### 2. Vector Graphics & Mathematical Diagram Generation (`report/figures/make_diagrams.py`)
* **Files Added:** `report/figures/make_diagrams.py`
* **Generated Diagrams:**
  1. `fig_render_pipeline.pdf` / `.png`: Two-pass frame architecture (Shadow FBO Pass $\to$ Forward Raster / GPU Ray Tracing $\to$ 2D Orthographic HUD Pass $\to$ Double Buffer Swap).
  2. `fig_scene_graph_tree.pdf` / `.png`: Hierarchical Scene Graph DAG detailing parent-child TRS matrix propagation.
  3. `fig_blinn_phong_vectors.pdf` / `.png`: Geometric vector diagram of incident light $\mathbf{L}$, view $\mathbf{V}$, normal $\mathbf{N}$, and halfway vector $\mathbf{H}$.
  4. `fig_day_night_curves.pdf` / `.png`: Parametric time curves for sun elevation, ambient intensity, lantern emissive boost, and sky RGB interpolation.
  5. `fig_shadow_pcf.pdf` / `.png`: Orthographic light frustum, depth buffer rasterization, and 16-sample PCF filter disk convolution.
  6. `fig_catenary_bezier.pdf` / `.png`: Mathematical curves comparing hyperbolic catenary hanging sag against cubic Bézier roof curvature.

#### 3. Formal Academic Project Report (`report/main.tex` & Modular Sections)
* **Files Added:** `report/main.tex`, `report/sections/01_introduction.tex` through `14_conclusion.tex`, `report/analysis/project_inventory.md`, `report/README_COMPILE.txt`
* **Format & Standards:** Formatted in accordance with KUET Department of Computer Science & Engineering (CSE4102) academic guidelines (A4 paper, Times New Roman typography via `mathptmx`, 1-inch margins, LaTeX chapter hierarchy, mathematical code citations `File.h/cpp: functionName()`).
* **Content Structure (50 Pages, 14 Chapters):**
  * **Chapter 1: Introduction & Project Scope:** Academic requirements fulfillment, 3D transformations, multi-body kinematics, day/night cycles.
  * **Chapter 2: Architecture & Rendering Pipeline:** Double-buffered frame lifecycle, OpenGL state machines, shader uniform bindings.
  * **Chapter 3: Mathematical Foundations:** TRS matrix algebra, Gram-Schmidt orthonormalization, halfway vector $\mathbf{H}$, catenary calculus, Bishop parallel transport.
  * **Chapter 4: Geometry Engine & Primitive Generation:** Cube, Cylinder, Cone, UV Sphere, Plane, swept tubes, Bézier curvature.
  * **Chapter 5: Hierarchical Scene Graph & Kinematics:** Recursive tree traversal, world matrix accumulation, relative reference frames.
  * **Chapter 6: Illumination & Shading Pipeline:** Directional sun/moon, 14 dynamic point lights, stage spotlight, indoor skylight bounce, live shading modes.
  * **Chapter 7: Realistic Soft Shadow Mapping:** Two-pass FBO depth generation, normal-scaled slope bias, 16-sample PCF filter.
  * **Chapter 8: Procedural Texture Synthesis & Materials:** 7 procedural BMP textures, UV mapping, material parameters.
  * **Chapter 9: Animation Catalogue & Dynamic Systems:** Takoyaki flip projectile physics, Kakigori shaved ice kinematics, lantern pendulums, sakura blossom drift, firework rocket explosions, magic show state machine.
  * **Chapter 10: Dual Ray Tracing Architectures:** Full-screen GPU Whitted ray tracer (`shaders/raytrace.frag`), multi-threaded CPU snapshot engine (`RayTracer.h`).
  * **Chapter 11: UI, HUD & Interaction:** Embedded $256\times 256$ Consolas Bold atlas, 2D orthographic batched text renderer, context action dispatcher.
  * **Chapter 12: Automated Verification & Testing:** 312 unit test suite breakdown across all 5 verification categories.
  * **Chapter 13: Controls Reference & Documentation Audit:** Authoritative keybinding catalog and reconciliation table of legacy documentation against source code.
  * **Chapter 14: Conclusion & Future Enhancements:** Academic summary, PBR Cook-Torrance roadmap, volumetric fog, and bibliography.
* **Compilation Status:** Built cleanly with MiKTeX `pdflatex` (0 errors, 50 pages, 13.7 MB output at `report/main.pdf`).

---

### [2026-10-10] — Feature: Decorated Torii Shrine Gate Illuminations & 14 Dynamic Point Lights

#### 1. Decorated Shrine Entrance Architecture (`Objects.h`, `Scene.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h), [`Matsuri Nights — A Japanese Festival Street/shaders/basic.frag`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/shaders/basic.frag)
* **Goal & Scope:** Decorate the grand Torii Shrine Gate at the street terminus ($Z = -32.0\text{m}$) with traditional Japanese shrine lanterns and light sources so that in festival darkness it radiates a warm, majestic, and beautiful atmosphere.
* **New Architectural Components Added to `ToriiGate`:**
  1. **Four Grand Hanging Chochin Lanterns:** Suspended beneath the Nuki crossbeam at $X \in \{-3.2\text{m}, -1.15\text{m}, +1.15\text{m}, +3.2\text{m}\}$ at $Y = 7.08\text{m}$. Modeled with dark bronze suspension cords, black lacquer top caps, ribbed vermilion washi paper bodies (`isEmissive = true`, glowing `(2.6, 1.4, 0.5)`), white kanji bands, black bottom caps, and golden silk hanging tassels.
  2. **Two Front Pillar-Mounted Cantilever Bracket Lanterns (*Tsuri-Doro*):** Mounted at $Y = 4.80\text{m}$ on each main pillar ($X = \pm 4.5\text{m}$) projecting forward along $+Z$ to $Z = 0.85\text{m}$. Modeled with wrought-iron cantilever arms, diagonal support struts, hexagonal pagoda roof canopies, glowing warm amber washi diffuser cylinders (`(3.0, 1.8, 0.6)`), wooden base trays, and teardrop finials.
  3. **Twin Traditional Japanese Stone Lanterns (*Ishi-Doro* 石灯籠):** Flanking the front entrance approach at $X = \pm 3.8\text{m}, Z = 2.4\text{m}$ (world $Z = -29.6\text{m}$). Each lantern stands $2.4\text{m}$ tall and is assembled from 6 authentic components: stepped foundation plinth (*Kiso*), cylindrical stone shaft (*Sao*), middle lotus platform (*Chudai*), hollow light chamber (*Hibukuro*) with 4 framing corner posts and radiant sacred flame core (`isEmissive = true`, glowing fire amber `(3.5, 2.4, 1.0)`), flared pagoda umbrella roof (*Kasa*), and lotus pearl jewel finial (*Hoju*).
  4. **Sacred Straw Rope (*Shimenawa*) & Folded Paper Streamers (*Shide*):** Spans beneath the Nuki crossbeam between pillars ($X \in [-4.3\text{m}, +4.3\text{m}]$). Modeled with a braided golden wheat straw rope cylinder, 3 hanging straw tassels, and 4 folded white zigzag paper streamers (*Shide*).
  5. **Gilded Plaque Frame & Crest:** Central Gakuzuka tablet updated with a gilded gold leaf frame (`shininess = 64.0`, `specularStrength = 0.90`) and circular gold shrine crest disk (*Shinmon*).

#### 2. Multi-Source Illumination Expansion: 14 Dynamic Point Lights (`basic.frag`, `Scene.h`)
* **Fragment Shader Pipeline (`shaders/basic.frag`):**
  * Increased `#define NR_POINT_LIGHTS` from 12 to 14.
* **Scene Lighting Engine (`Scene.h`):**
  * Expanded `pointLights` vector size to 14.
  * **Point Light 12 (Torii Left Gate Shrine Lantern):** Positioned at $(X = -2.8\text{m}, Y = 6.8\text{m}, Z = -31.5\text{m})$, radiating warm golden-amber illumination (`diffuse = (1.50, 1.05, 0.50)`, `specular = (1.30, 1.00, 0.55)`, $k_c = 1.0, k_l = 0.07, k_q = 0.018$).
  * **Point Light 13 (Torii Right Gate Shrine Lantern):** Positioned at $(X = +2.8\text{m}, Y = 6.8\text{m}, Z = -31.5\text{m})$, radiating matching golden-amber illumination across the right pillar, crossbeam, and entrance path.
  * **Hierarchical Anchor Tracking:** Created `leftLightAnchor` and `rightLightAnchor` child nodes inside `ToriiGate`. In `Scene::updateLighting()`, Point Lights 12 & 13 dynamically query `getWorldPosition()`, guaranteeing that if an examiner transforms or inspects the Torii Gate, the point lights dynamically follow the gate in 3D space.
  * **Dynamic Night & Toggle Modulation:** Diffuse and specular terms scale smoothly with day/night transitions ($\text{boost} = \text{mix}(0.25, 1.45, \text{dayNightFactor}) \times \text{lightScale}$) and dim immediately when festival lights are toggled with <kbd>0</kbd> / <kbd>KP_0</kbd>.
* **Emissive Dynamic Update (`ToriiGate::update`):**
  * Added `ToriiGate::update(dt, nightFactor, lightsOn)` called per-frame in `Scene::update()`, scaling the emissive brilliance of all 8 glowing lantern cores and flame nodes between daylight and night.
  * Integrated with `Scene::toggleLanternLights()` so <kbd>0</kbd> / <kbd>KP_0</kbd> toggles all Torii gate illuminations.

---

### [2026-10-10] — Documentation: In-Depth Technical Specification (`Details.md`) & `Plan.md` Milestone Completion

#### 1. In-Depth Technical Specification Document (`Details.md`)
* **Files Added:** [`Details.md`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Details.md)
* **Goal & Scope:** Author an exhaustive, rigorous 1,388-line technical reference manual and academic project report detailing every system, mathematical formula, shader layout, lighting source, and asset in "Matsuri Nights — A Japanese Festival Street".
* **Key Sections & Coverage:**
  1. **Project Overview & Academic Compliance:** Course details (CSE4102, KUET), 3D transformations, relative coordinate frames, complex animated objects (Takoyaki, Kakigori, Magic show), 12 dynamic lights, moving light sources, day/night transitions, and complete phase 1, 2, 3 verification.
  2. **Tech Stack & Build System:** C++17, OpenGL 3.3 Core Profile, GLFW 3.5.1, GLAD, GLM, Visual Studio 2026 (v145 toolset) MSBuild configuration, and single-header dependencies.
  3. **Repository Structure & File Registry:** Annotated directory tree and exhaustive file-by-file table documenting purpose, primary classes, and dependencies.
  4. **System Architecture & Frame Flow:** Complete lifecycle from `main()` to termination, frame execution order (input $\to$ kinematics $\to$ shadow depth pass $\to$ Blinn-Phong 3D pass / Ray Tracer $\to$ HUD overlay $\to$ buffer swap), OpenGL state management, and full uniform layout tables for all 4 shader programs (`basic`, `shadowDepth`, `raytrace`, `hud`). Included Mermaid architectural and frame execution flowcharts.
  5. **Geometry System:** Analytical vertex generation, parameter matrices, UV layout, and triangle count breakdowns for Cube, Cylinder, Cone, Sphere, and Plane primitives, accompanied by the `Mesh` VAO/VBO/EBO wrapper.
  6. **Transform System & Scene Graph:** Local matrix composition ($M = T \cdot R_z \cdot R_y \cdot R_x \cdot S$), recursive world matrix propagation ($W_c = W_p \cdot L_c$), and complete Mermaid hierarchy tree of the entire festival street scene graph.
  7. **Camera System & User Controls:** First-person Euler angle spherical trigonometry ($\text{yaw}, \text{pitch}$), view matrix calculation, FPS mouse look, FOV zoom, smooth collision sliding, preset camera viewpoints, and exhaustive keybinding reference matrix.
  8. **Lighting & Material Pipeline:** Blinn-Phong illumination model with halfway vector $\mathbf{H}$, directional sunlight/moonlight, 12 dynamic point lights, stage spotlight with smooth angular falloff, indoor skylight bounce, 16-sample PCF soft shadow mapping, and dual Whitted ray tracing engines (real-time GPU + multi-threaded CPU).
  9. **Procedural Texture Generation & Materials:** 7 procedurally synthesized 24-bit uncompressed `.bmp` texture maps (wood timber, roof tiles, stone pavement, tatami cloth, gold leaf, sakura bark, lantern paper) and UV texture mapping math.
  10. **Kinematics & Hierarchical Animation:** Rigorous physics formulas for Takoyaki parabolic projectile flipping, Kakigori presentation hopping and shaver flywheel rotation, wind-driven lantern pendulum kinematics, cherry blossom flutter and ground drifting, crowd navigation corridor, and magic trick phases.
  11. **Doorway Kinematics & Collision Engine:** Sliding Shoji door and window interpolation, camera-to-portal distance validation, and dynamic AABB collision masking allowing passage only when doors are open.
  12. **In-Window HUD Overlay & Interaction System:** Orthographic 2D overlay pass, OpenGL state preservation, embedded Consolas Bold texture atlas, 10 Hz rate limiter, and context-sensitive action dispatcher.
  13. **Verification & Test Suite:** 311 automated unit & integration tests, execution instructions, and verification proof.
  14. **Plan vs Implementation Analysis:** Detailed audit contrasting initial design notes against actual production code.

#### 2. Plan.md Milestone Completion Signoff (`Plan.md`)
* **Files Modified:** [`Plan.md`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Plan.md)
* **Status:** Completely audited and rewrote `Plan.md` to reflect the final state of the codebase. Marked all Phase 1, Phase 2, and Phase 3 milestones, advanced additions, and HUD/interaction deliverables as fully completed (`[x]`).

---

### [2026-10-10] — Verification & Test: Expanded 311-Test Automated Control, Transform & UI Harness

#### 1. Automated Verification Suite (`Main.cpp`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/Main.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Main.cpp)
* **Goal & Scope:** Expand the automated test suite from 239 to 311 tests, providing complete end-to-end coverage across all 15 inspectable objects (including all 4 traditional Machiya townhouses), HUD rendering, high-DPI framebuffer resizing, clean screenshot capture, and lighting key remapping.
* **Results: 311 / 311 Automated Tests Passed (100% Success):**
  * **Section 1: All 15 Controllable Objects & 6-DOF Manual Transforms:**
    1. `Lantern [Body]` (Child of Swinging Rope Pivot)
    2. `Lantern [Rope Pivot]` (Parent Anchor Node)
    3. `Magic Orb` (Child of Magician's Hand Bone)
    4. `Magician Figure` (Root Articulated Rig)
    5. `Vanishing Box` (Scale-to-Zero Demo)
    6. `Stage Spotlight Housing` (Tracking Rig)
    7. `Takoyaki Stall` (Full Unit with 6 Balls)
    8. `Kakigori Stall` (Full Unit with Shaver & 6 Bowls)
    9. `Torii Gate` (Grand Shrine Entrance)
    10. `Sakura Blossom Tree` (Hierarchical Foliage)
    11. `Crowd Walker #1` (Walking Street Rig)
    12. `Machiya Building L1` (Left Front Townhouse)
    13. `Machiya Building L2` (Left Rear Townhouse)
    14. `Machiya Building R1` (Right Front Townhouse)
    15. `Machiya Building R2` (Right Rear Townhouse)
    * For every object: verified non-null pointer, translation along $+X/-X, +Y/-Y, +Z/-Z$, pitch and yaw rotations, scaling up ($1.1\times$) and scaling down ($0.9\times$), world matrix propagation, and cycling forward/backward with wrap-around across all 15 indices $[0, 14]$.
  * **Section 2: Interactive House Doors & Sliding Windows:**
    * Tested all 4 Machiya buildings (`Machiya_L1`, `Machiya_L2`, `Machiya_R1`, `Machiya_R2`):
      * Front Shoji door toggle open/closed (Key `H`), progress interpolation along local Z ($1.35\text{m}$ track), and distance threshold rejection ($>5.0\text{m}$).
      * Sliding Shoji windows toggle open/closed (Key `G`), progress interpolation, and distance rejection ($>14.0\text{m}$).
  * **Section 3: Environment, Rendering & Lighting Toggles:**
    * Day / Night transition toggle (Key `N`): verified `targetNight` inversion and smooth factor blend.
    * Animation pause / resume (Key `Space`): verified `isPaused` and time freezing.
    * Shading mode cycle (Key `P`): verified Blinn-Phong $\to$ Diffuse $\to$ Ambient $\to$ Blinn-Phong cycle.
    * Textures toggle (Key `X`): verified `enableTextures` toggle.
    * Soft shadow mapping toggle (Key `V`): verified `enableShadows` toggle.
    * Real-time GPU ray tracing toggle (Key `Z`): verified `rayTracingMode` toggle.
    * Wall collision & doorway kinematics (Key `B`): verified solid wall blocking when door is closed, pass-through portal when door is open, and noclip penetration.
    * Manual firework rocket launch (Key `F`): verified `LAUNCHING` state $\to$ ascent $\to$ `BURSTING` active burst phase.
    * Festival lights toggle (Keys `0` and `KP_0`): verified point and street light illumination toggle without interfering with <kbd>J</kbd>/<kbd>L</kbd> object translation.
    * Camera presets (Keys `1`, `2`, `3`, `R`): verified positions, yaw, pitch, and origin reset.
  * **Section 4: Complex Multi-Object Animations:**
    * Verified 6 Takoyaki balls, 6 Kakigori bowls, ice shaver wheel rotation, active ice mound, and dynamic update cycles.
  * **Section 5: HUD Overlay & State Integrity:**
    * Verified HUD enable/disable toggle (Key `F1`).
    * Verified framebuffer resizing layout safety (ignoring zero-size framebuffers on minimize).
    * Verified clean screenshot capture (HUD automatically hidden for <kbd>P</kbd>, captured for <kbd>Shift</kbd>+<kbd>P</kbd>).
* **Test Command:** Run `.\x64\Debug\Matsuri Nights - A Japanese Festival Street.exe --test`.

---

### [2026-10-10] — Bug Fix: Machiya Townhouse Inspection & Transformation Synchronization

#### 1. Inspection Synchronization & Scene Integration (`Scene.h`, `InteractionManager.cpp`, `Objects.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h), [`Matsuri Nights — A Japanese Festival Street/src/ui/InteractionManager.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/ui/InteractionManager.cpp), [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Problem Addressed:**
  * When cycling selected objects with <kbd>T</kbd>, the HUD displayed `Machiya_L1`, `Machiya_L2`, `Machiya_R1`, or `Machiya_R2` when cycling past index 10. However, manual transformation keystrokes (<kbd>I</kbd>/<kbd>K</kbd>, <kbd>J</kbd>/<kbd>L</kbd>, <kbd>U</kbd>/<kbd>O</kbd>, Numpad keys) continued to move `Walker #1` instead of the selected Machiya building.
* **Root Cause:**
  * `Scene::inspectables` only contained 11 items (indices 0..10), while `InteractionManager` contained 15 registered interactables. When selection index reached 11..14, `Scene::getSelectedNode()` clamped or fell back to the last valid inspectable (`Walker #1`), causing a desynchronization between what the HUD reported and what the transformation engine manipulated.
* **Solution & Fix Implementation:**
  1. **Registered All 4 Machiya Buildings in `Scene::inspectables`:**
     * Added entries 11, 12, 13, and 14 to `Scene::setupInspectables()`:
       * Index 11: `Machiya Building L1` (`machiyaL1->buildingNode.get()`)
       * Index 12: `Machiya Building L2` (`machiyaL2->buildingNode.get()`)
       * Index 13: `Machiya Building R1` (`machiyaR1->buildingNode.get()`)
       * Index 14: `Machiya Building R2` (`machiyaR2->buildingNode.get()`)
  2. **Synchronized `scene.selectedIndex` in `InteractionManager::cycleSelection()`:**
     * Updated `InteractionManager::cycleSelection()` so that whenever selection is cycled (via <kbd>T</kbd> or context interaction), `scene.selectedIndex` is explicitly updated to match, guaranteeing 100% 1:1 parity across both systems.
  3. **Synchronized Transform State in `MachiyaBuilding::update()`:**
     * Added synchronization of `worldPos` and `rotationY` from `buildingNode->transform.position` and `rotation.y` inside `MachiyaBuilding::update()`. As a result, when an examiner translates or rotates a Machiya townhouse with the transformation keys, its doorway interaction trigger zones, window positions, and collision boundary boxes dynamically follow the new building location in real-time.

---

### [2026-10-10] — Controls: Festival Lighting Hotkey Remapped to `0` / `KP_0`

#### 1. Keystroke Collision Elimination (`Main.cpp`, `Scene.h`, `InteractionManager.cpp`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/Main.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Main.cpp), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h), [`Matsuri Nights — A Japanese Festival Street/src/ui/InteractionManager.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/ui/InteractionManager.cpp)
* **Problem Addressed:**
  * Previously, key <kbd>L</kbd> was bound to toggle festival point lights. However, <kbd>L</kbd> is also the primary horizontal translation key for moving the currently selected inspectable object along $+X$ (<kbd>J</kbd>/<kbd>L</kbd> translation axis). Pressing <kbd>L</kbd> caused the lighting to flicker on and off while attempting to translate objects.
* **Solution:**
  * Remapped festival lights toggle to <kbd>0</kbd> (Number row 0, `GLFW_KEY_0`) and <kbd>KP_0</kbd> (Keypad 0, `GLFW_KEY_KP_0`).
  * Restored uninterrupted, collision-free 6-DOF transformation control:
    * Translation: <kbd>I</kbd> / <kbd>K</kbd> ($\pm Z$), <kbd>J</kbd> / <kbd>L</kbd> ($\pm X$), <kbd>U</kbd> / <kbd>O</kbd> ($\pm Y$).
    * Rotation: <kbd>Numpad 8</kbd> / <kbd>Numpad 2</kbd> (Pitch), <kbd>Numpad 4</kbd> / <kbd>Numpad 6</kbd> (Yaw).
    * Scaling: <kbd>+</kbd> / <kbd>-</kbd> ($1.1\times$ / $0.9\times$).
  * Updated HUD contextual action hints and console logs to display `[0] Toggle Lights`.

---

### [2026-10-10] — UX Refinement: Minimalist HUD Redesign & Consolas Bold Font Atlas

#### 1. Embedded Consolas Bold Font Atlas (`FontAtlasData.h`, `shaders/hud.frag`)
* **Files Added:** [`Matsuri Nights — A Japanese Festival Street/src/ui/FontAtlasData.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/ui/FontAtlasData.h)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/shaders/hud.frag`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/shaders/hud.frag)
* **Goal & Scope:** Eliminate blurry or hard-to-read bitmap text rendering by introducing a pre-baked $256 \times 256$ texture atlas with crisp Consolas Bold glyphs for ASCII characters 32..126.
* **Implementation Details:**
  * `FontAtlasData.h` defines a self-contained, statically compiled C++ byte array containing uncompressed 8-bit glyph textures and per-character UV bounding coordinates.
  * `shaders/hud.frag` applies smoothstep edge interpolation and a subtle dark shadow pass behind each character, ensuring crystal-clear legibility against bright daytime skies as well as dark nighttime backdrops.

#### 2. Sleek Minimalist HUD Panel Layout (`Hud.h`, `Hud.cpp`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/ui/Hud.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/ui/Hud.h), [`Matsuri Nights — A Japanese Festival Street/src/ui/Hud.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/ui/Hud.cpp)
* **Design Transformation:**
  * Replaced the verbose multi-section HUD overlay with an eye-soothing, compact card (~225px wide $\times$ ~100px tall) anchored cleanly to the top-right corner.
  * Formatted content into 5 essential lines:
    1. **Status Header:** Live frame rate indicator (`STATUS: 60 FPS`).
    2. **Active Target:** Color-coded selected object name (yellow for selected, cyan for nearby interactable).
    3. **Transform Keystroke Hint:** `[T] Cycle Target` / transform keys.
    4. **Context Action Hint:** Relevant real-time interaction (e.g. `[H] Toggle Door`, `[G] Toggle Window`, `[0] Toggle Lights`, `[M] Magic Trick`).
    5. **Footer:** Toggle reminder (`[F1] Toggle HUD`).
  * Built-in 10 Hz rate limiter on CPU vertex buffer regeneration to eliminate per-frame dynamic memory allocation.

---

### [2026-10-10] — Feature: In-Window Semi-Transparent HUD Overlay & Context Interaction System

#### 1. Core UI Architecture & Interaction Framework (`Hud.h`, `Hud.cpp`, `Interactable.h`, `InteractionManager.h`, `InteractionManager.cpp`)
* **Files Added:**
  * [`Matsuri Nights — A Japanese Festival Street/src/ui/Hud.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/ui/Hud.h)
  * [`Matsuri Nights — A Japanese Festival Street/src/ui/Hud.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/ui/Hud.cpp)
  * [`Matsuri Nights — A Japanese Festival Street/src/ui/Interactable.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/ui/Interactable.h)
  * [`Matsuri Nights — A Japanese Festival Street/src/ui/InteractionManager.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/ui/InteractionManager.h)
  * [`Matsuri Nights — A Japanese Festival Street/src/ui/InteractionManager.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/ui/InteractionManager.cpp)
  * [`Matsuri Nights — A Japanese Festival Street/shaders/hud.vert`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/shaders/hud.vert)
  * [`Matsuri Nights — A Japanese Festival Street/shaders/hud.frag`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/shaders/hud.frag)
* **Goal & Scope:** Implement an in-window Heads-Up Display (HUD) and context-sensitive interaction framework without introducing heavy external dependencies (such as Dear ImGui or FreeType).
* **Render Pipeline & OpenGL State Isolation:**
  * Executed as a distinct 2D orthographic pass following completion of the 3D scene pass.
  * Orthographic projection computed in pixel coordinates: `glm::ortho(0.0f, fbWidth, fbHeight, 0.0f, -1.0f, 1.0f)`.
  * Comprehensive OpenGL state isolation: queried and saved previous `GL_DEPTH_TEST`, `GL_CULL_FACE`, `GL_BLEND`, blend functions, bound shader program, and active VAO. Disabled depth testing and face culling during the overlay pass; enabled alpha blending `glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)`. Restored the exact OpenGL state upon pass completion.
* **Anchor & High-DPI Handling:**
  * Uses `glfwGetFramebufferSize()` to anchor to the top-right window boundary.
  * Guards against zero-sized framebuffers on window minimization to prevent divide-by-zero or OpenGL projection errors.
* **Screenshot Integration:**
  * Automatically suppresses HUD rendering during screenshot capture (<kbd>P</kbd>) so exported promotional stills remain clean.
  * Allows <kbd>Shift</kbd>+<kbd>P</kbd> to capture screenshots with the HUD visible when desired.

---

### [2026-10-10] — Verification & Test: Comprehensive Automated Control & Transform Test Suite

#### 1. Automated Verification Suite (`Main.cpp`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/Main.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Main.cpp)
* **Goal & Scope:** Implement and execute an end-to-end automated test harness covering all 11 controllable inspectable objects, interactive Shoji house doors and windows, rendering and environment toggles, kinematics, and animations.
* **Coverage & Verifications (239 / 239 Tests Passed - 100% Success):**
  * **Section 1: All 11 Controllable Objects & Manual Transforms:**
    1. `Lantern [Body]` (Child of Swinging Rope Pivot)
    2. `Lantern [Rope Pivot]` (Parent Anchor Node)
    3. `Magic Orb` (Child of Magician's Hand Bone)
    4. `Magician Figure` (Root Articulated Rig)
    5. `Vanishing Box` (Scale-to-Zero Demo)
    6. `Stage Spotlight Housing` (Tracking Rig)
    7. `Takoyaki Stall` (Full Unit with 6 Balls)
    8. `Kakigori Stall` (Full Unit with Shaver & 6 Bowls)
    9. `Torii Gate` (Grand Shrine Entrance)
    10. `Sakura Blossom Tree` (Hierarchical Foliage)
    11. `Crowd Walker #1` (Walking Street Rig)
    * For every object: verified non-null node, translation along $+X/-X, +Y/-Y, +Z/-Z$, rotation along pitch & yaw, scaling up (1.1x) and scaling down (0.9x), world matrix propagation, and cycling forward/backward with wrap-around across indices $[0, 10]$.
  * **Section 2: Interactive House Doors & Sliding Windows:**
    * Tested all 4 Machiya buildings (`Machiya_L1`, `Machiya_L2`, `Machiya_R1`, `Machiya_R2`):
      * Front Shoji door toggle open/closed (Key `H`), progress interpolation along local Z ($1.35\text{m}$ track), and distance threshold rejection ($>5.0\text{m}$).
      * Sliding Shoji windows toggle open/closed (Key `G`), progress interpolation, and distance rejection ($>14.0\text{m}$).
  * **Section 3: Environment, Rendering & Lighting Toggles:**
    * Day / Night transition toggle (Key `N`): verified `targetNight` inversion and smooth factor blend.
    * Animation pause / resume (Key `Space`): verified `isPaused` and time freezing.
    * Shading mode cycle (Key `P`): verified Blinn-Phong $\to$ Diffuse $\to$ Ambient $\to$ Blinn-Phong cycle.
    * Textures toggle (Key `X`): verified `enableTextures` toggle.
    * Soft shadow mapping toggle (Key `V`): verified `enableShadows` toggle.
    * Real-time GPU ray tracing toggle (Key `Z`): verified `rayTracingMode` toggle.
    * Wall collision & doorway kinematics (Key `B`): verified solid wall blocking when door is closed, pass-through portal when door is open, and noclip penetration.
    * Manual firework rocket launch (Key `F`): verified `LAUNCHING` state $\to$ ascent $\to$ `BURSTING` active burst phase.
    * Camera presets (Keys `1`, `2`, `3`, `R`): verified positions, yaw, pitch, and origin reset.
  * **Section 4: Complex Multi-Object Animations:**
    * Verified 6 Takoyaki balls, 6 Kakigori bowls, ice shaver wheel rotation, active ice mound, and dynamic update cycles.
* **Test Command:** Run `.\x64\Release\Matsuri Nights — A Japanese Festival Street.exe --test` or `.\x64\Debug\Matsuri Nights — A Japanese Festival Street.exe --test`.

---

### [2026-10-10] — Feature: Kakigori Stall Desserts & Dynamic Hopping / Presentation Animation

#### 1. Kakigori Dessert Servings & Stall Counter Layout (`Objects.h`, `Scene.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Goal & Scope:** Place authentic, colorful Japanese shaved ice dessert servings (Kakigori) on the stall counter directly in front of the vendor and beside "the box" (the vintage ice shaving machine), complete with dynamic multi-joint animations matching the style of the Takoyaki stall.
* **Component Details & Architecture:**
  * **Vintage Japanese Ice Shaver Machine ("The Box"):**
    * Positioned on the right side of the stall counter at $(X = 0.85\text{m}, Y = 1.20\text{m}, Z = 0.0\text{m})$.
    * Modeled with vintage cast iron stand legs, retro swan-cyan ice chamber housing (`Shaver_Body`), polished gold nameplate crest (`Shaver_Crest`), crystalline translucent ice block (`Shaver_IceBlock`) inside the shaving chamber, vertical mechanical spindle and crown gear (`Shaver_Spindle`), conical dispensing chute (`Shaver_Chute`), red spoked cast iron flywheel (`Shaver_Wheel`), and orbiting brass crank handle peg (`Shaver_Handle`).
    * Active shaved ice bowl under the chute receiving freshly shaved snow-white ice flakes (`Shaver_ActiveIce`).
    * 4-bottle syrup pump dispenser condiment rack beside the machine featuring Strawberry crimson, Melon green, Blue Hawaii cyan, and Lemon golden syrups with pump nozzles.
  * **Presentation Lacquer Tray & Kakigori Servings:**
    * Elegant Japanese black lacquer tray (`Urushi` tray, $1.70\text{m} \times 0.92\text{m}$) with vermilion rim placed on the counter in front of the vendor ($X \in [-0.75\text{m}, 0.35\text{m}]$) and beside the shaver machine.
    * 6 distinct, beautifully detailed Kakigori dessert servings arranged across 3 columns and 2 rows (preparation line and front customer serving line):
      1. **Ichigo (Strawberry Delux):** Crystalline footed glass cup, fluffy shaved ice dome, rich ruby strawberry glaze, sweetened condensed milk (`Rennyu`) drizzle, fresh cherry/strawberry topper with green stem, and red dessert spoon.
      2. **Matcha (Uji-Kintoki):** Flared dessert cup, towering ice mound, deep matcha syrup, sweetened adzuki red bean cluster (`Ogura-an`) on the side, milk drizzle, and matcha spoon.
      3. **Blue Hawaii:** Flared crystal cup, electric cyan-blue syrup, fluffy white snow crest, festive mini pink parasol umbrella pick, and cyan spoon.
      4. **Mango Passion / Golden Lemon:** Footed glass, golden mango glaze, sweet cream drizzle, tropical mango cube garnish, and golden spoon.
      5. **Kyoho Grape:** Fluted dessert cup, royal amethyst purple syrup, sweet milk swirl, juicy grape garnish, and purple spoon.
      6. **Melon Cream Float:** Festive glass cup, vivid honeydew melon syrup, rich milk swirl, melon ball topper, and green spoon.
* **Dynamic Animations (`KakigoriStall::update`):**
  * **Flywheel & Crank Handle:** Continuous high-speed spin: $\theta_{\text{wheel}} += 240^\circ \times \Delta t$ with handle peg orbiting the wheel axis.
  * **Active Shaver Bowl Pulse:** Fresh shaved ice mound under the chute rotates ($\theta_y += 90^\circ \times \Delta t$) and gently pulses with rhythmic breathing.
  * **Continuous Presentation Spin:** Each of the 6 Kakigori servings smoothly rotates around its vertical axis ($\theta_y += \omega_{\text{spin}} \times \Delta t$) to display its layered colors, milk drizzle, spoon, and toppings from all angles.
  * **Staggered Parabolic Hopping Arc:** In the style of the Takoyaki flipping animation, each Kakigori bowl periodically takes flight in a graceful parabolic arc ($y(t) = y_0 + 4 \cdot h_{\max} \cdot t(1-t)$) between the rear preparation spot and the front customer serving counter, executing a playful mid-air spin ($\theta_y += 360^\circ \times \Delta t$) and celebratory tilt ($\theta_z = \sin(\pi t) \times 12^\circ$), then landing softly on the counter with positions smoothly swapping for the return hop.
  * **Breathing Rest Bob:** Between hops, bowls gently bob on the lacquer tray with sinusoidal floating offset ($\sin(2.8t + \phi) \times 0.012\text{m}$).
* **Material & Texture Protection (`Scene.h`):**
  * Updated `assignKaki` traversal in `Scene.h` to exempt `Ice`, `Bowl`, `Cup`, `Syrup`, `Milk`, `Spoon`, `Garnish`, `Topping`, `Bottle`, `Pump`, `Umbrella`, `Berry`, `Bean`, `Mango`, `Grape`, `Melon`, `Shaver`, and `Tray` from `texWood` assignment, ensuring pristine Blinn-Phong specular and material colors.
  * Added `Kakigori Stall (Full Unit)` to the interactive inspectables list.

---

### [2026-10-10] — Fix: Magician Natural Downward Elbow Flexion & Arm Kinematics

#### 1. Downward Elbow Biomechanics & Forward/Upward Forearm Flexion (`Objects.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Problem Addressed:**
  * Previously, the upper arm had an exaggerated upward angle ($-62^\circ$) and the forearm had a positive pitch angle ($+42^\circ$), which caused the elbow joint to thrust upwards into the air above the wrist, unnaturally pointing up rather than hanging down as real human elbows do when bending arms.
* **Fix & Anatomical Realignment:**
  * **Downward Upper Arm Hang:** Upper arm rotates downward from the shoulder:
    * `rightArm->transform.rotation = (-16.0f, 10.0f, -16.0f)`.
    * Shoulder is at $Y = 1.62\text{m}$, and the elbow descends naturally down to waist level at $Y \approx 1.25\text{m}$.
    * The elbow is positioned firmly as the lowest point of the upper arm, pointing naturally **DOWNWARDS** towards the stage floor.
  * **Forward/Upward Forearm Flexion:** Forearm articulates with negative pitch angle from the downward elbow:
    * `rightForearm->transform.rotation = (-74.0f, 0.0f, 0.0f)`.
    * Bends smoothly forward and upward from the downward-pointing elbow ($Y = 1.25\text{m} \to Y = 1.27\text{m}$, extending forward along $+Z$ by $+0.36\text{m}$ to the wrist at $Z \approx 0.51\text{m}$).
    * Hand and wand hold forward over the stage toward the audience and floating magic orb.
  * **Companion Left Arm Downward Alignment:**
    * `leftArm->transform.rotation = (-10.0f, 0.0f, 16.0f)` (upper arm hangs downward from shoulder, elbow down at $Y \approx 1.25\text{m}$).
    * `leftForearm->transform.rotation = (-12.0f, 0.0f, 0.0f)` (relaxed gentle forward flexion).
    * `leftHand->transform.position = (0.0f, -0.42f, 0.0f)` with `rotation = (0.0f, 0.0f, 180.0f)`, resting flush along the left hip/thigh with fingertips extending downward.
  * **Kinematics & Animation (`Magician::update`):**
    * Upper arm shoulder pitch stays strictly between $-20^\circ$ and $-12^\circ$ (elbow NEVER goes upwards throughout the animation).
    * Forearm articulates smoothly between $-82^\circ$ and $-66^\circ$, gesturing gracefully with the wand while keeping the elbow anchored downwards.
    * Magic orb helical orbit centered cleanly in front of the forward wand tip ($Z = +0.50\text{m}$).

---

### [2026-10-10] — Fix: Magician Hand Swap (Right Hand Wand/Orb Grasp & Left Hand Resting Pose)

#### 1. Magician Hand Roles & Articulated Rig Swap (`Objects.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Goal & Scope:** Swap the magician's hands so the right hand grasps the magic wand and commands the floating orb, while the left hand rests gracefully along the left hip/side.
* **Articulated Joint Hierarchy & Transformations:**
  * **Right Arm (Spellcasting Limb holding Wand & commanding Orb):**
    $$\text{root} \xrightarrow{\text{Shoulder}} \text{rightArm} \xrightarrow{\text{Elbow}} \text{rightForearm} \xrightarrow{\text{Wrist}} \text{rightHand} \xrightarrow{\text{Grasp}} \text{wand} \ \& \ \text{orbNode}$$
    * Shoulder: Position $(+0.36\text{m}, 1.62\text{m}, 0.05\text{m})$, rotation $(-62.0^\circ, 18.0^\circ, -22.0^\circ)$, raised forward and abducted outward.
    * Elbow: Position $(0.0\text{m}, -0.38\text{m}, 0.0\text{m})$, rotation $(42.0^\circ, 10.0^\circ, 0.0^\circ)$, bent forward/upward toward the audience and orb.
    * Wrist (`rightHand`): Position $(0.0\text{m}, -0.36\text{m}, 0.0\text{m})$, rotation $(-15.0^\circ, 0.0^\circ, -20.0^\circ)$, serving as moving reference frame for wand and orb.
    * Right Palm: Position $(0.01\text{m}, 0.035\text{m}, 0.065\text{m})$, rotation $(-65.0^\circ, 10.0^\circ, -15.0^\circ)$, wrist condyle flush with forearm wrist, fingers wrapping forward around wand.
    * Wand: Position $(0.01\text{m}, 0.060\text{m}, 0.15\text{m})$, rotation $(-65.0^\circ, 10.0^\circ, -15.0^\circ)$, extending forward/upward through grasp directly toward orb.
    * Orb & Tails: Transformed directly relative to `rightHand`'s moving reference frame in a 3D helical orbit, with Point Light 0 tracking the orb in world space.
  * **Left Arm (Relaxed Companion Limb at left side / hip):**
    * Shoulder: Position $(-0.36\text{m}, 1.62\text{m}, 0.05\text{m})$, rotation $(-15.0^\circ, 0.0^\circ, 22.0^\circ)$, hanging naturally at side.
    * Elbow: Position $(0.0\text{m}, -0.38\text{m}, 0.0\text{m})$, rotation $(35.0^\circ, 0.0^\circ, 0.0^\circ)$, bent gently forward at $+35^\circ$.
    * Left Hand: Position $(0.0\text{m}, -0.42\text{m}, 0.0\text{m})$, rotation $(10.0^\circ, 0.0^\circ, 180.0^\circ)$. Fingertips extend downward along the side of the yukata/robe, wrist connects flush without gaps, and palm rests comfortably along the hip.
  * **Dynamic Kinematics (`Magician::update`):**
    * Right arm spellcasting flourishes: $\pm 16^\circ$ shoulder sweep, $\pm 18^\circ$ elbow bend, $\pm 22^\circ$ wrist flicking.
    * Left arm companion posture: subtle $\pm 4^\circ$ shoulder, $\pm 5^\circ$ elbow, $\pm 3^\circ$ wrist breathing sway.
    * Head continues smooth $\pm 4^\circ$ horizontal scanning across the seated audience.

---

### [2026-10-10] — Fix: Magician Wand in Left Hand, Right Hand Rest/Sway, Audience-Facing Alignment & Articulated Kinematics

#### 1. Left-Hand Wand Grasp & Magic Orb Reference Frame (`Objects.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Problem Addressed:**
  * The wand and orb were previously attached to the right hand, whereas requirements specify the left hand must grasp the magic wand and command the magic orb, with the right hand serving as the companion arm.
* **Fix & Articulated Rig Updates:**
  * **Left Arm Spellcasting Hierarchy:** Configured the left arm as the spellcasting limb:
    $$\text{root} \xrightarrow{\text{Shoulder}} \text{leftArm} \xrightarrow{\text{Elbow}} \text{leftForearm} \xrightarrow{\text{Wrist}} \text{leftHand} \xrightarrow{\text{Grasp}} \text{wand} \ \& \ \text{orbNode}$$
    * Shoulder: Position $(-0.36\text{m}, 1.62\text{m}, 0.05\text{m})$, raised forward and abducted outward.
    * Elbow: Position $(0.0\text{m}, -0.38\text{m}, 0.0\text{m})$, bent upward/forward at $+42^\circ$ pitch toward the orb.
    * Wrist (`leftHand`): Located at forearm distal condyle $(0.0\text{m}, -0.36\text{m}, 0.0\text{m})$.
    * Left Palm: Positioned at $(-0.01\text{m}, 0.035\text{m}, 0.065\text{m})$ with rotation $(-65.0^\circ, -10.0^\circ, 15.0^\circ)$, grasping the wand forward toward the orb.
    * Wand: Positioned at $(-0.01\text{m}, 0.060\text{m}, 0.15\text{m})$ with rotation $(-65.0^\circ, -10.0^\circ, 15.0^\circ)$, extending forward/upward toward the audience and orb.
    * Orb & Tails: Transformed directly relative to `leftHand`'s moving reference frame in a 3D helical orbit, with Point Light 0 tracking the orb in world space.
  * **Right Arm Companion Hierarchy:** Configured the right arm as the relaxed companion limb:
    * Shoulder: Position $(+0.36\text{m}, 1.62\text{m}, 0.05\text{m})$, hanging naturally at side.
    * Elbow: Position $(0.0\text{m}, -0.38\text{m}, 0.0\text{m})$, bent gently at $+35^\circ$ pitch.
    * Right Hand: Placed at $(0.0\text{m}, -0.42\text{m}, 0.0\text{m})$ with rotation $(10.0^\circ, 0.0^\circ, 180.0^\circ)$. Fingertips extend downward along the side of the yukata/robe, wrist connects flush to forearm distal joint without gaps, and palm rests comfortably along the hip.

#### 2. Audience-Facing Alignment & Interactive Gaze Kinematics (`Objects.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Alignment Details:**
  * Magician root stands on the stage at $(6.2\text{m}, 0.95\text{m}, -19.0\text{m})$ facing $+Z$ (`rotation = (0, 0, 0)`), directly facing the spectator benches placed in front of the stage at $Z \in [-14.5\text{m}, -12.7\text{m}]$ (which face $-Z$ at $180^\circ$ yaw).
  * Magician's head is tilted downward at $+4.0^\circ$ pitch (`rotation = (4.0f, 0.0f, 0.0f)`), making direct eye contact with the seated spectators below.
  * Hat brim and cone are parented directly to `head` (offsets $Y = +0.27\text{m}$ and $+0.82\text{m}$), staying locked with head rotation.
  * In `Magician::update(float time)`, the left arm performs dynamic spellcasting flourishes ($\pm 16^\circ$ shoulder, $\pm 18^\circ$ elbow, $\pm 22^\circ$ wrist flicking), the right arm performs subtle breathing and posture swaying ($\pm 4^\circ$ shoulder, $\pm 5^\circ$ elbow, $\pm 3^\circ$ wrist), and the head scans smoothly across the seated audience ($\pm 4^\circ$ yaw).

---

### [2026-10-10] — Fix: Magician Hands Orientation & Stall Vendor Streetward Facing Correction

#### 1. Magician Anatomical Hand & Wand Alignment (`Objects.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Problem Addressed:**
  * Magician's left hand was rotated with a rigid 90-degree Z-axis tilt (`rotation.z = 90.0f`), protruding awkwardly sideways from the body instead of naturally following the downward vector of the forearm.
  * Magician's right hand mesh (`palm`) had inverted Y/Z axes relative to forearm space (`rotation.x = -15.0f`), directing fingertips backward into the magician's sleeve rather than forward grasping the wand towards the hovering spellcasting orb.
* **Fix & Anatomical Realignment:**
  * **Left Hand Natural Extension:** Realigned left hand to `transform.position = (0.0f, -0.42f, 0.0f)` with `rotation = (10.0f, 0.0f, 180.0f)`. Flips the hand geometry so fingertips point downwards in line with the forearm, carpal wrist connects flush to forearm $(0, -0.36, 0)$ without a visible gap, and the palm rests naturally against the side of the yukata/robe.
  * **Right Hand & Wand Grasp Realignment:** Realigned `palm` and `wand` along matching pitch and yaw angles:
    * `palm`: `position = (0.01f, 0.035f, 0.065f)`, `rotation = (-65.0f, 10.0f, -15.0f)`
    * `wand`: `position = (0.01f, 0.060f, 0.15f)`, `rotation = (-65.0f, 10.0f, -15.0f)`
    The wrist connects flush to the wrist joint $(0, 0, 0)$, fingers grasp the wand handle firmly, and the wand extends forward and upward directly aiming into the dynamic orb's helical orbit.
  * **Vendor Figure Hand Alignment:** Applied matching anatomical corrections to `VendorFigure` left and right hands in `Objects.h` so resting arm hands hang naturally and cooking hands grip their utensils facing forward over the cooking surfaces.

#### 2. Stall Vendor Streetward Facing Correction (`Scene.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Problem Addressed:**
  * Due to inverted stall placement vectors, the food stall vendors (`Vendor_Takoyaki` and `Vendor_Kakigori`) were facing the back wall away from their cooking carts, with their backs turned towards the central festival promenade road.
* **Fix & Orientation Correction:**
  * Corrected `Vendor_Takoyaki` at $X = -6.65\text{m}$ to yaw `+90.0f`, orienting chest, face, and forward-reaching arms towards $+X$ (through the counter at $X = -5.2\text{m}$ and directly facing the central road $X = 0$).
  * Corrected `Vendor_Kakigori` at $X = +6.65\text{m}$ to yaw `-90.0f`, orienting chest, face, and arms towards $-X$ (through the counter at $X = +5.2\text{m}$ and directly facing the central road $X = 0$).

---

### [2026-10-10] — Comprehensive Engine Optimization & Frame Rate Restoration

#### 1. Performance Bottleneck Analysis & Optimization Overview
* **Files Modified:**
  * [`Shader.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Shader.h)
  * [`Mesh.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Mesh.h)
  * [`Transform.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Transform.h)
  * [`SceneNode.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/SceneNode.h)
  * [`Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
  * [`Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
  * [`Main.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Main.cpp)
  * [`shaders/basic.vert`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/shaders/basic.vert)
  * [`shaders/basic.frag`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/shaders/basic.frag)
  * [`Matsuri Nights — A Japanese Festival Street.vcxproj`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street.vcxproj)
* **Goal & Scope:** Resolve severe lagging without altering, degrading, or disabling any project features, visual details, character kinematics, curved geometries, 16-sample PCF soft shadows, lighting effects, or interactive controls.

#### 2. Root Causes Identified & Fixes Implemented
1. **Shader Uniform Location Lookup Bottleneck (`Shader.h`, `SceneNode.h`):**
   * *Issue:* `glGetUniformLocation` was queried dynamically on every setter (`setMat4`, `setVec4`, `setFloat`, `setBool`) across 4,000+ scene graph nodes every frame. This resulted in >30,000 synchronous OpenGL driver queries per frame (~1.8 million per second), stalling the CPU-GPU pipeline.
   * *Fix:*
     * Added `std::unordered_map<std::string, GLint> m_UniformLocationCache` in `Shader` to cache all uniform locations.
     * Introduced static direct-location setters (`Shader::setMat4(GLint loc, ...)`, `Shader::setVec3`, etc.) that execute in $O(1)$ with zero string hashing or driver queries.
     * Created `RenderContext` and `DepthRenderContext` in `SceneNode.h` that pre-resolve uniform IDs once per frame and stream them directly into draw calls.
2. **Redundant World Matrix Multiplications Across Static Geometry (`Transform.h`, `SceneNode.h`):**
   * *Issue:* `rootNode->updateWorldMatrix(1.0f)` was recursively recalculating local matrices (involving multiple trigonometric functions and matrix multiplications) and world matrices for thousands of static nodes (walls, roofs, stairs, furniture, terrain) that never move.
   * *Fix:*
     * Implemented local matrix caching (`cachedLocalMatrix`) with dirty tracking (`checkDirty()`) in `Transform`.
     * Added `lastParentMatrix` comparison and `matrixDirty` flag in `SceneNode::updateWorldMatrix()`. Static subtrees bypass matrix recalculation completely if neither their local transform nor their parent transform has changed.
3. **Vertex Shader Per-Vertex Matrix Inversion (`shaders/basic.vert`):**
   * *Issue:* `Normal = mat3(transpose(inverse(model))) * aNormal;` computed full 4x4 matrix inversions per-vertex across 200,000+ vertices every frame (over 12 million inversions/second on vertex ALUs).
   * *Fix:* Precomputed `normalMatrix = glm::transpose(glm::inverse(glm::mat3(worldMatrix)))` on CPU only when a node's transform changes, and passed `uniform mat3 normalMatrix` directly to the vertex shader. Vertex shader now executes a single 3x3 matrix multiplication.
4. **Driver VAO State Thrashing (`Mesh.h`):**
   * *Issue:* `glBindVertexArray(0)` was called after every single mesh draw, forcing the driver to bind and unbind VAOs thousands of times per frame across identical primitives (e.g., 224 falling petals, 154 canopy lobes).
   * *Fix:* Implemented `inline static unsigned int s_CurrentBoundVAO` tracking in `Mesh::Draw()`. Consecutive draw calls sharing the same mesh VAO skip redundant binding, and unbinding to 0 between draws is eliminated.
5. **Directional Shadow Map Interior Culling (`Scene.h`, `Objects.h`):**
   * *Issue:* `renderShadowDepth()` rendered every micro-object inside closed and semi-enclosed Machiya rooms (fine cups, tea trays, tatami borders, cushions, beddings, 14 stair steps) into the 48-meter directional sun/moon shadow map, wasting thousands of vertex shader evaluations.
   * *Fix:* Added `castShadow` flag on `SceneNode` and tagged interior decorative nodes via `child->setCastShadowRecursive(false)`. Interior objects render with full Blinn-Phong lighting in the primary view, but are skipped in the outdoor shadow pass.
6. **Dynamic String Allocation in Lighting Uploads (`Scene.h`):**
   * *Issue:* `Scene::render()` generated dynamic strings (`std::string prefix = "pointLights[" + std::to_string(i) + "].";`) for 12 point lights every frame, causing 84 heap allocations and driver lookups per frame.
   * *Fix:* Pre-cached all point light uniform locations into `LightUniformLocations` upon initialization.
7. **Fragment Shader Lighting & PCF Optimization (`shaders/basic.frag`):**
   * *Issue:* Shaded fragments evaluated all 12 point lights and 16 PCF shadow samples regardless of distance or light reach.
   * *Fix:*
     * Added distance-squared early cutoff (`distSq > 700.0` / ~26m) and attenuation thresholds in `CalcPointLight` and `CalcSpotLight`.
     * Reused precalculated distance to eliminate redundant square roots.
     * Skipped specular exponent power calculations (`pow(..., shininess)`) on backfacing (`diff <= 0.0`) or unlit surfaces.
     * Replaced per-fragment `textureSize(shadowMap, 0)` with constant texel size `vec2(1.0 / 2048.0)` and added frustum-bounds early out.
8. **Live FPS / Frame-Time Monitoring (`Main.cpp`):**
   * Added dynamic window title updater displaying smoothed FPS and frame time in milliseconds (e.g. `Matsuri Nights - A Japanese Festival Street [CSE4102] | FPS: 60.0 (16.6 ms)`).
9. **Visual Studio Project Configuration (`vcxproj`):**
   * Configured `IncludePath`, `LibraryPath`, and `AdditionalDependencies` for `Release|x64`, enabling fully optimized builds with MSVC LTCG (`/O2 /Oi /Ot /GL`) in addition to the existing `Debug|x64` target.

---

### [2026-10-10] — Fix: Shopkeeper Orientation & Food Cart Clearance + Magician Arm/Hand Kinematics

#### 1. Shopkeeper Orientation & Stall Clearance (`Objects.h`, `Scene.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Problem Addressed:**
  * Shopkeepers of both stalls (Takoyaki and Kakigori) were facing 180 degrees away from the front of the shop (facing backward into the rear wall instead of outward towards the counter and customers).
  * Shopkeepers were positioned at $X = \pm 6.05\text{m}$, which intersected and stood inside the food cart table base ($X \in [\pm 4.25\text{m}, \pm 6.15\text{m}]$) and counter overhang ($X = \pm 6.30\text{m}$).
  * Cooking pick arm rotation in `VendorFigure` was using positive X rotation, causing the arm to swing backwards behind the vendor's back instead of reaching forward over the counter.
* **Fix & Solution:**
  * **180° Orientation Flip:** Flipped yaw rotation for both vendors:
    * `Vendor_Takoyaki`: Changed from $90.0^\circ$ to $-90.0^\circ$ so the vendor directly faces the front counter and customers.
    * `Vendor_Kakigori`: Changed from $-90.0^\circ$ to $90.0^\circ$ so the vendor directly faces the front counter and customers.
  * **Food Cart Clearance Translation:** Translated both vendors backwards from the stall:
    * `Vendor_Takoyaki`: Moved from $X = -6.05\text{m}$ to $X = -6.65\text{m}$ ($0.35\text{m}$ behind counter back edge, platform tucked neatly under counter).
    * `Vendor_Kakigori`: Moved from $X = +6.05\text{m}$ to $X = +6.65\text{m}$ ($0.35\text{m}$ behind counter back edge).
  * **Forward Cooking Kinematics:** Updated `VendorFigure` arm resting pose (`hand->rotation.x = -30.0f`) and animated forward reaching motion:
    $$\theta_{\text{shoulder}} = -28^\circ + \sin(4t) \times 14^\circ, \quad \theta_{\text{elbow}} = -25^\circ + \sin(4t + 0.5) \times 16^\circ$$
    Reaching naturally forward over the counter and actively stirring the takoyaki grill plate / shaved ice bowl.

#### 2. Magician Hand Movement & Articulated Spellcasting Rig (`Objects.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Problem Addressed:**
  * The magician's hand movement was completely non-functional: `rightHand` was placed directly as a child of `root` at fixed coordinates $(0.9, 2.3, 0.5)$, disconnected from the arm hierarchy.
  * `rightArm` and `rightForearm` were static local variables in the constructor, never articulated or updated in `Magician::update()`.
* **Fix & Solution:**
  * **Full Anatomical Hierarchy:** Added `rightArm`, `rightForearm`, `rightHand`, and `wand` member variables to `Magician`:
    $$\text{root} \xrightarrow{\text{Shoulder}} \text{rightArm} \xrightarrow{\text{Elbow}} \text{rightForearm} \xrightarrow{\text{Wrist}} \text{rightHand} \xrightarrow{\text{Held}} \text{wand} \ \& \ \text{orbNode}$$
  * **Dynamic Multi-Joint Kinematics:** Implemented dynamic spellcasting animations in `Magician::update(float time)`:
    * Shoulder: $\theta_x = -62^\circ + \sin(2.4t) \times 16^\circ$, $\theta_y = 18^\circ + \cos(1.8t) \times 12^\circ$, $\theta_z = -22^\circ + \sin(1.6t) \times 8^\circ$.
    * Elbow: $\theta_x = 42^\circ + \sin(2.6t + 0.5) \times 18^\circ$, $\theta_y = \cos(2.2t) \times 10^\circ$.
    * Wrist Flourishes & Wand Flicking: $\theta_x = -15^\circ + \sin(3.4t) \times 22^\circ$, $\theta_y = \cos(2.8t) \times 16^\circ$, $\theta_z = -20^\circ + \sin(2.5t) \times 12^\circ$.
    * Left Arm Breathing Motion: $\theta_x = -15^\circ + \sin(1.6t) \times 4^\circ$, $\theta_{\text{elbow}} = 35^\circ + \cos(1.6t) \times 5^\circ$.
  * **Dynamic Magic Orb Reference Frame:** Magic orb and its 3 trailing comet particles dynamically execute 3D helical orbits centered on the articulated moving hand reference frame, driving the dynamic stage point light in real time.

---

### [2026-10-10] — Cherry Tree Road Clearance & Off-Road Placement (Plaza Verges & Entrance Lawns)

#### 1. Road Boundary Separation & Sakura Tree Relocation (`Scene.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu\OneDrive\Desktop\practice\Graphics\Matsuri-Nights-A-Japanese-Festival-Street\Matsuri Nights — A Japanese Festival Street\src\Scene.h)
* **Problem Addressed:**
  * The street stone pavement spans $X \in [-7.0\text{m}, +7.0\text{m}]$ with cobblestone borders at $X \in [-7.45\text{m}, -6.95\text{m}]$ and $X \in [+6.95\text{m}, +7.45\text{m}]$.
  * Previously, the 3 cherry trees near the Torii Gate ($X = -5.8\text{m}, +6.8\text{m}, -5.8\text{m}$) and the 2 cherry trees framing the south street entrance ($X = -5.6\text{m}, +5.6\text{m}$) were located within $|X| < 7.0\text{m}$, placing their trunks, roots, and falling blossom beds directly onto the paved stone roadway.
* **Solution & Updated Coordinates:**
  * Relocated all 5 trees outward onto the surrounding earth and grass terrain ($|X| \ge 10.0\text{m}$), providing $>2.55\text{m}$ clearance beyond the cobblestone borders and ensuring complete separation of roots, trunks, and pedestrians:
    1. **Grand Shrine Sakura Tree (Tree #1):** Moved from $(-5.8\text{m}, 0.0\text{m}, -22.0\text{m})$ to $(-10.2\text{m}, 0.0\text{m}, -22.0\text{m})$ (West plaza lawn, completely off the roadway).
    2. **Festival Stage Plaza Sakura Tree (Tree #2):** Moved from $(+6.8\text{m}, 0.0\text{m}, -26.5\text{m})$ to $(+10.2\text{m}, 0.0\text{m}, -26.5\text{m})$ (East plaza lawn, clear of roadway and magic stage).
    3. **Left Courtyard Garden Gap (Tree #3):** Retained at $(-10.8\text{m}, 0.0\text{m}, 5.0\text{m})$ (already located in the courtyard between Machiya L1 & L2).
    4. **Right Courtyard Garden Gap (Tree #4):** Retained at $(+10.8\text{m}, 0.0\text{m}, 5.0\text{m})$ (already located in the courtyard between Machiya R1 & R2).
    5. **South Entrance Avenue West (Tree #5):** Moved from $(-5.6\text{m}, 0.0\text{m}, 28.0\text{m})$ to $(-10.0\text{m}, 0.0\text{m}, 28.0\text{m})$ (West entrance lawn outside roadway).
    6. **South Entrance Avenue East (Tree #6):** Moved from $(+5.6\text{m}, 0.0\text{m}, 27.0\text{m})$ to $(+10.0\text{m}, 0.0\text{m}, 27.0\text{m})$ (East entrance lawn outside roadway).
    7. **North Torii Sacred Grove Sakura Tree (Tree #7):** Moved from $(-5.8\text{m}, 0.0\text{m}, -31.5\text{m})$ to $(-10.2\text{m}, 0.0\text{m}, -33.5\text{m})$ (West of Torii gate Kasagi lintel tip, framing the shrine approach on natural soil).

---

### [2026-10-10] — Organic Curved Botanical Foliage (Leaves, Petals, Evergreen Pads), Sculpted Sakura Blossom Clouds, Multi-Tree Street Population & Articulated Biomechanical Human Rigs

#### 1. Procedural Botanical Curve Generators (`Curves.h`, `Primitives.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Curves.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Curves.h), [`Matsuri Nights — A Japanese Festival Street/src/Primitives.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Primitives.h)
* **Mathematical Foundations & Implementation:**
  * **Botanical Curved 3D Leaf Mesh (`Curves::createCurvedLeafMesh`):** Replaced flat rectangular boxes with smooth botanical 3D leaf surfaces governed by parametric longitudinal parabolic arching ($\Delta y_{\text{arch}} = h_{\text{arch}} \cdot \sin(\pi u)$), natural teardrop width expansion ($w(u) = w_{\max} \cdot \sin(\pi u) \cdot (1.0 - 0.25u)$), transversal cupping fold angle ($\Delta y_{\text{cup}} = |v| \cdot \sin(\theta_{\text{cup}})$), smooth analytical vertex normals, seamless $(u,v)$ UV coordinate mapping, and double-sided triangle indices for realistic two-sided rendering.
  * **Organic Cupped Flower Petal Mesh (`Curves::createCurvedPetalMesh`):** Parametric concave cupped petal geometry featuring radial curvature, scalloped organic tips, and smooth double-sided normal generation.
  * **Multi-Lobed Sakura Blossom Canopy Mesh (`Curves::createSakuraBlossomLobe`):** Replaced primitive geometric spheres with organic cherry blossom cloud lobes perturbed by procedural spherical harmonics and billow ripples:
    $$R(\theta, \phi) = r \cdot \left(1.0 + 0.18 \sin(5\phi)\cos(3\theta) + 0.10 \sin(3\phi)\sin(4\theta) + 0.06 \cos(7\theta)\right)$$
    Producing fluffy, organic, clustered floral canopies that catch light naturally.
  * **Layered Tiered Pine Needle Cluster Mesh (`Curves::createPineNeedleClusterMesh`):** Tiered radial evergreen foliage pads designed for traditional Japanese bonsai trees and window garden shrubbery.

#### 2. Fully Connected Organic Floral Anatomy (`Objects.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Features:**
  * **Window Planter Box Gardens (`createWindowPlanterBox`):** Replaced primitive spheres and cylinders with `pineCluster` shrub bases, swept 3D Bézier stems, botanical `curvedLeaf` meshes sprouting naturally along stem curves, and 4-petal floral rosettes composed of cupped `curvedPetal` petals seamlessly attached to stem tips.
  * **Cascading Kokedama Moss Balls (`createHangingKokedama`):** Added botanical `curvedLeaf` foliage along swept 3D Bézier vines and cupped blossom rosettes at vine terminations.
  * **Ikebana Flower Arrangements (`createIkebanaVase`):** Replaced flat box leaves with botanical `curvedLeaf` meshes aligned to the tangent vectors of Shin, Soe, Hikae, and Accent living line stems; replaced sphere blooms with multi-petal rosettes using `curvedPetal` attached rigidly to calyx bases without spatial gaps.
  * **Gnarled Bonsai Trees (`createBonsaiTree`):** Sculpted branch foliage clouds replaced with tiered `pineCluster` evergreen pads oriented naturally along the swept *Moyogi* serpentine branches.

#### 3. Sakura Tree Enhancements & Avenue-Wide Population (`Objects.h`, `Scene.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Features:**
  * **Sculpted Sakura Canopy Lobes:** Replaced all 22 canopy spheres on every Sakura tree with organic `sakuraBlossomLobe` clusters.
  * **Botanical Leaf Sprigs:** Embedded 9 fresh green botanical leaf sprigs (`curvedLeaf`) along primary and secondary bough junctions, capturing authentic spring Sakura blossoming aesthetics where tender leaves emerge alongside blossoms.
  * **Cupped Falling Petals:** Updated falling petal particle system to instantiate parametric `curvedPetal` meshes drifting and tumbling in the wind.
  * **Multi-Tree Street & Courtyard Population:** Expanded from a single tree to 7 distinct, fully instantiated Sakura trees across the festival grounds:
    1. **Grand Shrine Sakura Tree:** $X = -5.8\text{m}, Z = -22.0\text{m}$ (Scale: 1.15, Rotation: $15^\circ$, Tone: $0.00$)
    2. **Festival Stage / Plaza Sakura Tree:** $X = +6.8\text{m}, Z = -26.5\text{m}$ (Scale: 1.05, Rotation: $135^\circ$, Tone: $+0.25$)
    3. **Left Courtyard Garden Gap (between Machiya L1 & L2):** $X = -10.8\text{m}, Z = +5.0\text{m}$ (Scale: 0.95, Rotation: $75^\circ$, Tone: $-0.20$)
    4. **Right Courtyard Garden Gap (between Machiya R1 & R2):** $X = +10.8\text{m}, Z = +5.0\text{m}$ (Scale: 0.95, Rotation: $210^\circ$, Tone: $+0.30$)
    5. **South Entrance Avenue West:** $X = -5.6\text{m}, Z = +28.0\text{m}$ (Scale: 0.90, Rotation: $40^\circ$, Tone: $-0.15$)
    6. **South Entrance Avenue East:** $X = +5.6\text{m}, Z = +27.0\text{m}$ (Scale: 0.92, Rotation: $190^\circ$, Tone: $+0.15$)
    7. **North Torii Sacred Grove:** $X = -5.8\text{m}, Z = -31.5\text{m}$ (Scale: 0.85, Rotation: $290^\circ$, Tone: $-0.10$)
  * Trees feature individual scale, azimuth rotation, trunk lean, and subtle petal color tone variation (ranging from pale white-pink to rich sakura rose). All trees are integrated into the Bark texture mapping and particle wind drift simulation.

#### 4. Articulated Anatomical Human Models & Biomechanical Kinematics (`Curves.h`, `Primitives.h`, `Objects.h`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Curves.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Curves.h), [`Matsuri Nights — A Japanese Festival Street/src/Primitives.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Primitives.h), [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Features:**
  * **Procedural Anatomical Geometry Generators:**
    * **Sculpted Human Head (`createHumanHeadMesh`):** Anatomical facial contours with chin, tapered jaw, cheekbone structure, nasal bridge, eye sockets, and lips.
    * **Contoured Human Torso (`createHumanTorsoMesh`):** Chest flare, waist taper, and authentic traditional crossed kimono/yukata collar (*Eri*).
    * **Articulated Muscle Limb Segments (`createArticulatedLimbMesh`):** Limb mesh with rounded joint pivot condyle at $Y = 0$, anatomical muscle bulge (quadriceps/gastrocnemius or biceps/brachioradialis), and smooth distal taper.
    * **Contoured Hands (`createHandMesh`):** Palm and fingers.
    * **Japanese Geta Sandals (`createGetaFootMesh`):** Contoured wooden platform (*Dai*) with dual elevated teeth (*Ha*) sitting flush at ground level.
  * **Articulated Walking Crowd Kinematics (`CrowdGroup`):**
    * Hierarchical joint rig: Pelvis $\to$ Thighs $\to$ Shins $\to$ Geta Feet; Torso $\to$ Shoulders $\to$ Upper Arms $\to$ Forearms $\to$ Hands; Head.
    * Forward/backward hip swing: $\theta_{\text{hip}} = \sin(\omega t) \times 24^\circ$.
    * Natural biomechanical knee flexion (backward bending only):
      $$\theta_{\text{knee}} = \max\left(0, -\theta_{\text{hip}} \times 1.35\right)$$
    * Natural arm swing with biomechanical forward elbow flexion:
      $$\theta_{\text{elbow}} = 18^\circ + \max\left(0, \theta_{\text{arm}} \times 0.65\right)$$
  * **Articulated Stall Vendors (`VendorFigure`):**
    * Equipped with Happi coat torso, Hachimaki headband head, articulated legs, and articulated upper/lower arms holding cooking utensils with live stirring/flipping kinematics.
  * **Articulated Stage Magician (`Magician`):**
    * Sculpted wizard head, cape torso, boots, articulated limbs with wand-wielding arm animating levitation spells.
  * **Articulated Audience Spectators (`AudienceGroup`):**
    * 6 seated spectators on Mousen benches with realistic horizontal thigh resting angle ($-85^\circ$), vertical shin drop ($+85^\circ$), wooden Geta feet on the floor, upright torsos, and contoured hands resting comfortably on knees.

---

### [2026-10-10] — Implementation of Mathematical Curves Architecture & Organic Swept Geometries (Splines, Bézier Curves, Bishop Frames, Swept Tubes, Curved Beams & Ropes)

#### 1. Mathematical Curves & Swept Geometry Architecture (`Curves.h`, `Primitives.h`, `SceneNode.h`)
* **Files Added:** [`Matsuri Nights — A Japanese Festival Street/src/Curves.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Curves.h)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Primitives.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Primitives.h), [`Matsuri Nights — A Japanese Festival Street/src/SceneNode.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/SceneNode.h), [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Mathematical Foundations:**
  * **Quadratic & Cubic Bézier Curves (`Curves::Bezier2`, `Curves::Bezier3`):** Parametric 3D polynomial curves with analytical first derivative tangent vectors $\mathbf{B}'(t)$ for precise orientation alignment.
  * **Catmull-Rom Splines (`Curves::CatmullRomSpline`):** Smooth $C^1$ cubic spline interpolation through arbitrary 3D waypoint sequences.
  * **Parallel Transport Frames (Bishop Frames / Rotation-Minimizing Frames):** Implemented `Curves::computeBishopFrames` using Rodrigues' rotation formula. Eliminates the twisting artifacts and inflection singularities inherent to classical Frenet-Serret frames, producing continuous, smooth normal and binormal basis vectors along any 3D trajectory.
  * **Generalized Swept Tube Generator (`createSweptTube`):** Sweeps circular cross-sections with variable radius functions $r(t)$ (linear or non-linear tapering) along arbitrary 3D curves. Computes smooth vertex normals, seamless $(u, v)$ texture wrapping, and watertight start/end caps.
  * **Swept Curved Architectural Beams (`createCurvedBeam`):** Sweeps rectangular or beveled trapezoidal profiles along upward parabolic curves with flared wingtips (*sori* / *nokizori*) and beveled roof ridges.
  * **Continuous Catenary Rope Mesh (`createCatenaryRope`):** Evaluates mathematical catenary sag formulas into a continuous, smooth tubular rope mesh.
  * **`SceneNode` Procedural Mesh Ownership:** Added `ownedMesh` and `setMesh()` to `SceneNode`, enabling seamless lifecycle management for procedural curved geometries alongside shared primitives.

#### 2. Organic Sakura Tree: Swept 3D Spline Trunk, Roots & Boughs (`SakuraTree`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Features:**
  * **Continuous Spline Trunk (`Sakura_Trunk_Curved`):** Replaced stacked straight cylinders with a continuous 3D Catmull-Rom spline tube (6 waypoints) featuring an organic flaring buttress base ($r = 0.76\text{m}$), windswept natural twists, and smooth taper to the crown ($r = 0.42\text{m}$) with continuous bark texture mapping.
  * **5 Organic Curved Root Spurs:** Swept 3D cubic Bézier tubes arching down and out into the earth ($r = 0.32\text{m} \to 0.10\text{m}$), replacing straight rigid sticks.
  * **5 Primary Spreading Scaffold Boughs:** Swept 3D cubic Bézier tubes emerging from the trunk crown and arching gracefully into the canopy ($r = 0.28\text{m} \to 0.14\text{m}$).
  * **8 Secondary Curved Branches:** Swept 3D curved tubes branching upward to support the 22 cherry blossom cloud clusters and falling petal system.

#### 3. Authentic Japanese Ikebana Floral Living Lines (`createIkebanaVase`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Features:**
  * Replaced straight rigid cylinder stems with swept curved 3D tubes embodying traditional Japanese *Kadō* living line aesthetics:
    * **Shin (Heaven/Truth):** Elegant upright sweeping arc curve (`ikebanaStemShin`).
    * **Soe (Man/Supporting):** Dynamic outward and upward S-curve (`ikebanaStemSoe`).
    * **Hikae (Earth/Restrained):** Low arching sweeping bow curve (`ikebanaStemHikae`).
    * **Accent:** Delicate curving accent stem (`ikebanaStemAccent`).
  * Green leaves sprout along the exact curved trajectories, and green calyx cups, blossoms, petal lobes, and golden stamens nestle seamlessly at the curved stem tips.

#### 4. Gnarled Windswept Miniature Bonsai Trees (`createBonsaiTree`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Features:**
  * **Spline Bonsai Trunk (`bonsaiTrunkCurved`):** Replaced 3 disconnected cylinders with a continuous 3D Catmull-Rom spline tube in classic *Moyogi* (informal upright) serpentine style, tapering from $0.065\text{m}$ at the mossy soil to $0.030\text{m}$ at the crown.
  * **Curved Bonsai Branches (`bonsaiBranchCurved1`, `bonsaiBranchCurved2`):** Swept 3D curved limbs arching gracefully beneath the sculpted evergreen foliage clouds.

#### 5. Curved Window Planter Stems & Cascading Kokedama Vines
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Features:**
  * **Window Planter Boxes (`createWindowPlanterBox`):** Replaced straight vertical cylinder stems with organic curved stems (`planterStemCurvedA`, `planterStemCurvedB`) bowing gently under flower blossom weight.
  * **Hanging Kokedama Moss Balls (`createHangingKokedama`):** Replaced straight rods with swept 3D Bézier ivy vines (`kokedamaVineCurved1`, `kokedamaVineCurved2`) cascading gracefully from the moss balls.

#### 6. Grand Torii Gate Architectural Curves (`ToriiGate`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Features:**
  * **Kasagi (Upper Lintel):** Replaced flat rectangular box with a swept curved beam featuring authentic Japanese upward *sori* ($0.40\text{m}$ upward curve), flared wingtips, and beveled roof ridge cap (`curvedKasagi`, $15.4\text{m}$ span).
  * **Shimaki (Sub-Lintel):** Replaced flat box with a matching swept curved beam with upward *sori* ($0.28\text{m}$ curve) finished in vermilion lacquer (`curvedShimaki`, $14.2\text{m}$ span).

#### 7. Continuous Catenary Street Lantern Ropes (`StreetLanternSpan`)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Features:**
  * Replaced 10 faceted cylinder segments with a single continuous, perfectly smooth swept 3D catenary rope mesh (`catenaryRope`) spanning $7.6\text{m}$ between cedar poles with natural sag ($0.65\text{m}$).

---

### [2026-10-10] — Machiya Townhouse Staircase Relocation & Spacious 2nd Floor Landing Gallery Clearance

#### 1. Staircase Repositioning & Upper Floor Landing Clearance Fix
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Issues Resolved:**
  * **Cramped Stairwell Arrival:** The staircase run was previously positioned ascending towards $X = -3.60\text{m}$ along $Z = -3.50\text{m}$. With the exterior perimeter corner wall located at $X = -3.70\text{m}$, there was only $10\text{cm}$ ($0.10\text{m}$) of clearance between the top stair tread and the solid back wall.
  * **Player Pinch & Immobility:** With player collision radius ($0.35\text{m}$), the player became immediately pinched against the back corner wall upon climbing the top step, unable to turn or step onto the second floor bedroom tatami.
* **Solutions Implemented:**
  * **Forward Relocation of Staircase Run:**
    * Relocated the 14-step staircase run along $Z = -3.50\text{m}$ from $[X = -0.60\text{m} \to -3.60\text{m}]$ forward to $[X = +0.75\text{m} \to -1.95\text{m}]$.
    * The foot of the stairs starts conveniently near the ground-floor living area ($X = +0.75\text{m}$); the top step finishes at $X = -1.95\text{m}$.
    * Clearance between the top tread and the exterior back wall ($X = -3.70\text{m}$) is now a generous **$1.75\text{m}$ (over 5.7 feet)**, creating a wide, open walkway.
  * **Spacious 2nd Floor Landing Gallery (`floor2Landing`):**
    * Created an expansive tatami landing gallery spanning $X \in [-3.70\text{m}, -1.95\text{m}]$, $Z \in [-4.15\text{m}, -2.85\text{m}]$ ($1.75\text{m} \times 1.30\text{m}$) at $Y = 4.22\text{m}$ with top surface at $Y = 4.30\text{m}$, meeting the top stair tread flush.
    * Floor ahead of the stair foot (`floor2Front`) spans $X \in [+0.75\text{m}, +3.70\text{m}]$, $Z \in [-4.15\text{m}, -2.85\text{m}]$.
    * Unobstructed egress: Along $Z > -2.88\text{m}$, the entire $1.75\text{m}$ width of the landing gallery is open into the master bedroom with zero guardrails, walls, or obstacles.
  * **Cedar Balustrade Guardrail Overhaul:**
    * Reconfigured the longitudinal cedar balustrade along the stairwell opening edge at $Z = -2.88\text{m}$ from $X = -1.95\text{m}$ to $X = +0.75\text{m}$ ($2.70\text{m}$ length), complete with top rail, bottom rail, landing newel post, front newel post, and 6 turned vertical balusters.
    * Added a transverse front-end guardrail across $X = +0.75\text{m}$ from $Z = -2.88\text{m}$ to $Z = -4.15\text{m}$ ($1.27\text{m}$ width) with an anchor wall post and 3 balusters to prevent players from falling into the lower stair run from the second floor.
  * **Walkway Clearance:**
    * Repositioned the upper-floor decorative flower stand and Ikebana vase (`f2Ikebana`) from $Z = -1.80\text{m}$ to $Z = -0.20\text{m}$ against the center back wall, keeping the landing gallery completely clear of furniture.
  * **Collision Kinematics & Solid Guardrail Updates (`Scene::resolveCollision`):**
    * Adjusted the staircase climbing bounding volume to $X \in [-2.00\text{m}, +0.85\text{m}]$, $Z \in [-4.15\text{m}, -2.85\text{m}]$.
    * Updated parametric height interpolation: `stairT = (0.75f - newL.x) / 2.70f`, seamlessly elevating player eye height from ground level ($1.65\text{m}$) to upper floor ($5.75\text{m}$) when climbing, and stepping down smoothly when descending.
    * Landing zone ($X \le -1.95\text{m}$, $Z \in [-4.15\text{m}, -2.85\text{m}]$) is recognized as solid 2nd floor, locking player eye height at $5.75\text{m}$.
    * Added solid blocking collision along the balustrades at $Z = -2.85\text{m}$ and $X = +0.75\text{m}$, preventing accidental falls into the open stairwell.

---

### [2026-10-10] — Machiya Townhouse Bug Fixes & Refinements: Windows Architecture, Stairwell-Floor Connection, and Fully Connected Floral Anatomy

#### 1. Windows Architecture, Wall Cutouts & Dual-Track Sliding Mechanics Fix
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Issues Resolved:**
  * **Frame Occlusion:** Previously, window outer frames were solid cubes (`winF1_OuterFrame`, `winFrame`) that completely obstructed the window apertures and hid the paper sashes behind solid wood.
  * **Solid 2nd Floor Facade:** The second-floor front wall was a solid monolithic block without window openings, causing windows to render inside the wall.
  * **Wall Penetration on Slide:** Sliding sashes had positive $Z$ sliding offsets ($+0.85\text{m}$, $+0.80\text{m}$) that pushed the geometry outward through the window jamb and into solid building walls.
  * **Z-Fighting on Lattice Ribs:** Wooden mullion ribs were coplanar with paper sashes at identical depth coordinates, causing flickering.
* **Solutions Implemented:**
  * **4-Piece Open Border Frames:** Replaced all solid window frames (Ground Floor front, Ground Floor back, and Second Floor front pairs) with 4-piece timber surrounds (top header, bottom sill, left jamb, right jamb) forming real hollow window apertures.
  * **2nd Floor Framed Facade Wall Cutouts:** Divided the second-floor front wall into framed timber sections (center wall, left wall, right wall, sub-sill walls, and lintel header walls) providing true open window bays so visitors can look outside onto the festival avenue and vice-versa.
  * **Dual-Track Parallel Sliding (-0.95m / -0.90m):** Sashes now slide along parallel wooden guide tracks (inner track vs outer track). When opened, the sliding sash retreats neatly behind the stationary sash, opening half the window aperture with **zero wall penetration**.
  * **Z-Fighting Elimination & Kumiko Grid:** Ribs and 3 horizontal lattice mullions per sash are offset slightly in front of the rice paper, eliminating coplanar depth conflicts while reproducing authentic Japanese Shoji craftsmanship. All lattice bars are registered to slide synchronously with the sash.

#### 2. Stairwell-to-Upper-Floor Connection & Kinematic Safety Fix
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Issues Resolved:**
  * **Interior Eaves Blockade:** An intermediate roof slab (`eaves1`, $8.8\text{m} \times 0.35\text{m} \times 10.0\text{m}$) was rendered as a solid cube across the entire house at $Y = 4.40\text{m}$, physically cutting across the staircase and ceiling.
  * **0.85m Floor Void at Stair Top:** The second-floor tatami slab stopped at $X = -2.75\text{m}$ while the top step ended at $X = -3.50\text{m}$, leaving a gaping hole between the staircase and the bedroom.
  * **Step Discontinuity:** Height difference between the top stair tread ($Y = 4.30\text{m}$) and the floor surface created a step bump.
* **Solutions Implemented:**
  * **Exterior Overhanging Eaves (*Hisashi*):** Converted `eaves1` into 4 exterior sloped cedar overhangs projecting outward over street, garden, and side walls, leaving the entire interior living space and stairwell open to the second floor.
  * **Full Tatami Floor System & Seamless Landing:**
    * `floor2Main`: Covers the main bedroom ($Z \in [-2.90\text{m}, +4.15\text{m}], X \in [-3.70\text{m}, +3.70\text{m}]$).
    * `floor2Landing`: Dedicated upper stair landing ($X \in [-3.70\text{m}, -3.40\text{m}], Z \in [-4.15\text{m}, -2.90\text{m}]$) that meets the top step tread flush at $Y = 4.30\text{m}$, completely eliminating the floor gap.
    * `floor2Front`: Covers the floor in front of the stair base ($X \in [-0.55\text{m}, +3.70\text{m}]$).
  * **Turned Cedar Balustrade:** Replaced the plain guard block with a crafted handrail, base rail, corner newel posts, and 6 vertical turned cedar balusters along the open stairwell edge ($Z = -2.88\text{m}$).
  * **Collision Kinematics & Solid Guardrail:** Updated `resolveCollision` so the player cannot accidentally walk off the bedroom floor into the stairwell opening; the solid guardrail physically stops the player, requiring them to enter through the top landing to walk down the stairs.

#### 3. Fully Connected Floral Anatomy Fix (Ikebana & Window Planters)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Issues Resolved:**
  * **Disconnected Ikebana Petals & Leaves:** Leaves and bloom spheres were placed using hardcoded world offsets that did not match rotated stem endpoints, causing petals to float in mid-air and leaves to detach.
  * **Hovering Planter Flowers:** Window planter blooms hovered in mid-air over soil without any stems.
* **Solutions Implemented:**
  * **Hierarchical Parent-Child Transforms (`createIkebanaVase`):**
    * Each flower branch is now a dedicated hierarchical `SceneNode` parent anchored at the vase rim.
    * The stem cylinder extends from $Y = 0$ to $Y = \text{stemLen}$.
    * Green leaves sprout directly off the stem at $0.42 \times \text{stemLen}$ and $0.68 \times \text{stemLen}$.
    * A green calyx cup cradles the stem tip.
    * Central blossom spheres and sculpted petal lobes are parented directly to the stem tip.
    * Golden stamen centers sit inside the petal core.
    * Because all components are child nodes, all floral parts remain rigidly and realistically connected regardless of branch rotation or scaling.
  * **Rooted Window Planters (`createWindowPlanterBox`):** Added slender green stems anchored into the soil bed ($Y = 0.18\text{m}$), green leaves, calyx cups, and golden stamen centers for every bloom.
  * **Material Shader Exemption:** Added `Calyx`, `Stem`, and `Branch` to `Scene::applyTexturesAndMaterials` to preserve rich organic vegetative colors without wood texture overrides.

---

### [2026-10-10] — Realistic Interior Lamps, Bright Room Illumination, Interactive Sliding Windows, 14-Step Hakokaidan Stair Kinematics, Authentic 2-Storied Machiya Rooms, Corner Ikebana Flower Vases, Detailed Sakura Blossom Tree, and Window Gardens (Planters, Bonsai & Kokedama)

#### 1. Realistic Interior Lamps (Andon Floor Lamps & Ceiling Pendant Washi Chandeliers)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Architectural Lighting Implementation:**
  * **Traditional Japanese Floor Lamp (*Andon*):** Created modular `createAndonFloorLamp` with 4 elevated corner feet, solid cedar base plinth, 4 vertical structural corner posts, fine *kumiko* lattice framing ribs, translucent washi paper diffuser panels (`color = (0.94, 0.90, 0.82)`), and an inner glowing flame core (`isEmissive = true`, `emissiveColor = (2.2, 1.6, 0.8)`). Placed beside the Tatami living room chabudai and second-floor futon bedroom in all Machiya townhouses.
  * **Ceiling Pendant Chandelier (*Tsurigomi-andon*):** Created `createCeilingPendantLamp` suspended from the exposed ceiling rafters via a ceiling mounting rosette and slender suspension cord. Features a multi-tier octagonal cedar wooden frame, warm cream washi paper shade, and a high-intensity emissive core (`isEmissive = true`, `emissiveColor = (2.8, 2.1, 1.1)`) radiating down into the living and bedroom chambers.

#### 2. Bright Interior Room Illumination (Indoor Skylight Bounce & 12-Point-Light Architecture)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/shaders/basic.frag`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/shaders/basic.frag), [`Matsuri Nights — A Japanese Festival Street/shaders/raytrace.frag`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/shaders/raytrace.frag), [`Matsuri Nights — A Japanese Festival Street/src/RayTracer.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/RayTracer.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Lighting Engine Expansion:**
  * Expanded GPU uniform array `NR_POINT_LIGHTS` from 6 to 12 in `shaders/basic.frag`.
  * Dedicated Point Lights 6 through 11 specifically for indoor townhouse illumination:
    * **Light 6:** Machiya L1 Ground Floor living/tea room (`(1.4, 1.05, 0.65)` intensity, $16.0\text{m}$ radius).
    * **Light 7:** Machiya L1 Second Floor master bedroom (`(1.35, 1.0, 0.6)` intensity, $16.0\text{m}$ radius).
    * **Light 8:** Machiya R1 Ground Floor living/tea room (`(1.4, 1.05, 0.65)` intensity, $16.0\text{m}$ radius).
    * **Light 9:** Machiya R1 Second Floor bedroom (`(1.35, 1.0, 0.6)` intensity, $16.0\text{m}$ radius).
    * **Light 10:** Machiya L2 interior living room (`(1.2, 0.9, 0.55)` intensity, $14.0\text{m}$ radius).
    * **Light 11:** Machiya R2 interior living room (`(1.2, 0.9, 0.55)` intensity, $14.0\text{m}$ radius).
  * **Indoor Indirect Skylight Bounce:** Integrated ambient window bounce in `CalcDirLight`:
    $$\text{indoorBounce} = \operatorname{mix}(0.48, 0.18, \text{dayNightFactor})$$
    providing soft secondary photon scatter throughout enclosed rooms, completely eliminating shadow blackout and dark corners while maintaining realistic contrast.
  * Synchronized the 6 interior point lights and indoor ambient baselines into both the GPU ray tracer (`shaders/raytrace.frag`) and multi-threaded CPU software ray tracer (`src/RayTracer.h`).

#### 3. Machiya Shoji Lattice Windows & Interactive Sliding System (<kbd>G</kbd>)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h), [`Matsuri Nights — A Japanese Festival Street/Main.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Main.cpp), [`controls.md`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/controls.md)
* **Design & Interactive Mechanics:**
  * Implemented matching traditional Japanese Shoji lattice sliding window pairs across both floors:
    * Ground floor: Front street window pair and rear garden window pair.
    * Second floor: Two front street-facing window pairs, side ventilation window, and rear window.
  * Designed with cedar perimeter frames, horizontal and vertical *Kumiko* lattice mullions, and translucent rice paper panes (`isWindow = true`).
  * Grouped into interactive `slidingWindowSashes` within `BuildingObject`.
  * Added `Scene::interactNearestWindow(camera.Position)` bound to hotkey **<kbd>G</kbd>**:
    * Smooth delta-time kinematics ($4.5 \times \Delta t$) slide the window sashes along their wooden sill tracks.
    * Proximity auto-opening within $2.2\text{m}$ allows visitors approaching a window to look out over festival lanterns and street trees.

#### 4. 14-Step Authentic Wooden Staircase (*Hakokaidan*) & Natural Bi-Directional Climbing Kinematics
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Architectural Upgrades:**
  * Upgraded stairs from 9 steep blocks to 14 authentic cedar steps with bullnose tread overhangs, dark risers, diagonal side stringer boards, vertical balusters, and turned newel posts.
  * Built traditional Japanese stepped under-stair storage cabinetry (*Hakokaidan* / *Kaidan-dansu*) featuring cedar drawer faces, dark borders, and brass ring pulls.
* **Dual-Direction Kinematic Floor-Tracking:**
  * In `Scene::resolveCollision`, replaced unilateral max-clamping with continuous kinematic floor-tracking:
    $$Y_{\text{eye}}(x) = Y_{\text{stairBase}} + \frac{x - x_{\text{start}}}{x_{\text{end}} - x_{\text{start}}} \cdot (Y_{\text{secondFloor}} - Y_{\text{stairBase}}) + 1.45\text{m}$$
  * Operates across $X \in [-3.70, -0.50]$ and $Z \in [-4.10, -2.85]$: player can seamlessly walk **UP** to the 2nd floor ($Y=5.75\text{m}$) and walk **DOWN** to the ground floor ($Y=1.65\text{m}$) with realistic stair stepping and zero floating!

#### 5. Authentic 2-Storied Machiya Townhouses & Complete Second-Floor Living Chambers
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Complete Interior Floor Plan:**
  * **Ground Floor (*Zashiki* & *Genkan*):** Sunken stone entryway with Agari-kamachi step-up beam, shoe cabinet, raised Tatami floor, low Chabudai table, 4 Zabuton silk cushions, Kyusu teapot, Yunomi cups, Tokonoma alcove with hanging scroll and Ikebana vase, Andon floor lamp, and ceiling pendant chandelier.
  * **Second Floor (*Shinshitsu* / Master Chamber):**
    * Full tatami matting with stairwell protective cedar balustrade guardrail.
    * Exposed structural ceiling timber crossbeams (*Hari*) under the gable roof.
    * Traditional **Futon bed**: plush white cotton mattress (*Shikibuton*), folded crimson/gold festival brocade duvet (*Kakebuton*), and navy silk buckwheat pillow (*Makura*).
    * Three-panel folding screen (*Byoubu*) framed in dark lacquer with shimmering gold-leaf inner panels (`shininess = 64.0`, `specularStrength = 0.65`).
    * Low study desk (*Tsukue*) with ceramic inkstone (*Suzuri*) and rolled manuscript calligraphy scroll.
    * Traditional stepped wooden chest (*Tansu* storage cabinetry).
    * Bedside glowing Andon lamp and ceiling pendant washi lantern.
    * Corner flower pedestal stand with authentic ceramic vase.

#### 6. Detailed Corner *Ikebana* Flower Vases with Multi-Colored Flora
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Floral Artistry (*Kado*):**
  * Created `createIkebanaVase` featuring a glazed ceramic pedestal, bulbous vessel body, slender neck, and flared rim with high specular ceramic gloss (`shininess = 80.0`). Themed in celadon jade (`(0.48, 0.64, 0.58)`), cobalt porcelain (`(0.18, 0.28, 0.52)`), and earthy stoneware (`(0.42, 0.35, 0.28)`).
  * Artfully composed asymmetrical Japanese floral arrangements:
    * **Crimson Camellia (*Tsubaki*):** 5 overlapping crimson petals (`(0.88, 0.16, 0.22)`) with a golden-yellow cluster stamen core (`(0.95, 0.82, 0.20)`).
    * **Pink Peony (*Botan*):** Layered soft rose-pink blossom petals (`(0.96, 0.65, 0.78)`).
    * **Golden Plum Blossom (*Ume*):** Vibrant golden-yellow petals (`(0.98, 0.82, 0.22)`).
    * **Violet Japanese Iris (*Ayame*):** Deep imperial violet petals (`(0.55, 0.25, 0.70)`).
    * Slender arching dark green stems (`(0.24, 0.44, 0.20)`) and sculpted pointed green leaves angled outwards.
  * Installed in house corners, Tokonoma display alcoves, and second-floor corner flower pedestals.

#### 7. Photorealistic Procedural Cherry Blossom Tree (*SakuraTree*) Overhaul
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Botanical Architecture & Particle Enhancements:**
  * **Organic Trunk & Roots:** Replaced primitive single cylinder with 5 spreading buttress root spurs anchoring into the earth, a 3-segment tapering curved organic trunk, 5 major spreading scaffold boughs, and 8 branch forks reaching gracefully outwards.
  * **Volumetric Foliage Canopy:** Expanded from 8 to 22 volumetric cloud clusters organized with a 3-tier botanical color gradient:
    * Inner heartwood / shadow clusters: Deep magenta sakura (`(0.88, 0.52, 0.68)`).
    * Mid-canopy clusters: Classic festival sakura pink (`(0.98, 0.72, 0.82)`).
    * Sunlit exterior tips: Luminous white-pink blooms (`(1.0, 0.88, 0.93)`).
    * Weeping drooping blossom sprays cascading gently beneath the boughs.
  * **Quadrupled Particle System:** Expanded from 8 to 32 cascading wind-blown blossom petals with 3D tumbling pitch, roll, and yaw rotations and gentle horizontal wind drift.
  * **Ground Blossom Patches:** Added 7 fallen petal clusters scattered across the cobblestone pavement around the trunk base.

#### 8. Window Gardens (Cedar Planters, Miniature Bonsai Trees & Hanging Kokedama)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Window Horticultural Features:**
  * **Exterior Window Planter Troughs (`createWindowPlanterBox`):** Cedar wooden planter boxes with corner bracket joinery mounted onto window sills. Filled with rich organic soil and an assortment of 10 vibrantly colored flowers (festival crimson, golden marigold, rose pink, white lily, violet lavender) with green leaves.
  * **Miniature Bonsai Trees (`createBonsaiTree`):** Sculpted Japanese miniature trees planted in glazed rectangular cobalt-blue bonsai trays with 4 corner feet. Features rich mossy soil, an accent weathered viewing stone (*Suiseki*), a gnarled twisting aged bark trunk with 3 bent branch forks, and sculpted dark-pine green foliage pads (`(0.18, 0.42, 0.22)`). Displayed on ground and second-floor window sills.
  * **Hanging *Kokedama* Moss Balls (`createHangingKokedama`):** Traditional Japanese hanging moss balls suspended beneath townhouse roof eaves via slender braided suspension cords. Encased in lush forest moss (`(0.28, 0.52, 0.22)`), with trailing green ivy vines and dangling colorful blossoms.

---

### [2026-10-09] — Visitable Realistic Machiya Houses (Interiors, Bedrooms, Tables, Stairs), Interactive Doors, Wall Collision, Photorealistic Takoyaki Food, and Celestial Sun/Moon/Stars

#### 1. Visitable Traditional Machiya Townhouses (Interior & Exterior Architecture)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Architectural Overhaul:**
  * Replaced solid outer blocks with a realistic architectural shell consisting of perimeter timber walls, plaster interiors, structural posts, and upper gable roof rafters.
  * **Ground Floor Entryway (*Genkan*):**
    * Sunken chiseled stone tile entryway floor ($Y = 0.05\text{m}$).
    * Polished cedar threshold step-up beam (*Agari-kamachi*) connecting stone to living space.
    * Traditional wooden shoe storage bench (*Geta-bako*).
  * **Living & Tea Room (*Zashiki*):**
    * Raised Tatami floor ($Y = 0.18\text{m}$) mapped with authentic woven tatami mats (`texTatami`) and border seams.
    * Traditional low Japanese wooden floor table (*Chabudai*) in rich dark lacquer mahogany with 4 carved legs.
    * 4 silk floor cushions (*Zabuton*) arranged neatly around the table (2 festival crimson silk, 2 deep indigo silk).
    * Authentic Japanese green-tea set on bamboo serving tray: dark clay ceramic teapot (*Kyusu*) with spout, lid, and side handle, plus 2 delicate celadon jade teacups (*Yunomi*).
    * Standing traditional floor lantern (*Andon*) in translucent paper emitting warm ambient yellow-amber night light (`emissiveColor = (1.40, 1.10, 0.50)`).
    * Hanging Japanese decorative wall scroll (*Kakemono*).
  * **Traditional Wooden Staircase (*Kaidan*):**
    * 9 distinct wooden steps with polished cedar treads and risers ascending from Ground Floor $Y = 0.22\text{m}$ to Upper Floor $Y = 4.30\text{m}$.
    * Slanted wooden handrail balustrade along the open side.
  * **Second Floor (*Shinshitsu* / Bedroom):**
    * Upper floor Tatami slab ($Y = 4.30\text{m}$) with cut-out stairwell opening and protective guardrail.
    * 3 Shoji lattice windows looking directly out over the festival avenue and hanging lanterns.
    * Traditional Japanese **Futon bed**: plush white cotton mattress (*Shikibuton*), folded crimson/gold festival duvet quilt (*Kakebuton*), and navy silk buckwheat pillow (*Makura*).
    * Traditional stepped wooden chest (*Tansu* drawers).
    * Bedside glowing paper lantern (*Andon*) casting cozy ambient illumination.
    * Exposed structural ceiling timber beams beneath the classic "V" gable roof.

#### 2. Interactive House Accessibility & Solid Wall Collision System
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h), [`Matsuri Nights — A Japanese Festival Street/Main.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Main.cpp)
* **Interactive Sliding Shoji Doors (<kbd>H</kbd>):**
  * Grouped the entrance sliding door panels and timber frames into `slidingDoorGroup`.
  * Pressing **<kbd>H</kbd>** smoothly slides the nearest house front Shoji door open or closed along its guide track ($1.35\text{m}$ travel).
  * Added proximity auto-opening within $2.2\text{m}$ for natural walkthroughs.
* **Solid Wall Collision Detection (<kbd>B</kbd>):**
  * Implemented `Scene::resolveCollision(oldPos, newPos)` enabled by default:
    * Tested against all 4 Machiya buildings using local-space bounding transformations.
    * Solid exterior walls, roofs, and interior boundaries block camera movement, preventing flying through walls.
    * **Doorway Portal Passage:** When the Shoji door is open, player walks smoothly inside the house; when closed, the door panel blocks entry.
    * **Stair Climbing Kinematics:** Automatically elevates the camera step-by-step as the player walks up the staircase, and steps down smoothly when descending.
    * Ground clamp: prevents dipping below ground level.
  * Hotkey **<kbd>B</kbd>** toggles between Solid Wall Walk Mode and free Noclip Fly Mode.

#### 3. Photorealistic Authentic Takoyaki Food Color, Texture & Details
* **Files Added / Modified:** [`Matsuri Nights — A Japanese Festival Street/src/TextureGenerator.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/TextureGenerator.h), [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **Procedural Texture Asset (`takoyaki_food.bmp`):**
  * Generated high-resolution 24-bit bitmap texture with golden-crisp batter, toasted griddle caramelization marks, rich dark-brown Takoyaki sauce (*Otafuku* glaze), criss-crossing Kewpie mayonnaise zig-zag stripes, emerald dried seaweed flakes (*Aonori*), and shaved bonito flakes (*Katsuobushi*).
* **3D Geometry & Material Upgrades:**
  * Replaced plain wooden ball color with golden fried batter `glm::vec4(0.92, 0.72, 0.38, 1.0)`.
  * Added glossy dark-brown savory sauce dome (`TakoSauce`) with high specular gloss (`shininess = 72.0, specularStrength = 0.85`).
  * Added creamy Kewpie mayonnaise drizzle (`TakoMayo`).
  * Added emerald green *Aonori* seaweed sprinkles (`TakoAonori`).
  * Mapped `texTakoyaki` with high specular shine in `Scene::applyTexturesAndMaterials`.

#### 4. Celestial Sun and Moon at Infinite Position + Twinkling Stars in Night Sky
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/shaders/basic.frag`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/shaders/basic.frag), [`Matsuri Nights — A Japanese Festival Street/shaders/raytrace.frag`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/shaders/raytrace.frag), [`Matsuri Nights — A Japanese Festival Street/src/RayTracer.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/RayTracer.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **True Infinite Astronomical Position:**
  * Sky dome center follows camera eye position, guaranteeing zero translation parallax across the entire environment.
* **The Celestial Sun:**
  * Radiant golden solar disk with atmospheric corona bloom opposite the daytime directional light vector; sets smoothly as twilight transitions into night.
* **The Celestial Moon:**
  * Silvery-white lunar disk featuring procedural lunar maria craters and soft nocturnal ethereal halo; rises at night.
* **Twinkling Night Stars:**
  * Multi-frequency celestial star field with bright twinkling stars and faint background Milky Way dust.
  * Real-time scintillation driven by `totalTime`.
  * Spectral color temperature variation (diamond white, warm golden, cool blue).
  * Atmospheric horizon extinction fading toward the skyline.
* **Cross-Pipeline Synchronization:**
  * Fully integrated across the primary Blinn-Phong rasterizer (`basic.frag`), the real-time GPU ray tracer (`raytrace.frag`), and the multi-threaded CPU software ray tracer (`RayTracer.h`), with reflections appearing on the crystal magic orb and wet stone street!

---

### [2026-10-09] — Implementation of Realistic Shadow Effects (16-Sample PCF Soft Shadow Mapping & Ray-Traced Penumbra)

#### 1. Directional Light PCF Soft Shadow Mapping (Primary Rasterizer Pipeline)
* **Files Added / Modified:**
  * Added [`Matsuri Nights — A Japanese Festival Street/shaders/shadow_depth.vert`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/shaders/shadow_depth.vert)
  * Added [`Matsuri Nights — A Japanese Festival Street/shaders/shadow_depth.frag`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/shaders/shadow_depth.frag)
  * Updated [`Matsuri Nights — A Japanese Festival Street/shaders/basic.vert`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/shaders/basic.vert)
  * Updated [`Matsuri Nights — A Japanese Festival Street/shaders/basic.frag`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/shaders/basic.frag)
  * Updated [`Matsuri Nights — A Japanese Festival Street/src/SceneNode.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/SceneNode.h)
  * Updated [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
  * Updated [`Matsuri Nights — A Japanese Festival Street/Main.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Main.cpp)
* **Shadow Architecture & Engineering:**
  * **High-Resolution Depth FBO:** $2048 \times 2048$ 32-bit floating point depth map with `GL_CLAMP_TO_BORDER` ($1.0$ border) to eliminate out-of-frustum artifacting.
  * **Light-Space Projection Matrix:** Computes dynamic orthographic projection matrix $\mathbf{M}_{\text{light}} = \mathbf{P}_{\text{ortho}} \times \mathbf{V}_{\text{light}}$ centered on the festival street avenue ($48\text{m} \times 48\text{m} \times 90\text{m}$ volume).
  * **Front-Face Culling (`glCullFace(GL_FRONT)`):** Depth pass renders solid back-faces into the shadow map, eliminating self-shadow acne without requiring exaggerated depth offsets.
  * **Fast Depth Traversal (`SceneNode::drawDepth`):** Renders visible non-emissive meshes directly without texture or material uploads, maintaining high frame rates.
  * **Adaptive Slope-Scaled Bias:** $\text{bias} = \max(0.0035 \times (1.0 - \mathbf{N} \cdot \mathbf{L}), 0.0006)$ dynamically offsets depth comparisons according to surface incline.
  * **16-Sample Percentage-Closer Filtering (PCF):** Evaluates a $4 \times 4$ bilinear box kernel across neighbor texels to produce natural, smooth penumbra edges along building silhouettes, Torii beams, stall canopies, and pedestrians.
  * **Celestial Motion Sync:** As Day/Night transitions (<kbd>N</kbd>), cast shadows smoothly pivot, lengthen, and soften across the cobblestone street.
  * **Interactive Shadow Toggle (<kbd>V</kbd>):** Hotkey toggles shadow calculations on and off live with console confirmation.

#### 2. Ray-Traced Soft Penumbra Shadows & Contact Ambient Occlusion
* **Files Modified:**
  * Updated [`Matsuri Nights — A Japanese Festival Street/shaders/raytrace.frag`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/shaders/raytrace.frag)
  * Updated [`Matsuri Nights — A Japanese Festival Street/src/RayTracer.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/RayTracer.h)
* **Features:**
  * Multi-sample jittered shadow rays distributed across celestial angular diameter to produce soft, physically accurate penumbra in both the real-time GPU ray tracer and the multi-threaded CPU snapshot renderer.
  * Contact ambient occlusion factor darkens crevices near ground level, realistically grounding pillars, stalls, and figures.

#### 3. Documentation & Verification
* Updated [`controls.md`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/controls.md) with <kbd>V</kbd> keybinding details.
* Updated [`color_changes.md`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/color_changes.md) with shadow mapping formulas and illumination modulation.
* Verified zero-warning, zero-error MSBuild compilation.

---

### [2026-10-09] — Implementation of Dual Ray Tracing Architecture (Real-Time GPU Mode + CPU Snapshot Export)

#### 1. Real-Time GPU Ray Tracer (Interactive Full-Screen Pass)
* **Files Added / Modified:**
  * Added [`Matsuri Nights — A Japanese Festival Street/shaders/raytrace.vert`](file:///C:/Users/mdabu\OneDrive\Desktop\practice\Graphics\Matsuri-Nights-A-Japanese-Festival-Street\Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/shaders/raytrace.vert)
  * Added [`Matsuri Nights — A Japanese Festival Street/shaders/raytrace.frag`](file:///C:/Users/mdabu\OneDrive\Desktop\practice\Graphics\Matsuri-Nights-A-Japanese-Festival-Street\Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/shaders/raytrace.frag)
  * Updated [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu\OneDrive\Desktop\practice\Graphics\Matsuri-Nights-A-Japanese-Festival-Street\Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
  * Updated [`Matsuri Nights — A Japanese Festival Street/Main.cpp`](file:///C:/Users/mdabu\OneDrive\Desktop\practice\Graphics\Matsuri-Nights-A-Japanese-Festival-Street\Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Main.cpp)
* **Features:**
  * **Interactive 60+ FPS Performance:** Full-screen quad rendering pass driven by per-pixel ray casting on the GPU.
  * **Primary Ray Reconstruction:** Generated directly from camera eye position, gaze direction (`Front`), `Up`, `Right`, FOV, and aspect ratio.
  * **Analytical Geometric Primitives:**
    * **Spheres:** Quadratic discriminant $\|\mathbf{P} - \mathbf{C}\|^2 = r^2$ for the magic orb, chochin lanterns, tree blossom puffs, and character heads.
    * **Boxes / Slabs:** Kay-Kajiya bounding interval method for 4 Machiya houses, roofs, shoji doors, stalls, magic stage, golden byobu screen, and vanishing box.
    * **Cylinders:** Vertical $Y$-cylinder equations for Torii shrine pillars, cedar street poles, and sakura tree trunk.
    * **Planes:** Ground plane with procedural mortar cobblestone calculation and curb borders.
  * **Analytical Hard Shadow Rays:** Shot from hit points to directional sun/moon and dynamic point lights (magic orb, stall lanterns, fireworks flash).
  * **Multi-Bounce Recursive Specular Reflections:** Evaluates up to 3 reflection bounces across reflective surfaces ($k_r > 0$):
    * Crystal Magic Orb ($k_r = 0.85$): Reflects the stage and sky dome.
    * Metallic Vanishing Box ($k_r = 0.60$) & Gold Byobu Screen ($k_r = 0.65$): High metallic sheen.
    * Magic Stage Floor ($k_r = 0.40$): Polished dark lacquer wood floor reflecting the magician, orb, and props.
    * Wet Cobblestone Pavement ($k_r = 0.22$): Reflects the overhead hanging lanterns!
  * **Live Controls Integration:** Hotkey **<kbd>Z</kbd>** seamlessly toggles between rasterized and ray-traced modes while retaining full camera flight navigation (<kbd>W</kbd>/<kbd>A</kbd>/<kbd>S</kbd>/<kbd>D</kbd> + Mouse) and Day/Night transitions (<kbd>N</kbd>).

#### 2. Multi-Threaded CPU Software Ray Tracer (`src/RayTracer.h`)
* **Files Added:**
  * Added [`Matsuri Nights — A Japanese Festival Street/src/RayTracer.h`](file:///C:/Users/mdabu\OneDrive\Desktop\practice\Graphics\Matsuri-Nights-A-Japanese-Festival-Street\Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/RayTracer.h)
* **Features:**
  * Multithreaded execution across all CPU hardware cores (`std::thread::hardware_concurrency()`).
  * Traces primary rays, analytical shadow rays, and 3 recursive reflection bounces.
  * Applies standard display gamma correction ($2.2$).
  * Exports 24-bit uncompressed bitmap image snapshot to **`raytraced_snapshot.bmp`** on disk via `TextureGenerator::writeBMP24`.
  * Triggered via hotkey **<kbd>F9</kbd>**; logs resolution, core count, elapsed rendering time, and output path to console.

#### 3. Documentation & Verification
* Updated [`controls.md`](file:///C:/Users/mdabu\OneDrive\Desktop\practice\Graphics\Matsuri-Nights-A-Japanese-Festival-Street\controls.md) with <kbd>Z</kbd> and <kbd>F9</kbd> reference manual entries.
* Updated [`color_changes.md`](file:///C:/Users/mdabu\OneDrive\Desktop\practice\Graphics\Matsuri-Nights-A-Japanese-Festival-Street\color_changes.md) with ray tracing optics, analytical formulas, and reflectivity parameters.
* Verified zero-warning, zero-error MSBuild compilation.

---

### [2026-10-09] — Completion of Course Phase 2 (Lighting & Illumination) & Phase 3 (Texturing & Material Pipeline)

#### 1. Phase 2: Blinn-Phong Illumination & Multi-Light Engine
* **Files Added / Modified:**
  * Added [`Matsuri Nights — A Japanese Festival Street/src/Light.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Light.h)
  * Updated [`Matsuri Nights — A Japanese Festival Street/shaders/basic.frag`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/shaders/basic.frag)
  * Updated [`Matsuri Nights — A Japanese Festival Street/src/SceneNode.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/SceneNode.h)
  * Updated [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
  * Updated [`Matsuri Nights — A Japanese Festival Street/Main.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Main.cpp)
* **Mathematical Lighting Formulation:**
  * Implemented the **Blinn-Phong reflection model** across all non-emissive scene geometry:
    $$\mathbf{H} = \frac{\mathbf{L} + \mathbf{V}}{\|\mathbf{L} + \mathbf{V}\|}$$
    $$\mathbf{I} = \mathbf{I}_{\text{ambient}} + \mathbf{I}_{\text{diffuse}} \cdot \max(\mathbf{N} \cdot \mathbf{L}, 0) + \mathbf{I}_{\text{specular}} \cdot k_s \cdot (\max(\mathbf{N} \cdot \mathbf{H}, 0))^{\alpha}$$
  * Material properties integrated into `SceneNode`: `shininess` ($\alpha \in [8.0, 128.0]$) and `specularStrength` ($k_s \in [0.1, 0.85]$).
* **Multiple Dynamic Light Sources:**
  1. **Directional Light (Sun / Moon):** Sweeps through the celestial dome; golden daytime sun transitioning to cool silvery-blue moonlight.
  2. **6 Dynamic Point Lights:**
     * **Point Light 0:** Tracks the arcane Magic Orb ($X, Y, Z$) as it performs helical orbits around the magician's hand.
     * **Point Light 1:** Takoyaki stall lantern (warm amber light illuminating counter & vendors).
     * **Point Light 2:** Kakigori stall lantern (cool magenta-ice illumination).
     * **Point Lights 3 & 4:** Overhead rope lanterns swinging dynamically with wind pendulum kinematics.
     * **Point Light 5:** Sky fireworks flash tracking exploding shell particles with quadratic distance falloff ($1.0 / (1.0 + 0.04d + 0.0075d^2)$).
  3. **Stage Spotlight:** Conical light source mounted on the stage spotlight housing that swivels and tilts to track the magician, featuring inner cutoff ($\cos 15^\circ$) and outer cutoff ($\cos 20^\circ$) smooth penumbra attenuation.
* **Interactive Shading Mode Switcher (<kbd>P</kbd>):**
  * Allows live cycling through three shading configurations:
    * `Mode 0`: Full Blinn-Phong Shading (Ambient + Diffuse + Specular).
    * `Mode 1`: Diffuse Only (Ambient + Lambertian Diffuse, specular disabled).
    * `Mode 2`: Flat Ambient Only (Uniform ambient lighting baseline).

#### 2. Phase 3: Texture Mapping & Procedural Image Generation Pipeline
* **Files Added / Modified:**
  * Added [`Matsuri Nights — A Japanese Festival Street/src/Texture.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Texture.h)
  * Added [`Matsuri Nights — A Japanese Festival Street/src/TextureGenerator.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/TextureGenerator.h)
  * Integrated single-header image loader `stb_image.h`.
* **Procedural Texture Asset Generation:**
  * Standalone binary BMP-24 file generator creates valid bitmap assets on disk in `assets/textures/`:
    * `wood_timber.bmp` ($512 \times 512$): Japanese cedar wood with realistic ring grain and cellular fibers.
    * `roof_tiles.bmp` ($512 \times 512$): Traditional scalloped ceramic Japanese roof tiles (*kawara*) with drop shadows.
    * `stone_pavement.bmp` ($512 \times 512$): Chiseled cobblestone street pavers with mortared joints.
    * `tatami_cloth.bmp` ($256 \times 256$): Woven textile threads with alternating warp and weft fiber pattern.
    * `gold_leaf.bmp` ($256 \times 256$): Hammered gold foil with micro-faceted metallic sheen for the folding screen and vanishing box.
    * `sakura_bark.bmp` ($512 \times 512$): Furrowed cherry tree bark with horizontal lenticels and organic fissures.
    * `lantern_paper.bmp` ($256 \times 256$): Translucent red washi paper with horizontal bamboo ribbing rings.
* **OpenGL Texture Engine:**
  * Implemented `Texture` class managing OpenGL `GL_TEXTURE_2D` IDs, automatic mipmap generation (`glGenerateMipmap`), linear filtering (`GL_LINEAR_MIPMAP_LINEAR`), and `GL_REPEAT` wrapping.
  * Extended `SceneNode` with `Texture* texture;` and `glm::vec2 textureTiling;`.
  * Modulated texture colors with base object colors in `shaders/basic.frag`:
    $$\mathbf{C}_{\text{diffuse}} = \mathbf{C}_{\text{base}} \times \mathbf{C}_{\text{tex}}(\mathbf{uv} \cdot \mathbf{tiling})$$
* **Interactive Texture Toggle (<kbd>X</kbd>):**
  * Single hotkey toggles textures ON and OFF globally for side-by-side visual comparison.

#### 3. Compilation & Architecture Hardening
* **Header Scope Guard:** Undefined `STB_IMAGE_IMPLEMENTATION` in `Main.cpp` immediately after inclusion to prevent duplicate function body generation when included indirectly through `Texture.h`.
* **Zero-Warning Clean Build:** Verified with MSBuild on MSVC C++20 x64.
* **Documentation Synchronization:** Fully updated [`controls.md`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/controls.md) and [`color_changes.md`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/color_changes.md) to document lighting, shading modes, materials, and textures.

---

### [2026-09-25] — Phase 1.5 Polish: Anatomical Rigs, Stall Lighting, Sagging Catenary Ropes & Input Polish

#### 1. Walking Crowd: Added Hands & Realistic Locomotion
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Additions:**
  * Added `std::shared_ptr<SceneNode> armL;` and `armR;` to struct `WalkingPerson`.
  * Added shoulder joint pivots (`Walk_ArmPivotL`, `Walk_ArmPivotR`) positioned at $Y = 1.55\text{m}$.
  * Added Yukata sleeves (`meshes.cylinder`, scale `(0.14, 0.44, 0.14)`) inheriting the pedestrian's robe color.
  * Added hands (`meshes.sphere`, scale `(0.12, 0.14, 0.12)`) in warm skin tone.
* **Kinematics & Walking Motion:**
  * Implemented natural human arm-leg opposition in `CrowdGroup::update()`: when the left leg swings forward (+X rotation), the left arm swings backward (-X rotation) and the right arm swings forward ($\pm 22^\circ$).
  * Added outward shoulder flare ($\pm 5^\circ$ roll) and vertical step bobbing ($0.04\text{m}$).
  * Ground clearance verified: all footfalls sit at $Y \ge 0.0\text{m}$.

#### 2. Legs & Footwear for Magician & Audience Figures
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Magician Character Rig:**
  * Added trouser legs (`legL`, `legR`) in dark formal fabric spanning $Y \in [0.05\text{m}, 0.85\text{m}]$ connecting torso to stage.
  * Added formal polished black leather boots (`bootL`, `bootR`) resting flush on the stage surface ($Y \in [0.0\text{m}, 0.10\text{m}]$). Lowest face sits at exact stage surface $Y = 0.0\text{m}$.
  * Added flowing cape tail (`capeTail`) draped behind the magician.
  * Added left arm and left hand resting gracefully at the hip to complement the raised wand/orb right arm.
* **Audience Spectators Rig:**
  * Added lower legs (`legL`, `legR`) extending downwards from the bench cushion ($Y \in [0.06\text{m}, 0.50\text{m}]$).
  * Added traditional Japanese wooden *geta* sandals (`getaL`, `getaR`) resting squarely on the ground ($Y \in [0.0\text{m}, 0.07\text{m}]$). Bottom face is exactly at $Y = 0.0\text{m}$ (no ground penetration).
  * Added red festival sandal straps (*Hanao*) (`strapL`, `strapR`).
  * Added hands (`handL`, `handR`) resting on the knees.

#### 3. Stall Night Lighting (Takoyaki & Kakigori Stalls)
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)
* **Features:**
  * Added hanging festival lanterns (*ko-chochin*) to both stalls suspended from the front awning eaves:
    * **Takoyaki Stall:** Two lanterns with red paper body and emissive night glow (`emissiveColor = (1.20, 0.50, 0.18)`).
    * **Kakigori Stall:** Two lanterns with ice-cyan paper body and bright night glow (`emissiveColor = (0.45, 0.95, 1.25)`).
  * Integrated wind sway animation in both `TakoyakiStall::update()` and `KakigoriStall::update()`.
  * Added `stallLanterns` member vector to both classes.

#### 4. Scaling (`+/-`) Transformation Fix
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/Main.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Main.cpp), [`controls.md`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/controls.md)
* **Fix:**
  * Resolved key event mismatch where terminal indicated `[+/-]` but code only checked brackets `[` and `]`.
  * Added support for `GLFW_KEY_EQUAL` (`=` / `+`), `GLFW_KEY_KP_ADD` (numpad `+`), and `GLFW_KEY_RIGHT_BRACKET` (`]`) for scaling up (+10%).
  * Added support for `GLFW_KEY_MINUS` (`-`), `GLFW_KEY_KP_SUBTRACT` (numpad `-`), and `GLFW_KEY_LEFT_BRACKET` (`[`) for scaling down (-10%).
  * Updated `controls.md` reference table and code guide.

#### 5. Street Lantern Poles & Realistic Catenary Sagging Ropes
* **Files Modified:** [`Matsuri Nights — A Japanese Festival Street/src/Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h), [`Matsuri Nights — A Japanese Festival Street/src/Scene.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Scene.h)
* **New Class `StreetLanternSpan`:**
  * **Cedar Poles:** Placed on both sides of the street along the curb line at $X = \pm 3.8\text{m}$. Each pole features a stone base pedestal ($Y \in [0.0\text{m}, 0.40\text{m}]$), tall wooden shaft ($H = 6.5\text{m}$), and a top crossarm peg at $Y = 6.20\text{m}$.
  * **Realistic Catenary Sagging Rope:** 10 segmented cylinders connecting left and right poles according to the catenary curve:
    $$Y(x) = 6.20 - 0.65 \cdot \left(1.0 - \left(\frac{x}{3.8}\right)^2\right)$$
  * **Aligned Lanterns:** Hanging lanterns are anchored precisely onto the sagging curve at $X = \pm 1.90\text{m}$ ($Y = 5.71\text{m}$) and sway with wind pendulum kinematics (`LanternObject::update`).
  * **Collision-Free Coordinates:** Spans instantiated at $Z \in \{ 22.0\text{m}, 11.0\text{m}, 0.0\text{m}, -9.0\text{m}, -24.5\text{m} \}$, guaranteeing complete separation ($>2.2\text{m}$ clearance) from walking promenade ($X \in [-1.6, 1.4]$), stalls ($Z = 6.0$), magic stage ($Z \in [-21.8, -16.2]$), audience ($Z \in [-14.5, -12.7]$), trees, and buildings.
  * Preserved full inspectable object support for item `#1` and `#2` in `Scene::setupInspectables()`.

#### 6. Executable Launch Script Robustness
* **Files Modified:** [`run.bat`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/run.bat)
* **Update:** Updated launcher to dynamically resolve both standard hyphen and Unicode em-dash binary names.

#### 7. Repository Management & Clean Build Gitignore
* **Files Added:** [`.gitignore`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/.gitignore)
* **Configuration:** Added comprehensive Visual Studio and C++ build exclusions (`.vs/`, `x64/`, `*.pdb`, `*.ilk`, `*.obj`, `*.user`, intermediate folders) ensuring clean repository commits without local machine build artifacts.

---

### [2026-09-24] — Phase 2: Bug Fixes, Collisions, Roof Orientation & Architecture

#### 1. Shopkeeper / Vendor Visibility
* **Fix:** Vendors inside Takoyaki and Kakigori stalls were too short to be seen over the counter.
* **Solution:** Raised vendors on wooden platforms behind the counter and enlarged their upper torso and Happi coat so they stand tall and visibly stir/serve over the counter.

#### 2. Machiya House Roof Inversion
* **Fix:** Roofs were inverted (shaped like a "V" valley instead of a gable peak).
* **Solution:** Flipped roof slope angles so rafters ascend to a central ridge beam in the traditional upside-down "V" gable configuration.

#### 3. Magic Show Audience Placement & Ground Clearance
* **Fix:** Audience members were clustered toward the road side and some parts clipped the terrain.
* **Solution:** Relocated audience benches directly in front of the magic stage ($Z \in [-14.5, -12.7]$) facing the magician (-Z direction). Clamped all components to $Y \ge 0.0\text{m}$.

#### 4. Machiya Entrance Doors & Z-Fighting Flicker Fix
* **Fix:** Houses lacked front entrance doors, and subsequent doors suffered Z-fighting flickering on their right half.
* **Solution:** Added traditional sliding lattice doors (*shoji/fusuma*) facing the central road. Offset door position outward by $+0.01\text{m}$ from the building wall surface to completely eliminate coplanar depth fighting.

#### 5. Crowd Walking Corridor Collision Avoidance
* **Fix:** Pedestrians were walking through trees, magic stage, stalls, and buildings.
* **Solution:** Constrained all crowd walker paths strictly to the central open promenade $X \in [-1.6\text{m}, +1.4\text{m}]$, with turn-around boundary loops at $Z = -30\text{m}$ and $Z = +36\text{m}$.

---

### [2026-09-23] — Phase 1: Core Festival Street Implementation
* Ground and stone pavement.
* 4 Machiya traditional Japanese townhouse buildings.
* Grand Torii Gate entrance anchor.
* Hierarchical Sakura Blossom Tree with wind-drift falling petals.
* Takoyaki Stall with flipping and hopping takoyaki balls.
* Kakigori Stall with spinning shaver wheel and fluttering nobori banner.
* Magic Show Stage with disappearing box trick and articulated magician.
* Spotlight rig with panning/tilting head.
* Sky dome and day/night transition engine.
* Fireworks particle system with manual burst and automated night shows.
* Free-fly camera with FPS mouse look, field-of-view zooming, and 3 preset views.
* Interactive 10-object live transformation inspection system.
