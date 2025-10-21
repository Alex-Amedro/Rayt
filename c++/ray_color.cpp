#include "ray_color.hpp"
#include "vec3.hpp"
#include "hittable.hpp"
#include <vector>
#include <memory>
#include <limits>
#include "material.hpp"


const int MAX_DEPTH = 50; 
const double EPSILON = 0.0001;

vec3 ray_color(
    const vec3& ray_origin, 
    const vec3& ray_direction,
    const std::vector<std::shared_ptr<hittable>>& scene,
    int depth 
) {
    if (depth <= 0) {
        return vec3(0, 0, 0); 
    }

    
    double t_min = std::numeric_limits<double>::infinity();
    std::shared_ptr<hittable> closest_object = nullptr;
    for (const auto& obj : scene) {
        double t_temp = obj->hit(ray_origin, ray_direction);
        if (t_temp > EPSILON && t_temp < t_min) {
            t_min = t_temp;
            closest_object = obj; 
        }
    }
    if (closest_object) {
        
        vec3 hit_point = ray_origin + (ray_direction * t_min);
        vec3 hit_normal = closest_object->get_normal(hit_point);
        
        vec3 attenuation;         
        vec3 scattered_direction; 

        if (closest_object->mat->scatter(ray_direction, hit_point, hit_normal, attenuation, scattered_direction
            )) {
            return attenuation * ray_color(hit_point + (hit_normal*EPSILON), scattered_direction, scene, depth - 1);
        } else {
            return vec3(0, 0, 0);
        }
    }

    // 3. Si on n'a rien touché, renvoie le ciel (INCHANGÉ)
    return vec3(135, 206, 235);
}
