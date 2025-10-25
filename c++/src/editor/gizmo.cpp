// ============================================================================
// GIZMO - IMPLÉMENTATION
// ============================================================================

#include "editor/gizmo.hpp"
#include "preview/shader_manager.hpp"
#include "preview/camera_gl.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <iostream>
#include <cmath>
#include <vector>

// ============================================================================
// CONSTRUCTEUR
// ============================================================================
Gizmo::Gizmo(float length, float radius)
    : position(0.0f, 0.0f, 0.0f)
    , axis_length(length)
    , axis_radius(radius)
    , selected_axis(-1)
{
}

// ============================================================================
// RAY PICKING : Détecter quel axe est cliqué
// ============================================================================
int Gizmo::pick_axis(glm::vec3 ray_origin, glm::vec3 ray_direction) {
    float closest_distance = 1e6;  // Augmenté pour permettre une sélection plus large
    int closest_axis = -1;
    float closest_t = 1e6;
    
    // Les 3 axes
    glm::vec3 axes[3] = {
        glm::vec3(1, 0, 0),  // X
        glm::vec3(0, 1, 0),  // Y
        glm::vec3(0, 0, 1)   // Z
    };
    
    std::cout << "[GIZMO] Test pick_axis à position: (" 
              << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    
    // Tester l'intersection avec chaque axe
    for (int i = 0; i < 3; i++) {
        glm::vec3 axis_start = position;
        glm::vec3 axis_end = position + axes[i] * axis_length;
        
        float distance, t_hit;
        if (ray_line_distance(ray_origin, ray_direction, axis_start, axis_end, distance, t_hit)) {
            std::cout << "  Axe " << i << ": distance=" << distance 
                      << ", t_hit=" << t_hit 
                      << ", radius+tolerance=" << (axis_radius + 0.5f) << std::endl;
            
            if (distance < closest_distance && t_hit > 0 && t_hit < closest_t) {
                closest_distance = distance;
                closest_axis = i;
                closest_t = t_hit;
            }
        }
    }
    
    if (closest_axis != -1) {
        std::cout << "  → AXE SÉLECTIONNÉ: " << closest_axis 
                  << " (distance=" << closest_distance << ")" << std::endl;
    } else {
        std::cout << "  → Aucun axe sélectionné" << std::endl;
    }
    
    selected_axis = closest_axis;
    return closest_axis;
}

// ============================================================================
// RAY-LINE DISTANCE : Calcule la distance entre un rayon et une ligne
// ============================================================================
bool Gizmo::ray_line_distance(
    glm::vec3 ray_origin,
    glm::vec3 ray_direction,
    glm::vec3 line_start,
    glm::vec3 line_end,
    float& distance,
    float& t_hit
) const {
    // Méthode simplifiée : échantillonner des points le long de l'axe
    // et trouver le plus proche du rayon
    
    glm::vec3 axis_dir = line_end - line_start;
    float axis_len = glm::length(axis_dir);
    axis_dir = glm::normalize(axis_dir);
    
    float min_distance = 1e6;
    float best_t = -1;
    
    // Échantillonner 20 points le long de l'axe
    const int num_samples = 20;
    for (int i = 0; i <= num_samples; i++) {
        float alpha = (float)i / (float)num_samples;
        glm::vec3 point_on_axis = line_start + axis_dir * (axis_len * alpha);
        
        // Projeter ce point sur le rayon
        glm::vec3 to_point = point_on_axis - ray_origin;
        float t = glm::dot(to_point, ray_direction);
        
        if (t > 0) {  // Point devant la caméra
            glm::vec3 closest_on_ray = ray_origin + ray_direction * t;
            float dist = glm::length(point_on_axis - closest_on_ray);
            
            if (dist < min_distance) {
                min_distance = dist;
                best_t = t;
            }
        }
    }
    
    distance = min_distance;
    t_hit = best_t;
    
    // Tolérance généreuse pour faciliter la sélection
    return min_distance < 0.3f && best_t > 0;
}

// ============================================================================
// DÉPLACEMENT : Calculer le delta de position
// ============================================================================
glm::vec3 Gizmo::calculate_delta(int axis, glm::vec3 ray_origin, glm::vec3 ray_direction) {
    if (axis < 0 || axis > 2) return glm::vec3(0);
    
    glm::vec3 axes[3] = {
        glm::vec3(1, 0, 0),  // X
        glm::vec3(0, 1, 0),  // Y
        glm::vec3(0, 0, 1)   // Z
    };
    
    glm::vec3 axis_dir = axes[axis];
    
    // Créer un plan qui contient l'axe ET la caméra
    // Normal du plan = perpendiculaire à l'axe ET au rayon de la caméra
    glm::vec3 cam_to_gizmo = position - ray_origin;
    glm::vec3 plane_normal = glm::normalize(glm::cross(axis_dir, cam_to_gizmo));
    
    // Si l'axe est aligné avec la caméra, utiliser un plan de secours
    if (glm::length(plane_normal) < 0.01f) {
        // Choisir un vecteur perpendiculaire arbitraire
        glm::vec3 perp = (std::abs(axis_dir.y) < 0.9f) ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
        plane_normal = glm::normalize(glm::cross(axis_dir, perp));
    }
    
    // Intersection rayon-plan
    float denom = glm::dot(ray_direction, plane_normal);
    if (std::abs(denom) < 1e-6) {
        // Rayon parallèle au plan
        return glm::vec3(0);
    }
    
    float t = glm::dot(position - ray_origin, plane_normal) / denom;
    if (t < 0) {
        // Intersection derrière la caméra
        return glm::vec3(0);
    }
    
    // Point d'intersection sur le plan
    glm::vec3 intersection_point = ray_origin + ray_direction * t;
    
    // Projeter ce point sur l'axe
    glm::vec3 to_intersection = intersection_point - position;
    float projection = glm::dot(to_intersection, axis_dir);
    
    // Le delta est le mouvement le long de l'axe
    return axis_dir * projection;
}

// ============================================================================
// RENDU : Dessiner les axes avec des lignes simples
// ============================================================================
void Gizmo::draw(ShaderManager& shader) {
    shader.use();
    
    // Les 3 axes avec leurs couleurs
    glm::vec3 axis_colors[3] = {
        glm::vec3(1, 0, 0),  // Rouge (X)
        glm::vec3(0, 1, 0),  // Vert (Y)
        glm::vec3(0, 0, 1)   // Bleu (Z)
    };
    
    glm::vec3 axis_directions[3] = {
        glm::vec3(1, 0, 0),  // X
        glm::vec3(0, 1, 0),  // Y
        glm::vec3(0, 0, 1)   // Z
    };
    
    // Créer VAO + VBO pour les lignes
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Créer les vertices des 3 axes
    std::vector<float> vertices;
    for (int i = 0; i < 3; i++) {
        // Début de l'axe
        glm::vec3 start = position;
        vertices.push_back(start.x);
        vertices.push_back(start.y);
        vertices.push_back(start.z);
        
        // Fin de l'axe
        glm::vec3 end = position + axis_directions[i] * axis_length;
        vertices.push_back(end.x);
        vertices.push_back(end.y);
        vertices.push_back(end.z);
    }
    
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    
    // Layout
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Désactiver la profondeur pour que les axes soient toujours visibles
    glDisable(GL_DEPTH_TEST);
    
    // Rendre les lignes plus épaisses
    glLineWidth(5.0f);
    
    // Dessiner les 3 axes
    for (int i = 0; i < 3; i++) {
        glm::vec3 color = axis_colors[i];
        if (selected_axis == i) {
            color = color * 1.5f;  // Plus lumineux si sélectionné
        }
        shader.set_uniform_3f("objectColor", color.x, color.y, color.z);
        
        // Ajouter une matrice identité pour le modèle
        shader.set_uniform_matrix4fv("model", glm::value_ptr(glm::mat4(1.0f)));
        
        // Dessiner les 2 vertices de cet axe (une ligne)
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, i * 2, 2);
    }
    
    // Réactiver l'épaisseur par défaut
    glLineWidth(1.0f);
    
    // Réactiver la profondeur
    glEnable(GL_DEPTH_TEST);
    
    // Nettoyer
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

