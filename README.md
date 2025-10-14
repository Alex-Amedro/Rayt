# Rayt - Ray Tracer Project

Projet de ray tracing en C++.

## 🛠️ Configuration de l'environnement

### Prérequis installés
- ✅ **g++** (version 13.3.0) - Compilateur C++
- ✅ **gdb** (version 15.0) - Débogueur
- ✅ **VS Code Extensions** :
  - C/C++ IntelliSense (ms-vscode.cpptools)
  - CMake Tools
  - C/C++ Extension Pack

### Configuration VS Code
Les fichiers de configuration suivants ont été créés dans `.vscode/` :
- `c_cpp_properties.json` - Configuration IntelliSense (C++17)
- `tasks.json` - Tâches de compilation
- `launch.json` - Configuration du débogueur
- `settings.json` - Paramètres de l'éditeur

## 🚀 Utilisation

### Compilation rapide
```bash
# Compiler le fichier courant
g++ -std=c++17 -Wall -o test test.cpp

# Avec optimisations
g++ -std=c++17 -Wall -O3 -o test test.cpp
```

### Utilisation du Makefile
```bash
# Compiler tout le projet
make

# Compiler en mode debug
make debug

# Nettoyer les fichiers compilés
make clean

# Compiler et exécuter
make run

# Recompiler complètement
make rebuild
```

### Raccourcis VS Code

#### Compilation
- **Ctrl+Shift+B** : Build (compile le fichier actif)
- **F5** : Build + Debug (compile et lance le débogueur)

#### Tâches disponibles
1. **C/C++: g++ build active file** - Compile avec debug (-g)
2. **C/C++: g++ build optimized** - Compile avec optimisations (-O3)
3. **Build and Run** - Compile et exécute automatiquement

### Debug
1. Place des breakpoints en cliquant à gauche des numéros de ligne
2. Appuie sur **F5** pour démarrer le debug
3. Utilise :
   - **F10** : Step Over (ligne suivante)
   - **F11** : Step Into (entre dans la fonction)
   - **Shift+F11** : Step Out (sort de la fonction)
   - **F5** : Continue

## 📁 Structure du projet

```
Rayt/
├── .vscode/              # Configuration VS Code
│   ├── c_cpp_properties.json
│   ├── tasks.json
│   ├── launch.json
│   └── settings.json
├── test.cpp              # Fichier de test C++
├── 1.py                  # Scripts Python
├── 2.py
├── Makefile              # Système de build
├── .gitignore            # Fichiers ignorés par git
└── README.md             # Ce fichier
```

## 🎯 Standards utilisés

- **C++ Standard** : C++17
- **Compilateur** : g++ 13.3.0
- **Flags de compilation** :
  - `-std=c++17` : Standard C++17
  - `-Wall` : Tous les warnings
  - `-Wextra` : Warnings supplémentaires
  - `-O2` / `-O3` : Optimisations
  - `-g` : Informations de debug

## 📚 Ressources utiles

- [Documentation C++](https://en.cppreference.com/)
- [Ray Tracing in One Weekend](https://raytracing.github.io/)
- [Learn OpenGL](https://learnopengl.com/)

## 💡 Prochaines étapes

1. Implémente ta classe Vector3D
2. Crée la classe Ray
3. Ajoute les primitives (sphères, plans)
4. Implémente le calcul d'intersection
5. Gère l'éclairage et les ombres
6. Ajoute les matériaux et réflexions

Bon coding ! 🚀
