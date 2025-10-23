// ============================================================================
// IMPLÉMENTATION DE SCENE
// ============================================================================

#include "editor/scene.hpp"
#include "preview/shader_manager.hpp"
#include "preview/sphere.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// ============================================================================
// CONSTRUCTEUR
// ============================================================================

Scene::Scene()
    : selected_index(-1)  // Aucun objet sélectionné
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

void Scene::render_all(ShaderManager& shader, Sphere& sphere_mesh) {
    shader.use();
    
    for (SceneObject* obj : objects) {
        if (obj->type == ObjectType::SPHERE) {
            // 1. Envoyer la matrice model (position + échelle)
            glm::mat4 model = obj->get_model_matrix();
            shader.set_uniform_matrix4fv("model", glm::value_ptr(model));
            
            // 2. Envoyer la couleur
            shader.set_uniform_3f("objectColor", obj->color.r, obj->color.g, obj->color.b);
            
            // 3. Dessiner la sphère
            sphere_mesh.draw();
        }
        
        // TODO : EXERCICE POUR TOI !
        // Ajouter le rendu des PLANS ici
        // if (obj->type == ObjectType::PLANE) {
        //     // ...
        // }
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
