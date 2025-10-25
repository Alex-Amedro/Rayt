// ============================================================================
// IMPLÉMENTATION DE SCENEOBJECT
// ============================================================================

#include "editor/scene_object.hpp"
#include <glm/gtc/matrix_transform.hpp>

// ============================================================================
// CONSTRUCTEUR PAR DÉFAUT (sphère diffuse blanche)
// ============================================================================

SceneObject::SceneObject()
    : type(ObjectType::SPHERE)
    , name("Sphere")
    , position(0.0f, 0.0f, 0.0f)
    , color(1.0f, 1.0f, 1.0f)  // Blanc
    , size(1.0f)               // Rayon = 1
    , material(MaterialType::DIFFUSE)
    , roughness(0.0f)
    , refraction_index(1.5f)   // Verre normal
    , emission_strength(5.0f)  // Néon classique
    , selected(false)
{
}

// ============================================================================
// CONSTRUCTEUR AVEC TYPE
// ============================================================================

SceneObject::SceneObject(ObjectType t, const std::string& n)
    : type(t)
    , name(n)
    , position(0.0f, 0.0f, 0.0f)
    , color(1.0f, 1.0f, 1.0f)
    , size(1.0f)
    , material(MaterialType::DIFFUSE)
    , roughness(0.0f)
    , refraction_index(1.5f)
    , emission_strength(5.0f)
    , selected(false)
{
    // Valeurs par défaut selon le type
    if (t == ObjectType::PLANE) {
        name = "Plan";
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        size = 10.0f;  // Un grand plan
    }
}

// ============================================================================
// MATRICE DE TRANSFORMATION (position + échelle)
// ============================================================================
// Cette matrice sera utilisée pour dessiner l'objet en OpenGL
// On va l'envoyer au shader avec glUniformMatrix4fv()

glm::mat4 SceneObject::get_model_matrix() const {
    glm::mat4 model = glm::mat4(1.0f);  // Matrice identité
    
    // 1. Translation (position)
    model = glm::translate(model, position);
    
    // 2. Échelle (taille)
    model = glm::scale(model, glm::vec3(size));
    
    return model;
}

// ============================================================================
// NOMS LISIBLES DES TYPES
// ============================================================================

std::string SceneObject::get_type_name() const {
    switch (type) {
        case ObjectType::SPHERE:   return "Sphère";
        case ObjectType::PLANE:    return "Plan";
        case ObjectType::TRIANGLE: return "Triangle";
        default:                   return "Inconnu";
    }
}

std::string SceneObject::get_material_name() const {
    switch (material) {
        case MaterialType::DIFFUSE:     return "Diffus";
        case MaterialType::METAL:       return "Métal";
        case MaterialType::DIELECTRIC:  return "Verre";
        case MaterialType::EMISSIVE:    return "Néon";
        case MaterialType::MIRROR:      return "Miroir";
        default:                        return "Inconnu";
    }
}
