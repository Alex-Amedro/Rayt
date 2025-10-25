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
#include "editor/gizmo.hpp"

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
    Grid grid(100.0f, 100);     // Grille de 10x10 unités, 10 divisions
    
    // ====================================================================
    // 4. CRÉER LA CAMÉRA ET SON CONTRÔLEUR
    // ====================================================================
    CameraGL camera(
        glm::vec3(0, 2, 10),    // Position
        glm::vec3(0, 0, 0),     // Target (centre de la scène)
        glm::vec3(0, 1, 0),     // Up
        45.0f,                  // FOV
        16.0f / 9.0f            // Aspect ratio (sera mis à jour après)
    );
    
    // Mettre à jour l'aspect ratio avec la taille réelle de la fenêtre
    int window_width, window_height;
    glfwGetWindowSize(window.get_glfw_window(), &window_width, &window_height);
    camera.set_aspect_ratio((float)window_width / (float)window_height);
    
    CameraController controller(camera);
    
    // ====================================================================
    // 5. CRÉER LA SCÈNE ET L'INTERFACE UI
    // ====================================================================
    Scene scene;
    scene.create_default_scene();  // 3 sphères de test
    
    EditorUI ui(scene);
    
    // ====================================================================
    // 5.5. CRÉER LE GIZMO POUR LA TRANSFORMATION
    // ====================================================================
    Gizmo gizmo(2.0f);  // Longueur des axes = 2.0
    ui.set_gizmo(&gizmo);  // Donner le gizmo à l'UI pour qu'elle le mette à jour
    
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
    
    // Struct pour passer multiple données au callback
    struct CallbackData {
        CameraController* controller;
        Scene* scene;
        CameraGL* camera;
        Gizmo* gizmo;
        int window_width;
        int window_height;
        bool dragging_gizmo;          // Est-ce qu'on est en train de drag un axe ?
        int dragging_axis;            // Quel axe ? (-1 = aucun, 0=X, 1=Y, 2=Z)
        glm::vec3 drag_start_pos;     // Position de l'objet au début du drag
        glm::vec3 drag_start_gizmo;   // Position du gizmo au début du drag (référence fixe)
        double last_mouse_x;          // Position souris précédente pour calcul incrémental
        double last_mouse_y;
    };
    
    CallbackData cb_data = {&controller, &scene, &camera, &gizmo, 1600, 900, false, -1, glm::vec3(0), glm::vec3(0), 0, 0};
    glfwSetWindowUserPointer(window.get_glfw_window(), &cb_data);
    
    // Callback souris : seulement si ImGui ne veut pas l'événement
    glfwSetMouseButtonCallback(window.get_glfw_window(), [](GLFWwindow* w, int button, int action, int /*mods*/) {
        // IMPORTANT : Laisser ImGui traiter AVANT
        ImGui_ImplGlfw_MouseButtonCallback(w, button, action, 0);
        
        // ImGui veut l'événement ? (souris sur le menu)
        if (ImGui::GetIO().WantCaptureMouse) {
            return;  // Ne rien faire de plus, ImGui a géré
        }
        
        auto* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(w));
        
        // CLIC GAUCHE = sélectionner un objet OU manipuler le gizmo
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            double mouse_x, mouse_y;
            glfwGetCursorPos(w, &mouse_x, &mouse_y);
            
            // Récupérer la taille du viewport OpenGL
            GLint viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);
            int viewport_width = viewport[2];
            int viewport_height = viewport[3];
            
            // Créer le rayon en utilisant la taille du viewport 3D
            float ndc_x = (2.0f * (float)mouse_x) / viewport_width - 1.0f;
            float ndc_y = 1.0f - (2.0f * (float)mouse_y) / viewport_height;  // Y inversé pour OpenGL
            
            glm::vec4 ray_clip(ndc_x, ndc_y, -1.0f, 1.0f);
            glm::vec4 ray_eye = glm::inverse(data->camera->get_projection_matrix()) * ray_clip;
            ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);
            
            glm::vec3 ray_dir = glm::normalize(
                glm::vec3(glm::inverse(data->camera->get_view_matrix()) * ray_eye)
            );
            glm::vec3 ray_origin = data->camera->get_position();
            
            // PRIORITÉ 1 : Tester les axes du gizmo si un objet est sélectionné
            if (data->scene->get_selected() != nullptr) {
                int axis = data->gizmo->pick_axis(ray_origin, ray_dir);
                if (axis != -1) {
                    // Commencer à déplacer sur cet axe
                    data->dragging_gizmo = true;
                    data->dragging_axis = axis;
                    data->drag_start_pos = data->scene->get_selected()->position;
                    data->drag_start_gizmo = data->gizmo->get_position();  // Position fixe de référence
                    data->last_mouse_x = mouse_x;  // Sauvegarder position souris initiale
                    data->last_mouse_y = mouse_y;
                    std::cout << "🎯 DRAG DÉMARRÉ sur axe " << axis << " depuis position ("
                              << data->drag_start_pos.x << ", " 
                              << data->drag_start_pos.y << ", " 
                              << data->drag_start_pos.z << ")" << std::endl;
                    return;  // Ne pas tester les objets de la scène
                }
            }
            
            // PRIORITÉ 2 : Si aucun axe n'est touché, tester les objets
            int index = data->scene->pick_object(ray_origin, ray_dir);
            
            if (index >= 0) {
                data->scene->select_object(index);
                // Mettre à jour la position du gizmo
                SceneObject* obj = data->scene->get_object(index);
                if (obj) {
                    data->gizmo->set_position(obj->position);
                }
            }
        }
        // CLIC GAUCHE RELÂCHÉ = arrêter le drag du gizmo
        else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
            if (data->dragging_gizmo) {
                std::cout << "🛑 DRAG TERMINÉ" << std::endl;
            }
            data->dragging_gizmo = false;
            data->dragging_axis = -1;
        }
        // CLIC DROIT = rotation caméra
        else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            data->controller->on_mouse_button(button, action);
        }
    });

    // Callback mouvement : gestion du drag gizmo et rotation caméra
    glfwSetCursorPosCallback(window.get_glfw_window(), [](GLFWwindow* w, double x, double y) {
        ImGui_ImplGlfw_CursorPosCallback(w, x, y);
        
        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }
        
        auto* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(w));
        
        // Si on est en train de déplacer avec le gizmo
        if (data->dragging_gizmo && data->scene->get_selected() != nullptr) {
            // Calculer le déplacement de la souris en pixels
            double delta_mouse_x = x - data->last_mouse_x;
            double delta_mouse_y = y - data->last_mouse_y;
            
            // Mettre à jour la dernière position
            data->last_mouse_x = x;
            data->last_mouse_y = y;
            
            // Convertir le mouvement souris en mouvement 3D
            // Facteur de sensibilité : plus grand = plus rapide
            float sensitivity = 0.07f;  // Compromis entre 0.005 et 0.05
            
            SceneObject* obj = data->scene->get_selected();
            
            // Calculer la direction de l'axe en espace écran pour déterminer le sens
            glm::vec3 axes[3] = {
                glm::vec3(1, 0, 0),  // X
                glm::vec3(0, 1, 0),  // Y
                glm::vec3(0, 0, 1)   // Z
            };
            
            glm::vec3 axis_world = axes[data->dragging_axis];
            glm::vec3 axis_end = obj->position + axis_world;
            
            // Projeter le début et la fin de l'axe en espace écran
            glm::mat4 view = data->camera->get_view_matrix();
            glm::mat4 proj = data->camera->get_projection_matrix();
            glm::mat4 vp = proj * view;
            
            glm::vec4 pos_clip = vp * glm::vec4(obj->position, 1.0);
            glm::vec4 end_clip = vp * glm::vec4(axis_end, 1.0);
            
            glm::vec2 pos_ndc = glm::vec2(pos_clip.x / pos_clip.w, pos_clip.y / pos_clip.w);
            glm::vec2 end_ndc = glm::vec2(end_clip.x / end_clip.w, end_clip.y / end_clip.w);
            
            // Direction de l'axe en espace écran (NON normalisée pour garder la longueur)
            glm::vec2 axis_screen_raw = end_ndc - pos_ndc;
            float axis_screen_length = glm::length(axis_screen_raw);
            
            // Si l'axe est trop perpendiculaire à la vue (presque invisible), ignorer ce mouvement
            if (axis_screen_length < 0.01f) {
                data->last_mouse_x = x;
                data->last_mouse_y = y;
                return;
            }
            
            glm::vec2 axis_screen = axis_screen_raw / axis_screen_length;  // Normaliser manuellement
            
            // Mouvement de la souris en NDC (-1 à 1)
            GLint viewport[4];
            glGetIntegerv(GL_VIEWPORT, viewport);
            glm::vec2 mouse_delta_ndc(
                (float)delta_mouse_x / viewport[2] * 2.0f,
                -(float)delta_mouse_y / viewport[3] * 2.0f  // Y inversé
            );
            
            // Projeter le mouvement souris sur la direction de l'axe
            float movement = glm::dot(mouse_delta_ndc, axis_screen);
            
            // Appliquer le mouvement sur l'axe approprié (SANS compensation, c'était trop)
            obj->position += axis_world * (movement * sensitivity * 100.0f);
            
            // Mettre à jour le gizmo pour suivre l'objet
            data->gizmo->set_position(obj->position);
        } else {
            // Sinon, comportement normal (rotation caméra)
            data->controller->on_mouse_move(x, y);
        }
    });
    
    // Callback molette : seulement si ImGui ne veut pas
    glfwSetScrollCallback(window.get_glfw_window(), [](GLFWwindow* w, double xoffset, double yoffset) {
        ImGui_ImplGlfw_ScrollCallback(w, xoffset, yoffset);
        
        if (ImGui::GetIO().WantCaptureMouse) {
            return;
        }
        
        auto* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(w));
        data->controller->on_scroll(yoffset);
    });
    
    // Callback clavier : ImGui gère automatiquement avec install_callbacks=true
    glfwSetKeyCallback(window.get_glfw_window(), [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        ImGui_ImplGlfw_KeyCallback(w, key, scancode, action, mods);
    });
    
    // Callback de redimensionnement : mettre à jour l'aspect ratio de la caméra
    glfwSetFramebufferSizeCallback(window.get_glfw_window(), [](GLFWwindow* w, int width, int height) {
        glViewport(0, 0, width, height);  // Mettre à jour le viewport OpenGL
        
        auto* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(w));
        if (data && data->camera && width > 0 && height > 0) {
            data->camera->set_aspect_ratio((float)width / (float)height);
            std::cout << "[RESIZE] Fenêtre redimensionnée : " << width << "x" << height 
                      << " (aspect ratio = " << ((float)width / (float)height) << ")\n";
        }
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
        // 9.4 ENVOYER LES MATRICES VIEW ET PROJECTION AU SHADER
        // ================================================================
        shader.use();
        shader.set_uniform_matrix4fv("view", glm::value_ptr(camera.get_view_matrix()));
        shader.set_uniform_matrix4fv("projection", glm::value_ptr(camera.get_projection_matrix()));
        
        // ================================================================
        // 9.5 DESSINER TOUS LES OBJETS DE LA SCÈNE
        // ================================================================
        scene.render_all(shader, sphere_mesh, plane_mesh, grid);
        
        // ================================================================
        // 9.5.5 DESSINER LE GIZMO SI UN OBJET EST SÉLECTIONNÉ
        // ================================================================
        if (scene.get_selected() != nullptr) {
            gizmo.draw(shader);
        }
        
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
