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

    float get_radius() const { return radius; }
};

// ============================================================================
// FONCTION D'INTERSECTION RAYON/SPHÈRE (pour ray picking)
// ============================================================================
bool ray_sphere_intersection(
    glm::vec3 ray_origin,
    glm::vec3 ray_dir,
    glm::vec3 sphere_center,
    float sphere_radius,
    float& t_hit
);
