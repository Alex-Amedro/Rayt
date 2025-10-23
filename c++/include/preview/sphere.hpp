// ============================================================================
// LEÇON 5 : CLASSE SPHERE
// ============================================================================
//
// Qu'est-ce qu'une sphère en OpenGL ?
//
// C'est une géométrie créée avec plein de petits triangles
// arrangés pour former une forme sphérique.
//
// Algorithme utilisé : UV Sphere (le plus courant)
// - On crée des "bandes horizontales" (latitudes)
// - On crée des "bandes verticales" (longitudes)
// - Chaque intersection = un point sur la sphère
// - On relie les points avec des triangles
//
// Paramètres :
// - radius = rayon de la sphère
// - stacks = nombre de bandes horizontales (plus = plus lisse)
// - sectors = nombre de bandes verticales (plus = plus lisse)
//
// Exemple : radius=1, stacks=20, sectors=20
// = 20 * 20 = 400 points ≈ 1600 triangles
//
// ============================================================================

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;  // Normal pour futur usage (lumière)
};

class Sphere {
private:
    // ====================================================================
    // Paramètres
    // ====================================================================
    float radius;
    int stacks;
    int sectors;
    
    // ====================================================================
    // Données GPU
    // ====================================================================
    GLuint VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    
    // ====================================================================
    // Générer la géométrie
    // ====================================================================
    void generate_sphere();

public:
    // ====================================================================
    // CONSTRUCTEUR
    // ====================================================================
    // Crée une sphère avec paramètres donnés
    Sphere(float r = 1.0f, int st = 20, int se = 20);
    
    // ====================================================================
    // DESTRUCTEUR
    // ====================================================================
    // Nettoie la mémoire GPU
    ~Sphere();
    
    // ====================================================================
    // DESSINER LA SPHÈRE
    // ====================================================================
    void draw();
    
    // ====================================================================
    // GETTERS
    // ====================================================================
    int get_vertex_count() const { return vertices.size(); }
    int get_index_count() const { return indices.size(); }
};
