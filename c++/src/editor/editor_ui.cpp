#include "editor/editor_ui.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include "../../external/nlohmann/json.hpp"
#include <fstream>
#include <filesystem>


using json = nlohmann::json;

// ============================================================================
// CONSTRUCTEUR
// ============================================================================

EditorUI::EditorUI(Scene& s, CameraGL& cam)
    : scene(s)
    , gizmo(nullptr)
    , camera(cam)
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
    // IMPORTANT : install_callbacks = true pour que ImGui gère les callbacks automatiquement
    // (incluant le clavier pour les zones de texte)
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

// ============================================================================
// NETTOYAGE IMGUI
// ============================================================================

void EditorUI::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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
        static char buffer[128] = "";
        strncpy(buffer, obj->name.c_str(), sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';  // Garantir la terminaison
        if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
            obj->name = buffer;
        }
        ImGui::Text("Type : %s", obj->get_type_name().c_str());
        
        ImGui::Spacing();
        
        // ====================================================================
        // POSITION (exemple fait par moi)
        // ====================================================================
        ImGui::Text("Position");
        bool pos_changed = false;
        pos_changed |= ImGui::SliderFloat("X", &obj->position.x, -10.0f, 10.0f);
        pos_changed |= ImGui::SliderFloat("Y", &obj->position.y, -10.0f, 10.0f);
        pos_changed |= ImGui::SliderFloat("Z", &obj->position.z, -10.0f, 10.0f);
        
        // Mettre à jour le gizmo si la position a changé
        if (pos_changed && gizmo != nullptr) {
            gizmo->set_position(obj->position);
        }
        
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
        
        // ====================================================================
        // GRILLE (afficher la grille au sol)
        // ====================================================================
        bool grid = scene.is_grid_enabled();
        if (ImGui::Checkbox("Afficher la grille au sol", &grid)) {
            scene.set_grid(grid);
        }
        
        ImGui::Spacing();
        
        // Bouton Supprimer (seulement si objet sélectionné)
        if (scene.get_selected() != nullptr) {
            if (ImGui::Button("Supprimer l'objet", ImVec2(-1, 0))) {
                scene.remove_selected();
            }
        }
        
        ImGui::Spacing();
        
        // ====================================================================
        // IMAGE SETTINGS (Paramètres pour le raytracer)
        // ====================================================================
        if (ImGui::CollapsingHeader("Image Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Text("Résolution");
            
            // ComboBox pour les presets de résolution
            const char* resolution_presets[] = { "480p (854x480)", "720p (1280x720)", "1080p (1920x1080)", "2K (2560x1440)", "4K (3840x2160)" };
            static int current_preset = 2; // Par défaut 1080p
            
            if (ImGui::Combo("Preset", &current_preset, resolution_presets, IM_ARRAYSIZE(resolution_presets))) {
                // Appliquer le preset sélectionné
                switch (current_preset) {
                    case 0: image_width = 854;  image_height = 480;  break;  // 480p
                    case 1: image_width = 1280; image_height = 720;  break;  // 720p
                    case 2: image_width = 1920; image_height = 1080; break;  // 1080p
                    case 3: image_width = 2560; image_height = 1440; break;  // 2K
                    case 4: image_width = 3840; image_height = 2160; break;  // 4K
                }
            }
            
            // Ou entrée manuelle
            ImGui::InputInt("Width", &image_width);
            ImGui::InputInt("Height", &image_height);
            
            ImGui::Spacing();
            ImGui::Text("Qualité de rendu");
            ImGui::InputInt("Samples per Pixel", &samples_per_pixel);
            ImGui::InputInt("Max Depth", &max_depth);
            
            ImGui::Spacing();
            ImGui::Text("Profondeur de champ");
            ImGui::SliderFloat("Aperture", &aperture, 0.0f, 2.0f);
            ImGui::SliderFloat("Focus Distance", &focus_distance, 1.0f, 100.0f);
            
            ImGui::Spacing();
            ImGui::Text("Environnement");
            ImGui::SliderFloat("Sun Intensity", &sun_intensity, 0.0f, 2.0f);
            
            ImGui::Spacing();
            ImGui::Text("Post-Processing");
            ImGui::SliderFloat("Gamma", &gamma, 1.0f, 3.0f);
            
            ImGui::Spacing();
            ImGui::Text("Performance");
            ImGui::InputInt("Threads", &num_threads);
        }
        
        ImGui::Spacing();

        if (ImGui::Button("Save", ImVec2(-1, 0))) {
            try {
                std::cout << "[SAVE] Début de la sauvegarde de la scène...\n";
                json scene_data;
                
                // Camera data - utiliser les vrais paramètres de la caméra
                glm::vec3 cam_pos = camera.get_position();
                glm::vec3 cam_target = camera.get_target();
                
                scene_data["camera"]["origin"]["x"] = cam_pos.x;
                scene_data["camera"]["origin"]["y"] = cam_pos.y;
                scene_data["camera"]["origin"]["z"] = cam_pos.z;
                scene_data["camera"]["look_at"]["x"] = cam_target.x;
                scene_data["camera"]["look_at"]["y"] = cam_target.y;
                scene_data["camera"]["look_at"]["z"] = cam_target.z;
                scene_data["camera"]["up"]["x"] = 0.0f;
                scene_data["camera"]["up"]["y"] = 1.0f;
                scene_data["camera"]["up"]["z"] = 0.0f;
                scene_data["camera"]["fov"] = camera.get_fov();
                scene_data["camera"]["aspect_ratio"] = static_cast<double>(image_width) / static_cast<double>(image_height);
                scene_data["camera"]["aperture"] = aperture;
                scene_data["camera"]["focus_distance"] = focus_distance;

                // Render settings
                scene_data["render"]["image_width"] = image_width;
                scene_data["render"]["image_height"] = image_height;
                scene_data["render"]["samples_per_pixel"] = samples_per_pixel;
                scene_data["render"]["max_depth"] = max_depth;
                scene_data["render"]["sun_intensity"] = sun_intensity;
                scene_data["render"]["gamma"] = gamma;
                scene_data["render"]["num_threads"] = num_threads;

                //scene_data["environment"]["background"] = background_color;
                //scene_data["environment"]["hour"] = hour;



                scene_data["objects"] = json::array();
                for (int i = 0; i < scene.get_object_count(); i++) {
                    SceneObject* obj = scene.get_object(i);
                    json obj_data;
                    obj_data["type"] = obj->get_type_name();
                    obj_data["name"] = obj->name;
                    obj_data["position"]["x"] = obj->position.x;
                    obj_data["position"]["y"] = obj->position.y;
                    obj_data["position"]["z"] = obj->position.z;
                    obj_data["color"]["r"] = obj->color.r;
                    obj_data["color"]["g"] = obj->color.g;
                    obj_data["color"]["b"] = obj->color.b;
                    obj_data["size"] = obj->size;
                    obj_data["material"] = obj->get_material_name();
                    if (obj->material == MaterialType::METAL) {
                        obj_data["roughness"] = obj->roughness;
                    } else if (obj->material == MaterialType::DIELECTRIC) {
                        obj_data["refraction_index"] = obj->refraction_index;
                    }
                    scene_data["objects"].push_back(obj_data);
                }

                std::string save_path = "../c++/src/data/save/save1.json";
                std::ofstream ofs(save_path);
                ofs << scene_data.dump(4);
                ofs.close();
               
            } catch (const std::exception& e) {
                std::cerr << "[SAVE] ✗ Erreur lors de la sauvegarde : " << e.what() << std::endl;
            }
        }
        
        // Bouton Load
        if (ImGui::Button("Load", ImVec2(-1, 0))) {
            std::string load_path = "../c++/src/data/save/save1.json";
            scene.load_from_json(load_path);
            
            // Load render settings from JSON
            try {
                std::ifstream ifs(load_path);
                json scene_data = json::parse(ifs);
                
                if (scene_data.contains("render")) {
                    if (scene_data["render"].contains("image_width"))
                        image_width = scene_data["render"]["image_width"];
                    if (scene_data["render"].contains("image_height"))
                        image_height = scene_data["render"]["image_height"];
                    if (scene_data["render"].contains("samples_per_pixel"))
                        samples_per_pixel = scene_data["render"]["samples_per_pixel"];
                    if (scene_data["render"].contains("max_depth"))
                        max_depth = scene_data["render"]["max_depth"];
                    if (scene_data["render"].contains("sun_intensity"))
                        sun_intensity = scene_data["render"]["sun_intensity"];
                    if (scene_data["render"].contains("gamma"))
                        gamma = scene_data["render"]["gamma"];
                    if (scene_data["render"].contains("num_threads"))
                        num_threads = scene_data["render"]["num_threads"];
                }
                
                if (scene_data["camera"].contains("aperture"))
                    aperture = scene_data["camera"]["aperture"];
                if (scene_data["camera"].contains("focus_distance"))
                    focus_distance = scene_data["camera"]["focus_distance"];
            } catch (const std::exception& e) {
                std::cerr << "[LOAD] Erreur lors du chargement des render settings : " << e.what() << std::endl;
            }
        }
        
        ImGui::Spacing();
        
        // Bouton Render
        if (ImGui::Button("Lancer le Ray Tracer", ImVec2(-1, 0))) {
            try {
                json scene_data;
                
                // Camera data - utiliser les vrais paramètres de la caméra
                glm::vec3 cam_pos = camera.get_position();
                glm::vec3 cam_target = camera.get_target();
                
                scene_data["camera"]["origin"]["x"] = cam_pos.x;
                scene_data["camera"]["origin"]["y"] = cam_pos.y;
                scene_data["camera"]["origin"]["z"] = cam_pos.z;
                scene_data["camera"]["look_at"]["x"] = cam_target.x;
                scene_data["camera"]["look_at"]["y"] = cam_target.y;
                scene_data["camera"]["look_at"]["z"] = cam_target.z;
                scene_data["camera"]["up"]["x"] = 0.0f;
                scene_data["camera"]["up"]["y"] = 1.0f;
                scene_data["camera"]["up"]["z"] = 0.0f;
                scene_data["camera"]["fov"] = camera.get_fov();
                scene_data["camera"]["aspect_ratio"] = static_cast<double>(image_width) / static_cast<double>(image_height);
                scene_data["camera"]["aperture"] = aperture;
                scene_data["camera"]["focus_distance"] = focus_distance;

                // Render settings
                scene_data["render"]["image_width"] = image_width;
                scene_data["render"]["image_height"] = image_height;
                scene_data["render"]["samples_per_pixel"] = samples_per_pixel;
                scene_data["render"]["max_depth"] = max_depth;
                scene_data["render"]["sun_intensity"] = sun_intensity;
                scene_data["render"]["gamma"] = gamma;
                scene_data["render"]["num_threads"] = num_threads;

                //scene_data["environment"]["background"] = background_color;
                //scene_data["environment"]["hour"] = hour;

                scene_data["objects"] = json::array();
                for (int i = 0; i < scene.get_object_count(); i++) {
                    SceneObject* obj = scene.get_object(i);
                    json obj_data;
                    obj_data["type"] = obj->get_type_name();
                    obj_data["name"] = obj->name;
                    obj_data["position"]["x"] = obj->position.x;
                    obj_data["position"]["y"] = obj->position.y;
                    obj_data["position"]["z"] = obj->position.z;
                    obj_data["color"]["r"] = obj->color.r;
                    obj_data["color"]["g"] = obj->color.g;
                    obj_data["color"]["b"] = obj->color.b;
                    obj_data["size"] = obj->size;
                    obj_data["material"] = obj->get_material_name();
                    if (obj->material == MaterialType::METAL) {
                        obj_data["roughness"] = obj->roughness;
                    } else if (obj->material == MaterialType::DIELECTRIC) {
                        obj_data["refraction_index"] = obj->refraction_index;
                    }
                    scene_data["objects"].push_back(obj_data);
                }

                std::string save_path = "../c++/src/data/save/save1.json";
                std::ofstream ofs(save_path);
                ofs << scene_data.dump(4);
                ofs.close();

                //lancer le render
                int ret = system("make run");
                if (ret != 0) {
                    std::cerr << "Erreur lors du lancement du raytracer (code: " << ret << ")\n";
                }

            } catch (const std::exception& e) {
                std::cerr << "Erreur lors de la sauvegarde de la scène : " << e.what() << std::endl;
        }   
        }
    }
}
