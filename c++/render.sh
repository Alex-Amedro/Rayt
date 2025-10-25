#!/bin/bash
# Script pour lancer le raytracer sans crasher l'éditeur

echo "🎨 Lancement du Ray Tracer..."
echo ""

# Vérifier si l'éditeur tourne
if pgrep -f "./editor" > /dev/null; then
    echo "⚠️  WARNING: L'éditeur est en cours d'exécution!"
    echo "   Pour éviter les crashs OOM (Out of Memory):"
    echo "   - Fermez l'éditeur AVANT de lancer le raytracer"
    echo "   - OU lancez dans un terminal séparé"
    echo ""
    read -p "Continuer quand même? (y/N) " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        echo "Annulé."
        exit 0
    fi
fi

# Vérifier que demo_scene.json existe
if [ ! -f "src/data/save/demo_scene.json" ]; then
    echo "❌ Erreur: src/data/save/demo_scene.json introuvable!"
    echo "   Sauvegardez une scène depuis l'éditeur d'abord."
    exit 1
fi

echo "✓ Scène trouvée: src/data/save/demo_scene.json"
echo ""
echo "Compilation..."
make 2>&1 | grep -E "(Compiling|Linking|complete|error)" || make

if [ $? -ne 0 ]; then
    echo "❌ Erreur de compilation!"
    exit 1
fi

echo ""
echo "🚀 Lancement du raytracer..."
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""

./rayt

if [ $? -eq 0 ]; then
    echo ""
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo "✅ Rendu terminé!"
    echo "📸 Image: c++/image.ppm"
    echo ""
    echo "Pour visualiser:"
    echo "  - eog image.ppm      (Eye of GNOME)"
    echo "  - gimp image.ppm     (GIMP)"
    echo "  - feh image.ppm      (Feh image viewer)"
else
    echo ""
    echo "❌ Erreur durant le rendu (code: $?)"
fi
