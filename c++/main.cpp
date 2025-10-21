#include <iostream>
#include <vector>
#include <memory>
#include <fstream>   
#include <limits>    
#include <algorithm> 
#include <random>
#include <list>

#include "hittable.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "vec3.hpp"
#include "diffuse.hpp"
#include "material.hpp"
#include "ray_color.hpp"


int main() {

    // image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 5;
    const int max_depth = 50;

    //color
    auto mat_sol = std::make_shared<diffuse>(vec3(150, 150, 150));
    auto mat_sphere1 = std::make_shared<diffuse>(vec3(255, 0, 0));
    auto mat_sphere2 = std::make_shared<diffuse>(vec3(0, 255, 0));

    // scene
    std::vector<std::shared_ptr<hittable>> scene_objects;
    scene_objects.push_back(std::make_shared<sphere>(vec3(0, 0, -1), 0.5, mat_sphere1));
    scene_objects.push_back(std::make_shared<sphere>(vec3(-1, 0, -1.5), 0.5, mat_sphere2));
    scene_objects.push_back(std::make_shared<plane>(vec3(0, -0.5, -1), vec3(0, 1, 0), mat_sol));
    
    //camera
    vec3 camera_origin(0, 0, 0);
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0; 

    vec3 horizontal(viewport_width, 0, 0);
    vec3 vertical(0, viewport_height, 0);
    vec3 lower_left_corner = camera_origin - (horizontal / 2) - (vertical / 2) - vec3(0, 0, focal_length);

    //anti-aliasing Random
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    //file output
    std::ofstream file("image.ppm");
    file << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int y = image_height - 1; y >= 0; --y) {

        std::cout << "\rLignes restantes: " << y << " " << std::flush; 

        for (int x = 0; x < image_width; ++x) {

            vec3 total_color(0, 0, 0);

            for (int s = 0; s < samples_per_pixel; ++s) {

                double u = (double(x) + distribution(generator)) / (image_width - 1);
                double v = (double(y) + distribution(generator)) / (image_height - 1);

                vec3 ray_direction = lower_left_corner + (horizontal * u) + (vertical * v) - camera_origin;
                vec3 pixel_color = ray_color(camera_origin, ray_direction, scene_objects, max_depth);

                total_color = total_color + pixel_color;
            }

            // Fais la moyenne (double / int -> double)
            vec3 avg_color = total_color / samples_per_pixel;

            // N'OUBLIE PAS la correction gamma (cruciale !)
            avg_color.x = std::sqrt(avg_color.x / 255.0) * 255.0;
            avg_color.y = std::sqrt(avg_color.y / 255.0) * 255.0;
            avg_color.z = std::sqrt(avg_color.z / 255.0) * 255.0;

            // Convertis en 'int' UNE SEULE FOIS, à la fin
            int r = static_cast<int>(avg_color.x);
            int g = static_cast<int>(avg_color.y);
            int b = static_cast<int>(avg_color.z);

            file << r << " " << g << " " << b << "\n";
        }
    }
    
    std::cout << "\nTermine.\n";
    file.close();
    return 0;
}
