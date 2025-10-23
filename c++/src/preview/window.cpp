#include "preview/window.hpp"
#include <iostream>

// ============================================================================
// LEÇON 1 : IMPLÉMENTER UNE FENÊTRE OPENGL
// ============================================================================
//
// Chaque fonction fait une petite chose simple.
// Ensemble, elles créent une fenêtre où on peut dessiner en OpenGL.
//
// ============================================================================

// ============================================================================
// CONSTRUCTEUR : Window::Window()
// ============================================================================
Window::Window(int w, int h, const std::string& t) 
    : window(nullptr), width(w), height(h), title(t) {
    
    std::cout << "\n[LEÇON 1] Création de la fenêtre..." << std::endl;
    
    // ========================================================================
    // ÉTAPE 1 : Initialiser GLFW
    // ========================================================================
    // GLFW = "Graphics Library Framework"
    // C'est la librairie qui crée les fenêtres et gère les événements
    //
    if (!glfwInit()) {
        std::cerr << "ERREUR : GLFW n'a pas pu s'initialiser !" << std::endl;
        exit(1);
    }
    std::cout << "✓ GLFW initialisé" << std::endl;
    
    // ========================================================================
    // ÉTAPE 2 : Configurer GLFW
    // ========================================================================
    // On dit à GLFW comment on veut que la fenêtre soit créée
    //
    // glfwWindowHint = "hint" = conseil/instruction pour la prochaine fenêtre
    //
    
    // On veut OpenGL 4.6 (la version la plus récente possible)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    
    // On veut le "core profile" (juste les features modernes)
    // vs "compatibility profile" (toutes les old features aussi)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    std::cout << "✓ GLFW configuré (OpenGL 4.6 Core)" << std::endl;
    
    // ========================================================================
    // ÉTAPE 3 : Créer la fenêtre
    // ========================================================================
    // glfwCreateWindow(largeur, hauteur, titre, fullscreen?, shared?)
    //
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    
    if (!window) {
        std::cerr << "ERREUR : Impossible de créer la fenêtre !" << std::endl;
        glfwTerminate();
        exit(1);
    }
    std::cout << "✓ Fenêtre créée (" << width << "x" << height << ")" << std::endl;
    
    // ========================================================================
    // ÉTAPE 4 : Activer le contexte OpenGL
    // ========================================================================
    // Maintenant on dit à OpenGL : "dessine dans cette fenêtre"
    // Un contexte = "l'espace où on dessine"
    //
    glfwMakeContextCurrent(window);
    
    std::cout << "✓ Contexte OpenGL activé" << std::endl;
    
    // ========================================================================
    // ÉTAPE 5 : Initialiser GLEW
    // ========================================================================
    // GLEW = "OpenGL Extension Wrangler"
    // C'est une librairie qui charge les fonctions OpenGL modernes
    //
    // Sans GLEW, on n'aurait accès qu'aux vieilles fonctions OpenGL
    //
    glewExperimental = GL_TRUE; // On demande à GLEW d'être "agressif"
    GLenum err = glewInit();
    
    if (err != GLEW_OK) {
        std::cerr << "ERREUR GLEW : " << glewGetErrorString(err) << std::endl;
        exit(1);
    }
    std::cout << "✓ GLEW initialisé" << std::endl;
    
    // ========================================================================
    // ÉTAPE 6 : Afficher les infos OpenGL
    // ========================================================================
    std::cout << "\n📊 Informations OpenGL :" << std::endl;
    std::cout << "   Vendor:   " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "   Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "   Version:  " << glGetString(GL_VERSION) << std::endl;
    
    // ========================================================================
    // ÉTAPE 7 : Configuration supplémentaire
    // ========================================================================
    
    // V-SYNC = Synchroniser avec l'écran
    // 1 = oui (60 FPS), 0 = non (max FPS)
    glfwSwapInterval(1);
    
    // Faire passer les évènements (clicks, clavier) à la fenêtre
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    
    std::cout << "\n✓ Fenêtre prête à dessiner !" << std::endl << std::endl;
}

// ============================================================================
// DESTRUCTEUR : Window::~Window()
// ============================================================================
Window::~Window() {
    std::cout << "[LEÇON 1] Fermeture de la fenêtre..." << std::endl;
    
    // Fermer la fenêtre
    if (window) {
        glfwDestroyWindow(window);
    }
    
    // Nettoyer GLFW
    glfwTerminate();
    
    std::cout << "✓ Fenêtre fermée, adieu !" << std::endl;
}

// ============================================================================
// should_close() : Est-ce qu'on doit fermer la fenêtre ?
// ============================================================================
bool Window::should_close() const {
    // glfwWindowShouldClose() retourne true si l'utilisateur a cliqué sur X
    return glfwWindowShouldClose(window);
}

// ============================================================================
// poll_events() : Traiter les événements
// ============================================================================
void Window::poll_events() {
    // glfwPollEvents() regarde s'il y a des événements (clicks, mouvements)
    // et les traite
    // 
    // C'est différent de glfwWaitEvents() qui attendrait un événement
    //
    glfwPollEvents();
}

// ============================================================================
// swap_buffers() : Afficher ce qu'on a dessiné
// ============================================================================
void Window::swap_buffers() {
    // Double buffering :
    // - Buffer 1 (caché) = on dessine dedans
    // - Buffer 2 (visible) = ce qu'on affiche à l'écran
    // 
    // Quand on appelle swap, on échange les deux
    // Ça évite le scintillement
    //
    glfwSwapBuffers(window);
}

// ============================================================================
// clear() : Effacer l'écran avec une couleur
// ============================================================================
void Window::clear(float r, float g, float b) {
    // glClearColor(r, g, b, a) = mettre la couleur d'effacement
    // Les valeurs sont entre 0.0 et 1.0 (PAS 0-255 !)
    // 
    // C'est différent de notre ray tracer qui utilise 0-255
    // OpenGL utilise 0.0-1.0 partout
    //
    glClearColor(r, g, b, 1.0f);
    
    // glClear(GL_COLOR_BUFFER_BIT) = effacer l'écran avec cette couleur
    // GL_COLOR_BUFFER_BIT = "efface le buffer de couleur"
    // (il y a aussi GL_DEPTH_BUFFER_BIT pour 3D, GL_STENCIL_BUFFER_BIT)
    //
    glClear(GL_COLOR_BUFFER_BIT);
}
