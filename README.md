# Matsuri Nights — A Japanese Festival Street
### CSE4102: Computer Graphics and Image Processing Laboratory
**Student Name:** MD. Abu Hasanat Soykot  
**Roll:** 2107100 | **Group:** B2  

---

## Overview
**Matsuri Nights** is a real-time interactive 3D simulation of a traditional Japanese summer street festival (*matsuri*). Built from scratch using modern OpenGL (Core Profile 3.3+) and C++20, the project demonstrates 3D model transformations, hierarchical (parent-child) scene graph architectures, dynamic lighting, and complex kinematic systems.

---

## Implemented Architecture (Phase 1 Complete)

1. **Hierarchical Scene Graph System ([`SceneNode`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/SceneNode.h) & [`Transform`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Transform.h)):**
   - Centralized local transformations (Position, Euler Rotation, Scale).
   - Parent-child tree traversal where world matrices are computed via:
     $$\mathbf{M}_{\text{world}} = \mathbf{M}_{\text{parent}} \times \mathbf{M}_{\text{local}}$$
   - Directly demonstrates transformations relative to another object's reference frame.

2. **5 Reusable Base Primitives ([`Primitives.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Primitives.h)):**
   - **Cube** (24 unique vertices with orthogonal normals & UVs)
   - **Cylinder** (Discs + side mantle with radial normals)
   - **Cone** (Base + slanted mantle normals)
   - **Sphere** (UV sphere parameterized by latitude/longitude)
   - **Plane** (Subdivided XZ grid)

3. **All 17 Planned Scene Objects ([`Objects.h`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/src/Objects.h)):**
   - **Ground & Street:** Cobblestone curbs, stone pavement, and surrounding festival earth.
   - **Machiya Buildings (x4):** 2-story traditional timber townhouses with sloped tile eaves roofs, ridge caps, and shoji window screens that glow warmly at night.
   - **Torii Gate:** Grand shrine entrance portal anchoring the street end, with base stones, tilted pillars, nuki, gakuzuka, shimaki, and curved kasagi.
   - **Sakura Tree & Falling Petals:** Gnarled trunk, pink blossom foliage clusters, and 8 detached falling blossom petals drifting with sinusoidal sway.
   - **Swaying Lanterns (x10):** Chochin lanterns hung on overhead street ropes. Each lantern swings like a pendulum via its rope pivot node ($`\theta = A \sin(\omega t + \phi)`$).
   - **Takoyaki Stall:** Wood stall with awning, griddle, and 6 takoyaki balls spinning in place and periodically leaping between molds in parabolic hopping arcs.
   - **Kakigori Stall:** Shaved ice yatai featuring an ice machine with spinning crank wheel, rippling *noren* cloth banner, and fluttering *nobori* flag.
   - **Vendor Figures (x2):** Articulated vendor figures with shoulder and elbow joint kinematics stirring and serving.
   - **Magic Show Stage:** Raised wooden deck with festive red carpet trim and golden folding screen (*byobu*).
   - **Magician Figure:** Stands on stage with wizard hat and raised wand hand.
   - **Floating Magic Orb (Trick 1):** Child node of the magician's hand bone performing a 3D helical orbit ($`r\cos(t), y(t), r\sin(t)`$) with a 3-sphere comet tail.
   - **Vanishing Box (Trick 2):** State machine animating a silk cloth covering the box, scaling the box down to 0, whipping the cloth aside, and restoring the box at a second stage location.
   - **Spotlight Rig:** Cone lamp housing smoothly rotating and tilting to track the magician.
   - **Audience Figures (x6):** Seated semicircle facing the stage with heads bobbing and turning.
   - **Crowd Figures (x6):** Figures walking down the street with alternating leg-swing walk cycles that wrap around seamlessly.
   - **Fireworks Particle System:** Multi-rocket launcher system in the night sky with upward launch trails and spherical particle burst explosions under gravity.
   - **Sky Dome:** Atmospheric sky hemisphere with smooth gradient transitioning from daylight blue to festive starry night indigo.

---

## Interactive Controls
*(For full detailed descriptions and academic evaluation notes, see [controls.md](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/controls.md)).*

| Input | Function |
|---|---|
| **W / A / S / D** | Fly camera Forward / Left / Backward / Right |
| **E / Q** | Fly camera Up / Down |
| **Mouse Look** | Rotate camera view direction |
| **C** | Toggle mouse cursor capture |
| **Space** | Pause / Resume all kinematics and animations |
| **N** | Smooth Day $\longleftrightarrow$ Festival Night transition |
| **F** | Manually trigger a Firework rocket launch & burst |
| **1 / 2 / 3** | Preset camera viewpoints (1: Entrance, 2: Magic Stage, 3: Torii & Sky) |
| **T** | Cycle through objects for **Live In-Class Inspection & Transform** |
| **I / K** | Translate selected object along $\pm Y$ (Up / Down) |
| **J / L** | Translate selected object along $\pm X$ (Left / Right) |
| **U / O** | Translate selected object along $\pm Z$ (Forward / Backward) |
| **Arrow Keys** | Rotate selected object (Pitch / Yaw) live |
| **[ / ]** | Scale selected object down / up live |
| **R** | Reset camera position |
| **Esc** | Exit application |

---

## Build & Run Instructions

### Prerequisites
- Visual Studio 2022 / 2026 with C++ Desktop Development workload (MSVC v145 or v143, C++20).
- Windows 10 / 11.

### Using Visual Studio
1. Open [`Matsuri Nights — A Japanese Festival Street/Matsuri Nights - A Japanese Festival Street.slnx`](file:///C:/Users/mdabu/OneDrive/Desktop/practice/Graphics/Matsuri-Nights-A-Japanese-Festival-Street/Matsuri%20Nights%20%E2%80%94%20A%20Japanese%20Festival%20Street/Matsuri%20Nights%20-%20A%20Japanese%20Festival%20Street.slnx).
2. Set configuration to **Debug | x64**.
3. Press **F5** (Local Windows Debugger) to run.

### Direct Execution
The precompiled 64-bit executable is ready at:
```
Matsuri Nights — A Japanese Festival Street/x64/Debug/Matsuri Nights - A Japanese Festival Street.exe
```