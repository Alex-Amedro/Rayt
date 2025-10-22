#pragma once
#include "core/vec3.hpp"
#include "geometry/hittable.hpp"
#include <vector>
#include <memory>

vec3 ray_color(
    const vec3& ray_origin,
    const vec3& ray_direction,
    const std::vector<std::shared_ptr<hittable>>& scene,
    int depth 
);
