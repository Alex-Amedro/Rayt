// ============================================================================
// LEÇON 3 : RENDERER - IMPLÉMENTATION
// ============================================================================

#include "preview/renderer.hpp"
#include <iostream>

// ============================================================================
// CONSTRUCTEUR
// ============================================================================
Renderer::Renderer() : VAO(0), VBO(0), vertex_count(3) {
    std::cout << "\n[LEÇON 3] Initialisation du Renderer..." << std::endl;
    setup_triangle();
    std::cout << "✓ Triangle initialisé en mémoire GPU\n" << std::endl;
}

// ============================================================================
// DESTRUCTEUR
// ============================================================================
Renderer::~Renderer() {
    // Nettoyer les buffers GPU
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
}

// ============================================================================
// SETUP_TRIANGLE : Initialiser les données du triangle
// ============================================================================
void Renderer::setup_triangle() {
    // ========================================================================
    // ÉTAPE 1 : Définir les données du triangle
    // ========================================================================
    // Format : [Position (x, y, z)] [Couleur (r, g, b)]
    //
    // 3 vertices :
    // - Bas-gauche : (-0.5, -0.5, 0.0) → Rouge (1.0, 0.0, 0.0)
    // - Bas-droite : (0.5, -0.5, 0.0) → Vert (0.0, 1.0, 0.0)
    // - Haut-centre : (0.0, 0.5, 0.0) → Bleu (0.0, 0.0, 1.0)
    //
    // Les coordonnées sont en "Normalized Device Coordinates" (NDC) :
    // - x, y, z entre -1.0 et 1.0
    // - (0, 0) = centre de l'écran
    // - (-1, -1) = bas-gauche
    // - (1, 1) = haut-droite
    //
    float vertices[] = {
        // Position             // Couleur
        -0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,  // Vertex 0 : Rouge
         0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,  // Vertex 1 : Vert
         0.0f,  0.5f, 0.0f,    0.0f, 0.0f, 1.0f   // Vertex 2 : Bleu
    };
    
    // ========================================================================
    // ÉTAPE 2 : Créer le VAO (Vertex Array Object)
    // ========================================================================
    // Le VAO stocke la configuration : "comment sont arrangées mes données"
    // C'est comme un "template" de la structure des données
    //
    glGenVertexArrays(1, &VAO);
    std::cout << "  ✓ VAO créé (ID: " << VAO << ")" << std::endl;
    
    // Active le VAO pour configurer ses attributs
    glBindVertexArray(VAO);
    
    // ========================================================================
    // ÉTAPE 3 : Créer et remplir le VBO (Vertex Buffer Object)
    // ========================================================================
    // Le VBO est la vraie mémoire GPU qui contient les données
    //
    glGenBuffers(1, &VBO);
    std::cout << "  ✓ VBO créé (ID: " << VBO << ")" << std::endl;
    
    // Active le VBO (le "buffer courant")
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Copie les données du CPU vers la GPU
    // GL_STATIC_DRAW = on ne change pas les données, on les lit juste
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    std::cout << "  ✓ Données copiées sur GPU (" << sizeof(vertices) << " bytes)" << std::endl;
    
    // ========================================================================
    // ÉTAPE 4 : Configurer l'attribut POSITION (location = 0)
    // ========================================================================
    // On dit à OpenGL : "L'attribut 0 contient 3 floats (x, y, z)"
    //
    // Chaque vertex fait 6 floats (3 pos + 3 col) = 24 bytes
    // L'attribut position commence au byte 0
    //
    glVertexAttribPointer(
        0,                          // location = 0 (comme dans le vertex shader)
        3,                          // 3 floats (x, y, z)
        GL_FLOAT,                   // type : float
        GL_FALSE,                   // pas de normalisation
        6 * sizeof(float),          // stride = 6 floats = 24 bytes
        (void*)0                    // offset = 0 bytes
    );
    glEnableVertexAttribArray(0);   // Active cet attribut
    std::cout << "  ✓ Attribut POSITION configuré (location 0)" << std::endl;
    
    // ========================================================================
    // ÉTAPE 5 : Configurer l'attribut COULEUR (location = 1)
    // ========================================================================
    // On dit à OpenGL : "L'attribut 1 contient 3 floats (r, g, b)"
    //
    // Chaque vertex fait 6 floats, et la couleur commence au byte 12
    // (après les 3 floats de position)
    //
    glVertexAttribPointer(
        1,                              // location = 1 (comme dans le vertex shader)
        3,                              // 3 floats (r, g, b)
        GL_FLOAT,                       // type : float
        GL_FALSE,                       // pas de normalisation
        6 * sizeof(float),              // stride = 6 floats = 24 bytes
        (void*)(3 * sizeof(float))      // offset = 12 bytes (après la position)
    );
    glEnableVertexAttribArray(1);       // Active cet attribut
    std::cout << "  ✓ Attribut COULEUR configuré (location 1)" << std::endl;
    
    // ========================================================================
    // ÉTAPE 6 : Débind (nettoyage)
    // ========================================================================
    // On peut débind le VBO (il reste attaché au VAO)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // On peut débind le VAO aussi (on le rebindra avant de dessiner)
    glBindVertexArray(0);
    
    std::cout << "  ✓ Triangle prêt à dessiner !" << std::endl;
}

// ============================================================================
// DRAW_TRIANGLE : Dessiner le triangle
// ============================================================================
void Renderer::draw_triangle() {
    // Active le VAO (qui contient la configuration + le VBO)
    glBindVertexArray(VAO);
    
    // Dessine le triangle
    // GL_TRIANGLES = dessine des triangles (3 vertices = 1 triangle)
    // 0 = commence à la position 0
    // 3 = utilise 3 vertices
    glDrawArrays(GL_TRIANGLES, 0, vertex_count);
    
    // Débind (optionnel, mais bonne pratique)
    glBindVertexArray(0);
}
