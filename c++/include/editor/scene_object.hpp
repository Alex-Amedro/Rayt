// ============================================================================
// SCENE OBJECT : UN OBJET DANS LA SCÈNE
// ============================================================================
//
// Qu'est-ce qu'un SceneObject ?
//
// C'est la représentation d'UN objet dans la scène d'édition :
// - Une sphère
// - Un plan
// - Un triangle (futur)
// - etc.
//
// Chaque objet a :
// - Un TYPE (sphère, plan, triangle...)
// - Une POSITION (x, y, z)
// - Une COULEUR (r, g, b)
// - Une TAILLE (rayon pour sphère, etc)
// - Un MATÉRIAU (diffuse, métal, verre)
// - Un NOM (pour l'afficher dans la liste)
//
// ============================================================================

#pragma once

#include <string>
#include <glm/glm.hpp>

// ============================================================================
// ENUMS : Types d'objets et matériaux
// ============================================================================

enum class ObjectType {
    SPHERE,
    PLANE,
    TRIANGLE  // Pour le futur
};

enum class MaterialType {
    DIFFUSE,
    METAL,
    DIELECTRIC  // Verre
};

// ============================================================================
// CLASSE SCENEOBJECT
// ============================================================================

class SceneObject {
public:
    // ====================================================================
    // Propriétés de l'objet
    // ====================================================================
    ObjectType type;
    std::string name;           // Ex: "Sphere 1", "Plan Sol"
    
    glm::vec3 position;         // Position (x, y, z)
    glm::vec3 color;            // Couleur (r, g, b) entre 0.0 et 1.0
    float size;                 // Rayon pour sphère, échelle pour plan
    
    MaterialType material;
    float roughness;            // Pour le métal (0 = lisse, 1 = rugueux)
    float refraction_index;     // Pour le verre (1.5 = verre normal)
    
    bool selected;              // Est-il sélectionné dans l'éditeur ?
    
    // ====================================================================
    // CONSTRUCTEURS
    // ====================================================================
    
    // Constructeur par défaut (sphère diffuse blanche)
    SceneObject();
    
    // Constructeur avec type
    SceneObject(ObjectType t, const std::string& n);
    
    // ====================================================================
    // MÉTHODES UTILITAIRES
    // ====================================================================
    
    // Retourne une matrice de transformation (position + échelle)
    glm::mat4 get_model_matrix() const;
    
    // Retourne un nom lisible du type
    std::string get_type_name() const;
    
    // Retourne un nom lisible du matériau
    std::string get_material_name() const;
};
