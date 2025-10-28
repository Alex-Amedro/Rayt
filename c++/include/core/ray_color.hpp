#pragma once
#include "vec3.hpp"
#include "light.hpp"
#include "geometry/hittable.hpp"
#include <vector>
#include <memory>
#include <optional>

vec3 ray_color(
    const vec3& ray_origin,
    const vec3& ray_direction,
    const std::vector<std::shared_ptr<hittable>>& scene,
    const std::vector<PointLight>& lights,
    const std::optional<DirectionalLight>& sun,
    int depth,
    double ambient_light
);
