// ============================================================================
// SCENE : GESTIONNAIRE DE TOUS LES OBJETS
// ============================================================================
//
// Qu'est-ce qu'une Scene ?
//
// C'est le CONTENEUR de tous les objets (sphères, plans, triangles...)
// Elle gère :
// - La liste des objets : std::vector<SceneObject*>
// - La sélection : quel objet est sélectionné ?
// - L'ajout/suppression d'objets
// - Le rendu de tous les objets en OpenGL
//
// Exemple d'utilisation :
//
//     Scene scene;
//     
//     // Ajouter une sphère
//     SceneObject* sphere = new SceneObject(ObjectType::SPHERE, "Ma Sphère");
//     sphere->position = glm::vec3(0, 0, -5);
//     sphere->color = glm::vec3(1, 0, 0);  // Rouge
//     scene.add_object(sphere);
//     
//     // Ajouter un plan
//     SceneObject* plane = new SceneObject(ObjectType::PLANE, "Sol");
//     plane->position = glm::vec3(0, -1, 0);
//     scene.add_object(plane);
//     
//     // Sélectionner la sphère
//     scene.select_object(0);
//     
//     // Dessiner tous les objets
//     scene.render_all(shader, sphere_mesh, plane_mesh);
//
// ============================================================================

#pragma once

#include "scene_object.hpp"
#include <vector>

// Forward declarations (on inclura les headers dans le .cpp)
class ShaderManager;
class Sphere;
class Plane;
class Grid;
// ============================================================================
// CLASSE SCENE
// ============================================================================

class Scene {
private:
    std::vector<SceneObject*> objects;  // Liste de tous les objets
    int selected_index;                 // Index de l'objet sélectionné (-1 si aucun)
    bool wireframe_enabled;             // Afficher le wireframe ?
    bool grid_enabled;                  // Afficher la grille ?
    
public:
    // ====================================================================
    // CONSTRUCTEUR / DESTRUCTEUR
    // ====================================================================
    Scene();
    ~Scene();  // Libère la mémoire de tous les objets
    
    // ====================================================================
    // GESTION DES OBJETS
    // ====================================================================
    
    // Ajouter un objet à la scène
    void add_object(SceneObject* obj);
    
    // Supprimer un objet par index
    void remove_object(int index);
    
    // Supprimer l'objet sélectionné
    void remove_selected();
    
    // Obtenir le nombre d'objets
    int get_object_count() const { return objects.size(); }
    
    // Obtenir un objet par index
    SceneObject* get_object(int index);
    
    // Obtenir tous les objets (pour la liste UI)
    const std::vector<SceneObject*>& get_objects() const { return objects; }
    
    // ====================================================================
    // GESTION DE LA SÉLECTION
    // ====================================================================
    
    // Sélectionner un objet par index
    void select_object(int index);
    
    // Désélectionner tout
    void clear_selection();
    
    // Obtenir l'objet sélectionné (nullptr si aucun)
    SceneObject* get_selected();
    
    // Obtenir l'index de la sélection (-1 si aucun)
    int get_selected_index() const { return selected_index; }
    
    // ====================================================================
    // WIREFRAME
    // ====================================================================
    void set_wireframe(bool enabled) { wireframe_enabled = enabled; }
    bool is_wireframe_enabled() const { return wireframe_enabled; }
    
    // ====================================================================
    // GRILLE
    // ====================================================================
    void set_grid(bool enabled) { grid_enabled = enabled; }
    bool is_grid_enabled() const { return grid_enabled; }
    
    // ====================================================================
    // RENDU (OpenGL)
    // ====================================================================
    
    // Dessiner tous les objets
    // shader : le shader OpenGL à utiliser
    // sphere_mesh : la géométrie de la sphère (pour dessiner les sphères)
    // plane_mesh : la géométrie du plan (pour dessiner les plans)
    void render_all(ShaderManager& shader, Sphere& sphere_mesh, Plane& plane_mesh, Grid& grid_mesh);
    
    // ====================================================================
    // UTILITAIRES
    // ====================================================================
    
    // Supprimer tous les objets
    void clear();
    
    // Créer une scène par défaut (pour tester)
    void create_default_scene();
};
