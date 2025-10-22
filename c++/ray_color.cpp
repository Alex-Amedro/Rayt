#include "ray_color.hpp"
#include "vec3.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include <vector>
#include <memory>
#include <limits>

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
            vec3 next_origin = hit_point + (hit_normal * hittable::epsilon);
            vec3 color_from_scatter = ray_color(next_origin, scattered_direction, scene, depth - 1);
            
            // Apply material attenuation (albedo in [0, 255] range)
            double r = (attenuation.x / 255.0) * color_from_scatter.x;
            double g = (attenuation.y / 255.0) * color_from_scatter.y;
            double b = (attenuation.z / 255.0) * color_from_scatter.z;
            
            return vec3(r, g, b);
        } else {
            // Material absorbed the ray
            return vec3(0, 0, 0);
        }
    }
    
    // No hit - return sky gradient
    vec3 unit_direction = ray_direction.normalize();
    double t_sky = 0.5 * (unit_direction.y + 1.0);

    vec3 color_white(255.0, 255.0, 255.0);
    vec3 color_blue(135.0, 206.0, 235.0);

    double r = (1.0 - t_sky) * color_white.x + t_sky * color_blue.x;
    double g = (1.0 - t_sky) * color_white.y + t_sky * color_blue.y;
    double b = (1.0 - t_sky) * color_white.z + t_sky * color_blue.z;

    return vec3(r, g, b);
}
