# Matsuri Nights — Interactive Controls Reference Manual
### CSE4102: Computer Graphics and Image Processing Laboratory
**Project:** Matsuri Nights — A Japanese Festival Street  
**Student:** MD. Abu Hasanat Soykot | **Roll:** 2107100 | **Group:** B2  

---

## 1. Quick Reference Cheat Sheet

| Key / Input | Action / Purpose |
|:---:|---|
| **<kbd>W</kbd> / <kbd>A</kbd> / <kbd>S</kbd> / <kbd>D</kbd>** | Fly camera Forward / Left / Backward / Right |
| **<kbd>E</kbd> / <kbd>Q</kbd>** | Fly camera Up / Down |
| **Mouse Move** | Look around (FPS-style Pitch and Yaw) |
| **Mouse Scroll** | Zoom in / Zoom out (Adjusts Field of View from $1^\circ$ to $60^\circ$) |
| **<kbd>C</kbd>** | Toggle mouse cursor lock / unlock |
| **<kbd>F1</kbd>** | **Toggle In-Window Semi-Transparent HUD Overlay** (Top-right corner) |
| **<kbd>F10</kbd>** | **Capture Viewport Screenshot** (`screenshot_clean.bmp`; <kbd>Shift+F10</kbd> captures with HUD) |
| **<kbd>R</kbd>** | Reset camera back to festival street entrance |
| **<kbd>1</kbd>** | Preset View 1: Festival Street Entrance |
| **<kbd>2</kbd>** | Preset View 2: Magic Show Stage Close-up |
| **<kbd>3</kbd>** | Preset View 3: Torii Gate & Fireworks Sky |
| **<kbd>Space</kbd>** | Pause / Resume all scene animations and motion |
| **<kbd>N</kbd>** | Smooth Day $\longleftrightarrow$ Festival Night transition |
| **<kbd>0</kbd> / <kbd>KP_0</kbd>** | **Toggle Lantern & Stall Illumination** (Lights ON / Dimmed) |
| **<kbd>M</kbd>** | **Replay Magic Show Trick Sequence** (Vanishing box & floating orb) |
| **<kbd>F</kbd>** | Manually launch a Firework rocket |
| **<kbd>P</kbd>** | Cycle Shading Mode (Blinn-Phong $\rightarrow$ Diffuse Only $\rightarrow$ Ambient Only) |
| **<kbd>X</kbd>** | Toggle Diffuse Textures ON / OFF |
| **<kbd>V</kbd>** | Toggle **Realistic PCF Soft Shadows** ON / OFF (16-sample filter & shadow acne bias) |
| **<kbd>H</kbd>** | **Interact with Nearest House Door** (Smoothly slide Shoji door open / close) |
| **<kbd>G</kbd>** | **Interact with Nearest House Windows** (Smoothly slide Shoji windows open / close) |
| **<kbd>B</kbd>** | Toggle **Wall Collision Mode** (Walk Mode: solid walls & stairs $\longleftrightarrow$ Noclip Fly Mode) |
| **<kbd>Z</kbd>** | Toggle **Real-Time GPU Ray Tracing Mode** ON / OFF (60+ FPS Whitted Ray Tracer) |
| **<kbd>F9</kbd>** | Capture & Export **CPU Multi-Threaded Ray-Traced Snapshot** to `raytraced_snapshot.bmp` |
| **<kbd>T</kbd>** | **Cycle Target Object Selection** (<kbd>Shift+T</kbd> returns to Proximity Auto-Select) |
| **<kbd>I</kbd> / <kbd>K</kbd>** | Translate selected object along $\pm Y$ (Up / Down) |
| **<kbd>J</kbd> / <kbd>L</kbd>** | Translate selected object along $\pm X$ (Left / Right) |
| **<kbd>U</kbd> / <kbd>O</kbd>** | Translate selected object along $\pm Z$ (Forward / Backward) |
| **<kbd>↑</kbd> / <kbd>↓</kbd>** | Rotate selected object Pitch (around X axis) |
| **<kbd>←</kbd> / <kbd>→</kbd>** | Rotate selected object Yaw (around Y axis) |
| **<kbd>+</kbd> / <kbd>-</kbd>** or **<kbd>[</kbd> / <kbd>]</kbd>** | Scale selected object up (+10%) / down (-10%) |
| **<kbd>Esc</kbd>** | Exit application cleanly |

---

## 2. Camera & Navigation Controls

* **Fly Navigation (<kbd>W</kbd>, <kbd>A</kbd>, <kbd>S</kbd>, <kbd>D</kbd>, <kbd>E</kbd>, <kbd>Q</kbd>):**
  * <kbd>W</kbd> moves the camera in the forward viewing direction.
  * <kbd>S</kbd> moves the camera backward.
  * <kbd>A</kbd> and <kbd>D</kbd> strafe horizontally left and right.
  * <kbd>E</kbd> ascends vertically into the air (+Y).
  * <kbd>Q</kbd> descends toward the ground (-Y).
* **Mouse Look:**
  * Moving the mouse rotates the camera smoothly with pitch clamping ($\pm 89^\circ$) to prevent screen flipping.
* **Mouse Cursor Capture (<kbd>C</kbd>):**
  * When active, the mouse is captured for immersive 3D navigation.
  * Press <kbd>C</kbd> to free the mouse cursor if you want to click outside the window or interact with other applications.
* **Field-of-View Zoom (Mouse Wheel):**
  * Scroll up to zoom in on distant objects (such as the Torii gate or building roof ridges).
  * Scroll down to widen the field of view for panoramic scene inspection.
* **Camera Reset (<kbd>R</kbd>):**
  * Instantly returns the camera to coordinates `(0.0, 3.5, 26.0)` facing directly down the center of the festival street.

---

## 3. Camera Viewpoint Presets

Press the number keys to quickly showcase key areas to the evaluator:

1. **<kbd>1</kbd> — Festival Street Entrance:**
   * Places the camera at the street entrance.
   * Shows the full avenue lined with traditional Machiya townhouses, overhead rows of swinging lanterns, food stalls, and the distant Torii gate.
2. **<kbd>2</kbd> — Magic Show Stage Close-up:**
   * Positions the camera directly in front of the magic stage.
   * Perfectly frames the Magician, the floating Magic Orb in helical orbit around his hand, the Vanishing Box trick on the table, and the seated audience.
3. **<kbd>3</kbd> — Grand Torii Gate & Fireworks Sky:**
   * Angles the camera upward at the vermilion Torii shrine portal and sakura blossom tree to watch the fireworks burst in the night sky.

---

## 4. Environment & Animation Controls

* **Animation Pause / Freeze (<kbd>Space</kbd>):**
  * Freezes all motion across the entire scene mid-frame (lantern swings, orb orbit, falling sakura petals, takoyaki flipping, cloth rippling, crowd walking, and firework explosions).
  * Allows you to fly the camera around frozen geometry to verify exact transforms from any perspective.
  * Press <kbd>Space</kbd> again to resume continuous motion.
* **Smooth Day $\longleftrightarrow$ Night Transition (<kbd>N</kbd>):**
  * Initiates a smooth, gradual transition between daylight and festival nightlight:
    * **Sky Dome:** Blends between bright daytime cyan-blue and festival starry night indigo.
    * **Shoji Windows:** Fade from neutral white paper in daylight to warm glowing yellow-amber at night.
    * **Lanterns & Fireworks:** Become vibrant emissive light sources against the night sky.
* **Manual Firework Launch (<kbd>F</kbd>):**
  * Fires a single glowing shell rocket from ground level that arches upward and detonates into an expanding spherical burst of colorful particles falling under gravity.
  * *(Fireworks also launch automatically at periodic intervals when night mode is active).*
* **Cycle Shading Mode (<kbd>P</kbd>):**
  * Toggles between all 3 required shading configurations live in the fragment shader:
    1. **Full Blinn-Phong Shading:** Ambient + Diffuse (Lambert) + Specular (Halfway vector $\mathbf{H} = \frac{\mathbf{L}+\mathbf{V}}{\|\mathbf{L}+\mathbf{V}\|}$) across Directional Light, 12 Dynamic Point Lights, and Stage Spotlight.
    2. **Diffuse Only:** Ambient + Diffuse terms only (specular highlights disabled to demonstrate Lambertian reflectance).
    3. **Ambient Only:** Uniform ambient lighting only (flat shading baseline).
* **Toggle Textures (<kbd>X</kbd>):**
  * Globally toggles texture sampling on and off.
  * When ON, procedural high-res textures (`wood_timber.bmp`, `roof_tiles.bmp`, `stone_pavement.bmp`, `tatami_cloth.bmp`, `gold_leaf.bmp`, `sakura_bark.bmp`, `lantern_paper.bmp`) are mapped across the scene with UV tiling and modulated by base color.
  * When OFF, clean untextured base material colors are rendered for comparative evaluation.
* **Toggle Realistic Soft Shadows (<kbd>V</kbd>):**
  * Toggles **Directional Light Soft Shadow Mapping** with a 16-sample Percentage-Closer Filtering (PCF) kernel and slope-scaled depth bias:
    * **Depth Pass (2048x2048 FBO):** Renders scene depth from the dynamic Sun / Moon light viewpoint with front-face culling to eliminate peter-panning and shadow acne.
    * **16-Sample PCF Penumbra:** Softens shadow contours naturally across building walls, stall awnings, Torii crossbeams, and street pavement.
    * **Day/Night Sweep:** As the celestial sun/moon rotates with <kbd>N</kbd>, cast shadows dynamically lengthen, tilt, and soften across the street!
    * **Comparison Mode:** Press <kbd>V</kbd> to switch between realistic soft shadows and unshadowed lighting for presentation and evaluation.
* **Toggle Real-Time GPU Ray Tracing (<kbd>Z</kbd>):**
  * Instantly switches the entire rendering pipeline from standard rasterization to a full **Real-Time GPU Whitted Ray Tracer** running at 60+ FPS (`shaders/raytrace.frag`):
    * **Per-Pixel Primary Rays:** Cast from camera origin through view plane screen pixels.
    * **Analytical Primitives:** Tests ray intersection against mathematical planes (ground pavement), cylinders (Torii pillars, lantern poles, tree trunk), spheres (magic orb, hanging chochin lanterns, blossom canopy), and boxes (4 Machiya houses, roofs, shoji screens, food stalls, magic stage, golden byobu screen, and vanishing box).
    * **Analytical Hard Shadow Rays:** Traced from hit points to directional sun/moon and all dynamic point lights (magic orb, stall lanterns, fireworks flash).
    * **Multi-Bounce Recursive Specular Reflections:** Traces up to 3 reflection bounces across reflective surfaces:
      * **Magic Orb:** Crystal mirror reflection of stage and sky.
      * **Vanishing Box & Golden Byobu:** Metallic gold specular reflections.
      * **Magic Stage Floor:** Polished dark lacquer mirror reflection of magician and props.
      * **Wet Stone Pavement:** Reflects the warm glowing lanterns overhead!
  * Free fly camera navigation (<kbd>W</kbd>/<kbd>A</kbd>/<kbd>S</kbd>/<kbd>D</kbd> + Mouse) and Day/Night mode (<kbd>N</kbd>) continue to operate live in ray-traced mode!
* **Capture CPU Multi-Threaded Ray-Traced Snapshot (<kbd>F9</kbd>):**
  * Freezes the current frame and executes a high-fidelity CPU software ray tracer (`src/RayTracer.h`) across all CPU cores (`std::thread::hardware_concurrency()`).
  * Traces primary rays, analytical shadows, and 3 recursive reflection bounces.
  * Applies gamma correction ($2.2$) and exports the rendered image to **`raytraced_snapshot.bmp`** on disk.
  * Displays performance metrics (elapsed execution time, resolution, core count) in the console window.

---

## 5. Live In-Class Inspection & Defense Tool

Built specifically to satisfy the course requirement: *"Any object's position, rotation, and scale must be reachable and editable at runtime so that a specific object can be transformed on request relative to another object's reference frame without recompiling code."*

### How to Use
1. Observe the **Console Window** running beside the 3D graphics window.
2. Press **<kbd>T</kbd>** to cycle through the 10 registered inspectable objects.
3. The console prints:
   * **Node Name & Role**
   * **Local Position, Rotation (degrees), and Scale**
   * **Computed World Position**
   * **Parent Node Name in the Scene Graph**
4. Use the transform keys to modify the selected object live:
   * **<kbd>I</kbd> / <kbd>K</kbd>:** Translate Up / Down along local $\pm Y$.
   * **<kbd>J</kbd> / <kbd>L</kbd>:** Translate Left / Right along local $\pm X$.
   * **<kbd>U</kbd> / <kbd>O</kbd>:** Translate Forward / Backward along local $\pm Z$.
   * **<kbd>↑</kbd> / <kbd>↓</kbd> (Up / Down Arrows):** Rotate Pitch live.
   * **<kbd>←</kbd> / <kbd>→</kbd> (Left / Right Arrows):** Rotate Yaw live.
   * **<kbd>[</kbd> / <kbd>]</kbd>:** Scale down (shrink) or scale up (enlarge).

### The 15 Inspectable Objects & Academic Requirements

| # | Inspectable Object | Hierarchy / Academic Requirement Demonstrated |
|:---:|---|---|
| **1** | **Lantern Body** | **Child of Swinging Rope Pivot.** Directly demonstrates an object transformed relative to another object's moving reference frame. Moving this node offsets it while it continues to inherit the rope's pendulum swing. |
| **2** | **Lantern Rope Pivot** | **Parent Anchor Node.** Demonstrates rotational hierarchy: rotating this node swings the rope, lantern body, caps, and band as a single rigid chain. |
| **3** | **Magic Orb** | **Child of Magician's Hand Bone.** Demonstrates complex 3D helical motion ($r\cos t, y(t), r\sin t$) computed relative to a moving articulated character hand. |
| **4** | **Magician Figure** | **Character Root.** Full hierarchical character rig standing on the stage platform. |
| **5** | **Vanishing Box** | **Scale-to-Zero Transform.** Demonstrates dynamic continuous scaling to zero ($s \rightarrow 0$) followed by discrete positional teleportation and restoration. |
| **6** | **Spotlight Housing** | **Tracking Dynamic Rotation.** Cone lamp housing rotating in pitch and yaw to track the stage. |
| **7** | **Takoyaki Stall** | **Complex Multi-Part Model.** Assembly of stall counter, awning, griddle, and spinning/hopping takoyaki spheres. |
| **8** | **Kakigori Stall** | **Complex Multi-Part Model.** Assembly of stall counter, awning, vintage ice shaving machine with rotating flywheel, and 6 animated shaved ice dessert bowls. |
| **9** | **Torii Gate** | **Static Shrine Anchor.** Serves as the fixed reference architecture against which all moving objects are judged. |
| **10** | **Sakura Blossom Tree** | **Branched Hierarchy & Petal Kinematics.** Trunk, bough branches, blossom foliage clusters, and falling petals with sinusoidal drift. |
| **11** | **Crowd Walker #1** | **Articulated Walk Cycle.** Moving figure with translational pathing, vertical step bobbing, and alternating leg cycles. |
| **12** | **Machiya_L1** | **Interactive Townhouse Building.** Traditional architecture with sliding Shoji door, windows, and interior tatami/furniture (Left Front). |
| **13** | **Machiya_L2** | **Interactive Townhouse Building.** Second townhouse on left street flank (Left Rear). |
| **14** | **Machiya_R1** | **Interactive Townhouse Building.** Townhouse on right street flank with mirrored orientation (Right Front). |
| **15** | **Machiya_R2** | **Interactive Townhouse Building.** Second townhouse on right street flank (Right Rear). |

---

## 6. How to Manually Change / Remap Controls in Code

All keyboard, mouse, and camera controls are defined in **`Matsuri Nights — A Japanese Festival Street/Main.cpp`** and **`src/Camera.h`**.

### 1. Changing Continuous Keys (Camera Flight & Live Object Moves)
* **File:** `Main.cpp` inside `void processContinuousInput(GLFWwindow* window, Scene& scene)` (lines 140–179).
* Every key is checked each frame using `glfwGetKey(window, GLFW_KEY_...) == GLFW_PRESS`.

#### A. Remapping Camera Navigation Keys:
```cpp
// Change WASD / EQ to any key you prefer:
if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // Move Forward
    camera.ProcessKeyboard(FORWARD, deltaTime);
if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // Move Backward
    camera.ProcessKeyboard(BACKWARD, deltaTime);
if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // Strafe Left
    camera.ProcessKeyboard(LEFT, deltaTime);
if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // Strafe Right
    camera.ProcessKeyboard(RIGHT, deltaTime);
if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // Fly Up (+Y)
    camera.ProcessKeyboard(UP, deltaTime);
if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // Fly Down (-Y)
    camera.ProcessKeyboard(DOWN, deltaTime);
```

#### B. Changing Object Move & Rotate Speeds:
```cpp
// Increase or decrease movement/rotation speeds of the selected object:
float tSpeed = 2.5f * deltaTime;  // Translation speed (meters per second)
float rSpeed = 45.0f * deltaTime; // Rotation speed (degrees per second)
```

#### C. Remapping Selected Object Transform Keys:
```cpp
// Translation keys (currently I/K, J/L, U/O):
if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) // Move Up (+Y)
    scene.modifySelectedPosition(glm::vec3(0.0f, tSpeed, 0.0f));
if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) // Move Down (-Y)
    scene.modifySelectedPosition(glm::vec3(0.0f, -tSpeed, 0.0f));
if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) // Move Left (-X)
    scene.modifySelectedPosition(glm::vec3(-tSpeed, 0.0f, 0.0f));
if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // Move Right (+X)
    scene.modifySelectedPosition(glm::vec3(tSpeed, 0.0f, 0.0f));
if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // Move Forward (-Z)
    scene.modifySelectedPosition(glm::vec3(0.0f, 0.0f, -tSpeed));
if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) // Move Backward (+Z)
    scene.modifySelectedPosition(glm::vec3(0.0f, 0.0f, tSpeed));

// Rotation keys (currently Arrow Keys):
if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    // Pitch Up
    scene.modifySelectedRotation(glm::vec3(rSpeed, 0.0f, 0.0f));
if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  // Pitch Down
    scene.modifySelectedRotation(glm::vec3(-rSpeed, 0.0f, 0.0f));
if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  // Turn Left
    scene.modifySelectedRotation(glm::vec3(0.0f, rSpeed, 0.0f));
if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // Turn Right
    scene.modifySelectedRotation(glm::vec3(0.0f, -rSpeed, 0.0f));
```

---

### 2. Changing Single-Press Hotkeys & Triggers
* **File:** `Main.cpp` inside `void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)` (lines 182–250).
* Triggers once per press (`action == GLFW_PRESS`):

```cpp
// Change pause key:
if (key == GLFW_KEY_SPACE && g_Scene)
    g_Scene->togglePause();

// Change Day/Night toggle key:
if (key == GLFW_KEY_N && g_Scene)
    g_Scene->toggleDayNight();

// Change Firework rocket trigger:
if (key == GLFW_KEY_F && g_Scene)
    g_Scene->triggerFirework();

// Cycle Shading Mode:
if (key == GLFW_KEY_P && g_Scene)
    g_Scene->cycleShadingMode();

// Toggle Textures:
if (key == GLFW_KEY_X && g_Scene)
    g_Scene->toggleTextures();

// Toggle Real-Time GPU Ray Tracing:
if (key == GLFW_KEY_Z && g_Scene)
    g_Scene->toggleRayTracing();

// Capture & Export CPU Ray-Traced Snapshot to BMP:
if (key == GLFW_KEY_F9 && g_Scene)
{
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    g_Scene->captureCPURayTracedSnapshot(camera, w, h, "raytraced_snapshot.bmp");
}

// Change cycle inspectable key:
if (key == GLFW_KEY_T && g_Scene)
    g_Scene->cycleInspectable(1);

// Change mouse cursor lock/unlock key:
if (key == GLFW_KEY_C)
    mouseLookActive = !mouseLookActive;

// Scale selected object (+ / - keys, keypad + / -, and brackets [ / ]):
if ((key == GLFW_KEY_EQUAL || key == GLFW_KEY_KP_ADD || key == GLFW_KEY_RIGHT_BRACKET) && g_Scene)
    g_Scene->modifySelectedScale(1.1f); // Enlarge by 10%
if ((key == GLFW_KEY_MINUS || key == GLFW_KEY_KP_SUBTRACT || key == GLFW_KEY_LEFT_BRACKET) && g_Scene)
    g_Scene->modifySelectedScale(0.9f); // Shrink by 10%

// Change Camera Reset key:
if (key == GLFW_KEY_R) {
    camera.Position = glm::vec3(0.0f, 3.5f, 26.0f);
    camera.Yaw = -90.0f;
    camera.Pitch = -2.0f;
}
```

#### How to Add a Brand New Custom Hotkey:
Simply add a new `if` block inside `key_callback`:
```cpp
if (key == GLFW_KEY_P && g_Scene)
{
    std::cout << "[Custom Key] Pressed P!" << std::endl;
    // Call any function you want, e.g.:
    // g_Scene->togglePause();
}
```

---

### 3. Changing Camera Speed, Mouse Sensitivity, & FOV
* **File:** `src/Camera.h` (lines 16–20):
```cpp
const float YAW         = -90.0f; // Default facing direction (-Z)
const float PITCH       =  0.0f;  // Default horizontal gaze
const float SPEED       = 12.0f;  // Camera fly speed (increase to fly faster)
const float SENSITIVITY =  0.1f;  // Mouse look sensitivity (lower = smoother)
const float ZOOM        = 45.0f;  // Default Field of View (FOV) in degrees
```

---

### 4. Common GLFW Key Codes Reference

To remap any key, replace `GLFW_KEY_...` with any of the following standard GLFW constants:

| Key Category | Available GLFW Identifiers |
|---|---|
| **Alphabet Letters** | `GLFW_KEY_A` to `GLFW_KEY_Z` |
| **Number Row** | `GLFW_KEY_0` to `GLFW_KEY_9` |
| **Numpad** | `GLFW_KEY_KP_0` to `GLFW_KEY_KP_9`, `GLFW_KEY_KP_ADD`, `GLFW_KEY_KP_SUBTRACT` |
| **Arrow Keys** | `GLFW_KEY_UP`, `GLFW_KEY_DOWN`, `GLFW_KEY_LEFT`, `GLFW_KEY_RIGHT` |
| **Function Keys** | `GLFW_KEY_F1` to `GLFW_KEY_F12` |
| **Punctuation / Symbols** | `GLFW_KEY_SPACE`, `GLFW_KEY_TAB`, `GLFW_KEY_ENTER`, `GLFW_KEY_ESCAPE`<br>`GLFW_KEY_LEFT_BRACKET` (`[`), `GLFW_KEY_RIGHT_BRACKET` (`]`)<br>`GLFW_KEY_MINUS` (`-`), `GLFW_KEY_EQUAL` (`=`)<br>`GLFW_KEY_COMMA` (`,`), `GLFW_KEY_PERIOD` (`.`), `GLFW_KEY_SLASH` (`/`) |
| **Modifiers** | `GLFW_KEY_LEFT_SHIFT`, `GLFW_KEY_LEFT_CONTROL`, `GLFW_KEY_LEFT_ALT` |

---

### 5. Compiling and Applying Your Control Changes

After changing any key binding in `Main.cpp` or speed in `src/Camera.h`:

1. **In Visual Studio:**
   * Press **<kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>B</kbd>** to recompile.
   * Press **<kbd>Ctrl</kbd> + <kbd>F5</kbd>** to run.
2. **In Terminal / PowerShell:**
   ```powershell
   & "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe" "Matsuri Nights — A Japanese Festival Street\Matsuri Nights — A Japanese Festival Street.vcxproj" /p:Configuration=Debug /p:Platform=x64
   ```

---

## 7. Interactive House Visiting, Collision & Celestial Sky Guide

### A. Visiting the Traditional Machiya Houses
* **Interactive Shoji Doors (<kbd>H</kbd>):**
  * Walk up to any of the 4 Machiya townhouse entrances along the street.
  * Press **<kbd>H</kbd>** to smoothly slide the front Shoji lattice door open or closed.
  * Proximity auto-open: Standing right at the entrance threshold also slides the door open for seamless entry.
* **Ground Floor Living & Tea Room (*Zashiki*):**
  * Step over the stone Genkan entryway and wooden threshold (*Agari-kamachi*).
  * Explore the living room featuring woven Tatami floor mats, traditional low wooden floor table (*Chabudai*), 4 silk floor cushions (*Zabuton* in crimson and indigo), authentic ceramic green-tea teapot (*Kyusu*), 2 celadon teacups (*Yunomi*) on a bamboo serving tray, a decorative hanging wall scroll (*Kakemono*), and glowing paper floor lantern (*Andon*).
* **Ascending the Wooden Staircase (*Kaidan*):**
  * Walk into the wooden staircase along the wall; collision detection automatically steps the camera up each of the 9 timber steps, leading straight up to the second floor!
* **Second Floor Bedroom (*Shinshitsu*):**
  * Arrive on the upper tatami floor with its protective stairwell balustrade.
  * Look out of the 3 Shoji windows over the lively festival street and swinging lanterns below.
  * Inspect the traditional Japanese **Futon bed** (white mattress, folded crimson/gold duvet quilt, navy buckwheat pillow), antique stepped chest (*Tansu*), and bedside glowing paper lamp.

### B. Wall Collision System (<kbd>B</kbd>)
* **Solid Wall Walk Mode (Default: ON):**
  * Prevents clipping or passing through exterior house walls, roofs, or interior barriers.
  * Enforces entering and exiting strictly through the front doorway when the door is open.
* **Toggle Noclip (<kbd>B</kbd>):**
  * Press **<kbd>B</kbd>** to switch between Solid Wall Walk Mode and free Noclip Fly Mode.

### C. Celestial Sun, Moon, and Twinkling Night Stars
* **Infinite Position Celestial Projection:**
  * Rendered at astronomical infinity with zero translational parallax.
  * **The Sun:** Radiant golden-white solar disk with glowing atmospheric corona bloom.
  * **The Moon:** Silvery lunar disk with procedural crater maria and soft nocturnal aura.
  * **Night Stars:** Hundreds of sparkling, twinkling stars with scintillation and atmospheric horizon extinction that emerge dynamically as twilight falls into festival night (<kbd>N</kbd>).

