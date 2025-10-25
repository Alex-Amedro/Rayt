#include "core/ray_color.hpp"
#include "core/vec3.hpp"
#include "geometry/hittable.hpp"
#include "materials/material.hpp"
#include <vector>
#include <memory>
#include <limits>
#include <cmath>
#include <algorithm>

bool refract(const vec3& v_in_normalized, const vec3& n, double ior_ratio, vec3& refracted_direction) {

    double cos_theta = std::min((vec3(0.0, 0.0, 0.0)-v_in_normalized).dot(n), 1.0);
    
    vec3 r_out_perp = (v_in_normalized + (n * cos_theta)) * ior_ratio;
    
    double r_out_parallel_sq = 1.0 - r_out_perp.length_squared();
    if (r_out_parallel_sq > 0.0) {
        
        vec3 r_out_parallel = n * -std::sqrt(r_out_parallel_sq);
        refracted_direction = r_out_perp + r_out_parallel;
        return true; 

    } else {
        return false;
    }
}


double reflectance(double cosine, double ref_idx_ratio) {
    double r0 = (1.0 - ref_idx_ratio) / (1.0 + ref_idx_ratio);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * std::pow((1.0 - cosine), 5);
}

vec3 ray_color(
    const vec3& ray_origin,
    const vec3& ray_direction,
    const std::vector<std::shared_ptr<hittable>>& scene,
    int depth
) {
    // Recursion depth limit
    if (depth <= 0) {
        return vec3(0, 0, 0);
    }

    // Find closest object in scene
    double t_min = std::numeric_limits<double>::infinity();
    std::shared_ptr<hittable> closest_object = nullptr;
    for (const auto& obj : scene) {
        double t_temp = obj->hit(ray_origin, ray_direction);
        if (t_temp > hittable::epsilon && t_temp < t_min) {
            t_min = t_temp;
            closest_object = obj;
        }
    }

    // Ray hit an object - calculate material interaction
    if (closest_object) {
        vec3 hit_point = ray_origin + (ray_direction * t_min);
        vec3 hit_normal = closest_object->get_normal(hit_point);

        vec3 attenuation;
        vec3 scattered_direction;

        // Ask material how the ray scatters
        if (closest_object->mat->scatter(
                ray_direction, hit_point, hit_normal,
                attenuation, scattered_direction
            ))
        {
            // Material scattered the ray - trace recursively
            // For refraction, we need to offset in the opposite direction
            // Check if scattered ray is going into or out of the surface
            bool same_hemisphere = scattered_direction.dot(hit_normal) > 0;
            vec3 offset = same_hemisphere ? (hit_normal * hittable::epsilon) 
                                          : (hit_normal * -hittable::epsilon);
            vec3 next_origin = hit_point + offset;
            
            vec3 color_from_scatter = ray_color(next_origin, scattered_direction, scene, depth - 1);
            
            // Apply material attenuation (albedo already in [0, 1] range)
            double r = attenuation.x * color_from_scatter.x;
            double g = attenuation.y * color_from_scatter.y;
            double b = attenuation.z * color_from_scatter.z;
            
            return vec3(r, g, b);
        } else {
            // Material absorbed the ray
            return vec3(0, 0, 0);
        }
    }
    
    // No hit - return sky gradient
    vec3 unit_direction = ray_direction.normalize();
    double t_sky = 0.5 * (unit_direction.y + 1.0);

    // Sky colors in [0, 1] range
    vec3 color_white(1.0, 1.0, 1.0);
    vec3 color_blue(0.5, 0.7, 1.0);

    double r = (1.0 - t_sky) * color_white.x + t_sky * color_blue.x;
    double g = (1.0 - t_sky) * color_white.y + t_sky * color_blue.y;
    double b = (1.0 - t_sky) * color_white.z + t_sky * color_blue.z;

    return vec3(r, g, b);
}
