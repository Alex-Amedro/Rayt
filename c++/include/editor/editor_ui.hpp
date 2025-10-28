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
#include "preview/camera_gl.hpp"
#include <GLFW/glfw3.h>

// ============================================================================
// CLASSE EDITORUI
// ============================================================================

class EditorUI {
private:
    Scene& scene;  // Référence à la scène (pour ajouter/modifier des objets)
    Gizmo* gizmo;  // Référence au gizmo (pour mettre à jour sa position)
    CameraGL& camera;  // Référence à la caméra (pour la sauvegarde)
    
    // ====================================================================
    // ÉTAT DE L'INTERFACE
    // ====================================================================
    bool show_add_menu;      // Menu "Ajouter un objet" visible ?
    bool show_properties;    // Panneau de propriétés visible ?
    
    // ====================================================================
    // RENDER SETTINGS (pour le raytracer)
    // ====================================================================
    // Image quality
    int image_width = 1920;
    int image_height = 1080;
    int samples_per_pixel = 200;  // Augmenté pour réduire le bruit
    int max_depth = 50;
    
    // Camera
    float aperture = 0.0f;
    float focus_distance = 10.0f;
    
    // Environment
    float sun_intensity = 0.0f;      // 0.0 par défaut = désactivé
    float sun_direction_x = 0.5f;    // Direction X from which sun light comes
    float sun_direction_y = -1.0f;   // Direction Y (negative = from above)
    float sun_direction_z = 0.3f;    // Direction Z
    float ambient_light = 0.3f;      // Réduit pour scènes sombres/néons
    float gamma = 2.2f;
    
    // Post-processing
    bool enable_denoise = false;
    int denoise_type = 1;  // 0=Box, 1=Gaussian, 2=Bilateral
    float denoise_strength = 1.0f;  // 0.5-2.0
    
    // Performance
    int num_threads = 8;
    
public:
    // ====================================================================
    // CONSTRUCTEUR
    // ====================================================================
    EditorUI(Scene& s, CameraGL& cam);
    
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
