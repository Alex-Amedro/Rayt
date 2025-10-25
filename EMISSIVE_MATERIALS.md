# 🎨 Nouveaux Matériaux & Contrôles ACES

## ✨ **Matériaux Implémentés**

### 1. **Néon / Émissif** 
- **Type**: `"Néon"` ou `"Emissive"` dans JSON
- **Propriétés**:
  - `emission_strength`: 1.0 à 20.0+ (5.0 = néon classique, 10.0+ = très brillant)
- **Utilisation**: Sources lumineuses HDR (néons, LEDs, écrans)
- **Impact ACES**: Préserve les couleurs saturées brillantes sans les cramer en blanc

### 2. **Miroir**
- **Type**: `"Miroir"` ou `"Mirror"` dans JSON
- **Propriétés**: `color` pour teinte optionnelle (1,1,1 = miroir pur)
- **Utilisation**: Chrome, réflexions parfaites
- **Impact visuel**: Reflets nets avec ACES pour détails préservés

### 3. **Lumière Ambiante Contrôlable**
- **Paramètre**: `ambient_light` dans `render` (0.0 à 1.0)
  - `1.0` = Ciel complet (défaut)
  - `0.2` = Ambiance dramatique (recommandé pour néons)
  - `0.0` = Noir total (scènes cyberpunk)
- **Utilisation**: Contraste pour sources émissives

---

## 🚀 **Utilisation dans l'Éditeur**

1. **Lance l'éditeur** : `./editor`
2. **Sélectionne un objet** dans la liste
3. **Change le matériau** :
   - Combo "Type" → `Néon` ou `Miroir`
   - Si Néon : Slider "Intensité lumineuse" (1.0 - 20.0)
4. **Ajuste l'ambiance** :
   - Render Settings → "Ambient Light" (0.0 - 1.0)
   - Pour scènes néon : `0.15 - 0.3` recommandé
5. **Sauvegarde** : Bouton "Save" → `demo_scene.json`
6. **Render** : `./rayt`

---

## 🎬 **Scène Showcase Pré-faite**

**Fichier** : `src/data/save/neon_showcase.json`

**Contient** :
- 3 néons colorés (rouge, cyan, violet) HDR
- Miroirs chrome et or pour reflets
- Sol miroir pour ambiance cyberpunk
- Ambient light = 0.15 (noir dramatique)
- Verre + métal rugueux pour variété

**Test rapide** :
```bash
# Copie la scène showcase
cp src/data/save/neon_showcase.json src/data/save/demo_scene.json

# Render
./rayt
```

**Comparaison ACES :**
- **Sans ACES** : Néons = blanc cramé, sans couleur
- **Avec ACES** : Néons = couleurs vibrantes préservées avec halos naturels

---

## 🎯 **Pourquoi ACES Brille Maintenant**

### Avant (matériaux diffus seulement) :
- Valeurs couleur = [0, 1]
- Pas d'HDR → ACES inutile
- Gamma seul suffit

### Maintenant (avec émissifs + miroirs) :
- Néons = couleurs > 1.0 (ex: 10.0 pour rouge saturé)
- Miroirs = reflets brillants HDR
- **ACES compresse** ces valeurs sans les cramer
- **Gamma seul** cramait tout en blanc

### Résultat :
✅ Néons rouges restent **rouges brillants** (pas blancs)  
✅ Reflets chrome gardent les **détails**  
✅ Couleurs **naturelles style cinéma**

---

## 📊 **Paramètres Recommandés Portfolio**

**Scène Néon Dramatique** :
```json
"ambient_light": 0.2,
"samples_per_pixel": 200,
"emission_strength": 8.0,
"gamma": 2.2
```

**Scène Chrome Jour** :
```json
"ambient_light": 1.0,
"samples_per_pixel": 100,
"gamma": 2.2
```

---

## 🔧 **Intégration Code**

### main.cpp
- Includes: `emissive.hpp`, `mirror.hpp`
- Parsing JSON: Support `"Néon"`, `"Miroir"`, `emission_strength`
- Ray color: Passage de `ambient_light`

### ray_color.cpp
- Ajout `emitted()` sur chaque hit
- Skybox modulé par `ambient_light`
- Support matériaux non-scattering (émissifs)

### editor_ui.cpp
- Nouveaux matériaux dans combo (5 types)
- Slider "Intensité lumineuse" pour émissifs
- Slider "Ambient Light" dans Render Settings
- Save/Load de `emission_strength` et `ambient_light` en JSON

---

## 🎨 **Prochaine Étape : Point Lights**

Avec les émissifs fonctionnels, la prochaine étape naturelle :
- **Point Lights** avec shadow rays
- **3-point lighting** pour scènes hero
- Impact portfolio : **5/5** ⭐

Voir `QUICKSTART_LIGHTS.md` pour le guide d'implémentation !
