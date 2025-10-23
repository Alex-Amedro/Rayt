// ============================================================================
// LEÇON 5 : SPHERE - IMPLÉMENTATION
// ============================================================================

#include "preview/sphere.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <cmath>

const float PI = 3.14159265359f;

// ============================================================================
// CONSTRUCTEUR
// ============================================================================
Sphere::Sphere(float r, int st, int se) 
    : radius(r), stacks(st), sectors(se), VAO(0), VBO(0), EBO(0) {
    
    std::cout << "[LEÇON 5] Génération de la sphère..." << std::endl;
    std::cout << "  Rayon: " << r << std::endl;
    std::cout << "  Stacks: " << st << ", Sectors: " << se << std::endl;
    
    generate_sphere();
    
    std::cout << "  ✓ Sphère créée (" << vertices.size() << " vertices, " 
              << indices.size() / 3 << " triangles)" << std::endl << std::endl;
}

// ============================================================================
// DESTRUCTEUR
// ============================================================================
Sphere::~Sphere() {
    if (EBO != 0) glDeleteBuffers(1, &EBO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
}

// ============================================================================
// GENERATE_SPHERE : Créer la géométrie
// ============================================================================
void Sphere::generate_sphere() {
    // ========================================================================
    // ÉTAPE 1 : Générer les vertices
    // ========================================================================
    // Formule pour un point sur une sphère en coordonnées sphériques :
    // x = radius * sin(latitude) * cos(longitude)
    // y = radius * cos(latitude)
    // z = radius * sin(latitude) * sin(longitude)
    
    for (int i = 0; i <= stacks; ++i) {
        // latitude va de 0 (haut) à π (bas)
        float latitude = PI * i / stacks;
        float sin_lat = sin(latitude);
        float cos_lat = cos(latitude);
        
        for (int j = 0; j <= sectors; ++j) {
            // longitude va de 0 à 2π
            float longitude = 2.0f * PI * j / sectors;
            float sin_lon = sin(longitude);
            float cos_lon = cos(longitude);
            
            // Position sur la sphère
            glm::vec3 pos(
                radius * sin_lat * cos_lon,
                radius * cos_lat,
                radius * sin_lat * sin_lon
            );
            
            // Couleur : gradient basé sur la latitude
            // (haut = rouge, bas = bleu)
            glm::vec3 color(
                0.2f + 0.8f * (1.0f - i / (float)stacks),  // R : moins rouge en bas
                0.3f,                                        // G : constant
                0.2f + 0.8f * i / (float)stacks             // B : plus bleu en bas
            );
            
            // Normal : pour une sphère, la normale est la direction du centre
            glm::vec3 normal = glm::normalize(pos);
            
            Vertex v;
            v.position = pos;
            v.color = color;
            v.normal = normal;
            
            vertices.push_back(v);
        }
    }
    
    // ========================================================================
    // ÉTAPE 2 : Générer les indices (triangles)
    // ========================================================================
    // On relie les points en triangles
    // Chaque "bande" crée 2 triangles par segment
    
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < sectors; ++j) {
            // Index des 4 coins du "quad"
            GLuint first = i * (sectors + 1) + j;
            GLuint second = first + sectors + 1;
            
            // Premier triangle du quad
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);
            
            // Deuxième triangle du quad
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
    
    // ========================================================================
    // ÉTAPE 3 : Créer le VAO et VBO
    // ========================================================================
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    // VBO pour les vertices
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), 
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                          (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    
    // Attribut 1 : Couleur (3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                          (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    
    // Attribut 2 : Normal (pour futur usage)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
                          (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    
    // Débind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// ============================================================================
// DRAW : Dessiner la sphère
// ============================================================================
void Sphere::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
