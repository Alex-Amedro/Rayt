#pragma once
#include "core/vec3.hpp"

// Camera class handling viewport setup and ray generation
class Camera {
public:
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;

    // Simple camera constructor
    Camera(
        vec3 lookfrom,
        vec3 lookat,
        vec3 vup,
        double vfov,        // Vertical field of view in degrees
        double aspect_ratio
    ) {
        // Calculate viewport dimensions from field of view
        double theta = vfov * 3.14159265359 / 180.0;
        double h = tan(theta / 2.0);
        double viewport_height = 2.0 * h;
        double viewport_width = aspect_ratio * viewport_height;

        // Camera coordinate system
        vec3 w = (lookfrom - lookat).normalize();  // Forward (camera looks at -w)
        vec3 u = vup.cross(w).normalize();          // Right
        vec3 v = w.cross(u);                        // Up

        origin = lookfrom;
        horizontal = u * viewport_width;
        vertical = v * viewport_height;
        lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - w;
    }

    // Generate a ray for given screen coordinates (u, v in [0, 1])
    vec3 get_ray_direction(double u, double v) const {
        return lower_left_corner + horizontal * u + vertical * v - origin;
    }
};
