#include <iostream>
#include <vector>
#include <memory>
#include <fstream>   
#include <limits>    
#include <algorithm> 
#include <random>

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

// ==================== END USER INTERFACE ====================

int main() {

    // Display menu
    display_menu();
    
    // Get user input
    int image_width = get_positive_input("Image width (pixels)", 800);
    int image_height = get_positive_input("Image height (pixels)", 400);
    int samples_per_pixel = get_positive_input("Samples per pixel (quality)", 200);
    int max_depth = get_positive_input("Max ray depth (bounces)", 50);
    
    std::cout << "\n✓ Configuration accepted\n";
    std::cout << "  Resolution: " << image_width << "x" << image_height << "\n";
    std::cout << "  Samples: " << samples_per_pixel << " | Max depth: " << max_depth << "\n";
    std::cout << "\nStarting render...\n\n";
    
    const double aspect_ratio = static_cast<double>(image_width) / image_height;

    // Materials setup
    auto mat_ground = std::make_shared<diffuse>(vec3(128, 128, 128));     
    auto mat_glass = std::make_shared<dielectric>(1.5, vec3(255,255,255)); // Glass sphere
    auto mat_left = std::make_shared<diffuse>(vec3(255, 0, 0));            
    auto mat_right = std::make_shared<metal>(vec3(200, 150, 50), 0.1);    
    
    // Materials for spheres behind glass
    auto mat_behind_red = std::make_shared<diffuse>(vec3(255, 50, 50));        // Red diffuse
    auto mat_behind_metal = std::make_shared<metal>(vec3(180, 180, 255), 0.0); // Shiny blue metal
    auto mat_behind_rough = std::make_shared<metal>(vec3(100, 200, 100), 0.3); // Rough green metal

    // Scene objects
    std::vector<std::shared_ptr<hittable>> scene_objects;
    scene_objects.push_back(std::make_shared<plane>(vec3(0, -1, 0), vec3(0, 1, 0), mat_ground)); 
    
    // Front spheres
    scene_objects.push_back(std::make_shared<sphere>(vec3(0, 0, -3), 1.0, mat_glass));      // Glass in center
    scene_objects.push_back(std::make_shared<sphere>(vec3(-2.5, 0, -3), 1.0, mat_left));    // Red left
    scene_objects.push_back(std::make_shared<sphere>(vec3(2.5, 0, -3), 1.0, mat_right));    // Metal right
    
    // Three spheres BEHIND the glass (visible through refraction!)
    scene_objects.push_back(std::make_shared<sphere>(vec3(-0.6, 0.3, -5.5), 0.5, mat_behind_red));    // Red diffuse
    scene_objects.push_back(std::make_shared<sphere>(vec3(0, -0.2, -5.5), 0.5, mat_behind_metal));   // Shiny metal
    scene_objects.push_back(std::make_shared<sphere>(vec3(0.6, 0.3, -5.5), 0.5, mat_behind_rough));   // Rough metal
    
    // Camera setup
    vec3 lookfrom(0, 0, 0);
    vec3 lookat(0, 0, -1);
    vec3 vup(0, 1, 0);
    double vfov = 90.0;
    Camera camera(lookfrom, lookat, vup, vfov, aspect_ratio);

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
                vec3 pixel_color = ray_color(camera.origin, ray_direction, scene_objects, max_depth);

                total_color = total_color + pixel_color;
            }

            // Average color across all samples
            vec3 avg_color = total_color / samples_per_pixel;

            // Normalize to [0, 1] for gamma correction
            avg_color.x = avg_color.x / 255.0;
            avg_color.y = avg_color.y / 255.0;
            avg_color.z = avg_color.z / 255.0;

            // Gamma correction (gamma = 2.0)
            avg_color.x = std::sqrt(avg_color.x);
            avg_color.y = std::sqrt(avg_color.y);
            avg_color.z = std::sqrt(avg_color.z);

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
