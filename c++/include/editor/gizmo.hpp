// ============================================================================
// GIZMO : OUTILS DE MANIPULATION 3D (AXES X, Y, Z)
// ============================================================================
//
// Un gizmo affiche 3 axes colorés autour d'un objet sélectionné :
// - X = Rouge (movable sur l'axe X)
// - Y = Vert (movable sur l'axe Y)
// - Z = Bleu (movable sur l'axe Z)
//
// L'utilisateur peut cliquer et glisser pour transformer l'objet.
//
// ============================================================================

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ShaderManager;

class Gizmo {
private:
    glm::vec3 position;        // Position du gizmo (centre)
    float axis_length;         // Longueur des axes
    float axis_radius;         // Rayon des cylindres (pour le ray casting)
    
    // Quels axes sont actuellement sélectionnés
    int selected_axis;         // -1 = aucun, 0 = X, 1 = Y, 2 = Z

public:
    // ====================================================================
    // CONSTRUCTEUR
    // ====================================================================
    Gizmo(float length = 1.0f, float radius = 0.1f);
    
    // ====================================================================
    // SETTERS
    // ====================================================================
    void set_position(glm::vec3 new_pos) { position = new_pos; }
    void set_axis_length(float len) { axis_length = len; }
    
    // ====================================================================
    // GETTERS
    // ====================================================================
    glm::vec3 get_position() const { return position; }
    int get_selected_axis() const { return selected_axis; }
    float get_axis_length() const { return axis_length; }
    
    // ====================================================================
    // RAY PICKING : Détecter quel axe est cliqué
    // ====================================================================
    // Retourne : -1 (aucun), 0 (X), 1 (Y), 2 (Z)
    int pick_axis(glm::vec3 ray_origin, glm::vec3 ray_direction);
    
    // ====================================================================
    // DÉPLACEMENT : Calculer le delta de position quand on glisse
    // ====================================================================
    // axis : 0=X, 1=Y, 2=Z
    // Retourne le déplacement sur l'axe sélectionné
    glm::vec3 calculate_delta(int axis, glm::vec3 ray_origin, glm::vec3 ray_direction);
    
    // ====================================================================
    // RENDU : Dessiner les axes
    // ====================================================================
    void draw(ShaderManager& shader);
    
    // ====================================================================
    // HELPERS : Intersection ray-segment (pour détecter les clics sur les axes)
    // ====================================================================
private:
    // Calcule la distance entre un rayon et une ligne 3D
    // Retourne true si distance < radius, et remplit t_hit avec le paramètre du rayon
    bool ray_line_distance(
        glm::vec3 ray_origin,
        glm::vec3 ray_direction,
        glm::vec3 line_start,
        glm::vec3 line_end,
        float& distance,
        float& t_hit
    ) const;
};
