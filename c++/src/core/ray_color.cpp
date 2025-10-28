#include "core/ray_color.hpp"
#include "core/vec3.hpp"
#include "core/light.hpp"
#include "geometry/hittable.hpp"
#include "materials/material.hpp"
#include <vector>
#include <memory>
#include <limits>
#include <cmath>
#include <algorithm>
#include <optional>

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
    const std::vector<PointLight>& lights,
    const std::optional<DirectionalLight>& sun,
    int depth,
    double ambient_light
) {
    if (depth <= 0) {
        return vec3(0, 0, 0);
    }

    double t_min = std::numeric_limits<double>::infinity();
    std::shared_ptr<hittable> closest_object = nullptr;
    for (const auto& obj : scene) {
        double t_temp = obj->hit(ray_origin, ray_direction);
        if (t_temp > hittable::epsilon && t_temp < t_min) {
            t_min = t_temp;
            closest_object = obj;
        }
    }

    if (closest_object) {
        vec3 hit_point = ray_origin + (ray_direction * t_min);
        vec3 hit_normal = closest_object->get_normal(hit_point);
        vec3 emitted = closest_object->mat->emitted();

        vec3 attenuation;
        vec3 scattered_direction;

        if (closest_object->mat->scatter(ray_direction, hit_point, hit_normal, attenuation, scattered_direction)) {
            
            // ========== DIRECT LIGHTING FROM POINT LIGHTS ==========
            vec3 direct_light(0, 0, 0);
            for (const auto& light : lights) {
                vec3 to_light = light.direction_from(hit_point);
                double light_distance = light.distance_from(hit_point);
                
                // Shadow ray test
                vec3 shadow_origin = hit_point + (hit_normal * hittable::epsilon);
                bool in_shadow = false;
                
                for (const auto& obj : scene) {
                    double t_shadow = obj->hit(shadow_origin, to_light);
                    if (t_shadow > hittable::epsilon && t_shadow < light_distance) {
                        in_shadow = true;
                        break;
                    }
                }
                
                if (!in_shadow) {
                    double n_dot_l = std::max(0.0, hit_normal.dot(to_light));
                    vec3 light_contribution = light.get_illumination(hit_point);
                    direct_light = direct_light + (attenuation * light_contribution * n_dot_l);
                }
            }
            
            // ========== DIRECT LIGHTING FROM DIRECTIONAL LIGHT (SUN) ==========
            if (sun.has_value()) {
                vec3 to_sun = sun->direction_from(hit_point);
                double sun_distance = sun->distance_from(hit_point);
                
                // Shadow ray test for sun
                vec3 shadow_origin = hit_point + (hit_normal * hittable::epsilon);
                bool in_shadow = false;
                
                for (const auto& obj : scene) {
                    double t_shadow = obj->hit(shadow_origin, to_sun);
                    if (t_shadow > hittable::epsilon && t_shadow < sun_distance) {
                        in_shadow = true;
                        break;
                    }
                }
                
                if (!in_shadow) {
                    double n_dot_l = std::max(0.0, hit_normal.dot(to_sun));
                    vec3 sun_contribution = sun->get_illumination(hit_point);
                    direct_light = direct_light + (attenuation * sun_contribution * n_dot_l);
                }
            }
            
            // Recursive path tracing
            bool same_hemisphere = scattered_direction.dot(hit_normal) > 0;
            vec3 offset = same_hemisphere ? (hit_normal * hittable::epsilon) : (hit_normal * -hittable::epsilon);
            vec3 next_origin = hit_point + offset;
            
            vec3 color_from_scatter = ray_color(next_origin, scattered_direction, scene, lights, sun, depth - 1, ambient_light);
            
            double r = attenuation.x * color_from_scatter.x;
            double g = attenuation.y * color_from_scatter.y;
            double b = attenuation.z * color_from_scatter.z;
            
            return vec3(r, g, b) + emitted + direct_light;
        } else {
            return emitted;
        }
    }
    
    vec3 unit_direction = ray_direction.normalize();
    double t_sky = 0.5 * (unit_direction.y + 1.0);
    vec3 color_white(1.0, 1.0, 1.0);
    vec3 color_blue(0.5, 0.7, 1.0);
    double r = (1.0 - t_sky) * color_white.x + t_sky * color_blue.x;
    double g = (1.0 - t_sky) * color_white.y + t_sky * color_blue.y;
    double b = (1.0 - t_sky) * color_white.z + t_sky * color_blue.z;
    return vec3(r, g, b) * ambient_light;
}
