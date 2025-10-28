# Technical Architecture & Features

## 📐 Architecture Overview

### Project Structure

```
Rayt/
├── c++/                         # Main C++ source directory
│   ├── include/                 # Header files (interfaces)
│   │   ├── core/                # Core engine components
│   │   │   ├── vec3.hpp         # 3D vector mathematics
│   │   │   ├── camera.hpp       # Camera model & ray generation
│   │   │   ├── ray_color.hpp    # Ray tracing color computation
│   │   │   └── denoise.hpp      # Denoising algorithms
│   │   ├── geometry/            # Geometric primitives
│   │   │   ├── hittable.hpp     # Abstract intersection interface
│   │   │   ├── sphere.hpp       # Sphere intersection
│   │   │   └── plane.hpp        # Infinite plane intersection
│   │   ├── materials/           # Material system
│   │   │   ├── material.hpp     # Abstract material interface
│   │   │   ├── diffuse.hpp      # Lambertian diffuse
│   │   │   ├── metal.hpp        # Metallic with roughness
│   │   │   ├── dielectric.hpp   # Glass/transparent materials
│   │   │   ├── emissive.hpp     # Light-emitting materials
│   │   │   └── mirror.hpp       # Perfect reflection
│   │   ├── editor/              # Interactive editor components
│   │   │   ├── scene.hpp        # Scene management
│   │   │   ├── editor_ui.hpp    # ImGui interface
│   │   │   ├── camera_controller.hpp  # Camera manipulation
│   │   │   └── gizmo.hpp        # 3D transformation gizmo
│   │   └── preview/             # OpenGL preview renderer
│   │       ├── renderer.hpp     # OpenGL rendering pipeline
│   │       ├── shader_manager.hpp # GLSL shader management
│   │       └── window.hpp       # GLFW window handling
│   ├── src/                     # Implementation files
│   │   ├── core/                # Core implementations
│   │   ├── geometry/            # Geometry implementations
│   │   ├── editor/              # Editor implementations
│   │   └── preview/             # Preview implementations
│   ├── external/                # Third-party libraries
│   │   ├── imgui/              # Dear ImGui
│   │   ├── ImGuizmo/           # 3D gizmo
│   │   └── nlohmann/           # JSON library
│   ├── main.cpp                # Command-line ray tracer entry point
│   ├── Makefile                # Build configuration
│   └── Makefile.editor         # Editor build configuration
└── docs/                       # Documentation and assets
```

---

## 🔬 Ray Tracing Implementation

### Path Tracing Algorithm

The engine uses **Monte Carlo path tracing** for physically accurate light simulation:

```cpp
// Simplified ray tracing loop
Color ray_color(const Ray& r, const Scene& scene, int depth) {
    // Russian roulette termination
    if (depth <= 0) return Color(0, 0, 0);
    
    // Test ray intersection with scene
    HitRecord rec;
    if (!scene.hit(r, 0.001, infinity, rec)) {
        return background_color(r);  // Sky/environment
    }
    
    // Material scattering
    Ray scattered;
    Color attenuation;
    Color emitted = rec.material->emitted();
    
    if (rec.material->scatter(r, rec, attenuation, scattered)) {
        // Recursive ray tracing (next bounce)
        return emitted + attenuation * ray_color(scattered, scene, depth - 1);
    }
    
    return emitted;  // Light source or absorption
}
```

**Key Features:**
- **Global Illumination**: Light bounces multiple times, creating realistic indirect lighting
- **Importance Sampling**: Materials generate scattered rays based on BRDF
- **Energy Conservation**: Color attenuation ensures physical correctness
- **Recursive Depth**: Configurable bounce limit balances quality vs performance

---

## 🎨 Material System

### Polymorphic Material Architecture

All materials inherit from an abstract `Material` interface:

```cpp
class Material {
public:
    virtual bool scatter(
        const Ray& ray_in,
        const HitRecord& rec,
        Color& attenuation,
        Ray& scattered
    ) const = 0;
    
    virtual Color emitted() const { return Color(0,0,0); }
};
```

### Material Implementations

#### 1. **Diffuse (Lambertian)**
- **Physics**: Perfectly matte surface scattering light uniformly
- **Implementation**: Random hemisphere sampling around surface normal
- **Use Case**: Wood, stone, paper, matte paint

```cpp
// Scatter direction: random in hemisphere
Vec3 scatter_direction = rec.normal + random_unit_vector();
```

#### 2. **Metal**
- **Physics**: Specular reflection with optional roughness
- **Implementation**: Perfect reflection + random perturbation based on roughness
- **Parameters**:
  - `albedo`: Base color (RGB)
  - `roughness`: 0.0 (mirror) to 1.0 (very rough)

```cpp
// Reflected ray with roughness
Vec3 reflected = reflect(ray_in.direction(), rec.normal);
scattered = Ray(rec.p, reflected + roughness * random_in_unit_sphere());
```

#### 3. **Dielectric (Glass)**
- **Physics**: Refraction using Snell's law + internal reflection
- **Implementation**: 
  - Fresnel effect (view-angle dependent reflection)
  - Total internal reflection
  - Schlick's approximation for realistic behavior
- **Parameters**: 
  - `index_of_refraction`: Glass ≈ 1.5, Water ≈ 1.33, Diamond ≈ 2.4

```cpp
// Snell's law: n1 * sin(θ1) = n2 * sin(θ2)
double refraction_ratio = rec.front_face ? (1.0/ior) : ior;
Vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);

// Fresnel: more reflection at grazing angles
double reflectance = fresnel_schlick(cos_theta, refraction_ratio);
```

#### 4. **Emissive**
- **Physics**: Self-illuminating surface (light source)
- **Implementation**: Returns emission color independent of incoming light
- **Parameters**:
  - `emission_color`: RGB color of light
  - `intensity`: Brightness multiplier

#### 5. **Mirror**
- **Physics**: Perfect specular reflection (roughness = 0)
- **Implementation**: Ideal reflection without diffusion
- **Use Case**: Perfect mirrors, polished metal

---

## 🖥️ Interactive Editor

### OpenGL Real-Time Preview

The editor provides instant visual feedback using OpenGL for 3D rendering:

**Features:**
- **Vertex Buffer Objects (VBOs)**: Efficient GPU geometry storage
- **GLSL Shaders**: Programmable rendering pipeline
- **Depth Testing**: Correct occlusion and 3D perception
- **Wireframe Mode**: Toggle between solid and edge visualization

### Camera System

**Two Camera Implementations:**

1. **Ray Tracing Camera** (`camera.hpp`):
   - Generates rays for each pixel
   - Configurable field of view (FOV)
   - Position and look-at target

2. **OpenGL Camera** (`camera_gl.hpp`):
   - View matrix transformation
   - Projection matrix (perspective)
   - Interactive controls (orbit, pan, zoom)

### Scene Management

**JSON Serialization Schema:**

```json
{
  "camera": {
    "position": [x, y, z],
    "look_at": [x, y, z],
    "fov": 90.0
  },
  "render_settings": {
    "width": 1920,
    "height": 1080,
    "samples": 100,
    "max_depth": 50,
    "gamma": 2.2
  },
  "objects": [
    {
      "type": "sphere",
      "center": [0, 0, 0],
      "radius": 1.0,
      "material": {
        "type": "metal",
        "albedo": [0.7, 0.6, 0.5],
        "roughness": 0.3
      }
    }
  ]
}
```

---

## ⚡ Performance Optimizations

### Multi-Threading

**Implementation:**
```cpp
// Divide image into strips, one per thread
const int num_threads = std::thread::hardware_concurrency();
std::vector<std::thread> threads;

for (int t = 0; t < num_threads; ++t) {
    threads.emplace_back([=]() {
        // Each thread renders its assigned rows
        for (int j = start_row; j < end_row; ++j) {
            for (int i = 0; i < image_width; ++i) {
                // Render pixel (i, j)
            }
        }
    });
}

// Wait for all threads to complete
for (auto& thread : threads) {
    thread.join();
}
```

**Performance Gains:**
- 4-core CPU: ~3.5x speedup
- 8-core CPU: ~6-7x speedup
- Scales well with available cores

### Future Optimizations

1. **Bounding Volume Hierarchy (BVH)**
   - Current: O(n) intersection tests per ray
   - With BVH: O(log n) intersection tests
   - Expected speedup: 10-100x for complex scenes

2. **GPU Acceleration**
   - CUDA/OpenCL implementation
   - Potential for real-time path tracing (30+ FPS)

---

## 🧮 Mathematical Foundations

### Vector Operations

Custom `Vec3` class implements:
- **Dot Product**: `a · b = |a||b|cos(θ)` - for angles, projections
- **Cross Product**: `a × b` - for normals, perpendicular vectors
- **Normalization**: `v̂ = v / |v|` - unit vectors
- **Reflection**: `r = v - 2(v · n)n` - for mirrors
- **Refraction**: Snell's law implementation

### Ray-Geometry Intersection

#### Sphere Intersection
Solve: `|P(t) - C|² = r²` where `P(t) = O + td`

```cpp
// Quadratic equation: at² + bt + c = 0
Vec3 oc = ray.origin() - center;
double a = dot(ray.direction(), ray.direction());
double b = 2.0 * dot(oc, ray.direction());
double c = dot(oc, oc) - radius * radius;

double discriminant = b*b - 4*a*c;
if (discriminant < 0) return false;  // No intersection

double t = (-b - sqrt(discriminant)) / (2.0*a);
```

#### Plane Intersection
Solve: `(P - P₀) · n = 0` where P₀ is point on plane, n is normal

```cpp
double denom = dot(ray.direction(), normal);
if (fabs(denom) < 1e-6) return false;  // Parallel

double t = dot(plane_point - ray.origin(), normal) / denom;
```

---

## 🎯 Learning Outcomes

### Technical Skills Demonstrated

| Skill Category | Specific Skills |
|----------------|----------------|
| **C++ Programming** | Templates, polymorphism, RAII, smart pointers, STL containers |
| **Computer Graphics** | Ray tracing, rasterization, shaders, transformation matrices |
| **Mathematics** | Linear algebra, calculus (Monte Carlo integration), trigonometry |
| **Software Architecture** | Design patterns, modular design, clean code principles |
| **Performance** | Multi-threading, profiling, algorithmic optimization |
| **Tools** | Makefiles, GDB debugging, version control (Git) |
| **Libraries** | OpenGL, GLFW, ImGui, JSON parsing |

### Problem-Solving Examples

1. **Camera Export Bug**: Fixed incorrect vector export by saving position/target instead of view matrix
2. **Z-Fighting**: Resolved grid flickering by offsetting plane slightly (y = -0.01)
3. **Color Space**: Implemented proper gamma correction for realistic color reproduction
4. **Thread Safety**: Used proper synchronization for multi-threaded rendering

---

## 🔮 Next Steps

For further development, prioritized enhancements:

1. **✅ High Priority**: Triangle meshes + OBJ loader
2. **⭐ High Impact**: Point lights with shadow rays
3. **🎨 Visual Quality**: ACES tone mapping
4. **🚀 Performance**: BVH acceleration structure
5. **🎓 Advanced**: Texture mapping and normal maps

---

**Last Updated**: October 2025  
**Author**: Alex Amedro
