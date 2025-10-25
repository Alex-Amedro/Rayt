// ============================================================================
// IMPLÉMENTATION DE SCENE
// ============================================================================

#include "editor/scene.hpp"
#include "preview/shader_manager.hpp"
#include "preview/sphere.hpp"
#include "preview/plane.hpp"
#include "preview/grid.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <limits>

// ============================================================================
// CONSTRUCTEUR
// ============================================================================

Scene::Scene()
    : selected_index(-1)  // Aucun objet sélectionné
    , wireframe_enabled(true)  // Wireframe activé par défaut
    , grid_enabled(true)  // Grille activée par défaut
{
}

// ============================================================================
// DESTRUCTEUR (libère la mémoire de tous les objets)
// ============================================================================

Scene::~Scene() {
    clear();
}

// ============================================================================
// AJOUTER UN OBJET
// ============================================================================

void Scene::add_object(SceneObject* obj) {
    if (obj == nullptr) {
        std::cerr << "Erreur : tentative d'ajout d'un objet nullptr\n";
        return;
    }
    objects.push_back(obj);
    std::cout << "Objet ajouté : " << obj->name << " (" << obj->get_type_name() << ")\n";
}

// ============================================================================
// SUPPRIMER UN OBJET PAR INDEX
// ============================================================================

void Scene::remove_object(int index) {
    if (index < 0 || index >= (int)objects.size()) {
        std::cerr << "Erreur : index invalide (" << index << ")\n";
        return;
    }
    
    std::cout << "Suppression de : " << objects[index]->name << "\n";
    
    // Libérer la mémoire
    delete objects[index];
    
    // Supprimer de la liste
    objects.erase(objects.begin() + index);
    
    // Ajuster la sélection
    if (selected_index == index) {
        selected_index = -1;  // L'objet sélectionné a été supprimé
    } else if (selected_index > index) {
        selected_index--;  // Décaler l'index
    }
}

// ============================================================================
// SUPPRIMER L'OBJET SÉLECTIONNÉ
// ============================================================================

void Scene::remove_selected() {
    if (selected_index != -1) {
        remove_object(selected_index);
    }
}

// ============================================================================
// OBTENIR UN OBJET PAR INDEX
// ============================================================================

SceneObject* Scene::get_object(int index) {
    if (index < 0 || index >= (int)objects.size()) {
        return nullptr;
    }
    return objects[index];
}

// ============================================================================
// SÉLECTIONNER UN OBJET
// ============================================================================

void Scene::select_object(int index) {
    // Désélectionner l'ancien
    if (selected_index != -1 && selected_index < (int)objects.size()) {
        objects[selected_index]->selected = false;
    }
    
    // Sélectionner le nouveau
    selected_index = index;
    if (index != -1 && index < (int)objects.size()) {
        objects[index]->selected = true;
        std::cout << "Objet sélectionné : " << objects[index]->name << "\n";
    }
}

// ============================================================================
// DÉSÉLECTIONNER TOUT
// ============================================================================

void Scene::clear_selection() {
    select_object(-1);
}

// ============================================================================
// OBTENIR L'OBJET SÉLECTIONNÉ
// ============================================================================

SceneObject* Scene::get_selected() {
    if (selected_index == -1 || selected_index >= (int)objects.size()) {
        return nullptr;
    }
    return objects[selected_index];
}

// ============================================================================
// RENDU DE TOUS LES OBJETS
// ============================================================================

void Scene::render_all(ShaderManager& shader, Sphere& sphere_mesh, Plane& plane_mesh, Grid& grid_mesh) {
    shader.use();
    if (grid_enabled) {
        // Dessiner la grille
        glm::mat4 model = glm::mat4(1.0f);
        shader.set_uniform_matrix4fv("model", glm::value_ptr(model));
        shader.set_uniform_3f("objectColor", 0.7f, 0.7f, 0.7f);  // Gris clair
        grid_mesh.draw();
    }

    for (SceneObject* obj : objects) {
        // 1. Envoyer la matrice model (position + échelle)
        glm::mat4 model = obj->get_model_matrix();
        shader.set_uniform_matrix4fv("model", glm::value_ptr(model));
        
        // ================================================================
        // PASS 1 : Dessiner l'objet REMPLI (avec sa couleur)
        // ================================================================
        shader.set_uniform_3f("objectColor", obj->color.r, obj->color.g, obj->color.b);
        
        // Mode rempli (par défaut)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        if (obj->type == ObjectType::SPHERE) {
            sphere_mesh.draw();
        }
        else if (obj->type == ObjectType::PLANE) {
            plane_mesh.draw();
        }
        
        // ================================================================
        // PASS 2 : Dessiner les CONTOURS (wireframe en noir)
        // ================================================================
        if (wireframe_enabled) {
            if (!obj->selected) {   
                shader.set_uniform_3f("objectColor", 0.0f, 0.0f, 0.0f);  // Noir
            } else {
                shader.set_uniform_3f("objectColor", 1.0f - obj->color.r, 1.0f - obj->color.g, 1.0f - obj->color.b);  // Blanc
            }
            
            // Mode wireframe (lignes seulement)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glLineWidth(1.5f);  // Épaisseur des lignes
            
            if (obj->type == ObjectType::SPHERE) {
                sphere_mesh.draw();
            }
            else if (obj->type == ObjectType::PLANE) {
                plane_mesh.draw();
            }
            
            // Remettre en mode rempli pour le prochain objet
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}

// ============================================================================
// SUPPRIMER TOUS LES OBJETS
// ============================================================================

void Scene::clear() {
    for (SceneObject* obj : objects) {
        delete obj;
    }
    objects.clear();
    selected_index = -1;
    std::cout << "Scène vidée\n";
}

// ============================================================================
// CRÉER UNE SCÈNE PAR DÉFAUT (pour tester)
// ============================================================================

void Scene::create_default_scene() {
    clear();

    // Sphère rouge
    SceneObject* sphere1 = new SceneObject(ObjectType::SPHERE, "Sphère Rouge");
    sphere1->position = glm::vec3(-2.0f, 0.0f, -5.0f);
    sphere1->color = glm::vec3(1.0f, 0.0f, 0.0f);  // Rouge
    sphere1->size = 1.0f;
    add_object(sphere1);
    
    // Sphère verte
    SceneObject* sphere2 = new SceneObject(ObjectType::SPHERE, "Sphère Verte");
    sphere2->position = glm::vec3(2.0f, 0.0f, -5.0f);
    sphere2->color = glm::vec3(0.0f, 1.0f, 0.0f);  // Vert
    sphere2->size = 1.5f;
    add_object(sphere2);
    
    // Sphère bleue
    SceneObject* sphere3 = new SceneObject(ObjectType::SPHERE, "Sphère Bleue");
    sphere3->position = glm::vec3(0.0f, 2.0f, -5.0f);
    sphere3->color = glm::vec3(0.0f, 0.0f, 1.0f);  // Bleu
    sphere3->size = 0.8f;
    add_object(sphere3);
    
    std::cout << "Scène par défaut créée (3 sphères)\n";
}

int Scene::pick_object(glm::vec3 ray_origin, glm::vec3 ray_direction) {
    int closest_index = -1;
    float closest_distance = std::numeric_limits<float>::max();
    
    std::cout << "[RAY PICKING] Rayon origine: (" << ray_origin.x << ", " << ray_origin.y << ", " << ray_origin.z << ")\n";
    std::cout << "[RAY PICKING] Direction: (" << ray_direction.x << ", " << ray_direction.y << ", " << ray_direction.z << ")\n";
    
    for (int i = 0; i < (int)objects.size(); i++){
        SceneObject* obj = objects[i];
        float t_hit;
        bool hit = false;
        
        if (obj -> type == ObjectType::SPHERE){
            // La sphère a un rayon de 1.0, et obj->size est un multiplicateur
            float effective_radius = 1.0f * obj->size;
            hit = ray_sphere_intersection(ray_origin, ray_direction, obj->position, effective_radius, t_hit);
            if (hit) {
                std::cout << "[RAY PICKING] Sphère " << i << " (" << obj->name << ") touchée à t=" << t_hit << "\n";
            }
        }else if( obj -> type == ObjectType::PLANE){
            hit = ray_plane_intersection(ray_origin, ray_direction, obj->position, glm::vec3(0,1,0), obj->size, t_hit);
            if (hit) {
                std::cout << "[RAY PICKING] Plan " << i << " (" << obj->name << ") touché à t=" << t_hit << "\n";
            }
        }
        if (hit && t_hit < closest_distance){
            closest_distance = t_hit;
            closest_index = i;
        }
    }
    
    std::cout << "[RAY PICKING] Objet sélectionné : " << closest_index << "\n";
    return closest_index;
}
