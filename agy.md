# Matsuri Nights — Development & Change Log (`agy.md`)
**Project:** Matsuri Nights — A Japanese Festival Street  
**Course:** CSE4102 Computer Graphics & Image Processing Laboratory  
**Author:** MD. Abu Hasanat Soykot | **Roll:** 2107100 | **Group:** B2  

---

## Overview
This document tracks all features, additions, bug fixes, transformations, and architectural updates implemented in the project. Whenever any component, asset, animation, control, or shader is modified, added, or fixed, a detailed entry is logged here.

---

## Log Entries

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
