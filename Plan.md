# Matsuri Nights — Japanese Festival Street Scene
### OpenGL / C++ Computer Graphics Course Project — Master Plan & Implementation Specification

**Course:** CSE4102 — Computer Graphics and Image Processing Laboratory  
**Project Title:** Matsuri Nights — A Japanese Festival Street  
**Status:** **100% Complete — All Milestones & Advanced Extensions Implemented and Verified**

---

## 1. Project Summary & Core Achievements

**Matsuri Nights — A Japanese Festival Street** is a real-time interactive 3D graphics application built in modern C++17 and OpenGL Core Profile. The application faithfully recreates a traditional Japanese summer street festival (*matsuri*): a central stone-paved street flanked by four two-story wooden townhouses (*machiya*), strung with hanging paper lanterns, anchored by a grand vermilion *torii* gate at the terminus, featuring an animated cherry blossom (*sakura*) tree, two functional festival food stalls (*takoyaki* and *kakigori*), a magic show pavilion with an audience and two stage tricks, walking crowd pedestrians, and a night sky illuminated by fireworks.

The project demonstrates:
1. **Hierarchical 3D Model Transformations:** Complex parent-child scene graph structures with animated multi-joint rigs, swinging lanterns, orbiting orbs, and sliding doors.
2. **Transformations Relative to Another Object's Reference Frame:** Pendulum swinging lanterns relative to catenary rope pivots; orbiting magic orb relative to the magician's articulated hand bone; stage spotlight aiming relative to tracking housing; sliding Shoji doors and windows relative to Machiya townhouse frames.
3. **Advanced Dynamic Illumination & Shading:** 16 dynamic lights (1 directional sun/moon light, 14 dynamic point lights, and 1 cone-attenuated tracking spotlight), continuous day-to-night state transitions, and real-time Blinn-Phong, Diffuse-Only, and Ambient-Only shading models.
4. **Diffuse & Emissive Texturing:** Multi-sampled diffuse texture mapping with procedural fallbacks, material shininess parameters, and nighttime emissive glows on lanterns and Shoji rice-paper screens.
5. **Advanced Graphics Pipeline Extensions:**
   - **Real-Time GPU Whitted Ray Tracing** (<kbd>Z</kbd>) via a dedicated full-screen fragment shader and analytical geometry intersections.
   - **High-Resolution CPU Ray-Traced Snapshot Generator** (<kbd>F9</kbd>) exporting to uncompressed BMP.
   - **16-Sample Percentage-Closer Filtered (PCF) Soft Shadow Mapping** (<kbd>V</kbd>) via a $2048 \times 2048$ depth framebuffer.
   - **Continuous Collision Detection (CCD) & Doorway Portals** (<kbd>B</kbd>) with AABB interior/exterior wall physics and interactive portal pass-through.
   - **Interactive Sliding Shoji Doors** (<kbd>H</kbd>) and **Sliding Windows** (<kbd>G</kbd>) with smooth ease-in-out translation.
   - **In-Window Minimalist Heads-Up Display (HUD)** (<kbd>F1</kbd>) using an embedded $256 \times 256$ Consolas Bold texture atlas and context-sensitive action telemetry.
   - **Interactive Selection & 6-DOF Debug Transformation Engine** (<kbd>T</kbd> / <kbd>Shift+T</kbd>) providing live manual translation, rotation, and scaling across 15 inspectable objects.
   - **Automated Verification Harness:** 311 automated unit and integration tests passing with 100% success.

---

## 2. Tech Stack & Environment

- **Language:** C++17
- **Graphics API:** Modern OpenGL (Core Profile 3.3+)
- **Windowing & Input:** GLFW 3.5.1 (native window management, high-DPI framebuffer queries, input callbacks)
- **OpenGL Loader:** GLAD (dynamically loaded OpenGL function pointers)
- **Math Library:** GLM (OpenGL Mathematics, header-only) — vectors, matrices, quaternions, perspective/orthographic projections
- **Texture Engine:** Custom BMP file loaders and procedural texture synthesizers (`src/TextureGenerator.h`, `src/Texture.h`)
- **IDE / Build System:** Visual Studio 2022 (MSVC Toolset v143, x64 Release / Debug), MSBuild
- **Fonts & UI:** Embedded single-header Consolas Bold font atlas (`src/ui/FontAtlasData.h`) with dedicated HUD GLSL shaders; zero external GUI bloat (no ImGui or FreeType dependencies).

### Complete Project File Structure

```
Matsuri-Nights-A-Japanese-Festival-Street/
├── Matsuri Nights.sln
├── Matsuri Nights — A Japanese Festival Street/
│   ├── Matsuri Nights — A Japanese Festival Street.vcxproj
│   ├── Main.cpp                                  # Entry point, GLFW setup, render loop, test suite
│   ├── src/
│   │   ├── Camera.h                              # FPS fly/walk camera with yaw/pitch and presets
│   │   ├── Transform.h                           # 3D TRS representation (position, rotation, scale)
│   │   ├── SceneNode.h                           # Hierarchical scene graph node with world matrix chaining
│   │   ├── Mesh.h                                # VAO/VBO/EBO wrapper for indexed vertex rendering
│   │   ├── Primitives.h                          # Cube, Cylinder, Cone, Sphere, Plane mesh generators
│   │   ├── Light.h                               # DirectionalLight, PointLight, SpotLight structs
│   │   ├── Shader.h                              # GLSL compile, link, and uniform dispatcher
│   │   ├── Texture.h                             # Texture2D wrapper and BMP image loader
│   │   ├── TextureGenerator.h                    # Procedural bitmap fallback texture synthesizer
│   │   ├── Curves.h                              # Parametric curves and spline evaluation helpers
│   │   ├── RayTracer.h                           # CPU Whitted ray tracing engine & BMP exporter
│   │   ├── Objects.h                             # 17 composite object classes & animation state machines
│   │   ├── Scene.h                               # Scene coordinator: lighting, shadows, ray tracing, physics
│   │   ├── ui/
│   │   │   ├── FontAtlasData.h                   # Embedded 256x256 Consolas Bold bitmap texture
│   │   │   ├── Hud.h / Hud.cpp                   # Minimal 2D orthographic heads-up display overlay
│   │   │   ├── Interactable.h                    # Interaction interface, proximity data, action hints
│   │   │   └── InteractionManager.h / .cpp       # Context selection, view cone filter, key dispatch
│   │   └── shaders/
│   │       ├── hud.vert / hud.frag               # UI text and translucent card shaders
│   │       ├── basic.vert / basic.frag           # Blinn-Phong, shadows, texturing, emissive shader
│   │       ├── shadow_depth.vert / .frag         # Directional shadow map depth-pass shaders
│   │       └── raytrace.vert / raytrace.frag     # GPU Whitted ray tracer full-screen quad shaders
│   └── assets/
│       └── textures/                             # Diffuse & emissive bitmap texture assets
└── Plan.md                                       # Master Plan & Implementation Specification
```

---

## 3. Core Architecture & Design

### 3.1 Hierarchical Scene Graph (`Transform` & `SceneNode`)
Every visual entity in the world inherits from or is composed of `SceneNode` instances:
- **`Transform` Struct:** Maintains `position` (vec3), `rotation` (Euler angles in degrees), and `scale` (vec3). Generates local affine transformations via `mat4 localMatrix = translate * rotateZ * rotateY * rotateX * scale`.
- **`SceneNode` Chaining:** Each node contains a weak parent pointer and a list of shared child nodes. The world transformation matrix is defined recursively:
  $$\mathbf{M}_{\text{world}} = \mathbf{M}_{\text{parent\_world}} \times \mathbf{M}_{\text{local}}$$
- **Relative Reference Frame Execution:**
  - **Hanging Lanterns:** A `RopeAnchor` node oscillates its local roll/pitch via $\sin(\omega t + \phi)$. The `LanternBody` child node and its internal point light automatically inherit this swinging pivot.
  - **Floating Magic Orb:** The orb is parented directly to the magician's articulated wrist/hand bone. As the magician's arm raises, the orb's helical trajectory $(\cos t, \sin 2t, \sin t)$ computes directly within the hand's local coordinate frame.
  - **Shoji Doors & Windows:** Attached as child nodes of the building's exterior frame. Toggling doors or windows applies an ease-in-out local translation along the wall axis ($Z$-axis for doors, $X$-axis for upper windows), moving strictly relative to the townhouse orientation.

### 3.2 Reusable Geometric Primitives (`src/Primitives.h`)
Five parametric geometric generators provide optimized, indexed vertex meshes (`Mesh.h`) with normals, UV coordinates, and tangent vectors:
1. **Cube:** 24 vertices, 36 indices, with per-face normal and UV assignments.
2. **Cylinder:** Parametric tessellation with circular cap rings, smooth body normals, and cylindrical UV unwrap.
3. **Cone:** Parametric circular base and conical apex with smooth normal interpolation.
4. **Sphere:** UV latitude/longitude sphere with normalized radial normals.
5. **Plane:** Subdivided quad surface with perpendicular upward normal $(0, 1, 0)$ and customizable texture tiling.

---

## 4. Complete Object List & Animation System

The matsuri scene features **17 distinct composite objects** (exceeding the course requirement of 5 objects), each composed hierarchically from primitive meshes:

| # | Composite Object | Primitive Components | Hierarchical Rig & Dynamic Animation |
|:---:|---|---|---|
| **1** | **Ground Plane** | Scaled textured `Plane` | Static reference street surface with stone pavement tiling. |
| **2** | **Machiya Townhouses (x4)** | Scaled `Cube` frames, pyramidal `Cone`/`Cube` roofs, timber beams, interior rooms, tatami floors, stairs | Features **interactive sliding Shoji doors** (<kbd>H</kbd>) and **sliding windows** (<kbd>G</kbd>) with animated local translation; houses warm interior living room and bedroom lantern point lights. |
| **3** | **Torii Gate** | Vertical `Cylinder` columns, curved `Cube` lintels (kasagi/shimaki), tie-beams | Grand vermilion shrine gate decorated with 4 hanging Chochin lanterns, 2 pillar bracket lanterns, twin stone lanterns (Ishi-Doro), sacred Shimenawa straw rope with Shide streamers, and 2 dedicated dynamic point lights framing the street terminus. |
| **4** | **Sakura Tree** | `Cylinder` trunk & branches, multi-cluster pink `Sphere` foliage | **Complex motion:** Individual falling petal spheres detach, drift laterally via sinusoidal wind drift, and respawn at top branch nodes. |
| **5** | **Overhead Lantern Spans (x4)** | Catenary rope splines, 16 paper lanterns (`Sphere` + `Cylinder` caps) | **Relative transform:** Catenary rope pivot oscillation; child lantern bodies swing like pendulums; Point Lights #3 & #4 track swinging lantern positions. |
| **6** | **Takoyaki Food Stall** | Timber frame, fabric awning, metal griddle, 6 takoyaki spheres with sauce & aonori | **Complex motion:** Takoyaki balls spin continuously in grill cavities and execute periodic parabolic hops (flipping simulation). |
| **7** | **Kakigori Food Stall** | Timber stall, fabric noren, vintage hand-crank shaved ice machine, 6 dessert bowls | **Complex motion:** Shaved ice machine hand wheel continuously rotates; active mound dynamically shaves; 6 multi-flavored Kakigori bowls spin and hop on counter. |
| **8** | **Vendor Figures (x2)** | `Cylinder` torsos, heads, upper arms, forearms, chef bandanas | **Hierarchical rig:** Multi-joint shoulder and elbow articulation performing continuous cooking and serving motions. |
| **9** | **Magic Show Stage** | Low wooden platform `Cube`, red carpet `Plane`, gold leaf byobu folding screen | Static performance area elevating the magician and tricks. |
| **10** | **Magician Figure** | Articulated torso, cape, top hat, raising arm joints | Arm articulates into presentation pose holding the floating orb trick. |
| **11** | **Magic Trick 1: Floating Orb** | Glowing cyan `Sphere`, trailing comet-tail particle spheres | **Hierarchical motion:** Orb executes helical orbits around magician's hand bone; moving Point Light #0 brightens magician's face and stage. |
| **12** | **Magic Trick 2: Vanishing Box** | Gold-trimmed `Cube`, animated rippling silk cloth `Plane` | **State machine:** Silk cloth descends, box scales to zero ($1 \rightarrow 0$), cloth pulls aside, and box reappears at secondary stage location. |
| **13** | **Stage Spotlight Rig** | `Cylinder` pole, conical lamp housing, mounting gimbal | **Tracking motion:** Lamp housing continuously rotates and pitches to track the magician; dynamic Spot Light updates position and cone direction. |
| **14** | **Seated Audience (x8)** | 8 seated figures in a semicircle facing the magic stage | Subtle head-turning yaw oscillation for natural lifelike ambiance. |
| **15** | **Walking Crowd Figures** | Articulated figures with walking leg cycles | Figures traverse down the festival street, looping back when reaching boundaries. |
| **16** | **Fireworks Particle System** | Multi-rocket pool with burst particles | **Two-stage particle physics:** Rocket launch trajectory followed by radial spherical burst with gravity deceleration; triggers sky flash Point Light #5. |
| **17** | **Sky Dome** | Inverted celestial `Sphere` enclosing world | Smooth day/night color and gradient blending tied to directional sun/moon arc. |

---

## 5. Illumination, Shading & Colors

### 5.1 Dynamic Lighting Engine (16 Total Lights)
The lighting pipeline supports three distinct light classes rendered per-fragment using the **Blinn-Phong** reflection model:
- **1 Directional Light (Sun/Moon):** Sweeps along a celestial arc. Interpolates between warm sunlight $(\text{ambient } 0.42, \text{diffuse } 0.85)$ and cool moonlight $(\text{ambient } 0.12, \text{diffuse } 0.20)$ via the day/night blend factor.
- **14 Point Lights with Quadratic Distance Attenuation ($1 / (k_c + k_l d + k_q d^2)$):**
  - *Light 0 (Magic Orb):* Tracks moving orb position in real-time; casts dynamic cyan highlights.
  - *Lights 1 & 2 (Stalls):* Amber and cyan illumination above the food stalls.
  - *Lights 3 & 4 (Lantern Spans):* Track the oscillating world positions of swinging paper lanterns.
  - *Light 5 (Fireworks Sky Flash):* Dynamically activates at firework apex burst positions with random vibrant explosion tints.
  - *Lights 6–11 (Machiya Interiors):* Warm amber lighting inside ground-floor living rooms and upper bedrooms of all 4 townhouses.
  - *Lights 12 & 13 (Torii Shrine Gate):* Warm golden-amber radiant illumination positioned on the left and right sides of the Torii gate, tracking gate transforms.
- **1 Dynamic Spotlight (Stage):** Mounted inside the stage cone housing. Constrained by inner ($14^\circ$) and outer ($22^\circ$) cutoff cosines; rotates in real-time to track the magician.

### 5.2 Shading Models (<kbd>P</kbd>)
Pressing <kbd>P</kbd> cycles the scene shader through three modes:
1. **Full Blinn-Phong Shading:** Ambient + Diffuse + Specular highlights using the half-vector $\mathbf{H} = \frac{\mathbf{L} + \mathbf{V}}{\|\mathbf{L} + \mathbf{V}\|}$.
2. **Diffuse Only:** Ambient + Diffuse terms only (specular highlights disabled).
3. **Ambient Only:** Flat ambient baseline for visual comparison in academic reports.

---

## 6. Texturing & Emissive Glow Maps

### 6.1 Multi-Sampled Texture Engine
- Implemented in `src/Texture.h` with procedural fallbacks in `src/TextureGenerator.h`.
- Generates and binds dedicated 2D textures with bilinear filtering and mipmapping:
  - `wood_timber.bmp`: Rich grain texture for machiya building posts and stall frames.
  - `roof_tiles.bmp`: Traditional Japanese ceramic roof tile pattern.
  - `stone_pavement.bmp`: Flagstone street cobblestone texture.
  - `lantern_paper.bmp`: Washi paper texture with festival kanji characters.
  - `tatami_cloth.bmp`: Woven straw tatami mats and festival stall cloth.
  - `gold_leaf.bmp`: Ornate metallic finish for the magic stage byobu screen.
  - `sakura_bark.bmp`: Cherry blossom tree bark.
  - `takoyaki_food.bmp`: Golden-brown fried batter with seaweed/mayo details.
- **Emissive Maps:** Paper lanterns and interior Shoji window panes feature active emissive terms that radiate a warm golden glow during nighttime, independent of external diffuse lighting.
- **Texture Toggle (<kbd>X</kbd>):** Toggles texturing on/off to compare textured vs untextured Phong materials.

---

## 7. Advanced Graphics & Physics Extensions

### 7.1 Real-Time GPU Whitted Ray Tracing (<kbd>Z</kbd>) & CPU BMP Snapshot (<kbd>F9</kbd>)
- **Real-Time GPU Ray Tracer:** Renders through a full-screen quad (`shaders/raytrace.vert`, `shaders/raytrace.frag`). Evaluates ray-sphere, ray-box, and ray-plane analytical intersections with recursive specular reflections, shadow rays, and dynamic day/night sky dome.
- **CPU Ray Tracer Export:** High-precision offline ray tracer (`src/RayTracer.h`) exports a clean $1920 \times 1080$ snapshot directly to `raytraced_snapshot.bmp`.

### 7.2 16-Sample PCF Soft Shadow Mapping (<kbd>V</kbd>)
- Directional light shadow map rendered to a $2048 \times 2048$ depth framebuffer (`depthMapFBO`).
- Evaluates depth using an orthographic light-space matrix.
- Fragment shader samples a 16-point Poisson/grid disk with slope-scaled depth bias, producing soft shadow penumbras across the festival street.

### 7.3 Continuous Collision Detection & Doorway Portals (<kbd>B</kbd>)
- Axis-aligned bounding box (AABB) continuous collision resolver prevents the camera from clipping through townhouse exterior walls, interior dividing partitions, and stall counters.
- **Doorway Portals:** Sliding open the front Shoji door (<kbd>H</kbd>) removes the front entrance collision barrier, allowing the player to walk seamlessly inside the furnished townhouse.
- Pressing <kbd>B</kbd> toggles Noclip fly mode for aerial inspection.

---

## 8. Interactive Selection & In-Window HUD System

### 8.1 15 Controllable Inspectables (<kbd>T</kbd> / <kbd>Shift+T</kbd>)
The project exposes 15 distinct scene entities for live manual inspection and 6-DOF transformation:

1. `1. Lantern [Body]` (Child of Swinging Rope Pivot)
2. `2. Lantern [Rope Pivot]` (Parent Anchor Node)
3. `3. Magic Orb` (Child of Magician's Hand Bone)
4. `4. Magician Figure (Root)`
5. `5. Vanishing Box` (Scale-to-zero demo)
6. `6. Stage Spotlight Housing` (Tracking pivot)
7. `7. Takoyaki Stall` (Full Unit)
8. `8. Kakigori Stall` (Full Unit + Shaved Ice Machine)
9. `9. Torii Gate` (Grand Entrance)
10. `10. Sakura Blossom Tree`
11. `11. Crowd Walker #1`
12. `12. Machiya_L1` (Townhouse Building with Door & Windows)
13. `13. Machiya_L2` (Townhouse Building with Door & Windows)
14. `14. Machiya_R1` (Townhouse Building with Door & Windows)
15. `15. Machiya_R2` (Townhouse Building with Door & Windows)

**Selection Modes:**
- **Automatic Proximity Selection:** Automatically selects the nearest object in front of the camera (view cone dot product $\ge 0.45$).
- **Manual Locked Selection:** Pressing <kbd>T</kbd> cycles sequentially through all 15 objects. Pressing <kbd>Shift+T</kbd> unlocks manual mode and returns to automatic proximity selection.

**Live 6-DOF Transformation Controls:**
- **Translation:** $\pm X$ (<kbd>J</kbd> / <kbd>L</kbd>), $\pm Y$ (<kbd>I</kbd> / <kbd>K</kbd>), $\pm Z$ (<kbd>U</kbd> / <kbd>O</kbd>)
- **Rotation:** Pitch (<kbd>↑</kbd> / <kbd>↓</kbd>), Yaw (<kbd>←</kbd> / <kbd>→</kbd>)
- **Scaling:** Uniform Scale Up (<kbd>+</kbd> or <kbd>[</kbd>), Scale Down (<kbd>-</kbd> or <kbd>]</kbd>)

### 8.2 In-Window Minimal HUD Overlay (<kbd>F1</kbd>)
- **Rendering:** Separate 2D orthographic pass anchored to the top-right corner.
- **Font Rendering:** High-resolution embedded Consolas Bold atlas (`src/ui/FontAtlasData.h`) rendered with dedicated alpha-boosted shaders (`hud.vert`, `hud.frag`). Zero external dependencies.
- **Card Styling:** Minimal semi-transparent dark glass panel (~$225 \times 100$ px, 72% opacity, subtle gold border).
- **Minimal 5-Line Telemetry:**
  1. `FPS: xx.x` (Live framerate)
  2. `Target: [Object Name]` (Currently focused inspectable)
  3. `[T] Toggle target` (Selection cycle instruction)
  4. `[0] Lights: ON/OFF` (Lighting state)
  5. Context-sensitive action hint (e.g. `[H] Slide Open Shoji Door`, `[M] Replay Magic Trick`)
  6. `[F1] Hide HUD` (Discreet toggle hint)
- **Screenshot Protection:** The HUD automatically hides during clean screenshot captures (<kbd>F10</kbd> / <kbd>F9</kbd>).

---

## 9. Complete Interactive Controls Reference

| Key / Input | Category | Action / Purpose |
|:---:|:---:|---|
| **<kbd>W</kbd> / <kbd>A</kbd> / <kbd>S</kbd> / <kbd>D</kbd>** | Navigation | Move camera Forward / Left / Backward / Right |
| **<kbd>E</kbd> / <kbd>Q</kbd>** | Navigation | Move camera Vertically Up / Down |
| **Mouse Move** | Navigation | Look around (FPS Pitch and Yaw) |
| **Mouse Scroll** | Navigation | Field-of-View Zoom in / out ($1^\circ$ to $60^\circ$) |
| **<kbd>C</kbd>** | Navigation | Toggle mouse cursor lock / unlock |
| **<kbd>1</kbd> / <kbd>2</kbd> / <kbd>3</kbd>** | Navigation | Preset Camera Viewpoints (Street Entrance, Magic Stage, Torii & Sky) |
| **<kbd>R</kbd>** | Navigation | Reset camera position to street entrance origin |
| **<kbd>B</kbd>** | Physics | Toggle **Wall Collision Mode** (Solid walls & doors $\longleftrightarrow$ Noclip fly mode) |
| **<kbd>F1</kbd>** | Interface | **Toggle In-Window Minimal HUD Overlay** (Top-right corner) |
| **<kbd>F10</kbd>** | Capture | **Capture Viewport Screenshot** (`screenshot_clean.bmp`; <kbd>Shift+F10</kbd> captures with HUD) |
| **<kbd>Space</kbd>** | Simulation | Pause / Resume all scene animations |
| **<kbd>N</kbd>** | Lighting | Smooth Day $\longleftrightarrow$ Festival Night transition |
| **<kbd>0</kbd> / <kbd>KP_0</kbd>** | Lighting | **Toggle Lantern & Stall Illumination** (Lights ON / Dimmed) |
| **<kbd>P</kbd>** | Shading | Cycle Shading Model (Blinn-Phong $\rightarrow$ Diffuse Only $\rightarrow$ Ambient Only) |
| **<kbd>X</kbd>** | Texturing | Toggle Texturing (Textures ON / OFF) |
| **<kbd>V</kbd>** | Shadows | Toggle Realistic 16-Sample PCF Soft Shadows (ON / OFF) |
| **<kbd>Z</kbd>** | Ray Tracing | Toggle Real-Time GPU Whitted Ray Tracing Mode (ON / OFF) |
| **<kbd>F9</kbd>** | Ray Tracing | Capture & Export CPU Ray-Traced Snapshot to `raytraced_snapshot.bmp` |
| **<kbd>H</kbd>** | Interaction | **Slide Open / Close Nearest Shoji Door** (Smooth local translation) |
| **<kbd>G</kbd>** | Interaction | **Slide Open / Close Nearest Shoji Windows** (Smooth local translation) |
| **<kbd>M</kbd>** | Interaction | **Replay Magic Show Trick Sequence** (Vanishing box & orb) |
| **<kbd>F</kbd>** | Effects | Manually trigger a Firework rocket launch & apex burst |
| **<kbd>T</kbd>** | Inspection | **Cycle Selected Target Object** (<kbd>Shift+T</kbd> returns to Proximity Auto-Select) |
| **<kbd>J</kbd> / <kbd>L</kbd>** | Transform | Translate selected object along $\pm X$ (Left / Right) |
| **<kbd>I</kbd> / <kbd>K</kbd>** | Transform | Translate selected object along $\pm Y$ (Up / Down) |
| **<kbd>U</kbd> / <kbd>O</kbd>** | Transform | Translate selected object along $\pm Z$ (Forward / Backward) |
| **<kbd>↑</kbd> / <kbd>↓</kbd>** | Transform | Rotate selected object Pitch (around X axis) |
| **<kbd>←</kbd> / <kbd>→</kbd>** | Transform | Rotate selected object Yaw (around Y axis) |
| **<kbd>+</kbd> / <kbd>-</kbd>** or **<kbd>[</kbd> / <kbd>]</kbd>** | Transform | Scale selected object up (+10%) / down (-10%) |
| **<kbd>Esc</kbd>** | System | Exit application cleanly |

---

## 10. Milestone Checklist & Verification Status

- [x] **Phase 1: Core Framework & Windowing**
  - GLFW 3.5.1 window and modern OpenGL 3.3 Core Profile context initialized.
  - High-DPI framebuffer scaling, viewport callbacks, and delta-time loop active.
- [x] **Phase 1: Parametric Primitive Meshes**
  - Cube, Cylinder, Cone, Sphere, and Plane generators implemented with normals and UVs.
  - Reusable VBO/VAO/EBO mesh architecture verified.
- [x] **Phase 1: Hierarchical Scene Graph**
  - `Transform` matrix generation and `SceneNode` parent-child chaining fully functional.
  - World matrix propagation verified across articulated models.
- [x] **Phase 1: 17 Composite Scene Objects**
  - All 17 objects assembled, positioned, and animated across the festival street.
- [x] **Phase 1: Dynamic Animations**
  - Lantern pendulum oscillation, orb helical orbit, vanishing box sequence, firework particle stages, crowd walking cycles, takoyaki hops, kakigori shaving wheel, and falling sakura petals running smoothly.
- [x] **Phase 2: Illumination & Phong Shading**
  - Per-fragment Blinn-Phong lighting shader implemented with material shininess.
  - Shading model cycle (<kbd>P</kbd>) allows live switching between Blinn-Phong, Diffuse, and Ambient.
- [x] **Phase 2: 16 Dynamic Light Sources**
  - Directional Sun/Moonlight, 14 Point Lights (Orb, Stalls, Lanterns, Fireworks, Machiya interiors, Torii Gate shrine illuminations), and 1 Stage Spotlight implemented.
  - Real-time light position tracking verified (orb light, swinging lantern lights, spotlight housing tracking).
- [x] **Phase 2: Smooth Day $\longleftrightarrow$ Night Transition**
  - Key <kbd>N</kbd> interpolates celestial vectors, sky dome colors, ambient levels, and light intensities.
- [x] **Phase 3: Diffuse & Emissive Texturing**
  - Bitmap texture loader and procedural generators implemented for wood, roof tiles, stone, paper, tatami, and gold.
  - Emissive night glow verified on paper lanterns and Shoji rice-paper window panels.
- [x] **Phase 3: Texture Toggle**
  - Key <kbd>X</kbd> cleanly toggles texture mapping to evaluate Phong material colors.
- [x] **Advanced: Real-Time GPU Whitted Ray Tracing (<kbd>Z</kbd>)**
  - Full-screen quad ray-tracer shader with reflections, shadows, and analytical geometry intersections verified.
- [x] **Advanced: High-Resolution CPU Ray-Traced Snapshot (<kbd>F9</kbd>)**
  - Offline ray-tracing engine generating uncompressed BMP snapshots.
- [x] **Advanced: 16-Sample PCF Soft Shadows (<kbd>V</kbd>)**
  - $2048 \times 2048$ shadow map FBO and percentage-closer filtering operational.
- [x] **Advanced: Continuous Collision Detection & Portals (<kbd>B</kbd>)**
  - AABB wall collision prevents clipping; doorway portals allow entry when Shoji doors open.
- [x] **Advanced: Interactive Shoji Doors (<kbd>H</kbd>) & Sliding Windows (<kbd>G</kbd>)**
  - Hierarchical ease-in-out local translation relative to Machiya frame verified.
- [x] **Advanced: In-Window Minimalist HUD Overlay (<kbd>F1</kbd>)**
  - Embedded Consolas Bold font atlas, 5-line clean telemetry, and auto-hide during screenshots active.
- [x] **Advanced: 15 Controllable Inspectables & Selection Manager (<kbd>T</kbd> / <kbd>Shift+T</kbd>)**
  - Proximity view-cone auto-selection and manual cycling across all 15 inspectables (including the 4 Machiya buildings).
- [x] **Automated Verification Suite (Main.cpp)**
  - Comprehensive headless test harness verifying **311 / 311 automated tests with 100% success**.

---

## 11. Verification & Automated Test Harness

The project includes an automated test suite executed via `--test` in [`Main.cpp`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Main.cpp). It verifies 311 individual assertions covering:
1. **6-DOF Transformations on All 15 Inspectables:** $\pm X, \pm Y, \pm Z$ translation, pitch/yaw rotation, scaling up/down, and world matrix mathematical validity.
2. **Shoji Doors & Windows:** Initial closed state, proximity trigger, animated opening progression, closing toggle, and distance rejection.
3. **Environmental & Lighting Toggles:** Day/night state inversion, animation pause/resume, Blinn-Phong/diffuse/ambient shading cycle, texture toggle, PCF shadow toggle, GPU ray tracing toggle, and lantern light remapping (<kbd>0</kbd> / <kbd>KP_0</kbd>).
4. **Collision & Portals:** Solid wall blocking, doorway portal pass-through when door is open, and noclip bypass.
5. **Particle Physics & Animation Dynamics:** Firework rocket launch and sky burst lifecycles, Kakigori shaver wheel rotation, dessert bowl spinning.
6. **HUD & Interaction Manager:** Top-right overlay rendering, font atlas integrity, context action generation, auto-selection view cone evaluation, and BMP screenshot generation.

**Test Execution Result:** `311 / 311 TESTS PASSED (100% SUCCESS)`.
