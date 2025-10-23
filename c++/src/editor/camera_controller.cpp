// ============================================================================
// IMPLÉMENTATION DE CAMERACONTROLLER
// ============================================================================

#include "editor/camera_controller.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// Constantes
const float YAW_DEFAULT = -90.0f;  // Regarder vers -Z
const float PITCH_DEFAULT = 0.0f;  // Horizontal

// ============================================================================
// CONSTRUCTEUR
// ============================================================================

CameraController::CameraController(CameraGL& cam)
    : camera(cam)
    , move_speed(5.0f)          // 5 unités/seconde
    , mouse_sensitivity(0.1f)   // Sensibilité souris
    , scroll_speed(1.0f)
    , first_mouse(true)
    , mouse_pressed(false)
    , last_x(0.0), last_y(0.0)
    , yaw(YAW_DEFAULT)
    , pitch(PITCH_DEFAULT)
{
}

// ============================================================================
// MISE À JOUR (chaque frame)
// ============================================================================

void CameraController::update(GLFWwindow* window, float delta_time) {
    // ====================================================================
    // 1. CLAVIER : ZQSD (ou WASD)
    // ====================================================================
    
    glm::vec3 position = camera.get_position();
    glm::vec3 target = camera.get_target();
    
    // Direction avant (normalisée)
    glm::vec3 forward = glm::normalize(target - position);
    
    // Direction droite (normalisée)
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
    
    float velocity = move_speed * delta_time;
    
    // Z ou W : AVANT
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        position += forward * velocity;
    }
    
    // S : ARRIÈRE
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= forward * velocity;
    }
    
    // Q ou A : GAUCHE
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        position -= right * velocity;
    }
    
    // D : DROITE
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += right * velocity;
    }
    
    // ESPACE : MONTER
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position.y += velocity;
    }
    
    // SHIFT : DESCENDRE
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        position.y -= velocity;
    }
    
    // ====================================================================
    // 2. CALCULER LA NOUVELLE DIRECTION (yaw/pitch)
    // ====================================================================
    
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = glm::normalize(direction);
    
    // ====================================================================
    // 3. METTRE À JOUR LA CAMÉRA
    // ====================================================================
    
    target = position + direction;
    camera.set_position(position);
    camera.set_target(target);
}

// ============================================================================
// CALLBACK : BOUTON DE SOURIS
// ============================================================================

void CameraController::on_mouse_button(int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            mouse_pressed = true;
            first_mouse = true;  // Réinitialiser pour éviter les sauts
        } else if (action == GLFW_RELEASE) {
            mouse_pressed = false;
        }
    }
}

// ============================================================================
// CALLBACK : MOUVEMENT DE SOURIS
// ============================================================================

void CameraController::on_mouse_move(double xpos, double ypos) {
    // Seulement si le bouton droit est maintenu
    if (!mouse_pressed) {
        return;
    }
    
    // Premier mouvement ? Initialiser sans bouger la caméra
    if (first_mouse) {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
        return;
    }
    
    // Calculer le déplacement
    double xoffset = xpos - last_x;
    double yoffset = last_y - ypos;  // Inversé (y va vers le bas)
    
    last_x = xpos;
    last_y = ypos;
    
    // Appliquer la sensibilité
    xoffset *= mouse_sensitivity;
    yoffset *= mouse_sensitivity;
    
    // Mettre à jour les angles
    yaw += xoffset;
    pitch += yoffset;
    
    // Limiter le pitch (éviter de basculer la tête)
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

// ============================================================================
// CALLBACK : MOLETTE (zoom)
// ============================================================================

void CameraController::on_scroll(double yoffset) {
    // Changer le FOV pour zoomer/dézoomer
    float fov = camera.get_fov();
    fov -= yoffset * scroll_speed;
    
    // Limiter le FOV
    if (fov < 10.0f) fov = 10.0f;
    if (fov > 120.0f) fov = 120.0f;
    
    camera.set_fov(fov);
}
