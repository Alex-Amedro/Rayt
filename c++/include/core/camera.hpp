#pragma once
#include "core/vec3.hpp"
#include <random>

// Camera class handling viewport setup and ray generation
class Camera {
public:
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;           // Camera coordinate system
    double lens_radius;     // For depth of field

    // Camera constructor with depth of field support
    Camera(
        vec3 lookfrom,
        vec3 lookat,
        vec3 vup,
        double vfov,        // Vertical field of view in degrees
        double aspect_ratio,
        double aperture = 0.0,      // Lens aperture (0 = no blur)
        double focus_dist = 1.0     // Focus distance
    ) {
        // Calculate viewport dimensions from field of view
        double theta = vfov * 3.14159265359 / 180.0;
        double h = tan(theta / 2.0);
        double viewport_height = 2.0 * h;
        double viewport_width = aspect_ratio * viewport_height;

        // Camera coordinate system
        w = (lookfrom - lookat).normalize();  // Forward (camera looks at -w)
        u = vup.cross(w).normalize();          // Right
        v = w.cross(u);                        // Up

        origin = lookfrom;
        horizontal = u * viewport_width * focus_dist;
        vertical = v * viewport_height * focus_dist;
        lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - w * focus_dist;
        
        lens_radius = aperture / 2.0;
    }

    // Generate a ray for given screen coordinates (u, v in [0, 1])
    // Uses global random generator
    vec3 get_ray_direction(double s, double t, std::mt19937& gen, std::uniform_real_distribution<double>& dist) const {
        vec3 rd(0, 0, 0);
        
        // Depth of field: random point on lens
        if (lens_radius > 0.0) {
            // Random point in unit disk
            double angle = 2.0 * 3.14159265359 * dist(gen);
            double radius = lens_radius * std::sqrt(dist(gen));
            rd = u * (radius * cos(angle)) + v * (radius * sin(angle));
        }
        
        vec3 offset = rd;
        return lower_left_corner + horizontal * s + vertical * t - origin - offset;
    }
    
    // Get ray origin (with lens offset for depth of field)
    vec3 get_ray_origin(std::mt19937& gen, std::uniform_real_distribution<double>& dist) const {
        if (lens_radius > 0.0) {
            double angle = 2.0 * 3.14159265359 * dist(gen);
            double radius = lens_radius * std::sqrt(dist(gen));
            vec3 rd = u * (radius * cos(angle)) + v * (radius * sin(angle));
            return origin + rd;
        }
        return origin;
    }
};
