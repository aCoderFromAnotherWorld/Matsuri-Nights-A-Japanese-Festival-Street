# Matsuri Nights — Japanese Festival Street Scene
### OpenGL / C++ Computer Graphics Course Project — Master Plan

**Course:** CSE4102 — Computer Graphics and Image Processing Laboratory

This document fully describes the project so that any developer or AI coding agent
can read it and implement the project from scratch without further clarification.

---

## 1. Project Summary

**Title:** Matsuri Nights — A Japanese Festival Street

**Description:** A real-time interactive 3D recreation of a traditional Japanese
summer street festival (matsuri). A single street lined with Japanese-style wooden
buildings (machiya), strung with paper lanterns, ending at a torii gate, with a
cherry blossom tree, two food stalls, a small magic-show area (magician + seated
audience performing two tricks), a walking crowd, and a night sky with fireworks.
The scene renders in two lighting states — daylight and nightlight — with a
transition between them. The project emphasizes hierarchical (parent-child) 3D
transformations, transformations relative to another object's reference frame,
multiple moving/transforming light sources, Phong illumination and shading, and
(in the final phase) texturing.

**Course constraints to satisfy:**
- Keep the scene simple/compact — smaller scope than prior years' projects.
- Must demonstrate every model-transformation type taught in class (translate,
  rotate, scale, and combined/hierarchical transforms).
- Must have at least 5 distinct objects, including complex moving objects.
- Must show: an object transformed relative to another object's reference frame,
  a change in lighting conditions, how transforming an object affects its
  color/lighting, and moving/transforming the light source itself and its effect.

---

## 2. Tech Stack & Environment

- **Language:** C++ (C++17)
- **Graphics API:** Modern OpenGL (Core Profile, 3.3+)
- **Windowing/Input:** GLFW 3.5.1 (already downloaded)
- **OpenGL Loader:** GLAD (already downloaded)
- **IDE:** Visual Studio (Windows)
- **Math library:** GLM (header-only; add via vcpkg or manual include) — used for
  vec3/mat4/quaternion math, `glm::translate`, `glm::rotate`, `glm::scale`,
  `glm::perspective`, `glm::lookAt`.
- **Image loading (Phase 3 only):** stb_image.h (single-header, for textures).
- **Build setup:** A native Visual Studio project/solution (.sln + .vcxproj).
  Link against `glfw3.lib` and compile `glad.c` as part of the project. Include
  directories point to `glfw-3.5.1/include`, `glad/include`, and `glm/`.

### Suggested folder structure

```
MatsuriNights/
├── MatsuriNights.sln
├── MatsuriNights.vcxproj
├── external/
│   ├── glfw-3.5.1/
│   ├── glad/
│   └── glm/
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── Shader.h / Shader.cpp
│   │   ├── Camera.h / Camera.cpp
│   │   ├── Mesh.h / Mesh.cpp
│   │   ├── Transform.h            (position, rotation, scale -> mat4)
│   │   ├── SceneNode.h / .cpp     (hierarchical parent-child node)
│   │   └── Light.h                (point/directional/spot light structs)
│   ├── primitives/
│   │   ├── Cube.h/.cpp
│   │   ├── Cylinder.h/.cpp
│   │   ├── Cone.h/.cpp
│   │   ├── Sphere.h/.cpp
│   │   └── Plane.h/.cpp
│   ├── objects/
│   │   ├── Building.h/.cpp
│   │   ├── ToriiGate.h/.cpp
│   │   ├── SakuraTree.h/.cpp
│   │   ├── Lantern.h/.cpp
│   │   ├── FoodStall.h/.cpp
│   │   ├── VendorFigure.h/.cpp
│   │   ├── CrowdFigure.h/.cpp
│   │   ├── Magician.h/.cpp
│   │   ├── MagicOrb.h/.cpp
│   │   ├── VanishingBox.h/.cpp
│   │   └── Firework.h/.cpp        (particle system)
│   ├── scene/
│   │   └── Scene.h/.cpp           (owns all objects, update() and render())
│   └── shaders/
│       ├── basic.vert / basic.frag       (Phase 1: flat color, no lighting)
│       ├── phong.vert / phong.frag       (Phase 2: Phong lighting)
│       └── textured.vert / textured.frag (Phase 3: lighting + textures)
└── assets/
    └── textures/   (added in Phase 3)
```

---

## 3. Core Architecture

### 3.1 Transform & Hierarchy
Every object has a `Transform` (position, rotation as Euler angles or quaternion,
scale) that produces a local `mat4`. Objects are organized as a **scene graph**:
each `SceneNode` has a parent and a list of children. A node's world matrix is
`parent.worldMatrix * node.localMatrix`. This is the mechanism used for every
"transform relative to another object's reference frame" requirement:

- `Lantern` node is a child of a `Rope` node, which is a child of the `Building`
  or `Cart` node. Swinging the lantern only changes the local rotation of the
  lantern node — its world position automatically follows the parent.
- `MagicOrb` node is a child of the `Magician`'s hand-bone node while orbiting,
  so the orb's world position is `handWorldMatrix * orbitOffset(t)`.
- Vendor / magician / crowd limb rotations use the same parent-child chain
  (torso → upper arm → forearm).

### 3.2 Mesh / Primitives
Build 5 base primitive generators first (positions + normals, indices for
indexed drawing): `Cube`, `Cylinder`, `Cone`, `Sphere`, `Plane`. Each returns a
`Mesh` (VAO/VBO/EBO) that can be reused (instanced) with different transforms
and colors. All complex objects in section 5 are built by combining and
transforming these primitives (e.g., a lantern = 1 stretched sphere/cylinder +
2 small cylinders as caps; a building = several cubes + 1 stretched pyramid/cone
for the roof).

### 3.3 Camera
Free-fly or orbit camera (WASD + mouse look, or click-drag orbit) so the user
can inspect the street from any angle — used to demonstrate the transformations
during the class presentation.

### 3.4 Scene / Update-Render Loop
`Scene::update(float dt)` advances all animations (lantern swing angle, orb
orbit angle, firework particle positions, crowd walk positions, day-night
blend factor). `Scene::render()` walks the scene graph, computing world
matrices and issuing draw calls with the active shader.

### 3.5 Live Testing / Manual Transform Support
Because every object and light is built on the single `Transform` +
`SceneNode` system above, none of it should be special-cased. Any object's
position/rotation/scale, and any light's position/direction/color, must be
reachable and editable at runtime (e.g., via a small debug key map or an
ImGui-free simple input scheme) so that, during in-class evaluation, a
specific object can be transformed on request relative to another object's
reference frame, or a light source can be moved/retargeted live to show its
effect on nearby shading and color — without needing to change and recompile
code mid-demo.

---

## 4. World Layout (approximate, along the Z axis, street running north-south)

```
                              [Torii Gate]
                                   |
   [Building] [Building]     [Sakura Tree]     [Building] [Building]
        |                                              |
   [Lantern rows strung across the street, both sides]
        |
   [Takoyaki Stall]                              [Kakigori Stall]
        |                                              |
   [Magic Show Stage + Magician + Audience semicircle]
        |
   [Crowd figures walking along the street]
        |
   [Ground plane / street pavement]
        |
   [Sky dome + Sun/Moon directional light + Fireworks above]
```

---

## 5. Object List (build order = Phase 1 order)

For each object: primitive composition, hierarchy, and required motion.

1. **Ground Plane** — 1 large scaled `Plane`. Static. Reference surface for
   everything else.
2. **Machiya Buildings (x4, instanced)** — box (`Cube`, scaled) body + a
   stretched, rotated `Cube` or pyramid-like shape (built from a scaled `Cone`
   with 4 sides, or a hand-built pyramid) for the sloped roof + smaller cubes
   for window frames. Static, but window color/emission will change between
   day/night in Phase 2.
3. **Torii Gate** — 2 vertical `Cylinder` posts + 2 horizontal `Cylinder`/`Cube`
   crossbeams. Static, placed at the far end of the street as a focal point.
4. **Sakura Tree** — `Cylinder` trunk + several small `Sphere` blossom clusters
   (instanced, pink). **Moving part:** a small number of individual blossom
   `Sphere`s detach periodically and fall (translate downward + slow rotate +
   slight sideways drift, looping/respawning at the top) — a simple particle-like
   complex motion.
5. **Lanterns (x8–12, instanced along ropes)** — a stretched `Sphere` or capsule
   (two cylinders + a sphere) body + small cylinder caps. Each lantern is a
   child node of a short `Rope` node; the rope node's local rotation oscillates
   (`sin(time * speed + phaseOffset)`) producing a pendulum swing. **This is a
   required hierarchical/relative-transform demo object.**
6. **Takoyaki Stall** — `Cube` stall frame + `Plane` awning + `Cylinder` grill
   plate with 6–8 small `Sphere` takoyaki balls. **Moving part:** balls rotate
   in place (spin) and periodically arc-translate (parabolic hop) from one grill
   hole to the next, simulating flipping.
7. **Kakigori Stall** — `Cube` stall frame + `Plane` awning/noren curtain (with
   a vertex-shader or per-vertex sine-wave ripple for cloth motion) + a `Cone`
   nobori flag pole with a `Plane` flag that flutters (sine-wave rotation about
   its vertical edge).
8. **Vendor Figures (x2, one per stall)** — hierarchical rig: `Cube`/`Cylinder`
   torso → `Cylinder` upper arm → `Cylinder` forearm, each a child SceneNode.
   **Moving part:** shoulder/elbow joints rotate in a repeating stir/serve cycle.
9. **Magic Show Stage** — a low, wide `Cube` platform. Static.
10. **Magician** — hierarchical rig like the vendor (torso, head `Sphere`, two
    arms with shoulder/elbow joints, legs optional). One arm periodically raises
    (rotation) to present the orb trick.
11. **Magic Trick 1 — Floating Orb** — a glowing `Sphere`, child node of the
    magician's raised hand. **Moving part:** local offset follows a helical path
    `offset = (r*cos(t), h(t), r*sin(t))` relative to the hand — combined
    rotation + vertical translation, i.e. an orbit that also rises and falls.
    A few smaller trailing spheres (delayed copies of recent orb positions,
    shrinking/fading) create a comet-tail effect.
12. **Magic Trick 2 — Vanishing Box** — a small `Cube` on the stage + a `Plane`
    "silk cloth" above it with a rippling vertex animation. Animation sequence
    (looping, driven by a timer/state machine): cloth descends onto the box →
    box scales to 0 over ~0.5s → cloth is yanked aside (fast translate+rotate)
    → box reappears (scale from 0 → 1) at a second stage position. Demonstrates
    scale-to-zero transform and a discrete position swap.
13. **Spotlight Rig** — a small `Cone` "lamp housing" mounted on a thin
    `Cylinder` pole above the stage. Its light direction/target slowly pans to
    track the magician (rotation). Counts as both an object and a moving light
    (see Section 6).
14. **Audience (x8, simple static or lightly animated figures)** — simplified
    `Cube`/`Cylinder` seated figures arranged in a semicircle facing the stage.
    Optionally 1–2 have a slow head-turn rotation for subtle life.
15. **Crowd Figures (x5–8, walking)** — same simple rig as vendor figures,
    minus the stirring arm. **Moving part:** each figure translates along a
    straight or gently curved path down the street at a constant speed,
    looping back to the start (or reversing direction) when it reaches the end.
    Optional: a light leg-swing (two `Cylinder` legs alternating rotation) for
    a walk-cycle look.
16. **Fireworks (particle system, night only)** — implemented as a pool of
    small `Sphere` (or billboard quad) particles. Each firework instance runs a
    2-stage animation: (a) **launch** — a single bright particle translates
    upward from ground level to a random height with ease-out; (b) **explosion**
    — at apex, spawn 20–40 particles that translate outward from the burst
    point along random unit directions, scaled by elapsed time, pulled down by
    a simple gravity term, and fade alpha to 0 over ~1.5s. Stagger multiple
    fireworks with randomized delay, color, and burst position so several are
    visible at once.
17. **Sky Dome** — a large inverted `Sphere` (or hemisphere) surrounding the
    scene, used first as a flat-color/gradient day-night background (Phase 1–2)
    and optionally texture-mapped with a starfield/sky texture in Phase 3.

*(Objects 1–17 total well over the minimum 5-object requirement, with objects
4, 5, 6, 8, 10, 11, 12, 13, 15, and 16 all qualifying as "complex moving
objects.")*

---

## 6. Phase Breakdown

### Phase 1 — Geometry, Structure & Motion (no lighting yet)

Goal: every object above exists, is correctly positioned/scaled, and all
animations run correctly, rendered with **flat per-object colors** only
(`basic.vert`/`basic.frag`, no lighting math — just `gl_Position` and a
uniform/vertex color).

Tasks:
1. Set up the Visual Studio project: link GLFW + GLAD, create a window, GL
   context, and a basic render loop with delta-time.
2. Implement `Shader` class (load/compile/link .vert+.frag, set uniforms).
3. Implement the 5 base primitive mesh generators (Cube, Cylinder, Cone,
   Sphere, Plane) with position+normal data (normals unused until Phase 2 but
   generate them now to avoid redoing meshes later).
4. Implement `Transform` and `SceneNode` (parent-child hierarchy, world matrix
   computation).
5. Implement free-fly/orbit `Camera` with keyboard + mouse controls.
6. Build each object in Section 5 as a small class that constructs its
   `SceneNode` sub-tree from the primitives, exposing an `update(float t)` for
   its own animation logic and using flat/solid colors per part.
7. Assemble the full `Scene`: instantiate all objects at their world-layout
   positions (Section 4), implement `Scene::update()` and `Scene::render()`.
8. Verify every required motion behaves correctly: lantern swing, orb orbit
   around the hand, vanishing-box sequence, firework launch+explosion, crowd
   walking loops, stall ball flipping, flags/cloth rippling, falling sakura
   petals.
9. Add keyboard toggles: pause/resume all animation, and a day/night flag
   (color-only placeholder for now — e.g., swap the sky-dome flat color and
   building window color) to confirm the state machine works before real
   lighting is added.

**Phase 1 deliverable:** a navigable scene with correct geometry, hierarchy,
and all motion, in flat colors, day/night background color swap working.

---

### Phase 2 — Illumination, Shading & Colors

Goal: replace flat shading with a full Phong lighting pipeline and multiple
light sources, and use it to demonstrate every lighting requirement from the
course brief.

Tasks:
1. Write `phong.vert`/`phong.frag`: pass world-space position and normal
   (transformed by the normal matrix) to the fragment shader; compute
   ambient + diffuse + specular per light, per fragment (Phong shading, i.e.
   normal interpolated and lit per-fragment, not per-vertex).
2. Define a `Light` struct supporting three types: **directional** (sun/moon),
   **point** (lanterns, stall lights, orb, firework bursts), and **spot**
   (stage spotlight). Support an array of active lights in the fragment shader
   (a fixed max, e.g. 8, with an active count uniform).
3. **Sun/Moon (directional light):** direction/angle sweeps across the sky
   over time (or via a day↔night key toggle that lerps direction, color and
   intensity between a bright warm-white "sun" state and a dim cool-blue
   "moon" state). Drive the sky-dome color and overall scene ambient from this
   same day/night blend factor.
4. **Lantern point lights:** each lit lantern object also registers a point
   light at its (animated, swinging) world position with a warm orange color;
   intensity is ~0 by day and on by night. Because the lantern's position is
   driven by the same hierarchical transform as Section 5.5, this directly
   demonstrates "transforming an object relative to another's reference frame
   changes the lighting it produces."
5. **Stall lights:** small point lights above each food stall, always-on at
   night, for local illumination and specular highlights on stall surfaces.
6. **Spotlight:** implement as a cone-attenuated spot light at the rig from
   Section 5.13; its direction is updated each frame to track the magician,
   so its rotation visibly moves the lit region and shifts specular highlights
   on the magician and stage.
7. **Orb light:** while Trick 1 plays, the floating orb itself contributes a
   small, moving point light (bright, slightly colored) that brightens the
   magician's face and hands as it orbits — ties motion directly to lighting
   change.
8. **Firework lights:** during an explosion frame, briefly add a strong,
   short-lived colored point light at the burst position so nearby buildings/
   lanterns visibly tint for a few frames, then it's removed as particles fade.
9. Assign distinct material colors + shininess per object/part (wood, roof
   tile, paper lantern, cloth, skin, metal grill, stage) so the Phong
   specular term is visually meaningful across different materials.
10. Optional but recommended for the report: add a debug key to switch between
    **Flat**, **Gouraud** (per-vertex lighting), and **Phong** (per-fragment)
    shading on one object (e.g., the takoyaki grill or a lantern) to visually
    compare and justify using Phong shading in the final version.
11. Finalize the day→night transition: interpolate directional light,
    ambient level, sky color, and lantern/stall/spotlight intensities together
    over a few seconds (triggered by a key or auto-cycled), so the "whole
    scenario has daylight and nightlight views" requirement is a smooth,
    demonstrable transition rather than an abrupt cut.

**Phase 2 deliverable:** full Phong-lit scene, multiple static and moving
light sources, a working day-night lighting transition, and a clear in-class
demo where moving/transforming a light (lantern swing, spotlight pan, orb
orbit, firework burst) visibly changes surrounding colors/shading.

---

### Phase 3 — Texturing

Goal: replace flat material colors with image textures to make the scene
visually polished, without breaking Phase 1/2 functionality.

Tasks:
1. Integrate `stb_image.h`; implement a `Texture` loader (2D texture, mipmaps,
   wrap/filter settings).
2. Extend the shader to `textured.vert`/`textured.frag`: add UV coordinates to
   every primitive generator (Cube, Cylinder, Cone, Sphere, Plane), sample a
   diffuse texture and combine with the existing Phong lighting (texture color
   as the diffuse/ambient base, keep specular from material settings or a
   specular map if time allows).
3. Source/create textures for: wood paneling (buildings, stall frames), roof
   tiles, red paper-lantern texture (with a warm glow/emissive map), cloth
   patterns (noren curtain, nobori flag, magician's cloth), stone/wood ground
   pavement, tree bark + sakura blossom, skin/clothing tones for figures
   (vendor, magician, crowd, audience), and a night starfield / day sky
   gradient texture for the sky dome.
4. Apply an emissive texture/term to lantern paper and window shoji panels so
   they visually "glow" at night even before nearby point lights fully light
   the scene.
5. Re-test day/night transition and all Phase-1/2 motions with textures
   applied to confirm nothing regressed (UV seams, incorrect tiling, lighting
   still correct).

**Phase 3 deliverable:** the complete, textured, lit, animated festival scene
— the final submission build.

---

## 7. Controls (suggested)

| Key / Input | Action |
|---|---|
| W/A/S/D + mouse | Move/look camera |
| Space | Pause / resume all animation |
| N | Toggle / trigger day → night transition |
| 1 / 2 / 3 | Force flat / Gouraud / Phong shading on the debug test object (Phase 2 only) |
| L | Toggle lantern lights on/off |
| F | Manually trigger a firework burst |
| M | Replay the magic show trick sequence |
| T | Cycle a debug selection through key objects/lights for live manual transform testing |

---

## 8. Milestone Checklist

- [x] Phase 1: Project builds and runs (GLFW window + GLAD context)
- [x] Phase 1: All 5 primitives implemented and reusable
- [x] Phase 1: Scene graph / hierarchical transform system working
- [x] Phase 1: Any object/light reachable for live manual transform testing (Section 3.5)
- [x] Phase 1: All 17 objects placed and correctly composed from primitives
- [x] Phase 1: All required motions implemented and verified
- [ ] Phase 2: Phong shader (ambient+diffuse+specular, per-fragment) working
- [ ] Phase 2: Directional (sun/moon), point (lanterns/stalls/orb/fireworks), and spot (stage) lights implemented
- [ ] Phase 2: Day↔night transition working end-to-end
- [ ] Phase 2: Flat/Gouraud/Phong comparison mode (optional, for the report)
- [ ] Phase 3: Texture loader + UV coordinates on all primitives
- [ ] Phase 3: All planned textures applied and scene re-verified
- [ ] Final: Record demo video / prepare in-class walkthrough covering every course requirement (relative transform, lighting change, moving light source, color/lighting under transform)

---

## 9. Notes for an Implementing Agent

- Build strictly in the phase order above; do not add textures or lighting
  math before Phase 1's geometry and motion are verified — this matches the
  student's requested 3-phase plan and keeps each phase independently
  demonstrable.
- Reuse the 5 primitive mesh generators everywhere; do not hand-author
  one-off vertex arrays for individual objects.
- Every "moving relative to another object" requirement in the course brief
  must be implemented via the `SceneNode` parent-child hierarchy (local
  transform composed with parent's world transform), not by manually
  computing world-space offsets in object code.
- Keep object counts/detail modest per the "simple scene" course constraint —
  favor a smaller number of well-animated, well-lit objects (already listed
  in Section 5) over adding many more; the object list above is intentionally
  sized to comfortably clear the 5-object/complex-motion minimum without
  overshooting project scope.
