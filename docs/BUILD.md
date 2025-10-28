# Building & Running Instructions

## Quick Start

### Prerequisites

Make sure you have the required dependencies installed:

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential libglfw3-dev libglew-dev libglm-dev

# Fedora/RHEL
sudo dnf install gcc-c++ glfw-devel glew-devel glm-devel

# macOS (with Homebrew)
brew install glfw glew glm
```

### Build Commands

The project uses Makefiles for compilation. Navigate to the `c++` directory:

```bash
cd c++
```

#### Option 1: Build the Command-Line Ray Tracer

```bash
make main
./main
```

This renders the scene defined in `src/data/save/demo_scene.json` and outputs `image.ppm`.

#### Option 2: Build the Interactive 3D Editor

```bash
make editor
./editor
```

This launches the full OpenGL-based scene editor with ImGui interface.

#### Option 3: Build the Real-Time Preview

```bash
make preview
./preview
```

This provides a lightweight OpenGL preview without the full editor UI.

### Clean Build

To remove all compiled objects and executables:

```bash
make clean
```

To rebuild everything from scratch:

```bash
make clean
make editor  # or make main, or make preview
```

---

## Creating Your First Scene

1. **Launch the editor**:
   ```bash
   cd c++
   make editor
   ./editor
   ```

2. **Build your scene**:
   - Click "Add Sphere" or "Add Plane" in the UI
   - Select objects by clicking them in the 3D viewport
   - Adjust properties in the right panel:
     - Position (X, Y, Z coordinates)
     - Size (radius for spheres)
     - Color (RGB sliders)
     - Material type (Diffuse, Metal, Glass, Emissive, Mirror)
     - Material properties (roughness, refraction index, etc.)

3. **Configure the camera**:
   - Use mouse controls:
     - **Left drag**: Rotate camera (orbit)
     - **Middle drag**: Pan camera
     - **Scroll wheel**: Zoom in/out
   - Or manually set position in the Camera panel

4. **Set render settings**:
   - Resolution: Choose preset or custom dimensions
   - Samples per pixel: Higher = smoother (but slower)
   - Max depth: Ray bounce limit (higher = more reflections)
   - Threads: CPU thread count for parallel rendering

5. **Save your scene**:
   - Click "Save Scene"
   - Scene saved to `src/data/save/demo_scene.json`

6. **Render high-quality image**:
   - Click "Render Ray Traced Image" in the editor
   - Or exit and run: `./main`
   - Output: `image.ppm` in the c++ directory

---

## Converting PPM to PNG/JPEG

The ray tracer outputs PPM format (simple but large). Convert using ImageMagick:

```bash
# Install ImageMagick
sudo apt-get install imagemagick  # Ubuntu/Debian
brew install imagemagick          # macOS

# Convert to PNG
convert image.ppm output.png

# Convert to JPEG
convert image.ppm output.jpg

# Resize while converting
convert image.ppm -resize 50% output_half.png
```

---

## Project Structure

```
c++/
├── Makefile              # Main build configuration
├── Makefile.editor       # Editor-specific build
├── Makefile.preview      # Preview-specific build
├── main.cpp              # Command-line ray tracer entry
├── include/              # Header files (.hpp)
│   ├── core/            # Core engine (vec3, camera, ray_color)
│   ├── geometry/        # Shapes (sphere, plane, hittable)
│   ├── materials/       # Material types
│   ├── editor/          # Editor UI and scene management
│   └── preview/         # OpenGL preview renderer
├── src/                 # Implementation files (.cpp)
│   ├── core/
│   ├── geometry/
│   ├── editor/
│   └── preview/
├── external/            # Third-party libraries
│   ├── imgui/          # Dear ImGui UI library
│   ├── ImGuizmo/       # 3D gizmo widget
│   └── nlohmann/       # JSON library
└── obj/                # Compiled object files (generated)
```

---

## Compilation Details

### Compiler Flags

- **C++ Standard**: `-std=c++17`
- **Warnings**: `-Wall -Wextra -Wpedantic`
- **Optimization**: `-O3` for release builds
- **Debug**: `-g` for debug builds

### Libraries Linked

- **OpenGL**: `-lGL`
- **GLFW**: `-lglfw`
- **GLEW**: `-lGLEW`
- **Threading**: `-lpthread`
- **Math**: `-lm`

### Modular Compilation

Each `.cpp` file is compiled to `.o` independently, then linked:

```bash
# Example compilation flow
g++ -c src/core/vec3.cpp -o obj/vec3.o -std=c++17 -O3
g++ -c src/geometry/sphere.cpp -o obj/sphere.o -std=c++17 -O3
# ... (all other files)
g++ obj/*.o -o main -lGL -lglfw -lGLEW -lpthread
```

This allows faster incremental builds (only changed files recompile).

---

## Troubleshooting

### "GLFW/GLEW not found"

Install development packages:
```bash
sudo apt-get install libglfw3-dev libglew-dev
```

### "undefined reference to pthread"

Add `-lpthread` to linker flags in Makefile.

### "Scene file not found"

Make sure you've created and saved a scene using the editor first:
```bash
./editor  # Create scene, then "Save Scene"
./main    # Now render
```

### Render is too slow

- Reduce samples per pixel (try 50-100 for preview)
- Lower resolution (try 1280x720)
- Decrease max depth (try 10-20)
- Increase thread count to match your CPU cores

### Black image or weird colors

- Check that objects are within camera view
- Ensure materials are properly set
- Try adding an emissive object (light source)
- Verify gamma correction is enabled (default 2.2)

---

## Development Workflow

### Recommended Iteration Cycle

1. **Edit code** in VS Code or your favorite editor
2. **Compile** with `make editor` or `make main`
3. **Test** in the editor or by rendering
4. **Debug** with GDB if needed:
   ```bash
   gdb ./editor
   (gdb) run
   (gdb) backtrace  # if crash occurs
   ```

### Adding New Features

1. Create header file in `include/<category>/`
2. Create implementation in `src/<category>/`
3. Add `.o` target to Makefile
4. Include header in relevant files
5. Rebuild with `make clean && make`

---

## Performance Tips

- **Multi-threading**: Set `num_threads` to your CPU core count
- **Resolution**: Start low (720p) for testing, render 4K final
- **Samples**: 50-100 for preview, 500-1000 for final quality
- **Depth**: 10-20 is usually sufficient, >50 rarely helps
- **Denoising**: Enable if you use fewer samples (trades speed for smoothness)

---

## Need Help?

- Check the [README.md](../README.md) for project overview
- See [ARCHITECTURE.md](docs/ARCHITECTURE.md) for technical details
- Open an issue on GitHub for bugs or questions

---

**Happy Ray Tracing!** 🎨✨
