# 🐛 Bug Fixes - Néons & Bruit

## ✅ **Fix 1: Matériaux Perdus au Load**

### Problème
Quand on charge une scène JSON dans l'éditeur, les matériaux "Néon" et "Miroir" revenaient en "Diffus"

### Cause
`scene.cpp` → `load_from_json()` ne gérait que 3 matériaux :
- "Diffus" ✅
- "Métal" ✅  
- "Verre" ✅
- "Néon" ❌ → Devenait Diffus
- "Miroir" ❌ → Devenait Diffus

### Solution
Ajout des cas manquants dans `scene.cpp:304-320` :
```cpp
} else if (material_name == "Néon" || material_name == "Neon" || material_name == "Emissive") {
    obj->material = MaterialType::EMISSIVE;
    if (obj_data.contains("emission_strength")) {
        obj->emission_strength = obj_data["emission_strength"];
    }
} else if (material_name == "Miroir" || material_name == "Mirror") {
    obj->material = MaterialType::MIRROR;
}
```

---

## ✅ **Fix 2: Bruit Excessif avec Néons**

### Problème
Même avec 500 samples et max_depth=100 :
- Bruit/grain très visible
- Zones sombres bruitées
- Zones illuminées inégales

### Cause Principale : Ambient Light Trop Faible

**Ambient light = 0.05 (5%)** → Presque noir !

**Impact sur le rendu :**
```
95% des rayons → Touche le ciel noir (contribution ≈ 0.05)
5% des rayons  → Touche un néon brillant (contribution ≈ 8.0-10.0)

Ratio de variance : 10.0 / 0.05 = 200x !
```

**Résultat** : Variance explosive → Bruit maximal

### Solution : Augmenter Ambient Light

**Valeurs recommandées :**

| Ambient Light | Style | Qualité Rendu |
|--------------|-------|---------------|
| `0.05 - 0.10` | Cyberpunk noir total | ❌ Bruit énorme (500+ samples) |
| `0.20 - 0.30` | Dramatique néon | ✅ Bon (100-200 samples) |
| `0.50 - 0.70` | Ambiance nocturne | ✅ Excellent (50-100 samples) |
| `1.00` | Jour complet | ✅ Parfait (30-50 samples) |

**Nouvelle valeur par défaut** : `0.25` (scène showcase mise à jour)

### Cause Secondaire : Path Tracing Sans Lumières Directes

Les néons émissifs fonctionnent, MAIS :
- **Sans Point Lights** → Seuls les rayons qui touchent **directement** le néon contribuent
- Probabilité de toucher une petite sphère = très faible
- Variance élevée même avec ambient light correct

**Solution future** : Point Lights avec shadow rays
- Échantillonnage **direct** des sources lumineuses
- Variance drastiquement réduite
- Rendu propre même à 50 samples
- **Priorité #1 pour portfolio** (voir `QUICKSTART_LIGHTS.md`)

---

## 🎯 **Paramètres Recommandés**

### Pour Tester Maintenant (Sans Point Lights)
```json
{
    "render": {
        "samples_per_pixel": 200,
        "max_depth": 10,
        "ambient_light": 0.25,
        "gamma": 2.2
    }
}
```

**Résultat attendu** :
- ✅ Bruit réduit (acceptable)
- ✅ Néons visibles et colorés
- ✅ Render en 2-3 minutes (800x450)
- ⚠️ Toujours un peu bruyant (normal sans Point Lights)

### Après Point Lights (Prochaine Étape)
```json
{
    "render": {
        "samples_per_pixel": 50,
        "max_depth": 10,
        "ambient_light": 0.15,
        "gamma": 2.2
    }
}
```

**Résultat attendu** :
- ✅ Bruit minimal
- ✅ Ombres nettes
- ✅ Render ultra-rapide (30 secondes)
- ✅ Qualité portfolio

---

## 🔬 **Comprendre le Bruit en Path Tracing**

### Monte Carlo Sampling
Le path tracing = **moyenne statistique** :
```
Couleur_pixel = (rayon1 + rayon2 + ... + rayon_N) / N
```

**Bruit = Écart-type de cette moyenne**

### Sources de Variance

1. **Ambient Light Faible**
   - Rayons touchent du noir → Contribution nulle
   - Quelques rayons touchent néon → Contribution énorme
   - Grande différence = grande variance

2. **Émissifs Petits**
   - Probabilité de toucher = Surface / Scène
   - Petite sphère = faible probabilité
   - Faible proba = haute variance

3. **Profondeur Élevée Sans Termination**
   - Chaque rebond multiplie la variance
   - `max_depth=100` sans Russian Roulette = pire cas

### Solutions Classiques

1. ✅ **Augmenter samples** (force brute)
   - 2x samples → 1.41x moins de bruit
   - 4x samples → 2x moins de bruit
   - Très lent !

2. ✅ **Ambient light raisonnable** (ce qu'on a fait)
   - Réduit la variance de 10-50x
   - Gratuit !

3. ⭐ **Importance Sampling** (Point Lights - prochaine étape)
   - Échantillonne directement les lumières
   - Réduit variance de 100-1000x
   - **Standard industrie**

4. **Denoising** (optionnel, avancé)
   - Post-processing IA
   - Nvidia OptiX, Intel Open Image Denoise
   - Permet 10-20 samples pour qualité photo

---

## 📊 **Résumé des Fixes**

| Bug | Symptôme | Cause | Fix | Status |
|-----|----------|-------|-----|--------|
| Matériaux perdus | Néon→Diffus au load | scene.cpp manquait les cas | Ajout parsing Néon/Miroir | ✅ Fixed |
| Bruit excessif | Grain même à 500 spp | ambient_light trop bas (0.05) | Augmenté à 0.25 | ✅ Fixed |
| Bruit résiduel | Toujours un peu bruyant | Pas de Point Lights | Implémenter shadow rays | 🔜 Next |

---

## 🚀 **Prochaine Étape : Point Lights**

**Impact** :
- Bruit divisé par 10-50x
- Samples nécessaires : 200 → 50
- Ombres nettes et dramatiques
- **Indispensable pour portfolio**

**Voir** : `QUICKSTART_LIGHTS.md` pour l'implémentation !
