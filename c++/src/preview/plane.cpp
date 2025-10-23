// ============================================================================
// PLANE (QUAD) : IMPLÉMENTATION
// ============================================================================

#include "preview/plane.hpp"
#include <iostream>

// ============================================================================
// CONSTRUCTEUR
// ============================================================================
Plane::Plane(float s) 
    : size(s), VAO(0), VBO(0), EBO(0) {
    
    std::cout << "[PLANE] Génération du plan..." << std::endl;
    std::cout << "  Taille: " << s << "x" << s << std::endl;
    
    generate_plane();
    
    std::cout << "  ✓ Plan créé (" << vertices.size() << " vertices, " 
              << indices.size() / 3 << " triangles)" << std::endl << std::endl;
}

// ============================================================================
// DESTRUCTEUR
// ============================================================================
Plane::~Plane() {
    if (EBO != 0) glDeleteBuffers(1, &EBO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
}

// ============================================================================
// GENERATE_PLANE : Créer la géométrie
// ============================================================================
void Plane::generate_plane() {
    // ========================================================================
    // ÉTAPE 1 : Créer les 4 coins du rectangle
    // ========================================================================
    //
    // Le plan est dans le plan XZ (horizontal), centré à l'origine
    //
    //       -X          +X
    //        │          │
    //   p0 ─┼──────────┼─ p1     ← +Z (avant)
    //        │          │
    //        │  CENTRE  │
    //        │    (0,0) │
    //   p2 ─┼──────────┼─ p3     ← -Z (arrière)
    //        │          │
    //
    
    float half = size / 2.0f;  // Demi-taille
    
    // Coin haut-gauche (p0)
    PlaneVertex p0;
    p0.position = glm::vec3(-half, 0.0f, half);
    p0.color = glm::vec3(0.5f, 0.5f, 0.5f);  // Gris par défaut
    p0.normal = glm::vec3(0.0f, 1.0f, 0.0f);  // Normal = vers le haut (Y+)
    
    // Coin haut-droit (p1)
    PlaneVertex p1;
    p1.position = glm::vec3(half, 0.0f, half);
    p1.color = glm::vec3(0.5f, 0.5f, 0.5f);
    p1.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    
    // Coin bas-gauche (p2)
    PlaneVertex p2;
    p2.position = glm::vec3(-half, 0.0f, -half);
    p2.color = glm::vec3(0.5f, 0.5f, 0.5f);
    p2.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    
    // Coin bas-droit (p3)
    PlaneVertex p3;
    p3.position = glm::vec3(half, 0.0f, -half);
    p3.color = glm::vec3(0.5f, 0.5f, 0.5f);
    p3.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    
    // Ajouter les vertices
    vertices.push_back(p0);
    vertices.push_back(p1);
    vertices.push_back(p2);
    vertices.push_back(p3);
    
    // ========================================================================
    // ÉTAPE 2 : Créer les indices (2 triangles)
    // ========================================================================
    //
    // Triangle 1 : p0 → p2 → p1 (sens anti-horaire vu de dessus)
    // Triangle 2 : p1 → p2 → p3
    //
    
    // Premier triangle
    indices.push_back(0);  // p0
    indices.push_back(2);  // p2
    indices.push_back(1);  // p1
    
    // Deuxième triangle
    indices.push_back(1);  // p1
    indices.push_back(2);  // p2
    indices.push_back(3);  // p3
    
    // ========================================================================
    // ÉTAPE 3 : Créer le VAO et VBO (comme pour la sphère)
    // ========================================================================
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // VBO pour les vertices
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PlaneVertex), 
                 vertices.data(), GL_STATIC_DRAW);
    
    // EBO pour les indices
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), 
                 indices.data(), GL_STATIC_DRAW);
    
    // ========================================================================
    // ÉTAPE 4 : Configurer les attributs
    // ========================================================================
    
    // Attribut 0 : Position (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PlaneVertex), 
                          (void*)offsetof(PlaneVertex, position));
    glEnableVertexAttribArray(0);
    
    // Attribut 1 : Couleur (3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PlaneVertex), 
                          (void*)offsetof(PlaneVertex, color));
    glEnableVertexAttribArray(1);
    
    // Attribut 2 : Normal (3 floats)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(PlaneVertex), 
                          (void*)offsetof(PlaneVertex, normal));
    glEnableVertexAttribArray(2);
    
    // Débind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// ============================================================================
// DRAW : Dessiner le plan
// ============================================================================
void Plane::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
