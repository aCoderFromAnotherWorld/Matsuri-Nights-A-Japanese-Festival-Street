# Matsuri Nights — A Japanese Festival Street
### CSE4102: Computer Graphics and Image Processing Laboratory
**Student Name:** MD. Abu Hasanat Soykot  
**Roll:** 2107100 | **Group:** B2  
**Institution:** Khulna University of Engineering & Technology (KUET)  
**Department:** Computer Science and Engineering  

---

## Overview
**Matsuri Nights** is an advanced, real-time interactive 3D simulation of a traditional Japanese summer evening festival (*matsuri*). Built from scratch using modern C++17, OpenGL 3.3+ Core Profile, GLFW 3.5.1, GLAD, and GLM, the project demonstrates core computer graphics fundamentals alongside cutting-edge rendering techniques:

* **Hierarchical Scene Graphs & Kinematics:** Full parent-child transformation tree with multi-joint articulated rigs, projectile parabolic physics, and catenary rope curves.
* **Blinn-Phong Illumination Pipeline:** Directional sun/moon, 14 dynamic point lights, stage spotlight with smooth angular falloff, and indoor skylight bounce.
* **Soft Shadow Mapping:** 16-sample PCF filter with normal-scaled adaptive depth bias rendered via an offscreen $2048 \times 2048$ FBO.
* **Procedural Texture Synthesis:** 7 procedurally generated 24-bit uncompressed texture maps (timber, roof tiles, stone pavement, tatami cloth, gold leaf, sakura bark, washi paper).
* **Dual Ray Tracing Pipeline:** Real-time GPU Whitted ray tracer (60+ FPS, <kbd>Z</kbd>) and multi-threaded CPU software ray tracer (<kbd>F9</kbd>) with recursive specular reflections and analytical hard shadows.
* **Interactive Doorways & Collision Engine:** Sliding Shoji lattice doors (<kbd>H</kbd>) and windows (<kbd>G</kbd>) with dynamic AABB collision masking (<kbd>B</kbd>).
* **In-Window Minimalist HUD:** Sleek, semi-transparent 2D orthographic overlay (<kbd>F1</kbd>) powered by an embedded $256 \times 256$ Consolas Bold texture atlas and context-sensitive interaction engine.
* **Automated Verification Harness:** 312 / 312 unit and integration tests passing (`--test`).
* **Academic LaTeX Report:** Concise 40-page (34-page core) academic laboratory report (`report/main.pdf`) with 6 vector diagrams, 20 figure captures, and Gemini 3.8 Flash acknowledgments.

---

## Documentation Index
* **[`explanation.md`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/explanation.md):** Exhaustive step-by-step codebase engineering and theoretical explanation guide (1,677 lines) answering What, Why, and How across every subsystem, line/block, mathematical derivation, matrix proof, and algorithm from zero to final project.
* **[`report/main.pdf`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/report/main.pdf):** Authoritative 40-page (34-page core) academic laboratory report formatted in LaTeX (A4, Times New Roman, MiKTeX `pdflatex`) submitted for CSE4102.
* **[`Details.md`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Details.md):** Exhaustive technical specification covering every system, math formula, shader layout, lighting source, and asset hierarchy.
* **[`Plan.md`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Plan.md):** Master milestone ledger with all Phase 1, Phase 2, and Phase 3 goals verified and completed.
* **[`agy.md`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/agy.md):** Complete chronological engineering and development log tracking every feature, bug fix, and optimization.
* **[`controls.md`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/controls.md):** Comprehensive interactive controls reference manual.
* **[`color_changes.md`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/color_changes.md):** Color customization palette, Blinn-Phong material properties, and shader customization guide.

---

## Core Systems & Architecture

### 1. Hierarchical Scene Graph & Transform Tree
* Centralized [`Transform`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Transform.h) matrix composition ($M = T \cdot R_z \cdot R_y \cdot R_x \cdot S$).
* Recursive [`SceneNode`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/SceneNode.h) world matrix propagation:
  $$\mathbf{M}_{\text{world}} = \mathbf{M}_{\text{parent}} \times \mathbf{M}_{\text{local}}$$
* Full academic demonstration of motion relative to moving reference frames (Lantern swinging body relative to catenary rope pivot; Magic Orb orbiting articulated magician hand bone).

### 2. Multi-Source Lighting & PCF Shadow Pipeline
* **Directional Sun / Moon:** Dynamic celestial light sweeping across the sky during Day/Night transitions (<kbd>N</kbd>).
* **14 Dynamic Point Lights:**
  * Light 0: Magic Orb arcane cyan glow `(0.2, 0.6, 1.0)`.
  * Light 1: Takoyaki Stall warm amber lantern `(1.0, 0.6, 0.2)`.
  * Light 2: Kakigori Stall rose-magenta lantern `(1.0, 0.4, 0.6)`.
  * Lights 3 & 4: Overhead catenary swinging lanterns `(1.0, 0.45, 0.15)`.
  * Light 5: High-altitude exploding fireworks flash.
  * Lights 6–9: Machiya L1 & R1 living rooms and upper bedrooms.
  * Lights 10–11: Machiya L2 & R2 cozy domestic living rooms.
  * Lights 12–13: Torii Shrine Gate Left & Right radiant amber illuminations `(1.50, 1.05, 0.50)`.
* **Stage Spotlight:** Smooth inner ($\cos 15^\circ$) and outer ($\cos 20^\circ$) angular penumbra tracking the magician.
* **PCF Soft Shadows:** 16-sample kernel with normal-scaled depth bias eliminating shadow acne across building walls, awnings, and cobblestones.

### 3. Dual Ray Tracing Architecture
1. **Real-Time GPU Ray Tracer (<kbd>Z</kbd>):** Executed in fragment shader (`shaders/raytrace.frag`) on a full-screen quad at 60+ FPS with analytical ray-sphere, ray-box slab, ray-cylinder, and ray-plane intersections, analytical shadow rays, and recursive mirror reflections.
2. **Multi-Threaded CPU Software Ray Tracer (<kbd>F9</kbd>):** High-resolution Whitted offline renderer parallelized across all CPU cores, exporting to `raytraced_snapshot.bmp`.

### 4. In-Window Minimalist HUD & Context Interaction
* Separate 2D orthographic pass in pixel coordinates (`glm::ortho(0, fbWidth, fbHeight, 0, -1, 1)`).
* Strict OpenGL state isolation: saves and restores depth test, face culling, blending, active shader, and VAO.
* High-DPI top-right anchor with `glfwGetFramebufferSize()`.
* Embedded $256 \times 256$ Consolas Bold texture atlas (`src/ui/FontAtlasData.h`) with smoothstep drop-shadow fragment shader.
* Context-sensitive hints for nearby doors, windows, lighting toggles, and magic tricks.
* Screenshot HUD bypass: <kbd>P</kbd> captures pristine 3D scene; <kbd>Shift+P</kbd> captures with HUD.

---

## Interactive Controls Cheat Sheet

| Key / Input | Action / Purpose |
|:---:|---|
| **<kbd>W</kbd> / <kbd>A</kbd> / <kbd>S</kbd> / <kbd>D</kbd>** | Fly camera Forward / Left / Backward / Right |
| **<kbd>E</kbd> / <kbd>Q</kbd>** | Fly camera Up / Down |
| **Mouse Move** | Look around (FPS Pitch and Yaw) |
| **Mouse Scroll** | Zoom in / Zoom out (Adjusts FOV from $1^\circ$ to $60^\circ$) |
| **<kbd>C</kbd>** | Toggle mouse cursor capture |
| **<kbd>F1</kbd>** | **Toggle Minimalist HUD Overlay** (Top-right corner) |
| **<kbd>0</kbd> / <kbd>KP_0</kbd>** | **Toggle Festival Lights** (Lantern & stall point lights ON / Dimmed) |
| **<kbd>N</kbd>** | **Toggle Smooth Day $\longleftrightarrow$ Festival Night** |
| **<kbd>Space</kbd>** | **Pause / Resume** all animations and kinematics |
| **<kbd>P</kbd>** | **Cycle Shading Mode** (Blinn-Phong $\to$ Diffuse Only $\to$ Ambient Only) |
| **<kbd>V</kbd>** | **Toggle PCF Soft Shadows** ON / OFF |
| **<kbd>X</kbd>** | **Toggle Diffuse Textures** ON / OFF |
| **<kbd>Z</kbd>** | **Toggle Real-Time GPU Ray Tracer** ON / OFF |
| **<kbd>F9</kbd>** | Render & Export **CPU Multi-Threaded Ray-Traced Snapshot** |
| **<kbd>H</kbd>** | **Toggle Nearest House Shoji Door** (Smooth slide open / close within 5m) |
| **<kbd>G</kbd>** | **Toggle Nearest House Shoji Windows** (Smooth slide open / close within 14m) |
| **<kbd>B</kbd>** | **Toggle Wall Collision Mode** (Walk Mode $\longleftrightarrow$ Noclip Fly Mode) |
| **<kbd>M</kbd>** | **Replay Magic Show Trick** (Vanishing box & orbiting orb) |
| **<kbd>F</kbd>** | Manually launch a Firework rocket |
| **<kbd>1</kbd> / <kbd>2</kbd> / <kbd>3</kbd>** | Camera Viewpoint Presets (1: Entrance, 2: Magic Stage, 3: Torii & Sky) |
| **<kbd>R</kbd>** | Reset camera to street entrance origin |
| **<kbd>T</kbd>** | **Cycle Target Object** across all 15 inspectables (<kbd>Shift+T</kbd> for Auto) |
| **<kbd>I</kbd> / <kbd>K</kbd>** | Translate selected object along $\pm Z$ (Forward / Backward) |
| **<kbd>J</kbd> / <kbd>L</kbd>** | Translate selected object along $\pm X$ (Left / Right) |
| **<kbd>U</kbd> / <kbd>O</kbd>** | Translate selected object along $\pm Y$ (Up / Down) |
| **<kbd>Num 8</kbd> / <kbd>Num 2</kbd>** | Rotate selected object Pitch (around X axis) |
| **<kbd>Num 4</kbd> / <kbd>Num 6</kbd>** | Rotate selected object Yaw (around Y axis) |
| **<kbd>+</kbd> / <kbd>-</kbd>** | Scale selected object up ($1.1\times$) / down ($0.9\times$) |
| **<kbd>P</kbd> / <kbd>Shift+P</kbd>** | Capture Viewport Screenshot (Clean / With HUD) |
| **<kbd>Esc</kbd>** | Exit application cleanly |

---

## The 15 Controllable Inspectable Objects
Press <kbd>T</kbd> to cycle target selection live in the viewport:
1. `Lantern Body` (Pendulum kinematics child)
2. `Lantern Rope Pivot` (Parent catenary anchor)
3. `Magic Orb` (Helical 3D orbit around magician's hand bone)
4. `Magician Figure` (Root character rig)
5. `Vanishing Box` (Continuous scale-to-zero demo)
6. `Stage Spotlight Housing` (Tracking rig)
7. `Takoyaki Stall` (Full unit with 6 flipping/hopping balls)
8. `Kakigori Stall` (Full unit with ice shaver, spinning flywheel, and 6 animated bowls)
9. `Torii Gate` (Grand decorated shrine entrance with hanging lanterns, pillar lights, and stone lanterns)
10. `Sakura Blossom Tree` (Hierarchical foliage and drifting petals)
11. `Crowd Walker #1` (Street walking pedestrian rig)
12. `Machiya Building L1` (Left Front Townhouse)
13. `Machiya Building L2` (Left Rear Townhouse)
14. `Machiya Building R1` (Right Front Townhouse)
15. `Machiya Building R2` (Right Rear Townhouse)

---

## Build & Run Instructions

### Prerequisites
* Windows 10 / 11 64-bit
* Visual Studio 2022 or 2026 with **Desktop Development with C++** (MSVC toolset v143 or v145, C++17+)

### Building from Visual Studio
1. Open [`Matsuri Nights — A Japanese Festival Street/Matsuri Nights - A Japanese Festival Street.slnx`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Matsuri%20Nights%20-%20A%20Japanese%20Festival%20Street.slnx).
2. Set configuration to **Debug \| x64** or **Release \| x64**.
3. Press **Ctrl + Shift + B** to build solution.
4. Press **F5** (or **Ctrl + F5**) to launch.

### Running Automated Test Suite
From the repository root, run the test executable with `--test`:
```powershell
& "Matsuri Nights - A Japanese Festival Street\x64\Debug\Matsuri Nights - A Japanese Festival Street.exe" --test
```
**Test Result:** `311 / 311 TESTS PASSED (100% SUCCESS)`.