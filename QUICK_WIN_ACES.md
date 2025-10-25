# ⚡ QUICK WIN - ACES Tone Mapping (30 minutes)

**Objectif:** Couleurs naturelles style Hollywood avec 10 lignes de code

**Impact visuel:** ⭐⭐⭐⭐⭐ (5/5)  
**Difficulté:** ⭐ (1/5)  
**Temps:** 30 minutes  

---

## 🎯 **Avant / Après**

### ❌ **Avant (Gamma simple)**
```
Problèmes:
- Blancs cramés (100% blanc = perte détails)
- Noirs écrasés (0% noir = trop sombre)
- Couleurs saturées artificiellement
- Contraste faible
```

### ✅ **Après (ACES)**
```
Résultats:
- Blancs lumineux MAIS détaillés
- Noirs profonds MAIS visibles
- Couleurs saturées NATURELLEMENT
- Contraste cinématographique
```

**Exemples concrets:**
- Reflets métal: Brillants sans être blancs purs
- Ombres: Sombres avec nuances visibles
- Rouge saturé: Vibrant mais pas "fluo"

---

## 📝 **IMPLÉMENTATION (3 étapes)**

### **Étape 1: Ajouter la fonction ACES**

**Fichier:** `c++/main.cpp`  
**Ligne:** Après les includes, avant `main()`

```cpp
// ============================================
// ACES Tone Mapping (Academy Color Encoding System)
// Standard industrie cinéma (Uncharted, Avatar, etc.)
// ============================================
vec3 aces_tonemap(const vec3& color) {
    // Coefficients ACES approximation (Narkowicz 2015)
    const double a = 2.51;
    const double b = 0.03;
    const double c = 2.43;
    const double d = 0.59;
    const double e = 0.14;
    
    vec3 x = color;
    vec3 numerator = x * (x * a + b);
    vec3 denominator = x * (x * c + d) + e;
    
    return vec3(
        numerator.x / denominator.x,
        numerator.y / denominator.y,
        numerator.z / denominator.z
    );
}
```

---

### **Étape 2: Remplacer gamma correction**

**Fichier:** `c++/main.cpp`  
**Fonction:** `main()`, dans la boucle de rendu  
**Chercher:** Le code qui fait `std::pow(color, 1.0/gamma)`

**ANCIEN CODE (à trouver):**
```cpp
// Gamma correction
vec3 final_color(
    std::pow(accumulated_color.x, 1.0/gamma),
    std::pow(accumulated_color.y, 1.0/gamma),
    std::pow(accumulated_color.z, 1.0/gamma)
);

// Clamp
final_color.x = std::clamp(final_color.x, 0.0, 1.0);
final_color.y = std::clamp(final_color.y, 0.0, 1.0);
final_color.z = std::clamp(final_color.z, 0.0, 1.0);
```

**NOUVEAU CODE (remplacer par):**
```cpp
// ACES Tone Mapping (HDR → LDR avec couleurs naturelles)
vec3 final_color = aces_tonemap(accumulated_color);

// Gamma correction APRÈS tone mapping
final_color = vec3(
    std::pow(final_color.x, 1.0/gamma),
    std::pow(final_color.y, 1.0/gamma),
    std::pow(final_color.z, 1.0/gamma)
);

// Clamp (sécurité, normalement ACES produit déjà [0,1])
final_color.x = std::clamp(final_color.x, 0.0, 1.0);
final_color.y = std::clamp(final_color.y, 0.0, 1.0);
final_color.z = std::clamp(final_color.z, 0.0, 1.0);
```

---

### **Étape 3: Compiler et tester**

```bash
cd c++
make clean
make
./rayt
```

**Comparer:**
1. Rendre une image SANS ACES (version actuelle)
2. Modifier le code avec ACES
3. Recompiler et rendre la MÊME scène
4. Ouvrir les 2 images côte à côte

---

## 🎨 **SCÈNE DE TEST**

**Utiliser:** `save1.json` (ta scène actuelle)

**Vérifier sur:**
- ✅ Sphère blanche (verre): Doit être lumineuse mais PAS 100% blanc
- ✅ Zones d'ombre: Doivent rester visibles (pas noir pur)
- ✅ Sphères colorées: Couleurs vibrantes mais naturelles
- ✅ Dégradé ciel: Transition douce sans banding

---

## 📊 **DIFFÉRENCES ATTENDUES**

### **Mesures (sur pixels lumineux)**

| Zone | Avant (RGB) | Après ACES (RGB) | Commentaire |
|------|-------------|------------------|-------------|
| Reflet brillant | (255, 255, 255) | (245, 248, 250) | Détails dans blancs |
| Ombre profonde | (5, 5, 5) | (15, 12, 10) | Nuances visibles |
| Rouge saturé | (255, 50, 50) | (235, 45, 42) | Moins "fluo" |
| Ciel bleu | (150, 180, 255) | (135, 165, 240) | Plus naturel |

---

## 🔍 **TROUBLESHOOTING**

### **Image trop sombre**
```cpp
// Solution: Multiplier avant ACES
vec3 final_color = aces_tonemap(accumulated_color * 1.2);
```

### **Image trop claire**
```cpp
// Solution: Exposure adjustment
vec3 final_color = aces_tonemap(accumulated_color * 0.8);
```

### **Couleurs ternes**
```cpp
// Vérifier: accumulated_color doit être en HDR (peut dépasser 1.0)
// Si déjà clampé avant ACES → ACES ne sert à rien!
// IMPORTANT: Ne PAS clamper avant tone mapping
```

### **Pas de différence visible**
```cpp
// Cause probable: Scène trop simple (pas de zones très lumineuses/sombres)
// Solution: Ajouter lumière forte + ombres profondes
```

---

## 🚀 **AMÉLIORATIONS FUTURES**

### **Exposure Control (30 min de plus)**
```cpp
// Paramètre réglable dans UI
double exposure = 1.0;  // 0.5 = plus sombre, 2.0 = plus clair

vec3 final_color = aces_tonemap(accumulated_color * exposure);
```

**Ajouter dans `editor_ui.cpp`:**
```cpp
if (ImGui::CollapsingHeader("Post-Processing")) {
    ImGui::SliderFloat("Exposure", &exposure, 0.1f, 3.0f);
}
```

---

### **Comparaison Tone Mappers (optionnel)**
```cpp
enum ToneMapper { GAMMA, ACES, REINHARD, UNCHARTED };

// Reinhard (simple)
vec3 reinhard(vec3 c) {
    return c / (c + vec3(1,1,1));
}

// Uncharted 2 (jeux vidéo)
vec3 uncharted2(vec3 c) {
    // Formule complexe...
}
```

---

## 📚 **THÉORIE (optionnel)**

### **Pourquoi "Tone Mapping"?**
```
Ray tracer calcule en HDR (High Dynamic Range):
  - Soleil = 10,000
  - Ciel = 1.0
  - Ombre = 0.01
  
Écran affiche en LDR (Low Dynamic Range):
  - Max = 1.0 (255 en 8-bit)
  - Min = 0.0
  
Tone mapping = Compresser HDR → LDR intelligemment
```

### **Courbe ACES vs Gamma**
```
Gamma: Linéaire puis clamp
  f(x) = x^(1/2.2)
  Problème: x > 1 → reste > 1 → clampé à 1 → perte

ACES: S-curve (sigmoïde)
  f(x) = (x * (a*x + b)) / (x * (c*x + d) + e)
  Avantage: x → ∞ asymptote vers 1 (pas de clamp brutal)
```

### **Utilisé par:**
- Hollywood: Avatar, Avengers, etc.
- Jeux AAA: Uncharted, Last of Us
- Logiciels 3D: Blender, Unreal Engine

---

## ✅ **CHECKLIST**

- [ ] Fonction `aces_tonemap()` ajoutée
- [ ] Ancien gamma simple remplacé
- [ ] Gamma appliqué APRÈS ACES (pas avant!)
- [ ] Compilé sans erreurs
- [ ] Testé sur scène existante
- [ ] Comparé avant/après visuellement
- [ ] (Optionnel) Exposure control ajouté dans UI

---

## 🎯 **RÉSULTAT FINAL**

**Avant:** Rendu "amateur" avec couleurs plates  
**Après:** Rendu "pro" avec couleurs cinéma  

**Effort:** 30 minutes  
**ROI:** ÉNORME 🔥

---

## 🚀 **PROCHAINE ÉTAPE**

Une fois ACES fonctionnel:

**Quick Win #2:** Point Lights + Shadows (4 heures)
- Voir `QUICKSTART_LIGHTS.md`
- Impact visuel encore plus grand
- Ombres portées = ancrage spatial

**Ou Quick Win #3:** Fresnel sur matériaux (2 heures)
- Reflets réalistes selon angle
- Code simple, effet pro

**GO! 💪**
