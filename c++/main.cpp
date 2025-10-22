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

int main() {

    // Image configuration
    const int image_width = 800;   
    const int image_height = 400;  
    const double aspect_ratio = static_cast<double>(image_width) / image_height;
    const int samples_per_pixel = 200;  
    const int max_depth = 50;

    // Materials setup
    auto mat_ground = std::make_shared<diffuse>(vec3(128, 128, 128));     
    auto mat_center = std::make_shared<metal>(vec3(255, 255, 255), 0.0);  
    auto mat_left = std::make_shared<diffuse>(vec3(255, 0, 0));            
    auto mat_right = std::make_shared<metal>(vec3(200, 150, 50), 0.1);    

    // Scene objects
    std::vector<std::shared_ptr<hittable>> scene_objects;
    scene_objects.push_back(std::make_shared<plane>(vec3(0, -1, 0), vec3(0, 1, 0), mat_ground)); 
    scene_objects.push_back(std::make_shared<sphere>(vec3(0, 0, -3), 1.0, mat_center));           
    scene_objects.push_back(std::make_shared<sphere>(vec3(-2.5, 0, -3), 1.0, mat_left));         
    scene_objects.push_back(std::make_shared<sphere>(vec3(2.5, 0, -3), 1.0, mat_right));         
    
    // Camera setup
    vec3 lookfrom(0, 0, 0);
    vec3 lookat(0, 0, -1);
    vec3 vup(0, 1, 0);
    double vfov = 90.0;
    Camera camera(lookfrom, lookat, vup, vfov, aspect_ratio);

    // Random number generator for anti-aliasing
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    // Output file
    std::ofstream file("image.ppm");
    file << "P3\n" << image_width << " " << image_height << "\n255\n";

    // Render loop
    for (int y = image_height - 1; y >= 0; --y) {

        // Progress indicator
        double percent_complete = static_cast<double>(image_height - y) / image_height * 100.0;
        int progress_blocks = static_cast<int>(percent_complete / 2.0); 

        std::string filled_part(progress_blocks, '#'); // Partie remplie
        std::string empty_part(50 - progress_blocks, '-'); // Partie vide

        std::cout << "\rProgression: [" << filled_part << empty_part << "] "
          << static_cast<int>(percent_complete) << "% " << std::flush;

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
    
    std::cout << "\nDone.\n";
    file.close();
    return 0;
}
