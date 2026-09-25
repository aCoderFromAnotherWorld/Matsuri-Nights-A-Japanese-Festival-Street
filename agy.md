# Matsuri Nights — Development & Change Log (`agy.md`)
**Project:** Matsuri Nights — A Japanese Festival Street  
**Course:** CSE4102 Computer Graphics & Image Processing Laboratory  
**Author:** MD. Abu Hasanat Soykot | **Roll:** 2107100 | **Group:** B2  

---

## Overview
This document tracks all features, additions, bug fixes, transformations, and architectural updates implemented in the project. Whenever any component, asset, animation, control, or shader is modified, added, or fixed, a detailed entry is logged here.

---

## Log Entries

### [2026-09-25] — Phase 3: Anatomical Rigs, Stall Lighting, Sagging Catenary Ropes & Input Polish

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
