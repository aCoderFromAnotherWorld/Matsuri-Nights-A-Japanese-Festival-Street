# Matsuri Nights — Color Customization & Shading Guide
### CSE4102: Computer Graphics and Image Processing Laboratory
**Project:** Matsuri Nights — A Japanese Festival Street  
**Student:** MD. Abu Hasanat Soykot | **Roll:** 2107100 | **Group:** B2  

---

## 1. How Color Works in the Engine

Every visual mesh in the project belongs to a `SceneNode` in the hierarchical scene graph. Coloring is managed through a normalized floating-point system coupled with custom OpenGL shaders:

### A. Normalized RGBA Vectors
* Colors are represented using `glm::vec4(R, G, B, A)` where each channel ranges from `0.0f` to `1.0f`:
  ```cpp
  node->color = glm::vec4(R, G, B, 1.0f);
  ```
* **Conversion from Standard 0–255 RGB:**
  $$\text{Normalized Value} = \frac{\text{Color (0–255)}}{255.0\text{f}}$$
  * *Example:* Pure Red $(255, 0, 0) \longrightarrow$ `glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)`
  * *Example:* Forest Green $(34, 139, 34) \longrightarrow$ `glm::vec4(34/255.0f, 139/255.0f, 34/255.0f, 1.0f)`

### B. Shading Pipeline & Lighting
* Base colors are uploaded to the GPU as `uniform vec4 objectColor;` in `shaders/basic.frag` (and embedded in `src/Shader.h`).
* The fragment shader computes directional sunlight/moonlight diffuse shading and ambient lighting to give 3D depth to every primitive:
  $$\text{Lighting} = \text{Ambient} + \max(\mathbf{N} \cdot \mathbf{L}, 0.0) \times \text{LightIntensity}$$
  $$\text{FinalColor} = \text{objectColor.rgb} \times \text{Lighting}$$

### C. Special Material Modes
1. **Emissive Objects (`isEmissive = true`):**
   * Used for the **Chochin Lanterns**, **Magic Orb**, **Spotlight Lens**, and **Fireworks**.
   * Bypasses shadow/diffuse calculation and radiates self-illuminated light that intensifies at night:
     ```cpp
     node->isEmissive = true;
     node->emissiveColor = glm::vec3(R, G, B);
     ```
2. **Translucent Shoji Screens (`isWindow = true`):**
   * Used for townhouse doors and second-floor shoji windows.
   * Dynamically blends from neutral white daylight paper `(0.90, 0.88, 0.82)` to warm golden-amber interior glow `(1.0, 0.82, 0.45)` when toggling Day/Night with <kbd>N</kbd>.
3. **Procedural Sky Dome (`isSky = true`):**
   * Computes a vertical gradient from horizon to zenith, smoothly interpolating between clear day cyan-blue and starry festival night indigo.

---

## 2. Quick Color Reference Palette

| Color Name | Standard RGB (0–255) | Normalized GLM Vector |
|---|:---:|---|
| **Shrine Vermilion Red** | `(217, 56, 31)` | `glm::vec4(0.85f, 0.22f, 0.12f, 1.0f)` |
| **Traditional Timber Wood** | `(92, 56, 33)` | `glm::vec4(0.36f, 0.22f, 0.13f, 1.0f)` |
| **Dark Ebonized Wood** | `(46, 28, 15)` | `glm::vec4(0.18f, 0.11f, 0.06f, 1.0f)` |
| **Roof Slate Charcoal** | `(41, 43, 51)` | `glm::vec4(0.16f, 0.17f, 0.20f, 1.0f)` |
| **Pavement Stone Grey** | `(89, 89, 94)` | `glm::vec4(0.35f, 0.35f, 0.37f, 1.0f)` |
| **Sakura Blossom Pink** | `(250, 184, 209)` | `glm::vec4(0.98f, 0.72f, 0.82f, 1.0f)` |
| **Festival Awning Cyan** | `(51, 166, 224)` | `glm::vec4(0.20f, 0.65f, 0.88f, 1.0f)` |
| **Indigo Festival Blue** | `(41, 56, 115)` | `glm::vec4(0.16f, 0.22f, 0.45f, 1.0f)` |
| **Golden Yellow / Brass** | `(224, 191, 51)` | `glm::vec4(0.88f, 0.75f, 0.20f, 1.0f)` |
| **Matcha Moss Green** | `(77, 140, 89)` | `glm::vec4(0.30f, 0.55f, 0.35f, 1.0f)` |
| **Crimson Red Velvet** | `(217, 38, 46)` | `glm::vec4(0.85f, 0.15f, 0.18f, 1.0f)` |
| **Translucent Shoji Paper** | `(235, 224, 204)` | `glm::vec4(0.92f, 0.88f, 0.80f, 1.0f)` |
| **Pure White** | `(255, 255, 255)` | `glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)` |
| **Pure Black** | `(0, 0, 0)` | `glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)` |

---

## 3. Where to Manually Change Each Object's Color

All object colors are located inside **`Matsuri Nights — A Japanese Festival Street/src/Objects.h`**.

### 1. Torii Shrine Gate
* **File:** `src/Objects.h` (around line 340)
* **Code to edit:**
```cpp
glm::vec4 vermilion(0.85f, 0.22f, 0.12f, 1.0f); // Main pillars & crossbeams
glm::vec4 black(0.12f, 0.12f, 0.14f, 1.0f);     // Central tablet strut (Gakuzuka)
glm::vec4 stone(0.45f, 0.45f, 0.48f, 1.0f);     // Pedestal bases (Kamebara)
```

### 2. Traditional Machiya Townhouses & Front Doors
* **File:** `src/Objects.h` (around lines 94–105)
* **Code to edit:**
```cpp
glm::vec4 timber(0.36f, 0.22f, 0.13f, 1.0f);     // House wall body
glm::vec4 darkWood(0.18f, 0.11f, 0.06f, 1.0f);   // Door frame, lattice ribs, window frame
glm::vec4 roofSlate(0.16f, 0.17f, 0.20f, 1.0f);  // Pitch roof tiles & door canopies
glm::vec4 paperColor(0.92f, 0.88f, 0.80f, 1.0f); // Shoji screen paper
```
* **Front Door Noren Curtain:** (around line 239)
```cpp
noren->color = glm::vec4(0.16f, 0.22f, 0.45f, 1.0f); // Indigo dyed entrance curtain
```
* **Front Stone Step (*Kutsunugi-ishi*):** (around line 222)
```cpp
stoneStep->color = stone; // Threshold stone step
```

### 3. Chochin Hanging Paper Lanterns
* **File:** `src/Objects.h` (around lines 570–585)
* **Code to edit:**
```cpp
paper->color = glm::vec4(0.92f, 0.18f, 0.12f, 1.0f);   // Exterior red paper
paper->emissiveColor = glm::vec3(1.0f, 0.35f, 0.15f);   // Warm amber inner light
band->color = glm::vec4(0.95f, 0.92f, 0.85f, 1.0f);    // White festival kanji band
capTop->color = glm::vec4(0.12f, 0.12f, 0.12f, 1.0f);  // Top black cap
capBot->color = glm::vec4(0.12f, 0.12f, 0.12f, 1.0f);  // Bottom black cap
```

### 4. Takoyaki Food Stall
* **File:** `src/Objects.h` (around lines 630–640)
* **Code to edit:**
```cpp
glm::vec4 wood(0.42f, 0.28f, 0.18f, 1.0f);  // Stall counter base and corner poles
glm::vec4 red(0.88f, 0.20f, 0.15f, 1.0f);   // Slanted awning roof
glm::vec4 white(0.92f, 0.90f, 0.88f, 1.0f); // Front hanging Noren banner
```
* **Grill Plate & Takoyaki Balls:** (around lines 685–705)
```cpp
grill->color = glm::vec4(0.14f, 0.14f, 0.15f, 1.0f);    // Cast iron grill plate
ballNode->color = glm::vec4(0.85f, 0.62f, 0.28f, 1.0f); // Golden browned octopus ball
```

### 5. Kakigori (Shaved Ice) Food Stall
* **File:** `src/Objects.h` (around lines 765–775)
* **Code to edit:**
```cpp
glm::vec4 cyan(0.20f, 0.65f, 0.88f, 1.0f);  // Blue-cyan awning & ice machine
glm::vec4 white(0.94f, 0.94f, 0.96f, 1.0f); // Noren banner
```
* **Shaved Ice Bowl & Syrup:** (around lines 830–845)
```cpp
bowl->color = glm::vec4(0.9f, 0.95f, 1.0f, 1.0f);      // Ceramic bowl
iceMound->color = glm::vec4(0.20f, 0.85f, 0.95f, 1.0f); // Blue Hawaii syrup
```
* **Nobori Banner Flag:** (around line 855)
```cpp
noboriFlag->color = cyan; // Fluttering vertical festival banner
```

### 6. Magic Show Stage & Props
* **File:** `src/Objects.h` (around lines 1015–1045)
* **Code to edit:**
```cpp
deck->color   = glm::vec4(0.28f, 0.18f, 0.12f, 1.0f); // Dark stage wood platform
carpet->color = glm::vec4(0.85f, 0.15f, 0.18f, 1.0f); // Red festive stage carpet
screen->color = glm::vec4(0.88f, 0.78f, 0.42f, 1.0f); // Gold-leaf folding Byobu screen
```
* **Floating Magic Orb:** (around line 1125)
```cpp
orbNode->color = glm::vec4(0.3f, 0.85f, 1.0f, 1.0f);
orbNode->emissiveColor = glm::vec3(0.3f, 0.9f, 1.0f); // Mystical blue-cyan aura
```
* **Vanishing Box & Silk Cloth:** (around lines 1205–1215)
```cpp
boxNode->color   = glm::vec4(0.88f, 0.72f, 0.15f, 1.0f); // Gold leaf treasure box
clothNode->color = glm::vec4(0.85f, 0.15f, 0.25f, 1.0f); // Crimson silk cover
```

### 7. Spectator Benches & Audience Group
* **File:** `src/Objects.h` (around lines 1360–1375)
* **Code to edit:**
```cpp
glm::vec4 benchWood(0.35f, 0.22f, 0.14f, 1.0f); // Wooden bench legs and plank
glm::vec4 redFelt(0.82f, 0.16f, 0.18f, 1.0f);   // Traditional Mousen crimson felt
glm::vec4 skin(0.92f, 0.76f, 0.64f, 1.0f);      // Character skin tone
glm::vec4 darkHair(0.12f, 0.12f, 0.14f, 1.0f);  // Hair and topknot

// Colors assigned across seated spectators:
std::vector<glm::vec4> yukataColors = {
    { 0.20f, 0.45f, 0.65f, 1.0f }, // Indigo blue
    { 0.78f, 0.25f, 0.35f, 1.0f }, // Cherry blossom red
    { 0.30f, 0.55f, 0.35f, 1.0f }, // Matcha green
    { 0.85f, 0.65f, 0.20f, 1.0f }, // Golden yellow
    { 0.48f, 0.30f, 0.62f, 1.0f }, // Purple
    { 0.25f, 0.55f, 0.58f, 1.0f }  // Turquoise teal
};
```

### 8. Characters & Figures
* **Shop Vendors:** `src/Scene.h` (lines 117–123):
  * Takoyaki Vendor Happi Coat: `glm::vec4(0.18f, 0.35f, 0.75f, 1.0f)` (Royal blue)
  * Kakigori Vendor Happi Coat: `glm::vec4(0.85f, 0.25f, 0.22f, 1.0f)` (Bright red)
* **Magician:** `src/Objects.h` (around line 1055):
  * Wizard Robe: `glm::vec4(0.15f, 0.12f, 0.22f, 1.0f)` (Midnight indigo)
  * Wand Trim: `glm::vec4(0.92f, 0.80f, 0.25f, 1.0f)` (Polished gold)
* **Crowd Walkers:** `src/Objects.h` (around line 1475):
  * `yukataTones` list provides 8 distinct traditional Japanese yukata color palettes for walking pedestrians.

### 9. Sakura Blossom Tree
* **File:** `src/Objects.h` (around lines 425–445)
* **Code to edit:**
```cpp
glm::vec4 bark(0.32f, 0.20f, 0.14f, 1.0f);    // Trunk & branch wood
glm::vec4 blossom(0.98f, 0.72f, 0.82f, 1.0f); // Foliage crown & falling petals
```

### 10. Street Pavement & Earth Ground
* **File:** `src/Objects.h` (around lines 49–73)
* **Code to edit:**
```cpp
street->color  = glm::vec4(0.35f, 0.35f, 0.37f, 1.0f); // Stone road pavement
borderL->color = glm::vec4(0.22f, 0.22f, 0.24f, 1.0f); // Road curb stone border
dirt->color    = glm::vec4(0.18f, 0.24f, 0.16f, 1.0f); // Surrounding mossy ground
```

### 11. Sky Dome & Lighting
* **File:** `shaders/basic.frag` (lines 21–22) and `src/Shader.h` (lines 113–114)
* **Day Sky Gradient:**
```glsl
vec3 daySky = mix(vec3(0.55, 0.75, 0.98), vec3(0.82, 0.88, 0.98), heightRatio);
```
* **Night Sky Gradient:**
```glsl
vec3 nightSky = mix(vec3(0.02, 0.03, 0.08), vec3(0.06, 0.08, 0.18), heightRatio);
```

---

## 4. How to Compile & Verify Your Color Changes

After modifying any color values in `src/Objects.h` or shaders:

1. **Using Visual Studio:**
   * Press **<kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>B</kbd>** to build.
   * Press **<kbd>Ctrl</kbd> + <kbd>F5</kbd>** to launch without debugging.

2. **Using Developer PowerShell:**
   Run the MSBuild command from the workspace folder:
   ```powershell
   & "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe" "Matsuri Nights — A Japanese Festival Street\Matsuri Nights — A Japanese Festival Street.vcxproj" /p:Configuration=Debug /p:Platform=x64
   ```
   Then launch:
   ```powershell
   & "Matsuri Nights — A Japanese Festival Street\x64\Debug\Matsuri Nights — A Japanese Festival Street.exe"
   ```
