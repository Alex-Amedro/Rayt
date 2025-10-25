# 🚀 QUICK START - Ajouter les Lumières Ponctuelles

**Objectif:** Implémenter l'éclairage direct avec ombres pour un effet WAW immédiat !

---

## 📋 **Checklist (5-6 heures)**

### ✅ **Étape 1: Structure de données (30 min)**

**Créer `include/lights/point_light.hpp`:**
```cpp
#pragma once
#include "core/vec3.hpp"

class PointLight {
public:
    vec3 position;      // Position dans l'espace
    vec3 color;         // Couleur RGB [0,1]
    double intensity;   // Intensité (1.0 = normal)

    PointLight(vec3 pos, vec3 col, double inten)
        : position(pos), color(col), intensity(inten) {}

    // Calculer la contribution de cette lumière sur un point
    vec3 illuminate(
        const vec3& hit_point,
        const vec3& hit_normal,
        const vec3& view_direction,
        const vec3& albedo,
        double roughness
    ) const;
};
```

---

### ✅ **Étape 2: Ajouter lumières à la scène (20 min)**

**Dans `main.cpp`:**
```cpp
#include "lights/point_light.hpp"

int main() {
    // ... charge JSON ...
    
    // Parser les lumières du JSON
    std::vector<PointLight> lights;
    if (scene_data.contains("lights")) {
        for (auto& light_json : scene_data["lights"]) {
            vec3 pos(
                light_json["position"]["x"],
                light_json["position"]["y"],
                light_json["position"]["z"]
            );
            vec3 color(
                light_json["color"]["r"],
                light_json["color"]["g"],
                light_json["color"]["b"]
            );
            double intensity = light_json["intensity"];
            lights.push_back(PointLight(pos, color, intensity));
        }
    }
    
    // Passer lights à ray_color
}
```

---

### ✅ **Étape 3: Modifier ray_color (2 heures)**

**Dans `core/ray_color.cpp`:**
```cpp
vec3 ray_color(
    const vec3& ray_origin,
    const vec3& ray_direction,
    const std::vector<std::shared_ptr<hittable>>& scene,
    const std::vector<PointLight>& lights,  // ← NOUVEAU
    int depth
) {
    if (depth <= 0) return vec3(0, 0, 0);

    // ... hit detection ...

    if (closest_object) {
        vec3 hit_point = ray_origin + (ray_direction * t_min);
        vec3 hit_normal = closest_object->get_normal(hit_point);
        
        // ========================================
        // NOUVEAU: ÉCLAIRAGE DIRECT
        // ========================================
        vec3 direct_lighting(0, 0, 0);
        
        for (const auto& light : lights) {
            // 1. Direction vers la lumière
            vec3 light_dir = (light.position - hit_point).normalize();
            
            // 2. SHADOW RAY: vérifier si quelque chose bloque la lumière
            bool in_shadow = false;
            double light_distance = (light.position - hit_point).length();
            
            for (const auto& obj : scene) {
                double t = obj->hit(hit_point + hit_normal * 0.001, light_dir);
                if (t > 0.001 && t < light_distance) {
                    in_shadow = true;
                    break;
                }
            }
            
            if (!in_shadow) {
                // 3. Calcul diffuse (Lambert)
                double diffuse = std::max(0.0, hit_normal.dot(light_dir));
                
                // 4. Atténuation distance (inverse square law)
                double attenuation = 1.0 / (light_distance * light_distance);
                
                // 5. Contribution totale de cette lumière
                vec3 light_contribution = 
                    albedo * light.color * light.intensity * diffuse * attenuation;
                
                direct_lighting = direct_lighting + light_contribution;
            }
        }
        
        // ========================================
        // ÉCLAIRAGE INDIRECT (path tracing existant)
        // ========================================
        vec3 indirect_lighting(0, 0, 0);
        if (/* material scatters */) {
            indirect_lighting = ray_color(next_origin, scattered, scene, lights, depth-1);
        }
        
        // Combiner direct + indirect
        return direct_lighting + (indirect_lighting * 0.5);  // 50% indirect
    }
    
    // ... sky ...
}
```

---

### ✅ **Étape 4: UI pour éditer lumières (2 heures)**

**Dans `editor_ui.cpp` - Ajouter section "Lumières":**
```cpp
void EditorUI::render_lights() {
    if (ImGui::CollapsingHeader("Lumières", ImGuiTreeNodeFlags_DefaultOpen)) {
        
        // Liste des lumières
        for (int i = 0; i < scene.get_light_count(); i++) {
            Light* light = scene.get_light(i);
            
            ImGui::PushID(i);
            if (ImGui::TreeNode(light->name.c_str())) {
                
                // Position
                ImGui::DragFloat3("Position", &light->position.x, 0.1f);
                
                // Couleur
                float col[3] = {light->color.r, light->color.g, light->color.b};
                if (ImGui::ColorEdit3("Couleur", col)) {
                    light->color = vec3(col[0], col[1], col[2]);
                }
                
                // Intensité
                ImGui::SliderFloat("Intensité", &light->intensity, 0.0f, 10.0f);
                
                // Bouton supprimer
                if (ImGui::Button("Supprimer")) {
                    scene.remove_light(i);
                }
                
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        
        // Bouton ajouter lumière
        if (ImGui::Button("+ Ajouter Lumière Ponctuelle")) {
            scene.add_light(PointLight(vec3(0, 5, 0), vec3(1,1,1), 1.0));
        }
    }
}
```

---

### ✅ **Étape 5: Visualiser lumières dans l'éditeur (1 heure)**

**Dans `scene.cpp` - Dessiner icônes lumières:**
```cpp
void Scene::render_lights(ShaderManager& shader, Sphere& icon_mesh) {
    for (auto& light : lights) {
        // Matrice model pour la lumière
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, light.position);
        model = glm::scale(model, glm::vec3(0.2f));  // Petite sphère
        
        shader.set_uniform_matrix4fv("model", glm::value_ptr(model));
        shader.set_uniform_3f("objectColor", 
            light.color.r, light.color.g, light.color.b);
        
        icon_mesh.draw();
    }
}
```

---

### ✅ **Étape 6: JSON save/load (30 min)**

**Format JSON:**
```json
{
    "lights": [
        {
            "type": "point",
            "position": {"x": 0.0, "y": 5.0, "z": 0.0},
            "color": {"r": 1.0, "g": 1.0, "b": 1.0},
            "intensity": 10.0
        },
        {
            "type": "point",
            "position": {"x": -3.0, "y": 3.0, "z": 2.0},
            "color": {"r": 1.0, "g": 0.5, "b": 0.2},
            "intensity": 5.0
        }
    ]
}
```

---

## 🎨 **Scène de Test**

**Cornell Box avec 1 lumière:**
```json
{
    "lights": [
        {
            "type": "point",
            "position": {"x": 0.0, "y": 3.9, "z": 0.0},
            "color": {"r": 1.0, "g": 1.0, "b": 1.0},
            "intensity": 20.0
        }
    ],
    "objects": [
        // Sol blanc
        {"type": "Plan", "position": {"y": 0}, "color": {"r": 0.8, "g": 0.8, "b": 0.8}},
        // Plafond blanc
        {"type": "Plan", "position": {"y": 4}, "color": {"r": 0.8, "g": 0.8, "b": 0.8}},
        // Sphère rouge
        {"type": "Sphère", "position": {"x": -1, "y": 1, "z": 0}, "size": 1, "color": {"r": 0.8, "g": 0.1, "b": 0.1}},
        // Sphère verte
        {"type": "Sphère", "position": {"x": 1, "y": 1, "z": 0}, "size": 1, "color": {"r": 0.1, "g": 0.8, "b": 0.1}}
    ]
}
```

---

## 🔥 **Résultat Attendu**

**Avant (sans lumières):**
- Éclairage ambiant uniforme
- Pas d'ombres
- Plat et peu réaliste

**Après (avec lumières):**
- Ombres portées nettes
- Zones d'ombre et de lumière
- Dégradés réalistes
- Éclairage dramatique

**Effet WAW:** 🔥🔥🔥🔥🔥

---

## 🚀 **Optimisations Futures**

1. **Soft Shadows:** Area lights avec échantillonnage multiple
2. **Specular:** Ajouter highlights (Blinn-Phong ou GGX)
3. **Atténuation:** Modèle physique amélioré
4. **Light culling:** Ne tester que lumières proches
5. **IES profiles:** Lumières réalistes (spots, projecteurs)

---

## 📚 **Références**

- Phong reflection model
- Shadow rays (basics)
- Inverse square law (attenuation)

**Go build it! 💪**
