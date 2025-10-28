# Rayt - Advanced C++ Ray Tracer with Interactive 3D Editor



A feature-rich **physically-based ray tracing engine** built from scratch in C++17, featuring an interactive OpenGL-based scene editor with real-time preview capabilities.



---<!-- Add your best render here once ready:



## 🎯 Project Overview<p align="center">



**Rayt** is a comprehensive ray tracing project demonstrating advanced computer graphics techniques, including:  <img src="docs/hero_render.png" alt="Ray Traced Scene" width="800"/><!-- Add your best render here once ready:



- **Path Tracing Algorithm**: Full global illumination with multiple bounce lighting</p>

- **Interactive 3D Editor**: Real-time OpenGL preview with ImGui-based interface

- **Physically-Based Materials**: Diffuse (Lambertian), metallic, dielectric (glass), emissive, and mirror materials--><p align="center">

- **Advanced Features**: Denoising system, anti-aliasing, and ACES tone mapping

- **Scene Management**: JSON-based scene serialization for saving and loading complex scenes



This project was developed as a learning experience in computer graphics, demonstrating proficiency in:A feature-rich **physically-based ray tracing engine** built from scratch in C++17, featuring an interactive OpenGL-based scene editor with real-time preview capabilities.  <img src="docs/hero_render.png" alt="Ray Traced Scene" width="800"/><p align="center">Projet de ray tracing en C++.

- Modern C++ (C++17) with STL

- 3D mathematics and ray-geometry intersection algorithms

- OpenGL for real-time rendering

- UI/UX design with ImGui---</p>

- Software engineering best practices



---

## 🎯 Project Overview-->  <img src="docs/hero_render.png" alt="Ray Traced Scene" width="800"/>

## ✨ Key Features



### 🎨 Ray Tracing Engine

- **Path Tracing**: Physically accurate light simulation with configurable bounce depth**Rayt** is a comprehensive ray tracing project demonstrating advanced computer graphics techniques, including:

- **Multiple Geometries**: Spheres and infinite planes with efficient intersection algorithms

- **Material System**: 

  - **Diffuse**: Lambertian reflection with realistic light scattering

  - **Metal**: Configurable roughness for brushed to polished metal surfaces- **Path Tracing Algorithm**: Full global illumination with multiple bounce lightingA feature-rich **physically-based ray tracing engine** built from scratch in C++17, featuring an interactive OpenGL-based scene editor with real-time preview capabilities.</p>## 🛠️ Configuration de l'environnement

  - **Dielectric**: Glass-like materials with refraction and internal reflection

  - **Emissive**: Self-illuminating materials for light sources- **Interactive 3D Editor**: Real-time OpenGL preview with ImGui-based interface

  - **Mirror**: Perfect specular reflection

- **Anti-Aliasing**: Multi-sample per pixel (MSAA) for smooth edges- **Physically-Based Materials**: Diffuse (Lambertian), metallic, dielectric (glass), emissive, and mirror materials

- **ACES Tone Mapping**: Industry-standard HDR to LDR conversion (same as Unreal Engine)

- **Gamma Correction**: Proper color space management- **Advanced Features**: Multi-threading architecture, denoising system, anti-aliasing, and tone mapping

- **Denoising**: Three filter types (box, Gaussian, bilateral) for noise reduction

- **Scene Management**: JSON-based scene serialization for saving and loading complex scenes---

### 🖥️ Interactive 3D Editor

- **Real-Time OpenGL Preview**: Instant visual feedback while editing scenes

- **ImGui Interface**: Professional, intuitive controls for all scene parameters

- **3D Gizmo**: Interactive manipulation of objects in 3D spaceThis project was developed as a learning experience in computer graphics, demonstrating proficiency in:

- **Camera Controls**: 

  - Orbit rotation with mouse drag- Modern C++ (C++17) with STL

  - Zoom with mouse wheel

  - Pan with middle mouse button- 3D mathematics and ray-geometry intersection algorithms## 🎯 Project OverviewA feature-rich **physically-based ray tracing engine** built from scratch in C++17, featuring an interactive OpenGL-based scene editor with real-time preview capabilities.### Prérequis installés

- **Ray Picking**: Click objects to select them directly in the viewport

- **Wireframe Mode**: Toggle between solid and wireframe rendering- OpenGL for real-time rendering

- **Scene Hierarchy**: Manage multiple objects with easy add/remove functionality

- UI/UX design with ImGui

### ⚙️ Rendering Configuration

- **Resolution Presets**: Quick selection from 480p to 4K (3840×2160)- Multi-threaded application architecture

- **Custom Resolution**: Fine-tune width and height independently

- **Quality Settings**:- Software engineering best practices**Rayt** is a comprehensive ray tracing project demonstrating advanced computer graphics techniques, including:- ✅ **g++** (version 13.3.0) - Compilateur C++

  - Samples per pixel (1-1000+) for anti-aliasing quality

  - Max ray bounce depth (1-50) for light transport accuracy

  - Gamma correction factor

  - Ambient light control for scene mood---



### 💾 Scene Management

- **JSON Serialization**: Save and load complete scenes

- **Persistent Settings**: Camera position, render settings, and object properties## ✨ Key Features- **Path Tracing Algorithm**: Full global illumination with multiple bounce lighting---- ✅ **gdb** (version 15.0) - Débogueur

- **Export Ready**: Generate high-quality renders for portfolio or presentation



---

### 🎨 Ray Tracing Engine- **Interactive 3D Editor**: Real-time OpenGL preview with ImGui-based interface

## 🛠️ Technologies & Tools

- **Path Tracing**: Physically accurate light simulation with configurable bounce depth

| Category | Technologies |

|----------|-------------|- **Multiple Geometries**: Spheres and infinite planes with efficient intersection algorithms- **Physically-Based Materials**: Diffuse (Lambertian), metallic, dielectric (glass), emissive, and mirror materials- ✅ **VS Code Extensions** :

| **Language** | C++17 |

| **Graphics APIs** | OpenGL 3.3+, GLFW, GLEW, GLM |- **Material System**: 

| **UI Framework** | Dear ImGui, ImGuizmo |

| **Build System** | Makefile (GCC/G++) |  - **Diffuse**: Lambertian reflection with realistic light scattering- **Advanced Features**: Multi-threading, denoising system, anti-aliasing, and tone mapping

| **Libraries** | nlohmann/json, STL (containers, filesystem) |

| **Development** | VS Code, GDB debugger |  - **Metal**: Configurable roughness for brushed to polished metal surfaces

| **Math** | Custom Vec3 implementation for vectors and colors |

  - **Dielectric**: Glass-like materials with refraction and internal reflection- **Scene Management**: JSON-based scene serialization for saving and loading complex scenes## 🎯 Project Overview  - C/C++ IntelliSense (ms-vscode.cpptools)

---

  - **Emissive**: Self-illuminating materials for light sources

## 🚀 Getting Started

  - **Mirror**: Perfect specular reflection

### Prerequisites

- **Anti-Aliasing**: Multi-sample per pixel (MSAA) for smooth edges

```bash

# Ubuntu/Debian- **Gamma Correction**: Proper color space managementThis project was developed as a learning experience in computer graphics, demonstrating proficiency in:  - CMake Tools

sudo apt-get install build-essential libglfw3-dev libglew-dev libglm-dev

- **Multi-Threading Architecture**: Parallel rendering design for scalability

# Fedora

sudo dnf install gcc-c++ glfw-devel glew-devel glm-devel- Modern C++ (C++17) with STL



# macOS### 🖥️ Interactive 3D Editor

brew install glfw glew glm

```- **Real-Time OpenGL Preview**: Instant visual feedback while editing scenes- 3D mathematics and ray-geometry intersection algorithms**Rayt** is a comprehensive ray tracing project demonstrating advanced computer graphics techniques, including:  - C/C++ Extension Pack



### Building the Project- **ImGui Interface**: Professional, intuitive controls for all scene parameters



The project includes three main executables:- **3D Gizmo**: Interactive manipulation of objects in 3D space- OpenGL for real-time rendering



#### 1. **Main Ray Tracer** (Command-line renderer)- **Camera Controls**: 

```bash

cd c++  - Orbit rotation with mouse drag- UI/UX design with ImGui

make main

./main  - Zoom with mouse wheel

```

  - Pan with middle mouse button- Multi-threaded application architecture

#### 2. **Interactive Editor** (3D scene editor with OpenGL preview)

```bash- **Ray Picking**: Click objects to select them directly in the viewport

cd c++

make editor- **Wireframe Mode**: Toggle between solid and wireframe rendering- Software engineering best practices- **Path Tracing Algorithm**: Full global illumination with multiple bounce lighting### Configuration VS Code

./editor

```- **Scene Hierarchy**: Manage multiple objects with easy add/remove functionality



#### 3. **Real-Time Preview** (OpenGL-only fast preview)

```bash

cd c++### ⚙️ Rendering Configuration

make preview

./preview- **Resolution Presets**: Quick selection from 480p to 4K (3840×2160)---- **Interactive 3D Editor**: Real-time OpenGL preview with ImGui-based interfaceLes fichiers de configuration suivants ont été créés dans `.vscode/` :

```

- **Custom Resolution**: Fine-tune width and height independently

### Quick Render

- **Quality Settings**:

```bash

# Render with default settings  - Samples per pixel (1-1000+) for anti-aliasing quality

./main

  - Max ray bounce depth (1-50) for light transport accuracy## ✨ Key Features- **Physically-Based Materials**: Diffuse (Lambertian), metallic, dielectric (glass), emissive, and mirror materials- `c_cpp_properties.json` - Configuration IntelliSense (C++17)

# The output will be saved as image.ppm

# Convert to PNG (requires ImageMagick)  - Gamma correction factor

convert image.ppm output.png

```- **Performance Options**: Configurable thread count for optimal CPU utilization



---



## 📖 Usage Guide### 💾 Scene Management### 🎨 Ray Tracing Engine- **Advanced Features**: Multi-threading, denoising system, anti-aliasing, and tone mapping- `tasks.json` - Tâches de compilation



### Using the Editor- **JSON Serialization**: Save and load complete scenes



1. **Launch the editor**:- **Persistent Settings**: Camera position, render settings, and object properties- **Path Tracing**: Physically accurate light simulation with configurable bounce depth

   ```bash

   ./editor- **Export Ready**: Generate high-quality renders for portfolio or presentation

   ```

- **Multiple Geometries**: Spheres and infinite planes with efficient intersection algorithms- **Scene Management**: JSON-based scene serialization for saving and loading complex scenes- `launch.json` - Configuration du débogueur

2. **Create your scene**:

   - Use "Add Object" to insert spheres or planes---

   - Select objects by clicking them in the 3D viewport

   - Adjust properties (position, radius, color, material) in the UI panel- **Material System**: 

   - Configure camera position and orientation

## 🛠️ Technologies & Tools

3. **Configure render settings**:

   - Choose resolution preset or set custom dimensions  - **Diffuse**: Lambertian reflection with realistic light scattering- `settings.json` - Paramètres de l'éditeur

   - Adjust samples per pixel for quality (more = smoother but slower)

   - Set max depth for light bounces (more = more realistic reflections)| Category | Technologies |



4. **Save your scene**:|----------|-------------|  - **Metal**: Configurable roughness for brushed to polished metal surfaces

   - Use "Save Scene" to export as JSON

   - Scenes are saved in `c++/src/data/save/`| **Language** | C++17 |



5. **Render final image**:| **Graphics APIs** | OpenGL 3.3+, GLFW, GLEW |  - **Dielectric**: Glass-like materials with refraction and internal reflectionThis project was developed as a learning experience in computer graphics, demonstrating proficiency in:

   - Click "Render Ray Traced Image"

   - High-quality rendering will begin (may take several minutes)| **UI Framework** | Dear ImGui, ImGuizmo |

   - Output saved as `image.ppm`

| **Build System** | Makefile (GCC/G++) |  - **Emissive**: Self-illuminating materials for light sources

### Material Types Guide

| **Libraries** | nlohmann/json, STL (threading, filesystem) |

- **Diffuse**: Use for matte surfaces (wood, stone, paper)

  - Parameter: Base color| **Development** | VS Code, GDB debugger |  - **Mirror**: Perfect specular reflection- Modern C++ (C++17) with STL## 🚀 Utilisation

  

- **Metal**: Use for metallic surfaces| **Math** | Custom Vec3 implementation for vectors and colors |

  - Parameters: Base color, roughness (0.0 = mirror, 1.0 = rough)

  - **Anti-Aliasing**: Multi-sample per pixel (MSAA) for smooth edges

- **Dielectric**: Use for transparent materials (glass, water)

  - Parameter: Index of refraction (glass ≈ 1.5)---

  

- **Emissive**: Use for light sources- **Gamma Correction**: Proper color space management- 3D mathematics and ray-geometry intersection algorithms

  - Parameters: Emission color, intensity

  ## 🚀 Getting Started

- **Mirror**: Perfect reflective surface

- **Multi-Threading**: Parallel rendering for improved performance

---

### Prerequisites

## 🎓 What I Learned

- OpenGL for real-time rendering### Compilation rapide

This project was a deep dive into computer graphics and software engineering:

```bash

### Computer Graphics Concepts

- **Ray Tracing Fundamentals**: Ray-geometry intersection, Snell's law for refraction, Fresnel equations# Ubuntu/Debian### 🖥️ Interactive 3D Editor

- **Physically-Based Rendering**: BRDF models, importance sampling, Monte Carlo integration

- **Light Transport**: Global illumination, caustics, color bleedingsudo apt-get install build-essential libglfw3-dev libglew-dev

- **Camera Models**: Perspective projection, field of view calculations

- **Post-Processing**: Tone mapping (ACES), gamma correction, denoising filters- **Real-Time OpenGL Preview**: Instant visual feedback while editing scenes- UI/UX design with ImGui```bash



### Software Engineering# Fedora

- **Modern C++ Patterns**: Smart pointers, RAII, const correctness, move semantics

- **Object-Oriented Design**: Polymorphic materials, abstract interfaces, clean architecturesudo dnf install gcc-c++ glfw-devel glew-devel- **ImGui Interface**: Professional, intuitive controls for all scene parameters

- **Build Systems**: Makefiles, dependency management, modular compilation

- **File I/O**: JSON parsing, scene serialization/deserialization



### Graphics Programming# macOS- **3D Gizmo**: Interactive manipulation of objects in 3D space- Multi-threaded application architecture# Compiler le fichier courant

- **OpenGL Pipeline**: Vertex buffers, shaders (GLSL), framebuffers

- **Real-Time Rendering**: Depth testing, backface culling, wireframe renderingbrew install glfw glew

- **UI/UX Development**: ImGui integration, event handling, user interaction patterns

- **3D Mathematics**: Vector operations, matrix transformations, coordinate spaces```- **Camera Controls**: 



### Tools & Workflow

- **Version Control**: Git for iterative development

- **Debugging**: GDB for C++ debugging, visual debugging techniques### Building the Project  - Orbit rotation with mouse drag- Software engineering best practicesg++ -std=c++17 -Wall -o test test.cpp

- **Cross-Platform Development**: Writing portable C++ code



---

The project includes three main executables:  - Zoom with mouse wheel

## 📊 Project Statistics



- **Lines of Code**: ~3,500+ lines of C++

- **Development Time**: Several weeks of intensive development#### 1. **Main Ray Tracer** (Command-line renderer)  - Pan with middle mouse button

- **Files**: 30+ source files with modular architecture

- **External Libraries**: 3 (ImGui, ImGuizmo, nlohmann/json)```bash



---cd c++- **Ray Picking**: Click objects to select them directly in the viewport



## 📚 Documentationmake main



- **[BUILD.md](docs/BUILD.md)** - Detailed build instructions and troubleshooting./main- **Wireframe Mode**: Toggle between solid and wireframe rendering---# Avec optimisations

- **[ARCHITECTURE.md](docs/ARCHITECTURE.md)** - Technical implementation details

- **[ACHIEVEMENTS.md](docs/ACHIEVEMENTS.md)** - Project highlights and learning outcomes```



---- **Scene Hierarchy**: Manage multiple objects with easy add/remove functionality



## 🗺️ Future Enhancements#### 2. **Interactive Editor** (3D scene editor with OpenGL preview)



- [ ] **Multi-Threading**: Parallel rendering for faster computation```bashg++ -std=c++17 -Wall -O3 -o test test.cpp

- [ ] **Triangle Mesh Support**: Load and render OBJ/STL 3D models

- [ ] **Texture Mapping**: UV coordinates and image-based texturescd c++

- [ ] **BVH Acceleration**: Bounding Volume Hierarchy for 10-100x performance improvement

- [ ] **Point Lights**: Explicit light sources with shadow raysmake editor### ⚙️ Rendering Configuration

- [ ] **Advanced Materials**: Subsurface scattering, anisotropic reflections

- [ ] **Post-Processing**: Bloom, depth of field, motion blur./editor

- [ ] **GPU Acceleration**: CUDA or OpenCL implementation for real-time path tracing

```- **Resolution Presets**: Quick selection from 480p to 4K (3840×2160)## ✨ Key Features```

---



## 📄 License

#### 3. **Real-Time Preview** (OpenGL-only fast preview)- **Custom Resolution**: Fine-tune width and height independently

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

```bash

---

cd c++- **Quality Settings**:

## 🙏 Acknowledgments

make preview

- **Ray Tracing in One Weekend** series by Peter Shirley for foundational concepts

- **Dear ImGui** by Omar Cornut for the excellent immediate mode GUI library./preview  - Samples per pixel (1-1000+) for anti-aliasing quality

- **ImGuizmo** by CedricGuillemet for 3D manipulation gizmos

- **nlohmann/json** for elegant JSON parsing in C++```



---  - Max ray bounce depth (1-50) for light transport accuracy### 🎨 Ray Tracing Engine### Utilisation du Makefile



## 📬 Contact### Quick Render



**Alex Amedro** - Looking for M1 internship opportunities in Computer Graphics, Game Development, or Software Engineering  - Gamma correction factor



- GitHub: [@Alex-Amedro](https://github.com/Alex-Amedro)```bash

- Project Link: [https://github.com/Alex-Amedro/Rayt](https://github.com/Alex-Amedro/Rayt)

# Render with default settings- **Performance Options**: Configurable thread count for optimal CPU utilization- **Path Tracing**: Physically accurate light simulation with configurable bounce depth```bash

---

./main

<p align="center">

  <em>Built with ❤️ and lots of ☕ by Alex Amedro</em>

</p>

# The output will be saved as image.ppm

# Convert to PNG (requires ImageMagick)### 💾 Scene Management- **Multiple Geometries**: Spheres and infinite planes with efficient intersection algorithms# Compiler tout le projet

convert image.ppm output.png

```- **JSON Serialization**: Save and load complete scenes



---- **Persistent Settings**: Camera position, render settings, and object properties- **Material System**: make



## 📖 Usage Guide- **Export Ready**: Generate high-quality renders for portfolio or presentation



### Using the Editor  - **Diffuse**: Lambertian reflection with realistic light scattering



1. **Launch the editor**:---

   ```bash

   ./editor  - **Metal**: Configurable roughness for brushed to polished metal surfaces# Compiler en mode debug

   ```

## 🛠️ Technologies & Tools

2. **Create your scene**:

   - Use "Add Object" to insert spheres or planes  - **Dielectric**: Glass-like materials with refraction and internal reflectionmake debug

   - Select objects by clicking them in the 3D viewport

   - Adjust properties (position, radius, color, material) in the UI panel| Category | Technologies |

   - Configure camera position and orientation

|----------|-------------|  - **Emissive**: Self-illuminating materials for light sources

3. **Configure render settings**:

   - Choose resolution preset or set custom dimensions| **Language** | C++17 |

   - Adjust samples per pixel for quality (more = smoother but slower)

   - Set max depth for light bounces (more = more realistic reflections)| **Graphics APIs** | OpenGL 3.3+, GLFW, GLEW |  - **Mirror**: Perfect specular reflection# Nettoyer les fichiers compilés



4. **Save your scene**:| **UI Framework** | Dear ImGui, ImGuizmo |

   - Use "Save Scene" to export as JSON

   - Scenes are saved in `c++/src/data/save/`| **Build System** | Makefile (GCC/G++) |- **Anti-Aliasing**: Multi-sample per pixel (MSAA) for smooth edgesmake clean



5. **Render final image**:| **Libraries** | nlohmann/json, STL (threading, filesystem) |

   - Click "Render Ray Traced Image"

   - High-quality rendering will begin (may take several minutes)| **Development** | VS Code, GDB debugger |- **Gamma Correction**: Proper color space management

   - Output saved as `image.ppm`

| **Math** | Custom Vec3 implementation for vectors and colors |

### Material Types Guide

- **Multi-Threading**: Parallel rendering for improved performance# Compiler et exécuter

- **Diffuse**: Use for matte surfaces (wood, stone, paper)

  - Parameter: Base color---

  

- **Metal**: Use for metallic surfacesmake run

  - Parameters: Base color, roughness (0.0 = mirror, 1.0 = rough)

  ## 🚀 Getting Started

- **Dielectric**: Use for transparent materials (glass, water)

  - Parameter: Index of refraction (glass ≈ 1.5)### 🖥️ Interactive 3D Editor

  

- **Emissive**: Use for light sources### Prerequisites

  - Parameters: Emission color, intensity

  - **Real-Time OpenGL Preview**: Instant visual feedback while editing scenes# Recompiler complètement

- **Mirror**: Perfect reflective surface

```bash

---

# Ubuntu/Debian- **ImGui Interface**: Professional, intuitive controls for all scene parametersmake rebuild

## 🎓 What I Learned

sudo apt-get install build-essential libglfw3-dev libglew-dev

This project was a deep dive into computer graphics and software engineering:

- **3D Gizmo**: Interactive manipulation of objects in 3D space```

### Computer Graphics Concepts

- **Ray Tracing Fundamentals**: Ray-geometry intersection, Snell's law for refraction, Fresnel equations# Fedora

- **Physically-Based Rendering**: BRDF models, importance sampling, Monte Carlo integration

- **Light Transport**: Global illumination, caustics, color bleedingsudo dnf install gcc-c++ glfw-devel glew-devel- **Camera Controls**: 

- **Camera Models**: Perspective projection, field of view calculations



### Software Engineering

- **Modern C++ Patterns**: Smart pointers, RAII, const correctness, move semantics# macOS  - Orbit rotation with mouse drag### Raccourcis VS Code

- **Object-Oriented Design**: Polymorphic materials, abstract interfaces, clean architecture

- **Concurrent Programming**: Multi-threading with `std::thread`, parallel design patternsbrew install glfw glew

- **Build Systems**: Makefiles, dependency management, modular compilation

```  - Zoom with mouse wheel

### Graphics Programming

- **OpenGL Pipeline**: Vertex buffers, shaders (GLSL), framebuffers

- **Real-Time Rendering**: Depth testing, backface culling, wireframe rendering

- **UI/UX Development**: ImGui integration, event handling, user interaction patterns### Building the Project  - Pan with middle mouse button#### Compilation



### Tools & Workflow

- **Version Control**: Git for iterative development

- **Debugging**: GDB for C++ debugging, visual debugging techniquesThe project includes three main executables:- **Ray Picking**: Click objects to select them directly in the viewport- **Ctrl+Shift+B** : Build (compile le fichier actif)

- **Cross-Platform Development**: Writing portable C++ code



---

#### 1. **Main Ray Tracer** (Command-line renderer)- **Wireframe Mode**: Toggle between solid and wireframe rendering- **F5** : Build + Debug (compile et lance le débogueur)

## 📊 Project Statistics

```bash

- **Lines of Code**: ~3,500+ lines of C++

- **Development Time**: Several weeks of intensive developmentcd c++- **Scene Hierarchy**: Manage multiple objects with easy add/remove functionality

- **Files**: 30+ source files with modular architecture

- **External Libraries**: 3 (ImGui, ImGuizmo, nlohmann/json)make main



---./main#### Tâches disponibles



## 📚 Documentation```



- **[BUILD.md](docs/BUILD.md)** - Detailed build instructions and troubleshooting### ⚙️ Rendering Configuration1. **C/C++: g++ build active file** - Compile avec debug (-g)

- **[ARCHITECTURE.md](docs/ARCHITECTURE.md)** - Technical implementation details

- **[ACHIEVEMENTS.md](docs/ACHIEVEMENTS.md)** - Project highlights and learning outcomes#### 2. **Interactive Editor** (3D scene editor with OpenGL preview)



---```bash- **Resolution Presets**: Quick selection from 480p to 4K (3840×2160)2. **C/C++: g++ build optimized** - Compile avec optimisations (-O3)



## 🗺️ Future Enhancementscd c++



- [ ] **Triangle Mesh Support**: Load and render OBJ/STL 3D modelsmake editor- **Custom Resolution**: Fine-tune width and height independently3. **Build and Run** - Compile et exécute automatiquement

- [ ] **Texture Mapping**: UV coordinates and image-based textures

- [ ] **BVH Acceleration**: Bounding Volume Hierarchy for 10-100x performance improvement./editor

- [ ] **Advanced Materials**: Subsurface scattering, anisotropic reflections

- [ ] **Post-Processing**: Bloom, depth of field, motion blur```- **Quality Settings**:

- [ ] **GPU Acceleration**: CUDA or OpenCL implementation for real-time path tracing



---

#### 3. **Real-Time Preview** (OpenGL-only fast preview)  - Samples per pixel (1-1000+) for anti-aliasing quality### Debug

## 📄 License

```bash

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

cd c++  - Max ray bounce depth (1-50) for light transport accuracy1. Place des breakpoints en cliquant à gauche des numéros de ligne

---

make preview

## 🙏 Acknowledgments

./preview  - Gamma correction factor2. Appuie sur **F5** pour démarrer le debug

- **Ray Tracing in One Weekend** series by Peter Shirley for foundational concepts

- **Dear ImGui** by Omar Cornut for the excellent immediate mode GUI library```

- **ImGuizmo** by CedricGuillemet for 3D manipulation gizmos

- **nlohmann/json** for elegant JSON parsing in C++- **Performance Options**: Configurable thread count for optimal CPU utilization3. Utilise :



---### Quick Render



## 📬 Contact   - **F10** : Step Over (ligne suivante)



**Alex Amedro** - Looking for M1 internship opportunities in Computer Graphics, Game Development, or Software Engineering```bash



- GitHub: [@Alex-Amedro](https://github.com/Alex-Amedro)# Render with default settings### 💾 Scene Management   - **F11** : Step Into (entre dans la fonction)

- Project Link: [https://github.com/Alex-Amedro/Rayt](https://github.com/Alex-Amedro/Rayt)

./main

---

- **JSON Serialization**: Save and load complete scenes   - **Shift+F11** : Step Out (sort de la fonction)

<p align="center">

  <em>Built with ❤️ and lots of ☕ by Alex Amedro</em># The output will be saved as image.ppm

</p>

# Convert to PNG (requires ImageMagick)- **Persistent Settings**: Camera position, render settings, and object properties   - **F5** : Continue

convert image.ppm output.png

```- **Export Ready**: Generate high-quality renders for portfolio or presentation



---## 📁 Structure du projet



## 📖 Usage Guide---



### Using the Editor```



1. **Launch the editor**:## 🛠️ Technologies & ToolsRayt/

   ```bash

   ./editor├── .vscode/              # Configuration VS Code

   ```

| Category | Technologies |│   ├── c_cpp_properties.json

2. **Create your scene**:

   - Use "Add Object" to insert spheres or planes|----------|-------------|│   ├── tasks.json

   - Select objects by clicking them in the 3D viewport

   - Adjust properties (position, radius, color, material) in the UI panel| **Language** | C++17 |│   ├── launch.json

   - Configure camera position and orientation

| **Graphics APIs** | OpenGL 3.3+, GLFW, GLEW |│   └── settings.json

3. **Configure render settings**:

   - Choose resolution preset or set custom dimensions| **UI Framework** | Dear ImGui, ImGuizmo |├── test.cpp              # Fichier de test C++

   - Adjust samples per pixel for quality (more = smoother but slower)

   - Set max depth for light bounces (more = more realistic reflections)| **Build System** | Makefile (GCC/G++) |├── 1.py                  # Scripts Python



4. **Save your scene**:| **Libraries** | nlohmann/json, STL (threading, filesystem) |├── 2.py

   - Use "Save Scene" to export as JSON

   - Scenes are saved in `c++/src/data/save/`| **Development** | VS Code, GDB debugger |├── Makefile              # Système de build



5. **Render final image**:| **Math** | Custom Vec3 implementation for vectors and colors |├── .gitignore            # Fichiers ignorés par git

   - Click "Render Ray Traced Image"

   - High-quality rendering will begin (may take several minutes)└── README.md             # Ce fichier

   - Output saved as `image.ppm`

---```

### Material Types Guide



- **Diffuse**: Use for matte surfaces (wood, stone, paper)

  - Parameter: Base color## 🚀 Getting Started## 🎯 Standards utilisés

  

- **Metal**: Use for metallic surfaces

  - Parameters: Base color, roughness (0.0 = mirror, 1.0 = rough)

  ### Prerequisites- **C++ Standard** : C++17

- **Dielectric**: Use for transparent materials (glass, water)

  - Parameter: Index of refraction (glass ≈ 1.5)- **Compilateur** : g++ 13.3.0

  

- **Emissive**: Use for light sources```bash- **Flags de compilation** :

  - Parameters: Emission color, intensity

  # Ubuntu/Debian  - `-std=c++17` : Standard C++17

- **Mirror**: Perfect reflective surface

sudo apt-get install build-essential libglfw3-dev libglew-dev  - `-Wall` : Tous les warnings

---

  - `-Wextra` : Warnings supplémentaires

## 🎓 What I Learned

# Fedora  - `-O2` / `-O3` : Optimisations

This project was a deep dive into computer graphics and software engineering:

sudo dnf install gcc-c++ glfw-devel glew-devel  - `-g` : Informations de debug

### Computer Graphics Concepts

- **Ray Tracing Fundamentals**: Ray-geometry intersection, Snell's law for refraction, Fresnel equations

- **Physically-Based Rendering**: BRDF models, importance sampling, Monte Carlo integration

- **Light Transport**: Global illumination, caustics, color bleeding# macOS## 📚 Ressources utiles

- **Camera Models**: Perspective projection, field of view calculations

brew install glfw glew

### Software Engineering

- **Modern C++ Patterns**: Smart pointers, RAII, const correctness, move semantics```- [Documentation C++](https://en.cppreference.com/)

- **Object-Oriented Design**: Polymorphic materials, abstract interfaces, clean architecture

- **Performance Optimization**: Multi-threading, cache-friendly data structures, algorithmic improvements- [Ray Tracing in One Weekend](https://raytracing.github.io/)

- **Build Systems**: Makefiles, dependency management, modular compilation

### Building the Project- [Learn OpenGL](https://learnopengl.com/)

### Graphics Programming

- **OpenGL Pipeline**: Vertex buffers, shaders (GLSL), framebuffers

- **Real-Time Rendering**: Depth testing, backface culling, wireframe rendering

- **UI/UX Development**: ImGui integration, event handling, user interaction patternsThe project includes three main executables:## 💡 Prochaines étapes



### Tools & Workflow

- **Version Control**: Git for iterative development

- **Debugging**: GDB for C++ debugging, visual debugging techniques#### 1. **Main Ray Tracer** (Command-line renderer)1. Implémente ta classe Vector3D

- **Cross-Platform Development**: Writing portable C++ code

```bash2. Crée la classe Ray

---

cd c++3. Ajoute les primitives (sphères, plans)

## 📊 Project Statistics

make main4. Implémente le calcul d'intersection

- **Lines of Code**: ~3,500+ lines of C++

- **Development Time**: Several weeks of intensive development./main5. Gère l'éclairage et les ombres

- **Files**: 30+ source files with modular architecture

- **External Libraries**: 3 (ImGui, ImGuizmo, nlohmann/json)```6. Ajoute les matériaux et réflexions



---



## 📚 Documentation#### 2. **Interactive Editor** (3D scene editor with OpenGL preview)Bon coding ! 🚀



- **[BUILD.md](docs/BUILD.md)** - Detailed build instructions and troubleshooting```bash

- **[ARCHITECTURE.md](docs/ARCHITECTURE.md)** - Technical implementation detailscd c++

- **[ACHIEVEMENTS.md](docs/ACHIEVEMENTS.md)** - Project highlights and learning outcomesmake editor

./editor

---```



## 🗺️ Future Enhancements#### 3. **Real-Time Preview** (OpenGL-only fast preview)

```bash

- [ ] **Triangle Mesh Support**: Load and render OBJ/STL 3D modelscd c++

- [ ] **Texture Mapping**: UV coordinates and image-based texturesmake preview

- [ ] **BVH Acceleration**: Bounding Volume Hierarchy for faster ray intersection./preview

- [ ] **Advanced Materials**: Subsurface scattering, anisotropic reflections```

- [ ] **Post-Processing**: Bloom, depth of field, motion blur

- [ ] **GPU Acceleration**: CUDA or OpenCL implementation for real-time path tracing### Quick Render



---```bash

# Render with default settings

## 📄 License./main



This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.# The output will be saved as image.ppm

# Convert to PNG (requires ImageMagick)

---convert image.ppm output.png

```

## 🙏 Acknowledgments

---

- **Ray Tracing in One Weekend** series by Peter Shirley for foundational concepts

- **Dear ImGui** by Omar Cornut for the excellent immediate mode GUI library## 📖 Usage Guide

- **ImGuizmo** by CedricGuillemet for 3D manipulation gizmos

- **nlohmann/json** for elegant JSON parsing in C++### Using the Editor



---1. **Launch the editor**:

   ```bash

## 📬 Contact   ./editor

   ```

**Alex Amedro** - Looking for M1 internship opportunities in Computer Graphics, Game Development, or Software Engineering

2. **Create your scene**:

- GitHub: [@Alex-Amedro](https://github.com/Alex-Amedro)   - Use "Add Object" to insert spheres or planes

- Project Link: [https://github.com/Alex-Amedro/Rayt](https://github.com/Alex-Amedro/Rayt)   - Select objects by clicking them in the 3D viewport

   - Adjust properties (position, radius, color, material) in the UI panel

---   - Configure camera position and orientation



<p align="center">3. **Configure render settings**:

  <em>Built with ❤️ and lots of ☕ by Alex Amedro</em>   - Choose resolution preset or set custom dimensions

</p>   - Adjust samples per pixel for quality (more = smoother but slower)

   - Set max depth for light bounces (more = more realistic reflections)

4. **Save your scene**:
   - Use "Save Scene" to export as JSON
   - Scenes are saved in `c++/src/data/save/`

5. **Render final image**:
   - Click "Render Ray Traced Image"
   - High-quality rendering will begin (may take several minutes)
   - Output saved as `image.ppm`

### Material Types Guide

- **Diffuse**: Use for matte surfaces (wood, stone, paper)
  - Parameter: Base color
  
- **Metal**: Use for metallic surfaces
  - Parameters: Base color, roughness (0.0 = mirror, 1.0 = rough)
  
- **Dielectric**: Use for transparent materials (glass, water)
  - Parameter: Index of refraction (glass ≈ 1.5)
  
- **Emissive**: Use for light sources
  - Parameters: Emission color, intensity
  
- **Mirror**: Perfect reflective surface

---

## 🎓 What I Learned

This project was a deep dive into computer graphics and software engineering:

### Computer Graphics Concepts
- **Ray Tracing Fundamentals**: Ray-geometry intersection, Snell's law for refraction, Fresnel equations
- **Physically-Based Rendering**: BRDF models, importance sampling, Monte Carlo integration
- **Light Transport**: Global illumination, caustics, color bleeding
- **Camera Models**: Perspective projection, field of view calculations

### Software Engineering
- **Modern C++ Patterns**: Smart pointers, RAII, const correctness, move semantics
- **Object-Oriented Design**: Polymorphic materials, abstract interfaces, clean architecture
- **Performance Optimization**: Multi-threading, cache-friendly data structures, algorithmic improvements
- **Build Systems**: Makefiles, dependency management, modular compilation

### Graphics Programming
- **OpenGL Pipeline**: Vertex buffers, shaders (GLSL), framebuffers
- **Real-Time Rendering**: Depth testing, backface culling, wireframe rendering
- **UI/UX Development**: ImGui integration, event handling, user interaction patterns

### Tools & Workflow
- **Version Control**: Git for iterative development
- **Debugging**: GDB for C++ debugging, visual debugging techniques
- **Cross-Platform Development**: Writing portable C++ code

---

## 📊 Project Statistics

- **Lines of Code**: ~3,500+ lines of C++
- **Development Time**: Several weeks of intensive development
- **Files**: 30+ source files with modular architecture
- **External Libraries**: 3 (ImGui, ImGuizmo, nlohmann/json)

---

## 🖼️ Gallery

<p align="center">
  <img src="docs/editor_screenshot.png" alt="Interactive Editor" width="600"/>
  <br/>
  <em>Interactive 3D Editor with real-time OpenGL preview</em>
</p>

<p align="center">
  <img src="docs/materials_showcase.png" alt="Material Showcase" width="600"/>
  <br/>
  <em>Showcase of different material types: metal, glass, diffuse, emissive</em>
</p>

---

## 🗺️ Future Enhancements

- [ ] **Triangle Mesh Support**: Load and render OBJ/STL 3D models
- [ ] **Texture Mapping**: UV coordinates and image-based textures
- [ ] **BVH Acceleration**: Bounding Volume Hierarchy for faster ray intersection
- [ ] **Advanced Materials**: Subsurface scattering, anisotropic reflections
- [ ] **Post-Processing**: Bloom, depth of field, motion blur
- [ ] **GPU Acceleration**: CUDA or OpenCL implementation for real-time path tracing

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

- **Ray Tracing in One Weekend** series by Peter Shirley for foundational concepts
- **Dear ImGui** by Omar Cornut for the excellent immediate mode GUI library
- **ImGuizmo** by CedricGuillemet for 3D manipulation gizmos
- **nlohmann/json** for elegant JSON parsing in C++

---

## 📬 Contact

**Alex Amedro** - Looking for M1 internship opportunities in Computer Graphics, Game Development, or Software Engineering

- GitHub: [@Alex-Amedro](https://github.com/Alex-Amedro)
- Project Link: [https://github.com/Alex-Amedro/Rayt](https://github.com/Alex-Amedro/Rayt)

---

<p align="center">
  <em>Built with ❤️ and lots of ☕ by Alex Amedro</em>
</p>
