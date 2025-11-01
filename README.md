# Rayt - Ray Tracing Engine

Physically-based ray tracing engine written in C++17 with interactive OpenGL editor.

## Technologies

- **Language**: C++17
- **Graphics**: OpenGL 3.3+, GLFW, GLEW, GLM
- **Interface**: Dear ImGui, ImGuizmo
- **Build**: Makefile (GCC/G++)
- **Libraries**: nlohmann/json, STL

## Features

### Ray Tracing
- Path tracing with global illumination
- Geometries: spheres, infinite planes
- Materials: diffuse (Lambertian), metal, dielectric (glass), emissive, mirror
- Anti-aliasing (MSAA)
- ACES tone mapping
- Gamma correction
- Denoising filters (box, Gaussian, bilateral)

### Interactive Editor
- Real-time OpenGL preview
- ImGui interface
- 3D gizmo for object manipulation
- Camera controls (rotation, zoom, pan)
- Ray picking for object selection
- Wireframe mode
- JSON scene serialization

## Installation

### Dependencies

```bash
# Ubuntu/Debian
sudo apt-get install build-essential libglfw3-dev libglew-dev libglm-dev

# Fedora
sudo dnf install gcc-c++ glfw-devel glew-devel glm-devel

# macOS
brew install glfw glew glm
```

### Build

```bash
cd c++

# Interactive editor
make editor
./editor

# Command-line ray tracer
make main
./main

# OpenGL preview
make preview
./preview
```

## Usage

### Editor

1. Run `./editor`
2. Add objects (spheres, planes)
3. Modify properties (position, color, materials)
4. Configure rendering (resolution, samples, bounces)
5. Save scene as JSON
6. Launch final render (output: `image.ppm`)

### Render Settings

- **Resolution**: 480p to 4K (or custom)
- **Samples per pixel**: anti-aliasing quality (1-1000+)
- **Max depth**: light bounces (1-50)
- **Gamma**: gamma correction
- **Ambient**: ambient light

### Materials

- **Diffuse**: matte surfaces
- **Metal**: metallic surfaces (parameter: roughness)
- **Dielectric**: transparent materials (parameter: refraction index)
- **Emissive**: light sources
- **Mirror**: perfect specular reflection

## Structure

```
c++/
├── src/
│   ├── core/          # Ray tracing (Ray, Camera, Scene)
│   ├── geometry/      # Primitives (Sphere, Plane)
│   ├── materials/     # Materials
│   ├── utils/         # Vec3, utilities
│   ├── rendering/     # Post-processing
│   └── editor/        # OpenGL/ImGui interface
├── include/
├── lib/               # External libraries
└── data/save/         # Saved scenes
```

## Makefile

```bash
make          # Build all
make main     # Build ray tracer
make editor   # Build editor
make preview  # Build preview
make clean    # Clean builds
make rebuild  # Rebuild all
```

## License

MIT License - see [LICENSE](LICENSE)

## Author

Alex Amedro - Looking for M1 internship in Computer Graphics / Game Development

GitHub: [@Alex-Amedro](https://github.com/Alex-Amedro)
