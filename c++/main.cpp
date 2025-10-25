#include <iostream>
#include <vector>
#include <memory>
#include <fstream>   
#include <limits>    
#include <algorithm> 
#include <random>
#include <cmath>
#include <filesystem>
#include "../external/nlohmann/json.hpp"


using json = nlohmann::json;

#include "core/vec3.hpp"
#include "core/camera.hpp"
#include "geometry/hittable.hpp"
#include "core/ray_color.hpp"
#include "geometry/sphere.hpp"
#include "geometry/plane.hpp"
#include "materials/material.hpp"
#include "materials/diffuse.hpp"
#include "materials/metal.hpp"
#include "materials/dielectric.hpp"
#include "materials/emissive.hpp"
#include "materials/mirror.hpp"

// Global random number generator for all materials
std::random_device rd;
std::mt19937 generator(rd());
std::uniform_real_distribution<double> distribution(0.0, 1.0);

// ==================== USER INPUT INTERFACE ====================

void display_menu() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════╗\n";
    std::cout << "║       Ray Tracer Configuration       ║\n";
    std::cout << "╚══════════════════════════════════════╝\n\n";
}

int get_positive_input(const std::string& prompt, int default_value) {
    std::string input;
    std::cout << prompt << " [default: " << default_value << "]: ";
    std::getline(std::cin, input);
    
    if (input.empty()) {
        return default_value;
    }
    
    try {
        int value = std::stoi(input);
        if (value > 0) {
            return value;
        } else {
            std::cout << "  ⚠ Value must be positive, using default: " << default_value << "\n";
            return default_value;
        }
    } catch (...) {
        std::cout << "  ⚠ Invalid input, using default: " << default_value << "\n";
        return default_value;
    }
}

void show_progress_bar(int current, int total) {
    double percent = static_cast<double>(current) / total * 100.0;
    int blocks = static_cast<int>(percent / 2.0);
    
    std::string bar(blocks, '#');
    std::string empty(50 - blocks, '-');
    
    std::cout << "\r[" << bar << empty << "] " 
              << static_cast<int>(percent) << "% (" 
              << current << "/" << total << ")" << std::flush;
}

vec3 aces_tonemap(const vec3& color) {
    // ACES filmic tone mapping curve
    // Référence: Narkowicz 2015 "ACES Filmic Tone Mapping Curve"
    const double a = 2.51;
    const double b = 0.03;
    const double c = 2.43;
    const double d = 0.59;
    const double e = 0.14;
    
    vec3 x = color;
    vec3 numerator = x * (x * a + b);      // Fix: pas de parenthèses en trop
    vec3 denominator = x * (x * c + d) + e;
    
    return vec3(
        numerator.x / denominator.x,
        numerator.y / denominator.y,
        numerator.z / denominator.z
    );
}
// ==================== END USER INTERFACE ====================

int main() {

    // Display menu
    display_menu();
    
    std::string scene_file = "src/data/save/demo_scene.json";
    std::ifstream ifs(scene_file);

    if (!ifs.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier " << scene_file << std::endl;
        std::cerr << "Veuillez créer une scène avec l'éditeur et sauvegarder avant de lancer le raytracer.\n";
        return 1;
    }
    json scene_data;
    try {
        
        ifs >> scene_data;
    } catch (json::parse_error& e) {
        std::cerr << "Erreur de parsing JSON: " << e.what() << std::endl;
        ifs.close(); 
        return 1; 
    }

    ifs.close();

    // Load scene data into the rendering engine
    int image_width = scene_data["render"]["image_width"];
    int image_height = scene_data["render"]["image_height"];
    int samples_per_pixel = scene_data["render"]["samples_per_pixel"];
    int max_depth = scene_data["render"]["max_depth"];
    int num_threads = scene_data["render"]["num_threads"];
    double gamma = scene_data["render"]["gamma"];
    
    // Ambient light control (default 1.0 if not in JSON)
    double ambient_light = 1.0;
    if (scene_data["render"].contains("ambient_light")) {
        ambient_light = scene_data["render"]["ambient_light"];
    }

    vec3 lookfrom(
        scene_data["camera"]["origin"]["x"],
        scene_data["camera"]["origin"]["y"],
        scene_data["camera"]["origin"]["z"]
    );
    vec3 lookat(
        scene_data["camera"]["look_at"]["x"],
        scene_data["camera"]["look_at"]["y"],
        scene_data["camera"]["look_at"]["z"]
    );
    vec3 vup(
        scene_data["camera"]["up"]["x"],
        scene_data["camera"]["up"]["y"],
        scene_data["camera"]["up"]["z"]
    );
    double vfov = scene_data["camera"]["fov"];
    double aspect_ratio = scene_data["camera"]["aspect_ratio"];

    Camera camera(lookfrom, lookat, vup, vfov, aspect_ratio);
    
    std::vector<std::shared_ptr<hittable>> scene_objects;
    for (auto& obj : scene_data["objects"]) {
        std::string type = obj["type"];
        vec3 center(
            obj["position"]["x"],
            obj["position"]["y"],
            obj["position"]["z"]
        );
        
        // Couleur de l'objet (toujours dans obj["color"])
        vec3 color(
            obj["color"]["r"],
            obj["color"]["g"],
            obj["color"]["b"]
        );
        
        std::shared_ptr<material> mat;
        std::string mat_type = obj["material"];
        
        if (mat_type == "Diffus") {
            mat = std::make_shared<diffuse>(color);
        } else if (mat_type == "Métal") {
            double fuzz = obj.contains("roughness") ? double(obj["roughness"]) : 0.0;
            mat = std::make_shared<metal>(color, fuzz);
        } else if (mat_type == "Verre") {
            double ir = obj.contains("refraction_index") ? double(obj["refraction_index"]) : 1.5;
            mat = std::make_shared<dielectric>(ir, color);
        } else if (mat_type == "Néon" || mat_type == "Neon" || mat_type == "Emissive") {
            double strength = obj.contains("emission_strength") ? double(obj["emission_strength"]) : 5.0;
            mat = std::make_shared<emissive>(color, strength);
        } else if (mat_type == "Miroir" || mat_type == "Mirror") {
            mat = std::make_shared<mirror>(color);
        } else {
            // Matériau par défaut si inconnu
            mat = std::make_shared<diffuse>(color);
        }

        if (type == "Sphère") {
            double radius = obj["size"];
            scene_objects.push_back(std::make_shared<sphere>(center, radius, mat));
        } else if (type == "Plan") {
            vec3 normal(0.0, 1.0, 0.0); // Default normal pointing up
            scene_objects.push_back(std::make_shared<plane>(center, normal, mat));
        }
    }
    

    // Output file
    std::ofstream file("image.ppm");
    file << "P3\n" << image_width << " " << image_height << "\n255\n";

    // Render loop
    for (int y = image_height - 1; y >= 0; --y) {
        
        // Show progress bar
        show_progress_bar(image_height - y, image_height);

        for (int x = 0; x < image_width; ++x) {
            vec3 total_color(0, 0, 0);

            // Anti-aliasing: sample multiple rays per pixel
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (double(x) + distribution(generator)) / (image_width - 1);
                double v = (double(y) + distribution(generator)) / (image_height - 1);

                vec3 ray_direction = camera.get_ray_direction(u, v);
                vec3 pixel_color = ray_color(camera.origin, ray_direction, scene_objects, max_depth, ambient_light);

                total_color = total_color + pixel_color;
            }

            // Average color across all samples
            vec3 avg_color = total_color / samples_per_pixel;
            
            // ACES Tone Mapping (HDR → LDR avec préservation des détails)
            avg_color = aces_tonemap(avg_color);

            // Apply gamma correction APRÈS tone mapping
            avg_color.x = std::pow(avg_color.x, 1.0 / gamma);
            avg_color.y = std::pow(avg_color.y, 1.0 / gamma);
            avg_color.z = std::pow(avg_color.z, 1.0 / gamma);

            // Convert to [0, 255] with clamping
            int r = static_cast<int>(256.0 * std::clamp(avg_color.x, 0.0, 0.999));
            int g = static_cast<int>(256.0 * std::clamp(avg_color.y, 0.0, 0.999));
            int b = static_cast<int>(256.0 * std::clamp(avg_color.z, 0.0, 0.999));
            
            file << r << " " << g << " " << b << "\n";
        }
    }
    
    std::cout << "\n\n✓ Render complete! Image saved as 'image.ppm'\n";
    file.close();
    return 0;
}
