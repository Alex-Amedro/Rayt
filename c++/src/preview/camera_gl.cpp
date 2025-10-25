// ============================================================================
// LEÇON 4 : CAMERA OPENGL - IMPLÉMENTATION
// ============================================================================

#include "preview/camera_gl.hpp"
#include <iostream>

// ============================================================================
// CONSTRUCTEUR
// ============================================================================
CameraGL::CameraGL(
    glm::vec3 pos,
    glm::vec3 target_pos,
    glm::vec3 up_dir,
    float fov_deg,
    float aspect,
    float near_z,
    float far_z
) : position(pos), target(target_pos), up(up_dir), fov(fov_deg),
    aspect_ratio(aspect), near_plane(near_z), far_plane(far_z) {
    
    update_matrices();
}

// ============================================================================
// UPDATE_MATRICES : Recalculer les matrices
// ============================================================================
void CameraGL::update_matrices() {
    // ========================================================================
    // VIEW MATRIX
    // ========================================================================

    view_matrix = glm::lookAt(
        position,              // Où on est
        target,                // Où on regarde
        up                     // Quelle direction est "haut"
    );
    
    // ========================================================================
    // PROJECTION MATRIX
    // ========================================================================

    projection_matrix = glm::perspective(
        glm::radians(fov),       // Convertir FOV en radians
        aspect_ratio,            // Ratio largeur/hauteur
        near_plane,              // Plan proche
        far_plane                // Plan lointain
    );
}

// ============================================================================
// SETTERS
// ============================================================================
void CameraGL::set_position(glm::vec3 new_pos) {
    position = new_pos;
    update_matrices();
}

void CameraGL::set_target(glm::vec3 new_target) {
    target = new_target;
    update_matrices();
}

void CameraGL::set_up(glm::vec3 new_up) {
    up = glm::normalize(new_up);
    update_matrices();
}

void CameraGL::set_fov(float new_fov) {
    fov = new_fov;
    update_matrices();
}

void CameraGL::set_aspect_ratio(float new_aspect) {
    aspect_ratio = new_aspect;
    update_matrices();
}

