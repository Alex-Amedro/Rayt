# Technical Skills Demonstrated

## Computer Graphics
- **Path tracing**: Monte Carlo ray tracer with global illumination
- **Material system**: Lambertian diffuse, metal (roughness), dielectric (refraction), emissive, mirror
- **Post-processing**: ACES tone mapping, gamma correction, denoising (box, Gaussian, bilateral)
- **Camera**: Perspective projection with anti-aliasing (multi-sample per pixel)

## C++17
- **3,500 lines**: Modular architecture (core, geometry, materials, editor, preview)
- **OOP design**: Polymorphic material system with `Hittable` interface
- **Template metaprogramming**: Header-only `vec3<T>` class
- **Modern practices**: RAII, smart pointers, const correctness

## Real-time Graphics
- **OpenGL 3.3+**: Shader-based rendering for 3D scene preview
- **ImGui**: UI with camera controls, render settings, scene hierarchy
- **ImGuizmo**: Transform gizmos for object manipulation
- **GLFW/GLEW**: Window management and OpenGL extensions

## Mathematics
- **Ray-geometry intersection**: Quadratic solver (spheres), plane equations
- **Vector algebra**: Dot/cross products, reflection, refraction (Snell's law)
- **Probability**: Cosine-weighted hemisphere sampling, Monte Carlo integration
- **Numerical methods**: Schlick's approximation for Fresnel, random sampling

## Software Engineering
- **JSON serialization**: Scene save/load with nlohmann/json
- **Build system**: Makefiles with modular compilation
- **Version control**: Git with .gitignore for clean repository
- **File I/O**: PPM image format writer

## Key Challenges Solved
- Color space bugs (forgot gamma correction initially)
- Dielectric refraction with total internal reflection
- Bilateral filter optimization (avoided O(n²) naive implementation)
- Managing OpenGL context across multiple windows

## Project Stats
- ~3,500 lines of C++17
- 5 material types, 2 geometry primitives
- 3 denoising filters
- Real-time 3D editor + offline renderer

---

**Technologies**: C++17 · OpenGL 3.3+ · ImGui · GLFW · GLEW · nlohmann/json
