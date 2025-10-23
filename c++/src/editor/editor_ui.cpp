#include "editor/editor_ui.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>

// ============================================================================
// CONSTRUCTEUR
// ============================================================================

EditorUI::EditorUI(Scene& s)
    : scene(s)
    , show_add_menu(true)
    , show_properties(true)
{
}

// ============================================================================
// INITIALISATION IMGUI
// ============================================================================

void EditorUI::init(GLFWwindow* window) {
    // 1. Créer le contexte ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;  // Éviter le warning "unused variable"
    
    // 2. Style sombre
    ImGui::StyleColorsDark();
    
    // 3. Initialiser les backends GLFW + OpenGL3
    // IMPORTANT : install_callbacks = false car on gère manuellement les callbacks
    ImGui_ImplGlfw_InitForOpenGL(window, false);
    ImGui_ImplOpenGL3_Init("#version 460");
    
    std::cout << "ImGui initialisé\n";
}

// ============================================================================
// NETTOYAGE IMGUI
// ============================================================================

void EditorUI::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    std::cout << "ImGui nettoyé\n";
}

// ============================================================================
// RENDU DE L'INTERFACE (chaque frame)
// ============================================================================

void EditorUI::render() {
    // 1. Nouvelle frame ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    // 2. Fenêtre principale (menu latéral)
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.8f, 0));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x * 0.2f, ImGui::GetIO().DisplaySize.y));
    
    ImGui::Begin("Éditeur de Scène", nullptr, 
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    
    // 3. Menu "Ajouter un objet"
    render_add_menu();
    
    ImGui::Separator();
    
    // 4. Liste des objets
    render_object_list();
    
    ImGui::Separator();
    
    // 5. Propriétés de l'objet sélectionné
    render_properties();
    
    ImGui::Separator();
    
    // 6. Actions (Supprimer, Render)
    render_actions();
    
    ImGui::End();
    
    // 7. Rendu final
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// ============================================================================
// MENU "AJOUTER UN OBJET"
// ============================================================================

void EditorUI::render_add_menu() {
    if (ImGui::CollapsingHeader("Ajouter un objet", ImGuiTreeNodeFlags_DefaultOpen)) {
        
        // ====================================================================
        // BOUTON : AJOUTER UNE SPHÈRE (exemple fait par moi)
        // ====================================================================
        if (ImGui::Button("Ajouter une Sphère", ImVec2(-1, 0))) {
            // Créer une nouvelle sphère
            SceneObject* sphere = new SceneObject(ObjectType::SPHERE, "Sphère");
            sphere->position = glm::vec3(0.0f, 0.0f, -5.0f);
            sphere->color = glm::vec3(1.0f, 1.0f, 1.0f);  // Blanc
            sphere->size = 1.0f;
            
            // Ajouter à la scène
            scene.add_object(sphere);
            
            // Sélectionner automatiquement
            scene.select_object(scene.get_object_count() - 1);
        }
        
        // ====================================================================
        // TODO : EXERCICE POUR TOI !
        // ====================================================================
        // Ajouter un bouton "Ajouter un Plan" en suivant le même modèle
        // Copie le code ci-dessus et adapte-le pour :
        // - ObjectType::PLANE
        // - Nom : "Plan"
        // - Position : glm::vec3(0, -1, 0)  // Un plan horizontal sous les objets
        // - Couleur : glm::vec3(0.5f, 0.5f, 0.5f)  // Gris
        // - Size : 10.0f  // Un grand plan
        //
        // if (ImGui::Button("Ajouter un Plan", ImVec2(-1, 0))) {
        //     // ... TON CODE ICI ...
        // }
        if (ImGui::Button("Ajouter une Plan", ImVec2(-1, 0))) {

            SceneObject* plane = new SceneObject(ObjectType::PLANE, "PLANE");
            plane->position = glm::vec3(0.0f, -1.0f, -5.0f);
            plane->color = glm::vec3(0.5f, 0.5f, 0.5f);  // Gris
            plane->size = 10.0f;  // Un grand plan

            scene.add_object(plane);
            
            scene.select_object(scene.get_object_count() - 1);
        }
    }
}

// ============================================================================
// LISTE DES OBJETS
// ============================================================================

void EditorUI::render_object_list() {
    if (ImGui::CollapsingHeader("Objets dans la scène", ImGuiTreeNodeFlags_DefaultOpen)) {
        
        // Afficher le nombre d'objets
        ImGui::Text("Nombre d'objets : %d", scene.get_object_count());
        
        ImGui::Spacing();
        
        // Liste déroulante des objets
        for (int i = 0; i < scene.get_object_count(); i++) {
            SceneObject* obj = scene.get_object(i);
            
            // Nom de l'objet (avec son type)
            std::string label = obj->name + " (" + obj->get_type_name() + ")";
            
            // Bouton sélectionnable (surligné si sélectionné)
            if (ImGui::Selectable(label.c_str(), obj->selected)) {
                scene.select_object(i);
            }
        }
    }
}

// ============================================================================
// PROPRIÉTÉS DE L'OBJET SÉLECTIONNÉ
// ============================================================================

void EditorUI::render_properties() {
    if (ImGui::CollapsingHeader("Propriétés", ImGuiTreeNodeFlags_DefaultOpen)) {
        
        SceneObject* obj = scene.get_selected();
        
        if (obj == nullptr) {
            ImGui::TextDisabled("Aucun objet sélectionné");
            return;
        }
        
        // ====================================================================
        // NOM DE L'OBJET
        // ====================================================================
        ImGui::Text("Objet : %s", obj->name.c_str());
        ImGui::Text("Type : %s", obj->get_type_name().c_str());
        
        ImGui::Spacing();
        
        // ====================================================================
        // POSITION (exemple fait par moi)
        // ====================================================================
        ImGui::Text("Position");
        ImGui::SliderFloat("X", &obj->position.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Y", &obj->position.y, -10.0f, 10.0f);
        ImGui::SliderFloat("Z", &obj->position.z, -10.0f, 10.0f);
        
        ImGui::Spacing();
        
        // ====================================================================
        // COULEUR
        // ====================================================================
        ImGui::Text("Couleur");
        ImGui::ColorEdit3("RGB", &obj->color.r);
        
        ImGui::Spacing();
        
        // ====================================================================
        // TAILLE
        // ====================================================================
        ImGui::Text("Taille");
        if (obj->type == ObjectType::SPHERE) {
            ImGui::SliderFloat("Rayon", &obj->size, 0.1f, 5.0f);
        } else if (obj->type == ObjectType::PLANE) {
            ImGui::SliderFloat("Échelle", &obj->size, 1.0f, 20.0f);
        }
        
        ImGui::Spacing();
        
        // ====================================================================
        // MATÉRIAU
        // ====================================================================
        ImGui::Text("Matériau");
        
        const char* materials[] = { "Diffus", "Métal", "Verre" };
        int current_material = (int)obj->material;
        
        if (ImGui::Combo("Type", &current_material, materials, 3)) {
            obj->material = (MaterialType)current_material;
        }
        
        // Propriétés selon le matériau
        if (obj->material == MaterialType::METAL) {
            ImGui::SliderFloat("Rugosité", &obj->roughness, 0.0f, 1.0f);
        } else if (obj->material == MaterialType::DIELECTRIC) {
            ImGui::SliderFloat("Indice de réfraction", &obj->refraction_index, 1.0f, 2.5f);
        }
    }
}

// ============================================================================
// ACTIONS (Supprimer, Render)
// ============================================================================

void EditorUI::render_actions() {
    if (ImGui::CollapsingHeader("Actions", ImGuiTreeNodeFlags_DefaultOpen)) {
        
        // ====================================================================
        // WIREFRAME (afficher les contours)
        // ====================================================================
        bool wireframe = scene.is_wireframe_enabled();
        if (ImGui::Checkbox("Afficher les contours (wireframe)", &wireframe)) {
            scene.set_wireframe(wireframe);
        }
        
        ImGui::Spacing();
        
        // Bouton Supprimer (seulement si objet sélectionné)
        if (scene.get_selected() != nullptr) {
            if (ImGui::Button("Supprimer l'objet", ImVec2(-1, 0))) {
                scene.remove_selected();
            }
        }
        
        ImGui::Spacing();
        
        // Bouton Render
        if (ImGui::Button("Lancer le Ray Tracer", ImVec2(-1, 0))) {
            std::cout << "TODO : Lancer le ray tracer\n";
            // TODO : Exporter la scène et lancer le ray tracer C++
        }
    }
}
