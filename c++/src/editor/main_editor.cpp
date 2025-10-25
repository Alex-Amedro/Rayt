#include "preview/window.hpp"
#include "preview/shader_manager.hpp"
#include "preview/renderer.hpp"
#include "preview/camera_gl.hpp"
#include "preview/sphere.hpp"
#include "preview/plane.hpp"
#include "preview/grid.hpp"
#include "editor/scene.hpp"
#include "editor/editor_ui.hpp"
#include "editor/camera_controller.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <glm/gtc/type_ptr.hpp>
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
    Window window(1600, 900, "Éditeur de Scène");
    
    // ====================================================================
    // 2. CHARGER LES SHADERS
    // ====================================================================
    ShaderManager shader;

    if (!shader.load_shaders("src/preview/shaders/vertex.glsl", 
                             "src/preview/shaders/fragment.glsl")) {
        return -1;
    }
    
    // ====================================================================
    // 3. CRÉER LA GÉOMÉTRIE (sphère + plan)
    // ====================================================================
    Sphere sphere_mesh;
    Plane plane_mesh(10.0f);  // Plan de 10x10 unités
    Grid grid(10.0f, 10);     // Grille de 10x10 unités, 10 divisions
    
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
    
    // ====================================================================
    // 6. ENVOYER LES MATRICES À LA CAMÉRA (une seule fois)
    // ====================================================================
    shader.use();
    shader.set_uniform_matrix4fv("projection", glm::value_ptr(camera.get_projection_matrix()));
    
    // ====================================================================
    // 7. INITIALISER IMGUI (APRÈS avoir configuré les matrices)
    // ====================================================================
    // IMPORTANT : On initialise ImGui AVANT les callbacks
    // pour que ImGui installe SES callbacks en premier
    ui.init(window.get_glfw_window());
    
    // ====================================================================
    // 8. CALLBACKS POUR LE CONTRÔLEUR
    // ====================================================================
    // IMPORTANT : ImGui a déjà installé ses callbacks.
    // On vérifie WantCaptureMouse pour savoir si on doit passer l'événement au contrôleur
    
    glfwSetWindowUserPointer(window.get_glfw_window(), &controller);
    
    // Callback souris : seulement si ImGui ne veut pas l'événement
    glfwSetMouseButtonCallback(window.get_glfw_window(), [](GLFWwindow* w, int button, int action, int /*mods*/) {
        // IMPORTANT : Laisser ImGui traiter AVANT
        ImGui_ImplGlfw_MouseButtonCallback(w, button, action, 0);
        
        // ImGui veut l'événement ? (souris sur le menu)
        if (ImGui::GetIO().WantCaptureMouse) {
            return;  // Ne rien faire de plus, ImGui a géré
        }
        
        // Sinon, passer au contrôleur
        auto* ctrl = static_cast<CameraController*>(glfwGetWindowUserPointer(w));
        ctrl->on_mouse_button(button, action);
    });
    
    // Callback mouvement : seulement si ImGui ne veut pas
    glfwSetCursorPosCallback(window.get_glfw_window(), [](GLFWwindow* w, double x, double y) {
        ImGui_ImplGlfw_CursorPosCallback(w, x, y);
        
        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }
        
        auto* ctrl = static_cast<CameraController*>(glfwGetWindowUserPointer(w));
        ctrl->on_mouse_move(x, y);
    });
    
    // Callback molette : seulement si ImGui ne veut pas
    glfwSetScrollCallback(window.get_glfw_window(), [](GLFWwindow* w, double xoffset, double yoffset) {
        ImGui_ImplGlfw_ScrollCallback(w, xoffset, yoffset);
        
        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }
        
        auto* ctrl = static_cast<CameraController*>(glfwGetWindowUserPointer(w));
        ctrl->on_scroll(yoffset);
    });
    
    // Callback clavier : ImGui gère automatiquement avec install_callbacks=true
    // Mais on peut ajouter un callback personnalisé si besoin
    glfwSetKeyCallback(window.get_glfw_window(), [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        // Laisser ImGui traiter le clavier en premier
        ImGui_ImplGlfw_KeyCallback(w, key, scancode, action, mods);
        // ImGui gère maintenant l'input clavier (zones de texte, etc.)
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
        controller.update(window.get_glfw_window(), delta_time);
        
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
        shader.set_uniform_matrix4fv("view", glm::value_ptr(camera.get_view_matrix()));
        
        // ================================================================
        // 9.5 DESSINER TOUS LES OBJETS DE LA SCÈNE
        // ================================================================
        scene.render_all(shader, sphere_mesh, plane_mesh, grid);
        
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
