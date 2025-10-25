# Ray Tracer - Status & Roadmap
**Dernière mise à jour: 25 octobre 2025**

---

## ✅ **COMPLÉTÉ (Phase 1 - Fondations)**

### 🎓 **Apprentissage (7 leçons)**
- [x] **Leçon 1:** Gradient simple
- [x] **Leçon 2:** Sphère avec intersection rayon
- [x] **Leçon 3:** Normales et ombrage basique
- [x] **Leçon 4:** Anti-aliasing
- [x] **Leçon 5:** Matériaux diffus (Lambertian)
- [x] **Leçon 6:** Matériaux métalliques
- [x] **Leçon 7:** Matériaux diélectriques (verre)

### 🏗️ **Infrastructure**
- [x] Architecture propre (hittable, material, vec3)
- [x] Path tracing complet
- [x] Multiple objets (sphères + plans)
- [x] Trois types de matériaux (Diffus, Métal, Verre)
- [x] Gamma correction
- [x] Anti-aliasing (samples multiples)

### 🎨 **Éditeur de Scène 3D (OpenGL)**
- [x] Visualisation temps réel OpenGL
- [x] Caméra interactive (rotation, zoom, pan)
- [x] Gizmo 3D pour manipulation objets
- [x] UI ImGui complète
- [x] Ajout/suppression objets
- [x] Édition propriétés (couleur, matériau, taille)
- [x] Wireframe + grille au sol
- [x] Ray picking (sélection à la souris)
- [x] Save/Load scènes (JSON)

### ⚙️ **Paramètres de Rendu**
- [x] Résolution configurable (presets 480p-4K)
- [x] Samples per pixel (anti-aliasing)
- [x] Max depth (rebonds)
- [x] Gamma correction paramétrable
- [x] Multi-threading (configurable)

### 🐛 **Corrections Récentes**
- [x] **Caméra:** Export correct position/target (pas vecteurs view_matrix)
- [x] **Aspect ratio:** Calculé depuis image_width/height
- [x] **Couleurs:** Normalisation [0,1] (plus [0,255])
- [x] **Matériaux:** Noms français (Diffus/Métal/Verre)
- [x] **JSON:** Structure correcte pour tous les paramètres
- [x] **Grille:** Z-fighting corrigé (y=-0.01)
- [x] **UI:** ComboBox résolution + InputInt pour valeurs précises

---

## 🚀 **PROCHAINES ÉTAPES (Voir ROADMAP_V2.md)**

Le fichier `ROADMAP_V2.md` contient la roadmap complète pour atteindre un rendu photo-réaliste professionnel.

**Top 5 priorités recommandées:**
1. Lumières ponctuelles + ombres portées
2. Matériaux émissifs (néons, lampes)
3. Tone mapping ACES (couleurs cinéma)
4. BVH (accélération 50×)
5. Textures procédurales (checker, Perlin noise)

---

## 📊 **État Actuel du Projet**

**Fonctionnalités:** ⭐⭐⭐⭐ (8/10)
- Éditeur complet et fonctionnel
- Ray tracing path tracing correct
- 3 matériaux physiquement plausibles

**Qualité visuelle:** ⭐⭐⭐ (6/10)
- Bon pour scènes simples
- Manque éclairage avancé
- Pas de textures

**Performance:** ⭐⭐ (4/10)
- Multi-threading basique
- Pas d'accélération spatiale
- Lent pour scènes complexes

**Workflow:** ⭐⭐⭐⭐⭐ (10/10)
- Éditeur visuel excellent
- Save/Load parfait
- UI intuitive

---

## 🎯 **Objectif Final**

**Vision:** Concurrent open-source de Blender Cycles
- Rendu photo-réaliste
- Temps réel avec GPU
- Bibliothèque de matériaux
- Import OBJ/GLTF
- Animation support

**ETA:** 2-3 mois de développement actif

---

## 📚 **Resources & Références**

**Code actuel:**
- `c++/main.cpp` - Ray tracer CPU
- `c++/src/editor/` - Éditeur OpenGL/ImGui
- `c++/include/materials/` - Matériaux physiques
- `c++/include/core/` - Path tracing

**Documentation:**
- ROADMAP_V2.md - Roadmap détaillée complète
- README.md - Guide utilisateur

**Inspirations:**
- Peter Shirley's "Ray Tracing in One Weekend"
- PBRT (Physically Based Rendering Textbook)
- Blender Cycles source code

---

**What's Good ✅**
- Clean architecture with proper inheritance (hittable, material)
- Separation of concerns (geometry, materials, ray tracing)
- Path tracing foundation is solid
- Good use of modern C++ (smart pointers, const correctness)
- Vector math is clean and efficient

**What Could Be Better 🔧**
- Limited primitives (only sphere and plane)
- No acceleration structure (slow for complex scenes)
- Basc materials only (no glass/refraction, no textures)
- Simple camera (no depth of field, motion blur)
- No advanced lighting (environment maps, importance sampling)
- Single-threaded (not using all CPU cores)

---

## Roadmap to Photorealism

### Phase 1: Core Features (Essential for realism)
**Priority: CRITICAL**

#### 1.1 Dielectric Material (Glass/Water)
**Impact: HIGH** | **Difficulty: MEDIUM**
```i
Why: Transparent materials are fundamental for realism
Files to modify: Create dielectric.hpp
Key concepts: Refraction, Snell's law, Fresnel equations
Reference: Your Python code already has this!
```

#### 1.2 Importance Sampling
**Impact: VERY HIGH** | **Difficulty: MEDIUM**
```
Why: Dramatically reduces noise for same sample count
Current: Random hemisphere sampling (inefficient)
Better: Cosine-weighted sampling for diffuse
Best: Multiple Importance Sampling (MIS)
Files: ray_color.cpp, diffuse.hpp
```

#### 1.3 Better Camera
**Impact: MEDIUM** | **Difficulty: EASY**
```
Features to add:
- Adjustable field of view (FOV)
- Depth of field (aperture + focus distance)
- Camera positioning (lookAt, lookFrom, up vector)
Files: Create camera.hpp/cpp
```

---

### Phase 2: Visual Quality (Making it beautiful)
**Priority: HIGH**

#### 2.1 Textures
**Impact: VERY HIGH** | **Difficulty: MEDIUM**
```
Types needed:
- Image textures (load JPG/PNG)
- Procedural textures (checkerboard, noise, marble)
- Normal maps (for surface detail without geometry)
Files: Create texture.hpp, modify materials
Library: stb_image.h (single header, easy to use)
```

#### 2.2 Advanced Materials
**Impact: HIGH** | **Difficulty: MEDIUM-HIGH**
```
Materials to add:
- Anisotropic (brushed metal)
- Subsurface scattering (skin, wax, marble)
- Emissive (area lights)
- Clearcoat (car paint)
Files: Create new material classes
```

#### 2.3 Environment Mapping
**Impact: VERY HIGH** | **Difficulty: MEDIUM**
```
Why: Realistic lighting from HDR images
Current: Simple gradient sky
Better: HDR environment maps (.hdr files)
Features: Importance sampling of environment
Files: Create environment.hpp, modify ray_color.cpp
```

---

### Phase 3: Performance (Making it fast)
**Priority: CRITICAL for complex scenes**

#### 3.1 Multi-threading
**Impact: VERY HIGH** | **Difficulty: EASY**
```
Why: 4-16x speedup on modern CPUs
Method: Split image into tiles, use std::thread or OpenMP
Files: main.cpp
Complexity: Low - ray tracing is "embarrassingly parallel"
```

#### 3.2 Bounding Volume Hierarchy (BVH)
**Impact: EXTREME** | **Difficulty: HIGH**
```
Why: 100-1000x speedup for complex scenes
Current: O(n) intersection tests per ray
With BVH: O(log n) intersection tests
Files: Create bvh.hpp/cpp
Note: Essential for scenes with >100 objects
```

#### 3.3 GPU Acceleration (Optional, advanced)
**Impact: EXTREME** | **Difficulty: VERY HIGH**
```
Technologies: CUDA, OptiX, or Vulkan ray tracing
Speedup: 10-100x vs CPU
Difficulty: Requires complete rewrite
Recommendation: Do this LAST, when CPU version is complete
```

---

### Phase 4: Advanced Rendering (Professional features)
**Priority: MEDIUM**

#### 4.1 Triangle Meshes + OBJ Loading
**Impact: VERY HIGH** | **Difficulty: MEDIUM**
```
Why: Essential for real models (characters, buildings, etc.)
Library: tinyobjloader (single header)
Files: Create triangle.hpp, mesh.hpp
Optimization: Must use BVH (millions of triangles)
```

#### 4.2 Participating Media (Fog/Smoke)
**Impact: MEDIUM** | **Difficulty: HIGH**
```
Features: Volumetric fog, smoke, clouds
Method: Volume scattering along rays
Files: Create volume.hpp
```

#### 4.3 Denoising
**Impact: HIGH** | **Difficulty: VERY HIGH**
```
Why: Get clean images with 10x fewer samples
Methods:
- Intel Open Image Denoise (OIDN) - easiest
- OptiX denoiser
- Machine learning denoiser
Recommendation: Use OIDN library (pre-trained)
```

---

### Phase 5: Production Features
**Priority: LOW (but cool)**

#### 5.1 Progressive Rendering
**Impact: MEDIUM** | **Difficulty: MEDIUM**
```
Why: See preview while rendering
Method: Write image after each sample pass
Files: main.cpp - add incremental saving
```

#### 5.2 Adaptive Sampling
**Impact: MEDIUM** | **Difficulty: MEDIUM**
```
Why: Use more samples where needed (complex areas)
Method: Variance-based sample allocation
Benefit: 20-30% faster for same quality
```

#### 5.3 Spectral Rendering
**Impact: LOW** | **Difficulty: HIGH**
```
Why: Physically accurate color (dispersion, etc.)
Current: RGB (3 wavelengths)
Better: Full spectrum (many wavelengths)
Use case: Prisms, rainbows, accurate color
```

---

## Recommended Priority Order

### If you want QUALITY first:
```
1. Dielectric material (glass)          [1 day]
2. Textures (image + procedural)        [2 days]
3. Environment mapping                  [2 days]
4. Better camera (DOF)                  [1 day]
5. Importance sampling                  [2 days]
6. Triangle meshes + OBJ loader         [3 days]
```

### If you want PERFORMANCE first:
```
1. Multi-threading                      [1 day]
2. BVH acceleration                     [3-5 days]
3. Dielectric material                  [1 day]
4. Triangle meshes                      [2 days]
5. Environment mapping                  [2 days]
```

### If you want REALISM first (my recommendation):
```
1. Dielectric material                  [1 day]  ← Start here!
2. Importance sampling                  [2 days]
3. Multi-threading                      [1 day]
4. Environment mapping                  [2 days]
5. Textures                            [2 days]
6. Better camera (DOF)                  [1 day]
7. BVH (when scenes get complex)        [4 days]
8. Triangle meshes                      [3 days]
```

---

## Code Architecture Review

### What You Did RIGHT ✅

1. **Polymorphism for materials/objects**
   - Perfect! This is exactly the right design
   - Easy to add new materials/shapes

2. **Separation of geometry and materials**
   - Good! Sphere doesn't care about color
   - Material handles all appearance

3. **Ray-material interaction via scatter()**
   - Excellent design pattern
   - Industry standard approach

4. **Using [0,255] color space internally**
   - Actually FINE for your needs
   - Many engines use float [0,1], but both work

### What Could Be BETTER 🔧

1. **Camera hardcoded in main.cpp**
   ```cpp
   // Current: viewport setup scattered in main
   // Better: Create a Camera class
   class Camera {
       vec3 origin, lookat;
       double fov, aperture, focus_dist;
       vec3 get_ray(double u, double v);
   };
   ```

2. **Scene is just a vector**
   ```cpp
   // Current: std::vector<hittable>
   // Better: Scene class with acceleration structure
   class Scene {
       std::vector<shared_ptr<hittable>> objects;
       BVH* acceleration;  // added later
   };
   ```

3. **Color handling could be cleaner**
   ```cpp
   // Consider: Create a Color class
   class Color {
       double r, g, b;
       Color gamma_correct();
       Color clamp();
       // Uses vec3 internally but clearer intent
   };
   ```

4. **No scene description format**
   ```cpp
   // Future: Load scenes from JSON/YAML
   // For now: Hardcoding in main.cpp is fine
   // Later: scene_loader.hpp
   ```

---

## Can You Still Refactor?

**Answer: YES! But strategically.**

### DON'T Change (already good):
- ✅ Material system (inheritance model)
- ✅ Hittable system (polymorphism)
- ✅ Vec3 class (works perfectly)
- ✅ Ray_color function (path tracing core)

### DO Add (missing abstractions):
- 📦 Camera class (before it gets messier)
- 📦 Scene class (when you add BVH)
- 📦 Texture class (before materials get complex)
- 📦 Color class (optional, for clarity)

### Code Smell to Fix:
```cpp
// In main.cpp - this will get messy:
vec3 horizontal(viewport_width, 0, 0);
vec3 vertical(0, viewport_height, 0);
vec3 lower_left_corner = camera_origin - ...;

// Better: Extract to Camera class NOW
Camera cam(origin, lookat, vfov, aspect);
Ray r = cam.get_ray(u, v);
```

---

## Next Steps - Concrete Actions

### This Week:
```bash
1. Test new Makefile:
   make clean
   make all
   make run

2. Implement dielectric material (glass):
   - Copy your Python glass code
   - Create dielectric.hpp
   - Test with glass sphere

3. Extract Camera class:
   - Create camera.hpp
   - Move viewport logic there
   - Add adjustable FOV
```

### Next Week:
```bash
4. Add multi-threading:
   - Split image into tiles
   - Use OpenMP or std::thread
   - 8x speedup on your CPU

5. Add importance sampling:
   - Cosine-weighted for diffuse
   - Better noise reduction

6. Load HDR environment map:
   - Download free HDRI from polyhaven.com
   - Use stb_image.h to load
   - Sample for sky color
```

---

## Resources

### Essential Reading:
- Ray Tracing in One Weekend (free book - you're already there!)
- PBRT Book (free online - industry bible)
- Physically Based Rendering course (Naty Hoffman)

### Code Examples:
- Smallpt (99 lines of path tracer)
- PBRT v3/v4 (reference implementation)
- Your own Python code! (already has glass)

### Assets:
- polyhaven.com - Free HDRIs and models
- turbosquid.com - 3D models
- freepbr.com - PBR textures

---

## Final Verdict on Your Code

**Grade: A- (Very Good Foundation)**

**Strengths:**
- Clean, readable C++ code
- Solid architecture (extensible)
- Working path tracer (hardest part done!)
- Good coding practices

**Room for improvement:**
- Missing Camera abstraction
- No acceleration structure yet
- Limited materials
- Single-threaded

**Recommendation:**
Your code is **excellent for the current stage**.
Don't refactor the core - just ADD features.
The architecture supports everything you need.

**You're ready to make this photorealistic! 🚀**

Start with glass material - it's easy and impressive.
Then add multi-threading for speed.
Then environment maps for beauty.

You've got this! 💪
