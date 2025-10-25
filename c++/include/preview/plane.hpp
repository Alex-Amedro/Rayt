#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

// Structure d'un vertex (identique à Sphere)
struct PlaneVertex {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
};

class Plane {
private:
    float size;  // Taille du rectangle (size x size)
    
    GLuint VAO, VBO, EBO;
    std::vector<PlaneVertex> vertices;
    std::vector<GLuint> indices;
    
    void generate_plane();
    
public:
    
    Plane(float s = 10.0f);
    ~Plane();

    void draw();
    
    float get_size() const { return size; }
};

// ============================================================================
// FONCTION D'INTERSECTION RAYON/PLAN (pour ray picking)
// ============================================================================
bool ray_plane_intersection(
    glm::vec3 ray_origin,
    glm::vec3 ray_dir,
    glm::vec3 plane_center,
    glm::vec3 plane_normal,
    float plane_size,
    float& t_hit
);


