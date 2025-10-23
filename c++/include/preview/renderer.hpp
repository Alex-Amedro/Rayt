// ============================================================================
// LEÇON 3 : RENDERER
// ============================================================================
//
// Qu'est-ce qu'un Renderer ?
//
// C'est la classe qui gère le dessin des objets 3D.
// Elle stocke les données (VAO, VBO) et dessine quand on le lui demande.
//
// Concept clé : Séparation des responsabilités
// - Window : Gère la fenêtre OpenGL
// - ShaderManager : Gère les shaders
// - Renderer : Gère le dessin des objets
//
// ============================================================================

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Renderer {
private:
    // ====================================================================
    // VAO : Vertex Array Object
    // ====================================================================
    // C'est un conteneur qui dit à OpenGL :
    // "Voilà comment mes données sont organisées"
    GLuint VAO;
    
    // ====================================================================
    // VBO : Vertex Buffer Object
    // ====================================================================
    // C'est la mémoire GPU qui stocke les données réelles
    // (positions et couleurs des vertices)
    GLuint VBO;
    
    // Nombre de vertices à dessiner
    GLuint vertex_count;
    
    // ====================================================================
    // FONCTION PRIVÉE : Initialiser le triangle
    // ====================================================================
    // Cette fonction remplit VAO et VBO avec les données du triangle
    void setup_triangle();

public:
    // ====================================================================
    // CONSTRUCTEUR
    // ====================================================================
    // Crée le VAO/VBO et configure le triangle
    Renderer();
    
    // ====================================================================
    // DESTRUCTEUR
    // ====================================================================
    // Nettoie la mémoire GPU
    ~Renderer();
    
    // ====================================================================
    // DESSINER LE TRIANGLE
    // ====================================================================
    // Fonction appelée à chaque frame pour dessiner
    void draw_triangle();
};
