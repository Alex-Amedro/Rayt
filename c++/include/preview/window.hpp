#pragma once

// ============================================================================
// LEÇON 1 : UNE FENÊTRE OPENGL
// ============================================================================
// 
// Objectif : Créer une classe qui gère une fenêtre OpenGL
//
// Concepts clés :
// 1. GLFW = Gère la fenêtre (crée, input, events)
// 2. OpenGL = L'API pour le rendu 3D (on dessine dedans)
// 3. Context = La "toile" où OpenGL dessine
//
// Analogie : GLFW = cadre du tableau, OpenGL = la peinture
//
// ============================================================================

// ATTENTION : L'ordre est important !
// GLEW doit être inclus AVANT GLFW et les autres headers OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Window {
private:
    // La fenêtre GLFW
    GLFWwindow* window;
    
    // Dimensions
    int width, height;
    
    // Titre de la fenêtre (déclaré en dernier = initialisé en dernier)
    std::string title;

public:
    // ========================================================================
    // CONSTRUCTEUR
    // ========================================================================
    // Crée une fenêtre de taille (w, h) avec un titre
    // 
    // Ce que ça fait :
    // 1. Initialise GLFW (la librairie qui gère les fenêtres)
    // 2. Crée une fenêtre
    // 3. Crée un contexte OpenGL (espace pour dessiner)
    // 4. Active ce contexte (= "je vais dessiner ici")
    //
    Window(int w, int h, const std::string& title);
    
    // ========================================================================
    // DESTRUCTEUR
    // ========================================================================
    // Nettoie tout quand la fenêtre se ferme
    ~Window();
    
    // ========================================================================
    // VÉRIFIER SI ON DOIT FERMER LA FENÊTRE
    // ========================================================================
    // Retourne true si l'utilisateur a cliqué sur la croix
    // On l'utilise dans la boucle principale
    //
    bool should_close() const;
    
    // ========================================================================
    // TRAITER LES ÉVÉNEMENTS
    // ========================================================================
    // Regarde s'il y a des events (clics souris, touches clavier)
    // IMPORTANT : À appeler dans la boucle principale
    //
    void poll_events();
    
    // ========================================================================
    // AFFICHER CE QU'ON A DESSINÉ
    // ========================================================================
    // Double buffering = on dessine dans un buffer caché, puis on l'affiche
    // C'est pour éviter le scintillement
    //
    void swap_buffers();
    
    // ========================================================================
    // EFFACER L'ÉCRAN
    // ========================================================================
    // Remplit l'écran avec une couleur (rouge, vert, bleu)
    // Les valeurs sont entre 0.0 et 1.0 (pas 0-255 !)
    //
    void clear(float r, float g, float b);
    
    // ========================================================================
    // GETTERS
    // ========================================================================
    int get_width() const { return width; }
    int get_height() const { return height; }
    GLFWwindow* get_glfw_window() const { return window; }
};
