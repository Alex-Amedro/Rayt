// ============================================================================
// LEÇON 5 : PROGRAMME TEST - SPHÈRE 3D
// ============================================================================
//
// Qu'est-ce qu'on fait ici ?
// 
// 1. Créer une fenêtre OpenGL (Leçon 1)
// 2. Charger les shaders avec matrices (Leçon 2/4)
// 3. Créer une caméra OpenGL (Leçon 4)
// 4. Générer une géométrie de sphère (Leçon 5)
// 5. Boucle principale :
//    - Effacer l'écran
//    - Utiliser les shaders
//    - Passer les matrices de la caméra aux shaders
//    - Dessiner la sphère
//    - Afficher le résultat
//
// Résultat : Une belle sphère colorée en 3D ! 🌐
//
// ============================================================================

#include "preview/window.hpp"
#include "preview/shader_manager.hpp"
#include "preview/renderer.hpp"
#include "preview/camera_gl.hpp"
#include "preview/sphere.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
    std::cout << "\n🎨 LEÇON 5 : Sphère 3D\n" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    // ========================================================================
    // ÉTAPE 1 : Créer la fenêtre
    // ========================================================================
    Window window(800, 600, "Leçon 5 : Sphère OpenGL");
    
    // ========================================================================
    // ÉTAPE 2 : Charger les shaders
    // ========================================================================
    ShaderManager shader_manager;
    
    if (!shader_manager.load_shaders("src/preview/shaders/vertex.glsl",
                                      "src/preview/shaders/fragment.glsl")) {
        std::cerr << "\n❌ Erreur : Impossible de charger les shaders !" << std::endl;
        return 1;
    }
    
    // ========================================================================
    // ÉTAPE 3 : Créer la caméra OpenGL
    // ========================================================================
    CameraGL camera(
        glm::vec3(0.0f, 0.0f, 3.0f),     // Position
        glm::vec3(0.0f, 0.0f, 0.0f),     // Target (où on regarde)
        glm::vec3(0.0f, 1.0f, 0.0f),     // Up
        45.0f,                            // FOV (degrés)
        800.0f / 600.0f,                  // Aspect ratio
        0.1f, 100.0f                      // Near et far planes
    );
    
    // ========================================================================
    // ÉTAPE 4 : Générer la sphère
    // ========================================================================
    // radius=1, stacks=30 (bandes horizontales), sectors=30 (bandes verticales)
    // Plus de stacks/sectors = plus de détails = plus lent
    Sphere sphere(1.0f, 30, 30);
    
    std::cout << "🌐 Sphère créée !" << std::endl;
    std::cout << "\nCliquez sur la croix pour fermer.\n" << std::endl;
    
    // ========================================================================
    // ÉTAPE 5 : BOUCLE PRINCIPALE
    // ========================================================================
    while (!window.should_close()) {
        // Effacer l'écran
        window.clear(0.1f, 0.1f, 0.15f);
        
        // Utiliser le shader program
        shader_manager.use();
        
        // ====================================================================
        // Passer les matrices de la caméra au shader
        // ====================================================================
        
        glm::mat4 model = glm::mat4(1.0f);  // Pas de transformation sur la sphère
        
        shader_manager.set_uniform_matrix4fv("projection", 
            glm::value_ptr(camera.get_projection_matrix()));
        shader_manager.set_uniform_matrix4fv("view", 
            glm::value_ptr(camera.get_view_matrix()));
        shader_manager.set_uniform_matrix4fv("model", 
            glm::value_ptr(model));
        
        // Dessiner la sphère
        sphere.draw();
        
        // Afficher le résultat
        window.swap_buffers();
        
        // Traiter les événements
        window.poll_events();
    }
    
    std::cout << "\n✓ Programme terminé !" << std::endl << std::endl;
    
    return 0;
}
