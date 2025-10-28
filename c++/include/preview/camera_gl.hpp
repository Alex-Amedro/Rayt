// ============================================================================
// LESSON 4: OPENGL CAMERA
// ============================================================================
//
// What is a camera in OpenGL?
//
// It's an object that tells OpenGL:
// - Where is the observer (position)
// - Where are they looking (target)
// - What is the up orientation (up)
// - What is the field of view (FOV)
//
// All of this transforms into two matrices:
// 1. View Matrix = where and how we're looking
// 2. Projection Matrix = the perspective (how it projects onto screen)
//
// These matrices are sent to shaders as "uniforms"
// (uniforms = global variables for shaders)
//
// ============================================================================

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class CameraGL {
private:
    // ====================================================================
    // Position and orientation
    // ====================================================================
    glm::vec3 position;      // Where we're looking from
    glm::vec3 target;        // Where we're looking at
    glm::vec3 up;            // "Up" direction
    
    // ====================================================================
    // Projection
    // ====================================================================
    float fov;               // Field of view (vertical angle)
    float aspect_ratio;      // Width / Height
    float near_plane;        // Near plane (what's too close)
    float far_plane;         // Far plane (what's too far)
    
    // ====================================================================
    // Transformation matrices
    // ====================================================================
    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
    
    // Recalculate matrices when something changes
    void update_matrices();

public:
    // ====================================================================
    // CONSTRUCTOR
    // ====================================================================
    // Creates a camera with position, target, and orientation
    CameraGL(
        glm::vec3 pos,           // Camera position
        glm::vec3 target,        // Where we're looking
        glm::vec3 up,            // Up direction (usually (0, 1, 0))
        float fov = 45.0f,       // Field of view (degrees)
        float aspect = 16.0f/9.0f, // Width/height ratio
        float near_z = 0.1f,     // Near plane
        float far_z = 100.0f     // Far plane
    );
    
    // ====================================================================
    // SETTERS: Change the camera
    // ====================================================================
    void set_position(glm::vec3 new_pos);
    void set_target(glm::vec3 new_target);
    void set_up(glm::vec3 new_up);
    void set_fov(float new_fov);
    void set_aspect_ratio(float new_aspect);  // Update aspect ratio
    
    // ====================================================================
    // GETTERS: Get the matrices
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
