# Rayt - Ray Tracing Engine

Moteur de ray tracing physiquement réaliste écrit en C++17 avec éditeur interactif OpenGL.

## Technologies

- **Langage**: C++17
- **Graphiques**: OpenGL 3.3+, GLFW, GLEW, GLM
- **Interface**: Dear ImGui, ImGuizmo
- **Build**: Makefile (GCC/G++)
- **Librairies**: nlohmann/json, STL

## Fonctionnalités

### Ray Tracing
- Path tracing avec illumination globale
- Géométries: sphères, plans infinis
- Matériaux: diffus (Lambertian), métal, diélectrique (verre), émissif, miroir
- Anti-aliasing (MSAA)
- Tone mapping ACES
- Correction gamma
- Filtres de débruitage (box, Gaussian, bilateral)

### Éditeur Interactif
- Prévisualisation temps réel OpenGL
- Interface ImGui
- Gizmo 3D pour manipuler les objets
- Contrôles caméra (rotation, zoom, pan)
- Ray picking pour sélection d'objets
- Mode wireframe
- Sérialisation JSON des scènes

## Installation

### Dépendances

```bash
# Ubuntu/Debian
sudo apt-get install build-essential libglfw3-dev libglew-dev libglm-dev

# Fedora
sudo dnf install gcc-c++ glfw-devel glew-devel glm-devel

# macOS
brew install glfw glew glm
```

### Compilation

```bash
cd c++

# Éditeur interactif
make editor
./editor

# Ray tracer ligne de commande
make main
./main

# Prévisualisation OpenGL
make preview
./preview
```

## Utilisation

### Éditeur

1. Lancer `./editor`
2. Ajouter des objets (sphères, plans)
3. Modifier les propriétés (position, couleur, matériaux)
4. Configurer le rendu (résolution, samples, bounces)
5. Sauvegarder la scène en JSON
6. Lancer le rendu final (sortie: `image.ppm`)

### Paramètres de rendu

- **Résolution**: 480p à 4K (ou personnalisée)
- **Samples per pixel**: qualité anti-aliasing (1-1000+)
- **Max depth**: bounces de lumière (1-50)
- **Gamma**: correction gamma
- **Ambient**: lumière ambiante

### Matériaux

- **Diffuse**: surfaces mates
- **Metal**: surfaces métalliques (paramètre: rugosité)
- **Dielectric**: matériaux transparents (paramètre: indice de réfraction)
- **Emissive**: sources lumineuses
- **Mirror**: réflexion spéculaire parfaite

## Structure

```
c++/
├── src/
│   ├── core/          # Ray tracing (Ray, Camera, Scene)
│   ├── geometry/      # Primitives (Sphere, Plane)
│   ├── materials/     # Matériaux
│   ├── utils/         # Vec3, utilitaires
│   ├── rendering/     # Post-processing
│   └── editor/        # Interface OpenGL/ImGui
├── include/
├── lib/               # Librairies externes
└── data/save/         # Scènes sauvegardées
```

## Makefile

```bash
make          # Compile tout
make main     # Compile le ray tracer
make editor   # Compile l'éditeur
make preview  # Compile la preview
make clean    # Nettoie les builds
make rebuild  # Recompile tout
```

## License

MIT License - voir [LICENSE](LICENSE)

## Auteur

Alex Amedro - Recherche stage M1 en Computer Graphics / Game Development

GitHub: [@Alex-Amedro](https://github.com/Alex-Amedro)
