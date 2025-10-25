// ============================================================================
// EDITOR UI : INTERFACE IMGUI
// ============================================================================
//
// C'est le menu latéral droit (1/5 de la largeur) qui permet de :
// - Ajouter des objets (sphères, plans)
// - Voir la liste des objets
// - Sélectionner un objet
// - Modifier ses propriétés (position, couleur, taille, matériau)
// - Supprimer des objets
// - Lancer le ray tracer
//
// ImGui c'est quoi ?
//
// C'est une bibliothèque pour faire des interfaces graphiques simples.
// On appelle des fonctions comme :
//     ImGui::Button("Mon Bouton")
//     ImGui::SliderFloat("Position X", &x, -10, 10)
//
// Et ImGui dessine tout automatiquement en OpenGL !
//
// ============================================================================

#pragma once

#include "scene.hpp"
#include "gizmo.hpp"
#include <GLFW/glfw3.h>

// ============================================================================
// CLASSE EDITORUI
// ============================================================================

class EditorUI {
private:
    Scene& scene;  // Référence à la scène (pour ajouter/modifier des objets)
    Gizmo* gizmo;  // Référence au gizmo (pour mettre à jour sa position)
    
    // ====================================================================
    // ÉTAT DE L'INTERFACE
    // ====================================================================
    bool show_add_menu;      // Menu "Ajouter un objet" visible ?
    bool show_properties;    // Panneau de propriétés visible ?
    
public:
    // ====================================================================
    // CONSTRUCTEUR
    // ====================================================================
    EditorUI(Scene& s);
    
    // ====================================================================
    // SETTER
    // ====================================================================
    void set_gizmo(Gizmo* g) { gizmo = g; }
    
    // ====================================================================
    // INITIALISATION / NETTOYAGE IMGUI
    // ====================================================================
    void init(GLFWwindow* window);
    void shutdown();
    
    // ====================================================================
    // RENDU DE L'INTERFACE (chaque frame)
    // ====================================================================
    void render();
    
    // ====================================================================
    // SECTIONS DE L'INTERFACE
    // ====================================================================
private:
    // Menu "Ajouter un objet"
    void render_add_menu();
    
    // Liste des objets
    void render_object_list();
    
    // Propriétés de l'objet sélectionné
    void render_properties();
    
    // Boutons d'actions (Supprimer, Render)
    void render_actions();
};
