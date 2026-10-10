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

### B. Shading Pipeline & Blinn-Phong Illumination Model (Phase 2)
* The fragment shader (`shaders/basic.frag`) implements the **Blinn-Phong Reflection Model** evaluating Ambient, Lambertian Diffuse, and Specular terms using the halfway vector $\mathbf{H}$:
  $$\mathbf{H} = \frac{\mathbf{L} + \mathbf{V}}{\|\mathbf{L} + \mathbf{V}\|}$$
  $$\mathbf{I}_{\text{specular}} = \mathbf{L}_{\text{specular}} \times k_s \times (\max(\mathbf{N} \cdot \mathbf{H}, 0))^{\alpha}$$
  where $\alpha$ is the material shininess exponent (`shininess`) and $k_s$ is the specular reflectivity coefficient (`specularStrength`).
* **Active Scene Light Sources:**
  1. **Directional Light (Sun / Moon):** Sweeps across the sky; sunlight is warm golden-white `(1.0, 0.95, 0.8)` during day; moon is cold silvery indigo `(0.15, 0.20, 0.35)` at night.
  2. **12 Dynamic Point Lights:**
     * **Point Light 0 (Magic Orb):** Cyan-blue arcane glow `(0.2, 0.6, 1.0)` orbiting the magician.
     * **Point Light 1 (Takoyaki Stall Lantern):** Warm amber glow `(1.0, 0.6, 0.2)` illuminating food stall counters.
     * **Point Light 2 (Kakigori Stall Lantern):** Rose-magenta glow `(1.0, 0.4, 0.6)` on shaved ice syrups.
     * **Point Light 3 & 4 (Swinging Overhead Lanterns):** Warm vermilion lanterns `(1.0, 0.45, 0.15)` following rope physics.
     * **Point Light 5 (Sky Fireworks Flash):** Dynamic bursts detonating in brilliant sky colors with quadratic distance falloff.
     * **Point Light 6 (Machiya L1 Ground Living Room):** Warm golden-amber interior illumination `(1.4, 1.05, 0.65)`.
     * **Point Light 7 (Machiya L1 Second Floor Bedroom):** Soft warm amber chamber lighting `(1.35, 1.0, 0.60)`.
     * **Point Light 8 (Machiya R1 Ground Living Room):** Warm golden-amber interior illumination `(1.4, 1.05, 0.65)`.
     * **Point Light 9 (Machiya R1 Second Floor Bedroom):** Soft warm amber chamber lighting `(1.35, 1.0, 0.60)`.
     * **Point Light 10 (Machiya L2 Living Room):** Cozy domestic interior glow `(1.2, 0.9, 0.55)`.
     * **Point Light 11 (Machiya R2 Living Room):** Cozy domestic interior glow `(1.2, 0.9, 0.55)`.
  3. **Stage Spotlight:**
     * Conical spotlight mounted above the stage housing tracking the magician.
     * Inner cutoff angle $\cos(15^\circ)$ and outer cutoff angle $\cos(20^\circ)$ for smooth penumbra falloff.
  4. **Indoor Indirect Skylight Bounce:**
     * Secondary photon bounce through Shoji windows and doors (`indoorBounce = mix(0.48, 0.18, dayNightFactor)`), ensuring rooms are vibrantly illuminated without dark shadow blackouts.
  5. **Interactive Festival Lighting Toggle (<kbd>0</kbd> / <kbd>NumPad 0</kbd>):**
     * Dynamically toggles all festival street lanterns, stall lighting, and decorative point lights on and off for live lighting comparison.
     * *(Note: Remapped from <kbd>L</kbd> to <kbd>0</kbd> / <kbd>KP_0</kbd> to avoid conflicting with continuous <kbd>J</kbd>/<kbd>L</kbd> object translation).*
* **Realistic Soft Shadow Mapping (PCF Filtered):**
  * **Depth Pass:** Uses a $2048 \times 2048$ resolution depth framebuffer (`depthMapFBO`) to capture orthographic light-space depth values across the entire festival promenade.
  * **Adaptive Depth Bias:** Applies a slope-scaled normal bias $\text{bias} = \max(0.0035 \times (1.0 - \mathbf{N} \cdot \mathbf{L}), 0.0006)$ eliminating surface shadow acne while preserving crisp shadow contact.
  * **16-Sample PCF Penumbra:** Softens shadow contours naturally across building walls, stall awnings, and cobblestones:
    $$\text{shadow} = \frac{1}{16} \sum_{x=-1}^{2} \sum_{y=-1}^{2} (\text{projCoords.z} - \text{bias} > \text{depthMap}(\text{projCoords.xy} + \text{offset}(x,y)))$$
  * **Illumination Modulation:** Direct sunlight and moonlight terms are modulated by $(1.0 - \text{shadow})$, while ambient sky illumination remains intact, guaranteeing realistic, naturally illuminated shadows.
  * **Shadow Toggle (<kbd>V</kbd>):** Dynamically toggles soft shadow mapping on and off for live comparative evaluation.

### C. Texture Mapping Pipeline (Phase 3)
* Textures are generated procedurally as valid 24-bit uncompressed `.bmp` files on disk in `assets/textures/` via `src/TextureGenerator.h` and loaded into OpenGL using `stb_image.h` (`src/Texture.h`).
* Texture sampling is modulated directly with the object's base color:
  $$\text{Effective Diffuse} = \text{baseColor} \times \text{textureColor}(\mathbf{uv} \cdot \text{tiling})$$
* **Texture Toggle (<kbd>X</kbd>):** Can disable texture sampling dynamically to compare shaded untextured polygons against textured surfaces.

### D. Ray Tracing Pipeline & Whitted Optics (Advanced Feature)
* The project includes a dual-engine **Ray Tracing System**:
  1. **Real-Time GPU Ray Tracer (<kbd>Z</kbd>):** Executed per-fragment on a full-screen quad at 60+ FPS (`shaders/raytrace.frag`).
  2. **Multi-Threaded CPU Software Ray Tracer (<kbd>F9</kbd>):** Executed across all CPU hardware threads exporting high-resolution rendered stills to `raytraced_snapshot.bmp` (`src/RayTracer.h`).
* **Analytical Ray-Primitive Intersections:**
  * **Spheres:** Quadratic discriminant solution $\|\mathbf{P} - \mathbf{C}\|^2 = r^2$.
  * **Boxes / Slabs:** Kay-Kajiya bounding interval slab tests for houses, roofs, stalls, stage, and vanishing box.
  * **Cylinders:** Vertical $Y$-cylinder equations for Torii shrine pillars, cedar street poles, and tree trunk.
  * **Planes:** Ground plane with procedural cobblestone paver and mortar calculation.
* **Analytical Shadow Rays:**
  * From any intersection point $\mathbf{P} + \epsilon \mathbf{N}$, a shadow ray is shot toward active light sources (directional sun/moon, magic orb, food stall lanterns, and fireworks). If occluded by geometry, diffuse and specular illumination are zeroed out (casting true physical hard shadows).
* **Multi-Bounce Recursive Reflections:**
  * Evaluates up to 3 recursive bounces using reflection direction:
    $$\mathbf{R} = \text{reflect}(\mathbf{D}, \mathbf{N})$$
  * **Surface Reflectivity ($k_r$):**
    * **Magic Orb ($k_r = 0.85$):** Highly reflective crystalline arcane sphere.
    * **Vanishing Box ($k_r = 0.60$):** Gold leaf foil reflection.
    * **Golden Byobu Screen ($k_r = 0.65$):** Gilded folding screen mirror.
    * **Magic Stage Floor ($k_r = 0.40$):** Polished dark lacquer wood floor reflecting magician and orb.
    * **Wet Stone Pavement ($k_r = 0.22$):** Nighttime festival cobblestone reflecting overhead red lanterns!

### E. Special Material Modes
1. **Emissive Objects (`isEmissive = true`):**
   * Used for the **Chochin Lanterns**, **Magic Orb**, **Spotlight Lens**, and **Fireworks**.
   * Radiates self-illuminated light that intensifies at night:
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
| **Andon Flame Glow (Emissive)** | `(255, 204, 102)` | `glm::vec3(2.20f, 1.60f, 0.80f)` |
| **Celadon Jade Ceramic** | `(122, 163, 148)` | `glm::vec4(0.48f, 0.64f, 0.58f, 1.0f)` |
| **Cobalt Glaze Porcelain** | `(46, 71, 133)` | `glm::vec4(0.18f, 0.28f, 0.52f, 1.0f)` |
| **Camellia Crimson Bloom** | `(224, 41, 56)` | `glm::vec4(0.88f, 0.16f, 0.22f, 1.0f)` |
| **Peony Rose Pink Bloom** | `(245, 166, 199)` | `glm::vec4(0.96f, 0.65f, 0.78f, 1.0f)` |
| **Iris Imperial Violet** | `(140, 64, 178)` | `glm::vec4(0.55f, 0.25f, 0.70f, 1.0f)` |
| **Bonsai Pine Needle Green** | `(46, 107, 56)` | `glm::vec4(0.18f, 0.42f, 0.22f, 1.0f)` |
| **Kokedama Forest Moss** | `(71, 133, 56)` | `glm::vec4(0.28f, 0.52f, 0.22f, 1.0f)` |
| **Byoubu Gold Leaf Foil** | `(235, 199, 56)` | `glm::vec4(0.92f, 0.78f, 0.22f, 1.0f)` |
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

### 2. Traditional Machiya Townhouses (Interior & Exterior)
* **File:** `src/Objects.h` inside `MachiyaBuilding`
* **Exterior & Structural Frame:**
```cpp
glm::vec4 timber(0.36f, 0.22f, 0.13f, 1.0f);     // House wall body & timber siding
glm::vec4 darkWood(0.18f, 0.11f, 0.06f, 1.0f);   // Door frame, lattice ribs, window frames
glm::vec4 roofSlate(0.16f, 0.17f, 0.20f, 1.0f);  // Pitch roof tiles & door canopies
glm::vec4 paperColor(0.92f, 0.88f, 0.80f, 1.0f); // Shoji screen paper
```
* **Interior Rooms & Furnishings:**
```cpp
// Tatami floor mats (living room & upper bedroom):
tatamiFloor->color = glm::vec4(0.80f, 0.78f, 0.60f, 1.0f);
// Low Japanese Chabudai table:
chabudaiTable->color = glm::vec4(0.16f, 0.09f, 0.05f, 1.0f); // dark lacquer mahogany
// Silk Zabuton floor cushions:
zabutonCrimson->color = glm::vec4(0.78f, 0.16f, 0.16f, 1.0f); // festival crimson silk
zabutonIndigo->color  = glm::vec4(0.18f, 0.26f, 0.55f, 1.0f); // deep indigo blue silk
// Authentic Japanese green-tea Kyusu teapot:
teapot->color = glm::vec4(0.18f, 0.16f, 0.16f, 1.0f);         // dark clay ceramic
// Celadon green Yunomi teacups:
teacup->color = glm::vec4(0.38f, 0.58f, 0.48f, 1.0f);         // celadon jade
// Standing Andon room lantern (warm night light):
andon->emissiveColor = glm::vec3(1.40f, 1.10f, 0.50f);
// Second floor Futon bed:
futonBase->color   = glm::vec4(0.95f, 0.94f, 0.90f, 1.0f);    // ivory cotton mattress
futonQuilt->color  = glm::vec4(0.75f, 0.14f, 0.14f, 1.0f);    // crimson festival pattern
futonPillow->color = glm::vec4(0.14f, 0.18f, 0.38f, 1.0f);    // navy silk
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

### 4. Takoyaki Food Stall & Authentic Food Details
* **File:** `src/Objects.h` inside `TakoyakiStall`
* **Stall Structure:**
```cpp
glm::vec4 wood(0.42f, 0.28f, 0.18f, 1.0f);  // Stall counter base and corner poles
glm::vec4 red(0.88f, 0.20f, 0.15f, 1.0f);   // Slanted awning roof
glm::vec4 white(0.92f, 0.90f, 0.88f, 1.0f); // Front hanging Noren banner
```
* **Cast Iron Grill Plate & Authentic Takoyaki Food:**
```cpp
grill->color = glm::vec4(0.14f, 0.14f, 0.15f, 1.0f);    // Dimpled cast-iron grill plate
// Fried Takoyaki Batter Ball:
ballNode->color = glm::vec4(0.92f, 0.72f, 0.38f, 1.0f); // Golden browned crispy batter
ballNode->shininess = 64.0f; ballNode->specularStrength = 0.65f;
// Glossy Dark Savory Takoyaki Sauce (Otafuku Sauce):
sauce->color = glm::vec4(0.18f, 0.08f, 0.03f, 1.0f);    // Deep sweet soy glaze
sauce->shininess = 72.0f; sauce->specularStrength = 0.85f;
// Creamy Japanese Kewpie Mayonnaise:
mayo->color = glm::vec4(0.96f, 0.94f, 0.84f, 1.0f);     // Pale-cream egg mayo
// Emerald Green Aonori (dried seaweed flakes):
aonori->color = glm::vec4(0.10f, 0.50f, 0.16f, 1.0f);   // Seaweed flake sprinkles
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

### 12. Traditional House Interior Lamps (Andon & Pendant Chandelier)
* **File:** `src/Objects.h` (in `createAndonFloorLamp` and `createCeilingPendantLamp`)
* **Code to edit:**
```cpp
// Andon Floor Lamp:
glm::vec4 frameWood(0.24f, 0.14f, 0.08f, 1.0f);     // Cedar legs, posts, kumiko ribs
glm::vec4 washiPaper(0.94f, 0.90f, 0.82f, 1.0f);    // Translucent rice paper diffuser
flameCore->emissiveColor = glm::vec3(2.2f, 1.6f, 0.8f); // Warm glowing oil flame core

// Ceiling Pendant Chandelier:
glm::vec4 pendantPaper(0.96f, 0.92f, 0.84f, 1.0f);  // Multi-tier octagonal washi shade
pendantBulb->emissiveColor = glm::vec3(2.8f, 2.1f, 1.1f); // High-lumen warm chamber core
```

### 13. Ikebana Flower Vases & Asymmetrical Floral Arrangements
* **File:** `src/Objects.h` (in `createIkebanaVase`)
* **Code to edit:**
```cpp
// Vase Ceramic Glazes (based on vaseStyle index 0, 1, 2):
glm::vec4 celadonJade(0.48f, 0.64f, 0.58f, 1.0f);   // Pale celadon crackle jade
glm::vec4 cobaltGlaze(0.18f, 0.28f, 0.52f, 1.0f);   // Deep imperial cobalt blue
glm::vec4 earthyClay(0.42f, 0.35f, 0.28f, 1.0f);    // Bizen unglazed stoneware

// Floral Blooms & Foliage:
glm::vec4 camelliaRed(0.88f, 0.16f, 0.22f, 1.0f);   // Crimson Camellia petals
glm::vec4 peonyPink(0.96f, 0.65f, 0.78f, 1.0f);     // Rose-pink Peony petals
glm::vec4 plumYellow(0.98f, 0.82f, 0.22f, 1.0f);    // Golden Plum Blossom petals
glm::vec4 irisViolet(0.55f, 0.25f, 0.70f, 1.0f);    // Imperial Violet Iris petals
glm::vec4 stamenGold(0.95f, 0.82f, 0.20f, 1.0f);    // Golden flower stamen center
glm::vec4 stemGreen(0.24f, 0.44f, 0.20f, 1.0f);     // Slender green stem & leaves
```

### 14. Exterior Window Planters & Blooming Flora
* **File:** `src/Objects.h` (in `createWindowPlanterBox`)
* **Code to edit:**
```cpp
glm::vec4 cedarBox(0.38f, 0.24f, 0.14f, 1.0f);      // Weathered cedar sill trough
glm::vec4 bracketIron(0.14f, 0.14f, 0.16f, 1.0f);   // Mounting corner brackets
glm::vec4 pottingSoil(0.18f, 0.13f, 0.09f, 1.0f);   // Rich organic potting earth
// Flower Colors array includes Crimson, Marigold, Pink, White, Violet, and Coral
```

### 15. Miniature Bonsai Trees & Accent Stones (*Suiseki*)
* **File:** `src/Objects.h` (in `createBonsaiTree`)
* **Code to edit:**
```cpp
glm::vec4 glazedPot(0.16f, 0.26f, 0.48f, 1.0f);     // Glazed cobalt rectangular tray
glm::vec4 mossSoil(0.24f, 0.38f, 0.18f, 1.0f);      // Lush living green moss mound
glm::vec4 suisekiStone(0.38f, 0.38f, 0.42f, 1.0f);  // Weathered viewing stone accent
glm::vec4 gnarledBark(0.32f, 0.20f, 0.14f, 1.0f);   // Aged twisting pine trunk & forks
glm::vec4 pineNeedles(0.18f, 0.42f, 0.22f, 1.0f);   // Sculpted dark-green pine pads
```

### 16. Hanging Kokedama Moss Balls & Cascading Vines
* **File:** `src/Objects.h` (in `createHangingKokedama`)
* **Code to edit:**
```cpp
glm::vec4 cordColor(0.62f, 0.54f, 0.42f, 1.0f);     // Braided jute suspension cord
glm::vec4 mossBall(0.28f, 0.52f, 0.22f, 1.0f);      // Sphere of living green forest moss
glm::vec4 ivyLeaf(0.20f, 0.46f, 0.18f, 1.0f);       // Cascading green ivy vine foliage
// Trailing blossoms in Crimson, Golden, and Pale Pink
```

### 17. Second Floor Shinshitsu Furnishings
* **File:** `src/Objects.h` (inside `createMachiyaBuilding`)
* **Code to edit:**
```cpp
// Traditional Futon Bed:
glm::vec4 shikibuton(0.96f, 0.95f, 0.92f, 1.0f);    // White cotton mattress base
glm::vec4 kakebuton(0.78f, 0.18f, 0.22f, 1.0f);     // Crimson brocade festival quilt
glm::vec4 kakeGoldTrim(0.92f, 0.78f, 0.25f, 1.0f);  // Gold embroidery border band
glm::vec4 makura(0.18f, 0.24f, 0.42f, 1.0f);        // Indigo silk buckwheat pillow

// Folding Screen (Byoubu):
glm::vec4 byoubuFrame(0.14f, 0.10f, 0.08f, 1.0f);   // Ebonized dark lacquer frame
glm::vec4 byoubuLeaf(0.92f, 0.78f, 0.22f, 1.0f);    // Shimmering gold leaf foil panels

// Study Desk (Tsukue):
glm::vec4 tsukueWood(0.28f, 0.16f, 0.10f, 1.0f);    // Polished dark cherrywood desk
glm::vec4 suzuriStone(0.12f, 0.12f, 0.14f, 1.0f);   // Jet black ceramic inkstone
glm::vec4 scrollPaper(0.92f, 0.88f, 0.80f, 1.0f);   // Calligraphy manuscript parchment
```

---

## 4. How to Adjust Material Specularity & Textures in Code

All material properties and texture assignments are set per-node in `src/Scene.h` (inside `applyTexturesAndMaterials()`):

### A. Adjusting Specular Highlight Intensity & Shininess
In `src/Scene.h` or on any `SceneNode*`:
```cpp
// Set specular highlight reflectivity (0.0 = completely matte, 1.0 = highly glossy):
node->specularStrength = 0.5f;

// Set shininess exponent alpha (higher = tighter, sharper specular reflection pinpoint):
// e.g. 8.0 = soft cloth/wood, 32.0 = smooth plastic, 64.0 = polished ceramic/lacquer, 128.0 = shiny metal/gold
node->shininess = 64.0f;
```

### B. Changing or Adding Textures to Any Node
Textures are generated in `assets/textures/` and stored in `textures` map:
```cpp
// Assign an existing texture to a node with custom UV tiling:
node->texture = textures["wood_timber"].get();
node->textureTiling = glm::vec2(2.0f, 4.0f); // Tiles 2x horizontally, 4x vertically

// Available generated textures:
// - textures["wood_timber"]    : Natural grain cedar wood
// - textures["roof_tiles"]     : Scalloped Japanese ceramic roof shingles
// - textures["stone_pavement"] : Mortared stone flagstones
// - textures["tatami_cloth"]   : Woven festival awning & banner cloth
// - textures["gold_leaf"]      : Shimmering gold leaf foil
// - textures["sakura_bark"]    : Rough cherry tree bark
// - textures["lantern_paper"]  : Translucent washi paper with bamboo rings
```

---

## 5. How to Compile & Verify Your Color Changes

After modifying any color values in `src/Objects.h` or shaders:

1. **Using Visual Studio:**
   * Press **<kbd>Ctrl</kbd> + <kbd>Shift</kbd> + <kbd>B</kbd>** to build.
   * Press **<kbd>Ctrl</kbd> + <kbd>F5</kbd>** to launch without debugging.

2. **Using Developer PowerShell:**
   Run the MSBuild command from the workspace folder:
   ```powershell
   & "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe" "Matsuri Nights — A Japanese Festival Street\Matsuri Nights — A Japanese Festival Street.vcxproj" /p:Configuration=Debug /p:Platform=x64
   ```
   Then launch:
   ```powershell
   & "Matsuri Nights — A Japanese Festival Street\x64\Debug\Matsuri Nights - A Japanese Festival Street.exe"
   ```

---

## 6. Live Interactive Hotkeys for Visual & Lighting Inspection

| Key Binding | Function / Visual Effect | Purpose |
|---|---|---|
| <kbd>F1</kbd> | **Toggle Minimal HUD Overlay** | Displays active FPS, selected object, and contextual hints in top-right corner. |
| <kbd>0</kbd> / <kbd>KP_0</kbd> | **Toggle Festival Lights** | Turns all 12 festival point lights and street lanterns on/off. |
| <kbd>N</kbd> | **Toggle Day / Night** | Blends between daytime sunlight and nighttime moonlight/starlight. |
| <kbd>P</kbd> | **Cycle Shading Modes** | Cycles Blinn-Phong $\to$ Pure Diffuse $\to$ Pure Ambient to inspect lighting terms. |
| <kbd>V</kbd> | **Toggle PCF Soft Shadows** | Toggles 16-sample soft shadow mapping on/off. |
| <kbd>X</kbd> | **Toggle Texture Mapping** | Toggles procedural textures on/off to evaluate base vertex colors. |
| <kbd>Z</kbd> | **Toggle Real-Time GPU Ray Tracer** | Switches between rasterized Blinn-Phong and analytical Whitted ray tracing. |
| <kbd>T</kbd> | **Cycle Selected Target Object** | Cycles through all 15 inspectable objects (including all 4 Machiya townhouses). |
| <kbd>H</kbd> / <kbd>G</kbd> | **Toggle Doors & Windows** | Opens/closes Shoji doors (<kbd>H</kbd>) and sliding windows (<kbd>G</kbd>) within interaction range. |
| <kbd>F</kbd> | **Launch Fireworks Rocket** | Launches high-altitude rocket that explodes into brilliant sky colors. |
| <kbd>M</kbd> | **Replay Magic Trick** | Triggers the magician's levitating orb and disappearing treasure box trick. |
| <kbd>I</kbd> / <kbd>K</kbd> | **Translate Target $\pm Z$** | Moves selected object forward / backward along the street. |
| <kbd>J</kbd> / <kbd>L</kbd> | **Translate Target $\pm X$** | Moves selected object left / right across the street. |
| <kbd>U</kbd> / <kbd>O</kbd> | **Translate Target $\pm Y$** | Moves selected object up / down. |
| <kbd>Numpad 8</kbd> / <kbd>2</kbd> | **Pitch Target $\pm X$-axis** | Tilts selected object forward / backward. |
| <kbd>Numpad 4</kbd> / <kbd>6</kbd> | **Yaw Target $\pm Y$-axis** | Rotates selected object horizontally. |
| <kbd>+</kbd> / <kbd>-</kbd> | **Scale Target Up / Down** | Scales selected object up (1.1x) or down (0.9x). |
| <kbd>F9</kbd> | **CPU Software Ray Tracer** | Renders high-fidelity offline still to `raytraced_snapshot.bmp`. |
| <kbd>Screenshot</kbd> | **Clean Screenshot** | Press <kbd>P</kbd> (or <kbd>Shift</kbd>+<kbd>P</kbd> to include HUD) to save BMP snapshot. |

