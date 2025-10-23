// ============================================================================
// ÉDITEUR DE SCÈNE - MAIN
// ============================================================================
//
// Ce programme combine :
// - OpenGL pour le rendu 3D en temps réel (preview)
// - ImGui pour l'interface graphique (menu)
// - Une scène éditable (ajouter/modifier des objets)
// - Des contrôles caméra (ZQSD + souris)
//
// Layout :
// ┌──────────────────────────────────┬──────────┐
// │ Viewport 3D (OpenGL)             │  Menu UI │
// │ - Fond gris                      │  (ImGui) │
// │ - Objets de la scène             │          │
// │ - Contrôles ZQSD + souris        │  1/5     │
// │ 4/5 largeur                      │  largeur │
// └──────────────────────────────────┴──────────┘
//
// ============================================================================

#include "preview/window.hpp"
#include "preview/shader_manager.hpp"
#include "preview/renderer.hpp"
#include "preview/camera_gl.hpp"
#include "preview/sphere.hpp"
#include "editor/scene.hpp"
#include "editor/editor_ui.hpp"
#include "editor/camera_controller.hpp"

#include <iostream>

// ============================================================================
// MAIN
// ============================================================================

int main() {
    std::cout << "\n";
    std::cout << "============================================\n";
    std::cout << "   ÉDITEUR DE SCÈNE - RAY TRACER\n";
    std::cout << "============================================\n\n";
    
    // ====================================================================
    // 1. CRÉER LA FENÊTRE OPENGL
    // ====================================================================
    Window window("Éditeur de Scène", 1600, 900);
    if (!window.is_initialized()) {
        std::cerr << "Erreur lors de la création de la fenêtre\n";
        return -1;
    }
    
    // ====================================================================
    // 2. CHARGER LES SHADERS
    // ====================================================================
    ShaderManager shader;
    if (!shader.load_shaders("src/preview/shaders/vertex.glsl", 
                             "src/preview/shaders/fragment.glsl")) {
        return -1;
    }
    
    // ====================================================================
    // 3. CRÉER LA GÉOMÉTRIE (sphère)
    // ====================================================================
    Sphere sphere_mesh;
    
    // ====================================================================
    // 4. CRÉER LA CAMÉRA ET SON CONTRÔLEUR
    // ====================================================================
    CameraGL camera(
        glm::vec3(0, 2, 10),    // Position
        glm::vec3(0, 0, 0),     // Target (centre de la scène)
        glm::vec3(0, 1, 0),     // Up
        45.0f,                  // FOV
        16.0f / 9.0f            // Aspect ratio
    );
    
    CameraController controller(camera);
    
    // ====================================================================
    // 5. CRÉER LA SCÈNE ET L'INTERFACE UI
    // ====================================================================
    Scene scene;
    scene.create_default_scene();  // 3 sphères de test
    
    EditorUI ui(scene);
    ui.init(window.get_window());
    
    // ====================================================================
    // 6. ENVOYER LES MATRICES À LA CAMÉRA (une seule fois)
    // ====================================================================
    shader.use();
    shader.set_uniform_matrix4fv("projection", camera.get_projection_matrix());
    
    // ====================================================================
    // 7. CALLBACKS POUR LE CONTRÔLEUR
    // ====================================================================
    // On va utiliser des lambdas pour connecter GLFW aux callbacks
    glfwSetWindowUserPointer(window.get_window(), &controller);
    
    glfwSetMouseButtonCallback(window.get_window(), [](GLFWwindow* w, int button, int action, int mods) {
        auto* ctrl = static_cast<CameraController*>(glfwGetWindowUserPointer(w));
        ctrl->on_mouse_button(button, action);
    });
    
    glfwSetCursorPosCallback(window.get_window(), [](GLFWwindow* w, double x, double y) {
        auto* ctrl = static_cast<CameraController*>(glfwGetWindowUserPointer(w));
        ctrl->on_mouse_move(x, y);
    });
    
    glfwSetScrollCallback(window.get_window(), [](GLFWwindow* w, double xoffset, double yoffset) {
        auto* ctrl = static_cast<CameraController*>(glfwGetWindowUserPointer(w));
        ctrl->on_scroll(yoffset);
    });
    
    // ====================================================================
    // 8. VARIABLES DE TEMPS (pour delta_time)
    // ====================================================================
    float last_frame = 0.0f;
    
    std::cout << "\n";
    std::cout << "Contrôles :\n";
    std::cout << "  - ZQSD / WASD : Déplacer la caméra\n";
    std::cout << "  - ESPACE : Monter\n";
    std::cout << "  - SHIFT : Descendre\n";
    std::cout << "  - CLIC DROIT MAINTENU + SOURIS : Regarder autour\n";
    std::cout << "  - MOLETTE : Zoom\n";
    std::cout << "  - ESC : Quitter\n";
    std::cout << "\n";
    
    // ====================================================================
    // 9. BOUCLE DE RENDU
    // ====================================================================
    while (!window.should_close()) {
        // ================================================================
        // 9.1 CALCULER LE DELTA TIME
        // ================================================================
        float current_frame = glfwGetTime();
        float delta_time = current_frame - last_frame;
        last_frame = current_frame;
        
        // ================================================================
        // 9.2 METTRE À JOUR LE CONTRÔLEUR CAMÉRA
        // ================================================================
        controller.update(window.get_window(), delta_time);
        
        // ================================================================
        // 9.3 EFFACER L'ÉCRAN (fond gris)
        // ================================================================
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        
        // ================================================================
        // 9.4 ENVOYER LA MATRICE VIEW AU SHADER (change chaque frame)
        // ================================================================
        shader.use();
        shader.set_uniform_matrix4fv("view", camera.get_view_matrix());
        
        // ================================================================
        // 9.5 DESSINER TOUS LES OBJETS DE LA SCÈNE
        // ================================================================
        scene.render_all(shader, sphere_mesh);
        
        // ================================================================
        // 9.6 DESSINER L'INTERFACE IMGUI
        // ================================================================
        ui.render();
        
        // ================================================================
        // 9.7 SWAP BUFFERS ET POLL EVENTS
        // ================================================================
        window.swap_buffers();
        window.poll_events();
    }
    
    // ====================================================================
    // 10. NETTOYAGE
    // ====================================================================
    ui.shutdown();
    
    std::cout << "\nÉditeur fermé proprement\n";
    return 0;
}
