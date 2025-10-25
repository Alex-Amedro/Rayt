#pragma once
#include "core/vec3.hpp"
#include "geometry/hittable.hpp"
#include <vector>
#include <memory>

vec3 ray_color(
    const vec3& ray_origin,
    const vec3& ray_direction,
    const std::vector<std::shared_ptr<hittable>>& scene,
    int depth,
    double ambient_light = 1.0  // Default: full ambient lighting
);

bool refract(const vec3& v_in_normalized, const vec3& n, double ior_ratio, vec3& refracted_direction);

double reflectance(double cosine, double ref_idx_ratio);
