// ============================================================================
// LEÇON 4 : CAMERA OPENGL
// ============================================================================
//
// Qu'est-ce qu'une caméra en OpenGL ?
//
// C'est un objet qui dit à OpenGL :
// - Où est l'observateur (position)
// - Où regarde-t-il (target)
// - Quelle est l'orientation vers le haut (up)
// - Quel est l'angle de vision (FOV)
//
// Tout ça se transforme en deux matrices :
// 1. View Matrix = où et comment on regarde
// 2. Projection Matrix = la perspective (comment ça s'écrase sur l'écran)
//
// Ces matrices sont envoyées aux shaders comme "uniforms"
// (uniforms = variables globales pour les shaders)
//
// ============================================================================

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CameraGL {
private:
    // ====================================================================
    // Position et orientation
    // ====================================================================
    glm::vec3 position;      // Où on regarde depuis
    glm::vec3 target;        // Où on regarde
    glm::vec3 up;            // Direction "haut"
    
    // ====================================================================
    // Projection
    // ====================================================================
    float fov;               // Field of view (angle vertical)
    float aspect_ratio;      // Largeur / Hauteur
    float near_plane;        // Plan proche (quoi est trop proche)
    float far_plane;         // Plan lointain (quoi est trop loin)
    
    // ====================================================================
    // Matrices de transformation
    // ====================================================================
    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
    
    // Recalcule les matrices quand quelque chose change
    void update_matrices();

public:
    // ====================================================================
    // CONSTRUCTEUR
    // ====================================================================
    // Crée une caméra avec position, target, et orientation
    CameraGL(
        glm::vec3 pos,           // Position de la caméra
        glm::vec3 target,        // Où on regarde
        glm::vec3 up,            // Direction haut (normalement (0, 1, 0))
        float fov = 45.0f,       // Angle de vision (degrés)
        float aspect = 16.0f/9.0f, // Ratio largeur/hauteur
        float near_z = 0.1f,     // Plan proche
        float far_z = 100.0f     // Plan lointain
    );
    
    // ====================================================================
    // SETTERS : Changer la caméra
    // ====================================================================
    void set_position(glm::vec3 new_pos);
    void set_target(glm::vec3 new_target);
    void set_up(glm::vec3 new_up);
    void set_fov(float new_fov);
    void set_aspect_ratio(float new_aspect);  // Nouveau : mettre à jour l'aspect ratio
    
    // ====================================================================
    // GETTERS : Récupérer les matrices
    // ====================================================================
    glm::mat4 get_view_matrix() const { return view_matrix; }
    glm::mat4 get_projection_matrix() const { return projection_matrix; }
    
    // ====================================================================
    // HELPER : Passer les matrices aux shaders
    // ====================================================================
    // Utile pour envoyer les matrices au vertex shader
    const float* get_view_matrix_ptr() const {
        return glm::value_ptr(view_matrix);
    }
    
    const float* get_projection_matrix_ptr() const {
        return glm::value_ptr(projection_matrix);
    }
    
    // ====================================================================
    // GETTERS : Infos sur la caméra
    // ====================================================================
    glm::vec3 get_position() const { return position; }
    glm::vec3 get_target() const { return target; }
    float get_fov() const { return fov; }
    float get_aspect_ratio() const { return aspect_ratio; }
};
