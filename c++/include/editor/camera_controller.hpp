// ============================================================================
// CAMERA CONTROLLER : CONTRÔLE CLAVIER + SOURIS
// ============================================================================
//
// Ce contrôleur permet de déplacer la caméra dans la scène avec :
// - ZQSD (ou WASD) : se déplacer avant/arrière/gauche/droite
// - CLIC MAINTENU + SOURIS : regarder autour (pitch/yaw)
// - MOLETTE : zoom (changer le FOV)
//
// Il met à jour une caméra OpenGL (CameraGL) en temps réel.
//
// Utilisation :
//
//     CameraController controller(camera);
//     
//     // Dans la boucle de rendu
//     while (!window.should_close()) {
//         float delta_time = /* temps depuis la dernière frame */;
//         controller.update(window.get_window(), delta_time);
//         
//         // La caméra est maintenant à jour
//         shader.set_uniform_matrix4fv("view", camera.get_view_matrix());
//     }
//
// ============================================================================

#pragma once

#include "preview/camera_gl.hpp"
#include <GLFW/glfw3.h>

// ============================================================================
// CLASSE CAMERACONTROLLER
// ============================================================================

class CameraController {
private:
    CameraGL& camera;  // Référence à la caméra à contrôler
    
    // ====================================================================
    // PARAMÈTRES DE MOUVEMENT
    // ====================================================================
    float move_speed;      // Vitesse de déplacement (unités/seconde)
    float mouse_sensitivity;  // Sensibilité de la souris
    float scroll_speed;    // Vitesse de zoom
    
    // ====================================================================
    // ÉTAT DE LA SOURIS
    // ====================================================================
    bool first_mouse;      // Premier mouvement de souris ?
    bool mouse_pressed;    // Bouton maintenu ?
    double last_x, last_y; // Position précédente de la souris
    
    // ====================================================================
    // ANGLES DE LA CAMÉRA (Euler angles)
    // ====================================================================
    float yaw;    // Rotation gauche/droite (en degrés)
    float pitch;  // Rotation haut/bas (en degrés)
    
public:
    // ====================================================================
    // CONSTRUCTEUR
    // ====================================================================
    CameraController(CameraGL& cam);
    
    // ====================================================================
    // MISE À JOUR (appeler chaque frame)
    // ====================================================================
    void update(GLFWwindow* window, float delta_time);
    
    // ====================================================================
    // CALLBACKS GLFW
    // ====================================================================
    void on_mouse_button(int button, int action);
    void on_mouse_move(double xpos, double ypos);
    void on_scroll(double yoffset);
    
    // ====================================================================
    // GETTERS / SETTERS
    // ====================================================================
    void set_move_speed(float speed) { move_speed = speed; }
    void set_mouse_sensitivity(float sens) { mouse_sensitivity = sens; }
    void set_scroll_speed(float speed) { scroll_speed = speed; }
    
    float get_move_speed() const { return move_speed; }
    float get_mouse_sensitivity() const { return mouse_sensitivity; }
    float get_scroll_speed() const { return scroll_speed; }
};
